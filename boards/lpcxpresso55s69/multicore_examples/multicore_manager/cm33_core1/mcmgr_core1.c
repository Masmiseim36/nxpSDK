/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2020 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "mcmgr.h"

#include "fsl_common.h"
#include "pin_mux.h"
#include "fsl_power.h"
#include "fsl_gpio.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define LED_INIT()   GPIO_PinInit(GPIO, BOARD_LED_RED_GPIO_PORT, BOARD_LED_RED_GPIO_PIN, &led_config);
#define LED_TOGGLE() GPIO_PortToggle(GPIO, BOARD_LED_RED_GPIO_PORT, 1u << BOARD_LED_RED_GPIO_PIN);
#define APP_READY_EVENT_DATA (1U)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Application-specific implementation of the SystemInitHook() weak function.
 */
void SystemInitHook(void)
{
    /* Initialize MCMGR - low level multicore management library. Call this
       function as close to the reset entry as possible to allow CoreUp event
       triggering. The SystemInitHook() weak function overloading is used in this
       application. */
    (void)MCMGR_EarlyInit();
}

/*!
 * @brief Main function
 */
int main(void)
{
    uint32_t startupData, i;
    mcmgr_status_t status;

    /* After 100 cycles, an exception will occur */
    uint32_t timeToException = 100U;

    /* Define the init structure for the output LED pin*/
    gpio_pin_config_t led_config = {
        kGPIO_DigitalOutput,
        0,
    };

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
    /* Init board hardware.*/
    /* set BOD VBAT level to 1.65V */
    POWER_SetBodVbatLevel(kPOWER_BodVbatLevel1650mv, kPOWER_BodHystLevel50mv, false);
    /* enable clock for GPIO */
    CLOCK_EnableClock(kCLOCK_Gpio0);
    CLOCK_EnableClock(kCLOCK_Gpio1);
    BOARD_InitBootPins();
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
