/*
 * Copyright 2020-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
 * @brief display dev HAL driver implementation for lcdifv2 and rk055ahd091.
 */

#include "mpp_config.h"
#include "board_config.h"
#include "mpp_api_types.h"
#include "hal_debug.h"
#include "hal_display_dev.h"
#include "hal_utils.h"

#if (defined HAL_ENABLE_DISPLAY) && (HAL_ENABLE_DISPLAY_DEV_Lcdifv2Rk055ah == 1)
#include <FreeRTOS.h>
#include <queue.h>

#include "fsl_common.h"
#include "display_support.h"

#if (ENABLE_FB_CHEKSUM == 1)
#include "fsl_dcic.h"
#endif

#if defined(__cplusplus)
extern "C" {
#endif

int s_DisplayDev_Lcdif_rk055ahd091_register();

#if defined(__cplusplus)
}
#endif

/**** constants ****/

/* fixed values */
#define DISPLAY_NAME "Lcdifv2Rk055ah"
#define DISPLAY_DEV_Lcdifv2Rk055ah_HEIGHT 1280
#define DISPLAY_DEV_Lcdifv2Rk055ah_WIDTH 720
#define DISPLAY_DEV_Lcdifv2Rk055ah_LEFT 0
#define DISPLAY_DEV_Lcdifv2Rk055ah_TOP 0
#define DISPLAY_DEV_Lcdifv2Rk055ah_RIGHT 719
#define DISPLAY_DEV_Lcdifv2Rk055ah_BOTTOM 1279
#define DISPLAY_DEV_Lcdifv2Rk055ah_ROTATE ROTATE_0
/* configurable default values */
#define DISPLAY_DEV_Lcdifv2Rk055ah_FORMAT MPP_PIXEL_RGB565
#define DISPLAY_DEV_Lcdifv2Rk055ah_BUFFER_COUNT 1
#ifndef HAL_DISPLAY_MAX_BPP
#define HAL_DISPLAY_MAX_BPP 2   /* RGB565 assumed by default */
#elif ( (HAL_DISPLAY_MAX_BPP < 2) || (HAL_DISPLAY_MAX_BPP > 4) )
#error "HAL: DisplayDev: Lcdifv2Rk055ah: HAL_DISPLAY_MAX_BPP value not supported"
#endif
#define APP_DCIC DCIC1

/**** declarations ****/

hal_display_status_t HAL_DisplayDev_Lcdifv2Rk055ah_Init(
    display_dev_t *dev, mpp_display_params_t *config, mpp_callback_t callback, void *param);
hal_display_status_t HAL_DisplayDev_Lcdifv2Rk055ah_Deinit(const display_dev_t *dev);
hal_display_status_t HAL_DisplayDev_Lcdifv2Rk055ah_Start(display_dev_t *dev);
hal_display_status_t HAL_DisplayDev_Lcdifv2Rk055ah_Stop(display_dev_t *dev);
hal_display_status_t HAL_DisplayDev_Lcdifv2Rk055ah_Blit(const display_dev_t *dev, void *frame, int width, int height);
hal_display_status_t HAL_DisplayDev_Lcdifv2Rk055ah_Getbufdesc(const display_dev_t *dev, hw_buf_desc_t *in_buf, mpp_memory_policy_t *policy);

/**** static variables ****/

AT_NONCACHEABLE_SECTION_ALIGN(
    static uint8_t s_LcdBuffer[DISPLAY_DEV_Lcdifv2Rk055ah_BUFFER_COUNT][DISPLAY_DEV_Lcdifv2Rk055ah_HEIGHT][DISPLAY_DEV_Lcdifv2Rk055ah_WIDTH * HAL_DISPLAY_MAX_BPP],
    FRAME_BUFFER_ALIGN);

static volatile bool s_newFrameShown = false;
static dc_fb_info_t s_fbInfo;
static volatile uint8_t s_lcdActiveFbIdx;


const static display_dev_operator_t s_DisplayDev_LcdifOps = {
    .init        = HAL_DisplayDev_Lcdifv2Rk055ah_Init,
    .deinit      = HAL_DisplayDev_Lcdifv2Rk055ah_Deinit,
    .start       = HAL_DisplayDev_Lcdifv2Rk055ah_Start,
    .stop        = HAL_DisplayDev_Lcdifv2Rk055ah_Stop,
    .blit        = HAL_DisplayDev_Lcdifv2Rk055ah_Blit,
    .get_buf_desc    = HAL_DisplayDev_Lcdifv2Rk055ah_Getbufdesc,
};

