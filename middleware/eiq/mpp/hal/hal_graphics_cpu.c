/*
 * Copyright 2023-2024 NXP.
 * All rights reserved.
 *
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include "mpp_config.h"
#include "hal_graphics_dev.h"
#include "hal_debug.h"
#if (defined HAL_ENABLE_2D_IMGPROC) && (HAL_ENABLE_GFX_DEV_Cpu == 1)
#include "fsl_common.h"
#include "hal_utils.h"

typedef struct
{
    /* get c coefficient from yuv format */
    int (*c_from_yuv)(uint8_t *pix, int pixel_id);
    /* get d coefficient from yuv format */
    int (*d_from_yuv)(uint8_t *pix);
    /* get e coefficient from yuv format */
    int (*e_from_yuv)(uint8_t *pix);
} hal_gfx_cpu_yuv_ops;

typedef struct
{
    unsigned int src_w;
    unsigned int src_h;
    unsigned int dst_w;
    unsigned int dst_h;
} cpu_blit_dims_t;

/*
 * Following operations are performed to write into dest buffer
 * parsing x values first then y values
 *
 * get_src_pos:    get index from source buffer (includes rotate)
 * get_color_byte: get (byte) value from source buffer
 * color_conv:     convert color pixel
 * get_dest_pos:   get index from dest buffer (specific to 422 format)
 * write_pixel:    write converted value into destination buffer
 */
typedef uint32_t (*get_src_pos)(int x, int y, int pitch, int bpp, int offset,
                               int width, int height);

typedef uint8_t (*get_color_byte)(int x, int y, uint8_t *buf, int pitch, int bpp, int offset, get_src_pos f_get_src_pos, int width, int height);
typedef void (*color_conv)(uint8_t pix_id, uint8_t *pixel, uint8_t *r, uint8_t *g, uint8_t *b);
typedef void (*write_pixel)(void *pixel, uint8_t red, uint8_t green, uint8_t blue);
typedef uint32_t (*get_dest_pos)(int x, int y, int pitch, int bpp, int pix_id);

/* structure used for color conversion from
 * a pixel format (source) to another pixel format (destination)
 */
typedef struct
{
    /* color position in source pixel */
    uint8_t pos_src;
    /* color mask in source pixel */
    uint8_t mask_src;
    /* left shift required to transform source color bits into destination color bits */
    uint8_t shift_dst;
} hal_gfx_cpu_color_conv;

static int HAL_GfxDev_Cpu_Rotate(gfx_surface_t *pSrc, gfx_surface_t *pDst,
                                 mpp_rotate_degree_t degree, cpu_blit_dims_t *pBlit_dims)  __attribute__((unused));
static int HAL_GfxDev_Cpu_Scale(gfx_surface_t *pSrc, gfx_surface_t *pDst,
                                cpu_blit_dims_t *pBlit_dims)  __attribute__((unused));
static int HAL_GfxDev_Cpu_ColorConvert(gfx_surface_t *pSrc, gfx_surface_t *pDst,
                                       cpu_blit_dims_t *pBlit_dims)  __attribute__((unused));
static int HAL_GfxDev_Cpu_NoneConvert(gfx_surface_t *pSrc, gfx_surface_t *pDst,
                                      cpu_blit_dims_t *pBlit_dims)  __attribute__((unused));
static int HAL_GfxDev_Scale_RGB565to888(const gfx_dev_t *dev, const gfx_surface_t *pSrc,
        const gfx_surface_t *pDst, const gfx_rotate_config_t *pRotate, mpp_flip_mode_t flip);
static int HAL_GfxDev_any_OP(const gfx_dev_t *dev, const gfx_surface_t *pSrc,
        const gfx_surface_t *pDst, const gfx_rotate_config_t *pRotate, mpp_flip_mode_t flip);

int HAL_GfxDev_Cpu_Getbufdesc(const gfx_dev_t *dev, hw_buf_desc_t *in_buf, hw_buf_desc_t *out_buf, mpp_memory_policy_t *policy)
{
    int error = 0;
    do {
        if ((in_buf == NULL) || (out_buf == NULL) || (policy == NULL))
        {
            HAL_LOGE("NULL pointer to buffer descriptor\n");
            error =  -1;
            break;
        }
        /* set memory policy */
        *policy = HAL_MEM_ALLOC_NONE;
        /* set hw requirement */
        in_buf->alignment = 0;
        in_buf->nb_lines = 0;
        in_buf->cacheable = true;
        in_buf->stride = 0;
        out_buf->alignment = 0;
        out_buf->cacheable = true;
        out_buf->stride = 0;
    } while(false);

    return error;
}

/*******************************************************************************
 * Implementation for function pointer get_src_pos
 ******************************************************************************/
/*
 * Mapping functions used for Image Rotation
 *
 * Map the component byte located in [x,y] from the rotated buffer (dest)
 * with the source buffer of params: pitch, bpp, width*height
 *
 * The function returns the map location expressed
 * in offset (bytes) of the source buffer.
 */

/* Map for Image Rotation 0° */
static inline uint32_t rotate0_map(int x, int y, int pitch, int bpp, int offset,
                                    int width, int height)
{
    return (y*pitch + (x*bpp) + offset);
}

/* Map for Image Rotation 90° */
static inline uint32_t rotate90_map(int x, int y, int pitch, int bpp, int offset,
                                    int width, int height)
{
    return ((height-x-1)*pitch + (y*bpp) + offset);
}

/* Map for Image Rotation 180° */
static inline uint32_t rotate180_map(int x, int y, int pitch, int bpp, int offset,
                                     int width, int height)
{
    return ((height-y-1)*pitch + (width-x-1)*bpp + offset);
}

/* Map for Image Rotation 270° */
static inline uint32_t rotate270_map(int x, int y, int pitch, int bpp, int offset,
                                     int width, int height)
{
    return ((x*pitch) + (width-y-1)*bpp + offset);
}

/* Map for Image horizontal flip */
static inline uint32_t flip_h_map(int x, int y, int pitch, int bpp, int offset,
                                     int width, int height)
{
    return (y*pitch + (width-x-1)*bpp + offset);
}

/* Map for Image vertical flip */
static inline uint32_t flip_v_map(int x, int y, int pitch, int bpp, int offset,
                                     int width, int height)
{
    return ((height-y-1)*pitch + (x*bpp) + offset);
}

/* Map for Image h+v flip */
static inline uint32_t flip_both_map(int x, int y, int pitch, int bpp, int offset,
                                     int width, int height)
{
    return ((height-y-1)*pitch + (width-x-1)*bpp + offset);
}

/* Map for Image horizontal flip 90° => equivalent to vertical flip 270° */
static inline uint32_t flip_h_90_map(int x, int y, int pitch, int bpp, int offset,
                                     int width, int height)
{
    return ((height-x-1)*pitch + (width-y-1)*bpp + offset);
}

/* Map for Image vertical flip 90° => equivalent to horizontal flip 270° */
static inline uint32_t flip_v_90_map(int x, int y, int pitch, int bpp, int offset,
                                     int width, int height)
{
    return (x*pitch + (y*bpp) + offset);
}

