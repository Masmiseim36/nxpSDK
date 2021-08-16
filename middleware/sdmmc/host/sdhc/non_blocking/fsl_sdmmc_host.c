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

    (void)SDMMC_OSAEventSet(&(((sdmmchost_t *)userData)->hostEvent), SDMMC_OSA_EVENT_CARD_INSERTED);
    (void)SDMMC_OSAEventClear(&(((sdmmchost_t *)userData)->hostEvent), SDMMC_OSA_EVENT_CARD_REMOVED);

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

            if (cd->dat3PullFunc != NULL)
            {
                cd->dat3PullFunc(kSD_DAT3PullUp);
            }
        }
    }
}

static void SDMMCHOST_DetectCardRemoveByHost(SDHC_Type *base, void *userData)
{
    sd_detect_card_t *cd = NULL;

    (void)SDMMC_OSAEventSet(&(((sdmmchost_t *)userData)->hostEvent), SDMMC_OSA_EVENT_CARD_REMOVED);
    (void)SDMMC_OSAEventClear(&(((sdmmchost_t *)userData)->hostEvent), SDMMC_OSA_EVENT_CARD_INSERTED);

    SDHC_DisableInterruptSignal(base, kSDHC_CardRemovalFlag);

    if (userData != NULL)
    {
        cd = (sd_detect_card_t *)(((sdmmchost_t *)userData)->cd);
        if (cd != NULL)
        {
            if (cd->callback != NULL)
            {
                cd->callback(false, cd->userData);
            }

            if (cd->dat3PullFunc != NULL)
            {
                cd->dat3PullFunc(kSD_DAT3PullUp);
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
    if ((cd == NULL) || (sdCD->type != kSD_DetectCardByHostDATA3))
    {
        return kStatus_Fail;
    }

    host->cd = cd;

    /* enable card detect interrupt */
    SDHC_EnableInterruptStatus(base, kSDHC_CardRemovalFlag);
    SDHC_ClearInterruptStatusFlags(base, (uint32_t)kSDHC_CardInsertionFlag | (uint32_t)kSDHC_CardRemovalFlag);

    if (SDMMCHOST_CardDetectStatus(host) == (uint32_t)kSD_Inserted)
    {
        (void)SDMMC_OSAEventSet(&(host->hostEvent), SDMMC_OSA_EVENT_CARD_INSERTED);
        /* notify application about the card insertion status */
        if (sdCD->callback != NULL)
        {
            sdCD->callback(true, sdCD->userData);
        }
    }
    else
    {
        (void)SDMMC_OSAEventSet(&(host->hostEvent), SDMMC_OSA_EVENT_CARD_REMOVED);
        SDHC_EnableInterruptStatus(base, kSDHC_CardInsertionFlag);
        SDHC_EnableInterruptSignal(base, kSDHC_CardInsertionFlag);
    }

    return kStatus_Success;
}

uint32_t SDMMCHOST_CardDetectStatus(sdmmchost_t *host)
{
    sd_detect_card_t *sdCD = (sd_detect_card_t *)(host->cd);
    uint32_t insertStatus  = kSD_Removed;

    if (sdCD->dat3PullFunc != NULL)
    {
        sdCD->dat3PullFunc(kSD_DAT3PullDown);
        SDHC_CardDetectByData3(host->hostController.base, true);
        SDMMC_OSADelay(1U);
    }

    if ((SDHC_GetPresentStatusFlags(host->hostController.base) & (uint32_t)kSDHC_CardInsertedFlag) != 0U)
    {
        insertStatus = kSD_Inserted;
        if (sdCD->dat3PullFunc != NULL)
        {
            sdCD->dat3PullFunc(kSD_DAT3PullUp);
            SDHC_CardDetectByData3(host->hostController.base, false);
        }
    }

    return insertStatus;
}

status_t SDMMCHOST_PollingCardDetectStatus(sdmmchost_t *host, uint32_t waitCardStatus, uint32_t timeout)
{
    assert(host != NULL);
    assert(host->cd != NULL);

    sd_detect_card_t *cd = host->cd;
    uint32_t event       = 0U;

    if (((SDMMCHOST_CardDetectStatus(host) == (uint32_t)kSD_Inserted) && (waitCardStatus == (uint32_t)kSD_Inserted)) ||
        ((SDMMCHOST_CardDetectStatus(host) == (uint32_t)kSD_Removed) && (waitCardStatus == (uint32_t)kSD_Removed)))
    {
        return kStatus_Success;
    }

    (void)SDMMC_OSAEventClear(&(host->hostEvent), SDMMC_OSA_EVENT_CARD_INSERTED | SDMMC_OSA_EVENT_CARD_REMOVED);

    if (cd->type == kSD_DetectCardByHostDATA3)
    {
        if (cd->dat3PullFunc != NULL)
        {
            cd->dat3PullFunc(kSD_DAT3PullDown);
        }
        SDHC_ClearInterruptStatusFlags(host->hostController.base,
                                       (uint32_t)kSDHC_CardInsertionFlag | (uint32_t)kSDHC_CardRemovalFlag);
        SDHC_EnableInterruptSignal(host->hostController.base, waitCardStatus == (uint32_t)kSD_Inserted ?
                                                                  kSDHC_CardInsertionFlag :
                                                                  kSDHC_CardRemovalFlag);
    }

    /* Wait card inserted. */
    do
    {
        if (SDMMC_OSAEventWait(&(host->hostEvent), SDMMC_OSA_EVENT_CARD_INSERTED | SDMMC_OSA_EVENT_CARD_REMOVED,
                               timeout, &event) != kStatus_Success)
        {
            return kStatus_Fail;
        }
        else
        {
            if ((waitCardStatus == (uint32_t)kSD_Inserted) &&
                ((event & SDMMC_OSA_EVENT_CARD_INSERTED) == SDMMC_OSA_EVENT_CARD_INSERTED))
            {
                SDMMC_OSADelay(cd->cdDebounce_ms);
                (void)SDMMC_OSAEventGet(&(host->hostEvent), SDMMC_OSA_EVENT_CARD_INSERTED, &event);
                if ((event & SDMMC_OSA_EVENT_CARD_INSERTED) == SDMMC_OSA_EVENT_CARD_INSERTED)
                {
                    break;
                }
            }

            if (((event & SDMMC_OSA_EVENT_CARD_REMOVED) == SDMMC_OSA_EVENT_CARD_REMOVED) &&
                (waitCardStatus == (uint32_t)kSD_Removed))
            {
                break;
            }
        }
    } while (true);

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

    (void)SDMMC_OSAEventSet(&(((sdmmchost_t *)userData)->hostEvent), eventStatus);
}

status_t SDMMCHOST_TransferFunction(sdmmchost_t *host, sdmmchost_transfer_t *content)
{
    status_t error = kStatus_Success;
    uint32_t event = 0U;

    SDMMC_OSAMutexLock(&host->lock, osaWaitForever_c);

    /* clear redundant transfer event flag */
    (void)SDMMC_OSAEventClear(&(host->hostEvent),
                              SDMMC_OSA_EVENT_TRANSFER_CMD_SUCCESS | SDMMC_OSA_EVENT_TRANSFER_CMD_FAIL |
                                  SDMMC_OSA_EVENT_TRANSFER_DATA_SUCCESS | SDMMC_OSA_EVENT_TRANSFER_DATA_FAIL);

    do
    {
        error = SDHC_TransferNonBlocking(host->hostController.base, &host->handle, host->dmaDesBuffer,
                                         host->dmaDesBufferWordsNum, content);
    } while (error == kStatus_SDHC_BusyTransferring);

    if (error == kStatus_Success)
    {
        /* wait command event */
        if ((kStatus_Fail ==
             SDMMC_OSAEventWait(&(host->hostEvent),
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
                        (kStatus_Fail == SDMMC_OSAEventWait(
                                             &(host->hostEvent),
                                             SDMMC_OSA_EVENT_TRANSFER_DATA_SUCCESS | SDMMC_OSA_EVENT_TRANSFER_DATA_FAIL,
                                             SDMMCHOST_TRANSFER_COMPLETE_TIMEOUT, &event) ||
                         ((event & SDMMC_OSA_EVENT_TRANSFER_DATA_FAIL) != 0U)))
                    {
                        error = kStatus_Fail;
                    }
                }
            }
        }
    }

    if (error != kStatus_Success)
    {
        /* host error recovery */
        SDMMCHOST_ErrorRecovery(host->hostController.base);
    }

    SDMMC_OSAMutexUnlock(&host->lock);

    return error;
}

static void SDMMCHOST_ErrorRecovery(SDHC_Type *base)
{
    uint32_t status = 0U;
    /* get host present status */
    status = SDHC_GetPresentStatusFlags(base);
    /* check command inhibit status flag */
    if ((status & (uint32_t)kSDHC_CommandInhibitFlag) != 0U)
    {
        /* reset command line */
        (void)SDHC_Reset(base, kSDHC_ResetCommand, 100U);
    }
    /* check data inhibit status flag */
    if ((status & (uint32_t)kSDHC_DataInhibitFlag) != 0U)
    {
        /* reset data line */
        (void)SDHC_Reset(base, kSDHC_ResetData, 100U);
    }
}

void SDMMCHOST_ConvertDataToLittleEndian(sdmmchost_t *host, uint32_t *data, uint32_t wordSize, uint32_t format)
{
    uint32_t temp = 0U;

    if (((uint32_t)host->hostController.config.endianMode == (uint32_t)kSDMMCHOST_EndianModeLittle) &&
        (format == kSDMMC_DataPacketFormatMSBFirst))
    {
        for (uint32_t i = 0U; i < wordSize; i++)
        {
            temp    = data[i];
            data[i] = SWAP_WORD_BYTE_SEQUENCE(temp);
        }
    }
    else if ((uint32_t)host->hostController.config.endianMode == (uint32_t)kSDMMCHOST_EndianModeHalfWordBig)
    {
        for (uint32_t i = 0U; i < wordSize; i++)
        {
            temp    = data[i];
            data[i] = SWAP_HALF_WROD_BYTE_SEQUENCE(temp);
        }
    }
    else if (((uint32_t)host->hostController.config.endianMode == (uint32_t)kSDMMCHOST_EndianModeBig) &&
             (format == kSDMMC_DataPacketFormatLSBFirst))
    {
        for (uint32_t i = 0U; i < wordSize; i++)
        {
            temp    = data[i];
            data[i] = SWAP_WORD_BYTE_SEQUENCE(temp);
        }
    }
    else
    {
        /* nothing to do */
    }
}

status_t SDMMCHOST_Init(sdmmchost_t *host)
{
    assert(host != NULL);

    sdhc_transfer_callback_t sdhcCallback = {0};
    sdhc_host_t *sdhcHost                 = &(host->hostController);
    status_t error                        = kStatus_Success;

    /* host capability flags */
    host->capability = (uint32_t)kSDMMCHOST_SupportHighSpeed | (uint32_t)kSDMMCHOST_SupportSuspendResume |
                       (uint32_t)kSDMMCHOST_SupportVoltage3v3 | (uint32_t)kSDMMCHOST_Support4BitDataWidth |
                       (uint32_t)kSDMMCHOST_Support8BitDataWidth | (uint32_t)kSDMMCHOST_SupportDetectCardByData3 |
                       (uint32_t)kSDMMCHOST_SupportAutoCmd12;
    host->maxBlockCount = SDMMCHOST_SUPPORT_MAX_BLOCK_COUNT;
    host->maxBlockSize  = SDMMCHOST_SUPPORT_MAX_BLOCK_LENGTH;
    /* sdmmc osa init */

    SDMMC_OSAMutexCreate(&host->lock);
    SDMMC_OSAMutexLock(&host->lock, osaWaitForever_c);

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
    if (kStatus_Success != SDMMC_OSAEventCreate(&(host->hostEvent)))
    {
        error = kStatus_Fail;
    }

    SDMMC_OSAMutexUnlock(&host->lock);

    return error;
}

void SDMMCHOST_Reset(sdmmchost_t *host)
{
    /* reserved for future */
}

void SDMMCHOST_SetCardBusWidth(sdmmchost_t *host, uint32_t dataBusWidth)
{
    SDHC_SetDataBusWidth(host->hostController.base, dataBusWidth == (uint32_t)kSDMMC_BusWdith1Bit ?
                                                        kSDHC_DataBusWidth1Bit :
                                                        dataBusWidth == (uint32_t)kSDMMC_BusWdith4Bit ?
                                                        kSDHC_DataBusWidth4Bit :
                                                        kSDHC_DataBusWidth8Bit);
}

void SDMMCHOST_Deinit(sdmmchost_t *host)
{
    sdhc_host_t *sdhcHost = &host->hostController;
    SDMMC_OSAMutexLock(&host->lock, osaWaitForever_c);
    SDHC_Deinit(sdhcHost->base);
    (void)SDMMC_OSAEventDestroy(&(host->hostEvent));
    SDMMC_OSAMutexDestroy(&host->lock);
}