static display_dev_t s_DisplayDev_Lcdif = {.id   = 0,
                                           .name = DISPLAY_NAME,
                                           .ops  = &s_DisplayDev_LcdifOps,
                                           .cap  = {.width       = DISPLAY_DEV_Lcdifv2Rk055ah_WIDTH,
                                                   .height      = DISPLAY_DEV_Lcdifv2Rk055ah_HEIGHT,
                                                   .pitch       = DISPLAY_DEV_Lcdifv2Rk055ah_WIDTH * HAL_DISPLAY_MAX_BPP,
                                                   .left        = DISPLAY_DEV_Lcdifv2Rk055ah_LEFT,
                                                   .top         = DISPLAY_DEV_Lcdifv2Rk055ah_TOP,
                                                   .right       = DISPLAY_DEV_Lcdifv2Rk055ah_RIGHT,
                                                   .bottom      = DISPLAY_DEV_Lcdifv2Rk055ah_BOTTOM,
                                                   .rotate      = DISPLAY_DEV_Lcdifv2Rk055ah_ROTATE,
                                                   .format      = DISPLAY_DEV_Lcdifv2Rk055ah_FORMAT,
                                                   .nbFrameBuffer = DISPLAY_DEV_Lcdifv2Rk055ah_BUFFER_COUNT,
                                                   .frameBuffers = NULL,
                                                   .callback    = NULL,
                                                   .user_data   = NULL}};

/**** definitions ****/

static void DISPLAY_BufferSwitchOffCallback(void *param, void *switchOffBuffer)
{
    s_newFrameShown = true;
    /* TODO allow buffer switching */
    s_lcdActiveFbIdx ^= 1;

#if (ENABLE_FB_CHEKSUM == 1)
    display_dev_private_capability_t *cap = param;
    uint32_t chksum = DCIC_GetRegionCalculatedCrc(APP_DCIC, 0);
    if (cap->callback != NULL) cap->callback(NULL, MPP_EVENT_INTERNAL_TEST_RESERVED, (void *) &chksum, cap->user_data);
#endif
}

static hal_display_status_t DISPLAY_InitDisplay(display_dev_private_capability_t *cap)
{
    status_t status;
    do {
        if (s_newFrameShown) {
            /*
             * already initialized but was stopped
             * re-enable layer is enough
             */
            break;
        }
        BOARD_PrepareDisplayController();

        status = g_dc.ops->init(&g_dc);

        if (kStatus_Success != status)
        {
            HAL_LOGE("Display initialization failed\n");
            return kStatus_HAL_DisplayError;
        }

        g_dc.ops->getLayerDefaultConfig(&g_dc, 0, &s_fbInfo);
        switch(cap->format) {
            case MPP_PIXEL_RGB565:
                s_fbInfo.pixelFormat = kVIDEO_PixelFormatRGB565;
                break;
            case MPP_PIXEL_ARGB:
                s_fbInfo.pixelFormat = kVIDEO_PixelFormatXRGB8888;
                break;
            default:
                HAL_LOGE("DISPLAY_InitDisplay: invalid pixel format parameter.\n");
                return kStatus_HAL_DisplayError;
        }
        s_fbInfo.width       = DISPLAY_DEV_Lcdifv2Rk055ah_WIDTH;
        s_fbInfo.height      = DISPLAY_DEV_Lcdifv2Rk055ah_HEIGHT;
        s_fbInfo.startX      = DISPLAY_DEV_Lcdifv2Rk055ah_LEFT;
        s_fbInfo.startY      = DISPLAY_DEV_Lcdifv2Rk055ah_TOP;
        s_fbInfo.strideBytes = cap->pitch;
        g_dc.ops->setLayerConfig(&g_dc, 0, &s_fbInfo);

        g_dc.ops->setCallback(&g_dc, 0, DISPLAY_BufferSwitchOffCallback, cap);

        s_lcdActiveFbIdx = 0;
        s_newFrameShown  = false;
        g_dc.ops->setFrameBuffer(&g_dc, 0, s_LcdBuffer[s_lcdActiveFbIdx]);

        /* For the DBI interface display, application must wait for the first
         * frame buffer sent to the panel.
         */
        if ((g_dc.ops->getProperty(&g_dc) & kDC_FB_ReserveFrameBuffer) == 0)
        {
            while (s_newFrameShown == false)
            {
            }
        }

        s_newFrameShown = true;
    } while (false);

    g_dc.ops->enableLayer(&g_dc, 0);
    return kStatus_HAL_DisplaySuccess;
}

