/*
 * Copyright (c) 2015 Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <string.h>
#include "fsl_card.h"
#include "fsl_sdmmc.h"
#include "microseconds/microseconds.h"

/*******************************************************************************
 * Definitons
 ******************************************************************************/
/*! @brief The divide value used to avoid float point calculation when calculate max speed in normal mode. */
#define DIVIDER_IN_TRANSFER_SPEED (10U)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief Send SELECT_CARD command to set the card enter or exit transfer state.
 *
 * @param card Card descriptor.
 * @param isSelected True to enter transfer state.
 * @retval kStatus_SDMMC_TransferFailed Transfer failed.
 * @retval kStatus_Success Operate successfully.
 */
static status_t inline MMC_SelectCard(mmc_card_t *card, bool isSelected);

/*!
 * @brief Wait write process complete.
 *
 * @param card Card descriptor.
 * @retval kStatus_Timeout Operation timeout.
 * @retval kStatus_Success Operate successfully.
 */
static status_t MMC_WaitWriteComplete(mmc_card_t *card);

/*!
 * @brief Send SET_BLOCK_COUNT command.
 *
 * @param card Card descriptor.
 * @param blockCount Block count.
 * @retval kStatus_SDMMC_TransferFailed Transfer failed.
 * @retval kStatus_Success Operate successfully.
 */
static status_t inline MMC_SetBlockCount(mmc_card_t *card, uint32_t blockCount);

/*!
 * @brief Send GO_IDLE command to reset all cards to idle state
 *
 * @param card Card descriptor.
 * @retval kStatus_SDMMC_TransferFailed Transfer failed.
 * @retval kStatus_Success Operate successfully.
 */
static status_t inline MMC_GoIdle(mmc_card_t *card);

/*!
 * @brief Send STOP_TRANSMISSION command to card to stop ongoing data transferring.
 *
 * @param card Card descriptor.
 * @retval kStatus_SDMMC_TransferFailed Transfer failed.
 * @retval kStatus_Success Operate successfully.
 */
static status_t MMC_StopTransmission(mmc_card_t *card);

/*!
 * @brief Send SET_BLOCK_SIZE command to set the block length in bytes for MMC cards.
 *
 * @param card Card descriptor.
 * @param blockSize Block size.
 * @retval kStatus_SDMMC_TransferFailed Transfer failed.
 * @retval kStatus_Success Operate successfully.
 */
static status_t inline MMC_SetBlockSize(mmc_card_t *card, uint32_t blockSize);

/*!
 * @brief switch voltage.
 *
 * @param card Card descriptor.
 * @param opcode use to send operation condition
 * @retval kStatus_SDMMC_HostNotSupport Host doesn't support the voltage window to access the card.
 * @retval kStatus_Success Operate successfully.
 */
static status_t MMC_SwitchVoltage(mmc_card_t *card, uint32_t *opCode);

/*!
 * @brief Send SEND_OPERATION_CONDITION command to validate if the card support host's voltage window
 *
 * @param card Card descriptor.
 * @param arg Command argument.
 * @retval kStatus_SDMMC_TransferFailed Transfers failed.
 * @retval kStatus_Timeout Operation timeout.
 * @retval kStatus_Success Operate successfully.
 */
static status_t MMC_SendOperationCondition(mmc_card_t *card, uint32_t arg);

/*!
 * @brief Send SET_RCA command to set the relative address of the card.
 *
 * @param card Card descriptor.
 * @retval kStatus_SDMMC_TransferFailed Transfer failed.
 * @retval kStatus_Success Operate successfully.
 */
static status_t MMC_SetRelativeAddress(mmc_card_t *card);

/*!
 * @brief Decode CSD register content.
 *
 * @param card Card descriptor.
 * @param rawCsd raw CSD register content.
 */
static void MMC_DecodeCsd(mmc_card_t *card, uint32_t *rawCsd);

/*!
 * @brief Set the card to max transfer speed in non-high speed mode.
 *
 * @param card Card descriptor.
 */
static void MMC_SetMaxFrequency(mmc_card_t *card);

/*!
 * @brief Set erase unit size of the card
 *
 * @param card Card descriptor.
 * @retval kStatus_SDMMC_ConfigureExtendedCsdFailed Configure Extended CSD failed.
 * @retval kStatus_Success Operate successfully.
 */
static status_t MMC_SetMaxEraseUnitSize(mmc_card_t *card);

/*!
 * @brief Send SWITCH command to set the specific byte in Extended CSD.
 *
 * Example:
   @code
   mmc_extended_csd_config_t config;
   config.accessMode = kMMC_ExtendedCsdAccessModeSetBits;
   config.ByteIndex = 1U;
   config.ByteValue = 0x033U;
   config.commandSet = kMMC_CommandSetStandard;
   MMC_SetExtendedCsdConfig(card, &config);
   @endcode
 *
 * @param card Card descriptor.
 * @param config Configuration for Extended CSD.
 * @retval kStatus_SDMMC_TransferFailed Transfer failed.
 * @retval kStatus_SDMMC_WaitWriteCompleteFailed Wait write complete failed.
 * @retval kStatus_Success Operate successfully.
 */
static status_t MMC_SetExtendedCsdConfig(mmc_card_t *card, const mmc_extended_csd_config_t *config);

/*!
 * @brief Decode the Extended CSD register
 *
 * @param card Card descriptor.
 * @param rawExtendedCsd Raw extended CSD register content.
 */
static void MMC_DecodeExtendedCsd(mmc_card_t *card, uint32_t *rawExtendedCsd);

/*!
 * @brief Send SEND_EXTENDED_CSD command to get the content of the Extended CSD register
 * Allow read the special byte index value if targetAddr is not NULL
 * @param card Card descriptor.
 * @param targetAddr Pointer to store the target byte value.
 * @param byteIndex Target byte index.
 * @retval kStatus_SDMMC_TransferFailed Transfer failed.
 * @retval kStatus_Success Operate successfully.
 */
static status_t MMC_SendExtendedCsd(mmc_card_t *card, uint8_t *targetAddr, uint32_t byteIndex);

/*!
 * @brief Set the power class of the card at specific bus width and host intended voltage window.
 *
 * @param card Card descriptor.
 * @return The power class switch status.
 */
static status_t MMC_SetPowerClass(mmc_card_t *card);

/*!
 * @brief Send test pattern to get the functional pin in the MMC bus
 *
 * @param card Card descriptor.
 * @param blockSize Test pattern block size.
 * @param pattern Test pattern data buffer.
 * @retval kStatus_SDMMC_TransferFailed Transfer failed.
 * @retval kStatus_Success Operate successfully.
 */
static status_t MMC_SendTestPattern(mmc_card_t *card, uint32_t blockSize, uint32_t *pattern);

/*!
 * @brief Receive test pattern reversed by the card.
 *
 * @param card Card descriptor.
 * @param blockSize Test pattern block size.
 * @param pattern Test pattern data buffer.
 * @retval kStatus_SDMMC_TransferFailed Transfer failed.
 * @retval kStatus_Success Operate successfully.
 */
static status_t MMC_ReceiveTestPattern(mmc_card_t *card, uint32_t blockSize, uint32_t *pattern);

/*!
 * @brief Bus test procedure to get the functional data pin in the bus
 *
 * @param card Card descriptor.
 * @param width Data bus width.
 * @retval kStatus_SDMMC_SendTestPatternFailed Send test pattern failed.
 * @retval kStatus_SDMMC_ReceiveTestPatternFailed Receive test pattern failed.
 * @retval kStatus_Fail Test failed.
 * @retval kStatus_Success Operate successfully.
 */
static status_t MMC_TestDataBusWidth(mmc_card_t *card, mmc_data_bus_width_t width);

/*!
 * @brief Send SET_BUS_WIDTH command to set the bus width.
 *
 * @param card Card descriptor.
 * @param width Data bus width.
 * @param isDDR DDR timing flag.
 * @retval kStatus_SDMMC_ConfigureExtendedCsdFailed Configure extended CSD failed.
 * @retval kStatus_Success Operate successfully.
 */
static status_t MMC_SetDataBusWidth(mmc_card_t *card, mmc_data_bus_width_t width, bool isDDR);

/*!
 * @brief Set max the bus width automatically
 *
 * @param card Card descriptor.
 * @param isDDR DDR timing flag.
 * @retval kStatus_SDMMC_SetDataBusWidthFailed switch fail.
 * @retval kStatus_Success switch success.
 */
static status_t MMC_SetMaxDataBusWidth(mmc_card_t *card, bool isDDR);

/*!
 * @brief Switch the card to high speed mode
 *
 * @param card Card descriptor.
 * @retval kStatus_SDMMC_ConfigureExtendedCsdFailed Configure extended CSD failed.
 * @retval kStatus_SDMMC_CardNotSupport Card doesn't support high speed.
 * @retval kStatus_Success Operate successfully.
 */
static status_t MMC_SelectBusTiming(mmc_card_t *card);

/*!
 * @brief select card HS_TIMING value and card driver strength
 *
 * @param card Card descriptor.
 * @param timing Timing interface value.
 * @param driverStrength driver strength value.
 * @retval kStatus_Success switch success.
 * @retval kStatus_SDMMC_ConfigureExtendedCsdFailed , config extend csd register fail.
 */
static status_t MMC_SwitchHSTiming(mmc_card_t *card, uint8_t timing, uint8_t driverStrength);

/*!
 * @brief switch to HS400 mode.
 *
 * @param card Card descriptor.
 * @retval kStatus_SDMMC_ConfigureExtendedCsdFailed Configure extended CSD failed.
 * @retval kStatus_SDMMC_SwitchBusTimingFailed switch bus timing fail.
 * @retval kStatus_SDMMC_SetDataBusWidthFailed switch bus width fail.
 * @retval kStatus_Success Operate successfully.
 */
static status_t MMC_SwitchToHS400(mmc_card_t *card);

/*!
 * @brief switch to HS200 mode.
 *
 * @param card Card descriptor.
 * @param freq Target frequency.
 * @retval kStatus_SDMMC_ConfigureExtendedCsdFailed Configure extended CSD failed.
 * @retval kStatus_SDMMC_TuningFail tuning fail.
 * @retval kStatus_SDMMC_SetDataBusWidthFailed switch bus width fail.
 * @retval kStatus_Success Operate successfully.
 */
static status_t MMC_SwitchToHS200(mmc_card_t *card, uint32_t freq);

/*!
 * @brief switch to HS400 mode.
 *
 * @param card Card descriptor.
 * @param freq Target frequency.
 * @retval kStatus_SDMMC_ConfigureExtendedCsdFailed Configure extended CSD failed.
 * @retval kStatus_SDMMC_SetDataBusWidthFailed switch bus width fail.
 * @retval kStatus_Success Operate successfully.
 */
static status_t MMC_SwitchToHighSpeed(mmc_card_t *card, uint32_t freq);

/*!
 * @brief Decode CID register
 *
 * @param card Card descriptor.
 * @param rawCid Raw CID register content.
 */
static void MMC_DecodeCid(mmc_card_t *card, uint32_t *rawCid);

/*!
 * @brief Send ALL_SEND_CID command
 *
 * @param card Card descriptor.
 * @retval kStatus_SDMMC_TransferFailed Transfer failed.
 * @retval kStatus_Success Operate successfully.
 */
static status_t MMC_AllSendCid(mmc_card_t *card);

/*!
 * @brief Send SEND_CSD command to get CSD from card
 *
 * @param card Card descriptor.
 * @retval kStatus_SDMMC_TransferFailed Transfer failed.
 * @retval kStatus_Success Operate successfully.
 */
static status_t MMC_SendCsd(mmc_card_t *card);

/*!
 * @brief Check if the block range accessed is within current partition.
 *
 * @param card Card descriptor.
 * @param startBlock Start block to access.
 * @param blockCount Block count to access.
 * @retval kStatus_InvalidArgument Invalid argument.
 * @retval kStatus_Success Operate successfully.
 */
static status_t MMC_CheckBlockRange(mmc_card_t *card, uint32_t startBlock, uint32_t blockCount);

/*!
 * @brief Check if the erase group range accessed is within current partition.
 *
 * @param card Card descriptor.
 * @param startGroup Start group to access.
 * @param endGroup End group to access.
 * @retval kStatus_InvalidArgument Invalid argument.
 * @retval kStatus_Success Operate successfully.
 */
static status_t MMC_CheckEraseGroupRange(mmc_card_t *card, uint32_t startGroup, uint32_t endGroup);

/*!
 * @brief MMC excute tuning function.
 *
 * @param card Card descriptor.
 * @retval kStatus_Success Operate successfully.
 * @retval kStatus_SDMMC_TuningFail tuning fail.
 * @retval kStatus_SDMMC_TransferFailed transfer fail
 */
static status_t inline MMC_ExecuteTuning(mmc_card_t *card);

/*!
 * @brief Read data from specific MMC card
 *
 * @param card Card descriptor.
 * @param buffer Buffer to save received data.
 * @param startBlock Start block to read.
 * @param blockSize Block size.
 * @param blockCount Block count to read.
 * @retval kStatus_SDMMC_CardNotSupport Card doesn't support.
 * @retval kStatus_SDMMC_WaitWriteCompleteFailed Wait write complete failed.
 * @retval kStatus_SDMMC_SetBlockCountFailed Set block count failed.
 * @retval kStatus_SDMMC_TransferFailed Transfer failed.
 * @retval kStatus_SDMMC_StopTransmissionFailed Stop transmission failed.
 * @retval kStatus_Success Operate successfully.
 */
