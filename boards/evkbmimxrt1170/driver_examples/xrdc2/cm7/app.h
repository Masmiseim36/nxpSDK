/*
 * Copyright 2019-2020 NXP
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

#define DEMO_XRDC2 XRDC2_D0

/* Domain could be 0 ~ (FSL_FEATURE_XRDC2_DOMAIN_COUNT -1 ) */
#define DEMO_CORE_DOMAIN 2

/*
 * Memory
 * M4 LMEM region is used in this example. M7 accesses this region
 * through the address 0x20200000 ~ 0x2023ffff.
 */
#define DEMO_XRDC2_MEM            kXRDC2_Mem_M4LMEM_Region0
#define DEMO_XRDC2_MEM_START_ADDR 0x20200000
#define DEMO_XRDC2_MEM_END_ADDR   0x2023ffff

/* Peripheral */
#define DEMO_XRDC2_PERIPH kXRDC2_Periph_GPIO1

/* Memory Slot */
#define DEMO_XRDC2_MEM_SLOT            kXRDC2_MemSlot_GPV0
#define DEMO_XRDC2_MEM_SLOT_START_ADDR 0x41000000
/* The memory slot region is large, in this example, only access part of region. */
#define DEMO_XRDC2_MEM_SLOT_ACCESS_SIZE 0x10

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);

void DEMO_AssignDomain(void);

void DEMO_SetAllMemAccessible(void);

void DEMO_TouchMemory(void);

void DEMO_TouchPeriph(void);

void DEMO_TouchMemSlot(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
