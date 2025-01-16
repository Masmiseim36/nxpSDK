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
#define EXAMPLE_MASTER             I3C3
#define EXAMPLE_I2C_BAUDRATE       400000
#define EXAMPLE_I3C_OD_BAUDRATE    1500000
#define EXAMPLE_I3C_PP_BAUDRATE    6000000
#define WAIT_TIME                  1000
#define I3C_MASTER_CLOCK_FREQUENCY CLOCK_GetI3cClkFreq()

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
