/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_sdio.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @brief define the tuple number will be read during init */
#define SDIO_COMMON_CIS_TUPLE_NUM (3U)
/*! @brief SDIO retry times */
#define SDIO_RETRY_TIMES (1000U)
/*!@brief power reset delay */
#define SDIO_POWER_RESET_DELAY (500U)
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief probe bus voltage.
 * @param card Card descriptor.
 */
static status_t SDIO_ProbeBusVoltage(sdio_card_t *card);

/*!
 * @brief send card operation condition
 * @param card Card descriptor.
 * @param command argment
 * @param accept1V8 flag indicate card acccpt 1v8 switch or not
 *  argument = 0U , means to get the operation condition
 *  argument !=0 , set the operation condition register
 */
static status_t SDIO_SendOperationCondition(sdio_card_t *card, uint32_t argument, uint32_t *accept1V8);

/*!
 * @brief card Send relative address
 * @param card Card descriptor.
 */
static status_t SDIO_SendRca(sdio_card_t *card);

/*!
 * @brief card select card
 * @param card Card descriptor.
 * @param select/diselect flag
 */
static inline status_t SDIO_SelectCard(sdio_card_t *card, bool isSelected);

/*!
 * @brief card go idle
 * @param card Card descriptor.
 */
static inline status_t SDIO_GoIdle(sdio_card_t *card);

/*!
 * @brief decode CIS
 * @param card Card descriptor.
 * @param func number
 * @param data buffer pointer
 * @param tuple code
 * @param tuple link
 */
static status_t SDIO_DecodeCIS(
    sdio_card_t *card, sdio_func_num_t func, uint8_t *dataBuffer, uint32_t tplCode, uint32_t tplLink);

/*!
 * @brief switch to the maxium support bus width, depend on the host and card's capability.
 * @param card Card descriptor.
 */
static status_t SDIO_SetMaxDataBusWidth(sdio_card_t *card);

#if SDMMCHOST_SUPPORT_SDR104 || SDMMCHOST_SUPPORT_SDR50
/*!
 * @brief sdio card excute tuning.
 * @param card Card descriptor.
 */
static status_t SDIO_ExecuteTuning(sdio_card_t *card);
#endif
/*******************************************************************************
 * Variables
 ******************************************************************************/
/* define the tuple list */
static const uint32_t s_tupleList[SDIO_COMMON_CIS_TUPLE_NUM] = {
    SDIO_TPL_CODE_MANIFID,
    SDIO_TPL_CODE_FUNCID,
    SDIO_TPL_CODE_FUNCE,
};

/*******************************************************************************
 * Code
 ******************************************************************************/
static inline status_t SDIO_SelectCard(sdio_card_t *card, bool isSelected)
{
    assert(card);

    return SDMMC_SelectCard(card->host, card->relativeAddress, isSelected);
}

static inline status_t SDIO_GoIdle(sdio_card_t *card)
{
    assert(card);

    return SDMMC_GoIdle(card->host);
}

#if SDMMCHOST_SUPPORT_SDR104 || SDMMCHOST_SUPPORT_SDR50
static status_t SDIO_SwitchVoltage(sdio_card_t *card, sdmmc_operation_voltage_t voltage)
{
    assert(card);

    sdmmchost_transfer_t content = {0};
    sdmmchost_cmd_t command      = {0};
    status_t error               = kStatus_Success;

    command.index        = kSD_VoltageSwitch;
    command.argument     = 0U;
    command.responseType = kCARD_ResponseTypeR1;

    content.command = &command;
    content.data    = NULL;
    if (kStatus_Success != SDMMCHOST_TransferFunction(card->host, &content))
    {
        return kStatus_SDMMC_TransferFailed;
    }

    /* check data line and cmd line status */
    if (SDMMCHOST_GetSignalLineStatus(card->host, kSDMMC_SignalLineData0 | kSDMMC_SignalLineData1 |
                                                      kSDMMC_SignalLineData2 | kSDMMC_SignalLineData3) != 0U)
    {
        return kStatus_SDMMC_SwitchVoltageFail;
    }

    if (card->usrParam.ioVoltage->func != NULL)
    {
        card->usrParam.ioVoltage->func(voltage);
    }
    else
    {
        /* host switch to 1.8V */
        SDMMCHOST_SwitchToVoltage(card->host, (uint32_t)voltage);
    }

    SDMMC_OSADelay(100U);

    /*enable force clock on*/
    SDMMCHOST_ForceClockOn(card->host, true);
    /* dealy 1ms,not exactly correct when use while */
    SDMMC_OSADelay(10U);
    /*disable force clock on*/
    SDMMCHOST_ForceClockOn(card->host, false);

    /* check data line and cmd line status */
    if (SDMMCHOST_GetSignalLineStatus(card->host, kSDMMC_SignalLineData0 | kSDMMC_SignalLineData1 |
                                                      kSDMMC_SignalLineData2 | kSDMMC_SignalLineData3) == 0U)
    {
        error = kStatus_SDMMC_SwitchVoltageFail;
        /* power reset the card */
        SDIO_SetCardPower(card, false);
        SDIO_SetCardPower(card, true);
        /* re-check the data line status */
        if (SDMMCHOST_GetSignalLineStatus(card->host, kSDMMC_SignalLineData0 | kSDMMC_SignalLineData1 |
                                                          kSDMMC_SignalLineData2 | kSDMMC_SignalLineData3) != 0U)
        {
            error = kStatus_SDMMC_SwitchVoltage18VFail33VSuccess;
            SDMMC_LOG("\r\nNote: Current card support 1.8V, but board don't support, so sdmmc switch back to 3.3V.");
        }
        else
        {
            SDMMC_LOG(
                "\r\nError: Current card support 1.8V, but board don't support, sdmmc tried to switch back\
                    to 3.3V, but failed, please check board setting.");
        }
    }

    return error;
}

static status_t SDIO_ExecuteTuning(sdio_card_t *card)
{
    assert(card);

    return SDMMCHOST_ExecuteTuning(card->host, kSD_SendTuningBlock, (uint32_t *)card->internalBuffer, 64U);
}
#endif

static status_t SDIO_SendRca(sdio_card_t *card)
{
    assert(card);

    uint32_t i = FSL_SDMMC_MAX_CMD_RETRIES;

    sdmmchost_transfer_t content = {0};
    sdmmchost_cmd_t command      = {0};

    command.index              = kSDIO_SendRelativeAddress;
    command.argument           = 0U;
    command.responseType       = kCARD_ResponseTypeR6;
    command.responseErrorFlags = kSDIO_StatusR6Error | kSDIO_StatusIllegalCmd | kSDIO_StatusCmdCRCError;

    content.command = &command;
    content.data    = NULL;

    while (--i)
    {
        if (kStatus_Success == SDMMCHOST_TransferFunction(card->host, &content))
        {
            /* check illegal state and cmd CRC error, may be the voltage or clock not stable, retry the cmd*/
            if (command.response[0U] & (kSDIO_StatusIllegalCmd | kSDIO_StatusCmdCRCError))
            {
                continue;
            }

            card->relativeAddress = (command.response[0U] >> 16U);

            return kStatus_Success;
        }
    }

    return kStatus_SDMMC_TransferFailed;
}

status_t SDIO_CardInActive(sdio_card_t *card)
{
    assert(card);

    return SDMMC_SetCardInactive(card->host);
}

static status_t SDIO_SendOperationCondition(sdio_card_t *card, uint32_t argument, uint32_t *accept1V8)
{
    assert(card);

    sdmmchost_transfer_t content = {0U};
    sdmmchost_cmd_t command      = {0U};
    uint32_t i                   = SDIO_RETRY_TIMES;

    command.index        = kSDIO_SendOperationCondition;
    command.argument     = argument;
    command.responseType = kCARD_ResponseTypeR4;

    content.command = &command;
    content.data    = NULL;

    while (--i)
    {
        if (kStatus_Success != SDMMCHOST_TransferFunction(card->host, &content) || (command.response[0U] == 0U))
        {
            continue;
        }

        /* if argument equal 0, then should check and save the info */
        if ((argument != 0U) && ((command.response[0U] & SDMMC_MASK(kSDIO_OcrPowerUpBusyFlag)) == 0U))
        {
            continue;
        }
        else if (argument == 0U)
        {
            /* check the io number and ocr value */
            if ((((command.response[0U] & SDIO_OCR_IO_NUM_MASK) >> kSDIO_OcrIONumber) == 0U) ||
                ((command.response[0U] & 0xFFFFFFU) == 0U))
            {
                return kStatus_Fail;
            }

            break;
        }
        else
        {
            /* check if memory present */
            if ((command.response[0U] & SDMMC_MASK(kSDIO_OcrMemPresent)) == SDMMC_MASK(kSDIO_OcrMemPresent))
            {
                card->memPresentFlag = true;
            }
            /* save the io number */
            card->ioTotalNumber = (command.response[0U] & SDIO_OCR_IO_NUM_MASK) >> kSDIO_OcrIONumber;
            /* save the operation condition */
            card->ocr = command.response[0U] & 0xFFFFFFU;
            if (accept1V8 != NULL)
            {
                *accept1V8 = command.response[0U] & 0x1000000U;
            }

            break;
        }
    }

    return ((i != 0U) ? kStatus_Success : kStatus_Fail);
}

