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
#define LPSPI_TRANSFER_SIZE     64U     /*! Transfer dataSize */
#define LPSPI_TRANSFER_BAUDRATE 500000U /*! Transfer baudrate - 500k */

#define BOARD_EEPROM_LPSPI_BASEADDR (LPSPI1)
#define BOARD_LPSPI_IRQN            LPSPI1_IRQn
#define BOARD_LPSPI_IRQHandler      LPSPI1_IRQHandler

#define BOARD_LPSPI_PCS_FOR_INIT     (kLPSPI_Pcs0)
#define BOARD_LPSPI_PCS_FOR_TRANSFER (kLPSPI_MasterPcs0)

/* Select USB1 PLL PFD0 (720 MHz) as lpspi clock source */
#define BOARD_LPSPI_CLOCK_SOURCE_SELECT (1U)
/* Clock divider for master lpspi clock source */
#define BOARD_LPSPI_CLOCK_SOURCE_DIVIDER (7U)

#define BOARD_LPSPI_CLK_FREQ (CLOCK_GetFreq(kCLOCK_Usb1PllPfd0Clk) / (BOARD_LPSPI_CLOCK_SOURCE_DIVIDER + 1U))
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif
