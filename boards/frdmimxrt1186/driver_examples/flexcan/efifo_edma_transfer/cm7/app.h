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
#define EXAMPLE_CAN                CAN1
#define EXAMPLE_CAN_DMA            DMA3
#define FLEXCAN_DMA_REQUEST_SOURCE kDma3RequestMuxCAN1
#define EXAMPLE_CAN_DMA_CHANNEL    (0)

#define TX_MESSAGE_BUFFER_NUM      (0U)
#define EXAMPLE_CAN_CLK_FREQ       CLOCK_GetRootClockFreq(kCLOCK_Root_Can1)
#define USE_IMPROVED_TIMING_CONFIG (1)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
