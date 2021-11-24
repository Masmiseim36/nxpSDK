/*
 * Copyright 2020-2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in
 * accordance with the license terms that accompany it. By expressly accepting
 * such terms or by downloading, installing, activating and/or otherwise using
 * the software, you are agreeing that you have read, and that you agree to
 * comply with and are bound by, such license terms. If you do not agree to be
 * bound by the applicable license terms, then you may not retain, install,
 * activate or otherwise use the software.
 */

/*
 * @brief camera dev csi obU1S HAL driver implementation.
 */
#include <FreeRTOS.h>
#include <stdlib.h>
#include <task.h>

#include "board.h"
#include "fsl_camera.h"
#include "fsl_camera_device.h"
#include "fsl_camera_receiver.h"
#include "fsl_csi.h"
#include "fsl_csi_camera_adapter.h"

#include "ObShiftToDepth.h"
#include "depth.h"
#include "sln_orbbecU1S.h"
#include "sln_tc358748xbg.h"

#include "fwk_camera_manager.h"
#include "fwk_log.h"
#include "hal_camera_dev.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define CAMERA_NAME             "csi_OrbbecU1S"
#define CAMERA_DEV_WIDTH        OBU1S_SENSOR_WIDTH
#define CAMERA_DEV_HEIGHT       OBU1S_SENSOR_HEIGHT
#define CAMERA_BYTES_PER_PIXEL  2
#define CAMERA_DEV_BUFFER_COUNT 4
#define CAMERA_DEV_BUFFER_ALIGN 64

#define CAMERA_DEV_CONTROL_FLAGS (kCAMERA_VsyncActiveHigh | kCAMERA_HrefActiveHigh | kCAMERA_DataLatchOnRisingEdge)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */
void BOARD_Pull3DCameraResetPin(bool pullUp);
void BOARD_Pull3DCameraPowerEnablePin(bool pullUp);
void BOARD_Pull3DCameraTestModePin(bool pullUp);
void BOARD_Pull3DCameraLDPIntPin(bool pullUp);
void BOARD_pull_3d_camera_bridge_reset_pin(bool pullUp);
void BOARD_3DCameraI2CInit(void);
status_t BOARD_3DCameraI2CSend(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, const uint8_t *txBuff, uint8_t txBuffSize);
status_t BOARD_3DCameraI2CReceive(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, uint8_t *rxBuff, uint8_t rxBuffSize);
static void _CameraDevCSIDataClear4BitsRShift2Bits(uint16_t *src, uint16_t *dst, int w, int h);
#if defined(__cplusplus)
}
#endif /* __cplusplus */

/*******************************************************************************
 * Variables
 ******************************************************************************/
// AT_NONCACHEABLE_SECTION_ALIGN(
/*
 * Note: the RAW16 format needs the CPU to do the color space conversion for the display and algorithm, set
 * the frame buffer to cacheable to speed up the CPU access.
 */
SDK_ALIGN(
    static uint8_t s_FrameBuffer[CAMERA_DEV_BUFFER_COUNT][CAMERA_DEV_HEIGHT][CAMERA_DEV_WIDTH * CAMERA_BYTES_PER_PIXEL],
    CAMERA_DEV_BUFFER_ALIGN);
// AT_NONCACHEABLE_SECTION_ALIGN(
SDK_ALIGN(static uint8_t s_DepthBuffer[CAMERA_DEV_HEIGHT][CAMERA_DEV_WIDTH * CAMERA_BYTES_PER_PIXEL],
          CAMERA_DEV_BUFFER_ALIGN);

static uint32_t *s_pCurrentFrameBuffer = 0;

/* Camera connect to CSI. */
static csi_resource_t s_CSIResource = {
    .csiBase = CSI,
    .dataBus = kCSI_DataBus16Bit,
};

static csi_private_data_t s_CSIPrivateData;

static camera_receiver_handle_t s_CameraReceiver = {
    .resource    = &s_CSIResource,
    .ops         = &csi_ops,
    .privateData = &s_CSIPrivateData,
};

