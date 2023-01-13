/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_sdmmc_host.h"
#include "fsl_sdmmc_common.h"
#if ((defined __DCACHE_PRESENT) && __DCACHE_PRESENT) || (defined FSL_FEATURE_HAS_L1CACHE && FSL_FEATURE_HAS_L1CACHE)
#if !(defined(FSL_SDK_ENABLE_DRIVER_CACHE_CONTROL) && FSL_SDK_ENABLE_DRIVER_CACHE_CONTROL)
#include "fsl_cache.h"
#endif
#endif
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
static void SDMMCHOST_ErrorRecovery(USDHC_Type *base);

#if SDMMCHOST_SUPPORT_SDR104 || SDMMCHOST_SUPPORT_SDR50 || SDMMCHOST_SUPPORT_HS200 || SDMMCHOST_SUPPORT_HS400
/*!
 * @brief SDMMCHOST execute manual tuning.
 * @param host host handler.
 * @param tuningCmd tuning command
 * @param revBuf receive buffer pointer
 * @param blockSize receive block size
 */
static status_t SDMMCHOST_ExecuteManualTuning(sdmmchost_t *host,
                                              uint32_t tuningCmd,
                                              uint32_t *revBuf,
                                              uint32_t blockSize);
#endif
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
    USDHC_Type *base       = host->hostController.base;
    sd_detect_card_t *sdCD = (sd_detect_card_t *)cd;

    if ((cd == NULL) || ((sdCD->type != kSD_DetectCardByHostDATA3) && (sdCD->type != kSD_DetectCardByHostCD)))
    {
        return kStatus_Fail;
    }

    host->cd = cd;

    /* enable card detect interrupt */
    USDHC_EnableInterruptStatus(base, kUSDHC_CardInsertionFlag);
    USDHC_EnableInterruptStatus(base, kUSDHC_CardRemovalFlag);

    return kStatus_Success;
}

uint32_t SDMMCHOST_CardDetectStatus(sdmmchost_t *host)
{
    sd_detect_card_t *sdCD = (sd_detect_card_t *)(host->cd);
    uint32_t insertStatus  = kSD_Removed;

    if (sdCD->type == kSD_DetectCardByHostDATA3)
    {
        USDHC_CardDetectByData3(host->hostController.base, true);
        if (sdCD->dat3PullFunc != NULL)
        {
            sdCD->dat3PullFunc(kSD_DAT3PullDown);
            SDMMC_OSADelay(SDMMCHOST_DATA3_DETECT_CARD_DELAY);
        }
    }
    else
    {
        USDHC_CardDetectByData3(host->hostController.base, false);
    }

    if ((USDHC_GetPresentStatusFlags(host->hostController.base) & (uint32_t)kUSDHC_CardInsertedFlag) != 0U)
    {
        insertStatus = kSD_Inserted;

        if (sdCD->type == kSD_DetectCardByHostDATA3)
        {
            if (sdCD->dat3PullFunc != NULL)
            {
                sdCD->dat3PullFunc(kSD_DAT3PullUp);
            }
            /* disable the DAT3 card detec function */
            USDHC_CardDetectByData3(host->hostController.base, false);
        }
    }

    return insertStatus;
}

status_t SDMMCHOST_PollingCardDetectStatus(sdmmchost_t *host, uint32_t waitCardStatus, uint32_t timeout)
{
    assert(host != NULL);
    assert(host->cd != NULL);

    sd_detect_card_t *cd = host->cd;
    bool isCardInserted  = false;

    /* Wait card inserted. */
    do
    {
        isCardInserted = (SDMMCHOST_CardDetectStatus(host) == (uint32_t)kSD_Inserted);
        if ((waitCardStatus == (uint32_t)kSD_Inserted) && isCardInserted)
        {
            SDMMC_OSADelay(cd->cdDebounce_ms);
            if (SDMMCHOST_CardDetectStatus(host) == (uint32_t)kSD_Inserted)
            {
                break;
            }
        }

        if ((isCardInserted == false) && (waitCardStatus == (uint32_t)kSD_Removed))
        {
            break;
        }
    } while (true);

    return kStatus_Success;
}

#if defined SDMMCHOST_ENABLE_CACHE_LINE_ALIGN_TRANSFER && SDMMCHOST_ENABLE_CACHE_LINE_ALIGN_TRANSFER

