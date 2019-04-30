/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_acmp.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "fsl_gpio.h"

#include "fsl_common.h"
#include "pin_mux.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_ACMP_BASEADDR CMP1
#define DEMO_ACMP_MINUS_INPUT 6U
#define DEMO_ACMP_PLUS_INPUT 7U /*  Internal 8bit DAC output. */
#define DEMO_ACMP_IRQ_ID CMP1_IRQn
#define DEMO_ACMP_IRQ_HANDLER_FUNC CMP1_IRQHandler


/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile uint32_t g_acmpOutput = 0U;

/*******************************************************************************
 * Code
 ******************************************************************************/

void DEMO_ACMP_IRQ_HANDLER_FUNC(void)
{
    uint32_t statusFlags;

    statusFlags = ACMP_GetStatusFlags(DEMO_ACMP_BASEADDR);
    ACMP_ClearStatusFlags(DEMO_ACMP_BASEADDR, statusFlags);

    if ((kACMP_OutputAssertEventFlag == (statusFlags & kACMP_OutputAssertEventFlag)))
    {
        g_acmpOutput = 1U;
    }
    else if ((kACMP_OutputAssertEventFlag != (statusFlags & kACMP_OutputAssertEventFlag)))
    {
        g_acmpOutput = 0U;
    }
    else
    {
        /* Unknown interrupt. */
    }
    /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
      exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}

/*!
 * @brief Main function
 */
int main(void)
{
    acmp_config_t acmpConfigStruct;
    acmp_channel_config_t channelConfigStruct;
    acmp_dac_config_t dacConfigStruct;
    acmp_discrete_mode_config_t acmpDiscreteconfig;

    /*Enable PMC0->ACTRL BUFEN bit filed to make sure ACMP voltage reference available*/
    PMC0->ACTRL |= PMC0_ACTRL_BUFEN_MASK;

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /* Configure ACMP. */
    /*
     * acmpConfigStruct.enableHighSpeed = false;
     * acmpConfigStruct.enableInvertOutput = false;
     * acmpConfigStruct.useUnfilteredOutput = false;
     * acmpConfigStruct.enablePinOut = false;
     * acmpConfigStruct.offsetMode = kACMP_OffsetLevel0;
     * acmpConfigStruct.hysteresisMode = kACMP_HysteresisLevel0;
     */
    ACMP_GetDefaultConfig(&acmpConfigStruct);
    ACMP_Init(DEMO_ACMP_BASEADDR, &acmpConfigStruct);

    /* Configure negative inputs are coming from 3v domain. */
    ACMP_GetDefaultDiscreteModeConfig(&acmpDiscreteconfig);
    acmpDiscreteconfig.enableNegativeChannelDiscreteMode = true;
    ACMP_SetDiscreteModeConfig(DEMO_ACMP_BASEADDR, &acmpDiscreteconfig);

    /* Configure channel. Select the positive port input from DAC and negative port input from minus mux input. */
    channelConfigStruct.minusMuxInput = DEMO_ACMP_MINUS_INPUT;
    channelConfigStruct.plusMuxInput  = DEMO_ACMP_PLUS_INPUT;
    ACMP_SetChannelConfig(DEMO_ACMP_BASEADDR, &channelConfigStruct);

    /* Configure DAC. */
    dacConfigStruct.referenceVoltageSource = kACMP_VrefSourceVin1;
    dacConfigStruct.DACValue               = 0x7FU; /* Half of referene voltage. */
    dacConfigStruct.enableOutput           = false;
    dacConfigStruct.workMode               = kACMP_DACWorkLowSpeedMode;
    ACMP_SetDACConfig(DEMO_ACMP_BASEADDR, &dacConfigStruct);

    /* Enable the interrupts. */
    ACMP_EnableInterrupts(DEMO_ACMP_BASEADDR, kACMP_OutputRisingInterruptEnable | kACMP_OutputFallingInterruptEnable);
    EnableIRQ(DEMO_ACMP_IRQ_ID);

    ACMP_Enable(DEMO_ACMP_BASEADDR, true);

    /* Get ACMP's output initial value. */
    if (kACMP_OutputAssertEventFlag == (kACMP_OutputAssertEventFlag & ACMP_GetStatusFlags(DEMO_ACMP_BASEADDR)))
    {
        g_acmpOutput = 1U;
    }
    else
    {
        g_acmpOutput = 0U;
    }

    PRINTF("The example compares analog input to the reference DAC output(CMP positive port).\r\n");
    PRINTF("The terminal will print CMP's output value when press any key.\r\n");
    PRINTF("Please press any key to get CMP's output value.\r\n");

    while (true)
    {
        GETCHAR();
        /* Check the comparison result. */
        if (g_acmpOutput == 0U)
        {
            PRINTF("The analog input is HIGHER than DAC output\r\n");
        }
        else if (g_acmpOutput == 1U)
        {
            PRINTF("The analog input is LOWER than DAC output\r\n");
        }
        else
        {
            /* Unknown status. */
        }
    }
}
