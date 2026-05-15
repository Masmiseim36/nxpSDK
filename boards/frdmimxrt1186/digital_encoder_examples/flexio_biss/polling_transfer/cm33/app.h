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
#define BOARD_FLEXIO_BASE         FLEXIO1

#define FLEXIO_BISS_DATA_PIN      0U
#define FLEXIO_BISS_CLOCK_PIN     2U
#define FLEXIO_BISS_BASE_SHIFTER  0U
#define FLEXIO_BISS_BASE_TIMER    0U

#define DEMO_FLEXIO_CLOCK         (kCLOCK_Root_Flexio1)
#define FLEXIO_CLOCK_FREQUENCY    (CLOCK_GetRootClockFreq(DEMO_FLEXIO_CLOCK))
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
