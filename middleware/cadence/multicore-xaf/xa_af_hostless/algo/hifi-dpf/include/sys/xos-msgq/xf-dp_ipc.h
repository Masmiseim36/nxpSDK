/*
* Copyright (c) 2015-2024 Cadence Design Systems Inc.
*
* Permission is hereby granted, free of charge, to any person obtaining
* a copy of this software and associated documentation files (the
* "Software"), to deal in the Software without restriction, including
* without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to
* permit persons to whom the Software is furnished to do so, subject to
* the following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
/*******************************************************************************
 * xf-ipc.h
 *
 * Xtensa IPC mechanism
 *******************************************************************************/

#ifndef __XF_H
#error "xf-ipc.h mustn't be included directly"
#endif

/*******************************************************************************
 * Includes
 ******************************************************************************/

/* ...system-specific shared memory configuration */
#include "xf-shmem.h"

/*******************************************************************************
 * Remote IPI interrupt mode
 ******************************************************************************/

/* ...enable/disable IPI interrupt */
static inline void xf_ipi_enable(UWORD32 core, int on)
{
}

/* ...wait in low-power mode for interrupt arrival if "ptr" is 0 */
static inline int xf_ipi_wait(UWORD32 core)
{
    xf_core_ro_data_t *ro = XF_CORE_RO_DATA(core);
    xf_event_t *msgq_event = ro->ipc.msgq_event;

    __xf_event_wait_any(msgq_event, CMD_MSGQ_READY | DSP_DIE_MSGQ_ENTRY);
    if (__xf_event_get(msgq_event) & DSP_DIE_MSGQ_ENTRY)
    {
        return 0;
    }
	__xf_event_clear(msgq_event, CMD_MSGQ_READY | DSP_DIE_MSGQ_ENTRY);

    return 1;
}

/* ...complete IPI waiting (may be called from any context on local core) */
static inline void xf_ipi_resume(UWORD32 core)
{
}

/* ...complete IPI waiting and resume dsp thread (non ISR-safe) */
static inline void xf_ipi_resume_dsp(UWORD32 core)
{
    if(core == XF_CORE_ID_MASTER)
    {
    	xf_core_ro_data_t *ro = XF_CORE_RO_DATA(core);

	    __xf_event_set(ro->ipc.msgq_event, CMD_MSGQ_READY);
    }
#if (XF_CFG_CORES_NUM > 1)
    else
    {
        __xf_event_set(xf_g_dsp->pmsgq_event, CMD_MSGQ_READY);
    }
#endif
}

/* ...complete IPI waiting and resume dsp thread (ISR safe) */
static inline void xf_ipi_resume_dsp_isr(UWORD32 core)
{
    if(core == XF_CORE_ID_MASTER)
    {
        xf_core_ro_data_t *ro = XF_CORE_RO_DATA(core);

        __xf_event_set_isr(ro->ipc.msgq_event, CMD_MSGQ_READY);
    }
#if (XF_CFG_CORES_NUM > 1)
    else
    {
        __xf_event_set_isr(xf_g_dsp->pmsgq_event, CMD_MSGQ_READY);
    }
#endif
}

/* ...assert IPI interrupt on remote core - board-specific */
static inline void xf_ipi_assert(UWORD32 core)
{
	xf_core_ro_data_t *ro = XF_CORE_RO_DATA(core);

	__xf_event_set(ro->ipc.msgq_event, RESP_MSGQ_READY);
}

/* ...initialize IPI subsystem */
static inline int xf_ipi_init(UWORD32 core)
{
    return 0;
}

/* ...deinitialize IPI subsystem */
static inline int xf_ipi_deinit(UWORD32 core)
{
    return 0;
}

/*******************************************************************************
 * Shared memory operations
 ******************************************************************************/

/* ...translate buffer address to shared proxy address */
static inline UWORD32 xf_ipc_b2a(UWORD32 core, void *b)
{
    return (UWORD32) b;
}

/* ...translate shared proxy address to local pointer */
static inline void * xf_ipc_a2b(UWORD32 core, UWORD32 address)
{
    return (void *) address;
}

/* ...system-specific IPC layer initialization */
extern int xf_ipc_init(UWORD32 core);

/* ...system-specific IPC layer deinitialization */
extern int xf_ipc_deinit(UWORD32 core);
