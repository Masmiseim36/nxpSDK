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

#include <stdio.h>
#include "fsl_card.h"
#include "fsl_debug_console.h"
#include "board.h"

#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_common.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/


/* define data buffer size */
#define DATA_BUFFER_SIZE (256U)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*! @brief Card descriptor */
sdio_card_t g_sdio;

/* define the tuple list */
static const uint32_t g_funcTupleList[2U] = {
    SDIO_TPL_CODE_FUNCID, SDIO_TPL_CODE_FUNCE,
};

/*! @brief Data written to the card */
SDK_ALIGN(uint8_t g_dataRead[SDK_SIZEALIGN(DATA_BUFFER_SIZE, SDMMC_DATA_BUFFER_ALIGN_CAHCE)],
          MAX(SDMMC_DATA_BUFFER_ALIGN_CAHCE, HOST_DMA_BUFFER_ADDR_ALIGN));
/*! @brief Data read from the card */
SDK_ALIGN(uint8_t g_dataBlockRead[SDK_SIZEALIGN(DATA_BUFFER_SIZE, SDMMC_DATA_BUFFER_ALIGN_CAHCE)],
          MAX(SDMMC_DATA_BUFFER_ALIGN_CAHCE, HOST_DMA_BUFFER_ADDR_ALIGN));

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Main function
 */
int main(void)
{
    sdio_card_t *card = &g_sdio;

    BOARD_InitPins();
    BOARD_BootClockRUN();
    CLOCK_SetDiv(kCLOCK_Usdhc1Div, 0U);
    BOARD_InitMemory();
    BOARD_InitDebugConsole();

    SystemInitIrqTable();

    PRINTF("SDIO card simple example.\r\n");

    card->host.base = SD_HOST_BASEADDR;
    card->host.sourceClock_Hz = SD_HOST_CLK_FREQ;

    /* Init SDIO card. */
    if (kStatus_Success != SDIO_Init(card))
    {
        PRINTF("\r\nSDIO card init failed.\r\n");
        return kStatus_Fail;
    }

    PRINTF("\r\nSDIO init Done\r\n");

    if (card->ioTotalNumber > 0U)
    {
        /* read function capability through GetCardCapability function first */
        if (kStatus_Success != SDIO_GetCardCapability(card, kSDIO_FunctionNum1))
        {
            return kStatus_Fail;
        }

        PRINTF("\r\nRead function CIS, in direct way\r\n");
        if (kStatus_Success != SDIO_ReadCIS(card, kSDIO_FunctionNum1, g_funcTupleList, 2U))
        {
            return kStatus_Fail;
        }

        PRINTF("\r\nRead function CIS, in extended way, non-block mode, non-word aligned size\r\n");

        if (kStatus_Success != SDIO_IO_Read_Extended(card, kSDIO_FunctionNum0, card->ioFBR[0].ioPointerToCIS,
                                                     g_dataRead, 31U, SDIO_EXTEND_CMD_OP_CODE_MASK))
        {
            return kStatus_Fail;
        }
        PRINTF("\r\nRead function CIS, in extended way, block mode, non-word aligned size\r\n");

        /* set block size to a non-word aligned size for test */
        if (kStatus_Success != SDIO_SetBlockSize(card, kSDIO_FunctionNum0, 31U))
        {
            return kStatus_Fail;
        }

        if (kStatus_Success != SDIO_IO_Read_Extended(card, kSDIO_FunctionNum0, card->ioFBR[0].ioPointerToCIS,
                                                     g_dataBlockRead, 1U,
                                                     SDIO_EXTEND_CMD_OP_CODE_MASK | SDIO_EXTEND_CMD_BLOCK_MODE_MASK))
        {
            return kStatus_Fail;
        }

        if (memcmp(g_dataRead, g_dataBlockRead, 31U))
        {
            PRINTF("\r\nThe read content isn't consistent.\r\n");
        }
        else
        {
            PRINTF("\r\nThe read content is consistent.\r\n");
        }
    }

    SDIO_DeInit(card);

    while (true)
    {
    }
}
