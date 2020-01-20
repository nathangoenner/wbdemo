################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Utilities/subdir.mk
-include Middlewares/STM32_WPAN/utilities/subdir.mk
-include Middlewares/STM32_WPAN/interface/patterns/ble_thread/tl/subdir.mk
-include Middlewares/STM32_WPAN/interface/patterns/ble_thread/shci/subdir.mk
-include Middlewares/STM32_WPAN/ble/menu/subdir.mk
-include Middlewares/STM32_WPAN/ble/core/subdir.mk
-include Middlewares/STM32_WPAN/ble/blesvc/subdir.mk
-include Drivers/STM32WBxx_HAL_Driver/subdir.mk
-include Drivers/CMSIS/subdir.mk
-include Drivers/BSP/P-NUCLEO-WB55.Nucleo/subdir.mk
-include Application/SW4STM32/subdir.mk
-include Application/STM32_WPAN/target/subdir.mk
-include Application/STM32_WPAN/app/subdir.mk
-include Application/Core/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
BLE_CableReplacement.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
BLE_CableReplacement.list \

OBJCOPY_HEX += \
BLE_CableReplacement.hex \


# All Target
all: BLE_CableReplacement.elf secondary-outputs

# Tool invocations
BLE_CableReplacement.elf: $(OBJS) $(USER_OBJS) ../stm32wb55xx_flash_cm4.ld
	arm-none-eabi-gcc -o "BLE_CableReplacement.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"../stm32wb55xx_flash_cm4.ld" --specs=nosys.specs -Wl,-Map="BLE_CableReplacement.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

BLE_CableReplacement.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "BLE_CableReplacement.list"
	@echo 'Finished building: $@'
	@echo ' '

BLE_CableReplacement.hex: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O ihex $(EXECUTABLES) "BLE_CableReplacement.hex"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_HEX)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
