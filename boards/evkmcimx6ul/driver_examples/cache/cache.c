/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "board.h"
#include "fsl_cache.h"
#include "fsl_debug_console.h"
#include "fsl_device_registers.h"
#include "pin_mux.h"

#include "clock_config.h"
#include "fsl_sdma.h"
#include <stdint.h>
#include <stdbool.h>
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define MEM_DMATRANSFER_LEN 10U*FSL_FEATURE_L1DCACHE_LINESIZE_BYTE

/* DMA Timtout. */
#define DMA_TRANSFER_TIMEOUT 0xFFFFU

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
uint32_t APP_MemoryInit(void);
void APP_CacheConfig(bool enable);
void APP_DMAInit(void *userData);
void APP_DMAMem2memTransfer(uint8_t *srcAddr, uint32_t srcWidth, 
       uint8_t *dstAddr, uint32_t dstWidth, uint32_t size);

/*******************************************************************************
 * Variables
 ******************************************************************************/

uint32_t g_count = 0;
extern volatile bool g_Transfer_Done;
AT_NONCACHEABLE_SECTION(uint8_t g_data[MEM_DMATRANSFER_LEN]);
/*******************************************************************************
 * Code
 ******************************************************************************/
AT_NONCACHEABLE_SECTION(sdma_handle_t g_SDMA_Handle);
AT_NONCACHEABLE_SECTION_ALIGN(sdma_context_data_t context, 4);
volatile bool g_Transfer_Done = false;
SDK_ALIGN(uint8_t g_MemoryTest[MEM_DMATRANSFER_LEN], FSL_FEATURE_L1DCACHE_LINESIZE_BYTE);


void SDMA_Callback(sdma_handle_t *handle, void *param, bool transferDone, uint32_t bds)
{
    if (transferDone)
    {
        g_Transfer_Done = true;
    }
}

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

uint32_t APP_MemoryInit(void)
{
    /* First disable cache for data initialization. */
    L1CACHE_DisableDCache();
    /* Use the DDR memory (except the non-cacheable region) for test. */
    return (uint32_t)&g_MemoryTest[0];
}

void APP_DMAInit(void *userData)
{
    sdma_config_t userConfig;
    /* Configure SDMA one shot transfer */
    SDMA_GetDefaultConfig(&userConfig);
    SDMA_Init(SDMAARM, &userConfig);
    SDMA_CreateHandle(&g_SDMA_Handle, SDMAARM, 1, &context);
    SDMA_SetCallback(&g_SDMA_Handle, SDMA_Callback, NULL);    
}

void APP_DMAMem2memTransfer(uint8_t *srcAddr, uint32_t srcWidth, uint8_t *dstAddr, uint32_t dstWidth, uint32_t size)
{
    sdma_transfer_config_t transferConfig;

    SDMA_PrepareTransfer(&transferConfig, (uint32_t)srcAddr, (uint32_t)dstAddr, sizeof(srcAddr[0]),
                         sizeof(dstAddr[0]), sizeof(srcAddr[0]), size, 0, kSDMA_PeripheralTypeMemory, kSDMA_MemoryToMemory);
    SDMA_SubmitTransfer(&g_SDMA_Handle, &transferConfig);

    SDMA_SetChannelPriority(SDMAARM, 1, 2U);
    SDMA_StartTransfer(&g_SDMA_Handle);
}
/*!
 * @brief Main function
 */
