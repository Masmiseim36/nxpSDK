/*
 * Copyright 2019-2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
 * @brief PXP (Pixel Pipeline) HAL gfx driver implementation. Used for augmenting image data to convert between
 * different image formats and compose images w/ GUI overlays.
 */

#include <fonts/font.h>
#include "FreeRTOS.h"
#include "semphr.h"
#include "fsl_common.h"
#include "fsl_pxp.h"

#include "fwk_log.h"
#include "fwk_graphics.h"

#define PXP_DEV PXP

/*
 * Notice: enable the lock will introduce 1 ms time cost for each PXP operation
 */
#define ENABLE_PXP_LOCK 0

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
    SemaphoreHandle_t mutex; /*!< Mutex to lock the handle during a transfer */
#endif
    SemaphoreHandle_t semaphore; /*!< Semaphore to notify and unblock task when transfer ends */
} gfx_pxp_handle_t;

static gfx_pxp_handle_t s_GfxPxpHandle;
static gfx_surface_t s_SurfGray888x;

static int HAL_GfxDev_Pxp_ComposeSurfaceAdaptForScaleAndPsRotate(const gfx_surface_t *pSrc,
                                                                 const gfx_surface_t *pDst,
                                                                 gfx_surface_t *pSrc_adapt);

void PXP_IRQHandler(void)
{
    if (s_GfxPxpHandle.semaphore != NULL)
    {
        BaseType_t result;
        BaseType_t HigherPriorityTaskWoken = pdFALSE;
        PXP_ClearStatusFlags(PXP_DEV, kPXP_CompleteFlag);
        result = xSemaphoreGiveFromISR(s_GfxPxpHandle.semaphore, &HigherPriorityTaskWoken);

        if (result != pdFAIL)
        {
            portYIELD_FROM_ISR(HigherPriorityTaskWoken);
        }
    }
}

static int _HAL_GfxDev_Pxp_Lock()
{
    int error = 0;

    // TODO Temporary here
    HAL_GfxDev_Pxp_Init(NULL, NULL);

#if ENABLE_PXP_LOCK
    BaseType_t ret;

    if (s_GfxPxpHandle.mutex == NULL)
    {
        return -1;
    }

    ret = xSemaphoreTake(s_GfxPxpHandle.mutex, portMAX_DELAY);

    if (pdTRUE != ret)
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
    BaseType_t ret;

    if (s_GfxPxpHandle.mutex == NULL)
    {
        return -1;
    }

    ret = xSemaphoreGive(s_GfxPxpHandle.mutex);

    if (pdTRUE != ret)
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
        NVIC_SetPriority(PXP_IRQn, configMAX_SYSCALL_INTERRUPT_PRIORITY - 1);

        PXP_SetProcessSurfaceBackGroundColor(PXP_DEV, 0U);
        // disable the AS
        PXP_SetAlphaSurfacePosition(PXP_DEV, 0xFFFFU, 0xFFFFU, 0U, 0U);

#if ENABLE_PXP_LOCK
        s_GfxPxpHandle.mutex = xSemaphoreCreateMutex();

        if (s_GfxPxpHandle.mutex == NULL)
        {
            LOGE("Create PXP mutex");
            return error;
        }

#endif
        s_GfxPxpHandle.semaphore = xSemaphoreCreateBinary();

        if (s_GfxPxpHandle.semaphore == NULL)
        {
            LOGE("Create PXP semaphore");
            return error;
        }

        s_GfxPxpHandle.pxpInit = 1;
    }

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
        case kPixelFormat_YUV1P444_RGB:
        case kPixelFormat_YUV1P444_Gray:
        {
            pPsBufferConfig->pixelFormat = kPXP_PsPixelFormatYUV1P444;
        }
        break;

        case kPixelFormat_UYVY1P422_RGB:
        case kPixelFormat_UYVY1P422_Gray:
        {
            pPsBufferConfig->pixelFormat = kPXP_PsPixelFormatUYVY1P422;
        }
        break;

        case kPixelFormat_VYUY1P422:
        {
            pPsBufferConfig->pixelFormat = kPXP_PsPixelFormatVYUY1P422;
        }
        break;

        case kPixelFormat_Gray888X:
        {
            pPsBufferConfig->pixelFormat = kPXP_PsPixelFormatRGB888;
        }
        break;

        case kPixelFormat_RGB565:
        {
            pPsBufferConfig->pixelFormat = kPXP_PsPixelFormatRGB565;
        }
        break;

        default:
        {
            error = -1;
            LOGE("Unsupported source surface pixel format: %d", pSrc->format);
            return error;
        }
        break;
    }

    return error;
}

