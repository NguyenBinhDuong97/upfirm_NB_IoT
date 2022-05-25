// Ver1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "hex_file_manipulate.h"
#include "MyLib.h"
#include "SerialApp.h"
#include "Tserial.h"

char Zone1[] = "0801";
char Zone2[] = "0807";

int main()
{
    Initial_Preface_Of_Program();

    AppZone = SeriApp_COM_SourceFile_StartingAddress(AppZone, Zone1, Zone2);
    if (AppZone == 0)
        return 0;
    std::cout << "\nApplication Zone is operating is: " << unsigned(AppZone) << std::endl;

    if (HexFile_Open_WordDataFile() == 0)
        return 0;
    HexFile_Calculate_Word_In_WordDataFile();
    
    if (SeriApp_COM_Open(ComWin.name, ComWin.baud_rate) == 0)
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
