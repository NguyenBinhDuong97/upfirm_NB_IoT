#include "MyLib.h"

uint8_t Reset_Buffer(uint8_t* array, uint8_t numb)
{
	for (uint8_t ui8_i = 0; ui8_i < numb; ui8_i++)
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