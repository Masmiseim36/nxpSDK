/*
 * Copyright (c) 2015 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_smartcard_freertos.h"

/* Component ID definition, used by tools. */
#ifndef FSL_COMPONENT_ID
#define FSL_COMPONENT_ID "platform.drivers.smartcard_freertos"
#endif

#define REMOVE_SYNC // os sync is only need in the beginning of  the T0/T1 cmd, rather than this place

/* Returns non zero value if the controller is in ISR */
#define IS_IRQ_CONTEXT() (SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk)

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
    BaseType_t reschedule;

    if (IS_IRQ_CONTEXT())
    {
        xSemaphoreGiveFromISR(ctx->x_event, &reschedule);
        portYIELD_FROM_ISR(reschedule);
    }
    else
    {
        xSemaphoreGive(ctx->x_event);
    }
}

/*!
 * brief Initializes a Smart card (EMVSIM/UART) peripheral for Smart card/ISO-7816 operation.
 * Also initialize Smart card PHY interface.
 *
 * This function ungates the Smart card clock, initializes the module to EMV default settings,
 * configures the IRQ state structure, and enables the module-level interrupt to the core.
 * Initializes RTOS synchronization objects and context.
 *
 * param base The Smart card peripheral base address.
 * param ctx The Smart card RTOS structure.
 * param sourceClockHz Smart card clock generation module source clock.
 *
 * return A zero in success or error code.
 */
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
    ctx->x_event = xSemaphoreCreateBinary();
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
        vSemaphoreDelete(ctx->x_event);
        vSemaphoreDelete(ctx->x_sem);
        return kStatus_Fail;
    }
    /* Initialize and configure smartcard PHY driver */
    if (kStatus_SMARTCARD_Success !=
        (smartcard_status_t)SMARTCARD_PHY_Init(base, &ctx->x_context.interfaceConfig, sourceClockHz))
    {
        vSemaphoreDelete(ctx->x_event);
        vSemaphoreDelete(ctx->x_sem);
        return kStatus_Fail;
    }
    /* Set transfer callback */
    ctx->x_context.transferCallback = SMARTCARD_RTOS_Callback;
    ctx->x_context.transferCallbackParam = (void *)ctx;

    return kStatus_Success;
}

/*!
 * brief
 *
 * This function disables the Smart card (EMVSIM/UART) interrupts, disables the transmitter and receiver, and
 * flushes the FIFOs (for modules that support FIFOs) and gates Smart card clock in SIM. It also deactivates  Smart card
 * PHY
 * interface, stops Smart card clocks, and frees all synchronization objects allocated in the RTOS Smart card context.
 *
 * param ctx The Smart card RTOS state.
 *
 * return A zero in success or error code.
 */
int SMARTCARD_RTOS_Deinit(rtos_smartcard_context_t *ctx)
{
    if ((NULL == ctx))
    {
        return kStatus_InvalidArgument;
    }
    if ((ctx->x_context.base == NULL) || (ctx->x_event == NULL) || (ctx->x_sem == NULL))
    {
        return kStatus_Fail;
    }
    /* De-initialize SMARTCARD PHY driver */
    SMARTCARD_PHY_Deinit(ctx->x_context.base, &ctx->x_context.interfaceConfig);
    /* De-initialize SMARTCARD driver */
    SMARTCARD_Deinit(ctx->x_context.base);
    /* Free allocated RTOS synchronization objects */
    vSemaphoreDelete(ctx->x_event);
    /* Give the semaphore. This is for functional safety */
    xSemaphoreGive(ctx->x_sem);
    vSemaphoreDelete(ctx->x_sem);

    return kStatus_Success;
}

/*!
 * brief Transfers data using interrupts.
 *
 * A blocking (also known as synchronous) function means that the function returns
 * after the transfer is done. User can cancel this transfer by calling the function AbortTransfer.
 *
 * param ctx A pointer to the RTOS Smart card driver context.
 * param xfer Smart card transfer structure.
 *
 * return A zero in success or error code.
 */
