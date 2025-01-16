/*
 * Copyright 2019-2024 NXP.
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

/*
 * @brief PXP (Pixel Pipeline) HAL gfx driver implementation. Used for augmenting image data to convert between
 * different image formats and compose images w/ GUI overlays.
 */

#include "mpp_config.h"
#include "hal_graphics_dev.h"
#include "hal_debug.h"
#if (defined HAL_ENABLE_2D_IMGPROC) && (HAL_ENABLE_GFX_DEV_Pxp == 1)
#include "hal_os.h"
#include "fsl_common.h"
#include "fsl_pxp.h"
#include "fsl_cache.h"
#include "hal.h"
#include "hal_utils.h"

#define PXP_DEV PXP

/* PXP build gray operation is disabled by default since implementation
 * requires rework.
 */
#ifndef PXP_BUILD_GRAY
#define PXP_BUILD_GRAY 0
#endif

/*
 * Notice: enable the lock will introduce 1 ms time cost for each PXP operation
 */
#define ENABLE_PXP_LOCK 1

#if defined(__cplusplus)
extern "C" {
#endif
int HAL_GfxDev_Pxp_Register();
int HAL_GfxDev_Pxp_Init(const gfx_dev_t *dev, void *param);
#if defined(__cplusplus)
}
#endif

typedef struct _gfx_pxp_handle
{
    int pxpInit;
    pxp_ps_buffer_config_t psBufferConfig;
    pxp_as_buffer_config_t asBufferConfig;
    pxp_as_blend_config_t asBlendConfig;
    pxp_output_buffer_config_t outputBufferConfig;
#if ENABLE_PXP_LOCK
    hal_mutex_t mutex; /*!< Mutex to lock the handle during a transfer */
#endif
    hal_sema_t semaphore; /*!< Semaphore to notify and unblock task when transfer ends */
} gfx_pxp_handle_t;

static gfx_pxp_handle_t s_GfxPxpHandle;
#if (PXP_BUILD_GRAY == 1)
static gfx_surface_t s_SurfGray888x;
#endif

static int HAL_GfxDev_Pxp_ComposeSurfaceAdaptForScaleAndPsRotate(const gfx_dev_t *dev,
                                                                 const gfx_surface_t *pSrc,
                                                                 const gfx_surface_t *pDst,
                                                                 gfx_surface_t *pSrc_adapt);

void PXP_IRQHandler(void)
{
    if (s_GfxPxpHandle.semaphore != NULL)
    {
        bool result;
        long HigherPriorityTaskWoken = 0;
        PXP_ClearStatusFlags(PXP_DEV, kPXP_CompleteFlag);
        result = hal_sema_give_isr(s_GfxPxpHandle.semaphore, &HigherPriorityTaskWoken);

        if (result != true)
        {
            hal_sched_yield(HigherPriorityTaskWoken);
        }
    }
}

static int _HAL_GfxDev_Pxp_Lock()
{
    int error = 0;

    // TODO Temporary here
    HAL_GfxDev_Pxp_Init(NULL, NULL);

#if ENABLE_PXP_LOCK
    int ret;

    if (s_GfxPxpHandle.mutex == NULL)
    {
        return -1;
    }

    ret = hal_mutex_lock(s_GfxPxpHandle.mutex);

    if (MPP_SUCCESS != ret)
    {
        error = -1;
    }
    else
    {
        error = 0;
    }

#endif

    return error;
}

static int _HAL_GfxDev_Pxp_Unlock()
{
    int error = 0;

#if ENABLE_PXP_LOCK
    int ret;

    if (s_GfxPxpHandle.mutex == NULL)
    {
        return -1;
    }

    ret = hal_mutex_unlock(s_GfxPxpHandle.mutex);

    if (MPP_SUCCESS != ret)
    {
        error = -1;
    }
    else
    {
        error = 0;
    }

#endif

    return error;
}

int HAL_GfxDev_Pxp_Init(const gfx_dev_t *dev, void *param)
{
    int error = 0;

    if (s_GfxPxpHandle.pxpInit == 0)
    {
        // Notice: do the PXP_Init at the first PXP operation as observed the PXP didn't work if we put the
        //         PXP_Init in the HAL_GfxDev_Pxp_Register which will run in the main context.
        PXP_Init(PXP_DEV);
        PXP_EnableInterrupts(PXP_DEV, kPXP_CompleteInterruptEnable);
        EnableIRQ(PXP_IRQn);
        NVIC_SetPriority(PXP_IRQn, hal_get_max_syscall_prio() + 1);

        PXP_SetProcessSurfaceBackGroundColor(PXP_DEV, 0U);
        // disable the AS
        PXP_SetAlphaSurfacePosition(PXP_DEV, 0xFFFFU, 0xFFFFU, 0U, 0U);

#if ENABLE_PXP_LOCK
        hal_mutex_create(&s_GfxPxpHandle.mutex);

        if (s_GfxPxpHandle.mutex == NULL)
        {
            HAL_LOGE("Create PXP mutex\n");
            return error;
        }

#endif
        s_GfxPxpHandle.semaphore = hal_sema_create_binary();

        if (s_GfxPxpHandle.semaphore == NULL)
        {
            HAL_LOGE("Create PXP semaphore\n");
            return error;
        }

        s_GfxPxpHandle.pxpInit = 1;
    }

    return error;
}

int HAL_GfxDev_Pxp_Getbufdesc(const gfx_dev_t *dev, hw_buf_desc_t *in_buf, hw_buf_desc_t *out_buf, mpp_memory_policy_t *policy)
{
    int error = 0;
    do {
        if ((in_buf == NULL) || (out_buf == NULL) || (policy == NULL))
        {
            HAL_LOGE("\nNULL pointer to buffer descriptor\n");
            error =  -1;
            break;
        }
        /* set memory policy */
        *policy = HAL_MEM_ALLOC_NONE;
        /* set hw requirement */
        in_buf->alignment = 0;
        in_buf->nb_lines = 0;
        in_buf->cacheable = false;
        in_buf->stride = 0;
        out_buf->alignment = 0;
        out_buf->cacheable = false;
        out_buf->stride = 0;
    } while(false);
    return error;
}

