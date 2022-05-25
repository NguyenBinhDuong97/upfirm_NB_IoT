#ifndef __BC66_UART_H
#define __BC66_UART_H

/*----------------------------------------------------------------------*/


#include "main.h"
#include "usart.h"
#include "stdint.h"
#include "pin_config.h"
/*----------------------------------------------------------------------*/


/* define maximum element of uart receive buffer */
#define MAX_ELEMENT_RECEIVE 500UL

/* define first element of uart receive buffer */
#define FIRST_ELEMENT_RECEIVE 0UL

/* define status when process done */
#define  DONE 	2UL

/* define status when process not done*/
#define NOT_DONE 1UL

/* define status uart not in receive process */
#define NONE 0UL

/*---------------------------------------------------------------------------*/


typedef struct 
{
 uint8_t ui8_rx_data;                         // data receive from UART
 uint8_t ui8buf_rx[MAX_ELEMENT_RECEIVE];    // uart receive buffer
 uint8_t *u8p_rx_pointer;                     // poiter of uart receive buffer
 uint16_t ui16_rx_byte;                        // bytes were received from UART module
 uint16_t ui16_pre_rx_byte;                    // bytes receive from previous check
 uint8_t ui8_rx_over_flow_flag;               // receive buffer over flow flag
} sType_uart_receive;


/*--------------------------------------------------------------------------*/

extern volatile uint16_t CountReceive_u16;
/*--------------------------------------------------------------------------*/


/* delcare functions area */
void BC66_UART_Receive_Init (void);
void BC66_UART_Send_Data_To_BC66 ( uint8_t *Data , uint16_t data_len );
uint8_t BC66_UART_Check_Receive_Process_Status ( void );
void BC66_UART_Send_Data_To_Terminal ( uint8_t *Data , uint16_t data_length );
void BC66_UART_Clear_BC66_Data ( void );

#endif