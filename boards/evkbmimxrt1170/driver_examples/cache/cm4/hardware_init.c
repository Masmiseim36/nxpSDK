/*
 * Copyright 2020 NXP
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
#include "app.h"
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
        if (LMEM_PSCCR_ENCACHE_MASK != (LMEM_PSCCR_ENCACHE_MASK & LMEM->PSCCR))
        {
            L1CACHE_EnableSystemCache();
        }
    }
    else
    {
        if (LMEM_PSCCR_ENCACHE_MASK == (LMEM_PSCCR_ENCACHE_MASK & LMEM->PSCCR))
        {
            L1CACHE_DisableSystemCache();
        }
    }
}

void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
}

uint32_t APP_MemoryInit(void)
{
    /* Use OCRAM2 region */
    return 0x202C0000;
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
    DMAMUX_Init(EXAMPLE_DMAMUX_BASE);
    DMAMUX_EnableAlwaysOn(EXAMPLE_DMAMUX_BASE, 0, true);
    DMAMUX_EnableChannel(EXAMPLE_DMAMUX_BASE, 0);
    /* Configure DMA one shot transfer */
    EDMA_GetDefaultConfig(&config);
    EDMA_Init(EXAMPLE_DMA_BASE, &config);
    EDMA_CreateHandle(&g_DMA_Handle, EXAMPLE_DMA_BASE, 0);
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
