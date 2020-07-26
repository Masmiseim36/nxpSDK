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
#define SDMMCHOST_TRANSFER_CMD_EVENT                                                                                   \
    SDMMC_OSA_EVENT_TRANSFER_CMD_SUCCESS | SDMMC_OSA_EVENT_TRANSFER_CMD_FAIL | SDMMC_OSA_EVENT_TRANSFER_DATA_SUCCESS | \
        SDMMC_OSA_EVENT_TRANSFER_DATA_FAIL
#define SDMMCHOST_TRANSFER_DATA_EVENT SDMMC_OSA_EVENT_TRANSFER_DATA_SUCCESS | SDMMC_OSA_EVENT_TRANSFER_DATA_FAIL
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief SDMMCHOST detect card insert status by host controller.
 * @param base host base address.
 * @param userData user can register a application card insert callback through userData.
 */
static void SDMMCHOST_DetectCardInsertByHost(USDHC_Type *base, void *userData);

/*!
 * @brief SDMMCHOST detect card remove status by host controller.
 * @param base host base address.
 * @param userData user can register a application card insert callback through userData.
 */
static void SDMMCHOST_DetectCardRemoveByHost(USDHC_Type *base, void *userData);

/*!
 * @brief SDMMCHOST transfer complete callback.
 * @param base host base address.
 * @param handle host handle.
 * @param status interrupt status.
 * @param userData user data.
 */
static void SDMMCHOST_TransferCompleteCallback(USDHC_Type *base,
                                               usdhc_handle_t *handle,
                                               status_t status,
                                               void *userData);

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
static void SDMMCHOST_DetectCardInsertByHost(USDHC_Type *base, void *userData)
{
    sd_detect_card_t *cd = NULL;

    SDMMC_OSAEventSet(((sdmmchost_t *)userData)->hostEvent, SDMMC_OSA_EVENT_CARD_INSERTED);

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

static void SDMMCHOST_DetectCardRemoveByHost(USDHC_Type *base, void *userData)
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

static void SDMMCHOST_CardInterrupt(USDHC_Type *base, void *userData)
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
    USDHC_Type *base       = host->hostController.base;
    sd_detect_card_t *sdCD = (sd_detect_card_t *)cd;
    if (cd == NULL)
    {
        return kStatus_Fail;
    }

    host->cd                           = cd;
    host->handle.callback.CardInserted = SDMMCHOST_DetectCardInsertByHost;
    host->handle.callback.CardRemoved  = SDMMCHOST_DetectCardRemoveByHost;
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
    USDHC_EnableInterruptSignal(base, kUSDHC_CardInsertionFlag);
    USDHC_EnableInterruptSignal(base, kUSDHC_CardRemovalFlag);

    if (SDMMCHOST_CardDetectStatus(host) == kSD_Inserted)
    {
        SDMMC_OSAEventSet(host->hostEvent, SDMMC_OSA_EVENT_CARD_INSERTED);
        /* notify application about the card insertion status */
        if (sdCD->callback)
        {
            sdCD->callback(true, sdCD->userData);
        }
    }
    else
    {
        SDMMC_OSAEventSet(host->hostEvent, SDMMC_OSA_EVENT_CARD_REMOVED);
    }

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
    uint32_t event       = 0U;

    SDMMC_OSAEventGet(host->hostEvent, SDMMC_OSA_EVENT_CARD_INSERTED | SDMMC_OSA_EVENT_CARD_REMOVED, &event);
    if ((((event & SDMMC_OSA_EVENT_CARD_INSERTED) == SDMMC_OSA_EVENT_CARD_INSERTED) &&
         (waitCardStatus == kSD_Inserted)) ||
        (((event & SDMMC_OSA_EVENT_CARD_REMOVED) == SDMMC_OSA_EVENT_CARD_REMOVED) && (waitCardStatus == kSD_Removed)))
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
                if (SDMMCHOST_CardDetectStatus(host) == kSD_Inserted)
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

    while ((USDHC_GetInterruptStatusFlags(hostBase) & (uint32_t)kUSDHC_CardInsertionFlag) != (uint32_t)waitCardStatus)
    {
    }

    return kStatus_Success;
}

