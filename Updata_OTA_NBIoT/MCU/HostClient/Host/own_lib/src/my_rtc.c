#include "main.h"
#include "my_rtc.h"
#include "rtc.h"
#include "my_lib.h"

ST_TIME_FORMAT              sRTC;
RTC_TimeTypeDef 	        sRTCTime;
RTC_DateTypeDef 	        sRTCDate;
RTC_TimeTypeDef 	        sSetTime;
RTC_DateTypeDef 	        sSetDate;
ST_TIME_FORMAT            sRTC_Timer;

RTC_TimeTypeDef mTime = {0};
RTC_DateTypeDef mDate = {0};
RTC_AlarmTypeDef mAlarm = {0};

RTC_AlarmTypeDef svAlarm = {0};
RTC_AlarmTypeDef sAlarm = {0};

uint32_t ui32_alarm_freq = 6; // alarm frequency in seconds

const uint8_t DaysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
const uint8_t DaysInMonthLeapYear[] = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };


/*======================================  my own functions   =========================================================================================*/

///**
//  * @brief   Convert time ?n seconds to hours
//  * @param   time_in_sec time in seconds
//  * @retval  Hour
//*/
//uint8_t RTC_Change_Time_In_Sec_To_Hours ( uint32_t time_in_sec )
//{
//	uint8_t ui8_Hour = 0;
//	ui8_Hour = time_in_sec/3600;
//	return ui8_Hour;
//}

///**
//  * @brief  Convert time in second into minutes
//  * @param  time_in_sec time in sec
//  * @retval Minutes
//*/
//uint16_t RTC_Change_Time_In_Sec_To_Mins ( uint32_t time_in_sec )
//{
//	uint16_t ui16_Mins = 0;
//	ui16_Mins = time_in_sec/60;
//  return ui16_Mins;	
//}

///**
//  * @brief   Caculated time of Alarm A
//  * @param   freq fequency time Alarm 
//  * @retval  NONE
//*/
//void RTC_Caculated_Alarm_A ( uint32_t freq )
//{
//	uint32_t ui32_time_in_sec = 0;    // real time in second
//	uint32_t ui32_alarm_time_in_sec = 0; // time alarm turn on in second
//	ui32_time_in_sec = ui32_time_in_sec + ( (RTC_Bcd2ToByte(mTime.Hours)) * 3600 );
//	ui32_time_in_sec = ui32_time_in_sec + ( (RTC_Bcd2ToByte(mTime.Minutes)) * 60 );
//	ui32_time_in_sec = ui32_time_in_sec + mTime.Seconds;
//	ui32_alarm_time_in_sec = ui32_time_in_sec + freq;
//	if ( ui32_alarm_time_in_sec >= TIME_A_DAY )
//	{
//		ui32_alarm_time_in_sec = ui32_alarm_time_in_sec - TIME_A_DAY;
//	  mAlarm.AlarmTime.Hours = ui32_alarm_time_in_sec/3600;
//		mAlarm.AlarmTime.Minutes = ( ui32_alarm_time_in_sec - (mAlarm.AlarmTime.Hours * 3600) )/60;
//		mAlarm.AlarmTime.Seconds = ui32_alarm_time_in_sec - ( mAlarm.AlarmTime.Hours * 3600 ) -  ( mAlarm.AlarmTime.Minutes*60 );	
//	}
//	else
//	{
//    mAlarm.AlarmTime.Hours = ui32_alarm_time_in_sec/3600;
//		mAlarm.AlarmTime.Minutes = ( ui32_alarm_time_in_sec - (mAlarm.AlarmTime.Hours * 3600) )/60;
//		mAlarm.AlarmTime.Seconds = ui32_alarm_time_in_sec - ( mAlarm.AlarmTime.Hours * 3600 ) -  ( mAlarm.AlarmTime.Minutes*60 );
//	}
//}

///**
//  * @brief  Set Alarm Element in my global variable
//  * @param  NONE
//  * @retval NONE
//*/
//void RTC_Init_Alarm_Element_In_My_Global_Variable ( void )
//{
//  mAlarm.AlarmTime.Hours = 0x0;
//  mAlarm.AlarmTime.Minutes = 0x0;
//  mAlarm.AlarmTime.Seconds = 0x0;
//  mAlarm.AlarmTime.SubSeconds = 0x0;
//  mAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
//  mAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
//  mAlarm.AlarmMask = RTC_ALARMMASK_HOURS|RTC_ALARMMASK_MINUTES
//                              |RTC_ALARMMASK_SECONDS;
//  mAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_SS14_3;
//  mAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_WEEKDAY;
//  mAlarm.AlarmDateWeekDay = 1;
//  mAlarm.Alarm = RTC_ALARM_A;
//}

///**
//  * @brief Set AlarmA 
//*/

//void Set_AlarmA_With_Interrupt ( RTC_HandleTypeDef *hrtc , RTC_TimeTypeDef *sTime , RTC_AlarmTypeDef *sAlarm , uint32_t sec_freq )
//{
//	HAL_RTC_DeactivateAlarm( hrtc , RTC_ALARM_A );
//	RTC_Caculated_Alarm_A( sec_freq );
//	HAL_RTC_SetTime( hrtc , sTime , RTC_FORMAT_BCD );
//	HAL_RTC_SetAlarm_IT( hrtc , sAlarm , RTC_FORMAT_BCD );
//}
/*============================ now on i will get use library of Sao Viet company because of time ===================================================*/

/**
  * @brief   Get RTC time and put it in own defined struct
  * @param   NONE
  * @retval  NONE
*/
void Get_RTC(void)
{
  HAL_RTC_GetTime(&hrtc,&sRTCTime,FORMAT_BIN);
  HAL_RTC_GetDate(&hrtc,&sRTCDate,FORMAT_BIN);
  sRTC.SubSeconds = sRTCTime.SubSeconds;
  sRTC.sec 	      = sRTCTime.Seconds;
  sRTC.min 	      = sRTCTime.Minutes;
  sRTC.hour 	    = sRTCTime.Hours;
  sRTC.day 	      = sRTCDate.WeekDay;
  sRTC.date 	    = sRTCDate.Date;
  sRTC.month 	    = sRTCDate.Month;
  sRTC.year 	    = sRTCDate.Year;
}

/**
  * @brief   Check RTC variables (when need to set RTC from code or any thing else we need to check that RTC is correct format or not) to set to RTC
  * @param   sRTC_temp point to struct of RTC time that need to set RTC
  * @retval  1 TRUE this struct is correct form and can apply to set RTC
             0 this struct is not correct form and cant apply to set RTC
*/
uint8_t Check_update_Time(ST_TIME_FORMAT *sRTC_temp)
{
	if (sRTC_temp->sec > 60) return 0;
	if (sRTC_temp->min > 60) return 0;
	if (sRTC_temp->hour > 23) return 0;
	if (sRTC_temp->date > 31) return 0;
	if (sRTC_temp->month > 12) return 0;
	if (sRTC_temp->year > 99) return 0;
	return 1;
}

/**
  * @brief   Set RTC time
  * @param   sRTC_Var struct of own defined RTC time
  * @retval  1 Set done
             0 Set fail 
*/
uint8_t Set_RTC_2(ST_TIME_FORMAT sRTC_Var)
{
    RTC_TimeTypeDef 		sRTCTime_Temp = {0};
    RTC_DateTypeDef 		sRTCDate_Temp = {0};

    if(Check_update_Time(&sRTC_Var) == TRUE)
    {
        sRTCTime_Temp.SubSeconds     = sRTC_Var.SubSeconds;
        sRTCTime_Temp.Seconds 	     = sRTC_Var.sec;
        sRTCTime_Temp.Minutes        = sRTC_Var.min;
        sRTCTime_Temp.Hours 		     = sRTC_Var.hour;
        sRTCDate_Temp.WeekDay 	     = 2;
        sRTCDate_Temp.Date 		       = sRTC_Var.date;
        sRTCDate_Temp.Month 		     = sRTC_Var.month;
        sRTCDate_Temp.Year 		       = sRTC_Var.year;

        HAL_RTC_SetTime(&hrtc,&sRTCTime_Temp,FORMAT_BIN);
        HAL_RTC_SetDate(&hrtc,&sRTCDate_Temp,FORMAT_BIN);
        return 1;
    }
    return 0;
}