static status_t MMC_Read(
    mmc_card_t *card, uint8_t *buffer, uint32_t startBlock, uint32_t blockSize, uint32_t blockCount);

/*!
 * @brief Write data from specific MMC card
 *
 * @param card Card descriptor.
 * @param buffer Buffer to hold the data to write.
 * @param startBlock Start block to write.
 * @param blockSize Block size.
 * @param blockCount Block count to write.
 * @retval kStatus_SDMMC_CardNotSupport Card doesn't support.
 * @retval kStatus_SDMMC_SetBlockCountFailed Set block count failed.
 * @retval kStatus_SDMMC_TransferFailed Transfer failed.
 * @retval kStatus_SDMMC_StopTransmissionFailed Stop transmission failed.
 * @retval kStatus_Success Operate successfully.
 */
static status_t MMC_Write(
    mmc_card_t *card, const uint8_t *buffer, uint32_t startBlock, uint32_t blockSize, uint32_t blockCount);

/*!
 * @brief card transfer function wrapper
 * This function is used to do tuning before transfer if the cmd won't casue re-tuning
 * request, then you can call host transfer function directly
 * @param card Card descriptor.
 * @param content Transfer content.
 * @param retry Retry times.
 * @retval kStatus_SDMMC_TransferFailed transfer fail
 * @retval kStatus_SDMMC_TuningFail tuning fail
 * @retval kStatus_Success transfer success
 */
static status_t MMC_Transfer(mmc_card_t *card, HOST_TRANSFER *content, uint32_t retry);

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* Frequency unit defined in TRANSFER SPEED field in CSD */
static const uint32_t g_transerSpeedFrequencyUnit[] = { 100000U, 1000000U, 10000000U, 100000000U };
/* The multiplying value defined in TRANSFER SPEED field in CSD */
static const uint32_t g_transerSpeedMultiplierFactor[] = { 0U,  10U, 12U, 13U, 15U, 20U, 26U, 30U,
                                                           35U, 40U, 45U, 52U, 55U, 60U, 70U, 80U };
/* g_sdmmc statement */
extern uint32_t g_sdmmc[SDK_SIZEALIGN(SDMMC_GLOBAL_BUFFER_SIZE, SDMMC_DATA_BUFFER_ALIGN_CAHCE)];

/*******************************************************************************
 * Code
 ******************************************************************************/
static status_t inline MMC_SelectCard(mmc_card_t *card, bool isSelected)
{
    assert(card);

    return SDMMC_SelectCard(card->host.base, card->host.transfer, card->relativeAddress, isSelected);
}

static status_t inline MMC_SetBlockCount(mmc_card_t *card, uint32_t blockCount)
{
    assert(card);

    return SDMMC_SetBlockCount(card->host.base, card->host.transfer, blockCount);
}

static status_t inline MMC_GoIdle(mmc_card_t *card)
{
    assert(card);

    return SDMMC_GoIdle(card->host.base, card->host.transfer);
}

static status_t inline MMC_SetBlockSize(mmc_card_t *card, uint32_t blockSize)
{
    assert(card);

    return SDMMC_SetBlockSize(card->host.base, card->host.transfer, blockSize);
}

static status_t MMC_ExecuteTuning(mmc_card_t *card)
{
    assert(card);

    uint32_t blockSize = 0U;

    if (card->flags & kMMC_DataBusWidth4BitFlag)
    {
        blockSize = 64U;
    }
    else if (card->flags & kMMC_DataBusWidth8BitFlag)
    {
        blockSize = 128U;
    }
    else
    {
        /* do not need tuning in this situation */
        return kStatus_Success;
    }

    return SDMMC_ExecuteTuning(card->host.base, card->host.transfer, kMMC_SendTuningBlock, blockSize);
}

static status_t MMC_Transfer(mmc_card_t *card, HOST_TRANSFER *content, uint32_t retry)
{
    assert(card->host.transfer);
    assert(content);
    status_t error;

    do
    {
        error = card->host.transfer(card->host.base, content);
        if (((error == HOST_RETUNING_REQUEST) || (error == HOST_TUNING_ERROR)) &&
            ((card->currentTiming == kMMC_HighSpeed200Timing) || (card->currentTiming == kMMC_HighSpeed400Timing)))
        {
            /* tuning error need reset tuning circuit */
            if (error == HOST_TUNING_ERROR)
            {
                HOST_RESET_TUNING(card->host.base, 100U);
            }
            /* execute re-tuning */
            if (MMC_ExecuteTuning(card) != kStatus_Success)
            {
                error = kStatus_SDMMC_TuningFail;
                break;
            }
            /* Avoid the infinite loop when re-tuning success but transfer failed.*/
            // else
            //{
            //    continue;
            //}
        }
        else if (error != kStatus_Success)
        {
            error = kStatus_SDMMC_TransferFailed;
        }
        else
        {
        }

        if (retry != 0U)
        {
            retry--;
        }
        else
        {
            break;
        }

    } while ((error != kStatus_Success) && (error != kStatus_SDMMC_TuningFail));

    return error;
}

static status_t MMC_WaitWriteComplete(mmc_card_t *card)
{
    assert(card);

    HOST_TRANSFER content = { 0 };
    HOST_COMMAND command = { 0 };

    command.index = kSDMMC_SendStatus;
    command.argument = card->relativeAddress << 16U;
    command.responseType = kCARD_ResponseTypeR1;

    do
    {
        content.command = &command;
        content.data = 0U;
        if (kStatus_Success != MMC_Transfer(card, &content, 0U))
        {
            return kStatus_SDMMC_TransferFailed;
        }

        /* check the response error */
        if ((command.response[0U] & (kSDMMC_R1ErrorAllFlag | kSDMMC_R1SwitchErrorFlag)))
        {
            return kStatus_SDMMC_WaitWriteCompleteFailed;
        }

        if ((command.response[0U] & kSDMMC_R1ReadyForDataFlag) &&
            (SDMMC_R1_CURRENT_STATE(command.response[0U]) != kSDMMC_R1StateProgram))
        {
            break;
        }
    } while (true);

    return kStatus_Success;
}

static status_t MMC_StopTransmission(mmc_card_t *card)
{
    assert(card);

    HOST_TRANSFER content = { 0 };
    HOST_COMMAND command = { 0 };

    command.index = kSDMMC_StopTransmission;
    command.argument = 0U;
    command.type = kCARD_CommandTypeAbort;
    command.responseType = kCARD_ResponseTypeR1b;
    command.responseErrorFlags = kSDMMC_R1ErrorAllFlag;

    content.command = &command;
    content.data = 0U;
    if (kStatus_Success != MMC_Transfer(card, &content, 2U))
    {
        return kStatus_SDMMC_TransferFailed;
    }

    return kStatus_Success;
}

static status_t MMC_SwitchVoltage(mmc_card_t *card, uint32_t *opCode)
{
    mmc_voltage_window_t tempVoltage;
    /* Get host's voltage window. */
    if (((kHOST_SupportV330 != HOST_NOT_SUPPORT) || (kHOST_SupportV300 != HOST_NOT_SUPPORT)) &&
        (card->ocr & MMC_OCR_V270TO360_MASK) && ((card->hostVoltageWindowVCC == kMMC_VoltageWindowNone) ||
                                                 (card->hostVoltageWindowVCC == kMMC_VoltageWindows270to360)))
    {
        /* Save host intended voltage range */
        tempVoltage = kMMC_VoltageWindows270to360;
        /* set the opcode */
        *opCode = MMC_OCR_V270TO360_MASK;
        /* power off the card first */
        HOST_ENABLE_MMC_POWER(false);
        /* power off time */
        SDMMC_Delay(1U);
        /*switch voltage to 3.3V*/
        HOST_SWITCH_VCC_TO_330V();
        /* repower the card */
        HOST_ENABLE_MMC_POWER(true);
        /* meet emmc spec, wait 1ms and 74 clocks */
        SDMMC_Delay(2U);
    }

    if ((kHOST_SupportV180 != HOST_NOT_SUPPORT) && (card->ocr & MMC_OCR_V170TO195_MASK) &&
        ((card->hostVoltageWindowVCC == kMMC_VoltageWindowNone) ||
         (card->hostVoltageWindowVCC == kMMC_VoltageWindow170to195)))
    {
        /* Save host intended voltage range */
        tempVoltage = kMMC_VoltageWindow170to195;
        /* set the opcode */
        *opCode = MMC_OCR_V170TO195_MASK;
        /* power off the card first */
        HOST_ENABLE_MMC_POWER(false);
        /* power off time */
        SDMMC_Delay(1U);
        /* switch voltage to 1.8V */
        HOST_SWITCH_VCC_TO_180V();
        /* repower the card */
        HOST_ENABLE_MMC_POWER(true);
        /* meet emmc spec, wait 1ms and 74 clocks */
        SDMMC_Delay(2U);
    }

    card->hostVoltageWindowVCC = tempVoltage;

    return kStatus_Success;
}

static status_t MMC_SendOperationCondition(mmc_card_t *card, uint32_t arg)
{
    assert(card);
    assert(card->host.transfer);

    HOST_COMMAND command = { 0 };
    HOST_TRANSFER content = { 0 };
    status_t error;
    uint32_t i = FSL_SDMMC_MAX_VOLTAGE_RETRIES;

    /* Send CMD1 with the intended voltage range in the argument(either 0x00FF8000 or 0x00000080) */
    command.index = kMMC_SendOperationCondition;
    command.argument = arg;
    command.responseType = kCARD_ResponseTypeR3;

    content.command = &command;
    content.data = NULL;
    do
    {
        if (kStatus_Success != card->host.transfer(card->host.base, &content))
        {
            return kStatus_SDMMC_TransferFailed;
        }

        if (arg == 0U)
        {
            error = kStatus_Success;
            card->ocr = command.response[0U];
        }
        /* Repeat CMD1 until the busy bit is cleared. */
        else if (!(command.response[0U] & MMC_OCR_BUSY_MASK))
        {
            error = kStatus_Timeout;
        }
        else
        {
            error = kStatus_Success;
        }
    } while ((i--) && (error != kStatus_Success));

    return error;
}

static status_t MMC_BL_SendOperationCondition(mmc_card_t *card, uint32_t arg)
{
    assert(card);
    assert(card->host.transfer);

    HOST_COMMAND command = { 0 };
    HOST_TRANSFER content = { 0 };
    status_t error;
    uint32_t i = FSL_SDMMC_MAX_VOLTAGE_RETRIES;

    /* Send CMD1 with the intended voltage range in the argument(either 0x00FF8000 or 0x00000080) */
    command.index = kMMC_SendOperationCondition;
    command.argument = arg;
    command.responseType = kCARD_ResponseTypeR3;

    content.command = &command;
    content.data = NULL;

    uint64_t timeoutTicks = microseconds_get_ticks() + microseconds_convert_to_ticks(SDMMC_WAIT_BUSY_TIMEOUT_US);

    do
    {
        if (kStatus_Success != card->host.transfer(card->host.base, &content))
        {
            return kStatus_SDMMC_TransferFailed;
        }

        if (arg == 0U)
        {
            error = kStatus_Success;
            card->ocr = command.response[0U];
        }
        /* Repeat CMD1 until the busy bit is cleared. */
        else if (!(command.response[0U] & MMC_OCR_BUSY_MASK))
        {
            error = kStatus_Timeout;
        }
        else
        {
            error = kStatus_Success;
        }
    } while ((i--) && (error != kStatus_Success) && (microseconds_get_ticks() < timeoutTicks));

    return error;
}

static status_t MMC_SetRelativeAddress(mmc_card_t *card)
{
    assert(card);
    assert(card->host.transfer);

    HOST_COMMAND command = { 0 };
    HOST_TRANSFER content = { 0 };

    /* Send CMD3 with a chosen relative address, with value greater than 1 */
    command.index = kMMC_SetRelativeAddress;
    command.argument = (MMC_DEFAULT_RELATIVE_ADDRESS << 16U);
    command.responseType = kCARD_ResponseTypeR1;

    content.command = &command;
    content.data = NULL;
    if ((kStatus_Success == card->host.transfer(card->host.base, &content)) ||
        (!((command.response[0U]) & kSDMMC_R1ErrorAllFlag)))
    {
        card->relativeAddress = MMC_DEFAULT_RELATIVE_ADDRESS;
        return kStatus_Success;
    }

    return kStatus_SDMMC_TransferFailed;
}

