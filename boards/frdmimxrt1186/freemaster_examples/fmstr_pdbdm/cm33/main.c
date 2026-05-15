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
#include "fsl_common.h"
#include "fsl_debug_console.h"

#include "freemaster.h"
#include "freemaster_example.h"

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

#ifdef USE_HYPERRAM
/* In HYPERRAM targets, the PDBDM buffer should be located in a non-cacheable section
  (see the FMSTR_PDBDM_USER_BUFFER also defined in freemaster_cfg.h) */
AT_NONCACHEABLE_SECTION(FMSTR_PDBDM_COMBUFF _pdbdm);
#endif

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

int main(void)
{
    /* Board initialization */
    BOARD_ConfigMPU();
    BOARD_InitDEBUG_UARTPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* This example uses shared code from FreeMASTER generic example application */
    FMSTR_Example_Init();

    PRINTF(
        "\nFreeMASTER PDBDM Example.\n"
        "Connect using FreeMASTER Packet Driven JTAG/BDM Communication Plug-in.\n"
        "This communication cannot be used along with existing debugger session.\n\n");

    while(1)
    {
        /* FreeMASTER example increments several variables periodically,
           use the FreeMASTER PC Host tool to visualize the variables */
        FMSTR_Example_Poll();
    }
}
////////////////////////////////////////////////////////////////////////////////
// EOF
/////////////////////////////////////////////////////////////////////////////////
