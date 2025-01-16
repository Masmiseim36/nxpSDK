/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "ezhv_api.h"

#if (EZHV_LINEAR_TO_TILE_EN)
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void EZHV_LinearS420toLinearRGB565(uint16_t width, uint16_t height, 
                                    uint32_t yAddr, uint32_t uvAddr,
                                    uint32_t rgbAddr);
static void EZHV_LinearNv12ToTiledNv12(uint32_t yBase, uint32_t uvBase,
                                uint32_t yOutBase, uint32_t uvOutBase,
                                size_t width/* pixel number = 8*m */,
                                size_t height/* pixel number = 8*n */);
static void EZHV_LinearI422ToTiledYUY2(uint32_t base, uint32_t outBase,
                                size_t width/* pixel number = 4*m */,
                                size_t height/* pixel number = 4*n */,
                                size_t depth);
static void EZHV_LinearYUV444ToTiledYUY2(uint32_t base, uint32_t outBase, 
                                size_t width/* pixel number = 4*m */,
                                size_t height/* pixel number = 4*n */,
                                size_t depth);
static void EZHV_LinearYUV444ToRGB888(uint32_t base, uint32_t outBase, 
                                size_t width/* pixel number = 4*m */,
                                size_t height/* pixel number = 4*n */
                                );
static void EZHV_LinearYUV444ToYV24(uint16_t width,
                             uint16_t height,
                             uint32_t srcAddr, 
                             uint32_t dstAddrY,
                             uint32_t dstAddrU,
                             uint32_t dstAddrV);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
void EZHV_Linear2Tile(uint32_t paraAddr)
{
    assert(0 != paraAddr);

    ezhv_para_linear2tile_t *pstJpgInfo = (ezhv_para_linear2tile_t*)paraAddr;
    switch (pstJpgInfo->fmt_conv)
    {
        case kFMT_nv12_linear2rgb565:
            EZHV_LinearS420toLinearRGB565(pstJpgInfo->width,
                pstJpgInfo->height,
                pstJpgInfo->srcBuff0,
                pstJpgInfo->srcBuff1,
                pstJpgInfo->dstBuff0);
            break;
        case kFMT_nv12_linear2tiled:
            EZHV_LinearNv12ToTiledNv12(pstJpgInfo->srcBuff0,
                pstJpgInfo->srcBuff1,
                pstJpgInfo->dstBuff0,
                pstJpgInfo->dstBuff1,
                pstJpgInfo->width,
                pstJpgInfo->height);
            break;
        case kFMT_yuyv_linear2tiled:
            EZHV_LinearI422ToTiledYUY2(pstJpgInfo->srcBuff0,
                pstJpgInfo->dstBuff0,
                pstJpgInfo->width,
                pstJpgInfo->height,
                pstJpgInfo->fb_bpp);
            break;
        case kFMT_yuv444_linear2tiledyuyv:
            EZHV_LinearYUV444ToTiledYUY2(pstJpgInfo->srcBuff0,
                pstJpgInfo->dstBuff0, 
                pstJpgInfo->width,
                pstJpgInfo->height,
                pstJpgInfo->jpgPatch);
            break;
        case kFMT_yuv444_linear2rgb888:
            EZHV_LinearYUV444ToRGB888(pstJpgInfo->srcBuff0,
                pstJpgInfo->dstBuff0, 
                pstJpgInfo->width,
                pstJpgInfo->height);
            break;
        case kFMT_yuv444_linear2yv24:
            EZHV_LinearYUV444ToYV24(pstJpgInfo->width,
                pstJpgInfo->height,
                pstJpgInfo->srcBuff0,
                pstJpgInfo->dstBuff0,
                pstJpgInfo->dstBuff1,
                pstJpgInfo->dstBuff2);
            break;
        default:
            break;
    }

    XZMSG_IndIntChan(1);
}

static uint8_t BYTECLIP(int val)
{
    if (val < 0)
    {
        return 0U;
    }
    else if (val > 255)
    {
        return 255U;
    }
    else
    {
        return (uint8_t)val;
    }
}

