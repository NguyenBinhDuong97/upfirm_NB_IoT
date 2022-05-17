/*
 * myGPIO.h
 *
 *  Created on: May 11, 2022
 *      Author: Admin
 */

#ifndef MYGPIO_H_
#define MYGPIO_H_

#include "main.h"
#include "stdint.h"

uint8_t GPIO_Led_BootError_Notify(void);
uint8_t GPIO_Led_BootStop_Notify(void);
uint8_t GPIO_Led_BootData_Notify(void);
uint8_t GPIO_Led_BootStart_Notify(void);
uint8_t GPIO_ResetAll_LED(void);
uint8_t GPIO_BootStart_Push_Handle(void);
uint8_t GPIO_Led_Initial(void);
#endif /* MYGPIO_H_ */
