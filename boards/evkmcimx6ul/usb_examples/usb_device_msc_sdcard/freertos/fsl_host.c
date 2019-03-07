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

#include "fsl_host.h"
#include "board.h"
#include "event.h"
#include "fsl_gpio.h"
#ifdef BOARD_USDHC_CD_PORT_BASE
#include "fsl_port.h"
#endif

/*******************************************************************************
* Definitions
******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief host controller error recovery.
 * @param host base address.
 */
static void Host_ErrorRecovery(HOST_TYPE *hostBase);
/*******************************************************************************
 * Variables
 ******************************************************************************/
/* DMA descriptor should allocate at non-cached memory */
AT_NONCACHEABLE_SECTION_ALIGN(uint32_t g_usdhcAdma2Table[USDHC_ADMA_TABLE_WORDS], USDHC_ADMA2_ADDR_ALIGN);

static usdhc_handle_t g_usdhcHandle;
static volatile bool g_usdhcTransferSuccessFlag = true;
static volatile bool g_sdInsertedFlag;
static volatile status_t g_reTuningFlag = false;
/*******************************************************************************
 * Code
 ******************************************************************************/

/* Delay some time united in milliseconds. */
static void Delay(uint32_t milliseconds)
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

static void DetectCardByGpio(void)
{
    if (HOST_CARD_DETECT_STATUS() != HOST_CARD_INSERT_CD_LEVEL)
    {
        g_sdInsertedFlag = false;
    }
    else
    {
        g_sdInsertedFlag = true;
    }
}

/* Card detect. */
status_t CardInsertDetect(HOST_TYPE *hostBase)
{
    if (!EVENT_Create(kEVENT_CardDetect))
    {
        return kStatus_Fail;
    }

    HOST_CARD_DETECT_INIT();
    /* only SD card need card detect*/
    HOST_ENABLE_SD_POWER(false);
    /* Open card detection pin NVIC. */
    EnableIRQ(HOST_CARD_DETECT_IRQ);
/* set IRQ priority */
#if defined(__CORTEX_M)
    NVIC_SetPriority(HOST_CARD_DETECT_IRQ, 6U);
#else
    GIC_SetPriority(HOST_CARD_DETECT_IRQ, 26U);
#endif
    /* detect card status */
    DetectCardByGpio();

    if (!g_sdInsertedFlag)
    {
        /* Wait card inserted. */
        do
        {
            if (!EVENT_Wait(kEVENT_CardDetect, EVENT_TIMEOUT_CARD_DETECT))
            {
                return kStatus_Fail;
            }
        } while (!g_sdInsertedFlag);
    }

    EVENT_Delete(kEVENT_CardDetect);
    /* Delat some time to make card stable. */
    Delay(500U);
    /* card power on */
    HOST_ENABLE_SD_POWER(true);
    /* Delat some time to make card stable. */
    Delay(500U);

    return kStatus_Success;
}

/* Card detect pin port interrupt handler. */
void HOST_CARD_DETECT_INTERRUPT_HANDLER(void)
{
    if (HOST_CARD_DETECT_INTERRUPT_STATUS() & (1U << BOARD_USDHC_CD_GPIO_PIN))
    {
        DetectCardByGpio();
    }
    /* Clear interrupt flag.*/
    HOST_CARD_DETECT_INTERRUPT_CLEAR(~0U);
    EVENT_Notify(kEVENT_CardDetect);
}

/* USDHC transfer complete callback function. */
static void USDHC_TransferCompleteCallback(USDHC_Type *base, usdhc_handle_t *handle, status_t status, void *userData)
{
    /* wait the target status and then notify the transfer complete */
    if (status == kStatus_Success)
    {
        g_usdhcTransferSuccessFlag = true;
    }
    else
    {
        g_usdhcTransferSuccessFlag = false;
    }

    EVENT_Notify(kEVENT_TransferComplete);
}

/* USDHC transfer complete callback function. */
static void USDHC_ReTuningCallback(void)
{
    g_reTuningFlag = true;
    EVENT_Notify(kEVENT_TransferComplete);
}