status_t SDIO_IO_Write_Direct(sdio_card_t *card, sdio_func_num_t func, uint32_t regAddr, uint8_t *data, bool raw)
{
    assert(card);
    assert(func <= kSDIO_FunctionNum7);

    sdmmchost_transfer_t content = {0U};
    sdmmchost_cmd_t command      = {0U};

    command.index    = kSDIO_RWIODirect;
    command.argument = (func << SDIO_CMD_ARGUMENT_FUNC_NUM_POS) |
                       ((regAddr & SDIO_CMD_ARGUMENT_REG_ADDR_MASK) << SDIO_CMD_ARGUMENT_REG_ADDR_POS) |
                       (1U << SDIO_CMD_ARGUMENT_RW_POS) | ((raw ? 1U : 0U) << SDIO_DIRECT_CMD_ARGUMENT_RAW_POS) |
                       (*data & SDIO_DIRECT_CMD_DATA_MASK);
    command.responseType       = kCARD_ResponseTypeR5;
    command.responseErrorFlags = (kSDIO_StatusCmdCRCError | kSDIO_StatusIllegalCmd | kSDIO_StatusError |
                                  kSDIO_StatusFunctionNumError | kSDIO_StatusOutofRange);

    content.command = &command;
    content.data    = NULL;

    if (kStatus_Success != SDMMCHOST_TransferFunction(card->host, &content))
    {
        return kStatus_SDMMC_TransferFailed;
    }

    /* read data from response */
    *data = command.response[0U] & SDIO_DIRECT_CMD_DATA_MASK;

    return kStatus_Success;
}

status_t SDIO_IO_Read_Direct(sdio_card_t *card, sdio_func_num_t func, uint32_t regAddr, uint8_t *data)
{
    assert(card);
    assert(func <= kSDIO_FunctionNum7);

    sdmmchost_transfer_t content = {0U};
    sdmmchost_cmd_t command      = {0U};

    command.index    = kSDIO_RWIODirect;
    command.argument = (func << SDIO_CMD_ARGUMENT_FUNC_NUM_POS) |
                       ((regAddr & SDIO_CMD_ARGUMENT_REG_ADDR_MASK) << SDIO_CMD_ARGUMENT_REG_ADDR_POS);
    command.responseType       = kCARD_ResponseTypeR5;
    command.responseErrorFlags = (kSDIO_StatusCmdCRCError | kSDIO_StatusIllegalCmd | kSDIO_StatusError |
                                  kSDIO_StatusFunctionNumError | kSDIO_StatusOutofRange);

    content.command = &command;
    content.data    = NULL;

    if (kStatus_Success != SDMMCHOST_TransferFunction(card->host, &content))
    {
        return kStatus_SDMMC_TransferFailed;
    }

    /* read data from response */
    *data = command.response[0U] & SDIO_DIRECT_CMD_DATA_MASK;

    return kStatus_Success;
}

status_t SDIO_IO_RW_Direct(sdio_card_t *card,
                           sdio_io_direction_t direction,
                           sdio_func_num_t func,
                           uint32_t regAddr,
                           uint8_t dataIn,
                           uint8_t *dataOut)
{
    assert(card);
    assert(func <= kSDIO_FunctionNum7);

    sdmmchost_transfer_t content = {0U};
    sdmmchost_cmd_t command      = {0U};

    command.index    = kSDIO_RWIODirect;
    command.argument = (func << SDIO_CMD_ARGUMENT_FUNC_NUM_POS) |
                       ((regAddr & SDIO_CMD_ARGUMENT_REG_ADDR_MASK) << SDIO_CMD_ARGUMENT_REG_ADDR_POS);

    if ((dataOut != NULL) && (direction == kSDIO_IOWrite))
    {
        command.argument |= (1U << SDIO_CMD_ARGUMENT_RW_POS) | (1U << SDIO_DIRECT_CMD_ARGUMENT_RAW_POS);
    }

    if (direction == kSDIO_IOWrite)
    {
        command.argument |= dataIn & SDIO_DIRECT_CMD_DATA_MASK;
    }

    command.responseType       = kCARD_ResponseTypeR5;
    command.responseErrorFlags = (kSDIO_StatusCmdCRCError | kSDIO_StatusIllegalCmd | kSDIO_StatusError |
                                  kSDIO_StatusFunctionNumError | kSDIO_StatusOutofRange);

    command.responseType       = kCARD_ResponseTypeR5;
    command.responseErrorFlags = (kSDIO_StatusCmdCRCError | kSDIO_StatusIllegalCmd | kSDIO_StatusError |
                                  kSDIO_StatusFunctionNumError | kSDIO_StatusOutofRange);

    content.command = &command;
    content.data    = NULL;

    if (kStatus_Success != SDMMCHOST_TransferFunction(card->host, &content))
    {
        return kStatus_SDMMC_TransferFailed;
    }

    if (dataOut != NULL)
    {
        /* read data from response */
        *dataOut = command.response[0U] & SDIO_DIRECT_CMD_DATA_MASK;
    }

    return kStatus_Success;
}

status_t SDIO_IO_Write_Extended(
    sdio_card_t *card, sdio_func_num_t func, uint32_t regAddr, uint8_t *buffer, uint32_t count, uint32_t flags)
{
    assert(card);
    assert(buffer);
    assert(func <= kSDIO_FunctionNum7);

    sdmmchost_transfer_t content = {0U};
    sdmmchost_cmd_t command      = {0U};
    sdmmchost_data_t data        = {0U};
    bool blockMode               = false;
    bool opCode                  = false;

    /* check if card support block mode */
    if ((card->cccrflags & kSDIO_CCCRSupportMultiBlock) && (flags & SDIO_EXTEND_CMD_BLOCK_MODE_MASK))
    {
        blockMode = true;
    }

    if (flags & SDIO_EXTEND_CMD_OP_CODE_MASK)
    {
        opCode = true;
    }

    /* check the byte size counter in non-block mode
     * so you need read CIS for each function first,before you do read/write
     */
    if (!blockMode)
    {
        if ((func == kSDIO_FunctionNum0) && (card->commonCIS.fn0MaxBlkSize != 0U) &&
            (count > card->commonCIS.fn0MaxBlkSize))
        {
            return kStatus_SDMMC_SDIO_InvalidArgument;
        }
        else if ((func != kSDIO_FunctionNum0) && (card->funcCIS[func - 1U].ioMaxBlockSize != 0U) &&
                 (count > card->funcCIS[func - 1U].ioMaxBlockSize))
        {
            return kStatus_SDMMC_SDIO_InvalidArgument;
        }
    }

    command.index    = kSDIO_RWIOExtended;
    command.argument = (func << SDIO_CMD_ARGUMENT_FUNC_NUM_POS) |
                       ((regAddr & SDIO_CMD_ARGUMENT_REG_ADDR_MASK) << SDIO_CMD_ARGUMENT_REG_ADDR_POS) |
                       (1U << SDIO_CMD_ARGUMENT_RW_POS) | (count & SDIO_EXTEND_CMD_COUNT_MASK) |
                       ((blockMode ? 1 : 0) << SDIO_EXTEND_CMD_ARGUMENT_BLOCK_MODE_POS |
                        ((opCode ? 1 : 0) << SDIO_EXTEND_CMD_ARGUMENT_OP_CODE_POS));
    command.responseType       = kCARD_ResponseTypeR5;
    command.responseErrorFlags = (kSDIO_StatusCmdCRCError | kSDIO_StatusIllegalCmd | kSDIO_StatusError |
                                  kSDIO_StatusFunctionNumError | kSDIO_StatusOutofRange);

    if (blockMode)
    {
        if (func == kSDIO_FunctionNum0)
        {
            data.blockSize = card->io0blockSize;
        }
        else
        {
            data.blockSize = card->ioFBR[func - 1U].ioBlockSize;
        }
        data.blockCount = count;
    }
    else
    {
        data.blockSize  = count;
        data.blockCount = 1U;
    }
    data.txData = (uint32_t *)buffer;

    content.command = &command;
    content.data    = &data;

    if (kStatus_Success != SDMMCHOST_TransferFunction(card->host, &content))
    {
        return kStatus_SDMMC_TransferFailed;
    }

    return kStatus_Success;
}

