/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "fsl_snvs_hp.h"

#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_common.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define kCLOCK_SnvsHp0 kCLOCK_SnvsHp
#define EXAMPLE_SNVS_IRQn SNVS_Consolidated_IRQn

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
 * @brief Main function
 */

void EXAMPLE_SNVS_IRQHandler(void)
{
    if (SNVS_HP_RTC_GetStatusFlags(SNVS) & kSNVS_RTC_AlarmInterruptFlag)
    {
        busyWait = false;

        /* Clear alarm flag */
        SNVS_HP_RTC_ClearStatusFlags(SNVS, kSNVS_RTC_AlarmInterruptEnable);
    }
}

int main(void)
{
    uint32_t sec;
    uint8_t index;
    snvs_hp_rtc_datetime_t rtcDate;
    snvs_hp_rtc_config_t snvsRtcConfig;

    /* Board pin, clock, debug console init */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitMemory();
    BOARD_InitDebugConsole();

    /* Install IRQ Handler */
    SystemInitIrqTable();
    /* Tricky here: As IRQ handler in example doesn't use parameters, just ignore them */
    SystemInstallIrqHandler(SNVS_Consolidated_IRQn, (system_irq_handler_t)(uint32_t)EXAMPLE_SNVS_IRQHandler, NULL);
    /* Init SNVS */
    /*
     * snvsConfig->rtccalenable = false;
     * snvsConfig->rtccalvalue = 0U;
     * snvsConfig->srtccalenable = false;
     * snvsConfig->srtccalvalue = 0U;
     * snvsConfig->PIFreq = 0U;
     */
    SNVS_HP_RTC_GetDefaultConfig(&snvsRtcConfig);
    SNVS_HP_RTC_Init(SNVS, &snvsRtcConfig);

    PRINTF("SNVS HP example:\r\n");

    /* Set a start date time and start RT */
    rtcDate.year = 2014U;
    rtcDate.month = 12U;
    rtcDate.day = 25U;
    rtcDate.hour = 19U;
    rtcDate.minute = 0;
    rtcDate.second = 0;

    /* Set RTC time to default time and date and start the RTC */
    SNVS_HP_RTC_SetDatetime(SNVS, &rtcDate);
    SNVS_HP_RTC_StartTimer(SNVS);

    /* Enable SNVS alarm interrupt */
    SNVS_HP_RTC_EnableInterrupts(SNVS, kSNVS_RTC_AlarmInterruptEnable);

    /* Enable at the NVIC */
    EnableIRQ(EXAMPLE_SNVS_IRQn);

    PRINTF("Set up time to wake up an alarm.\r\n");
    /* This loop will set the SNVS alarm */
    while (1)
    {
        busyWait = true;
        index = 0;
        sec = 0;

        /* Get date time */
        SNVS_HP_RTC_GetDatetime(SNVS, &rtcDate);

        /* print default time */
        PRINTF("Current datetime: %04hd-%02hd-%02hd %02hd:%02hd:%02hd\r\n", rtcDate.year, rtcDate.month, rtcDate.day,
               rtcDate.hour, rtcDate.minute, rtcDate.second);

        /* Get alarm time from user */
        PRINTF("Please input the number of second to wait for alarm and press enter \r\n");
        PRINTF("The second must be positive value\r\n");

        while (index != 0x0D)
        {
            index = GETCHAR();
            if((index >= '0') && (index <= '9'))
            {
                PUTCHAR(index);
                sec = sec * 10 + (index - 0x30U);
            }
        }
        PRINTF("\r\n");

        SNVS_HP_RTC_GetDatetime(SNVS, &rtcDate);
        if ((rtcDate.second + sec) < 60)
        {
            rtcDate.second += sec;
        }
        else
        {
            rtcDate.minute += (rtcDate.second + sec) / 60U;
            rtcDate.second = (rtcDate.second + sec) % 60U;
        }

        SNVS_HP_RTC_SetAlarm(SNVS, &rtcDate);

        /* Get alarm time */
        SNVS_HP_RTC_GetAlarm(SNVS, &rtcDate);

        /* Print alarm time */
        PRINTF("Alarm will occur at: %04hd-%02hd-%02hd %02hd:%02hd:%02hd\r\n", rtcDate.year, rtcDate.month, rtcDate.day,
               rtcDate.hour, rtcDate.minute, rtcDate.second);

        /* Wait until alarm occurs */
        while (busyWait)
        {
        }

        PRINTF("\r\n Alarm occurs !!!! ");
    }
}
