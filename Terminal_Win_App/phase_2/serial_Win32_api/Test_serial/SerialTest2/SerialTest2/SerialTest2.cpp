// SerialTest2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include "MyLib.h"
#include "Tserial.h"
#include "SerialApp.h"
#include <stdio.h>
#include <string>


int main()
{
    int n;
    //int old_n;


    char ComName[7];
    int BaudRate;

    char LengthArray[5];
    uint16_t LengthReceive;
    char ReceiveArray[300];

    char TransData[50];

    //com = new Tserial();
    //if (com != 0)
    //{
        if ( SeriApp_SetUp_COM(ComName, BaudRate) == 0 )
        {
            std::cout << "\nCOM open error";
            wait(2000);
            return 0;
        }

        if ( SeriApp_Connect_COM(ComName, BaudRate, spNONE) != 0 )
        {
            std::cout << "\nCOM open error";
            wait(2000);
            return 0;
        }
        else
        {
            std::cout << "\nCOM opened successfully";
            wait(2000);
        }
        while (1)
        {
            Reset_Buffer((uint8_t*)TransData, 50);
            Reset_Buffer((uint8_t*)LengthArray, 5);
            Reset_Buffer((uint8_t*)ReceiveArray, 300);
            //std::cout << "\nData transfer: ";
            //std::cin.ignore(1);
            //std::cin.getline(TransData, 50);
            //KeyBoard_Array_Input((char*)TransData, 50, '.');
            //n = strlen(TransData);
            //SeriApp_COM_SendArray ((char*)TransData, n);

            SeriApp_COM_getLength_ReceiveArray(LengthArray, LengthReceive);
            //SeriApp_COM_sendAck_Length_Receive();
            //std::cout << LengthReceive << std::endl;
            //wait(1000);
            SeriApp_COM_getReceiveArray(ReceiveArray, LengthReceive);
            std::cout << ReceiveArray << std::endl;
            wait(1);
        }

        //com->getArray(buffer, 5);
        //for (short count = 0; count < 5; count++)
        //{
        //    std::cout << buffer[count];
        //}

        SeriApp_COM_disconnect();
        //delete com;
        //com = 0;
    //}
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
