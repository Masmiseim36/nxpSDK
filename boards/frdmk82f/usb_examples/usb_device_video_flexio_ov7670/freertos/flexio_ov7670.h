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

#ifndef __FLEXIO_OV7670_H__
#define __FLEXIO_OV7670_H__

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define FLEXIO_DMA_CHANNEL (17U)

#define OV7670_BYTES_PER_PIXEL 2U
#define OV7670_HORIZONTAL_POINTS 160U
#define OV7670_VERTICAL_POINTS 120U
#define OV7670_FRAME_PIXELS (OV7670_HORIZONTAL_POINTS * OV7670_VERTICAL_POINTS)
#define OV7670_FRAME_BYTES (OV7670_FRAME_PIXELS * OV7670_BYTES_PER_PIXEL)
#define LCD_HORIZONTAL_START_POINT ((SCREEN_SIZE_LONGER_SIDE - OV7670_HORIZONTAL_POINTS) >> 1)
#define LCD_VERTICAL_START_POINT ((SCREEN_SIZE_SHORTER_SIDE - OV7670_VERTICAL_POINTS) >> 1)
#define LCD_HORIZONTAL_END_POINT (LCD_HORIZONTAL_START_POINT + OV7670_HORIZONTAL_POINTS - 1)
#define LCD_VERTICAL_END_POINT (LCD_VERTICAL_START_POINT + OV7670_VERTICAL_POINTS - 1)

/* OV7670 device advanced configuration structure initialization */
#define OV7670_ADVANCED_CONFIGURATION                                                                \
    \
{                                                                                             \
        .filter = (ov7670_filter_config_t *)&OV7670_FILTER_DISABLED,                                 \
        .nightMode = (ov7670_night_mode_config_t *)&OV7670_NIGHT_MODE_DISABLED,                     \
        .whiteBalance = (ov7670_white_balance_config_t *)&OV7670_WHITE_BALANCE_SIMPLE,              \
        .lightMode = (ov7670_light_mode_config_t *)&OV7670_LIGHT_MODE_DISABLED,                     \
        .colorSaturation = (ov7670_color_saturation_config_t *)&OV7670_COLOR_SATURATION_2PLUS,      \
        .specialEffect = (ov7670_special_effect_config_t *)&OV7670_SPECIAL_EFFECT_DISABLED,         \
        .gammaCurveSlope = (ov7670_gamma_curve_slope_config_t *)&OV7670_GAMMA_CURVE_SLOPE_DEFAULT, \
    \
}

/* OV7670 device configuration structure initialization */
#define OV7670_CONFIGURATION                                                                                        \
    \
{                                                                                                            \
        .outputFormat = (ov7670_output_format_config_t *)&OV7670_FORMAT_RGB565,                                    \
        .resolution =                                                                                               \
            {                                                                                                       \
                .width = OV7670_HORIZONTAL_POINTS, .heigth = OV7670_VERTICAL_POINTS,                                \
        },                                                                                                          \
        .frameRate = (ov7670_frame_rate_config_t *)&OV7670_25FPS_12MHZ_XCLK, .contrast = 0x30, .brightness = 0x10, \
        .advancedConfig = &s_Ov7670CameraAdvancedConfig,                                                           \
    \
}

/*******************************************************************************
 * API
 ******************************************************************************/

void FLEXIO_Ov7670Init(void);
void FLEXIO_Ov7670StartCapture(uint8_t bufferIndex);
void FLEXIO_Ov7670AsynCallback(void);

#endif /* __FLEXIO_OV7670_H__ */
