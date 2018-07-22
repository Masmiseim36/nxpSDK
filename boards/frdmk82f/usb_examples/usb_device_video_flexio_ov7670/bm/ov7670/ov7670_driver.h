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

#ifndef _OV7670_DRIVER_H_
#define _OV7670_DRIVER_H_

#include "fsl_common.h"
#include "fsl_i2c.h"
#include "ov7670_def.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @brief OV7670 I2C address. */
#define OV7670_I2C_ADDR 0x21U
#define OV7670_I2C_BAUDRATE 100U
#define OV7670_I2C_TIMEOUT_MS 500U
#define OV7670_I2C_CLK_SRC kCLOCK_CoreSysClk

/*!
 * @addtogroup ov7670
 * @{
 */

/*! @brief Resolution structure                                               */
typedef struct ov7670_resolution
{
    uint16_t width;
    uint16_t heigth;
} ov7670_resolution_t;
/*! end of Resolution structure                                               */

/*! @brief Configuration of window by setting the starting point configuration structure */
typedef struct ov7670_window_startPoint
{
    uint16_t hstartCoordinate;
    uint16_t vstartCoordinate;
} ov7670_window_startPoint_t;

/*! end of ov7670 handler configuration structure                             */

/*! @brief Output format configuration structure                              */
typedef struct ov7670_output_format_config
{
    uint8_t com7;
    uint8_t com15;
    uint8_t rgb444;
} ov7670_output_format_config_t;
/*! end of Output format configuration structure                              */

/*! @brief Resolution configuration structure                                 */
typedef struct ov7670_resolution_config
{
    uint8_t com7;
    uint8_t com3;
    uint8_t com14;
    uint8_t scalingXsc;
    uint8_t scalingYsc;
    uint8_t scalingDcwctr;
    uint8_t scalingPclkDiv;
    uint8_t scalingPclkDelay;
} ov7670_resolution_config_t;
/*! end of Output format configuration structure                              */

/*! @brief Frame rate configuration data structure                            */
typedef struct ov7670_frame_rate_config
{
    uint8_t clkrc;
    uint8_t dblv;
    uint8_t exhch;
    uint8_t exhcl;
    uint8_t dm_lnl;
    uint8_t dm_lnh;
} ov7670_frame_rate_config_t;
/*! end of Frame rate configuration data structure                            */

/*! @brief Night mode configuration data structure                            */
typedef struct ov7670_night_mode_config
{
    uint8_t com11;
} ov7670_night_mode_config_t;
/*! end of night mode configuration data structure                            */

/*! @brief Banding filter selection data structure                            */
typedef struct ov7670_filter_config
{
    uint8_t com8;
    uint8_t bd50st;
    uint8_t bd60st;
    uint8_t bd50max;
    uint8_t bd60max;
    uint8_t com11;
} ov7670_filter_config_t;
/*! end of Banding filter selection data structure                            */

/*! @brief White balance configuration structure                              */
typedef struct ov7670_white_balance_config
{
    uint8_t com8;
    uint8_t awbctr0;
    uint8_t awbctr1;
    uint8_t awbctr2;
    uint8_t awbctr3;
    uint8_t awbc1;
    uint8_t awbc2;
    uint8_t awbc3;
    uint8_t awbc4;
    uint8_t awbc5;
    uint8_t awbc6;
    uint8_t com16;
} ov7670_white_balance_config_t;
/*! end of White balance configuration structure                              */

/*! @brief Light mode configuration structure                                 */
typedef struct ov7670_light_mode_config
{
    uint8_t com8;
    uint8_t com9;
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} ov7670_light_mode_config_t;
/*! end of Light mode configuration structure                                 */

/*! @brief Color saturation configuration structure                           */
typedef struct ov7670_color_saturation_config
{
    uint8_t mtx1;
    uint8_t mtx2;
    uint8_t mtx3;
    uint8_t mtx4;
    uint8_t mtx5;
    uint8_t mtx6;
    uint8_t mtxs;
    uint8_t com16;
} ov7670_color_saturation_config_t;
/*! end of Color saturation configuration structure                           */

/*! @brief Special effects configuration structure                          */
typedef struct ov7670_special_effect_config
{
    uint8_t tslb;
    uint8_t manu;
    uint8_t manv;
} ov7670_special_effect_config_t;
/*! end of Special effects configuration structure                            */

/*! @brief Windowing configuration structure                                  */
typedef struct ov7670_windowing_config
{
    uint8_t href;
    uint8_t hstart;
    uint8_t hstop;
    uint8_t vref;
    uint8_t vstart;
    uint8_t vstop;
} ov7670_windowing_config_t;