static void MMC_DecodeCsd(mmc_card_t *card, uint32_t *rawCsd)
{
    assert(card);
    assert(rawCsd);

    mmc_csd_t *csd;
    uint32_t multiplier;

    csd = &(card->csd);
    csd->csdStructureVersion = (uint8_t)((rawCsd[3U] & 0xC0000000U) >> 30U);
    csd->systemSpecificationVersion = (uint8_t)((rawCsd[3U] & 0x3C000000U) >> 26U);
    csd->dataReadAccessTime1 = (uint8_t)((rawCsd[3U] & 0xFF0000U) >> 16U);
    csd->dataReadAccessTime2 = (uint8_t)((rawCsd[3U] & 0xFF00U) >> 8U);
    csd->transferSpeed = (uint8_t)(rawCsd[3U] & 0xFFU);
    csd->cardCommandClass = (uint16_t)((rawCsd[2U] & 0xFFF00000U) >> 20U);
    /* Max block length read/write one time */
    csd->readBlockLength = (uint8_t)((rawCsd[2U] & 0xF0000U) >> 16U);
    if (rawCsd[2U] & 0x8000U)
    {
        csd->flags |= kMMC_CsdReadBlockPartialFlag;
    }
    if (rawCsd[2U] & 0x4000U)
    {
        csd->flags |= kMMC_CsdWriteBlockMisalignFlag;
    }
    if (rawCsd[2U] & 0x2000U)
    {
        csd->flags |= kMMC_CsdReadBlockMisalignFlag;
    }
    if (rawCsd[2U] & 0x1000U)
    {
        csd->flags |= kMMC_CsdDsrImplementedFlag;
    }
    csd->deviceSize = (uint16_t)(((rawCsd[2U] & 0x3FFU) << 2U) + ((rawCsd[1U] & 0xC0000000U) >> 30U));
    csd->readCurrentVddMin = (uint8_t)((rawCsd[1U] & 0x38000000U) >> 27U);
    csd->readCurrentVddMax = (uint8_t)((rawCsd[1U] & 0x07000000U) >> 24U);
    csd->writeCurrentVddMin = (uint8_t)((rawCsd[1U] & 0x00E00000U) >> 21U);
    csd->writeCurrentVddMax = (uint8_t)((rawCsd[1U] & 0x001C0000U) >> 18U);
    csd->deviceSizeMultiplier = (uint8_t)((rawCsd[1U] & 0x00038000U) >> 15U);
    csd->eraseGroupSize = (uint8_t)((rawCsd[1U] & 0x00007C00U) >> 10U);
    csd->eraseGroupSizeMultiplier = (uint8_t)((rawCsd[1U] & 0x000003E0U) >> 5U);
    csd->writeProtectGroupSize = (uint8_t)(rawCsd[1U] & 0x0000001FU);
    if (rawCsd[0U] & 0x80000000U)
    {
        csd->flags |= kMMC_CsdWriteProtectGroupEnabledFlag;
    }
    csd->defaultEcc = (uint8_t)((rawCsd[0U] & 0x60000000U) >> 29U);
    csd->writeSpeedFactor = (uint8_t)((rawCsd[0U] & 0x1C000000U) >> 26U);
    csd->maxWriteBlockLength = (uint8_t)((rawCsd[0U] & 0x03C00000U) >> 22U);
    if (rawCsd[0U] & 0x00200000U)
    {
        csd->flags |= kMMC_CsdWriteBlockPartialFlag;
    }
    if (rawCsd[0U] & 0x00010000U)
    {
        csd->flags |= kMMC_ContentProtectApplicationFlag;
    }
    if (rawCsd[0U] & 0x00008000U)
    {
        csd->flags |= kMMC_CsdFileFormatGroupFlag;
    }
    if (rawCsd[0U] & 0x00004000U)
    {
        csd->flags |= kMMC_CsdCopyFlag;
    }
    if (rawCsd[0U] & 0x00002000U)
    {
        csd->flags |= kMMC_CsdPermanentWriteProtectFlag;
    }
    if (rawCsd[0U] & 0x00001000U)
    {
        csd->flags |= kMMC_CsdTemporaryWriteProtectFlag;
    }
    csd->fileFormat = (uint8_t)((rawCsd[0U] & 0x00000C00U) >> 10U);
    csd->eccCode = (uint8_t)((rawCsd[0U] & 0x00000300U) >> 8U);

    /* Calculate the device total block count. */
    /* For the card capacity of witch higher than 2GB, the maximum possible value should be set to this register
    is 0xFFF. */
    if (card->csd.deviceSize != 0xFFFU)
    {
        multiplier = (2U << (card->csd.deviceSizeMultiplier + 2U - 1U));
        card->userPartitionBlocks = (((card->csd.deviceSize + 1U) * multiplier) / FSL_SDMMC_DEFAULT_BLOCK_SIZE);
    }
    else
    {
        card->flags |= kMMC_SupportHighCapacityFlag;
    }
    card->blockSize = FSL_SDMMC_DEFAULT_BLOCK_SIZE;
}

static void MMC_SetMaxFrequency(mmc_card_t *card)
{
    assert(card);

    uint32_t frequencyUnit;
    uint32_t multiplierFactor;
    uint32_t maxBusClock_Hz;

    /* g_fsdhcCommandUnitInTranSpeed and g_transerSpeedMultiplierFactor are used to calculate the max speed in normal
    mode not high speed mode.
    For cards supporting version 4.0, 4.1, and 4.2 of the specification, the value shall be 20MHz(0x2A).
    For cards supporting version 4.3, the value shall be 26 MHz (0x32H). In High speed mode, the max
    frequency is decided by CARD_TYPE in Extended CSD. */
    frequencyUnit = g_transerSpeedFrequencyUnit[READ_MMC_TRANSFER_SPEED_FREQUENCY_UNIT(card->csd)];
    multiplierFactor = g_transerSpeedMultiplierFactor[READ_MMC_TRANSFER_SPEED_MULTIPLIER(card->csd)];
    maxBusClock_Hz = (frequencyUnit * multiplierFactor) / DIVIDER_IN_TRANSFER_SPEED;
    card->busClock_Hz = HOST_SET_CARD_CLOCK(card->host.base, card->host.sourceClock_Hz, maxBusClock_Hz);
}

static status_t MMC_SetMaxEraseUnitSize(mmc_card_t *card)
{
    assert(card);

    uint32_t erase_group_size;
    uint32_t erase_group_multiplier;
    mmc_extended_csd_config_t extendedCsdconfig;

    if (((!(card->flags & kMMC_SupportHighCapacityFlag)) || (card->extendedCsd.highCapacityEraseUnitSize == 0)) ||
        (card->extendedCsd.highCapacityEraseTimeout == 0))
    {
        erase_group_size = card->csd.eraseGroupSize;
        erase_group_multiplier = card->csd.eraseGroupSizeMultiplier;
        card->eraseGroupBlocks = ((erase_group_size + 1U) * (erase_group_multiplier + 1U));
    }
    else
    {
        /* Erase Unit Size = 512Kbyte * HC_ERASE_GRP_SIZE. Block size is 512 bytes. */
        card->eraseGroupBlocks = (card->extendedCsd.highCapacityEraseUnitSize * 1024U);
        /* Enable high capacity erase unit size. */
        extendedCsdconfig.accessMode = kMMC_ExtendedCsdAccessModeSetBits;
        extendedCsdconfig.ByteIndex = kMMC_ExtendedCsdIndexEraseGroupDefinition;
        extendedCsdconfig.ByteValue = 0x01U; /* The high capacity erase unit size enable bit is bit 0 */
        extendedCsdconfig.commandSet = kMMC_CommandSetStandard;
        if (kStatus_Success != MMC_SetExtendedCsdConfig(card, &extendedCsdconfig))
        {
            return kStatus_SDMMC_ConfigureExtendedCsdFailed;
        }
    }

    return kStatus_Success;
}

static status_t MMC_SetExtendedCsdConfig(mmc_card_t *card, const mmc_extended_csd_config_t *config)
{
    assert(card);
    assert(card->host.transfer);
    assert(config);

    uint32_t parameter = 0U;
    HOST_COMMAND command = { 0 };
    HOST_TRANSFER content = { 0 };

    parameter |= ((uint32_t)(config->commandSet) << MMC_SWITCH_COMMAND_SET_SHIFT);
    parameter |= ((uint32_t)(config->ByteValue) << MMC_SWITCH_VALUE_SHIFT);
    parameter |= ((uint32_t)(config->ByteIndex) << MMC_SWITCH_BYTE_INDEX_SHIFT);
    parameter |= ((uint32_t)(config->accessMode) << MMC_SWITCH_ACCESS_MODE_SHIFT);

    command.index = kMMC_Switch;
    command.argument = parameter;
    command.responseType = kCARD_ResponseTypeR1b; /* Send switch command to set the pointed byte in Extended CSD. */
    command.responseErrorFlags = kSDMMC_R1ErrorAllFlag | kSDMMC_R1SwitchErrorFlag;

    content.command = &command;
    content.data = NULL;
    if (kStatus_Success != MMC_Transfer(card, &content, 2U))
    {
        return kStatus_SDMMC_TransferFailed;
    }

    /* Wait for the card write process complete because of that card read process and write process use one buffer. */
    if (kStatus_Success != MMC_WaitWriteComplete(card))
    {
        return kStatus_SDMMC_WaitWriteCompleteFailed;
    }

    return kStatus_Success;
}

static void MMC_DecodeExtendedCsd(mmc_card_t *card, uint32_t *rawExtendedCsd)
{
    assert(card);
    assert(rawExtendedCsd);

    uint8_t *buffer = (uint8_t *)rawExtendedCsd;
    mmc_extended_csd_t *extendedCsd = &(card->extendedCsd);

    /* Extended CSD is transferred as a data block from least byte indexed 0. */
    card->generalPartitionSize[0] =
        (64 * 1024 * buffer[145U] + 256 * buffer[144U] + 1 * buffer[143U]) * buffer[221U] * buffer[224U];
    card->generalPartitionSize[1] =
        (64 * 1024 * buffer[148U] + 256 * buffer[147U] + 1 * buffer[146U]) * buffer[221U] * buffer[224U];
    card->generalPartitionSize[2] =
        (64 * 1024 * buffer[151U] + 256 * buffer[150U] + 1 * buffer[149U]) * buffer[221U] * buffer[224U];
    card->generalPartitionSize[3] =
        (64 * 1024 * buffer[154U] + 256 * buffer[153U] + 1 * buffer[152U]) * buffer[221U] * buffer[224U];
    extendedCsd->highDensityEraseGroupDefinition = buffer[175U];
    extendedCsd->bootDataBusWidth = buffer[177U];
    extendedCsd->partitionConfig = buffer[179U];
    extendedCsd->eraseMemoryContent = buffer[181U];
    extendedCsd->dataBusWidth = buffer[183U];
    extendedCsd->highSpeedTiming = buffer[185U];
    extendedCsd->powerClass = buffer[187U];
    extendedCsd->commandSetRevision = buffer[189U];
    extendedCsd->commandSet = buffer[191U];
    extendedCsd->extendecCsdVersion = buffer[192U];
    extendedCsd->csdStructureVersion = buffer[194U];
    extendedCsd->partitionAttribute = buffer[156U];
    extendedCsd->extPartitionSupport = buffer[494U];
    extendedCsd->cardType = buffer[196U];
    /* This field define the type of the card. The only currently valid values for this field are 0x01 and 0x03. */
    card->flags |= extendedCsd->cardType;

    extendedCsd->ioDriverStrength = buffer[197U];

    extendedCsd->powerClass52MHz195V = buffer[200U];
    extendedCsd->powerClass26MHz195V = buffer[201U];
    extendedCsd->powerClass52MHz360V = buffer[202U];
    extendedCsd->powerClass26MHz360V = buffer[203U];
    extendedCsd->powerClass200MHZVCCQ130VVCC360V = buffer[236U];
    extendedCsd->powerClass200MHZVCCQ195VVCC360V = buffer[237U];
    extendedCsd->powerClass52MHZDDR195V = buffer[238U];
    extendedCsd->powerClass52MHZDDR360V = buffer[239U];
    extendedCsd->powerClass200MHZDDR360V = buffer[253U];
    extendedCsd->minimumReadPerformance4Bit26MHz = buffer[205U];
    extendedCsd->minimumWritePerformance4Bit26MHz = buffer[206U];
    extendedCsd->minimumReadPerformance8Bit26MHz4Bit52MHz = buffer[207U];
    extendedCsd->minimumWritePerformance8Bit26MHz4Bit52MHz = buffer[208U];
    extendedCsd->minimumReadPerformance8Bit52MHz = buffer[209U];
    extendedCsd->minimumWritePerformance8Bit52MHz = buffer[210U];
    extendedCsd->minReadPerformance8bitAt52MHZDDR = buffer[234U];
    extendedCsd->minWritePerformance8bitAt52MHZDDR = buffer[235U];
    /* Get user partition size. */
    extendedCsd->sectorCount = ((((uint32_t)buffer[215U]) << 24U) + (((uint32_t)buffer[214U]) << 16U) +
                                (((uint32_t)buffer[213U]) << 8U) + (uint32_t)buffer[212U]);
    if (card->flags & kMMC_SupportHighCapacityFlag)
    {
        card->userPartitionBlocks = card->extendedCsd.sectorCount;
    }

    extendedCsd->sleepAwakeTimeout = buffer[217U];
    extendedCsd->sleepCurrentVCCQ = buffer[219U];
    extendedCsd->sleepCurrentVCC = buffer[220U];
    extendedCsd->highCapacityWriteProtectGroupSize = buffer[221U];
    extendedCsd->reliableWriteSectorCount = buffer[222U];
    extendedCsd->highCapacityEraseTimeout = buffer[223U];
    extendedCsd->highCapacityEraseUnitSize = buffer[224U];
    extendedCsd->accessSize = buffer[225U];

    /* Get boot partition size. */
    extendedCsd->bootSizeMultiplier = buffer[226U];
    card->bootPartitionBlocks = ((128U * 1024U * extendedCsd->bootSizeMultiplier) / FSL_SDMMC_DEFAULT_BLOCK_SIZE);

    /* Check if card support alternate boot. */
    extendedCsd->bootInformation = buffer[228U];
    if (extendedCsd->bootInformation & kMMC_SupportAlternateBoot)
    {
        card->flags |= kMMC_SupportAlternateBootFlag;
    }
    else if (extendedCsd->bootInformation & kMMC_SupportDDRBootFlag)
    {
        card->flags |= kMMC_SupportDDRBootFlag;
    }
    else if (extendedCsd->bootInformation & kMMC_SupportHighSpeedBoot)
    {
        card->flags |= kMMC_SupportHighSpeedBootFlag;
    }
    else
    {
    }
    /* cache size unit 1kb */
    extendedCsd->cacheSize = (((uint32_t)buffer[252U]) << 24) | (((uint32_t)buffer[251U]) << 16) |
                             (((uint32_t)buffer[250U]) << 8) | (((uint32_t)buffer[249U]));

    extendedCsd->supportedCommandSet = buffer[504U];
}

