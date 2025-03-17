################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Firmware/DriverMotor/DriverMotor.c 

OBJS += \
./Core/Firmware/DriverMotor/DriverMotor.o 

C_DEPS += \
./Core/Firmware/DriverMotor/DriverMotor.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Firmware/DriverMotor/%.o Core/Firmware/DriverMotor/%.su Core/Firmware/DriverMotor/%.cyclo: ../Core/Firmware/DriverMotor/%.c Core/Firmware/DriverMotor/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Core/Firmware -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Firmware-2f-DriverMotor

clean-Core-2f-Firmware-2f-DriverMotor:
	-$(RM) ./Core/Firmware/DriverMotor/DriverMotor.cyclo ./Core/Firmware/DriverMotor/DriverMotor.d ./Core/Firmware/DriverMotor/DriverMotor.o ./Core/Firmware/DriverMotor/DriverMotor.su

.PHONY: clean-Core-2f-Firmware-2f-DriverMotor

