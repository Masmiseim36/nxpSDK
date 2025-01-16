/*
 * Copyright 2022-2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_CORE0_H_
#define _APP_CORE0_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${header:start}*/
#include "mcmgr.h"
/*${header:end}*/

/*${macro:start}*/
/* Address of memory, from which the secondary core will boot */
#define CORE1_BOOT_ADDRESS    (void *)0x303C0000
#define CORE1_KICKOFF_ADDRESS 0x0
// #define APP_INVALIDATE_CACHE_FOR_SECONDARY_CORE_IMAGE_MEMORY

#if defined(__CC_ARM) || defined(__ARMCC_VERSION)
extern uint32_t Image$$CORE1_REGION$$Base;
extern uint32_t Image$$CORE1_REGION$$Length;
#define CORE1_IMAGE_START &Image$$CORE1_REGION$$Base
#elif defined(__ICCARM__)
#pragma section = "__core1_image"
#define CORE1_IMAGE_START __section_begin("__core1_image")
#elif (defined(__GNUC__)) && (!defined(__MCUXPRESSO))
extern const char core1_image_start[];
extern const char *core1_image_end;
extern int core1_image_size;
#define CORE1_IMAGE_START ((void *)core1_image_start)
#define CORE1_IMAGE_SIZE  ((void *)core1_image_size)
#endif

#define EXAMPLE_SWT_PORT1_PHY_ADDR 0x05U
#define EXAMPLE_NETC_FREQ          CLOCK_GetRootClockFreq(kCLOCK_Root_Netc)

#define EXAMPLE_EP_RING_NUM          3U
#define EXAMPLE_EP_RXBD_NUM          8U
#define EXAMPLE_EP_TXBD_NUM          8U
#define EXAMPLE_EP_BD_ALIGN          128U
#define EXAMPLE_EP_BUFF_SIZE_ALIGN   64U
#define EXAMPLE_EP_RXBUFF_SIZE       1518U
#define EXAMPLE_EP_RXBUFF_SIZE_ALIGN SDK_SIZEALIGN(EXAMPLE_EP_RXBUFF_SIZE, EXAMPLE_EP_BUFF_SIZE_ALIGN)
#define EXAMPLE_EP_TEST_FRAME_SIZE   1000U
/*${macro:end}*/
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
status_t APP_MDIO_Init(void);
status_t APP_PHY_Init(void);
status_t APP_PHY_GetLinkStatus(bool *link);
#ifdef CORE1_IMAGE_COPY_TO_RAM
uint32_t get_core1_image_size(void);
#endif
/*${prototype:end}*/

#endif /* _APP_CORE0_H_ */