/*! @brief Gamma curve slope configuration structure                          */
typedef struct ov7670_gammaCurveSlopeConfig
{
    uint8_t slope;
    uint8_t gam1;
    uint8_t gam2;
    uint8_t gam3;
    uint8_t gam4;
    uint8_t gam5;
    uint8_t gam6;
    uint8_t gam7;
    uint8_t gam8;
    uint8_t gam9;
    uint8_t gam10;
    uint8_t gam11;
    uint8_t gam12;
    uint8_t gam13;
    uint8_t gam14;
    uint8_t gam15;
} ov7670_gamma_curve_slope_config_t;
/*! end of Gamma curve slope  configuration structure                         */

/*! @brief ov7670 return status. */
typedef enum _ov7670_status
{
    kStatus_OV7670_Success = 0x0, /*!< success */
    kStatus_OV7670_I2CFail = 0x1, /*!< I2C failure */
    kStatus_OV7670_Fail = 0x2,    /*!< fail */
} ov7670_status_t;

/*! @brief ov7670 handler configuration structure                             */
typedef struct ov7670_handler
{
    /* I2C revelant definition. */
    I2C_Type *i2cBase;     /*!< I2C instance. */
    uint8_t i2cDeviceAddr; /*!< I2C device address */
} ov7670_handler_t;

/*! @brief Advanced initialization structure of ov7670 */
typedef struct ov7670_advanced_config
{
    ov7670_filter_config_t *filter;
    ov7670_night_mode_config_t *nightMode;
    ov7670_white_balance_config_t *whiteBalance;
    ov7670_light_mode_config_t *lightMode;
    ov7670_color_saturation_config_t *colorSaturation;
    ov7670_special_effect_config_t *specialEffect;
    ov7670_gamma_curve_slope_config_t *gammaCurveSlope;
} ov7670_advanced_config_t;

/*! @brief Initializion structure of ov7670 */
typedef struct ov7670_config
{
    ov7670_output_format_config_t *outputFormat;
    ov7670_resolution_t resolution;
    ov7670_frame_rate_config_t *frameRate;
    uint8_t contrast;
    uint8_t brightness;
    ov7670_advanced_config_t *advancedConfig;
} ov7670_config_t;

/*******************************************************************************
 * Constants
 ******************************************************************************/
/*! @brief OV7670 resolution options                                          */
extern ov7670_resolution_t OV7670_VGA;
extern ov7670_resolution_t OV7670_QVGA;
extern ov7670_resolution_t OV7670_QQVGA;
extern ov7670_resolution_t OV7670_CIF;
extern ov7670_resolution_t OV7670_QCIF;
extern ov7670_resolution_t OV7670_QQCIF;

/*! @brief OV7670 resolution options                                          */
extern ov7670_window_startPoint_t OV7670_WINDOW_START_POINT_VGA_DEFAULT;
extern ov7670_window_startPoint_t OV7670_WINDOW_START_POINT_QVGA_DEFAULT;
extern ov7670_window_startPoint_t OV7670_WINDOW_START_POINT_QQVGA_DEFAULT;
extern ov7670_window_startPoint_t OV7670_WINDOW_START_POINT_CIF_DEFAULT;
extern ov7670_window_startPoint_t OV7670_WINDOW_START_POINT_QCIF_DEFAULT;
extern ov7670_window_startPoint_t OV7670_WINDOW_START_POINT_QQCIF_DEFAULT;

/*! @brief Night mode initialization structure data                           */
extern ov7670_output_format_config_t OV7670_FORMAT_RawBayerRGB;
extern ov7670_output_format_config_t OV7670_FORMAT_ProcessedBayerRGB;
extern ov7670_output_format_config_t OV7670_FORMAT_YUV422;
extern ov7670_output_format_config_t OV7670_FORMAT_GRB422;
extern ov7670_output_format_config_t OV7670_FORMAT_RGB565;
extern ov7670_output_format_config_t OV7670_FORMAT_RGB555;
extern ov7670_output_format_config_t OV7670_FORMAT_xRGB444;
extern ov7670_output_format_config_t OV7670_FORMAT_RGBx444;

/*! @brief resolution initialization structure data                           */
extern ov7670_resolution_config_t OV7670_RESOLUTION_VGA;
extern ov7670_resolution_config_t OV7670_RESOLUTION_QVGA;
extern ov7670_resolution_config_t OV7670_RESOLUTION_QQVGA;

