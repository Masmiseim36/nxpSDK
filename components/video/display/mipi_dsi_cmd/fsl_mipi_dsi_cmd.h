/*
 * Copyright (c) 2017, NXP Semiconductors, Inc.
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _FSL_MIPI_DSI_CMD_H_
#define _FSL_MIPI_DSI_CMD_H_

#include "fsl_common.h"
#include "fsl_mipi_dsi.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

enum _mipi_dsc
{
    kMIPI_DCS_Nop                  = 0x00,
    kMIPI_DCS_SoftReset            = 0x01,
    kMIPI_DCS_GetRedChannel        = 0x06,
    kMIPI_DCS_GetGreenChannel      = 0x07,
    kMIPI_DCS_GetBlueChannel       = 0x08,
    kMIPI_DCS_GetPowerMode         = 0x0A,
    kMIPI_DCS_GetAddressMode       = 0x0B,
    kMIPI_DCS_GetPixelFormat       = 0x0C,
    kMIPI_DCS_GetDisplayMode       = 0x0D,
    kMIPI_DCS_GetSignalMode        = 0x0E,
    kMIPI_DCS_GetDiagnosticResult  = 0x0F,
    kMIPI_DCS_EnterSleepMode       = 0x10,
    kMIPI_DCS_ExitSleepMode        = 0x11,
    kMIPI_DCS_EnterPartialMode     = 0x12,
    kMIPI_DCS_EnterNormalMode      = 0x13,
    kMIPI_DCS_ExitInvertMode       = 0x20,
    kMIPI_DCS_EnterInvertMode      = 0x21,
    kMIPI_DCS_SetGammaCurve        = 0x26,
    kMIPI_DCS_SetDisplayOff        = 0x28,
    kMIPI_DCS_SetDisplayOn         = 0x29,
    kMIPI_DCS_SetColumnAddress     = 0x2a,
    kMIPI_DCS_SetPageAddress       = 0x2b,
    kMIPI_DCS_WriteMemoryStart     = 0x2C,
    kMIPI_DCS_WriteLUT             = 0x2D,
    kMIPI_DCS_ReadMemoryStart      = 0x2E,
    kMIPI_DCS_SetPartialRows       = 0x30,
    kMIPI_DCS_SetPartialColumns    = 0x31,
    kMIPI_DCS_SetScrollArea        = 0x33,
    kMIPI_DCS_SetTearOff           = 0x34,
    kMIPI_DCS_SetTearOn            = 0x35,
    kMIPI_DCS_SetAddressMode       = 0x36,
    kMIPI_DCS_SetScrollStart       = 0x37,
    kMIPI_DCS_ExitIdleMode         = 0x38,
    kMIPI_DCS_EnterIdleMode        = 0x39,
    kMIPI_DCS_SetPixelFormat       = 0x3A,
    kMIPI_DCS_WriteMemoryContinue  = 0x3C,
    kMIPI_DCS_Set3DControl         = 0x3D,
    kMIPI_DCS_ReadMemoryContinue   = 0x3E,
    kMIPI_DCS_Get3DControl         = 0x3F,
    kMIPI_DCS_SetVsyncTiming       = 0x40,
    kMIPI_DCS_SetTearScanline      = 0x44,
    kMIPI_DCS_GetScanline          = 0x45,
    kMIPI_DCS_SetDisplayBrightness = 0x51,
    kMIPI_DCS_GetDisplayBrightness = 0x52,
    kMIPI_DCS_WriteControlDisplay  = 0x53,
    kMIPI_DCS_GetControlDisplay    = 0x54,
    kMIPI_DCS_WritePowerSave       = 0x55,
    kMIPI_DCS_GetPowerSave         = 0x56,
    kMIPI_DCS_SetCABCMinBrightness = 0x5E,
    kMIPI_DCS_GetCABCMinBrightness = 0x5F,
    kMIPI_DCS_ReadDDBStart         = 0xA1,
    kMIPI_DCS_ReadDDBContinue      = 0xA8,
};

/*! @brief MIPI DSI transfer function. */
typedef status_t (*mipi_dsi_transfer_func_t)(dsi_transfer_t *xfer);

/*! @brief MIPI DSI device. */
typedef struct _mipi_dsi_device
{
    uint8_t virtualChannel;
    mipi_dsi_transfer_func_t xferFunc;
} mipi_dsi_device_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Send software reset to MIPI DSI device.
 *
 * @param device The MIPI DSI device.
 * @return Returns @ref kStatus_Success if success, otherwise returns error code.
 */
status_t MIPI_DSI_DCS_SoftReset(mipi_dsi_device_t *device);

/*!
 * @brief Set display on or off.
 *
 * @param device The MIPI DSI device.
 * @param on Set true to turn on, false to turn off.
 * @return Returns @ref kStatus_Success if success, otherwise returns error code.
 */
status_t MIPI_DSI_DCS_SetDisplayOn(mipi_dsi_device_t *device, bool on);

/*!
 * @brief Enter or exit sleep mode.
 *
 * @param device The MIPI DSI device.
 * @param enter Set true to enter sleep mode, false to exit.
 * @return Returns @ref kStatus_Success if success, otherwise returns error code.
 */
status_t MIPI_DSI_DCS_EnterSleepMode(mipi_dsi_device_t *device, bool enter);

/*!
 * @brief Enter or exit partial mode.
 *
 * @param device The MIPI DSI device.
 * @param enter Set true to enter partial mode, false to exit.
 * @return Returns @ref kStatus_Success if success, otherwise returns error code.
 */
status_t MIPI_DSI_DCS_EnterPartialMode(mipi_dsi_device_t *device, bool enter);

/*!
 * @brief Enter or exit invert mode.
 *
 * @param device The MIPI DSI device.
 * @param enter Set true to enter invert mode, false to exit.
 * @return Returns @ref kStatus_Success if success, otherwise returns error code.
 */
status_t MIPI_DSI_DCS_EnterInvertMode(mipi_dsi_device_t *device, bool enter);

/*!
 * @brief Enter or exit idle mode.
 *
 * @param device The MIPI DSI device.
 * @param enter Set true to enter idle mode, false to exit.
 * @return Returns @ref kStatus_Success if success, otherwise returns error code.
 */
status_t MIPI_DSI_DCS_EnterIdleMode(mipi_dsi_device_t *device, bool enter);

/*!
 * @brief Send DCS command.
 *
 * @param device The MIPI DSI device.
 * @param txData The data to send.
 * @param txDataSize Size of the data to send (in bytes).
 * @return Returns @ref kStatus_Success if success, otherwise returns error code.
 */
status_t MIPI_DSI_DCS_Write(mipi_dsi_device_t *device, const uint8_t *txData, int32_t txDataSize);

/*!
 * @brief Send generic data.
 *
 * @param device The MIPI DSI device.
 * @param txData The data to send.
 * @param txDataSize Size of the data to send (in bytes).
 * @return Returns @ref kStatus_Success if success, otherwise returns error code.
 */
status_t MIPI_DSI_GenericWrite(mipi_dsi_device_t *device, const uint8_t *txData, int32_t txDataSize);

#if defined(__cplusplus)
}
#endif

#endif /* _FSL_MIPI_DSI_CMD_H_ */