/* Map for Image h+v flip 90° => equivalent to rotate 270° */
static inline uint32_t flip_both_90_map(int x, int y, int pitch, int bpp, int offset,
                                        int width, int height)
{
    return ((x*pitch) + (width-y-1)*bpp + offset);
}

/* Map for Image horizontal flip 180° => equivalent to vertical flip 0° */
static inline uint32_t flip_h_180_map(int x, int y, int pitch, int bpp, int offset,
                                      int width, int height)
{
    return ((height-y-1)*pitch + (x*bpp) + offset);
}

/* Map for Image vertical flip 180° => equivalent to horizontal flip 0° */
static inline uint32_t flip_v_180_map(int x, int y, int pitch, int bpp, int offset,
                                      int width, int height)
{
    return (y*pitch + (width-x-1)*bpp + offset);
}

/* Map for Image h+v flip 180° => equivalent to rotate 0° */
static inline uint32_t flip_both_180_map(int x, int y, int pitch, int bpp, int offset,
                                         int width, int height)
{
    return (y*pitch + (x*bpp) + offset);
}

/* Map for Image horizontal flip 270° => equivalent to vertical flip 90° */
static inline uint32_t flip_h_270_map(int x, int y, int pitch, int bpp, int offset,
                                      int width, int height)
{
    return (x*pitch + (y*bpp) + offset);
}

/* Map for Image vertical flip 270° => equivalent to horizontal flip 90° */
static inline uint32_t flip_v_270_map(int x, int y, int pitch, int bpp, int offset,
                                      int width, int height)
{
    return ((height-x-1)*pitch + (width-y-1)*bpp + offset);
}

/* Map for Image h+v flip 270° => equivalent to rotate 90° */
static inline uint32_t flip_both_270_map(int x, int y, int pitch, int bpp, int offset,
                                         int width, int height)
{
    return ((height-x-1)*pitch + (y*bpp) + offset);
}

/*******************************************************************************
 * Implementation for function pointer get_dest_pos
 ******************************************************************************/
static inline uint32_t get_dest_pos_default(int x, int y, int pitch, int bpp, int pix_id)
{
    return ((y * pitch) + (x * bpp));
}

/*
 * For YUV422, the formula for f_get_dst_pos() is determined:
 * - based on get_dest_pos_default and
 * - adapted depending on orientation (flip/rotation)
 * The adaptation takes, from f_get_src_pos() implementation, the dimension (x or y) used for the horizontal offset:
 * For f_get_dst_pos(), this dimension (x or y) gets to be:
 * - multiplied by 2
 * - incremented by pix_id
 *   - if the horizontal offset from f_get_src_pos() is incremented from start of line => pix_id offset is respected
 *   - if the horizontal offset from f_get_src_pos() is decremented from end of line => pix_id count is reversed (if 0 => get 1, if 1 => get 0)
 *
 * pix_id: index (0 or 1) of the YUV422 2Bytes pixel sharing U and V values
 */
static inline uint32_t get_dest_pos422_x_non_reversed(int x, int y, int pitch, int bpp, int pix_id)
{
    return ((y * pitch) + ((x * 2) + pix_id) * bpp);
}

static inline uint32_t get_dest_pos422_y_non_reversed(int x, int y, int pitch, int bpp, int pix_id)
{
    return ((((y * 2) + pix_id) * pitch) + (x * bpp));
}

static inline uint32_t get_dest_pos422_x_reversed(int x, int y, int pitch, int bpp, int pix_id)
{
    return ((y * pitch) + ((x * 2) + (pix_id^1)) * bpp);
}

static inline uint32_t get_dest_pos422_y_reversed(int x, int y, int pitch, int bpp, int pix_id)
{
    return ((((y * 2) + (pix_id^1)) * pitch) + (x * bpp));
}

/* RGB565 format */
#define RGB565_RMASK 0x1F
#define RGB565_GMASK 0x3F
#define RGB565_BMASK 0x1F
#define RGB565_RSHIFT 11
#define RGB565_GSHIFT 5
#define RGB565_BSHIFT 0

/*******************************************************************************
 * Implementation for function pointer write_pixel
 ******************************************************************************/
static inline void write_rgb888(void *pixel, uint8_t red, uint8_t green, uint8_t blue)
{
    uint8_t *dstpix = (uint8_t *)pixel;

    *(dstpix+0) = red;
    *(dstpix+1) = green;
    *(dstpix+2) = blue;
}

static inline void write_bgr888(void *pixel, uint8_t red, uint8_t green, uint8_t blue)
{
    uint8_t *dstpix = (uint8_t *)pixel;

    *(dstpix+0) = blue;
    *(dstpix+1) = green;
    *(dstpix+2) = red;
}

static inline void write_rgb565(void *pixel, uint8_t red, uint8_t green, uint8_t blue)
{
    uint16_t *rgb565 = (uint16_t *)pixel;
    *rgb565  = (((red >> 3) & RGB565_RMASK) << RGB565_RSHIFT |
                ((green >> 2) & RGB565_GMASK) << RGB565_GSHIFT |
                ((blue >> 3) & RGB565_BMASK) << RGB565_BSHIFT);
}
static int HAL_GfxDev_Cpu_RGB888ToRGB(gfx_surface_t *pSrc, gfx_surface_t *pDst,
                                      cpu_blit_dims_t *pBlit_dims)
{
    uint8_t *srcbuf = pSrc->buf;
    uint8_t *dstbuf = pDst->buf;
    int src_pitch = pSrc->pitch;
    int srcBPP = get_bitpp(pSrc->format)/8;
    int dstBPP = get_bitpp(pDst->format)/8;
    int dst_pitch = pDst->pitch;
    int x, y;

    HAL_LOGD("Output window: width: [%d], height: [%d].\n",
             pBlit_dims->dst_w, pBlit_dims->dst_h);

    switch (pDst->format) {
    case MPP_PIXEL_RGB565:
        break;
    default:
        HAL_LOGE("Unsupported conversion from RGB888 format to RGB format [%d]\n",
                 pDst->format);
        return -1;
    }

    for(y = 0; y < pBlit_dims->dst_h; y++) {
        for(x = 0; x < pBlit_dims->dst_w; x++) {
            int i = (y * src_pitch) + (x * srcBPP);
            write_rgb565(&dstbuf[(y * dst_pitch) + (x * dstBPP)],
                         srcbuf[i], srcbuf[i+1], srcbuf[i+2]);
        }
    }

    return 0;
}

static int HAL_GfxDev_Cpu_RGB565ToRGB(gfx_surface_t *pSrc, gfx_surface_t *pDst,
                                      cpu_blit_dims_t *pBlit_dims)
{
    uint16_t *srcbuf = (uint16_t *)pSrc->buf;
    uint8_t *dstbuf = pDst->buf;
    int src_pitch = pSrc->pitch;
    int srcBPP = get_bitpp(pSrc->format)/8;
    int dstBPP = get_bitpp(pDst->format)/8;
    int dst_pitch = pDst->pitch;
    int x, y;
    void (*write_rgb)(void *pixel, uint8_t red, uint8_t green, uint8_t blue) = NULL;
    uint8_t r8, g8, b8;

    HAL_LOGD("Output window: width: [%d], height: [%d].\n",
             pBlit_dims->dst_w, pBlit_dims->dst_h);

    switch (pDst->format) {
    case MPP_PIXEL_RGB:
        write_rgb = &write_rgb888;
        break;
    case MPP_PIXEL_BGR:
        write_rgb = &write_bgr888;
        break;
    default:
        HAL_LOGE("Unsupported conversion from RGB565 format to RGB format [%d]\n",
                 pDst->format);
        return -1;
    }

    for(y = 0; y < pBlit_dims->dst_h; y++) {
        for(x = 0; x < pBlit_dims->dst_w; x++) {
            int i = (y * src_pitch/srcBPP) + x;
            r8 = ((srcbuf[i] >> RGB565_RSHIFT) & RGB565_RMASK) << 3;
            g8 = ((srcbuf[i] >> RGB565_GSHIFT) & RGB565_GMASK) << 2;
            b8 = (srcbuf[i] & RGB565_BMASK) << 3;
            write_rgb(&dstbuf[(y * dst_pitch) + (x * dstBPP)], r8, g8, b8);
        }
    }

    return 0;
}

/*
 * Clamp a value to the range of 0 to 255
 */
static inline int clamp_to_8bits(int x)
{
    if (x > 255) return 255;
    if (x < 0)   return 0;
    return x;
}

#define YUV2R(c, d, e) clamp_to_8bits((298 * (c) + 409 * (e) + 128) >> 8)
#define YUV2G(c, d, e) clamp_to_8bits((298 * (c) - 100 * (d) - 208 * (e) + 128) >> 8)
#define YUV2B(c, d, e) clamp_to_8bits((298 * (c) + 516 * (d) + 128) >> 8)

/* Coefficients c, d and e for UYVY422 format
 *
 * c = Y - 16
 * d = U - 128
 * e = V - 128
 */
static inline int c_from_uyvy422(uint8_t *pix, int pixel_id)
{
    return (*(pix+1+2*pixel_id) - 16);
}
static inline int d_from_uyvy422(uint8_t *pix)
{
    return (*(pix) - 128);
}
static inline int e_from_uyvy422(uint8_t *pix)
{
    return (*(pix+2) - 128);
}

const static hal_gfx_cpu_yuv_ops s_GfxDevCpuOps_UYVY422 = {
    .c_from_yuv = c_from_uyvy422,
    .d_from_yuv = d_from_uyvy422,
    .e_from_yuv = e_from_uyvy422,
};

/* Coefficients c, d and e for VYUY422 format
 *
 * c = Y - 16
 * d = U - 128
 * e = V - 128
 */
static inline int c_from_vyuy422(uint8_t *pix, int pixel_id)
{
    return (*(pix+1+2*pixel_id) - 16);
}
static inline int d_from_vyuy422(uint8_t *pix)
{
    return (*(pix+2) - 128);
}
static inline int e_from_vyuy422(uint8_t *pix)
{
    return (*(pix) - 128);
}

const static hal_gfx_cpu_yuv_ops s_GfxDevCpuOps_VYUY422 = {
    .c_from_yuv = c_from_vyuy422,
    .d_from_yuv = d_from_vyuy422,
    .e_from_yuv = e_from_vyuy422,
};

/* Coefficients c, d and e for VUYX444 format
 *
 * c = Y - 16
 * d = U - 128
 * e = V - 128
 */
static inline int c_from_vuyx444(uint8_t *pix, int pixel_id)
{
    return (*(pix+2) - 16);
}
static inline int d_from_vuyx444(uint8_t *pix)
{
    return (*(pix+1) - 128);
}
static inline int e_from_vuyx444(uint8_t *pix)
{
    return (*(pix) - 128);
}

const static hal_gfx_cpu_yuv_ops s_GfxDevCpuOps_VUYX444 = {
    .c_from_yuv = c_from_vuyx444,
    .d_from_yuv = d_from_vuyx444,
    .e_from_yuv = e_from_vuyx444,
};

/* nb of RGB pixels in 4 bytes YUV */
#define RGB_PIXELS_IN_4B_YUV444  1
#define RGB_PIXELS_IN_4B_YUV422  2
#define RGB_PIXELS_MAX_IN_4B_YUV 2

/*
 * This function converts from YUV format to RGB format.
 *
 * The RGB formats supported are:
 * - RGB565
 * - other RGB formats not supported yet
 *
 * The YUV formats supported are:
 * - VUYX444
 * - UYVY422
 * - VYUY422
 */
static int HAL_GfxDev_Cpu_YUVToRGB(gfx_surface_t *pSrc, gfx_surface_t *pDst,
                                   cpu_blit_dims_t *pBlit_dims)
{
    uint8_t *srcbuf = pSrc->buf;
    uint8_t *dstbuf = pDst->buf;
    int src_pitch = pSrc->pitch;
    int srcBPP = get_bitpp(pSrc->format)/8;
    int dstBPP = get_bitpp(pDst->format)/8;
    int dst_pitch = pDst->pitch;
    int x, y;
    uint32_t *srcpix;
    /* coefficient of y, u, v */
    int c[RGB_PIXELS_MAX_IN_4B_YUV], d, e;
    /* nb of RGB pixels in 4 bytes YUV */
    int pixels;
    const hal_gfx_cpu_yuv_ops *yuv_ops = NULL;
    void (*write_rgb)(void *pixel, uint8_t red, uint8_t green, uint8_t blue) = NULL;

    HAL_LOGD("From format: [%d] To format: [%d].\n", pSrc->format, pDst->format);

    switch (pSrc->format) {
    case MPP_PIXEL_YUV1P444:
        pixels = RGB_PIXELS_IN_4B_YUV444;
        yuv_ops = &s_GfxDevCpuOps_VUYX444;
        break;
    case MPP_PIXEL_UYVY1P422:
        pixels = RGB_PIXELS_IN_4B_YUV422;
        yuv_ops = &s_GfxDevCpuOps_UYVY422;
        break;
    case MPP_PIXEL_VYUY1P422:
        pixels = RGB_PIXELS_IN_4B_YUV422;
        yuv_ops = &s_GfxDevCpuOps_VYUY422;
        break;
    default:
        HAL_LOGE("Unsupported YUV format [%d]\n", pSrc->format);
        return -1;
    }

    switch (pDst->format) {
    case MPP_PIXEL_RGB565:
        write_rgb = &write_rgb565;
        break;
    case MPP_PIXEL_RGB:
        write_rgb = &write_rgb888;
        break;
    case MPP_PIXEL_BGR:
        write_rgb = &write_bgr888;
        break;
    default:
        HAL_LOGE("Unsupported conversion from YUV format to RGB format [%d]\n",
                 pDst->format);
        return -1;
    }

    /* Parse pixels from destination buffer to map
     * with converted pixels in color from source buffer.
     */
    for(y = 0; y < pBlit_dims->dst_h; y++) {
        for(x = 0; x < pBlit_dims->dst_w/pixels; x++) {
            srcpix = (uint32_t *)(srcbuf + (y * src_pitch) + (x * srcBPP * pixels));
            d = yuv_ops->d_from_yuv((uint8_t *)srcpix);
            e = yuv_ops->e_from_yuv((uint8_t *)srcpix);
            for(int pix_id = 0; pix_id < pixels; pix_id++) {
                c[pix_id] = yuv_ops->c_from_yuv((uint8_t *)srcpix, pix_id);
                write_rgb(&dstbuf[(y*dst_pitch) + (x*pixels*dstBPP) + pix_id*dstBPP],
                          YUV2R(c[pix_id], d, e),
                          YUV2G(c[pix_id], d, e),
                          YUV2B(c[pix_id], d, e));
            }
        }
    }

    return 0;
}

