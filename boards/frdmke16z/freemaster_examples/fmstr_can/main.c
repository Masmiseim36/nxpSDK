/*
 * Copyright (c) 2007-2015 Freescale Semiconductor, Inc.
 * Copyright 2018-2021 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * FreeMASTER Communication Driver - Example Application
 */

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////

#include "pin_mux.h"
#include "fsl_mscan.h"
#include "fsl_common.h"
#include "board.h"

#include "freemaster.h"
#include "freemaster_mscan.h"

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
    BOARD_InitPins();
    BOARD_BootClockRUN();

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
    mscan_config_t mscanConfig;

    /* Get MSCAN module default Configuration. */
    /*
     * mscanConfig.baudRate = 1000000U;
     * mscanConfig.enableTimer = false;
     * mscanConfig.enableWakeup = false;
     * mscanConfig.clkSrc = kMSCAN_ClkSrcOsc;
     * mscanConfig.enableLoopBack = false;
     * mscanConfig.enableListen = false;
     * mscanConfig.busoffrecMode = kMSCAN_BusoffrecAuto;
     * mscanConfig.filterConfig.filterMode = kMSCAN_Filter32Bit;
     */
    mscanConfig.enableTimer = true;
    MSCAN_GetDefaultConfig(&mscanConfig);

    mscanConfig.enableTimer = true;
    mscanConfig.baudRate    = 500000U;

    /* Acceptance filter configuration. */
    mscanConfig.filterConfig.u32IDAR0 = 0;
    mscanConfig.filterConfig.u32IDAR1 = 0;
    /* All bits are don't care. */
    mscanConfig.filterConfig.u32IDMR0 = 0xffffffff;
    mscanConfig.filterConfig.u32IDMR1 = 0xffffffff;

    /* Initialize MSCAN module. */
    MSCAN_Init(MSCAN, &mscanConfig, CLOCK_GetFreq(kCLOCK_ScgSysOscAsyncDiv2Clk));

    /* Register communication module used by FreeMASTER driver. */
    FMSTR_CanSetBaseAddress(MSCAN);

#if FMSTR_SHORT_INTR || FMSTR_LONG_INTR
    /* Enable RX fifo0 new message interrupt using interrupt line 0. */
    EnableIRQ(MSCAN_Rx_IRQn);
    EnableIRQ(MSCAN_ORed_IRQn);
    /* Enable interrupts. */
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

void MSCAN_Rx_IRQHandler(void)
{
    /* Call FreeMASTER Interrupt routine handler */
    FMSTR_CanIsr();

    /* May be needed for ARM errata 838869 */
    SDK_ISR_EXIT_BARRIER;
}

void MSCAN_ORed_IRQHandler(void)
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
