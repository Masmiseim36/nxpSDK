/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define BOARD_FLEXIO_INSTANCE           FLEXIO1
#define BOARD_FLEXIO_IRQN                FLEXIO1_IRQn
#define BOARD_FLEXIO_IRQ_HANDLER        FLEXIO1_IRQHandler
#define BOARD_FLEXIO_DA_PIN_INDEX       0U
#define BOARD_FLEXIO_WE_PIN_INDEX       16U
#define BOARD_FLEXIO_OE_PIN_INDEX       17U
#define BOARD_FLEXIO_ADV_PIN_INDEX      18U
#define BOARD_FLEXIO_CE_PIN_INDEX       19U
#define BOARD_FLEXIO_WE_SHIFTER_INDEX  3U      /* SHIFTER3 support parallel receive */
#define BOARD_FLEXIO_ADV_SHIFTER_INDEX 7U      /* SSHIFTER7 support parallel receive */
#define BOARD_FLEXIO_WE_TIMER_INDEX     0U
#define BOARD_FLEXIO_ADV_TIMER_INDEX    2U
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */