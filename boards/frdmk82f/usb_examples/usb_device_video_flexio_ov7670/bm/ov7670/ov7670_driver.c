/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
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
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
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

#include "ov7670_driver.h"
#include "fsl_sccb_master_driver.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief OV7670 resolution options                                          */
ov7670_resolution_t OV7670_VGA = {640, 480};
ov7670_resolution_t OV7670_QVGA = {320, 240};
ov7670_resolution_t OV7670_QQVGA = {160, 120};
ov7670_resolution_t OV7670_CIF = {352, 288};
ov7670_resolution_t OV7670_QCIF = {176, 144};
ov7670_resolution_t OV7670_QQCIF = {88, 72};

/*! @brief OV7670 resolution options                                          */
ov7670_window_startPoint_t OV7670_WINDOW_START_POINT_VGA_DEFAULT = {140, 16};
ov7670_window_startPoint_t OV7670_WINDOW_START_POINT_QVGA_DEFAULT = {272, 16};
ov7670_window_startPoint_t OV7670_WINDOW_START_POINT_QQVGA_DEFAULT = {140, 16};
ov7670_window_startPoint_t OV7670_WINDOW_START_POINT_CIF_DEFAULT = {140, 16};
ov7670_window_startPoint_t OV7670_WINDOW_START_POINT_QCIF_DEFAULT = {140, 16};
ov7670_window_startPoint_t OV7670_WINDOW_START_POINT_QQCIF_DEFAULT = {140, 16};

/*! @brief Night mode initialization structure data                           */
ov7670_output_format_config_t OV7670_FORMAT_RawBayerRGB = {0x01, 0x00, 0x00};
ov7670_output_format_config_t OV7670_FORMAT_ProcessedBayerRGB = {0x05, 0x00, 0x00};
ov7670_output_format_config_t OV7670_FORMAT_YUV422 = {0x00, 0x00, 0x00};
ov7670_output_format_config_t OV7670_FORMAT_GRB422 = {0x04, 0x00, 0x00};
ov7670_output_format_config_t OV7670_FORMAT_RGB565 = {0x04, 0xd0, 0x00};
ov7670_output_format_config_t OV7670_FORMAT_RGB555 = {0x04, 0xf0, 0x00};
ov7670_output_format_config_t OV7670_FORMAT_xRGB444 = {0x04, 0xd0, 0x2};
ov7670_output_format_config_t OV7670_FORMAT_RGBx444 = {0x04, 0xd0, 0x3};

/*! @brief resolution initialization structure data                           */
ov7670_resolution_config_t OV7670_RESOLUTION_VGA = {0x00, 0x00, 0x00, 0x3a, 0x35, 0x11, 0xf0, 0x02}; /*!< 640 x 480 */
ov7670_resolution_config_t OV7670_RESOLUTION_QVGA_ORIGINAL = {0x10, 0x00, 0x00, 0x3a,
                                                              0x35, 0x11, 0xf0, 0x02};                 /*!< 320 x 240 */
ov7670_resolution_config_t OV7670_RESOLUTION_QVGA = {0x10, 0x04, 0x19, 0x3a, 0x35, 0x11, 0xf1, 0x02};  /*!< 320 x 240 */
ov7670_resolution_config_t OV7670_RESOLUTION_QQVGA = {0x10, 0x04, 0x1A, 0x3a, 0x35, 0x22, 0xf2, 0x02}; /*!< 160 x 120 */

ov7670_resolution_config_t OV7670_RESOLUTION_CIF = {0x20, 0x08, 0x11, 0x3a, 0x35, 0x11, 0xf1, 0x02}; /*!< 352 x 288 */
ov7670_resolution_config_t OV7670_RESOLUTION_QCIF_ORIGINAL = {0x21, 0x08, 0x11, 0x3a,
                                                              0x35, 0x11, 0xf1, 0x02};                 /*!< 176 x 144 */
ov7670_resolution_config_t OV7670_RESOLUTION_QCIF = {0x28, 0x00, 0x11, 0x3a, 0x35, 0x11, 0xf1, 0x02};  /*!< 176 x 144 */
ov7670_resolution_config_t OV7670_RESOLUTION_QQCIF = {0x28, 0x0c, 0x12, 0x3a, 0x35, 0x22, 0xf2, 0x02}; /*!< 88 x 72 */

