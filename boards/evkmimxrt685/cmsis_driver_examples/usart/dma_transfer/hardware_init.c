/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "app.h"
#include "pin_mux.h"
#include "board.h"
#include <stdbool.h>
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* attach FRG0 clock to FLEXCOMM0 (debug console) */
    CLOCK_SetFRGClock(BOARD_DEBUG_UART_FRG_CLK);
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    USART0_InitPins();
    BOARD_BootClockRUN();

    DMA_Init(DMA0);
}

uint32_t USART0_GetFreq(void)
{
    return CLOCK_GetFlexCommClkFreq(0U);
}
uint32_t USART1_GetFreq(void)
{
    return CLOCK_GetFlexCommClkFreq(1U);
}
uint32_t USART2_GetFreq(void)
{
    return CLOCK_GetFlexCommClkFreq(2U);
}
uint32_t USART3_GetFreq(void)
{
    return CLOCK_GetFlexCommClkFreq(3U);
}
uint32_t USART4_GetFreq(void)
{
    return CLOCK_GetFlexCommClkFreq(4U);
}
uint32_t USART5_GetFreq(void)
{
    return CLOCK_GetFlexCommClkFreq(5U);
}
/*${function:end}*/
