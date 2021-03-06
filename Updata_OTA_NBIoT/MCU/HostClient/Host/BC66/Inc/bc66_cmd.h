#ifndef  __BC66_CMD_H
#define  __BC66_CMD_H

/*---------------------------------------------------------------------*/


#include "main.h"
#include "stdint.h"
#include "pin_config.h"
#include "my_lib.h"
#include "bc66_uart.h"

/*---------------------------------------------------------------------*/


/*typedef struct{
	uint8_t b;
}struct_Gdgdfgdf;

struct_Gdgdfgdf sTesst;
*/
/*----------------------------------------------------------------------*/
                            /* defination of macros */

/* defination macro that BC66 response AT command Fail */
#define BC66_RESP_FAIL 0

/* defination macro that BC66 response AT command Success*/
#define BC66_RESP_OK 1UL



/* defination macro of uart response process checking cycle */
#define UART_CHECK_CYCLE 50UL

/* defination macro of maximum time that process allow to not reset the module */
#define MAX_FAIL_TIME 10UL

/* defination macro of time that status of RESET_PIN keep in active state to reset BC66 */
#define RESET_KEEP_TIME 75UL

/* defination macro of time that status of PWRKEY_PIN keep in active state to power on BC66 module */
#define POWER_ON_KEEP_TIME 700UL


#define AT_RESP_TIMEOUT  10000

#define TCP_MAX_OPEN_FAIL 3


/*----------------------------------------------------------------------------------------------------*/
/* define data type BC66 */
typedef struct
{
  uint8_t *data;
	uint16_t ui16_data_len;
	uint32_t ui32_time_res;
} sType_bc66_data;

/* define AT command number */
typedef enum
{
	// Khoi dong BC66
	_PWR_ON_ = 0,
	_SYN_BAUDRATE_BC66_ ,
	_CHECK_PIN_BC66_,
	_SET_NB_BAND_BC66_,
	_OFF_SLEEP_BC66_,
	_CHECK_SIM_ATTACH_BC66_ ,
	_SET_PSD_CONECTION_,
	_FREE_LOCK_NB_FREQUENCY_,
	_ENABLE_WAKEUP_INDICATION_,
	_OFF_eDRX_ ,
	_SAVE_NVRAM_,
	
	//chua su dung phan nay
	_DISPLAY_CELLS_INFOR_BC66_ ,
	_CHECK_IP_ADDRESS_  ,
	
	// cai dat TCP
	_TCP_SET_DATA_TEXT_FORMAT_,
	_TCP_SET_VIEW_MODE_ ,
	_TCP_SET_SHOW_LENGTH_ ,
	
	// van hanh TCP
	_TCP_OPEN_1_,
	_TCP_OPEN_2_,
	_TCP_CLOSE_,

	// MQTT
	_MQTT_OPEN_1_,
    _MQTT_OPEN_2_,
	
	_MQTT_PUB_1_,
	_MQTT_PUB_2_,
	
	_MQTT_PINGREQ_1_ ,
	_MQTT_PINGREQ_2_,
	
	_MQTT_SUBSCRIBE_1_ ,
	_MQTT_SUBSCRIBE_2_,
	
	//Check loi
	_GET_ERROR_,
	
	// config and setting PSM parametter of module
	_REQUEST_PSM_NETWORK_,  // request NETWORK has PSM mode
	_CONFIG_SLEEP_MODE_,    // config disable sleep mode; enable light and deep sleep mode; enable light sleep mode only 
	_SETTING_PSM_,          // setting time device may sleep (Timer T3412) and time a device can be reachable (Timer T3324)
	_CHECK_PSM_REGISTRATION_ , // Check PSM registration net work of device   
	
	
	// go to Low Power MCU
	_MCU_LOW_PWR_,
	// trong de test
	_IDLE_,
} eType_AT_cmd;

typedef uint8_t (*func_callback_pointer)(void);   
typedef void (*func_handle_error)(void);

/* define struct of AT command include command and desired response and number place in set of AT (AT_cmd) */
typedef struct
{
  uint8_t ui8_cmd;
  sType_bc66_data dType_AT_req;
  sType_bc66_data dType_AT_res;
  func_callback_pointer FUNC;
} sType_AT_struct;	

typedef struct
{
	uint8_t ui8_fail_time;
	uint8_t ui8_reset_flag;
	uint8_t ui8_reset_rx_buf_flag;
} sType_reset_struct;

typedef struct 
{
	uint8_t ui8_ena_send;
	uint8_t ui8_send_done;
} sType_tcp_indicate;

typedef struct
{
	uint8_t ui8_pointer;
	uint8_t ui8_result;
	sType_tcp_indicate dType_tcp;
} sType_AT_command;

typedef struct
{
  sType_reset_struct dType_bc66_reset;
  volatile sType_uart_receive  dType_bc66_receive;
  sType_AT_command dType_AT;
} sType_BC66;

typedef enum
{
	_MQTT_SEND_SUCCESS_,
	_MQTT_OPEN_FAIL_,
	_MQTT_PUB_FAIL_,
	_MQTT_PING_FAIL_,
	_TCP_OPEN_FAIL_,
} eType_err_name;

typedef struct
{
	uint8_t ui8_reason_fail;
	uint8_t ui8_max_fail_time;
	func_handle_error FncHandler_Err;
} sType_reason_close;

typedef struct
{
 uint8_t options;
 sType_string choosen_type;
} sType_con_opt;

typedef enum
{
	_PROTOCOL_,
	_SERVER_,
	_PORT_,
} eType_bc66_con_opt;
/*----------------------------------------------------------------------*/
                         /* extern variables section*/    
												 
extern volatile uint32_t ui32_tick_count;

extern sType_BC66 dType_water_NB_IoT;



/*----------------------------------------------------------------------*/
                         /* declare function section */
												 
uint8_t BC66_Check_AT_Response ( uint8_t AT_command );
void BC66_Check_Reset ( void );
void BC66_Reset ( void );
uint8_t BC66_Excute_AT_Cmd ( uint8_t AT_command );
void BC66_Power_On ( void );
void BC66_Initialize_AT_Commands_Handler ( uint8_t current_cmd , uint8_t next_cmd );
uint8_t BC66_Check_Time_Out ( uint32_t mark , uint32_t cycle );
uint8_t BC66_Check_Ready_To_Send_Data_TCP ( void );
uint8_t BC66_Check_Send_Data_Success_TCP ( void );
uint8_t BC66_Send_Data_TCP_IP ( uint8_t *data , uint16_t len_data );
void BC66_Wait_Return_Wanted_Value_In_A_Interval ( uint8_t return_var , uint8_t wanted_value , uint32_t interval );
uint8_t BC66_Open_TCP_To_Server ( void );
uint8_t BC66_Open_MQTT (void);

void BC66_Handle_Reason_Close_TCP ( uint8_t reason , uint8_t internet_fail_time , func_handle_error tcp_close );
void BC66_Handler_TCP_Close_None_Err ( void );
void BC66_Handler_TCP_Close_MQTT_Max_Open_Fail (void);
void BC66_Handler_TCP_Close_MQTT_Max_Pub_Fail (void);
void BC66_Handler_TCP_Close_TCP_Max_Open_Fail (void);
void BC66_Handler_TCP_Close_MQTT_Max_Ping_Fail (void);
void BC66_TCP_Close_Scan_And_Handler_Error ( void );

uint8_t BC66_Check_MQTT_Receive ( void );

void BC66_WakeUp_DeepSleep ( void );
/*----------------------------------------------------------------------*/
#endif
