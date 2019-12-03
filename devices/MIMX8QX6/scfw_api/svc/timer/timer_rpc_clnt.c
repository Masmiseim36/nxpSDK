/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2017-2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
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

/*!
 * File containing client-side RPC functions for the TIMER service. These
 * functions are ported to clients that communicate to the SC.
 *
 * @addtogroup TIMER_SVC
 * @{
 */

/* Includes */

#include "main/types.h"
#include "svc/rm/rm_api.h"
#include "svc/timer/timer_api.h"
#include "../../main/rpc.h"
#include "svc/timer/timer_rpc.h"

/* Local Defines */

/* Local Types */

/* Local Functions */

sc_err_t sc_timer_set_wdog_timeout(sc_ipc_t ipc,
    sc_timer_wdog_time_t timeout)
{
    sc_rpc_msg_t msg;
    uint8_t result;

    RPC_VER(&msg) = SC_RPC_VERSION;
    RPC_SVC(&msg) = U8(SC_RPC_SVC_TIMER);
    RPC_FUNC(&msg) = U8(TIMER_FUNC_SET_WDOG_TIMEOUT);
    RPC_U32(&msg, 0U) = U32(timeout);
    RPC_SIZE(&msg) = 2U;

    sc_call_rpc(ipc, &msg, SC_FALSE);

    result = RPC_R8(&msg);
    return (sc_err_t) result;
}

sc_err_t sc_timer_set_wdog_pre_timeout(sc_ipc_t ipc,
    sc_timer_wdog_time_t pre_timeout)
{
    sc_rpc_msg_t msg;
    uint8_t result;

    RPC_VER(&msg) = SC_RPC_VERSION;
    RPC_SVC(&msg) = U8(SC_RPC_SVC_TIMER);
    RPC_FUNC(&msg) = U8(TIMER_FUNC_SET_WDOG_PRE_TIMEOUT);
    RPC_U32(&msg, 0U) = U32(pre_timeout);
    RPC_SIZE(&msg) = 2U;

    sc_call_rpc(ipc, &msg, SC_FALSE);

    result = RPC_R8(&msg);
    return (sc_err_t) result;
}

sc_err_t sc_timer_start_wdog(sc_ipc_t ipc, sc_bool_t lock)
{
    sc_rpc_msg_t msg;
    uint8_t result;

    RPC_VER(&msg) = SC_RPC_VERSION;
    RPC_SVC(&msg) = U8(SC_RPC_SVC_TIMER);
    RPC_FUNC(&msg) = U8(TIMER_FUNC_START_WDOG);
    RPC_U8(&msg, 0U) = B2U8(lock);
    RPC_SIZE(&msg) = 2U;

    sc_call_rpc(ipc, &msg, SC_FALSE);

    result = RPC_R8(&msg);
    return (sc_err_t) result;
}

sc_err_t sc_timer_stop_wdog(sc_ipc_t ipc)
{
    sc_rpc_msg_t msg;
    uint8_t result;

    RPC_VER(&msg) = SC_RPC_VERSION;
    RPC_SVC(&msg) = U8(SC_RPC_SVC_TIMER);
    RPC_FUNC(&msg) = U8(TIMER_FUNC_STOP_WDOG);
    RPC_SIZE(&msg) = 1U;

    sc_call_rpc(ipc, &msg, SC_FALSE);

    result = RPC_R8(&msg);
    return (sc_err_t) result;
}

sc_err_t sc_timer_ping_wdog(sc_ipc_t ipc)
{
    sc_rpc_msg_t msg;
    uint8_t result;

    RPC_VER(&msg) = SC_RPC_VERSION;
    RPC_SVC(&msg) = U8(SC_RPC_SVC_TIMER);
    RPC_FUNC(&msg) = U8(TIMER_FUNC_PING_WDOG);
    RPC_SIZE(&msg) = 1U;

    sc_call_rpc(ipc, &msg, SC_FALSE);

    result = RPC_R8(&msg);
    return (sc_err_t) result;
}

sc_err_t sc_timer_get_wdog_status(sc_ipc_t ipc,
    sc_timer_wdog_time_t *timeout, sc_timer_wdog_time_t *max_timeout,
    sc_timer_wdog_time_t *remaining_time)
{
    sc_rpc_msg_t msg;
    uint8_t result;

    RPC_VER(&msg) = SC_RPC_VERSION;
    RPC_SVC(&msg) = U8(SC_RPC_SVC_TIMER);
    RPC_FUNC(&msg) = U8(TIMER_FUNC_GET_WDOG_STATUS);
    RPC_SIZE(&msg) = 1U;

    sc_call_rpc(ipc, &msg, SC_FALSE);

    if (timeout != NULL)
    {
        *timeout = RPC_U32(&msg, 0U);
    }

    if (max_timeout != NULL)
    {
        *max_timeout = RPC_U32(&msg, 4U);
    }

    if (remaining_time != NULL)
    {
        *remaining_time = RPC_U32(&msg, 8U);
    }

    result = RPC_R8(&msg);
    return (sc_err_t) result;
}

