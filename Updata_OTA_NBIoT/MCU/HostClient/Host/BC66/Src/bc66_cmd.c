/**
  * @brief   This file was created to define all function to contact with BC66 module.
	*          User can easily apply or change to fit their application
	* @creator Nguyen Binh Duong _ R&D Staff _ Sao Viet company
	* @data    7/12/2021
*/
/*-----------------------------------------------------------------------------------------*/
                            /* include area */
#include "main.h"														
#include "string.h"
#include "cmsis_os.h"
#include "pin_config.h"
#include "my_lib.h"
#include "MQTTPacket.h"
#include "MQTTFormat.h"
#include "MQTT.h"
#include "bc66_uart.h"
#include "bc66_cmd.h"
#include "call_back_fnc.h"
#include "queue.h"
#include "my_freeRTOS.h"
/*-----------------------------------------------------------------------------------------*/
                              /* variable decalaration area */

volatile uint32_t ui32_tick_count;

const uint8_t deba=0x1A;

sType_BC66 dType_water_NB_IoT = 
{
	.dType_bc66_reset.ui8_fail_time = 0,
	.dType_bc66_reset.ui8_reset_flag = 0,
	.dType_bc66_reset.ui8_reset_rx_buf_flag = YES,
	
	.dType_AT.ui8_pointer = _PWR_ON_,
	.dType_AT.ui8_result = NOT_MATCH,
	
	.dType_AT.dType_tcp.ui8_ena_send = FALSE,
	.dType_AT.dType_tcp.ui8_send_done = FALSE
};

const sType_reason_close dType_TCP_close_reason[] =
{
	{ _MQTT_SEND_SUCCESS_      , MQTT_SEND_SUCCESS       , BC66_Handler_TCP_Close_None_Err },
	{ _MQTT_OPEN_FAIL_         , MQTT_MAX_OPEN_FAIL      , BC66_Handler_TCP_Close_MQTT_Max_Open_Fail },
    { _MQTT_PUB_FAIL_          , MQTT_MAX_PUB_FAIL       , BC66_Handler_TCP_Close_MQTT_Max_Pub_Fail },
	{ _MQTT_PING_FAIL_         , MQTT_MAX_PING_FAIL      , BC66_Handler_TCP_Close_MQTT_Max_Ping_Fail },
	{ _TCP_OPEN_FAIL_          , TCP_MAX_OPEN_FAIL       , BC66_Handler_TCP_Close_TCP_Max_Open_Fail },
}
;
/**
  * @brief open TCP protocol
*/
//sType_string test_protcol = 
//{
//	.pData = (uint8_t*)"TCP",
//	.u16_len_data = 3,
//};

//sType_string test_server =
//{
//  .pData = (uint8_t*)"test.mosquitto.org",
//  .u16_len_data = 18,
//};

//sType_string test_port = 
//{
//	.pData = (uint8_t*)"1883",
//	.u16_len_data = 4,
//};

sType_con_opt BC66_con_opt[] =
{
	{ _PROTOCOL_ , { (uint8_t*)"TCP" , 3 } },
	{ _SERVER_   , { (uint8_t*)"test.mosquitto.org" , 18 } },
	{ _PORT_     , { (uint8_t*)"1883" , 4 } },
};

/*-----------------------------------------------------------------------------------------*/
                             /* constant declaration area */