void SDMMCHOST_InstallCacheAlignBuffer(sdmmchost_t *host, void *cacheAlignBuffer, uint32_t cacheAlignBufferSize)
{
    /* blocking adapter not support the cache line align transfer */
    assert(false);
}
#endif

status_t SDMMCHOST_TransferFunction(sdmmchost_t *host, sdmmchost_transfer_t *content)
{
    status_t error = kStatus_Success;

    usdhc_adma_config_t dmaConfig;

    if (content->data != NULL)
    {
        (void)memset(&dmaConfig, 0, sizeof(usdhc_adma_config_t));
        /* config adma */
        dmaConfig.dmaMode = SDMMCHOST_DMA_MODE;
#if !(defined(FSL_FEATURE_USDHC_HAS_NO_RW_BURST_LEN) && FSL_FEATURE_USDHC_HAS_NO_RW_BURST_LEN)
        dmaConfig.burstLen = kUSDHC_EnBurstLenForINCR;
#endif
        dmaConfig.admaTable      = host->dmaDesBuffer;
        dmaConfig.admaTableWords = host->dmaDesBufferWordsNum;

#if ((defined __DCACHE_PRESENT) && __DCACHE_PRESENT) || (defined FSL_FEATURE_HAS_L1CACHE && FSL_FEATURE_HAS_L1CACHE)
#if !(defined(FSL_SDK_ENABLE_DRIVER_CACHE_CONTROL) && FSL_SDK_ENABLE_DRIVER_CACHE_CONTROL)
        if (host->enableCacheControl == (uint32_t)kSDMMCHOST_CacheControlRWBuffer)
        {
            /* no matter read or write transfer, clean the cache line anyway to avoid data miss */
            DCACHE_CleanByRange(
                (uint32_t)(content->data->txData == NULL ? content->data->rxData : content->data->txData),
                (content->data->blockSize) * (content->data->blockCount));
        }
#endif
#endif
    }

    error = USDHC_TransferBlocking(host->hostController.base, &dmaConfig, content);

    if (error != kStatus_Success)
    {
        /* host error recovery */
        SDMMCHOST_ErrorRecovery(host->hostController.base);
    }
#if ((defined __DCACHE_PRESENT) && __DCACHE_PRESENT) || (defined FSL_FEATURE_HAS_L1CACHE && FSL_FEATURE_HAS_L1CACHE)
#if !(defined(FSL_SDK_ENABLE_DRIVER_CACHE_CONTROL) && FSL_SDK_ENABLE_DRIVER_CACHE_CONTROL)
    else
    {
        if ((content->data != NULL) && (content->data->rxData != NULL) &&
            (host->enableCacheControl == (uint32_t)kSDMMCHOST_CacheControlRWBuffer))
        {
            /* no matter read or write transfer, clean the cache line anyway to avoid data miss */
            DCACHE_InvalidateByRange((uint32_t)content->data->rxData,
                                     (content->data->blockSize) * (content->data->blockCount));
        }
    }
#endif
#endif

    return error;
}

static void SDMMCHOST_ErrorRecovery(USDHC_Type *base)
{
    uint32_t status = 0U;
    /* get host present status */
    status = USDHC_GetPresentStatusFlags(base);
    /* check command inhibit status flag */
    if ((status & (uint32_t)kUSDHC_CommandInhibitFlag) != 0U)
    {
        /* reset command line */
        (void)USDHC_Reset(base, kUSDHC_ResetCommand, 100U);
    }
    /* check data inhibit status flag */
    if (((status & (uint32_t)kUSDHC_DataInhibitFlag) != 0U) || (USDHC_GetAdmaErrorStatusFlags(base) != 0U))
    {
        /* reset data line */
        (void)USDHC_Reset(base, kUSDHC_ResetData, 100U);
    }
}

void SDMMCHOST_SetCardPower(sdmmchost_t *host, bool enable)
{
    /* host not support */
}

void SDMMCHOST_SetCardBusWidth(sdmmchost_t *host, uint32_t dataBusWidth)
{
    USDHC_SetDataBusWidth(host->hostController.base, dataBusWidth == (uint32_t)kSDMMC_BusWdith1Bit ?
                                                         kUSDHC_DataBusWidth1Bit :
                                                         dataBusWidth == (uint32_t)kSDMMC_BusWdith4Bit ?
                                                         kUSDHC_DataBusWidth4Bit :
                                                         kUSDHC_DataBusWidth8Bit);
}