static status_t MMC_SendExtendedCsd(mmc_card_t *card, uint8_t *targetAddr, uint32_t byteIndex)
{
    assert(card);
    assert(card->host.transfer);

    HOST_COMMAND command = { 0 };
    HOST_TRANSFER content = { 0 };
    HOST_DATA data = { 0 };
    uint32_t i;

    command.index = kMMC_SendExtendedCsd;
    command.argument = 0U;
    command.responseType = kCARD_ResponseTypeR1;

    data.blockCount = 1U;
    data.blockSize = MMC_EXTENDED_CSD_BYTES;
    data.rxData = card->rawExtendedCsd;

    content.command = &command;
    content.data = &data;
    if ((kStatus_Success == card->host.transfer(card->host.base, &content)) &&
        (!(command.response[0U] & kSDMMC_R1ErrorAllFlag)))
    {
        /* The response is from bit 127:8 in R2, corresponding to command.response[3][31:0] to
        command.response[0U][31:8] */
        switch (card->host.config.endianMode)
        {
            case kHOST_EndianModeLittle:
                /* Doesn't need to switch byte sequence when decode bytes as little endian sequence. */
                break;
            case kHOST_EndianModeBig:
                /* In big endian mode, the SD bus byte transferred first is the byte stored in highest position
                in a word which cause 4 byte's sequence in a word is not consistent with their original sequence
                from card. */
                for (i = 0U; i < MMC_EXTENDED_CSD_BYTES / 4U; i++)
                {
                    card->rawExtendedCsd[i] = SWAP_WORD_BYTE_SEQUENCE(card->rawExtendedCsd[i]);
                }
                break;
            case kHOST_EndianModeHalfWordBig:
                for (i = 0U; i < MMC_EXTENDED_CSD_BYTES / 4U; i++)
                {
                    card->rawExtendedCsd[i] = SWAP_HALF_WROD_BYTE_SEQUENCE(card->rawExtendedCsd[i]);
                }
                break;
            default:
                return kStatus_SDMMC_NotSupportYet;
        }
        if (targetAddr != NULL)
        {
            *targetAddr = ((uint8_t *)card->rawExtendedCsd)[byteIndex];
        }
        else
        {
            MMC_DecodeExtendedCsd(card, card->rawExtendedCsd);
        }

        return kStatus_Success;
    }

    return kStatus_SDMMC_TransferFailed;
}

static status_t MMC_SetPowerClass(mmc_card_t *card)
{
    assert(card);

    uint8_t mask = 0, shift = 0U;
    uint8_t powerClass = 0;
    mmc_extended_csd_config_t extendedCsdconfig;

    if (card->flags & kMMC_DataBusWidth4BitFlag)
    {
        mask = MMC_POWER_CLASS_4BIT_MASK; /* The mask of 4 bit bus width's power class */
        shift = 0U;
    }
    else if (card->flags & kMMC_DataBusWidth8BitFlag)
    {
        mask = MMC_POWER_CLASS_8BIT_MASK; /* The mask of 8 bit bus width's power class */
        shift = 4U;
    }
    else
    {
        return kStatus_Success;
    }

    switch (card->hostVoltageWindowVCC)
    {
        case kMMC_VoltageWindows270to360:

            if (card->currentTiming == kMMC_HighSpeed200Timing)
            {
                if (card->hostVoltageWindowVCCQ == kMMC_VoltageWindow170to195)
                {
                    powerClass = ((card->extendedCsd.powerClass200MHZVCCQ195VVCC360V) & mask);
                }
                else if (card->hostVoltageWindowVCCQ == kMMC_VoltageWindow120)
                {
                    powerClass = ((card->extendedCsd.powerClass200MHZVCCQ130VVCC360V) & mask);
                }
            }
            else if (card->currentTiming == kMMC_HighSpeed400Timing)
            {
                powerClass = ((card->extendedCsd.powerClass200MHZDDR360V) & mask);
            }
            else if (card->currentTiming == kMMC_HighSpeedDDR52Timing)
            {
                powerClass = ((card->extendedCsd.powerClass52MHZDDR360V) & mask);
            }
            else if (card->currentTiming == kMMC_HighSpeed52MHZTiming)
            {
                powerClass = ((card->extendedCsd.powerClass52MHz360V) & mask);
            }
            else if (card->currentTiming == kMMC_HighSpeed26MHZTiming)
            {
                powerClass = ((card->extendedCsd.powerClass26MHz360V) & mask);
            }

            break;

        case kMMC_VoltageWindow170to195:

            if (card->currentTiming == kMMC_HighSpeed26MHZTiming)
            {
                powerClass = ((card->extendedCsd.powerClass26MHz195V) & mask);
            }
            else if (card->currentTiming == kMMC_HighSpeed52MHZTiming)
            {
                powerClass = ((card->extendedCsd.powerClass52MHz195V) & mask);
            }
            else if (card->currentTiming == kMMC_HighSpeedDDR52Timing)
            {
                powerClass = ((card->extendedCsd.powerClass52MHZDDR195V) & mask);
            }

            break;
        default:
            powerClass = 0;
            break;
    }

    /* due to 8bit power class position [7:4] */
    powerClass >>= shift;

    if (powerClass > 0U)
    {
        extendedCsdconfig.accessMode = kMMC_ExtendedCsdAccessModeWriteBits;
        extendedCsdconfig.ByteIndex = kMMC_ExtendedCsdIndexPowerClass;
        extendedCsdconfig.ByteValue = powerClass;
        extendedCsdconfig.commandSet = kMMC_CommandSetStandard;
        if (kStatus_Success != MMC_SetExtendedCsdConfig(card, &extendedCsdconfig))
        {
            return kStatus_SDMMC_ConfigureExtendedCsdFailed;
        }
        /* restore power class */
        card->extendedCsd.powerClass = powerClass;
    }

    return kStatus_Success;
}

static status_t MMC_SendTestPattern(mmc_card_t *card, uint32_t blockSize, uint32_t *pattern)
{
    assert(card);
    assert(card->host.transfer);
    assert(blockSize <= FSL_SDMMC_DEFAULT_BLOCK_SIZE);
    assert(pattern);

    HOST_TRANSFER content = { 0 };
    HOST_COMMAND command = { 0 };
    HOST_DATA data = { 0 };

    command.index = kMMC_SendingBusTest;
    command.argument = 0U;
    command.responseType = kCARD_ResponseTypeR1;

    /* Ignore errors in bus test procedure to improve chances that the test will work. */
    data.enableIgnoreError = true;
    data.blockCount = 1U;
    data.blockSize = blockSize;
    data.txData = pattern;

    content.command = &command;
    content.data = &data;
    if ((kStatus_Success != card->host.transfer(card->host.base, &content)) ||
        (command.response[0U] & kSDMMC_R1ErrorAllFlag))
    {
        return kStatus_SDMMC_TransferFailed;
    }

    return kStatus_Success;
}

static status_t MMC_ReceiveTestPattern(mmc_card_t *card, uint32_t blockSize, uint32_t *pattern)
{
    assert(card);
    assert(card->host.transfer);
    assert(blockSize <= FSL_SDMMC_DEFAULT_BLOCK_SIZE);
    assert(pattern);

    HOST_TRANSFER content = { 0 };
    HOST_COMMAND command = { 0 };
    HOST_DATA data = { 0 };

    command.index = kMMC_BusTestRead;
    command.responseType = kCARD_ResponseTypeR1;

    /* Ignore errors in bus test procedure to improve chances that the test will work. */
    data.enableIgnoreError = true;
    data.blockCount = 1U;
    data.blockSize = blockSize;
    data.rxData = pattern;

    content.command = &command;
    content.data = &data;
    if ((kStatus_Success != card->host.transfer(card->host.base, &content)) ||
        ((command.response[0U]) & kSDMMC_R1ErrorAllFlag))
    {
        return kStatus_SDMMC_TransferFailed;
    }

    return kStatus_Success;
}

static status_t MMC_TestDataBusWidth(mmc_card_t *card, mmc_data_bus_width_t width)
{
    assert(card);

    uint32_t blockSize = 0U;
    uint32_t tempsendPattern = 0U;
    uint32_t *tempPattern = g_sdmmc;
    uint32_t xorMask = 0U;
    uint32_t xorResult = 0U;

    /* For 8 data lines the data block would be (MSB to LSB): 0x0000_0000_0000_AA55,
    For 4 data lines the data block would be (MSB to LSB): 0x0000_005A,
    For only 1 data line the data block would be: 0x80 */
    switch (width)
    {
        case kMMC_DataBusWidth8bit:
            blockSize = 8U;
            tempPattern[0U] = 0xAA55U;
            xorMask = 0xFFFFU;
            xorResult = 0xFFFFU;
            break;
        case kMMC_DataBusWidth4bit:
            blockSize = 4U;
            tempPattern[0U] = 0x5AU;
            xorMask = 0xFFU;
            xorResult = 0xFFU;
            break;
        default:
            blockSize = 4U;
            tempPattern[0U] = 0x80U;
            xorMask = 0xFFU;
            xorResult = 0xC0U;
            break;
    }
    switch (card->host.config.endianMode)
    {
        case kHOST_EndianModeLittle:
            /* Doesn't need to switch byte sequence when decodes bytes as little endian sequence. */
            break;
        case kHOST_EndianModeBig:
            /* In big endian mode, the byte transferred first is the byte stored in highest byte position in a word
            which will cause the card receive the inverted byte sequence in a word in bus test procedure. So the
            sequence of 4 bytes stored in a word should be converted. */
            tempPattern[0] = SWAP_WORD_BYTE_SEQUENCE(tempPattern[0]);
            xorMask = SWAP_WORD_BYTE_SEQUENCE(xorMask);
            xorResult = SWAP_WORD_BYTE_SEQUENCE(xorResult);
            break;
        case kHOST_EndianModeHalfWordBig:
            /* In half word big endian mode, the byte transferred first is the lower byte in the higher half word.
            0xAA55U should be converted to 0xAA550000U to set the 0x55 to be the first byte to transfer. */
            tempPattern[0] = SWAP_HALF_WROD_BYTE_SEQUENCE(tempPattern[0]);
            xorMask = SWAP_HALF_WROD_BYTE_SEQUENCE(xorMask);
            xorResult = SWAP_HALF_WROD_BYTE_SEQUENCE(xorResult);
            tempPattern[0] = SWAP_WORD_BYTE_SEQUENCE(tempPattern[0]);
            xorMask = SWAP_WORD_BYTE_SEQUENCE(xorMask);
            xorResult = SWAP_WORD_BYTE_SEQUENCE(xorResult);
            break;
        default:
            return kStatus_SDMMC_NotSupportYet;
    }

    if (kStatus_Success != MMC_SendTestPattern(card, blockSize, tempPattern))
    {
        return kStatus_SDMMC_SendTestPatternFailed;
    }
    /* restore the send pattern */
    tempsendPattern = tempPattern[0U];
    /* reset the global buffer */
    tempPattern[0U] = 0U;

    if (kStatus_Success != MMC_ReceiveTestPattern(card, blockSize, tempPattern))
    {
        return kStatus_SDMMC_ReceiveTestPatternFailed;
    }

    /* XOR the send pattern and receive pattern */
    if (((tempPattern[0U] ^ tempsendPattern) & xorMask) != xorResult)
    {
        return kStatus_Fail;
    }

    return kStatus_Success;
}

static status_t MMC_SetDataBusWidth(mmc_card_t *card, mmc_data_bus_width_t width, bool isDDR)
{
    assert(card);

    mmc_extended_csd_config_t extendedCsdconfig;

    if ((width == kMMC_DataBusWidth4bit) && isDDR)
    {
        width = kMMC_DataBusWidth4bitDDR;
    }

    if ((width == kMMC_DataBusWidth8bit) && isDDR)
    {
        width = kMMC_DataBusWidth8bitDDR;
    }

    /* Set data bus width */
    extendedCsdconfig.accessMode = kMMC_ExtendedCsdAccessModeWriteBits;
    extendedCsdconfig.ByteIndex = kMMC_ExtendedCsdIndexBusWidth;
    extendedCsdconfig.ByteValue = width;
    extendedCsdconfig.commandSet = kMMC_CommandSetStandard;
    if (kStatus_Success != MMC_SetExtendedCsdConfig(card, &extendedCsdconfig))
    {
        return kStatus_SDMMC_ConfigureExtendedCsdFailed;
    }
    /* restore data bus width */
    card->extendedCsd.dataBusWidth = width;

    return kStatus_Success;
}

