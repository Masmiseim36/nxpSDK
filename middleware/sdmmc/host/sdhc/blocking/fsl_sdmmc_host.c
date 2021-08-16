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
status_t SDMMCHOST_CardIntInit(sdmmchost_t *host, void *sdioInt)
{
    host->cardInt = sdioInt;

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

    /* enable card detect through DATA3 */
    SDHC_CardDetectByData3(base, true);
    /* enable card detect interrupt */
    SDHC_EnableInterruptStatus(base, kSDHC_CardInsertionFlag);
    SDHC_EnableInterruptStatus(base, kSDHC_CardRemovalFlag);

    return kStatus_Success;
}

uint32_t SDMMCHOST_CardDetectStatus(sdmmchost_t *host)
{
    sd_detect_card_t *sdCD = (sd_detect_card_t *)(host->cd);
    uint32_t insertStatus  = kSD_Removed;

    if (sdCD->dat3PullFunc != NULL)
    {
        sdCD->dat3PullFunc(kSD_DAT3PullDown);
        SDMMC_OSADelay(1U);
    }

    if ((SDHC_GetPresentStatusFlags(host->hostController.base) & (uint32_t)kSDHC_CardInsertedFlag) != 0U)
    {
        insertStatus = kSD_Inserted;
        if (sdCD->dat3PullFunc != NULL)
        {
            sdCD->dat3PullFunc(kSD_DAT3PullUp);
        }
    }

    return insertStatus;
}

status_t SDMMCHOST_PollingCardDetectStatus(sdmmchost_t *host, uint32_t waitCardStatus, uint32_t timeout)
{
    assert(host != NULL);
    assert(host->cd != NULL);

    sd_detect_card_t *cd        = host->cd;
    uint32_t cardInsertedStatus = kSD_Removed;

    /* Wait card inserted. */
    do
    {
        cardInsertedStatus = SDMMCHOST_CardDetectStatus(host);
        if ((waitCardStatus == (uint32_t)kSD_Inserted) && (cardInsertedStatus == (uint32_t)kSD_Inserted))
        {
            SDMMC_OSADelay(cd->cdDebounce_ms);
            if (SDMMCHOST_CardDetectStatus(host) == (uint32_t)kSD_Inserted)
            {
                break;
            }
        }

        if ((cardInsertedStatus == (uint32_t)kSD_Inserted) && (waitCardStatus == (uint32_t)kSD_Removed))
        {
            break;
        }
    } while (true);

    return kStatus_Success;
}

status_t SDMMCHOST_TransferFunction(sdmmchost_t *host, sdmmchost_transfer_t *content)
{
    status_t error = kStatus_Success;

    do
    {
        error =
            SDHC_TransferBlocking(host->hostController.base, host->dmaDesBuffer, host->dmaDesBufferWordsNum, content);
    } while (error == kStatus_SDHC_BusyTransferring);

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

    sdhc_host_t *sdhcHost = &(host->hostController);
    /* host capability flags */
    host->capability = (uint32_t)kSDMMCHOST_SupportHighSpeed | (uint32_t)kSDMMCHOST_SupportSuspendResume |
                       (uint32_t)kSDMMCHOST_SupportVoltage3v3 | (uint32_t)kSDMMCHOST_Support4BitDataWidth |
                       (uint32_t)kSDMMCHOST_Support8BitDataWidth | (uint32_t)kSDMMCHOST_SupportDetectCardByData3 |
                       (uint32_t)kSDMMCHOST_SupportAutoCmd12;
    host->maxBlockCount = SDMMCHOST_SUPPORT_MAX_BLOCK_COUNT;
    host->maxBlockSize  = SDMMCHOST_SUPPORT_MAX_BLOCK_LENGTH;

    /* Initializes SDHC. */
    sdhcHost->config.endianMode          = kSDHC_EndianModeLittle;
    sdhcHost->config.dmaMode             = kSDHC_DmaModeAdma2;
    sdhcHost->config.readWatermarkLevel  = 0x80U;
    sdhcHost->config.writeWatermarkLevel = 0x80U;
    SDHC_Init(sdhcHost->base, &(sdhcHost->config));

    return kStatus_Success;
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
    SDHC_Deinit(sdhcHost->base);
}