int SMARTCARD_RTOS_Transfer(rtos_smartcard_context_t *ctx, smartcard_xfer_t *xfer)
{
    int retval = kStatus_Success;

    if ((NULL == ctx) || (NULL == xfer))
    {
        return kStatus_InvalidArgument;
    }
    if ((0u == xfer->size))
    {
        return kStatus_Success;
    }

#ifndef REMOVE_SYNC
    /* Try to take semaphore */
    if ((pdFALSE == xSemaphoreTake(ctx->x_sem, 0)))
    { /* We could not take the semaphore, exit with 0 data received */
        return kStatus_Fail;
    }
#endif
    /* Non-blocking call */
    if (kStatus_SMARTCARD_Success != SMARTCARD_Transfer(ctx->x_context.base, &ctx->x_context, xfer))
    {
        retval = kStatus_Fail;
    }
    if (kStatus_Success == retval)
    {
        /* Wait until all bytes are transferred */
        xSemaphoreTake(ctx->x_event, portMAX_DELAY);
    }

#ifndef REMOVE_SYNC
    if (pdFALSE == xSemaphoreGive(ctx->x_sem))
    { /* We could not release the semaphore, exit with error */
        retval = kStatus_Fail;
    }
#endif
    return retval;
}

/*!
 * brief Terminates an asynchronous EMVSIM transfer early.
 *
 * During an async EMVSIM transfer, the user can terminate the transfer early
 * if the transfer is still in progress.
 *
 * param ctx A pointer to the RTOS Smart card driver context.

 * return A zero in success or error code.
 */
int SMARTCARD_RTOS_AbortTransfer(rtos_smartcard_context_t *ctx)
{
    if ((NULL == ctx))
    {
        return kStatus_InvalidArgument;
    }

    SMARTCARD_AbortTransfer(ctx->x_context.base, &ctx->x_context);

    return kStatus_Success;
}

/*!
 * brief Waits until the transfer is finished.
 *
 * Task waits on a transfer finish event. Don't initialize the transfer. Instead, wait for a transfer callback.
 * This function can be used while waiting on an initial TS character.
 *
 * param ctx A pointer to the RTOS Smart card driver context.
 *
 * return A zero in success or error code.
 */
int SMARTCARD_RTOS_WaitForXevent(rtos_smartcard_context_t *ctx)
{
    int retval = kStatus_Success;

    if ((NULL == ctx))
    {
        return kStatus_InvalidArgument;
    }

#ifndef REMOVE_SYNC
    /* Try to take semaphore */
    if ((pdFALSE == xSemaphoreTake(ctx->x_sem, 0)))
    { /* We could not take the semaphore, exit with 0 data received */
        return kStatus_Fail;
    }
#endif

    /* Wait until all bytes are transferred */
    xSemaphoreTake(ctx->x_event, portMAX_DELAY);

#ifndef REMOVE_SYNC
    if (pdFALSE == xSemaphoreGive(ctx->x_sem))
    { /* We could not release the semaphore, exit with error */
        retval = kStatus_Fail;
    }
#endif

    return retval;
}

/*!
 * brief Controls the Smart card module per different user requests.
 *
 * param ctx The Smart card RTOS context pointer.
 * param control Control type.
 * param param Integer value to control the command.
 *
 * return A zero in success or error code.
 */
int SMARTCARD_RTOS_Control(rtos_smartcard_context_t *ctx, smartcard_control_t control, uint32_t param)
{
    int retval = kStatus_Success;

    if ((NULL == ctx))
    {
        return kStatus_InvalidArgument;
    }

#ifndef REMOVE_SYNC
    /* Try to take semaphore */
    if ((pdFALSE == xSemaphoreTake(ctx->x_sem, 0)))
    { /* We could not take the semaphore, exit with 0 data received */
        return kStatus_Fail;
    }
#endif

    /* Call SMARTCARD PHY control function */
    if (kStatus_SMARTCARD_Success !=
        (smartcard_status_t)SMARTCARD_Control(ctx->x_context.base, &ctx->x_context, control, param))
    {
        retval = kStatus_Fail;
    }

#ifndef REMOVE_SYNC
    /* Try to give semaphore */
    if (pdFALSE == xSemaphoreGive(ctx->x_sem))
    { /* We could not release the semaphore, exit with error */
        retval = kStatus_Fail;
    }
#endif
    return retval;
}