int HAL_GfxDev_Pxp_Deinit(const gfx_dev_t *dev)
{
    int error = 0;
    return error;
}

static int set_input_buffer_format(pxp_ps_buffer_config_t *pPsBufferConfig, gfx_surface_t *pSrc)
{
    int error = 0;

    // setup input buffer configuration
    switch (pSrc->format)
    {
        case MPP_PIXEL_BGRA:
        {
#if (!(defined(FSL_FEATURE_PXP_HAS_NO_EXTEND_PIXEL_FORMAT) && FSL_FEATURE_PXP_HAS_NO_EXTEND_PIXEL_FORMAT)) || \
		(!(defined(FSL_FEATURE_PXP_V3) && FSL_FEATURE_PXP_V3))
            pPsBufferConfig->pixelFormat = kPXP_PsPixelFormatARGB8888;
#else
            pPsBufferConfig->pixelFormat = kPXP_PsPixelFormatRGB888;
#endif
        }
        break;

        case MPP_PIXEL_YUV1P444:
        {
            pPsBufferConfig->pixelFormat = kPXP_PsPixelFormatYUV1P444;
        }
        break;

        case MPP_PIXEL_UYVY1P422:
        {
            pPsBufferConfig->pixelFormat = kPXP_PsPixelFormatUYVY1P422;
        }
        break;

        case MPP_PIXEL_VYUY1P422:
        {
            pPsBufferConfig->pixelFormat = kPXP_PsPixelFormatVYUY1P422;
        }
        break;

        case MPP_PIXEL_GRAY888X:
        {
#if (!(defined(FSL_FEATURE_PXP_HAS_NO_EXTEND_PIXEL_FORMAT) && FSL_FEATURE_PXP_HAS_NO_EXTEND_PIXEL_FORMAT)) || \
		(!(defined(FSL_FEATURE_PXP_V3) && FSL_FEATURE_PXP_V3))
            pPsBufferConfig->pixelFormat = kPXP_PsPixelFormatARGB8888;
#else
            pPsBufferConfig->pixelFormat = kPXP_PsPixelFormatRGB888;
#endif
        }
        break;

        case MPP_PIXEL_RGB565:
        {
            pPsBufferConfig->pixelFormat = kPXP_PsPixelFormatRGB565;
        }
        break;

        default:
        {
            error = -1;
            HAL_LOGE("Unsupported source surface pixel format: %d\n", pSrc->format);
            return error;
        }
        break;
    }

    return error;
}

static int init_input_buffer(pxp_ps_buffer_config_t *pPsBufferConfig, gfx_surface_t *pSrc)
{
    int error = 0;
    int bpp = get_bitpp(pSrc->format);

    error = set_input_buffer_format(pPsBufferConfig, pSrc);

    if (error == -1)
    {
        return error;
    }

    pPsBufferConfig->swapByte    = pSrc->swapByte;
    pPsBufferConfig->bufferAddrU = 0U;
    pPsBufferConfig->bufferAddrV = 0U;
    pPsBufferConfig->pitchBytes  = pSrc->pitch;
    pPsBufferConfig->bufferAddr  = (uint32_t)pSrc->buf + (pSrc->left * bpp / 8) + (pSrc->top * pSrc->pitch);
    PXP_SetProcessSurfaceBackGroundColor(PXP_DEV, 0U);
    PXP_SetProcessSurfaceBufferConfig(PXP_DEV, pPsBufferConfig);

    return error;
}

static void set_surface_flip(mpp_flip_mode_t flip, pxp_flip_mode_t *flipMode)
{
    switch (flip)
    {
        case FLIP_HORIZONTAL:
        {
            *flipMode = kPXP_FlipHorizontal;
        }
        break;

        case FLIP_VERTICAL:
        {
            *flipMode = kPXP_FlipVertical;
        }
        break;

        case FLIP_BOTH:
        {
            *flipMode = kPXP_FlipBoth;
        }
        break;

        default:
        {
            *flipMode = kPXP_FlipDisable;
        }
        break;
    }
}

static void set_surface_rotate(gfx_rotate_config_t *pRotate,
                               pxp_rotate_position_t *rotatePosition,
                               pxp_rotate_degree_t *rotateDegree)
{
    if (pRotate != NULL)
    {
        if (pRotate->target == kGFXRotate_SRCSurface)
        {
            *rotatePosition = kPXP_RotateProcessSurface;
        }

        switch (pRotate->degree)
        {
            case ROTATE_90:
            {
                *rotateDegree = kPXP_Rotate90;
            }
            break;

            case ROTATE_180:
            {
                *rotateDegree = kPXP_Rotate180;
            }
            break;

            case ROTATE_270:
            {
                *rotateDegree = kPXP_Rotate270;
            }
            break;

            default:
            {
                *rotateDegree = kPXP_Rotate0;
            }
            break;
        }
    }
}

