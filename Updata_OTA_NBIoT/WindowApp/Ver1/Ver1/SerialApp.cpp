#include "SerialApp.h"


char ACK_DATA[5] = "data";
char ACK_START[6] = "start";

char END_PROCESS[8] = "0000end";
char ACK_END[7] = "endACK";

char LengthArray[5];
char ReceiveArray[50];
char TransData[50];

char ComName[7];

uint32_t ui32_LoadedWord = 0;
uint32_t ui32_LoadedPercent = 0;

uint8_t AppZone = 0;

COM ComWin{
     ComName,
};

sData Transfer = {
     TransData,
     0,
     NULL,
};

sData Receive = {
    ReceiveArray,
    0,
    LengthArray,
};


char              port_2[20] = "\\\\.\\";                      // port name "com1",...
int               rate_2 = 0;                          // baudrate
serial_parity     parityMode_2 = spNONE;
HANDLE            serial_handle_2;                 // ...


uint8_t SeriApp_SetUp_COM( char *ComArray, int& baudrate )
{
	std::cout << "COM: ";
    std::cin.ignore(1);
	std::cin.getline( ComArray, 7 );
    std::cout << std::endl << "Baud rate: ";
    std::cin >> baudrate;
    if ( SeriApp_SetUp_BaudRate(baudrate) == 0 )
        return 0;
    else
        return 1;
}

uint8_t SeriApp_SetUp_BaudRate( int& baud_rate )
{
    switch (baud_rate)
    {
    case 600:
        break;
    case 1200:
        break;
    case 2400:
        break;
    case 4800:
        break;
    case 9600:
        break;
    case 14400:
        break;
    case 19200:
        break;
    case 28800:
        break;
    case 38400:
        break;
    case 56000:
        break;
    case 57600:
        break;
    case 115200:
        break;
    case 128000:
        break;
    case 256000:
        break;
    default:
        std::cout << "Baud rate is invalid" << std::endl;
        return 0;
        break;
    }
    return 1;
}


int     SeriApp_Connect_COM(char* port_arg, int rate_arg, serial_parity parity_arg)
{
    int erreur;
    DCB  dcb;
    COMMTIMEOUTS cto = { 0, 0, 0, 0, 0 };

    /* --------------------------------------------- */

    serial_handle_2 = INVALID_HANDLE_VALUE;
    if (serial_handle_2 != INVALID_HANDLE_VALUE)
        CloseHandle(serial_handle_2);
    serial_handle_2 = INVALID_HANDLE_VALUE;

    erreur = 0;

    if (port_arg != 0)
    {
        strcat(port_2, port_arg);
        rate_2 = rate_arg;
        parityMode_2 = parity_arg;
        memset(&dcb, 0, sizeof(dcb));

        /* -------------------------------------------------------------------- */
        // set DCB to configure the serial port
        dcb.DCBlength = sizeof(dcb);

        /* ---------- Serial Port Config ------- */
        dcb.BaudRate = rate_2;

        switch (parityMode_2)
        {
        case spNONE:
            dcb.Parity = NOPARITY;
            dcb.fParity = 0;
            break;
        case spEVEN:
            dcb.Parity = EVENPARITY;
            dcb.fParity = 1;
            break;
        case spODD:
            dcb.Parity = ODDPARITY;
            dcb.fParity = 1;
            break;
        }


        dcb.StopBits = ONESTOPBIT;
        dcb.ByteSize = 8;

        dcb.fOutxCtsFlow = 0;
        dcb.fOutxDsrFlow = 0;
        dcb.fDtrControl = DTR_CONTROL_DISABLE;
        dcb.fDsrSensitivity = 0;
        dcb.fRtsControl = RTS_CONTROL_DISABLE;
        dcb.fOutX = 0;
        dcb.fInX = 0;

        /* ----------------- misc parameters ----- */
        dcb.fErrorChar = 0;
        dcb.fBinary = 1;
        dcb.fNull = 0;
        dcb.fAbortOnError = 0;
        dcb.wReserved = 0;
        dcb.XonLim = 2;
        dcb.XoffLim = 4;
        dcb.XonChar = 0x13;
        dcb.XoffChar = 0x19;
        dcb.EvtChar = 0;

        /* -------------------------------------------------------------------- */
        serial_handle_2 = CreateFile(port_2, GENERIC_READ | GENERIC_WRITE,
            0, NULL, OPEN_EXISTING, NULL, NULL);
        // opening serial port


        if (serial_handle_2 != INVALID_HANDLE_VALUE)
        {
            if (!SetCommMask(serial_handle_2, 0))
                erreur = 1;

            // set timeouts
            if (!SetCommTimeouts(serial_handle_2, &cto))
                erreur = 2;

            // set DCB
            if (!SetCommState(serial_handle_2, &dcb))
                erreur = 4;
        }
        else
        {
            erreur = 8;
            DWORD err = GetLastError();
        }
    }
    else
        erreur = 16;


    /* --------------------------------------------- */
    if (erreur != 0)
    {
        CloseHandle(serial_handle_2);
        serial_handle_2 = INVALID_HANDLE_VALUE;
    }
    return(erreur);
}

