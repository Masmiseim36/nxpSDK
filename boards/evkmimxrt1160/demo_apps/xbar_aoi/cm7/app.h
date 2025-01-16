/*
 * Copyright 2021 NXP
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

#define DEMO_PIT_BASEADDR PIT1
#define DEMO_CMP_BASEADDR CMP1
#define DEMO_AOI_BASEADDR AOI1

#define DEMO_XBARA_IRQ_HANDLER_FUNC XBAR1_IRQ_0_1_IRQHandler
#define DEMO_XBARA_IRQ_ID           XBAR1_IRQ_0_1_IRQn

#define DEMO_CMP_USE_ALT_VREF true

#define DEMO_CMP_MINUS_CHANNEL 7U
#define DEMO_CMP_PLUS_CHANNEL  2U
#define DEMO_PIT_CHANNEL       kPIT_Chnl_0

#define BUS_CLK_FREQ CLOCK_GetRootClockFreq(kCLOCK_Root_Bus)

#define DEMO_XBARB_INPUT_CMP_SIGNAL    kXBARB2_InputAcmp1Out
#define DEMO_XBARB_OUTPUT_AOI_SIGNAL_1 kXBARB2_OutputAoi1In00

#define DEMO_XBARB_INPUT_PIT_SIGNAL    kXBARB2_InputPit1Trigger0
#define DEMO_XBARB_OUTPUT_AOI_SIGNAL_2 kXBARB2_OutputAoi1In01

#define DEMO_XBARA_INPUT_AOI_SIGNAL kXBARA1_InputAoi1Out0
#define DEMO_XBARA_OUTPUT_SIGNAL    kXBARA1_OutputDmaChMuxReq81

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
