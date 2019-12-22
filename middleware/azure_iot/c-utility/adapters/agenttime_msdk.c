// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <time.h>
#include "azure_c_shared_utility/gballoc.h"
#include "azure_c_shared_utility/agenttime.h"
#include "FreeRTOS.h"
#include "task.h"
#include "fsl_debug_console.h"

/* Initialize time.h */
#undef CLOCKS_PER_SEC
#define CLOCKS_PER_SEC configTICK_RATE_HZ
clock_t clk_count = 0;
time_t time_data = 0;

/* lwip sntp callback, propagate sntp response to system */
void msdk_set_system_timer(time_t seconds)
{
  time_data = seconds;
}

/* lwip sntp wait until time is obtained*/
time_t sntp_get_current_timestamp(void)
{
    while (time_data < 40 * 365 * 24 * 3600)
    {
        vTaskDelay(2000 / portTICK_PERIOD_MS);
        time(&time_data);
    }
    return time_data;
}

/* FreeRTOS tick hook */
void vApplicationTickHook( void )
{
    clk_count ++;
    if ((clk_count% configTICK_RATE_HZ) == 0)
    {
         time_data ++;
    }
}

/* Agentime functions */
time_t get_time(time_t* p)
{
    return time(p);
}

struct tm* get_gmtime(time_t* currentTime)
{
    return gmtime(currentTime);
}

time_t get_mktime(struct tm* cal_time)
{
    return mktime(cal_time);
}

char* get_ctime(time_t* timeToGet)
{
    return ctime(timeToGet);
}

double get_difftime(time_t stopTime, time_t startTime)
{
    return difftime(stopTime, startTime);
}

/* time.h adaptation */
/* clock redirection is aplicable for all toolchains */
clock_t clock(void)
{
    return clk_count;
}

/* Adapter for NXP MCUXpresso and ARM GCC */
#if defined(__GNUC__) && !defined(__ARMCC_VERSION)
__attribute__(( weak )) int _gettimeofday (struct timeval *tv, void *tz)
{
    tv->tv_sec=time_data;
    tv->tv_usec=0;
    return 0;
}
#endif

/* Adapter for IAR */
#if defined( __ICCARM__ )
time_t __time32 (time_t * p)
{
    return time_data;
}
#endif

/* Keil MDK */
#if defined(__CC_ARM) /* Keil MDK V5 */
/* Disable linking of semihosting functions from C library into application. */
#pragma import(__use_no_semihosting_swi)
#elif defined(__ARMCC_VERSION) /* Keil MDK V6 */
__asm(".global __use_no_semihosting\n\t");
#endif

#if defined(__CC_ARM) || defined(__ARMCC_VERSION)
time_t time(time_t *timer)
{
    return time_data;
}

void _clock_init(void)
{
    clk_count = 0;
}
#endif
