/*
 * Copyright (c) 2017, NXP Semiconductors, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "fsl_video_common.h"
#include "fsl_camera.h"
#include "fsl_camera_device.h"
#include "fsl_ov5640.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define OV5640_SCCB_ADDR 0x3CU
#define OV5640_RESOLUTION_PARAM_NUM 0x14U

typedef struct _ov5640_reg_val
{
    uint16_t regAddr; /*!< Register address. */
    uint8_t regVal;   /*!<Register value. */
} ov5640_reg_val_t;

typedef struct _ov5640_resolution_param
{
    uint32_t resolution; /*!< Resolution, see @ref video_resolution_t and @ref FSL_VIDEO_RESOLUTION. */
    uint8_t param[OV5640_RESOLUTION_PARAM_NUM]; /*!< Parameter 0x3800 to 0x3813. */
} ov5640_resolution_param_t;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
status_t OV5640_Init(camera_device_handle_t *handle, const camera_config_t *config);

status_t OV5640_Deinit(camera_device_handle_t *handle);

status_t OV5640_Start(camera_device_handle_t *handle);

status_t OV5640_Stop(camera_device_handle_t *handle);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static const ov5640_reg_val_t ov5640InitParam[] = {
    {0x3008, 0x42},

    /* System setting. */
    {0x3103, 0x03},
    {0x3017, 0xff},
    {0x3018, 0xff},
    {0x3034, 0x1a},
    {0x3035, 0x11},
    {0x3036, 0x46},
    {0x3000, 0x00},
    {0x3004, 0xff},
    {0x3002, 0x1c},
    {0x3006, 0xc3},
    {0x300e, 0x58},
    {0x302e, 0x00},
    {0x3037, 0x13},
    {0x3108, 0x01},
    {0x3618, 0x00},
    {0x3612, 0x29},
    {0x3708, 0x64},
    {0x3709, 0x52},
    {0x370c, 0x03},
    {0x3820, 0x41},
    {0x3821, 0x07},
    {0x3630, 0x36},
    {0x3631, 0x0e},
    {0x3632, 0xe2},
    {0x3633, 0x12},
    {0x3621, 0xe0},
    {0x3704, 0xa0},
    {0x3703, 0x5a},
    {0x3715, 0x78},
    {0x3717, 0x01},
    {0x370b, 0x60},
    {0x3705, 0x1a},
    {0x3905, 0x02},
    {0x3906, 0x10},
    {0x3901, 0x0a},
    {0x3731, 0x12},
    {0x3600, 0x08},
    {0x3601, 0x33},
    {0x302d, 0x60},
    {0x3620, 0x52},
    {0x371b, 0x20},
    {0x471c, 0x50},
    {0x3a13, 0x43},
    {0x3a18, 0x00},
    {0x3a19, 0x7c},
    {0x3635, 0x13},
    {0x3636, 0x03},
    {0x3634, 0x40},
    {0x3622, 0x01},
    {0x3c01, 0x00},
    {0x3a00, 0x58},
    {0x3814, 0x31},
    {0x3815, 0x31},
    {0x4001, 0x02},
    {0x4004, 0x02},
    {0x4005, 0x1a},
    {0x5001, 0xa3},
};

static const ov5640_resolution_param_t resolutionParam[] = {
    {
        .resolution = kVIDEO_ResolutionVGA,
        .param = {0x00, 0x00, 0x00, 0x04, 0x0a, 0x3f, 0x07, 0x9b, 0x01, 0x40, 0x00, 0xf0, 0x07, 0x68, 0x03, 0xd8, 0x00,
                  0x10, 0x00, 0x06},
    },
    {
        .resolution = kVIDEO_ResolutionQVGA,
        .param = {0x00, 0x00, 0x00, 0x04, 0x0a, 0x3f, 0x07, 0x9b, 0x01, 0x40, 0x00, 0xf0, 0x07, 0x68, 0x03, 0xd8, 0x00,
                  0x10, 0x00, 0x06},
    },
    {
        .resolution = FSL_VIDEO_RESOLUTION(480, 272),
        .param = {0x00, 0x00, 0x00, 0xfa, 0x0a, 0x3f, 0x06, 0xa9, 0x01, 0xE0, 0x01, 0x10, 0x07, 0x64, 0x02, 0xe4, 0x00,
                  0x10, 0x00, 0x04},
    },
    {
        .resolution = kVIDEO_Resolution720P,
        .param = {0x00, 0x00, 0x00, 0xfa, 0x0a, 0x3f, 0x06, 0xa9, 0x05, 0x00, 0x02, 0xd0, 0x07, 0x64, 0x02, 0xe4, 0x00,
                  0x10, 0x00, 0x04},
    },
};

const camera_device_operations_t ov5640_ops = {
    .init = OV5640_Init, .deinit = OV5640_Deinit, .start = OV5640_Start, .stop = OV5640_Stop,
};

/*******************************************************************************
 * Code
 ******************************************************************************/

static void OV5640_DelayMs(uint32_t ms)
{
    volatile uint32_t i;

    i = SystemCoreClock * ms / 3000;
    while (i--)
    {
    }
}

static status_t OV5640_LoadRegVal(sccb_i2c_t i2c, const ov5640_reg_val_t regVal[], uint32_t num)
{
    uint32_t i;
    status_t status;

    for (i = 0; i < num; i++)
    {
        status = SCCB_WriteReg(i2c, OV5640_SCCB_ADDR, kSCCB_RegAddr16Bit, regVal[i].regAddr, regVal[i].regVal);

        if (kStatus_Success != status)
        {
            return status;
        }
    }

    return kStatus_Success;
}