/**
  * @brief   set alarm for RTC (even with param hour to set > 24 hour, min to set > 60min , sec to set > 60 sec)
  * @param   time_hour hour set Alarm 
  * @param   time_min  minute set Alarm
  * @param   time_sec  sec set Alarm
  * @retval  NONE
*/
void set_alarm_defaut_time (uint8_t time_hour,uint8_t time_min, uint8_t time_sec)
{
    if(time_hour >= 24) time_hour = time_hour%24;
    if(time_min >= 60) time_min = time_min%60;
    if(time_sec >= 60) time_sec = time_sec%60;
    
	svAlarm.AlarmTime.Seconds=time_sec;
	svAlarm.AlarmTime.Minutes=time_min;
	svAlarm.AlarmTime.Hours=time_hour;
	if (HAL_RTC_SetAlarm_IT(&hrtc, &svAlarm, RTC_FORMAT_BIN) != HAL_OK)
	{
		Error_Handler();
	}
}

/**
  * @brief   set alarm with param is interval of time hour, minutes, sec
  * @param   time_hour hour interval time to next Alarm
  * @param   time_min minute interval time to next Alarm 
  * @param   time_sec seccond interval time to next Alarm
  * @retval  NONE
*/ 
void set_alarm_for_time (uint8_t time_hour,uint8_t time_min, uint8_t time_sec)
{
  uint16_t Temp_Sec = 0;
  uint16_t Temp_min = 0;
	Get_RTC(); 
  Temp_Sec = sRTC.sec + time_sec;
  Temp_min = sRTC.min + time_min;
	sAlarm.AlarmTime.Seconds  = Temp_Sec%60;
	sAlarm.AlarmTime.Minutes  = (Temp_min+(Temp_Sec/60))%60;
	sAlarm.AlarmTime.Hours=((sRTC.hour+time_hour)+((Temp_min+(Temp_Sec/60))/60))%24;
	
	if (HAL_RTC_SetAlarm_IT(&hrtc, &svAlarm, RTC_FORMAT_BIN) != HAL_OK)
	{
		Error_Handler();
	}
}

/**
  * @brief   set alarm with param is interval of time hour, minutes, sec
  * @param   time_hour hour interval time to next Alarm
  * @param   time_min minute interval time to next Alarm 
  * @param   time_sec seccond interval time to next Alarm
  * @retval  NONE
*/ 
void set_alarm_for_time_Duong (uint8_t time_hour,uint8_t time_min, uint8_t time_sec)
{
  uint16_t Temp_Sec = 0;
  uint16_t Temp_min = 0;
	Get_RTC(); 
  Temp_Sec = sRTC.sec + time_sec;
  Temp_min = sRTC.min + time_min;
	svAlarm.AlarmTime.Seconds  = Temp_Sec%60;
	svAlarm.AlarmTime.Minutes  = (Temp_min+(Temp_Sec/60))%60;
	svAlarm.AlarmTime.Hours=((sRTC.hour+time_hour)+((Temp_min+(Temp_Sec/60))/60))%24;
	
	svAlarm.AlarmTime.SubSeconds = 0;
	svAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  svAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
  svAlarm.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY;
  svAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
  svAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
  svAlarm.AlarmDateWeekDay = 1;
  svAlarm.Alarm = RTC_ALARM_A;
	
	if (HAL_RTC_SetAlarm_IT(&hrtc, &svAlarm, RTC_FORMAT_BIN) != HAL_OK)
	{
		Error_Handler();
	}
}

/**
  * @brief   Stop Alarm 
  * @param   NONE
  * @retval  NONE
*/
void BC_RTC_StopAlarm( void )
{

  /* Clear RTC Alarm Flag */
  __HAL_RTC_ALARM_CLEAR_FLAG( &hrtc, RTC_FLAG_ALRAF);

  /* Disable the Alarm A interrupt */

  HAL_RTC_DeactivateAlarm(&hrtc, RTC_ALARM_A );
}