static hal_display_status_t DISPLAY_DeInitDisplay(void)
{
    status_t status;

    status = g_dc.ops->disableLayer(&g_dc, 0);

    if (kStatus_Success != status)
    {
        HAL_LOGE("Display disableLayer failed\n");
        return kStatus_HAL_DisplayError;
    }

    return kStatus_HAL_DisplaySuccess;
}

hal_display_status_t HAL_DisplayDev_Lcdifv2Rk055ah_Init(
    display_dev_t *dev, mpp_display_params_t *config, mpp_callback_t callback, void *user_data)
{
    hal_display_status_t ret = kStatus_HAL_DisplaySuccess;
    HAL_LOGD("++HAL_DisplayDev_Lcdifv2Rk055ah_Init\n");

    memset(s_LcdBuffer, 0x0, sizeof(s_LcdBuffer));

    /* set default config */
    memcpy(&dev->cap, &s_DisplayDev_Lcdif.cap, sizeof(display_dev_private_capability_t) );

    /* check input pixel depth versus static config */
    if (get_bitpp(config->format)/8 > HAL_DISPLAY_MAX_BPP)
    {
        HAL_LOGE("Pixel depth higher than max defined in mpp_config.h.\n");
        return kStatus_HAL_DisplayError;
    }

    dev->cap.format = config->format;
    switch(config->format) {
    case MPP_PIXEL_RGB565:
        dev->cap.pitch = dev->cap.width * 2;
        break;
    case MPP_PIXEL_ARGB:
        dev->cap.pitch = dev->cap.width * 4;
        break;
    default:
        HAL_LOGE("HAL_DisplayDev_Lcdifv2Rk055ah_Init: invalid pixel format parameter.\n");
        return kStatus_HAL_DisplayError;
    }

    /* check that fixed resolution is not modified by user */
    if (((config->width != 0) && (config->width != DISPLAY_DEV_Lcdifv2Rk055ah_WIDTH))
            || ((config->height != 0) && (config->height != DISPLAY_DEV_Lcdifv2Rk055ah_HEIGHT))
            || ((config->pitch != 0) && (config->pitch != dev->cap.pitch)))
    {
        HAL_LOGE("HAL_DisplayDev_Lcdifv2Rk055ah_Init: invalid resolution parameter.\n");
        return kStatus_HAL_DisplayError;
    }

    /* check that fixed display area is not modified by user */
    if (((config->top != 0) && (config->top != DISPLAY_DEV_Lcdifv2Rk055ah_TOP))
            || ((config->left != 0) && (config->left != DISPLAY_DEV_Lcdifv2Rk055ah_LEFT))
            || ((config->right != 0) && (config->right != DISPLAY_DEV_Lcdifv2Rk055ah_RIGHT))
            || ((config->bottom != 0) && (config->bottom != DISPLAY_DEV_Lcdifv2Rk055ah_BOTTOM)))
    {
        HAL_LOGE("HAL_DisplayDev_Lcdifv2Rk055ah_Init: invalid area parameter.\n");
        return kStatus_HAL_DisplayError;
    }

    dev->cap.frameBuffers = (void **)s_LcdBuffer;
    dev->cap.callback = callback;
    dev->cap.user_data = user_data;

    HAL_LOGD("--HAL_DisplayDev_Lcdifv2Rk055ah_Init\n");
    return ret;
}

hal_display_status_t HAL_DisplayDev_Lcdifv2Rk055ah_Deinit(const display_dev_t *dev)
{
    hal_display_status_t ret = kStatus_HAL_DisplaySuccess;
    return ret;
}

