/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "fsl_sdio.h"
#include "fsl_debug_console.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_common.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/


/* define data buffer size */
#define DATA_BUFFER_SIZE (256U)

/*! @brief Task stack size. */
#define ACCESSCARD_TASK_STACK_SIZE (1024)
/*! @brief Task stack priority. */
#define ACCESSCARD_TASK_PRIORITY (configMAX_PRIORITIES - 2U)

/*! @brief Task stack size. */
#define CARDDETECT_TASK_STACK_SIZE (512)
/*! @brief Task stack priority. */
#define CARDDETECT_TASK_PRIORITY (configMAX_PRIORITIES - 1U)

/*! @brief Task stack size. */
#define CARDINTERRUPT_TASK_STACK_SIZE (256)
/*! @brief Task stack priority. */
#define CARDINTERRUPT_TASK_PRIORITY (configMAX_PRIORITIES - 3U)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief SDIO card access task.
 *
 * @param pvParameters Task parameter.
 */
static void AccessCardTask(void *pvParameters);

/*!
 * @brief SDIO card detect task.
 *
 * @param pvParameters Task parameter.
 */
static void CardDetectTask(void *pvParameters);

/*!
 * @brief SDIO card interrupt task.
 *
 * @param pvParameters Task parameter.
 */
static void CardInterruptTask(void *pvParameters);

/*!
 * @brief call back function for SDIO card detect.
 *
 * @param isInserted  true,  indicate the card is insert.
 *                    false, indicate the card is remove.
 * @param userData
 */
static void SDIO_DetectCallBack(bool isInserted, void *userData);

/*!
 * @brief card read/write function.
 *
 * @param card card descriptor
 */
static status_t SDIO_Access(sdio_card_t *card);

/*!
 * @brief card interrupt callback function.
 *
 * @param userData user data.
 */
static void SDIO_CardInterruptCallBack(void *userData);

/*!
 * @brief SDIO IO1 interrupt IRQ handler.
 *
 * @param card card descriptor.
 * @param func function number.
 */
static void DEMO_SDIO_IO1_IRQ_Handler(sdio_card_t *card, uint32_t func);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*! @brief Card descriptor. */
sdio_card_t g_sdio;

/* define the tuple list */
static const uint32_t g_funcTupleList[2U] = {
    SDIO_TPL_CODE_FUNCID,
    SDIO_TPL_CODE_FUNCE,
};

/* @brief decription about the read/write buffer
 * The size of the read/write buffer should be a multiple of 512, since SDHC/SDXC card uses 512-byte fixed
 * block length and this driver example is enabled with a SDHC/SDXC card.If you are using a SDSC card, you
 * can define the block length by yourself if the card supports partial access.
 * The address of the read/write buffer should align to the specific DMA data buffer address align value if
 * DMA transfer is used, otherwise the buffer address is not important.
 * At the same time buffer address/size should be aligned to the cache line size if cache is supported.
 */
/*! @brief Data written to the card */
SDK_ALIGN(uint8_t g_dataRead[SDK_SIZEALIGN(DATA_BUFFER_SIZE, SDMMC_DATA_BUFFER_ALIGN_CACHE)],
          MAX(SDMMC_DATA_BUFFER_ALIGN_CACHE, SDMMCHOST_DMA_BUFFER_ADDR_ALIGN));
/*! @brief Data read from the card */
SDK_ALIGN(uint8_t g_dataBlockRead[SDK_SIZEALIGN(DATA_BUFFER_SIZE, SDMMC_DATA_BUFFER_ALIGN_CACHE)],
          MAX(SDMMC_DATA_BUFFER_ALIGN_CACHE, SDMMCHOST_DMA_BUFFER_ADDR_ALIGN));

/*! @brief SDMMC host detect card configuration */
static const sdmmchost_detect_card_t s_sdioCardDetect = {
#ifndef BOARD_SD_DETECT_TYPE
    .cdType = kSDMMCHOST_DetectCardByGpioCD,
#else
    .cdType = BOARD_SD_DETECT_TYPE,
#endif
    .cdTimeOut_ms = (~0U),
    .cardInserted = SDIO_DetectCallBack,
    .cardRemoved  = SDIO_DetectCallBack,
};

static const sdmmchost_card_int_t s_sdioCardInt = {
    .userData      = NULL,
    .cardInterrupt = SDIO_CardInterruptCallBack,
};
/*! @brief SDIO card detect flag  */
static volatile bool s_cardInserted     = false;
static volatile bool s_cardInsertStatus = false;
/*! @brief Card semaphore  */
static SemaphoreHandle_t s_CardAccessSemaphore    = NULL;
static SemaphoreHandle_t s_CardDetectSemaphore    = NULL;
static SemaphoreHandle_t s_CardInterruptSemaphore = NULL;

/*******************************************************************************
 * Code
 ******************************************************************************/
static void BOARD_USDHCClockConfiguration(void)
{
    /*configure system pll PFD0 fractional divider to 24, output clock is 528MHZ * 18 / 24 = 396 MHZ*/
    CLOCK_InitSysPfd(kCLOCK_Pfd0, 24U);
    /* Configure USDHC clock source and divider */
    CLOCK_SetDiv(kCLOCK_Usdhc1Div, 0U);
    CLOCK_SetMux(kCLOCK_Usdhc1Mux, 1U);
}

static void SDIO_DetectCallBack(bool isInserted, void *userData)
{
    s_cardInsertStatus = isInserted;
    xSemaphoreGiveFromISR(s_CardDetectSemaphore, NULL);
}