status_t SDIO_IO_Read_Extended(
    sdio_card_t *card, sdio_func_num_t func, uint32_t regAddr, uint8_t *buffer, uint32_t count, uint32_t flags)
{
    assert(card);
    assert(buffer);
    assert(func <= kSDIO_FunctionNum7);

    sdmmchost_transfer_t content = {0U};
    sdmmchost_cmd_t command      = {0U};
    sdmmchost_data_t data        = {0U};
    bool blockMode               = false;
    bool opCode                  = false;

    /* check if card support block mode */
    if ((card->cccrflags & kSDIO_CCCRSupportMultiBlock) && (flags & SDIO_EXTEND_CMD_BLOCK_MODE_MASK))
    {
        blockMode = true;
    }

    /* op code =0 : read/write to fixed addr
     *  op code =1 :read/write addr incrementing
     */
    if (flags & SDIO_EXTEND_CMD_OP_CODE_MASK)
    {
        opCode = true;
    }

    /* check the byte size counter in non-block mode
     * so you need read CIS for each function first,before you do read/write
     */
    if (!blockMode)
    {
        if ((func == kSDIO_FunctionNum0) && (card->commonCIS.fn0MaxBlkSize != 0U) &&
            (count > card->commonCIS.fn0MaxBlkSize))
        {
            return kStatus_SDMMC_SDIO_InvalidArgument;
        }
        else if ((func != kSDIO_FunctionNum0) && (card->funcCIS[func - 1U].ioMaxBlockSize != 0U) &&
                 (count > card->funcCIS[func - 1U].ioMaxBlockSize))
        {
            return kStatus_SDMMC_SDIO_InvalidArgument;
        }
    }

    command.index    = kSDIO_RWIOExtended;
    command.argument = (func << SDIO_CMD_ARGUMENT_FUNC_NUM_POS) |
                       ((regAddr & SDIO_CMD_ARGUMENT_REG_ADDR_MASK) << SDIO_CMD_ARGUMENT_REG_ADDR_POS) |
                       (count & SDIO_EXTEND_CMD_COUNT_MASK) |
                       ((blockMode ? 1U : 0U) << SDIO_EXTEND_CMD_ARGUMENT_BLOCK_MODE_POS |
                        ((opCode ? 1U : 0U) << SDIO_EXTEND_CMD_ARGUMENT_OP_CODE_POS));
    command.responseType       = kCARD_ResponseTypeR5;
    command.responseErrorFlags = (kSDIO_StatusCmdCRCError | kSDIO_StatusIllegalCmd | kSDIO_StatusError |
                                  kSDIO_StatusFunctionNumError | kSDIO_StatusOutofRange);

    if (blockMode)
    {
        if (func == kSDIO_FunctionNum0)
        {
            data.blockSize = card->io0blockSize;
        }
        else
        {
            data.blockSize = card->ioFBR[func - 1U].ioBlockSize;
        }
        data.blockCount = count;
    }
    else
    {
        data.blockSize  = count;
        data.blockCount = 1U;
    }
    data.rxData = (uint32_t *)buffer;

    content.command = &command;
    content.data    = &data;

    if (kStatus_Success != SDMMCHOST_TransferFunction(card->host, &content))
    {
        return kStatus_SDMMC_TransferFailed;
    }

    return kStatus_Success;
}

status_t SDIO_IO_Transfer(sdio_card_t *card,
                          sdio_command_t cmd,
                          uint32_t argument,
                          uint32_t blockSize,
                          uint8_t *txData,
                          uint8_t *rxData,
                          uint16_t dataSize,
                          uint32_t *response)
{
    assert(card != NULL);

    uint32_t actualSize          = dataSize;
    sdmmchost_transfer_t content = {0U};
    sdmmchost_cmd_t command      = {0U};
    sdmmchost_data_t data        = {0U};
    uint32_t i                   = SDIO_RETRY_TIMES;
    uint32_t *dataAddr           = (uint32_t *)(txData == NULL ? rxData : txData);
    uint8_t *alignBuffer         = FSL_SDMMC_CARD_INTERNAL_BUFFER_ALIGN_ADDR(card->internalBuffer);

    if ((dataSize != 0U) && (txData != NULL) && (rxData != NULL))
    {
        return kStatus_InvalidArgument;
    }

    command.index              = cmd;
    command.argument           = argument;
    command.responseType       = kCARD_ResponseTypeR5;
    command.responseErrorFlags = (kSDIO_StatusCmdCRCError | kSDIO_StatusIllegalCmd | kSDIO_StatusError |
                                  kSDIO_StatusFunctionNumError | kSDIO_StatusOutofRange);
    content.command            = &command;
    content.data               = NULL;

    if (dataSize)
    {
        /* if block size bigger than 1, then use block mode */
        if (argument & SDIO_EXTEND_CMD_BLOCK_MODE_MASK)
        {
            if (dataSize % blockSize != 0)
            {
                actualSize = ((dataSize / blockSize) + 1) * blockSize;
            }

            data.blockCount = actualSize / blockSize;
            data.blockSize  = blockSize;
        }
        else
        {
            data.blockCount = 1;
            data.blockSize  = dataSize;
        }
        /* if data buffer address can not meet host controller internal DMA requirement, sdio driver will try to use
         * internal align buffer if data size is not bigger than internal buffer size,
         * Align address transfer always can get a better performance, so if you want sdio driver make buffer address
         * align, you should
         * redefine the SDMMC_GLOBAL_BUFFER_SIZE macro to a value which is big enough for your application.
         */
        if (((uint32_t)dataAddr & (SDMMCHOST_DMA_DESCRIPTOR_BUFFER_ALIGN_SIZE - 1U)) &&
            (actualSize <= FSL_SDMMC_DEFAULT_BLOCK_SIZE) && (!card->noInternalAlign))
        {
            dataAddr = (uint32_t *)alignBuffer;
            memset(alignBuffer, 0U, actualSize);
            if (txData)
            {
                memcpy(alignBuffer, txData, dataSize);
            }
        }

        if (rxData)
        {
            data.rxData = dataAddr;
        }
        else
        {
            data.txData = dataAddr;
        }

        content.data = &data;
    }

    do
    {
        if (kStatus_Success == SDMMCHOST_TransferFunction(card->host, &content))
        {
            if ((rxData != NULL) && ((uint32_t)rxData & (SDMMCHOST_DMA_DESCRIPTOR_BUFFER_ALIGN_SIZE - 1U)) &&
                (actualSize <= FSL_SDMMC_DEFAULT_BLOCK_SIZE) && (!card->noInternalAlign))
            {
                memcpy(rxData, alignBuffer, dataSize);
            }

            if (response != NULL)
            {
                *response = command.response[0];
            }

            return kStatus_Success;
        }

    } while (i--);

    return kStatus_Fail;
}

