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
#define APP_FLEXRAM     FLEXRAM
#define APP_FLEXRAM_IRQ FLEXRAM_IRQn

#define APP_FLEXRAM_IRQ_HANDLER FLEXRAM_IRQHandler

#define APP_FLEXRAM_OCRAM_START_ADDR 0x20280000
#define APP_FLEXRAM_OCRAM_MAGIC_ADDR 0x202800A0

#define APP_FLEXRAM_DTCM_START_ADDR 0x20000000
#define APP_FLEXRAM_DTCM_MAGIC_ADDR 0x200000A0

/* Start adddress shouldn't be 0x00U, because some toolchain(such as ARMGCC) maybe consider it as null point and assert
 * UDF instruction. */
#define APP_FLEXRAM_ITCM_START_ADDR 0x4
#define APP_FLEXRAM_ITCM_MAGIC_ADDR 0xA0

/* OCRAM relocate definition */
#define APP_OCRAM_SIZE              (512 * 1024U)
#define APP_OCRAM_ALLOCATE_BANK_NUM 4
#define APP_ITCM_ALLOCATE_BANK_NUM  8
#define APP_DTCM_ALLOCATE_BANK_NUM  4

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
