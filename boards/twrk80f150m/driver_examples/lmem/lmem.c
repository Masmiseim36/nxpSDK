/*
 * The Clear BSD License
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 * that the following conditions are met:
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
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
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
#include <stdio.h>
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "fsl_lmem_cache.h"
#include "fsl_sdram.h"
#include "fsl_edma.h"
#include "fsl_dmamux.h"
#include "pin_mux.h"

#include <stdbool.h>
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define EXAMPLE_LMEM LMEM
#define SDRAM_START_ADDRESS (0x70000000U)
#define BUS_CLK_FREQ CLOCK_GetFreq(kCLOCK_BusClk)

/* DATA write test length. */
#define LMEM_MEMORY_DMATRANSFER_LEN 10U
/* DMA Timtout. */
#define LMEM_DMA_TRANSFER_TIMEOUT 0xFFFFU

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*!
 * @brief User callback function for EDMA transfer.
 */
void EDMA_Callback(edma_handle_t *handle, void *param, bool transferDone, uint32_t tcds);

/*******************************************************************************
 * Variables
 ******************************************************************************/

edma_handle_t g_EDMA_Handle;
uint32_t g_count = 0;
volatile bool g_Transfer_Done = false;

/*******************************************************************************
 * Code
 ******************************************************************************/

void EDMA_Callback(edma_handle_t *handle, void *param, bool transferDone, uint32_t tcds)
{
    if (transferDone)
    {
        g_Transfer_Done = true;
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    uint8_t data[LMEM_MEMORY_DMATRANSFER_LEN] = {0};
    uint32_t count;
    uint32_t clockSrc;
    uint32_t sdramAddr = SDRAM_START_ADDRESS;
    bool transferDone = false;
    edma_config_t config;
    edma_transfer_config_t transferConfig;
    bool invalidateResult = false;
    bool pushResult = false;

    uint32_t soptReg;
    uint32_t fbReg;

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    /* Set clock out to flexbus CLKOUT. */
    CLOCK_SetClkOutClock(0);

    /* Sets the Flexbus security level*/
    soptReg = SIM->SOPT2 & ~SIM_SOPT2_FBSL_MASK;
    SIM->SOPT2 = soptReg | SIM_SOPT2_FBSL(3);

    /* Enable the FB_BE_xx_yy signal in Flexbus */
    CLOCK_EnableClock(kCLOCK_Flexbus0);

    fbReg = FB->CSPMCR & ~FB_CSPMCR_GROUP2_MASK;
    FB->CSPMCR = fbReg | FB_CSPMCR_GROUP2(2);
    fbReg = FB->CSPMCR & ~FB_CSPMCR_GROUP3_MASK;
    FB->CSPMCR = fbReg | FB_CSPMCR_GROUP3(2);
    fbReg = FB->CSPMCR & ~FB_CSPMCR_GROUP4_MASK;
    FB->CSPMCR = fbReg | FB_CSPMCR_GROUP4(2);
    fbReg = FB->CSPMCR & ~FB_CSPMCR_GROUP5_MASK;
    FB->CSPMCR = fbReg | FB_CSPMCR_GROUP5(2);
    /* Due to the uart/sdram pin mux multiplex. so enable uart first for log. */
    BOARD_InitPinsForUart();

    PRINTF("\r\n LMEM Controller Cache Example Start.\r\n");

    /* Data initialize. */
    for (count = 0; count < LMEM_MEMORY_DMATRANSFER_LEN; count++)
    {
        data[count] = count;
    }

    /* Configure DMAMUX. */
    DMAMUX_Init(DMAMUX0);
    DMAMUX_SetSource(DMAMUX0, 0, 63);
    DMAMUX_EnableChannel(DMAMUX0, 0);

    /* Initialize EMDA. */
    /*
     * config.enableRoundRobinArbitration = false;
     * config.enableHaltOnError = true;
     * config.enableContinuousLinkMode = false;
     * config.enableDebugMode = false;
     */
    EDMA_GetDefaultConfig(&config);
    EDMA_Init(DMA0, &config);
    EDMA_CreateHandle(&g_EDMA_Handle, DMA0, 0);
    EDMA_SetCallback(&g_EDMA_Handle, EDMA_Callback, &transferDone);

    /* SDRAM Initialize. */
#if (defined TWR_K81F150M) || (defined TWR_K80F150M)
    BOARD_InitPinsForSdram();
#endif
    clockSrc = BUS_CLK_FREQ;
    SDRAM_Init(SDRAM, SDRAM_START_ADDRESS, clockSrc);

    /* Data Initialize. */
    memset((void *)sdramAddr, 0, LMEM_MEMORY_DMATRANSFER_LEN);

    /* Enable System Cache. */
    LMEM_EnableSystemCache(EXAMPLE_LMEM, true);
    /* Update the new data in sdram with EDMA transfer. */
    EDMA_PrepareTransfer(&transferConfig, &data[0], sizeof(data[0]), (void *)sdramAddr, sizeof(data[0]),
                         sizeof(data[0]), sizeof(data), kEDMA_MemoryToMemory);
    EDMA_SubmitTransfer(&g_EDMA_Handle, &transferConfig);
    EDMA_StartTransfer(&g_EDMA_Handle);
    /* Wait for EDMA transfer finished. */
    while ((g_Transfer_Done != true) && (g_count < LMEM_DMA_TRANSFER_TIMEOUT))
    {
        g_count++;
    }

    if (g_count != LMEM_DMA_TRANSFER_TIMEOUT)
    {
        /* Invalidate the cache to update the new data of the sdram memory. */
        LMEM_SystemCacheInvalidateMultiLines(EXAMPLE_LMEM, sdramAddr, LMEM_MEMORY_DMATRANSFER_LEN);

        if (memcmp((void *)&data[0], (void *)sdramAddr, LMEM_MEMORY_DMATRANSFER_LEN) == 0)
        {
            invalidateResult = true;
        }
        /* Update the new data to sdram with cpu access. */
        memset((void *)sdramAddr, 0xaaU, LMEM_MEMORY_DMATRANSFER_LEN);

        /* Push the memory to update the data in physical sdram address before read. */
        LMEM_SystemCachePushMultiLines(EXAMPLE_LMEM, sdramAddr, LMEM_MEMORY_DMATRANSFER_LEN);

        /* Transfer from the sdram to data[]. */
        g_Transfer_Done = false;
        g_count = 0;
        EDMA_PrepareTransfer(&transferConfig, (void *)sdramAddr, sizeof(data[0]), &data[0], sizeof(data[0]),
                             sizeof(data[0]), sizeof(data), kEDMA_MemoryToMemory);

        EDMA_SubmitTransfer(&g_EDMA_Handle, &transferConfig);
        EDMA_StartTransfer(&g_EDMA_Handle);
        /* Wait for EDMA transfer finished. */
        while ((g_Transfer_Done != true) && (g_count < LMEM_DMA_TRANSFER_TIMEOUT))
        {
            g_count++;
        }

        if (g_count != LMEM_DMA_TRANSFER_TIMEOUT)
        {
            if (memcmp((void *)&data[0], (void *)sdramAddr, LMEM_MEMORY_DMATRANSFER_LEN) == 0)
            {
                pushResult = true;
            }
        }
    }

    /* Wait for last UART output finish. */
    count = LMEM_DMA_TRANSFER_TIMEOUT;
    while (count--)
    {
        __NOP();
    }
#if (defined TWR_K81F150M) || (defined TWR_K80F150M)
    /* Set Uart Pin after SDRAM read/write process due to the pin multiplex. */
    BOARD_InitPinsForUart();
#endif
    PRINTF("\r\n");

    /* Disable System Cache. */
    LMEM_EnableSystemCache(EXAMPLE_LMEM, false);

    if (invalidateResult)
    {
        PRINTF("\r\n The data in cache is synchronized with the sdram memory. \r\n");
    }
    else
    {
        PRINTF("\r\n The memory cache invalidate failed.\r\n");
    }

    if (pushResult)
    {
        PRINTF("\r\n The memory for edma transfer is synchronized with sdram memory. \r\n");
    }
    else
    {
        PRINTF("\r\n The memory cache push failed.\r\n");
    }

    PRINTF(" \r\n LMEM controller example end. \r\n");

    while (1)
    {
    }
}
