#ifndef _HEX_FILE_MANIPULATE_H
#define _HEX_FILE_MANIPULATE_H

#pragma warning(disable:4996)

using namespace std;

typedef enum {
	_DATA_,
    _EOF_,
	_EXTEND_SEGMENT_ADD_,
	_START_SEGMENT_ADD_,
	_EXTENDED_LINEAR_ADD_,
	_START_LINEAR_ADD_,
	_RECORD_ERROR_,
}Hex_Record;

typedef struct {
	uint8_t *Line;// Contain all of a line in hex file
	uint8_t DataByte; // Number of data in bytes
	uint8_t RecordType; // Record type of a line in hex file;
	uint8_t *Data; // Data feild of a line in hex file
}Hex_Line;
/*-----------------------------------------------------*/
extern string strHexFile_Source;
extern string strHexFile_Data;
extern fstream SourceFile;
extern fstream DataFile;
extern uint32_t ui32_DataFileWordSize;
extern uint8_t ui8_SurplusByte;
/*-----------------------------------------------------*/
uint8_t HexFile_Check_Record_Type( uint8_t *line );
uint8_t HexFile_Check_Byte_Data( uint8_t *line );
uint8_t HexFile_Extract_Line_Data( uint8_t *ui8_ContainArray, uint8_t *line, uint8_t NumberDataBytes );
uint8_t HexFile_Write_Data_To_DataFile( uint8_t *array, uint8_t NumberData );
uint8_t HexFile_Convert_HextoDecimal( uint8_t *array );
uint8_t HexFile_Enter_SourceFile_Path_From_Console( void );
uint8_t HexFile_Enter_DataFile_Path_From_Console(void);
uint8_t HexFile_Open_File_HexSource( void );
uint8_t HexFile_Open_File_HexData( void );
uint8_t HexFile_Close_File_HexSource(void);
uint8_t HexFile_Close_File_HexData(void);
uint8_t HexFile_Extract_Data_From_Line_InSourceFileTo_DataFile_and_CalculateSize_DataFile(uint8_t* Array, uint8_t NumberElements, uint32_t& size);
uint8_t HexFile_Calculate_WordSize_DataFile(uint8_t* Array, uint32_t& size);
uint8_t HexFile_Put_GetPointer_atBegin_DataFile(void);
uint8_t HexFile_Extract_WordByWord_From_A_Line(uint8_t* source, uint8_t* destination);
uint8_t HexFile_Convert_A_Word_LittleEndian(uint8_t* Array);
uint8_t HexFile_Extract_Word_From_SourceFile (uint8_t* Array, uint8_t NumberElements, uint32_t& size);
uint8_t HexFile_Extract_Byte_From_A_Line(uint8_t* source, uint8_t* destination, uint8_t ByteNumb);
#endif
