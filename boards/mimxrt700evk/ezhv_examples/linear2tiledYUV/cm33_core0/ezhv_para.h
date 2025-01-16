/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _EZHV_PARA_
#define _EZHV_PARA_

#include "fsl_common.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#ifndef EZHV_LINEAR_TO_TILE_EN
#define EZHV_LINEAR_TO_TILE_EN    (0)
#endif

#ifndef EZHV_DVP_EN
#define EZHV_DVP_EN               (0)
#endif

#ifndef EZHV_MIP_EN
#define EZHV_MIP_EN               (0)
#endif

/*! @brief definition of EZH-V APIs */
typedef enum _ezhv_api_idx
{
#if (EZHV_LINEAR_TO_TILE_EN)
    kEZHV_API_linear2tile,
#endif
#if (EZHV_DVP_EN)
    kEZHV_API_flexioDvp,
#endif
#if (EZHV_MIP_EN)
    kEZHV_API_MiP,
#endif
} EZHV_ApiIdx_t;

/*! @brief parameters to run which api with given parameters */
typedef struct tag_EzhvSharedData
{
    uint32_t apiIdx;   /*!< value from EZHV_ApiIdx_t */
    uint32_t paraAddr; /*!< &paraAddr is the start location of paras */
} EZHV_Para_t;

#if (EZHV_LINEAR_TO_TILE_EN)
/*! @brief color format conversion by EZH-V */
typedef enum _fmt_conv
{
    /* YUV420 (2-planar, Y at the first planar, and UV at the second planar) */
    kFMT_nv12_linear2rgb565      = 0U,
    kFMT_nv12_linear2tiled       = 1U,
    /* YUV422 (1-planar in the YUYV sequence) */
    kFMT_yuyv_linear2tiled       = 2U,
    /* YUV444 (1-planar in the YUVYUV sequence) */
    kFMT_yuv444_linear2nv12      = 3U, /* not supported */
    kFMT_yuv444_linear2tilednv12 = 4U, /* not supported */
    kFMT_yuv444_linear2yuyv      = 5U, /* not supported */
    kFMT_yuv444_linear2tiledyuyv = 6U,
    kFMT_yuv444_linear2rgb888    = 7U,
    kFMT_yuv444_linear2yv24      = 8U,
} fmt_conv_t;

/*! @brief definition for parameters of the linear2tile API */
typedef struct tag_EzhvFmtConv
{
    uint32_t width;
    uint32_t height;

    uint32_t srcBuff0;
    uint32_t srcBuff1;
    uint32_t srcBuff2;

    uint32_t dstBuff0;
    uint32_t dstBuff1;
    uint32_t dstBuff2;

    uint32_t jpgPatch;
    uint32_t fb_bpp;
    uint32_t fmt_conv;
    uint32_t start;
} ezhv_para_linear2tile_t;
#endif

#if (EZHV_DVP_EN)
/*! @brief definition for paras of display video port API */
typedef struct _ezhv_para_dvp_t
{
    uint32_t len; /*!< buffer length, bytes */
    void   *pBuf; /*!< buffer pointer */
    //uint32_t stride;
} ezhv_para_dvp_t;
#endif

#if (EZHV_MIP_EN)
/*! @brief definition for timing parameters of memory-in-pixel LCD panel */
typedef struct _mip_timing
{
    uint32_t xrstSetupTime;        /*!< unit: 1us*/
    uint32_t vstSetupTime;         /*!< unit: 1us */
    uint32_t vstHoldTime;          /*!< unit: 1us */
    uint32_t vckHighLevelTime;     /*!< unit: 1us */
    uint32_t vckFastHighLevelTime; /*!< high width of fast vck, uint: 1ns */
    uint32_t tdHst;                /*!< HST delay time, unit is ns */
    uint32_t tsHST;                /*!< HST setup time, unit is ns */
    uint32_t hckHighLevelTime;     /*!< 1/2 cycle of hck, in state3 */
    uint32_t hckEdgeNum;           /*!< the number of hck edge, in state3 */
    uint32_t extVckEdgeNum1;       /*!< number1 of extra vck edge, stat5 */
    uint32_t extVckEdgeNum2;       /*!< number2 of extra vck edge, stat5 */
} mip_timing_t;

/*! @brief definition for the parameters of the MiP API */
typedef struct _mip_display_config
{
    uint32_t *pdmaStart;  /*!< the start address of data buffer */
    uint32_t startline;   /*!< the 1st line which need to be updated*/
    uint32_t endline;     /*!< the last line which need to be updated */
    uint32_t panelWidth;  /*!< panel width, equal to resolution */
    uint32_t panelHeight; /*!< panel height, equal to resolution*/
    uint32_t stride;   /*!< the number of bytes each line, 4bytes-align */
    uint32_t freqHz;      /*!< root clock frequency of EZH-V unit Hz*/

    mip_timing_t engine; /* the struct mip_timing_t */
} ezhv_para_mip_t;
#endif

/* to add more data struct definition here */

#endif