extern ov7670_resolution_config_t OV7670_RESOLUTION_CIF;
extern ov7670_resolution_config_t OV7670_RESOLUTION_QCIF;
extern ov7670_resolution_config_t OV7670_RESOLUTION_QQCIF;

/*! @brief Special effects configuration initialization structure data       */
extern ov7670_windowing_config_t OV7670_WINDOW_VGA;
extern ov7670_windowing_config_t OV7670_WINDOW_QVGA;
extern ov7670_windowing_config_t OV7670_WINDOW_QQVGA;
extern ov7670_windowing_config_t OV7670_WINDOW_CIF;
extern ov7670_windowing_config_t OV7670_WINDOW_QCIF;
extern ov7670_windowing_config_t OV7670_WINDOW_QQCIF;

/*! @brief Frame rate initialization structure data                           */
extern ov7670_frame_rate_config_t OV7670_30FPS_26MHZ_XCLK;
extern ov7670_frame_rate_config_t OV7670_25FPS_26MHZ_XCLK;
extern ov7670_frame_rate_config_t OV7670_15FPS_26MHZ_XCLK;
extern ov7670_frame_rate_config_t OV7670_14FPS_26MHZ_XCLK;

extern ov7670_frame_rate_config_t OV7670_30FPS_24MHZ_XCLK;
extern ov7670_frame_rate_config_t OV7670_25FPS_24MHZ_XCLK;
extern ov7670_frame_rate_config_t OV7670_15FPS_24MHZ_XCLK;
extern ov7670_frame_rate_config_t OV7670_14FPS_24MHZ_XCLK;

extern ov7670_frame_rate_config_t OV7670_30FPS_13MHZ_XCLK;
extern ov7670_frame_rate_config_t OV7670_25FPS_13MHZ_XCLK;
extern ov7670_frame_rate_config_t OV7670_15FPS_13MHZ_XCLK;
extern ov7670_frame_rate_config_t OV7670_14FPS_13MHZ_XCLK;

extern ov7670_frame_rate_config_t OV7670_30FPS_12MHZ_XCLK;
extern ov7670_frame_rate_config_t OV7670_25FPS_12MHZ_XCLK;
extern ov7670_frame_rate_config_t OV7670_15FPS_12MHZ_XCLK;
extern ov7670_frame_rate_config_t OV7670_14FPS_12MHZ_XCLK;

/*! @brief Night mode initialization structure data                           */
extern ov7670_night_mode_config_t OV7670_NIGHT_MODE_DISABLED;
extern ov7670_night_mode_config_t OV7670_NIGHT_MODE_AUTO_FR_DIVBY2;
extern ov7670_night_mode_config_t OV7670_NIGHT_MODE_AUTO_FR_DIVBY4;
extern ov7670_night_mode_config_t OV7670_NIGHT_MODE_AUTO_FR_DIVBY8;

/*! @brief Banding filter initialization structure data                       */
extern ov7670_filter_config_t OV7670_FILTER_DISABLED;
extern ov7670_filter_config_t OV7670_FILTER_30FPS_60HZ;
extern ov7670_filter_config_t OV7670_FILTER_15FPS_60HZ;
extern ov7670_filter_config_t OV7670_FILTER_25FPS_50HZ;
extern ov7670_filter_config_t OV7670_FILTER_14FPS_50HZ;
extern ov7670_filter_config_t OV7670_FILTER_30FPS_60HZ_AUTO_LIGHT_FREQ_DETECT;
extern ov7670_filter_config_t OV7670_FILTER_15FPS_60HZ_AUTO_LIGHT_FREQ_DETECT;
extern ov7670_filter_config_t OV7670_FILTER_25FPS_50HZ_AUTO_LIGHT_FREQ_DETECT;
extern ov7670_filter_config_t OV7670_FILTER_14FPS_50HZ_AUTO_LIGHT_FREQ_DETECT;

/*! @brief White balance initialization structure data                        */
extern ov7670_white_balance_config_t OV7670_WHITE_BALANCE_DEFAULT;
extern ov7670_white_balance_config_t OV7670_WHITE_BALANCE_DISABLED;
extern ov7670_white_balance_config_t OV7670_WHITE_BALANCE_SIMPLE;

/*! @brief Light mode configuration initialization structure data             */
extern ov7670_light_mode_config_t OV7670_LIGHT_MODE_DISABLED;
extern ov7670_light_mode_config_t OV7670_LIGHT_MODE_AUTO;
extern ov7670_light_mode_config_t OV7670_LIGHT_MODE_SUNNY;
extern ov7670_light_mode_config_t OV7670_LIGHT_MODE_CLOUDY;
extern ov7670_light_mode_config_t OV7670_LIGHT_MODE_OFFICE;
extern ov7670_light_mode_config_t OV7670_LIGHT_MODE_HOME;

