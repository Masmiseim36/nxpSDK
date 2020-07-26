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
static void SDMMCHOST_ErrorRecovery(USDHC_Type *base);
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
    if (cd == NULL)
    {
        return kStatus_Fail;
    }

    host->cd = cd;

    if (sdCD->type == kSD_DetectCardByHostDATA3)
    {
        USDHC_CardDetectByData3(base, true);
    }
    else if (sdCD->type == kSD_DetectCardByHostCD)
    {
        USDHC_CardDetectByData3(base, false);
    }
    else
    {
        assert(false);
    }

    /* enable card detect interrupt */
    USDHC_EnableInterruptStatus(base, kUSDHC_CardInsertionFlag);
    USDHC_EnableInterruptStatus(base, kUSDHC_CardRemovalFlag);

    return kStatus_Success;
}

uint32_t SDMMCHOST_CardDetectStatus(sdmmchost_t *host)
{
    if ((USDHC_GetPresentStatusFlags(host->hostController.base) & kUSDHC_CardInsertedFlag) != 0U)
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
    bool isCardInserted  = false;

    /* Wait card inserted. */
    do
    {
        isCardInserted = (SDMMCHOST_CardDetectStatus(host) == kSD_Inserted);
        if ((waitCardStatus == kSD_Inserted) && isCardInserted)
        {
            SDMMC_OSADelay(cd->cdDebounce_ms);
            if (SDMMCHOST_CardDetectStatus(host) == kSD_Inserted)
            {
                break;
            }
        }

        if ((isCardInserted == false) && (waitCardStatus == kSD_Removed))
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

    while ((USDHC_GetInterruptStatusFlags(hostBase) & (uint32_t)kUSDHC_CardInsertionFlag) != (uint32_t)waitCardStatus)
    {
    }

    return kStatus_Success;
}

status_t SDMMCHOST_TransferFunction(sdmmchost_t *host, sdmmchost_transfer_t *content)
{
    status_t error = kStatus_Success;

    usdhc_adma_config_t dmaConfig;

    if (content->data != NULL)
    {
        memset(&dmaConfig, 0, sizeof(usdhc_adma_config_t));
        /* config adma */
        dmaConfig.dmaMode = kUSDHC_DmaModeAdma2;
#if !(defined(FSL_FEATURE_USDHC_HAS_NO_RW_BURST_LEN) && FSL_FEATURE_USDHC_HAS_NO_RW_BURST_LEN)
        dmaConfig.burstLen = kUSDHC_EnBurstLenForINCR;
#endif
        dmaConfig.admaTable      = host->dmaDesBuffer;
        dmaConfig.admaTableWords = host->dmaDesBufferWordsNum;
    }

    error = USDHC_TransferBlocking(host->hostController.base, &dmaConfig, content);

    if (error != kStatus_Success)
    {
        /* host error recovery */
        SDMMCHOST_ErrorRecovery(host->hostController.base);
    }

    return error;
}

static void SDMMCHOST_ErrorRecovery(USDHC_Type *base)
{
    uint32_t status = 0U;
    /* get host present status */
    status = USDHC_GetPresentStatusFlags(base);
    /* check command inhibit status flag */
    if ((status & kUSDHC_CommandInhibitFlag) != 0U)
    {
        /* reset command line */
        USDHC_Reset(base, kUSDHC_ResetCommand, 100U);
    }
    /* check data inhibit status flag */
    if ((status & kUSDHC_DataInhibitFlag) != 0U)
    {
        /* reset data line */
        USDHC_Reset(base, kUSDHC_ResetData, 100U);
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

    usdhc_host_t *usdhcHost = &(host->hostController);

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

    /* voltage switch to normal but not 1.8V */
    UDSHC_SelectVoltage(base, false);
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
    USDHC_Reset(base, kUSDHC_ResetAll, 100U);
}

void SDMMCHOST_SetCardBusWidth(sdmmchost_t *host, uint32_t dataBusWidth)
{
    USDHC_SetDataBusWidth(host->hostController.base,
                          dataBusWidth == kSDMMC_BusWdith1Bit ?
                              kUSDHC_DataBusWidth1Bit :
                              dataBusWidth == kSDMMC_BusWdith4Bit ? kUSDHC_DataBusWidth4Bit : kUSDHC_DataBusWidth8Bit);
}

void SDMMCHOST_Deinit(sdmmchost_t *host)
{
    usdhc_host_t *sdhcHost = &host->hostController;
    SDMMCHOST_Reset(host);
    USDHC_Deinit(sdhcHost->base);
    SDMMC_OSAEventDestroy(host->hostEvent);
}

void SDMMCHOST_SwitchToVoltage(sdmmchost_t *host, uint32_t voltage)
{
    if (voltage == kSDMMC_OperationVoltage180V)
    {
        UDSHC_SelectVoltage(host->hostController.base, true);
    }
    else
    {
        UDSHC_SelectVoltage(host->hostController.base, false);
    }
}

#if SDMMCHOST_SUPPORT_SDR104 || SDMMCHOST_SUPPORT_SDR50 || SDMMCHOST_SUPPORT_HS200 || SDMMCHOST_SUPPORT_HS400

status_t SDMMCHOST_ExecuteStdTuning(sdmmchost_t *host, uint32_t tuningCmd, uint32_t *revBuf, uint32_t blockSize)
{
    sdmmchost_transfer_t content = {0U};
    sdmmchost_cmd_t command      = {0U};
    sdmmchost_data_t data        = {0U};
    bool tuningError             = true;

    command.index        = tuningCmd;
    command.argument     = 0U;
    command.responseType = kCARD_ResponseTypeR1;

    data.blockSize  = blockSize;
    data.blockCount = 1U;
    data.rxData     = revBuf;
    data.dataType   = kUSDHC_TransferDataTuning;

    content.command = &command;
    content.data    = &data;

    USDHC_Reset(host->hostController.base, kUSDHC_ResetTuning, 100U);

    /* enable the standard tuning */
    USDHC_EnableStandardTuning(host->hostController.base, SDMMCHOST_STANDARD_TUNING_START, SDMMCHOST_TUINIG_STEP, true);

    while (true)
    {
        /* send tuning block */
        if ((kStatus_Success != SDMMCHOST_TransferFunction(host, &content)))
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
            USDHC_AdjustDelayForManualTuning(host->hostController.base, SDMMCHOST_STANDARD_TUNING_START);
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

    USDHC_EnableAutoTuning(host->hostController.base, true);
    /* do not handle tuning error status, since the tuning error detect circuit is very sensetive which may drop down
     * the transfer performance */
    USDHC_DisableInterruptStatus(host->hostController.base, kUSDHC_TuningErrorFlag);

    return kStatus_Success;
}

status_t SDMMCHOST_ReceiveTuningBlock(sdmmchost_t *host, uint32_t tuningCmd, uint32_t *revBuf, uint32_t size)
{
    assert(revBuf != NULL);

    usdhc_command_t command   = {0U};
    uint32_t interruptStatus  = 0U;
    uint32_t transferredWords = 0U;
    uint32_t wordSize         = size / sizeof(uint32_t);
    USDHC_Type *base          = host->hostController.base;

    command.index        = tuningCmd;
    command.argument     = 0U;
    command.responseType = kCARD_ResponseTypeR1;
    command.flags        = kUSDHC_DataPresentFlag;

    /* disable DMA first */
    USDHC_EnableInternalDMA(base, false);
    /* set data configurations */
    USDHC_SetDataConfig(base, kUSDHC_TransferDirectionReceive, 1U, size);
    /* enable status */
    USDHC_EnableInterruptStatus(base,
                                kUSDHC_CommandCompleteFlag | kUSDHC_CommandErrorFlag | kUSDHC_BufferReadReadyFlag);
    /* polling cmd done */
    USDHC_SendCommand(base, &command);
    while (!(interruptStatus & (kUSDHC_CommandCompleteFlag | kUSDHC_CommandErrorFlag)))
    {
        interruptStatus = USDHC_GetInterruptStatusFlags(base);
    }
    /* clear interrupt status */
    USDHC_ClearInterruptStatusFlags(base, interruptStatus);
    /* check command inhibit status flag */
    if ((USDHC_GetPresentStatusFlags(base) & kUSDHC_CommandInhibitFlag) != 0U)
    {
        /* reset command line */
        USDHC_Reset(base, kUSDHC_ResetCommand, 100U);
    }

    while (!(interruptStatus & kUSDHC_BufferReadReadyFlag))
    {
        interruptStatus = USDHC_GetInterruptStatusFlags(base);
    }

    while (transferredWords < wordSize)
    {
        revBuf[transferredWords++] = USDHC_ReadData(base);
    }

    USDHC_ClearInterruptStatusFlags(base, interruptStatus | kUSDHC_DataCompleteFlag | kUSDHC_DataErrorFlag);

    return kStatus_Success;
}

status_t SDMMC_CheckTuningResult(uint32_t *buffer, uint32_t size)
{
    uint32_t i = 0U;

    for (i = 0U; i < size / sizeof(uint32_t); i++)
    {
        if (SDMMC_TuningBlockPattern4Bit[i] != SWAP_WORD_BYTE_SEQUENCE(buffer[i]))
        {
#if SDMMC_ENABLE_LOG_PRINT
            SDMMC_LOG("tuning unmatch target: %x, read :%x\r\n", SDMMC_TuningBlockPattern4Bit[i],
                      SWAP_WORD_BYTE_SEQUENCE(buffer[i]));
#endif
            return kStatus_SDMMC_TuningFail;
        }
    }

    return kStatus_Success;
}

status_t SDMMCHOST_ExecuteManualTuning(sdmmchost_t *host, uint32_t tuningCmd, uint32_t *revBuf, uint32_t blockSize)
{
    uint32_t *buffer             = revBuf;
    uint32_t tuningDelayCell     = 0U;
    uint32_t validDelayCellStart = 0U;
    bool validWindowFound        = false;
    uint32_t validWindowCounter  = 0U;
    status_t ret                 = kStatus_Success;
    USDHC_Type *base             = host->hostController.base;

    USDHC_Reset(base, kUSDHC_ResetTuning, 100U);
    USDHC_EnableManualTuning(base, true);

    while (true)
    {
        USDHC_AdjustDelayForManualTuning(base, tuningDelayCell);

        SDMMCHOST_ReceiveTuningBlock(host, tuningCmd, buffer, blockSize);

        if (kStatus_Success == SDMMC_CheckTuningResult(buffer, blockSize))
        {
            if (validWindowFound == false)
            {
                validDelayCellStart = tuningDelayCell;
                validWindowFound    = true;
            }

            if ((validWindowCounter + validDelayCellStart) != tuningDelayCell)
            {
                validWindowFound   = false;
                validWindowCounter = 0U;
            }

            validWindowCounter++;

#if SDMMC_ENABLE_LOG_PRINT
            SDMMC_LOG("tuning pass point: %d\r\n", tuningDelayCell);
#endif
        }
        else
        {
            if ((validWindowFound) && (validWindowCounter > 2U))
            {
                break;
            }
        }

        if (++tuningDelayCell >= SDMMCHOST_MAX_TUNING_DELAY_CELL)
        {
            break;
        }

        memset(buffer, 0U, blockSize);

        SDMMC_OSADelay(2U);
    }
    memset(buffer, 0U, blockSize);

    SDMMC_OSADelay(2U);

    /* select middle position of the window */
    USDHC_AdjustDelayForManualTuning(base, validDelayCellStart + validWindowCounter / 2U);
    /* send tuning block with the average delay cell */
    SDMMCHOST_ReceiveTuningBlock(host, tuningCmd, buffer, blockSize);
    ret = SDMMC_CheckTuningResult(buffer, blockSize);
    /* abort tuning */
    USDHC_EnableManualTuning(base, false);

    /* enable auto tuning */
    USDHC_EnableAutoTuning(base, true);
    /* do not handle tuning error status, since the tuning error detect circuit is very sensetive which may drop down
     * the transfer performance */
    USDHC_DisableInterruptStatus(base, kUSDHC_TuningErrorFlag);

    return ret;
}

status_t SDMMCHOST_ExecuteTuning(sdmmchost_t *host, uint32_t tuningCmd, uint32_t *revBuf, uint32_t blockSize)
{
    if (host->tuningType == (uint32_t)kSDMMCHOST_StandardTuning)
    {
        return SDMMCHOST_ExecuteStdTuning(host, tuningCmd, revBuf, blockSize);
    }

    return SDMMCHOST_ExecuteManualTuning(host, tuningCmd, revBuf, blockSize);
}
#endif

status_t SDMMCHOST_StartBoot(sdmmchost_t *host,
                             sdmmchost_boot_config_t *hostConfig,
                             sdmmchost_cmd_t *cmd,
                             uint8_t *buffer)
{
    sdmmchost_transfer_t content = {0};
    sdmmchost_data_t data        = {0};

    USDHC_SetMmcBootConfig(host->hostController.base, hostConfig);

    data.blockSize  = hostConfig->blockSize;
    data.blockCount = hostConfig->blockCount;
    data.rxData     = (uint32_t *)buffer;
    data.dataType   = kUSDHC_TransferDataBoot;

    content.data    = &data;
    content.command = cmd;

    /* should check tuning error during every transfer*/
    if (kStatus_Success != SDMMCHOST_TransferFunction(host, &content))
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

    USDHC_SetMmcBootConfig(host->hostController.base, hostConfig);
    USDHC_EnableMmcBoot(host->hostController.base, true);

    data.blockSize  = hostConfig->blockSize;
    data.blockCount = hostConfig->blockCount;
    data.rxData     = (uint32_t *)buffer;
    data.dataType   = kUSDHC_TransferDataBootcontinous;
    /* no command should be send out  */
    command.type = kCARD_CommandTypeEmpty;

    content.data    = &data;
    content.command = &command;

    if (kStatus_Success != SDMMCHOST_TransferFunction(host, &content))
    {
        return kStatus_SDMMC_TransferFailed;
    }

    return kStatus_Success;
}