/*! @brief Special effects configuration initialization structure data       */
ov7670_windowing_config_t OV7670_WINDOW_VGA = {0x36, 0x13, 0x01, 0x0a, 0x02, 0x7a};
ov7670_windowing_config_t OV7670_WINDOW_QVGA = {0x80, 0x15, 0x03, 0x00, 0x03, 0x7b};
ov7670_windowing_config_t OV7670_WINDOW_QQVGA = {0x64, 0x16, 0x04, 0x0a, 0x03, 0x7b};
ov7670_windowing_config_t OV7670_WINDOW_CIF = {0x12, 0x15, 0x0b, 0x0a, 0x03, 0x7b};
ov7670_windowing_config_t OV7670_WINDOW_QCIF = {0x80, 0x39, 0x03, 0x0a, 0x03, 0x7b};
ov7670_windowing_config_t OV7670_WINDOW_QQCIF = {0x64, 0x3a, 0x03, 0x06, 0x03, 0x7b};

/*! @brief Frame rate initialization structure data                           */
ov7670_frame_rate_config_t OV7670_30FPS_26MHZ_XCLK = {0x80, 0x0a, 0x00, 0x00, 0x2b, 0x00};
ov7670_frame_rate_config_t OV7670_25FPS_26MHZ_XCLK = {0x80, 0x0a, 0x00, 0x00, 0x99, 0x00};
ov7670_frame_rate_config_t OV7670_15FPS_26MHZ_XCLK = {0x00, 0x0a, 0x00, 0x00, 0x2b, 0x00};
ov7670_frame_rate_config_t OV7670_14FPS_26MHZ_XCLK = {0x00, 0x0a, 0x00, 0x00, 0x46, 0x00};

ov7670_frame_rate_config_t OV7670_30FPS_24MHZ_XCLK = {0x80, 0x0a, 0x00, 0x00, 0x00, 0x00};
ov7670_frame_rate_config_t OV7670_25FPS_24MHZ_XCLK = {0x80, 0x0a, 0x00, 0x00, 0x66, 0x00};
ov7670_frame_rate_config_t OV7670_15FPS_24MHZ_XCLK = {0x00, 0x0a, 0x00, 0x00, 0x00, 0x00};
ov7670_frame_rate_config_t OV7670_14FPS_24MHZ_XCLK = {0x00, 0x0a, 0x00, 0x00, 0x1a, 0x00};

ov7670_frame_rate_config_t OV7670_30FPS_13MHZ_XCLK = {0x00, 0x4a, 0x00, 0x00, 0x2b, 0x00};
ov7670_frame_rate_config_t OV7670_25FPS_13MHZ_XCLK = {0x00, 0x4a, 0x00, 0x00, 0x99, 0x00};
ov7670_frame_rate_config_t OV7670_15FPS_13MHZ_XCLK = {0x01, 0x4a, 0x00, 0x00, 0x2b, 0x00};
ov7670_frame_rate_config_t OV7670_14FPS_13MHZ_XCLK = {0x01, 0x4a, 0x00, 0x00, 0x46, 0x00};

ov7670_frame_rate_config_t OV7670_30FPS_12MHZ_XCLK = {0x00, 0x4a, 0x00, 0x00, 0x2b, 0x00};
ov7670_frame_rate_config_t OV7670_25FPS_12MHZ_XCLK = {0x00, 0x4a, 0x00, 0x00, 0x66, 0x00};
ov7670_frame_rate_config_t OV7670_15FPS_12MHZ_XCLK = {0x01, 0x4a, 0x00, 0x00, 0x2b, 0x00};
ov7670_frame_rate_config_t OV7670_14FPS_12MHZ_XCLK = {0x01, 0x4a, 0x00, 0x00, 0x46, 0x00};

/*! @brief Night mode initialization structure data                           */
ov7670_night_mode_config_t OV7670_NIGHT_MODE_DISABLED = {0x00};
ov7670_night_mode_config_t OV7670_NIGHT_MODE_AUTO_FR_DIVBY2 = {0xa0};
ov7670_night_mode_config_t OV7670_NIGHT_MODE_AUTO_FR_DIVBY4 = {0xc0};
ov7670_night_mode_config_t OV7670_NIGHT_MODE_AUTO_FR_DIVBY8 = {0xe0};

