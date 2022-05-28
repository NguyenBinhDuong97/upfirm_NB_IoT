################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM0/port.c 

OBJS += \
./Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM0/port.o 

C_DEPS += \
./Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM0/port.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM0/port.o: ../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM0/port.c Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM0/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L082xx -c -I../Core/Inc -I../Drivers/STM32L0xx_HAL_Driver/Inc -I../Drivers/STM32L0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L0xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM0 -I"D:/Source_Tree_data/projects/NB_IoT/Update_OTA/upfirm_NB_IoT/Updata_OTA_NBIoT/MCU/HostClient/Host/LibMQTT/inc" -I"D:/Source_Tree_data/projects/NB_IoT/Update_OTA/upfirm_NB_IoT/Updata_OTA_NBIoT/MCU/HostClient/Host/own_lib/inc" -I"D:/Source_Tree_data/projects/NB_IoT/Update_OTA/upfirm_NB_IoT/Updata_OTA_NBIoT/MCU/HostClient/Host/BC66/Inc" -I"D:/Source_Tree_data/projects/NB_IoT/Update_OTA/upfirm_NB_IoT/Updata_OTA_NBIoT/MCU/HostClient/Host/connect" -I"D:/Source_Tree_data/projects/NB_IoT/Update_OTA/upfirm_NB_IoT/Updata_OTA_NBIoT/MCU/HostClient/Host/uartWinApp" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM0/port.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