const sType_AT_struct dTypeArr_set_AT[] =
{
 // initialize sequence BC66
 { _PWR_ON_                    ,    { NULL }                                                                 , { NULL }                                            , NULL }, 	
 { _SYN_BAUDRATE_BC66_         ,    { (uint8_t*)"AT\r" , 3 , 5000 }                                          , { (uint8_t*)"OK" , 2 , 0 }                          , NULL },
 { _CHECK_PIN_BC66_            ,    { (uint8_t*)"AT+CPIN?\r" , 9 , 5000 }                                    , { (uint8_t*)"+CPIN: READY" , 12 , 0 }               , NULL },
 { _SET_NB_BAND_BC66_          ,    { (uint8_t*)"AT+QBAND=1,3\r" , 13 , 5000 }                               , { (uint8_t*)"OK" , 2 , 0 }                          , NULL },
 { _OFF_SLEEP_BC66_            ,    { (uint8_t*)"AT+QSCLK=0\r" , 12 , 5000 }                                 , { (uint8_t*)"OK" , 2 , 0 }                          , NULL },
 { _CHECK_SIM_ATTACH_BC66_     ,    { (uint8_t*)"AT+CGATT?\r" , 10 , 12000 }                                , { (uint8_t*)"+CGATT: 1" , 9 , 0 }                   , NULL },
 { _SET_PSD_CONECTION_         ,    { (uint8_t*)"AT+QCGDEFCONT=\"IP\",\"nbiot\"\r" , 27 , 5000 }             , { (uint8_t*)"OK" , 2 , 0 }                          , NULL },
 { _FREE_LOCK_NB_FREQUENCY_    ,    { (uint8_t*)"AT+QLOCKF=0\r" , 12 , 5000}                                 , { (uint8_t*)"OK" , 2 , 0 }                          , NULL },
 { _ENABLE_WAKEUP_INDICATION_  ,    { (uint8_t*)"AT+QATWAKEUP=1\r" , 15 , 5000}                              , { (uint8_t*)"OK" , 2 , 0 }                          , NULL },
 { _OFF_eDRX_                  ,    { (uint8_t*)"AT+CEDRXS=0\r" , 12 , 5000 }                                , { (uint8_t*)"OK" , 2 , 0 }                          , NULL },
 { _SAVE_NVRAM_                ,    { (uint8_t*)"AT&W0\r" , 6 , 5000 }                                       , { (uint8_t*)"OK" , 2 , 0 }                          , NULL },
 
 // not use yet
 { _DISPLAY_CELLS_INFOR_BC66_  ,    { (uint8_t*)"AT+QENG=0\r" , 10 , 5000 }                                  , { (uint8_t*)"OK" , 2 , 0 }                          , NULL }, 
 { _CHECK_IP_ADDRESS_          ,    { (uint8_t*)"AT+QIPADDR\r" , 11 , 10000 }                                , { (uint8_t*)"OK" , 2 , 0 }                          , NULL },
 
 // TCP setting
 { _TCP_SET_DATA_TEXT_FORMAT_  ,    { (uint8_t*)"AT+QICFG=\"dataformat\",0,1\r" , 26 , 5000 }                , { (uint8_t*)"OK" , 2 , 0 }                          , NULL },
 { _TCP_SET_VIEW_MODE_         ,    { (uint8_t*)"AT+QICFG=\"viewmode\",1\r" , 22 , 5000 }                    , { (uint8_t*)"OK" , 2 , 0 }                          , NULL },
 { _TCP_SET_SHOW_LENGTH_       ,    { (uint8_t*)"AT+QICFG=\"showlength\",0\r" , 24 , 5000 }                  , { (uint8_t*)"OK" , 2 , 0 }                          , NULL },
 
 // TCP operate
 { _TCP_OPEN_1_                ,    { (uint8_t*)"AT+QIOPEN=1,0," , 14 , 5000}                                , { NULL }                                            , FncCallback_TCP_Open_1 },
 { _TCP_OPEN_2_                ,    { (uint8_t*)"0,1\r" , 4 , 800000 }                                       , { (uint8_t*)"+QIOPEN: 0," , 11 , 0 }                , FncCallback_TCP_Open_2 },
 
 { _TCP_CLOSE_                 ,    { (uint8_t*)"AT+QICLOSE=0\r" , 13 , 5000 }                               , { (uint8_t*)"CLOSE OK" , 8 , 0 }                    , NULL },
 
 
 // MQTT 
 { _MQTT_OPEN_1_               ,    { (uint8_t*)"AT+QISEND=0\r" , 12 , 5000 }                                , { (uint8_t*)">" , 1 , 0 }                           , FncCallback_MQTT_Open_1 },
 { _MQTT_OPEN_2_               ,    { (uint8_t*)&deba , 1 , 60000 }                                          , { (uint8_t*)"4,20020000" , 10 , 0 }                 , NULL },
 
 { _MQTT_PUB_1_                ,    { (uint8_t*)"AT+QISEND=0\r" , 12 , 5000 }                                , { (uint8_t*)">" , 1 , 0 }                           , FncCallback_MQTT_Pub_1 },
 { _MQTT_PUB_2_                ,    { (uint8_t*)&deba , 1 , 60000 }                                          , { (uint8_t*)"+QIURC:" , 7 , 0 }                     , FncCallback_MQTT_Pub_2 },
 
 { _MQTT_PINGREQ_1_            ,    { (uint8_t*)"AT+QISEND=0\r" , 12 , 5000 }                                , { (uint8_t*)">" , 1 , 0 }                           , FncCallback_MQTT_Ping_1 },
 { _MQTT_PINGREQ_2_            ,    { (uint8_t*)&deba , 1 , 30000 }                                          , { (uint8_t*)"+QIURC: \"recv\",0,2,D000" , 23 , 0 }  , NULL },
 
 { _MQTT_SUBSCRIBE_1_          ,    { (uint8_t*)"AT+QISEND=0\r" , 12 , 5000 }                                , { (uint8_t*)">" , 1 , 0 }                           , FncCallback_MQTT_Sub_1 },
 { _MQTT_SUBSCRIBE_2_          ,    { (uint8_t*)&deba , 1 , 60000 }                                          , { (uint8_t*)"+QIURC:" , 7 , 0 }                     , FncCallback_MQTT_Sub_2 },

 
 // Check error
 { _GET_ERROR_                 ,    { (uint8_t*)"AT+QIGETERROR\r" , 14 , 5000 }                              , { NULL }                                            , NULL },

 
 // Setting PSM parametters
 { _REQUEST_PSM_NETWORK_       ,    { (uint8_t*)"AT+CEREG=5\r" , 11 , 1000 }                                 , { (uint8_t*)"OK" , 2 , 0 }                          , NULL },
 { _CONFIG_SLEEP_MODE_         ,    { (uint8_t*)"AT+QSCLK=1\r" , 11 , 1000 }                                 , { (uint8_t*)"OK" , 2 , 0 }                          , NULL },
 { _SETTING_PSM_               ,    { (uint8_t*)"AT+CPSMS=1,\"00111100\",\"00000001\"\r" , 33 , 1000 }       , { (uint8_t*)"OK" , 2 , 0 }                          , NULL },
 { _CHECK_PSM_REGISTRATION_    ,    { (uint8_t*)"AT+CEREG?\r" , 10 ,1000 }                                   , { (uint8_t*)"+CEREG: 5,1" , 11 , 0 }                , NULL },
 
 
};
	