static void init_surface(mpp_flip_mode_t flip, gfx_rotate_config_t *pRotate, gfx_surface_t *pSrc, gfx_surface_t *pDst)
{
    // setup flip/rotation/position/scaler configuration
    pxp_rotate_position_t rotatePosition = kPXP_RotateOutputBuffer;
    pxp_rotate_degree_t rotateDegree     = kPXP_Rotate0;
    pxp_flip_mode_t flipMode             = kPXP_FlipDisable;
    uint16_t top, bottom, left, right;

    set_surface_flip(flip, &flipMode);

    set_surface_rotate(pRotate, &rotatePosition, &rotateDegree);

    if (pRotate->target == kGFXRotate_DSTSurface)
    {
        if ((pRotate->degree == ROTATE_90) || (pRotate->degree == ROTATE_270))
        {
            left = pDst->top;
            top = pDst->left;
            right = pDst->bottom;
            bottom = pDst->right;
        } else if (pRotate->degree == ROTATE_180)
        {
            left = pDst->right;
            top = pDst->bottom;
            right = pDst->left;
            bottom = pDst->top;
        } else  /* 0 degree */
        {
            left = pDst->left;
            top = pDst->top;
            right = pDst->right;
            bottom = pDst->bottom;
        }
    } else  /* 0 degree */
    {
        left = pDst->left;
        top = pDst->top;
        right = pDst->right;
        bottom = pDst->bottom;
    }

    PXP_SetRotateConfig(PXP_DEV, rotatePosition, rotateDegree, flipMode);
    PXP_SetProcessSurfacePosition(PXP_DEV, left, top, right, bottom);
    PXP_SetProcessSurfaceScaler(PXP_DEV, (pSrc->right - pSrc->left + 1), (pSrc->bottom - pSrc->top + 1),
                                (right - left + 1), (bottom - top + 1));
}

static int set_output_buffer_format(pxp_output_buffer_config_t *pOutputBufferConfig, gfx_surface_t *pDst)
{
    int error = 0;

    switch (pDst->format)
    {
    /* TODO support RGB 32 bits unpacked */
#if (HAL_PXP_WORKAROUND_OUT_RGB == 1)
        case MPP_PIXEL_BGR:
#endif
        case MPP_PIXEL_RGB:
        {
            pOutputBufferConfig->pixelFormat = kPXP_OutputPixelFormatRGB888P;
        }
        break;

        case MPP_PIXEL_RGB565:
        {
            pOutputBufferConfig->pixelFormat = kPXP_OutputPixelFormatRGB565;
        }
        break;

        case MPP_PIXEL_YUV1P444:
        {
            pOutputBufferConfig->pixelFormat = kPXP_OutputPixelFormatYUV1P444;
        }
        break;

        case MPP_PIXEL_UYVY1P422:
        {
            pOutputBufferConfig->pixelFormat = kPXP_OutputPixelFormatUYVY1P422;
        }
        break;

        case MPP_PIXEL_VYUY1P422:
        {
            pOutputBufferConfig->pixelFormat = kPXP_OutputPixelFormatVYUY1P422;
        }
        break;

        default:
        {
            error = -1;
            HAL_LOGE("Unsupported output surface pixel format: %d\n", pDst->format);
            return error;
        }
        break;
    }

    return error;
}

static int init_output_buffer(pxp_output_buffer_config_t *pOutputBufferConfig, gfx_surface_t *pDst)
{
    int error = 0;

    error = set_output_buffer_format(pOutputBufferConfig, pDst);

    if (error == -1)
    {
        return error;
    }

    pOutputBufferConfig->interlacedMode = kPXP_OutputProgressive;
    pOutputBufferConfig->buffer1Addr    = 0U;
    pOutputBufferConfig->pitchBytes     = pDst->pitch;
    pOutputBufferConfig->width          = pDst->width;
    pOutputBufferConfig->height         = pDst->height;
    pOutputBufferConfig->buffer0Addr    = (uint32_t)pDst->buf;
    PXP_SetOutputBufferConfig(PXP_DEV, pOutputBufferConfig);

    return error;
}

static int HAL_GfxDev_Pxp_Gray16_2_gray888x(gfx_surface_t *pSrc, gfx_surface_t *pDst)
{
    int error = -1;

    if (pSrc->format == MPP_PIXEL_GRAY16 && pDst->format == MPP_PIXEL_GRAY888X)
    {
        /* use the cpu to do the gray16 to gray888x conversion */
        int srcBlit_w = pSrc->right - pSrc->left + 1;
        int srcBlit_h = pSrc->bottom - pSrc->top + 1;

        int dstBlit_w = pDst->right - pDst->left + 1;
        int dstBlit_h = pDst->bottom - pDst->top + 1;

        if (dstBlit_w > srcBlit_w || dstBlit_h > srcBlit_h)
        {
            HAL_LOGE("Unsupported blit\n");
            error = -1;
            return error;
        }

        unsigned short *pGray16 = (unsigned short *)((unsigned char *)pSrc->buf + pSrc->pitch * pSrc->top);
        unsigned int *pGray888X = (unsigned int *)((unsigned char *)pDst->buf + pDst->pitch * pDst->top);

        for (int i = 0; i < dstBlit_h; i++)
        {
            unsigned short *pGray16Line = &pGray16[(pSrc->pitch * i) >> 1];
            unsigned int *pGray888XLine = &pGray888X[(pDst->pitch * i) >> 2];

            for (int j = 0; j < dstBlit_w; j += 2)
            {
                unsigned char tmp0   = pGray16Line[j] & 0xFF;
                unsigned char tmp1   = pGray16Line[j + 1] & 0xFF;
                pGray888XLine[j]     = (tmp0 << 16) | (tmp0 << 8) | tmp0;
                pGray888XLine[j + 1] = (tmp1 << 16) | (tmp1 << 8) | tmp1;
            }
        }

        return 0;
    }

    return error;
}

