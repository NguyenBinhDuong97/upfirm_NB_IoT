#include "SerialApp.h"


//COM::COM(char* Com_Array, int baudrate)
//{
//    Com_name = Com_Array;
//    Com_baudrate = baudrate;
//}

char              port_2[20] = "\\\\.\\";                      // port name "com1",...
int               rate_2 = 0;                          // baudrate
serial_parity     parityMode_2 = spNONE;
HANDLE            serial_handle_2;                 // ...


uint8_t SeriApp_SetUp_COM( char *ComArray, int& baudrate )
{
	std::cout << "Which COM do you want to open: ";
	std::cin.getline( ComArray, 7 );
    std::cout << std::endl << "Baud rate: ";
    //std::cin.ignore(1);
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

