/*
 * Copyright 2021 NXP
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
#define APP_MECC                      MECC1
#define APP_MECC_IRQ                  MECC1_INT_IRQn
#define APP_MECC_IRQ_HANDLER          MECC1_INT_IRQHandler
#define APP_MECC_OCRAM_SIZE           512 * 1024                    /* 512KB */
#define APP_MECC_OCRAM_START_ADDR     0x20240000                    /* OCRAM1 512KB */
#define APP_MECC_OCRAM_END_ADDR       0x202BFFFF
#define APP_MECC_OCRAM_ADDR_OFFSET    0x20                          /* Offset 0x20 from Ocram start address */
#define APP_MECC_OCRAM_ECC_START_ADDR (0x20240000 + 512 * 1024 + 8) /* OCRAM1 ECC 64KB */
#define APP_MECC_OCRAM_SELECTED_BANK  0U                            /* Ocram bank 0 */
#define APP_MECC_SINGLE_BIT_POSTION   2U                            /* 0-base */

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
