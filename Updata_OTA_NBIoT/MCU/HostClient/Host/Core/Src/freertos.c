/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "MQTTPacket.h"
#include "MQTTFormat.h"
#include "MQTT.h"
#include "bc66_cmd.h"
#include "bc66_uart.h"
#include "MQTTConnect.h"
#include "stdio.h"
#include "string.h"
#include "queue.h"
#include "my_freeRTOS.h"
#include "uartWinnApp.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
extern QueueHandle_t qBC66step;
extern QueueHandle_t qPrintQueue;
uint8_t ping_flag = OFF;
//uint8_t BC66send_flag = OFF;
//uint8_t BC
BaseType_t xStatus;

/* USER CODE END Variables */
osThreadId BC66_TaskHandle;
osThreadId UART_BC66Handle;
osThreadId UART_WinApp_TasHandle;
osThreadId GateKeeperHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void Start_BC66_Task(void const * argument);
void UART_BC66_Task(void const * argument);
void StartUART_WinApp_Task(void const * argument);
void StartGateKeeper(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
   qBC66step = xQueueCreate( 50, sizeof(uint8_t) );
   qPrintQueue = xQueueCreate( 5, sizeof(sType_string) );
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of BC66_Task */
  osThreadDef(BC66_Task, Start_BC66_Task, osPriorityNormal, 0, 128);
  BC66_TaskHandle = osThreadCreate(osThread(BC66_Task), NULL);

  /* definition and creation of UART_BC66 */
  osThreadDef(UART_BC66, UART_BC66_Task, osPriorityNormal, 0, 128);
  UART_BC66Handle = osThreadCreate(osThread(UART_BC66), NULL);

  /* definition and creation of UART_WinApp_Tas */
  osThreadDef(UART_WinApp_Tas, StartUART_WinApp_Task, osPriorityNormal, 0, 128);
  UART_WinApp_TasHandle = osThreadCreate(osThread(UART_WinApp_Tas), NULL);

  /* definition and creation of GateKeeper */
  osThreadDef(GateKeeper, StartGateKeeper, osPriorityAboveNormal, 0, 128);
  GateKeeperHandle = osThreadCreate(osThread(GateKeeper), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_Start_BC66_Task */
/**
  * @brief  Function implementing the BC66_Task thread.
  * @param  argument: Not used
  * @retval None
  */

int length_test = 0;
uint8_t  MQTT_process_result = 0;
uint8_t  test123 = 0;

uint8_t MQTT_test_pingreq_array[200] = {0x00};

uint32_t dem_gui = 0;
uint32_t gui_thanh_cong = 0;

uint8_t debug_send[200] = "So lan gui di:";

uint8_t event;
/* USER CODE END Header_Start_BC66_Task */
void Start_BC66_Task(void const * argument)
{
  /* USER CODE BEGIN Start_BC66_Task */
 // dType_water_NB_IoT.dType_bc66_reset.ui8_fail_time == MAX_FAIL_TIME;
  event = _PWR_ON_;
  xStatus = xQueueSendToBack( qBC66step, &event, 0 );
  if (xStatus != pdPASS)
  {
	  BC66_UART_Send_Data_To_Terminal ((uint8_t*)"loi push\r\n", 10);
  }
  /* Infinite loop */
  for(;;)
  {
		BC66_Check_Reset();
		xQueueReceive( qBC66step, &dType_water_NB_IoT.dType_AT.ui8_pointer, 0 );
		switch ( dType_water_NB_IoT.dType_AT.ui8_pointer )
		{
			case _PWR_ON_:
				BC66_Power_On ();
				osDelay(5000);
				break;
			case _SYN_BAUDRATE_BC66_:
//				osDelay ( 10000 );
			    BC66_Initialize_AT_Commands_Handler ( _SYN_BAUDRATE_BC66_ , _OFF_SLEEP_BC66_ );

//				HAL_UART_Transmit(&uart_bc66, (uint8_t*)"AT\r", 3, 500);
//				osDelay(2000);
//				HAL_UART_Transmit(&uart_bc66, (uint8_t*)"AT+QSCLK=0\r", 12, 500);
//				osDelay(2000);
//				HAL_UART_Transmit(&uart_bc66, (uint8_t*)"AT+CPIN?\r", 9, 500);
//				osDelay(2000);
//				HAL_UART_Transmit(&uart_bc66, (uint8_t*)"AT+QBAND=1,3\r", 13, 500);
//				osDelay(2000);
				break;
			case _OFF_SLEEP_BC66_:
				BC66_Initialize_AT_Commands_Handler ( _OFF_SLEEP_BC66_ , _CHECK_PIN_BC66_ );
				break;
			case _CHECK_PIN_BC66_:
				BC66_Initialize_AT_Commands_Handler ( _CHECK_PIN_BC66_ , _SET_NB_BAND_BC66_ );
			    break;
			case _SET_NB_BAND_BC66_:
				BC66_Initialize_AT_Commands_Handler ( _SET_NB_BAND_BC66_ , _CHECK_SIM_ATTACH_BC66_ );
				break;
			case _CHECK_SIM_ATTACH_BC66_:
				BC66_Initialize_AT_Commands_Handler ( _CHECK_SIM_ATTACH_BC66_ , _SET_PSD_CONECTION_ );
			    break;
			case _SET_PSD_CONECTION_:
				BC66_Initialize_AT_Commands_Handler ( _SET_PSD_CONECTION_ , _FREE_LOCK_NB_FREQUENCY_ );
			    break;
			case _FREE_LOCK_NB_FREQUENCY_:
				BC66_Initialize_AT_Commands_Handler ( _FREE_LOCK_NB_FREQUENCY_ , _ENABLE_WAKEUP_INDICATION_ );
			    break;
			case _ENABLE_WAKEUP_INDICATION_:
                BC66_Initialize_AT_Commands_Handler ( _ENABLE_WAKEUP_INDICATION_ , _OFF_eDRX_ );
			    break;
			case _OFF_eDRX_:
				BC66_Initialize_AT_Commands_Handler ( _OFF_eDRX_ , _SAVE_NVRAM_ );
			    break;
			case _SAVE_NVRAM_:
			    BC66_Initialize_AT_Commands_Handler ( _SAVE_NVRAM_ , _TCP_SET_DATA_TEXT_FORMAT_ );
			    break;
			case _TCP_SET_DATA_TEXT_FORMAT_:
				BC66_Initialize_AT_Commands_Handler ( _TCP_SET_DATA_TEXT_FORMAT_ , _TCP_SET_VIEW_MODE_ );
			    break;
			case _TCP_SET_VIEW_MODE_:
				BC66_Initialize_AT_Commands_Handler ( _TCP_SET_VIEW_MODE_ , _TCP_SET_SHOW_LENGTH_ );
			    break;
			case _TCP_SET_SHOW_LENGTH_:
				BC66_Initialize_AT_Commands_Handler ( _TCP_SET_SHOW_LENGTH_ , _TCP_OPEN_1_ );
				break;
			case _TCP_OPEN_1_:
			    BC66_Initialize_AT_Commands_Handler ( _TCP_OPEN_1_ , _TCP_OPEN_2_ );
				break;
			case _TCP_OPEN_2_:
			    BC66_Initialize_AT_Commands_Handler ( _TCP_OPEN_2_ , _MQTT_OPEN_1_ );
				break;
			case _MQTT_OPEN_1_:
				BC66_Initialize_AT_Commands_Handler ( _MQTT_OPEN_1_ , _MQTT_OPEN_2_ );
				break;
			case _MQTT_OPEN_2_:
				BC66_Initialize_AT_Commands_Handler ( _MQTT_OPEN_2_ , _IDLE_ );
				Push_Message_to_Queue( &Connect_String );
			    break;
			case _MQTT_PUB_1_:
				BC66_Initialize_AT_Commands_Handler ( _MQTT_PUB_1_ , _MQTT_PUB_2_ );
			    break;
			case _MQTT_PUB_2_:
				BC66_Initialize_AT_Commands_Handler ( _MQTT_PUB_2_ , _IDLE_ );
			    break;
			case _MQTT_PINGREQ_1_:
				ping_flag = ON;
				BC66_Initialize_AT_Commands_Handler ( _MQTT_PINGREQ_1_ , _MQTT_PINGREQ_2_ );
			    break;
			case _MQTT_PINGREQ_2_:
				BC66_Initialize_AT_Commands_Handler ( _MQTT_PINGREQ_2_ ,  _IDLE_ );
			    break;
			case _MQTT_SUBSCRIBE_1_:
			    BC66_Initialize_AT_Commands_Handler ( _MQTT_SUBSCRIBE_1_ , _MQTT_SUBSCRIBE_2_ );
				break;
			case _MQTT_SUBSCRIBE_2_:
				BC66_Initialize_AT_Commands_Handler ( _MQTT_SUBSCRIBE_2_ , _IDLE_ );
				break;
			case _TCP_CLOSE_:
			  if ( BC66_Excute_AT_Cmd ( _TCP_CLOSE_ ) == BC66_RESP_OK )
				{
					dType_MQTT_watermetter.ui8_start_keepalive = OFF;
					// test send stable lap gui di gui lai
                    BC66_TCP_Close_Scan_And_Handler_Error ();
				    BC66_UART_Send_Data_To_Terminal ( (uint8_t*)"da vao TCP CLOSE\r\n" , strlen ( (char*)"da vao TCP CLOSE\r\n" ) );
				}
				else
					dType_water_NB_IoT.dType_bc66_reset.ui8_fail_time++;
				break;

             /* PSM setting and operate zone*/
//			case _REQUEST_PSM_NETWORK_:
//				BC66_Initialize_AT_Commands_Handler ( _REQUEST_PSM_NETWORK_ , _CONFIG_SLEEP_MODE_ );
//				break;
//			case _CONFIG_SLEEP_MODE_:
//				BC66_Initialize_AT_Commands_Handler ( _CONFIG_SLEEP_MODE_ , _SETTING_PSM_ );
//			  break;
//			case _SETTING_PSM_:
//				BC66_Initialize_AT_Commands_Handler ( _SETTING_PSM_ , _CHECK_PSM_REGISTRATION_ );
//				break;
//			case _CHECK_PSM_REGISTRATION_:
//				BC66_Initialize_AT_Commands_Handler ( _CHECK_PSM_REGISTRATION_ , _MCU_LOW_PWR_ );
//				break;
//			case _MCU_LOW_PWR_:
//				Low_Power_Stop_Mode_Init();
//				break;

			case _IDLE_:
				break;
		}
		MQTT_Check_Keepalive_Time();
		osDelay ( 50 );
  }
  /* USER CODE END Start_BC66_Task */
}

/* USER CODE BEGIN Header_UART_BC66_Task */
/**
* @brief Function implementing the UART_BC66 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_UART_BC66_Task */
void UART_BC66_Task(void const * argument)
{
  /* USER CODE BEGIN UART_BC66_Task */
  uint32_t uart_mark_time = 0;
  uart_mark_time = ui32_tick_count;
  /* Infinite loop */
  for(;;)
  {
		if ( BC66_Check_Time_Out ( uart_mark_time , UART_CHECK_CYCLE ) == TRUE ) //xu ly uart: k bi miss du lieu -> <2 lan req tu ser
		{
			uart_mark_time = ui32_tick_count;
			dType_water_NB_IoT.dType_AT.ui8_result = NOT_MATCH;

		   if ( (BC66_UART_Check_Receive_Process_Status() == DONE) && (CountReceive_u16 !=0) )
		   {
				dType_water_NB_IoT.dType_AT.ui8_result = BC66_Check_AT_Response ( dType_water_NB_IoT.dType_AT.ui8_pointer );
				Push_BC66_Message_to_Queue();
			    if (sub_flag == ON)
				BC66_Check_MQTT_Receive ();
			    BC66_UART_Clear_BC66_Data ();
				CountReceive_u16 = 0;
		 /*----------------------------------------------*/
			// kiem tra va thuc hien cac tac vu URC gui ve
//				if ( dType_water_NB_IoT.dType_bc66_reset.ui8_reset_rx_buf_flag == NO )
//				{
////				  dType_water_NB_IoT.dType_AT.dType_tcp.ui8_ena_send = BC66_Check_Ready_To_Send_Data_TCP();
////				  dType_water_NB_IoT.dType_AT.dType_tcp.ui8_send_done = BC66_Check_Send_Data_Success_TCP();
////				  MQTT_Check_Packages_Result ();
//				}
//	      /*----------------------------------------------*/
//	            else
//				{
//	              if (sub_flag == ON)
//				  BC66_Check_MQTT_Receive ();
////			  BC66_UART_Send_Data_To_Terminal ( (uint8_t*)dType_water_NB_IoT.dType_bc66_receive.ui8buf_rx , CountReceive_u16 );
//				  Push_BC66_Message_to_Queue();
//			      BC66_UART_Clear_BC66_Data ();
//				  CountReceive_u16 = 0;
//				}
//			den_bao_hieu = COUNTINUE;
		   }
		 }
  }
  /* USER CODE END UART_BC66_Task */
}

/* USER CODE BEGIN Header_StartUART_WinApp_Task */
/**
* @brief Function implementing the UART_WinApp_Tas thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartUART_WinApp_Task */
void StartUART_WinApp_Task(void const * argument)
{
  /* USER CODE BEGIN StartUART_WinApp_Task */
  uint32_t Win_uart_mark_time = 0;
  Win_uart_mark_time = ui32_tick_count;
  /* Infinite loop */
  for(;;)
  {
      if ( WinApp_Check_Time_Out ( Win_uart_mark_time , UART_CHECK_CYCLE ) == TRUE ) //xu ly uart: k bi miss du lieu -> <2 lan req tu ser
	  {
	      Win_uart_mark_time = ui32_tick_count;
		  if ((WinApp_UART_Check_Receive_Process_Status() == DONE) && (WinUart.numb_rec !=0))
		  {
              WinApp_Handle_UART_Receive();
              WinApp_Reset();
		  }
      }
    osDelay(10);
  }
  /* USER CODE END StartUART_WinApp_Task */
}

/* USER CODE BEGIN Header_StartGateKeeper */
/**
* @brief Function implementing the GateKeeper thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartGateKeeper */
void StartGateKeeper(void const * argument)
{
  /* USER CODE BEGIN StartGateKeeper */
  sType_string pcMessageToPrint;
  /* Infinite loop */
  for(;;)
  {
	xQueueReceive( qPrintQueue, &pcMessageToPrint, portMAX_DELAY );
	UART_Send_Data_To_Terminal (&pcMessageToPrint);
    osDelay(1);
  }
  /* USER CODE END StartGateKeeper */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

