#ifndef __my_lib_H__
#define __my_lib_H__

#ifdef __cplusplus
extern "C" {
#endif
/*===================================================================================================*/
#include "main.h"
/*===================================================================================================*/
#define STOP 0

#define COUNTINUE 1

#define FAIL 0

#define SUCCESS 1

#define TRUE 1

#define FALSE 0
/* defination macro that indicate status of flag */
#define ON 1UL

/* defination macro that indicate staus of flag */
#define OFF 0UL

/* defination macro that indicate that BC66 response is match with compared buffer */
#define MATCH 1UL

/* defination macro that indicate that BC66 response is not match with compared buffer */
#define NOT_MATCH 0UL

#define YES 1

#define NO 0


#define     BACKUP_PRIMASK()            uint32_t primask_bit= __get_PRIMASK()
#define     DISABLE_IRQ()               __disable_irq()
#define     ENABLE_IRQ()                __enable_irq()
#define     RESTORE_PRIMASK()           __set_PRIMASK(primask_bit)
/*==============================================================================================================*/

typedef struct
{
	uint8_t *pData;
	uint16_t u16_len_data;
} sType_string;



/*==============================================================================================================*/
extern char num_array[100];
/*==============================================================================================================*/
/*created function*/
 /* Ansi C "itoa" based on Kernighan & Ritchie's "Ansi C":
	(chuyen so thanh chuoi)
 */
void strreverse(char* begin, char* end) ;
//void itoa(int value, char* str, int base);
uint8_t Search_String_In_Buffer ( uint8_t *src , uint16_t src_len , uint8_t *string , uint16_t string_len );
void     Reset_Buffer ( uint8_t *buffer , uint16_t length_buf );
void     Interger_To_Char ( uint32_t numb , uint8_t *buffer , uint16_t length_buf );
uint8_t *Search_String_Location_In_Buffer (uint8_t *src , uint16_t src_len , uint8_t *string , uint16_t string_len);
uint8_t *Search_String_Location_In_Buffer_2 (uint8_t *src , uint16_t src_len , uint8_t *string , uint16_t string_len);
uint8_t  Interger_To_Hex (int decimal, uint8_t *hexcimal);

void LPM_EnterStopMode ( void );
void LPM_ExitStopMode( void );
void Low_Power_Stop_Mode_Init ( void );
void Send_RTC_Time_To_Terminal ( void );
uint8_t Convert_Char_To_Hex (uint8_t *array, uint16_t arrayLength);

#ifdef __cplusplus
}
#endif

#endif