void SDMMCHOST_ConvertDataToLittleEndian(sdmmchost_t *host, uint32_t *data, uint32_t wordSize, uint32_t format)
{
    uint32_t temp = 0U;

    if (((uint32_t)host->hostController.config.endianMode == (uint32_t)kSDMMCHOST_EndianModeLittle) &&
        (format == (uint32_t)kSDMMC_DataPacketFormatMSBFirst))
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
             (format == (uint32_t)kSDMMC_DataPacketFormatLSBFirst))
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

    usdhc_host_t *usdhcHost = &(host->hostController);

#if defined FSL_FEATURE_USDHC_INSTANCE_SUPPORT_8_BIT_WIDTHn
    uint32_t bus8bitCapability = (uint32_t)FSL_FEATURE_USDHC_INSTANCE_SUPPORT_8_BIT_WIDTHn(host->hostController.base);
#else
    uint32_t bus8bitCapability    = 0U;
#endif

#if (defined(FSL_FEATURE_USDHC_HAS_HS400_MODE) && (FSL_FEATURE_USDHC_HAS_HS400_MODE))
#if defined FSL_FEATURE_USDHC_INSTANCE_SUPPORT_HS400_MODEn
    uint32_t hs400Capability = (uint32_t)FSL_FEATURE_USDHC_INSTANCE_SUPPORT_HS400_MODEn(host->hostController.base);
#else
    uint32_t hs400Capability = 0U;
#endif
#endif

#if defined FSL_FEATURE_USDHC_INSTANCE_SUPPORT_1V8_SIGNALn
    uint32_t voltage1v8Capability = (uint32_t)FSL_FEATURE_USDHC_INSTANCE_SUPPORT_1V8_SIGNALn(host->hostController.base);
#else
    uint32_t voltage1v8Capability = 0U;
#endif
    /* sdmmc osa init */

    host->capability = (uint32_t)kSDMMCHOST_SupportHighSpeed | (uint32_t)kSDMMCHOST_SupportSuspendResume |
                       (uint32_t)kSDMMCHOST_SupportVoltage3v3 | (uint32_t)kSDMMCHOST_SupportVoltage1v8 |
                       (uint32_t)kSDMMCHOST_SupportVoltage1v2 | (uint32_t)kSDMMCHOST_Support4BitDataWidth |
                       (uint32_t)kSDMMCHOST_SupportDDRMode | (uint32_t)kSDMMCHOST_SupportDetectCardByData3 |
                       (uint32_t)kSDMMCHOST_SupportDetectCardByCD | (uint32_t)kSDMMCHOST_SupportAutoCmd12;

    if (bus8bitCapability != 0U)
    {
        host->capability |= (uint32_t)kSDMMCHOST_Support8BitDataWidth;
    }

    if (voltage1v8Capability != 0U)
    {
#if (defined(FSL_FEATURE_USDHC_HAS_SDR104_MODE) && (FSL_FEATURE_USDHC_HAS_SDR104_MODE))
        host->capability |= (uint32_t)kSDMMCHOST_SupportSDR104;
#endif
#if (defined(FSL_FEATURE_USDHC_HAS_SDR50_MODE) && (FSL_FEATURE_USDHC_HAS_SDR50_MODE))
        host->capability |= (uint32_t)kSDMMCHOST_SupportSDR50 | (uint32_t)kSDMMCHOST_SupportHS200;
#endif
    }

#if (defined(FSL_FEATURE_USDHC_HAS_HS400_MODE) && (FSL_FEATURE_USDHC_HAS_HS400_MODE))
    if (hs400Capability != 0U)
    {
        host->capability |= (uint32_t)kSDMMCHOST_SupportHS400;
    }
#endif

    host->maxBlockCount = SDMMCHOST_SUPPORT_MAX_BLOCK_COUNT;
    host->maxBlockSize  = SDMMCHOST_SUPPORT_MAX_BLOCK_LENGTH;

    /* Initializes USDHC. */
    usdhcHost->config.endianMode          = kUSDHC_EndianModeLittle;
    usdhcHost->config.dataTimeout         = 0xFU;
    usdhcHost->config.readWatermarkLevel  = 0x80U;
    usdhcHost->config.writeWatermarkLevel = 0x80U;
    USDHC_Init(usdhcHost->base, &(usdhcHost->config));

    return kStatus_Success;
}

