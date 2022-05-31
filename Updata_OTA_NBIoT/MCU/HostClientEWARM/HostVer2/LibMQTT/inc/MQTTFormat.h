/*******************************************************************************
 * Copyright (c) 2014 IBM Corp.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * and the Eclipse Distribution License is available at
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Ian Craggs - initial API and implementation and/or initial documentation
 *******************************************************************************/

#if !defined(MQTTFORMAT_H)
#define MQTTFORMAT_H

#include "StackTrace.h"
#include "MQTTPacket.h"

//=====================================================================================================================//
/**
 * Defines the MQTT "Last Will and Testament" (LWT) settings for
 * the connect packet.
 */
typedef struct
{
	/** The eyecatcher for this structure.  must be MQTW. */
	char struct_id[4];
	/** The version number of this structure.  Must be 0 */
	int struct_version;
	/** The LWT topic to which the LWT message will be published. */
	MQTTString topicName;
	/** The LWT payload. */
	MQTTString message;
	/**
      * The retained flag for the LWT message (see MQTTAsync_message.retained).
      */
	unsigned char retained;
	/**
      * The quality of service setting for the LWT message (see
      * MQTTAsync_message.qos and @ref qos).
      */
	char qos;
} MQTTPacket_willOptions;


#define MQTTPacket_willOptions_initializer { {'M', 'Q', 'T', 'W'}, 0, {NULL, {0, NULL}}, {NULL, {0, NULL}}, 0, 0 }

typedef struct
{
	/** The eyecatcher for this structure.  must be MQTC. */
	char struct_id[4];
	/** The version number of this structure.  Must be 0 */
	int struct_version;
	/** Version of MQTT to be used.  3 = 3.1 
	                                 4 = 3.1.1
	  */
	unsigned char MQTTVersion;
	MQTTString clientID;
	unsigned short keepAliveInterval;
	unsigned char cleansession;
	unsigned char willFlag;
	MQTTPacket_willOptions will;
	MQTTString username;
	MQTTString password;
} MQTTPacket_connectData;

//extern struct MQTTPacket_connectData data;
//===================================================================================================================//

const char* MQTTPacket_getName(unsigned short packetid);
int MQTTStringFormat_connect(char* strbuf, int strbuflen, MQTTPacket_connectData* data);
int MQTTStringFormat_connack(char* strbuf, int strbuflen, unsigned char connack_rc, unsigned char sessionPresent);
int MQTTStringFormat_publish(char* strbuf, int strbuflen, unsigned char dup, int qos, unsigned char retained,
		unsigned short packetid, MQTTString topicName, unsigned char* payload, int payloadlen);
int MQTTStringFormat_ack(char* strbuf, int strbuflen, unsigned char packettype, unsigned char dup, unsigned short packetid);
int MQTTStringFormat_subscribe(char* strbuf, int strbuflen, unsigned char dup, unsigned short packetid, int count,
		MQTTString topicFilters[], int requestedQoSs[]);
int MQTTStringFormat_suback(char* strbuf, int strbuflen, unsigned short packetid, int count, int* grantedQoSs);
int MQTTStringFormat_unsubscribe(char* strbuf, int strbuflen, unsigned char dup, unsigned short packetid,
		int count, MQTTString topicFilters[]);
char* MQTTFormat_toClientString(char* strbuf, int strbuflen, unsigned char* buf, int buflen);
char* MQTTFormat_toServerString(char* strbuf, int strbuflen, unsigned char* buf, int buflen);

#endif
