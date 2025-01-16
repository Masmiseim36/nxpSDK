/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
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
#define DEMO_XBARA_USER_CHANNEL_INPUT  kXBARA1_InputPitTrigger0
#define DEMO_XBARA_USER_CHANNEL_OUTPUT kXBARA1_OutputDmaChMuxReq30
#define BUS_CLK_FREQ                   CLOCK_GetFreq(kCLOCK_OscClk)
#define DEMO_XBARA_BASEADDR            XBARA1
#define DEMO_PIT_BASEADDR              PIT
#define DEMO_XBARA_IRQn                XBAR1_IRQ_0_1_IRQn
#define DEMO_XBARA_IRQHandler          XBAR1_IRQ_0_1_IRQHandler
#define PIT_CHANNEL                    kPIT_Chnl_0
#define PIT_PERIOD                     1000000U

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
