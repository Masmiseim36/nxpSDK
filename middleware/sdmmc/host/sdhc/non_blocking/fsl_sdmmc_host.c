/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_sdmmc_host.h"
#include "fsl_sdmmc_common.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define SDMMCHOST_TRANSFER_COMPLETE_TIMEOUT (~0U)
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief SDMMCHOST detect card insert status by host controller.
 * @param base host base address.
 * @param userData user can register a application card insert callback through userData.
 */
static void SDMMCHOST_DetectCardInsertByHost(SDHC_Type *base, void *userData);

/*!
 * @brief SDMMCHOST detect card remove status by host controller.
 * @param base host base address.
 * @param userData user can register a application card insert callback through userData.
 */
static void SDMMCHOST_DetectCardRemoveByHost(SDHC_Type *base, void *userData);

/*!
 * @brief SDMMCHOST transfer complete callback.
 * @param base host base address.
 * @param handle host handle.
 * @param status interrupt status.
 * @param userData user data.
 */
static void SDMMCHOST_TransferCompleteCallback(SDHC_Type *base, sdhc_handle_t *handle, status_t status, void *userData);

/*!
 * @brief SDMMCHOST error recovery.
 * @param base host base address.
 */
static void SDMMCHOST_ErrorRecovery(SDHC_Type *base);
/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
static void SDMMCHOST_DetectCardInsertByHost(SDHC_Type *base, void *userData)
{
    sd_detect_card_t *cd = NULL;

    SDMMC_OSAEventSet(((sdmmchost_t *)userData)->hostEvent, SDMMC_OSA_EVENT_CARD_INSERTED);
    /* disable card insert insterrupt */
    SDHC_DisableInterruptSignal(base, kSDHC_CardInsertionFlag);

    if (userData != NULL)
    {
        cd = (sd_detect_card_t *)(((sdmmchost_t *)userData)->cd);
        if (cd != NULL)
        {
            if (cd->callback != NULL)
            {
                cd->callback(true, cd->userData);
            }
        }
    }
}

static void SDMMCHOST_DetectCardRemoveByHost(SDHC_Type *base, void *userData)
{
    sd_detect_card_t *cd = NULL;

    SDMMC_OSAEventSet(((sdmmchost_t *)userData)->hostEvent, SDMMC_OSA_EVENT_CARD_REMOVED);

    if (userData != NULL)
    {
        cd = (sd_detect_card_t *)(((sdmmchost_t *)userData)->cd);
        if (cd != NULL)
        {
            if (cd->callback != NULL)
            {
                cd->callback(false, cd->userData);
            }
        }
    }
}

static void SDMMCHOST_CardInterrupt(SDHC_Type *base, void *userData)
{
    sdio_card_int_t *cardInt = NULL;

    /* application callback */
    if (userData != NULL)
    {
        cardInt = ((sdmmchost_t *)userData)->cardInt;
        if ((cardInt != NULL) && (cardInt->cardInterrupt != NULL))
        {
            cardInt->cardInterrupt(cardInt->userData);
        }
    }
}

status_t SDMMCHOST_CardIntInit(sdmmchost_t *host, void *sdioInt)
{
    host->cardInt                       = sdioInt;
    host->handle.callback.SdioInterrupt = SDMMCHOST_CardInterrupt;
    SDMMCHOST_EnableCardInt(host, true);

    return kStatus_Success;
}

status_t SDMMCHOST_CardDetectInit(sdmmchost_t *host, void *cd)
{
    SDHC_Type *base        = host->hostController.base;
    sd_detect_card_t *sdCD = (sd_detect_card_t *)cd;
    if (cd == NULL)
    {
        return kStatus_Fail;
    }

    host->cd = cd;

    /* enable card detect through DATA3 */
    SDHC_CardDetectByData3(base, true);
    /* enable card detect interrupt */
    SDHC_EnableInterruptStatus(base, kSDHC_CardInsertionFlag);
    SDHC_EnableInterruptStatus(base, kSDHC_CardRemovalFlag);
    SDHC_EnableInterruptSignal(base, kSDHC_CardInsertionFlag);
    SDHC_EnableInterruptSignal(base, kSDHC_CardRemovalFlag);

    if ((SDHC_GetInterruptStatusFlags(base) & (uint32_t)kSDHC_CardInsertionFlag) != 0U)
    {
        /* notify application about the card insertion status */
        if (sdCD->callback)
        {
            sdCD->callback(true, sdCD->userData);
        }
    }

    return kStatus_Success;
}

uint32_t SDMMCHOST_CardDetectStatus(sdmmchost_t *host)
{
    uint32_t event = 0U;

    SDMMC_OSAEventGet(host->hostEvent, SDMMC_OSA_EVENT_CARD_INSERTED | SDMMC_OSA_EVENT_CARD_REMOVED, &event);

    if ((event & SDMMC_OSA_EVENT_CARD_INSERTED) == SDMMC_OSA_EVENT_CARD_INSERTED)
    {
        return kSD_Inserted;
    }

    return kSD_Removed;
}

