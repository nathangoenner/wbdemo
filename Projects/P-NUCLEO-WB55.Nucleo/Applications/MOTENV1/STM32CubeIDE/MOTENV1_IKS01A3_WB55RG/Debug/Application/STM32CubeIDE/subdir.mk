################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
C:/Users/nkgoe/Downloads/en.fp-sns-motenvwb1\ (1)/STM32CubeFunctionPack_MOTENVWB1_V1.0.0/Projects/P-NUCLEO-WB55.Nucleo/Applications/MOTENV1/STM32CubeIDE/startup_stm32wb55xx_cm4.s 

OBJS += \
./Application/STM32CubeIDE/startup_stm32wb55xx_cm4.o 


# Each subdirectory must supply rules for building sources it contributes
Application/STM32CubeIDE/startup_stm32wb55xx_cm4.o: C:/Users/nkgoe/Downloads/en.fp-sns-motenvwb1\ (1)/STM32CubeFunctionPack_MOTENVWB1_V1.0.0/Projects/P-NUCLEO-WB55.Nucleo/Applications/MOTENV1/STM32CubeIDE/startup_stm32wb55xx_cm4.s
	arm-none-eabi-gcc -mcpu=cortex-m4 -g3 -c -x assembler-with-cpp --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb -o "$@" "$<"

