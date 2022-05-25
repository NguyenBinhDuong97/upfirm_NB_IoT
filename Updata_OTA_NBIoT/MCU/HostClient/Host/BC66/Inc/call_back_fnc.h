#ifndef __CALL_BACK_FNC_H
#define __CALL_BACK_FNC_H

/*================================================================================================================*/
#include "main.h"

/*================================================================================================================*/

uint8_t FncCallback_TCP_Open_1 ( void );
uint8_t FncCallback_TCP_Open_2 ( void );
uint8_t FncCallback_MQTT_Open_1 ( void );
uint8_t FncCallback_MQTT_Pub_1 (void);
uint8_t FncCallback_MQTT_Pub_2 (void);
uint8_t FncCallback_MQTT_Ping_1 (void);
uint8_t FncCallback_MQTT_Ping_2 (void);
uint8_t FncCallback_MQTT_Sub_1 (void);
uint8_t FncCallback_MQTT_Sub_2 (void);
void Send_Debug_Infor (void);
#endif