hal_display_status_t HAL_DisplayDev_Lcdifv2Rk055ah_Start(display_dev_t *dev)
{
    hal_display_status_t ret = kStatus_HAL_DisplaySuccess;
    HAL_LOGD("++HAL_DisplayDev_Lcdifv2Rk055ah_Start\n");
    ret = DISPLAY_InitDisplay(&dev->cap);

#if (ENABLE_FB_CHEKSUM == 1)
    dcic_config_t dcicConfig;
    dcic_region_config_t dcicRegionConfig = {0};

    /*
     *  config->polarityFlags = kDCIC_VsyncActiveLow | kDCIC_HsyncActiveLow |
     *                          kDCIC_DataEnableActiveLow | kDCIC_DriveDataOnFallingClkEdge;
     *  config->enableExternalSignal = false;
     *  config->enableInterrupts = 0;
     */
    DCIC_GetDefaultConfig(&dcicConfig);

    dcicConfig.enableExternalSignal = false;
    dcicConfig.polarityFlags =
        kDCIC_VsyncActiveLow | kDCIC_HsyncActiveLow | kDCIC_DataEnableActiveHigh | kDCIC_DriveDataOnFallingClkEdge;

    DCIC_Init(APP_DCIC, &dcicConfig);

    /* Configure the region. */
    dcicRegionConfig.lock        = false;
    dcicRegionConfig.upperLeftX  = 0;
    dcicRegionConfig.upperLeftY  = 0;
    dcicRegionConfig.lowerRightX = DISPLAY_DEV_Lcdifv2Rk055ah_RIGHT;
    dcicRegionConfig.lowerRightY = DISPLAY_DEV_Lcdifv2Rk055ah_BOTTOM;
    dcicRegionConfig.refCrc      = 0;   /* unused */

    DCIC_EnableRegion(APP_DCIC, 0, &dcicRegionConfig);  /* region 0 */

    DCIC_Enable(APP_DCIC, true);
#endif

    HAL_LOGD("--HAL_DisplayDev_Lcdifv2Rk055ah_Start\n");
    return ret;
}

hal_display_status_t HAL_DisplayDev_Lcdifv2Rk055ah_Stop(display_dev_t *dev)
{
    hal_display_status_t ret = kStatus_HAL_DisplaySuccess;
    HAL_LOGD("++\n");
    ret = DISPLAY_DeInitDisplay();
    HAL_LOGD("--\n");
    return ret;
}

hal_display_status_t HAL_DisplayDev_Lcdifv2Rk055ah_Blit(const display_dev_t *dev, void *frame, int width, int height)
{
    hal_display_status_t ret = kStatus_HAL_DisplaySuccess;
    HAL_LOGD("++HAL_DisplayDev_Lcdifv2Rk055ah_Blit\n");
    g_dc.ops->setFrameBuffer(&g_dc, 0, frame);
    HAL_LOGD("--HAL_DisplayDev_Lcdifv2Rk055ah_Blit\n");
    return ret;
}

hal_display_status_t HAL_DisplayDev_Lcdifv2Rk055ah_Getbufdesc(const display_dev_t *dev, hw_buf_desc_t *in_buf, mpp_memory_policy_t *policy)
{
    hal_display_status_t ret = kStatus_HAL_DisplaySuccess;
    HAL_LOGD("++HAL_DisplayDev_Lcdifv2Rk055ah_GetInput\n");

    do
    {
        if ((in_buf == NULL) || (policy == NULL))
        {
            HAL_LOGE("\nNULL pointer to buffer descriptor\n");
            ret = kStatus_HAL_DisplayError;
            break;
        }
        /* set memory policy */
        *policy = HAL_MEM_ALLOC_INPUT;
        in_buf->alignment = FRAME_BUFFER_ALIGN;
        in_buf->cacheable = false;
        in_buf->stride = dev->cap.pitch;
        in_buf->addr = (unsigned char *) (s_LcdBuffer[s_lcdActiveFbIdx]);
    } while (false);

    HAL_LOGD("--HAL_DisplayDev_Lcdifv2Rk055ah_GetInput\n");
    return ret;
}

int HAL_DisplayDev_Lcdifv2Rk055a_setup(display_dev_t *dev)
{
    dev->ops = &s_DisplayDev_LcdifOps;

    return 0;
}
#else /* (defined HAL_ENABLE_DISPLAY) && (HAL_ENABLE_DISPLAY_DEV_Lcdifv2Rk055ah == 1) */
int HAL_DisplayDev_Lcdifv2Rk055a_setup(display_dev_t *dev)
{
    HAL_LOGE("Display Lcdifv2Rk055ah not enabled\n");
    return -1;
}
#endif /* (defined HAL_ENABLE_DISPLAY) && (HAL_ENABLE_DISPLAY_DEV_Lcdifv2Rk055ah == 1) */
