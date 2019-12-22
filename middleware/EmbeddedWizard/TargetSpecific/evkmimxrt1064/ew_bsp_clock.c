/*******************************************************************************
*
* E M B E D D E D   W I Z A R D   P R O J E C T
*
*                                                Copyright (c) TARA Systems GmbH
*                                    written by Paul Banach and Manfred Schweyer
*
********************************************************************************
*
* This software is delivered "as is" and shows the usage of other software
* components. It is provided as an example software which is intended to be
* modified and extended according to particular requirements.
*
* TARA Systems hereby disclaims all warranties and conditions with regard to the
* software, including all implied warranties and conditions of merchantability
* and non-infringement of any third party IPR or other rights which may result
* from the use or the inability to use the software.
*
********************************************************************************
*
* DESCRIPTION:
*   This file is part of the interface (glue layer) between an Embedded Wizard
*   generated UI application and the board support package (BSP) of a dedicated
*   target.
*   This template is responsible to initialize the system clock of the hardware,
*   to provide a timer tick for the Embedded Wizard UI application and to get
*   access to the realtime clock (RTC). Additionally, this file contains some
*   performance measurements to analyse the CPU usage of the UI application.
*
*******************************************************************************/

#include "fsl_clock.h"
#include "fsl_snvs_hp.h"
#include "ew_bsp_clock.h"

#include <string.h>

#if EW_USE_FREE_RTOS == 1

  #include "FreeRTOS.h"
  #include "task.h"

  void xPortSysTickHandler( void );

#endif


#define RtcTicksPerSecond    ( 1UL )
#define RtcTicksPerMinute    ( RtcTicksPerSecond * 60UL )
#define RtcTicksPerHour      ( RtcTicksPerMinute * 60UL )
#define RtcTicksPerDay       ( RtcTicksPerHour   * 24UL )

const unsigned long DaysToMonth[] =
{
  0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365
};

const unsigned long DaysToMonthInLeapYear[] =
{
  0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366
};


/* timer tick for Embedded Wizard UI applications */
unsigned int                  EmWiSystemTicks = 0;

/* variables for cycle counter / CPU load measurement  */
#if EW_CPU_LOAD_MEASURING == 1
static unsigned long          WorkingTime  = 0;
static unsigned long          SleepingTime = 0;
static unsigned long          PrevCycCnt   = 0;
#endif

/* helper function to initialize measuring unit (cycle counter) */
static void CycleCounterInit( void )
{
  /* Enable TRC */
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;

  /* Unlock DWT registers */
  if ((*(uint32_t*)0xE0001FB4) & 1)
    *(uint32_t*)0xE0001FB0 = 0xC5ACCE55;

  /* clear the cycle counter */
  DWT->CYCCNT = 0;

  /* start the cycle counter */
  DWT->CTRL = 0x40000001;

  /* initialize counters */
  #if EW_CPU_LOAD_MEASURING == 1
  WorkingTime   = 0;
  SleepingTime  = 0;
  PrevCycCnt    = 0;
  #endif
}


#if EW_CPU_LOAD_MEASURING == 1
/* helper function to get cycle counter value since last call */
static unsigned long GetCycleCounterDelta( void )
{
  unsigned long cycCnt;
  unsigned long result;

  /* get delta between current counter value and last counter value */
  cycCnt = DWT->CYCCNT;
  result = cycCnt - PrevCycCnt;
  PrevCycCnt = cycCnt;

  return result;
}

#endif


/*******************************************************************************
* FUNCTION:
*   EwBspConfigSystemTick
*
* DESCRIPTION:
*   Configure the system tick counter.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBspConfigSystemTick( void )
{
  EmWiSystemTicks = 0;

  /* set systick reload value to generate 1ms interrupt */
  SysTick_Config( CLOCK_GetFreq(kCLOCK_CoreSysClk) / 1000U );

  /* initialize system cycle counter (also used for display synchronisation) */
  CycleCounterInit();
}


/*******************************************************************************
* FUNCTION:
*   EwBspSystemTickIncrement
*
* DESCRIPTION:
*   The function EwBspSystemTickIncrement increments the millisecond counter,
*   which is used by the Runtime Environment (RTE) to trigger timer events.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBspSystemTickIncrement( void )
{
  EmWiSystemTicks++;
}


/*******************************************************************************
* FUNCTION:
*   SysTick_Handler
*
* DESCRIPTION:
*   System tick handler. Called on every system tick.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void SysTick_Handler(void)
{
  EwBspSystemTickIncrement();

  #if EW_USE_FREE_RTOS == 1

  if ( xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED )
    xPortSysTickHandler();

  #endif
}


/*******************************************************************************
* FUNCTION:
*   EwBspConfigRealTimeClock
*
* DESCRIPTION:
*   Configures the Real Time Clock.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBspConfigRealTimeClock( void )
{
  snvs_hp_rtc_config_t snvsRtcConfig;

  SNVS_HP_RTC_GetDefaultConfig( &snvsRtcConfig );
  SNVS_HP_RTC_Init( SNVS, &snvsRtcConfig );
  SNVS_HP_RTC_StartTimer(SNVS);
}


/*******************************************************************************
* FUNCTION:
*   EwBspGetTime
*
* DESCRIPTION:
*   Returns the current time in seconds since 01.01.1970.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   The current time in seconds since 01.01.1970.
*
*******************************************************************************/
unsigned long EwBspGetTime( void )
{
  snvs_hp_rtc_datetime_t rtcDateTime;

  SNVS_HP_RTC_GetDatetime( SNVS, &rtcDateTime );

  unsigned long   days;
  unsigned long   year   = rtcDateTime.year - 2000;
  unsigned long   month  = rtcDateTime.month;
  unsigned long   day    = rtcDateTime.day;
  unsigned long   hour   = rtcDateTime.hour;
  unsigned long   minute = rtcDateTime.minute;
  unsigned long   second = rtcDateTime.second;
  unsigned long   timeInSeconds;

  /* year is the number of years since 2000 */
  /* calculate number of days since 01.01.1970 until begin of current
     year including the additional days of leap years */

  days = (year + 30) * 365 + (year + 27) / 4;

  if (year & 0x3)
  {
    days += DaysToMonth[month - 1];
  }
  else
  {
    days += DaysToMonthInLeapYear[month - 1];
  }

  days += day;

  timeInSeconds =
    days   * RtcTicksPerDay +
    hour   * RtcTicksPerHour +
    minute * RtcTicksPerMinute +
    second;

  return timeInSeconds;
}