void SDMMCHOST_Reset(sdmmchost_t *host)
{
    USDHC_Type *base = host->hostController.base;

#if SDMMCHOST_SUPPORT_VOLTAGE_CONTROL
    /* voltage switch to normal but not 1.8V */
    UDSHC_SelectVoltage(base, false);
#endif
    /* Disable DDR mode */
    USDHC_EnableDDRMode(base, false, 0U);
    /* disable tuning */
#if defined(FSL_FEATURE_USDHC_HAS_SDR50_MODE) && (FSL_FEATURE_USDHC_HAS_SDR50_MODE)
    USDHC_EnableStandardTuning(base, 0, 0, false);
    USDHC_EnableAutoTuning(host->hostController.base, false);
#endif

#if FSL_FEATURE_USDHC_HAS_HS400_MODE
    /* Disable HS400 mode */
    USDHC_EnableHS400Mode(base, false);
    /* Disable DLL */
    USDHC_EnableStrobeDLL(base, false);
#endif

    /* reset data/command/tuning circuit */
    (void)USDHC_Reset(base, kUSDHC_ResetAll, 100U);
}

void SDMMCHOST_Deinit(sdmmchost_t *host)
{
    usdhc_host_t *sdhcHost = &host->hostController;
    SDMMCHOST_Reset(host);
    USDHC_Deinit(sdhcHost->base);
}

void SDMMCHOST_SwitchToVoltage(sdmmchost_t *host, uint32_t voltage)
{
#if !(defined(FSL_FEATURE_USDHC_HAS_NO_VOLTAGE_SELECT) && (FSL_FEATURE_USDHC_HAS_NO_VOLTAGE_SELECT))
    if (voltage == (uint32_t)kSDMMC_OperationVoltage180V)
    {
        UDSHC_SelectVoltage(host->hostController.base, true);
    }
    else
    {
        UDSHC_SelectVoltage(host->hostController.base, false);
    }
#endif
}
#if SDMMCHOST_SUPPORT_SDR104 || SDMMCHOST_SUPPORT_SDR50 || SDMMCHOST_SUPPORT_HS200 || SDMMCHOST_SUPPORT_HS400

static status_t SDMMCHOST_ExecuteStdTuning(sdmmchost_t *host, uint32_t tuningCmd, uint32_t *revBuf, uint32_t blockSize)
{
    sdmmchost_transfer_t content = {0U};
    sdmmchost_cmd_t command      = {0U};
    sdmmchost_data_t data        = {0U};
    bool tuningError             = true;
    status_t error               = kStatus_Success;

    command.index        = tuningCmd;
    command.argument     = 0U;
    command.responseType = kCARD_ResponseTypeR1;

    data.blockSize  = blockSize;
    data.blockCount = 1U;
    data.rxData     = revBuf;
    data.dataType   = kUSDHC_TransferDataTuning;

    content.command = &command;
    content.data    = &data;

    (void)USDHC_Reset(host->hostController.base, kUSDHC_ResetTuning, 100U);

    /* disable standard tuning */
    USDHC_EnableStandardTuning(host->hostController.base, SDMMCHOST_STANDARD_TUNING_START, SDMMCHOST_TUINIG_STEP,
                               false);
    USDHC_ForceClockOn(host->hostController.base, true);
    /*
     * Tuning fail found on some SOCS caused by the difference of delay cell, so we need to i
     * ncrease the tuning counter to cover the adjustable tuninig window
     */
    USDHC_SetStandardTuningCounter(host->hostController.base, SDMMCHOST_STANDARD_TUNING_COUNTER);
    /* enable the standard tuning */
    USDHC_EnableStandardTuning(host->hostController.base, SDMMCHOST_STANDARD_TUNING_START, SDMMCHOST_TUINIG_STEP, true);

    while (true)
    {
        error = SDMMCHOST_TransferFunction(host, &content);
        /* send tuning block */
        if (kStatus_Success != error)
        {
            return kStatus_SDMMC_TransferFailed;
        }
        SDMMC_OSADelay(1U);

        /*wait excute tuning bit clear*/
        if ((USDHC_GetExecuteStdTuningStatus(host->hostController.base) != 0U))
        {
            continue;
        }

        /* if tuning error , re-tuning again */
        if ((USDHC_CheckTuningError(host->hostController.base) != 0U) && tuningError)
        {
            tuningError = false;
            /* enable the standard tuning */
            USDHC_EnableStandardTuning(host->hostController.base, SDMMCHOST_STANDARD_TUNING_START,
                                       SDMMCHOST_TUINIG_STEP, true);
            (void)USDHC_AdjustDelayForManualTuning(host->hostController.base, SDMMCHOST_STANDARD_TUNING_START);
        }
        else
        {
            break;
        }
    }

    /* check tuning result*/
    if (USDHC_CheckStdTuningResult(host->hostController.base) == 0U)
    {
        return kStatus_SDMMC_TuningFail;
    }
    USDHC_ForceClockOn(host->hostController.base, false);
    USDHC_EnableAutoTuning(host->hostController.base, true);
    /* do not handle tuning error status, since the tuning error detect circuit is very sensetive which may drop down
     * the transfer performance */
    USDHC_DisableInterruptStatus(host->hostController.base, kUSDHC_TuningErrorFlag);

    return kStatus_Success;
}

