/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include <stdint.h>
#include <stdbool.h>
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define EXAMPLE_DMA_BASE            DMA1
#define EXAMPLE_DMAMUX_BASE         DMAMUX1
#define MEM_DMATRANSFER_LEN         10U * FSL_FEATURE_L1DCACHE_LINESIZE_BYTE
#define DMA0_DMA16_DriverIRQHandler DMA_CH_0_16_DriverIRQHandler
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void APP_CacheConfig(bool enable);
uint32_t APP_MemoryInit(void);
void APP_DMAInit(void *userData);
void APP_DMAMem2memTransfer(uint8_t *srcAddr, uint32_t srcWidth, uint8_t *dstAddr, uint32_t dstWidth, uint32_t size);
/*${prototype:end}*/

#endif /* _APP_H_ */