/*! @brief Banding filter initialization structure data                       */
ov7670_filter_config_t OV7670_FILTER_DISABLED = {0x00, 0x98, 0x7f, 0x02, 0x03, 0x02};
ov7670_filter_config_t OV7670_FILTER_30FPS_60HZ = {0x20, 0x98, 0x7f, 0x02, 0x03, 0x02};
ov7670_filter_config_t OV7670_FILTER_15FPS_60HZ = {0x20, 0x4c, 0x3f, 0x05, 0x07, 0x02};
ov7670_filter_config_t OV7670_FILTER_25FPS_50HZ = {0x20, 0x98, 0x7f, 0x03, 0x03, 0x0a};
ov7670_filter_config_t OV7670_FILTER_14FPS_50HZ = {0x20, 0x4c, 0x3f, 0x06, 0x07, 0x0a};
ov7670_filter_config_t OV7670_FILTER_30FPS_60HZ_AUTO_LIGHT_FREQ_DETECT = {0x20, 0x98, 0x7f, 0x02, 0x03, 0x12};
ov7670_filter_config_t OV7670_FILTER_15FPS_60HZ_AUTO_LIGHT_FREQ_DETECT = {0x20, 0x4c, 0x3f, 0x05, 0x07, 0x12};
ov7670_filter_config_t OV7670_FILTER_25FPS_50HZ_AUTO_LIGHT_FREQ_DETECT = {0x20, 0x98, 0x7f, 0x03, 0x03, 0x1a};
ov7670_filter_config_t OV7670_FILTER_14FPS_50HZ_AUTO_LIGHT_FREQ_DETECT = {0x20, 0x4c, 0x3f, 0x06, 0x07, 0x1a};

/*! @brief White balance initialization structure data                        */
ov7670_white_balance_config_t OV7670_WHITE_BALANCE_DEFAULT = {0x02, 0x9a, 0xc0, 0x55, 0x02, 0x14,
                                                              0xf0, 0x45, 0x61, 0x51, 0x79, 0x08};
ov7670_white_balance_config_t OV7670_WHITE_BALANCE_DISABLED = {0x00, 0x9a, 0xc0, 0x55, 0x02, 0x14,
                                                               0xf0, 0x45, 0x61, 0x51, 0x79, 0x00};
ov7670_white_balance_config_t OV7670_WHITE_BALANCE_SIMPLE = {0x02, 0x9f, 0x10, 0x55, 0x02, 0x14,
                                                             0xf0, 0x45, 0x61, 0x51, 0x79, 0x08};

/*! @brief Light mode configuration initialization structure data             */
ov7670_light_mode_config_t OV7670_LIGHT_MODE_DISABLED = {0x05, 0x0a, 0x08, 0x00, 0x08};
ov7670_light_mode_config_t OV7670_LIGHT_MODE_AUTO = {0xc5, 0x3a, 0x08, 0x00, 0x08};
ov7670_light_mode_config_t OV7670_LIGHT_MODE_SUNNY = {0xc5, 0x6a, 0x5a, 0x00, 0x5c};
ov7670_light_mode_config_t OV7670_LIGHT_MODE_CLOUDY = {0xc5, 0x0a, 0x58, 0x00, 0x60};
ov7670_light_mode_config_t OV7670_LIGHT_MODE_OFFICE = {0xc5, 0x2a, 0x84, 0x00, 0x4c};
ov7670_light_mode_config_t OV7670_LIGHT_MODE_HOME = {0xc5, 0x1a, 0x96, 0x00, 0x40};

/*! @brief Color saturation configuration initialization structure data       */
ov7670_color_saturation_config_t OV7670_COLOR_SATURATION_4PLUS = {0xc0, 0xc0, 0x00, 0x33, 0x8d, 0xc0, 0x9e, 0x02};
ov7670_color_saturation_config_t OV7670_COLOR_SATURATION_3PLUS = {0x99, 0x99, 0x00, 0x28, 0x71, 0x99, 0x9e, 0x02};
ov7670_color_saturation_config_t OV7670_COLOR_SATURATION_2PLUS = {0xc0, 0xc0, 0x00, 0x33, 0x8d, 0xc0, 0x9e, 0x00};
ov7670_color_saturation_config_t OV7670_COLOR_SATURATION_1PLUS = {0x99, 0x99, 0x00, 0x28, 0x71, 0x99, 0x9e, 0x00};
ov7670_color_saturation_config_t OV7670_COLOR_SATURATION_0 = {0x80, 0x80, 0x00, 0x22, 0x5e, 0x80, 0x9e, 0x00};
ov7670_color_saturation_config_t OV7670_COLOR_SATURATION_DEFAULT = {0x40, 0x34, 0x0c, 0x17, 0x29, 0x40, 0x1e, 0x00};
ov7670_color_saturation_config_t OV7670_COLOR_SATURATION_1MINUS = {0x66, 0x66, 0x00, 0x1b, 0x4b, 0x66, 0x9e, 0x00};
ov7670_color_saturation_config_t OV7670_COLOR_SATURATION_2MINUS = {0x40, 0x40, 0x00, 0x11, 0x2f, 0x40, 0x9e, 0x00};