/*
 * Image rotation using CPU backend:
 */
static int HAL_GfxDev_Cpu_Rotate(gfx_surface_t *pSrc, gfx_surface_t *pDst,
                                 mpp_rotate_degree_t degree, cpu_blit_dims_t *pBlit_dims)
{
    uint8_t *srcbuf = (uint8_t *)pSrc->buf;
    uint8_t *dstbuf = (uint8_t *)pDst->buf;
    int dst_pitch = pDst->pitch;
    int src_pitch = pSrc->pitch;
    /* srcBPP and dstBPP are the same */
    int srcBPP = get_bitpp(pSrc->format)/8;
    int dstBPP = get_bitpp(pDst->format)/8;
    int x, y;
    uint32_t (*map_in_src)(int x, int y, int pitch, int bpp, int offset,
                           int width, int height) = NULL;

    HAL_LOGD("Input window: width=[%d], height=[%d].\n", pBlit_dims->src_w, pBlit_dims->src_h);
    HAL_LOGD("Output window: width=[%d], height=[%d].\n", pBlit_dims->dst_w, pBlit_dims->dst_h);

    switch (degree) {
    case ROTATE_90:
        HAL_LOGD("ROTATE 90.\n");
        map_in_src = &rotate90_map;
	break;
    case ROTATE_180:
        HAL_LOGD("ROTATE 180.\n");
        map_in_src = &rotate180_map;
	break;
    case ROTATE_270:
        HAL_LOGD("ROTATE 270.\n");
        map_in_src = &rotate270_map;
	break;
    default:
        HAL_LOGE("Unsupported rotation value [%d]\n", degree);
        return -1;
    }

    /* Parse pixels from destination buffer to map
     * with pixels rotated from source buffer.
     */
    for (y = 0; y < pBlit_dims->dst_h; y++) {
        for (x = 0; x < pBlit_dims->dst_w; x++) {
            for (int comp_offset = 0; comp_offset < dstBPP; comp_offset++) {
                dstbuf[(y * dst_pitch) + (x * dstBPP) + comp_offset] =
                    srcbuf[map_in_src(x, y, src_pitch, srcBPP, comp_offset,
                                            pBlit_dims->src_w, pBlit_dims->src_h)];
            }
        }
    }

    return 0;
}

/* Scaler: number of sub-pixels increments in source image */
#define SUBPIXPOW 8
#define SUBPIXINC 256 /* 2 power 8 (SUBPIXPOW) */
/*
 * MAX_COMP_PER_PIXEL defines the maximum number of components in 1 pixel (in 2 pixels  for YUV422)
 * For YUV422: 4 components Y1, Y2, U, V
 * for other formats: 3 components (alpha channel can be ignored)
 * It is used as the number of loops to get a byte value for 1 pixel (2 pixels for YUV422)
 */
#define MAX_COMP_PER_PIXEL 4

/*******************************************************************************
 * Implementation for function pointer get_color_byte
 ******************************************************************************/
static inline uint8_t get_color_byte_from_rgb888(int x, int y, uint8_t *buf, int pitch, int bpp, int offset, get_src_pos f_get_src_pos, int width, int height)
{
    return buf[f_get_src_pos(x, y, pitch, bpp, offset, width, height)];
}

const hal_gfx_cpu_color_conv rgb565_to_rgb888[MAX_COMP_PER_PIXEL] =
{
    /* position in 16b pixel, mask in 16b pixel, left shift on 8b color value */
    {RGB565_RSHIFT, RGB565_RMASK, (8-5)}, /* red color */
    {RGB565_GSHIFT, RGB565_GMASK, (8-6)}, /* green color */
    {RGB565_BSHIFT, RGB565_BMASK, (8-5)}, /* blue color */
};

static inline uint8_t get_color_byte_from_rgb565(int x, int y, uint8_t *buf, int pitch, int bpp, int offset, get_src_pos f_get_src_pos, int width, int height)
{
    uint16_t *rgb565 = (uint16_t *)buf;
    return (((rgb565[f_get_src_pos(x, y, pitch/bpp, 1, offset, width, height)]
              >> rgb565_to_rgb888[offset].pos_src) &
             rgb565_to_rgb888[offset].mask_src) <<
            rgb565_to_rgb888[offset].shift_dst);
}

static inline uint8_t get_color_byte_from_rgb565_rot0(int x, int y, uint8_t *buf, int pitch, int offset, int width, int height)
{
    uint16_t *rgb565 = (uint16_t *)buf;
    return (((rgb565[rotate0_map(x, y, pitch/2, 1, offset, width, height)]
              >> rgb565_to_rgb888[offset].pos_src) &
             rgb565_to_rgb888[offset].mask_src) <<
            rgb565_to_rgb888[offset].shift_dst);
}

/*
 * Image scaling using CPU backend:
 *
 * Bi-linear scaler
 * Uses sub-pixel increments.
 * No float, no division.
 **/
static inline uint8_t scale_rgb888(int x, int y, uint8_t *buf, int pitch, int bpp, int offset, get_src_pos f_get_src_pos, int width, int height)
{
    uint8_t tl, tr, bl, br; /* 4 neighbors values in source */
    int src_y = y >> SUBPIXPOW;
    int disty = y % SUBPIXINC;

    /* get 4 neighbors at source pixel positions */
    /* divide by SUBPIXINC */
    int src_x = x >> SUBPIXPOW;
    tl = get_color_byte_from_rgb888(src_x, src_y, buf, pitch, bpp, offset, f_get_src_pos, width, height);
    tr = get_color_byte_from_rgb888(src_x+1, src_y, buf, pitch, bpp, offset, f_get_src_pos, width, height);
    bl = get_color_byte_from_rgb888(src_x, src_y+1, buf, pitch, bpp, offset, f_get_src_pos, width, height);
    br = get_color_byte_from_rgb888(src_x+1, src_y+1, buf, pitch, bpp, offset, f_get_src_pos, width, height);
    /* interpolate horizontally */
    int distx = x % SUBPIXINC;
    int top = ( (tl * (SUBPIXINC - distx)) + (tr * distx) ) >> SUBPIXPOW;
    int bot = ( (bl * (SUBPIXINC - distx)) + (br * distx) ) >> SUBPIXPOW;
    /* interpolate vertically */
    return ( (top * (SUBPIXINC - disty)) + (bot * disty) ) >> SUBPIXPOW;
}

