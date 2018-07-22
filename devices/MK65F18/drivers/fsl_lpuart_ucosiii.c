/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
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
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
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

#include "fsl_lpuart_ucosiii.h"
#include <os.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

static void LPUART_RTOS_Callback(LPUART_Type *base, lpuart_handle_t *state, status_t status, void *param)
{
    OS_ERR err;
    lpuart_rtos_handle_t *handle = (lpuart_rtos_handle_t *)param;

    if (status == kStatus_LPUART_RxIdle)
    {
        OSFlagPost(&handle->rxEvent, RTOS_LPUART_COMPLETE, OS_OPT_POST_FLAG_SET, &err);
    }
    else if (status == kStatus_LPUART_TxIdle)
    {
        OSFlagPost(&handle->txEvent, RTOS_LPUART_COMPLETE, OS_OPT_POST_FLAG_SET, &err);
    }
    else if (status == kStatus_LPUART_RxRingBufferOverrun)
    {
        OSFlagPost(&handle->rxEvent, RTOS_LPUART_RING_BUFFER_OVERRUN, OS_OPT_POST_FLAG_SET, &err);
    }
    else if (status == kStatus_LPUART_RxHardwareOverrun)
    {
        /* Clear Overrun flag (OR) in LPUART STAT register */
        LPUART_ClearStatusFlags(base, kLPUART_RxOverrunFlag);
        OSFlagPost(&handle->rxEvent, RTOS_LPUART_HARDWARE_BUFFER_OVERRUN, OS_OPT_POST_FLAG_SET, &err);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LPUART_RTOS_Init
 * Description   : Initializes the LPUART instance for application
 *
 *END**************************************************************************/
int LPUART_RTOS_Init(lpuart_rtos_handle_t *handle, lpuart_handle_t *t_handle, const lpuart_rtos_config_t *cfg)
{
    lpuart_config_t defcfg;
    OS_ERR err;

    if (NULL == handle)
    {
        return kStatus_InvalidArgument;
    }
    if (NULL == t_handle)
    {
        return kStatus_InvalidArgument;
    }
    if (NULL == cfg)
    {
        return kStatus_InvalidArgument;
    }
    if (NULL == cfg->base)
    {
        return kStatus_InvalidArgument;
    }
    if (0 == cfg->srcclk)
    {
        return kStatus_InvalidArgument;
    }
    if (0 == cfg->baudrate)
    {
        return kStatus_InvalidArgument;
    }

    handle->base = cfg->base;
    handle->t_state = t_handle;

    OSSemCreate(&handle->txSemaphore, "LPUART TX", (OS_SEM_CTR)1, &err);
    if (OS_ERR_NONE != err)
    {
        return kStatus_Fail;
    }
    OSSemCreate(&handle->rxSemaphore, "LPUART TX", (OS_SEM_CTR)1, &err);
    if (OS_ERR_NONE != err)
    {
#if (OS_CFG_SEM_DEL_EN == DEF_ENABLED)
        OSSemDel(&handle->txSemaphore, OS_OPT_DEL_ALWAYS, &err);
#endif
        return kStatus_Fail;
    }
    OSFlagCreate(&handle->txEvent, "LPUART TX", (OS_FLAGS)0, &err);
    if (OS_ERR_NONE != err)
    {
#if (OS_CFG_SEM_DEL_EN == DEF_ENABLED)
        OSSemDel(&handle->rxSemaphore, OS_OPT_DEL_ALWAYS, &err);
        OSSemDel(&handle->txSemaphore, OS_OPT_DEL_ALWAYS, &err);
#endif
        return kStatus_Fail;
    }
    OSFlagCreate(&handle->rxEvent, "LPUART RX", (OS_FLAGS)0, &err);
    if (OS_ERR_NONE != err)
    {
#if (OS_CFG_FLAG_DEL_EN == DEF_ENABLED)
        OSFlagDel(&handle->rxEvent, OS_OPT_DEL_ALWAYS, &err);
#endif
#if (OS_CFG_SEM_DEL_EN == DEF_ENABLED)
        OSSemDel(&handle->rxSemaphore, OS_OPT_DEL_ALWAYS, &err);
        OSSemDel(&handle->txSemaphore, OS_OPT_DEL_ALWAYS, &err);
#endif
        return kStatus_Fail;
    }

    LPUART_GetDefaultConfig(&defcfg);

    defcfg.baudRate_Bps = cfg->baudrate;
    defcfg.parityMode = cfg->parity;
#if defined(FSL_FEATURE_LPUART_HAS_STOP_BIT_CONFIG_SUPPORT) && FSL_FEATURE_LPUART_HAS_STOP_BIT_CONFIG_SUPPORT
    defcfg.stopBitCount = cfg->stopbits;
#endif

    LPUART_Init(handle->base, &defcfg, cfg->srcclk);
    LPUART_TransferCreateHandle(handle->base, handle->t_state, LPUART_RTOS_Callback, handle);
    LPUART_TransferStartRingBuffer(handle->base, handle->t_state, cfg->buffer, cfg->buffer_size);

    LPUART_EnableTx(handle->base, true);
    LPUART_EnableRx(handle->base, true);

    return 0;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LPUART_RTOS_Deinit
 * Description   : Deinitializes the LPUART instance and frees resources
 *
 *END**************************************************************************/
int LPUART_RTOS_Deinit(lpuart_rtos_handle_t *handle)
{
    OS_ERR err = OS_ERR_NONE;
    (void)err; /* Remove warning variable unused */

    LPUART_Deinit(handle->base);

#if (OS_CFG_FLAG_DEL_EN == DEF_ENABLED)
    OSFlagDel(&handle->txEvent, OS_OPT_DEL_ALWAYS, &err);
    OSFlagDel(&handle->rxEvent, OS_OPT_DEL_ALWAYS, &err);
#endif

#if (OS_CFG_SEM_DEL_EN == DEF_ENABLED)
    OSSemDel(&handle->txSemaphore, OS_OPT_DEL_ALWAYS, &err);
    OSSemDel(&handle->rxSemaphore, OS_OPT_DEL_ALWAYS, &err);
#endif

    /* Invalidate the handle */
    handle->t_state = NULL;
    handle->base = NULL;

    return 0;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LPUART_RTOS_Send
 * Description   : Initializes the LPUART instance for application
 *
 *END**************************************************************************/
int LPUART_RTOS_Send(lpuart_rtos_handle_t *handle, const uint8_t *buffer, uint32_t length)
{
    OS_FLAGS ev;
    OS_ERR err;
    int retval = kStatus_Success;

    if (NULL == handle->base)
    {
        /* Invalid handle. */
        return kStatus_Fail;
    }
    if (0 == length)
    {
        return 0;
    }
    if (NULL == buffer)
    {
        return kStatus_InvalidArgument;
    }

    OSSemPend(&handle->txSemaphore, 0, OS_OPT_PEND_BLOCKING, NULL, &err);
    if (OS_ERR_NONE != err)
    {
        /* We could not take the semaphore, exit with 0 data received */
        return kStatus_Fail;
    }

    handle->txTransfer.data = (uint8_t *)buffer;
    handle->txTransfer.dataSize = (uint32_t)length;

    /* Non-blocking call */
    LPUART_TransferSendNonBlocking(handle->base, handle->t_state, &handle->txTransfer);

    ev = OSFlagPend(&handle->txEvent, RTOS_LPUART_COMPLETE, 0, OS_OPT_PEND_FLAG_SET_ALL + OS_OPT_PEND_FLAG_CONSUME,
                    NULL, &err);
    if (!(ev & RTOS_LPUART_COMPLETE))
    {
        retval = kStatus_Fail;
    }

    OSSemPost(&handle->txSemaphore, OS_OPT_POST_1, &err);
    if (OS_ERR_NONE != err)
    {
        /* We could not post the semaphore, exit with error */
        retval = kStatus_Fail;
    }

    return retval;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LPUART_RTOS_Recv
 * Description   : Receives chars for the application
 *
 *END**************************************************************************/
int LPUART_RTOS_Receive(lpuart_rtos_handle_t *handle, uint8_t *buffer, uint32_t length, size_t *received)
{
    OS_ERR err;
    OS_FLAGS ev;
    size_t n = 0;
    int retval = kStatus_Success;
    size_t local_received = 0;

    if (NULL == handle->base)
    {
        /* Invalid handle. */
        return kStatus_Fail;
    }
    if (0 == length)
    {
        if (received != NULL)
        {
            *received = 0;
        }
        return 0;
    }
    if (NULL == buffer)
    {
        return kStatus_InvalidArgument;
    }

    /* New transfer can be performed only after current one is finished */
    OSSemPend(&handle->rxSemaphore, 0, OS_OPT_PEND_BLOCKING, NULL, &err);
    if (OS_ERR_NONE != err)
    {
        /* We could not take the semaphore, exit with 0 data received */
        return kStatus_Fail;
    }

    handle->rxTransfer.data = buffer;
    handle->rxTransfer.dataSize = (uint32_t)length;

    /* Non-blocking call */
    LPUART_TransferReceiveNonBlocking(handle->base, handle->t_state, &handle->rxTransfer, &n);

    ev = OSFlagPend(&handle->rxEvent,
                    RTOS_LPUART_COMPLETE | RTOS_LPUART_RING_BUFFER_OVERRUN | RTOS_LPUART_HARDWARE_BUFFER_OVERRUN, 0,
                    OS_OPT_PEND_FLAG_SET_ANY + OS_OPT_PEND_FLAG_CONSUME, NULL, &err);
    if (ev & RTOS_LPUART_HARDWARE_BUFFER_OVERRUN)
    {
        /* Stop data transfer to application buffer, ring buffer is still active */
        LPUART_TransferAbortReceive(handle->base, handle->t_state);
        /* Prevent false indication of successful transfer in next call of LPUART_RTOS_Receive.
           RTOS_LPUART_COMPLETE flag could be set meanwhile overrun is handled */
        OSFlagPost(&handle->rxEvent, RTOS_LPUART_COMPLETE, OS_OPT_POST_FLAG_CLR, &err);
        retval = kStatus_LPUART_RxHardwareOverrun;
        local_received = 0;
    }
    else if (ev & RTOS_LPUART_RING_BUFFER_OVERRUN)
    {
        /* Stop data transfer to application buffer, ring buffer is still active */
        LPUART_TransferAbortReceive(handle->base, handle->t_state);
        /* Prevent false indication of successful transfer in next call of LPUART_RTOS_Receive.
           RTOS_LPUART_COMPLETE flag could be set meanwhile overrun is handled */
        OSFlagPost(&handle->rxEvent, RTOS_LPUART_COMPLETE, OS_OPT_POST_FLAG_CLR, &err);
        retval = kStatus_LPUART_RxRingBufferOverrun;
        local_received = 0;
    }
    else if (ev & RTOS_LPUART_COMPLETE)
    {
        retval = kStatus_Success;
        local_received = length;
    }

    /* Prevent repetitive NULL check */
    if (received != NULL)
    {
        *received = local_received;
    }

    /* Enable next transfer. Current one is finished */
    OSSemPost(&handle->rxSemaphore, OS_OPT_POST_1, &err);
    if (OS_ERR_NONE != err)
    {
        /* We could not post the semaphore, exit with error */
        retval = kStatus_Fail;
    }
    return retval;
}
