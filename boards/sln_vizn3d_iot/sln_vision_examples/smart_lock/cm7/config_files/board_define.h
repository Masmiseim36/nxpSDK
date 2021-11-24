/*
 * Copyright 2020-2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
 * @brief board-related macro definitions. Please place each required definition here before compiling.
 */

#ifndef BOARD_DEFINE_H_
#define BOARD_DEFINE_H_

#include "fwk_common.h"
#include "hal_camera_dev.h"

#define AT_NONCACHEABLE_SECTION_ALIGN_DTC(var, alignbytes) \
    __attribute__((section("NonCacheable_DTC,\"aw\",%nobits @"))) var __attribute__((aligned(alignbytes)))

#define AT_CACHEABLE_SECTION_ALIGN_OCRAM(var, alignbytes) \
    __attribute__((section(".bss.$SRAM_OCRAM_CACHED,\"aw\",%nobits @"))) var __attribute__((aligned(alignbytes)))
#define AT_NONCACHEABLE_SECTION_ALIGN_OCRAM(var, alignbytes) \
    __attribute__((section(".bss.$SRAM_OCRAM_NCACHED,\"aw\",%nobits @"))) var __attribute__((aligned(alignbytes)))

#define APP_TASK_ID(n) (kFWKTaskID_APPStart + n)

/*----------------------------------------------------------------------------------------------
 * Put all RT117H/VIZN3D definitions here
 * ---------------------------------------------------------------------------------------------*/

//#define APP_FFI

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*
 * Camera definition
 */
#if defined(SMART_LOCK_3D)
/* 3D Camera connected to the MIPI2CSI bridge */
#define ENABLE_CSI_3DCAMERA  1
#define ENABLE_MIPI_3DCAMERA 0

/* FlexIO camera */
#define ENABLE_FLEXIO_CAMERA 1
#define ENABLE_CSI_CAMERA    0

#elif defined(SMART_LOCK_2D) || defined(SMART_ACCESS_2D)
#define ENABLE_CSI_3DCAMERA  0
#define ENABLE_MIPI_3DCAMERA 0
/* FlexIO and CSI camera */
#define ENABLE_FLEXIO_CAMERA 1
#define ENABLE_CSI_CAMERA    1
#else
#error "***Invalid demo APP definition! Supported definition [SMART_LOCK_3D | SMART_LOCK_2D | SMART_ACCESS_2D]***"
#endif

/*
 * Debug console definition
 */
#define DEBUG_CONSOLE_UART_INDEX 12

/*
 * Shell definition
 */
#define ENABLE_SHELL_USB  1
#define ENABLE_SHELL_UART 0

/*
 * Display over lpuart or usbcdc definition
 */
#define ENABLE_DISPLAY_OVER_LPUART 0
#define ENABLE_DISPLAY_OVER_USBCDC 0

/*
 * Panel definition
 */
/* RK024HH298 Panel connected to Parallel LCDIF */
#define ENABLE_LCDIF_RK024HH298 1
/* RK055AHD091 Panel connected to MIPI LCDIFV2 */
#define ENABLE_LCDIFV2_RK055AHD091 0

int HAL_GfxDev_Pxp_Register();
int HAL_DisplayDev_UsbUvc_Register();
int HAL_CameraDev_CsiObU1s_Register();
int HAL_CameraDev_MipiOrbbecU1s_Register();
int HAL_CameraDev_3DSim_Register();
int SLN_CameraDevRGBSimRegister();
int HAL_Dev_BleWuartQn9090_Register();
int HAL_LpmDev_Register();
int HAL_OutputDev_UiSmartlock_Register();
int HAL_OutputDev_UiFfi_Register();
int HAL_OutputDev_Console_Register();
int HAL_OutputDev_RgbLed_Register();
int HAL_OutputDev_IrWhiteLeds_Register();
int HAL_InputDev_PushButtons_Register();

int HAL_OutputDev_SmartLockConfig_Register();

