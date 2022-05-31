#ifndef __MY_LIB_H__
#define __MY_LIB_H__

#include "main.h"
#include "rtc.h"
/*===========================================================================================================*/
#define TIME_A_DAY 86400
#define DAYS_IN_LEAP_YEAR                        ( ( uint32_t )  366U )
#define DAYS_IN_YEAR                             ( ( uint32_t )  365U )
#define SECONDS_IN_1DAY                          ( ( uint32_t )86400U )
#define SECONDS_IN_1HOUR                         ( ( uint32_t ) 3600U )
#define SECONDS_IN_1MINUTE                       ( ( uint32_t )   60U )
#define MINUTES_IN_1HOUR                         ( ( uint32_t )   60U )
#define HOURS_IN_1DAY                            ( ( uint32_t )   24U )

#define  DAYS_IN_MONTH_CORRECTION_NORM           ((uint32_t) 0x99AAA0 )
#define  DAYS_IN_MONTH_CORRECTION_LEAP           ((uint32_t) 0x445550 )

#define DIVC( X, N )                             ( ( ( X ) + ( N ) -1 )  / ( N ) )
/*=============================================================================================================*/
typedef struct 
{
    uint8_t hour;  // read hour
    uint8_t min;   // read minute
    uint8_t sec;   // read second
    uint32_t SubSeconds;
    uint8_t day;   // read day
    uint8_t date;  // read date
    uint8_t month; // read month
    uint8_t year;  // read year
    
} ST_TIME_FORMAT;

/*=============================================================================================================*/
extern RTC_TimeTypeDef mTime;
extern RTC_DateTypeDef mDate;
extern RTC_AlarmTypeDef mAlarm;
extern uint32_t ui32_alarm_freq;
extern RTC_AlarmTypeDef svAlarm;

extern const uint8_t DaysInMonth[];
extern const uint8_t DaysInMonthLeapYear[]; 
/*============================================================================================================*/

void RTC_Init_Alarm_Element_In_My_Global_Variable ( void );
void RTC_Caculated_Alarm_A ( uint32_t freq );
uint16_t RTC_Change_Time_In_Sec_To_Mins ( uint32_t time_in_sec );
uint8_t RTC_Change_Time_In_Sec_To_Hours ( uint32_t time_in_sec );

void set_alarm_defaut_time (uint8_t time_hour,uint8_t time_min, uint8_t time_sec);
void set_alarm_for_time (uint8_t time_hour,uint8_t time_min, uint8_t time_sec);
void BC_RTC_StopAlarm( void );

uint32_t HW_RTC_GetCalendarValue(void);
void Convert_Time_GMT (ST_TIME_FORMAT* sRTC_Check, uint8_t GMT);

void Get_RTC(void);
uint8_t Check_update_Time(ST_TIME_FORMAT *sRTC_temp);
uint8_t Set_RTC_2(ST_TIME_FORMAT sRTC_Var);
void Set_AlarmA_With_Interrupt_Duong ( uint8_t time_hour,uint8_t time_min, uint8_t time_sec );
void set_alarm_for_time_Duong (uint8_t time_hour,uint8_t time_min, uint8_t time_sec);
#endif