/**
  * @brief   Caculate time from 1/1/2000 until now in second
  * @param   NONE
  * @retval  seconds
*/
uint32_t HW_RTC_GetCalendarValue(void)
{
//  uint32_t calendarValue = 0;
  uint32_t correction;
  uint32_t seconds;
  
  RTC_TimeTypeDef 	        RTC_TimeStruct;
  RTC_DateTypeDef 	        RTC_DateStruct;

 
  /* Get Time and Date*/
  HAL_RTC_GetTime( &hrtc, &RTC_TimeStruct, RTC_FORMAT_BIN );
  
  HAL_RTC_GetDate( &hrtc, &RTC_DateStruct, FORMAT_BIN);
 
  /* calculte amount of elapsed days since 01/01/2000 */
  seconds= DIVC( (DAYS_IN_YEAR*3 + DAYS_IN_LEAP_YEAR) * RTC_DateStruct.Year , 4);

  correction = ( (RTC_DateStruct.Year % 4) == 0 ) ? DAYS_IN_MONTH_CORRECTION_LEAP : DAYS_IN_MONTH_CORRECTION_NORM ;
 
  seconds +=( DIVC( (RTC_DateStruct.Month-1)*(30+31) ,2 ) - (((correction>> ((RTC_DateStruct.Month-1)*2) )&0x3)));

  seconds += (RTC_DateStruct.Date -1);
  
  /* convert from days to seconds */
  seconds *= SECONDS_IN_1DAY; 

  seconds += ( ( uint32_t )RTC_TimeStruct.Seconds + 
             ( ( uint32_t )RTC_TimeStruct.Minutes * SECONDS_IN_1MINUTE ) +
             ( ( uint32_t )RTC_TimeStruct.Hours * SECONDS_IN_1HOUR ) ) ;
//  calendarValue = (((uint64_t) seconds)<<N_PREDIV_S) + ( PREDIV_S - RTC_TimeStruct.SubSeconds);
//
//  return( calendarValue );
  return( seconds );
}

/**
  * @brief   Convert timeto desired GMT
  * @param   sRTC_Check struct contain RTC content that want to convert GMT
  * @param   GMT the GMT want to convert to
  * @retval  NONE
*/
//bat dau tu hour
void Convert_Time_GMT (ST_TIME_FORMAT* sRTC_Check, uint8_t GMT)
{      
    sRTC_Check->hour = sRTC_Check->hour + GMT;
      
    if((sRTC_Check->hour >= 24) && (sRTC_Check->hour < 48))
    {
       sRTC_Check->hour = sRTC_Check->hour%24;
       //c?ng ngay them 1
       if((sRTC_Check->year %4) == 0)
       {
             sRTC_Check->date += 1;
             if(sRTC_Check->date > DaysInMonthLeapYear[sRTC_Check->month - 1])
             {
                sRTC_Check->date = 1;
                sRTC_Check->month += 1;
                //
                if(sRTC_Check->month > 12)
                {
                    sRTC_Check->month = 1;
                    sRTC_Check->year  = (sRTC_Check->year + 1) %100;
                }
             }   
       }else
       {
             sRTC_Check->date += 1;
             if(sRTC_Check->date > DaysInMonth[sRTC_Check->month - 1])
             {
                sRTC_Check->date = 1;
                sRTC_Check->month += 1;
                //
                if(sRTC_Check->month > 12)
                {
                    sRTC_Check->month = 1;
                    sRTC_Check->year  = (sRTC_Check->year + 1) %100;
                }
             } 
       }
    }  
}

/**

*/
void Set_AlarmA_With_Interrupt_Duong ( uint8_t time_hour,uint8_t time_min, uint8_t time_sec )
{
	BC_RTC_StopAlarm();
	Get_RTC();
	Set_RTC_2(sRTC);
	set_alarm_for_time( time_hour ,  time_min , time_sec );
	HAL_RTC_SetAlarm_IT (&hrtc, &svAlarm, RTC_FORMAT_BIN);
}