/*! @brief Special effects configuration initialization structure data       */
ov7670_special_effect_config_t OV7670_SPECIAL_EFFECT_ANTIQUE = {0x18, 0, 255};
ov7670_special_effect_config_t OV7670_SPECIAL_EFFECT_SEPHIA = {0x18, 16, 146};
ov7670_special_effect_config_t OV7670_SPECIAL_EFFECT_BLUISH = {0x18, 240, 146};
ov7670_special_effect_config_t OV7670_SPECIAL_EFFECT_GREENISH = {0x18, 0, 30};
ov7670_special_effect_config_t OV7670_SPECIAL_EFFECT_REDISH = {0x18, 90, 240};
ov7670_special_effect_config_t OV7670_SPECIAL_EFFECT_BW = {0x18, 110, 110};
ov7670_special_effect_config_t OV7670_SPECIAL_EFFECT_NEGATIVE = {0x28, 0x80, 0x80};
ov7670_special_effect_config_t OV7670_SPECIAL_EFFECT_BW_NEGATIVE = {0x38, 110, 110};
ov7670_special_effect_config_t OV7670_SPECIAL_EFFECT_NORMAL = {0x0c, 0x80, 0x80};
ov7670_special_effect_config_t OV7670_SPECIAL_EFFECT_DISABLED = {0x08, 0x80, 0x80};

/*! @brief Special effects configuration initialization structure data       */
ov7670_gamma_curve_slope_config_t OV7670_GAMMA_CURVE_SLOPE_DEFAULT = {0x24, 0x04, 0x07, 0x10, 0x28, 0x36, 0x44, 0x52,
                                                                      0x60, 0x6c, 0x78, 0x8c, 0x9e, 0xbb, 0xd2, 0xe5};
ov7670_gamma_curve_slope_config_t OV7670_GAMMA_CURVE_SLOPE1 = {0x20, 0x10, 0x1e, 0x35, 0x5a, 0x69, 0x76, 0x80,
                                                               0x88, 0x8f, 0x96, 0xa3, 0xaf, 0xc4, 0xd7, 0xe8};

/*******************************************************************************
 * Code
 ******************************************************************************/
static volatile uint32_t i2c_state = 0;

static void delay(void)
{
    volatile uint32_t i = 0;
    for (i = 0; i < 1000; ++i)
    {
        /* code */
        __asm("NOP");
    }
}

ov7670_status_t OV7670_Init(ov7670_handler_t *handle, const ov7670_config_t *config)
{
    uint8_t u8TempVal0, u8TempVal1;

    /* Reset Device */
    I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, OV7670_COM7_REG, 0x80);
    /* wait for a least 1ms */
    delay(); /* 5ms */
    /* Read product ID nuumber MSB */
    if (I2C_Read_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, OV7670_PID_REG, &u8TempVal0, 1) !=
        kStatus_OV7670_Success)
    {
        return kStatus_OV7670_Fail;
    }
    /* Read product ID nuumber MSB */
    if (I2C_Read_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, OV7670_VER_REG, &u8TempVal1, 1) !=
        kStatus_OV7670_Success)
    {
        return kStatus_OV7670_Fail;
    }
    if ((u8TempVal0 != OV7670_PID_NUM) && (u8TempVal1 != OV7670_VER_NUM))
    {
        return kStatus_OV7670_Fail;
    }

    /* NULL pointer means default setting. */
    if (config == NULL)
    {
        /* Reset Device */
        I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, OV7670_COM7_REG, 0x80);
        /* wait for a bit */
        delay(); /* 5ms */
    }
    else
    {
        OV7670_Configure(handle, config);
    }
    /* MVFP */
    if (I2C_Read_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, OV7670_MVFP_REG, &u8TempVal1, 1) !=
        kStatus_OV7670_Success)
    {
        return kStatus_OV7670_Fail;
    }
    if (!(u8TempVal1 & OV7670_MVFP_MIRROR_MASK))
    {
        u8TempVal1 |= OV7670_MVFP_MIRROR_MASK;
        I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, OV7670_MVFP_REG, u8TempVal1);
    }
    return kStatus_OV7670_Success;
}

ov7670_status_t OV7670_Deinit(ov7670_handler_t *handle)
{
    i2c_state--;
    if (i2c_state == 0)
    {
        I2C_MasterDeinit(handle->i2cBase);
    }
    return kStatus_OV7670_Success;
}

