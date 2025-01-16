/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_edma.h"
#if defined FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET && FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET
#include "fsl_memory.h"
#endif
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
        if (XCACHE_CCR_ENCACHE_MASK != (XCACHE_CCR_ENCACHE_MASK & XCACHE_PS->CCR))
        {
            XCACHE_EnableCache(XCACHE_PS);
        }
    }
    else
    {
        if (XCACHE_CCR_ENCACHE_MASK == (XCACHE_CCR_ENCACHE_MASK & XCACHE_PS->CCR))
        {
            XCACHE_DisableCache(XCACHE_PS);
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
    return 0x20484000;
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

    /* Configure DMA one shot transfer */
    EDMA_GetDefaultConfig(&config);
    EDMA_Init(EXAMPLE_DMA_BASE, &config);
    EDMA_CreateHandle(&g_DMA_Handle, EXAMPLE_DMA_BASE, 0);
    EDMA_SetCallback(&g_DMA_Handle, EDMA_Callback, userData);
}

void APP_DMAMem2memTransfer(uint8_t *srcAddr, uint32_t srcWidth, uint8_t *dstAddr, uint32_t dstWidth, uint32_t size)
{
    edma_transfer_config_t transferConfig;

/* If there is address offset, convert the address */
#if defined FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET && FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET
    srcAddr = (uint8_t *)(MEMORY_ConvertMemoryMapAddress((uint32_t)srcAddr, kMEMORY_Local2DMA));
    dstAddr = (uint8_t *)(MEMORY_ConvertMemoryMapAddress((uint32_t)dstAddr, kMEMORY_Local2DMA));
#endif
    EDMA_PrepareTransfer(&transferConfig, srcAddr, srcWidth, dstAddr, dstWidth, size, size, kEDMA_MemoryToMemory);
    EDMA_SubmitTransfer(&g_DMA_Handle, &transferConfig);
    EDMA_StartTransfer(&g_DMA_Handle);
}

/*${function:end}*/
