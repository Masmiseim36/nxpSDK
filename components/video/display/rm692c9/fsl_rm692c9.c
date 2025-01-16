/*
 * Copyright 2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_display.h"
#include "fsl_rm692c9.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define RM692c9_DelayMs VIDEO_DelayMs

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* The lcmInitSetting table for RM692c9 */
static const uint8_t lcmInitSetting[][2] = {
    {0xFE, 0x00}, {0xC2, 0x08}, {0x35, 0x00},
};

const display_operations_t rm692c9_ops = {
    .init   = RM692c9_Init,
    .deinit = RM692c9_Deinit,
    .start  = RM692c9_Start,
    .stop   = RM692c9_Stop,
};

/*******************************************************************************
 * Code
 ******************************************************************************/

status_t RM692c9_Init(display_handle_t *handle, const display_config_t *config)
{
    uint32_t i;
    status_t status                    = kStatus_Success;
    const rm692c9_resource_t *resource = (const rm692c9_resource_t *)(handle->resource);
    mipi_dsi_device_t *dsiDevice       = resource->dsiDevice;

    /* Perform reset. */
    resource->pullResetPin(true);

    RM692c9_DelayMs(10);

    resource->pullResetPin(false);

    RM692c9_DelayMs(5);

    resource->pullResetPin(true);
    RM692c9_DelayMs(20);

    /* Set the LCM MIPI_DSI_DCS_SoftResetinit settings. */
    for (i = 0; i < ARRAY_SIZE(lcmInitSetting); i++)
    {
        status = MIPI_DSI_GenericWrite(dsiDevice, lcmInitSetting[i], 2);

        if (kStatus_Success != status)
        {
            return status;
        }
    }

    /* Change to send user command. */
    const uint8_t rm692c9UserCmdEntry[] = {RM692c9_WRMAUCCTR, 0x00};
    status = MIPI_DSI_GenericWrite(dsiDevice, rm692c9UserCmdEntry, (int32_t)ARRAY_SIZE(rm692c9UserCmdEntry));
    if (kStatus_Success != status)
    {
        return status;
    }

    /* Software reset */
    status = MIPI_DSI_DCS_SoftReset(dsiDevice);
    if (kStatus_Success != status)
    {
        return status;
    }

    RM692c9_DelayMs(80);

    /* Set DSI mode */
    const uint8_t rm692c9DsiMode[] = {RM692c9_SETDSIMODE, 0x0B};
    status = MIPI_DSI_GenericWrite(dsiDevice, rm692c9DsiMode, (int32_t)ARRAY_SIZE(rm692c9DsiMode));
    if (kStatus_Success != status)
    {
        return status;
    }

    /* Brightness. */
    const uint8_t rm692c9Brightness[] = {RM692c9_WRDISBV, 0xff};
    status = MIPI_DSI_GenericWrite(dsiDevice, rm692c9Brightness, (int32_t)ARRAY_SIZE(rm692c9Brightness));
    if (kStatus_Success != status)
    {
        return status;
    }

    /* Exit sleep mode */
    status = MIPI_DSI_DCS_EnterSleepMode(dsiDevice, false);
    if (kStatus_Success != status)
    {
        return status;
    }

    RM692c9_DelayMs(120);

    /* Set display on. */
    status = MIPI_DSI_DCS_SetDisplayOn(dsiDevice, true);
    if (kStatus_Success != status)
    {
        return status;
    }

    RM692c9_DelayMs(100);

    return kStatus_Success;
}

status_t RM692c9_Deinit(display_handle_t *handle)
{
    status_t status;
    const rm692c9_resource_t *resource = (const rm692c9_resource_t *)(handle->resource);
    mipi_dsi_device_t *dsiDevice       = resource->dsiDevice;

    status = MIPI_DSI_DCS_EnterSleepMode(dsiDevice, true);
    if (kStatus_Success != status)
    {
        return status;
    }

    resource->pullResetPin(false);

    return kStatus_Success;
}

status_t RM692c9_Start(display_handle_t *handle)
{
    const rm692c9_resource_t *resource = (const rm692c9_resource_t *)(handle->resource);
    mipi_dsi_device_t *dsiDevice       = resource->dsiDevice;

    return MIPI_DSI_DCS_SetDisplayOn(dsiDevice, true);
}

status_t RM692c9_Stop(display_handle_t *handle)
{
    const rm692c9_resource_t *resource = (const rm692c9_resource_t *)(handle->resource);
    mipi_dsi_device_t *dsiDevice       = resource->dsiDevice;

    return MIPI_DSI_DCS_SetDisplayOn(dsiDevice, false);
}
