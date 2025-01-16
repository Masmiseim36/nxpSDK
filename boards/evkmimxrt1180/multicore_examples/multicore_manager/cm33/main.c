/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
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
static void MCMGR_RemoteCoreUpEventHandler(uint16_t remoteData, void *context);
static void MCMGR_RemoteCoreDownEventHandler(uint16_t remoteData, void *context);
static void MCMGR_RemoteExceptionEventHandler(uint16_t remoteData, void *context);

/*******************************************************************************
 * Code
 ******************************************************************************/
static volatile uint16_t RemoteReadyEventData = 0U;

static void RemoteReadyEventHandler(uint16_t eventData, void *context)
{
    RemoteReadyEventData = eventData;
}

/*!
 * @brief Main function
 */
int main(void)
{
#ifdef APP_ONE_BUTTON_ONLY
    uint32_t secondary_core_started = 0U;
#endif
    volatile uint32_t resetDone       = 0U;
    volatile uint32_t exceptionNumber = 0U;
    volatile uint32_t startupDone     = 0U;

    /* Initialize MCMGR, install generic event handlers */
    (void)MCMGR_Init();

    /* Init board hardware.*/
    BOARD_InitHardware();

    /* Configure BUTTONs */
    BUTTON_1_INIT();
#ifndef APP_ONE_BUTTON_ONLY
    BUTTON_2_INIT();
#endif
    /* Print the initial banner from Primary core */
    (void)PRINTF("\r\nHello World from the Primary Core!\r\n\n");

#ifdef CORE1_IMAGE_COPY_TO_RAM
    /* This section ensures the secondary core image is copied from flash location to the target RAM memory.
       It consists of several steps: image size calculation and image copying.
       These steps are not required on MCUXpresso IDE which copies the secondary core image to the target memory during
       startup automatically. */
    uint32_t core1_image_size;
    core1_image_size = get_core1_image_size();
    (void)PRINTF("Copy Secondary core image to address: 0x%x, size: %d\r\n", (void *)(char *)CORE1_BOOT_ADDRESS,
                 core1_image_size);

    /* Copy Secondary core application from FLASH to the target memory. */
    (void)memcpy((void *)(char *)CORE1_BOOT_ADDRESS, (void *)CORE1_IMAGE_START, core1_image_size);

#ifdef APP_INVALIDATE_CACHE_FOR_SECONDARY_CORE_IMAGE_MEMORY
    invalidate_cache_for_core1_image_memory(CORE1_BOOT_ADDRESS, core1_image_size);
#endif /* APP_INVALIDATE_CACHE_FOR_SECONDARY_CORE_IMAGE_MEMORY*/
#endif /* CORE1_IMAGE_COPY_TO_RAM */

    /* Install remote core up event handler */
    (void)MCMGR_RegisterEvent(kMCMGR_RemoteCoreUpEvent, MCMGR_RemoteCoreUpEventHandler, (void *)&startupDone);

    /* Install remote core down event handler */
    (void)MCMGR_RegisterEvent(kMCMGR_RemoteCoreDownEvent, MCMGR_RemoteCoreDownEventHandler, (void *)&resetDone);

    /* Install remote exception event handler */
    (void)MCMGR_RegisterEvent(kMCMGR_RemoteExceptionEvent, MCMGR_RemoteExceptionEventHandler, (void *)&exceptionNumber);

    /* Register the application event before starting the secondary core */
    (void)MCMGR_RegisterEvent(kMCMGR_RemoteApplicationEvent, RemoteReadyEventHandler, ((void *)0));

    /* Boot Secondary core application */
    (void)PRINTF("Starting Secondary core.\r\n");
    (void)MCMGR_StartCore(kMCMGR_Core1, (void *)(char *)CORE1_BOOT_ADDRESS, 2, kMCMGR_Start_Synchronous);
#ifdef APP_ONE_BUTTON_ONLY
    secondary_core_started = 1U;
#endif

    /* Wait until the secondary core application signals that it has been started. */
    while (APP_READY_EVENT_DATA != RemoteReadyEventData)
    {
    };

    (void)PRINTF("The secondary core application has been started.\r\n\r\n");
#ifdef APP_ONE_BUTTON_ONLY
    (void)PRINTF("Press the %s button to toggle Secondary core Start/Stop.\r\n", BUTTON_1_NAME);
#else
    (void)PRINTF("Press the %s button to Stop Secondary core.\r\n", BUTTON_1_NAME);
    (void)PRINTF("Press the %s button to Start Secondary core again.\r\n", BUTTON_2_NAME);
#endif
    (void)PRINTF(
        "When no action is taken the secondary core application crashes intentionally after 100 LED toggles (simulated "
        "exception), generating the RemoteExceptionEvent to this core.\r\n");
    (void)PRINTF("Use the Stop and then the Start button to get it running again.\r\n\r\n");

    for (;;)
    {
        /* Stop secondary core execution. */
        if (IS_BUTTON_1_PRESSED())
        {
            if (kStatus_MCMGR_Success == MCMGR_StopCore(kMCMGR_Core1))
            {
#ifdef APP_ONE_BUTTON_ONLY
                secondary_core_started = 0U;
#endif
                (void)PRINTF("Stopped Secondary core.\r\n");
            }
            else
            {
                (void)PRINTF("Secondary core already stopped!\r\n");
            }
            SDK_DelayAtLeastUs(1000000U, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
        }
        /* Start core from reset vector */
        if (IS_BUTTON_2_PRESSED())
        {
            if (kStatus_MCMGR_Success ==
                MCMGR_StartCore(kMCMGR_Core1, (void *)(char *)CORE1_BOOT_ADDRESS, 2, kMCMGR_Start_Synchronous))
            {
#ifdef APP_ONE_BUTTON_ONLY
                secondary_core_started = 1U;
#endif
                (void)PRINTF("Started Secondary core.\r\n");
            }
            else
            {
                (void)PRINTF("Secondary core already started!\r\n");
            }
            SDK_DelayAtLeastUs(1000000U, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
        }

        if (0U != resetDone)
        {
            (void)PRINTF("Secondary core HW reset executed.\r\n");
            resetDone = 0;
        }

        if (0U != exceptionNumber)
        {
            (void)PRINTF("Secondary core is in exception number %d.\r\n", exceptionNumber);
            exceptionNumber = 0;
        }

        if (0U != startupDone)
        {
            (void)PRINTF("Secondary core is in startup code.\r\n");
            startupDone = 0;
        }
    }
}

static void MCMGR_RemoteCoreUpEventHandler(uint16_t remoteData, void *context)
{
    uint32_t *startupDone = (uint32_t *)context;
    *startupDone          = 1U;
}

static void MCMGR_RemoteCoreDownEventHandler(uint16_t remoteData, void *context)
{
    uint32_t *resetDone = (uint32_t *)context;
    *resetDone          = 1U;
}

static void MCMGR_RemoteExceptionEventHandler(uint16_t remoteData, void *context)
{
    uint32_t *exceptionNumber = (uint32_t *)context;
    *exceptionNumber          = (uint32_t)remoteData;
}
