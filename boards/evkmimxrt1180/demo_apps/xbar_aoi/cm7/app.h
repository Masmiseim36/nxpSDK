/*
 * Copyright 2023 NXP
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

/* CMP setting */
#define DEMO_CMP_BASEADDR       CMP1
#define DEMO_CMP_USE_ALT_VREF   true
#define DEMO_CMP_PLUS_CHANNEL   2U
#define DEMO_CMP_MINUS_CHANNEL  7U

/* PIT setting */
#define DEMO_LPIT_BASE          LPIT1
#define DEMO_LPIT_CHANNEL       kLPIT_Chnl_0
#define LPIT_SOURCECLOCK        CLOCK_GetRootClockFreq(kCLOCK_Root_Bus_Aon)

/* XBAR setting */
#define DEMO_XBAR_INPUT_CMP_SIGNAL      kXBAR2_InputAcmp1Out
#define DEMO_XBAR_OUTPUT_AOI_SIGNAL_1   kXBAR2_OutputAoi1In00

#define DEMO_XBAR_INPUT_PIT_SIGNAL      kXBAR2_InputPit1Trigger0
#define DEMO_XBAR_OUTPUT_AOI_SIGNAL_2   kXBAR2_OutputAoi1In01

#define DEMO_XBAR_INPUT_AOI_SIGNAL      kXBAR1_InputAoi1Out0
#define DEMO_XBAR_OUTPUT_SIGNAL         kXBAR1_OutputDma4MuxReq154

#define DEMO_XBAR_IRQ_HANDLER_FUNC      XBAR1_CH0_CH1_IRQHandler
#define DEMO_XBAR_IRQ_ID                XBAR1_CH0_CH1_IRQn

/* AOI setting */
#define DEMO_AOI_BASEADDR               AOI1

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