static int HAL_GfxDev_Pxp_BuildGray888XFromGray16(gfx_surface_t *pSrc, gfx_surface_t **ppDst)
{
    int error = -1;

#if (PXP_BUILD_GRAY == 1)
    if ((s_SurfGray888x.height * s_SurfGray888x.pitch) < (pSrc->height * pSrc->width * 4))
    {
        /* need to re-allocate the buffer */
        if (s_SurfGray888x.buf != NULL)
        {
            hal_free(s_SurfGray888x.buf);
        }

        s_SurfGray888x.width  = pSrc->width;
        s_SurfGray888x.height = pSrc->height;
        s_SurfGray888x.pitch  = pSrc->width * 4;
        s_SurfGray888x.format = MPP_PIXEL_GRAY888X;

        s_SurfGray888x.buf = hal_malloc(s_SurfGray888x.height * s_SurfGray888x.pitch);
        if (s_SurfGray888x.buf == NULL)
        {
            HAL_LOGE("MPP_MALLOC\n");
            error = -1;
            return error;
        }
    }

    s_SurfGray888x.left   = pSrc->left;
    s_SurfGray888x.top    = pSrc->top;
    s_SurfGray888x.right  = pSrc->right;
    s_SurfGray888x.bottom = pSrc->bottom;
    s_SurfGray888x.lock   = pSrc->lock;

    error = HAL_GfxDev_Pxp_Gray16_2_gray888x(pSrc, &s_SurfGray888x);
    if (error)
    {
        HAL_LOGE("Failed to convert GRAY16 image to GRAY888X\n");
        return error;
    }

    *ppDst = &s_SurfGray888x;
#endif
    return error;
}

#if (PXP_BUILD_GRAY == 1)
static int HAL_GfxDev_Pxp_Depth16_2_gray888x(gfx_surface_t *pSrc, gfx_surface_t *pDst)
{
    int error = -1;

    if (pSrc->format == MPP_PIXEL_DEPTH16 && pDst->format == MPP_PIXEL_GRAY888X)
    {
        /* use the cpu to do the gray16 to gray888x conversion */
        int srcBlit_w = pSrc->right - pSrc->left + 1;
        int srcBlit_h = pSrc->bottom - pSrc->top + 1;

        int dstBlit_w = pDst->right - pDst->left + 1;
        int dstBlit_h = pDst->bottom - pDst->top + 1;

        if (dstBlit_w > srcBlit_w || dstBlit_h > srcBlit_h)
        {
            HAL_LOGE("Unsupported blit\n");
            error = -1;
            return error;
        }

        unsigned short *pDepth16 = (unsigned short *)((unsigned char *)pSrc->buf + pSrc->pitch * pSrc->top);
        unsigned int *pGray888X  = (unsigned int *)((unsigned char *)pDst->buf + pDst->pitch * pDst->top);

        for (int i = 0; i < dstBlit_h; i++)
        {
            unsigned short *pGray16Line = &pDepth16[(pSrc->pitch * i) >> 1];
            unsigned int *pGray888xLine = &pGray888X[(pDst->pitch * i) >> 2];

            for (int j = 0; j < dstBlit_w; j += 2)
            {
                unsigned char tmp0   = (pGray16Line[j] >> 2) & 0xFF;
                unsigned char tmp1   = (pGray16Line[j + 1] >> 2) & 0xFF;
                pGray888xLine[j]     = (tmp0 << 16) | (tmp0 << 8) | tmp0;
                pGray888xLine[j + 1] = (tmp1 << 16) | (tmp1 << 8) | tmp1;
            }
        }

        return 0;
    }

    return error;
}
#endif

static int HAL_GfxDev_Pxp_BuildGray888XFromDepth16(gfx_surface_t *pSrc, gfx_surface_t **ppDst)
{
    int error = -1;

#if (PXP_BUILD_GRAY == 1)
    if ((s_SurfGray888x.height * s_SurfGray888x.pitch) < (pSrc->height * pSrc->width * 4))
    {
        /* need to re-allocate the buffer */
        if (s_SurfGray888x.buf != NULL)
        {
            hal_free(s_SurfGray888x.buf);
        }

        s_SurfGray888x.width  = pSrc->width;
        s_SurfGray888x.height = pSrc->height;
        s_SurfGray888x.pitch  = pSrc->width * 4;
        s_SurfGray888x.format = MPP_PIXEL_GRAY888X;

        s_SurfGray888x.buf = hal_malloc(s_SurfGray888x.height * s_SurfGray888x.pitch);
        if (s_SurfGray888x.buf == NULL)
        {
            HAL_LOGE("MPP_MALLOC\n");
            error = -1;
            return error;
        }
    }

    s_SurfGray888x.left   = pSrc->left;
    s_SurfGray888x.top    = pSrc->top;
    s_SurfGray888x.right  = pSrc->right;
    s_SurfGray888x.bottom = pSrc->bottom;
    s_SurfGray888x.lock   = pSrc->lock;

    error = HAL_GfxDev_Pxp_Depth16_2_gray888x(pSrc, &s_SurfGray888x);
    if (error)
    {
        HAL_LOGE("Failed to convert DEPTH16 image to GRAY888X\n");
        return error;
    }

    *ppDst = &s_SurfGray888x;
#endif
    return error;
}

static int HAL_GfxDev_Pxp_YUYV1P422ToYUV420P(gfx_surface_t *pSrc, gfx_surface_t *pDst)
{
    unsigned char *pUYVY    = (unsigned char *)pSrc->buf;
    unsigned char *pYUV420P = (unsigned char *)pDst->buf;
    int w                   = pSrc->width;
    int h                   = pSrc->height;

    unsigned int *pY = (unsigned int *)pYUV420P;
    unsigned int *pU = (unsigned int *)(pYUV420P + w * h);
    unsigned int *pV = (unsigned int *)(pYUV420P + w * h + w * h / 4);

    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w / 16; j++)
        {
            /* fill the Y */
            *pY++ = (pUYVY[0] | (pUYVY[2] << 8) | (pUYVY[4] << 16) | (pUYVY[6] << 24));
            *pY++ = (pUYVY[8] | (pUYVY[10] << 8) | (pUYVY[12] << 16) | (pUYVY[14] << 24));

            *pY++ = (pUYVY[16] | (pUYVY[18] << 8) | (pUYVY[20] << 16) | (pUYVY[22] << 24));
            *pY++ = (pUYVY[24] | (pUYVY[26] << 8) | (pUYVY[28] << 16) | (pUYVY[30] << 24));

            /* sample the UV with only even row */
            if ((i & 0x1) == 0)
            {
                *pU++ = (pUYVY[1] | (pUYVY[5] << 8) | (pUYVY[9] << 16) | (pUYVY[13] << 24));
                *pU++ = (pUYVY[17] | (pUYVY[21] << 8) | (pUYVY[25] << 16) | (pUYVY[29] << 24));
                *pV++ = (pUYVY[3] | (pUYVY[7] << 8) | (pUYVY[11] << 16) | (pUYVY[15] << 24));
                *pV++ = (pUYVY[19] | (pUYVY[23] << 8) | (pUYVY[27] << 16) | (pUYVY[31] << 24));
            }
            pUYVY += 32;
        }
    }

    return 0;
}