status_t SDIO_GetCardCapability(sdio_card_t *card, sdio_func_num_t func)
{
    assert(card);
    assert(func <= kSDIO_FunctionNum7);

    uint8_t *tempBuffer = FSL_SDMMC_CARD_INTERNAL_BUFFER_ALIGN_ADDR(card->internalBuffer);
    uint32_t i          = 0U;

    memset(tempBuffer, 0U, SDIO_CCCR_REG_NUMBER);

    for (i = 0U; i <= SDIO_CCCR_REG_NUMBER; i++)
    {
        if (kStatus_Success !=
            SDIO_IO_RW_Direct(card, kSDIO_IORead, kSDIO_FunctionNum0, SDIO_FBR_BASE(func) + i, 0U, &tempBuffer[i]))
        {
            return kStatus_SDMMC_TransferFailed;
        }
    }

    switch (func)
    {
        case kSDIO_FunctionNum0:

            card->sdVersion    = tempBuffer[kSDIO_RegSDVersion];
            card->sdioVersion  = tempBuffer[kSDIO_RegCCCRSdioVer] >> 4U;
            card->cccrVersioin = tempBuffer[kSDIO_RegCCCRSdioVer] & 0xFU;
            /* continuous SPI interrupt */
            if (tempBuffer[kSDIO_RegBusInterface] & 0x40U)
            {
                card->cccrflags |= kSDIO_CCCRSupportContinuousSPIInt;
            }
            /* 8bit data bus */
            if (tempBuffer[kSDIO_RegBusInterface] & 0x4U)
            {
                card->cccrflags |= SDIO_CCCR_SUPPORT_8BIT_BUS;
            }

            /* card capability register */
            card->cccrflags |= (tempBuffer[kSDIO_RegCardCapability] & 0xDFU);
            /* master power control */
            if (tempBuffer[kSDIO_RegPowerControl] & 0x01U)
            {
                card->cccrflags |= kSDIO_CCCRSupportMasterPowerControl;
            }
            /* high speed flag */
            if (tempBuffer[kSDIO_RegBusSpeed] & 0x01U)
            {
                card->cccrflags |= SDIO_CCCR_SUPPORT_HIGHSPEED;
            }
            /* uhs mode flag */
            card->cccrflags |= (tempBuffer[kSDIO_RegUHSITimingSupport] & 7U) << 11U;
            /* driver type flag */
            card->cccrflags |= (tempBuffer[kSDIO_RegDriverStrength] & 7U) << 14U;
            /* low speed 4bit */
            if (tempBuffer[kSDIO_RegCardCapability] & 0x80U)
            {
                card->cccrflags |= kSDIO_CCCRSupportLowSpeed4Bit;
            }
            /* common CIS pointer */
            card->commonCISPointer = tempBuffer[kSDIO_RegCommonCISPointer] |
                                     (tempBuffer[kSDIO_RegCommonCISPointer + 1U] << 8U) |
                                     (tempBuffer[kSDIO_RegCommonCISPointer + 2U] << 16U);

            /* check card capability of support async interrupt */
            if ((tempBuffer[kSDIO_RegInterruptExtension] & SDIO_CCCR_ASYNC_INT_MASK) == SDIO_CCCR_ASYNC_INT_MASK)
            {
                card->cccrflags |= SDIO_CCCR_SUPPORT_ASYNC_INT;
            }

            break;

        case kSDIO_FunctionNum1:
        case kSDIO_FunctionNum2:
        case kSDIO_FunctionNum3:
        case kSDIO_FunctionNum4:
        case kSDIO_FunctionNum5:
        case kSDIO_FunctionNum6:
        case kSDIO_FunctionNum7:
            card->ioFBR[func - 1U].ioStdFunctionCode = tempBuffer[0U] & 0x0FU;
            card->ioFBR[func - 1U].ioExtFunctionCode = tempBuffer[1U];
            card->ioFBR[func - 1U].ioPointerToCIS = tempBuffer[9U] | (tempBuffer[10U] << 8U) | (tempBuffer[11U] << 16U);
            card->ioFBR[func - 1U].ioPointerToCSA =
                tempBuffer[12U] | (tempBuffer[13U] << 8U) | (tempBuffer[14U] << 16U);
            if (tempBuffer[2U] & 0x01U)
            {
                card->ioFBR[func - 1U].flags |= kSDIO_FBRSupportPowerSelection;
            }
            if (tempBuffer[0U] & 0x40U)
            {
                card->ioFBR[func - 1U].flags |= kSDIO_FBRSupportCSA;
            }

            break;

        default:
            break;
    }

    return kStatus_Success;
}

status_t SDIO_SetBlockSize(sdio_card_t *card, sdio_func_num_t func, uint32_t blockSize)
{
    assert(card);
    assert(func <= kSDIO_FunctionNum7);
    assert(blockSize <= SDIO_MAX_BLOCK_SIZE);

    uint8_t temp = 0U;

    /* check the block size for block mode
     * so you need read CIS for each function first,before you do read/write
     */
    if ((func == kSDIO_FunctionNum0) && (card->commonCIS.fn0MaxBlkSize != 0U) &&
        (blockSize > card->commonCIS.fn0MaxBlkSize))
    {
        return kStatus_SDMMC_SDIO_InvalidArgument;
    }
    else if ((func != kSDIO_FunctionNum0) && (card->funcCIS[func - 1U].ioMaxBlockSize != 0U) &&
             (blockSize > card->funcCIS[func - 1U].ioMaxBlockSize))
    {
        return kStatus_SDMMC_SDIO_InvalidArgument;
    }

    temp = blockSize & 0xFFU;

    if (kStatus_Success != SDIO_IO_RW_Direct(card, kSDIO_IOWrite, kSDIO_FunctionNum0,
                                             SDIO_FBR_BASE(func) + kSDIO_RegFN0BlockSizeLow, temp, &temp))
    {
        return kStatus_SDMMC_SetCardBlockSizeFailed;
    }

    temp = (blockSize >> 8U) & 0xFFU;

    if (kStatus_Success != SDIO_IO_RW_Direct(card, kSDIO_IOWrite, kSDIO_FunctionNum0,
                                             SDIO_FBR_BASE(func) + kSDIO_RegFN0BlockSizeHigh, temp, &temp))
    {
        return kStatus_SDMMC_SetCardBlockSizeFailed;
    }

    /* record the current block size */
    if (func == kSDIO_FunctionNum0)
    {
        card->io0blockSize = blockSize;
    }
    else
    {
        card->ioFBR[func - 1U].ioBlockSize = blockSize;
    }

    return kStatus_Success;
}

status_t SDIO_CardReset(sdio_card_t *card)
{
    return SDIO_IO_RW_Direct(card, kSDIO_IOWrite, kSDIO_FunctionNum0, kSDIO_RegIOAbort, 0x08U, NULL);
}

status_t SDIO_SetDataBusWidth(sdio_card_t *card, sdio_bus_width_t busWidth)
{
    assert(card);

    uint8_t regBusInterface = 0U;

    if (((busWidth == kSDIO_DataBus4Bit) && ((card->cccrflags & kSDIO_CCCRSupportHighSpeed) == 0U) &&
         ((card->cccrflags & kSDIO_CCCRSupportLowSpeed4Bit) == 0U)))
    {
        return kStatus_SDMMC_SDIO_InvalidArgument;
    }

#if SDMMCHOST_SUPPORT_8_BIT_WIDTH
    if ((((card->cccrflags & SDIO_CCCR_SUPPORT_8BIT_BUS) == 0U) ||
         ((card->usrParam.capability & kSDMMC_Support8BitWidth) == 0U)) &&
        (busWidth == kSDIO_DataBus8Bit))
    {
        return kStatus_SDMMC_SDIO_InvalidArgument;
    }
#else
    if (busWidth == kSDIO_DataBus8Bit)
    {
        return kStatus_SDMMC_SDIO_InvalidArgument;
    }
#endif

    /* load bus interface register */
    if (kStatus_Success !=
        SDIO_IO_RW_Direct(card, kSDIO_IORead, kSDIO_FunctionNum0, kSDIO_RegBusInterface, 0U, &regBusInterface))
    {
        return kStatus_SDMMC_TransferFailed;
    }
    /* set bus width */
    regBusInterface &= 0xFCU;
    regBusInterface |= busWidth;

    /* write to register */
    if (kStatus_Success != SDIO_IO_RW_Direct(card, kSDIO_IOWrite, kSDIO_FunctionNum0, kSDIO_RegBusInterface,
                                             regBusInterface, &regBusInterface))
    {
        return kStatus_SDMMC_TransferFailed;
    }

    if (busWidth == kSDIO_DataBus8Bit)
    {
        SDMMCHOST_SetCardBusWidth(card->host, kSDMMC_BusWdith8Bit);
    }
    else if (busWidth == kSDIO_DataBus4Bit)
    {
        SDMMCHOST_SetCardBusWidth(card->host, kSDMMC_BusWdith4Bit);
    }
    else
    {
        SDMMCHOST_SetCardBusWidth(card->host, kSDMMC_BusWdith1Bit);
    }

    return kStatus_Success;
}

static status_t SDIO_SetMaxDataBusWidth(sdio_card_t *card)
{
    sdio_bus_width_t busWidth = kSDIO_DataBus1Bit;

#if SDMMCHOST_SUPPORT_8_BIT_WIDTH
    if (((card->cccrflags & SDIO_CCCR_SUPPORT_8BIT_BUS) != 0U) &&
        (card->usrParam.capability & kSDMMC_Support8BitWidth) != 0U)
    {
        busWidth = kSDIO_DataBus8Bit;
    }
#endif
    /* switch data bus width */
    if (((card->cccrflags & kSDIO_CCCRSupportHighSpeed) || ((card->cccrflags & kSDIO_CCCRSupportLowSpeed4Bit) != 0U)) &&
        (busWidth == kSDIO_DataBus1Bit))
    {
        busWidth = kSDIO_DataBus4Bit;
    }

    return SDIO_SetDataBusWidth(card, busWidth);
}