static obU1S_resource_t s_ObU1SResource = {
    .i2cSendFunc        = BOARD_3DCameraI2CSend,
    .i2cReceiveFunc     = BOARD_3DCameraI2CReceive,
    .pullResetPin       = BOARD_Pull3DCameraResetPin,
    .pullPowerEnablePin = BOARD_Pull3DCameraPowerEnablePin,
    .pullTestModePin    = BOARD_Pull3DCameraTestModePin,
    .pullLdpIntPin      = BOARD_Pull3DCameraLDPIntPin,
};

static camera_device_handle_t s_ObU1SCameraDevice = {
    .resource = &s_ObU1SResource,
    .ops      = &obU1S_ops,
};

static tc358748_resource_t s_tc358748Resource = {
    .i2cSendFunc        = BOARD_3DCameraI2CSend,
    .i2cReceiveFunc     = BOARD_3DCameraI2CReceive,
    .pullResetPin       = BOARD_pull_3d_camera_bridge_reset_pin,
    .pullPowerEnablePin = NULL,
};

/*******************************************************************************
 * Code
 ******************************************************************************/

static void _CameraReceiverCallback(camera_receiver_handle_t *handle, status_t status, void *userData)
{
    camera_dev_t *dev = (camera_dev_t *)userData;

    if (dev->cap.callback != NULL)
    {
        uint8_t fromISR = __get_IPSR();
        dev->cap.callback(dev, kCameraEvent_SendFrame, dev->cap.param, fromISR);
    }
}

static void HAL_CameraDev_CsiOrbbecU1s_LibInit()
{
    ShiftInitConfig config;
    config.fZeroPlaneDistance    = 50.0f;
    config.fZeroPlanePixelSize   = 0.046904f;
    config.fEmitterDCmosDistance = -2.5f;

    config.nDeviceMaxShiftValue = 4095;
    config.nDeviceMaxDepthValue = 4095;

    config.nConstShift      = 200;
    config.nPixelSizeFactor = 1;
    config.nParamCoeff      = 8;
    config.nShiftScale      = 10;

    config.nDepthMinCutOff = 250;
    config.nDepthMaxCutOff = 1500;

    config.w = CAMERA_DEV_WIDTH;
    config.h = CAMERA_DEV_HEIGHT;

    OrbbecDevice_init(config);
    OrbbecDevice_refreshWorldConversionCache(CAMERA_DEV_WIDTH, CAMERA_DEV_HEIGHT, 1);
}

static hal_camera_status_t _HAL_CameraDev_CsiOrbbecU1s_Init(camera_dev_t *dev)
{
    LOGD("HAL_CameraDev_CsiOrbbecU1s_Init");
    int error               = 0;
    hal_camera_status_t ret = kStatus_HAL_CameraSuccess;
    camera_config_t cameraConfig;

    CLOCK_EnableClock(kCLOCK_Video_Mux);
    /* CSI Sensor data is from Parallel CSI . */
    VIDEO_MUX->VID_MUX_CTRL.CLR = VIDEO_MUX_VID_MUX_CTRL_CSI_SEL_MASK;

    const clock_root_config_t csiClockConfig = {
        .clockOff = false,
        .mux      = kCLOCK_CSI_ClockRoot_MuxOsc24MOut,
        .div      = 1,
    };
    /* Configure CSI using kCLOCK_CSI_ClockRoot_MuxOsc24MOut */
    CLOCK_SetRootClock(kCLOCK_Root_Csi, &csiClockConfig);

    HAL_CameraDev_CsiOrbbecU1s_LibInit();
    BOARD_3DCameraI2CInit();

    // init csi receiver
    memset(&cameraConfig, 0, sizeof(cameraConfig));
    cameraConfig.pixelFormat                = kVIDEO_PixelFormatRGB565;
    cameraConfig.bytesPerPixel              = CAMERA_BYTES_PER_PIXEL;
    cameraConfig.resolution                 = FSL_VIDEO_RESOLUTION(dev->config.width, dev->config.height);
    cameraConfig.frameBufferLinePitch_Bytes = dev->config.width * CAMERA_BYTES_PER_PIXEL;
    cameraConfig.interface                  = kCAMERA_InterfaceGatedClock;
    cameraConfig.controlFlags               = CAMERA_DEV_CONTROL_FLAGS;
    cameraConfig.framePerSec                = 30;

    NVIC_SetPriority(CSI_IRQn, configMAX_SYSCALL_INTERRUPT_PRIORITY - 1);
    error = CAMERA_RECEIVER_Init(&s_CameraReceiver, &cameraConfig, _CameraReceiverCallback, dev);
    if (error)
    {
        LOGE("CAMERA_RECEIVER_Init");
        ret = kStatus_HAL_CameraError;
    }

    // init camera dev
    error = CAMERA_DEVICE_Init(&s_ObU1SCameraDevice, &cameraConfig);
    if (error)
    {
        LOGE("CAMERA_DEVICE_Init");
        ret = kStatus_HAL_CameraError;
    }

    // init camera bridge dev
    error = TC358748_Init(&s_tc358748Resource);
    if (error)
    {
        LOGE("TC358748_Init");
        ret = kStatus_HAL_CameraError;
    }

    for (int i = 0; i < CAMERA_DEV_BUFFER_COUNT; i++)
    {
        CAMERA_RECEIVER_SubmitEmptyBuffer(&s_CameraReceiver, (uint32_t)s_FrameBuffer[i]);
    }
    return ret;
}