#if (HAL_PXP_WORKAROUND_OUT_RGB == 1)
/*
 * software conversion from BGR24 to RGB24
 */
static int convertCPU_BGR2RGB(uint8_t *buffer, int pitch, int width, int height)
{
    int x, y;
    uint8_t pixBlue;
    int srcBPP = 3;

    DCACHE_CleanInvalidateByRange((uint32_t) buffer, pitch*height);

    for (y = 0 ; y < height; y++) {
        for (x = 0 ; x < width; x++) {
            int i = (y * pitch) + (x * srcBPP);
            pixBlue     = buffer[i];
            buffer[i]   = buffer[i+2];
            buffer[i+2] = pixBlue;
        }
    }

    return 0;
}
#endif

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
int HAL_GfxDev_Pxp_Blit(
    const gfx_dev_t *dev, const gfx_surface_t *pSrc, const gfx_surface_t *pDst, const gfx_rotate_config_t *pRotate, mpp_flip_mode_t flip)
{
    int error                                       = 0;
    pxp_ps_buffer_config_t *pPsBufferConfig         = &s_GfxPxpHandle.psBufferConfig;
    pxp_output_buffer_config_t *pOutputBufferConfig = &s_GfxPxpHandle.outputBufferConfig;
    gfx_surface_t src, dst;
    gfx_rotate_config_t rotate;

    HAL_LOGD("Input buffer addr=0x%x\n", (unsigned int)pSrc->buf);
    HAL_LOGD("Output buffer addr=0x%x\n", (unsigned int)pDst->buf);

    if ( (pSrc->buf == NULL) || (pDst->buf == NULL) )
    {
        HAL_LOGE("PXP accessing buffer at address NULL\n");
        return -1;
    }

    memcpy(&src, pSrc, sizeof(gfx_surface_t));
    memcpy(&dst, pDst, sizeof(gfx_surface_t));
    memcpy(&rotate, pRotate, sizeof(gfx_rotate_config_t));

    if (dst.format == MPP_PIXEL_YUV420P)
    {
        // tmp swap back for testing
        if ((pRotate->target == kGFXRotate_SRCSurface) &&
            ((pRotate->degree == ROTATE_90) || (pRotate->degree == ROTATE_270)))
        {
            rotate.target = kGFXRotate_DSTSurface;
            //
            // need to swap the width and height as we force the rotate on DST surface
            // src
            src.height = pSrc->width;
            src.width  = pSrc->height;
            src.left   = pSrc->top;
            src.top    = pSrc->left;
            src.right  = pSrc->bottom;
            src.bottom = pSrc->right;
        }

        error = HAL_GfxDev_Pxp_YUYV1P422ToYUV420P(&src, &dst);
        return error;
    }

    // WR for PXP limitation: PXP can not do PS rotate and scale at the same time
    // which would cause several vertical garbage lines on the left
    // will do the rotate on the DST surface
    if (((src.height != dst.width) || (src.width != dst.height)) &&
        ((pRotate->degree == ROTATE_90) || (pRotate->degree == ROTATE_270)))
    {

        if (pRotate->target == kGFXRotate_SRCSurface)
        {
            rotate.target = kGFXRotate_DSTSurface;
        }
    }

    if ((pRotate->target == kGFXRotate_DSTSurface) &&
        ((pRotate->degree == ROTATE_90) || (pRotate->degree == ROTATE_270)))
    {
        // need to swap the width and height as we force the rotate on DST surface
        // dst
        dst.height = pDst->width;
        dst.width  = pDst->height;
    }

    error = _HAL_GfxDev_Pxp_Lock();
    if (error)
    {
        HAL_LOGE("PXP lock\n");
        return error;
    }

    if (((src.format == MPP_PIXEL_DEPTH16) && (dst.format == MPP_PIXEL_DEPTH16)) ||
        ((src.format == MPP_PIXEL_DEPTH8) && (dst.format == MPP_PIXEL_DEPTH8)))
    {
        if ((src.pitch == dst.pitch) && (src.height == dst.height))
        {
            memcpy(dst.buf, src.buf, dst.pitch * dst.height);
        }
        else
        {
            error = -1;
            HAL_LOGE("Depth frame resize is not currently supported\n");
        }
        _HAL_GfxDev_Pxp_Unlock();
        return error;
    }

    if (src.format == MPP_PIXEL_GRAY16)
    {
        if (dst.format == MPP_PIXEL_GRAY888X)
        {
            error = HAL_GfxDev_Pxp_Gray16_2_gray888x(&src, &dst);
            _HAL_GfxDev_Pxp_Unlock();
            return error;
        }
        else if (dst.format == MPP_PIXEL_GRAY16)
        {
            if ((src.pitch == dst.pitch) && (src.height == dst.height))
            {
                memcpy(dst.buf, src.buf, dst.pitch * dst.height);
            }
            else
            {
                error = -1;
                HAL_LOGE("GRAY16 frame resize is not currently supported\n");
            }
            _HAL_GfxDev_Pxp_Unlock();
            return error;
        }
        else
        {
            error = HAL_GfxDev_Pxp_BuildGray888XFromGray16(&src, NULL);
            if (error)
            {
                HAL_LOGE("Failed to build GRAY888X surface from GRAY16 image\n");
                _HAL_GfxDev_Pxp_Unlock();
                return error;
            }
        }
    }

    if (src.format == MPP_PIXEL_DEPTH16 && (dst.format == MPP_PIXEL_RGB565))
    {
        error = HAL_GfxDev_Pxp_BuildGray888XFromDepth16(&src, NULL);
        if (error)
        {
            HAL_LOGE("Failed to build GRAY888X surface from GRAY16 image\n");
            _HAL_GfxDev_Pxp_Unlock();
            return error;
        }
    }

    // disable the AS
    PXP_SetAlphaSurfacePosition(PXP_DEV, 0xFFFFU, 0xFFFFU, 0U, 0U);

    // setup input buffer configuration
    if (init_input_buffer(pPsBufferConfig, &src) == -1)
    {
        _HAL_GfxDev_Pxp_Unlock();
        return -1;
    }

    // setup flip/rotation/position/scaler configuration
    init_surface(flip, &rotate, &src, &dst);

    // setup the output buffer configuration
    if (init_output_buffer(pOutputBufferConfig, &dst) == -1)
    {
        _HAL_GfxDev_Pxp_Unlock();
        return -1;
    }

    if (pPsBufferConfig->pixelFormat >= kPXP_PsPixelFormatYUV1P444 &&
        pOutputBufferConfig->pixelFormat <= kPXP_OutputPixelFormatRGB565)
    {
        PXP_SetCsc1Mode(PXP_DEV, kPXP_Csc1YCbCr2RGB);
        PXP_EnableCsc1(PXP_DEV, true);
    }
    else
    {
        PXP_EnableCsc1(PXP_DEV, false);
    }

    // start the pxp operation
    PXP_Start(PXP_DEV);

    hal_sema_take(s_GfxPxpHandle.semaphore, HAL_MAX_TIMEOUT);

    _HAL_GfxDev_Pxp_Unlock();

#if (HAL_PXP_WORKAROUND_OUT_RGB == 1)
    /* Workaround for the PXP bug where BGR888 is output instead of RGB888 [MPP-97].
     * For the output format kPXP_OutputPixelFormatRGB888P, a software conversion
     * applies to invert blue and red pixels.
     */
    if (dst.format == MPP_PIXEL_RGB)
        convertCPU_BGR2RGB(dst.buf, dst.pitch, dst.width, dst.height);
#endif

    return error;
}