status_t SDIO_SwitchToHighSpeed(sdio_card_t *card)
{
    assert(card);

    uint8_t temp        = 0U;
    uint32_t retryTimes = SDIO_RETRY_TIMES;
    status_t status     = kStatus_SDMMC_SDIO_SwitchHighSpeedFail;

    if (card->cccrflags & SDIO_CCCR_SUPPORT_HIGHSPEED)
    {
        if (kStatus_Success != SDIO_IO_RW_Direct(card, kSDIO_IORead, kSDIO_FunctionNum0, kSDIO_RegBusSpeed, 0U, &temp))
        {
            return kStatus_SDMMC_TransferFailed;
        }

        do
        {
            temp &= ~SDIO_CCCR_BUS_SPEED_MASK;
            temp |= SDIO_CCCR_ENABLE_HIGHSPEED_MODE;

            retryTimes--;
            /* enable high speed mode */
            if (kStatus_Success !=
                SDIO_IO_RW_Direct(card, kSDIO_IOWrite, kSDIO_FunctionNum0, kSDIO_RegBusSpeed, temp, &temp))
            {
                continue;
            }
            /* either EHS=0 and SHS=0 ,the card is still in default mode  */
            if ((temp & 0x03U) == 0x03U)
            {
                card->busClock_Hz = SDMMCHOST_SetCardClock(
                    card->host, FSL_SDMMC_CARD_MAX_BUS_FREQ(card->usrParam.maxFreq, SD_CLOCK_50MHZ));
                status = kStatus_Success;
                break;
            }
            else
            {
                continue;
            }

        } while (retryTimes);
    }
    else
    {
        /* default mode 25MHZ */
        card->busClock_Hz =
            SDMMCHOST_SetCardClock(card->host, FSL_SDMMC_CARD_MAX_BUS_FREQ(card->usrParam.maxFreq, SD_CLOCK_25MHZ));
        status = kStatus_Success;
    }

    return status;
}

static status_t SDIO_SelectBusTiming(sdio_card_t *card)
{
    assert(card);

    uint32_t targetBusFreq   = SD_CLOCK_25MHZ;
    uint32_t targetTiming    = 0U;
    uint8_t temp             = 0U;
    uint32_t supportModeFlag = 0U;
    uint32_t retryTimes      = SDIO_RETRY_TIMES;

    do
    {
        switch (card->currentTiming)
        {
            /* if not select timing mode, sdmmc will handle it automatically*/
            case kSD_TimingSDR12DefaultMode:
#if SDMMCHOST_SUPPORT_SDR104
            case kSD_TimingSDR104Mode:
                if (((card->cccrflags & SDIO_CCCR_SUPPORT_SDR104) == SDIO_CCCR_SUPPORT_SDR104) &&
                    (card->operationVoltage == kSDMMC_OperationVoltage180V))
                {
                    card->currentTiming = kSD_TimingSDR104Mode;
                    targetTiming        = SDIO_CCCR_ENABLE_SDR104_MODE;
                    targetBusFreq       = FSL_SDMMC_CARD_MAX_BUS_FREQ(card->usrParam.maxFreq, SD_CLOCK_208MHZ);
                    supportModeFlag     = SDIO_CCCR_SUPPORT_SDR104;
                    break;
                }
                SUPPRESS_FALL_THROUGH_WARNING();
#endif

#if SDMMCHOST_SUPPORT_DDR50
            case kSD_TimingDDR50Mode:
                if (((card->cccrflags & SDIO_CCCR_SUPPORT_DDR50) == SDIO_CCCR_SUPPORT_DDR50) &&
                    (card->operationVoltage == kSDMMC_OperationVoltage180V))
                {
                    card->currentTiming = kSD_TimingDDR50Mode;
                    targetTiming        = SDIO_CCCR_ENABLE_DDR50_MODE;
                    targetBusFreq       = FSL_SDMMC_CARD_MAX_BUS_FREQ(card->usrParam.maxFreq, SD_CLOCK_50MHZ);
                    supportModeFlag     = SDIO_CCCR_SUPPORT_DDR50;
                    break;
                }
                SUPPRESS_FALL_THROUGH_WARNING();
#endif

#if SDMMCHOST_SUPPORT_SDR50
            case kSD_TimingSDR50Mode:
                if (((card->cccrflags & SDIO_CCCR_SUPPORT_SDR50) == SDIO_CCCR_SUPPORT_SDR50) &&
                    (card->operationVoltage == kSDMMC_OperationVoltage180V))
                {
                    card->currentTiming = kSD_TimingSDR50Mode;
                    targetTiming        = SDIO_CCCR_ENABLE_SDR50_MODE;
                    targetBusFreq       = FSL_SDMMC_CARD_MAX_BUS_FREQ(card->usrParam.maxFreq, SD_CLOCK_100MHZ);
                    supportModeFlag     = SDIO_CCCR_SUPPORT_SDR50;
                    break;
                }
                SUPPRESS_FALL_THROUGH_WARNING();
#endif

            case kSD_TimingSDR25HighSpeedMode:
                if ((card->cccrflags & SDIO_CCCR_SUPPORT_HIGHSPEED) == SDIO_CCCR_SUPPORT_HIGHSPEED)
                {
                    card->currentTiming = kSD_TimingSDR25HighSpeedMode;
                    targetTiming        = SDIO_CCCR_ENABLE_HIGHSPEED_MODE;
                    targetBusFreq       = FSL_SDMMC_CARD_MAX_BUS_FREQ(card->usrParam.maxFreq, SD_CLOCK_50MHZ);
                    supportModeFlag     = SDIO_CCCR_SUPPORT_HIGHSPEED;
                    break;
                }
                SUPPRESS_FALL_THROUGH_WARNING();

            default:
                /* default timing mode */
                card->currentTiming = kSD_TimingSDR12DefaultMode;
                targetBusFreq       = SD_CLOCK_25MHZ;
                break;
        }

        if (kStatus_Success != SDIO_IO_RW_Direct(card, kSDIO_IORead, kSDIO_FunctionNum0, kSDIO_RegBusSpeed, 0U, &temp))
        {
            return kStatus_SDMMC_TransferFailed;
        }
        do
        {
            temp &= ~SDIO_CCCR_BUS_SPEED_MASK;
            temp |= targetTiming;

            retryTimes--;
            if (kStatus_Success !=
                SDIO_IO_RW_Direct(card, kSDIO_IOWrite, kSDIO_FunctionNum0, kSDIO_RegBusSpeed, temp, &temp))
            {
                continue;
            }

            if ((temp & targetTiming) != targetTiming)
            {
                continue;
            }

            break;

        } while (retryTimes);

        if (retryTimes == 0U)
        {
            retryTimes = SDIO_RETRY_TIMES;
            /* if cannot switch target timing, it will switch continuously until find a valid timing. */
            card->cccrflags &= ~supportModeFlag;
            continue;
        }

        break;

    } while (1);

    card->busClock_Hz =
        SDMMCHOST_SetCardClock(card->host, FSL_SDMMC_CARD_MAX_BUS_FREQ(card->usrParam.maxFreq, targetBusFreq));

#if SDMMCHOST_SUPPORT_DDR50
    /* enable DDR mode if it is the target mode */
    if (card->currentTiming == kSD_TimingDDR50Mode)
    {
        SDMMCHOST_EnableDDRMode(card->host, true, 0U);
    }
#endif

#if SDMMCHOST_SUPPORT_SDR50 || SDMMCHOST_SUPPORT_SDR104

    if (card->usrParam.ioStrength != NULL)
    {
        card->usrParam.ioStrength(card->busClock_Hz);
    }

    /* SDR50 and SDR104 mode need tuning */
    if ((card->currentTiming == kSD_TimingSDR50Mode) || (card->currentTiming == kSD_TimingSDR104Mode))
    {
        /* execute tuning */
        if (SDIO_ExecuteTuning(card) != kStatus_Success)
        {
            return kStatus_SDMMC_TuningFail;
        }
    }

#endif

    return kStatus_Success;
}

status_t SDIO_SetDriverStrength(sdio_card_t *card, sd_driver_strength_t driverStrength)
{
    uint8_t strength = 0U, temp = 0U;

    switch (driverStrength)
    {
        case kSD_DriverStrengthTypeA:
            strength = SDIO_CCCR_ENABLE_DRIVER_TYPE_A;
            break;
        case kSD_DriverStrengthTypeC:
            strength = SDIO_CCCR_ENABLE_DRIVER_TYPE_C;
            break;
        case kSD_DriverStrengthTypeD:
            strength = SDIO_CCCR_ENABLE_DRIVER_TYPE_D;
            break;
        default:
            strength = SDIO_CCCR_ENABLE_DRIVER_TYPE_B;
            break;
    }

    if (kStatus_Success !=
        SDIO_IO_RW_Direct(card, kSDIO_IORead, kSDIO_FunctionNum0, kSDIO_RegDriverStrength, 0U, &temp))
    {
        return kStatus_SDMMC_TransferFailed;
    }

    temp &= ~SDIO_CCCR_DRIVER_TYPE_MASK;
    temp |= strength;

    return SDIO_IO_RW_Direct(card, kSDIO_IOWrite, kSDIO_FunctionNum0, kSDIO_RegDriverStrength, temp, &temp);
}

