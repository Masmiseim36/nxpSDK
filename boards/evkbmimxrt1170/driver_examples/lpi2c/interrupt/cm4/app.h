/*
 * Copyright 2018 NXP
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
#define EXAMPLE_LPI2C_MASTER_BASEADDR LPI2C1
#define EXAMPLE_LPI2C_SLAVE_BASEADDR  LPI2C5

/* Get frequency of lpi2c clock */
#define LPI2C_CLOCK_FREQUENCY (CLOCK_GetFreq(kCLOCK_OscRc48MDiv2))

#define LPI2C_MASTER_CLOCK_FREQUENCY LPI2C_CLOCK_FREQUENCY
#define LPI2C_SLAVE_CLOCK_FREQUENCY  LPI2C_CLOCK_FREQUENCY

#define LPI2C_MASTER_IRQ LPI2C1_IRQn
#define LPI2C_SLAVE_IRQ  LPI2C5_IRQn

#define LPI2C_MASTER_IRQHandler LPI2C1_IRQHandler
#define LPI2C_SLAVE_IRQHandler  LPI2C5_IRQHandler
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
AT_QUICKACCESS_SECTION_CODE(void LPI2C_MASTER_IRQHandler(void));
AT_QUICKACCESS_SECTION_CODE(void LPI2C_SLAVE_IRQHandler(void));

void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
