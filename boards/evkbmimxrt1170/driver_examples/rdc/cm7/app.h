/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define APP_RDC            RDC
#define APP_CUR_MASTER_DID 0 /* Current master domain ID. */
#define APP_RDC_PERIPH     kRDC_Periph_GPIO_1_6
/* Peripheral 0~63 use semaphore1, 64~127 use semaphore2. */
#define APP_RDC_SEMA42      RDC_SEMAPHORE1
#define APP_RDC_SEMA42_GATE (((uint8_t)APP_RDC_PERIPH) & 0x3F)

/*
 * OCRAM(M4) is used for demonstration here.
 * In this demo, the content of this region might be changed,
 * so the region must not be used to for other purpose, such as
 * used as data section, bss section, and so on.
 */
#define APP_RDC_MEM           kRDC_Mem_MRC6_0
#define APP_RDC_MEM_BASE_ADDR 0x20200000
#define APP_RDC_MEM_END_ADDR  0x20240000

/* The RDC domain ID for ARM core is fixed value. */
#define APP_ASSIGN_DOMAIN_ID_BY_RDC 0

/*
 * Master index:
 * ARM core: 0
 * eDMA: 1
 * DCP: 2
 * Others: 3
 */
#define APP_MASTER_INDEX 0

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
void APP_TouchPeriph(void);
void APP_TouchMem(void);
void APP_AssignCoreDomain(void);
/*${prototype:end}*/
