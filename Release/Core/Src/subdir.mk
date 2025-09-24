################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/24LC16B.c \
../Core/Src/Config.c \
../Core/Src/Ext_IO.c \
../Core/Src/Fan.c \
../Core/Src/Mesaure.c \
../Core/Src/Parameter.c \
../Core/Src/Process.c \
../Core/Src/Uart.c \
../Core/Src/main.c \
../Core/Src/stm32l4xx_hal_msp.c \
../Core/Src/stm32l4xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32l4xx.c 

OBJS += \
./Core/Src/24LC16B.o \
./Core/Src/Config.o \
./Core/Src/Ext_IO.o \
./Core/Src/Fan.o \
./Core/Src/Mesaure.o \
./Core/Src/Parameter.o \
./Core/Src/Process.o \
./Core/Src/Uart.o \
./Core/Src/main.o \
./Core/Src/stm32l4xx_hal_msp.o \
./Core/Src/stm32l4xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32l4xx.o 

C_DEPS += \
./Core/Src/24LC16B.d \
./Core/Src/Config.d \
./Core/Src/Ext_IO.d \
./Core/Src/Fan.d \
./Core/Src/Mesaure.d \
./Core/Src/Parameter.d \
./Core/Src/Process.d \
./Core/Src/Uart.d \
./Core/Src/main.d \
./Core/Src/stm32l4xx_hal_msp.d \
./Core/Src/stm32l4xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32l4xx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32L412xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/24LC16B.cyclo ./Core/Src/24LC16B.d ./Core/Src/24LC16B.o ./Core/Src/24LC16B.su ./Core/Src/Config.cyclo ./Core/Src/Config.d ./Core/Src/Config.o ./Core/Src/Config.su ./Core/Src/Ext_IO.cyclo ./Core/Src/Ext_IO.d ./Core/Src/Ext_IO.o ./Core/Src/Ext_IO.su ./Core/Src/Fan.cyclo ./Core/Src/Fan.d ./Core/Src/Fan.o ./Core/Src/Fan.su ./Core/Src/Mesaure.cyclo ./Core/Src/Mesaure.d ./Core/Src/Mesaure.o ./Core/Src/Mesaure.su ./Core/Src/Parameter.cyclo ./Core/Src/Parameter.d ./Core/Src/Parameter.o ./Core/Src/Parameter.su ./Core/Src/Process.cyclo ./Core/Src/Process.d ./Core/Src/Process.o ./Core/Src/Process.su ./Core/Src/Uart.cyclo ./Core/Src/Uart.d ./Core/Src/Uart.o ./Core/Src/Uart.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/stm32l4xx_hal_msp.cyclo ./Core/Src/stm32l4xx_hal_msp.d ./Core/Src/stm32l4xx_hal_msp.o ./Core/Src/stm32l4xx_hal_msp.su ./Core/Src/stm32l4xx_it.cyclo ./Core/Src/stm32l4xx_it.d ./Core/Src/stm32l4xx_it.o ./Core/Src/stm32l4xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32l4xx.cyclo ./Core/Src/system_stm32l4xx.d ./Core/Src/system_stm32l4xx.o ./Core/Src/system_stm32l4xx.su

.PHONY: clean-Core-2f-Src

