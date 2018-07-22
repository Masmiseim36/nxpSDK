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

#include "fsl_smartcard_freertos.h"

/*******************************************************************************
* Variables
******************************************************************************/

/*******************************************************************************
* Prototypes
******************************************************************************/

/*******************************************************************************
* Code
******************************************************************************/
/*! @brief Function is executed from ISR */
static void SMARTCARD_RTOS_Callback(void *handle, void *param)
{
    rtos_smartcard_context_t *ctx = (rtos_smartcard_context_t *)param;
    int32_t left = SMARTCARD_GetTransferRemainingBytes(ctx->x_context.base, (smartcard_context_t *)handle);
    BaseType_t xHigherPriorityTaskWoken, xResult;

    xHigherPriorityTaskWoken = pdFALSE;
    xResult = pdFAIL;

    if (0 == left)
    {
        xResult = xEventGroupSetBitsFromISR(ctx->x_event, RTOS_SMARTCARD_COMPLETE, &xHigherPriorityTaskWoken);
    }
    if (xResult != pdFAIL)
    {
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

int SMARTCARD_RTOS_Init(void *base, rtos_smartcard_context_t *ctx, uint32_t sourceClockHz)
{
    if ((NULL == ctx) || (0 == sourceClockHz) || (NULL == base))
    {
        return kStatus_InvalidArgument;
    }

    /* Allocate RTOS synchronization objects */
    ctx->x_sem = xSemaphoreCreateMutex();
    if ((NULL == ctx->x_sem))
    {
        return kStatus_Fail;
    }
    ctx->x_event = xEventGroupCreate();
    if ((NULL == ctx->x_event))
    {
        vSemaphoreDelete(ctx->x_sem);
        return kStatus_Fail;
    }
    /* Get default driver configuration */
    SMARTCARD_GetDefaultConfig(&ctx->x_context.cardParams);
    SMARTCARD_PHY_GetDefaultConfig(&ctx->x_context.interfaceConfig);
    /* Initialize and configure smartcard peripheral driver */
    if (kStatus_SMARTCARD_Success != (smartcard_status_t)SMARTCARD_Init(base, &ctx->x_context, sourceClockHz))
    {
        vEventGroupDelete(ctx->x_event);
        vSemaphoreDelete(ctx->x_sem);
        return kStatus_Fail;
    }
    /* Initialize and configure smartcard PHY driver */
    if (kStatus_SMARTCARD_Success !=
        (smartcard_status_t)SMARTCARD_PHY_Init(base, &ctx->x_context.interfaceConfig, sourceClockHz))
    {
        vEventGroupDelete(ctx->x_event);
        vSemaphoreDelete(ctx->x_sem);
        return kStatus_Fail;
    }
    /* Set transfer callback */
    ctx->x_context.transferCallback = SMARTCARD_RTOS_Callback;
    ctx->x_context.transferCallbackParam = (void *)ctx;

    return kStatus_Success;
}

int SMARTCARD_RTOS_Deinit(rtos_smartcard_context_t *ctx)
{
    if ((NULL == ctx))
    {
        return kStatus_InvalidArgument;
    }
    /* De-initialize SMARTCARD PHY driver */
    SMARTCARD_PHY_Deinit(ctx->x_context.base, &ctx->x_context.interfaceConfig);
    /* De-initialize SMARTCARD driver */
    SMARTCARD_Deinit(ctx->x_context.base);
    /* Free allocated RTOS synchronization objects */
    vEventGroupDelete(ctx->x_event);
    /* Give the semaphore. This is for functional safety */
    xSemaphoreGive(ctx->x_sem);
    vSemaphoreDelete(ctx->x_sem);

    return kStatus_Success;
}

int SMARTCARD_RTOS_Transfer(rtos_smartcard_context_t *ctx, smartcard_xfer_t *xfer)
{
    EventBits_t ev;
    int retval = kStatus_Success;

    if ((NULL == ctx) || (NULL == xfer))
    {
        return kStatus_InvalidArgument;
    }
    if ((0u == xfer->size))
    {
        return kStatus_Success;
    }

    /* Try to take semaphore */
    if ((pdFALSE == xSemaphoreTake(ctx->x_sem, 0)))
    { /* We could not take the semaphore, exit with 0 data received */
        return kStatus_Fail;
    }
    /* Non-blocking call */
    if (kStatus_SMARTCARD_Success != SMARTCARD_Transfer(ctx->x_context.base, &ctx->x_context, xfer))
    {
        retval = kStatus_Fail;
    }
    if (kStatus_Success == retval)
    {
        /* Wait until all bytes are transferred */
        ev = xEventGroupWaitBits(ctx->x_event, RTOS_SMARTCARD_COMPLETE, pdTRUE, pdFALSE, 0xFFFFFFFF);
        if (0u == (ev & RTOS_SMARTCARD_COMPLETE))
        {
            retval = kStatus_Fail;
        }
        ev = xEventGroupClearBits(ctx->x_event, RTOS_SMARTCARD_COMPLETE);
        assert(ev == 0u);
    }
    if (pdFALSE == xSemaphoreGive(ctx->x_sem))
    { /* We could not release the semaphore, exit with error */
        retval = kStatus_Fail;
    }

    return retval;
}

int SMARTCARD_RTOS_WaitForXevent(rtos_smartcard_context_t *ctx)
{
    EventBits_t ev;
    int retval = kStatus_Success;

    if ((NULL == ctx))
    {
        return kStatus_InvalidArgument;
    }

    /* Try to take semaphore */
    if ((pdFALSE == xSemaphoreTake(ctx->x_sem, 0)))
    { /* We could not take the semaphore, exit with 0 data received */
        return kStatus_Fail;
    }
    /* Wait until all bytes are transferred */
    ev = xEventGroupWaitBits(ctx->x_event, RTOS_SMARTCARD_COMPLETE, pdTRUE, pdFALSE, 0xFFFFFFFF);
    if (0u == (ev & RTOS_SMARTCARD_COMPLETE))
    {
        retval = kStatus_Fail;
    }
    ev = xEventGroupClearBits(ctx->x_event, RTOS_SMARTCARD_COMPLETE);
    assert(ev == 0);

    if (pdFALSE == xSemaphoreGive(ctx->x_sem))
    { /* We could not release the semaphore, exit with error */
        retval = kStatus_Fail;
    }

    return retval;
}

int SMARTCARD_RTOS_Control(rtos_smartcard_context_t *ctx, smartcard_control_t control, uint32_t param)
{
    int retval = kStatus_Success;

    if ((NULL == ctx))
    {
        return kStatus_InvalidArgument;
    }

    /* Try to take semaphore */
    if ((pdFALSE == xSemaphoreTake(ctx->x_sem, 0)))
    { /* We could not take the semaphore, exit with 0 data received */
        return kStatus_Fail;
    }
    /* Call SMARTCARD PHY control function */
    if (kStatus_SMARTCARD_Success !=
        (smartcard_status_t)SMARTCARD_Control(ctx->x_context.base, &ctx->x_context, control, param))
    {
        retval = kStatus_Fail;
    }
    /* Try to give semaphore */
    if (pdFALSE == xSemaphoreGive(ctx->x_sem))
    { /* We could not release the semaphore, exit with error */
        retval = kStatus_Fail;
    }

    return retval;
}

int SMARTCARD_RTOS_PHY_Control(rtos_smartcard_context_t *ctx, smartcard_interface_control_t control, uint32_t param)
{
    int retval = kStatus_Success;

    if ((NULL == ctx))
    {
        return kStatus_InvalidArgument;
    }

    /* Try to take semaphore */
    if ((pdFALSE == xSemaphoreTake(ctx->x_sem, 0)))
    { /* We could not take the semaphore, exit with 0 data received */
        return kStatus_Fail;
    }
    /* Call SMARTCARD PHY control function */
    if (kStatus_SMARTCARD_Success !=
        (smartcard_status_t)SMARTCARD_PHY_Control(ctx->x_context.base, &ctx->x_context, control, param))
    {
        retval = kStatus_Fail;
    }
    /* Try to give semaphore */
    if (pdFALSE == xSemaphoreGive(ctx->x_sem))
    { /* We could not release the semaphore, exit with error */
        retval = kStatus_Fail;
    }

    return retval;
}

int SMARTCARD_RTOS_PHY_Activate(rtos_smartcard_context_t *ctx, smartcard_reset_type_t resetType)
{
    int retval = kStatus_Success;

    if ((NULL == ctx))
    {
        return kStatus_InvalidArgument;
    }

    /* Try to take semaphore */
    if ((pdFALSE == xSemaphoreTake(ctx->x_sem, 0)))
    { /* We could not take the semaphore, exit with 0 data received */
        return kStatus_Fail;
    }
    /* Call SMARTCARD PHY activate function */
    if (kStatus_SMARTCARD_Success !=
        (smartcard_status_t)SMARTCARD_PHY_Activate(ctx->x_context.base, &ctx->x_context, resetType))
    {
        retval = kStatus_Fail;
    }
    /* Try to give semaphore */
    if (pdFALSE == xSemaphoreGive(ctx->x_sem))
    { /* We could not release the semaphore, exit with error */
        retval = kStatus_Fail;
    }

    return retval;
}

int SMARTCARD_RTOS_PHY_Deactivate(rtos_smartcard_context_t *ctx)
{
    int retval = kStatus_Success;

    if ((NULL == ctx))
    {
        return kStatus_InvalidArgument;
    }

    /* Try to take semaphore */
    if ((pdFALSE == xSemaphoreTake(ctx->x_sem, 0)))
    { /* We could not take the semaphore, exit with 0 data received */
        return kStatus_Fail;
    }
    /* Call SMARTCARD PHY deactivate function */
    if (kStatus_SMARTCARD_Success != (smartcard_status_t)SMARTCARD_PHY_Deactivate(ctx->x_context.base, &ctx->x_context))
    {
        retval = kStatus_Fail;
    }
    /* Try to give semaphore */
    if (pdFALSE == xSemaphoreGive(ctx->x_sem))
    { /* We could not release the semaphore, exit with error */
        retval = kStatus_Fail;
    }

    return retval;
}