static inline uint8_t scale_rgb565(int x, int y, uint8_t *buf, int pitch, int bpp, int offset, get_src_pos f_get_src_pos, int width, int height)
{
    uint8_t tl, tr, bl, br; /* 4 neighbors values in source */
    int src_y = y >> SUBPIXPOW;
    int disty = y % SUBPIXINC;

    /* get 4 neighbors at source pixel positions */
    /* divide by SUBPIXINC */
    int src_x = x >> SUBPIXPOW;
    tl = get_color_byte_from_rgb565(src_x, src_y, buf, pitch, bpp, offset, f_get_src_pos, width, height);
    tr = get_color_byte_from_rgb565(src_x+1, src_y, buf, pitch, bpp, offset, f_get_src_pos, width, height);
    bl = get_color_byte_from_rgb565(src_x, src_y+1, buf, pitch, bpp, offset, f_get_src_pos, width, height);
    br = get_color_byte_from_rgb565(src_x+1, src_y+1, buf, pitch, bpp, offset, f_get_src_pos, width, height);
    /* interpolate horizontally */
    int distx = x % SUBPIXINC;
    int top = ( (tl * (SUBPIXINC - distx)) + (tr * distx) ) >> SUBPIXPOW;
    int bot = ( (bl * (SUBPIXINC - distx)) + (br * distx) ) >> SUBPIXPOW;
    /* interpolate vertically */
    return ( (top * (SUBPIXINC - disty)) + (bot * disty) ) >> SUBPIXPOW;
}

static inline uint8_t scale_rgb565_rot0(int x, int y, uint8_t *buf, int pitch, int offset, int width, int height)
{
    uint8_t tl, tr, bl, br; /* 4 neighbors values in source */
    int src_y = y >> SUBPIXPOW;
    int disty = y % SUBPIXINC;

    /* get 4 neighbors at source pixel positions */
    /* divide by SUBPIXINC */
    int src_x = x >> SUBPIXPOW;
    tl = get_color_byte_from_rgb565_rot0(src_x, src_y, buf, pitch, offset, width, height);
    tr = get_color_byte_from_rgb565_rot0(src_x+1, src_y, buf, pitch, offset, width, height);
    bl = get_color_byte_from_rgb565_rot0(src_x, src_y+1, buf, pitch, offset, width, height);
    br = get_color_byte_from_rgb565_rot0(src_x+1, src_y+1, buf, pitch, offset, width, height);
    /* interpolate horizontally */
    int distx = x % SUBPIXINC;
    int top = ( (tl * (SUBPIXINC - distx)) + (tr * distx) ) >> SUBPIXPOW;
    int bot = ( (bl * (SUBPIXINC - distx)) + (br * distx) ) >> SUBPIXPOW;
    /* interpolate vertically */
    return ( (top * (SUBPIXINC - disty)) + (bot * disty) ) >> SUBPIXPOW;
}

/*******************************************************************************
 * Implementation for function pointer color_conv
 ******************************************************************************/
static inline void color_conv_none(uint8_t pix_id, uint8_t *pixel, uint8_t *r, uint8_t *g, uint8_t *b)
{
    *r = pixel[0];
    *g = pixel[1];
    *b = pixel[2];
}
static inline void color_VUYX444ToRGB(uint8_t pix_id, uint8_t *pixel, uint8_t *r, uint8_t *g, uint8_t *b)
{
    int c, d, e;

    d = s_GfxDevCpuOps_VUYX444.d_from_yuv(pixel);
    e = s_GfxDevCpuOps_VUYX444.e_from_yuv(pixel);
    c = s_GfxDevCpuOps_VUYX444.c_from_yuv(pixel, pix_id);
    *r = YUV2R(c, d, e);
    *g = YUV2G(c, d, e);
    *b = YUV2B(c, d, e);
}
static inline void color_UYVY422ToRGB(uint8_t pix_id, uint8_t *pixel, uint8_t *r, uint8_t *g, uint8_t *b)
{
    int c, d, e;

    d = s_GfxDevCpuOps_UYVY422.d_from_yuv(pixel);
    e = s_GfxDevCpuOps_UYVY422.e_from_yuv(pixel);
    c = s_GfxDevCpuOps_UYVY422.c_from_yuv(pixel, pix_id);
    *r = YUV2R(c, d, e);
    *g = YUV2G(c, d, e);
    *b = YUV2B(c, d, e);
}
static inline void color_VYUY422ToRGB(uint8_t pix_id, uint8_t *pixel, uint8_t *r, uint8_t *g, uint8_t *b)
{
    int c, d, e;

    d = s_GfxDevCpuOps_VYUY422.d_from_yuv(pixel);
    e = s_GfxDevCpuOps_VYUY422.e_from_yuv(pixel);
    c = s_GfxDevCpuOps_VYUY422.c_from_yuv(pixel, pix_id);
    *r = YUV2R(c, d, e);
    *g = YUV2G(c, d, e);
    *b = YUV2B(c, d, e);
}

/*
 * Image scaling using CPU backend:
 *
 * Bi-linear scaler
 * Uses sub-pixel increments.
 * No float, no division.
 **/
static int HAL_GfxDev_Cpu_Scale(gfx_surface_t *pSrc, gfx_surface_t *pDst,
                                cpu_blit_dims_t *pBlit_dims)
{
    uint8_t *srcbuf = pSrc->buf;
    int src_pitch = pSrc->pitch;
    int d_width = pBlit_dims->dst_w;
    int d_height = pBlit_dims->dst_h;
    int h_incr = (pBlit_dims->src_w - 1) * SUBPIXINC / (d_width - 1);
    int v_incr = (pBlit_dims->src_h - 1) * SUBPIXINC / (d_height - 1);
    int srcBPP = get_bitpp(pSrc->format)/8;
    int dstBPP = get_bitpp(pDst->format)/8;
    uint8_t *dstbuf = pDst->buf;
    int dst_pitch = pDst->pitch;
    int x, y, sub_x, sub_y;   /* position in destination */
    uint8_t tl, tr, bl, br; /* 4 neighbors values in source */
    uint8_t (*get_color)(int x, int y, uint8_t *buf, int pitch, int bpp, int offset,  get_src_pos f_get_src_pos, int width, int height) = NULL;
    void (*put_pixel)(void *pixel, uint8_t red, uint8_t green, uint8_t blue) = NULL;

    HAL_LOGD("Input window: width=[%d], height=[%d].\n", pBlit_dims->src_w, pBlit_dims->src_h);
    HAL_LOGD("Output window: width=[%d], height=[%d].\n", pBlit_dims->dst_w, pBlit_dims->dst_h);

    /* This implementation is only supported as single image operation.
     * Hence the color format is same for source and destination.
     */
    switch (pSrc->format) {
    case MPP_PIXEL_RGB:
    case MPP_PIXEL_YUV1P444:
        get_color = get_color_byte_from_rgb888;
        put_pixel = write_rgb888;
        break;
    case MPP_PIXEL_RGB565:
        get_color = get_color_byte_from_rgb565;
        put_pixel = write_rgb565;
        break;
    default:
        HAL_LOGE("Unsupported src format [%d]\n", pSrc->format);
        return -1;
    }

    for (y = 0, sub_y = 0; y < d_height; y++, sub_y += v_incr)
    {
        /* divide by SUBPIXINC */
        int src_y = sub_y >> SUBPIXPOW;
        int disty = sub_y % SUBPIXINC;
        for(x = 0, sub_x = 0 ; x < d_width; x++, sub_x += h_incr)
        {
            uint8_t val[MAX_COMP_PER_PIXEL];
            for (int comp_offset = 0; comp_offset < MAX_COMP_PER_PIXEL; comp_offset++)
            {
                /* get 4 neighbors at source pixel positions */
                /* divide by SUBPIXINC */
                int src_x = sub_x >> SUBPIXPOW;
                tl = get_color(src_x, src_y, srcbuf, src_pitch, srcBPP, comp_offset, &rotate0_map, pBlit_dims->src_w, pBlit_dims->src_h);
                tr = get_color(src_x+1, src_y, srcbuf, src_pitch, srcBPP, comp_offset, &rotate0_map, pBlit_dims->src_w, pBlit_dims->src_h);
                bl = get_color(src_x, src_y+1, srcbuf, src_pitch, srcBPP, comp_offset, &rotate0_map, pBlit_dims->src_w, pBlit_dims->src_h);
                br = get_color(src_x+1, src_y+1, srcbuf, src_pitch, srcBPP, comp_offset, &rotate0_map, pBlit_dims->src_w, pBlit_dims->src_h);
                /* interpolate horizontally */
                int distx = sub_x % SUBPIXINC;
                int top = ( (tl * (SUBPIXINC - distx)) + (tr * distx) ) >> SUBPIXPOW;
                int bot = ( (bl * (SUBPIXINC - distx)) + (br * distx) ) >> SUBPIXPOW;
                /* interpolate vertically */
                val[comp_offset] = ( (top * (SUBPIXINC - disty)) + (bot * disty) ) >> SUBPIXPOW;
            }
            put_pixel(&dstbuf[(y * dst_pitch) + (x * dstBPP)],
                      val[0], val[1], val[2]);
        }
    }

    return 0;
}

