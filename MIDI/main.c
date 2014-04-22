/*
 *  C2000 Brain MIDI Instrument
 *	File: main.c
 *	Authors: James Patrick Martins Bergin and Victor Lopes Takahashi
 *
 */


/*
 * Includes
 */

#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File
#include "f2802x_common/include/sci.h" // Includes Serial Connectivity
#include "f2802x_common/include/adc.h"
#include "f2802x_common/include/clk.h"
#include "f2802x_common/include/gpio.h"
#include "f2802x_common/include/pie.h"
#include "f2802x_common/include/pll.h"
#include "f2802x_common/include/wdog.h"

/*
 * 	Declarations
 */

void scia_echoback_init();
void scia_xmit(int a);
void scia_fifo_init();
Uint16 adc_conv();

/*
 * 	Handles
 */

ADC_Handle myAdc;
CLK_Handle myClk;
GPIO_Handle myGpio;
PIE_Handle myPie;
SCI_Handle mySci;




Uint16 velocity = 100;
Uint16 note_on = 144;
Uint16 note_off = 128;


Uint16 MIDI_Channel = 0; // Valid: 0-15 (for MIDI Channels 1-16)

/*
 *
 *  MAIN
 *
 */


void main(void) {

	CPU_Handle myCpu;
	PLL_Handle myPll;
	WDOG_Handle myWDog;


	// Initialize all the handles needed for this application
	myAdc = ADC_init((void *) ADC_BASE_ADDR, sizeof(ADC_Obj));
	myClk = CLK_init((void *) CLK_BASE_ADDR, sizeof(CLK_Obj));
	myCpu = CPU_init((void *) NULL, sizeof(CPU_Obj));
	myGpio = GPIO_init((void *) GPIO_BASE_ADDR, sizeof(GPIO_Obj));
	//myFlash = FLASH_init((void *)FLASH_BASE_ADDR, sizeof(FLASH_Obj));
	myPie = PIE_init((void *) PIE_BASE_ADDR, sizeof(PIE_Obj));
	myPll = PLL_init((void *) PLL_BASE_ADDR, sizeof(PLL_Obj));
	mySci = SCI_init((void *) SCIA_BASE_ADDR, sizeof(SCI_Obj));
	myWDog = WDOG_init((void *) WDOG_BASE_ADDR, sizeof(WDOG_Obj));

	// Perform basic system initialization
	WDOG_disable(myWDog);
	CLK_enableAdcClock(myClk);
	(*Device_cal )();

	//Select the internal oscillator 1 as the clock source
	CLK_setOscSrc(myClk, CLK_OscSrc_Internal);

	// Setup the PLL for x12 /2 which will yield 60Mhz = 10Mhz * 12 / 2
	PLL_setup(myPll, PLL_Multiplier_12, PLL_DivideSelect_ClkIn_by_2);

	// Disable the PIE and all interrupts
	PIE_disable(myPie);
	PIE_disableAllInts(myPie);
	CPU_disableGlobalInts(myCpu);
	CPU_clearIntFlags(myCpu);

	// If running from flash copy RAM only functions to RAM
#ifdef _FLASH
	memcpy(&RamfuncsRunStart, &RamfuncsLoadStart, (size_t)&RamfuncsLoadSize);
#endif


	// Initalize GPIO
	GPIO_setPullUp(myGpio, GPIO_Number_28, GPIO_PullUp_Enable);
	GPIO_setPullUp(myGpio, GPIO_Number_29, GPIO_PullUp_Disable);
	GPIO_setQualification(myGpio, GPIO_Number_28, GPIO_Qual_ASync);
	GPIO_setMode(myGpio, GPIO_Number_28, GPIO_28_Mode_SCIRXDA);
	GPIO_setMode(myGpio, GPIO_Number_29, GPIO_29_Mode_SCITXDA);




	// Setup a debug vector table and enable the PIE
	PIE_setDebugIntVectorTable(myPie);
	PIE_enable(myPie);

	scia_echoback_init();  // Initalize SCI for echoback
	scia_fifo_init();      // Initialize the SCI FIFO

	InitAdc();             // Inicializa o ADC
	//InitAdcAio();

	EALLOW;
	AdcRegs.ADCSOC1CTL.bit.CHSEL= 1;     // Seleciona o canal SOC 1 para o ADCINA0
	AdcRegs.ADCSOC1CTL.bit.ACQPS  = 6;   // Seleciona 6 + 1 ciclos de Sample and Hold window
	AdcRegs.ADCSOC0CTL.bit.CHSEL= 0;     // Seleciona o canal SOC 1 para o ADCINA0
	AdcRegs.ADCSOC0CTL.bit.ACQPS  = 6;   // Seleciona 6 + 1 ciclos de Sample and Hold window
	AdcRegs.ADCSOC2CTL.bit.CHSEL= 2;     // Seleciona o canal SOC 1 para o ADCINA0
	AdcRegs.ADCSOC2CTL.bit.ACQPS  = 6;   // Seleciona 6 + 1 ciclos de Sample and Hold window
    AdcRegs.INTSEL1N2.bit.INT1E = 1;     // ADCINT1 ENABLE
	AdcRegs.INTSEL1N2.bit.INT1CONT = 0;  // Nenhuma interrupcao é gerada até que o bit ADCINT1 flag no registrador ADCINTFLG seja limpo
	AdcRegs.ADCCTL1.bit.INTPULSEPOS = 1; //
	AdcRegs.INTSEL1N2.bit.INT1SEL = 2;   // End of Conversion 2 é o trigger da ADCINT1
	//AdcRegs.ADCINTSOCSEL1.bit.SOC0  = 1;
	AdcRegs.ADCINTSOCSEL1.bit.SOC0 = 1;		// ADCInterrupt 1 causes SOC0
	AdcRegs.ADCINTSOCSEL1.bit.SOC1 = 1;     // ADCInterrupt 1 causes SOC1
	AdcRegs.ADCINTSOCSEL1.bit.SOC2 = 1;     // ADCInterrupt 1 causes SOC2
	EDIS;
	DELAY_US(1000L);





	for(;;){

		scia_xmit(note_on);
		scia_xmit(50);
		scia_xmit(velocity);
		//DELAY_US(300000);

		scia_xmit(note_on);
		scia_xmit(55);
		scia_xmit(velocity);
		//DELAY_US(300000);

		scia_xmit(note_on);
		scia_xmit(58);
		scia_xmit(velocity);
		DELAY_US(300000);

		adc_conv();
	}
}

