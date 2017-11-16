################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include emlib/subdir.mk
-include CMSIS/EFM32LG/subdir.mk
-include BSP/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: SoleRunner.axf

# Tool invocations
SoleRunner.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU ARM C Linker'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb -T "SoleRunner.ld" -Xlinker --gc-sections -Xlinker -Map="SoleRunner.map" --specs=nano.specs -o SoleRunner.axf "./BSP/bsp_bcc.o" "./BSP/bsp_stk.o" "./BSP/bsp_stk_leds.o" "./BSP/bsp_trace.o" "./CMSIS/EFM32LG/startup_gcc_efm32lg.o" "./CMSIS/EFM32LG/system_efm32lg.o" "./emlib/em_assert.o" "./emlib/em_cmu.o" "./emlib/em_ebi.o" "./emlib/em_emu.o" "./emlib/em_gpio.o" "./emlib/em_system.o" "./emlib/em_usart.o" "./src/blink.o" -Wl,--start-group -lgcc -lc -lnosys -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

	@echo 'Building hex file: SoleRunner.hex'
	arm-none-eabi-objcopy -O ihex "SoleRunner.axf" "SoleRunner.hex"
	@echo ' '

	@echo 'Building bin file: SoleRunner.bin'
	arm-none-eabi-objcopy -O binary "SoleRunner.axf" "SoleRunner.bin"
	@echo ' '

	@echo 'Running size tool'
	arm-none-eabi-size "SoleRunner.axf"
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) SoleRunner.axf
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
