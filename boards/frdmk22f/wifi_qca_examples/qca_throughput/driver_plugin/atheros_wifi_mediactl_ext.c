//------------------------------------------------------------------------------
// Copyright (c) Qualcomm Atheros, Inc.
// All rights reserved.
// Redistribution and use in source and binary forms, with or without modification, are permitted (subject to
// the limitations in the disclaimer below) provided that the following conditions are met:
//
// - Redistributions of source code must retain the above copyright notice, this list of conditions and the
//   following disclaimer.
// - Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
//   following disclaimer in the documentation and/or other materials provided with the distribution.
// - Neither the name of nor the names of its contributors may be used to endorse or promote products derived
//   from this software without specific prior written permission.
//
// NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE. THIS SOFTWARE IS
// PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
// BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
// ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//------------------------------------------------------------------------------
//==============================================================================
// Author(s): ="Atheros"
//==============================================================================

#include "main.h"
#include <stdint.h>
#include <stdbool.h>
#include <a_config.h>
#include <a_types.h>
#include <a_osapi.h>
#include <driver_cxt.h>
#include <common_api.h>
#include <wlan_api.h>

#include <custom_wlan_api.h>
#include <targaddrs.h>
#include "qca_structs.h"

#include "atheros_wifi.h"
#include "atheros_wifi_api.h"
#include "atheros_wifi_internal.h"

#if DEMOCFG_USE_SENSOR
#include "tmp106.h"
#endif

boolean reg_query_bool = false;
void *pQuery;

struct event_profile
{
    uint16_t event_id;
    uint32_t timestamp;
};

#define MAX_PROFILE_SIZE 20

struct event_profile profiles[MAX_PROFILE_SIZE];
uint8_t profile_head = 0, profile_trail = 0;

char *event_str[] = {
    "wmi_ready",     // BOOT_PROFILE_WMI_READY
    "refclk read",   // BOOT_PROFILE_READ_REFCLK
    "config done",   // BOOT_PROFILE_DONE_CONFIG
    "start connect", // BOOT_PROFILE_START_CONNECT
    "end connect",   // BOOT_PROFILE_DONE_CONNECT
    "driver ready",  // BOOT_PROFILE_DRIVE_READY
    "start scan",    // BOOT_PROFILE_START_SCAN
    "end scan",      // BOOT_PROFILE_DONE_SCAN
    "power up",      // BOOT_PROFILE_POWER_UP
    "boot param",    // BOOT_PROFILE_BOOT_PARAMETER
};

static A_STATUS driver_thread_operation(A_VOID *pCxt)
{
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);
    union
    {
        uint32_t param;
    } stackU;

#define PARAM (stackU.param)
#define PTR_REG_MOD ((ATH_REGQUERY *)(pQuery))
    switch (PTR_REG_MOD->operation)
    {
        case ATH_REG_OP_READ: // read
            Driver_ReadRegDiag(pCxt, &PTR_REG_MOD->address, &(PTR_REG_MOD->value));
            PTR_REG_MOD->value = A_CPU2LE32((PTR_REG_MOD->value));
            break;
        case ATH_REG_OP_WRITE: // write-
            PTR_REG_MOD->value = A_CPU2LE32((PTR_REG_MOD->value));
            Driver_WriteRegDiag(pCxt, &PTR_REG_MOD->address, &(PTR_REG_MOD->value));
            break;
        case ATH_REG_OP_RMW: // read-mod-write
            if (A_OK != Driver_ReadRegDiag(pCxt, &PTR_REG_MOD->address, &PARAM))
            {
                break;
            }

            PARAM = A_CPU2LE32(PARAM);
            PARAM &= ~PTR_REG_MOD->mask;
            PARAM |= PTR_REG_MOD->value;
            PARAM = A_CPU2LE32(PARAM);

            Driver_WriteRegDiag(pCxt, &PTR_REG_MOD->address, &PARAM);
            break;
    }
    pDCxt->asynchRequest = NULL;
    reg_query_bool       = true;
    DRIVER_WAKE_USER(pCxt);
#undef PTR_REG_MOD
#undef PARAM

    return A_OK;
}

/*FUNCTION*-------------------------------------------------------------
 *
 *  Function Name  : ath_ioctl_handler
 *  PARAMS         :
 *                   qca_ptr-> ptr to qca context
 *                   inout_param -> input/output data for command.
 *  Returned Value : QCA_OK or error code
 *  Comments       :
 *        IOCTL implementation of Atheros Wifi device.
 *
 *END*-----------------------------------------------------------------*/