static status_t SDMMC_CheckTuningResult(uint32_t *tuningWindow, uint32_t *validWindowStart, uint32_t *validWindowEnd)
{
    uint32_t tempValidWindowLen = 0U, tempValidWindowStart = 0U, tempValidWindowEnd = 0U;
    uint32_t validWindowLenMax = 0U, ValidWindowStartMax = 0U, validWindowEndMax = 0U;

    for (uint32_t i = 0U; i < SDMMCHOST_MAX_TUNING_DELAY_CELL; i++)
    {
        if ((tuningWindow[i / 32U] & (1UL << (i % 32U))) != 0U)
        {
            if (tempValidWindowLen == 0U)
            {
                tempValidWindowStart = i;
            }
            tempValidWindowLen++;
        }
        else
        {
            if (tempValidWindowLen != 0U)
            {
                tempValidWindowEnd = i - 1U;

#if defined SDMMC_ENABLE_LOG_PRINT
                SDMMC_LOG("valid tuning window start: %d, end: %d\r\n", tempValidWindowStart, tempValidWindowEnd);
#endif
                if (tempValidWindowLen > validWindowLenMax)
                {
                    validWindowLenMax   = tempValidWindowLen;
                    ValidWindowStartMax = tempValidWindowStart;
                    validWindowEndMax   = tempValidWindowEnd;
                }
                tempValidWindowLen = 0U;
            }
        }
    }

    if (validWindowLenMax == 0U)
    {
        return kStatus_Fail;
    }

    *validWindowStart = ValidWindowStartMax;
    *validWindowEnd   = validWindowEndMax;

    return kStatus_Success;
}

static status_t SDMMCHOST_ExecuteManualTuning(sdmmchost_t *host,
                                              uint32_t tuningCmd,
                                              uint32_t *revBuf,
                                              uint32_t blockSize)
{
    uint32_t *buffer         = revBuf;
    status_t ret             = kStatus_Success;
    uint32_t tuningDelayCell = 0U;
    uint32_t tuningWindow[4] = {0U}, tuningWindowStart = 0U, tuningWindowEnd = 0U;

    sdmmchost_transfer_t content = {0U};
    sdmmchost_cmd_t command      = {0U};
    sdmmchost_data_t data        = {0U};

    command.index        = tuningCmd;
    command.argument     = 0U;
    command.responseType = kCARD_ResponseTypeR1;

    data.blockSize  = blockSize;
    data.blockCount = 1U;
    data.rxData     = revBuf;

    content.command = &command;
    content.data    = &data;

    (void)USDHC_Reset(host->hostController.base, kUSDHC_ResetAll, 100U);
    USDHC_EnableManualTuning(host->hostController.base, true);
    USDHC_ForceClockOn(host->hostController.base, true);

    while (true)
    {
        (void)USDHC_AdjustDelayForManualTuning(host->hostController.base, tuningDelayCell);

        if ((SDMMCHOST_TransferFunction(host, &content) == kStatus_Success) &&
            (((uint32_t)kUSDHC_TuningPassFlag & USDHC_GetInterruptStatusFlags(host->hostController.base)) != 0U))
        {
            USDHC_ClearInterruptStatusFlags(host->hostController.base, kUSDHC_TuningPassFlag);
            tuningWindow[tuningDelayCell / 32U] |= 1UL << (tuningDelayCell % 32U);

#if defined SDMMC_ENABLE_LOG_PRINT
            SDMMC_LOG("tuning pass point: %d\r\n", tuningDelayCell);
#endif
        }
        else
        {
#if defined SDMMC_ENABLE_LOG_PRINT
            SDMMC_LOG("tuning fail point: %d\r\n", tuningDelayCell);
#endif
        }

        if (++tuningDelayCell >= SDMMCHOST_MAX_TUNING_DELAY_CELL)
        {
            break;
        }

        (void)memset(buffer, 0, blockSize);

        SDMMC_OSADelay(2U);
    }

    /* After the whole 0-128 delay cell validated, tuning result information stored in tuningWindow, this function will
    check the valid winddow and will select a longest window as the final tuning delay setting */
    if (SDMMC_CheckTuningResult(tuningWindow, &tuningWindowStart, &tuningWindowEnd) == kStatus_Fail)
    {
        return kStatus_Fail;
    }

    /* abort tuning */
    USDHC_EnableManualTuning(host->hostController.base, false);
    USDHC_ForceClockOn(host->hostController.base, false);
    (void)USDHC_Reset(host->hostController.base, kUSDHC_ResetAll, 100U);

    /* select middle position of the window */
    (void)USDHC_SetTuningDelay(host->hostController.base, (tuningWindowStart + tuningWindowEnd) / 2U - 3U, 3U, 3U);
    tuningDelayCell = ((tuningWindowStart + tuningWindowEnd) / 2U - 3U) << 8U | 0x33U;
    /* wait the tuning delay value write successfully */
    while ((USDHC_GetTuningDelayStatus(host->hostController.base) & tuningDelayCell) != tuningDelayCell)
    {
    }
    /* enable auto tuning */
    USDHC_EnableAutoTuning(host->hostController.base, true);

    return ret;
}
#endif

