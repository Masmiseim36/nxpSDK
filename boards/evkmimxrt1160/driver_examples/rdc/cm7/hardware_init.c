/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_gpio.h"
#include "app.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
}

void APP_TouchPeriph(void)
{
    GPIO_PinRead(GPIO1, 0);
}

void APP_TouchMem(void)
{
    /* Touch the memory. */
    (*(volatile uint32_t *)APP_RDC_MEM_BASE_ADDR)++;
}

void APP_AssignCoreDomain(void)
{
    /* The domain ID is fixed. */
}
/*${function:end}*/