static uint32_t ath_ioctl_handler_ext(A_VOID *qca_ptr, ATH_IOCTL_PARAM_STRUCT_PTR param_ptr)
{
    A_VOID *pCxt;
    A_DRIVER_CONTEXT *pDCxt;
    uint32_t error = QCA_OK;
    ATH_REGQUERY regQuery;
    uint32_t *ptr, i;

    if ((pCxt = ((QCA_CONTEXT_STRUCT_PTR)qca_ptr)->MAC_CONTEXT_PTR) == NULL)
    {
        return QCA_ERROR;
    }
    pDCxt = GET_DRIVER_COMMON(pCxt);

    switch (param_ptr->cmd_id)
    {
        case ATH_REG_QUERY:
            /* set the operation to be executed by the driver thread */
            pQuery         = param_ptr->data;
            reg_query_bool = false;
            if (pDCxt->asynchRequest != NULL)
            {
                break;
            }
            pDCxt->asynchRequest = driver_thread_operation;
            /* wake up driver thread */
            DRIVER_WAKE_DRIVER(pCxt);
            /* wait for driver thread completion */
            Driver_WaitForCondition(pCxt, &(reg_query_bool), true, 5000);

            break;
        case ATH_MEM_QUERY:
            // read the memory location for stat storage
            regQuery.address   = TARG_VTOP(HOST_INTEREST_ITEM_ADDRESS(hi_flash_is_present));
            regQuery.operation = ATH_REG_OP_READ;
            pQuery             = &regQuery;
            reg_query_bool     = false;

            if (pDCxt->asynchRequest != NULL)
            {
                break;
            }

            pDCxt->asynchRequest = driver_thread_operation;
            DRIVER_WAKE_DRIVER(pCxt);
            Driver_WaitForCondition(pCxt, &(reg_query_bool), true, 5000);

            if (reg_query_bool == false)
            {
                break;
            }
            // capture the value in regQuery.address
            regQuery.address = TARG_VTOP(regQuery.value);
            ptr              = (uint32_t *)param_ptr->data;

            for (i = 0; i < 5; i++)
            {
                reg_query_bool = false;

                if (pDCxt->asynchRequest != NULL)
                {
                    break;
                }

                pDCxt->asynchRequest = driver_thread_operation;
                DRIVER_WAKE_DRIVER(pCxt);
                Driver_WaitForCondition(pCxt, &(reg_query_bool), true, 5000);

                if (reg_query_bool == false)
                {
                    break;
                }
                /* CAUTION: this next line assumes that the stats are stored in
                 * the same order that they appear in the ATH_MEMQUERY structure. */
                ptr[i] = regQuery.value;
                regQuery.address += 4;
            }
            /* for allocram remaining we query address allocram_remaining_bytes.*/
            regQuery.address = TARG_VTOP(0x541f2c);
            reg_query_bool   = false;

            if (pDCxt->asynchRequest != NULL)
            {
                break;
            }

            pDCxt->asynchRequest = driver_thread_operation;
            DRIVER_WAKE_DRIVER(pCxt);
            Driver_WaitForCondition(pCxt, &(reg_query_bool), true, 5000);

            if (reg_query_bool == false)
            {
                break;
            }
            /* CAUTION: this next line assumes that the stats are stored in
             * the same order that they appear in the ATH_MEMQUERY structure. */
            ptr[5] = regQuery.value;

            break;
        default:
            error = QCA_ERROR;
            break;
    }

    return error;
}

#define LED_1 BSP_LED1
#define LED_2 BSP_LED2

#ifdef KINETIS_PLATFORM

#define LED_ON GPIO_IOCTL_WRITE_LOG0
#define LED_OFF GPIO_IOCTL_WRITE_LOG1

#define LED_STATE_ERROR \
    (0x00000001) /* LED 3 will only light in the event of an error (because its red and red means bad) */
#define LED_STATE_PROGRESS                                                                                      \
    (0x00000002) /* LED 4 will only light in the event the progress is occurring (because its yellow and yellow \
                    means indeterminate). */
#define LED_STATE_INFO_1                                                                                             \
    (0x00000008) /* LED 1 will light to indicate different information under different conditions (because its green \
                    and we like green). */
#define LED_STATE_INFO_2                                                                                             \
    (0x00000004) /* LED 2 will light to indicate different information under different conditions (because its green \
                    and we like green). */

#else

#define LED_ON GPIO_IOCTL_WRITE_LOG1
#define LED_OFF GPIO_IOCTL_WRITE_LOG0