static void EZHV_LinearS420toLinearRGB565(uint16_t width, uint16_t height, 
                                    uint32_t yAddr, uint32_t uvAddr,
                                    uint32_t rgbAddr)
{
    uint16_t *rgb = (void *)rgbAddr;
    int16_t r, g, b;
    uint8_t R, G, B, y, u, v;

    uint16_t i, j;

    for (i = 0U; i < height; i++)
    {
        for (j = 0U; j < width; j++)
        {
            y = *(((uint8_t *)yAddr) + width * i + j);
            u = *(((uint8_t *)uvAddr) + width * (i / 2) + j - (j % 2));
            v = *(((uint8_t *)uvAddr) + width * (i / 2) + j + 1 - (j % 2));
            r = y + 1402 * (v - 128) / 1000;
            g = y - (344 * (u - 128) + 714 * (v - 128)) / 1000;
            b = y + 1772 * (u - 128) / 1000;
            R = BYTECLIP(r);
            G = BYTECLIP(g);
            B = BYTECLIP(b);
            *rgb++ = (uint16_t)((((uint16_t)R & 0xF8) << 8U) | (((uint16_t)G & 0xFC) << 3U) \
                   | (((uint16_t)B & 0xF8) >> 3U));
        }
    }
}


/* this is used to convert linear i444 into tiled NV12 */
static void EZHV_LinearYUV444ToRGB888(uint32_t base, uint32_t outBase, 
                                size_t width/* pixel number = 4*m */,
                                size_t height/* pixel number = 4*n */
                                )
{
    uint8_t *outP  =  (uint8_t *)outBase;
    uint8_t *pixel =  (uint8_t *)base;
    int16_t r, g, b;
    uint8_t y, u, v;

    for (uint32_t i = 0U; i < height; i++)
    {
        for (uint32_t j = 0U; j < width; j++)
        {
            y = pixel[0];
            u = pixel[1];
            v = pixel[2];
            r = y + 1402 * (v - 128) / 1000;
            g = y - (344 * (u - 128) + 714 * (v - 128)) / 1000;
            b = y + 1772 * (u - 128) / 1000;
            outP[2] = BYTECLIP(r);
            outP[1] = BYTECLIP(g);
            outP[0] = BYTECLIP(b);
            outP += 3;
            pixel += 3;
        }
    }
}

/* this is used to convert linear i444 into tiled NV12 */
static void EZHV_LinearYUV444ToTiledYUY2(uint32_t base, uint32_t outBase, 
                                size_t width/* pixel number = 4*m */,
                                size_t height/* pixel number = 4*n */,
                                size_t depth)
{
    uint32_t tile_width = 4;
    uint32_t tile_height = 4;
    uint32_t total_tile_w = (width + tile_width - 1) / tile_width;
    uint32_t total_tile_h = (height + tile_height - 1) / tile_height;
    uint32_t lineStride = width *depth;

    uint8_t *outP  =  (uint8_t *)outBase;
    uint8_t *pixel =  (uint8_t *)base;

    for (uint32_t ty = 0; ty < total_tile_h; ty++)
    {
        uint32_t tile_start_ty = ty * tile_height;
        for (uint32_t tx = 0; tx < total_tile_w; tx++)
        {
            uint32_t tile_start_tx = tx * tile_width * depth;
            for(uint32_t y = 0; y < tile_height; y++)
            {
                for (uint32_t x = 0; x < tile_width*depth/6; x++)
                {
                    uint32_t idx = (tile_start_ty+y) * lineStride 
                                 + tile_start_tx + x * 6;
                    outP[0] = pixel[idx];
                    outP[1] = pixel[idx + 1];
                    outP[2] = pixel[idx + 3];
                    outP[3] = pixel[idx + 2];
                    outP += 4;
                }
            }
        }
    }
}

static void EZHV_LinearYUV444ToYV24(uint16_t width,
                             uint16_t height,
                             uint32_t srcAddr, 
                             uint32_t dstAddrY,
                             uint32_t dstAddrU,
                             uint32_t dstAddrV)
{
    uint8_t *py, *pyo, *puo, *pvo;
    uint16_t i, j;

    for (i = 0U; i < height; i++)
    {
        py  = (uint8_t *)srcAddr  + i * width * 3;
        pyo = (uint8_t *)dstAddrY + i * width;
        puo = (uint8_t *)dstAddrU + i * width;
        pvo = (uint8_t *)dstAddrV + i * width;
        for (j = 0U; j < width; j++)
        {
           *pyo++ = *py++;
           *puo++ = *py++;
           *pvo++ = *py++;
        }
    }
}

