/*
 * Copyright 2025 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/

#define EXAMPLE_SRAMC_BASE          BLK_CTRL_WAKEUPMIX /* Peripheral base pointer alias */

/* External SRAM memory address */
#define EXAMPLE_SRAMC_START_ADDRESS (0x43880000) /* Example Base Address for CS0 */

/* Test Buffer Configuration */
#define BUFFER_SIZE                 (256U)       /* Test buffer size in bytes */

/* Board-specific SRAMC timing setting */

#define EXAMPLE_TURN_AROUND_TIME (0U)

#define EXAMPLE_ADDRESS_HOLD_TIME (2U)

#define EXAMPLE_ADDRESS_SETUP_TIME (6U)

#define EXAMPLE_READ_ENABLE_LOW_TIME (0x20U)

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */