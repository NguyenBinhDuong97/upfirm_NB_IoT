################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../applicattion/application.c 

OBJS += \
./applicattion/application.o 

C_DEPS += \
./applicattion/application.d 


# Each subdirectory must supply rules for building sources it contributes
applicattion/application.o: ../applicattion/application.c applicattion/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Admin/Desktop/stm32f4/Upfirm_wireless_MQTT/Boot_and_WinApp/Ver1/Stm32f4/Bootloader/myFlash" -I"C:/Users/Admin/Desktop/stm32f4/Upfirm_wireless_MQTT/Boot_and_WinApp/Ver1/Stm32f4/Bootloader/myGPIO" -I"C:/Users/Admin/Desktop/stm32f4/Upfirm_wireless_MQTT/Boot_and_WinApp/Ver1/Stm32f4/Bootloader/MyLib" -I"C:/Users/Admin/Desktop/stm32f4/Upfirm_wireless_MQTT/Boot_and_WinApp/Ver1/Stm32f4/Bootloader/myUART" -I"C:/Users/Admin/Desktop/stm32f4/Upfirm_wireless_MQTT/Boot_and_WinApp/Ver1/Stm32f4/Bootloader/queue_VHP" -I"C:/Users/Admin/Desktop/stm32f4/Upfirm_wireless_MQTT/Boot_and_WinApp/Ver1/Stm32f4/Bootloader/EventDrive" -I"C:/Users/Admin/Desktop/stm32f4/Upfirm_wireless_MQTT/Boot_and_WinApp/Ver1/Stm32f4/Bootloader/applicattion" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"applicattion/application.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

