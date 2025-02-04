/*
 * Copyright 2019 NXP
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
#define DEMO_XBARA_BASEADDR XBARA1
#define DEMO_XBARB_BASEADDR XBARB2

#define DEMO_XBARA_IRQ_HANDLER_FUNC XBAR1_IRQ_0_1_IRQHandler
#define DEMO_XBARA_IRQ_ID           XBAR1_IRQ_0_1_IRQn

#define DEMO_XBARB_INPUT_CMP_SIGNAL    kXBARB2_InputAcmp1Out
#define DEMO_XBARB_OUTPUT_AOI_SIGNAL_1 kXBARB2_OutputAoi1In00

#define DEMO_XBARB_INPUT_PIT_SIGNAL    kXBARB2_InputPitTrigger0
#define DEMO_XBARB_OUTPUT_AOI_SIGNAL_2 kXBARB2_OutputAoi1In01

#define DEMO_XBARA_INPUT_AOI_SIGNAL kXBARA1_InputAoi1Out0
#define DEMO_XBARA_OUTPUT_SIGNAL    kXBARA1_OutputDmaChMuxReq30

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