/*!
 * brief Controls the Smart card module as per different user request.
 *
 * param ctx The Smart card RTOS context pointer.
 * param control Control type
 * param param Integer value  to control the command.
 *
 * return A zero in success or error code.
 */
int SMARTCARD_RTOS_PHY_Control(rtos_smartcard_context_t *ctx, smartcard_interface_control_t control, uint32_t param)
{
    int retval = kStatus_Success;

    if ((NULL == ctx))
    {
        return kStatus_InvalidArgument;
    }

#ifndef REMOVE_SYNC
    /* Try to take semaphore */
    if ((pdFALSE == xSemaphoreTake(ctx->x_sem, 0)))
    { /* We could not take the semaphore, exit with 0 data received */
        return kStatus_Fail;
    }
#endif

    /* Call SMARTCARD PHY control function */
    if (kStatus_SMARTCARD_Success !=
        (smartcard_status_t)SMARTCARD_PHY_Control(ctx->x_context.base, &ctx->x_context, control, param))
    {
        retval = kStatus_Fail;
    }

#ifndef REMOVE_SYNC
    /* Try to give semaphore */
    if (pdFALSE == xSemaphoreGive(ctx->x_sem))
    { /* We could not release the semaphore, exit with error */
        retval = kStatus_Fail;
    }
#endif

    return retval;
}

/*!
 * brief Activates the Smart card interface.
 *
 * param ctx The Smart card RTOS driver context structure.
 * param resetType type of reset to be performed, possible values
 *                       = kSmartcardColdReset, kSmartcardWarmReset
 *
 * return A zero in success or error code.
 */
int SMARTCARD_RTOS_PHY_Activate(rtos_smartcard_context_t *ctx, smartcard_reset_type_t resetType)
{
    int retval = kStatus_Success;

    if ((NULL == ctx))
    {
        return kStatus_InvalidArgument;
    }

#ifndef REMOVE_SYNC
    /* Try to take semaphore */
    if ((pdFALSE == xSemaphoreTake(ctx->x_sem, 0)))
    { /* We could not take the semaphore, exit with 0 data received */
        return kStatus_Fail;
    }
#endif
    /* Call SMARTCARD PHY activate function */
    if (kStatus_SMARTCARD_Success !=
        (smartcard_status_t)SMARTCARD_PHY_Activate(ctx->x_context.base, &ctx->x_context, resetType))
    {
        retval = kStatus_Fail;
    }
#ifndef REMOVE_SYNC
    /* Try to give semaphore */
    if (pdFALSE == xSemaphoreGive(ctx->x_sem))
    { /* We could not release the semaphore, exit with error */
        retval = kStatus_Fail;
    }
#endif

    return retval;
}

/*!
 * brief Deactivates the Smart card interface.
 *
 * param ctx The Smart card RTOS driver context structure.
 *
 * return A zero in success or error code.
 */
int SMARTCARD_RTOS_PHY_Deactivate(rtos_smartcard_context_t *ctx)
{
    int retval = kStatus_Success;

    if ((NULL == ctx))
    {
        return kStatus_InvalidArgument;
    }

#ifndef REMOVE_SYNC
    /* Try to take semaphore */
    if ((pdFALSE == xSemaphoreTake(ctx->x_sem, 0)))
    { /* We could not take the semaphore, exit with 0 data received */
        return kStatus_Fail;
    }
#endif

    /* Call SMARTCARD PHY deactivate function */
    if (kStatus_SMARTCARD_Success != (smartcard_status_t)SMARTCARD_PHY_Deactivate(ctx->x_context.base, &ctx->x_context))
    {
        retval = kStatus_Fail;
    }

#ifndef REMOVE_SYNC
    /* Try to give semaphore */
    if (pdFALSE == xSemaphoreGive(ctx->x_sem))
    { /* We could not release the semaphore, exit with error */
        retval = kStatus_Fail;
    }
#endif

    return retval;
}
