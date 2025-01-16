/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include <stdbool.h>
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* Enable the FlexSPI reset pin P2_12. */
    OCOTP->OTP_SHADOW[0x61] |= (1U << 14U);  /* FlexSPI reset pin enable */
    OCOTP->OTP_SHADOW[0x61] |= (2U << 15U);  /* FlexSPI reset port */
    OCOTP->OTP_SHADOW[0x61] |= (12U << 18U); /* FlexSPI reset pin */
}

bool EXAMPLE_IsValidInterface(uint8_t idx)
{
    return (idx <= 8U) && (idx != 5U) && (idx != 6U);
}

/*${function:end}*/
