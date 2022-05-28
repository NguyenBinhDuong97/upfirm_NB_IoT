#include "main.h"
#include "stdio.h"
#include "string.h"
#include "gpio.h"
#include "usart.h"
#include "my_rtc.h"
#include "my_uart.h" 
#include "my_lib.h"
#include "stdlib.h"

char num_array[100];
/* hai ham nay duoc dung de chuyen tu dang so sang dang char */
//void itoa(int value, char* str, int base) {
//
//	static char num[] = "0123456789abcdefghijklmnopqrstuvwxyz";
//
//	char* wstr=str;
//
//	int sign;
//
//
//
//
//	// Validate base
//
//	if (base<2 || base>35){ *wstr='\0'; return; }
//
//
//
//	// Take care of sign
//
//	if ((sign=value) < 0) value = -value;
//
//
//
//
//	// Conversion. Number is reversed.
//
//	do *wstr++ = num[value%base]; while(value/=base);
//
//	if(sign<0) *wstr++='-';
//
//	*wstr='\0';
//
//
//
//	// Reverse string
//
//
//	strreverse(str,wstr-1);
//
//}
//void strreverse(char* begin, char* end) {
//
//	char aux;
//
//	while(end>begin)
//
//		aux=*end, *end--=*begin, *begin++=aux;
//
//}

/**
  * @brief   Search string in a source array
  * @param   src point to source array need to be searched
  * @param   src_len length of source array
  * @param   string point to string need to search in source array
  * @param   string_len length of string use to search
  * @retval  TRUE success finding
             FALSE fail finding
*/

uint8_t Search_String_In_Buffer ( uint8_t *src , uint16_t src_len , uint8_t *string , uint16_t string_len )
{
	uint8_t *pointer1;
	uint8_t *pointer2;
	uint16_t k = 0;
	
    pointer1 = src;
	pointer2 = string;
	uint16_t j = 0;
	for ( uint16_t m = 0 ; m < src_len ; m++ )
	{
		if ( *pointer1 == *pointer2 )
		{
			j = 0;
			k = 0;
			for ( j = 0 ; j < string_len ; j++ )
			{
				if ( *(pointer1) != *(pointer2+j) )
					break;
				else 
				{
					k++;
				}
					m++;
				  pointer1++;
			}
		}
		else
		pointer1++;
	}
	if ( k == (string_len) )
		return TRUE;
	else
		return FALSE;
}
/**
  * @brief Search and return begin location start of string need to find in a source array
*/
//uint8_t *pointer_result = NULL;
uint8_t *Search_String_Location_In_Buffer ( uint8_t *src , uint16_t src_len , uint8_t *string , uint16_t string_len )
{
//	pointer_result = NULL;
	static uint8_t *pointer_result = NULL;
	
	uint8_t *pointer1 = NULL;
	uint8_t *pointer2 = NULL;
	uint8_t m = 0;
	pointer1 = src;
	pointer2 = string;
	for ( uint16_t i=0 ; i < src_len ; ++i )
	{
		if ( *pointer1 == *pointer2 )
		{
			m++;
			pointer2 = string + m; 
			if ( m == string_len )
			{
			  pointer_result = src + i + 3;
			  return pointer_result;
		  }	
		}
		else
		{
			m = 0;
		}
		pointer1++;
 	}
	return NULL;
}

/**
  * @brief   Reset buffer that was pointed to
  * @param   uint8_t *buffer: Pointer point to buffer that need to reset
  * @retval  NULL
*/
void Reset_Buffer ( uint8_t *buffer , uint16_t length_buf )
{
	for ( uint16_t i = 0 ; i < length_buf ; i++ )
	{
		*(buffer+i) = 0x00;
	}
}

/** 
  * @brief   Change interger to char then put into an array
  * @param   buffer point to array that contain converted char elements
  * @param   numb the number need to convert
  * @param   length_buf the max elements of arrays
  * @retval  NULL
*/
void Interger_To_Char ( uint32_t numb , uint8_t *buffer , uint16_t length_buf  )
{
	Reset_Buffer ( buffer , length_buf );
  itoa ( numb , (char*)buffer , 10 );
}

void LPM_EnterStopMode ( void )
{
    BACKUP_PRIMASK();

    DISABLE_IRQ( );

//    MX_GPIO_DeInit();

    //    HAL_UART_MspDeInit(&huart1);
    //    HAL_UART_MspDeInit(&huart2);
    //  Uart_Deinit();

    //    HAL_I2C_MspDeInit(&hi2c1);
//    ADC_Desequence_Powerhungry_Channels();

//    HAL_ADC_DeInit( &hadc) ;

    /*clear wake up flag*/
    SET_BIT(PWR->CR, PWR_CR_CWUF);

    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);

    HAL_PWREx_EnableUltraLowPower();

    HAL_PWREx_EnableFastWakeUp(); 

    RESTORE_PRIMASK( );

    /* Enter Stop Mode */
    HAL_PWR_EnterSTOPMode ( PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI );
		
}

/**
  * @brief Exists Low Power Stop Mode
  * @note Enable the pll at 32MHz

  * @param none
  * @retval none
  */
