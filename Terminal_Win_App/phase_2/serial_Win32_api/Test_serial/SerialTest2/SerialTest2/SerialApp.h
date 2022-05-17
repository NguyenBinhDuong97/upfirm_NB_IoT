#ifndef _SERIAL_APP_H
#define _SERIAL_APP_H

#pragma warning(disable:4996)

#include <iostream>
#include "string"
#include <cstdlib>
#include <fstream>
#include <time.h>
#include <time.h>
#include "Tserial.h"
#include "MyLib.h"
#include <string>

//class COM
//{
//protected:
//	char* Com_name;
//	int Com_baudrate;
//public:
//	COM(char*, int );
//};

//extern char ComName[5];
//extern int BaudRate;

uint8_t  SeriApp_SetUp_COM(char* ComArray, int& baudrate);
uint8_t  SeriApp_SetUp_BaudRate(int& baud_rate);

int      SeriApp_Connect_COM(char* port_arg, int rate_arg, serial_parity parity_arg);
void     SeriApp_COM_SendArray(char* buffer, int len);
void     SeriApp_COM_disconnect(void);
int      SeriApp_COM_getArray(char* buffer, int len);

uint16_t SeriApp_COM_getLength_ReceiveArray(char* lengthArray, uint16_t& length);
uint8_t  SeriApp_COM_getReceiveArray(char* ReceiveArray, uint16_t length);
uint8_t  SeriApp_COM_sendAck_Length_Receive(void);
#endif