status_t SDMMCHOST_ExecuteTuning(sdmmchost_t *host, uint32_t tuningCmd, uint32_t *revBuf, uint32_t blockSize)
{
#if SDMMCHOST_SUPPORT_SDR104 || SDMMCHOST_SUPPORT_SDR50 || SDMMCHOST_SUPPORT_HS200 || SDMMCHOST_SUPPORT_HS400
    if (host->tuningType == (uint32_t)kSDMMCHOST_StandardTuning)
    {
        return SDMMCHOST_ExecuteStdTuning(host, tuningCmd, revBuf, blockSize);
    }

    return SDMMCHOST_ExecuteManualTuning(host, tuningCmd, revBuf, blockSize);
#else
    return kStatus_SDMMC_NotSupportYet;
#endif
}

status_t SDMMCHOST_StartBoot(sdmmchost_t *host,
                             sdmmchost_boot_config_t *hostConfig,
                             sdmmchost_cmd_t *cmd,
                             uint8_t *buffer)
{
    sdmmchost_transfer_t content = {0};
    sdmmchost_data_t data        = {0};
    status_t error               = kStatus_Success;

    USDHC_SetMmcBootConfig(host->hostController.base, hostConfig);

    data.blockSize  = hostConfig->blockSize;
    data.blockCount = hostConfig->blockCount;
    data.rxData     = (uint32_t *)(uint32_t)buffer;
    data.dataType   = kUSDHC_TransferDataBoot;

    content.data    = &data;
    content.command = cmd;

    error = SDMMCHOST_TransferFunction(host, &content);
    /* should check tuning error during every transfer*/
    if (kStatus_Success != error)
    {
        return kStatus_SDMMC_TransferFailed;
    }

    return kStatus_Success;
}

status_t SDMMCHOST_ReadBootData(sdmmchost_t *host, sdmmchost_boot_config_t *hostConfig, uint8_t *buffer)
{
    sdmmchost_cmd_t command      = {0};
    sdmmchost_transfer_t content = {0};
    sdmmchost_data_t data        = {0};
    status_t error               = kStatus_Success;

    USDHC_SetMmcBootConfig(host->hostController.base, hostConfig);
    USDHC_EnableMmcBoot(host->hostController.base, true);

    data.blockSize  = hostConfig->blockSize;
    data.blockCount = hostConfig->blockCount;
    data.rxData     = (uint32_t *)(uint32_t)buffer;
    data.dataType   = kUSDHC_TransferDataBootcontinous;
    /* no command should be send out  */
    command.type = kCARD_CommandTypeEmpty;

    content.data    = &data;
    content.command = &command;

    error = SDMMCHOST_TransferFunction(host, &content);
    if (kStatus_Success != error)
    {
        return kStatus_SDMMC_TransferFailed;
    }

    return kStatus_Success;
}