static status_t OV5640_SoftwareReset(sccb_i2c_t i2c)
{
    ov5640_reg_val_t param[] = {{0x3103, 0x11}, {0x3008, 0x82}};

    return OV5640_LoadRegVal(i2c, param, ARRAY_SIZE(param));
}

static const uint8_t *OV5640_GetResolutionParam(uint32_t resolution)
{
    /*
     * This function returns the parameter for specific resolution, if
     * the resolution is not supported, returns NULL.
     */
    uint32_t i;

    for (i = 0; i < ARRAY_SIZE(resolutionParam); i++)
    {
        if (resolution == resolutionParam[i].resolution)
        {
            return resolutionParam[i].param;
        }
    }

    return NULL;
}

static status_t OV5640_SetPixelFormat(sccb_i2c_t i2c, video_pixel_format_t pixelFormat)
{
    uint8_t param[2];

    switch (pixelFormat)
    {
        case kVIDEO_PixelFormatYUYV:
            param[0] = 0x3F;
            param[1] = 0x00;
            break;

        case kVIDEO_PixelFormatRGB565:
        default:
            param[0] = 0x6f;
            param[1] = 0x01;
            break;
    }

    SCCB_WriteReg(i2c, OV5640_SCCB_ADDR, kSCCB_RegAddr16Bit, 0x4300, param[0]);
    SCCB_WriteReg(i2c, OV5640_SCCB_ADDR, kSCCB_RegAddr16Bit, 0x501f, param[1]);

    return kStatus_Success;
}

status_t OV5640_Init(camera_device_handle_t *handle, const camera_config_t *config)
{
    status_t status;
    ov5640_resource_t *resource = (ov5640_resource_t *)(handle->resource);
    sccb_i2c_t i2c = resource->sccbI2C;

    /* Verify the configuration. */
    const uint8_t *resParam = OV5640_GetResolutionParam(config->resolution);

    if (NULL == resParam)
    {
        return kStatus_InvalidArgument;
    }

    if ((kVIDEO_PixelFormatYUYV != config->pixelFormat) && (kVIDEO_PixelFormatRGB565 != config->pixelFormat))
    {
        return kStatus_InvalidArgument;
    }

    if ((15 != config->framePerSec) && (30 != config->framePerSec))
    {
        return kStatus_InvalidArgument;
    }

    if ((kCAMERA_InterfaceGatedClock != config->interface) && (kCAMERA_InterfaceCCIR656 != config->interface))
    {
        return kStatus_InvalidArgument;
    }

    resource->pullPowerDownPin(true);
    resource->pullResetPin(false);

    /* Delay 5ms. */
    OV5640_DelayMs(5);

    resource->pullPowerDownPin(false);

    /* Delay 1ms. */
    OV5640_DelayMs(1);

    resource->pullResetPin(true);

    /* Delay 20ms. */
    OV5640_DelayMs(20);

    OV5640_SoftwareReset(i2c);

    /* Delay 5ms. */
    OV5640_DelayMs(5);

    /* Initialize. */
    status = OV5640_LoadRegVal(i2c, ov5640InitParam, ARRAY_SIZE(ov5640InitParam));
    if (kStatus_Success != status)
    {
        return status;
    }

    /* Resolution. */
    SCCB_WriteMultiRegs(i2c, OV5640_SCCB_ADDR, kSCCB_RegAddr16Bit, 0x3800, resParam, OV5640_RESOLUTION_PARAM_NUM);

    if (15 == config->framePerSec)
    {
        SCCB_WriteReg(i2c, OV5640_SCCB_ADDR, kSCCB_RegAddr16Bit, 0x3035, 0x21);
    }
    else
    {
        SCCB_WriteReg(i2c, OV5640_SCCB_ADDR, kSCCB_RegAddr16Bit, 0x3035, 0x11);
    }

    /* Pixel format. */
    OV5640_SetPixelFormat(i2c, config->pixelFormat);

    /* CCIR656. */
    if (kCAMERA_InterfaceCCIR656 == config->interface)
    {
        SCCB_WriteReg(i2c, OV5640_SCCB_ADDR, kSCCB_RegAddr16Bit, 0x4719, 0x01);
        SCCB_WriteReg(i2c, OV5640_SCCB_ADDR, kSCCB_RegAddr16Bit, 0x4730, 0x01);
    }

    /* Lenc on, raw gamma on, BPC on, WPC on, CIP on. */
    SCCB_WriteReg(i2c, OV5640_SCCB_ADDR, kSCCB_RegAddr16Bit, 0x5000, 0xa7);

    return kStatus_Success;
}

status_t OV5640_Deinit(camera_device_handle_t *handle)
{
    ((ov5640_resource_t *)(handle->resource))->pullPowerDownPin(true);

    return kStatus_Success;
}

status_t OV5640_Start(camera_device_handle_t *handle)
{
    sccb_i2c_t i2c = ((ov5640_resource_t *)(handle->resource))->sccbI2C;

    SCCB_WriteReg(i2c, OV5640_SCCB_ADDR, kSCCB_RegAddr16Bit, 0x3008, 0x02);

    return kStatus_Success;
}

status_t OV5640_Stop(camera_device_handle_t *handle)
{
    sccb_i2c_t i2c = ((ov5640_resource_t *)(handle->resource))->sccbI2C;

    SCCB_WriteReg(i2c, OV5640_SCCB_ADDR, kSCCB_RegAddr16Bit, 0x3008, 0x42);

    return kStatus_Success;
}