static void SDMMCHOST_TransferCompleteCallback(USDHC_Type *base,
                                               usdhc_handle_t *handle,
                                               status_t status,
                                               void *userData)
{
    uint32_t eventStatus = 0U;

    if (status == kStatus_USDHC_TransferDataFailed)
    {
        eventStatus = SDMMC_OSA_EVENT_TRANSFER_DATA_FAIL;
    }
    else if (status == kStatus_USDHC_TransferDataComplete)
    {
        eventStatus = SDMMC_OSA_EVENT_TRANSFER_DATA_SUCCESS;
    }
    else if (status == kStatus_USDHC_SendCommandFailed)
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

    /* clear redundant transfer event flag */
    SDMMC_OSAEventClear(host->hostEvent, SDMMCHOST_TRANSFER_CMD_EVENT);

    do
    {
        error = USDHC_TransferNonBlocking(host->hostController.base, &host->handle, &dmaConfig, content);
    } while (error == kStatus_USDHC_BusyTransferring);

    if (error == kStatus_Success)
    {
        /* wait command event */
        if ((kStatus_Fail == SDMMC_OSAEventWait(host->hostEvent, SDMMCHOST_TRANSFER_CMD_EVENT,
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
                        (kStatus_Fail == SDMMC_OSAEventWait(host->hostEvent, SDMMCHOST_TRANSFER_DATA_EVENT,
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

    usdhc_transfer_callback_t usdhcCallback = {0};
    usdhc_host_t *usdhcHost                 = &(host->hostController);

    /* sdmmc osa init */
    SDMMC_OSAInit();

    /* Initializes USDHC. */
    usdhcHost->config.endianMode          = kUSDHC_EndianModeLittle;
    usdhcHost->config.dataTimeout         = 0xFU;
    usdhcHost->config.readWatermarkLevel  = 0x80U;
    usdhcHost->config.writeWatermarkLevel = 0x80U;
    USDHC_Init(usdhcHost->base, &(usdhcHost->config));

    /* Create handle for SDHC driver */
    usdhcCallback.TransferComplete = SDMMCHOST_TransferCompleteCallback;
    USDHC_TransferCreateHandle(usdhcHost->base, &host->handle, &usdhcCallback, host);

    /* Create transfer event. */
    if (kStatus_Success != SDMMC_OSAEventCreate(host->hostEvent))
    {
        return kStatus_Fail;
    }

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

    USDHC_DisableInterruptSignal(base, kUSDHC_AllInterruptFlags);
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
    uint32_t i              = 0U;
    const uint32_t *pattern = SDMMC_TuningBlockPattern4Bit;

    if (size == 128U)
    {
        pattern = SDMMC_TuningBlockPattern8Bit;
    }

    for (i = 0U; i < size / sizeof(uint32_t); i++)
    {
        if (pattern[i] != SWAP_WORD_BYTE_SEQUENCE(buffer[i]))
        {
#if SDMMC_ENABLE_LOG_PRINT
            SDMMC_LOG("tuning unmatch target: %x, read :%x\r\n", pattern[i], SWAP_WORD_BYTE_SEQUENCE(buffer[i]));
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

    USDHC_Reset(host->hostController.base, kUSDHC_ResetTuning, 100U);
    USDHC_EnableManualTuning(host->hostController.base, true);

    while (true)
    {
        USDHC_AdjustDelayForManualTuning(host->hostController.base, tuningDelayCell);

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
    USDHC_AdjustDelayForManualTuning(host->hostController.base, validDelayCellStart + validWindowCounter / 2U);
    /* send tuning block with the average delay cell */
    SDMMCHOST_ReceiveTuningBlock(host, tuningCmd, buffer, blockSize);
    ret = SDMMC_CheckTuningResult(buffer, blockSize);
    /* abort tuning */
    USDHC_EnableManualTuning(host->hostController.base, false);

    /* enable auto tuning */
    USDHC_EnableAutoTuning(host->hostController.base, true);

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
