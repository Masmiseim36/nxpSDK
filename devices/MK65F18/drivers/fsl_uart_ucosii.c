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

#include "fsl_uart_ucosii.h"
#include <ucos_ii.h>

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

static void UART_RTOS_Callback(UART_Type *base, uart_handle_t *state, status_t status, void *param)
{
    uint8_t err;
    uart_rtos_handle_t *handle = (uart_rtos_handle_t *)param;

    if (status == kStatus_UART_RxIdle)
    {
        OSFlagPost(handle->rxEvent, RTOS_UART_COMPLETE, OS_FLAG_SET, &err);
    }
    else if (status == kStatus_UART_TxIdle)
    {
        OSFlagPost(handle->txEvent, RTOS_UART_COMPLETE, OS_FLAG_SET, &err);
    }
    else if (status == kStatus_UART_RxRingBufferOverrun)
    {
        OSFlagPost(handle->rxEvent, RTOS_UART_RING_BUFFER_OVERRUN, OS_FLAG_SET, &err);
    }
    else if (status == kStatus_UART_RxHardwareOverrun)
    {
        /* Clear Overrun flag (OR) in UART S1 register */
        UART_ClearStatusFlags(base, kUART_RxOverrunFlag);
        OSFlagPost(handle->rxEvent, RTOS_UART_HARDWARE_BUFFER_OVERRUN, OS_FLAG_SET, &err);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_RTOS_Init
 * Description   : Initializes the UART instance for application
 *
 *END**************************************************************************/
int UART_RTOS_Init(uart_rtos_handle_t *handle, uart_handle_t *t_handle, const uart_rtos_config_t *cfg)
{
    uart_config_t defcfg;
    uint8_t err;

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

    handle->txSemaphore = OSSemCreate(1);
    if (NULL == handle->txSemaphore)
    {
        return kStatus_Fail;
    }
    handle->rxSemaphore = OSSemCreate(1);
    if (NULL == handle->rxSemaphore)
    {
        OSSemDel(handle->txSemaphore, OS_DEL_ALWAYS, &err);
        return kStatus_Fail;
    }
    handle->txEvent = OSFlagCreate(0, &err);
    if (OS_ERR_NONE != err)
    {
        OSSemDel(handle->rxSemaphore, OS_DEL_ALWAYS, &err);
        OSSemDel(handle->txSemaphore, OS_DEL_ALWAYS, &err);
        return kStatus_Fail;
    }
    handle->rxEvent = OSFlagCreate(0, &err);
    if (OS_ERR_NONE != err)
    {
        OSFlagDel(handle->rxEvent, OS_DEL_ALWAYS, &err);
        OSSemDel(handle->rxSemaphore, OS_DEL_ALWAYS, &err);
        OSSemDel(handle->txSemaphore, OS_DEL_ALWAYS, &err);
        return kStatus_Fail;
    }

    UART_GetDefaultConfig(&defcfg);

    defcfg.baudRate_Bps = cfg->baudrate;
    defcfg.parityMode = cfg->parity;
#if defined(FSL_FEATURE_UART_HAS_STOP_BIT_CONFIG_SUPPORT) && FSL_FEATURE_UART_HAS_STOP_BIT_CONFIG_SUPPORT
    defcfg.stopBitCount = cfg->stopbits;
#endif

    UART_Init(handle->base, &defcfg, cfg->srcclk);
    UART_TransferCreateHandle(handle->base, handle->t_state, UART_RTOS_Callback, handle);
    UART_TransferStartRingBuffer(handle->base, handle->t_state, cfg->buffer, cfg->buffer_size);

    UART_EnableTx(handle->base, true);
    UART_EnableRx(handle->base, true);

    return 0;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_RTOS_Deinit
 * Description   : Deinitializes the UART instance and frees resources
 *
 *END**************************************************************************/
int UART_RTOS_Deinit(uart_rtos_handle_t *handle)
{
    uint8_t err;

    UART_Deinit(handle->base);

    OSFlagDel(handle->txEvent, OS_DEL_ALWAYS, &err);
    OSFlagDel(handle->rxEvent, OS_DEL_ALWAYS, &err);

    /* Give the semaphore. This is for functional safety */
    OSSemPost(handle->txSemaphore);
    OSSemPost(handle->rxSemaphore);

    OSSemDel(handle->txSemaphore, OS_DEL_ALWAYS, &err);
    OSSemDel(handle->rxSemaphore, OS_DEL_ALWAYS, &err);

    /* Invalidate the handle */
    handle->t_state = NULL;
    handle->base = NULL;

    return 0;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_RTOS_Send
 * Description   : Initializes the UART instance for application
 *
 *END**************************************************************************/
int UART_RTOS_Send(uart_rtos_handle_t *handle, const uint8_t *buffer, uint32_t length)
{
    OS_FLAGS ev;
    uint8_t err;
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

    OSSemPend(handle->txSemaphore, 0, &err);
    if (OS_ERR_NONE != err)
    {
        /* We could not take the semaphore, exit with 0 data received */
        return kStatus_Fail;
    }

    handle->txTransfer.data = (uint8_t *)buffer;
    handle->txTransfer.dataSize = (uint32_t)length;

    /* Non-blocking call */
    UART_TransferSendNonBlocking(handle->base, handle->t_state, &handle->txTransfer);

    ev = OSFlagPend(handle->txEvent, RTOS_UART_COMPLETE, OS_FLAG_WAIT_SET_ALL + OS_FLAG_CONSUME, 0, &err);
    if (!(ev & RTOS_UART_COMPLETE))
    {
        retval = kStatus_Fail;
    }

    if (OS_ERR_NONE != OSSemPost(handle->txSemaphore))
    {
        /* We could not post the semaphore, exit with error */
        retval = kStatus_Fail;
    }

    return retval;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_RTOS_Recv
 * Description   : Receives chars for the application
 *
 *END**************************************************************************/
int UART_RTOS_Receive(uart_rtos_handle_t *handle, uint8_t *buffer, uint32_t length, size_t *received)
{
    uint8_t err;
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
    OSSemPend(handle->rxSemaphore, 0, &err);
    if (OS_ERR_NONE != err)
    {
        /* We could not take the semaphore, exit with 0 data received */
        return kStatus_Fail;
    }

    handle->rxTransfer.data = buffer;
    handle->rxTransfer.dataSize = (uint32_t)length;

    /* Non-blocking call */
    UART_TransferReceiveNonBlocking(handle->base, handle->t_state, &handle->rxTransfer, &n);

    ev = OSFlagPend(handle->rxEvent,
                    RTOS_UART_COMPLETE | RTOS_UART_RING_BUFFER_OVERRUN | RTOS_UART_HARDWARE_BUFFER_OVERRUN,
                    OS_FLAG_WAIT_SET_ANY + OS_FLAG_CONSUME, 0, &err);
    if (ev & RTOS_UART_HARDWARE_BUFFER_OVERRUN)
    {
        /* Stop data transfer to application buffer, ring buffer is still active */
        UART_TransferAbortReceive(handle->base, handle->t_state);
        /* Prevent false indication of successful transfer in next call of UART_RTOS_Receive.
           RTOS_UART_COMPLETE flag could be set meanwhile overrun is handled */
        OSFlagPost(handle->rxEvent, RTOS_UART_COMPLETE, OS_FLAG_CLR, &err);
        retval = kStatus_UART_RxHardwareOverrun;
        local_received = 0;
    }
    else if (ev & RTOS_UART_RING_BUFFER_OVERRUN)
    {
        /* Stop data transfer to application buffer, ring buffer is still active */
        UART_TransferAbortReceive(handle->base, handle->t_state);
        /* Prevent false indication of successful transfer in next call of UART_RTOS_Receive.
           RTOS_UART_COMPLETE flag could be set meanwhile overrun is handled */
        OSFlagPost(handle->rxEvent, RTOS_UART_COMPLETE, OS_FLAG_CLR, &err);
        retval = kStatus_UART_RxRingBufferOverrun;
        local_received = 0;
    }
    else if (ev & RTOS_UART_COMPLETE)
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
    if (OS_ERR_NONE != OSSemPost(handle->rxSemaphore))
    {
        /* We could not post the semaphore, exit with error */
        retval = kStatus_Fail;
    }
    return retval;
}
