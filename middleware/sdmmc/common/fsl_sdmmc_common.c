/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_sdmmc_common.h"
/*******************************************************************************
 * Variables
 ******************************************************************************/
#if SDMMCHOST_SUPPORT_DDR50 || SDMMCHOST_SUPPORT_SDR104 || SDMMCHOST_SUPPORT_SDR50 || SDMMCHOST_SUPPORT_HS200 || \
    SDMMCHOST_SUPPORT_HS400
/* sdmmc tuning block */
const uint32_t SDMMC_TuningBlockPattern4Bit[16U] = {
    0xFF0FFF00, 0xFFCCC3CC, 0xC33CCCFF, 0xFEFFFEEF, 0xFFDFFFDD, 0xFFFBFFFB, 0xBFFF7FFF, 0x77F7BDEF,
    0xFFF0FFF0, 0x0FFCCC3C, 0xCC33CCCF, 0xFFEFFFEE, 0xFFFDFFFD, 0xDFFFBFFF, 0xBBFFF7FF, 0xF77F7BDE,
};
const uint32_t SDMMC_TuningBlockPattern8Bit[32U] = {
    0xffff00ff, 0xffff0000, 0xffffcccc, 0xcc33cccc, 0xcc3333cc, 0xccccffff, 0xffeeffff, 0xffeeeeff,
    0xffffddff, 0xffffdddd, 0xffffffbb, 0xffffffbb, 0xbbffffff, 0x77ffffff, 0x7777ff77, 0xbbddeeff,
    0xffffff00, 0xffffff00, 0x00ffffcc, 0xcccc33cc, 0xcccc3333, 0xccccccff, 0xffffeeff, 0xffffeeee,
    0xffffffdd, 0xffffffdd, 0xddffffff, 0xbbffffff, 0xbbbbffff, 0xff77ffff, 0xff7777ff, 0x77bbddee,
};
#endif
/*******************************************************************************
 * Code
 ******************************************************************************/
status_t SDMMC_SelectCard(sdmmchost_t *host, uint32_t relativeAddress, bool isSelected)
{
    sdmmchost_transfer_t content = {0};
    sdmmchost_cmd_t command      = {0};

    command.index = kSDMMC_SelectCard;
    if (isSelected)
    {
        command.argument     = relativeAddress << 16U;
        command.responseType = kCARD_ResponseTypeR1;
    }
    else
    {
        command.argument     = 0U;
        command.responseType = kCARD_ResponseTypeNone;
    }

    content.command = &command;
    content.data    = NULL;
    if ((kStatus_Success != SDMMCHOST_TransferFunction(host, &content)) ||
        (command.response[0U] & SDMMC_R1_ALL_ERROR_FLAG))
    {
        return kStatus_SDMMC_TransferFailed;
    }

    /* Wait until card to transfer state */
    return kStatus_Success;
}

status_t SDMMC_SendApplicationCommand(sdmmchost_t *host, uint32_t relativeAddress)
{
    sdmmchost_transfer_t content = {0};
    sdmmchost_cmd_t command      = {0};

    command.index        = kSDMMC_ApplicationCommand;
    command.argument     = (relativeAddress << 16U);
    command.responseType = kCARD_ResponseTypeR1;

    content.command = &command;
    content.data    = 0U;
    if ((kStatus_Success != SDMMCHOST_TransferFunction(host, &content)) ||
        (command.response[0U] & SDMMC_R1_ALL_ERROR_FLAG))
    {
        return kStatus_SDMMC_TransferFailed;
    }

    if (!(command.response[0U] & SDMMC_MASK(kSDMMC_R1ApplicationCommandFlag)))
    {
        return kStatus_SDMMC_CardNotSupport;
    }

    return kStatus_Success;
}

status_t SDMMC_SetBlockCount(sdmmchost_t *host, uint32_t blockCount)
{
    sdmmchost_transfer_t content = {0};
    sdmmchost_cmd_t command      = {0};

    command.index        = kSDMMC_SetBlockCount;
    command.argument     = blockCount;
    command.responseType = kCARD_ResponseTypeR1;

    content.command = &command;
    content.data    = 0U;
    if ((kStatus_Success != SDMMCHOST_TransferFunction(host, &content)) ||
        (command.response[0U] & SDMMC_R1_ALL_ERROR_FLAG))
    {
        return kStatus_SDMMC_TransferFailed;
    }

    return kStatus_Success;
}

status_t SDMMC_GoIdle(sdmmchost_t *host)
{
    sdmmchost_transfer_t content = {0};
    sdmmchost_cmd_t command      = {0};

    command.index = kSDMMC_GoIdleState;

    content.command = &command;
    content.data    = 0U;
    if (kStatus_Success != SDMMCHOST_TransferFunction(host, &content))
    {
        return kStatus_SDMMC_TransferFailed;
    }

    return kStatus_Success;
}

status_t SDMMC_SetBlockSize(sdmmchost_t *host, uint32_t blockSize)
{
    sdmmchost_transfer_t content = {0};
    sdmmchost_cmd_t command      = {0};

    command.index        = kSDMMC_SetBlockLength;
    command.argument     = blockSize;
    command.responseType = kCARD_ResponseTypeR1;

    content.command = &command;
    content.data    = 0U;
    if ((kStatus_Success != SDMMCHOST_TransferFunction(host, &content)) ||
        (command.response[0U] & SDMMC_R1_ALL_ERROR_FLAG))
    {
        return kStatus_SDMMC_TransferFailed;
    }

    return kStatus_Success;
}

status_t SDMMC_SetCardInactive(sdmmchost_t *host)
{
    sdmmchost_transfer_t content = {0};
    sdmmchost_cmd_t command      = {0};

    command.index        = kSDMMC_GoInactiveState;
    command.argument     = 0U;
    command.responseType = kCARD_ResponseTypeNone;

    content.command = &command;
    content.data    = 0U;
    if ((kStatus_Success != SDMMCHOST_TransferFunction(host, &content)))
    {
        return kStatus_SDMMC_TransferFailed;
    }

    return kStatus_Success;
}
