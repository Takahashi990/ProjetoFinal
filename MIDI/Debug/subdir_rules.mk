################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
F2802x_Adc.obj: ../F2802x_Adc.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c2000_6.2.0/bin/cl2000" -v28 -ml -mt -O4 --opt_for_speed=5 --include_path="C:/ti/controlSUITE/development_kits/C2000_LaunchPad" --include_path="C:/ti/controlSUITE/development_kits/C2000_LaunchPad/f2802x_common" --include_path="C:/ti/ccsv5/tools/compiler/c2000_6.2.0/include" -g --define="_FLASH" --define="_DEBUG" --define="DEBUG" --diag_warning=225 --display_error_number --diag_wrap=off --preproc_with_compile --preproc_dependency="F2802x_Adc.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

adc.obj: ../adc.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c2000_6.2.0/bin/cl2000" -v28 -ml -mt -O4 --opt_for_speed=5 --include_path="C:/ti/controlSUITE/development_kits/C2000_LaunchPad" --include_path="C:/ti/controlSUITE/development_kits/C2000_LaunchPad/f2802x_common" --include_path="C:/ti/ccsv5/tools/compiler/c2000_6.2.0/include" -g --define="_FLASH" --define="_DEBUG" --define="DEBUG" --diag_warning=225 --display_error_number --diag_wrap=off --preproc_with_compile --preproc_dependency="adc.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

main.obj: ../main.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c2000_6.2.0/bin/cl2000" -v28 -ml -mt -O4 --opt_for_speed=5 --include_path="C:/ti/controlSUITE/development_kits/C2000_LaunchPad" --include_path="C:/ti/controlSUITE/development_kits/C2000_LaunchPad/f2802x_common" --include_path="C:/ti/ccsv5/tools/compiler/c2000_6.2.0/include" -g --define="_FLASH" --define="_DEBUG" --define="DEBUG" --diag_warning=225 --display_error_number --diag_wrap=off --preproc_with_compile --preproc_dependency="main.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


