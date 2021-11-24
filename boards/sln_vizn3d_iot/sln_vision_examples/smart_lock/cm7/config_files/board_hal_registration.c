/*
 * Copyright 2020-2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
 * @brief hal device registration for all needed here.
 */

#include <FreeRTOS.h>
#include <task.h>

#include "board_define.h"
#include "board.h"

#include "fwk_message.h"
#include "fwk_display_manager.h"
#include "fwk_camera_manager.h"
#include "fwk_input_manager.h"
#include "fwk_output_manager.h"
#include "fwk_vision_algo_manager.h"
#include "fwk_log.h"

#include "hal_smart_lock_config.h"
#include "hal_vision_algo_oasis_lite.h"

int APP_RegisterHalDevices(void)
{
    int ret = 0;
    ret     = HAL_GfxDev_Pxp_Register();
    if (ret != 0)
    {
        LOGE("HAL_GfxDev_Pxp_Register error %d", ret);
        return ret;
    }

    /*Use LPUART12 to capture, which is also for debug console on VIZN3 board. So need to disable debug first. */
#if ENABLE_DISPLAY_OVER_LPUART && (SDK_DEBUGCONSOLE != 1U)
#if ENABLE_CSI_3DCAMERA || ENABLE_MIPI_3DCAMERA || ENABLE_3D_SIMCAMERA
    ret = HAL_DisplayDev_Lpuart3D_Register();
#else
    ret = HAL_DisplayDev_Lpuart2D_Register();
#endif
    if (ret != 0)
    {
        LOGE("SLN_DisplayDevLpuartRegister error %d", ret);
        return ret;
    }
#elif ENABLE_DISPLAY_OVER_USBCDC
#if ENABLE_CSI_3DCAMERA || ENABLE_MIPI_3DCAMERA || ENABLE_3D_SIMCAMERA
    ret = HAL_DisplayDev_UsbCdc3D_Register();
#else
    ret = HAL_DisplayDev_UsbCdc2D_Register();
#endif
    if (ret != 0)
    {
        LOGE("SLN_DisplayDevUSBCDCRegister error %d", ret);
        return ret;
    }
#endif

#if HEADLESS_ENABLE
#else
    display_output_t defaultDisplayOutput = FWK_ConfigGetDisplayOutput();
    if ((defaultDisplayOutput >= kDisplayOutput_Panel) && (defaultDisplayOutput < kDisplayOutput_Invalid))
    {
        LOGD("[DisplayOutput]:%d", defaultDisplayOutput);
    }
    else
    {
        LOGE("Invalid display output %d, set to %d", defaultDisplayOutput, kDisplayOutput_Panel);
        defaultDisplayOutput = kDisplayOutput_Panel;
        FWK_ConfigSetDisplayOutput(defaultDisplayOutput);
    }

    if (defaultDisplayOutput == kDisplayOutput_Panel)
    {
#if ENABLE_LCDIF_RK024HH298
        ret = HAL_DisplayDev_LcdifRk024hh298_Register();

#elif ENABLE_LCDIFV2_RK055AHD091
        ret = HAL_DisplayDev_Lcdifv2Rk055ahd091_Register();
#endif

        if (ret != 0)
        {
            LOGE("Display panel register error %d", ret);
            return ret;
        }
    }
    else
    {
        ret = HAL_DisplayDev_UsbUvc_Register();
        if (ret != 0)
        {
            LOGE("HAL_DisplayDev_UsbUvc_Register error %d", ret);
            return ret;
        }
    }
#endif

#if ENABLE_3D_SIMCAMERA
    ret = HAL_CameraDev_3DSim_Register();
    if (ret != 0)
    {
        LOGE("HAL_CameraDev_3DSim_Register error %d", ret);
        return ret;
    }
#endif

#if ENABLE_FLEXIO_CAMERA
    camera_dev_static_config_t gc0308_static_config = {
        .height   = CAMERA_HEIGHT_FLEXIO_GC0308,
        .width    = CAMERA_WIDTH_FLEXIO_GC0308,
        .pitch    = CAMERA_WIDTH_FLEXIO_GC0308 * 2,
        .left     = 0,
        .top      = 0,
        .right    = CAMERA_WIDTH_FLEXIO_GC0308 - 1,
        .bottom   = CAMERA_HEIGHT_FLEXIO_GC0308 - 1,
        .rotate   = CAMERA_ROTATION_FLEXIO_GC0308,
        .flip     = CAMERA_FLIP_FLEXIO_GC0308,
        .swapByte = CAMERA_SWAPBYTE_FLEXIO_GC0308,
    };

    ret = HAL_CameraDev_FlexioGc0308_Register(&gc0308_static_config);
    if (ret != 0)
    {
        LOGE("HAL_CameraDev_FlexioGc0308_Register error %d", ret);
        return ret;
    }
#endif

#if ENABLE_CSI_CAMERA
    camera_dev_static_config_t csi_gc0308_static_config = {
        .height   = CAMERA_HEIGHT_CSI_GC0308,
        .width    = CAMERA_WIDTH_CSI_GC0308,
        .pitch    = CAMERA_WIDTH_CSI_GC0308 * 2,
        .left     = 0,
        .top      = 0,
        .right    = CAMERA_WIDTH_CSI_GC0308 - 1,
        .bottom   = CAMERA_HEIGHT_CSI_GC0308 - 1,
        .rotate   = CAMERA_ROTATION_CSI_GC0308,
        .flip     = CAMERA_FLIP_CSI_GC0308,
        .swapByte = CAMERA_SWAPBYTE_CSI_GC0308,
    };

    ret = HAL_CameraDev_CsiGc0308_Register(&csi_gc0308_static_config);
    if (ret != 0)
    {
        LOGE("HAL_CameraDev_CsiGc0308_Register error %d", ret);
        return ret;
    }

#endif

#if ENABLE_CSI_3DCAMERA
    ret = HAL_CameraDev_CsiObU1s_Register();
    if (ret != 0)
    {
        LOGE("HAL_CameraDev_CsiObU1s_Register error %d", ret);
        return ret;
    }
#endif

#if ENABLE_MIPI_3DCAMERA
    ret = HAL_CameraDev_MipiOrbbecU1s_Register();
    if (ret != 0)
    {
        LOGE("HAL_CameraDev_MipiOrbbecU1s_Register error %d", ret);
        return ret;
    }
#endif

#if ENABLE_CSI_3DCAMERA || ENABLE_MIPI_3DCAMERA || ENABLE_3D_SIMCAMERA
#if defined(APP_FFI)
    ret = HAL_VisionAlgoDev_OasisLite3D_Register(kOASISLiteMode_FFI);
#else // default APP_SMARTLOCK
    ret = HAL_VisionAlgoDev_OasisLite3D_Register(kOASISLiteMode_SmartLock);
#endif
#else
#if defined(APP_FFI)
    ret = HAL_VisionAlgo_OasisLite2D_Register(kOASISLiteMode_FFI);
#else // default APP_SMARTLOCK
    ret = HAL_VisionAlgo_OasisLite2D_Register(kOASISLiteMode_SmartLock);
#endif
#endif
    if (ret != 0)
    {
        LOGE("vision_algo_oasis_lite_register error %d", ret);
        return ret;
    }

    ret = HAL_InputDev_PushButtons_Register();

    if (ret != 0)
    {
        LOGE("HAL_InputDev_PushButtons_Register error %d", ret);
        return ret;
    }

#if defined(ENABLE_SHELL_USB) & ENABLE_SHELL_USB
    ret = HAL_InputDev_ShellUsb_Register();
    if (ret != 0)
    {
        LOGE("HAL_InputDev_ShellUsb_Register error %d", ret);
        return ret;
    }
#else
    ret = HAL_InputDev_ShellUart_Register();
    if (ret != 0)
    {
        LOGE("HAL_InputDev_ShellUart_Register error %d", ret);
        return ret;
    }
#endif

    ret = HAL_OutputDev_RgbLed_Register();
    if (ret != 0)
    {
        LOGE("HAL_OutputDev_RgbLed_Register error %d", ret);
        return ret;
    }

    ret = HAL_OutputDev_IrWhiteLeds_Register();
    if (ret != 0)
    {
        LOGE("HAL_OutputDev_IrWhiteLeds_Register error %d", ret);
        return ret;
    }

#if HEADLESS_ENABLE
#else
#if defined(APP_FFI)
    ret = HAL_OutputDev_UiFfi_Register();
#else // default APP_SMARTLOCK
    ret = HAL_OutputDev_UiSmartlock_Register();
#endif
    if (ret != 0)
    {
        LOGE("HAL_OutputDev_UiSmartlock_Register error %d", ret);
        return ret;
    }
#endif

    ret = HAL_OutputDev_SmartLockConfig_Register();
    if (ret != 0)
    {
        LOGE("HAL_OutputDev_SmartLockConfig_Register error %d", ret);
        return ret;
    }

    ret = HAL_OutputDev_MqsAudio_Register();
    if (ret != 0)
    {
        LOGE("HAL_OutputDev_MqsAudio_Register error %d", ret);
        return ret;
    }

    ret = HAL_Dev_BleWuartQn9090_Register();
    if (ret != 0)
    {
        LOGE("HAL_Dev_BleWuartQn9090_Register error %d", ret);
        return ret;
    }

    return ret;
}