/*! @brief Color saturation configuration initialization structure data       */
extern ov7670_color_saturation_config_t OV7670_COLOR_SATURATION_4PLUS;
extern ov7670_color_saturation_config_t OV7670_COLOR_SATURATION_3PLUS;
extern ov7670_color_saturation_config_t OV7670_COLOR_SATURATION_2PLUS;
extern ov7670_color_saturation_config_t OV7670_COLOR_SATURATION_1PLUS;
extern ov7670_color_saturation_config_t OV7670_COLOR_SATURATION_0;
extern ov7670_color_saturation_config_t OV7670_COLOR_SATURATION_DEFAULT;
extern ov7670_color_saturation_config_t OV7670_COLOR_SATURATION_1MINUS;
extern ov7670_color_saturation_config_t OV7670_COLOR_SATURATION_2MINUS;
extern ov7670_color_saturation_config_t OV7670_COLOR_SATURATION_RGB565;

/*! @brief Special effects configuration initialization structure data       */
extern ov7670_special_effect_config_t OV7670_SPECIAL_EFFECT_ANTIQUE;
extern ov7670_special_effect_config_t OV7670_SPECIAL_EFFECT_SEPHIA;
extern ov7670_special_effect_config_t OV7670_SPECIAL_EFFECT_BLUISH;
extern ov7670_special_effect_config_t OV7670_SPECIAL_EFFECT_GREENISH;
extern ov7670_special_effect_config_t OV7670_SPECIAL_EFFECT_REDISH;
extern ov7670_special_effect_config_t OV7670_SPECIAL_EFFECT_BW;
extern ov7670_special_effect_config_t OV7670_SPECIAL_EFFECT_NEGATIVE;
extern ov7670_special_effect_config_t OV7670_SPECIAL_EFFECT_BW_NEGATIVE;
extern ov7670_special_effect_config_t OV7670_SPECIAL_EFFECT_NORMAL;
extern ov7670_special_effect_config_t OV7670_SPECIAL_EFFECT_DISABLED;

/*! @brief Special effects configuration initialization structure data       */
extern ov7670_gamma_curve_slope_config_t OV7670_GAMMA_CURVE_SLOPE_DEFAULT;
extern ov7670_gamma_curve_slope_config_t OV7670_GAMMA_CURVE_SLOPE1;

/*******************************************************************************
 * Global variables
 ******************************************************************************/

/*******************************************************************************
 * API
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief ov7670 initialize function.
 *
 * This function would call ov7670_i2c_init(), and in this function, some configurations
 * are fixed. The second parameter is NULL to ov7670 in this version. If users want
 * to change the settings, they have to use ov7670_WriteReg() or OV7670_ModifyReg()
 * to set the register value of ov7670.
 * Note: If the config is NULL, it would initialize ov7670 using default settings.
 * The default setting:
 * @param handle #ov7670_handler_t structure.
 * @param config ov7670 configuration structure #ov7670_config_t.
 */
ov7670_status_t OV7670_Init(ov7670_handler_t *handle, const ov7670_config_t *config);

/*!
 * @brief Deinit the ov7670 codec. Mainly used to close the I2C controller.
 * @param handle ov7670 handler structure pointer.
 */
ov7670_status_t OV7670_Deinit(ov7670_handler_t *handle);

/*!
 * @brief Modify some bits in the register using I2C.
 * @param handle #ov7670_handler_t structure.
 * @param reg The register address in ov7670.
 * @param clrMask The mask code for the bits want to write. The bit you want to write should be 0.
 * @param val Value needs to write into the register.
 */
ov7670_status_t OV7670_ModifyReg(ov7670_handler_t *handle, uint8_t reg, uint8_t clrMask, uint8_t val);

/*!
 * @brief Write CONTRAST register to ov7670 using I2C.
 * @param handle #ov7670_handler_t structure.
 * @param val contrast value.
 */
ov7670_status_t OV7670_ContrastAdjustment(ov7670_handler_t *handle, uint8_t val);

/*!
 * @brief Write BRIGHT register to ov7670 using I2C.
 * @param handle #ov7670_handler_t structure.
 * @param val brightness value.
 */
ov7670_status_t OV7670_BrightnessAdjustment(ov7670_handler_t *handle, uint8_t val);

/*!
 * @brief OV7670 configuration.
 * @param handle #ov7670_handler_t structure.
 * @param config #ov7670_config_t structure.
 */