int main(void)
{
    uint32_t count;
    uint32_t startAddr;
    bool transferDone = false;
    bool invalidateResult = false;
    bool pushResult = false;
    volatile uint32_t readDummy;

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitMemory();
    BOARD_InitDebugConsole();
    /* Install IRQ Handler */
    SystemInitIrqTable();

    PRINTF("\r\n Cache example start.\r\n");

    /* Application memory region related initialization. */
    startAddr = APP_MemoryInit();
    /* Data initialize. */
    for (count = 0; count < MEM_DMATRANSFER_LEN; count++)
    {
        g_data[count] = 0xaa;
        *(uint8_t *)(startAddr + count) = 0;
    }
    /* Configure Cache. */
    APP_CacheConfig(true);
    /* Initialize DMA. */
    APP_DMAInit(&transferDone);

    for (count = 0; count < MEM_DMATRANSFER_LEN; count++)
    {
        /* Access the memory first. */
        readDummy = *(uint8_t *)(startAddr + count);
        (void)readDummy;
    }

    /* Update the new data in sdram with EDMA transfer. */
    APP_DMAMem2memTransfer(&g_data[0], sizeof(g_data[0]), (void *)startAddr, sizeof(g_data[0]), sizeof(g_data));

    /* Wait for EDMA transfer finished. */
    while ((g_Transfer_Done != true) && (g_count < DMA_TRANSFER_TIMEOUT))
    {
        g_count++;
    }

    if (g_count != DMA_TRANSFER_TIMEOUT)
    {
        /* Invalidate the cache to update the new data of the test memory
         * when we wants to get the real data in the test memory.
         * Note:
         L2CACHE_InvalidateByRange(startAddr, MEM_DMATRANSFER_LEN);
         L1CACHE_InvalidateDCacheByRange(startAddr, MEM_DMATRANSFER_LEN);
         *  can be replaced by using DCACHE_InvalidateByRange(startAddr, MEM_DMATRANSFER_LEN)
         */
#if (FSL_FEATURE_SOC_L2CACHEC_COUNT > 0)
#if defined(FSL_SDK_DISBLE_L2CACHE_PRESENT) && !FSL_SDK_DISBLE_L2CACHE_PRESENT
        L2CACHE_InvalidateByRange(startAddr, MEM_DMATRANSFER_LEN);
#endif /* !FSL_SDK_DISBLE_L2CACHE_PRESENT */
#endif /* FSL_FEATURE_SOC_L2CACHEC_COUNT > 0 */
        if (memcmp((void *)&g_data[0], (void *)startAddr, MEM_DMATRANSFER_LEN) != 0)
        {
            L1CACHE_InvalidateDCacheByRange(startAddr, MEM_DMATRANSFER_LEN);

            /* Now the data in cache is align with the real data in test memory. */
            if (memcmp((void *)&g_data[0], (void *)startAddr, MEM_DMATRANSFER_LEN) == 0)
            {
                invalidateResult = true;
            }
        }

        /* Verify data by CPU, but now the data in cache is not sync the the real memory. */
        for (count = 0; count < MEM_DMATRANSFER_LEN; count++)
        {
            *(uint8_t *)(startAddr + count) = 0xffu;
        }

        /* Reset to zero. */
        g_Transfer_Done = false;
        g_count = 0;
        /* Get the real data in the memory . */
        APP_DMAMem2memTransfer((void *)startAddr, sizeof(g_data[0]), &g_data[0], sizeof(g_data[0]), sizeof(g_data));

        /* Wait for EDMA transfer finished. */
        while ((g_Transfer_Done != true) && (g_count < DMA_TRANSFER_TIMEOUT))
        {
            g_count++;
        }

        if (g_count != DMA_TRANSFER_TIMEOUT)
        {
            if (memcmp((void *)&g_data[0], (void *)startAddr, MEM_DMATRANSFER_LEN) != 0)
            {
                /* Push the memory to update the data in physical sdram address
                 * at this moment, the real sdram data will be align with the
                 * data in cache. Note:
                 L1CACHE_CleanDCacheByRange(startAddr, MEM_DMATRANSFER_LEN);
                 L2CACHE_CleanByRange(startAddr, MEM_DMATRANSFER_LEN);
                 *  can be replaced by using DCACHE_CleanByRange(startAddr, MEM_DMATRANSFER_LEN)
                 */
                L1CACHE_CleanDCacheByRange(startAddr, MEM_DMATRANSFER_LEN);
#if (FSL_FEATURE_SOC_L2CACHEC_COUNT > 0)
#if defined(FSL_SDK_DISBLE_L2CACHE_PRESENT) && !FSL_SDK_DISBLE_L2CACHE_PRESENT
                L2CACHE_CleanByRange(startAddr, MEM_DMATRANSFER_LEN);
#endif /* !FSL_SDK_DISBLE_L2CACHE_PRESENT */
#endif /* FSL_FEATURE_SOC_L2CACHEC_COUNT > 0 */

                /* Transfer from the sdram to data[]. */
                g_Transfer_Done = false;
                g_count = 0;
                APP_DMAMem2memTransfer((void *)startAddr, sizeof(g_data[0]), &g_data[0], sizeof(g_data[0]),
                                       sizeof(g_data));

                /* Wait for EDMA transfer finished. */
                while ((g_Transfer_Done != true) && (g_count < DMA_TRANSFER_TIMEOUT))
                {
                    g_count++;
                }

                if (g_count != DMA_TRANSFER_TIMEOUT)
                {
                    if (memcmp((void *)&g_data[0], (void *)startAddr, MEM_DMATRANSFER_LEN) == 0)
                    {
                        pushResult = true;
                    }
                }
            }
        }
    }

    /* Wait for last UART output finish. */
    count = DMA_TRANSFER_TIMEOUT;
    while (count--)
    {
        __NOP();
    }

    APP_CacheConfig(false);

    if (invalidateResult)
    {
        PRINTF("\r\n Cache Invalidate success! \r\n");
    }
    else
    {
        PRINTF("\r\n The memory cache invalidate failed!\r\n");
    }

    if (pushResult)
    {
        PRINTF("\r\n Cache Clean success! \r\n");
    }
    else
    {
        PRINTF("\r\n The memory cache clean failure!\r\n");
    }

    PRINTF(" \r\n Cache example end. \r\n");

    while (1)
    {
    }
}
