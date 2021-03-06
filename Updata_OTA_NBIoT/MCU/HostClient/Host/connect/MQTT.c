#include "main.h"
#include "stdio.h"
#include "string.h"

#include "MQTTPacket.h"
#include "MQTTFormat.h"
#include "MQTT.h"

#include "bc66_uart.h"
#include "bc66_cmd.h"
#include "my_lib.h"

#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
/*==================================================================================================================*/



//array test connect to MQTT server
uint8_t test_con_array[200] = {0x00};

//components to test publishing MQTT message 
uint8_t test_pub_array[200] = {0x00};
const uint8_t test_pub_topic[20] = "0866495609";
uint8_t test_pub_payload[100] = "this messege sent from WMNB project";
uint8_t test_pub_len_payload = 35;

//Components to test subscribing mqqtt topics
int test_requestedQoS[]={ 1 };
MQTTString test_TopicFiltes[] = 
{
	{ "0866495609" , {0 , NULL} },
//	{ "GPIO" , {0 , NULL} },
//	{ "Analog" , {0 , NULL} },
};

uint8_t test_sub_array[200] = {0x00};
sub_struct test_MQTT_sub;

//components to test disconnecting MQTT server
uint8_t test_discon_array[200] = {0x00};

//declare test connect object
MQTTPacket_connectData test_conect = { {'M', 'Q', 'T', 'T'}, 0, 4, {NULL, {0, NULL}}, 60, 1, 0, \
                                      MQTTPacket_willOptions_initializer, {NULL, {0, NULL}}, {NULL, {0, NULL}} };
//delclare test publish object
pub_struct test_pub_mes;
																																				
//extern uint8_t dType_MQTT_watermetter.ui8_MQTT_step;

extern uint32_t gui_thanh_cong;
extern uint32_t dem_gui;
extern uint8_t debug_send[200];
extern uint8_t event;
extern QueueHandle_t qBC66step;
extern uint8_t ping_flag;

//extern sType_AT_struct dTypeArr_set_AT[];
																			
sType_MQTT_packets dType_set_MQTT_packets[] =
{
	{ _MQTT_CON_          ,   OFF   ,   FALSE    ,  {(uint8_t*)"4,20020000" , 10} },
	{ _MQTT_PUB_          ,   OFF   ,   FALSE    ,  {(uint8_t*)"4,4002" , 6} },
	{ _MQTT_SUB_          ,   OFF   ,   FALSE    ,  { NULL , 0 } },
	{ _MQTT_DISCON_       ,   OFF   ,   FALSE    ,  {(uint8_t*)"closed" , 6} },
	{ _MQTT_PINGREQ_      ,   OFF   ,   FALSE    ,  {(uint8_t*)"4,D000" , 6} },
};

sType_MQTT dType_MQTT_watermetter =
{
	.ui32_keep_alive_mark_time = 0,
	.ui32_keep_alive_time = 0,
  .ui8_start_keepalive = OFF,
	
	.ui8_prev_MQTT_step = 0,
	
  .dType_MQTT_fail.ui8_open = 0,	
	.dType_MQTT_fail.ui8_ping = 0,
	.dType_MQTT_fail.ui8_pub = 0,
};



/*=================================================================================================================*/																																					
/**
	* @brief   Initialize components of test connect object
	* @param   NULL
	* @retval  NULL
*/																			
void MQTT_Init_Connect ( void )
{
	test_conect.clientID.cstring = "VNMILK";
	test_conect.keepAliveInterval = 60;
	test_conect.cleansession = 1;
//	test_conect.username.cstring = "admin";
//	test_conect.password.cstring = "admin";
}

/**
  * @brief   Initialize components of test publish object
  * @param   NULL
  * @retval  NULL
*/
void MQTT_Init_Pub_Opts (void)
{
	test_pub_mes.buf = test_pub_array;
	test_pub_mes.buflen = 200;
	test_pub_mes.dup = 0;
	test_pub_mes.qos = 1;
	test_pub_mes.retained = 0;
	test_pub_mes.packetid = 39;
	test_pub_mes.topicName.cstring = (char*)test_pub_topic;
	test_pub_mes.payload = (unsigned char*)test_pub_payload;
	test_pub_mes.payloadlen = test_pub_len_payload;
}

/**
	* @brief   Initialize components of test subscribe object
	* @param   NULL
	* @retval  NULL
*/
void MQTT_Init_Sub_Opts (void)
{
	test_MQTT_sub.buf = test_sub_array;
	test_MQTT_sub.buflen = 200;
	test_MQTT_sub.dup = 0;
	test_MQTT_sub.packetid = 20;
	test_MQTT_sub.count = 3;
	test_MQTT_sub.topicFilters = test_TopicFiltes;
	test_MQTT_sub.requestedQoS = test_requestedQoS;
}

/**
  * @brief   Check keep alive time out when flag check time out ON
  * @param   NULL
  * @retval  NULL
*/
void MQTT_Check_Keepalive_Time ( void )
{
	if ( dType_MQTT_watermetter.ui8_start_keepalive == ON )
	{
	  if ( BC66_Check_Time_Out (dType_MQTT_watermetter.ui32_keep_alive_mark_time , dType_MQTT_watermetter.ui32_keep_alive_time) == TRUE ) 
	  {
		  //dType_water_NB_IoT.dType_AT.ui8_pointer = _MQTT_PINGREQ_1_;
		  if (ping_flag == OFF)
		  {
		      event = _MQTT_PINGREQ_1_;
		      xQueueSendToBack( qBC66step, &event, 0 );
		  }
	  }
  }
}

void MQTT_Create_Topic_To_Sub (uint8_t *topic, uint16_t topic_length,
		                       unsigned char dup, unsigned short packetid, int qos)
{
	Reset_Buffer ((uint8_t*)test_sub_array, 200);
// 	memset (test_sub_array, 0x00, 200*sizeof(char));
//	memmove (test_sub_array, topic, 7*sizeof(uint8_t*));

//    char *ptr = NULL;
//    ptr = test_MQTT_sub.topicFilters->cstring;
//
	for ( uint8_t i = 0; i < topic_length; i++)
	{
		*(test_sub_array + i) = *((char*)topic + i);
	}
//	test_TopicFiltes[0].lenstring.len = 0;
//  test_TopicFiltes[0].lenstring.data = NULL;
	test_requestedQoS[0] = qos;
	test_MQTT_sub.buf = test_sub_array;
	test_MQTT_sub.buflen = 200;
	test_MQTT_sub.dup = dup;
	test_MQTT_sub.packetid = packetid;
	test_MQTT_sub.count = 1;
	test_MQTT_sub.topicFilters->cstring = (char*)test_sub_array;
	test_MQTT_sub.requestedQoS = test_requestedQoS;
}

void MQTT_Create_Message_To_Pub (uint8_t *message, uint16_t message_length, unsigned char dup,
		                         int qos, unsigned char retained, unsigned short packetid,
								 uint8_t* topicName)
{
	Reset_Buffer ((uint8_t*)test_pub_array, 200);
	for ( uint8_t i = 0; i < message_length; i++)
	{
		*(test_pub_array + i) = *((char*)message + i);
	}
	test_pub_mes.dup = dup;
	test_pub_mes.qos = qos;
	test_pub_mes.retained = retained;
	test_pub_mes.packetid = packetid;
	test_pub_mes.topicName.cstring = (char*)topicName;
	test_pub_mes.payload = (unsigned char*)test_pub_array;
	test_pub_mes.payloadlen = message_length;
}

