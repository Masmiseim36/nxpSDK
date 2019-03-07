/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
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
#include "FreeRTOS.h"
#include "task.h"
#include "board.h"
#include "event.h"

#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_common.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*! @brief Data block count accessed in card */
#define DATA_BLOCK_COUNT (5U)
/*! @brief Start data block number accessed in card */
#define DATA_BLOCK_START (2U)
/*! @brief The first group to erase */
#define ERASE_GROUP_START (0U)
/*! @brief The last group to erase */
#define ERASE_GROUP_END (0U)
/*! @brief Data buffer size */
#define DATA_BUFFER_SIZE (FSL_SDMMC_DEFAULT_BLOCK_SIZE * DATA_BLOCK_COUNT)

/* Task stack size and priority. */
#define AccessCardTask_STACK_SIZE (DATA_BUFFER_SIZE + 1000U)
#define MMC_TASK_PRIORITY (configMAX_PRIORITIES - 1U)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief MMC card access task.
 *
 * @param pvParameters Task parameter.
 */
static void AccessCardTask(void *pvParameters);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*! @brief Card descriptor. */
mmc_card_t g_mmc;

/*! @brief Data written to the card */
SDK_ALIGN(uint8_t g_dataWrite[SDK_SIZEALIGN(DATA_BUFFER_SIZE, SDMMC_DATA_BUFFER_ALIGN_CAHCE)],
          MAX(SDMMC_DATA_BUFFER_ALIGN_CAHCE, HOST_DMA_BUFFER_ADDR_ALIGN));
/*! @brief Data read from the card */
SDK_ALIGN(uint8_t g_dataRead[SDK_SIZEALIGN(DATA_BUFFER_SIZE, SDMMC_DATA_BUFFER_ALIGN_CAHCE)],
          MAX(SDMMC_DATA_BUFFER_ALIGN_CAHCE, HOST_DMA_BUFFER_ADDR_ALIGN));

/*******************************************************************************
 * Code
 ******************************************************************************/

/*Provides own implementation of vApplicationIRQHandler() to call SystemIrqHandler()
 *directly without saving the FPU registers on interrupt entry.
 */
void vApplicationIRQHandler(uint32_t ulICCIAR)
{
    SystemIrqHandler(ulICCIAR);
}

