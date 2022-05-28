#include "MyLib.h"

uint8_t Reset_Buffer(uint8_t* array, int numb)
{
	for (int ui8_i = 0; ui8_i < numb; ui8_i++)
	{
		*(array + ui8_i) = 0x00;
	}
	return 1;
}

uint8_t Initial_Preface_Of_Program( void )
{
	std::cout << "/*********************************************************************************************************************/" << std::endl;
	std::cout << "This is first verison of the application and it was written in short time" << std::endl;
	std::cout << "So that it can have a few problem if u found those please contact the author ";
	std::cout << "Nguyen Binh Duong through email:" << std::endl;
	std::cout << "   + binhhduong82@gmail.com" << std::endl;
	std::cout << "Thank for your support" << std::endl;
	std::cout << std::endl;
	std::cout << "* NOTE:  " << std::endl;
	std::cout << "      + When enter file path for a file u must change \'\\\' charactor become \'\\\\\' " << std::endl;
	std::cout << "/*********************************************************************************************************************/" << std::endl;
	return 1;
}

void wait(unsigned timeout)
{
	timeout += std::clock();
	while (std::clock() < timeout) continue;
}

void    KeyBoard_Array_Input(char* Array, int number_element, char delimiter)
{
	for (short i = 0; i < number_element; i++)
	{
		std::cin >> *(Array + i);
		if ( *(Array + i) == delimiter)
		{
			*(Array + i) = 0x00;
			break;
		}
	}
}

uint16_t BDLib_Convert_Length_Char_to_Int_2(sData* Receive)
{
	Receive->ui16_length = atoi(Receive->cLengthArray);
	return Receive->ui16_length;
}

uint16_t BDLib_Convert_Length_Char_to_Int_1(char* Array, uint16_t& result)
{
	result = atoi(Array);
	return result;
}