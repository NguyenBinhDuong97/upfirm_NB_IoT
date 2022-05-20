################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../queue_VHP/queue_p.c 

OBJS += \
./queue_VHP/queue_p.o 

C_DEPS += \
./queue_VHP/queue_p.d 


# Each subdirectory must supply rules for building sources it contributes
queue_VHP/queue_p.o: D:/Source_Tree_data/projects/NB_IoT/Update_OTA/upfirm_NB_IoT/Boot_and_WinApp/Ver1/Stm32f4/Bootloader1/queue_VHP/queue_p.c queue_VHP/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/Source_Tree_data/projects/NB_IoT/Update_OTA/upfirm_NB_IoT/Boot_and_WinApp/Ver1/Stm32f4/Bootloader1/applicattion" -I"D:/Source_Tree_data/projects/NB_IoT/Update_OTA/upfirm_NB_IoT/Boot_and_WinApp/Ver1/Stm32f4/Bootloader1/EventDrive" -I"D:/Source_Tree_data/projects/NB_IoT/Update_OTA/upfirm_NB_IoT/Boot_and_WinApp/Ver1/Stm32f4/Bootloader1/myGPIO" -I"D:/Source_Tree_data/projects/NB_IoT/Update_OTA/upfirm_NB_IoT/Boot_and_WinApp/Ver1/Stm32f4/Bootloader1/MyLib" -I"D:/Source_Tree_data/projects/NB_IoT/Update_OTA/upfirm_NB_IoT/Boot_and_WinApp/Ver1/Stm32f4/Bootloader1/myUART" -I"D:/Source_Tree_data/projects/NB_IoT/Update_OTA/upfirm_NB_IoT/Boot_and_WinApp/Ver1/Stm32f4/Bootloader1/queue_VHP" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"queue_VHP/queue_p.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

