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
#include "hex_file_manipulate.h"

typedef struct {
	char* name;
	int baud_rate;
}COM;

typedef enum {
	_INIT_,
    _COM_OPEN_,

};

extern COM ComWin;
extern sData Transfer;
extern sData Receive;
extern uint32_t ui32_LoadedWord;
extern uint32_t ui32_LoadedPercent;
extern char END_PROCESS[8];

extern uint8_t AppZone;

uint8_t  SeriApp_SetUp_COM(char* ComArray, int& baudrate);
uint8_t  SeriApp_SetUp_BaudRate(int& baud_rate);

int      SeriApp_Connect_COM(char* port_arg, int rate_arg, serial_parity parity_arg);
void     SeriApp_COM_SendArray(char* buffer, int len);
void     SeriApp_COM_disconnect(void);
int      SeriApp_COM_getArray(char* buffer, int len);

uint16_t SeriApp_COM_getLength_ReceiveArray(char* lengthArray, uint16_t& length);
uint8_t  SeriApp_COM_getReceiveArray(char* ReceiveArray, uint16_t length);
uint8_t  SeriApp_COM_sendAck_Length_Receive(void);
uint8_t  SeriApp_COM_Open(char* com, int& baud);

uint8_t  SeriApp_COM_File_Open(void);
uint8_t  SeriApp_COM_DataFile_Transfer(void);
uint8_t  SeriApp_COM_Wait_For_ACK(char*ACK);
uint8_t  SeriApp_COM_File_Open2(void);

uint8_t  SeriApp_COM_SourceFile_StartingAddress(uint8_t& appZone, char* AddZone1, char* AddZone2);
uint8_t  SeriApp_COM_Find_AppZone(uint8_t& appZone, char* AddZone1, char* AddZone2);
#endif
