/*
 * MyUART.c
 *
 *  Created on: May 11, 2022
 *      Author: Admin
 */
#include "MyUART.h"

uint8_t ReceiveArray[50] = {0x00};
uint8_t LengthReceiveArray[5] = {0x00};
uint8_t TransmitArray[50] = {0x00};

uint8_t ReceiveVar = 0;
uint16_t ui16_PrevReceive = 0;

sData Transmit = {
	.DataArray = TransmitArray,
	.ui16_length = 0,
	.LengthArray = NULL,
};

volatile sData Receive = {
	.DataArray = ReceiveArray,
	.ui16_length = 0,
	.LengthArray = NULL,
};


uint8_t UART_Additional_Init(void)
{
	HAL_UART_Receive_IT(&huart5, &ReceiveVar, 1);
	return 1;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	*(Receive.DataArray + Receive.ui16_length) = ReceiveVar;
	Receive.ui16_length++;
	HAL_UART_Receive_IT(&huart5, &ReceiveVar, 1);
}

void UART_Reset_Data (sData *Data)
{
	Data->ui16_length = 0;
	for (uint8_t i = 0; i < 50; i++)
	{
		*(Data->DataArray + i) = 0x00;
	}
	for (uint8_t i = 0; i < 5; i++)
	{
		*(Data->LengthArray + i) = 0x00;
	}
	if (Data == &Receive)
	{
		ui16_PrevReceive = 0;
	}
}

uint8_t UART_Send_ACK( sData *ACK )
{
    UART_Transmit_Data( ACK );
	return 1;
}

uint8_t UART_Transmit_Data (sData *Data)
{
	HAL_UART_Transmit(&huart5, Data->LengthArray, 5, 500);
	HAL_Delay(5);
    HAL_UART_Transmit(&huart5, Data->DataArray, Data->ui16_length, 500);
    HAL_Delay(5);
	return 1;
}