static int init_input_buffer(pxp_ps_buffer_config_t *pPsBufferConfig, gfx_surface_t *pSrc)
{
    int error = 0;

    error = set_input_buffer_format(pPsBufferConfig, pSrc);

    if (error == -1)
    {
        return error;
    }

    pPsBufferConfig->swapByte    = pSrc->swapByte;
    pPsBufferConfig->bufferAddrU = 0U;
    pPsBufferConfig->bufferAddrV = 0U;
    pPsBufferConfig->pitchBytes  = pSrc->pitch;
    pPsBufferConfig->bufferAddr  = (uint32_t)pSrc->buf;
    PXP_SetProcessSurfaceBackGroundColor(PXP_DEV, 0U);
    PXP_SetProcessSurfaceBufferConfig(PXP_DEV, pPsBufferConfig);

    return error;
}

static void set_surface_flip(flip_mode_t flip, pxp_flip_mode_t *flipMode)
{
    switch (flip)
    {
        case kFlipMode_Horizontal:
        {
            *flipMode = kPXP_FlipHorizontal;
        }
        break;

        case kFlipMode_Vertical:
        {
            *flipMode = kPXP_FlipVertical;
        }
        break;

        case kFlipMode_Both:
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
            case kCWRotateDegree_90:
            {
                *rotateDegree = kPXP_Rotate90;
            }
            break;

            case kCWRotateDegree_180:
            {
                *rotateDegree = kPXP_Rotate180;
            }
            break;

            case kCWRotateDegree_270:
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

static void init_surface(flip_mode_t flip, gfx_rotate_config_t *pRotate, gfx_surface_t *pSrc, gfx_surface_t *pDst)
{
    // setup flip/rotation/position/scaler configuration
    pxp_rotate_position_t rotatePosition = kPXP_RotateOutputBuffer;
    pxp_rotate_degree_t rotateDegree     = kPXP_Rotate0;
    pxp_flip_mode_t flipMode             = kPXP_FlipDisable;

    set_surface_flip(flip, &flipMode);

    set_surface_rotate(pRotate, &rotatePosition, &rotateDegree);

    PXP_SetRotateConfig(PXP_DEV, rotatePosition, rotateDegree, flipMode);
    PXP_SetProcessSurfacePosition(PXP_DEV, pDst->left, pDst->top, pDst->right, pDst->bottom);
    PXP_SetProcessSurfaceScaler(PXP_DEV, (pSrc->right - pSrc->left + 1), (pSrc->bottom - pSrc->top + 1),
                                (pDst->right - pDst->left + 1), (pDst->bottom - pDst->top + 1));
}

static int set_output_buffer_format(pxp_output_buffer_config_t *pOutputBufferConfig, gfx_surface_t *pDst)
{
    int error = 0;

    switch (pDst->format)
    {
        case kPixelFormat_BGR:
        {
            pOutputBufferConfig->pixelFormat = kPXP_OutputPixelFormatRGB888P;
        }
        break;

        case kPixelFormat_RGB565:
        {
            pOutputBufferConfig->pixelFormat = kPXP_OutputPixelFormatRGB565;
        }
        break;

        case kPixelFormat_YUV1P444_RGB:
        case kPixelFormat_YUV1P444_Gray:
        {
            pOutputBufferConfig->pixelFormat = kPXP_OutputPixelFormatYUV1P444;
        }
        break;

        case kPixelFormat_UYVY1P422_RGB:
        case kPixelFormat_UYVY1P422_Gray:
        {
            pOutputBufferConfig->pixelFormat = kPXP_OutputPixelFormatUYVY1P422;
        }
        break;

        case kPixelFormat_VYUY1P422:
        {
            pOutputBufferConfig->pixelFormat = kPXP_OutputPixelFormatVYUY1P422;
        }
        break;

        default:
        {
            error = -1;
            LOGE("Unsupported output surface pixel format: %d", pDst->format);
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

    if (pSrc->format == kPixelFormat_Gray16 && pDst->format == kPixelFormat_Gray888X)
    {
        /* use the cpu to do the gray16 to gray888x conversion */
        int srcBlit_w = pSrc->right - pSrc->left + 1;
        int srcBlit_h = pSrc->bottom - pSrc->top + 1;

        int dstBlit_w = pDst->right - pDst->left + 1;
        int dstBlit_h = pDst->bottom - pDst->top + 1;

        if (dstBlit_w > srcBlit_w || dstBlit_h > srcBlit_h)
        {
            LOGE("Unsupported blit");
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

    if ((s_SurfGray888x.height * s_SurfGray888x.pitch) < (pSrc->height * pSrc->width * 4))
    {
        /* need to re-allocate the buffer */
        if (s_SurfGray888x.buf != NULL)
        {
            SLN_FREE(s_SurfGray888x.buf);
        }

        s_SurfGray888x.width  = pSrc->width;
        s_SurfGray888x.height = pSrc->height;
        s_SurfGray888x.pitch  = pSrc->width * 4;
        s_SurfGray888x.format = kPixelFormat_Gray888X;

        s_SurfGray888x.buf = SLN_MALLOC(s_SurfGray888x.height * s_SurfGray888x.pitch);
        if (s_SurfGray888x.buf == NULL)
        {
            LOGE("SLN_MALLOC");
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
        LOGE("Failed to convert GRAY16 image to GRAY888X");
        return error;
    }

    *ppDst = &s_SurfGray888x;

    return 0;
}

static int HAL_GfxDev_Pxp_Depth16_2_gray888x(gfx_surface_t *pSrc, gfx_surface_t *pDst)
{
    int error = -1;

    if (pSrc->format == kPixelFormat_Depth16 && pDst->format == kPixelFormat_Gray888X)
    {
        /* use the cpu to do the gray16 to gray888x conversion */
        int srcBlit_w = pSrc->right - pSrc->left + 1;
        int srcBlit_h = pSrc->bottom - pSrc->top + 1;

        int dstBlit_w = pDst->right - pDst->left + 1;
        int dstBlit_h = pDst->bottom - pDst->top + 1;

        if (dstBlit_w > srcBlit_w || dstBlit_h > srcBlit_h)
        {
            LOGE("Unsupported blit");
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

static int HAL_GfxDev_Pxp_BuildGray888XFromDepth16(gfx_surface_t *pSrc, gfx_surface_t **ppDst)
{
    int error = -1;

    if ((s_SurfGray888x.height * s_SurfGray888x.pitch) < (pSrc->height * pSrc->width * 4))
    {
        /* need to re-allocate the buffer */
        if (s_SurfGray888x.buf != NULL)
        {
            SLN_FREE(s_SurfGray888x.buf);
        }

        s_SurfGray888x.width  = pSrc->width;
        s_SurfGray888x.height = pSrc->height;
        s_SurfGray888x.pitch  = pSrc->width * 4;
        s_SurfGray888x.format = kPixelFormat_Gray888X;

        s_SurfGray888x.buf = SLN_MALLOC(s_SurfGray888x.height * s_SurfGray888x.pitch);
        if (s_SurfGray888x.buf == NULL)
        {
            LOGE("SLN_MALLOC");
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
        LOGE("Failed to convert DEPTH16 image to GRAY888X");
        return error;
    }

    *ppDst = &s_SurfGray888x;

    return 0;
}

/*
 * @brief blit the source surface to the destination surface.
 *
 * @param *dev [in] Pointer to pxp device.
 * @param *pSrc [in] Pointer to source surface.
 * @param *pDst [out] Pointer to destination surface.
 * @param *pRotate [in] Pointer to the rotation config.
 * @param flip [in] Flip mode.
 *
 * @returns 0 for the success.
 */
int HAL_GfxDev_Pxp_Blit(
    const gfx_dev_t *dev, gfx_surface_t *pSrc, gfx_surface_t *pDst, gfx_rotate_config_t *pRotate, flip_mode_t flip)
{
    int error                                       = 0;
    pxp_ps_buffer_config_t *pPsBufferConfig         = &s_GfxPxpHandle.psBufferConfig;
    pxp_output_buffer_config_t *pOutputBufferConfig = &s_GfxPxpHandle.outputBufferConfig;

    // WR for PXP limitation: PXP can not do PS rotate and scale at the same time
    // which would cause several vertical garbage lines on the left
    // will do the rotate on the DST surface
    if (((pSrc->height != pDst->height) || (pSrc->width != pDst->width)) &&
        ((pRotate->degree == kCWRotateDegree_90) || (pRotate->degree == kCWRotateDegree_270)))
    {
        if ((pRotate->target == kGFXRotate_DSTSurface) || (flip != kFlipMode_None))
        {
            LOGE("PXP:scale + dst rotate with overlay is currently unsupported!!");
            return -1;
        }

        if (pRotate->target == kGFXRotate_SRCSurface)
        {
            pRotate->target = kGFXRotate_DSTSurface;
            //
            // need to swap the width and height as we force the rotate on DST surface
            // src
            int tmp      = pSrc->height;
            pSrc->height = pSrc->width;
            pSrc->width  = tmp;
            tmp          = pSrc->left;
            pSrc->left   = pSrc->top;
            pSrc->top    = tmp;
            tmp          = pSrc->right;
            pSrc->right  = pSrc->bottom;
            pSrc->bottom = tmp;

            // dst
            tmp          = pDst->height;
            pDst->height = pDst->width;
            pDst->width  = tmp;
            tmp          = pDst->left;
            pDst->left   = pDst->top;
            pDst->top    = tmp;
            tmp          = pDst->right;
            pDst->right  = pDst->bottom;
            pDst->bottom = tmp;
        }
    }

    error = _HAL_GfxDev_Pxp_Lock();
    if (error)
    {
        LOGE("PXP lock");
        return error;
    }

    if (((pSrc->format == kPixelFormat_Depth16) && (pDst->format == kPixelFormat_Depth16)) ||
        ((pSrc->format == kPixelFormat_Depth8) && (pDst->format == kPixelFormat_Depth8)))
    {
        if ((pSrc->pitch == pDst->pitch) && (pSrc->height == pDst->height))
        {
            memcpy(pDst->buf, pSrc->buf, pDst->pitch * pDst->height);
        }
        else
        {
            error = -1;
            LOGE("Depth frame resize is not currently supported");
        }
        _HAL_GfxDev_Pxp_Unlock();
        return error;
    }

    if (pSrc->format == kPixelFormat_Gray16)
    {
        if (pDst->format == kPixelFormat_Gray888X)
        {
            error = HAL_GfxDev_Pxp_Gray16_2_gray888x(pSrc, pDst);
            _HAL_GfxDev_Pxp_Unlock();
            return error;
        }
        else if (pDst->format == kPixelFormat_Gray16)
        {
            if ((pSrc->pitch == pDst->pitch) && (pSrc->height == pDst->height))
            {
                memcpy(pDst->buf, pSrc->buf, pDst->pitch * pDst->height);
            }
            else
            {
                error = -1;
                LOGE("GRAY16 frame resize is not currently supported");
            }
            _HAL_GfxDev_Pxp_Unlock();
            return error;
        }
        else
        {
            error = HAL_GfxDev_Pxp_BuildGray888XFromGray16(pSrc, &pSrc);
            if (error)
            {
                LOGE("Failed to build GRAY888X surface from GRAY16 image");
                _HAL_GfxDev_Pxp_Unlock();
                return error;
            }
        }
    }

    if (pSrc->format == kPixelFormat_Depth16 && (pDst->format == kPixelFormat_RGB565))
    {
        error = HAL_GfxDev_Pxp_BuildGray888XFromDepth16(pSrc, &pSrc);
        if (error)
        {
            LOGE("Failed to build GRAY888X surface from GRAY16 image");
            return error;
        }
    }

    // disable the AS
    PXP_SetAlphaSurfacePosition(PXP_DEV, 0xFFFFU, 0xFFFFU, 0U, 0U);

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

    // start the pxp operation
    PXP_Start(PXP_DEV);

    xSemaphoreTake(s_GfxPxpHandle.semaphore, portMAX_DELAY);

    _HAL_GfxDev_Pxp_Unlock();

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

    if (pOverlay->format != kPixelFormat_RGB565)
    {
        LOGE("PIXEL_RGB565 is currently the only supported overlay surface");
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

    if (pOverlay->format != kPixelFormat_RGB565)
    {
        LOGE("PIXEL_RGB565 is currently the only supported overlay surface");
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

int HAL_GfxDev_Pxp_DrawText(const gfx_dev_t *dev,
                            gfx_surface_t *pOverlay,
                            const int x,
                            const int y,
                            const int text_color,
                            const int bg_color,
                            const int type,
                            const char *pText)
{
    int error = 0;

    if ((dev == NULL) || (pOverlay == NULL))
    {
        return -1;
    }

    if (pOverlay->format != kPixelFormat_RGB565)
    {
        LOGE("PIXEL_RGB565 is currently the only supported overlay surface");
        return -1;
    }

    uint16_t *pCanvasBuffer = (uint16_t *)pOverlay->buf;
    int rgb565Width         = pOverlay->pitch / sizeof(uint16_t);
    put_string(x, y, (char *)pText, (unsigned short)(text_color & 0xFFFF), (unsigned short)(bg_color & 0xFFFF),
               (font_t)type, pCanvasBuffer, rgb565Width);

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
                                          flip_mode_t flip)
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

    if (pOverlay->format != kPixelFormat_RGB565)
    {
        LOGE("PIXEL_RGB565 is currently the only supported overlay surface");
        return -1;
    }

    error = _HAL_GfxDev_Pxp_Lock();
    if (error)
    {
        LOGE("PXP lock");
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
        xSemaphoreTake(pOverlay->lock, portMAX_DELAY);
    }

    // start the pxp operation
    PXP_Start(PXP_DEV);

    xSemaphoreTake(s_GfxPxpHandle.semaphore, portMAX_DELAY);

    // unlock overlay surface to avoid conflict with ui drawing on overlay surface
    if (pOverlay->lock)
    {
        xSemaphoreGive(pOverlay->lock);
    }

    _HAL_GfxDev_Pxp_Unlock();

    return error;
}

static int HAL_GfxDev_Pxp_ComposeSurfaceAdaptForScaleAndPsRotate(const gfx_surface_t *pSrc,
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
        ptmp_buffer     = SLN_MALLOC(tmp_buffer_size);
    }
    else
    {
        if (tmp_buffer_size < (pDst->pitch * pDst->height))
        {
            SLN_FREE(ptmp_buffer);
            tmp_buffer_size = pDst->pitch * pDst->height;
            ptmp_buffer     = SLN_MALLOC(tmp_buffer_size);
        }
    }
    if (ptmp_buffer == NULL)
    {
        LOGE("Cannot allocate memory for temporary buffer");
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
    rotate.degree = kCWRotateDegree_0;
    error         = gfx_blit(&src_surface, pSrc_adapt, &rotate, kFlipMode_None);

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
                           flip_mode_t flip)
{
    int error = 0;

    // hotfix for silicon bug: PXP can not do PS rotate and scale at the same time
    // which would cause several vertical garbage lines on the left
    // split to two steps for this case: first scale and then rotate
    if ((((pSrc->bottom - pSrc->top + 1) != (pDst->bottom - pDst->top + 1)) ||
         ((pSrc->right - pSrc->left + 1) != (pDst->right - pDst->left + 1))) &&
        ((pRotate->degree == kCWRotateDegree_90) || (pRotate->degree == kCWRotateDegree_270)))
    {
        gfx_surface_t src_adjust;

        memcpy(&src_adjust, pSrc, sizeof(gfx_surface_t));

        if ((pRotate->target == kGFXRotate_DSTSurface) || (flip != kFlipMode_None))
        {
            LOGE("PXP:scale + dst rotate with overlay is currently unsupported");
            return -1;
        }
        error = HAL_GfxDev_Pxp_ComposeSurfaceAdaptForScaleAndPsRotate(pSrc, pDst, &src_adjust);
        if (error != 0)
        {
            LOGE("HAL_GfxDev_Pxp_ComposeSurfaceAdaptForScaleAndPsRotate failed");
            return -1;
        }

        pSrc = &src_adjust;
    }

    /* handle the GRAY16 source surface as the PXP didn't support this format */
    if (pSrc->format == kPixelFormat_Gray16)
    {
        error = HAL_GfxDev_Pxp_BuildGray888XFromGray16(pSrc, &pSrc);
        if (error)
        {
            LOGE("Failed to build GRAY888X surface from GRAY16 image");
            return error;
        }
    }

    if (pSrc->format == kPixelFormat_Depth16)
    {
        error = HAL_GfxDev_Pxp_BuildGray888XFromDepth16(pSrc, &pSrc);
        if (error)
        {
            LOGE("Failed to build GRAY888X surface from DEPTH16 image");
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
    .drawText    = HAL_GfxDev_Pxp_DrawText,
    .compose     = HAL_GfxDev_Pxp_Compose,
};

static gfx_dev_t s_GfxDevPxp = {
    .id  = 0,
    .ops = &s_GfxDevPxpOps,
};

int HAL_GfxDev_Pxp_Register()
{
    int error = 0;
    LOGD("HAL_GfxDev_Pxp_Register");
    memset(&s_GfxPxpHandle, 0, sizeof(gfx_pxp_handle_t));
    error = gfx_dev_register(&s_GfxDevPxp);

    return error;
}