/* App task ID offset definition here. App task ID will start from kFWKTaskID_APPStart */
#define MQS_AUDIO_TASK_ID APP_TASK_ID(1)
int HAL_OutputDev_MqsAudio_Register();

// for vision_algo_oasis_lite2D device
// please include oasis2D/oasislite_runtime.h
// please link oasis2D/liboasis_lite2D_DEFAULT_117f_ae.a
int HAL_VisionAlgo_OasisLite2D_Register(int mode); // mode=0 smartlock; mode=1 ffi

// for vision_algo_oasis_lite3D device
// please include oasis3D/oasislite_runtime.h
// please link oasis3D/liboasis_lite3D_DEFAULT_117f_ae.a
int HAL_VisionAlgoDev_OasisLite3D_Register(int mode); // mode=0 smartlock; mode=1 ffi

/* 3D camera simulator */
#define ENABLE_3D_SIMCAMERA 0

#if ENABLE_3D_SIMCAMERA
int HAL_CameraDev_3DSim_Register();
#endif

int HAL_Dev_BleWuartQn9090_Register();

#if ENABLE_CSI_CAMERA
#define CAMERA_PIXEL_FORMAT_CSI_GC0308    kPixelFormat_UYVY1P422_Gray
#define CAMERA_WIDTH_CSI_GC0308           640
#define CAMERA_HEIGHT_CSI_GC0308          480
#define CAMERA_BYTES_PER_PIXEL_CSI_GC0308 2
#define CAMERA_BUFFER_COUNT_CSI_GC0308    4
#define CAMERA_ROTATION_CSI_GC0308        kCWRotateDegree_270
#define CAMERA_FLIP_CSI_GC0308            kFlipMode_None
#define CAMERA_SWAPBYTE_CSI_GC0308        1

int HAL_CameraDev_CsiGc0308_Register(camera_dev_static_config_t *format);
#endif

#if ENABLE_SHELL_USB
int HAL_InputDev_ShellUsb_Register();
#endif
#if ENABLE_SHELL_UART
int HAL_InputDev_ShellUart_Register();
#endif

#if ENABLE_FLEXIO_CAMERA
#define CAMERA_PIXEL_FORMAT_FLEXIO_GC0308    kPixelFormat_UYVY1P422_RGB
#define CAMERA_WIDTH_FLEXIO_GC0308           640
#define CAMERA_HEIGHT_FLEXIO_GC0308          480
#define CAMERA_BYTES_PER_PIXEL_FLEXIO_GC0308 2
#define CAMERA_BUFFER_COUNT_FLEXIO_GC0308    4
#define CAMERA_ROTATION_FLEXIO_GC0308        kCWRotateDegree_270

#define CAMERA_FLIP_FLEXIO_GC0308     kFlipMode_None
#define CAMERA_SWAPBYTE_FLEXIO_GC0308 1

int HAL_CameraDev_FlexioGc0308_Register(camera_dev_static_config_t *config);
#endif

#if ENABLE_LCDIF_RK024HH298
int HAL_DisplayDev_LcdifRk024hh298_Register();
#endif

#if ENABLE_LCDIFV2_RK055AHD091
int HAL_DisplayDev_Lcdifv2Rk055ahd091_Register();
#endif

#if ENABLE_DISPLAY_OVER_USBCDC
int HAL_DisplayDev_UsbCdc3D_Register();
int HAL_DisplayDev_UsbCdc2D_Register();
#endif

/*
 * The UI will dependent on the display resolution
 */
#if ENABLE_LCDIFV2_RK055AHD091
#define UI_BUFFER_WIDTH  480
#define UI_BUFFER_HEIGHT 640
#else
#define UI_BUFFER_WIDTH  240
#define UI_BUFFER_HEIGHT 320
#endif

int HAL_FlashDev_Littlefs_Init();

#if ENABLE_DISPLAY_OVER_LPUART
int HAL_DisplayDev_Lpuart3D_Register();
int HAL_DisplayDev_Lpuart2D_Register();
#endif

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif
