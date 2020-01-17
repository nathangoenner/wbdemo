################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Utilities/subdir.mk
-include Middlewares/STM32_WPAN/subdir.mk
-include Drivers/STM32WBxx_HAL_Driver/subdir.mk
-include Drivers/CMSIS/subdir.mk
-include Drivers/BSP/IKS01A3/subdir.mk
-include Drivers/BSP/Components/subdir.mk
-include Application/User/STM32_WPAN/Target/subdir.mk
-include Application/User/STM32_WPAN/App/subdir.mk
-include Application/User/MEMS/App/subdir.mk
-include Application/User/Core/subdir.mk
-include Application/STM32CubeIDE/subdir.mk
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
MOTENV1_IKS01A3_WB55RG.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
MOTENV1_IKS01A3_WB55RG.list \

OBJCOPY_BIN += \
MOTENV1_IKS01A3_WB55RG.bin \


# All Target
all:
	+@make --no-print-directory main-build && make --no-print-directory post-build

# Main-build Target
main-build: MOTENV1_IKS01A3_WB55RG.elf secondary-outputs

# Tool invocations
MOTENV1_IKS01A3_WB55RG.elf: $(OBJS) $(USER_OBJS) ../stm32wb55xx_flash_cm4.ld
	arm-none-eabi-gcc -o "MOTENV1_IKS01A3_WB55RG.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"../stm32wb55xx_flash_cm4.ld" --specs=nosys.specs -Wl,-Map="MOTENV1_IKS01A3_WB55RG.map" -Wl,--gc-sections -static -L../../../../../../../Middlewares/ST/STM32_MotionAR_Library/Lib -L../../../../../../../Middlewares/ST/STM32_MotionAW_Library/Lib -L../../../../../../../Middlewares/ST/STM32_MotionCP_Library/Lib -L../../../../../../../Middlewares/ST/STM32_MotionFX_Library/Lib -L../../../../../../../Middlewares/ST/STM32_MotionGR_Library/Lib -L../../../../../../../Middlewares/ST/STM32_MotionID_Library/Lib -L../../../../../../../Middlewares/ST/STM32_MotionPM_Library/Lib --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

MOTENV1_IKS01A3_WB55RG.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "MOTENV1_IKS01A3_WB55RG.list"
	@echo 'Finished building: $@'
	@echo ' '

MOTENV1_IKS01A3_WB55RG.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "MOTENV1_IKS01A3_WB55RG.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

post-build:
	arm-none-eabi-objcopy -O binary "MOTENV1_IKS01A3_WB55RG.elf" "MOTENV1_IKS01A3_WB55RG.bin"
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
