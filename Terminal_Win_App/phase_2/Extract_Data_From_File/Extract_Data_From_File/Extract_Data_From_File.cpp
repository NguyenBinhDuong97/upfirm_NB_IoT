// Extract_Data_From_File.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <fstream>
#include "hex_file_manipulate.h"
#include "MyLib.h"

using namespace std;


int main()
{
	uint8_t array[50];

	Initial_Preface_Of_Program();
	HexFile_Enter_SourceFile_Path_From_Console();
	if ( HexFile_Open_File_HexSource() == 0 )
		return 0;
	HexFile_Enter_DataFile_Path_From_Console();
	if ( HexFile_Open_File_HexData() == 0 )
		return 0;
	HexFile_Extract_Data_From_Line_InSourceFile(array, 50);
	HexFile_Close_File_HexSource();
	HexFile_Close_File_HexData();
	return 0;
}



// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
