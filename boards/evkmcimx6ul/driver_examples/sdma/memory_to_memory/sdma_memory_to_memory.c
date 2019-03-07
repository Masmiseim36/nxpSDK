/*
 * Copyright 2017 NXP
 * All rights reserved.
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

#define BUFF_LENGTH 4U

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
sdma_handle_t g_SDMA_Handle = {0};

volatile bool g_Transfer_Done = false;
AT_NONCACHEABLE_SECTION_ALIGN(sdma_context_data_t context, 4) = {0};

AT_NONCACHEABLE_SECTION_ALIGN(uint32_t srcAddr[BUFF_LENGTH], 4) = {0x01, 0x02, 0x03, 0x04};
AT_NONCACHEABLE_SECTION_ALIGN(uint32_t destAddr[BUFF_LENGTH], 4) = {0x00, 0x00, 0x00, 0x00};

/*******************************************************************************
 * Code
 ******************************************************************************/

/* User callback function for SDMA transfer. */
void SDMA_Callback(sdma_handle_t *handle, void *param, bool transferDone, uint32_t bds)
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
    PRINTF("\r\nSDMA memory to memory transfer example begin.\r\n");
    PRINTF("\r\nDestination Buffer:\r\n");
    for (i = 0; i < BUFF_LENGTH; i++)
    {
        PRINTF("%d\t", destAddr[i]);
    }

    /* Configure SDMA one shot transfer */
    SDMA_GetDefaultConfig(&userConfig);
    SDMA_Init(SDMAARM, &userConfig);
    SDMA_CreateHandle(&g_SDMA_Handle, SDMAARM, 1, &context);
    SDMA_SetCallback(&g_SDMA_Handle, SDMA_Callback, NULL);
    SDMA_PrepareTransfer(&transferConfig, (uint32_t)srcAddr, (uint32_t)destAddr, sizeof(srcAddr[0]),
                         sizeof(destAddr[0]), sizeof(srcAddr[0]), sizeof(srcAddr), 0, kSDMA_PeripheralTypeMemory,
                         kSDMA_MemoryToMemory);
    SDMA_SubmitTransfer(&g_SDMA_Handle, &transferConfig);
    SDMA_SetChannelPriority(SDMAARM, 1, 2U);
    SDMA_StartTransfer(&g_SDMA_Handle);
    /* Wait for SDMA transfer finish */
    while (g_Transfer_Done != true)
    {
    }
    /* Print destination buffer */
    PRINTF("\r\nSDMA memory to memory transfer example finish.\r\n");
    PRINTF("Destination Buffer:\r\n");
    for (i = 0; i < BUFF_LENGTH; i++)
    {
        PRINTF("%d\t", destAddr[i]);
    }

    while (1)
    {
    }
}