static status_t MMC_SetMaxDataBusWidth(mmc_card_t *card, bool isDDR)
{
    assert(card);

    status_t error = kStatus_Fail;

    /* Test and set the data bus width for card. */
    if (kHOST_Support8BitBusWidth != HOST_NOT_SUPPORT)
    {
        HOST_SET_CARD_BUS_WIDTH(card->host.base, kHOST_DATABUSWIDTH8BIT);
        if ((kStatus_Success == MMC_TestDataBusWidth(card, kMMC_DataBusWidth8bit)) &&
            (kStatus_Success == MMC_SetDataBusWidth(card, kMMC_DataBusWidth8bit, isDDR)))
        {
            card->flags |= kMMC_DataBusWidth8BitFlag;
            error = kStatus_Success;
        }
    }
    /* HS400 mode only support 8bit data bus */
    if ((error == kStatus_Fail) && (card->currentTiming == kMMC_HighSpeed400Timing))
    {
        return kStatus_SDMMC_SetDataBusWidthFailed;
    }

    if ((error == kStatus_Fail) && (kHOST_Support4BitBusWidth != HOST_NOT_SUPPORT))
    {
        HOST_SET_CARD_BUS_WIDTH(card->host.base, kHOST_DATABUSWIDTH4BIT);
        if ((kStatus_Success == MMC_TestDataBusWidth(card, kMMC_DataBusWidth4bit)) &&
            (kStatus_Success == MMC_SetDataBusWidth(card, kMMC_DataBusWidth4bit, isDDR)))
        {
            card->flags |= kMMC_DataBusWidth4BitFlag;
            error = kStatus_Success;
        }
    }

    /* HS200/DDR mode only support 4bit/8bit data bus */
    if ((error == kStatus_Fail) &&
        ((card->currentTiming == kMMC_HighSpeed200Timing) || (card->currentTiming == kMMC_HighSpeedDDR52Timing)))
    {
        return kStatus_SDMMC_SetDataBusWidthFailed;
    }

    if (error == kStatus_Fail)
    {
        card->flags |= kMMC_DataBusWidth1BitFlag;
        /* Card's data bus width will be default 1 bit mode. */
        HOST_SET_CARD_BUS_WIDTH(card->host.base, kHOST_DATABUSWIDTH1BIT);
    }

    return kStatus_Success;
}

static status_t MMC_BL_SelectBusWidth(mmc_card_t *card)
{
    assert(card);
    bool isDDR = false;
    mmc_data_bus_width_t targetBusWidth = card->userConfig.busWidth;

    switch (targetBusWidth)
    {
        case kMMC_DataBusWidth8bitDDR:
            isDDR = true;
        case kMMC_DataBusWidth8bit:
            /* Test and set the data bus width for card. */
            if (kHOST_Support8BitBusWidth != HOST_NOT_SUPPORT)
            {
                HOST_SET_CARD_BUS_WIDTH(card->host.base, kHOST_DATABUSWIDTH8BIT);
                if ((kStatus_Success == MMC_TestDataBusWidth(card, kMMC_DataBusWidth8bit)) &&
                    (kStatus_Success == MMC_SetDataBusWidth(card, kMMC_DataBusWidth8bit, isDDR)))
                {
                    card->flags |= kMMC_DataBusWidth8BitFlag;
                    card->currentBusWidth = targetBusWidth;
                    return kStatus_Success;
                }
                else
                {
                    return kStatus_SDMMC_SetDataBusWidthFailed;
                }
            }
            else
            {
                return kStatus_SDMMC_HostNotSupport;
            }
        case kMMC_DataBusWidth4bitDDR:
            isDDR = true;
        case kMMC_DataBusWidth4bit:
            /* Test and set the data bus width for card. */
            if (kHOST_Support4BitBusWidth != HOST_NOT_SUPPORT)
            {
                HOST_SET_CARD_BUS_WIDTH(card->host.base, kHOST_DATABUSWIDTH4BIT);
                if ((kStatus_Success == MMC_TestDataBusWidth(card, kMMC_DataBusWidth4bit)) &&
                    (kStatus_Success == MMC_SetDataBusWidth(card, kMMC_DataBusWidth4bit, isDDR)))
                {
                    card->flags |= kMMC_DataBusWidth4BitFlag;
                    card->currentBusWidth = targetBusWidth;
                    return kStatus_Success;
                }
                else
                {
                    return kStatus_SDMMC_SetDataBusWidthFailed;
                }
            }
            else
            {
                return kStatus_SDMMC_HostNotSupport;
            }
        case kMMC_DataBusWidth1bit: /*!< MMC data bus width is 1 bit */
        default:
            return kStatus_Success;
    }
}

static status_t MMC_SwitchHSTiming(mmc_card_t *card, uint8_t timing, uint8_t driverStrength)
{
    assert(card);

    uint8_t hsTiming = 0;

    mmc_extended_csd_config_t extendedCsdconfig;

    /* check the target driver strength support or not */
    if (((card->extendedCsd.ioDriverStrength & (1 << driverStrength)) == 0U) &&
        (card->extendedCsd.extendecCsdVersion >= kMMC_ExtendedCsdRevision17))
    {
        return kStatus_SDMMC_NotSupportYet;
    }
    /* calucate the register value */
    hsTiming = (timing & 0xF) | (uint8_t)(driverStrength << 4U);

    /* Switch to high speed timing. */
    extendedCsdconfig.accessMode = kMMC_ExtendedCsdAccessModeWriteBits;
    extendedCsdconfig.ByteIndex = kMMC_ExtendedCsdIndexHighSpeedTiming;
    extendedCsdconfig.ByteValue = hsTiming;
    extendedCsdconfig.commandSet = kMMC_CommandSetStandard;
    if (kStatus_Success != MMC_SetExtendedCsdConfig(card, &extendedCsdconfig))
    {
        return kStatus_SDMMC_ConfigureExtendedCsdFailed;
    }

    card->extendedCsd.highSpeedTiming = hsTiming;

    return kStatus_Success;
}

static status_t MMC_BL_SwitchToHighSpeed(mmc_card_t *card, uint32_t freq)
{
    assert(card);

    if (kStatus_Success != MMC_SwitchHSTiming(card, kMMC_HighSpeedTiming, kMMC_DriverStrength0))
    {
        return kStatus_SDMMC_SwitchBusTimingFailed;
    }

    bool isDDR = (card->userConfig.busWidth == kMMC_DataBusWidth4bitDDR) ||
                 (card->userConfig.busWidth == kMMC_DataBusWidth8bitDDR);

    if (isDDR)
    {
        /* Test procedure is not supported at DDR mode.
         * So, change the frequency at SDR mode to the same frequency of SDR mode,
         * before changing the bus width.
         */
        card->busClock_Hz = HOST_SET_CARD_CLOCK(card->host.base, card->host.sourceClock_Hz, freq / 2);
    }
    else
    {
        card->busClock_Hz = HOST_SET_CARD_CLOCK(card->host.base, card->host.sourceClock_Hz, freq);
    }

    /* config io speed and strength */
    HOST_UPDATE_MMC_IO(card->host.base, card->userConfig.busWidth, CARD_BUS_FREQ_100MHZ1, CARD_BUS_STRENGTH_7);

    /* Set card data width, it is nessesary to config the the data bus here, to meet emmc5.0 specification,
     * when you are working in DDR mode , HS_TIMING must set before set bus width
     */
    if (MMC_BL_SelectBusWidth(card) != kStatus_Success)
    {
        return kStatus_SDMMC_SetDataBusWidthFailed;
    }

    /* enable DDR if needed */
    if (isDDR)
    {
        /* Update the freq*/
        card->busClock_Hz = HOST_SET_CARD_CLOCK(card->host.base, card->host.sourceClock_Hz, freq);
        HOST_ENABLE_DDR_MODE(card->host.base, true);
    }

    card->currentTiming = kMMC_HighSpeedTiming;

    return kStatus_Success;
}

static status_t MMC_BL_SwitchToNomralSpeed(mmc_card_t *card)
{
    assert(card);

    /* Check bus width config */
    if ((card->userConfig.busWidth == kMMC_DataBusWidth4bitDDR) ||
        (card->userConfig.busWidth == kMMC_DataBusWidth8bitDDR))
    {
        /* Compatibility timming only supports SDR. */
        return kStatus_SDMMC_CardNotSupport;
    }
    /* Set card data width, it is nessesary to config the the data bus here, to meet emmc5.0 specification,
    * when you are working in DDR mode , HS_TIMING must set before set bus width
    */
    if (MMC_BL_SelectBusWidth(card) != kStatus_Success)
    {
        return kStatus_SDMMC_SetDataBusWidthFailed;
    }

    return kStatus_Success;
}

static status_t MMC_SwitchToHighSpeed(mmc_card_t *card, uint32_t freq)
{
    assert(card);

    /* check VCCQ voltage supply */
    if (kHOST_SupportV180 != HOST_NOT_SUPPORT)
    {
        if ((card->hostVoltageWindowVCCQ != kMMC_VoltageWindow170to195) &&
            (card->extendedCsd.extendecCsdVersion > kMMC_ExtendedCsdRevision10))
        {
            HOST_SWITCH_VOLTAGE180V(card->host.base, true);
            card->hostVoltageWindowVCCQ = kMMC_VoltageWindow170to195;
        }
    }
    else if (kHOST_SupportV120 != HOST_NOT_SUPPORT)
    {
        if ((card->hostVoltageWindowVCCQ != kMMC_VoltageWindow120) &&
            (card->extendedCsd.extendecCsdVersion >= kMMC_ExtendedCsdRevision16))
        {
            HOST_SWITCH_VOLTAGE120V(card->host.base, true);
            card->hostVoltageWindowVCCQ = kMMC_VoltageWindow120;
        }
    }
    else
    {
        card->hostVoltageWindowVCCQ = kMMC_VoltageWindows270to360;
    }

    if (kStatus_Success != MMC_SwitchHSTiming(card, kMMC_HighSpeedTiming, kMMC_DriverStrength0))
    {
        return kStatus_SDMMC_SwitchBusTimingFailed;
    }

    /* Set card data width, it is nessesary to config the the data bus here, to meet emmc5.0 specification,
    * when you are working in DDR mode , HS_TIMING must set before set bus width
    */
    if (MMC_SetMaxDataBusWidth(card, card->currentTiming == kMMC_HighSpeedDDR52Timing) != kStatus_Success)
    {
        return kStatus_SDMMC_SetDataBusWidthFailed;
    }

    card->busClock_Hz = HOST_SET_CARD_CLOCK(card->host.base, card->host.sourceClock_Hz, freq);
    /* config io speed and strength */
    HOST_CONFIG_MMC_IO(CARD_BUS_FREQ_100MHZ1, CARD_BUS_STRENGTH_7);
    /* enable DDR if needed */
    if (card->currentTiming == kMMC_HighSpeedDDR52Timing)
    {
        HOST_ENABLE_DDR_MODE(card->host.base, true);
    }

    return kStatus_Success;
}

static status_t MMC_SwitchToHS200(mmc_card_t *card, uint32_t freq)
{
    assert(card);

    /* check VCCQ voltage supply */
    if (kHOST_SupportV180 != HOST_NOT_SUPPORT)
    {
        if (card->hostVoltageWindowVCCQ != kMMC_VoltageWindow170to195)
        {
            HOST_SWITCH_VOLTAGE180V(card->host.base, true);
            card->hostVoltageWindowVCCQ = kMMC_VoltageWindow170to195;
        }
    }
    else if (kHOST_SupportV120 != HOST_NOT_SUPPORT)
    {
        if (card->hostVoltageWindowVCCQ != kMMC_VoltageWindow120)
        {
            HOST_SWITCH_VOLTAGE120V(card->host.base, true);
            card->hostVoltageWindowVCCQ = kMMC_VoltageWindow120;
        }
    }
    else
    {
        return kStatus_SDMMC_InvalidVoltage;
    }

    /* select bus width before select bus timing for HS200 mode */
    if (MMC_SetMaxDataBusWidth(card, false) != kStatus_Success)
    {
        return kStatus_SDMMC_SetDataBusWidthFailed;
    }
    /* switch to HS200 mode */
    if (kStatus_Success != MMC_SwitchHSTiming(card, kMMC_HighSpeed200Timing, kMMC_DriverStrength0))
    {
        return kStatus_SDMMC_SwitchBusTimingFailed;
    }

    card->busClock_Hz = HOST_SET_CARD_CLOCK(card->host.base, card->host.sourceClock_Hz, freq);
    /* config io speed and strength */
    HOST_CONFIG_MMC_IO(CARD_BUS_FREQ_200MHZ, CARD_BUS_STRENGTH_7);

    /* excute tuning for HS200 */
    if (MMC_ExecuteTuning(card) != kStatus_Success)
    {
        return kStatus_SDMMC_TuningFail;
    }

    /* Wait for the card status ready. */
    if (kStatus_Success != MMC_WaitWriteComplete(card))
    {
        return kStatus_SDMMC_WaitWriteCompleteFailed;
    }

    card->currentTiming = kMMC_HighSpeed200Timing;

    return kStatus_Success;
}

