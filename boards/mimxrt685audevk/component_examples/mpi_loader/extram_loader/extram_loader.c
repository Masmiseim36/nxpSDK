/*
 * Copyright 2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

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
    void (*appEntry)(void);

    /* Init board hardware. */
    BOARD_InitHardware();

    PRINTF("Loading finished, now enter external RAM application.\r\n");
    appEntry = (void (*)())(*((uint32_t *)((APP_EXTRAM_VECTOR_TABLE) + 4)));
    appEntry();

    /* Will never return. */
    assert(false);
    return 0;
}