status_t SDMMCHOST_PollingCardDetectStatus(sdmmchost_t *host, uint32_t waitCardStatus, uint32_t timeout)
{
    assert(host != NULL);
    assert(host->cd != NULL);

    sd_detect_card_t *cd = host->cd;
    uint32_t event       = 0U;

    if (SDMMCHOST_CardDetectStatus(host) == waitCardStatus)
    {
        return kStatus_Success;
    }

    /* Wait card inserted. */
    do
    {
        if (SDMMC_OSAEventWait(host->hostEvent, SDMMC_OSA_EVENT_CARD_INSERTED | SDMMC_OSA_EVENT_CARD_REMOVED, timeout,
                               &event) != kStatus_Success)
        {
            return kStatus_Fail;
        }
        else
        {
            if ((waitCardStatus == kSD_Inserted) &&
                ((event & SDMMC_OSA_EVENT_CARD_INSERTED) == SDMMC_OSA_EVENT_CARD_INSERTED))
            {
                SDMMC_OSADelay(cd->cdDebounce_ms);
                SDMMC_OSAEventGet(host->hostEvent, SDMMC_OSA_EVENT_CARD_INSERTED, &event);
                if ((event & SDMMC_OSA_EVENT_CARD_INSERTED) == SDMMC_OSA_EVENT_CARD_INSERTED)
                {
                    break;
                }
            }

            if (((event & SDMMC_OSA_EVENT_CARD_REMOVED) == SDMMC_OSA_EVENT_CARD_REMOVED) &&
                (waitCardStatus == kSD_Removed))
            {
                break;
            }
        }
    } while (1U);

    return kStatus_Success;
}

status_t SDMMCHOST_WaitCardDetectStatus(SDMMCHOST_TYPE *hostBase,
                                        const sdmmchost_detect_card_t *cd,
                                        bool waitCardStatus)
{
    assert(cd != NULL);

    while (((SDHC_GetInterruptStatusFlags(hostBase) & (uint32_t)kSDHC_CardInsertionFlag) != 0U) != waitCardStatus)
    {
    }

    return kStatus_Success;
}

static void SDMMCHOST_TransferCompleteCallback(SDHC_Type *base, sdhc_handle_t *handle, status_t status, void *userData)
{
    uint32_t eventStatus = 0U;

    if (status == kStatus_SDHC_TransferDataFailed)
    {
        eventStatus = SDMMC_OSA_EVENT_TRANSFER_DATA_FAIL;
    }
    else if (status == kStatus_SDHC_TransferDataComplete)
    {
        eventStatus = SDMMC_OSA_EVENT_TRANSFER_DATA_SUCCESS;
    }
    else if (status == kStatus_SDHC_SendCommandFailed)
    {
        eventStatus = SDMMC_OSA_EVENT_TRANSFER_CMD_FAIL;
    }
    else
    {
        eventStatus = SDMMC_OSA_EVENT_TRANSFER_CMD_SUCCESS;
    }

    SDMMC_OSAEventSet(((sdmmchost_t *)userData)->hostEvent, eventStatus);
}

status_t SDMMCHOST_TransferFunction(sdmmchost_t *host, sdmmchost_transfer_t *content)
{
    status_t error = kStatus_Success;
    uint32_t event = 0U;

    /* clear redundant transfer event flag */
    SDMMC_OSAEventClear(host->hostEvent, SDMMC_OSA_EVENT_TRANSFER_CMD_SUCCESS | SDMMC_OSA_EVENT_TRANSFER_CMD_FAIL |
                                             SDMMC_OSA_EVENT_TRANSFER_DATA_SUCCESS |
                                             SDMMC_OSA_EVENT_TRANSFER_DATA_FAIL);

    do
    {
        error = SDHC_TransferNonBlocking(host->hostController.base, &host->handle, host->dmaDesBuffer,
                                         host->dmaDesBufferWordsNum, content);
    } while (error == kStatus_SDHC_BusyTransferring);

    /* wait command event */
    if ((kStatus_Fail ==
         SDMMC_OSAEventWait(host->hostEvent,
                            SDMMC_OSA_EVENT_TRANSFER_CMD_SUCCESS | SDMMC_OSA_EVENT_TRANSFER_CMD_FAIL |
                                SDMMC_OSA_EVENT_TRANSFER_DATA_SUCCESS | SDMMC_OSA_EVENT_TRANSFER_DATA_FAIL,
                            SDMMCHOST_TRANSFER_COMPLETE_TIMEOUT, &event)) ||
        ((event & SDMMC_OSA_EVENT_TRANSFER_CMD_FAIL) != 0U))
    {
        error = kStatus_Fail;
    }
    else
    {
        if (content->data != NULL)
        {
            if ((event & SDMMC_OSA_EVENT_TRANSFER_DATA_SUCCESS) == 0U)
            {
                if (((event & SDMMC_OSA_EVENT_TRANSFER_DATA_FAIL) != 0U) ||
                    (kStatus_Fail ==
                         SDMMC_OSAEventWait(host->hostEvent,
                                            SDMMC_OSA_EVENT_TRANSFER_DATA_SUCCESS | SDMMC_OSA_EVENT_TRANSFER_DATA_FAIL,
                                            SDMMCHOST_TRANSFER_COMPLETE_TIMEOUT, &event) ||
                     ((event & SDMMC_OSA_EVENT_TRANSFER_DATA_FAIL) != 0U)))
                {
                    error = kStatus_Fail;
                }
            }
        }
    }

    if (error != kStatus_Success)
    {
        error = kStatus_Fail;
        /* host error recovery */
        SDMMCHOST_ErrorRecovery(host->hostController.base);
    }

    return error;
}

