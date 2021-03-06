#ifndef __MQTT_CONNECT_H
#define __MQTT_CONNECT_H

#include "main.h"
#include "MQTTformat.h"
#include "MQTTPacket.h"
#include "my_lib.h"
/*==============================================================================================================*/
#define MQTTCLIENT_SUCCESS 1
#define MQTTCLIENT_FAIL 0
#define MQTT_SEND_SUCCESS 0
#define MQTT_MAX_OPEN_FAIL 1
#define MQTT_MAX_PUB_FAIL 3
#define MQTT_MAX_PING_FAIL 2
/*==============================================================================================================*/


typedef struct
{
	unsigned char* buf;
	int buflen;
	unsigned char dup; 
	int qos;
	unsigned char retained; 
	unsigned short packetid;
	MQTTString topicName; 
	unsigned char* payload; 
     int payloadlen;
}	pub_struct;

typedef struct
{
	unsigned char* buf;
	int buflen;
	unsigned char dup; 
	unsigned short packetid;
	int count; 
	MQTTString *topicFilters;
	int *requestedQoS; 
}sub_struct;

typedef enum
{
	_MQTT_CON_,
	_MQTT_PUB_,
	_MQTT_SUB_,
	_MQTT_DISCON_,
	_MQTT_PINGREQ_,
} eType_MQTT_PACKAGES;

typedef struct
{
	uint8_t u8_packets_name;
	uint8_t u8_packet_check;
	uint8_t u8_packets_result;
	sType_string dtype_string;
} sType_MQTT_packets;

typedef struct
{
	uint8_t ui8_open;
	uint8_t ui8_ping;
	uint8_t ui8_pub;
} sType_MQTT_fail_reset;

typedef struct
{
	uint32_t ui32_keep_alive_mark_time;
	uint32_t ui32_keep_alive_time;
	uint32_t ui8_start_keepalive;
	
	uint8_t ui8_prev_MQTT_step;
	
	sType_MQTT_fail_reset dType_MQTT_fail;
} sType_MQTT;

/*==============================================================================================================*/
extern MQTTPacket_connectData test_conect;
extern uint8_t test_con_array[200];

extern uint8_t test_pub_array[200];
extern pub_struct test_pub_mes;

extern MQTTString test_TopicFiltes[];
extern int test_requestedQoS[];
extern uint8_t test_sub_array[200];
extern sub_struct test_MQTT_sub;

extern uint8_t test_discon_array[200];

extern sType_MQTT_packets dType_set_MQTT_packets[];

extern sType_MQTT dType_MQTT_watermetter;

/*==============================================================================================================*/
void MQTT_Init_Connect ( void );
void MQTT_Init_Pub_Opts ( void );
void MQTT_Init_Sub_Opts ( void );
void MQTT_Check_Keepalive_Time ( void );
void MQTT_Create_Topic_To_Sub (uint8_t *topic, uint16_t topic_length, unsigned char dup, unsigned short packetid, int qos);
void MQTT_Create_Message_To_Pub (uint8_t *message, uint16_t message_length, unsigned char dup,
		                         int qos, unsigned char retained, unsigned short packetid,
								 uint8_t* topicName);
#endif

