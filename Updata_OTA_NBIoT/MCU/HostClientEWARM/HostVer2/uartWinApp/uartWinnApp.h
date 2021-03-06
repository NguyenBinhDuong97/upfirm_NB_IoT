/*
 * uartWinnApp.h
 *
 *  Created on: May 26, 2022
 *      Author: Admin
 */

#ifndef UARTWINNAPP_H_
#define UARTWINNAPP_H_

#include "bc66_cmd.h"
#include "bc66_uart.h"
#include "cmsis_os.h"
#include "MQTTPacket.h"
#include "MQTTFormat.h"
#include "bc66_cmd.h"
#include "bc66_uart.h"
#include "MQTTConnect.h"
#include "MQTT.h"
#include "stdio.h"
#include "string.h"
#include "my_lib.h"
#include "stdlib.h"

typedef enum{
	_APPZONE_SIG_,
	_ID_SIG_,
//	_ADDRESS_SIG_,
	_DATA_SIG_,
	_END_SIG_,
} eUartWinCaseRec;

typedef struct{
    uint8_t sig;
    sType_string ReceiveString;
    func_callback_pointer uartWinAppHandle;
}sUartWinApp;

typedef struct{
	uint8_t data;
	uint8_t *pData;
	uint16_t numb_rec;
	uint16_t pre_numb_rec;
	uint16_t max_element;
}sWinAppRec;


extern sWinAppRec WinUart;
extern sType_string uart_rec_WinApp;
extern sType_string Connect_String;
extern uint8_t sub_flag;


uint8_t fncAppZoneHandle(void);
uint8_t fncIdHandle (void);
uint8_t fncAddressHandle (void);
uint8_t fncDataHandle (void);
uint8_t fncEndHandle (void);

uint8_t WinApp_Check_Time_Out( uint32_t mark, uint32_t cycle );
uint8_t WinApp_UART_Check_Receive_Process_Status (void);
uint8_t WinApp_Reset(void);
uint8_t WinApp_Handle_UART_Receive(void);
uint8_t WinApp_Find_Extract_Signal_To_An_Array (uint8_t *array, uint16_t numb_elem, uint8_t signal);
void    Convert_length_to_array (uint8_t *array, uint16_t ui16array_lenght);

uint8_t Push_BC66_Message_to_Queue (void);
uint8_t Push_WinApp_Message_to_Queue (void);
void    UART_Send_Data_To_Terminal ( sType_string *Message );
uint8_t Push_Message_to_Queue (sType_string *Message);
uint8_t WinApp_Find_Extract_Data_Signal_To_An_Array (uint8_t *array);
#endif /* UARTWINNAPP_H_ */
