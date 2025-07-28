/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_display.h"
#include "fsl_co5300.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define co5300_DelayMs VIDEO_DelayMs

/*******************************************************************************
 * Variables
 ******************************************************************************/

typedef struct _co5300_setting
{
    const uint8_t *value;
    uint8_t len;
} co5300_setting_t;

#define co5300_MAKE_SETTING_ITEM(setting)  \
    {                                       \
        (setting), (uint8_t)sizeof(setting) \
    }

static const uint8_t s_co5300Cmd0[]  = {0xFE, 0x20};
static const uint8_t s_co5300Cmd1[]  = {0xF4, 0x5A};
static const uint8_t s_co5300Cmd2[]  = {0xF5, 0x59};
static const uint8_t s_co5300Cmd3[]  = {0xFE, 0x40};
static const uint8_t s_co5300Cmd4[]  = {0x96, 0x00}; /* Set ELVDD to 4.6v */
static const uint8_t s_co5300Cmd5[]  = {0xC9, 0x00}; /* Set ELVSS to -2.4V */
static const uint8_t s_co5300Cmd6[]  = {0xFE, 0x00};
static const uint8_t s_co5300Cmd7[]  = {0x35, 0x00}; /* Enable tear effect */
static const uint8_t s_co5300Cmd8[]  = {0x53, 0x20}; /* Brightness control */
static const uint8_t s_co5300Cmd9[]  = {0x51, 0xFF}; /* Top brightness */
static const uint8_t s_co5300Cmd10[] = {0x63, 0xFF}; /* Top brightness */
/* Partial update, the circular panel actually starts to show form pixel 7, with width/height being 466. */
static const uint8_t s_co5300Cmd11[] = {0x2A, 0x00, 0x06, 0x01, 0xD7};
static const uint8_t s_co5300Cmd12[] = {0x2B, 0x00, 0x00, 0x01, 0xD1};
static const uint8_t s_co5300CmdRGB[] = {0x36, 0x00}; /* 0x0: Set component order to RGB. */
static const uint8_t s_co5300CmdBGR[] = {0x36, 0x08}; /* 0x8: Set component order to BGR. */

static const co5300_setting_t s_co5300InitSetting[] = {
    co5300_MAKE_SETTING_ITEM(s_co5300Cmd0),  co5300_MAKE_SETTING_ITEM(s_co5300Cmd1),
    co5300_MAKE_SETTING_ITEM(s_co5300Cmd2),  co5300_MAKE_SETTING_ITEM(s_co5300Cmd3),
    co5300_MAKE_SETTING_ITEM(s_co5300Cmd4),  co5300_MAKE_SETTING_ITEM(s_co5300Cmd5),
    co5300_MAKE_SETTING_ITEM(s_co5300Cmd6),  co5300_MAKE_SETTING_ITEM(s_co5300Cmd7),
    co5300_MAKE_SETTING_ITEM(s_co5300Cmd8),  co5300_MAKE_SETTING_ITEM(s_co5300Cmd9),
    co5300_MAKE_SETTING_ITEM(s_co5300Cmd10), co5300_MAKE_SETTING_ITEM(s_co5300Cmd11),
    co5300_MAKE_SETTING_ITEM(s_co5300Cmd12),
};

const display_operations_t co5300_ops = {
    .init   = CO5300_Init,
    .deinit = CO5300_Deinit,
    .start  = CO5300_Start,
    .stop   = CO5300_Stop,
};

/*******************************************************************************
 * Code
 ******************************************************************************/

status_t CO5300_Init(display_handle_t *handle, const display_config_t *config)
{
    uint32_t i;
    status_t status = kStatus_Success;
    mipi_dsc_pixel_format_t dscPixelFormat;
    const co5300_resource_t *resource = (const co5300_resource_t *)(handle->resource);
    mipi_dsi_device_t *dsiDevice       = resource->dsiDevice;

    if (config->resolution != FSL_VIDEO_RESOLUTION(480, 466))
    {
        return kStatus_InvalidArgument;
    }

    handle->height = FSL_VIDEO_EXTRACT_HEIGHT(config->resolution);
    handle->width  = FSL_VIDEO_EXTRACT_WIDTH(config->resolution);
    handle->pixelFormat = config->pixelFormat;

    /* Check pixel format. */
    if (config->pixelFormat == kVIDEO_PixelFormatRGB565)
    {
        dscPixelFormat = kMIPI_DCS_Pixel16Bits;
    }
    else if ((kVIDEO_PixelFormatXRGB8888 == config->pixelFormat) || (kVIDEO_PixelFormatRGB888 == config->pixelFormat))
    {
        dscPixelFormat = kMIPI_DCS_Pixel24Bits;
    }
    else
    {
        return kStatus_InvalidArgument;
    }

    /* Power on. */
    resource->pullPowerPin(true);
    co5300_DelayMs(100);

    /* Perform reset. */
    resource->pullResetPin(false);
    co5300_DelayMs(1);
    resource->pullResetPin(true);
    co5300_DelayMs(150);

    /* Set the LCM init settings. */
    for (i = 0; i < ARRAY_SIZE(s_co5300InitSetting); i++)
    {
        status = MIPI_DSI_DCS_Write(dsiDevice, s_co5300InitSetting[i].value, (int32_t)s_co5300InitSetting[i].len);

        if (kStatus_Success != status)
        {
            return status;
        }
    }

    /* Set color component order. For RGB565 the order shall be BGR. */
    if (config->pixelFormat == kVIDEO_PixelFormatRGB565)
    {
        status = MIPI_DSI_DCS_Write(dsiDevice, s_co5300CmdBGR, 2);
    }
    else
    {
        status = MIPI_DSI_DCS_Write(dsiDevice, s_co5300CmdRGB, 2);
    }
    if (kStatus_Success != status)
    {
        return status;
    }

    /* Set pixel format. */
    status = MIPI_DSI_DCS_SetPixelFormat(dsiDevice, dscPixelFormat, kMIPI_DCS_Pixel24Bits);
    if (kStatus_Success != status)
    {
        return status;
    }

    co5300_DelayMs(50);

    /* Sleep out. */
    status = MIPI_DSI_DCS_EnterSleepMode(dsiDevice, false);
    if (kStatus_Success != status)
    {
        return status;
    }

    co5300_DelayMs(150);

    return MIPI_DSI_DCS_SetDisplayOn(dsiDevice, true);
}

status_t CO5300_Deinit(display_handle_t *handle)
{
    const co5300_resource_t *resource = (const co5300_resource_t *)(handle->resource);
    mipi_dsi_device_t *dsiDevice       = resource->dsiDevice;

    (void)MIPI_DSI_DCS_EnterSleepMode(dsiDevice, true);

    resource->pullResetPin(false);
    resource->pullPowerPin(false);

    return kStatus_Success;
}

status_t CO5300_Start(display_handle_t *handle)
{
    const co5300_resource_t *resource = (const co5300_resource_t *)(handle->resource);
    mipi_dsi_device_t *dsiDevice      = resource->dsiDevice;

    return MIPI_DSI_DCS_SetDisplayOn(dsiDevice, true);
}

status_t CO5300_Stop(display_handle_t *handle)
{
    const co5300_resource_t *resource = (const co5300_resource_t *)(handle->resource);
    mipi_dsi_device_t *dsiDevice      = resource->dsiDevice;

    return MIPI_DSI_DCS_SetDisplayOn(dsiDevice, false);
}