status_t SDIO_EnableAsyncInterrupt(sdio_card_t *card, bool enable)
{
    assert(card);

    uint8_t eai = 0U;

    if ((card->cccrflags & SDIO_CCCR_SUPPORT_ASYNC_INT) == 0U)
    {
        return kStatus_SDMMC_NotSupportYet;
    }

    /* load interrupt enable register */
    if (kStatus_Success !=
        SDIO_IO_RW_Direct(card, kSDIO_IORead, kSDIO_FunctionNum0, kSDIO_RegInterruptExtension, 0U, &eai))
    {
        return kStatus_SDMMC_TransferFailed;
    }
    /* if already enable/disable , do not need enable/disable again */
    if (((eai)&SDIO_CCCR_ENABLE_AYNC_INT) == (enable ? SDIO_CCCR_ENABLE_AYNC_INT : 0U))
    {
        return kStatus_Success;
    }

    /* enable the eai */
    if (enable)
    {
        eai |= SDIO_CCCR_ENABLE_AYNC_INT;
    }
    else
    {
        eai &= ~(SDIO_CCCR_ENABLE_AYNC_INT);
    }

    /* write to register */
    if (kStatus_Success !=
        SDIO_IO_RW_Direct(card, kSDIO_IOWrite, kSDIO_FunctionNum0, kSDIO_RegInterruptExtension, eai, &eai))
    {
        return kStatus_SDMMC_TransferFailed;
    }

    return kStatus_Success;
}

static status_t SDIO_DecodeCIS(
    sdio_card_t *card, sdio_func_num_t func, uint8_t *dataBuffer, uint32_t tplCode, uint32_t tplLink)
{
    assert(card);
    assert(func <= kSDIO_FunctionNum7);

    if (func == kSDIO_FunctionNum0)
    {
        /* only decode MANIFID,FUNCID,FUNCE here  */
        if (tplCode == SDIO_TPL_CODE_MANIFID)
        {
            card->commonCIS.mID   = dataBuffer[0U] | (dataBuffer[1U] << 8U);
            card->commonCIS.mInfo = dataBuffer[2U] | (dataBuffer[3U] << 8U);
        }
        else if (tplCode == SDIO_TPL_CODE_FUNCID)
        {
            card->commonCIS.funcID = dataBuffer[0U];
        }
        else if (tplCode == SDIO_TPL_CODE_FUNCE)
        {
            /* max transfer block size and data size */
            card->commonCIS.fn0MaxBlkSize = dataBuffer[1U] | (dataBuffer[2U] << 8U);
            /* max transfer speed */
            card->commonCIS.maxTransSpeed = dataBuffer[3U];
        }
        else
        {
            /* reserved here */
            return kStatus_Fail;
        }
    }
    else
    {
        /* only decode FUNCID,FUNCE here  */
        if (tplCode == SDIO_TPL_CODE_FUNCID)
        {
            card->funcCIS[func - 1U].funcID = dataBuffer[0U];
        }
        else if (tplCode == SDIO_TPL_CODE_FUNCE)
        {
            if (tplLink == 0x2A)
            {
                card->funcCIS[func - 1U].funcInfo  = dataBuffer[1U];
                card->funcCIS[func - 1U].ioVersion = dataBuffer[2U];
                card->funcCIS[func - 1U].cardPSN =
                    dataBuffer[3U] | (dataBuffer[4U] << 8U) | (dataBuffer[5U] << 16U) | (dataBuffer[6U] << 24U);
                card->funcCIS[func - 1U].ioCSASize =
                    dataBuffer[7U] | (dataBuffer[8U] << 8U) | (dataBuffer[9U] << 16U) | (dataBuffer[10U] << 24U);
                card->funcCIS[func - 1U].ioCSAProperty  = dataBuffer[11U];
                card->funcCIS[func - 1U].ioMaxBlockSize = dataBuffer[12U] | (dataBuffer[13U] << 8U);
                card->funcCIS[func - 1U].ioOCR =
                    dataBuffer[14U] | (dataBuffer[15U] << 8U) | (dataBuffer[16U] << 16U) | (dataBuffer[17U] << 24U);
                card->funcCIS[func - 1U].ioOPMinPwr         = dataBuffer[18U];
                card->funcCIS[func - 1U].ioOPAvgPwr         = dataBuffer[19U];
                card->funcCIS[func - 1U].ioOPMaxPwr         = dataBuffer[20U];
                card->funcCIS[func - 1U].ioSBMinPwr         = dataBuffer[21U];
                card->funcCIS[func - 1U].ioSBAvgPwr         = dataBuffer[22U];
                card->funcCIS[func - 1U].ioSBMaxPwr         = dataBuffer[23U];
                card->funcCIS[func - 1U].ioMinBandWidth     = dataBuffer[24U] | (dataBuffer[25U] << 8U);
                card->funcCIS[func - 1U].ioOptimumBandWidth = dataBuffer[26U] | (dataBuffer[27U] << 8U);
                card->funcCIS[func - 1U].ioReadyTimeout     = dataBuffer[28U] | (dataBuffer[29U] << 8U);

                card->funcCIS[func - 1U].ioHighCurrentAvgCurrent = dataBuffer[34U] | (dataBuffer[35U] << 8U);
                card->funcCIS[func - 1U].ioHighCurrentMaxCurrent = dataBuffer[36U] | (dataBuffer[37U] << 8U);
                card->funcCIS[func - 1U].ioLowCurrentAvgCurrent  = dataBuffer[38U] | (dataBuffer[39U] << 8U);
                card->funcCIS[func - 1U].ioLowCurrentMaxCurrent  = dataBuffer[40U] | (dataBuffer[41U] << 8U);
            }
            else
            {
                return kStatus_Fail;
            }
        }
        else
        {
            return kStatus_Fail;
        }
    }

    return kStatus_Success;
}

status_t SDIO_ReadCIS(sdio_card_t *card, sdio_func_num_t func, const uint32_t *tupleList, uint32_t tupleNum)
{
    assert(card);
    assert(func <= kSDIO_FunctionNum7);
    assert(tupleList);

    uint8_t tplCode = 0U;
    uint8_t tplLink = 0U;
    uint32_t cisPtr = 0U;
    uint32_t i = 0U, num = 0U;
    bool tupleMatch = false;

    uint8_t dataBuffer[255U] = {0U};

    /* get the CIS pointer for each function */
    if (func == kSDIO_FunctionNum0)
    {
        cisPtr = card->commonCISPointer;
    }
    else
    {
        cisPtr = card->ioFBR[func - 1U].ioPointerToCIS;
    }

    if (0U == cisPtr)
    {
        return kStatus_SDMMC_SDIO_ReadCISFail;
    }

    do
    {
        if (kStatus_Success != SDIO_IO_RW_Direct(card, kSDIO_IORead, kSDIO_FunctionNum0, cisPtr++, 0U, &tplCode))
        {
            return kStatus_SDMMC_TransferFailed;
        }
        /* end of chain tuple */
        if (tplCode == 0xFFU)
        {
            break;
        }

        if (tplCode == 0U)
        {
            continue;
        }

        for (i = 0; i < tupleNum; i++)
        {
            if (tplCode == tupleList[i])
            {
                tupleMatch = true;
                break;
            }
        }

        if (kStatus_Success != SDIO_IO_RW_Direct(card, kSDIO_IORead, kSDIO_FunctionNum0, cisPtr++, 0U, &tplLink))
        {
            return kStatus_SDMMC_TransferFailed;
        }
        /* end of chain tuple */
        if (tplLink == 0xFFU)
        {
            break;
        }

        if (tupleMatch)
        {
            memset(dataBuffer, 0U, 255U);
            for (i = 0; i < tplLink; i++)
            {
                if (kStatus_Success !=
                    SDIO_IO_RW_Direct(card, kSDIO_IORead, kSDIO_FunctionNum0, cisPtr++, 0U, &dataBuffer[i]))
                {
                    return kStatus_SDMMC_TransferFailed;
                }
            }
            tupleMatch = false;
            /* pharse the data */
            SDIO_DecodeCIS(card, func, dataBuffer, tplCode, tplLink);
            /* read finish then return */
            if (++num == tupleNum)
            {
                break;
            }
        }
        else
        {
            /* move pointer */
            cisPtr += tplLink;
            /* tuple code not match,continue read tuple code */
            continue;
        }
    } while (1);
    return kStatus_Success;
}