/* this is to generate tiled YUY2 format 
 * Y0U0Y1V0, Y2U2Y3V2, Y4U4Y5V4,...
*/
static void EZHV_LinearI422ToTiledYUY2(uint32_t base, uint32_t outBase,
                                size_t width/* pixel number = 4*m */,
                                size_t height/* pixel number = 4*n */,
                                size_t depth)
{
    uint32_t tile_width = 4;
    uint32_t tile_height = 4;
    uint32_t total_tile_w = (width + tile_width - 1) / tile_width;
    uint32_t total_tile_h = (height + tile_height - 1) / tile_height;
    uint32_t stride = width *depth;
    uint32_t index = 0;
    uint32_t *in = (uint32_t *)base;
    uint32_t *out = (uint32_t *)outBase;

    for (uint32_t ty = 0; ty < total_tile_h; ty++)
    {
        uint32_t start_ty = ty * tile_height;
        for (uint32_t tx = 0; tx < total_tile_w; tx++)
        {
            uint32_t start_tx = tx * tile_width * depth;
            for(uint32_t y = 0; y < tile_height; y++)
            {
                for (uint32_t x = 0; x < tile_width*depth/4; x++)
                {
                    uint32_t idx = ((start_ty+y)*stride+start_tx+x*4)>>2;
                    out[index++] = in[idx];
                }
            }
        }
    }
}

/* this is used to convert linear semi-planer420 into tiled NV12 */
static void EZHV_LinearNv12ToTiledNv12(uint32_t yBase, uint32_t uvBase,
                                uint32_t yOutBase, uint32_t uvOutBase,
                                size_t width/* pixel number = 8*m */,
                                size_t height/* pixel number = 8*n */)
{
    /* when tile size is different, just modify these parameters below */
    uint32_t tile_width = 8;
    uint32_t tile_height = 8;
    uint32_t total_tile_w = (width + tile_width - 1) / tile_width;
    uint32_t total_tile_h = (height + tile_height - 1) / tile_height;
    uint32_t count=0;
    uint8_t *planar = (uint8_t*)yBase;
    uint32_t *pOut = (uint32_t *)yOutBase;

    /* Y-Planer */
    count = 0;
    for (uint32_t ty = 0; ty < total_tile_h; ty++)
    {
        /* which line, 1st pixel position of a tile in whole image*/
        uint32_t tile_start_ty = ty * tile_height;
        for (uint32_t tx = 0; tx < total_tile_w; tx++)
        {
            /* which column 1st pixel position of a tile in whole image */
            uint32_t tile_start_tx = tx * tile_width;
            for (uint32_t x = 0; x < 2; x++)
            {
                for (uint32_t y = 0; y < tile_height/2; y++)
                {
                  pOut[count++] = *(uint32_t*)&planar[(tile_start_ty+y) * width
                                + tile_start_tx + x*4];
                }
            }
            for (uint32_t x = 0; x < 2; x++)
            {
                for (uint32_t y = tile_height/2; y < tile_height; y++)
                {
                  pOut[count++] = *(uint32_t*)&planar[(tile_start_ty+y) * width
                                + tile_start_tx + x*4];
                }
            }
        }
    }

    /* NV-planar */
    count = 0;
    planar = (uint8_t*)uvBase;
    pOut = (uint32_t *)uvOutBase;
    for (uint32_t ty = 0; ty < total_tile_h; ty++)
    {
        uint32_t tile_start_y = ty * tile_height/2;
        for (uint32_t tx = 0; tx < total_tile_w; tx++)
        {
            uint32_t tile_start_x = tx * tile_width;
            for (uint32_t x = 0; x < 2; x++)
            {
                for (uint32_t y = 0; y < 2; y++)
                {
                  pOut[count++] = *(uint32_t*)&planar[(tile_start_y+y) * width
                                + tile_start_x + x*4];
                }
            }
            for (uint32_t x = 0; x < 2; x++)
            {
                for (uint32_t y = 2; y < 4; y++)
                {
                  pOut[count++] = *(uint32_t*)&planar[(tile_start_y+y) * width
                                + tile_start_x + x*4];
                }
            }
        }
    }
}
#endif