static void SDIO_CardInterruptCallBack(void *userData)
{
    SDMMCHOST_DISABLE_SDIO_INT(SD_HOST_BASEADDR);
    xSemaphoreGiveFromISR(s_CardInterruptSemaphore, NULL);
}

/*! @brief Main function */
int main(void)
{
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_USDHCClockConfiguration();
    BOARD_InitDebugConsole();
    NVIC_SetPriority(USDHC1_IRQn, 5U);
    NVIC_SetPriority(BOARD_UART_IRQ, 6U);

    PRINTF("SDIO freertos example.\r\n");

    if (pdPASS !=
        xTaskCreate(AccessCardTask, "AccessCardTask", ACCESSCARD_TASK_STACK_SIZE, NULL, ACCESSCARD_TASK_PRIORITY, NULL))
    {
        return -1;
    }

    if (pdPASS !=
        xTaskCreate(CardDetectTask, "CardDetectTask", CARDDETECT_TASK_STACK_SIZE, NULL, CARDDETECT_TASK_PRIORITY, NULL))
    {
        return -1;
    }

    if (pdPASS != xTaskCreate(CardInterruptTask, "CardInterruptTask", CARDINTERRUPT_TASK_STACK_SIZE, NULL,
                              CARDINTERRUPT_TASK_PRIORITY, NULL))
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

static void CardInterruptTask(void *pvParameters)
{
    while (true)
    {
        /* take card detect semaphore */
        if (xSemaphoreTake(s_CardInterruptSemaphore, portMAX_DELAY) == pdFALSE)
        {
            PRINTF("\r\nFailed to take semphone.\r\n");
            break;
        }

        if (xSemaphoreTake(s_CardAccessSemaphore, portMAX_DELAY) == pdFALSE)
        {
            PRINTF("\r\nFailed to take semphone.\r\n");
            break;
        }

        PRINTF("\r\nSDIO interrupt is received\r\n");
        SDIO_HandlePendingIOInterrupt(&g_sdio);
        SDMMCHOST_ENABLE_SDIO_INT(SD_HOST_BASEADDR);
        /* release card access semphore */
        xSemaphoreGive(s_CardAccessSemaphore);
    }

    vTaskSuspend(NULL);
}

static void CardDetectTask(void *pvParameters)
{
    s_CardAccessSemaphore    = xSemaphoreCreateBinary();
    s_CardDetectSemaphore    = xSemaphoreCreateBinary();
    s_CardInterruptSemaphore = xSemaphoreCreateBinary();

    g_sdio.host.base           = SD_HOST_BASEADDR;
    g_sdio.host.sourceClock_Hz = SD_HOST_CLK_FREQ;
    g_sdio.usrParam.cd         = &s_sdioCardDetect;
    g_sdio.usrParam.cardInt    = &s_sdioCardInt;

    /* SD host init function */
    if (SDIO_HostInit(&g_sdio) == kStatus_Success)
    {
        while (true)
        {
            /* take card detect semaphore*/
            if (xSemaphoreTake(s_CardDetectSemaphore, portMAX_DELAY) == pdFALSE)
            {
                PRINTF("\r\nFailed to take semphone.\r\n");
                break;
            }

            if (s_cardInserted != s_cardInsertStatus)
            {
                s_cardInserted = s_cardInsertStatus;
                /* power off card */
                SDIO_PowerOffCard(g_sdio.host.base, NULL);
                if (s_cardInserted)
                {
                    PRINTF("\r\nCard inserted.\r\n");
                    /* power on the card */
                    SDIO_PowerOnCard(g_sdio.host.base, NULL);
                    /* Init card. */
                    if (SDIO_CardInit(&g_sdio))
                    {
                        PRINTF("\r\nSDIO card init failed.\r\n");
                        break;
                    }

                    /* after card is finish initalization enable SDIO IO interrup */
                    SDIO_EnableIOInterrupt(&g_sdio, kSDIO_FunctionNum1, true);
                    SDIO_SetIOIRQHandler(&g_sdio, kSDIO_FunctionNum1, DEMO_SDIO_IO1_IRQ_Handler);
                    PRINTF("\r\nCard init finished, ready to access.\r\n");
                    xSemaphoreGive(s_CardAccessSemaphore);
                }
            }

            if (!s_cardInserted)
            {
                PRINTF("\r\nPlease insert a card into board.\r\n");
            }
        }
    }
    else
    {
        PRINTF("\r\nSDIO host init fail\r\n");
    }

    vTaskSuspend(NULL);
}

static void AccessCardTask(void *pvParameters)
{
    sdio_card_t *card = &g_sdio;
    char ch           = '0';

    while (ch != 'q')
    {
        if (xSemaphoreTake(s_CardAccessSemaphore, portMAX_DELAY) == pdFALSE)
        {
            PRINTF("\r\nFailed to take semphone.\r\n");
            break;
        }

        if (SDIO_Access(card) != kStatus_Success)
        {
            PRINTF("\r\nsdio card access failed.\r\n");
        }

        xSemaphoreGive(s_CardAccessSemaphore);

        PRINTF(
            "\r\nInput 'q' to quit card access task.\
            \r\nInput other char to access again.\r\n");
        ch = GETCHAR();
        PUTCHAR(ch);
    }

    PRINTF("\r\nThe card access task will not access card again.\r\n");

    vTaskSuspend(NULL);
}

static status_t SDIO_Access(sdio_card_t *card)
{
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

    return kStatus_Success;
}

static void DEMO_SDIO_IO1_IRQ_Handler(sdio_card_t *card, uint32_t func)
{
    PRINTF("IO %d interrupt detected.\r\n", func);
}
