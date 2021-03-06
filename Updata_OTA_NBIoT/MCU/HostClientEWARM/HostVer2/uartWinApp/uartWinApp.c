/*
 * uartWinApp.c
 *
 *  Created on: May 26, 2022
 *      Author: Admin
 */

#include "uartWinnApp.h"

uint8_t WinApp_RecBuff[300];
uint8_t sub_flag = OFF;

extern QueueHandle_t qPrintQueue;
extern QueueHandle_t qBC66step;
extern uint8_t event;

sUartWinApp UART_REC_WIN[]=
{
    { _APPZONE_SIG_,       {(uint8_t*)"appzone", 7 },        fncAppZoneHandle },
	{ _ID_SIG_,            {(uint8_t*)"id", 2},              fncIdHandle},
//	{ _ADDRESS_SIG_,       {(uint8_t*)"address", 7},         fncAddressHandle },
	{ _DATA_SIG_,          {(uint8_t*)"data", 4},            fncDataHandle},
	{ _END_SIG_,           {(uint8_t*)"end", 3},             fncEndHandle},
};

sWinAppRec WinUart = {
	.data = 0,
	.pData = WinApp_RecBuff,
    .numb_rec = 0,
	.pre_numb_rec = 0,
	.max_element = 300,
};

sType_string uart_rec_WinApp = {
		.pData = WinApp_RecBuff,
		.u16_len_data = 0,
};

//static sType_string Ready_String = {
//      .pData = (uint8_t*)"da nhan duoc appzone signal\n",
//	  .u16_len_data = 19,
//};

sType_string Connect_String = {
      .pData = (uint8_t*)"Connect Success\n",
	  .u16_len_data = 16,
};
/*===============================================================================*/

/**
  * @brief   Check time out of event
  * @param   mark:  time that start check time out
  *          cycle: time that event time out from mark time
  * @retval  TRUE:  time out happend
  *          FALSE: time out not happend
*/
uint8_t WinApp_Check_Time_Out ( uint32_t mark , uint32_t cycle )
{
	uint32_t delta = 0;
	delta = ui32_tick_count - mark;
	if ( delta >= cycle)
		return TRUE;
	else
		return FALSE;
}

uint8_t fncAppZoneHandle(void)
{
//	Push_Message_to_Queue( &Ready_String );
	uint8_t AppZoneArray[7] = {0x00};
	WinApp_Find_Extract_Signal_To_An_Array(AppZoneArray, 7, _APPZONE_SIG_);
	MQTT_Create_Message_To_Pub (AppZoneArray, 7, 0, 1, 0, 95, test_sub_array);
	while (dType_water_NB_IoT.dType_AT.ui8_pointer != _IDLE_)
	{
		osDelay(1);
	}
//    event = _MQTT_PUB_1_;
//    xQueueSendToBack( qBC66step, &event, 0 );
        dType_water_NB_IoT.dType_AT.ui8_pointer = _MQTT_PUB_1_;
	return 1;
}

uint8_t fncIdHandle (void)
{
    uint8_t IdArray[7] = {0x00};
    WinApp_Find_Extract_Signal_To_An_Array(IdArray, 7, _ID_SIG_);
    MQTT_Create_Topic_To_Sub (IdArray, 7, 0, 98, 1);
    while (dType_water_NB_IoT.dType_AT.ui8_pointer != _IDLE_)
    {
    	osDelay(1);
    }
//    event = _MQTT_SUBSCRIBE_1_;
//    xQueueSendToBack( qBC66step, &event, 0 );
    dType_water_NB_IoT.dType_AT.ui8_pointer = _MQTT_SUBSCRIBE_1_;
    sub_flag= ON;
    return 1;
};

uint8_t fncAddressHandle (void)
{
	return 1;
};

uint8_t fncDataHandle (void)
{
	uint8_t lengthArray = 0;
	uint8_t ArrayPub[12] = {0x00};
    WinApp_Find_Extract_Data_Signal_To_An_Array (ArrayPub);
    lengthArray =  strlen ((char*)ArrayPub);
    MQTT_Create_Message_To_Pub (ArrayPub, lengthArray, 0, 1, 0, 99, test_sub_array);
    while (dType_water_NB_IoT.dType_AT.ui8_pointer != _IDLE_)
    {
    	osDelay(1);
    }
//    event = _MQTT_PUB_1_;
//    xQueueSendToBack( qBC66step, &event, 0 );
     dType_water_NB_IoT.dType_AT.ui8_pointer = _MQTT_PUB_1_;
	return 1;
};

uint8_t fncEndHandle (void)
{
	return 1;
};

uint8_t WinApp_UART_Check_Receive_Process_Status (void)
{
	if ( WinUart.numb_rec != 0 )
	{
		if ( WinUart.numb_rec == WinUart.pre_numb_rec )
			return DONE;
		else if ( WinUart.numb_rec != WinUart.pre_numb_rec )
		{
			WinUart.pre_numb_rec = WinUart.numb_rec;
			return NOT_DONE;
		}
	}
	else if (WinUart.numb_rec == 0)
	{
		return NONE;
	}
}

uint8_t WinApp_Reset(void)
{
	Reset_Buffer ( WinUart.pData, WinUart.max_element );
	WinUart.numb_rec = 0;
	WinUart.pre_numb_rec = 0;
	return 1;
}

uint8_t WinApp_Handle_UART_Receive(void)
{
	for (uint8_t i = _APPZONE_SIG_; i <= _END_SIG_; i++)
	{
		if (Search_String_In_Buffer (WinUart.pData, WinUart.numb_rec,
		                             UART_REC_WIN[i].ReceiveString.pData,
									 UART_REC_WIN[i].ReceiveString.u16_len_data) == TRUE)
		{
			UART_REC_WIN[i].uartWinAppHandle();
		}
	}
	return 1;
}

void Convert_length_to_array (uint8_t *array, uint16_t ui16array_lenght)
{
    uint8_t fiction_array[5] = {0x00};
    uint16_t length = 0;
    length = strlen((char*)array);
    strcpy ((char*)fiction_array, (char*)array);
    for (uint8_t i = 0; i < ui16array_lenght; i++)
    {
        *(array + i) = '0';
    }
    for (uint8_t i = 0; i < length; i++)
    {
        *(array + ui16array_lenght - 1 - i) = *(fiction_array + length -1 - i);
    }
}

uint8_t Push_BC66_Message_to_Queue (void)
{
	bc66_rec_UART.u16_len_data = CountReceive_u16;
	xQueueSendToBack( qPrintQueue, &bc66_rec_UART, 0 );
	return 1;
}

uint8_t Push_WinApp_Message_to_Queue (void)
{
	uart_rec_WinApp.u16_len_data = WinUart.numb_rec;
	xQueueSendToBack( qPrintQueue, &uart_rec_WinApp, 0 );
	return 1;
}

uint8_t Push_Message_to_Queue (sType_string *Message)
{
    xQueueSendToBack( qPrintQueue, Message, 0);
	return 1;
}
//( uint8_t *Data , uint16_t data_length )
void UART_Send_Data_To_Terminal ( sType_string *Message )
{
	uint8_t length_array[5] = {0x00};
	itoa ( Message->u16_len_data, (char*)length_array, 10);
	Convert_length_to_array ((uint8_t*)length_array, 5);
	HAL_UART_Transmit (&uart_debug, length_array, 5, 500);
	HAL_UART_Transmit (&uart_debug, Message->pData, Message->u16_len_data, 500);
}


uint8_t WinApp_Find_Extract_Signal_To_An_Array (uint8_t *array, uint16_t numb_elem, uint8_t signal)
{
    uint8_t *pointer = NULL;
    pointer = Search_String_Location_In_Buffer_2(WinUart.pData, WinUart.numb_rec,
    		                                   UART_REC_WIN[signal].ReceiveString.pData,
											   UART_REC_WIN[signal].ReceiveString.u16_len_data);
    if (pointer == NULL)
    	return 0;
    else
    {
        for (uint8_t i = 0; i < numb_elem; i++)
        {
    	    *(array + i) = *(pointer + i);
        }
    }
	return 1;
}

/*
 *
 * @param array pointer point to array that store data will be publish through MQTT
 */
uint8_t WinApp_Find_Extract_Data_Signal_To_An_Array (uint8_t *array)
{
	int data_length = 0;
	uint8_t data_length_array[3] = {0x00};

    uint8_t *pointer = NULL;
    pointer = Search_String_Location_In_Buffer_2(WinUart.pData, WinUart.numb_rec,
    		                                   UART_REC_WIN[_DATA_SIG_].ReceiveString.pData,
											   UART_REC_WIN[_DATA_SIG_].ReceiveString.u16_len_data);
    if (pointer == NULL)
    	return 0;
    else
    {
        pointer = pointer + 4; // "data" co 4 ki tu
        for (uint8_t i = 0; i < 3; i++)
        {
    	    *(data_length_array + i) = *(pointer + i);
        }
    	data_length = atoi ((char*)data_length_array);
    	pointer = pointer + 3;
    	for (int i = 0; i < data_length; i++)
    	{
    		 *(array + i) = *(pointer + i);
    	}
    }
	return 1;
}