/* User defined transfer function. */
static status_t USDHC_TransferFunction(USDHC_Type *base, usdhc_transfer_t *content)
{
    status_t error = kStatus_Success;

    usdhc_adma_config_t dmaConfig;

    if (content->data != NULL)
    {
        memset(&dmaConfig, 0, sizeof(usdhc_adma_config_t));
        /* config adma */
        dmaConfig.dmaMode = USDHC_DMA_MODE;
        dmaConfig.burstLen = kUSDHC_EnBurstLenForINCR;
        dmaConfig.admaTable = g_usdhcAdma2Table;
        dmaConfig.admaTableWords = USDHC_ADMA_TABLE_WORDS;
    }

    do
    {
        error = USDHC_TransferNonBlocking(base, &g_usdhcHandle, &dmaConfig, content);
    } while (error == kStatus_USDHC_BusyTransferring);

    if ((error != kStatus_Success) || (false == EVENT_Wait(kEVENT_TransferComplete, EVENT_TIMEOUT_TRANSFER_COMPLETE)) ||
        (g_reTuningFlag) || (!g_usdhcTransferSuccessFlag))
    {
        if (g_reTuningFlag || (error == kStatus_USDHC_ReTuningRequest))
        {
            if (g_reTuningFlag)
            {
                g_reTuningFlag = false;
                error = kStatus_USDHC_TuningError;
            }
        }
        else
        {
            error = kStatus_Fail;
            /* host error recovery */
            Host_ErrorRecovery(base);
        }
    }

    return error;
}

static void Host_ErrorRecovery(HOST_TYPE *hostBase)
{
    uint32_t status = 0U;
    /* get host present status */
    status = USDHC_GetPresentStatusFlags(hostBase);
    /* check command inhibit status flag */
    if ((status & kUSDHC_CommandInhibitFlag) != 0U)
    {
        /* reset command line */
        USDHC_Reset(hostBase, kUSDHC_ResetCommand, 100U);
    }
    /* check data inhibit status flag */
    if ((status & kUSDHC_DataInhibitFlag) != 0U)
    {
        /* reset data line */
        USDHC_Reset(hostBase, kUSDHC_ResetData, 100U);
    }
}

status_t HOST_Init(void *host)
{
    usdhc_host_t *usdhcHost = (usdhc_host_t *)host;
    usdhc_transfer_callback_t callback;
    /* init card power control */
    HOST_INIT_SD_POWER();
    HOST_INIT_MMC_POWER();

    /* register callback function */
    callback.TransferComplete = USDHC_TransferCompleteCallback;
    callback.ReTuning = USDHC_ReTuningCallback;
    /* Initializes USDHC. */
    usdhcHost->config.dataTimeout = USDHC_DATA_TIMEOUT;
    usdhcHost->config.endianMode = USDHC_ENDIAN_MODE;
    usdhcHost->config.readWatermarkLevel = USDHC_READ_WATERMARK_LEVEL;
    usdhcHost->config.writeWatermarkLevel = USDHC_WRITE_WATERMARK_LEVEL;
    usdhcHost->config.readBurstLen = USDHC_READ_BURST_LEN;
    usdhcHost->config.writeBurstLen = USDHC_WRITE_BURST_LEN;

    USDHC_Init(usdhcHost->base, &(usdhcHost->config));

    /* disable the card insert/remove interrupt, due to use GPIO interrupt detect card */
    USDHC_DisableInterruptSignal(usdhcHost->base, kUSDHC_CardRemovalFlag | kUSDHC_CardInsertionFlag);
    /* create interrupt handler */
    USDHC_TransferCreateHandle(usdhcHost->base, &g_usdhcHandle, &callback, NULL);

    if (false == EVENT_Create(kEVENT_TransferComplete))
    {
        return kStatus_Fail;
    }

    /* Define transfer function. */
    usdhcHost->transfer = USDHC_TransferFunction;

    return kStatus_Success;
}

void HOST_Reset(HOST_TYPE *hostBase)
{
    /* voltage switch to normal but not 1.8V */
    HOST_SWITCH_VOLTAGE180V(hostBase, false);
    /* Disable DDR mode */
    HOST_ENABLE_DDR_MODE(hostBase, false);
    /* disable tuning */
    HOST_EXECUTE_STANDARD_TUNING_ENABLE(hostBase, false);
    /* Disable HS400 mode */
    HOST_ENABLE_HS400_MODE(hostBase, false);
    /* Disable DLL */
    HOST_ENABLE_STROBE_DLL(hostBase, false);
}

void HOST_Deinit(void *host)
{
    usdhc_host_t *usdhcHost = (usdhc_host_t *)host;
    USDHC_Deinit(usdhcHost->base);
    EVENT_Delete(kEVENT_TransferComplete);
}
