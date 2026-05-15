/*
 * Copyright 2019, 2023 NXP
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
#define APP_MECC                      MECC2
#define APP_MECC_IRQ                  MECC2_IRQn
#define APP_MECC_IRQ_HANDLER          MECC2_IRQHandler
#define APP_MECC_OCRAM_SIZE           256 * 1024                    /* 256KB */
#define APP_MECC_OCRAM_START_ADDR     0x20500000                    /* OCRAM2 256KB */
#define APP_MECC_OCRAM_END_ADDR       0x2053FFFF
#define APP_MECC_OCRAM_ADDR_OFFSET    0x20                          /* Offset 0x20 from Ocram start address */
#define APP_MECC_OCRAM_SELECTED_BANK  0U                            /* Ocram bank 0 */
#define APP_MECC_MULTI_BIT_POSTION    2U                            /* 0-base */
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