static void _HAL_CameraDev_CsiOrbbecU1s_InitTask(void *pvParameters)
{
    camera_dev_t *dev       = (camera_dev_t *)pvParameters;
    hal_camera_status_t ret = _HAL_CameraDev_CsiOrbbecU1s_Init(dev);

    if (dev->cap.callback != NULL)
    {
        uint8_t fromISR = __get_IPSR();
        dev->cap.callback(dev, kCameraEvent_CameraDeviceInit, &ret, fromISR);
    }
    vTaskDelete(NULL);
}

static hal_camera_status_t HAL_CameraDev_CsiOrbbecU1s_Init(
    camera_dev_t *dev, int width, int height, camera_dev_callback_t callback, void *param)
{
    hal_camera_status_t ret = kStatus_HAL_CameraSuccess;
    UBaseType_t priority    = uxTaskPriorityGet(NULL);
    LOGD("_CameraDevCSIObU1SInit");

    dev->config.width  = width;
    dev->config.height = height;
    dev->cap.callback  = callback;
    dev->cap.param     = param;

#if FWK_SUPPORT_ASYNC_CAMERA_INIT
    if (xTaskCreate(_HAL_CameraDev_CsiOrbbecU1s_InitTask, "InitCSIorrbecTask", 512, dev, priority, NULL) != pdPASS)
    {
        LOGE("Init orrbec csi");
        ret = kStatus_HAL_CameraError;
    }
    else
    {
        ret = kStatus_HAL_CameraNonBlocking;
    }
#else
    ret = _HAL_CameraDev_CsiOrbbecU1s_Init(dev);
#endif
    return ret;
}

static hal_camera_status_t HAL_CameraDev_CsiOrbbecU1s_Deinit(camera_dev_t *dev)
{
    hal_camera_status_t ret = kStatus_HAL_CameraSuccess;
    CAMERA_DEVICE_Stop(&s_ObU1SCameraDevice);
    CAMERA_DEVICE_Deinit(&s_ObU1SCameraDevice);
    TC358748_DeInit();
    return ret;
}

static hal_camera_status_t HAL_CameraDev_CsiOrbbecU1s_Start(const camera_dev_t *dev)
{
    hal_camera_status_t ret = kStatus_HAL_CameraSuccess;
    LOGD("HAL_CameraDev_CsiOrbbecU1s_Start");
    CAMERA_DEVICE_Start(&s_ObU1SCameraDevice);
    CAMERA_RECEIVER_Start(&s_CameraReceiver);

    return ret;
}

static hal_camera_status_t HAL_CameraDev_CsiOrbbecU1s_Enqueue(const camera_dev_t *dev, void *data)
{
    LOGI("++HAL_CameraDev_CsiOrbbecU1s_Enqueue");

    hal_camera_status_t ret = kStatus_HAL_CameraSuccess;

    if (s_pCurrentFrameBuffer != NULL)
    {
        LOGI("Submitting empty buffer");
        CAMERA_RECEIVER_SubmitEmptyBuffer(&s_CameraReceiver, (uint32_t)s_pCurrentFrameBuffer);
        s_pCurrentFrameBuffer = NULL;
    }

    LOGI("--HAL_CameraDev_CsiOrbbecU1s_Enqueue");
    return ret;
}

/**
 * @brief Used for shifting the 16 bits of data coming from the camera to get only the relevant 12 bits of data we care
 * about 16b = 2b (don't care) 12b (useful information) 2b (don't care)
 * @param src
 * @param dst
 * @param w
 * @param h
 */
static void _CameraDevCSIDataClear4BitsRShift2Bits(uint16_t *src, uint16_t *dst, int w, int h)
{
    uint32_t *src32 = (uint32_t *)src;
    uint32_t *dst32 = (uint32_t *)dst;
    w >>= 1;
    // Clear 4 Bits(0:1,14:15) and right shift 2 bits with CSI[2:13] lines
    for (int i = 0; i < h; i++)
    {
        int base_idx = i * w;
        for (int j = 0; j < w; j += 2)
        {
            dst32[base_idx + j]     = (src32[base_idx + j] & 0x3FFC3FFCUL) >> 2;
            dst32[base_idx + j + 1] = (src32[base_idx + j + 1] & 0x3FFC3FFCUL) >> 2;
        }
    }
}

/**
 * @brief Used for shifting IR Raw12 data from 0b' 0000 a11:a0 to 0b' 0000 00a9:a2
 */
static void _CameraDevIRData_Shift9_2ToGray(uint16_t *src, uint16_t *dst, int w, int h)
{
    uint32_t *src32 = (uint32_t *)src;
    uint32_t *dst32 = (uint32_t *)dst;
    w >>= 1;
    for (int i = 0; i < h; i++)
    {
        int base_idx = i * w;
        for (int j = 0; j < w; j += 2)
        {
            dst32[base_idx + j]     = (src32[base_idx + j] >> 2) & 0x00FF00FFUL;
            dst32[base_idx + j + 1] = (src32[base_idx + j + 1] >> 2) & 0x00FF00FFUL;
        }
    }
}

/**
 * Convert CSI RAW 16 to GRAY 16 with valid low GRAY 8 bits at CSI RAW 16 bit11 - bit4
 *    1. CSI 12 lines from bit2 to bit13
 *    2. RAW16 Gray to GRAY 8bits: take bit2 to bit9
 * So, CSI RAW 16 to GRAY 8: take bit11 - bit4
 */
static void _CameraDevIRData_Shift11_4ToGray(uint16_t *src, uint16_t *dst, int w, int h)
{
    uint32_t *src32 = (uint32_t *)src;
    uint32_t *dst32 = (uint32_t *)dst;
#if 0
    w >>= 1;
    for (int i = 0; i < h; i++)
    {
        int base_idx = i * w;
        for (int j = 0; j < w; j += 2)
        {
            dst32[base_idx + j]     = (src32[base_idx + j] >> 4) & 0x00FF00FFUL;
            dst32[base_idx + j + 1] = (src32[base_idx + j + 1] >> 4) & 0x00FF00FFUL;
        }
    }
#else
    /*
     * Note: our current optimization has the assumtion that the buffer is 16 bytes aligned
     *    540 * 640 * 2
     */
    /* uint32_t in one chunk */
#define CHUNK_SIZE 4
    int chunks = (w * h * sizeof(uint16_t)) >> CHUNK_SIZE;

    for (int i = 0; i < chunks; i++)
    {
        dst32[0] = (src32[0] >> 4) & 0x00FF00FFUL;
        dst32[1] = (src32[1] >> 4) & 0x00FF00FFUL;
        dst32[2] = (src32[2] >> 4) & 0x00FF00FFUL;
        dst32[3] = (src32[3] >> 4) & 0x00FF00FFUL;
        dst32 += CHUNK_SIZE;
        src32 += CHUNK_SIZE;
    }

#endif
}

static int s_PostProcessed;

/*
 * Only do the minimum determination (data point and the format) of the frame in the dequeue 
 *
 * And split the post process(IR and Depth data processing) to postProcess as they will eat CPU which is
 * critical for the whole system. (Camera manager is running with the highest priority).
 *
*/
hal_camera_status_t HAL_CameraDev_CsiOrbbecU1s_Dequeue(const camera_dev_t *dev, void **data, pixel_format_t *format)
{
    LOGI("++HAL_CameraDev_CsiOrbbecU1s_Dequeue");

    hal_camera_status_t ret = kStatus_HAL_CameraSuccess;

    while (kStatus_Success != CAMERA_RECEIVER_GetFullBuffer(&s_CameraReceiver, (uint32_t *)&s_pCurrentFrameBuffer))
    {
    }

    *data = (void *)s_pCurrentFrameBuffer;
    if (MODE_DEPTH == parseshift13_2Line((uint16_t *)s_pCurrentFrameBuffer))
    {
        *format = kPixelFormat_Depth16;
    }
    else
    {
        *format = kPixelFormat_Gray16;
    }

    /* Reset the post process for this frame */
    s_PostProcessed = 0;

    LOGI("--HAL_CameraDev_CsiOrbbecU1s_Dequeue");
    return ret;
}

/*
 * Only do the fully process (IR or Depth post process) of the frame in the previously dequeue.
 *
 * Need to guarantee the postProcess only do once for the first call for one frame.
 *
*/
hal_camera_status_t HAL_CameraDev_CsiOrbbecU1s_PostProcess(const camera_dev_t *dev, void **data, pixel_format_t *format)
{
    LOGI("++HAL_CameraDev_CsiOrbbecU1s_PostProcess");

    hal_camera_status_t ret = kStatus_HAL_CameraSuccess;

    /* Only do the post process once for one frame */
    if (s_PostProcessed == 0)
    {
        if (*format == kPixelFormat_Depth16)
        {
            OrbbecDevice_shift13_2ToDepth((uint16_t *)(*data), (uint16_t *)s_DepthBuffer,
                                          CAMERA_DEV_WIDTH, CAMERA_DEV_HEIGHT);
            *data   = (void *)s_DepthBuffer;
        }
        else
        {
            _CameraDevIRData_Shift11_4ToGray((uint16_t *)(*data), (uint16_t *)(*data),
                                             CAMERA_DEV_WIDTH, CAMERA_DEV_HEIGHT);
        }

        /* Mark the post process for this frame to skip the duplicate of post process for the same frame */
        s_PostProcessed = 1;
    }

    LOGI("--HAL_CameraDev_CsiOrbbecU1s_PostProcess");
    return ret;
}

const static camera_dev_operator_t s_CameraDev_CsiOrbbecU1sOps = {
    .init        = HAL_CameraDev_CsiOrbbecU1s_Init,
    .deinit      = HAL_CameraDev_CsiOrbbecU1s_Deinit,
    .start       = HAL_CameraDev_CsiOrbbecU1s_Start,
    .enqueue     = HAL_CameraDev_CsiOrbbecU1s_Enqueue,
    .dequeue     = HAL_CameraDev_CsiOrbbecU1s_Dequeue,
    .postProcess = HAL_CameraDev_CsiOrbbecU1s_PostProcess,
    .inputNotify = NULL,
};

static camera_dev_t s_CameraDev_CsiOrbbecU1s = {
    .id   = 0,
    .name = CAMERA_NAME,
    .ops  = &s_CameraDev_CsiOrbbecU1sOps,
    .config =
        {
            .height   = CAMERA_DEV_HEIGHT,
            .width    = CAMERA_DEV_WIDTH,
            .pitch    = CAMERA_DEV_WIDTH * CAMERA_BYTES_PER_PIXEL,
            .left     = 0,
            .top      = 0,
            .right    = CAMERA_DEV_WIDTH - 1,
            .bottom   = CAMERA_DEV_HEIGHT - 1,
            .rotate   = kCWRotateDegree_0,
            .flip     = kFlipMode_None,
            .swapByte = 0,
        },
    .cap =
        {

            .callback = NULL,
            .param    = NULL,
        },
};

int HAL_CameraDev_CsiObU1s_Register()
{
    int error = 0;
    LOGD("HAL_CameraDev_CsiObU1s_Register");
    error = FWK_CameraManager_DeviceRegister(&s_CameraDev_CsiOrbbecU1s);
    return error;
}