int HAL_GfxDev_Pxp_DrawRect(
    const gfx_dev_t *dev, gfx_surface_t *pOverlay, const int x, const int y, const int w, const int h, const int color)
{
    int error = 0;

    if ((dev == NULL) || (pOverlay == NULL))
    {
        return -1;
    }

    if (pOverlay->format != MPP_PIXEL_RGB565)
    {
        HAL_LOGE("MPP_PIXEL_RGB565 is currently the only supported overlay surface\n");
        return -1;
    }

    uint32_t startOdd       = 0;
    uint32_t endOdd         = 0;
    uint32_t m              = MIN((x + w), pOverlay->width);
    uint32_t n              = MIN((y + h), pOverlay->height);
    uint32_t rgb565Width    = pOverlay->pitch / sizeof(uint16_t);
    uint16_t *pCanvasBuffer = (uint16_t *)pOverlay->buf;
    uint32_t color32        = ((color & 0xFFFF) << 16) | (color & 0xFFFF);
    uint16_t color16        = (color & 0xFFFF);

    startOdd = x & 1;
    endOdd   = m & 1;

    for (int32_t i = y; i < n; i++)
    {
        int32_t j = x;
        if (startOdd)
        {
            *(pCanvasBuffer + i * rgb565Width + j++) = color16;
        }

        for (; j < m; j += 2)
        {
            *(uint32_t *)(pCanvasBuffer + i * rgb565Width + j) = color32;
        }

        if (endOdd)
        {
            *(pCanvasBuffer + i * rgb565Width + j - 1) = color16;
        }
    }

    return error;
}

int HAL_GfxDev_Pxp_DrawPicture(const gfx_dev_t *dev,
                               gfx_surface_t *pOverlay,
                               const int x,
                               const int y,
                               const int w,
                               const int h,
                               const int alpha,
                               const char *pIcon)
{
    int error = 0;

    if ((dev == NULL) || (pOverlay == NULL))
    {
        return -1;
    }

    if (pOverlay->format != MPP_PIXEL_RGB565)
    {
        HAL_LOGE("MPP_PIXEL_RGB565 is currently the only supported overlay surface\n");
        return -1;
    }

    uint32_t h_end              = y + h;
    uint32_t w_end              = x + w;
    uint16_t *pCanvasBuffer     = (uint16_t *)pOverlay->buf;
    uint32_t rgb565Width        = pOverlay->pitch / sizeof(uint16_t);
    const uint16_t *pIconRgb565 = (uint16_t *)pIcon;

    if ((alpha & 0xFFFF) != 0xFFFF)
    {
        for (int i = y; i < h_end; i++)
        {
            for (int j = x; j < w_end; j++)
            {
                if (*pIconRgb565 <= alpha)
                {
                    *(pCanvasBuffer + i * rgb565Width + j) = *pIconRgb565++;
                }
                else
                {
                    pIconRgb565++;
                }
            }
        }
    }
    else /* in most cases, we can do optimization by 2 pixels per operation */
    {
        uint32_t start_odd = x & 1;
        uint32_t end_odd   = w_end & 1;

        for (int i = y; i < h_end; i++)
        {
            int j = x;
            if (start_odd)
            {
                *(pCanvasBuffer + i * rgb565Width + j++) = *pIconRgb565++;
            }

            for (; j < w_end; j += 2)
            {
                *(uint32_t *)(pCanvasBuffer + i * rgb565Width + j) = *(uint32_t *)pIconRgb565;
                pIconRgb565 += 2;
            }

            if (end_odd)
            {
                *(pCanvasBuffer + i * rgb565Width + j - 1) = *pIconRgb565++;
            }
        }
    }

    return error;
}