static status_t SDIO_ProbeBusVoltage(sdio_card_t *card)
{
    assert(card);

    uint32_t ocr = 0U, accept1V8 = 0U;
    status_t error = kStatus_Success;

    /* application able to set the supported voltage window */
    if ((card->ocr & SDIO_OCR_VOLTAGE_WINDOW_MASK) != 0U)
    {
        ocr = card->ocr & SDIO_OCR_VOLTAGE_WINDOW_MASK;
    }
    else
    {
        /* 3.3V voltage should be supported as default */
        ocr |= SDMMC_MASK(kSD_OcrVdd29_30Flag) | SDMMC_MASK(kSD_OcrVdd32_33Flag) | SDMMC_MASK(kSD_OcrVdd33_34Flag);
    }

#if SDMMCHOST_SUPPORT_DDR50 || SDMMCHOST_SUPPORT_SDR104 || SDMMCHOST_SUPPORT_SDR50
    if (card->operationVoltage != kSDMMC_OperationVoltage180V)
    {
        /* allow user select the work voltage, if not select, sdmmc will handle it automatically */
        ocr |= SDMMC_MASK(kSD_OcrSwitch18RequestFlag);

        if ((card->usrParam.ioVoltage != NULL) && (card->usrParam.ioVoltage->type == kSD_IOVoltageCtrlByGpio))
        {
            /* make sure card signal line voltage is 3.3v before initalization */
            if (card->usrParam.ioVoltage->func != NULL)
            {
                card->usrParam.ioVoltage->func(kSDMMC_OperationVoltage330V);
            }
        }
        else if ((card->usrParam.ioVoltage != NULL) && (card->usrParam.ioVoltage->type == kSD_IOVoltageCtrlByHost))
        {
            SDMMCHOST_SwitchToVoltage(card->host, (uint32_t)kSDMMC_OperationVoltage330V);
        }
        else
        {
            ocr &= ~SDMMC_MASK(kSD_OcrSwitch18RequestFlag);
        }
    }

#endif

    do
    {
        /* card go idle */
        if (kStatus_Success != SDIO_GoIdle(card))
        {
            return kStatus_SDMMC_GoIdleFailed;
        }

        /* Get IO OCR-CMD5 with arg0 ,set new voltage if needed*/
        if (kStatus_Success != SDIO_SendOperationCondition(card, 0U, NULL))
        {
            return kStatus_SDMMC_HandShakeOperationConditionFailed;
        }

        if (kStatus_Success != SDIO_SendOperationCondition(card, ocr, &accept1V8))
        {
            return kStatus_SDMMC_InvalidVoltage;
        }

#if SDMMCHOST_SUPPORT_DDR50 || SDMMCHOST_SUPPORT_SDR104 || SDMMCHOST_SUPPORT_SDR50
        /* check if card support 1.8V */
        if ((accept1V8 & SDMMC_MASK(kSD_OcrSwitch18AcceptFlag)) != 0U)
        {
            if ((card->usrParam.ioVoltage != NULL) && (card->usrParam.ioVoltage->type == kSD_IOVoltageCtrlNotSupport))
            {
                break;
            }

            error = SDIO_SwitchVoltage(card, kSDMMC_OperationVoltage180V);
            if (kStatus_SDMMC_SwitchVoltageFail == error)
            {
                break;
            }

            if (error == kStatus_SDMMC_SwitchVoltage18VFail33VSuccess)
            {
                ocr &= ~SDMMC_MASK(kSD_OcrSwitch18RequestFlag);
                error = kStatus_Success;
                continue;
            }
            else
            {
                card->operationVoltage = kSDMMC_OperationVoltage180V;
                break;
            }
        }
#endif
        break;
    } while (1U);

    return error;
}

status_t SDIO_CardInit(sdio_card_t *card)
{
    assert(card);

    if (!card->isHostReady)
    {
        return kStatus_SDMMC_HostNotReady;
    }
    /* Identify mode ,set clock to 400KHZ. */
    card->busClock_Hz = SDMMCHOST_SetCardClock(card->host, SDMMC_CLOCK_400KHZ);
    SDMMCHOST_SetCardBusWidth(card->host, kSDMMC_BusWdith1Bit);
    SDMMCHOST_SendCardActive(card->host);

    if (SDIO_ProbeBusVoltage(card) != kStatus_Success)
    {
        return kStatus_SDMMC_SwitchVoltageFail;
    }

    /* there is a memonly card */
    if ((card->ioTotalNumber == 0U) && (card->memPresentFlag))
    {
        return kStatus_SDMMC_SDIO_InvalidCard;
    }

    /* send relative address ,cmd3*/
    if (kStatus_Success != SDIO_SendRca(card))
    {
        return kStatus_SDMMC_SendRelativeAddressFailed;
    }
    /* select card cmd7 */
    if (kStatus_Success != SDIO_SelectCard(card, true))
    {
        return kStatus_SDMMC_SelectCardFailed;
    }

    /* get card capability */
    if (kStatus_Success != SDIO_GetCardCapability(card, kSDIO_FunctionNum0))
    {
        return kStatus_SDMMC_TransferFailed;
    }

    /* read common CIS here */
    if (SDIO_ReadCIS(card, kSDIO_FunctionNum0, s_tupleList, SDIO_COMMON_CIS_TUPLE_NUM))
    {
        return kStatus_SDMMC_SDIO_ReadCISFail;
    }

    /* switch data bus width */
    if (kStatus_Success != SDIO_SetMaxDataBusWidth(card))
    {
        return kStatus_SDMMC_SetDataBusWidthFailed;
    }

    /* trying switch to card support timing mode. */
    if (kStatus_Success != SDIO_SelectBusTiming(card))
    {
        return kStatus_SDMMC_SDIO_SwitchHighSpeedFail;
    }

    return kStatus_Success;
}

void SDIO_CardDeinit(sdio_card_t *card)
{
    assert(card);

    SDIO_CardReset(card);
    SDIO_SelectCard(card, false);
}

status_t SDIO_HostInit(sdio_card_t *card)
{
    assert(card);

    if (((!card->isHostReady) && SDMMCHOST_Init(card->host)) != kStatus_Success)
    {
        return kStatus_Fail;
    }

    if ((card->usrParam.cd->type == kSD_DetectCardByHostCD) || (card->usrParam.cd->type == kSD_DetectCardByHostDATA3))
    {
        SDMMCHOST_CardDetectInit(card->host, card->usrParam.cd);
    }

    if (card->usrParam.sdioInt != NULL)
    {
        SDMMCHOST_CardIntInit(card->host, card->usrParam.sdioInt);
    }

    /* set the host status flag, after the card re-plug in, don't need init host again */
    card->isHostReady = true;

    return kStatus_Success;
}

void SDIO_HostDeinit(sdio_card_t *card)
{
    assert(card);

    SDMMCHOST_Deinit(card->host);

    /* should re-init host */
    card->isHostReady = false;
}

void SDIO_HostReset(SDMMCHOST_CONFIG *host)
{
    SDMMCHOST_Reset(host);
}

void SDIO_HostDoReset(sdio_card_t *card)
{
    SDMMCHOST_Reset(card->host);
}

status_t SDIO_WaitCardDetectStatus(SDMMCHOST_TYPE *hostBase, const sdmmchost_detect_card_t *cd, bool waitCardStatus)
{
    return SDMMCHOST_WaitCardDetectStatus(hostBase, cd, waitCardStatus);
}

status_t SDIO_PollingCardInsert(sdio_card_t *card, uint32_t status)
{
    assert(card != NULL);
    assert(card->usrParam.cd != NULL);

    if (card->usrParam.cd->type == kSD_DetectCardByGpioCD)
    {
        if (card->usrParam.cd->cardDetected == NULL)
        {
            return false;
        }

        do
        {
            if ((card->usrParam.cd->cardDetected() == true) && (status == kSD_Inserted))
            {
                SDMMC_OSADelay(card->usrParam.cd->cdDebounce_ms);
                if (card->usrParam.cd->cardDetected() == true)
                {
                    break;
                }
            }

            if ((card->usrParam.cd->cardDetected() == false) && (status == kSD_Removed))
            {
                break;
            }
        } while (1);
    }
    else
    {
        if (card->isHostReady == false)
        {
            return kStatus_Fail;
        }

        if (SDMMCHOST_PollingCardDetectStatus(card->host, status, ~0U) != kStatus_Success)
        {
            return kStatus_Fail;
        }
    }

    return kStatus_Success;
}

