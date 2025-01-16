/*
 * Copyright 2019 NXP
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
}

uint32_t USART0_GetFreq(void)
{
    return CLOCK_GetFlexcommClkFreq(0);
}
uint32_t USART1_GetFreq(void)
{
    return CLOCK_GetFlexcommClkFreq(1);
}
uint32_t USART2_GetFreq(void)
{
    return CLOCK_GetFlexcommClkFreq(2);
}
uint32_t USART3_GetFreq(void)
{
    return CLOCK_GetFlexcommClkFreq(3);
}
uint32_t USART4_GetFreq(void)
{
    return CLOCK_GetFlexcommClkFreq(4);
}
uint32_t USART5_GetFreq(void)
{
    return CLOCK_GetFlexcommClkFreq(5);
}
/*${function:end}*/
