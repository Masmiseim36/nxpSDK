/*
 * Copyright 2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/

#define DMAREQ_DMIC0            16U
#define APP_DMAREQ_CHANNEL      DMAREQ_DMIC0
#define APP_DMIC_CHANNEL        kDMIC_Channel0
#define APP_DMIC_CHANNEL_ENABLE DMIC_CHANEN_EN_CH0(1)
#define APP_DMIC_FIFO_DELAY_MS  15U
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
