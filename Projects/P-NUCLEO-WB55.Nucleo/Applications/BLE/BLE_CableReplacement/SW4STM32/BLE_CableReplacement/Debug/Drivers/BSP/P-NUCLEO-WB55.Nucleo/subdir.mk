################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/nkgoe/Downloads/en.stm32cubewb_v1-4-0/STM32Cube_FW_WB_V1.4.0/Drivers/BSP/P-NUCLEO-WB55.Nucleo/stm32wbxx_nucleo.c 

OBJS += \
./Drivers/BSP/P-NUCLEO-WB55.Nucleo/stm32wbxx_nucleo.o 

C_DEPS += \
./Drivers/BSP/P-NUCLEO-WB55.Nucleo/stm32wbxx_nucleo.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/P-NUCLEO-WB55.Nucleo/stm32wbxx_nucleo.o: C:/Users/nkgoe/Downloads/en.stm32cubewb_v1-4-0/STM32Cube_FW_WB_V1.4.0/Drivers/BSP/P-NUCLEO-WB55.Nucleo/stm32wbxx_nucleo.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_STM32WBXX_NUCLEO -DSTM32WB55xx -DUSE_HAL_DRIVER -DCORE_CM4 -c -I../../../../../../../../Middlewares/ST/STM32_WPAN/ble/core/template -I../../../../../../../../Middlewares/ST/STM32_WPAN/ble/core/auto -I../../../../../../../../Middlewares/ST/STM32_WPAN -I../../../../../../../../Drivers/CMSIS/Device/ST/STM32WBxx/Include -I../../../../../../../../Drivers/STM32WBxx_HAL_Driver/Inc -I../../../../../../../../Drivers/BSP/P-NUCLEO-WB55.Nucleo -I../../../../../../../../Middlewares/ST/STM32_WPAN/ble/core/ -I../../../../../../../../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/tl -I../../../../../../../../Middlewares/ST/STM32_WPAN/utilities -I../../../../../../../../Utilities/lpm/tiny_lpm -I../../../../../../../../Utilities/sequencer -I../../../../../../../../Drivers/CMSIS/Include -I../../../../../../../../Middlewares/ST/STM32_WPAN/ble/menu -I../../../../../../../../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/shci -I../../../../../../../../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread -I../../../../../../../../Middlewares/ST/STM32_WPAN/ble -I../../../Core/Inc -I../../../STM32_WPAN/App -Os -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/BSP/P-NUCLEO-WB55.Nucleo/stm32wbxx_nucleo.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