/*
 * @brief compose the source surface to the destination surface.
 *
 * @param *dev [in] Pointer to pxp device.
 * @param *pSrc [in] Pointer to source surface.
 * @param *pOverlay [in] Pointer to overlay surface.
 * @param *pDst [out] Pointer to destination surface.
 * @param *pRotate [in] Pointer to the rotation config.
 * @param flip [in] Flip mode.
 *
 * @returns 0 for the success.
 */

static int HAL_GfxDev_Pxp_ComposeInternal(const gfx_dev_t *dev,
                                          gfx_surface_t *pSrc,
                                          gfx_surface_t *pOverlay,
                                          gfx_surface_t *pDst,
                                          gfx_rotate_config_t *pRotate,
                                          mpp_flip_mode_t flip)
{
    int error                                       = 0;
    pxp_ps_buffer_config_t *pPsBufferConfig         = &s_GfxPxpHandle.psBufferConfig;
    pxp_as_buffer_config_t *pAsBufferConfig         = &s_GfxPxpHandle.asBufferConfig;
    pxp_as_blend_config_t *pAsBlendConfig           = &s_GfxPxpHandle.asBlendConfig;
    pxp_output_buffer_config_t *pOutputBufferConfig = &s_GfxPxpHandle.outputBufferConfig;

    if ((dev == NULL) || (pSrc == NULL) || (pOverlay == NULL) || (pDst == NULL))
    {
        return -1;
    }

    if (pOverlay->format != MPP_PIXEL_RGB565)
    {
        HAL_LOGE("MPP_PIXEL_RGB565 is currently the only supported overlay surface\n");
        return -1;
    }

    error = _HAL_GfxDev_Pxp_Lock();
    if (error)
    {
        HAL_LOGE("PXP lock\n");
        return error;
    }

    /* AS config. */
    pAsBufferConfig->pixelFormat = kPXP_AsPixelFormatRGB565;
    pAsBufferConfig->bufferAddr  = (uint32_t)pOverlay->buf;
    pAsBufferConfig->pitchBytes  = pOverlay->pitch;
    pAsBlendConfig->alpha        = 0xF0U; /* Don't care. */
    pAsBlendConfig->invertAlpha  = false; /* Don't care. */
    pAsBlendConfig->alphaMode    = kPXP_AlphaOverride;
    pAsBlendConfig->ropMode      = kPXP_RopMaskAs;
    PXP_SetAlphaSurfaceBufferConfig(PXP_DEV, pAsBufferConfig);
    PXP_SetAlphaSurfaceBlendConfig(PXP_DEV, pAsBlendConfig);
    PXP_SetAlphaSurfacePosition(PXP_DEV, pOverlay->left, pOverlay->top, pOverlay->right, pOverlay->bottom);
    PXP_SetAlphaSurfaceOverlayColorKey(PXP_DEV, 0x0, 0x0);
    PXP_EnableAlphaSurfaceOverlayColorKey(PXP_DEV, true);

    // setup input buffer configuration
    if (init_input_buffer(pPsBufferConfig, pSrc) == -1)
    {
        _HAL_GfxDev_Pxp_Unlock();
        return -1;
    }

    // setup flip/rotation/position/scaler configuration
    init_surface(flip, pRotate, pSrc, pDst);

    // setup the output buffer configuration
    if (init_output_buffer(pOutputBufferConfig, pDst) == -1)
    {
        _HAL_GfxDev_Pxp_Unlock();
        return -1;
    }

    if (pPsBufferConfig->pixelFormat >= kPXP_PsPixelFormatYUV1P444 &&
        pOutputBufferConfig->pixelFormat <= kPXP_OutputPixelFormatRGB565)
    {
        PXP_SetCsc1Mode(PXP_DEV, kPXP_Csc1YCbCr2RGB);
        PXP_EnableCsc1(PXP_DEV, true);
    }
    else
    {
        PXP_EnableCsc1(PXP_DEV, false);
    }

    // lock overlay surface to avoid conflict with ui drawing on overlay surface
    if (pOverlay->lock)
    {
        hal_sema_take(pOverlay->lock, HAL_MAX_TIMEOUT);
    }

    // start the pxp operation
    PXP_Start(PXP_DEV);

    hal_sema_take(s_GfxPxpHandle.semaphore, HAL_MAX_TIMEOUT);

    // unlock overlay surface to avoid conflict with ui drawing on overlay surface
    if (pOverlay->lock)
    {
        hal_sema_give(pOverlay->lock);
    }

    _HAL_GfxDev_Pxp_Unlock();

    return error;
}

