/*
 * Copyright 2019 NXP
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
/* Select OSC_CLK (24 MHz) as master lpspi clock source */
#define LPSPI_CLOCK_SOURCE_SELECT (0U)
/* Clock divider for master lpspi clock source */
#define LPSPI_CLOCK_SOURCE_DIVIDER (2U)
/* Select OSC_CLK (24 MHz) as master lpi2c clock source */
#define LPI2C_CLOCK_SOURCE_SELECT (1U)
/* Clock divider for master lpi2c clock source */
#define LPI2C_CLOCK_SOURCE_DIVIDER (1U)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