extern uint8_t ui8_sub_dup;

extern uint8_t event;
extern QueueHandle_t qBC66step;
extern uint8_t ping_flag;
extern BaseType_t xStatus;
/*-----------------------------------------------------------------------------------------*/
                             /* defination of functions */

/**
  * @brief   Excute AT command and return response that exactly as requested or not
  * @param   AT_command      name or number of AT command in set of AT commands ( dTypeArr_set_AT[] )
  * @retval  0  reponse AT command fail
             1  response AT sucess
*/
uint8_t BC66_Excute_AT_Cmd ( uint8_t AT_command )
{
	uint32_t time_mark = 0;
	//uint8_t ui8_call_back_result = 0;
	dType_water_NB_IoT.dType_AT.ui8_result = NOT_MATCH;
    uint8_t ui8_fnc_result = 0;
	
	if ( dTypeArr_set_AT[AT_command].dType_AT_req.data != NULL )
	 BC66_UART_Send_Data_To_BC66 ( dTypeArr_set_AT[AT_command].dType_AT_req.data , dTypeArr_set_AT[AT_command].dType_AT_req.ui16_data_len );
	 
	// ko chuoi check	 
  if (dTypeArr_set_AT[AT_command].dType_AT_res.data == NULL )
  {
	 if ( dTypeArr_set_AT[AT_command].FUNC != NULL )
	   return dTypeArr_set_AT[AT_command].FUNC();
	}
	
	// bat dau tinh thoi gian phan hoi
  time_mark = ui32_tick_count; 
	
  // doi phan hoi 
	do
	{
   if ( BC66_Check_Time_Out( time_mark , dTypeArr_set_AT[AT_command].dType_AT_req.ui32_time_res ) == TRUE )
	   break;
		 osDelay (5);
	} while ( dType_water_NB_IoT.dType_AT.ui8_result != MATCH );
  ui8_fnc_result = dType_water_NB_IoT.dType_AT.ui8_result;
	
  // ket qua dung mong muon
	if ( dType_water_NB_IoT.dType_AT.ui8_result == MATCH )
	{
	  if ( dTypeArr_set_AT[AT_command].FUNC != NULL ) 
      ui8_fnc_result = dTypeArr_set_AT[AT_command].FUNC();	
	}
  return ui8_fnc_result;	 
}


/**
  * @brief   Check time out of event
  * @param   mark:  time that start check time out
  *          cycle: time that event time out from mark time
  * @retval  TRUE:  time out happend
  *          FALSE: time out not happend
*/
uint8_t BC66_Check_Time_Out ( uint32_t mark , uint32_t cycle )
{
	uint32_t delta;
	delta = ui32_tick_count - mark;
	if ( delta >= cycle)
		return TRUE;
	else 
		return FALSE;
}

/**
  * @brief   Check AT resposne of AT command 
  * @param   AT_command  number or name of AT command in set of AT commands ( dTypeArr_set_AT[] )
  * @retval  MATCH       response match with required result 
             NOT_MATCH   response not match with required result
*/
uint8_t BC66_Check_AT_Response ( uint8_t AT_command )
{
	if ( Search_String_In_Buffer ( (uint8_t*)dType_water_NB_IoT.dType_bc66_receive.ui8buf_rx , CountReceive_u16 ,
		                                    dTypeArr_set_AT[AT_command].dType_AT_res.data , dTypeArr_set_AT[AT_command].dType_AT_res.ui16_data_len ) != FALSE )
		return MATCH;
	else
		return NOT_MATCH;
}

/**
  * @brief   Check process to determine need to reset or not
  * @param   NULL
  * @retval  NULL
*/
void BC66_Check_Reset ( void )
{
	switch ( dType_water_NB_IoT.dType_bc66_reset.ui8_reset_flag )
	{
		case OFF:
			if ( dType_water_NB_IoT.dType_bc66_reset.ui8_fail_time == MAX_FAIL_TIME )
			{
				BC66_UART_Clear_BC66_Data ();
				dType_water_NB_IoT.dType_bc66_reset.ui8_fail_time = 0;
				dType_water_NB_IoT.dType_AT.ui8_pointer = _SYN_BAUDRATE_BC66_;
				
				dType_MQTT_watermetter.dType_MQTT_fail.ui8_ping = 0;
				dType_MQTT_watermetter.dType_MQTT_fail.ui8_open = 0;
				dType_MQTT_watermetter.dType_MQTT_fail.ui8_pub = 0;
				dType_MQTT_watermetter.ui8_start_keepalive = OFF;
				dType_MQTT_watermetter.ui8_prev_MQTT_step = 0;
				BC66_Reset();
			}
			break;
		case ON:
			BC66_UART_Clear_BC66_Data ();
		    dType_water_NB_IoT.dType_bc66_reset.ui8_fail_time = 0;
            dType_water_NB_IoT.dType_bc66_reset.ui8_reset_flag = OFF;
            dType_water_NB_IoT.dType_AT.ui8_pointer = _SYN_BAUDRATE_BC66_;
            BC66_Reset();
			break;
	}
}

/**
  * @brief   This function was created to reset BC66
  * @param   NULL
  * @retval  NULL
*/
void BC66_Reset ( void )
{
	BC66_RESET_DOWN;
	HAL_Delay ( RESET_KEEP_TIME );
	BC66_RESET_UP;
	HAL_Delay ( 2000 );
}

/**
  * @brief   This function was created to power on module BC66
  * @param   NULL
  * @retval  NULL
*/
void BC66_Power_On ( void )
{
	if ( dType_water_NB_IoT.dType_AT.ui8_pointer == _PWR_ON_ )
	{
	 BC66_PWR_KEY_DOWN;
	 HAL_Delay ( POWER_ON_KEEP_TIME );
	 BC66_PWR_KEY_UP;
	 BC66_Reset ();
	 HAL_Delay ( 2000 );
	 //dType_water_NB_IoT.dType_AT.ui8_pointer = _CHECK_PIN_BC66_;
     event = _SYN_BAUDRATE_BC66_;
	 xQueueSendToBack(qBC66step, &event, 0);
	}
}