static status_t MMC_SwitchToHS400(mmc_card_t *card)
{
    assert(card);

    /* check VCCQ voltage supply */
    if (kHOST_SupportV180 != HOST_NOT_SUPPORT)
    {
        if (card->hostVoltageWindowVCCQ != kMMC_VoltageWindow170to195)
        {
            HOST_SWITCH_VOLTAGE180V(card->host.base, true);
            card->hostVoltageWindowVCCQ = kMMC_VoltageWindow170to195;
        }
    }
    else if (kHOST_SupportV120 != HOST_NOT_SUPPORT)
    {
        if (card->hostVoltageWindowVCCQ != kMMC_VoltageWindow120)
        {
            HOST_SWITCH_VOLTAGE120V(card->host.base, true);
            card->hostVoltageWindowVCCQ = kMMC_VoltageWindow120;
        }
    }
    else
    {
        return kStatus_SDMMC_InvalidVoltage;
    }

    /* check data bus width is 8 bit , otherwise return false */
    if ((card->flags & kMMC_DataBusWidth8BitFlag) == 0U)
    {
        return kStatus_SDMMC_SwitchBusTimingFailed;
    }

    /* switch to high speed first */
    card->busClock_Hz = HOST_SET_CARD_CLOCK(card->host.base, card->host.sourceClock_Hz, MMC_CLOCK_52MHZ);
    HOST_CONFIG_MMC_IO(CARD_BUS_FREQ_100MHZ1, CARD_BUS_STRENGTH_5);
    /*switch to high speed*/
    if (kStatus_Success != MMC_SwitchHSTiming(card, kMMC_HighSpeedTiming, kMMC_DriverStrength0))
    {
        return kStatus_SDMMC_ConfigureExtendedCsdFailed;
    }
    card->currentTiming = kMMC_HighSpeed400Timing;
    /* switch to 8 bit DDR data bus width */
    if (kStatus_Success != MMC_SetDataBusWidth(card, kMMC_DataBusWidth8bit, true))
    {
        return kStatus_SDMMC_SetDataBusWidthFailed;
    }
    /* switch to HS400 */
    if (kStatus_Success != MMC_SwitchHSTiming(card, kMMC_HighSpeed400Timing, kMMC_DriverStrength0))
    {
        return kStatus_SDMMC_SwitchBusTimingFailed;
    }
    /* config to target freq */
    card->busClock_Hz = HOST_SET_CARD_CLOCK(card->host.base, card->host.sourceClock_Hz, HOST_SUPPORT_HS400_FREQ);
    /* config io speed and strength */
    HOST_CONFIG_MMC_IO(CARD_BUS_FREQ_200MHZ, CARD_BUS_STRENGTH_7);
    /* enable HS400 mode */
    HOST_ENABLE_HS400_MODE(card->host.base, true);
    /* enable DDR mode */
    HOST_ENABLE_DDR_MODE(card->host.base, true);
    /* config strobe DLL */
    HOST_CONFIG_STROBE_DLL(card->host.base, HOST_STROBE_DLL_DELAY_TARGET, HOST_STROBE_DLL_DELAY_UPDATE_INTERVAL);
    /* enable DLL */
    HOST_ENABLE_STROBE_DLL(card->host.base, true);

    return kStatus_Success;
}

static status_t MMC_SelectBusTiming(mmc_card_t *card)
{
    assert(card);

    bool switchHS400 = false, switchHighSpeed = false;
    uint32_t targetFreq = 0U;
    mmc_high_speed_timing_t targetTiming = card->currentTiming;

    switch (targetTiming)
    {
        case kMMC_HighSpeedTimingNone:
        case kMMC_HighSpeed400Timing:
            if ((kHOST_SupportHS400 != HOST_NOT_SUPPORT) &&
                (card->flags & (kMMC_SupportHS400DDR200MHZ180VFlag | kMMC_SupportHS400DDR200MHZ120VFlag)))
            {
                switchHS400 = true;
            }
        case kMMC_HighSpeed200Timing:
            if ((card->flags & (kMMC_SupportHS200200MHZ180VFlag | kMMC_SupportHS200200MHZ120VFlag)) &&
                (kHOST_SupportHS200 != HOST_NOT_SUPPORT))
            {
                if (switchHS400)
                {
                    /* switch to HS200 perform tuning */
                    if (kStatus_Success != MMC_SwitchToHS200(card, HOST_SUPPORT_HS400_FREQ / 2U))
                    {
                        return kStatus_SDMMC_SwitchBusTimingFailed;
                    }
                    /* switch to HS400 */
                    if (kStatus_Success != MMC_SwitchToHS400(card))
                    {
                        return kStatus_SDMMC_SwitchBusTimingFailed;
                    }
                }
                else
                {
                    if (kStatus_Success != MMC_SwitchToHS200(card, HOST_SUPPORT_HS200_FREQ))
                    {
                        return kStatus_SDMMC_SwitchBusTimingFailed;
                    }
                }

                break;
            }
        case kMMC_HighSpeedDDR52Timing:

            if ((card->flags & (kMMC_SupportHighSpeedDDR52MHZ180V300VFlag | kMMC_SupportHighSpeedDDR52MHZ120VFlag)) &&
                (kHOST_SupportDDR50 != HOST_NOT_SUPPORT))
            {
                targetFreq = MMC_CLOCK_DDR52;
                card->currentTiming = kMMC_HighSpeedDDR52Timing;
                switchHighSpeed = true;
                break;
            }
        case kMMC_HighSpeed52MHZTiming:
            if (card->flags & kMMC_SupportHighSpeed52MHZFlag)
            {
                targetFreq = MMC_CLOCK_52MHZ;
                card->currentTiming = kMMC_HighSpeed52MHZTiming;
                switchHighSpeed = true;
                break;
            }
        case kMMC_HighSpeed26MHZTiming:
            if (card->flags & kMMC_SupportHighSpeed26MHZFlag)
            {
                targetFreq = MMC_CLOCK_26MHZ;
                card->currentTiming = kMMC_HighSpeed26MHZTiming;
                switchHighSpeed = true;
                break;
            }
        default:
            card->currentTiming = kMMC_HighSpeedTimingNone;
    }
    /* switch to high speed */
    if (switchHighSpeed)
    {
        if (kStatus_Success != MMC_SwitchToHighSpeed(card, targetFreq))
        {
            return kStatus_SDMMC_SwitchBusTimingFailed;
        }
    }

    return kStatus_Success;
}

static status_t MMC_BL_SelectBusTiming(mmc_card_t *card)
{
    assert(card);

    bool switchHighSpeed = false;
    uint32_t targetFreq = 0U;
    mmc_high_speed_timing_t targetTiming = card->userConfig.timing;

    switch (targetTiming)
    {
        case kMMC_HighSpeed400Timing:
        case kMMC_HighSpeed200Timing:
            return kStatus_SDMMC_NotSupportYet;
        case kMMC_HighSpeedTiming:
            /* DDR mode */
            if ((card->userConfig.busWidth == kMMC_DataBusWidth4bitDDR) ||
                (card->userConfig.busWidth == kMMC_DataBusWidth8bitDDR))
            {
                if (kHOST_SupportDDR50 != HOST_NOT_SUPPORT)
                {
                    if (card->flags &
                        (kMMC_SupportHighSpeedDDR52MHZ180V300VFlag | kMMC_SupportHighSpeedDDR52MHZ120VFlag))
                    {
                        targetFreq = MMC_CLOCK_DDR52;
                        switchHighSpeed = true;
                    }
                }
                else
                {
                    return kStatus_SDMMC_HostNotSupport;
                }
            }
            else /* SDR Mode*/
            {
                if (kHOST_SupportHighSpeed != HOST_NOT_SUPPORT)
                {
                    if (card->flags & kMMC_SupportHighSpeed52MHZFlag)
                    {
                        targetFreq = MMC_CLOCK_52MHZ;
                        switchHighSpeed = true;
                    }
                    else if (card->flags & kMMC_SupportHighSpeed26MHZFlag)
                    {
                        targetFreq = MMC_CLOCK_26MHZ;
                        switchHighSpeed = true;
                    }
                }
                else
                {
                    return kStatus_SDMMC_HostNotSupport;
                }
            }

            /* switch to high speed */
            if (switchHighSpeed)
            {
                return MMC_BL_SwitchToHighSpeed(card, targetFreq);
            }
            else
            {
                return kStatus_SDMMC_CardNotSupport;
            }
        case kMMC_HighSpeedTimingNone:
        default:
            return MMC_BL_SwitchToNomralSpeed(card);
    }
}

static void MMC_DecodeCid(mmc_card_t *card, uint32_t *rawCid)
{
    assert(card);
    assert(rawCid);

    mmc_cid_t *cid;

    cid = &(card->cid);
    cid->manufacturerID = (uint8_t)((rawCid[3U] & 0xFF000000U) >> 24U);
    cid->applicationID = (uint16_t)((rawCid[3U] & 0xFFFF00U) >> 8U);

    cid->productName[0U] = (uint8_t)((rawCid[3U] & 0xFFU));
    cid->productName[1U] = (uint8_t)((rawCid[2U] & 0xFF000000U) >> 24U);
    cid->productName[2U] = (uint8_t)((rawCid[2U] & 0xFF0000U) >> 16U);
    cid->productName[3U] = (uint8_t)((rawCid[2U] & 0xFF00U) >> 8U);
    cid->productName[4U] = (uint8_t)((rawCid[2U] & 0xFFU));

    cid->productVersion = (uint8_t)((rawCid[1U] & 0xFF000000U) >> 24U);

    cid->productSerialNumber = (uint32_t)((rawCid[1U] & 0xFFFFFFU) << 8U);
    cid->productSerialNumber |= (uint32_t)((rawCid[0U] & 0xFF000000U) >> 24U);

    cid->manufacturerData = (uint16_t)((rawCid[0U] & 0xFFF00U) >> 8U);
}

static status_t MMC_AllSendCid(mmc_card_t *card)
{
    assert(card);
    assert(card->host.transfer);

    HOST_TRANSFER content = { 0 };
    HOST_COMMAND command = { 0 };

    command.index = kSDMMC_AllSendCid;
    command.argument = 0U;
    command.responseType = kCARD_ResponseTypeR2;

    content.command = &command;
    content.data = NULL;
    if (kStatus_Success == card->host.transfer(card->host.base, &content))
    {
        memcpy(card->rawCid, command.response, sizeof(card->rawCid));
        MMC_DecodeCid(card, command.response);

        return kStatus_Success;
    }

    return kStatus_SDMMC_TransferFailed;
}

static status_t MMC_SendCsd(mmc_card_t *card)
{
    assert(card);
    assert(card->host.transfer);

    HOST_COMMAND command = { 0 };
    HOST_TRANSFER content = { 0 };

    command.index = kSDMMC_SendCsd;
    command.argument = (card->relativeAddress << 16U);
    command.responseType = kCARD_ResponseTypeR2;

    content.command = &command;
    content.data = 0U;
    if (kStatus_Success == card->host.transfer(card->host.base, &content))
    {
        memcpy(card->rawCsd, command.response, sizeof(card->rawCsd));
        /* The response is from bit 127:8 in R2, corresponding to command.response[3][31:0] to
        command.response[0U][31:8]. */
        MMC_DecodeCsd(card, card->rawCsd);

        return kStatus_Success;
    }

    return kStatus_SDMMC_TransferFailed;
}

static status_t MMC_CheckBlockRange(mmc_card_t *card, uint32_t startBlock, uint32_t blockCount)
{
    assert(card);
    assert(blockCount);

    status_t error = kStatus_Success;
    uint32_t partitionBlocks;

    switch (card->currentPartition)
    {
        case kMMC_AccessPartitionUserAera:
        {
            partitionBlocks = card->userPartitionBlocks;
            break;
        }
        case kMMC_AccessPartitionBoot1:
        case kMMC_AccessPartitionBoot2:
        {
            /* Boot partition 1 and partition 2 have the same partition size. */
            partitionBlocks = card->bootPartitionBlocks;
            break;
        }
        case kMMC_AccessGeneralPurposePartition1:
        {
            partitionBlocks = card->generalPartitionSize[0];
            break;
        }
        case kMMC_AccessGeneralPurposePartition2:
        {
            partitionBlocks = card->generalPartitionSize[1];
            break;
        }
        case kMMC_AccessGeneralPurposePartition3:
        {
            partitionBlocks = card->generalPartitionSize[2];
            break;
        }
        case kMMC_AccessGeneralPurposePartition4:
        {
            partitionBlocks = card->generalPartitionSize[3];
            break;
        }
        default:
            error = kStatus_InvalidArgument;
            break;
    }
    /* Check if the block range accessed is within current partition's block boundary. */
    if ((error == kStatus_Success) && ((startBlock + blockCount) > partitionBlocks))
    {
        error = kStatus_InvalidArgument;
    }

    return error;
}