ov7670_status_t OV7670_ModifyReg(ov7670_handler_t *handle, uint8_t reg, uint8_t clrMask, uint8_t val)
{
    uint8_t retval = 0U;
    uint8_t reg_val = 0U;
    uint8_t tmp = 0U;
    retval = I2C_Read_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, reg, &reg_val, 1);
    if (retval != kStatus_OV7670_Success)
    {
        return kStatus_OV7670_Fail;
    }
    tmp = ~clrMask;
    reg_val &= tmp;
    reg_val |= val;
    retval = I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, reg, reg_val);
    if (retval != kStatus_OV7670_Success)
    {
        return kStatus_OV7670_Fail;
    }
    return kStatus_OV7670_Success;
}

ov7670_status_t OV7670_ContrastAdjustment(ov7670_handler_t *handle, uint8_t val)
{
    ov7670_status_t status = kStatus_OV7670_Success;

    status = I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, OV7670_CONTRAS_CENTER_REG, 0x80);
    status = OV7670_ModifyReg(handle, OV7670_MTXS_REG, 0x80, 0x00);
    status = I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, OV7670_CONTRAS_REG, val);
    return status;
}

ov7670_status_t OV7670_BrightnessAdjustment(ov7670_handler_t *handle, uint8_t val)
{
    ov7670_status_t status = kStatus_OV7670_Success;

    status = I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, OV7670_BRIGHT_REG, val);

    return status;
}

ov7670_status_t OV7670_Configure(ov7670_handler_t *handle, const ov7670_config_t *config)
{
    ov7670_windowing_config_t *windowConfig;
    OV7670_OutputFormat(handle, config->outputFormat);
    OV7670_Resolution(handle, &config->resolution);
    uint32_t u32TempResolution = (config->resolution.width);
    u32TempResolution = (u32TempResolution << 16U);
    u32TempResolution |= config->resolution.heigth;

    switch (u32TempResolution)
    {
        case 0x028001e0:
            windowConfig = (ov7670_windowing_config_t *)&OV7670_WINDOW_VGA;
            break;
        case 0x014000f0:
            windowConfig = (ov7670_windowing_config_t *)&OV7670_WINDOW_QVGA;
            break;
        case 0x00a00078:
            windowConfig = (ov7670_windowing_config_t *)&OV7670_WINDOW_QQVGA;
            break;
        case 0x01600120:
            windowConfig = (ov7670_windowing_config_t *)&OV7670_WINDOW_CIF;
            break;
        case 0x00b00090:
            windowConfig = (ov7670_windowing_config_t *)&OV7670_WINDOW_QCIF;
            break;
        case 0x00580048:
            windowConfig = (ov7670_windowing_config_t *)&OV7670_WINDOW_QQCIF;
            break;
        default:
            return kStatus_OV7670_Fail; /* not supported resolution */
    }
    OV7670_SetWindow(handle, windowConfig);
    OV7670_FrameRateAdjustment(handle, config->frameRate);

    if (config->advancedConfig == NULL)
    {
        OV7670_BandingFilterSelection(handle, (ov7670_filter_config_t *)&OV7670_FILTER_25FPS_50HZ);
        OV7670_NighMode(handle, (ov7670_night_mode_config_t *)&OV7670_NIGHT_MODE_DISABLED);
        OV7670_WhiteBalance(handle, (ov7670_white_balance_config_t *)&OV7670_WHITE_BALANCE_SIMPLE);
        OV7670_LightMode(handle, (ov7670_light_mode_config_t *)&OV7670_LIGHT_MODE_DISABLED);
        OV7670_ColorSaturation(handle, (ov7670_color_saturation_config_t *)&OV7670_COLOR_SATURATION_2PLUS);
        OV7670_SpecialEffects(handle, (ov7670_special_effect_config_t *)&OV7670_SPECIAL_EFFECT_DISABLED);
        OV7670_GammaCurveSlope(handle, (ov7670_gamma_curve_slope_config_t *)&OV7670_GAMMA_CURVE_SLOPE_DEFAULT);
    }
    else
    {
        OV7670_BandingFilterSelection(handle, config->advancedConfig->filter);
        OV7670_NighMode(handle, config->advancedConfig->nightMode);
        OV7670_WhiteBalance(handle, config->advancedConfig->whiteBalance);
        OV7670_LightMode(handle, config->advancedConfig->lightMode);
        OV7670_ColorSaturation(handle, config->advancedConfig->colorSaturation);
        OV7670_SpecialEffects(handle, config->advancedConfig->specialEffect);
        OV7670_GammaCurveSlope(handle, config->advancedConfig->gammaCurveSlope);
    }

    OV7670_ContrastAdjustment(handle, config->contrast);
    OV7670_BrightnessAdjustment(handle, config->brightness);
    I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, 0xb0, 0x84); /*!< because of colors */
    I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, 0xff, 0xff);

    return kStatus_OV7670_Success;
}

