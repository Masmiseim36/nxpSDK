/*
 * Copyright 2024-2025 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * @brief Ezh+Ov7670 camera module HAL camera driver implementation.
 */

#include "mpp_config.h"
#include "hal_camera_dev.h"
#include "hal_debug.h"

#if (defined HAL_ENABLE_CAMERA) && (HAL_ENABLE_CAMERA_DEV_EzhV_Ov7670 == 1)
#include <stdlib.h>

#include "board.h"
#include "camera_config.h"
#include "fsl_flexio_camera.h"
#include "ezhv_support.h"
#include "fsl_ezhv.h"
#include "ezhv_para.h"
#include "flexio_camera.h"

#include "hal.h"
#include "hal_utils.h"
#include "hal_os.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define CAMERA_NAME "EzhV_Ov7670"

#define CAMERA_DEV_ALIGN 128      /* alignment requirement */

static EZHV_Para_t *pstPara = NULL;
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

hal_camera_status_t HAL_CameraDev_EzhV_Ov7670_Init(
    camera_dev_t *dev, mpp_camera_params_t *config, camera_dev_callback_t callback, void *param)
{
    hal_camera_status_t ret = kStatus_HAL_CameraSuccess;
    HAL_LOGD("++HAL_CameraDev_EzhV_Ov7670_Init( param[%p]\n", param);
    
    if ((config->width == 640) && (config->height == 480))
    {
        /* keep void for future resolution to be supported */
    }
    else
    {
        HAL_LOGE("Camera resolution unsupported\n");
        return kStatus_HAL_CameraError;
    }

    if (config->format != MPP_PIXEL_RGB565)
    {
        HAL_LOGE("Camera format unsupported\n");
        return kStatus_HAL_CameraError;
    }

    if (config->stripe != false)
    {
        HAL_LOGE("Camera stripe unsupported\n");
        return kStatus_HAL_CameraError;
    }

    /* init FlexIO */
    CAMERA_Init();

    /* save config */
    dev->config.width = config->width;
    dev->config.height = config->height;
    dev->config.framerate = config->fps;
    dev->config.format = config->format;
    dev->config.stripe = config->stripe;
    dev->cap.callback = callback;
    dev->cap.param    = param;
    dev->config.pitch = config->width * 2;  /* RGB565 */
    dev->config.stripe_size = 0;
    strncpy(dev->name, CAMERA_NAME, HAL_DEVICE_NAME_MAX_LENGTH);

    HAL_LOGD("--HAL_CameraDev_EzhV_Ov7670_Init\n");
    return ret;
}

hal_camera_status_t HAL_CameraDev_EzhV_Ov7670_Getbufdesc(const camera_dev_t *dev, hw_buf_desc_t *out_buf, mpp_memory_policy_t *policy)
{
    hal_camera_status_t ret = kStatus_HAL_CameraSuccess;
    HAL_LOGD("++HAL_CameraDev_EzhV_Ov7670_Getbufdesc(out_buf=[%p])\n", out_buf);

    if ((out_buf == NULL) || (policy == NULL))
    {
        HAL_LOGE("\nNULL pointer to buffer descriptor\n");
        return kStatus_HAL_CameraError;
    }
    /* set memory policy */
    *policy = HAL_MEM_ALLOC_OUTPUT;
    out_buf->alignment = CAMERA_DEV_ALIGN;
    out_buf->cacheable = false;
    out_buf->stride = dev->config.pitch;
    out_buf->nb_lines = dev->config.height;
    out_buf->addr = (uint8_t *)g_dvpTransfer.queue[g_dvpTransfer.userIdx].pBuf;

    HAL_LOGD("--HAL_CameraDev_EzhV_Ov7670_Getbufdesc\n");
    return ret;
}

hal_camera_status_t HAL_CameraDev_EzhV_Ov7670_Deinit(camera_dev_t *dev)
{
    hal_camera_status_t ret = kStatus_HAL_CameraSuccess;
    status_t status = kStatus_Success;
    
    /* TODO */

    if (status != kStatus_Success) return kStatus_HAL_CameraError;

    return ret;
}

