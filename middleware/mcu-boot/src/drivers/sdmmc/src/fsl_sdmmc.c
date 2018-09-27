/*
 * Copyright (c) 2015 Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_specification.h"
#include "fsl_card.h"
#include "microseconds/microseconds.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/
SDK_ALIGN(uint32_t g_sdmmc[SDK_SIZEALIGN(SDMMC_GLOBAL_BUFFER_SIZE, SDMMC_DATA_BUFFER_ALIGN_CAHCE)],
          MAX(SDMMC_DATA_BUFFER_ALIGN_CAHCE, HOST_DMA_BUFFER_ADDR_ALIGN));
/*******************************************************************************
 * Code
 ******************************************************************************/

void SDMMC_Delay(uint32_t num)
{
    volatile uint32_t i, j;

    for (i = 0U; i < num; i++)
    {
        for (j = 0U; j < 10000U; j++)
        {
            __asm("nop");
        }
    }
}

status_t SDMMC_SelectCard(HOST_TYPE *base, HOST_TRANSFER_FUNCTION transfer, uint32_t relativeAddress, bool isSelected)
{
    assert(transfer);

    HOST_TRANSFER content = { 0 };
    HOST_COMMAND command = { 0 };

    command.index = kSDMMC_SelectCard;
    if (isSelected)
    {
        command.argument = relativeAddress << 16U;
        command.responseType = kCARD_ResponseTypeR1;
    }
    else
    {
        command.argument = 0U;
        command.responseType = kCARD_ResponseTypeNone;
    }

    content.command = &command;
    content.data = NULL;
    if ((kStatus_Success != transfer(base, &content)) || (command.response[0U] & kSDMMC_R1ErrorAllFlag))
    {
        return kStatus_SDMMC_TransferFailed;
    }

    /* Wait until card to transfer state */
    return kStatus_Success;
}

status_t SDMMC_SendApplicationCommand(HOST_TYPE *base, HOST_TRANSFER_FUNCTION transfer, uint32_t relativeAddress)
{
    assert(transfer);

    HOST_TRANSFER content = { 0 };
    HOST_COMMAND command = { 0 };

    command.index = kSDMMC_ApplicationCommand;
    command.argument = (relativeAddress << 16U);
    command.responseType = kCARD_ResponseTypeR1;

    content.command = &command;
    content.data = 0U;
    if ((kStatus_Success != transfer(base, &content)) || (command.response[0U] & kSDMMC_R1ErrorAllFlag))
    {
        return kStatus_SDMMC_TransferFailed;
    }

    if (!(command.response[0U] & kSDMMC_R1ApplicationCommandFlag))
    {
        return kStatus_SDMMC_CardNotSupport;
    }

    return kStatus_Success;
}

status_t SDMMC_SetBlockCount(HOST_TYPE *base, HOST_TRANSFER_FUNCTION transfer, uint32_t blockCount)
{
    assert(transfer);

    HOST_TRANSFER content = { 0 };
    HOST_COMMAND command = { 0 };

    command.index = kSDMMC_SetBlockCount;
    command.argument = blockCount;
    command.responseType = kCARD_ResponseTypeR1;

    content.command = &command;
    content.data = 0U;
    if ((kStatus_Success != transfer(base, &content)) || (command.response[0U] & kSDMMC_R1ErrorAllFlag))
    {
        return kStatus_SDMMC_TransferFailed;
    }

    return kStatus_Success;
}

status_t SDMMC_GoIdle(HOST_TYPE *base, HOST_TRANSFER_FUNCTION transfer)
{
    assert(transfer);

    HOST_TRANSFER content = { 0 };
    HOST_COMMAND command = { 0 };

    command.index = kSDMMC_GoIdleState;

    content.command = &command;
    content.data = 0U;
    if (kStatus_Success != transfer(base, &content))
    {
        return kStatus_SDMMC_TransferFailed;
    }

    return kStatus_Success;
}

status_t SDMMC_SetBlockSize(HOST_TYPE *base, HOST_TRANSFER_FUNCTION transfer, uint32_t blockSize)
{
    assert(transfer);

    HOST_TRANSFER content = { 0 };
    HOST_COMMAND command = { 0 };

    command.index = kSDMMC_SetBlockLength;
    command.argument = blockSize;
    command.responseType = kCARD_ResponseTypeR1;

    content.command = &command;
    content.data = 0U;
    if ((kStatus_Success != transfer(base, &content)) || (command.response[0U] & kSDMMC_R1ErrorAllFlag))
    {
        return kStatus_SDMMC_TransferFailed;
    }

    return kStatus_Success;
}

status_t SDMMC_SetCardInactive(HOST_TYPE *base, HOST_TRANSFER_FUNCTION transfer)
{
    assert(transfer);

    HOST_TRANSFER content = { 0 };
    HOST_COMMAND command = { 0 };

    command.index = kSDMMC_GoInactiveState;
    command.argument = 0U;
    command.responseType = kCARD_ResponseTypeNone;

    content.command = &command;
    content.data = 0U;
    if ((kStatus_Success != transfer(base, &content)))
    {
        return kStatus_SDMMC_TransferFailed;
    }

    return kStatus_Success;
}

