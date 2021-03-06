/*
 * mylib.h
 *
 *  Created on: May 11, 2022
 *      Author: Admin
 */

#ifndef MYLIB_H_
#define MYLIB_H_

#include "main.h"
#include "stdint.h"
#include "stdlib.h"

typedef struct{
	uint8_t *DataArray;
	uint16_t ui16_length;
	uint8_t *LengthArray;
}sData;

extern sData ACK_START;
extern sData ACK_DATA;
extern sData ACK_END;
extern sData ACK_ERROR;

uint64_t Convert_Hex_to_Int(uint8_t *HexArray);
uint64_t Convert_Hex_to_Int2(uint8_t *HexArray, uint8_t NumberHex);

#endif /* MYLIB_H_ */