static status_t MMC_CheckEraseGroupRange(mmc_card_t *card, uint32_t startGroup, uint32_t endGroup)
{
    assert(card);

    status_t error = kStatus_Success;
    uint32_t partitionBlocks;
    uint32_t eraseGroupBoundary;

    switch (card->currentPartition)
    {
        case kMMC_AccessPartitionUserAera:
        {
            partitionBlocks = card->userPartitionBlocks;
            break;
        }
        case kMMC_AccessPartitionBoot1:
        case kMMC_AccessPartitionBoot2:
        {
            /* Boot partition 1 and partition 2 have the same partition size. */
            partitionBlocks = card->bootPartitionBlocks;
            break;
        }
        case kMMC_AccessGeneralPurposePartition1:
        {
            partitionBlocks = card->generalPartitionSize[0];
            break;
        }
        case kMMC_AccessGeneralPurposePartition2:
        {
            partitionBlocks = card->generalPartitionSize[1];
            break;
        }
        case kMMC_AccessGeneralPurposePartition3:
        {
            partitionBlocks = card->generalPartitionSize[2];
            break;
        }
        case kMMC_AccessGeneralPurposePartition4:
        {
            partitionBlocks = card->generalPartitionSize[3];
            break;
        }
        default:
            error = kStatus_InvalidArgument;
            break;
    }

    if (error == kStatus_Success)
    {
        /* Check if current partition's total block count is integer multiples of the erase group size. */
        if ((partitionBlocks % card->eraseGroupBlocks) == 0U)
        {
            eraseGroupBoundary = (partitionBlocks / card->eraseGroupBlocks);
        }
        else
        {
            /* Card will ignore the unavailable blocks within the last erase group automatically. */
            eraseGroupBoundary = (partitionBlocks / card->eraseGroupBlocks + 1U);
        }

        /* Check if the group range accessed is within current partition's erase group boundary. */
        if ((startGroup > eraseGroupBoundary) || (endGroup > eraseGroupBoundary))
        {
            error = kStatus_InvalidArgument;
        }
    }

    return error;
}

static status_t MMC_Read(
    mmc_card_t *card, uint8_t *buffer, uint32_t startBlock, uint32_t blockSize, uint32_t blockCount)
{
    assert(card);
    assert(card->host.transfer);
    assert(buffer);
    assert(blockCount);
    assert(blockSize);
    assert(blockSize == FSL_SDMMC_DEFAULT_BLOCK_SIZE);

    HOST_COMMAND command = { 0 };
    HOST_DATA data = { 0 };
    HOST_TRANSFER content = { 0 };
    status_t error;

    if (((card->flags & kMMC_SupportHighCapacityFlag) && (blockSize != 512U)) || (blockSize > card->blockSize) ||
        (blockSize > card->host.capability.maxBlockLength) || (blockSize % 4U))
    {
        return kStatus_SDMMC_CardNotSupport;
    }

    /* Wait for the card write process complete because of that card read process and write process use one buffer. */
    if (kStatus_Success != MMC_WaitWriteComplete(card))
    {
        return kStatus_SDMMC_WaitWriteCompleteFailed;
    }

    data.blockSize = blockSize;
    data.blockCount = blockCount;
    data.rxData = (uint32_t *)buffer;
    data.enableAutoCommand12 = true;
    command.index = kSDMMC_ReadMultipleBlock;
    if (data.blockCount == 1U)
    {
        command.index = kSDMMC_ReadSingleBlock;
    }
    else
    {
        if ((!(data.enableAutoCommand12)) && (card->enablePreDefinedBlockCount))
        {
            /* If enabled the pre-define count read/write feature of the card, need to set block count firstly. */
            if (kStatus_Success != MMC_SetBlockCount(card, blockCount))
            {
                return kStatus_SDMMC_SetBlockCountFailed;
            }
        }
    }
    command.argument = startBlock;
    if (!(card->flags & kMMC_SupportHighCapacityFlag))
    {
        command.argument *= data.blockSize;
    }
    command.responseType = kCARD_ResponseTypeR1;
    command.responseErrorFlags = kSDMMC_R1ErrorAllFlag;

    content.command = &command;
    content.data = &data;

    /* should check tuning error during every transfer */
    error = MMC_Transfer(card, &content, 1U);
    if (kStatus_Success != error)
    {
        return error;
    }

    /* When host's AUTO_COMMAND12 feature isn't enabled and PRE_DEFINED_COUNT command isn't enabled in multiple
    blocks transmission, sends STOP_TRANSMISSION command. */
    if ((blockCount > 1U) && (!(data.enableAutoCommand12)) && (!card->enablePreDefinedBlockCount))
    {
        if (kStatus_Success != MMC_StopTransmission(card))
        {
            return kStatus_SDMMC_StopTransmissionFailed;
        }
    }

    return kStatus_Success;
}

static status_t MMC_Write(
    mmc_card_t *card, const uint8_t *buffer, uint32_t startBlock, uint32_t blockSize, uint32_t blockCount)
{
    assert(card);
    assert(card->host.transfer);
    assert(buffer);
    assert(blockCount);
    assert(blockSize);
    assert(blockSize == FSL_SDMMC_DEFAULT_BLOCK_SIZE);

    HOST_COMMAND command = { 0 };
    HOST_DATA data = { 0 };
    HOST_TRANSFER content = { 0 };
    status_t error;

    /* Check address range */
    if (((card->flags & kMMC_SupportHighCapacityFlag) && (blockSize != 512U)) || (blockSize > card->blockSize) ||
        (blockSize > card->host.capability.maxBlockLength) || (blockSize % 4U))
    {
        return kStatus_SDMMC_CardNotSupport;
    }

    /* Wait for the card's buffer to be not full to write to improve the write performance. */
    while ((GET_HOST_STATUS(card->host.base) & CARD_DATA0_STATUS_MASK) != CARD_DATA0_NOT_BUSY)
    {
    }

    data.blockSize = blockSize;
    data.blockCount = blockCount;
    data.txData = (const uint32_t *)buffer;
    data.enableAutoCommand12 = true;

    command.index = kSDMMC_WriteMultipleBlock;
    if (data.blockCount == 1U)
    {
        command.index = kSDMMC_WriteSingleBlock;
    }
    else
    {
        if ((!(data.enableAutoCommand12)) && (card->enablePreDefinedBlockCount))
        {
            /* If enabled the pre-define count read/write featue of the card, need to set block count firstly */
            if (kStatus_Success != MMC_SetBlockCount(card, blockCount))
            {
                return kStatus_SDMMC_SetBlockCountFailed;
            }
        }
    }
    command.argument = startBlock;
    if (!(card->flags & kMMC_SupportHighCapacityFlag))
    {
        command.argument *= blockSize;
    }
    command.responseType = kCARD_ResponseTypeR1;
    command.responseErrorFlags = kSDMMC_R1ErrorAllFlag;

    content.command = &command;
    content.data = &data;

    /* should check tuning error during every transfer */
    error = MMC_Transfer(card, &content, 1U);
    if (kStatus_Success != error)
    {
        return error;
    }

    /* When host's AUTO_COMMAND12 feature isn't enabled and PRE_DEFINED_COUNT command isn't enabled in multiple
    blocks transmission, sends STOP_TRANSMISSION command. */
    if ((blockCount > 1U) && (!(data.enableAutoCommand12)) && (!card->enablePreDefinedBlockCount))
    {
        if (kStatus_Success != MMC_StopTransmission(card))
        {
            return kStatus_SDMMC_StopTransmissionFailed;
        }
    }

    return kStatus_Success;
}

status_t MMC_Init(mmc_card_t *card)
{
    assert(card);
    assert((card->hostVoltageWindowVCC != kMMC_VoltageWindowNone) &&
           (card->hostVoltageWindowVCC != kMMC_VoltageWindow120));

    status_t error = kStatus_Success;
    uint32_t opcode = 0U;

    if (!card->isHostReady)
    {
        error = HOST_Init(&(card->host));
        if (error != kStatus_Success)
        {
            return error;
        }
        /* set the host status flag, after the card re-plug in, don't need init host again */
        card->isHostReady = true;
    }
    else
    {
        /* reset the host */
        HOST_Reset(card->host.base);
    }
    /* set DATA bus width */
    HOST_SET_CARD_BUS_WIDTH(card->host.base, kHOST_DATABUSWIDTH1BIT);
    /* Set clock to 400KHz. */
    card->busClock_Hz = HOST_SET_CARD_CLOCK(card->host.base, card->host.sourceClock_Hz, SDMMC_CLOCK_400KHZ);
    /* get host capability first */
    GET_HOST_CAPABILITY(card->host.base, &(card->host.capability));

    /* Send CMD0 to reset the bus */
    if (kStatus_Success != MMC_GoIdle(card))
    {
        return kStatus_SDMMC_GoIdleFailed;
    }

    /* Hand-shaking with card to validata the voltage range Host first sending its expected
       information.*/
    if (kStatus_Success != MMC_SendOperationCondition(card, 0U))
    {
        return kStatus_SDMMC_HandShakeOperationConditionFailed;
    }

    /* switch the host voltage which the card can support */
    if (kStatus_Success != MMC_SwitchVoltage(card, &opcode))
    {
        return kStatus_SDMMC_HandShakeOperationConditionFailed;
    }

    /* Get host's access mode. */
    if (card->host.capability.maxBlockLength >= FSL_SDMMC_DEFAULT_BLOCK_SIZE)
    {
        opcode |= kMMC_AccessModeSector << MMC_OCR_ACCESS_MODE_SHIFT;
    }
    else
    {
        opcode |= kMMC_AccessModeSector << MMC_OCR_ACCESS_MODE_SHIFT;
    }

    if (kStatus_Success != MMC_SendOperationCondition(card, opcode))
    {
        return kStatus_SDMMC_HandShakeOperationConditionFailed;
    }

    /* Get card CID */
    if (kStatus_Success != MMC_AllSendCid(card))
    {
        return kStatus_SDMMC_AllSendCidFailed;
    }

    /* Set the card relative address */
    if (kStatus_Success != MMC_SetRelativeAddress(card))
    {
        return kStatus_SDMMC_SetRelativeAddressFailed;
    }

    /* Get the CSD register content */
    if (kStatus_Success != MMC_SendCsd(card))
    {
        return kStatus_SDMMC_SendCsdFailed;
    }

    /* Set to maximum speed in normal mode. */
    MMC_SetMaxFrequency(card);

    /* Send CMD7 with the card's relative address to place the card in transfer state. Puts current selected card in
    transfer state. */
    if (kStatus_Success != MMC_SelectCard(card, true))
    {
        return kStatus_SDMMC_SelectCardFailed;
    }

    /* Get Extended CSD register content. */
    if (kStatus_Success != MMC_SendExtendedCsd(card, NULL, 0U))
    {
        return kStatus_SDMMC_SendExtendedCsdFailed;
    }

    /* set block size */
    if (kStatus_Success != MMC_SetBlockSize(card, FSL_SDMMC_DEFAULT_BLOCK_SIZE))
    {
        return kStatus_SDMMC_SetCardBlockSizeFailed;
    }

    /* switch to host support speed mode, then switch MMC data bus width and select power class */
    if (kStatus_Success != MMC_SelectBusTiming(card))
    {
        return kStatus_SDMMC_SwitchBusTimingFailed;
    }

    /* switch power class */
    if (kStatus_Success != MMC_SetPowerClass(card))
    {
        return kStatus_SDMMC_SetPowerClassFail;
    }

    /* Set to max erase unit size */
    if (kStatus_Success != MMC_SetMaxEraseUnitSize(card))
    {
        return kStatus_SDMMC_EnableHighCapacityEraseFailed;
    }

    /* Set card default to access non-boot partition */
    card->currentPartition = kMMC_AccessPartitionUserAera;

    return kStatus_Success;
}

void MMC_Deinit(mmc_card_t *card)
{
    assert(card);

    MMC_SelectCard(card, false);
    HOST_Deinit(&(card->host));
    /* should re-init host */
    card->isHostReady = false;
}

