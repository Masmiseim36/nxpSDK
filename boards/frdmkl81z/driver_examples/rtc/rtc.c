/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "fsl_rtc.h"

#include "pin_mux.h"
#include "fsl_intmux.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/


/*******************************************************************************
 * Variables
 ******************************************************************************/

volatile bool busyWait;

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
* @brief ISR for Alarm interrupt
*
* This function changes the state of busyWait.
*/
void RTC_IRQHandler(void)
{
    uint32_t status = RTC_GetStatusFlags(RTC);

    if (status & kRTC_AlarmFlag)
    {
        busyWait = false;

        /* Clear alarm flag */
        RTC_ClearStatusFlags(RTC, kRTC_AlarmInterruptEnable);
    }
    else if (status & kRTC_TimeInvalidFlag)
    {
        /* Clear timer invalid flag */
        RTC_ClearStatusFlags(RTC, kRTC_TimeInvalidFlag);
    }
    else
    {
    }
    /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
      exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}

/*!
 * @brief Main function
 */
int main(void)
{
    uint32_t sec;
    uint32_t currSeconds;
    uint8_t index;
    rtc_datetime_t date;
    rtc_config_t rtcConfig;

    /* Board pin, clock, debug console init */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    INTMUX_Init(INTMUX0);
    INTMUX_SetChannelMode(INTMUX0, 0, kINTMUX_ChannelLogicOR);
    INTMUX_EnableInterrupt(INTMUX0, 0, RTC_Alarm_IRQn);
    /* Init RTC */
    /*
     * rtcConfig.wakeupSelect = false;
     * rtcConfig.updateMode = false;
     * rtcConfig.supervisorAccess = false;
     * rtcConfig.compensationInterval = 0;
     * rtcConfig.compensationTime = 0;
     */
    RTC_GetDefaultConfig(&rtcConfig);
    RTC_Init(RTC, &rtcConfig);
#if !(defined(FSL_FEATURE_RTC_HAS_NO_CR_OSCE) && FSL_FEATURE_RTC_HAS_NO_CR_OSCE)

    /* Select RTC clock source */
    RTC_SetClockSource(RTC);
#endif /* FSL_FEATURE_RTC_HAS_NO_CR_OSCE */

    PRINTF("RTC example: set up time to wake up an alarm\r\n");

    /* Set a start date time and start RT */
    date.year = 2014U;
    date.month = 12U;
    date.day = 25U;
    date.hour = 19U;
    date.minute = 0;
    date.second = 0;

    /* RTC time counter has to be stopped before setting the date & time in the TSR register */
    RTC_StopTimer(RTC);

    /* Set RTC time to default */
    RTC_SetDatetime(RTC, &date);

    /* Enable RTC alarm interrupt */
    RTC_EnableInterrupts(RTC, kRTC_AlarmInterruptEnable);

    /* Enable at the NVIC */
    EnableIRQ(RTC_IRQn);

    /* Start the RTC time counter */
    RTC_StartTimer(RTC);

    /* This loop will set the RTC alarm */
    while (1)
    {
        busyWait = true;
        index = 0;
        sec = 0;
        /* Get date time */
        RTC_GetDatetime(RTC, &date);

        /* print default time */
        PRINTF("Current datetime: %04hd-%02hd-%02hd %02hd:%02hd:%02hd\r\n", date.year, date.month, date.day, date.hour,
               date.minute, date.second);

        /* Get alarm time from user */
        PRINTF("Please input the number of second to wait for alarm \r\n");
        PRINTF("The second must be positive value\r\n");
        while (index != 0x0D)
        {
            index = GETCHAR();
            if ((index >= '0') && (index <= '9'))
            {
                PUTCHAR(index);
                sec = sec * 10 + (index - 0x30U);
            }
        }
        PRINTF("\r\n");

        /* Read the RTC seconds register to get current time in seconds */
        currSeconds = RTC->TSR;

        /* Add alarm seconds to current time */
        currSeconds += sec;

        /* Set alarm time in seconds */
        RTC->TAR = currSeconds;

        /* Get alarm time */
        RTC_GetAlarm(RTC, &date);

        /* Print alarm time */
        PRINTF("Alarm will occur at: %04hd-%02hd-%02hd %02hd:%02hd:%02hd\r\n", date.year, date.month, date.day,
               date.hour, date.minute, date.second);

        /* Wait until alarm occurs */
        while (busyWait)
        {
        }

        PRINTF("\r\n Alarm occurs !!!! ");
    }
}