static void AccessCardTask(void *pvParameters)
{
    mmc_card_t *card = &g_mmc;
    bool isReadOnly;
    bool failedFlag = false;
    char ch = '0';

/* Open card detection pin NVIC. */
/* set IRQ priority */
#if defined(__CORTEX_M)
    NVIC_SetPriority(MMC_HOST_IRQ, 5U);
#else
    GIC_SetPriority(MMC_HOST_IRQ, 25U);
#endif
    card->host.base = MMC_HOST_BASEADDR;
    card->host.sourceClock_Hz = MMC_HOST_CLK_FREQ;

    /* MMC card VCC supply, only allow 3.3 or 1.8v, depend on your board config.
    * If a power reset circuit is avaliable on you board for mmc, and 1.8v is supported,
    * #define BOARD_USDHC_MMCCARD_POWER_CONTROL_INIT()
    * #define BOARD_USDHC_MMCCARD_POWER_CONTROL(state)
    * in board.h must be implemented.
    * User can remove preset the voltage window and sdmmc will switch VCC automatically. */
    card->hostVoltageWindowVCC = BOARD_MMC_VCC_SUPPLY;

    /* Init card. */
    if (MMC_Init(card))
    {
        PRINTF("\n MMC card init failed \n");
        return;
    }

    PRINTF("\r\nRead/write the card continuously until encounter error......\r\n");
    /* Check if card is readonly. */
    isReadOnly = MMC_CheckReadOnly(card);
    if (isReadOnly)
    {
        while (true)
        {
            if (failedFlag || (ch == 'q'))
            {
                break;
            }

            PRINTF("\r\nRead one data block......\r\n");
            if (kStatus_Success != MMC_ReadBlocks(card, g_dataRead, DATA_BLOCK_START, 1U))
            {
                PRINTF("Read one data block failed.\r\n");
                failedFlag = true;
                continue;
            }

            PRINTF("Read multiple data blocks......\r\n");
            if (kStatus_Success != MMC_ReadBlocks(card, g_dataRead, DATA_BLOCK_START, DATA_BLOCK_COUNT))
            {
                PRINTF("Read multiple data blocks failed.\r\n");
                failedFlag = true;
                continue;
            }

            PRINTF(
                "\r\nInput 'q' to quit read process.\
                \r\nInput other char to read data blocks again.\r\n");
            ch = GETCHAR();
            PUTCHAR(ch);
        }
    }
    else
    {
        memset(g_dataWrite, 1U, sizeof(g_dataWrite));

        while (true)
        {
            if (failedFlag || (ch == 'q'))
            {
                break;
            }

            PRINTF("\r\nWrite/read one data block......\r\n");
            if (kStatus_Success != MMC_WriteBlocks(card, g_dataWrite, DATA_BLOCK_START, 1U))
            {
                PRINTF("Write one data block failed.\r\n");
                failedFlag = true;
                continue;
            }

            memset(g_dataRead, 0U, sizeof(g_dataRead));
            if (kStatus_Success != MMC_ReadBlocks(card, g_dataRead, DATA_BLOCK_START, 1U))
            {
                PRINTF("Read one data block failed.\r\n");
                failedFlag = true;
                continue;
            }

            PRINTF("Compare the read/write content......\r\n");
            if (memcmp(g_dataRead, g_dataWrite, FSL_SDMMC_DEFAULT_BLOCK_SIZE))
            {
                PRINTF("The read/write content isn't consistent.\r\n");
                failedFlag = true;
                continue;
            }
            PRINTF("The read/write content is consistent.\r\n");

            PRINTF("Write/read multiple data blocks......\r\n");
            if (kStatus_Success != MMC_WriteBlocks(card, g_dataWrite, DATA_BLOCK_START, DATA_BLOCK_COUNT))
            {
                PRINTF("Write multiple data blocks failed.\r\n");
                failedFlag = true;
                continue;
            }

            memset(g_dataRead, 0U, sizeof(g_dataRead));
            if (kStatus_Success != MMC_ReadBlocks(card, g_dataRead, DATA_BLOCK_START, DATA_BLOCK_COUNT))
            {
                PRINTF("Read multiple data blocks failed.\r\n");
                failedFlag = true;
                continue;
            }

            PRINTF("Compare the read/write content......\r\n");
            if (memcmp(g_dataRead, g_dataWrite, FSL_SDMMC_DEFAULT_BLOCK_SIZE))
            {
                PRINTF("The read/write content isn't consistent.\r\n");
                failedFlag = true;
                continue;
            }
            PRINTF("The read/write content is consistent.\r\n");

            PRINTF("Erase data groups......\r\n");
            if (kStatus_Success != MMC_EraseGroups(card, ERASE_GROUP_START, ERASE_GROUP_END))
            {
                PRINTF("\n Erases blocks failed \n");
                failedFlag = true;
                continue;
            }

            PRINTF(
                "\r\nInput 'q' to quit read/write/erase process.\
                \r\nInput other char to read/write/erase data blocks again.\r\n");
            ch = GETCHAR();
            PUTCHAR(ch);
        }
    }
    PRINTF("\r\nThe example will not read/write data blocks again.\r\n");

    MMC_Deinit(card);

    vTaskSuspend(NULL);
}

/*! @brief Main function */
int main(void)
{
    BOARD_InitPins();
    BOARD_BootClockRUN();
    CLOCK_SetDiv(kCLOCK_Usdhc1Div, 0U);
    BOARD_InitMemory();
    BOARD_InitDebugConsole();

    SystemInitIrqTable();

    if (pdPASS !=
        xTaskCreate(AccessCardTask, "AccessCardTask", AccessCardTask_STACK_SIZE, NULL, MMC_TASK_PRIORITY, NULL))
    {
        return -1;
    }

    /* Start the tasks and timer running. */
    vTaskStartScheduler();
    /* Scheduler should never reach this point. */
    while (true)
    {
    }
}