ov7670_status_t OV7670_OutputFormat(ov7670_handler_t *handle, const ov7670_output_format_config_t *outputFormatConfig)
{
    OV7670_ModifyReg(handle, OV7670_COM7_REG, 0x05, outputFormatConfig->com7);
    OV7670_ModifyReg(handle, OV7670_RGB444_REG, 0x03, outputFormatConfig->rgb444);
    OV7670_ModifyReg(handle, OV7670_COM15_REG, 0x30, outputFormatConfig->com15);

    return kStatus_OV7670_Success;
}

ov7670_status_t OV7670_Resolution(ov7670_handler_t *handle, const ov7670_resolution_t *resolution)
{
    ov7670_resolution_config_t *resolution_config;
    uint32_t u32TempResolution;
    u32TempResolution = resolution->width;
    u32TempResolution = u32TempResolution << 16;
    u32TempResolution |= resolution->heigth;
    switch (u32TempResolution)
    {
        case 0x028001e0:
            resolution_config = (ov7670_resolution_config_t *)&OV7670_RESOLUTION_VGA;
            break;
        case 0x014000f0:
            resolution_config = (ov7670_resolution_config_t *)&OV7670_RESOLUTION_QVGA;
            break;
        case 0x00a00078:
            resolution_config = (ov7670_resolution_config_t *)&OV7670_RESOLUTION_QQVGA;
            break;
        case 0x01600120:
            resolution_config = (ov7670_resolution_config_t *)&OV7670_RESOLUTION_CIF;
            break;
        case 0x00b00090:
            resolution_config = (ov7670_resolution_config_t *)&OV7670_RESOLUTION_QCIF;
            break;
        case 0x00580048:
            resolution_config = (ov7670_resolution_config_t *)&OV7670_RESOLUTION_QQCIF;
            break;
        default:
            return kStatus_OV7670_Fail; /*!< not supported resolution */
    }

    OV7670_ModifyReg(handle, OV7670_COM7_REG, 0x38, resolution_config->com7);
    OV7670_ModifyReg(handle, OV7670_COM3_REG, 0x0c, resolution_config->com3);
    I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, OV7670_COM14_REG, resolution_config->com14);
    I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, OV7670_SCALING_XSC_REG,
                         resolution_config->scalingXsc);
    I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, OV7670_SCALING_YSC_REG,
                         resolution_config->scalingYsc);
    OV7670_ModifyReg(handle, OV7670_SCALING_DCWCTR_REG, 0x33, resolution_config->scalingDcwctr);
    I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, OV7670_SCALING_PCLK_DIV_REG,
                         resolution_config->scalingPclkDiv);
    I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, OV7670_SCALING_PCLK_DELAY_REG,
                         resolution_config->scalingPclkDelay);

    /* Autotomatically set output window after resolution change                */
    OV7670_ModifyReg(handle, OV7670_TSLB_REG, 0x01, 0x01);

    return kStatus_OV7670_Success;
}

ov7670_status_t OV7670_SetWindow(ov7670_handler_t *handle, const ov7670_windowing_config_t *windowingConfig)
{
    OV7670_ModifyReg(handle, OV7670_TSLB_REG, 0x01, 0x00);

    I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, OV7670_HREF_REG, windowingConfig->href);
    I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, OV7670_HSTART_REG, windowingConfig->hstart);
    I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, OV7670_HSTOP_REG, windowingConfig->hstop);
    I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, OV7670_VREF_REG, windowingConfig->vref);
    I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, OV7670_VSTART_REG, windowingConfig->vstart);
    I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, OV7670_VSTOP_REG, windowingConfig->vstop);

    return kStatus_OV7670_Success;
}

ov7670_status_t OV7670_FrameRateAdjustment(ov7670_handler_t *handle,
                                           const ov7670_frame_rate_config_t *frameRateConfig)
{
    I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, OV7670_CLKRC_REG, frameRateConfig->clkrc);
    delay();
    I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, OV7670_DBLV_REG, frameRateConfig->dblv);
    delay();
    I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, OV7670_EXHCH_REG, frameRateConfig->exhch);
    delay();
    I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, OV7670_EXHCL_REG, frameRateConfig->exhcl);
    delay();
    I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, OV7670_DM_LNL_REG, frameRateConfig->dm_lnl);
    delay();
    I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, OV7670_DM_LNL_REG, frameRateConfig->dm_lnh);
    delay();

    return kStatus_OV7670_Success;
}