#define LED_STATE_ERROR \
    (0x00000004) /* LED 3 will only light in the event of an error (because its red and red means bad) */
#define LED_STATE_PROGRESS                                                                                      \
    (0x00000008) /* LED 4 will only light in the event the progress is occurring (because its yellow and yellow \
                    means indeterminate). */
#define LED_STATE_INFO_1                                                                                             \
    (0x00000001) /* LED 1 will light to indicate different information under different conditions (because its green \
                    and we like green). */
#define LED_STATE_INFO_2                                                                                             \
    (0x00000002) /* LED 2 will light to indicate different information under different conditions (because its green \
                    and we like green). */

#endif

void LED_SetOutput(uint32_t signal);
void LED_SetOutput(uint32_t signal)
{
#if 0
	E.Y. hide to disable the LED op
    static const uint32_t led1[] = {
        LED_1,
        GPIO_LIST_END
    };
    static const uint32_t led2[] = {
        LED_2,
        GPIO_LIST_END
    };
    static const uint32_t led3[] = {
        LED_3,
        GPIO_LIST_END
    };
    static const uint32_t led4[] = {
        LED_4,
        GPIO_LIST_END
    };


    if (output_port) {
        ioctl(output_port, (signal & 0x00000001) ? LED_ON : LED_OFF, (pointer) &led1);
        ioctl(output_port, (signal & 0x00000002) ? LED_ON : LED_OFF, (pointer) &led2);
        ioctl(output_port, (signal & 0x00000004) ? LED_ON : LED_OFF, (pointer) &led3);
        ioctl(output_port, (signal & 0x00000008) ? LED_ON : LED_OFF, (pointer) &led4);
    }
#endif
}

#define CYCLE_QUEUE_INC(ndx)                \
    {                                       \
        ndx = (ndx + 1) % MAX_PROFILE_SIZE; \
    }

void add_one_profile(uint8_t event_id, uint16_t timestamp)
{
    profiles[profile_head].event_id  = event_id;
    profiles[profile_head].timestamp = timestamp;

    CYCLE_QUEUE_INC(profile_head);
    if (profile_head == profile_trail)
        CYCLE_QUEUE_INC(profile_trail);
}

int32_t print_perf(int32_t argc, char_ptr argv[])
{
    int32_t ret = 0;
    uint8_t ndx;

    if (profile_trail == profile_head)
        return ret;

    PRINTF("          Event  TIME STAMP" NL);
    ndx = profile_trail;
    while (ndx != profile_head)
    {
        PRINTF("%15s   % 6d" NL, event_str[profiles[ndx].event_id], (int)profiles[ndx].timestamp);
        CYCLE_QUEUE_INC(ndx);
    }

    return ret;
}

void boot_profile_output(uint32_t signal)
{
    int32_t timestamp;
    TIME_STRUCT time;

    app_time_get_elapsed(&time);
    timestamp = (time.SECONDS * 1000 + time.MILLISECONDS);

    add_one_profile(signal, timestamp);

    // PRINTF ("event=%s   TIME: %d" NL, event_str[signal], timestamp);
}

void gpio_process_register_value(uint8_t *datap, uint32_t len)
{
    WMIX_GPIO_DATA_EVENT *pEvent = (WMIX_GPIO_DATA_EVENT *)datap;
    PRINTF(NL "process_register_value len:%d" NL, (int)len);
    PRINTF("reg id:0x%08X    value=0x%08X" NL, (unsigned int)pEvent->reg_id, (unsigned int)pEvent->value);
}

#define QUEUE_IDX_INC(a) ((a) = ((a) + 1) % MAX_PROFILE_SIZE)
#define kf_profile_head profile_head
#define kf_profile_trail profile_trail
#define kf_profiles profiles

void clear_kf_profile(void)
{
    kf_profile_head  = 0;
    kf_profile_trail = 0;
}

void add_one_kf_profile(uint16_t event_id, uint32_t timestamp)
{
    kf_profiles[kf_profile_head].event_id  = event_id;
    kf_profiles[kf_profile_head].timestamp = timestamp;

    QUEUE_IDX_INC(kf_profile_head);
    if (kf_profile_head == kf_profile_trail)
        QUEUE_IDX_INC(kf_profile_trail);
}

A_VOID Custom_PfmDataEventRx(uint8_t *datap, uint32_t len)
{
    int num, i;
    WMIX_PFM_DATA_EVENT *pfm_data;

    pfm_data = (WMIX_PFM_DATA_EVENT *)datap;

    num = len / sizeof(WMIX_PFM_DATA_EVENT);

    for (i = 0; i < num; i++, pfm_data++)
    {
        add_one_kf_profile(pfm_data->event_id, pfm_data->log_time);
    }

    return;
}

