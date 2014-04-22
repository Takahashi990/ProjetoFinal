// Inicialização do LCD
// Envio de Comandos

// Specs.: VDD +5V , VLCD +5V,

// Pin 1 ----------> 0V
// Pin 2 ----------> +5V // VDD
// Pin 3 ----------> +5V // VLCD
// Pin 4 ----------> RS ( Register Select 1:Data Input 0:Instruction Input)
// Pin 5 ----------> R/W (1: Read 0:Write)
// Pin 6 ----------> E (CLOCK)
// Pin 7 ----------> DB0
// Pin 8 ----------> DB1
// Pin 9 ----------> DB2
// Pin 10 ----------> DB3
// Pin 11 ----------> DB4
// Pin 12 ----------> DB5
// Pin 13 ----------> DB6
// Pin 14 ----------> DB7
// Pin 15 ----------> BACKLIGHT +5V
// Pin 16 ----------> BACKLIGHT GND

#include "f2802x_common/include/gpio.h"
#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File
#define tc 2 // time between commands or data
extern GPIO_Handle myGpio;

void Enable(void) {

	GPIO_setHigh(myGpio, GPIO_Number_1); // Set LCD Enable Mode High

	_nop;

	GPIO_setLow(myGpio, GPIO_Number_1); // Set LCD Enable Mode Low

}

void isBusy(void) {

	// TODO : Sets Pin_DB7..Pin_DB0 Input

	GPIO_setHigh(myGpio, GPIO_Number_0); // Set LCD Read Mode

	GPIO_setLow(myGpio, GPIO_Number_2);  // Set LCD Instruction mode

	do {

		Enable();

	} while (pin_DB7);

	GPIO_setLow(myGpio, GPIO_Number_0);  // Set LCD Write Mode

	// Sets Pin_DB7..Pin_DB0 Output

}

void clear_display(void) {

	isBusy();

	// TODO : Write the code for GPIO�s pins

	pin_DB7 = 0;
	pin_DB6 = 0;
	pin_DB5 = 0;
	pin_DB4 = 0;
	pin_DB3 = 0;
	pin_DB2 = 0;
	pin_DB1 = 0;
	pin_DB0 = 1;

	Enable();


}