/**
  * @brief   This function was created to handle AT commands in initialize BC66 section
  * @param   current_cmd: current AT command
  * @param   next_cmd: next AT command
  * @retval  NULL
*/
extern uint32_t ui32_send_cplt;
void BC66_Initialize_AT_Commands_Handler ( uint8_t current_cmd , uint8_t next_cmd )
{
  dType_water_NB_IoT.dType_bc66_reset.ui8_reset_rx_buf_flag = NO;
  switch ( BC66_Excute_AT_Cmd ( current_cmd ) )
  {
    case BC66_RESP_OK:
    	  event = next_cmd;
    	  xStatus = xQueueSendToBack (qBC66step, &event, 0);
    	  if (xStatus != pdPASS)
    	  {
    		  BC66_UART_Send_Data_To_Terminal ((uint8_t*)"loi push\r\n", 10);
    	  }
		  //dType_water_NB_IoT.dType_AT.ui8_pointer = next_cmd;
		  dType_water_NB_IoT.dType_bc66_reset.ui8_fail_time = 0;
		  switch ( dType_water_NB_IoT.dType_AT.ui8_pointer )
			{
				case _MQTT_OPEN_2_:
				  dType_MQTT_watermetter.ui8_start_keepalive = ON;
				  dType_MQTT_watermetter.ui32_keep_alive_mark_time = ui32_tick_count;
				  dType_MQTT_watermetter.ui32_keep_alive_time = 50000; // correct is 50000 this is just for test
				  dType_MQTT_watermetter.dType_MQTT_fail.ui8_open = 0;

				  ui8_sub_dup = 0;
				  ping_flag = OFF;
				  break;
				case _MQTT_PUB_2_:
				  test_pub_mes.dup = 0;
				  dType_MQTT_watermetter.ui32_keep_alive_mark_time = ui32_tick_count;
//				  ui32_send_cplt++;
				  dType_MQTT_watermetter.dType_MQTT_fail.ui8_pub = 0;
//				  Send_Debug_Infor();
					break;
				case _MQTT_PINGREQ_2_:
//					if ( dType_MQTT_watermetter.ui8_prev_MQTT_step != 0 )
//					{
//				    dType_water_NB_IoT.dType_AT.ui8_pointer = dType_MQTT_watermetter.ui8_prev_MQTT_step;
//		            }
					dType_MQTT_watermetter.ui32_keep_alive_mark_time = ui32_tick_count;
					ping_flag = OFF;
					break;
			}
			break;
		case BC66_RESP_FAIL:
			dType_water_NB_IoT.dType_bc66_reset.ui8_fail_time++;
		    switch ( dType_water_NB_IoT.dType_AT.ui8_pointer )
			{
				case _TCP_OPEN_2_:
					if ( dType_water_NB_IoT.dType_bc66_reset.ui8_fail_time >= 3 )
					{
//					  dType_water_NB_IoT.dType_AT.ui8_pointer = _TCP_CLOSE_;
			    	  event = _TCP_CLOSE_;
			    	  xStatus = xQueueSendToBack( qBC66step, &event, 0 );
			    	  if (xStatus != pdPASS)
			    	  {
			    		  BC66_UART_Send_Data_To_Terminal ((uint8_t*)"loi push\r\n", 10);
			    	  }
					}
					else
					{
//					  dType_water_NB_IoT.dType_AT.ui8_pointer = _TCP_OPEN_1_
			    	  event = _TCP_OPEN_1_;
			    	  xStatus = xQueueSendToBack( qBC66step, &event, 0 );
			    	  if (xStatus != pdPASS)
			    	  {
			    		  BC66_UART_Send_Data_To_Terminal ((uint8_t*)"loi push\r\n", 10);
			    	  }
					}
				    break;
				case _MQTT_OPEN_2_:
				    dType_MQTT_watermetter.dType_MQTT_fail.ui8_open++;
//					dType_water_NB_IoT.dType_AT.ui8_pointer = _TCP_CLOSE_;
			        event = _TCP_CLOSE_;
			        xStatus = xQueueSendToBack( qBC66step, &event, 0 );
			        if (xStatus != pdPASS)
			        {
			      	  BC66_UART_Send_Data_To_Terminal ((uint8_t*)"loi push\r\n", 10);
			        }
					break;
				case _MQTT_PUB_1_:
					if ( Search_String_In_Buffer( (uint8_t*)dType_water_NB_IoT.dType_bc66_receive.ui8buf_rx , CountReceive_u16 , (uint8_t*)"closed" , 6 ) == TRUE )
					{
						dType_MQTT_watermetter.ui8_start_keepalive = OFF;
//						dType_water_NB_IoT.dType_AT.ui8_pointer = _TCP_OPEN_1_;
				    	  event = _TCP_OPEN_1_;
				    	  xStatus = xQueueSendToBack( qBC66step, &event, 0 );
				    	  if (xStatus != pdPASS)
				    	  {
				    		  BC66_UART_Send_Data_To_Terminal ((uint8_t*)"loi push\r\n", 10);
				    	  }
						dType_MQTT_watermetter.ui8_prev_MQTT_step = 0;
					}
					break;
				case _MQTT_PUB_2_:
					dType_MQTT_watermetter.dType_MQTT_fail.ui8_pub++;
				    test_pub_mes.dup = 1;
				    if ( dType_MQTT_watermetter.dType_MQTT_fail.ui8_pub == 3)
				    {
				    	  event = _TCP_CLOSE_;
				    	  xStatus = xQueueSendToBack( qBC66step, &event, 0 );
				    	  if (xStatus != pdPASS)
				    	  {
				    		  BC66_UART_Send_Data_To_Terminal ((uint8_t*)"loi push\r\n", 10);
				    	  }
				    }
//						dType_water_NB_IoT.dType_AT.ui8_pointer = _TCP_CLOSE_;
					else
					{
				    	  event = _MQTT_PUB_1_;
				    	  xStatus = xQueueSendToBack( qBC66step, &event, 0 );
				    	  if (xStatus != pdPASS)
				    	  {
				    		  BC66_UART_Send_Data_To_Terminal ((uint8_t*)"loi push\r\n", 10);
				    	  }
//					  dType_water_NB_IoT.dType_AT.ui8_pointer = _MQTT_PUB_1_;
					}
					  dType_MQTT_watermetter.ui8_prev_MQTT_step = _MQTT_PUB_1_;
	                 // server test hay bi tu ngat ket noi them phan nay vao de xu ly viec tu ngat ket noi do
					if ( Search_String_In_Buffer( (uint8_t*)dType_water_NB_IoT.dType_bc66_receive.ui8buf_rx , CountReceive_u16 , (uint8_t*)"closed" , 6 ) == TRUE )
					{
						dType_MQTT_watermetter.ui8_start_keepalive = OFF;
//						dType_water_NB_IoT.dType_AT.ui8_pointer = _TCP_OPEN_1_;
				    	  event = _TCP_OPEN_1_;
				    	  xStatus = xQueueSendToBack( qBC66step, &event, 0 );
				    	  if (xStatus != pdPASS)
				    	  {
				    		  BC66_UART_Send_Data_To_Terminal ((uint8_t*)"loi push\r\n", 10);
				    	  }
						dType_MQTT_watermetter.ui8_prev_MQTT_step = 0;
					}
					break;
				case _MQTT_PINGREQ_2_:
					dType_MQTT_watermetter.dType_MQTT_fail.ui8_ping++;
//				    dType_water_NB_IoT.dType_AT.ui8_pointer = _TCP_CLOSE_;
			    	  event = _TCP_CLOSE_;
			    	  xStatus = xQueueSendToBack( qBC66step, &event, 0 );
			    	  if (xStatus != pdPASS)
			    	  {
			    		  BC66_UART_Send_Data_To_Terminal ((uint8_t*)"loi push\r\n", 10);
			    	  }
				    if ( Search_String_In_Buffer( (uint8_t*)dType_water_NB_IoT.dType_bc66_receive.ui8buf_rx , CountReceive_u16 , (uint8_t*)"closed" , 6 ) == TRUE )
					{
						dType_MQTT_watermetter.ui8_start_keepalive = OFF;
//						dType_water_NB_IoT.dType_AT.ui8_pointer = _TCP_OPEN_1_;
				    	  event = _TCP_OPEN_1_;
				    	  xStatus = xQueueSendToBack( qBC66step, &event, 0 );
				    	  if (xStatus != pdPASS)
				    	  {
				    		  BC66_UART_Send_Data_To_Terminal ((uint8_t*)"loi push\r\n", 10);
				    	  }
						dType_MQTT_watermetter.ui8_prev_MQTT_step = 0;
					}
					break;
				case _MQTT_SUBSCRIBE_2_:
//					dType_water_NB_IoT.dType_AT.ui8_pointer = _MQTT_SUBSCRIBE_1_;
			    	  event = _MQTT_SUBSCRIBE_1_;
			    	  xStatus = xQueueSendToBack( qBC66step, &event, 0 );
			    	  if (xStatus != pdPASS)
			    	  {
			    		  BC66_UART_Send_Data_To_Terminal ((uint8_t*)"loi push\r\n", 10);
			    	  }
					dType_MQTT_watermetter.ui8_prev_MQTT_step = _MQTT_SUBSCRIBE_1_;
	                // server test hay bi tu ngat ket noi them phan nay vao de xu ly viec tu ngat ket noi do
					if ( Search_String_In_Buffer( (uint8_t*)dType_water_NB_IoT.dType_bc66_receive.ui8buf_rx , CountReceive_u16 , (uint8_t*)"closed" , 6 ) == TRUE )
					{
						dType_MQTT_watermetter.ui8_start_keepalive = OFF;
//						dType_water_NB_IoT.dType_AT.ui8_pointer = _TCP_OPEN_1_;
				    	event = _TCP_OPEN_1_;
				    	xStatus = xQueueSendToBack( qBC66step, &event, 0 );
				    	if (xStatus != pdPASS)
				    	{
				    		BC66_UART_Send_Data_To_Terminal ((uint8_t*)"loi push\r\n", 10);
				    	}
						dType_MQTT_watermetter.ui8_prev_MQTT_step = 0;
					}
				    break;
				case _CHECK_SIM_ATTACH_BC66_:
//					dType_water_NB_IoT.dType_AT.ui8_pointer = _CHECK_SIM_ATTACH_BC66_;
			    	event = _CHECK_SIM_ATTACH_BC66_;
			    	  xStatus = xQueueSendToBack( qBC66step, &event, 0 );
			    	  if (xStatus != pdPASS)
			    	  {
			    		  BC66_UART_Send_Data_To_Terminal ((uint8_t*)"loi push\r\n", 10);
			    	  }
				    break;
			}
			break;
			
		default:
			BC66_UART_Send_Data_To_Terminal ( (uint8_t*)"nhay vao case khac\r\n" , sizeof ( (char*)"nhay vao case khac\r\n" ) );
		break;
	}
	dType_water_NB_IoT.dType_bc66_reset.ui8_reset_rx_buf_flag = YES;
}