sc_err_t sc_timer_pt_get_wdog_status(sc_ipc_t ipc, sc_rm_pt_t pt, sc_bool_t *enb,
    sc_timer_wdog_time_t *timeout, sc_timer_wdog_time_t *remaining_time)
{
    sc_rpc_msg_t msg;
    uint8_t result;

    RPC_VER(&msg) = SC_RPC_VERSION;
    RPC_SVC(&msg) = U8(SC_RPC_SVC_TIMER);
    RPC_FUNC(&msg) = U8(TIMER_FUNC_PT_GET_WDOG_STATUS);
    RPC_U8(&msg, 0U) = U8(pt);
    RPC_SIZE(&msg) = 2U;

    sc_call_rpc(ipc, &msg, SC_FALSE);

    if (timeout != NULL)
    {
        *timeout = RPC_U32(&msg, 0U);
    }

    if (remaining_time != NULL)
    {
        *remaining_time = RPC_U32(&msg, 4U);
    }

    result = RPC_R8(&msg);
    if (enb != NULL)
    {
        *enb = U2B(RPC_U8(&msg, 8U));
    }

    return (sc_err_t) result;
}

sc_err_t sc_timer_set_wdog_action(sc_ipc_t ipc,
    sc_rm_pt_t pt, sc_timer_wdog_action_t action)
{
    sc_rpc_msg_t msg;
    uint8_t result;

    RPC_VER(&msg) = SC_RPC_VERSION;
    RPC_SVC(&msg) = U8(SC_RPC_SVC_TIMER);
    RPC_FUNC(&msg) = U8(TIMER_FUNC_SET_WDOG_ACTION);
    RPC_U8(&msg, 0U) = U8(pt);
    RPC_U8(&msg, 1U) = U8(action);
    RPC_SIZE(&msg) = 2U;

    sc_call_rpc(ipc, &msg, SC_FALSE);

    result = RPC_R8(&msg);
    return (sc_err_t) result;
}

sc_err_t sc_timer_set_rtc_time(sc_ipc_t ipc, uint16_t year, uint8_t mon,
    uint8_t day, uint8_t hour, uint8_t min, uint8_t sec)
{
    sc_rpc_msg_t msg;
    uint8_t result;

    RPC_VER(&msg) = SC_RPC_VERSION;
    RPC_SVC(&msg) = U8(SC_RPC_SVC_TIMER);
    RPC_FUNC(&msg) = U8(TIMER_FUNC_SET_RTC_TIME);
    RPC_U16(&msg, 0U) = U16(year);
    RPC_U8(&msg, 2U) = U8(mon);
    RPC_U8(&msg, 3U) = U8(day);
    RPC_U8(&msg, 4U) = U8(hour);
    RPC_U8(&msg, 5U) = U8(min);
    RPC_U8(&msg, 6U) = U8(sec);
    RPC_SIZE(&msg) = 3U;

    sc_call_rpc(ipc, &msg, SC_FALSE);

    result = RPC_R8(&msg);
    return (sc_err_t) result;
}

sc_err_t sc_timer_get_rtc_time(sc_ipc_t ipc, uint16_t *year, uint8_t *mon,
    uint8_t *day, uint8_t *hour, uint8_t *min, uint8_t *sec)
{
    sc_rpc_msg_t msg;
    uint8_t result;

    RPC_VER(&msg) = SC_RPC_VERSION;
    RPC_SVC(&msg) = U8(SC_RPC_SVC_TIMER);
    RPC_FUNC(&msg) = U8(TIMER_FUNC_GET_RTC_TIME);
    RPC_SIZE(&msg) = 1U;

    sc_call_rpc(ipc, &msg, SC_FALSE);

    if (year != NULL)
    {
        *year = RPC_U16(&msg, 0U);
    }

    result = RPC_R8(&msg);
    if (mon != NULL)
    {
        *mon = RPC_U8(&msg, 2U);
    }

    if (day != NULL)
    {
        *day = RPC_U8(&msg, 3U);
    }

    if (hour != NULL)
    {
        *hour = RPC_U8(&msg, 4U);
    }

    if (min != NULL)
    {
        *min = RPC_U8(&msg, 5U);
    }

    if (sec != NULL)
    {
        *sec = RPC_U8(&msg, 6U);
    }

    return (sc_err_t) result;
}

sc_err_t sc_timer_get_rtc_sec1970(sc_ipc_t ipc, uint32_t *sec)
{
    sc_rpc_msg_t msg;
    uint8_t result;

    RPC_VER(&msg) = SC_RPC_VERSION;
    RPC_SVC(&msg) = U8(SC_RPC_SVC_TIMER);
    RPC_FUNC(&msg) = U8(TIMER_FUNC_GET_RTC_SEC1970);
    RPC_SIZE(&msg) = 1U;

    sc_call_rpc(ipc, &msg, SC_FALSE);

    if (sec != NULL)
    {
        *sec = RPC_U32(&msg, 0U);
    }

    result = RPC_R8(&msg);
    return (sc_err_t) result;
}

