/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "app.h"
#include "fsl_cache.h"
#include "fsl_debug_console.h"
#include "fsl_device_registers.h"
#include "pin_mux.h"

#include "board.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* DMA Timtout. */
#define DMA_TRANSFER_TIMEOUT 0xFFFFU

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

uint32_t g_count = 0;
extern volatile bool g_Transfer_Done;
AT_NONCACHEABLE_SECTION(uint8_t g_data[MEM_DMATRANSFER_LEN]);
/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{
    uint32_t count;
    uint32_t startAddr;
    bool transferDone     = false;
    bool invalidateResult = false;
    bool pushResult       = false;
    volatile uint32_t readDummy;

    BOARD_InitHardware();

    PRINTF("\r\n Cache example start.\r\n");

    /* Application memory region related initialization. */
    startAddr = APP_MemoryInit();
    /* Data initialize. */
    for (count = 0; count < MEM_DMATRANSFER_LEN; count++)
    {
        g_data[count]                   = 0xaa;
        *(uint8_t *)(startAddr + count) = 0;
    }

    /* Make sure the initial data are put into the physical memory */
    DCACHE_CleanByRange(startAddr, MEM_DMATRANSFER_LEN);

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

    /* Update the new data in target memory with EDMA transfer. */
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
         */
        if (memcmp((void *)&g_data[0], (void *)startAddr, MEM_DMATRANSFER_LEN) != 0)
        {
            DCACHE_InvalidateByRange(startAddr, MEM_DMATRANSFER_LEN);

            /* Now the data in cache is align with the real data in test memory. */
            if (memcmp((void *)&g_data[0], (void *)startAddr, MEM_DMATRANSFER_LEN) == 0)
            {
                invalidateResult = true;
            }
        }

        /* Verify data by CPU, but now the data in cache is not sync the real memory. */
        for (count = 0; count < MEM_DMATRANSFER_LEN; count++)
        {
            *(uint8_t *)(startAddr + count) = 0xffu;
        }

        /* Reset to zero. */
        g_Transfer_Done = false;
        g_count         = 0;
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
                /* Push the memory to update the data in physical target memory address
                 * at this moment, the real target memory data will be align with the
                 * data in cache.
                 */
                DCACHE_CleanByRange(startAddr, MEM_DMATRANSFER_LEN);

                /* Transfer from the target memory to data[]. */
                g_Transfer_Done = false;
                g_count         = 0;
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
