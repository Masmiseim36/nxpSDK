/* Copyright 2018-2019 Google LLC
 *
 * This is part of the Google Cloud IoT Device SDK for Embedded C.
 * It is licensed under the BSD 3-Clause license; you may not use this file
 * except in compliance with the License.
 *
 * You may obtain a copy of the License at:
 *  https://opensource.org/licenses/BSD-3-Clause
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <iotc_bsp_time.h>

#include <stddef.h>
#include <time.h>

#include "lwip/apps/sntp.h"

#include "FreeRTOS.h"

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
        vTaskDelay(1000 / portTICK_PERIOD_MS);
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

/* Adapter for Keil MDK V5 */
#if defined(__CC_ARM)
#pragma import(__use_no_semihosting_swi)

#elif defined(__ARMCC_VERSION) /* Keil MDK V6 */
__asm(".global __use_no_semihosting\n\t");
#endif

#if defined(__CC_ARM) || defined(__ARMCC_VERSION)
#if !defined(SDK_DEBUGCONSOLE_UART)
void _sys_exit(int ret)
{
    while(1);
}

void _ttywrch(int c)
{
    putchar(c);
}
#endif /* SDK_DEBUGCONSOLE_UART */

time_t time(time_t *timer)
{
    return time_data;
}

void _clock_init(void)
{
    clk_count = 0;
}
#endif


int SNTP_Init(void)
{
    time_t now = 0;
    PRINTF("Initializing SNTP \n");
    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_init();
    sntp_get_current_timestamp();
    PRINTF("SNTP initialization complete \n");
    now = time(NULL);
    PRINTF("Actual UTC time: %s \n", ctime(&now));
    return 0;
}

void iotc_bsp_time_init( void ) {
    SNTP_Init();
}

iotc_time_t iotc_bsp_time_getcurrenttime_seconds( void ) {
  time_t now = time(NULL);
  return (iotc_time_t)(now);
}

iotc_time_t iotc_bsp_time_getcurrenttime_milliseconds( void ) {
  return 1;
}

iotc_time_t iotc_bsp_time_getmonotonictime_milliseconds( void ) {
  return 1;
}
