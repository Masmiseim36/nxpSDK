/*
 * Copyright (c) 2007-2015 Freescale Semiconductor, Inc.
 * Copyright 2018-2019, 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * FreeMASTER Communication Driver - Example Application
 */

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////

#include "board.h"
#include "pin_mux.h"
#include "fsl_flexcan.h"
#include "fsl_clock.h"
#include "fsl_common.h"
#include "fsl_debug_console.h"

#include "freemaster.h"
#include "freemaster_flexcan.h"

#include "freemaster_example.h"

////////////////////////////////////////////////////////////////////////////////
// Defines
////////////////////////////////////////////////////////////////////////////////
#define EXAMPLE_CAN_BASE                      CAN1
#define EXAMPLE_CAN_INTERRUPT                 CAN1_IRQn
#define EXAMPLE_CAN_INTERRUPT_HANDLER         CAN1_IRQHandler
#define EXAMPLE_CAN_CLOCK_FREQUENCY           CLOCK_GetRootClockFreq(kCLOCK_Root_Can1)
#define EXAMPLE_CAN_BITRATE                   500000U
#define EXAMPLE_CAN_FD_BITRATE                2000000U

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
    BOARD_InitDEBUG_UARTPins();
    BOARD_InitCANPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* FreeMASTER communication layer initialization */
    init_freemaster_can();

    /* This example uses shared code from FreeMASTER generic example application */
    FMSTR_Example_Init();

    PRINTF(
       "\nFreeMASTER CAN Example.\n"
        "Connect using CAN and use FreeMASTER over CAN plug-in.\n\n");

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
    flexcan_config_t flexcanConfig = { 0 };
    flexcan_timing_config_t timing_config = { 0 };
    uint32_t canSrcClock = EXAMPLE_CAN_CLOCK_FREQUENCY;

    /* Get FlexCAN default configuration */
    FLEXCAN_GetDefaultConfig(&flexcanConfig);

    /* Update configuration */
    flexcanConfig.clkSrc   = kFLEXCAN_ClkSrcOsc;
    flexcanConfig.bitRate = EXAMPLE_CAN_BITRATE;

    /* FD bitrate applied only if FMSTR_CAN_USE_CANFD enabled */
    flexcanConfig.bitRateFD = EXAMPLE_CAN_FD_BITRATE;

    /* Update the improved timing configuration */
    if (FLEXCAN_FDCalculateImprovedTimingValues(EXAMPLE_CAN_BASE, flexcanConfig.bitRate, flexcanConfig.bitRateFD, canSrcClock, &timing_config))
        flexcanConfig.timingConfig = timing_config;

    /* Init FlexCAN module. */
    FLEXCAN_FDInit(EXAMPLE_CAN_BASE, &flexcanConfig, canSrcClock, kFLEXCAN_64BperMB, true);

    /* Register communication module used by FreeMASTER driver. */
    FMSTR_CanSetBaseAddress(EXAMPLE_CAN_BASE);

#if FMSTR_SHORT_INTR || FMSTR_LONG_INTR
    /* Enable CAN interrupt. */
    EnableIRQ(EXAMPLE_CAN_INTERRUPT);
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

void EXAMPLE_CAN_INTERRUPT_HANDLER(void)
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