ov7670_status_t OV7670_NighMode(ov7670_handler_t *handle, const ov7670_night_mode_config_t *nightModeConfig)
{
    OV7670_ModifyReg(handle, OV7670_COM11_REG, 0xe0, nightModeConfig->com11);
    delay();

    return kStatus_OV7670_Success;
}

ov7670_status_t OV7670_BandingFilterSelection(ov7670_handler_t *handle, const ov7670_filter_config_t *filterConfig)
{
    I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, OV7670_BD50ST_REG,
                         filterConfig->bd50st); /*!< 50Hz banding filter */
    I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, OV7670_BD60ST_REG,
                         filterConfig->bd60st); /*!< 60Hz banding filter */
    I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, OV7670_BD50MAX_REG,
                         filterConfig->bd50max); /*!< x step for 50hz */
    I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, OV7670_BD60MAX_REG,
                         filterConfig->bd60max);                           /*!< y step for 60hz */
    OV7670_ModifyReg(handle, OV7670_COM11_REG, 0x1a, filterConfig->com11); /*!< Automatic Detect banding filter */
    OV7670_ModifyReg(handle, OV7670_COM8_REG, 0x20, filterConfig->com8);   /*!< banding filter enable */

    return kStatus_OV7670_Success;
}

ov7670_status_t OV7670_WhiteBalance(ov7670_handler_t *handle, const ov7670_white_balance_config_t *whiteBalanceConfig)
{
    OV7670_ModifyReg(handle, OV7670_COM8_REG, 0x02, whiteBalanceConfig->com8); /*!< AWB on/off  */
    I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, OV7670_AWBCTR0_REG, whiteBalanceConfig->awbctr0);
    I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, OV7670_AWBCTR1_REG, whiteBalanceConfig->awbctr1);
    I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, OV7670_AWBCTR2_REG, whiteBalanceConfig->awbctr2);
    I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, OV7670_AWBCTR3_REG, whiteBalanceConfig->awbctr3);
    I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, OV7670_AWBC1_REG, whiteBalanceConfig->awbc1);
    I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, OV7670_AWBC2_REG, whiteBalanceConfig->awbc2);
    I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, OV7670_AWBC3_REG, whiteBalanceConfig->awbc3);
    I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, OV7670_AWBC4_REG, whiteBalanceConfig->awbc4);
    I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, OV7670_AWBC5_REG, whiteBalanceConfig->awbc5);
    I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, OV7670_AWBC6_REG, whiteBalanceConfig->awbc6);
    I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, 0x59, 0x91);
    I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, 0x5a, 0x94);
    I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, 0x5b, 0xaa);
    I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, 0x5c, 0x71);
    I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, 0x5d, 0x8d);
    I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, 0x5e, 0x0f);
    I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, 0x5f, 0xf0);
    I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, 0x60, 0xf0);
    I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, 0x61, 0xf0);
    OV7670_ModifyReg(handle, OV7670_COM16_REG, 0x08, whiteBalanceConfig->com16); /*!< AWB gain on */

    return kStatus_OV7670_Success;
}

ov7670_status_t OV7670_LightMode(ov7670_handler_t *handle, const ov7670_light_mode_config_t *lightModeConfig)
{
    OV7670_ModifyReg(handle, OV7670_COM8_REG, 0xc5, lightModeConfig->com8);
    OV7670_ModifyReg(handle, OV7670_COM9_REG, 0x7a, lightModeConfig->com9);
    I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, OV7670_RED_REG, lightModeConfig->red);
    I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, OV7670_GGAIN_REG, lightModeConfig->green);
    I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, OV7670_BLUE_REG, lightModeConfig->blue);
    I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, OV7670_GAIN_REG, 0x00);
    delay();

    /*!< Exposure value */
    I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, OV7670_AECH_REG, 0x00);
    delay();
    I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, OV7670_AECHH_REG, 0x00);
    delay();
    OV7670_ModifyReg(handle, OV7670_COM1_REG, 0x3, 0x00);
    delay();
    /*!< AGC/AEC stable operation region configuration */
    I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, OV7670_AEW_REG, 0x75);
    delay();
    I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, OV7670_AEB_REG, 0x63);
    delay();
    I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, OV7670_VPT_REG, 0xd4);
    delay();

    return kStatus_OV7670_Success;
}

