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
    SDHC_Type *base = host->hostController.base;
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

    return kStatus_Success;
}

uint32_t SDMMCHOST_CardDetectStatus(sdmmchost_t *host)
{
    return (SDHC_GetInterruptStatusFlags(host->hostController.base) & (uint32_t)kSDHC_CardInsertionFlag) != 0U ?
               kSD_Inserted :
               kSD_Removed;
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
        if ((waitCardStatus == kSD_Inserted) && (cardInsertedStatus == kSD_Inserted))
        {
            SDMMC_OSADelay(cd->cdDebounce_ms);
            if (cardInsertedStatus)
            {
                break;
            }
        }

        if ((cardInsertedStatus == kSD_Inserted) && (waitCardStatus == kSD_Removed))
        {
            break;
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

status_t SDMMCHOST_Init(sdmmchost_t *host)
{
    assert(host != NULL);
    assert(host->hostEvent != NULL);

    sdhc_host_t *sdhcHost = &(host->hostController);

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
    SDHC_SetDataBusWidth(host->hostController.base,
                         dataBusWidth == kSDMMC_BusWdith1Bit ?
                             kSDHC_DataBusWidth1Bit :
                             dataBusWidth == kSDMMC_BusWdith4Bit ? kSDHC_DataBusWidth4Bit : kSDHC_DataBusWidth8Bit);
}

void SDMMCHOST_Deinit(sdmmchost_t *host)
{
    sdhc_host_t *sdhcHost = &host->hostController;
    SDHC_Deinit(sdhcHost->base);
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
