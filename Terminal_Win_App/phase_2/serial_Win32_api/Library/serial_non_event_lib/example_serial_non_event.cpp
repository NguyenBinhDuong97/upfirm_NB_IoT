


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
#include "conio.h"
#include "Tserial.h"



int main(int argc, char* argv[])
{
    int c;
    int i;
    int n, old_n;
    Tserial *com;
    char buffer[20];

    com = new Tserial();
    if (com!=0)
    {
        com->connect("COM1", 19200, spNONE);

        // ------------------
        printf("Press 5 keys on keyboard, then 4 on serial \n");
        printf("then 3 keys on keyboard, then 5 on serial \n");
        // ------------------
        for (i=0; i<5; i++)
        {
            c = getch();
            printf("%c",c);
            com->sendChar((char) c);
        }

        for (i=0; i<4; i++)
        {
            c = (int) com->getChar();
            printf("%c",c);
        }
        printf("\n");

        // ------------------
        for (i=0; i<3; i++)
        {
            c = getch();
            printf("%c ",c);
            com->sendChar((byte) c);
        }

        printf("\n");
        old_n = -1;
        do
        {
            n = com->getNbrOfBytes();
            if (n!=old_n)
            {
                old_n = n;
                printf("Number of bytes = %d\n",n);
            }
        }
        while(n<5);

        com->getArray(buffer, 3);

        for (i=0; i<3; i++)
        {
            printf("%c",buffer[i]);
        }
        printf("_\n");
        printf("Number of bytes = %d\n",com->getNbrOfBytes());

        for (i=0; i<2; i++)
        {
            c = (int) com->getChar();
            printf("%c_",c);
        }
        printf("\n");
        com->sendArray("Hello World !",11);


        // ------------------
        com->disconnect();

        // ------------------
        delete com;
        com = 0;
    }
    return 0;
}
