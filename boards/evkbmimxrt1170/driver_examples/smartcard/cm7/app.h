/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define SMARTCARD_Control(base, handle, control, param)   SMARTCARD_EMVSIM_Control(base, handle, control, 0)
#define SMARTCARD_TransferNonBlocking(base, handle, xfer) SMARTCARD_EMVSIM_TransferNonBlocking(base, handle, xfer)
#define SMARTCARD_Init(base, handle, sourceClockHz)       SMARTCARD_EMVSIM_Init(base, handle, sourceClockHz)
#define SMARTCARD_Deinit(base)                            SMARTCARD_EMVSIM_Deinit(base)
#define SMARTCARD_GetTransferRemainingBytes(base, handle) SMARTCARD_EMVSIM_GetTransferRemainingBytes(base, handle)
#define SMARTCARD_AbortTransfer(base, handle)             SMARTCARD_EMVSIM_AbortTransfer(base, handle)

#define CORE_CLK_FREQ CLOCK_GetFreq(kCLOCK_CoreSysClk)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
