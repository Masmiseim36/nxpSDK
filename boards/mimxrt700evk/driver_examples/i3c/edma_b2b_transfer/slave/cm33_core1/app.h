/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define EXAMPLE_SLAVE              I3C3
#define I3C_SLAVE_CLOCK_FREQUENCY  CLOCK_GetI3cClkFreq()

#define EXAMPLE_DMA                     DMA3
#define EXAMPLE_I3C_TX_DMA_CHANNEL      (0U)
#define EXAMPLE_I3C_RX_DMA_CHANNEL      (1U)
#define EXAMPLE_I3C_TX_DMA_CHANNEL_MUX  kDmaRequestMuxI3c3Tx
#define EXAMPLE_I3C_RX_DMA_CHANNEL_MUX  kDmaRequestMuxI3c3Rx
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
