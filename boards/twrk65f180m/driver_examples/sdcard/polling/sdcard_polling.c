/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
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
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
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
#include "fsl_port.h"
#include "fsl_gpio.h"
#include "fsl_uart.h"
#include "fsl_sdhc.h"
#include "fsl_card.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "event.h"

#include "fsl_mpu.h"
#include "pin_mux.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief The maximum timeout time for the data event */
#define EVENT_TIMEOUT_CARD_DETECT (~0U)

/*! @brief Data block count accessed in card */
#define DATA_BLOCK_COUNT (5U)
/*! @brief Start data block number accessed in card */
#define DATA_BLOCK_START (2U)
/*! @brief Data buffer size */
#define DATA_BUFFER_SIZE (FSL_SDMMC_DEFAULT_BLOCK_SIZE * DATA_BLOCK_COUNT)

/*! @brief ADMA table length united as word
 *
 * SD card driver can't support ADMA1 transfer mode currently.
 * One ADMA2 table item occupy two words which can transfer maximum 0xFFFFU bytes one time.
 * The more data to be transferred in one time, the bigger value of SDHC_ADMA_TABLE_WORDS need to be set.
 */
#define SDHC_ADMA_TABLE_WORDS (8U)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief Card detect interrupt handler
 */
void DetectCardByGpio(void);

/*!
 * @brief Delay some time.
 *
 * @param milliseconds Time united in milliseconds.
 */
void Delay(uint32_t milliseconds);

/*!
 * @brief User defined SDHC transfer function
 *
 * @param base SDHC peripheral base address.
 * @param content Transfer content.
 * @retval kStatus_Fail Transfer failed.
 * @retval kStatus_Success Transfer successfully.
 */
status_t SDHC_TransferFunction(SDHC_Type *base, sdhc_transfer_t *content);

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*! @brief SDHC ADMA table. */
uint32_t g_sdhcAdmaTable[SDHC_ADMA_TABLE_WORDS];

/*! @brief Card descriptor */
sd_card_t g_sd;
/*! @brief Card detect flag */
static volatile uint32_t g_sdInsertedFlag;

/*! @brief Data written to the card */
uint8_t g_dataWrite[DATA_BUFFER_SIZE];
/*! @brief Data read from the card */
uint8_t g_dataRead[DATA_BUFFER_SIZE];

/*******************************************************************************
 * Code
 ******************************************************************************/
/* Card detect interrupt handler. */
void DetectCardByGpio(void)
{
    if (GPIO_ReadPinInput(BOARD_SDHC_CD_GPIO_BASE, BOARD_SDHC_CD_GPIO_PIN))
#if defined BOARD_SDHC_CD_LOGIC_RISING
    {
        g_sdInsertedFlag = 1U;
    }
    else
    {
        g_sdInsertedFlag = 0U;
    }
#else
    {
        g_sdInsertedFlag = 0U;
    }
    else
    {
        g_sdInsertedFlag = 1U;
    }
#endif
}

/* Card detect pin port interrupt handler. */
void BOARD_SDHC_CD_PORT_IRQ_HANDLER(void)
{
    if (PORT_GetPinsInterruptFlags(BOARD_SDHC_CD_PORT_BASE) == (1U << BOARD_SDHC_CD_GPIO_PIN))
    {
        DetectCardByGpio();
    }
    /* Clear interrupt flag.*/
    PORT_ClearPinsInterruptFlags(BOARD_SDHC_CD_PORT_BASE, ~0U);
    EVENT_Notify(kEVENT_CardDetect);
}

/* Delay some time united in milliseconds. */
void Delay(uint32_t milliseconds)
{
    uint32_t i;
    uint32_t j;

    for (i = 0; i < milliseconds; i++)
    {
        for (j = 0; j < 20000U; j++)
        {
            __asm("NOP");
        }
    }
}

/* User defined transfer function. */
status_t SDHC_TransferFunction(SDHC_Type *base, sdhc_transfer_t *content)
{
    status_t error = kStatus_Success;

    if (kStatus_Success != SDHC_TransferBlocking(base, g_sdhcAdmaTable, SDHC_ADMA_TABLE_WORDS, content))
    {
        error = kStatus_Fail;
    }

    return error;
}

/*!
 * @brief Main function
 */
