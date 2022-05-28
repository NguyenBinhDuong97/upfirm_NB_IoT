#ifndef _MY_LIB_H
#define _MY_LIB_H

#include <iostream>
#include "string"
#include <cstdlib>
#include <fstream>
#include <time.h>
#include <time.h>

typedef struct {
	char* cDataArray;
	uint16_t ui16_length;
	char cLengthArray[4];
} sData;

void wait(unsigned timeout);
uint8_t Reset_Buffer(uint8_t* array, int numb);
uint8_t Initial_Preface_Of_Program(void);
void    KeyBoard_Array_Input(char* Array, int number_element, char delimiter);
uint16_t BDLib_Convert_Length_Char_to_Int_2(sData* Receive);
uint16_t BDLib_Convert_Length_Char_to_Int_1(char* Array, uint16_t& reuslt);
#endif