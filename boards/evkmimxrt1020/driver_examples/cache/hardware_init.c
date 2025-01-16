/*
 * Copyright 2018-2019 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "fsl_dmamux.h"
#include "fsl_edma.h"
#include "fsl_debug_console.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_cache.h"
#include "fsl_semc.h"
/*${header:end}*/

/*${function:start}*/
/* DATA write test length. */
edma_handle_t g_DMA_Handle;
volatile bool g_Transfer_Done = false;

void APP_CacheConfig(bool enable)
{
    if (enable)
    {
        /* Enable the l1 data cache. */
        L1CACHE_EnableDCache();
    }
    else
    {
        L1CACHE_DisableDCache();
    }
}

void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
}

uint32_t APP_MemoryInit(void)
{
    return 0x20200000;
}

void EDMA_Callback(edma_handle_t *handle, void *param, bool transferDone, uint32_t tcds)
{
    if (transferDone)
    {
        g_Transfer_Done = true;
    }
}

void APP_DMAInit(void *userData)
{
    edma_config_t config;

    /* Configure DMAMUX */
    DMAMUX_Init(DMAMUX);
    DMAMUX_EnableAlwaysOn(DMAMUX, 0, true);
    DMAMUX_EnableChannel(DMAMUX, 0);
    /* Configure DMA one shot transfer */
    EDMA_GetDefaultConfig(&config);
    EDMA_Init(DMA0, &config);
    EDMA_CreateHandle(&g_DMA_Handle, DMA0, 0);
    EDMA_SetCallback(&g_DMA_Handle, EDMA_Callback, userData);
}

void APP_DMAMem2memTransfer(uint8_t *srcAddr, uint32_t srcWidth, uint8_t *dstAddr, uint32_t dstWidth, uint32_t size)
{
    edma_transfer_config_t transferConfig;

    EDMA_PrepareTransfer(&transferConfig, srcAddr, srcWidth, dstAddr, dstWidth, srcWidth, size, kEDMA_MemoryToMemory);
    EDMA_SubmitTransfer(&g_DMA_Handle, &transferConfig);
    EDMA_StartTransfer(&g_DMA_Handle);
}

/*${function:end}*/
