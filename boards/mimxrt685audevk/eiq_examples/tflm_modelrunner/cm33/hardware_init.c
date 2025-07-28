/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "timer.h"
#include "fsl_iap.h"
/*${header:end}*/

/*${function:start}*/
int64_t os_clock_now(){
    return TIMER_GetTimeInUS();
}

void BOARD_InitHardware(void)
{
    RESET_PeripheralReset(kHSGPIO2_RST_SHIFT_RSTn);
	/*  Config OTP SHADOW register to enable ROM code QSPI reset function. (Reset PIN is P2_12)*/
    OCOTP->OTP_SHADOW[97] = (uint32_t)((1 << 14) | (2 << 15) | (12 << 18));
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
	/* Switch flexspi clock to FFRO*/
	IAP_FlexspiSetClockSource(3);
  TIMER_Init();
}
/*${function:end}*/