/**
  * @brief   This function was created to send data through TCP/IP
  * @param   data point to data need to be sent
  * @retval  SUCCESS send process success
             FAIL send process fail
*/

uint8_t BC66_Send_Data_TCP_IP ( uint8_t *data , uint16_t len_data )
{
	BC66_UART_Send_Data_To_BC66 ( data , len_data );
	osDelay ( 100 );   //wait for data ATE receive completely 
	return 1;
}

/**
  * @brief   Check ready to send data or not 
  * @param   NULL
  * @retval  NULL
*/
uint8_t BC66_Check_Ready_To_Send_Data_TCP ( void )
{
	if ( strstr ( (char*)dType_water_NB_IoT.dType_bc66_receive.ui8buf_rx , (char*)">" ) != NULL )
		return TRUE;
	else
		return FALSE;
}

/**
  * @brief   Check send process success or not
  * @param   NULL
  * @retval  NULL
*/
uint8_t BC66_Check_Send_Data_Success_TCP ( void )
{
	uint8_t a = 0;
	a = Search_String_In_Buffer ( (uint8_t*)dType_water_NB_IoT.dType_bc66_receive.ui8buf_rx , CountReceive_u16 , (uint8_t*)"SEND OK" , 7 );
    return a;
}

/**
  * @brief   Wait desired value of a variable in a interval time and break out when variable return desired value
  * @param   return_var variable that want to wait for it's value turn to desired value
  * @param   wanted_value value that want return_var to be
  * @param   interval inteval of time that wait variable turn to desired value
  * @retval  NULL
*/
void BC66_Wait_Return_Wanted_Value_In_A_Interval ( uint8_t return_var , uint8_t wanted_value , uint32_t interval )
{
	uint32_t time_mark = 0;
	time_mark = ui32_tick_count;
	do
	{
	  if ( BC66_Check_Time_Out ( time_mark , 5000 ) == TRUE )
	  {
		break;
	  }
		osDelay (1);
	} while ( return_var != wanted_value );
}

