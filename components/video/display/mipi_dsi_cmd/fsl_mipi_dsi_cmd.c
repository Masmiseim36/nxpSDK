/*
 * Copyright (c) 2017, NXP Semiconductors, Inc.
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_mipi_dsi_cmd.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
status_t MIPI_DSI_DCS_SoftReset(mipi_dsi_device_t *device)
{
    dsi_transfer_t dsiXfer = {0};
    uint8_t txData         = kMIPI_DCS_SoftReset;

    dsiXfer.virtualChannel = device->virtualChannel;
    dsiXfer.txDataType     = kDSI_TxDataDcsShortWrNoParam;
    dsiXfer.txDataSize     = 1;
    dsiXfer.txData         = &txData;

    return device->xferFunc(&dsiXfer);
}

status_t MIPI_DSI_DCS_SetDisplayOn(mipi_dsi_device_t *device, bool on)
{
    dsi_transfer_t dsiXfer = {0};
    uint8_t txData;

    dsiXfer.virtualChannel = device->virtualChannel;
    dsiXfer.txDataType     = kDSI_TxDataDcsShortWrNoParam;
    dsiXfer.txDataSize     = 1;
    dsiXfer.txData         = &txData;

    if (on)
    {
        txData = kMIPI_DCS_SetDisplayOn;
    }
    else
    {
        txData = kMIPI_DCS_SetDisplayOff;
    }

    return device->xferFunc(&dsiXfer);
}

status_t MIPI_DSI_DCS_EnterSleepMode(mipi_dsi_device_t *device, bool enter)
{
    dsi_transfer_t dsiXfer = {0};
    uint8_t txData;

    dsiXfer.virtualChannel = device->virtualChannel;
    dsiXfer.txDataType     = kDSI_TxDataDcsShortWrNoParam;
    dsiXfer.txDataSize     = 1;
    dsiXfer.txData         = &txData;

    if (enter)
    {
        txData = kMIPI_DCS_EnterSleepMode;
    }
    else
    {
        txData = kMIPI_DCS_ExitSleepMode;
    }

    return device->xferFunc(&dsiXfer);
}

status_t MIPI_DSI_DCS_EnterPartialMode(mipi_dsi_device_t *device, bool enter)
{
    dsi_transfer_t dsiXfer = {0};
    uint8_t txData;

    dsiXfer.virtualChannel = device->virtualChannel;
    dsiXfer.txDataType     = kDSI_TxDataDcsShortWrNoParam;
    dsiXfer.txDataSize     = 1;
    dsiXfer.txData         = &txData;

    if (enter)
    {
        txData = kMIPI_DCS_EnterPartialMode;
    }
    else
    {
        txData = kMIPI_DCS_EnterNormalMode;
    }

    return device->xferFunc(&dsiXfer);
}

status_t MIPI_DSI_DCS_EnterInvertMode(mipi_dsi_device_t *device, bool enter)
{
    dsi_transfer_t dsiXfer = {0};
    uint8_t txData;

    dsiXfer.virtualChannel = device->virtualChannel;
    dsiXfer.txDataType     = kDSI_TxDataDcsShortWrNoParam;
    dsiXfer.txDataSize     = 1;
    dsiXfer.txData         = &txData;

    if (enter)
    {
        txData = kMIPI_DCS_EnterInvertMode;
    }
    else
    {
        txData = kMIPI_DCS_ExitInvertMode;
    }

    return device->xferFunc(&dsiXfer);
}

status_t MIPI_DSI_DCS_EnterIdleMode(mipi_dsi_device_t *device, bool enter)
{
    dsi_transfer_t dsiXfer = {0};
    uint8_t txData;

    dsiXfer.virtualChannel = device->virtualChannel;
    dsiXfer.txDataType     = kDSI_TxDataDcsShortWrNoParam;
    dsiXfer.txDataSize     = 1;
    dsiXfer.txData         = &txData;

    if (enter)
    {
        txData = kMIPI_DCS_EnterIdleMode;
    }
    else
    {
        txData = kMIPI_DCS_ExitIdleMode;
    }

    return device->xferFunc(&dsiXfer);
}

status_t MIPI_DSI_DCS_Write(mipi_dsi_device_t *device, const uint8_t *txData, int32_t txDataSize)
{
    dsi_transfer_t dsiXfer = {0};

    dsiXfer.virtualChannel = device->virtualChannel;
    dsiXfer.txDataSize     = txDataSize;
    dsiXfer.txData         = txData;

    if (0 == txDataSize)
    {
        /* For DSC command, the data size should not be 0. */
        return kStatus_InvalidArgument;
    }
    else if (1 == txDataSize)
    {
        dsiXfer.txDataType = kDSI_TxDataDcsShortWrNoParam;
    }
    else if (2 == txDataSize)
    {
        dsiXfer.txDataType = kDSI_TxDataDcsShortWrOneParam;
    }
    else
    {
        dsiXfer.txDataType = kDSI_TxDataDcsLongWr;
    }

    return device->xferFunc(&dsiXfer);
}

status_t MIPI_DSI_GenericWrite(mipi_dsi_device_t *device, const uint8_t *txData, int32_t txDataSize)
{
    dsi_transfer_t dsiXfer = {0};

    dsiXfer.virtualChannel = device->virtualChannel;
    dsiXfer.txDataSize     = txDataSize;
    dsiXfer.txData         = txData;

    if (0 == txDataSize)
    {
        dsiXfer.txDataType = kDSI_TxDataGenShortWrNoParam;
    }
    else if (1 == txDataSize)
    {
        dsiXfer.txDataType = kDSI_TxDataGenShortWrOneParam;
    }
    else if (2 == txDataSize)
    {
        dsiXfer.txDataType = kDSI_TxDataGenShortWrTwoParam;
    }
    else
    {
        dsiXfer.txDataType = kDSI_TxDataGenLongWr;
    }

    return device->xferFunc(&dsiXfer);
}