/*******************************************************************************
* FUNCTION:
*   EwBspSetTime
*
* DESCRIPTION:
*   Sets the given time in seconds since 01.01.1970 at real time clock.
*
*******************************************************************************/
void EwBspSetTime( unsigned long aTime )
{
  #define LeapYearsFromBeginToNext400Year   7
  #define DaysPer4Years       ( 4 * 365 + 1 )

  snvs_hp_rtc_datetime_t rtcDateTime;
  unsigned int year;
  unsigned int year4;
  unsigned int month;
  unsigned int day;
  unsigned int hour;
  unsigned int minute;
  unsigned int second;

  hour    = ( unsigned int ) (( aTime / RtcTicksPerHour    ) % 24 );
  minute  = ( unsigned int ) (( aTime / RtcTicksPerMinute  ) % 60 );
  second  = ( unsigned int ) (( aTime / RtcTicksPerSecond  ) % 60 );

  /* calculation of the date */
  day       = ( unsigned int ) ( aTime / RtcTicksPerDay + 365);

  if ( day >= 48213 )
    day++;

  year4     = day / DaysPer4Years;
  day      -= year4 * DaysPer4Years;
  year      = day / 365;

  if ( year == 4 )
  {
    year = 3;

    if ( !( year4 % 100 == LeapYearsFromBeginToNext400Year )
       && ( year4 %  25 == LeapYearsFromBeginToNext400Year ))
      year = 4;
  }

  day  -= year * 365;
  year  = year4 * 4 + year + 1969;

  /* all months have less than 32 days, days >> 5 is a good estimate
     for the month. */

  month = day >> 5;

  /* correct month if necessary */
  if ( ( year % 4 != 0 ) ||
      (( year % 100 == 0 ) && ( year % 400 != 0 )) )
  {
    /* no leap year */
    if ( day >= DaysToMonth[ month + 1 ] )
      month++;

    day -= DaysToMonth[ month ];
  }
  else
  {
    /* leap year */
    if ( day >= DaysToMonthInLeapYear[ month + 1 ] )
      month++;

    day -= DaysToMonthInLeapYear[ month ];
  }

  memset( &rtcDateTime, 0, sizeof( rtcDateTime ));
  rtcDateTime.hour   = hour;
  rtcDateTime.minute = minute;
  rtcDateTime.second = second;
  rtcDateTime.day    = day + 1;
  rtcDateTime.month  = month + 1;
  rtcDateTime.year   = year;

  SNVS_HP_RTC_SetDatetime( SNVS, &rtcDateTime );
}


/*******************************************************************************
* FUNCTION:
*   EwBspGetCpuLoad
*
* DESCRIPTION:
*   Returns the current CPU load as percent value.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   The current CPU load.
*
*******************************************************************************/
 int EwBspGetCpuLoad( void )
{
  int cpuLoad = 0;

  #if EW_CPU_LOAD_MEASURING == 1

    /* get actual counter value */
    WorkingTime += GetCycleCounterDelta();

    /* calculate CPU load as percent value */
    cpuLoad = ( int ) ( WorkingTime / (( SleepingTime + WorkingTime ) / 100 ));

    /* clear accumulated counter values */
    WorkingTime  = 0;
    SleepingTime = 0;

  #endif

  return cpuLoad;
}


/*******************************************************************************
* FUNCTION:
*   EwBspCpuLoadSetActive
*
* DESCRIPTION:
*   Starts the CPU load counting. Call this function whenever CPU processing
*   is needed.
*
* ARGUMENTS:
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBspCpuLoadSetActive( void )
{
  #if EW_CPU_LOAD_MEASURING == 1

    SleepingTime += GetCycleCounterDelta();

  #endif
}


/*******************************************************************************
* FUNCTION:
*   EwBspCpuLoadSetIdle
*
* DESCRIPTION:
*   Stops the CPU load counting. Call this function whenever CPU processing is
*   currently not needed since the program execution is waiting for some system
*   event.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBspCpuLoadSetIdle( void )
{
  #if EW_CPU_LOAD_MEASURING == 1

    WorkingTime += GetCycleCounterDelta();

  #endif
}


/* mli */
