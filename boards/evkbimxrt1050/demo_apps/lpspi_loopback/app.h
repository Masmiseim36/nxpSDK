/*
 * Copyright 2019 NXP
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
#define LPSPI_TRANSFER_SIZE          20
#define BOARD_EEPROM_LPSPI_BASEADDR  (LPSPI1)
#define BOARD_LPSPI_CLK_FREQ         12000000U
#define BOARD_LPSPI_PCS_FOR_INIT     kLPSPI_Pcs0
#define BOARD_LPSPI_PCS_FOR_TRANSFER kLPSPI_MasterPcs0

/* Select USB1 PLL PFD0 (720 MHz) as lpspi clock source */
#define EXAMPLE_LPSPI_CLOCK_SOURCE_SELECT (1U)
/* Clock divider for master lpspi clock source */
#define EXAMPLE_LPSPI_CLOCK_SOURCE_DIVIDER (7U)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif
