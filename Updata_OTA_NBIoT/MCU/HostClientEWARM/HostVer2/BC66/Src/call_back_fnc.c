#include "main.h"														
#include "string.h"
#include "stdlib.h"
#include "cmsis_os.h"
#include "pin_config.h"
#include "my_lib.h"
#include "MQTTPacket.h"
#include "MQTTFormat.h"
#include "MQTT.h"
#include "bc66_uart.h"
#include "bc66_cmd.h" 
#include "call_back_fnc.h"

/*====================================================================================================================*/


uint8_t ui8_sub_dup = 0;
uint8_t ui8_Buf[300];
uint16_t ui16_Buflen = 300;

extern sType_con_opt BC66_con_opt[];

extern uint8_t deba;
/*========================================================================================================================================================*/

/**
  * @brief   Call back function of _TCP_OPEN_1_ process
  * @param   NULL
  * @retval  1 function OK
             0 function FAIL
*/
uint8_t FncCallback_TCP_Open_1 ( void )
{
  uint8_t ui8Arr_trans[100] = { 0x00 };
	strcat ( (char*)ui8Arr_trans , "\"" );
	strcat ( (char*)ui8Arr_trans , (char*)BC66_con_opt[_PROTOCOL_].choosen_type.pData );
	strcat ( (char*)ui8Arr_trans , "\"," );
	strcat ( (char*)ui8Arr_trans , "\"" );
	strcat ( (char*)ui8Arr_trans , (char*)BC66_con_opt[_SERVER_].choosen_type.pData );
	strcat ( (char*)ui8Arr_trans , "\"," );
	strcat ( (char*)ui8Arr_trans , (char*)BC66_con_opt[_PORT_].choosen_type.pData );
	strcat ( (char*)ui8Arr_trans , "," );
	BC66_UART_Send_Data_To_BC66 ( (uint8_t*)ui8Arr_trans , strlen ((char*)ui8Arr_trans) );
	return 1;
}

/**
  * @brief   Call back function of _TCP_OPEN_2_ process
  * @param   NULL
  * @retval  1 function OK
             0 function FAIL
*/
uint8_t FncCallback_TCP_Open_2 ( void )
{
	uint16_t ui16_err_code = 0;
	uint8_t ui8_buf_check_TCP[30] = {0x00};
	uint8_t *ui8Ptr_buf = &ui8_buf_check_TCP[0];
	uint8_t *ui8Ptr = NULL;
	ui8Ptr = (uint8_t*)strstr ( (char*)dType_water_NB_IoT.dType_bc66_receive.ui8buf_rx , "+QIOPEN: 0,");
	ui8Ptr = ui8Ptr + 11;
	while ( *(ui8Ptr) != 0x0D )
	{
		*ui8Ptr_buf = *ui8Ptr;
		ui8Ptr_buf++;
		ui8Ptr++;
	}
	ui16_err_code = atoi ( (char*)ui8_buf_check_TCP );
	if ( ui16_err_code == 0 )
	{
		return 1;
	}
	else 
		return 0;
}

/**
  * @brief   Call back function of _MQTT_OPEN_1_ process
  * @param   NULL
  * @retval  1 function OK
             0 function FAIL
*/
uint8_t FncCallback_MQTT_Open_1 ( void )
{
	//uint8_t ui8_send_result = 0;
	uint16_t ui16_test_length = 0;
	ui16_test_length = MQTTSerialize_connect ( ui8_Buf , ui16_Buflen , &test_conect );
        BC66_UART_Send_Data_To_BC66 ( ui8_Buf , ui16_test_length );
	osDelay (100);
	return 1;
}

/**
  * @brief   Call back function of _MQTT_PUB_1_ process
  * @param   NULL
  * @retval  1 function OK
             0 function FAIL
*/
uint32_t ui32_time_send = 0;
uint32_t ui32_send_cplt = 0;
uint8_t FncCallback_MQTT_Pub_1 (void)
{
//	uint8_t ui8Arr_Payload[20] = {0x00};
//	uint16_t ui16_Arr_len = 20;
	uint16_t ui16_serialize_len = 0;
//	ui32_time_send++;
	
	Reset_Buffer ( ui8_Buf , ui16_Buflen );
//	Interger_To_Char ( ui32_send_cplt , ui8Arr_Payload , ui16_Arr_len );
//
//	test_pub_mes.buf = ui8_Buf;
//	test_pub_mes.buflen = ui16_Buflen;
//	test_pub_mes.qos = 1;
//	test_pub_mes.retained = 0;
//	test_pub_mes.packetid = 39;
//	test_pub_mes.topicName.cstring = (char*)"0866495609";
//	test_pub_mes.payload = (unsigned char*)ui8Arr_Payload;
//	test_pub_mes.payloadlen = strlen( (char*)ui8Arr_Payload );
	
    ui16_serialize_len = MQTTSerialize_publish (ui8_Buf , ui16_Buflen, test_pub_mes.dup, test_pub_mes.qos, test_pub_mes.retained,
	                                            test_pub_mes.packetid, test_pub_mes.topicName, test_pub_mes.payload, test_pub_mes.payloadlen);
	BC66_UART_Send_Data_To_BC66 ( ui8_Buf , ui16_serialize_len );
	osDelay (100);
	return 1;
}
/**
  * @brief   Call back function of _MQTT_PUB_2_ process
  * @param   NULL
  * @retval  1 function OK
             0 function FAIL
*/
uint8_t FncCallback_MQTT_Pub_2 (void)
{
	uint8_t ui8_response_receive = 0;
	ui8_response_receive = Search_String_In_Buffer ( (uint8_t*)dType_water_NB_IoT.dType_bc66_receive.ui8buf_rx , CountReceive_u16 , (uint8_t*)"4002" , 4 );
	if ( ui8_response_receive != FALSE )
		return 1;
	else 
		return 0;
}

/**
  * @brief   Call back function of _MQTT_PING_1_ process
  * @param   NULL
  * @retval  1 function OK
             0 function FAIL
*/
uint8_t FncCallback_MQTT_Ping_1 (void)
{
	uint16_t ui16_serialize_len = 0;
	
	Reset_Buffer ( ui8_Buf , ui16_Buflen );
	ui16_serialize_len = MQTTSerialize_pingreq ( ui8_Buf , ui16_Buflen );
	BC66_UART_Send_Data_To_BC66 ( ui8_Buf , ui16_serialize_len );
	osDelay (100);
	return 1;
}

/**
  * @brief   Call back function of _MQTT_PING_2_ process
  * @param   NULL
  * @retval  1 function OK
             0 function FAIL
*/
uint8_t FncCallback_MQTT_Ping_2 (void)
{
	uint8_t ui8_response_receive = 0;
	ui8_response_receive = Search_String_In_Buffer ( (uint8_t*)dType_water_NB_IoT.dType_bc66_receive.ui8buf_rx , CountReceive_u16 , (uint8_t*)"2,D000" , 6 );
	if ( ui8_response_receive == TRUE )
		return 1;
	else 
		return 0;
}

/**
  * @brief   function that send debug information of MQTT_PUB to Terminal
  * @param   NULL
  * @retval  NULL
*/
void Send_Debug_Infor (void)
{
	uint8_t ui8Arr_nothing[30] = {0x00};
	uint8_t ui8Arr_debugString[100] = "so lan gui: ";
	uint16_t ui16_SendLen = 0; 
	
	Interger_To_Char ( ui32_time_send , ui8Arr_nothing , 30 );
	strcat ( (char*)ui8Arr_debugString , (char*)ui8Arr_nothing );
	
	Reset_Buffer ( ui8Arr_nothing , 30 );
	strcat ( (char*)ui8Arr_debugString , (char*)", so lan gui thanh cong: " );
	Interger_To_Char ( ui32_send_cplt , ui8Arr_nothing , 30 );
	strcat ( (char*)ui8Arr_debugString , (char*)ui8Arr_nothing );
	strcat ( (char*)ui8Arr_debugString , "\r\n" );
	ui16_SendLen = strlen ( (char*)ui8Arr_debugString );
    BC66_UART_Send_Data_To_Terminal ( (uint8_t*)ui8Arr_debugString , ui16_SendLen );
}

/**
  * @brief   Call back function of _MQTT_SUBSCRIBE_1_ process
  * @param   NULL
  * @retval  1 function OK
             0 function FAIL
*/
uint8_t FncCallback_MQTT_Sub_1 (void)
{
	uint16_t ui16_serialize_len = 0;
	Reset_Buffer ( ui8_Buf , ui16_Buflen );
	ui16_serialize_len = MQTTSerialize_subscribe (ui8_Buf, ui16_Buflen, test_MQTT_sub.dup,
			                                      test_MQTT_sub.packetid, test_MQTT_sub.count,
												  test_MQTT_sub.topicFilters, test_MQTT_sub.requestedQoS);
	BC66_UART_Send_Data_To_BC66 ( ui8_Buf , ui16_serialize_len );
	osDelay (100);
	return 1;
}

/**
  * @brief   Call back function of _MQTT_SUBSCRIBE_2_ process
  * @param   NULL
  * @retval  1 function OK
             0 function FAIL
*/
uint8_t FncCallback_MQTT_Sub_2 (void)
{
  uint8_t Sub_Ret[10] = "900300";
  uint8_t SubPacket_id[2] = {0x00};

  uint8_t ui8_response_receive = 0;
  Interger_To_Hex(test_MQTT_sub.packetid, SubPacket_id);
  strcat ((char*)Sub_Ret, (char*)SubPacket_id);
  strcat ((char*)Sub_Ret, (char*)"01");

  ui8_response_receive = Search_String_In_Buffer ( (uint8_t*)dType_water_NB_IoT.dType_bc66_receive.ui8buf_rx , CountReceive_u16 , (uint8_t*)Sub_Ret , 10 );
	if ( ui8_response_receive == TRUE )
		return 1;
	else 
		return 0;
}

/*====================================================================================================================*/