sc_err_t sc_timer_set_rtc_alarm(sc_ipc_t ipc, uint16_t year, uint8_t mon,
    uint8_t day, uint8_t hour, uint8_t min, uint8_t sec)
{
    sc_rpc_msg_t msg;
    uint8_t result;

    RPC_VER(&msg) = SC_RPC_VERSION;
    RPC_SVC(&msg) = U8(SC_RPC_SVC_TIMER);
    RPC_FUNC(&msg) = U8(TIMER_FUNC_SET_RTC_ALARM);
    RPC_U16(&msg, 0U) = U16(year);
    RPC_U8(&msg, 2U) = U8(mon);
    RPC_U8(&msg, 3U) = U8(day);
    RPC_U8(&msg, 4U) = U8(hour);
    RPC_U8(&msg, 5U) = U8(min);
    RPC_U8(&msg, 6U) = U8(sec);
    RPC_SIZE(&msg) = 3U;

    sc_call_rpc(ipc, &msg, SC_FALSE);

    result = RPC_R8(&msg);
    return (sc_err_t) result;
}

sc_err_t sc_timer_set_rtc_periodic_alarm(sc_ipc_t ipc, uint32_t sec)
{
    sc_rpc_msg_t msg;
    uint8_t result;

    RPC_VER(&msg) = SC_RPC_VERSION;
    RPC_SVC(&msg) = U8(SC_RPC_SVC_TIMER);
    RPC_FUNC(&msg) = U8(TIMER_FUNC_SET_RTC_PERIODIC_ALARM);
    RPC_U32(&msg, 0U) = U32(sec);
    RPC_SIZE(&msg) = 2U;

    sc_call_rpc(ipc, &msg, SC_FALSE);

    result = RPC_R8(&msg);
    return (sc_err_t) result;
}

sc_err_t sc_timer_cancel_rtc_alarm(sc_ipc_t ipc)
{
    sc_rpc_msg_t msg;
    uint8_t result;

    RPC_VER(&msg) = SC_RPC_VERSION;
    RPC_SVC(&msg) = U8(SC_RPC_SVC_TIMER);
    RPC_FUNC(&msg) = U8(TIMER_FUNC_CANCEL_RTC_ALARM);
    RPC_SIZE(&msg) = 1U;

    sc_call_rpc(ipc, &msg, SC_FALSE);

    result = RPC_R8(&msg);
    return (sc_err_t) result;
}

sc_err_t sc_timer_set_rtc_calb(sc_ipc_t ipc, int8_t count)
{
    sc_rpc_msg_t msg;
    uint8_t result;

    RPC_VER(&msg) = SC_RPC_VERSION;
    RPC_SVC(&msg) = U8(SC_RPC_SVC_TIMER);
    RPC_FUNC(&msg) = U8(TIMER_FUNC_SET_RTC_CALB);
    RPC_I8(&msg, 0U) = I8(count);
    RPC_SIZE(&msg) = 2U;

    sc_call_rpc(ipc, &msg, SC_FALSE);

    result = RPC_R8(&msg);
    return (sc_err_t) result;
}

sc_err_t sc_timer_set_sysctr_alarm(sc_ipc_t ipc, uint64_t ticks)
{
    sc_rpc_msg_t msg;
    uint8_t result;

    RPC_VER(&msg) = SC_RPC_VERSION;
    RPC_SVC(&msg) = U8(SC_RPC_SVC_TIMER);
    RPC_FUNC(&msg) = U8(TIMER_FUNC_SET_SYSCTR_ALARM);
    RPC_U32(&msg, 0U) = U32(ticks >> 32ULL);
    RPC_U32(&msg, 4U) = U32(ticks);
    RPC_SIZE(&msg) = 3U;

    sc_call_rpc(ipc, &msg, SC_FALSE);

    result = RPC_R8(&msg);
    return (sc_err_t) result;
}

sc_err_t sc_timer_set_sysctr_periodic_alarm(sc_ipc_t ipc,
    uint64_t ticks)
{
    sc_rpc_msg_t msg;
    uint8_t result;

    RPC_VER(&msg) = SC_RPC_VERSION;
    RPC_SVC(&msg) = U8(SC_RPC_SVC_TIMER);
    RPC_FUNC(&msg) = U8(TIMER_FUNC_SET_SYSCTR_PERIODIC_ALARM);
    RPC_U32(&msg, 0U) = U32(ticks >> 32ULL);
    RPC_U32(&msg, 4U) = U32(ticks);
    RPC_SIZE(&msg) = 3U;

    sc_call_rpc(ipc, &msg, SC_FALSE);

    result = RPC_R8(&msg);
    return (sc_err_t) result;
}

sc_err_t sc_timer_cancel_sysctr_alarm(sc_ipc_t ipc)
{
    sc_rpc_msg_t msg;
    uint8_t result;

    RPC_VER(&msg) = SC_RPC_VERSION;
    RPC_SVC(&msg) = U8(SC_RPC_SVC_TIMER);
    RPC_FUNC(&msg) = U8(TIMER_FUNC_CANCEL_SYSCTR_ALARM);
    RPC_SIZE(&msg) = 1U;

    sc_call_rpc(ipc, &msg, SC_FALSE);

    result = RPC_R8(&msg);
    return (sc_err_t) result;
}

/**@}*/

