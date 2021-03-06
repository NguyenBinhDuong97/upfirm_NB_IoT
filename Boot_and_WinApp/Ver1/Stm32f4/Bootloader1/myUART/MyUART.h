/*
 * MyUART.h
 *
 *  Created on: May 11, 2022
 *      Author: Admin
 */

#ifndef MYUART_H_
#define MYUART_H_

#include "main.h"
#include "stdint.h"
#include "mylib.h"
#include "usart.h"
#include "application.h"

extern sData Transmit;
extern volatile sData Receive;
extern uint8_t ReceiveVar;
extern uint16_t ui16_PrevReceive;

uint8_t UART_Transmit_Data( sData *Data );
uint8_t UART_Wait_ACK( sData *ACK );
uint8_t UART_Send_ACK( sData *ACK );
uint8_t UART_Additional_Init(void);
void    UART_Reset_Data (sData *Data);

#endif /* MYUART_H_ */
