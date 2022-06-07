################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../LibMQTT/source/MQTTConnectClient.c \
../LibMQTT/source/MQTTConnectServer.c \
../LibMQTT/source/MQTTDeserializePublish.c \
../LibMQTT/source/MQTTFormat.c \
../LibMQTT/source/MQTTPacket.c \
../LibMQTT/source/MQTTSerializePublish.c \
../LibMQTT/source/MQTTSubscribeClient.c \
../LibMQTT/source/MQTTSubscribeServer.c \
../LibMQTT/source/MQTTUnsubscribeClient.c \
../LibMQTT/source/MQTTUnsubscribeServer.c 

OBJS += \
./LibMQTT/source/MQTTConnectClient.o \
./LibMQTT/source/MQTTConnectServer.o \
./LibMQTT/source/MQTTDeserializePublish.o \
./LibMQTT/source/MQTTFormat.o \
./LibMQTT/source/MQTTPacket.o \
./LibMQTT/source/MQTTSerializePublish.o \
./LibMQTT/source/MQTTSubscribeClient.o \
./LibMQTT/source/MQTTSubscribeServer.o \
./LibMQTT/source/MQTTUnsubscribeClient.o \
./LibMQTT/source/MQTTUnsubscribeServer.o 

C_DEPS += \
./LibMQTT/source/MQTTConnectClient.d \
./LibMQTT/source/MQTTConnectServer.d \
./LibMQTT/source/MQTTDeserializePublish.d \
./LibMQTT/source/MQTTFormat.d \
./LibMQTT/source/MQTTPacket.d \
./LibMQTT/source/MQTTSerializePublish.d \
./LibMQTT/source/MQTTSubscribeClient.d \
./LibMQTT/source/MQTTSubscribeServer.d \
./LibMQTT/source/MQTTUnsubscribeClient.d \
./LibMQTT/source/MQTTUnsubscribeServer.d 


# Each subdirectory must supply rules for building sources it contributes
LibMQTT/source/%.o LibMQTT/source/%.su: ../LibMQTT/source/%.c LibMQTT/source/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L082xx -c -I../Core/Inc -I../Drivers/STM32L0xx_HAL_Driver/Inc -I../Drivers/STM32L0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L0xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM0 -I"D:/Source_Tree_data/projects/NB_IoT/Update_OTA/upfirm_NB_IoT/Updata_OTA_NBIoT/MCU/HostClient/Host/LibMQTT/inc" -I"D:/Source_Tree_data/projects/NB_IoT/Update_OTA/upfirm_NB_IoT/Updata_OTA_NBIoT/MCU/HostClient/Host/own_lib/inc" -I"D:/Source_Tree_data/projects/NB_IoT/Update_OTA/upfirm_NB_IoT/Updata_OTA_NBIoT/MCU/HostClient/Host/BC66/Inc" -I"D:/Source_Tree_data/projects/NB_IoT/Update_OTA/upfirm_NB_IoT/Updata_OTA_NBIoT/MCU/HostClient/Host/connect" -I"D:/Source_Tree_data/projects/NB_IoT/Update_OTA/upfirm_NB_IoT/Updata_OTA_NBIoT/MCU/HostClient/Host/uartWinApp" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-LibMQTT-2f-source

clean-LibMQTT-2f-source:
	-$(RM) ./LibMQTT/source/MQTTConnectClient.d ./LibMQTT/source/MQTTConnectClient.o ./LibMQTT/source/MQTTConnectClient.su ./LibMQTT/source/MQTTConnectServer.d ./LibMQTT/source/MQTTConnectServer.o ./LibMQTT/source/MQTTConnectServer.su ./LibMQTT/source/MQTTDeserializePublish.d ./LibMQTT/source/MQTTDeserializePublish.o ./LibMQTT/source/MQTTDeserializePublish.su ./LibMQTT/source/MQTTFormat.d ./LibMQTT/source/MQTTFormat.o ./LibMQTT/source/MQTTFormat.su ./LibMQTT/source/MQTTPacket.d ./LibMQTT/source/MQTTPacket.o ./LibMQTT/source/MQTTPacket.su ./LibMQTT/source/MQTTSerializePublish.d ./LibMQTT/source/MQTTSerializePublish.o ./LibMQTT/source/MQTTSerializePublish.su ./LibMQTT/source/MQTTSubscribeClient.d ./LibMQTT/source/MQTTSubscribeClient.o ./LibMQTT/source/MQTTSubscribeClient.su ./LibMQTT/source/MQTTSubscribeServer.d ./LibMQTT/source/MQTTSubscribeServer.o ./LibMQTT/source/MQTTSubscribeServer.su ./LibMQTT/source/MQTTUnsubscribeClient.d ./LibMQTT/source/MQTTUnsubscribeClient.o ./LibMQTT/source/MQTTUnsubscribeClient.su ./LibMQTT/source/MQTTUnsubscribeServer.d ./LibMQTT/source/MQTTUnsubscribeServer.o ./LibMQTT/source/MQTTUnsubscribeServer.su

.PHONY: clean-LibMQTT-2f-source