ov7670_status_t OV7670_ColorSaturation(ov7670_handler_t *handle,
                                       const ov7670_color_saturation_config_t *colorSaturationConfig)
{
    I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, OV7670_MTX1_REG, colorSaturationConfig->mtx1);
    I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, OV7670_MTX2_REG, colorSaturationConfig->mtx2);
    I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, OV7670_MTX3_REG, colorSaturationConfig->mtx3);
    I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, OV7670_MTX4_REG, colorSaturationConfig->mtx4);
    I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, OV7670_MTX5_REG, colorSaturationConfig->mtx5);
    I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, OV7670_MTX6_REG, colorSaturationConfig->mtx6);
    I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, OV7670_MTXS_REG, colorSaturationConfig->mtxs);
    OV7670_ModifyReg(handle, OV7670_COM16_REG, 0x02, colorSaturationConfig->com16);

    return kStatus_OV7670_Success;
}

ov7670_status_t OV7670_SpecialEffects(ov7670_handler_t *handle,
                                      const ov7670_special_effect_config_t *specialEffectConfig)
{
    OV7670_ModifyReg(handle, OV7670_TSLB_REG, 0xfe, specialEffectConfig->tslb);
    I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, OV7670_MANU_REG, specialEffectConfig->manu);
    I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, OV7670_MANV_REG, specialEffectConfig->manv);

    return kStatus_OV7670_Success;
}

ov7670_status_t OV7670_SetWindowByCoordinates(ov7670_handler_t *handle,
                                              ov7670_window_startPoint_t *startPoint,
                                              ov7670_resolution_t *resolution)
{
    uint16_t u16Temp, u16Href, u16Vref;

    OV7670_ModifyReg(handle, OV7670_TSLB_REG, 0x01, 0x00);

    u16Temp = startPoint->hstartCoordinate + resolution->width;
    u16Href = (u16Temp & 0x07U);
    u16Href = u16Href << 3;
    u16Href |= (startPoint->hstartCoordinate & 0x07U);
    u16Href |= 0xc0;
    I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, OV7670_HREF_REG, (uint8_t)u16Href);
    u16Temp = u16Temp >> 3;
    I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, OV7670_HSTOP_REG, (uint8_t)u16Temp);
    u16Temp = ((startPoint->hstartCoordinate & 0x7f8U) >> 3);
    I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, OV7670_HSTART_REG, (uint8_t)u16Temp);

    u16Temp = startPoint->vstartCoordinate + resolution->heigth;
    u16Vref = (u16Temp & 0x03);
    u16Vref = u16Vref << 2;
    u16Vref |= (startPoint->vstartCoordinate & 0x03U);
    u16Vref &= 0xF0;
    OV7670_ModifyReg(handle, OV7670_VREF_REG, 0xc0, (uint8_t)u16Vref);
    u16Temp = u16Temp >> 2;
    I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, OV7670_VSTOP_REG, (uint8_t)u16Temp);
    u16Temp = ((startPoint->vstartCoordinate & 0x3fcU) >> 2);
    I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, OV7670_VSTART_REG, (uint8_t)u16Temp);

    return kStatus_OV7670_Success;
}

ov7670_status_t OV7670_GammaCurveSlope(ov7670_handler_t *handle,
                                       const ov7670_gamma_curve_slope_config_t *gammaCurveSlopeConfig)
{
    I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, OV7670_SLOP_REG, gammaCurveSlopeConfig->slope);
    I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, OV7670_GAM1_REG, gammaCurveSlopeConfig->gam1);
    I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, OV7670_GAM2_REG, gammaCurveSlopeConfig->gam2);
    I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, OV7670_GAM3_REG, gammaCurveSlopeConfig->gam3);
    I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, OV7670_GAM4_REG, gammaCurveSlopeConfig->gam4);
    I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, OV7670_GAM5_REG, gammaCurveSlopeConfig->gam5);
    I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, OV7670_GAM6_REG, gammaCurveSlopeConfig->gam6);
    I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, OV7670_GAM7_REG, gammaCurveSlopeConfig->gam7);
    I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, OV7670_GAM8_REG, gammaCurveSlopeConfig->gam8);
    I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, OV7670_GAM9_REG, gammaCurveSlopeConfig->gam9);
    I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, OV7670_GAM10_REG, gammaCurveSlopeConfig->gam10);
    I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, OV7670_GAM11_REG, gammaCurveSlopeConfig->gam11);
    I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, OV7670_GAM12_REG, gammaCurveSlopeConfig->gam12);
    I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, OV7670_GAM13_REG, gammaCurveSlopeConfig->gam13);
    I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, OV7670_GAM14_REG, gammaCurveSlopeConfig->gam14);
    I2C_Write_OV7670_Reg(handle->i2cBase, handle->i2cDeviceAddr, OV7670_GAM15_REG, gammaCurveSlopeConfig->gam15);

    return kStatus_OV7670_Success;
}