void SeriApp_COM_SendArray(char* buffer, int len)
{
    Reset_Buffer((uint8_t*)Receive.leng_array, 5);
    Reset_Buffer((uint8_t*)Receive.data_array, 50);

    unsigned long result;

    if (serial_handle_2 != INVALID_HANDLE_VALUE)
        WriteFile(serial_handle_2, buffer, len, &result, NULL);
}

void SeriApp_COM_disconnect(void)
{
    if (serial_handle_2 != INVALID_HANDLE_VALUE)
        CloseHandle(serial_handle_2);
    serial_handle_2 = INVALID_HANDLE_VALUE;
}

int  SeriApp_COM_getArray(char* buffer, int len)
{
    unsigned long read_nbr;

    read_nbr = 0;
    if (serial_handle_2 != INVALID_HANDLE_VALUE)
    {
        ReadFile(serial_handle_2, buffer, len, &read_nbr, NULL);
    }
    return((int)read_nbr);
}

uint16_t SeriApp_COM_getLength_ReceiveArray(char* lengthArray, uint16_t& length)
{
    length = 0;
    SeriApp_COM_getArray(lengthArray, 5);
    length = atoi(lengthArray);
    if (length == 0)
        return 0;
    return length;
}

uint8_t SeriApp_COM_getReceiveArray(char* ReceiveArray, uint16_t length)
{
    SeriApp_COM_getArray(ReceiveArray, length);
    return 1;
}

uint8_t  SeriApp_COM_sendAck_Length_Receive(void)
{
    SeriApp_COM_SendArray((char*)"Length Calculated", strlen((char*)"Length Calculated"));
    return 1;
}

uint8_t  SeriApp_COM_Open(char* com, int& baud)
{
    if (SeriApp_SetUp_COM( com, baud) == 0)
    {
        std::cout << "\nCOM open error";
        wait(2000);
        return 0;
    }

    if (SeriApp_Connect_COM( com, baud, spNONE) != 0)
    {
        std::cout << "\nCOM open error";
        wait(2000);
        return 0;
    }
    else
    {
        std::cout << "\nCOM opened successfully" << std::endl;
        wait(2000);
    }
    return 1;
}

uint8_t  SeriApp_COM_File_Open(void)
{
    uint8_t array[50];
    HexFile_Enter_SourceFile_Path_From_Console();
    if (HexFile_Open_File_HexSource() == 0)
        return 0;
    HexFile_Enter_DataFile_Path_From_Console();
    if (HexFile_Open_File_HexData() == 0)
        return 0;
    HexFile_Extract_Data_From_Line_InSourceFileTo_DataFile_and_CalculateSize_DataFile(array, 50, ui32_DataFileWordSize);
    std::cout << "Size of Data that will be loaded in microcontroller in word format is: " << ui32_DataFileWordSize << std::endl;
    //std::cout << "SurplusByte of the File hex is: " << unsigned(ui8_SurplusByte) << std::endl;
    HexFile_Close_File_HexSource();
    HexFile_Put_GetPointer_atBegin_DataFile();
    return 1;
}

uint8_t SeriApp_COM_Wait_For_ACK(char* ACK)
{
    //Reset_Buffer((uint8_t*)Receive.leng_array, 5);
    //Reset_Buffer((uint8_t*)Receive.data_array, 50);
    SeriApp_COM_getLength_ReceiveArray(Receive.leng_array, Receive.ui16_length);
    SeriApp_COM_getReceiveArray(Receive.data_array, Receive.ui16_length);
    if (strstr(Receive.data_array, ACK) != NULL)
        return 1;
    return 0;
}

