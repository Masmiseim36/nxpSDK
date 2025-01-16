/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2020 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "app.h"
#include "mcmgr.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define APP_READY_EVENT_DATA (1U)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{
    uint32_t startupData, i;
    mcmgr_status_t status;

    /* After 100 cycles, an exception will occur */
    uint32_t timeToException = 100U;

    /* Init board hardware.*/
    BOARD_InitHardware();

    /* Initialize MCMGR, install generic event handlers */
    (void)MCMGR_Init();

    /* Get the startup data */
    do
    {
        status = MCMGR_GetStartupData(&startupData);
    } while (status != kStatus_MCMGR_Success);

    /* Make a noticable delay after the reset */
    /* Use startup parameter from the master core... */
    for (i = 0; i < startupData; i++)
    {
        SDK_DelayAtLeastUs(1000000U, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    }

    /* Configure LED */
    LED_INIT();

    /* Signal the other core we are ready by triggering the event and passing the APP_READY_EVENT_DATA */
    (void)MCMGR_TriggerEvent(kMCMGR_RemoteApplicationEvent, APP_READY_EVENT_DATA);

    for (;;)
    {
        SDK_DelayAtLeastUs(500000U, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
        LED_TOGGLE();

        if (--timeToException == 0U)
        {
            /* Trigger an exception here! Try to write to an invalid address! */
            *((uint32_t *)0xFFFFFFFFU) = timeToException;
            /* mcmgr.c contains an exception handler,
               which will signal the exception to the other core */
        }
    }
}
