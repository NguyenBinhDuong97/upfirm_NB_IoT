/**
  * @brief    This file was created to initialize all function that use uart to contact with BC66
	            User can easily apply or change to fit their application
	* @creator 	Nguyen Binh Duong _ R&D Staff _ Sao Viet company
  * @data     7/12/2021	
*/
/*----------------------------------------------------------------------------------------------*/

#include "bc66_uart.h"
#include "string.h"
#include "bc66_cmd.h"
#include "stdlib.h"
#include "uartWinnApp.h"
/*---------------------------------------------------------------------------------------------*/

/* declare variables area */

/* declare bc66 uart receive variable */
//volatile sType_uart_receive  dType_bc66_receive;    

/* declare string that excute on screen when over flow happen*/
uint8_t string_over_flow[] = "Tran bo dem BC66 receive\r\n";

sType_string bc66_rec_UART = {
		.pData = dType_water_NB_IoT.dType_bc66_receive.ui8buf_rx,
		.u16_len_data = 0,
};

/*---------------------------------------------------------------------------------------------*/


/** 
  * @brief This function was excuted from HAL library 
	         and it excuted UART receive interrupt service rountine 
					 every time MCU complete receive 1 charater 
	* @note Read HAL UART driver for more detail
*/
volatile uint16_t CountReceive_u16 = 0;

uint16_t CountLength = 0;


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(huart);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_UART_RxCpltCallback can be implemented in the user file.
   */
	if (huart->Instance == uart_bc66.Instance)
	{
		 *( dType_water_NB_IoT.dType_bc66_receive.u8p_rx_pointer ) = dType_water_NB_IoT.dType_bc66_receive.ui8_rx_data;
		 dType_water_NB_IoT.dType_bc66_receive.u8p_rx_pointer++;
		 dType_water_NB_IoT.dType_bc66_receive.ui16_rx_byte++;
		 if ( dType_water_NB_IoT.dType_bc66_receive.ui16_rx_byte ==  MAX_ELEMENT_RECEIVE )
		 {
			 dType_water_NB_IoT.dType_bc66_receive.ui8_rx_over_flow_flag = 1; 
		 }
		CountReceive_u16++;
		 
//		if ( dType_water_NB_IoT.dType_AT.ui8_pointer != _TCP_SEND_DATA_ ) //test case send error and out send data MQTT
			 
	    HAL_UART_Receive_IT ( &uart_bc66 , &(dType_water_NB_IoT.dType_bc66_receive.ui8_rx_data), 1 ); //enable receive interrupt
	}
	else if (huart->Instance == uart_debug.Instance)
	{
        *(WinUart.pData + WinUart.numb_rec) = WinUart.data;
        WinUart.numb_rec++;
        if (WinUart.numb_rec == WinUart.max_element)
        {
          //xu ly khi tran bo nho dem
        }
        HAL_UART_Receive_IT ( &uart_debug, &WinUart.data, 1 ); //enable receive interrupt
	}
}

/**
  * @brief  Init receive BC66 uart interrupt and initialize element in dType_water_NB_IoT.dType_bc66_receive struct
  * @param  NULL
  * @retval NULL
*/
void BC66_UART_Receive_Init (void)
{
	dType_water_NB_IoT.dType_bc66_receive.u8p_rx_pointer = dType_water_NB_IoT.dType_bc66_receive.ui8buf_rx;// pointer of dType_water_NB_IoT.dType_bc66_receive point to dType_bc66_receive buffer
	dType_water_NB_IoT.dType_bc66_receive.ui16_rx_byte = 0;    //byte receive from BC66_uart
	dType_water_NB_IoT.dType_bc66_receive.ui8_rx_data = 0; // initialize dType_water_NB_IoT.dType_bc66_receive ui8_rx_data
	dType_water_NB_IoT.dType_bc66_receive.ui16_pre_rx_byte = 0; // initialize bc66 pre receive byte
	dType_water_NB_IoT.dType_bc66_receive.ui8_rx_over_flow_flag = 0; // initialize bc66 receive over flow flag
	HAL_UART_Receive_IT ( &uart_bc66 , &(dType_water_NB_IoT.dType_bc66_receive.ui8_rx_data) , 1 ); //enable receive interrupt
}

/**
  * @brief  This is function was created to transmit data from MCU to BC66
  * @param  uint8_t *Data: pointer point to location that contain data need to be transmited
  * @retval NULL
*/
void BC66_UART_Send_Data_To_BC66 ( uint8_t *data , uint16_t data_len )
{
	HAL_UART_Transmit ( &uart_bc66 , data , data_len , 1000 );
}

/**
  * @brief   This function was created to transmit data from MCU to PC terminal through UART
  * @param   uint8_t *Data: pointer point to location that contain data need to be transmited
  * @retval  NULL
*/
void BC66_UART_Send_Data_To_Terminal ( uint8_t *Data , uint16_t data_length )
{
	uint8_t length_array[5] = {0x00};
	itoa (data_length, (char*)length_array, 10);
	Convert_length_to_array ((uint8_t*)length_array, 5);
	HAL_UART_Transmit (&uart_debug, length_array, 5, 500);
	HAL_UART_Transmit (&uart_debug, Data, data_length, 500);
}

/**
  * @brief  Check receive process status
  * @param  NULL
  * @retval DONE
            NOT_DONE
*/
uint8_t BC66_UART_Check_Receive_Process_Status (void)
{
	if ( dType_water_NB_IoT.dType_bc66_receive.ui16_rx_byte != 0 )
	{
		if ( dType_water_NB_IoT.dType_bc66_receive.ui16_rx_byte == dType_water_NB_IoT.dType_bc66_receive.ui16_pre_rx_byte )
			return DONE;
		else if ( dType_water_NB_IoT.dType_bc66_receive.ui16_rx_byte != dType_water_NB_IoT.dType_bc66_receive.ui16_pre_rx_byte )
		{
		  dType_water_NB_IoT.dType_bc66_receive.ui16_pre_rx_byte = dType_water_NB_IoT.dType_bc66_receive.ui16_rx_byte;
			return NOT_DONE;
		}	
	}	
	else if (dType_water_NB_IoT.dType_bc66_receive.ui16_rx_byte == 0)
	{
		return NONE;
	}
}

/**
  * @brief   Clear data in receive from BC66 and recieve process status
  * @param   NULL
  * @retval  NULL
*/
void BC66_UART_Clear_BC66_Data ( void )
{
    Reset_Buffer ( ( uint8_t* )dType_water_NB_IoT.dType_bc66_receive.ui8buf_rx , 500 );
    dType_water_NB_IoT.dType_bc66_receive.ui16_rx_byte = 0;
    dType_water_NB_IoT.dType_bc66_receive.ui16_pre_rx_byte = 0;
    dType_water_NB_IoT.dType_bc66_receive.u8p_rx_pointer = dType_water_NB_IoT.dType_bc66_receive.ui8buf_rx;
}