status_t SDMMC_SwitchVoltage(HOST_TYPE *base, HOST_TRANSFER_FUNCTION transfer)
{
    assert(transfer);

    HOST_TRANSFER content = { 0 };
    HOST_COMMAND command = { 0 };

    command.index = kSD_VoltageSwitch;
    command.argument = 0U;
    command.responseType = kCARD_ResponseTypeR1;

    content.command = &command;
    content.data = NULL;
    if (kStatus_Success != transfer(base, &content))
    {
        return kStatus_SDMMC_TransferFailed;
    }
    /* disable card clock */
    HOST_ENABLE_CARD_CLOCK(base, false);

    /* check data line and cmd line status */
    if ((GET_HOST_STATUS(base) &
         (CARD_DATA1_STATUS_MASK | CARD_DATA2_STATUS_MASK | CARD_DATA3_STATUS_MASK | CARD_DATA0_NOT_BUSY)) != 0U)
    {
        return kStatus_SDMMC_SwitchVoltageFail;
    }

    /* host switch to 1.8V */
    HOST_SWITCH_VOLTAGE180V(base, true);

    SDMMC_Delay(100U);

    /*enable sd clock*/
    HOST_ENABLE_CARD_CLOCK(base, true);
    /*enable force clock on*/
    HOST_FORCE_SDCLOCK_ON(base, true);
    /* dealy 1ms,not exactly correct when use while */
    SDMMC_Delay(10U);
    /*disable force clock on*/
    HOST_FORCE_SDCLOCK_ON(base, false);

    /* check data line and cmd line status */
    if ((GET_HOST_STATUS(base) &
         (CARD_DATA1_STATUS_MASK | CARD_DATA2_STATUS_MASK | CARD_DATA3_STATUS_MASK | CARD_DATA0_NOT_BUSY)) == 0U)
    {
        return kStatus_SDMMC_SwitchVoltageFail;
    }

    return kStatus_Success;
}

status_t SDMMC_BL_SwitchVoltage(HOST_TYPE *base, HOST_TRANSFER_FUNCTION transfer)
{
    assert(transfer);

    HOST_TRANSFER content = { 0 };
    HOST_COMMAND command = { 0 };

    command.index = kSD_VoltageSwitch;
    command.argument = 0U;
    command.responseType = kCARD_ResponseTypeR1;

    content.command = &command;
    content.data = NULL;
    if (kStatus_Success != transfer(base, &content))
    {
        return kStatus_SDMMC_TransferFailed;
    }
    /* disable card clock */
    HOST_ENABLE_CARD_CLOCK(base, false);

    /* check data line and cmd line status */
    if ((GET_HOST_STATUS(base) &
         (CARD_DATA1_STATUS_MASK | CARD_DATA2_STATUS_MASK | CARD_DATA3_STATUS_MASK | CARD_DATA0_NOT_BUSY)) != 0U)
    {
        return kStatus_SDMMC_SwitchVoltageFail;
    }

    /* host switch to 1.8V */
    HOST_SWITCH_VOLTAGE180V(base, true);

    /* delay 5ms for 1.8V output stable. */
    microseconds_delay(5 * 1000);

    /*enable sd clock*/
    HOST_ENABLE_CARD_CLOCK(base, true);
    /*enable force clock on*/
    HOST_FORCE_SDCLOCK_ON(base, true);
    /* dealy 1ms,not exactly correct when use while */
    microseconds_delay(1 * 1000);

    /*disable force clock on*/
    HOST_FORCE_SDCLOCK_ON(base, false);

    /* check data line and cmd line status */
    if ((GET_HOST_STATUS(base) &
         (CARD_DATA1_STATUS_MASK | CARD_DATA2_STATUS_MASK | CARD_DATA3_STATUS_MASK | CARD_DATA0_NOT_BUSY)) == 0U)
    {
        return kStatus_SDMMC_SwitchVoltageFail;
    }

    return kStatus_Success;
}

status_t SDMMC_ExecuteTuning(HOST_TYPE *base, HOST_TRANSFER_FUNCTION transfer, uint32_t tuningCmd, uint32_t blockSize)
{
    HOST_TRANSFER content = { 0U };
    HOST_COMMAND command = { 0U };
    HOST_DATA data = { 0U };
    uint32_t buffer[32U] = { 0U };
    bool tuningError = true;

    command.index = tuningCmd;
    command.argument = 0U;
    command.responseType = kCARD_ResponseTypeR1;

    data.blockSize = blockSize;
    data.blockCount = 1U;
    data.rxData = buffer;
    /* add this macro for adpter to different driver */
    HOST_ENABLE_TUNING_FLAG(data);

    content.command = &command;
    content.data = &data;

    /* enable the standard tuning */
    HOST_EXECUTE_STANDARD_TUNING_ENABLE(base, true);

    while (true)
    {
        /* send tuning block */
        if ((kStatus_Success != transfer(base, &content)))
        {
            return kStatus_SDMMC_TransferFailed;
        }
        SDMMC_Delay(1U);

        /*wait excute tuning bit clear*/
        if ((HOST_EXECUTE_STANDARD_TUNING_STATUS(base) != 0U))
        {
            continue;
        }

        /* if tuning error , re-tuning again */
        if ((HOST_CHECK_TUNING_ERROR(base) != 0U) && tuningError)
        {
            tuningError = false;
            /* enable the standard tuning */
            HOST_EXECUTE_STANDARD_TUNING_ENABLE(base, true);
            HOST_ADJUST_TUNING_DELAY(base, HOST_STANDARD_TUNING_START);
        }
        else
        {
            break;
        }
    }

    /* delay to wait the host controller stable */
    SDMMC_Delay(100U);

    /* check tuning result*/
    if (HOST_EXECUTE_STANDARD_TUNING_RESULT(base) == 0U)
    {
        return kStatus_SDMMC_TuningFail;
    }

    HOST_AUTO_STANDARD_RETUNING_TIMER(base);

    return kStatus_Success;
}
