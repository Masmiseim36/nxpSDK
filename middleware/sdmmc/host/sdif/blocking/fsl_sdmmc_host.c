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
static void SDMMCHOST_ErrorRecovery(SDIF_Type *base);
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
    if (cd == NULL)
    {
        return kStatus_Fail;
    }

    host->cd = cd;

    return kStatus_Success;
}

uint32_t SDMMCHOST_CardDetectStatus(sdmmchost_t *host)
{
    SDIF_Type *base        = host->hostController.base;
    sd_detect_card_t *sdCD = (sd_detect_card_t *)host->cd;

#if defined(FSL_FEATURE_SDIF_ONE_INSTANCE_SUPPORT_TWO_CARD) && FSL_FEATURE_SDIF_ONE_INSTANCE_SUPPORT_TWO_CARD
    if (((host->hostPort == 0U) &&
         SDIF_DetectCardInsert(base, sdCD->type == kSD_DetectCardByHostDATA3 ? true : false)) ||
        ((host->hostPort == 1U) &&
         SDIF_DetectCard1Insert(base, sdCD->type == kSD_DetectCardByHostDATA3 ? true : false)))
#else
    if ((host->hostPort == 0U) && SDIF_DetectCardInsert(base, sdCD->type == kSD_DetectCardByHostDATA3 ? true : false))
#endif
    {
        return kSD_Inserted;
    }

    return kSD_Removed;
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
            if (SDMMCHOST_CardDetectStatus(host) == kSD_Inserted)
            {
                break;
            }
        }

        if ((cardInsertedStatus == kSD_Removed) && (waitCardStatus == kSD_Removed))
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

    while (SDIF_DetectCardInsert(hostBase, false) != waitCardStatus)
    {
    }

    return kStatus_Success;
}

status_t SDMMCHOST_TransferFunction(sdmmchost_t *host, sdmmchost_transfer_t *content)
{
    status_t error = kStatus_Success;
    sdif_dma_config_t dmaConfig;

    memset(&dmaConfig, 0, sizeof(dmaConfig));

    /* user DMA mode transfer data */
    if (content->data != NULL)
    {
        dmaConfig.enableFixBurstLen     = false;
        dmaConfig.mode                  = kSDIF_DualDMAMode;
        dmaConfig.dmaDesBufferStartAddr = host->dmaDesBuffer;
        dmaConfig.dmaDesBufferLen       = host->dmaDesBufferWordsNum;
        dmaConfig.dmaDesSkipLen         = 0U;
    }

    error = SDIF_TransferBlocking(host->hostController.base, &dmaConfig, content);

    if (error != kStatus_Success)
    {
        error = kStatus_Fail;
        /* host error recovery */
        SDMMCHOST_ErrorRecovery(host->hostController.base);
    }

    return error;
}

static void SDMMCHOST_ErrorRecovery(SDIF_Type *base)
{
    /* reserved for future */
}

void SDMMCHOST_SetCardPower(sdmmchost_t *host, bool enable)
{
    if (host->hostPort == 0U)
    {
        SDIF_EnableCardPower(host->hostController.base, enable);
    }
#if defined(FSL_FEATURE_SDIF_ONE_INSTANCE_SUPPORT_TWO_CARD) && FSL_FEATURE_SDIF_ONE_INSTANCE_SUPPORT_TWO_CARD
    else
    {
        SDIF_EnableCard1Power(host->hostController.base, enable);
    }
#endif

    if (enable)
    {
        /* perform SDIF host controller reset only when DATA BUSY is assert */
        if ((SDIF_GetControllerStatus(host->hostController.base) & SDIF_STATUS_DATA_BUSY_MASK) != 0U)
        {
            SDIF_Reset(host->hostController.base, kSDIF_ResetAll, 100U);
        }
    }
}

void SDMMCHOST_PowerOffCard(SDMMCHOST_TYPE *base, const sdmmchost_pwr_card_t *pwr)
{
    if (pwr != NULL)
    {
        pwr->powerOff();
        SDMMC_OSADelay(pwr->powerOffDelay_ms);
    }
    else
    {
        /* disable the card power */
        SDIF_EnableCardPower(base, false);
        /* Delay several milliseconds to make card stable. */
        SDMMC_OSADelay(500U);
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
        /* Enable the card power */
        SDIF_EnableCardPower(base, true);
        /* Delay several milliseconds to make card stable. */
        SDMMC_OSADelay(500U);
    }

    /* perform SDIF host controller reset only when DATA BUSY is assert */
    if ((SDIF_GetControllerStatus(base) & SDIF_STATUS_DATA_BUSY_MASK) != 0U)
    {
        SDIF_Reset(base, kSDIF_ResetAll, 100U);
    }
}

status_t SDMMCHOST_Init(sdmmchost_t *host)
{
    assert(host != NULL);

    sdif_host_t *sdifHost = &(host->hostController);

    /* sdmmc osa init */
    SDMMC_OSAInit();

    /* Initialize SDIF. */
    sdifHost->config.endianMode            = kSDMMCHOST_EndianModeLittle;
    sdifHost->config.responseTimeout       = 0xFFU;
    sdifHost->config.cardDetDebounce_Clock = 0xFFFFFFU;
    sdifHost->config.dataTimeout           = 0xFFFFFFU;
    SDIF_Init(sdifHost->base, &(sdifHost->config));

    return kStatus_Success;
}

void SDMMCHOST_Reset(sdmmchost_t *host)
{
    /* make sure host controller release all the bus line. */
    SDIF_Reset(host->hostController.base, kSDIF_ResetAll, 100U);
}

void SDMMCHOST_SetCardBusWidth(sdmmchost_t *host, uint32_t dataBusWidth)
{
    if (host->hostPort == 0U)
    {
        SDIF_SetCardBusWidth(host->hostController.base,
                             dataBusWidth == kSDMMC_BusWdith1Bit ?
                                 kSDIF_Bus1BitWidth :
                                 dataBusWidth == kSDMMC_BusWdith4Bit ? kSDIF_Bus4BitWidth : kSDIF_Bus8BitWidth);
    }
#if defined(FSL_FEATURE_SDIF_ONE_INSTANCE_SUPPORT_TWO_CARD) && FSL_FEATURE_SDIF_ONE_INSTANCE_SUPPORT_TWO_CARD
    else
    {
        SDIF_SetCard1BusWidth(host->hostController.base,
                              dataBusWidth == kSDMMC_BusWdith1Bit ?
                                  kSDIF_Bus1BitWidth :
                                  dataBusWidth == kSDMMC_BusWdith4Bit ? kSDIF_Bus4BitWidth : kSDIF_Bus8BitWidth);
    }
#endif
}

void SDMMCHOST_Deinit(sdmmchost_t *host)
{
    sdif_host_t *sdifHost = &host->hostController;
    SDIF_Deinit(sdifHost->base);
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