status_t MMC_BL_Init(mmc_card_t *card)
{
    assert(card);
    assert((card->hostVoltageWindowVCC != kMMC_VoltageWindowNone) &&
           (card->hostVoltageWindowVCC != kMMC_VoltageWindow120));
    assert(card->hostVoltageWindowVCCQ != kMMC_VoltageWindowNone);

    status_t error = kStatus_Success;
    uint32_t opcode = 0U;

    HOST_Reset(card->host.base);
    error = HOST_Init(&(card->host));
    if (error != kStatus_Success)
    {
        return error;
    }

    /* init the pin mux here. */
    HOST_CONFIG_MMC_MUX(card->host.base, card->userConfig.busWidth);

    /* execute the power cycle here */

    if (card->userConfig.enablePowerCycle)
    {
        HOST_INIT_MMC_POWER();

        /* card power off */
        HOST_ENABLE_MMC_POWER(card->userConfig.powerPolarity);
        /* Delay some time to make card stable. */
        microseconds_delay(card->userConfig.powerDownDelay_US);
        /* card power on */
        HOST_ENABLE_MMC_POWER(!card->userConfig.powerPolarity);
        /* Delay some time to make card stable. */
        microseconds_delay(card->userConfig.powerUpDelay_US);
    }
    if (card->userConfig.switch1V8)
    {
        HOST_INIT_MMC_VSEL();
        HOST_SWITCH_VOLTAGE180V(card->host.base, true);
    }

    /* set DATA bus width */
    HOST_SET_CARD_BUS_WIDTH(card->host.base, kHOST_DATABUSWIDTH1BIT);
    /* Set clock to 400KHz. */
    card->busClock_Hz = HOST_SET_CARD_CLOCK(card->host.base, card->host.sourceClock_Hz, SDMMC_CLOCK_400KHZ);
    /* get host capability first */
    GET_HOST_CAPABILITY(card->host.base, &(card->host.capability));

    /* Send CMD0 to reset the bus */
    if (kStatus_Success != MMC_GoIdle(card))
    {
        return kStatus_SDMMC_GoIdleFailed;
    }

    /* Hand-shaking with card to validata the voltage range Host first sending its expected
       information.*/
    if (kStatus_Success != MMC_BL_SendOperationCondition(card, 0U))
    {
        return kStatus_SDMMC_HandShakeOperationConditionFailed;
    }

    /* switch the host voltage which the card can support */
    // According to spec, do not switch the voltage after MMC power-up.
    // if (kStatus_Success != MMC_SwitchVoltage(card, &opcode))
    //{
    //    return kStatus_SDMMC_HandShakeOperationConditionFailed;
    //}

    /* Get host's access mode. */
    if (card->host.capability.maxBlockLength >= FSL_SDMMC_DEFAULT_BLOCK_SIZE)
    {
        opcode |= kMMC_AccessModeSector << MMC_OCR_ACCESS_MODE_SHIFT;
    }
    else
    {
        opcode |= kMMC_AccessModeSector << MMC_OCR_ACCESS_MODE_SHIFT;
    }

    opcode |= MMC_OCR_V270TO360_MASK;
    if (card->userConfig.switch1V8)
    {
        opcode |= MMC_OCR_V170TO195_MASK;
    }

    if (kStatus_Success != MMC_BL_SendOperationCondition(card, opcode))
    {
        return kStatus_SDMMC_HandShakeOperationConditionFailed;
    }

    /* Get card CID */
    if (kStatus_Success != MMC_AllSendCid(card))
    {
        return kStatus_SDMMC_AllSendCidFailed;
    }

    /* Set the card relative address */
    if (kStatus_Success != MMC_SetRelativeAddress(card))
    {
        return kStatus_SDMMC_SetRelativeAddressFailed;
    }

    /* Get the CSD register content */
    if (kStatus_Success != MMC_SendCsd(card))
    {
        return kStatus_SDMMC_SendCsdFailed;
    }

    /* Set to maximum speed in normal mode. */
    MMC_SetMaxFrequency(card);

    /* Send CMD7 with the card's relative address to place the card in transfer state. Puts current selected card in
    transfer state. */
    if (kStatus_Success != MMC_SelectCard(card, true))
    {
        return kStatus_SDMMC_SelectCardFailed;
    }

    /* Get Extended CSD register content. */
    if (kStatus_Success != MMC_SendExtendedCsd(card, NULL, 0U))
    {
        return kStatus_SDMMC_SendExtendedCsdFailed;
    }

    /* set block size */
    if (kStatus_Success != MMC_SetBlockSize(card, FSL_SDMMC_DEFAULT_BLOCK_SIZE))
    {
        return kStatus_SDMMC_SetCardBlockSizeFailed;
    }

    /* switch to host support speed mode, then switch MMC data bus width and select power class */
    error = MMC_BL_SelectBusTiming(card);
    if (error != kStatus_Success)
    {
        return error;
    }

    /* switch power class */
    // For low power, always use the default power classs.
    // if (kStatus_Success != MMC_SetPowerClass(card))
    //{
    //    return kStatus_SDMMC_SetPowerClassFail;
    //}

    /* Set to max erase unit size */
    if (kStatus_Success != MMC_SetMaxEraseUnitSize(card))
    {
        return kStatus_SDMMC_EnableHighCapacityEraseFailed;
    }

    /* Set card default to access non-boot partition */
    card->currentPartition = kMMC_AccessPartitionUserAera;

    return kStatus_Success;
}

bool MMC_CheckReadOnly(mmc_card_t *card)
{
    assert(card);

    return ((card->csd.flags & kMMC_CsdPermanentWriteProtectFlag) ||
            (card->csd.flags & kMMC_CsdTemporaryWriteProtectFlag));
}

status_t MMC_SelectPartition(mmc_card_t *card, mmc_access_partition_t partitionNumber)
{
    assert(card);

    uint8_t bootConfig;
    mmc_extended_csd_config_t extendedCsdconfig;

    bootConfig = card->extendedCsd.partitionConfig;
    bootConfig &= ~MMC_BOOT_CONFIG_PARTITION_ACCESS_MASK;
    bootConfig |= ((uint32_t)partitionNumber << MMC_BOOT_CONFIG_PARTITION_ACCESS_SHIFT);

    extendedCsdconfig.accessMode = kMMC_ExtendedCsdAccessModeWriteBits;
    extendedCsdconfig.ByteIndex = kMMC_ExtendedCsdIndexBootConfig;
    extendedCsdconfig.ByteValue = bootConfig;
    extendedCsdconfig.commandSet = kMMC_CommandSetStandard;
    if (kStatus_Success != MMC_SetExtendedCsdConfig(card, &extendedCsdconfig))
    {
        return kStatus_SDMMC_ConfigureExtendedCsdFailed;
    }

    /* Save current configuration. */
    card->extendedCsd.partitionConfig = bootConfig;
    card->currentPartition = partitionNumber;

    return kStatus_Success;
}

status_t MMC_ReadBlocks(mmc_card_t *card, uint8_t *buffer, uint32_t startBlock, uint32_t blockCount)
{
    assert(card);
    assert(buffer);
    assert(blockCount);

    uint32_t blockCountOneTime; /* The block count can be erased in one time sending READ_BLOCKS command. */
    uint32_t blockDone;         /* The blocks has been read. */
    uint32_t blockLeft;         /* Left blocks to be read. */
    uint8_t *nextBuffer;
    status_t error = kStatus_Success;

    blockLeft = blockCount;
    blockDone = 0U;
    if (kStatus_Success != MMC_CheckBlockRange(card, startBlock, blockCount))
    {
        return kStatus_InvalidArgument;
    }

    while (blockLeft)
    {
        if (blockLeft > card->host.capability.maxBlockCount)
        {
            blockLeft = blockLeft - card->host.capability.maxBlockCount;
            blockCountOneTime = card->host.capability.maxBlockCount;
        }
        else
        {
            blockCountOneTime = blockLeft;
            blockLeft = 0U;
        }

        nextBuffer = (buffer + blockDone * FSL_SDMMC_DEFAULT_BLOCK_SIZE);
        error = MMC_Read(card, nextBuffer, (startBlock + blockDone), FSL_SDMMC_DEFAULT_BLOCK_SIZE, blockCount);
        if (error != kStatus_Success)
        {
            return error;
        }

        blockDone += blockCountOneTime;
    }

    return error;
}

status_t MMC_WriteBlocks(mmc_card_t *card, const uint8_t *buffer, uint32_t startBlock, uint32_t blockCount)
{
    assert(card);
    assert(buffer);
    assert(blockCount);

    uint32_t blockCountOneTime;
    uint32_t blockLeft;
    uint32_t blockDone;
    const uint8_t *nextBuffer;
    status_t error = kStatus_Success;

    blockLeft = blockCount;
    blockDone = 0U;
    if (kStatus_Success != MMC_CheckBlockRange(card, startBlock, blockCount))
    {
        return kStatus_InvalidArgument;
    }

    while (blockLeft)
    {
        if (blockLeft > card->host.capability.maxBlockCount)
        {
            blockLeft = blockLeft - card->host.capability.maxBlockCount;
            blockCountOneTime = card->host.capability.maxBlockCount;
        }
        else
        {
            blockCountOneTime = blockLeft;
            blockLeft = 0U;
        }

        nextBuffer = (buffer + blockDone * FSL_SDMMC_DEFAULT_BLOCK_SIZE);
        error = MMC_Write(card, nextBuffer, (startBlock + blockDone), FSL_SDMMC_DEFAULT_BLOCK_SIZE, blockCount);
        if (error != kStatus_Success)
        {
            return error;
        }

        blockDone += blockCountOneTime;
    }

    return error;
}

status_t MMC_EraseGroups(mmc_card_t *card, uint32_t startGroup, uint32_t endGroup)
{
    assert(card);
    assert(card->host.transfer);

    uint32_t startGroupAddress;
    uint32_t endGroupAddress;
    HOST_COMMAND command = { 0 };
    HOST_TRANSFER content = { 0 };

    if (kStatus_Success != MMC_CheckEraseGroupRange(card, startGroup, endGroup))
    {
        return kStatus_InvalidArgument;
    }

    /* Wait for the card's buffer to be not full to write to improve the write performance. */
    while ((GET_HOST_STATUS(card->host.base) & CARD_DATA0_STATUS_MASK) != CARD_DATA0_NOT_BUSY)
    {
    }

    /* Calculate the start group address and end group address */
    startGroupAddress = startGroup;
    endGroupAddress = endGroup;
    if (card->flags & kMMC_SupportHighCapacityFlag)
    {
        /* The implementation of a higher than 2GB of density of memory will not be backwards compatible with the
        lower densities.First of all the address argument for higher than 2GB of density of memory is changed to
        be sector address (512B sectors) instead of byte address */
        startGroupAddress = (startGroupAddress * (card->eraseGroupBlocks));
        endGroupAddress = (endGroupAddress * (card->eraseGroupBlocks));
    }
    else
    {
        /* The address unit is byte when card capacity is lower than 2GB */
        startGroupAddress = (startGroupAddress * (card->eraseGroupBlocks) * FSL_SDMMC_DEFAULT_BLOCK_SIZE);
        endGroupAddress = (endGroupAddress * (card->eraseGroupBlocks) * FSL_SDMMC_DEFAULT_BLOCK_SIZE);
    }

    /* Set the start erase group address */
    command.index = kMMC_EraseGroupStart;
    command.argument = startGroupAddress;
    command.responseType = kCARD_ResponseTypeR1;
    command.responseErrorFlags = kSDMMC_R1ErrorAllFlag;

    content.command = &command;
    content.data = NULL;
    if (kStatus_Success != MMC_Transfer(card, &content, 0U))
    {
        return kStatus_SDMMC_TransferFailed;
    }

    /* Set the end erase group address */
    command.index = kMMC_EraseGroupEnd;
    command.argument = endGroupAddress;

    content.command = &command;
    content.data = NULL;
    if (kStatus_Success != MMC_Transfer(card, &content, 0U))
    {
        return kStatus_SDMMC_TransferFailed;
    }

    /* Start the erase process */
    command.index = kSDMMC_Erase;
    command.argument = 0U;
    command.responseType = kCARD_ResponseTypeR1b;
    command.responseErrorFlags = kSDMMC_R1ErrorAllFlag;

    content.command = &command;
    content.data = NULL;
    if (kStatus_Success != MMC_Transfer(card, &content, 0U))
    {
        return kStatus_SDMMC_TransferFailed;
    }

    return kStatus_Success;
}

status_t MMC_SetBootConfig(mmc_card_t *card, const mmc_boot_config_t *config)
{
    assert(card);
    assert(config);

    uint8_t bootParameter;
    mmc_extended_csd_config_t extendedCsdconfig;

    /* Set the BOOT_CONFIG field of Extended CSD */
    bootParameter = card->extendedCsd.partitionConfig;
    bootParameter &= ~(MMC_BOOT_CONFIG_ACK_MASK);
    bootParameter &= ~(MMC_BOOT_CONFIG_PARTITION_ENABLE_MASK);
    bootParameter |= ((config->enableBootAck ? 1U : 0U) << MMC_BOOT_CONFIG_ACK_SHIFT);
    bootParameter |= ((uint32_t)(config->bootPartition) << MMC_BOOT_CONFIG_PARTITION_ENABLE_SHIFT);

    extendedCsdconfig.accessMode = kMMC_ExtendedCsdAccessModeWriteBits;
    extendedCsdconfig.ByteIndex = kMMC_ExtendedCsdIndexBootConfig;
    extendedCsdconfig.ByteValue = bootParameter;
    extendedCsdconfig.commandSet = kMMC_CommandSetStandard;
    if (kStatus_Success != MMC_SetExtendedCsdConfig(card, &extendedCsdconfig))
    {
        return kStatus_SDMMC_ConfigureExtendedCsdFailed;
    }

    card->extendedCsd.partitionConfig = bootParameter;

    /*Set BOOT_BUS_WIDTH in Extended CSD */
    bootParameter = card->extendedCsd.bootDataBusWidth;
    bootParameter &= ~(MMC_BOOT_BUS_WIDTH_RESET_MASK);
    bootParameter &= ~(MMC_BOOT_BUS_WIDTH_WIDTH_MASK);
    bootParameter &= ~(MMC_BOOT_MODE_MASK);
    bootParameter |= ((config->retainBootBusWidth ? 1U : 0U) << MMC_BOOT_BUS_WIDTH_RESET_SHIFT);
    bootParameter |= ((uint32_t)(config->bootBusWidth) << MMC_BOOT_BUS_WIDTH_WIDTH_SHIFT);
    bootParameter |= ((uint32_t)(config->bootMode) << MMC_BOOT_MODE_SHIFT);

    extendedCsdconfig.accessMode = kMMC_ExtendedCsdAccessModeWriteBits;
    extendedCsdconfig.ByteIndex = kMMC_ExtendedCsdIndexBootBusWidth;
    extendedCsdconfig.ByteValue = bootParameter;
    if (kStatus_Success != MMC_SetExtendedCsdConfig(card, &extendedCsdconfig))
    {
        return kStatus_SDMMC_ConfigureBootFailed;
    }

    card->extendedCsd.bootDataBusWidth = bootParameter;

    return kStatus_Success;
}