/**
  * @brief   Handle reason why lead to _TCP_CLOSE_ by a specify function
  * @param   reason the reason lead to _TCP_CLOSE_
  * @param   internet_fail_time the INTERNET process time fail lead to _TCP_CLOSE_
  * @param   tcp_close function handle reason lead to _TCP_CLOSE_
  * @retval  NULL
*/ 
void BC66_Handle_Reason_Close_TCP ( uint8_t reason , uint8_t internet_fail_time , func_handle_error tcp_close )
{
	if ( reason >= internet_fail_time )
	{
		tcp_close();
	}
}

/**
  * @note    this function was created to test send stable in a loop that just send MQTT message
  * @brief   Function handle when MQTT send success (no error) then closed TCP
  * @param   NULL
  * @retval  NULL
*/
void BC66_Handler_TCP_Close_None_Err (void)
{
	dType_water_NB_IoT.dType_AT.ui8_pointer = _REQUEST_PSM_NETWORK_;
}

/**
  * @brief   Function handle when open MQTT max time fail lead to TCP Close
  * @param   NULL
  * @retval  NULL
*/
void BC66_Handler_TCP_Close_MQTT_Max_Open_Fail (void)
{
	dType_water_NB_IoT.dType_AT.ui8_pointer = _TCP_OPEN_1_;
	dType_MQTT_watermetter.dType_MQTT_fail.ui8_open = 0;	
}