A_VOID Custom_PfmDataDoneEventRx(A_VOID *pCxt, uint8_t *datap, uint32_t len)
{
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);

    pDCxt->pfmDone = true;

    //    Api_BootProfile(pCxt, BOOT_PROFILE_DONE_SCAN);

    API_SCAN_COMPLETE_EVENT(pCxt, status);

    return;
}

static const char *kf_once_event_str[] = {
    "tune start",       // BOOT_PROFILE_WMI_READY
    "wait boot param",  // BOOT_PROFILE_READ_REFCLK
    "got boot param",   // BOOT_PROFILE_DONE_CONFIG
    "radio init start", // BOOT_PROFILE_START_CONNECT
    "radio init done",  // BOOT_PROFILE_DONE_CONNECT
    "wmi ready",        // BOOT_PROFILE_DRIVE_READY
    "argo init start",  // BOOT_PROFILE_START_SCAN
    "argo init done",   // BOOT_PROFILE_DONE_SCAN
    "init start",       // BOOT_PROFILE_START_SCAN
    "init done",        // BOOT_PROFILE_DONE_SCAN
    "reserv 1",         // BOOT_PROFILE_DONE_SCAN
    "reserv 2",         // BOOT_PROFILE_DONE_SCAN
    "reserv 3",         // BOOT_PROFILE_DONE_SCAN
    "reserv 4",         // BOOT_PROFILE_DONE_SCAN
};

static const char *kf_repeat_event_str[] = {
    "wmi_ready",     // BOOT_PROFILE_WMI_READY
    "refclk read",   // BOOT_PROFILE_READ_REFCLK
    "config done",   // BOOT_PROFILE_DONE_CONFIG
    "start connect", // BOOT_PROFILE_START_CONNECT
    "end connect",   // BOOT_PROFILE_DONE_CONNECT
    "driver ready",  // BOOT_PROFILE_DRIVE_READY
    "start scan",    // BOOT_PROFILE_START_SCAN
    "end scan",      // BOOT_PROFILE_DONE_SCAN
};

static const char *kf_statistic_event_str[] = {
    "wmi_ready",     // BOOT_PROFILE_WMI_READY
    "refclk read",   // BOOT_PROFILE_READ_REFCLK
    "config done",   // BOOT_PROFILE_DONE_CONFIG
    "start connect", // BOOT_PROFILE_START_CONNECT
    "end connect",   // BOOT_PROFILE_DONE_CONNECT
    "driver ready",  // BOOT_PROFILE_DRIVE_READY
    "start scan",    // BOOT_PROFILE_START_SCAN
    "end scan",      // BOOT_PROFILE_DONE_SCAN
};

void output_kf_pfm(void)
{
    uint8_t ndx;
    uint16_t id;
    const char *type, *event_name;

    if (kf_profile_trail == kf_profile_head)
        return;

    PRINTF(NL "    Type            Event            TIME STAMP" NL);
    ndx = kf_profile_trail;
    while (ndx != kf_profile_head)
    {
        id = kf_profiles[ndx].event_id;
        switch ((id >> 12) & 0x0F)
        {
            case 1:
                type       = "once";
                event_name = kf_once_event_str[(id & 0x0F) - 1];
                break;

            case 2:
                type       = "repeat";
                event_name = kf_repeat_event_str[(id & 0x0F) - 1];
                break;

            case 3:
                type       = "stati";
                event_name = kf_statistic_event_str[(id & 0x0F) - 1];
                break;
            default:
                type       = "unknown";
                event_name = "unknown";
                break;
        }
        PRINTF("%8s  |  %15s   |  % 6d" NL, type, event_name, (int)kf_profiles[ndx].timestamp);
        QUEUE_IDX_INC(ndx);
    }
}

void atheros_driver_setup(void)
{
    ath_custom_mediactl.ath_ioctl_handler_ext = ath_ioctl_handler_ext;
    // ath_custom_init.Boot_Profile = LED_SetOutput;
    ath_custom_init.Boot_Profile = boot_profile_output;

    ath_custom_init.Api_GpioDataEventRx           = gpio_process_register_value;
    ath_custom_init.Custom_Api_PfmDataEventRx     = Custom_PfmDataEventRx;
    ath_custom_init.Custom_Api_PfmDataDoneEventRx = Custom_PfmDataDoneEventRx;
}