bool SDIO_IsCardPresent(sdio_card_t *card)
{
    assert(card != NULL);
    assert(card->usrParam.cd != NULL);

    if (card->usrParam.cd->type == kSD_DetectCardByGpioCD)
    {
        if (card->usrParam.cd->cardDetected == NULL)
        {
            return false;
        }
        return card->usrParam.cd->cardDetected();
    }
    else
    {
        if (card->isHostReady == false)
        {
            return false;
        }

        if (SDMMCHOST_CardDetectStatus(card->host) == kSD_Removed)
        {
            return false;
        }
    }

    return true;
}

void SDIO_PowerOnCard(SDMMCHOST_TYPE *base, const sdmmchost_pwr_card_t *pwr)
{
    SDMMCHOST_PowerOnCard(base, pwr);
}

void SDIO_PowerOffCard(SDMMCHOST_TYPE *base, const sdmmchost_pwr_card_t *pwr)
{
    SDMMCHOST_PowerOffCard(base, pwr);
}

void SDIO_SetCardPower(sdio_card_t *card, bool enable)
{
    assert(card != NULL);

    if (card->usrParam.pwr != NULL)
    {
        card->usrParam.pwr(enable);
    }
    else
    {
        SDMMCHOST_SetCardPower(card->host, enable);
    }

    SDMMC_OSADelay(SDIO_POWER_RESET_DELAY);
}

status_t SDIO_Init(sdio_card_t *card)
{
    assert(card);
    assert(card->host);

    if (!card->isHostReady)
    {
        if (SDIO_HostInit(card) != kStatus_Success)
        {
            return kStatus_SDMMC_HostNotReady;
        }
    }
    else
    {
        /* reset the host */
        SDIO_HostReset(card->host);
    }
    /* power off card */
    SDIO_SetCardPower(card, false);
    /* card detect */
    if (SDIO_PollingCardInsert(card, kSD_Inserted) != kStatus_Success)
    {
        return kStatus_SDMMC_CardDetectFailed;
    }
    /* power on card */
    SDIO_SetCardPower(card, true);

    return SDIO_CardInit(card);
}

void SDIO_Deinit(sdio_card_t *card)
{
    assert(card);

    SDIO_CardDeinit(card);
    SDIO_HostDeinit(card);
}

status_t SDIO_EnableIOInterrupt(sdio_card_t *card, sdio_func_num_t func, bool enable)
{
    assert(card);
    assert(func <= kSDIO_FunctionNum7);

    uint8_t intEn = 0U;

    /* load io interrupt enable register */
    if (kStatus_Success != SDIO_IO_RW_Direct(card, kSDIO_IORead, kSDIO_FunctionNum0, kSDIO_RegIOIntEnable, 0U, &intEn))
    {
        return kStatus_SDMMC_TransferFailed;
    }

    if (enable)
    {
        /* if already enable , do not need enable again */
        if ((((intEn >> func) & 0x01U) == 0x01U) && (intEn & 0x01U))
        {
            return kStatus_Success;
        }

        /* enable the interrupt and interrupt master */
        intEn |= (1U << func) | 0x01U;
        card->ioIntNums++;
    }
    else
    {
        /* if already disable , do not need enable again */
        if (((intEn >> func) & 0x01U) == 0x00U)
        {
            return kStatus_Success;
        }

        /* disable the interrupt, don't disable the interrupt master here */
        intEn &= ~(1U << func);
        if (card->ioIntNums)
        {
            card->ioIntNums--;
        }
    }

    /* write to register */
    if (kStatus_Success !=
        SDIO_IO_RW_Direct(card, kSDIO_IOWrite, kSDIO_FunctionNum0, kSDIO_RegIOIntEnable, intEn, &intEn))
    {
        return kStatus_SDMMC_TransferFailed;
    }

    return kStatus_Success;
}

status_t SDIO_GetPendingInterrupt(sdio_card_t *card, uint8_t *pendingInt)
{
    assert(card);

    /* load io interrupt enable register */

    if (kStatus_Success !=
        SDIO_IO_RW_Direct(card, kSDIO_IORead, kSDIO_FunctionNum0, kSDIO_RegIOIntPending, 0U, pendingInt))
    {
        return kStatus_SDMMC_TransferFailed;
    }

    return kStatus_Success;
}

status_t SDIO_EnableIO(sdio_card_t *card, sdio_func_num_t func, bool enable)
{
    assert(card);
    assert(func <= kSDIO_FunctionNum7);
    assert(func != kSDIO_FunctionNum0);

    uint8_t ioEn = 0U, ioReady = 0U;
    volatile uint32_t i       = SDIO_RETRY_TIMES;
    uint32_t ioReadyTimeoutMS = card->funcCIS[func - 1U].ioReadyTimeout * SDIO_IO_READY_TIMEOUT_UNIT;

    if (ioReadyTimeoutMS != 0U)
    {
        /* do not poll the IO ready status, but use IO ready timeout  */
        i = 1U;
    }

    /* load io enable register */
    if (kStatus_Success != SDIO_IO_RW_Direct(card, kSDIO_IORead, kSDIO_FunctionNum0, kSDIO_RegIOEnable, 0U, &ioEn))
    {
        return kStatus_SDMMC_TransferFailed;
    }
    /* if already enable/disable , do not need enable/disable again */
    if (((ioEn >> func) & 0x01U) == (enable ? 1U : 0U))
    {
        return kStatus_Success;
    }

    /* enable the io */
    if (enable)
    {
        ioEn |= (1U << func);
    }
    else
    {
        ioEn &= ~(1U << func);
    }

    /* write to register */
    if (kStatus_Success != SDIO_IO_RW_Direct(card, kSDIO_IOWrite, kSDIO_FunctionNum0, kSDIO_RegIOEnable, ioEn, &ioEn))
    {
        return kStatus_SDMMC_TransferFailed;
    }

    /* if enable io, need check the IO ready status */
    if (enable)
    {
        do
        {
            SDMMC_OSADelay(ioReadyTimeoutMS);
            /* wait IO ready */
            if (kStatus_Success !=
                SDIO_IO_RW_Direct(card, kSDIO_IORead, kSDIO_FunctionNum0, kSDIO_RegIOReady, 0U, &ioReady))
            {
                return kStatus_SDMMC_TransferFailed;
            }
            /* check if IO ready */
            if ((ioReady & (1 << func)) != 0U)
            {
                return kStatus_Success;
            }

            i--;
        } while (i);

        return kStatus_Fail;
    }

    return kStatus_Success;
}

status_t SDIO_SelectIO(sdio_card_t *card, sdio_func_num_t func)
{
    assert(card);
    assert(func <= kSDIO_FunctionMemory);

    uint8_t ioSel = func;

    /* write to register */
    if (kStatus_Success !=
        SDIO_IO_RW_Direct(card, kSDIO_IOWrite, kSDIO_FunctionNum0, kSDIO_RegFunctionSelect, ioSel, &ioSel))
    {
        return kStatus_SDMMC_TransferFailed;
    }

    return kStatus_Success;
}

status_t SDIO_AbortIO(sdio_card_t *card, sdio_func_num_t func)
{
    assert(card);
    assert(func <= kSDIO_FunctionNum7);

    uint8_t ioAbort = func;

    /* write to register */
    if (kStatus_Success !=
        SDIO_IO_RW_Direct(card, kSDIO_IOWrite, kSDIO_FunctionNum0, kSDIO_RegIOAbort, ioAbort, &ioAbort))
    {
        return kStatus_SDMMC_TransferFailed;
    }

    return kStatus_Success;
}

void SDIO_SetIOIRQHandler(sdio_card_t *card, sdio_func_num_t func, sdio_io_irq_handler_t handler)
{
    assert(card);
    assert((func <= kSDIO_FunctionNum7) && (func != kSDIO_FunctionNum0));

    card->ioIRQHandler[func - 1] = handler;
    card->ioIntIndex             = func;
}

status_t SDIO_HandlePendingIOInterrupt(sdio_card_t *card)
{
    assert(card);

    uint8_t i = 0, pendingInt = 0;

    /* call IRQ handler directly if one IRQ handler only */
    if (card->ioIntNums == 1U)
    {
        if (card->ioIRQHandler[card->ioIntIndex - 1])
        {
            (card->ioIRQHandler[card->ioIntIndex - 1])(card, card->ioIntIndex);
        }
    }
    else
    {
        /* get pending int firstly */
        if (SDIO_GetPendingInterrupt(card, &pendingInt) != kStatus_Success)
        {
            return kStatus_SDMMC_TransferFailed;
        }

        for (i = 1; i <= FSL_SDIO_MAX_IO_NUMS; i++)
        {
            if (pendingInt & (1 << i))
            {
                if (card->ioIRQHandler[i - 1])
                {
                    (card->ioIRQHandler[i - 1])(card, i);
                }
            }
        }
    }

    return kStatus_Success;
}