int main(void)
{
    sd_card_t *card = &g_sd;
    sdhc_config_t *sdhcConfig = &(g_sd.host.config);
    bool isReadOnly;
    bool failedFlag = false;
    char ch = '0';

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    MPU_Enable(MPU, false);
    EVENT_InitTimer();

    if (!EVENT_Create(kEVENT_CardDetect))
    {
        return -1;
    }

    /* Card detection pin will generate interrupt on either eage */
    PORT_SetPinInterruptConfig(BOARD_SDHC_CD_PORT_BASE, BOARD_SDHC_CD_GPIO_PIN, kPORT_InterruptEitherEdge);
    /* Open card detection pin NVIC. */
    NVIC_EnableIRQ(BOARD_SDHC_CD_PORT_IRQ);

    DetectCardByGpio();
    PRINTF("\r\nPlease insert a card into board.\r\n");
    if (!g_sdInsertedFlag)
    {
        /* Wait card inserted. */
        do
        {
            if (!EVENT_Wait(kEVENT_CardDetect, EVENT_TIMEOUT_CARD_DETECT))
            {
                return -1;
            }
        } while (!g_sdInsertedFlag);
    }
    PRINTF("Detected SD card inserted.\r\n");
    /* Delat some time to make card stable. */
    Delay(1000U);

    /* Initialize SDHC. */
    sdhcConfig->cardDetectDat3 = false;
    sdhcConfig->endianMode = kSDHC_EndianModeLittle;
    sdhcConfig->dmaMode = kSDHC_DmaModeAdma2;
    sdhcConfig->readWatermarkLevel = 0x80U;
    sdhcConfig->writeWatermarkLevel = 0x80U;
    SDHC_Init(BOARD_SDHC_BASEADDR, sdhcConfig);

    /* Save host information. */
    card->host.base = BOARD_SDHC_BASEADDR;
    card->host.sourceClock_Hz = CLOCK_GetFreq(BOARD_SDHC_CLKSRC);
    card->host.transfer = SDHC_TransferFunction;

    /* Init card. */
    if (SD_Init(card))
    {
        PRINTF("\r\nSD card init failed.\r\n");
        return -1;
    }

    PRINTF("\r\nRead/Write/Erase the card continuously until encounter error......\r\n");
    /* Check if card is readonly. */
    isReadOnly = SD_CheckReadOnly(card);
    if (isReadOnly)
    {
        while (true)
        {
            if (failedFlag || (ch == 'q'))
            {
                break;
            }

            PRINTF("\r\nRead one data block......\r\n");
            if (kStatus_Success != SD_ReadBlocks(card, g_dataRead, DATA_BLOCK_START, 1U))
            {
                PRINTF("Read one data block failed.\r\n");
                failedFlag = true;
                continue;
            }

            PRINTF("Read multiple data blocks......\r\n");
            if (kStatus_Success != SD_ReadBlocks(card, g_dataRead, DATA_BLOCK_START, DATA_BLOCK_COUNT))
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
        memset(g_dataWrite, 0x67U, sizeof(g_dataWrite));

        while (true)
        {
            if (failedFlag || (ch == 'q'))
            {
                break;
            }

            PRINTF("\r\nWrite/read one data block......\r\n");
            if (kStatus_Success != SD_WriteBlocks(card, g_dataWrite, DATA_BLOCK_START, 1U))
            {
                PRINTF("Write one data block failed.\r\n");
                failedFlag = true;
                continue;
            }

            memset(g_dataRead, 0U, sizeof(g_dataRead));
            if (kStatus_Success != SD_ReadBlocks(card, g_dataRead, DATA_BLOCK_START, 1U))
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
            if (kStatus_Success != SD_WriteBlocks(card, g_dataWrite, DATA_BLOCK_START, DATA_BLOCK_COUNT))
            {
                PRINTF("Write multiple data blocks failed.\r\n");
                failedFlag = true;
                continue;
            }

            memset(g_dataRead, 0U, sizeof(g_dataRead));
            if (kStatus_Success != SD_ReadBlocks(card, g_dataRead, DATA_BLOCK_START, DATA_BLOCK_COUNT))
            {
                PRINTF("Read multiple data blocks failed.\r\n");
                failedFlag = true;
                continue;
            }

            PRINTF("Compare the read/write content......\r\n");
            if (memcmp(g_dataRead, g_dataWrite, (FSL_SDMMC_DEFAULT_BLOCK_SIZE * DATA_BLOCK_COUNT)))
            {
                PRINTF("The read/write content isn't consistent.\r\n");
                failedFlag = true;
                continue;
            }
            PRINTF("The read/write content is consistent.\r\n");

            PRINTF("Erase multiple data blocks......\r\n");
            if (kStatus_Success != SD_EraseBlocks(card, DATA_BLOCK_START, DATA_BLOCK_COUNT))
            {
                PRINTF("Erase multiple data blocks failed.\r\n");
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

    EVENT_Delete(kEVENT_CardDetect);
    SD_Deinit(card);

    while (true)
    {
    }
}
