// Window_Application.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include "MyLib.h"
#include "Tserial.h"
#include "SerialApp.h"
#include <stdio.h>
#include <string>
#include "hex_file_manipulate.h"

int main()
{
    Initial_Preface_Of_Program();
    if (SeriApp_COM_Open(ComWin.name, ComWin.baud_rate) == 0)
        return 0;
    if (SeriApp_COM_File_Open() == 0)
        return 0;
    if (SeriApp_COM_DataFile_Transfer() == 0)
        return 0;
    std::cout << "Load process complete" << std::endl;
    SeriApp_COM_disconnect();
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
