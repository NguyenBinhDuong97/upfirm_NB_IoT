#ifndef   __WM_PIN_CONFIG_H
#define   __WM_PIN_CONFIG_H

#include "main.h"
#include "my_lib.h"


/*----------------------------------------------*/
#define V1 YES
#define V2 NO
#define V3 NO
#define V4 NO
/*----------------------------------------------*/
#if ( V1 == YES )
   #define uart_bc66         huart1
   #define uart_debug        huart2
   #define DEBUG_TX          GPIO_PIN_2
   #define DEBUG_TX_Port     GPIOA
   #define DEBUG_RX          GPIO_PIN_3
   #define DEBUG_RX_Port     GPIOA
   #define PWRKEY_BC66       GPIO_PIN_7
   #define PWRKEY_BC66_Port  GPIOA
   #define RESET_BC66        GPIO_PIN_0
   #define RESET_BC66_Port   GPIOB
   #define SIM_RI            GPIO_PIN_1
   #define SIM_RI_Port       GPIOB
   #define PSM_WKUP          GPIO_PIN_8
   #define PSM_WKUP_Port     GPIOA
	 #define BC66_TX           GPIO_PIN_9
	 #define BC66_TX_Port      GPIOA
	 #define BC66_RX           GPIO_PIN_10
	 #define BC66_Port         GPIOA
#endif
/*---------------------------------------------------------------------------------------------------------------*/
#if  ( V2 == YES )
#endif
/*---------------------------------------------------------------------------------------------------------------*/
#define    BC66_RESET_DOWN       HAL_GPIO_WritePin ( RESET_BC66_Port , RESET_BC66 , GPIO_PIN_SET )
#define    BC66_RESET_UP         HAL_GPIO_WritePin ( RESET_BC66_Port , RESET_BC66 , GPIO_PIN_RESET )
/*---------------------------------------------------------------------------------------------------------------*/
#define    BC66_PWR_KEY_DOWN     HAL_GPIO_WritePin ( PWRKEY_BC66_Port , PWRKEY_BC66 , GPIO_PIN_SET )
#define    BC66_PWR_KEY_UP       HAL_GPIO_WritePin ( PWRKEY_BC66_Port , PWRKEY_BC66 , GPIO_PIN_RESET )
/*----------------------------------------------------------------------------------------------------------------*/
#define    BC66_WKUP_DOWN        HAL_GPIO_WritePin ( PSM_WKUP_Port , PSM_WKUP , GPIO_PIN_RESET )
#define    BC66_WKUP_UP          HAL_GPIO_WritePin ( PSM_WKUP_Port , PSM_WKUP , GPIO_PIN_SET )

#endif