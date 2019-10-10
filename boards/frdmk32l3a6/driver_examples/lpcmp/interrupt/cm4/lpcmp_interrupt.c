/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "fsl_lpcmp.h"

#include "pin_mux.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_LPCMP_BASE LPCMP0
#define DEMO_LPCMP_USER_CHANNEL 2U
#define DEMO_LPCMP_DAC_CHANNEL 7U
#define DEMO_LPCMP_IRQ_ID LPCMP0_IRQn
#define DEMO_LPCMP_IRQ_HANDLER_FUNC LPCMP0_IRQHandler

#define LED_INIT() LED1_INIT(LOGIC_LED_OFF)
#define LED_ON() LED1_ON()
#define LED_OFF() LED1_OFF()

/*******************************************************************************
 * Prototypes
 ******************************************************************************/


/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Main function
 */
int main(void)
{
    lpcmp_config_t mLpcmpConfigStruct;
    lpcmp_dac_config_t mLpcmpDacConfigStruct;

    /* Initialize hardware. */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    PRINTF("LPCMP Interrupt Example.\r\n");

    /*
     *   k_LpcmpConfigStruct->enableStopMode      = false;
     *   k_LpcmpConfigStruct->enableOutputPin     = false;
     *   k_LpcmpConfigStruct->useUnfilteredOutput = false;
     *   k_LpcmpConfigStruct->enableInvertOutput  = false;
     *   k_LpcmpConfigStruct->hysteresisMode      = kLPCMP_HysteresisLevel0;
     *   k_LpcmpConfigStruct->powerMode           = kLPCMP_LowSpeedPowerMode;
     */
    LPCMP_GetDefaultConfig(&mLpcmpConfigStruct);
    /* Init the LPCMP module. */
    LPCMP_Init(DEMO_LPCMP_BASE, &mLpcmpConfigStruct);

    /* Configure the internal DAC to output half of reference voltage. */
    mLpcmpDacConfigStruct.enableLowPowerMode     = false;
    mLpcmpDacConfigStruct.referenceVoltageSource = kLPCMP_VrefSourceVin2;
    mLpcmpDacConfigStruct.DACValue               = 32U; /* Half of reference voltage. */
    LPCMP_SetDACConfig(DEMO_LPCMP_BASE, &mLpcmpDacConfigStruct);

    /* Configure LPCMP input channels. */
    LPCMP_SetInputChannels(DEMO_LPCMP_BASE, DEMO_LPCMP_USER_CHANNEL, DEMO_LPCMP_DAC_CHANNEL);

    /* Init the LED. */
    LED_INIT();

    /* Enable the interrupt. */
    EnableIRQ(DEMO_LPCMP_IRQ_ID);
    LPCMP_EnableInterrupts(DEMO_LPCMP_BASE, kLPCMP_OutputRisingEventFlag | kLPCMP_OutputFallingEventFlag);

    while (1)
    {
    }
}

/*!
 * @brief ISR for LPCMP interrupt function.
 */
void DEMO_LPCMP_IRQ_HANDLER_FUNC(void)
{
    if (kLPCMP_OutputRisingEventFlag == (kLPCMP_OutputRisingEventFlag & LPCMP_GetStatusFlags(DEMO_LPCMP_BASE)))
    {
        LPCMP_ClearStatusFlags(DEMO_LPCMP_BASE, kLPCMP_OutputRisingEventFlag);
        LED_ON(); /* Turn on the led. */
    }
    else
    {
        LPCMP_ClearStatusFlags(DEMO_LPCMP_BASE, kLPCMP_OutputFallingEventFlag);
        LED_OFF(); /* Turn off the led. */
    }
    /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
      exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}
