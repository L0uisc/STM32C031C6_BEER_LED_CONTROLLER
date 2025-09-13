################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/app_threadx.c \
../Core/Src/main.c \
../Core/Src/stm32c0xx_hal_msp.c \
../Core/Src/stm32c0xx_hal_timebase_tim.c \
../Core/Src/stm32c0xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32c0xx.c 

S_UPPER_SRCS += \
../Core/Src/tx_initialize_low_level.S 

OBJS += \
./Core/Src/app_threadx.o \
./Core/Src/main.o \
./Core/Src/stm32c0xx_hal_msp.o \
./Core/Src/stm32c0xx_hal_timebase_tim.o \
./Core/Src/stm32c0xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32c0xx.o \
./Core/Src/tx_initialize_low_level.o 

S_UPPER_DEPS += \
./Core/Src/tx_initialize_low_level.d 

C_DEPS += \
./Core/Src/app_threadx.d \
./Core/Src/main.d \
./Core/Src/stm32c0xx_hal_msp.d \
./Core/Src/stm32c0xx_hal_timebase_tim.d \
./Core/Src/stm32c0xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32c0xx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -DUSE_NUCLEO_64 -DUSE_HAL_DRIVER -DSTM32C031xx -DTX_INCLUDE_USER_DEFINE_FILE -c -I../Core/Inc -I../Drivers/STM32C0xx_HAL_Driver/Inc -I../Drivers/STM32C0xx_HAL_Driver/Inc/Legacy -I../Drivers/BSP/STM32C0xx_Nucleo -I../Drivers/CMSIS/Device/ST/STM32C0xx/Include -I../Drivers/CMSIS/Include -I../AZURE_RTOS/App -I../Middlewares/ST/threadx/common/inc -I../Middlewares/ST/threadx/ports/cortex_m0/gnu/inc -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Core/Src/%.o: ../Core/Src/%.S Core/Src/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m0plus -c -I../Core/Inc -I../AZURE_RTOS/App -I../Drivers/STM32C0xx_HAL_Driver/Inc -I../Drivers/STM32C0xx_HAL_Driver/Inc/Legacy -I../Middlewares/ST/threadx/common/inc -I../Drivers/BSP/STM32C0xx_Nucleo -I../Drivers/CMSIS/Device/ST/STM32C0xx/Include -I../Middlewares/ST/threadx/ports/cortex_m0/gnu/inc -I../Middlewares/ST/threadx/utility/low_power -I../Drivers/CMSIS/Include -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@" "$<"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/app_threadx.cyclo ./Core/Src/app_threadx.d ./Core/Src/app_threadx.o ./Core/Src/app_threadx.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/stm32c0xx_hal_msp.cyclo ./Core/Src/stm32c0xx_hal_msp.d ./Core/Src/stm32c0xx_hal_msp.o ./Core/Src/stm32c0xx_hal_msp.su ./Core/Src/stm32c0xx_hal_timebase_tim.cyclo ./Core/Src/stm32c0xx_hal_timebase_tim.d ./Core/Src/stm32c0xx_hal_timebase_tim.o ./Core/Src/stm32c0xx_hal_timebase_tim.su ./Core/Src/stm32c0xx_it.cyclo ./Core/Src/stm32c0xx_it.d ./Core/Src/stm32c0xx_it.o ./Core/Src/stm32c0xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32c0xx.cyclo ./Core/Src/system_stm32c0xx.d ./Core/Src/system_stm32c0xx.o ./Core/Src/system_stm32c0xx.su ./Core/Src/tx_initialize_low_level.d ./Core/Src/tx_initialize_low_level.o

.PHONY: clean-Core-2f-Src

