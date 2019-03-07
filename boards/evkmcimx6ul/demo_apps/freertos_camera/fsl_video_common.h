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

#ifndef _FSL_VIDEO_COMMON_H_
#define _FSL_VIDEO_COMMON_H_

#include "fsl_common.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief Pixel format FOURCC. */
#define FSL_VIDEO_FOURCC(a, b, c, d) \
    ((uint32_t)(a) | ((uint32_t)(b) << 8U) | ((uint32_t)(c) << 16U) | ((uint32_t)(d) << 24U))

/*! @brief Macro to define resolution. */
#define FSL_VIDEO_RESOLUTION(width, height) ((uint32_t)(width) | ((uint32_t)(height) << 16U))

#define FSL_VIDEO_EXTRACT_WIDTH(resolution) ((uint16_t)((resolution)&0xFFFFU))
#define FSL_VIDEO_EXTRACT_HEIGHT(resolution) ((uint16_t)((resolution) >> 16U))

/*! @brief Pixel format definition. */
typedef enum _video_pixel_format
{
    kVIDEO_PixelFormatYUYV = FSL_VIDEO_FOURCC('Y', 'U', 'Y', 'V'),   /*!< YUV422, Y-U-Y-V. */
    kVIDEO_PixelFormatYUV444 = FSL_VIDEO_FOURCC('Y', '4', '4', '4'), /*!< YUV444, X-Y-U-V. */
    kVIDEO_PixelFormatRGB565 = FSL_VIDEO_FOURCC('R', 'G', 'B', 'P'), /*!< RGB565. */
    kVIDEO_PixelFormatXRGB32 = FSL_VIDEO_FOURCC('B', 'X', '3', '2'), /*!< 32-bit XRGB8888. */
} video_pixel_format_t;

/*! @brief Resolution definition. */
typedef enum _video_resolution
{
    kVIDEO_ResolutionVGA = FSL_VIDEO_RESOLUTION(640, 480),   /*!< VGA, 640 * 480 */
    kVIDEO_ResolutionQVGA = FSL_VIDEO_RESOLUTION(320, 240),  /*!< QVGA, 320 * 240 */
    kVIDEO_Resolution720P = FSL_VIDEO_RESOLUTION(1280, 720), /*!< 720P, 1280 * 720 */
} video_resolution_t;

#endif /* _FSL_VIDEO_COMMON_H_ */
