/*
 * myGPIO.c
 *
 *  Created on: May 11, 2022
 *      Author: Admin
 */

#include "myGPIO.h"

extern uint8_t EnterBoot;

uint8_t GPIO_Led_BootData_Notify(void)
{
    HAL_GPIO_TogglePin(LED_START_GPIO_Port, LED_START_Pin);
 	return 1;
}

uint8_t GPIO_Led_BootStop_Notify(void)
{
	for (uint8_t i = 0; i < 10; i++)
	{
	  HAL_GPIO_TogglePin(LED_STOP_GPIO_Port, LED_STOP_Pin);
	  HAL_Delay(200);
	}
	HAL_GPIO_WritePin(LED_STOP_GPIO_Port, LED_STOP_Pin, GPIO_PIN_SET);
 	return 1;
}

uint8_t GPIO_Led_BootError_Notify(void)
{
	for (uint8_t i = 0; i < 10; i++)
	{
	  HAL_GPIO_TogglePin(LED_ERROR_GPIO_Port, LED_ERROR_Pin);
	  HAL_Delay(200);
	}
	HAL_GPIO_WritePin(LED_ERROR_GPIO_Port, LED_ERROR_Pin, GPIO_PIN_SET);
 	return 1;
}

uint8_t GPIO_Led_BootStart_Notify(void)
{
	for (uint8_t i = 0; i < 10; i++)
	{
	  HAL_GPIO_TogglePin(LED_START_GPIO_Port, LED_START_Pin);
	  HAL_Delay(200);
	}
	HAL_GPIO_WritePin(LED_START_GPIO_Port, LED_START_Pin, GPIO_PIN_SET);
 	return 1;
}


uint8_t GPIO_ResetAll_LED(void)
{
	HAL_GPIO_WritePin(LED_START_GPIO_Port, LED_START_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_ERROR_GPIO_Port, LED_ERROR_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_STOP_GPIO_Port, LED_STOP_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_DATA_GPIO_Port, LED_DATA_Pin, GPIO_PIN_RESET);
	return 1;
}

uint8_t GPIO_Led_Initial(void)
{
    for (uint8_t i = 0; i < 60; i++)
    {
    	HAL_GPIO_TogglePin(LED_DATA_GPIO_Port, LED_START_Pin|LED_DATA_Pin|LED_ERROR_Pin|LED_STOP_Pin);
    	HAL_Delay(500);
    }
    return 1;
}