uint8_t  SeriApp_COM_DataFile_Transfer(void)
{
    uint8_t ui8_HexDataArray[50] = {0x00};
    uint8_t ui8_LengthHexArray = 0;
    uint8_t ui8_SubArray[8] = {0x00};
    uint8_t ui8_WordHexArray = 0;
    //if (SeriApp_COM_Wait_For_ACK((char*)"start") == 0)
    //    return 0;
    Reset_Buffer((uint8_t*)Receive.leng_array, 5);
    Reset_Buffer((uint8_t*)Receive.data_array, 50);
    SeriApp_COM_Wait_For_ACK((char*)"start");
    std::cout << "Start loading process" << std::endl;
    std::cin.ignore(1);
    while (!DataFile.eof())
    {
        Reset_Buffer(ui8_HexDataArray, 50); // 50 la so phan tu co trong mang cua ui8_LineArray
        DataFile.getline((char*)ui8_HexDataArray, 50);
        //cout << ui8_HexDataArray << endl;
        ui8_LengthHexArray = strlen((char*)ui8_HexDataArray);
        ui8_WordHexArray = ui8_LengthHexArray / 8;
        for (uint8_t i = 0; i < ui8_WordHexArray; ++i)
        {
            HexFile_Extract_WordByWord_From_A_Line(ui8_HexDataArray+(i*8), ui8_SubArray);
            //std::cout << ui8_SubArray << std::endl;
            HexFile_Convert_A_Word_LittleEndian(ui8_SubArray);
            SeriApp_COM_SendArray((char*)ui8_SubArray, 8);
            if (SeriApp_COM_Wait_For_ACK((char*)"data") == 0)
            {
                std::cout << "data ack loi" << std::endl;
                return 0;
            }
            ui32_LoadedWord = ui32_LoadedWord + 1;
            ui32_LoadedPercent = (ui32_LoadedWord * 100) / ui32_DataFileWordSize;
            std::cout << "Loading: " << ui32_LoadedPercent << "%" << std::endl;
            Reset_Buffer(ui8_SubArray, 8);
        }
        Reset_Buffer(ui8_HexDataArray, 50);
    }
    std::cout << "Da load xong doi end ack thoi" << std::endl;
    wait(1000);
    SeriApp_COM_SendArray(END_PROCESS, 8);
    SeriApp_COM_Wait_For_ACK((char*)"endACK");
    return 1;
}

uint8_t  SeriApp_COM_File_Open2(void)
{
    uint8_t array[50] = {0x00};
    HexFile_Enter_SourceFile_Path_From_Console();
    if (HexFile_Open_File_HexSource() == 0)
        return 0;
    HexFile_Enter_DataFile_Path_From_Console();
    if (HexFile_Open_File_HexData() == 0)
        return 0;
    HexFile_Extract_Word_From_SourceFile(array, 50, ui32_DataFileWordSize);
    std::cout << "Size of Data that will be loaded in microcontroller in word format is: " << ui32_DataFileWordSize << std::endl;
    std::cout << "SurplusByte of the File hex is: " << unsigned(ui8_SurplusByte) << std::endl;
    HexFile_Close_File_HexSource();
    return 1; 
}

uint8_t  SeriApp_COM_SourceFile_StartingAddress(uint8_t& appZone, char* AddZone1, char* AddZone2)
{
    uint8_t array[50] = { 0x00 };
    HexFile_Enter_SourceFile_Path_From_Console();
    if (HexFile_Open_File_HexSource() == 0)
        return 0;
    SeriApp_COM_Find_AppZone(appZone, AddZone1, AddZone2);
    HexFile_Put_GetPointer_atBegin_SourceFile();
    HexFile_Close_File_HexSource();
    return 1;
}

uint8_t SeriApp_COM_Find_AppZone(uint8_t& appZone, char *AddZone1, char *AddZone2)
{
    uint8_t ui8_LineArray[50] = {0x00};
    uint8_t Array[10] = {0x00};
    uint8_t number = 0;
    while (!SourceFile.eof())
    {
        Reset_Buffer(ui8_LineArray, 50); // 50 la so phan tu co trong mang cua ui8_LineArray
        SourceFile.getline((char*)ui8_LineArray, 50);
        if (HexFile_Check_Record_Type(ui8_LineArray) == _EXTENDED_LINEAR_ADD_)
        {
            number = HexFile_Check_Byte_Data(ui8_LineArray);
            if (number != 0)
            {
                HexFile_Extract_Line_Data(Array, ui8_LineArray, number);
                if (strstr((char*)Array, AddZone1) != NULL)
                    return 1;
                else if (strstr((char*)Array, AddZone2) != NULL)
                    return 2;
                else
                    return 0;
            }
            break;
        }
    }
   // return 0;
}