/*
 * Color conversion using CPU backend:
 */
static int HAL_GfxDev_Cpu_ColorConvert(gfx_surface_t *pSrc, gfx_surface_t *pDst,
                                       cpu_blit_dims_t *pBlit_dims)
{
    int error = 0;

    HAL_LOGD("From format: [%d] To format: [%d].\n", pSrc->format, pDst->format);

    if ((pDst->format != MPP_PIXEL_RGB565) &&
        (pDst->format != MPP_PIXEL_RGB) &&
        (pDst->format != MPP_PIXEL_BGR))
    {
        HAL_LOGE("Unsupported color conversion to format [%d].\n", pDst->format);
        return -1;
    }

    switch (pSrc->format) {
    case MPP_PIXEL_RGB:
        error = HAL_GfxDev_Cpu_RGB888ToRGB(pSrc, pDst, pBlit_dims);
        break;
    case MPP_PIXEL_RGB565:
        error = HAL_GfxDev_Cpu_RGB565ToRGB(pSrc, pDst, pBlit_dims);
        break;
    case MPP_PIXEL_YUV1P444:
    case MPP_PIXEL_UYVY1P422:
    case MPP_PIXEL_VYUY1P422:
        error = HAL_GfxDev_Cpu_YUVToRGB(pSrc, pDst, pBlit_dims);
        break;
    default:
        HAL_LOGE("Unsupported color conversion from format [%d].\n", pSrc->format);
        return -1;
    }

    return error;
}

/*
 * No Image conversion
 */
static int HAL_GfxDev_Cpu_NoneConvert(gfx_surface_t *pSrc, gfx_surface_t *pDst,
                                      cpu_blit_dims_t *pBlit_dims)
{
    HAL_LOGD("No image conversion.\n");

    for (int y = 0; y < pBlit_dims->dst_h; y++)
        memcpy(pDst->buf + pDst->pitch*y, pSrc->buf + pSrc->pitch*y,
               pBlit_dims->src_w * get_bitpp(pSrc->format)/8);

    return 0;
}

/*
 * @brief blit the source surface to the destination surface.
 *
 * @param *dev [in] Pointer to pxp device.
 * @param *pSrc [in] Pointer to source surface.
 * @param *pDst [in] Pointer to destination surface.
 * @param *pRotate [in] Pointer to the rotation config.
 * @param flip [in] Flip mode.
 *
 * @returns 0 for the success.
 */
int HAL_GfxDev_Cpu_Blit(
    const gfx_dev_t *dev, const gfx_surface_t *pSrc, const gfx_surface_t *pDst, const gfx_rotate_config_t *pRotate, mpp_flip_mode_t flip)
{
    int ret;
    if ( (pSrc->format == MPP_PIXEL_RGB565)
            && (pDst->format == MPP_PIXEL_RGB)
            && (pRotate->degree == ROTATE_0)
            && (flip == FLIP_NONE) )
    {
        ret = HAL_GfxDev_Scale_RGB565to888(dev, pSrc, pDst, pRotate, flip);
    }
    else
    {
        ret = HAL_GfxDev_any_OP(dev, pSrc, pDst, pRotate, flip);
    }
    return ret;
}

static int HAL_GfxDev_Scale_RGB565to888(
        const gfx_dev_t *dev, const gfx_surface_t *pSrc, const gfx_surface_t *pDst, const gfx_rotate_config_t *pRotate, mpp_flip_mode_t flip)
{
    int error = 0;
    int src_pitch = pSrc->pitch;
    int dst_pitch = pDst->pitch;
    int h_incr = 1;
    int v_incr = 1;
    uint8_t *srcbuf;
    uint8_t *dstbuf;
    int x, y, sub_x, sub_y;   /* position in destination */
    uint8_t r, g, b;

    int src_w = pSrc->right - pSrc->left + 1;
    int src_h = pSrc->bottom - pSrc->top + 1;
    int dst_w = pDst->right - pDst->left + 1;
    int dst_h = pDst->bottom - pDst->top + 1;
    int loop_width = dst_w;
    int loop_height = dst_h;

    HAL_LOGD("Input window: width=[%d], height=[%d].\n", src_w, src_h);
    HAL_LOGD("Output window: width=[%d], height=[%d].\n", dst_w, dst_h);

    HAL_LOGD("Input buffer addr=0x%x\n", (unsigned int)pSrc->buf);
    HAL_LOGD("Output buffer addr=0x%x\n", (unsigned int)pDst->buf);

    /* adapt buffers with crop and output window parameters */
    srcbuf = (uint8_t *)(pSrc->buf + (pSrc->left * 2) + (pSrc->top * pSrc->pitch));
    dstbuf = (uint8_t *)(pDst->buf + (pDst->left * 3) + (pDst->top * pDst->pitch));

    h_incr = (src_w - 1) * SUBPIXINC / (dst_w - 1);
    v_incr = (src_h - 1) * SUBPIXINC / (dst_h - 1);

    for (y = 0, sub_y = 0; y < loop_height; y++, sub_y += v_incr)
    {
        for (x = 0, sub_x = 0; x < loop_width; x++, sub_x += h_incr)
        {
            r = scale_rgb565_rot0(sub_x, sub_y, srcbuf, src_pitch, 0, src_w, src_h);
            g = scale_rgb565_rot0(sub_x, sub_y, srcbuf, src_pitch, 1, src_w, src_h);
            b = scale_rgb565_rot0(sub_x, sub_y, srcbuf, src_pitch, 2, src_w, src_h);
            write_rgb888(&dstbuf[(y * dst_pitch) + (x * 3)], r, g, b);
        }
    }

#if (ENABLE_PISANO_CHECKSUM == 1)
    checksum_data_t checksum;
    checksum.type = CHECKSUM_TYPE_PISANO;
    checksum.value = calc_checksum((dst_w*get_bitpp(pDst->format)/8) * dst_h, dstbuf);
    HAL_LOGD("CHECKSUM=0x%X\n", checksum.value);
    if (dev->callback != NULL)
        dev->callback(NULL, MPP_EVENT_INTERNAL_TEST_RESERVED,
                      (void *) &checksum, dev->user_data);
#endif
    return error;
}

