/*
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
#include "fsl_debug_console.h"
#include "fsl_sdma.h"

#include "pin_mux.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define EXAMPLE_DMA SDMAARM
#define EXAMPLE_CHANNEL (1U)
#define BUFFER_LEN 8U
#define BD_QUEUE_SIZE 2U
#define BUFFER_TOTAL_LEN (BUFFER_LEN * BD_QUEUE_SIZE)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
sdma_handle_t g_sdma_Handle;
volatile bool g_Transfer_Done = false;
AT_NONCACHEABLE_SECTION_ALIGN(uint32_t srcAddr[BUFFER_TOTAL_LEN], 4) = {0};
AT_NONCACHEABLE_SECTION_ALIGN(uint32_t destAddr[BUFFER_TOTAL_LEN], 4) = {0};

/* SDMA buffer descriptor */
AT_NONCACHEABLE_SECTION_ALIGN(sdma_buffer_descriptor_t bd[BD_QUEUE_SIZE], 4) = {0};

/* SDMA channel context */
AT_NONCACHEABLE_SECTION_ALIGN(sdma_context_data_t context, 4) = {0};
/*******************************************************************************
 * Code
 ******************************************************************************/

/* User callback function for sdma transfer. */
void sdma_Callback(sdma_handle_t *handle, void *param, bool transferDone, uint32_t tcds)
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
    uint32_t i = 0;
    sdma_transfer_config_t transferConfig;
    sdma_config_t userConfig;

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitMemory();
    BOARD_InitDebugConsole();
    /* Install IRQ Handler */
    SystemInitIrqTable();
    /* Print source buffer */
    PRINTF("\r\nsdma scatter gather transfer example begin.\r\n\r\n");
    PRINTF("Destination Buffer:\r\n");
    for (i = 0; i < BUFFER_TOTAL_LEN; i++)
    {
        srcAddr[i] = i;
        PRINTF("%d\t", destAddr[i]);
    }

    /* Configure sdma one shot transfer */
    SDMA_GetDefaultConfig(&userConfig);
    SDMA_Init(EXAMPLE_DMA, &userConfig);
    SDMA_CreateHandle(&g_sdma_Handle, EXAMPLE_DMA, EXAMPLE_CHANNEL, &context);
    SDMA_SetCallback(&g_sdma_Handle, sdma_Callback, NULL);

    /* Install buffer descritptor list to handle */
    SDMA_InstallBDMemory(&g_sdma_Handle, bd, BD_QUEUE_SIZE);

    /* Configure the buffer descriptor except the last one */
    for (i = 0; i < BD_QUEUE_SIZE - 1U; i++)
    {
        SDMA_ConfigBufferDescriptor(&bd[i], (uint32_t)&srcAddr[i * BUFFER_LEN], (uint32_t)&destAddr[i * BUFFER_LEN],
                                    kSDMA_TransferSize4Bytes, sizeof(uint32_t) * BUFFER_LEN, false, false, false,
                                    kSDMA_MemoryToMemory);
    }

    /* Configure the last buffer descriptor, the last descriptor shall be last one and can generate interrupt */
    SDMA_ConfigBufferDescriptor(&bd[i], (uint32_t)&srcAddr[i * BUFFER_LEN], (uint32_t)&destAddr[i * BUFFER_LEN],
                                kSDMA_TransferSize4Bytes, sizeof(uint32_t) * BUFFER_LEN, true, true, false,
                                kSDMA_MemoryToMemory);

    /* Prepare transfer */
    SDMA_PrepareTransfer(&transferConfig, (uint32_t)srcAddr, (uint32_t)destAddr, sizeof(uint32_t), sizeof(uint32_t),
                         sizeof(uint32_t), BUFFER_LEN * sizeof(uint32_t), 0U, kSDMA_PeripheralTypeMemory,
                         kSDMA_MemoryToMemory);

    /* Submit the transfer contents into SDMA core */
    SDMA_SubmitTransfer(&g_sdma_Handle, &transferConfig);

    /* Set channel priority, or the channel cannot run */
    SDMA_SetChannelPriority(EXAMPLE_DMA, EXAMPLE_CHANNEL, 2U);

    /* Start transfer */
    SDMA_StartTransfer(&g_sdma_Handle);
    /* Wait for sdma transfer finish */
    while (g_Transfer_Done != true)
    {
    }
    /* Print destination buffer */
    PRINTF("\r\n\r\nsdma scatter gather transfer example finish.\r\n\r\n");
    PRINTF("Destination Buffer:\r\n");
    for (i = 0; i < BUFFER_TOTAL_LEN; i++)
    {
        PRINTF("%d\t", destAddr[i]);
    }

    while (1)
    {
    }
}