hal_camera_status_t HAL_CameraDev_EzhV_Ov7670_Start(const camera_dev_t *dev)
{
    hal_camera_status_t ret = kStatus_HAL_CameraSuccess;
    status_t status = kStatus_Success;
    HAL_LOGD("++HAL_CameraDev_EzhV_Ov7670_Start\n");

    /* boot EZHV */
    BOARD_EZHV_Init();

    pstPara = (EZHV_Para_t *)EZHV_GetParaAddr();
    pstPara->apiIdx = kEZHV_API_flexioDvp;
    g_stCamBuf = (CameraBuffer_t*)(&pstPara->paraAddr);
    g_stCamBuf->pBuf = g_dvpTransfer.queue[g_dvpTransfer.driverIdx].pBuf;
    g_stCamBuf->len = g_dvpTransfer.queue[g_dvpTransfer.driverIdx].len;

    EZHV_WakeUpEzhv(kEZHV_ARM2EZHV_MSI);

    if (status != kStatus_Success) return kStatus_HAL_CameraError;

    HAL_LOGD("--HAL_CameraDev_EzhV_Ov7670_Start\n");
    return ret;
}

hal_camera_status_t HAL_CameraDev_EzhV_Ov7670_Stop(const camera_dev_t *dev)
{
    hal_camera_status_t ret = kStatus_HAL_CameraSuccess;
    HAL_LOGD("++\nHAL_CameraDev_EzhV_Ov7670_Stop");

    /* TODO */

    HAL_LOGD("--\nHAL_CameraDev_EzhV_Ov7670_Stop");
    return ret;
}

hal_camera_status_t HAL_CameraDev_EzhV_Ov7670_Dequeue(const camera_dev_t *dev, void **data, int *stripe)
{
    hal_camera_status_t ret = kStatus_HAL_CameraSuccess;
    HAL_LOGD("++HAL_CameraDev_EzhV_Ov7670_Dequeue\n");

    if (g_newVideoFrame == 1)
    {
        g_newVideoFrame = 0;
        g_dvpTransfer.userIdx = (g_dvpTransfer.userIdx+1)%QUEUE_SIZE;
    }
    
    uint8_t *pSrcBuf = (uint8_t *)g_stCamBuf->pBuf;

    *data   = (void *)pSrcBuf;
    *stripe = 0;

    HAL_LOGD("--HAL_CameraDev_EzhV_Ov7670_Dequeue\n");
    return ret;
}

hal_camera_status_t HAL_CameraDev_EzhV_Ov7670_Enqueue(const camera_dev_t *dev, void *data)
{
    int error = 0;
    HAL_LOGD("++HAL_CameraDev_EzhV_Ov7670_Enqueue\n");
    /* nothing to do, see HAL_CameraDev_EzhV_Ov7670_Dequeue() */
    HAL_LOGD("--HAL_CameraDev_EzhV_Ov7670_Enqueue\n");
    return error;
}

const static camera_dev_operator_t camera_dev_ezh_ov7670_ops = {
    .init        = HAL_CameraDev_EzhV_Ov7670_Init,
    .deinit      = HAL_CameraDev_EzhV_Ov7670_Deinit,
    .start       = HAL_CameraDev_EzhV_Ov7670_Start,
    .stop        = HAL_CameraDev_EzhV_Ov7670_Stop,
    .enqueue     = HAL_CameraDev_EzhV_Ov7670_Enqueue,
    .dequeue     = HAL_CameraDev_EzhV_Ov7670_Dequeue,
    .get_buf_desc = HAL_CameraDev_EzhV_Ov7670_Getbufdesc,
};

int HAL_CameraDev_EzhV_Ov7670_setup(const char *name, camera_dev_t *dev)
{
    dev->ops = &camera_dev_ezh_ov7670_ops;

    return 0;
}
#else /* (defined HAL_ENABLE_CAMERA) && (HAL_ENABLE_CAMERA_DEV_EzhV_Ov7670 == 1) */
int HAL_CameraDev_EzhV_Ov7670_setup(const char *name, camera_dev_t *dev, _Bool defconfig)
{
    HAL_LOGE("Camera EzhV_Ov7670 not enabled\n");
    return -1;
}
#endif /* (defined HAL_ENABLE_CAMERA) && (HAL_ENABLE_CAMERA_DEV_EzhV_Ov7670 == 1) */
