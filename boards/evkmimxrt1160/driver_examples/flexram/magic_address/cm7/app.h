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
#define APP_FLEXRAM                  FLEXRAM
#define APP_FLEXRAM_IRQ              FLEXRAM_IRQn
#define APP_FLEXRAM_OCRAM_START_ADDR 0x20360000
#define APP_FLEXRAM_OCRAM_MAGIC_ADDR 0x203600a8
#define APP_FLEXRAM_OCRAM_END_ADDR   0x20380000

#define APP_FLEXRAM_IRQ_HANDLER FLEXRAM_IRQHandler

/*
 * If cache is enabled, this example should maintain the cache to make sure
 * CPU core accesses the memory, not cache only.
 */
#define APP_USING_CACHE 1

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
