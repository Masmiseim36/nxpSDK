/*
 * Copyright 2019 - 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_soc_src.h"
#include "fsl_common.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

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
    uint8_t ch;
    uint32_t flags;

    BOARD_InitHardware();

    PRINTF("Example: SRC Global System Reset.\r\n");

#if defined(DEMO_GET_RESET_STATUS_FLAGS)
    flags = DEMO_GET_RESET_STATUS_FLAGS;
#else
    flags = SRC_GetResetStatusFlags(DEMO_SRC_BASE);
#endif /* DEMO_GET_RESET_STATUS_FLAGS */

    if ((flags & DEMO_GLOBAL_RESET_FLAG) == 0UL)
    {
        APP_GlobalSystemResetInit();
    }
    else
    {
        PRINTF("Global System Reset Occurred.\r\n");
        SRC_ClearGlobalSystemResetStatus(DEMO_SRC_BASE, DEMO_GLOBAL_RESET_FLAG);
    }
    while (1)
    {
        ch = GETCHAR();
        PUTCHAR(ch);
    }
}
