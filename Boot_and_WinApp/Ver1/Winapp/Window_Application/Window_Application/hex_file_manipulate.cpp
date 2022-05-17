#include <iostream>
#include "hex_file_manipulate.h"
#include "string"
#include <cstdlib>
#include <fstream>
#include "MyLib.h"

string strHexFile_Source;
string strHexFile_Data;

fstream SourceFile;
fstream DataFile;
uint32_t ui32_DataFileWordSize = 0;
/**
  * @brief : Search RecordType of a line of hex file
  * @param : line pointer point to a string or an array that contain a line in hex file
  * @retval: RecordType of hex lien
*/
uint8_t HexFile_Check_Record_Type( uint8_t *line)
{
	uint8_t RecordType = _RECORD_ERROR_;
	uint8_t ui8_array[2] = { 0x08 };
	uint8_t *ui8_pointer = NULL;
	ui8_pointer = line + 7; // 7 la vi tri bat dau tu dong hex den vung record trong ma hex
	for (uint8_t ui8_i = 0; ui8_i < 2; ui8_i++)
	{
		ui8_array[ui8_i] = *(ui8_pointer + ui8_i);
	}
	RecordType = HexFile_Convert_HextoDecimal( ui8_array );
	switch (RecordType)
	{
	    case _DATA_:
			return _DATA_;
		    break;
		case _EOF_:
			return _EOF_;
			break;
		case _EXTEND_SEGMENT_ADD_:
			return _EXTEND_SEGMENT_ADD_;
			break;
		case _START_SEGMENT_ADD_:
			return _START_SEGMENT_ADD_;
			break;
		case _EXTENDED_LINEAR_ADD_:
			return _EXTENDED_LINEAR_ADD_;
			break;
		case _START_LINEAR_ADD_:
			return _START_LINEAR_ADD_;
			break;
		default:
			return _RECORD_ERROR_;
			break;
	}
}

/**
  * @brief  : Check number of data bytes in line of hex file
  * @param  : line pointer point to a string or an array that contain a line of hex file
  * @retval : Number of bytes in data feild in decimal format
*/
uint8_t HexFile_Check_Byte_Data( uint8_t *line )
{
	uint8_t DataBytes = 0;
	uint8_t ui8_array[2] = { 0x00 };
	uint8_t *ui8_ptr = NULL;
	ui8_ptr = line + 1; // 1 la vi tri bat dau tu dong hex den vi tri bieu thi so byte du lieu trong dong do 
	for (uint8_t ui8_i = 0; ui8_i < 2; ui8_i++)
	{
		ui8_array[ui8_i] = *(ui8_ptr + ui8_i);
	}
	DataBytes = HexFile_Convert_HextoDecimal( ui8_array );
	return DataBytes;
}

/**
  * @brief  : Extract data of a line in a hex file into an array
  * @param  : DestinationArray pointer point to array that will be contain data of line
  * @param  : SourceArray pointer point to array that contain a line of hex file
  * @param  : NumberDataBytes number data in byte of a line that will be contain in an array in hexfile 
    note: NumberDataBytes must be in decimal not hexa
  * @retval : 1 is function operate success
*/
uint8_t HexFile_Extract_Line_Data(uint8_t *DestinationArray, uint8_t *SourceArray,uint8_t NumberDataBytes)
{  
	uint8_t NumberData = 0;
	NumberData = NumberDataBytes * 2;
	uint8_t *ui8_ptr = NULL;
	ui8_ptr = SourceArray + 9; // 9 la vi tri bat dau tu dong hex den vi tri bieu thi du lieu cua dong do 
	for (uint8_t ui8_i = 0; ui8_i < NumberData; ui8_i++)
	{
		*(DestinationArray + ui8_i ) = *(ui8_ptr + ui8_i);
	}
	return 1;
}

/**
  * @brief  : Write data from an array to DataFile 
  * @param  : array pointer point to array that contain data need to written to file
  * @param  : NumberData the number data in bytes in an array need to be written to file  
  * @retval : 1 is function operate ok
  * @note   : The data file must be opened before using this function
*/
uint8_t HexFile_Write_Data_To_DataFile( uint8_t* array, uint8_t NumberData )
{
	NumberData *= 2;
	for (short i = 0; i < (NumberData); i++)
	{
		DataFile << *(array + i);
	}
	DataFile << endl;
	return 1;
}

/**
  * @brief  : Convert Hex to Decimal in a line of hex file
  * @param  : array pointer point to array contain hex data
  * @retval : Return decimal value
*/
uint8_t HexFile_Convert_HextoDecimal( uint8_t* array )
{
	uint8_t result = 0;
	uint8_t base = 1;
	for ( int i = 2; i >= 0; i--)
	{
		if ( *(array + i) >= '0' && *(array + i) <= '9') {
			result += (*(array + i) - 48) * base;
			// incrementing base by power
			base = base * 16;
		}
		else if (*(array + i) >= 'A' && *(array + i) <= 'F') {
			result += (*(array + i) - 55) * base;
			// incrementing base by power
			base = base * 16;
		}
	}
	return result;
}
 
