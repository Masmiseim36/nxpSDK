/*
 * Copyright (c) 2007-2015 Freescale Semiconductor, Inc.
 * Copyright 2018-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * FreeMASTER Communication Driver - Example Application
 */

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////

#include "pin_mux.h"
#include "fsl_flexcan.h"
#include "fsl_clock.h"
#include "fsl_common.h"
#include "board.h"

#include "freemaster.h"
#include "freemaster_flexcan.h"

#include "freemaster_example.h"

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

//! Note: All global variables accessed by FreeMASTER are defined in a shared
//! freemaster_example.c file

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////

static void init_freemaster_can(void);

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

int main(void)
{
    /* Board initialization */
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();

    /* Clock setting for FLEXCAN */
    clock_root_config_t rootCfg = {0};
    rootCfg.mux                 = 1;
    rootCfg.div                 = 1;
    CLOCK_SetRootClock(kCLOCK_Root_Can3, &rootCfg);

    /* FreeMASTER communication layer initialization */
    init_freemaster_can();

    /* This example uses shared code from FreeMASTER generic example application */
    FMSTR_Example_Init();

    while (1)
    {
        /* FreeMASTER example increments several variables periodically,
           use the FreeMASTER PC Host tool to visualize the variables */
        FMSTR_Example_Poll();
    }
}

/*!
 * @brief CAN Module initialization
 */
static void init_freemaster_can(void)
{
    flexcan_config_t flexcanConfig;
    flexcan_timing_config_t timing_config;
    uint32_t canSrcClock = (CLOCK_GetRootClockFreq(kCLOCK_Root_Can3) / 100000U) * 100000U;

    /* Init FlexCAN module. */
    /*
     * flexcanConfig.clkSrc = kFLEXCAN_ClkSrcOsc;
     * flexcanConfig.baudRate = 1000000U;
     * flexcanConfig.baudRateFD = 2000000U;
     * flexcanConfig.maxMbNum = 16;
     * flexcanConfig.enableLoopBack = false;
     * flexcanConfig.enableSelfWakeup = false;
     * flexcanConfig.enableIndividMask = false;
     * flexcanConfig.enableDoze = false;
     */
    FLEXCAN_GetDefaultConfig(&flexcanConfig);

    flexcanConfig.clkSrc   = kFLEXCAN_ClkSrc0;
    flexcanConfig.baudRate = 500000U;

    /* Update the improved timing configuration*/
    if (FLEXCAN_CalculateImprovedTimingValues(CAN3, flexcanConfig.baudRate, canSrcClock, &timing_config))
        flexcanConfig.timingConfig = timing_config;

    FLEXCAN_Init(CAN3, &flexcanConfig, canSrcClock);

    /* Register communication module used by FreeMASTER driver. */
    FMSTR_CanSetBaseAddress(CAN3);

#if FMSTR_SHORT_INTR || FMSTR_LONG_INTR
    /* Enable CAN interrupt. */
    EnableIRQ(CAN3_IRQn);
    EnableGlobalIRQ(0);
#endif
}

#if FMSTR_SHORT_INTR || FMSTR_LONG_INTR
/*
 *   Application interrupt handler of communication peripheral used in interrupt modes
 *   of FreeMASTER communication.
 *
 *   NXP MCUXpresso SDK framework defines interrupt vector table as a part of "startup_XXXXXX.x"
 *   assembler/C file. The table points to weakly defined symbols, which may be overwritten by the
 *   application specific implementation. FreeMASTER overrides the original weak definition and
 *   redirects the call to its own handler.
 *
 */

void CAN3_IRQHandler(void)
{
    /* Call FreeMASTER Interrupt routine handler */
    FMSTR_CanIsr();

    /* May be needed for ARM errata 838869 */
    SDK_ISR_EXIT_BARRIER;
}
#endif

////////////////////////////////////////////////////////////////////////////////
// EOF
/////////////////////////////////////////////////////////////////////////////////