static int HAL_GfxDev_Pxp_ComposeSurfaceAdaptForScaleAndPsRotate(const gfx_dev_t *dev,
                                                                 const gfx_surface_t *pSrc,
                                                                 const gfx_surface_t *pDst,
                                                                 gfx_surface_t *pSrc_adapt)
{
    int error = 0;

    // dynamic allocate temporary buffer once, not free until program ends
    static char *ptmp_buffer   = NULL;
    static int tmp_buffer_size = 0;
    if (ptmp_buffer == NULL)
    {
        tmp_buffer_size = pDst->pitch * pDst->height;
        ptmp_buffer     = hal_malloc(tmp_buffer_size);
    }
    else
    {
        if (tmp_buffer_size < (pDst->pitch * pDst->height))
        {
            hal_free(ptmp_buffer);
            tmp_buffer_size = pDst->pitch * pDst->height;
            ptmp_buffer     = hal_malloc(tmp_buffer_size);
        }
    }
    if (ptmp_buffer == NULL)
    {
        HAL_LOGE("Cannot allocate memory for temporary buffer\n");
        return -1;
    }
    // scale
    gfx_surface_t src_surface;
    src_surface.height   = pSrc->width;
    src_surface.width    = pSrc->height;
    src_surface.left     = pSrc->top;
    src_surface.top      = pSrc->left;
    src_surface.right    = pSrc->bottom;
    src_surface.bottom   = pSrc->right;
    src_surface.pitch    = pSrc->pitch;
    src_surface.format   = pSrc->format;
    src_surface.swapByte = pSrc->swapByte;
    src_surface.buf      = pSrc->buf;
    src_surface.lock     = NULL;

    pSrc_adapt->height   = pDst->width;
    pSrc_adapt->width    = pDst->height;
    pSrc_adapt->left     = pDst->top;
    pSrc_adapt->top      = pDst->left;
    pSrc_adapt->right    = pDst->bottom;
    pSrc_adapt->bottom   = pDst->right;
    pSrc_adapt->pitch    = pDst->pitch * pDst->height / pDst->width;
    pSrc_adapt->format   = pDst->format;
    pSrc_adapt->swapByte = 0;
    pSrc_adapt->buf      = ptmp_buffer;
    pSrc_adapt->lock     = NULL;

    gfx_rotate_config_t rotate;
    rotate.target = kGFXRotate_SRCSurface;
    rotate.degree = ROTATE_0;
    error         = HAL_GfxDev_Pxp_Blit(dev, &src_surface, pSrc_adapt, &rotate, FLIP_NONE);

    // prepare for rotate
    pSrc_adapt->height = pDst->height;
    pSrc_adapt->width  = pDst->width;
    pSrc_adapt->left   = pDst->left;
    pSrc_adapt->top    = pDst->top;
    pSrc_adapt->right  = pDst->right;
    pSrc_adapt->bottom = pDst->bottom;

    return error;
}

int HAL_GfxDev_Pxp_Compose(const gfx_dev_t *dev,
                           gfx_surface_t *pSrc,
                           gfx_surface_t *pOverlay,
                           gfx_surface_t *pDst,
                           gfx_rotate_config_t *pRotate,
                           mpp_flip_mode_t flip)
{
    int error = 0;
    gfx_surface_t src_adjust;

    // hotfix for silicon bug: PXP can not do PS rotate and scale at the same time
    // which would cause several vertical garbage lines on the left
    // split to two steps for this case: first scale and then rotate
    if ((((pSrc->bottom - pSrc->top + 1) != (pDst->bottom - pDst->top + 1)) ||
         ((pSrc->right - pSrc->left + 1) != (pDst->right - pDst->left + 1))) &&
        ((pRotate->degree == ROTATE_90) || (pRotate->degree == ROTATE_270)))
    {
        memcpy(&src_adjust, pSrc, sizeof(gfx_surface_t));

        if ((pRotate->target == kGFXRotate_DSTSurface) || (flip != FLIP_NONE))
        {
            HAL_LOGE("PXP:scale + dst rotate with overlay is currently unsupported\n");
            return -1;
        }
        error = HAL_GfxDev_Pxp_ComposeSurfaceAdaptForScaleAndPsRotate(dev, pSrc, pDst, &src_adjust);
        if (error != 0)
        {
            HAL_LOGE("HAL_GfxDev_Pxp_ComposeSurfaceAdaptForScaleAndPsRotate failed\n");
            return -1;
        }

        pSrc = &src_adjust;
    }

    /* handle the GRAY16 source surface as the PXP didn't support this format */
    if (pSrc->format == MPP_PIXEL_GRAY16)
    {
        error = HAL_GfxDev_Pxp_BuildGray888XFromGray16(pSrc, &pSrc);
        if (error)
        {
            HAL_LOGE("Failed to build GRAY888X surface from GRAY16 image\n");
            return error;
        }
    }

    if (pSrc->format == MPP_PIXEL_DEPTH16)
    {
        error = HAL_GfxDev_Pxp_BuildGray888XFromDepth16(pSrc, &pSrc);
        if (error)
        {
            HAL_LOGE("Failed to build GRAY888X surface from DEPTH16 image\n");
            return error;
        }
    }

    error = HAL_GfxDev_Pxp_ComposeInternal(dev, pSrc, pOverlay, pDst, pRotate, flip);

    return error;
}

const static gfx_dev_operator_t s_GfxDevPxpOps = {
    .init        = HAL_GfxDev_Pxp_Init,
    .deinit      = HAL_GfxDev_Pxp_Deinit,
    .blit        = HAL_GfxDev_Pxp_Blit,
    .drawRect    = HAL_GfxDev_Pxp_DrawRect,
    .drawPicture = HAL_GfxDev_Pxp_DrawPicture,
    .compose     = HAL_GfxDev_Pxp_Compose,
    .get_buf_desc = HAL_GfxDev_Pxp_Getbufdesc,
};

int HAL_GfxDev_PXP_Register(gfx_dev_t *dev)
{
    dev->id = 0;    /* TODO set unique id */
    dev->ops = &s_GfxDevPxpOps;

    return 0;
}
#else  /* (defined HAL_ENABLE_2D_IMGPROC) && (HAL_ENABLE_GFX_DEV_Pxp == 1) */
int HAL_GfxDev_PXP_Register(gfx_dev_t *dev)
{
    HAL_LOGE("PXP not enabled\n");
    return -1;
}
#endif /* (defined HAL_ENABLE_2D_IMGPROC) && (HAL_ENABLE_GFX_DEV_Pxp == 1) */