/**
  * @brief  : Enter file path for hexial source file in text
  * @param  : NONE
  * @retval : 1 if fucntion operate OK
*/
uint8_t HexFile_Enter_SourceFile_Path_From_Console( void )
{
	cout << endl;
	cout << "Enter file path that contain hex file soure in text format ";
	cout << "(example: \"C:\\\\Users\\\\Admin\\\\Document\\\\SourceFile.txt\")" << endl;
	cin >> strHexFile_Source;
	cout << endl;
	return 1;
}

/**
  * @brief  : Enter file path for hexical data file in text 
  * @param  : NONE
  * @retval : 1 if fucntion operate OK
*/
uint8_t HexFile_Enter_DataFile_Path_From_Console(void)
{
	cout << endl;
	cout << "Enter file path that will contain hex data file in text format ";
	cout << "(example: \"C:\\\\Users\\\\Admin\\\\Document\\\\DataFile.txt\")" << endl;
	cin >> strHexFile_Data;
	cout << endl;
	return 1;
}

/**
  * @brief  : Close the hexial source file
  * @param  : NONE
  * @retval : 1 if fucntion operate OK
*/
uint8_t HexFile_Open_File_HexSource( void )
{
	SourceFile.open( strHexFile_Source );
	if (SourceFile.is_open())
		cout << "Source file was opened successfully" << endl;
	else {
		cout << "Open process error" << endl;
		return 0;
	}
	wait(2000);
	return 1;
}

/**
  * @brief  : Open the hexial data file
  * @param  : NONE
  * @retval : 1 if fucntion operate OK
*/
uint8_t HexFile_Open_File_HexData( void )
{
	DataFile.open( strHexFile_Data, ios :: in | ios::out | ios :: app );
	if ( DataFile.is_open() )
		cout << "Data file was opened successfully" << endl;
	else {
		cout << "Open process error" << endl;
		return 0;
	}
	wait(2000);
	return 1;
}

/**
  * @brief  : Close the hexial source file
  * @param  : NONE
  * @retval : 1 if fucntion operate OK
*/
uint8_t HexFile_Close_File_HexSource( void )
{
	SourceFile.close();
	return 1;
}

/**
  * @brief  : Close the hexial data file
  * @param  : NONE
  * @retval : 1 if fucntion operate OK
*/
uint8_t HexFile_Close_File_HexData( void )
{
	DataFile.close();
	return 1;
}

/**
  * @brief  : Extract Data from a line in hexial source file into an array and calculate size of data file in word format
  * @param  : Array pointer point to array that will contain data of hex line in hex source file
  * @param  : NumberElements indicate the max element of array which will contain data of hex line in hex source file
  * @retval : 1 if fucntion operate OK
*/
uint8_t HexFile_Extract_Data_From_Line_InSourceFileTo_DataFile_and_CalculateSize_DataFile( uint8_t* Array , uint8_t NumberElements, uint32_t& size )
{
	uint8_t ui8_LineArray[50];
	uint8_t number = 0;
	cout << endl << "The content of source file:" << endl;
	while (!SourceFile.eof())
	{
		Reset_Buffer(ui8_LineArray, 50); // 50 la so phan tu co trong mang cua ui8_LineArray
		SourceFile.getline((char*)ui8_LineArray, 50);
		cout << ui8_LineArray << endl;
		if (HexFile_Check_Record_Type(ui8_LineArray) == _DATA_)
		{
			number = HexFile_Check_Byte_Data(ui8_LineArray);
			if (number != 0)
			{
				HexFile_Extract_Line_Data(Array, ui8_LineArray, number);
				// the function below was applied to test extract data proceess operate well or not 
				// you can delete it when you feel dont need to use it
				HexFile_Write_Data_To_DataFile(Array, number);
				ui32_DataFileWordSize = ui32_DataFileWordSize + number / 4;
				//HexFile_Calculate_WordSize_DataFile(Array, size);
				Reset_Buffer(Array, NumberElements);
			}
		}
	}
	return 1;
}

uint8_t HexFile_Calculate_WordSize_DataFile(uint8_t* Array, uint32_t& size)
{
	uint8_t do_dai = 0;
	do_dai = strlen((char*)Array);
	size = size + (do_dai/8);
	return 1;
}

uint8_t HexFile_Put_GetPointer_atBegin_DataFile(void)
{
	DataFile.clear();
	DataFile.seekg(0, ios::beg);
	return 1;
}

uint8_t HexFile_Extract_WordByWord_From_A_Line(uint8_t* source, uint8_t* destination)
{
	for (uint8_t pos = 0; pos < 8; pos++)
	{
		*(destination + pos) = *(source + pos);
	}
	return 1;
}

uint8_t HexFile_Convert_A_Word_LittleEndian(uint8_t* Array)
{
	uint8_t advArray[8] = {0x00};
	strcpy ((char*)advArray, (char*)Array);
	*(Array + 0) = *(advArray + 6);
	*(Array + 1) = *(advArray + 7);
	*(Array + 2) = *(advArray + 4);
	*(Array + 3) = *(advArray + 5);
	*(Array + 4) = *(advArray + 2);
	*(Array + 5) = *(advArray + 3);
	*(Array + 6) = *(advArray + 0);
	*(Array + 7) = *(advArray + 1);
	return 1;
}