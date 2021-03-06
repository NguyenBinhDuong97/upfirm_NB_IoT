/*
 * application.h
 *
 *  Created on: May 12, 2022
 *      Author: Admin
 */

#ifndef APPLICATION_H_
#define APPLICATION_H_

#include "main.h"
#include "stdint.h"
#include "EventDrive.h"
#include "MyUART.h"
#include "myGPIO.h"

#include "mylib.h"
#include "string.h"
#include "stdlib.h"

#define START_BOOT_ADDRESS ((uint32_t)0x08010000) //Base sector

typedef enum{
	_INITIAL_ = 1,             //Cac chuc nang khi khoi dong chuong trinh
	_PUSH_BOOT_BUTTON_,    //thuc hien cac chuc nang khi nhan BOOT BUTTON
	_BOOT_START_,          //thuc hien cac chuc nang trong phan khoi dong boot
	_BOOTING_,             //nhay vao qua trinh boot nhu nap vao flash
	_BOOT_END_,            //thuc hien gui khi xong qua trinh boot
	_CHECK_LOAD_DATA_RECEIVE_DONE_,  //kiem tra nhan xong hay chua
	_CHECK_RECEIVE_,       //Kiem tra phan nhan xem co dung yeu cau hay khong chu yeu phan ack
	_CALL_APPLICATION_,    //nhay den code application
	_CHECK_BOOT_PROCESS_,  //timer check quá trình boot có bị treo hay không
	_BOOT_ERROR_,          //xu ly qua trinh boot bi loi
} EVENT;

extern object BootApp;
extern uint8_t enter_boot;
extern volatile uint8_t CheckReceive;
extern uint8_t sent_boot;
extern uint32_t boot_address;

uint8_t  ApplicationBoot_Check_Receive_DataLoad_Done (void);
uint8_t  ApplicationBoot_Push_Event_to_Queue( sQueue_Struct_TypeDef *AppQueue, event EventName );
uint8_t  ApplicationBoot_Get_Event_from_Queue( sQueue_Struct_TypeDef *AppQueue, uint8_t Type );
void     ApplicationBoot_Hanlde_Event (event Event);
uint8_t  ApplicationBoot_Call_Event (event ApplicationEvent, event NameEvent);
uint8_t  ApplicationBoot_Initial (void);
uint8_t  ApplicationBoot_BootStart_Hanlde(void);
uint8_t  ApplicationBoot_UART_CheckReceive(void);
uint8_t  ApplicationBoot_Flash_Program (uint32_t Address, uint64_t Data);
void  ApplicationBoot_Jumpto_ApplicationProgram (void);
uint8_t  ApplicationBoot_EraseFlash_ApplicationProgram(void);
HAL_StatusTypeDef	OnchipFlashWriteData (uint32_t andress, uint8_t	*data_address, uint32_t data_length);

#endif /* APPLICATION_H_ */