/**
  * @brief   Function handle when publish MQTT messeage reach max time fail lead to _TCP_CLOSE_
  * @param   NULL
  * @param   NULL
*/
void BC66_Handler_TCP_Close_MQTT_Max_Pub_Fail (void)
{
	dType_water_NB_IoT.dType_AT.ui8_pointer = _TCP_OPEN_1_;
	dType_MQTT_watermetter.dType_MQTT_fail.ui8_pub = 0;
}

/**
  * @brief    Function handle when TCP open reach max time fail lead to TCP_CLOSE
  * @param    NULL
  * @retval   NULL
*/
void BC66_Handler_TCP_Close_TCP_Max_Open_Fail (void)
{
	dType_water_NB_IoT.dType_AT.ui8_pointer = _TCP_OPEN_1_;
}

/**
  * @brief    Function handle when MQTT Ping request reach max time fail lead to TCP_CLOSE
  * @param    NULL
  * @retval   NULL
*/
void BC66_Handler_TCP_Close_MQTT_Max_Ping_Fail (void)
{
	dType_water_NB_IoT.dType_AT.ui8_pointer = _TCP_OPEN_1_;
	dType_MQTT_watermetter.dType_MQTT_fail.ui8_ping = 0;
}

/**
  * @brief    Scan and check why process lead to _TCP_CLOSE_ then handle it with the function
  * @param    NULL
  * @retval   NULL
*/
void BC66_TCP_Close_Scan_And_Handler_Error ( void )
{
	for ( uint8_t ui8_reason = _MQTT_SEND_SUCCESS_ ; ui8_reason <= _TCP_OPEN_FAIL_ ; ++ui8_reason )
	{
		BC66_Handle_Reason_Close_TCP ( dType_TCP_close_reason[ui8_reason].ui8_reason_fail,
		                               dType_TCP_close_reason[ui8_reason].ui8_max_fail_time,
		                               dType_TCP_close_reason[ui8_reason].FncHandler_Err );
	}
}

/**
  * @brief  ham check va xu ly cac thong tin nhan tu server
*/
//extern uint8_t *pointer_result;
uint8_t BC66_Check_MQTT_Receive ( void )
{
	uint8_t topic_hex_array[200] = {0x00};
	uint8_t *pointer = NULL;
	uint16_t topic_length = 0;
	topic_length = strlen ((char*)test_sub_array);
//    memcpy (topic_hex_array, test_sub_array, topic_length);
	for (uint16_t i = 0; i < topic_length; i++)
	{
		*(topic_hex_array + i) = *(test_sub_array + i);
	}
    Convert_Char_To_Hex (topic_hex_array, topic_length);
    topic_length = strlen ((char*)topic_hex_array);
	pointer = Search_String_Location_In_Buffer_2 ((uint8_t*)dType_water_NB_IoT.dType_bc66_receive.ui8buf_rx, CountReceive_u16,
		                                           (uint8_t*)topic_hex_array, topic_length);
	if ( pointer != NULL )
    {

//		BC66_UART_Send_Data_To_Terminal ( (uint8_t*)"da nhan duoc tu server mqtt" ,27);
//		BC66_UART_Send_Data_To_Terminal ( (uint8_t*)"\r\n" , 2 );

		return 1;
	}
    return 0;
}

void BC66_WakeUp_DeepSleep ( void )
{
	BC66_WKUP_DOWN ;
	osDelay(8);
	BC66_WKUP_UP;
}