void LPM_ExitStopMode( void )
{
    /* Disable IRQ while the MCU is not running on HSI */

    BACKUP_PRIMASK();

    DISABLE_IRQ( );

    HAL_PWREx_DisableUltraLowPower();

    HAL_PWREx_DisableFastWakeUp();

    /* After wake-up from STOP reconfigure the system clock */
    SystemClock_Config();
	
    MX_GPIO_Init();
    
    RESTORE_PRIMASK( );
}

void Low_Power_Stop_Mode_Init (void)
{
  /** If the application needs to disable the external clock before entering Stop mode, the HSEON
    * bit must be first disabled and the system clock switched to HSI16.
  **/
	 // CLEAR_BIT (RCC->CR, RCC_CR_HSEON);
	 // SystemClock_Config_2();
/** Macros configure the main internal regulator output voltage.
  *  When exiting Low Power Run Mode or during dynamic voltage scaling configuration,
  *  the reference manual recommends to poll PWR_FLAG_REGLP bit to wait for the regulator 
  *  to reach main mode (resp. to get stabilized) for a transition from 0 to 1. 
  *  Only then the clock can be increased.
 **/
	// __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
/* The ADC, DAC can also consume power in Stop mode, unless they are disabled before
   entering it. To disable them, the ADDIS bit in the ADC_CR register must be set to 1 and the
   ENx bit in the DAC_CR register must be written to 0.*/
     // SET_BIT (ADC->CCR, ADC_CR_ADDIS);
     // CLEAR_BIT (DAC->CR, DAC_CR_EN1|DAC_CR_EN2);
/** It is important to ensure that the WUF
  * wakeup flag is cleared. To perform this action, it is possible to call the
  * following macro 
**/
      __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
/** When exiting Stop mode by issuing an interrupt or a wakeup event,
  * MSI or HSI16 RCoscillator is selected as system clock depending 
  * the bit STOPWUCK in the RCC_CFGR register
**/
      SET_BIT(RCC->CFGR, RCC_CFGR_STOPWUCK);
			
			// turn off systick -> disable systick interrupt because systick interupt can wake up device
			HAL_SuspendTick();
/**
  * @brief Enters Stop mode. 
  * @note In Stop mode, all I/O pins keep the same state as in Run mode.
  * @note When exiting Stop mode by issuing an interrupt or a wakeup event,
  *        MSI or HSI16 RCoscillator is selected as system clock depending 
  *        the bit STOPWUCK in the RCC_CFGR register.
  * @note When the voltage regulator operates in low power mode, an additional
  *         startup delay is incurred when waking up from Stop mode. 
  *         By keeping the internal regulator ON during Stop mode, the consumption
  *         is higher although the startup time is reduced.
  * @note Before entering in this function, it is important to ensure that the WUF
  *       wakeup flag is cleared. To perform this action, it is possible to call the
  *       following macro : __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU)
  *
  * @param Regulator: Specifies the regulator state in Stop mode.
  *          This parameter can be one of the following values:
  *            @arg PWR_MAINREGULATOR_ON: Stop mode with regulator ON
  *            @arg PWR_LOWPOWERREGULATOR_ON: Stop mode with low power regulator ON
  * @param STOPEntry: Specifies if Stop mode in entered with WFI or WFE instruction.
  *          This parameter can be one of the following values:
  *            @arg PWR_STOPENTRY_WFI: Enter Stop mode with WFI instruction
  *            @arg PWR_STOPENTRY_WFE: Enter Stop mode with WFE instruction   
  * @retval None
  */
		HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
		
		// turn on systick again 
		HAL_ResumeTick();
}

void Send_RTC_Time_To_Terminal ( void )
{ 
	uint8_t ui8_hour_send = 0;
	uint8_t ui8_minutes_send = 0;
	uint8_t ui8_seconds_send = 0;
	uint8_t ui8Arr_Bufsend[50] = { 0x00 };
	uint8_t ui8Arr_hour[5] = { 0x00 };
	uint8_t ui8Arr_min[5] = { 0x00 };
	uint8_t ui8Arr_sec[5] = { 0x00 };
	
	HAL_RTC_GetTime ( &hrtc , &mTime , RTC_FORMAT_BIN );
	HAL_RTC_GetDate ( &hrtc , &mDate , RTC_FORMAT_BIN );
	
	ui8_hour_send =  mTime.Hours ;
	ui8_minutes_send = mTime.Minutes ;
	ui8_seconds_send = mTime.Seconds ;
	
	Interger_To_Char ( ui8_hour_send , ui8Arr_hour , 5 );
	Interger_To_Char ( ui8_minutes_send , ui8Arr_min , 5 );	
	Interger_To_Char ( ui8_seconds_send , ui8Arr_sec , 5 );
	
	strcat ( (char*)ui8Arr_Bufsend , "\r\nGio: " );
	strcat ( (char*)ui8Arr_Bufsend , (char*)ui8Arr_hour );
	strcat ( (char*)ui8Arr_Bufsend , ",Phut: " );
	strcat ( (char*)ui8Arr_Bufsend , (char*)ui8Arr_min );
	strcat ( (char*)ui8Arr_Bufsend , ",Giay: " );
	strcat ( (char*)ui8Arr_Bufsend , (char*)ui8Arr_sec );
	strcat ( (char*)ui8Arr_Bufsend , "\r\n" );
	
	HAL_UART_Transmit ( &huart2 , ui8Arr_Bufsend , strlen ( (char*)ui8Arr_Bufsend ) , 500 );
}