static int HAL_GfxDev_any_OP(
        const gfx_dev_t *dev, const gfx_surface_t *pSrc, const gfx_surface_t *pDst, const gfx_rotate_config_t *pRotate, mpp_flip_mode_t flip)
{
    int error = 0;
    int src_pitch = pSrc->pitch;
    int dst_pitch = pDst->pitch;
    int h_incr = 1;
    int v_incr = 1;
    int srcBPP = get_bitpp(pSrc->format)/8;
    int dstBPP = get_bitpp(pDst->format)/8;
    uint8_t *srcbuf;
    uint8_t *dstbuf;
    int x, y, sub_x, sub_y;   /* position in destination */
    /* nb of RGB pixels in 4 bytes YUV */
    int nb_pixels_in_4B = RGB_PIXELS_IN_4B_YUV444;
    bool scaling = false;
    uint8_t pixel[MAX_COMP_PER_PIXEL];
    uint8_t r, g, b;

    get_color_byte f_get_color_byte = NULL;
    color_conv     f_color_conv = color_conv_none;
    write_pixel    f_write_pixel = NULL;
    get_src_pos    f_get_src_pos = NULL;
    get_dest_pos   f_get_dest_pos = get_dest_pos_default;

    int src_w = pSrc->right - pSrc->left + 1;
    int src_h = pSrc->bottom - pSrc->top + 1;
    int dst_w = pDst->right - pDst->left + 1;
    int dst_h = pDst->bottom - pDst->top + 1;
    int loop_width = dst_w;
    int loop_height = dst_h;

    HAL_LOGD("Input window: width=[%d], height=[%d].\n", src_w, src_h);
    HAL_LOGD("Output window: width=[%d], height=[%d].\n", dst_w, dst_h);

    HAL_LOGD("Input buffer addr=0x%x\n", (unsigned int)pSrc->buf);
    HAL_LOGD("Output buffer addr=0x%x\n", (unsigned int)pDst->buf);

    /* adapt buffers with crop and output window parameters */
    srcbuf = (uint8_t *)(pSrc->buf + (pSrc->left * get_bitpp(pSrc->format)/8) + (pSrc->top * pSrc->pitch));
    dstbuf = (uint8_t *)(pDst->buf + (pDst->left * get_bitpp(pDst->format)/8) + (pDst->top * pDst->pitch));

    if ( ((pRotate->degree == ROTATE_0) || (pRotate->degree == ROTATE_180)) &&
         ((dst_w != src_w) || (dst_h != src_h)) ) {
        h_incr = (src_w - 1) * SUBPIXINC / (dst_w - 1);
        v_incr = (src_h - 1) * SUBPIXINC / (dst_h - 1);
        scaling = true;
    }
    else if ( ((pRotate->degree == ROTATE_90) || (pRotate->degree == ROTATE_270)) &&
              ((dst_w != src_h) || (dst_h != src_w)) ) {
        h_incr = (src_h - 1) * SUBPIXINC / (dst_w - 1);
        v_incr = (src_w - 1) * SUBPIXINC / (dst_h - 1);
        scaling = true;
    }

    switch (pSrc->format) {
    case MPP_PIXEL_RGB:
        if (scaling)
            f_get_color_byte = scale_rgb888;
        else
            f_get_color_byte = get_color_byte_from_rgb888;
        break;
    case MPP_PIXEL_YUV1P444:
        nb_pixels_in_4B = RGB_PIXELS_IN_4B_YUV444;
        if (scaling)
            f_get_color_byte = scale_rgb888;
        else
            f_get_color_byte = get_color_byte_from_rgb888;
        f_color_conv = color_VUYX444ToRGB;
        break;
    case MPP_PIXEL_UYVY1P422:
        nb_pixels_in_4B = RGB_PIXELS_IN_4B_YUV422;
        if (scaling) {
            HAL_LOGE("Scaling for YUV422 is not supported yet\n");
            return -1;
        }
        else {
            f_get_color_byte = get_color_byte_from_rgb888;
        }
        f_color_conv = color_UYVY422ToRGB;
        break;
    case MPP_PIXEL_VYUY1P422:
        nb_pixels_in_4B = RGB_PIXELS_IN_4B_YUV422;
        if (scaling) {
            HAL_LOGE("Scaling for YUV422 is not supported yet\n");
            return -1;
        }
        else {
            f_get_color_byte = get_color_byte_from_rgb888;
        }
        f_color_conv = color_VYUY422ToRGB;
        break;
    case MPP_PIXEL_RGB565:
        if (scaling)
            f_get_color_byte = scale_rgb565;
        else
            f_get_color_byte = get_color_byte_from_rgb565;
        break;
    default:
        HAL_LOGE("Unsupported src format [%d]\n", pSrc->format);
        return -1;
    }

    switch (pDst->format) {
    case MPP_PIXEL_RGB:
        f_write_pixel = write_rgb888;
        break;
    case MPP_PIXEL_BGR:
        f_write_pixel = write_bgr888;
        break;
    case MPP_PIXEL_RGB565:
        f_write_pixel = write_rgb565;
        break;
    default:
        HAL_LOGE("Unsupported dst format [%d]\n", pDst->format);
        return -1;
    }

    /* Set f_get_src_pos() */
    switch (flip) {
    case FLIP_NONE:
        HAL_LOGD("FLIP_NONE.\n");
        if (pRotate->degree == ROTATE_0)        f_get_src_pos = rotate0_map;
        else if (pRotate->degree == ROTATE_90)  f_get_src_pos = rotate90_map;
        else if (pRotate->degree == ROTATE_180) f_get_src_pos = rotate180_map;
        else if (pRotate->degree == ROTATE_270) f_get_src_pos = rotate270_map;
        else HAL_LOGE("Unsupported rotation value [%d]\n", pRotate->degree);
	break;
    case FLIP_HORIZONTAL:
        HAL_LOGD("FLIP_HORIZONTAL.\n");
        if (pRotate->degree == ROTATE_0)        f_get_src_pos = flip_h_map;
        else if (pRotate->degree == ROTATE_90)  f_get_src_pos = flip_h_90_map;
        else if (pRotate->degree == ROTATE_180) f_get_src_pos = flip_h_180_map;
        else if (pRotate->degree == ROTATE_270) f_get_src_pos = flip_h_270_map;
        else HAL_LOGE("Unsupported rotation value [%d]\n", pRotate->degree);
        break;
    case FLIP_VERTICAL:
        HAL_LOGD("FLIP_VERTICAL.\n");
        if (pRotate->degree == ROTATE_0)        f_get_src_pos = flip_v_map;
        else if (pRotate->degree == ROTATE_90)  f_get_src_pos = flip_v_90_map;
        else if (pRotate->degree == ROTATE_180) f_get_src_pos = flip_v_180_map;
        else if (pRotate->degree == ROTATE_270) f_get_src_pos = flip_v_270_map;
        else HAL_LOGE("Unsupported rotation value [%d]\n", pRotate->degree);
	break;
    case FLIP_BOTH:
        HAL_LOGD("FLIP_BOTH.\n");
        if (pRotate->degree == ROTATE_0)        f_get_src_pos = flip_both_map;
        else if (pRotate->degree == ROTATE_90)  f_get_src_pos = flip_both_90_map;
        else if (pRotate->degree == ROTATE_180) f_get_src_pos = flip_both_180_map;
        else if (pRotate->degree == ROTATE_270) f_get_src_pos = flip_both_270_map;
        else HAL_LOGE("Unsupported rotation value [%d]\n", pRotate->degree);
	break;
    default:
        HAL_LOGE("Unsupported flip value [%d]\n", flip);
        return -1;
    }

    /* Set f_get_dest_pos() */
    switch (pRotate->degree) {
    case ROTATE_0:
        HAL_LOGD("ROTATE 0.\n");
        loop_width = dst_w/nb_pixels_in_4B;
        if (nb_pixels_in_4B == RGB_PIXELS_IN_4B_YUV422) {
            if (flip == FLIP_NONE)            f_get_dest_pos = get_dest_pos422_x_non_reversed;
            else if (flip == FLIP_HORIZONTAL) f_get_dest_pos = get_dest_pos422_x_reversed;
            else if (flip == FLIP_VERTICAL)   f_get_dest_pos = get_dest_pos422_x_non_reversed;
            else if (flip == FLIP_BOTH)       f_get_dest_pos = get_dest_pos422_x_reversed;
            else HAL_LOGE("Unsupported flip value [%d]\n", flip);
        }
	break;
    case ROTATE_90:
        HAL_LOGD("ROTATE 90.\n");
        loop_height = dst_h/nb_pixels_in_4B;
        if (nb_pixels_in_4B == RGB_PIXELS_IN_4B_YUV422) {
            if (flip == FLIP_NONE)            f_get_dest_pos = get_dest_pos422_y_non_reversed;
            else if (flip == FLIP_HORIZONTAL) f_get_dest_pos = get_dest_pos422_y_reversed;
            else if (flip == FLIP_VERTICAL)   f_get_dest_pos = get_dest_pos422_y_non_reversed;
            else if (flip == FLIP_BOTH)       f_get_dest_pos = get_dest_pos422_y_reversed;
            else HAL_LOGE("Unsupported flip value [%d]\n", flip);
        }
	break;
    case ROTATE_180:
        HAL_LOGD("ROTATE 180.\n");
        loop_width = dst_w/nb_pixels_in_4B;
        if (nb_pixels_in_4B == RGB_PIXELS_IN_4B_YUV422) {
            if (flip == FLIP_NONE)            f_get_dest_pos = get_dest_pos422_x_reversed;
            else if (flip == FLIP_HORIZONTAL) f_get_dest_pos = get_dest_pos422_x_non_reversed;
            else if (flip == FLIP_VERTICAL)   f_get_dest_pos = get_dest_pos422_x_reversed;
            else if (flip == FLIP_BOTH)       f_get_dest_pos = get_dest_pos422_x_non_reversed;
            else HAL_LOGE("Unsupported flip value [%d]\n", flip);
        }
	break;
    case ROTATE_270:
        HAL_LOGD("ROTATE 270.\n");
        loop_height = dst_h/nb_pixels_in_4B;
        if (nb_pixels_in_4B == RGB_PIXELS_IN_4B_YUV422) {
            if (flip == FLIP_NONE)            f_get_dest_pos = get_dest_pos422_y_reversed;
            else if (flip == FLIP_HORIZONTAL) f_get_dest_pos = get_dest_pos422_y_non_reversed;
            else if (flip == FLIP_VERTICAL)   f_get_dest_pos = get_dest_pos422_y_reversed;
            else if (flip == FLIP_BOTH)       f_get_dest_pos = get_dest_pos422_y_non_reversed;
            else HAL_LOGE("Unsupported flip value [%d]\n", flip);
        }
	break;
    default:
        HAL_LOGE("Unsupported rotation value [%d]\n", pRotate->degree);
        return -1;
    }

    if ((f_get_color_byte == NULL) || (f_color_conv == NULL) ||
        (f_write_pixel == NULL) || (f_get_src_pos == NULL) ||
        (f_get_dest_pos == NULL)) {
        HAL_LOGE("Error function pointers undefined\n");
        return -1;
    }

    for (y = 0, sub_y = 0; y < loop_height; y++, sub_y += v_incr)
    {
        for (x = 0, sub_x = 0; x < loop_width; x++, sub_x += h_incr)
        {
            /*
             * TODO: optimize the number of loops since 3 loops
             * is enough for other formats than YUV422 - [MPP-334]
             */
            for (int comp_offset = 0; comp_offset < MAX_COMP_PER_PIXEL; comp_offset++)
            {
                /* Since bpp of 422 is 2, but the loop is processing 4 bytes
                 * => the horizontal offset should be multiplied by nb_pixels_in_4B
                 * => the source width should be divided by nb_pixels_in_4B
                 */
                pixel[comp_offset] = f_get_color_byte(sub_x, sub_y, srcbuf, src_pitch, srcBPP*nb_pixels_in_4B, comp_offset, f_get_src_pos, src_w/nb_pixels_in_4B, src_h);
            }
            for (int pix_id = 0; pix_id < nb_pixels_in_4B; pix_id++) {
                f_color_conv(pix_id, pixel, &r, &g, &b);
                f_write_pixel(&dstbuf[f_get_dest_pos(x, y, dst_pitch, dstBPP, pix_id)], r, g, b);
            }
        }
    }

#if (ENABLE_PISANO_CHECKSUM == 1)
    checksum_data_t checksum;
    checksum.type = CHECKSUM_TYPE_PISANO;
    checksum.value = calc_checksum((dst_w*get_bitpp(pDst->format)/8) * dst_h, dstbuf);
    HAL_LOGD("CHECKSUM=0x%X\n", checksum.value);
    if (dev->callback != NULL)
        dev->callback(NULL, MPP_EVENT_INTERNAL_TEST_RESERVED,
                      (void *) &checksum, dev->user_data);
#endif
    return error;
}

const static gfx_dev_operator_t s_GfxDevCpuOps = {
    .blit         = HAL_GfxDev_Cpu_Blit,
    .get_buf_desc = HAL_GfxDev_Cpu_Getbufdesc,
};

int HAL_GfxDev_CPU_Register(gfx_dev_t *dev)
{
    dev->id = 0;    /* TODO set unique id */
    dev->ops = &s_GfxDevCpuOps;

    return 0;
}
#else  /* (defined HAL_ENABLE_2D_IMGPROC) */
int HAL_GfxDev_CPU_Register(gfx_dev_t *dev)
{
    HAL_LOGE("CPU graphics not enabled\n");
    return -1;
}
#endif  /* (defined HAL_ENABLE_2D_IMGPROC) */