ov7670_status_t OV7670_Configure(ov7670_handler_t *handle, const ov7670_config_t *config);

/*!
 * @brief OV7670 Output format configuration.
 * @param handle #ov7670_handler_t structure.
 * @param outputFormatConfig #ov7670_output_format_config_t structure.
 */
ov7670_status_t OV7670_OutputFormat(ov7670_handler_t *handle,
                                    const ov7670_output_format_config_t *outputFormatConfig);

/*!
 * @brief OV7670 Resolution configuration.
 * @param handle ov7670 handler structure #ov7670_handler_t.
 * @param resolution #ov7670_resolution_t structure.
 */
ov7670_status_t OV7670_Resolution(ov7670_handler_t *handle, const ov7670_resolution_t *resolution);

/*!
 * @brief OV7670 frame rate adjustment.
 * @param handle #ov7670_handler_t structure.
 * @param frameRateConfig #ov7670_frame_rate_config_t structure.
 */
ov7670_status_t OV7670_FrameRateAdjustment(ov7670_handler_t *handle,
                                           const ov7670_frame_rate_config_t *frameRateConfig);

/*!
 * @brief OV7670 night mode configuration.
 * @param handle #ov7670_handler_t structure.
 * @param nightModeConfig #ov7670_night_mode_config_t structure.
 */
ov7670_status_t OV7670_NighMode(ov7670_handler_t *handle, const ov7670_night_mode_config_t *nightModeConfig);

/*!
 * @brief OV7670 Bnading filter selection configuration.
 * @param handle #ov7670_handler_t structure.
 * @param filterConfig #ov7670_filter_config_t structure.
 */
ov7670_status_t OV7670_BandingFilterSelection(ov7670_handler_t *handle, const ov7670_filter_config_t *filterConfig);

/*!
 * @brief OV7670 White balance configuration.
 * @param handle #ov7670_handler_t structure.
 * @param whiteBalanceConfig #ov7670_white_balance_config_t structure.
 */
ov7670_status_t OV7670_WhiteBalance(ov7670_handler_t *handle,
                                    const ov7670_white_balance_config_t *whiteBalanceConfig);

/*!
 * @brief OV7670 Light mode configuration.
 * @param handle ov7670 handler structure #ov7670_handler_t.
 * @param lightModeConfig #ov7670_light_mode_config_t structure.
 */
ov7670_status_t OV7670_LightMode(ov7670_handler_t *handle, const ov7670_light_mode_config_t *lightModeConfig);

/*!
 * @brief OV7670 Color saturation configuration.
 * @param handle #ov7670_handler_t structure.
 * @param colorSaturationConfig #ov7670_color_saturation_config_t structure.
 */
ov7670_status_t OV7670_ColorSaturation(ov7670_handler_t *handle,
                                       const ov7670_color_saturation_config_t *colorSaturationConfig);

/*!
 * @brief OV7670 Special effects configuration.
 * @param handle #ov7670_handler_t structure.
 * @param specialEffectConfig #ov7670_special_effect_config_t structure.
 */
ov7670_status_t OV7670_SpecialEffects(ov7670_handler_t *handle,
                                      const ov7670_special_effect_config_t *specialEffectConfig);

/*!
 * @brief OV7670 Windowing configuration.
 * @param handle #ov7670_handler_t structure.
 * @param windowingConfig #ov7670_windowing_config_t structure.
 */
ov7670_status_t OV7670_SetWindow(ov7670_handler_t *handle, const ov7670_windowing_config_t *windowingConfig);

/*!
 * @brief OV7670 Configure the window by setting initial coordinates and
 *               resolution.
 * @param handle #ov7670_handler_t structure.
 * @param startPoint #ov7670_window_startPoint_t structure.
 * @param resolution #ov7670_resolution_t structure.
 */
ov7670_status_t OV7670_SetWindowByCoordinates(ov7670_handler_t *handle,
                                              ov7670_window_startPoint_t *startPoint,
                                              ov7670_resolution_t *resolution);

/*!
 * @brief OV7670 Gamma curve slope configuration.
 * @param handle #ov7670_handler_t structure.
 * @param gammaCurveSlopeConfig #ov7670_gamma_curve_slope_config_t structure.
 */
ov7670_status_t OV7670_GammaCurveSlope(ov7670_handler_t *handle,
                                       const ov7670_gamma_curve_slope_config_t *gammaCurveSlopeConfig);

#if defined(__cplusplus)
}
#endif

/*! @} */

#endif /*__OV7670_DRIVER_H__ */
