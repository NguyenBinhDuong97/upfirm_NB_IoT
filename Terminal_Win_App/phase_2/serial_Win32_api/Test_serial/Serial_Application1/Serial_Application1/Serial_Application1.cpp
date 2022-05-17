//// Serial_Application1.cpp : This file contains the 'main' function. Program execution begins and ends there.
////
//
//#include <iostream>
//#include "Tserial.h"
//#include "MyLib.h"
//
//int main()
//{
//    char Array_Send[13] = "test serial\n";
//    Tserial* com;
//    com = new Tserial();
//    std::cout << "Bat dau truyen du lieu qua UART" << std :: endl;
//    if (com != 0)
//    {
//        com->connect( (char*)"COM3", 9600, spNONE );
//        for (short i = 0; i < 200; i++)
//        {
//            com->sendArray(Array_Send, 13);
//        }
//    }
//    com->disconnect();
//    delete com;
//    return 0;
//}
//
//// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
//// Debug program: F5 or Debug > Start Debugging menu
//
//// Tips for Getting Started: 
////   1. Use the Solution Explorer window to add/manage files
////   2. Use the Team Explorer window to connect to source control
////   3. Use the Output window to see build output and other messages
////   4. Use the Error List window to view errors
////   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
////   6. In the future, to open this project again, go to File > Open > Project and select the .sln file






/* ------------------------------------------------------------------------ --
--                                                                          --
--                  Application example for using Tserial                   --
--                                                                          --
--                                                                          --
--                                                                          --
--  Copyright @ 2001          Thierry Schneider                             --
--                            thierry@tetraedre.com                         --
--                                                                          --
--                                                                          --
--                                                                          --
-- ------------------------------------------------------------------------ --
--                                                                          --
--  Filename : sertest2.cpp                                                 --
--  Author   : Thierry Schneider                                            --
--  Created  : April 8th 2001                                               --
--  Modified : April 24th 2001                                              --
--  Plateform: Windows 95, 98, NT, 2000 (Win32)                             --
-- ------------------------------------------------------------------------ --
--                                                                          --
--  This software is given without any warranty. It can be distributed      --
--  free of charge as long as this header remains, unchanged.               --
--                                                                          --
-- ------------------------------------------------------------------------ --
--                                                                          --
--  01.04.24    # ifdef __BORLANDC__ added in the header                    --
--                                                                          --
-- ------------------------------------------------------------------------ */




/* ---------------------------------------------------------------------- */
#ifdef __BORLANDC__
#pragma hdrstop            // borland specific
#include <condefs.h>       
#pragma argsused           
USEUNIT("Tserial.cpp");
#endif

/* ---------------------------------------------------------------------- */
#include <iostream>
#include "Tserial.h"
#include "MyLib.h"
#include <conio.h>


int main()
{
    int c;
    int i;
    int n, old_n;
    Tserial* com;
    char buffer[20];

    com = new Tserial();
    if (com != 0)
    {
        if (com->connect((char*)"COM4", 9600, spNONE) != 0)
        {
            std::cout << "mo com bi loi" << std::endl;
            return 0;
        }
        else
            std::cout << "mo cong com thanh cong" << std::endl;
        // ------------------
        //printf("Press 5 keys on keyboard, then 4 on serial \n");
        //printf("then 3 keys on keyboard, then 5 on serial \n");
        //// ------------------
        //for (i = 0; i < 5; i++)
        //{
        //    std::cin >> c;
        //    com->sendChar((char)c);
        //}

        ////for (i = 0; i < 4; i++)
        ////{
        ////    c = (int)com->getChar();
        ////    std::cout << c;
        ////}
        //std::cout << std::endl;

        //// ------------------
        //for (i = 0; i < 3; i++)
        //{
        //    std::cin >> c;
        //    std::cout << c;
        //    com->sendChar((byte)c);
        //}
        //std::cout << std::endl;
        //old_n = -1;
        //do
        //{
        //    n = com->getNbrOfBytes();
        //    if (n != old_n)
        //    {
        //        old_n = n;
        //        std::cout << "Number of bytes = " << n;
        //    }
        //} while (n < 5);

        //com->getArray(buffer, 3);

        //for (i = 0; i < 3; i++)
        //{
        //    std::cout << buffer[i] << " ";
        //}
        //std::cout << "_" << std::endl;
        //std::cout << "Number of bytes = \n" << com->getNbrOfBytes();
        //for (i = 0; i < 2; i++)
        //{
        //    c = (int)com->getChar();
        //    std::cout << c << "_";
        //}
        //std::cout << "\n";
        wait(4000);
        com->sendArray( (char*)"Hello World !", 11);
        wait(3000);

        //com->getArray(buffer, 3);

        //for (i = 0; i < 3; i++)
        //{
        //    std::cout << buffer[i] << " ";
        //}
        //std::cout << "_" << std::endl;
        com->disconnect();

        // ------------------
        delete com;
        com = 0;
    }
    return 0;
}