/*
 *
 *
 *  Definitions
 *
 *
 */

void scia_echoback_init() {

	CLK_enableSciaClock(myClk);

	// 1 stop bit,  No loopback
	// No parity,8 char bits,
	// async mode, idle-line protocol
	SCI_disableParity(mySci);
	SCI_setNumStopBits(mySci, SCI_NumStopBits_One);
	SCI_setCharLength(mySci, SCI_CharLength_8_Bits);

	SCI_enableTx(mySci);
	//SCI_enableRx(mySci);
	SCI_enableTxInt(mySci);
	//SCI_enableRxInt(mySci);

	// SCI BRR = LSPCLK/(SCI BAUDx8) - 1
#if (CPU_FRQ_60MHZ)
	SCI_setBaudRate(mySci, SCI_BaudRate_9_6_kBaud);
#elif (CPU_FRQ_50MHZ)
	SCI_setBaudRate(mySci, (SCI_BaudRate_e)162);
#elif (CPU_FRQ_40MHZ)
	SCI_setBaudRate(mySci, (SCI_BaudRate_e)129);
#endif

	SCI_enable(mySci);

	return;
}

// Transmit a character from the SCI
void scia_xmit(int a)
{
//    while (SciaRegs.SCIFFTX.bit.TXFFST != 0) {}
    while(SCI_getTxFifoStatus(mySci) != SCI_FifoStatus_Empty){
    }
//    SciaRegs.SCITXBUF=a;
    SCI_putDataBlocking(mySci, a);

}

// Initalize the SCI FIFO
void scia_fifo_init()
{

    SCI_enableFifoEnh(mySci);
    SCI_resetTxFifo(mySci);
    SCI_clearTxFifoInt(mySci);
    SCI_resetChannels(mySci);
    SCI_setTxFifoIntLevel(mySci, SCI_FifoLevel_Empty);

    //SCI_resetRxFifo(mySci);
    //SCI_clearRxFifoInt(mySci);
    //SCI_setRxFifoIntLevel(mySci, SCI_FifoLevel_4_Words);

    return;
}

Uint16 adc_conv()
{
	Uint16 result;





	 AdcRegs.ADCSOCFRC1.bit.SOC2 = 0x0001;

	 while (AdcRegs.ADCINTFLG.bit.ADCINT1 == 0){}   // Wait for EOC2 generate a ADCINT1 interrupt
	         AdcRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;   //Must clear ADCINT1 flag since INT1CONT = 0
	         result = AdcResult.ADCRESULT0;        // Take the result from ADCRESULT0 Register

	  return result;
}
