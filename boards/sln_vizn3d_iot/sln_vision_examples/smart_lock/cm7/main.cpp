/*
 * Copyright 2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/**
 * @brief   Application entry point.
 */

#include <stdio.h>
#include "board.h"

#include <FreeRTOS.h>
#include <task.h>

#include "board_define.h"

#include "fwk_message.h"
#include "fwk_display_manager.h"
#include "fwk_camera_manager.h"
#include "fwk_input_manager.h"
#include "fwk_output_manager.h"
#include "fwk_vision_algo_manager.h"
#include "fwk_log.h"

#include "hal_smart_lock_config.h"
#include "hal_vision_algo_oasis_lite.h"

/* Insert other definitions and declarations here. */
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

extern void BOARD_InitHardware(void);
int APP_RegisterHalDevices(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

void APP_BoardInit(void)
{
    BOARD_InitHardware();
}

int APP_InitFramework(void)
{
    int ret = 0;

    ret = HAL_LpmDev_Register();
    if (ret != 0)
    {
        LOGE("HAL_LpmDev_Register error %d", ret);
    }

    ret = HAL_FlashDev_Littlefs_Init();
    if (ret != 0)
    {
        LOGE("HAL_FlashDev_Littlefs_Init error %d", ret);
        return ret;
    }

    ret = HAL_OutputDev_SmartLockConfig_Init();
    if (ret != 0)
    {
        LOGE("HAL_OutputDev_SmartLockConfig_Init error %d", ret);
        return ret;
    }

    ret = FWK_Message_Init();
    if (ret != 0)
    {
        LOGE("FWK_Message_Init error %d", ret);
        return ret;
    }

    ret = FWK_CameraManager_Init();
    if (ret != 0)
    {
        LOGE("FWK_CameraManager_Init error %d", ret);
        return ret;
    }

    ret = FWK_DisplayManager_Init();
    if (ret != 0)
    {
        LOGE("FWK_DisplayManager_Init error %d", ret);
        return ret;
    }

    ret = FWK_VisionAlgoManager_Init();
    if (ret != 0)
    {
        LOGE("FWK_VisionAlgoManager_Init error %d", ret);
        return ret;
    }

    ret = FWK_OutputManager_Init();
    if (ret != 0)
    {
        LOGE("FWK_OutputManager_Init error %d", ret);
        return ret;
    }

    ret = FWK_InputManager_Init();
    if (ret != 0)
    {
        LOGE("FWK_InputManager_Init error %d", ret);
        return ret;
    }

    return ret;
}

int APP_StartFramework(void)
{
    int ret = 0;
    ret     = FWK_CameraManager_Start();
    if (ret != 0)
    {
        LOGE("FWK_CameraManager_Start error %d", ret);
        return ret;
    }

    ret = FWK_DisplayManager_Start();
    if (ret != 0)
    {
        LOGE("FWK_DisplayManager_Start error %d", ret);
        return ret;
    }

    ret = FWK_VisionAlgoManager_Start();
    if (ret != 0)
    {
        LOGE("FWK_VisionAlgoManager_Start error %d", ret);
        return ret;
    }

    ret = FWK_OutputManager_Start();
    if (ret != 0)
    {
        LOGE("FWK_OutputManager_Start error %d", ret);
        return ret;
    }

    ret = FWK_InputManager_Start();
    if (ret != 0)
    {
        LOGE("FWK_InputManager_Start error %d", ret);
        return ret;
    }

    return ret;
}

/*
 * @brief   Application entry point.
 */
int main(void)
{
    /* Init board hardware. */
    APP_BoardInit();
    LOGD("[MAIN]:Started");
    /* init the framework*/
    APP_InitFramework();

    /* register the hal devices*/
    APP_RegisterHalDevices();

    /* start the framework*/
    APP_StartFramework();

    // start
    vTaskStartScheduler();

    while (1)
    {
        LOGD("#");
    }

    return 0;
}