static void SDMMCHOST_ErrorRecovery(SDHC_Type *base)
{
    uint32_t status = 0U;
    /* get host present status */
    status = SDHC_GetPresentStatusFlags(base);
    /* check command inhibit status flag */
    if ((status & kSDHC_CommandInhibitFlag) != 0U)
    {
        /* reset command line */
        SDHC_Reset(base, kSDHC_ResetCommand, 100U);
    }
    /* check data inhibit status flag */
    if ((status & kSDHC_DataInhibitFlag) != 0U)
    {
        /* reset data line */
        SDHC_Reset(base, kSDHC_ResetData, 100U);
    }
}

void SDMMCHOST_SetCardPower(sdmmchost_t *host, bool enable)
{
    /* host not support */
}

void SDMMCHOST_PowerOffCard(SDMMCHOST_TYPE *base, const sdmmchost_pwr_card_t *pwr)
{
    if (pwr != NULL)
    {
        pwr->powerOff();
        SDMMC_OSADelay(pwr->powerOffDelay_ms);
    }
}

void SDMMCHOST_PowerOnCard(SDMMCHOST_TYPE *base, const sdmmchost_pwr_card_t *pwr)
{
    /* use user define the power on function  */
    if (pwr != NULL)
    {
        pwr->powerOn();
        SDMMC_OSADelay(pwr->powerOnDelay_ms);
    }
    else
    {
        /* Delay several milliseconds to make card stable. */
        SDMMC_OSADelay(1000U);
    }
}

status_t SDMMCHOST_Init(sdmmchost_t *host)
{
    assert(host != NULL);
    assert(host->hostEvent != NULL);

    sdhc_transfer_callback_t sdhcCallback = {0};
    sdhc_host_t *sdhcHost                 = &(host->hostController);

    /* sdmmc osa init */
    SDMMC_OSAInit();

    /* Initializes SDHC. */
    sdhcHost->config.endianMode          = kSDHC_EndianModeLittle;
    sdhcHost->config.dmaMode             = kSDHC_DmaModeAdma2;
    sdhcHost->config.readWatermarkLevel  = 0x80U;
    sdhcHost->config.writeWatermarkLevel = 0x80U;
    SDHC_Init(sdhcHost->base, &(sdhcHost->config));

    /* Create handle for SDHC driver */
    sdhcCallback.TransferComplete = SDMMCHOST_TransferCompleteCallback;
    sdhcCallback.CardInserted     = SDMMCHOST_DetectCardInsertByHost;
    sdhcCallback.CardRemoved      = SDMMCHOST_DetectCardRemoveByHost;
    SDHC_TransferCreateHandle(sdhcHost->base, &host->handle, &sdhcCallback, host);

    /* Create transfer event. */
    if (kStatus_Success != SDMMC_OSAEventCreate(host->hostEvent))
    {
        return kStatus_Fail;
    }

    return kStatus_Success;
}

void SDMMCHOST_Reset(sdmmchost_t *host)
{
    /* reserved for future */
}

void SDMMCHOST_SetCardBusWidth(sdmmchost_t *host, uint32_t dataBusWidth)
{
    SDHC_SetDataBusWidth(host->hostController.base,
                         dataBusWidth == kSDMMC_BusWdith1Bit ?
                             kSDHC_DataBusWidth1Bit :
                             dataBusWidth == kSDMMC_BusWdith4Bit ? kSDHC_DataBusWidth4Bit : kSDHC_DataBusWidth8Bit);
}

void SDMMCHOST_Deinit(sdmmchost_t *host)
{
    sdhc_host_t *sdhcHost = &host->hostController;
    SDHC_Deinit(sdhcHost->base);
    SDMMC_OSAEventDestroy(host->hostEvent);
}

status_t SDMMCHOST_StartBoot(sdmmchost_t *host,
                             sdmmchost_boot_config_t *hostConfig,
                             sdmmchost_cmd_t *cmd,
                             uint8_t *buffer)
{
    /* not support */
    return kStatus_Success;
}

status_t SDMMCHOST_ReadBootData(sdmmchost_t *host, sdmmchost_boot_config_t *hostConfig, uint8_t *buffer)
{
    /* not support */
    return kStatus_Success;
}
