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
 * @brief camera dev mipi obU1S HAL driver implementation.
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
#include "fsl_mipi_csi2rx.h"
#include "sln_orbbecU1S.h"

#include "fwk_camera_manager.h"
#include "fwk_log.h"
#include "hal_camera_dev.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define CAMERA_NAME            "mipi_orbbecU1S"
#define CAMERA_DEV_WIDTH       OBU1S_SENSOR_WIDTH
#define CAMERA_DEV_HEIGHT      OBU1S_SENSOR_HEIGHT
#define CAMERA_BYTES_PER_PIXEL 2
/* CSI output frame buffer is RGB565. */
/* Need an even number of buffers to avoid frame loss*/
#define CAMERA_DEV_BUFFER_COUNT 4
#define CAMERA_DEV_BUFFER_ALIGN 64

#define CAMERA_DEV_FRAME_RATE    30
#define CAMERA_DEV_CONTROL_FLAGS (kCAMERA_HrefActiveHigh | kCAMERA_DataLatchOnRisingEdge)
#define CAMERA_MIPI_CSI_LANE     2

#define DEMO_CSI_CLK_FREQ          (CLOCK_GetFreqFromObs(CCM_OBS_BUS_CLK_ROOT))
#define DEMO_MIPI_CSI2_UI_CLK_FREQ (CLOCK_GetFreqFromObs(CCM_OBS_CSI2_UI_CLK_ROOT))

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */
status_t BOARD_PrepareDisplayController(void);
void BOARD_Pull3DCameraResetPin(bool pullUp);
void BOARD_Pull3DCameraPowerEnablePin(bool pullUp);
void BOARD_Pull3DCameraTestModePin(bool pullUp);
void BOARD_Pull3DCameraLDPIntPin(bool pullUp);
void BOARD_3DCameraI2CInit(void);
status_t BOARD_3DCameraI2CSend(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, const uint8_t *txBuff, uint8_t txBuffSize);
status_t BOARD_3DCameraI2CReceive(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, uint8_t *rxBuff, uint8_t rxBuffSize);
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
SDK_ALIGN(static uint8_t s_TempBuffer[CAMERA_DEV_HEIGHT][CAMERA_DEV_WIDTH * CAMERA_BYTES_PER_PIXEL],
          CAMERA_DEV_BUFFER_ALIGN);
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

static status_t BOARD_VerifyCameraClockSource(void)
{
    status_t status;
    uint32_t srcClkFreq;
    /*
     * The MIPI CSI clk_ui, clk_esc, and core_clk are all from
     * System PLL3 (PLL_480M). Verify the clock source to ensure
     * it is ready to use.
     */
    srcClkFreq = CLOCK_GetPllFreq(kCLOCK_PllSys3);

    if (480 != (srcClkFreq / 1000000))
    {
        status = kStatus_Fail;
    }
    else
    {
        status = kStatus_Success;
    }

    return status;
}

static void BOARD_Init3DMipiCsi(void)
{
    csi2rx_config_t csi2rxConfig = {0};

    /* This clock should be equal or faster than the receive byte clock,
     * D0_HS_BYTE_CLKD, from the RX DPHY. For this board, there are two
     * data lanes, the MIPI CSI pixel format is 16-bit per pixel, the
     * max resolution supported is 720*1280@30Hz, so the MIPI CSI2 clock
     * should be faster than 720*1280*30 = 27.6MHz, choose 60MHz here.
     */
    const clock_root_config_t csi2ClockConfig = {
        .clockOff = false,
        .mux      = 5,
        .div      = 5,
    };

    /* ESC clock should be in the range of 60~80 MHz */
    const clock_root_config_t csi2EscClockConfig = {
        .clockOff = false,
        .mux      = 5,
        .div      = 7,
    };

    /* UI clock should be equal or faster than the input pixel clock.
     * The camera max resolution supported is 720*1280@30Hz, so this clock
     * should be faster than 720*1280*30 = 27.6MHz, choose 60MHz here.
     */
    const clock_root_config_t csi2UiClockConfig = {
        .clockOff = false,
        .mux      = 5,
        .div      = 5,
    };

    if (kStatus_Success != BOARD_VerifyCameraClockSource())
    {
        LOGE("MIPI CSI clock source not valid");
    }

    /* MIPI CSI2 connect to CSI. */
    CLOCK_EnableClock(kCLOCK_Video_Mux);
    VIDEO_MUX->VID_MUX_CTRL.SET = (VIDEO_MUX_VID_MUX_CTRL_CSI_SEL_MASK);

    CLOCK_SetRootClock(kCLOCK_Root_Csi2, &csi2ClockConfig);
    CLOCK_SetRootClock(kCLOCK_Root_Csi2_Esc, &csi2EscClockConfig);
    CLOCK_SetRootClock(kCLOCK_Root_Csi2_Ui, &csi2UiClockConfig);

    /* The CSI clock should be faster than MIPI CSI2 clk_ui. The CSI clock
     * is bus clock.
     */
    if (DEMO_CSI_CLK_FREQ < DEMO_MIPI_CSI2_UI_CLK_FREQ)
    {
        LOGE("CSI clock should be faster than MIPI CSI2 ui clock.");
    }

    /*
     * Initialize the MIPI CSI2
     *
     * From D-PHY specification, the T-HSSETTLE should in the range of 85ns+6*UI
     * to 145ns+10*UI UI is Unit Interval, equal to the duration of any HS state
     * on the Clock Lane
     *
     * T-HSSETTLE = csi2rxConfig.tHsSettle_EscClk * (Tperiod of RxClkInEsc)
     *
     * csi2rxConfig.tHsSettle_EscClk setting for camera:
     *
     *    Resolution  |  frame rate  |  T_HS_SETTLE
     *  =============================================
     *     720P       |     30       |     0x12
     *  ---------------------------------------------
     *     720P       |     15       |     0x17
     *  ---------------------------------------------
     *      VGA       |     30       |     0x1F
     *  ---------------------------------------------
     *      VGA       |     15       |     0x24
     *  ---------------------------------------------
     *     QVGA       |     30       |     0x1F
     *  ---------------------------------------------
     *     QVGA       |     15       |     0x24
     *  ---------------------------------------------
     */
    static const uint32_t csi2rxHsSettle[][3] = {
        {
            kVIDEO_Resolution720P,
            30,
            0x12,
        },
        {
            kVIDEO_Resolution720P,
            15,
            0x17,
        },
        {
            kVIDEO_ResolutionVGA,
            30,
            0x1F,
        },
        {
            kVIDEO_ResolutionVGA,
            15,
            0x24,
        },
        {
            kVIDEO_ResolutionQVGA,
            30,
            0x1F,
        },
        {
            kVIDEO_ResolutionQVGA,
            15,
            0x24,
        },
    };

    csi2rxConfig.laneNum          = CAMERA_MIPI_CSI_LANE;
    csi2rxConfig.tHsSettle_EscClk = 0x09;

    for (uint8_t i = 0; i < ARRAY_SIZE(csi2rxHsSettle); i++)
    {
        if ((FSL_VIDEO_RESOLUTION(CAMERA_DEV_WIDTH, CAMERA_DEV_HEIGHT) == csi2rxHsSettle[i][0]) &&
            (csi2rxHsSettle[i][1] == CAMERA_DEV_FRAME_RATE))
        {
            csi2rxConfig.tHsSettle_EscClk = csi2rxHsSettle[i][2];
            break;
        }
    }

    CSI2RX_Init(MIPI_CSI2RX, &csi2rxConfig);
}

static void HAL_CameraDev_MipiOrbbecU1s_LibInit()
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

static hal_camera_status_t HAL_CameraDev_MipiOrbbecU1s_Init(
    camera_dev_t *dev, int width, int height, camera_dev_callback_t callback, void *param)
{
    hal_camera_status_t ret = kStatus_HAL_CameraSuccess;
    int error = 0;
    camera_config_t cameraConfig;
    LOGD("HAL_CameraDev_MipiOrbbecU1s_Init");

    HAL_CameraDev_MipiOrbbecU1s_LibInit();
    BOARD_3DCameraI2CInit();

    dev->config.width  = width;
    dev->config.height = height;
    dev->cap.callback  = callback;
    dev->cap.param     = param;

    // init csi receiver
    memset(&cameraConfig, 0, sizeof(cameraConfig));
    /*TODO
     * RTVZ-160 temporary fix
     */
    BOARD_PrepareDisplayController();

    /* CSI input data bus is 16-bit, and save as RGB565.. */
    cameraConfig.pixelFormat                = kVIDEO_PixelFormatRGB565;
    cameraConfig.bytesPerPixel              = CAMERA_BYTES_PER_PIXEL;
    cameraConfig.resolution                 = FSL_VIDEO_RESOLUTION(width, height);
    cameraConfig.frameBufferLinePitch_Bytes = width * CAMERA_BYTES_PER_PIXEL;
    cameraConfig.interface                  = kCAMERA_InterfaceGatedClock;
    cameraConfig.controlFlags               = CAMERA_DEV_CONTROL_FLAGS;
    cameraConfig.framePerSec                = 30;

    NVIC_SetPriority(CSI_IRQn, configMAX_SYSCALL_INTERRUPT_PRIORITY - 1);
    CAMERA_RECEIVER_Init(&s_CameraReceiver, &cameraConfig, _CameraReceiverCallback, dev);

    BOARD_Init3DMipiCsi();
    // init camera dev
    status_t status = CAMERA_DEVICE_Init(&s_ObU1SCameraDevice, &cameraConfig);

    if (status != kStatus_Success)
    {
        LOGE(" init ORBBEC error[%d]", status);
        ret = kStatus_HAL_CameraError;
    }

    for (int i = 0; i < CAMERA_DEV_BUFFER_COUNT; i++)
    {
        CAMERA_RECEIVER_SubmitEmptyBuffer(&s_CameraReceiver, (uint32_t)s_FrameBuffer[i]);
    }

    return ret;
}

static hal_camera_status_t HAL_CameraDev_MipiOrbbecU1s_Deinit(camera_dev_t *dev)
{
    hal_camera_status_t ret = kStatus_HAL_CameraSuccess;
    CAMERA_DEVICE_Stop(&s_ObU1SCameraDevice);
    CAMERA_DEVICE_Deinit(&s_ObU1SCameraDevice);
    return ret;
}

static hal_camera_status_t HAL_CameraDev_MipiOrbbec_U1s_Start(const camera_dev_t *dev)
{
    hal_camera_status_t ret = kStatus_HAL_CameraSuccess;
    LOGD("HAL_CameraDev_MipiOrbbec_U1s_Start");
    CAMERA_DEVICE_Start(&s_ObU1SCameraDevice);
    CAMERA_RECEIVER_Start(&s_CameraReceiver);

    return ret;
}

static hal_camera_status_t HAL_CameraDev_MipiOrbbecU1s_Enqueue(const camera_dev_t *dev, void *data)
{
    LOGI("++HAL_CameraDev_MipiOrbbecU1s_Enqueue");

    hal_camera_status_t ret = kStatus_HAL_CameraSuccess;

    if (s_pCurrentFrameBuffer != NULL)
    {
        LOGI("Submitting empty buffer");
        CAMERA_RECEIVER_SubmitEmptyBuffer(&s_CameraReceiver, (uint32_t)s_pCurrentFrameBuffer);
        s_pCurrentFrameBuffer = NULL;
    }

    LOGI("--HAL_CameraDev_MipiOrbbecU1s_Enqueue");
    return ret;
}

static void HAL_CameraDev_MipiOrbbecU1s_DataLeftShift2Bits(uint16_t *src, uint16_t *dst, int w, int h)
{
    uint32_t *src32 = (uint32_t *)src;
    uint32_t *dst32 = (uint32_t *)dst;
    w >>= 1;
    // left shift 2bits, and fill a11 to 1 from the second line.
    for (int j = 0; j < w; j += 2)
    {
        dst32[j]     = (src32[j] & 0xFFFF3FFFUL) << 2;
        dst32[j + 1] = (src32[j + 1] & 0xFFFF3FFFUL) << 2;
    }
    for (int i = 1; i < h; i++)
    {
        int base_idx = i * w;
        for (int j = 0; j < w; j += 2)
        {
            dst32[base_idx + j]     = ((src32[base_idx + j] & 0xFFFF3FFFUL) << 2) | 0x08000800;
            dst32[base_idx + j + 1] = ((src32[base_idx + j + 1] & 0xFFFF3FFFUL) << 2) | 0x08000800;
        }
    }
}

static hal_camera_status_t HAL_CameraDev_MipiOrbbecU1s_Dequeue(const camera_dev_t *dev, void **data, pixel_format_t *format)
{
    LOGI("++HAL_CameraDev_MipiOrbbecU1s_Dequeue");

    hal_camera_status_t ret = kStatus_HAL_CameraSuccess;

    while (kStatus_Success != CAMERA_RECEIVER_GetFullBuffer(&s_CameraReceiver, &s_pCurrentFrameBuffer))
    {
    }
    // shift data 0b' 0000 0000 a9:a2  to 0b' 0000 x0a9:a200
    HAL_CameraDev_MipiOrbbecU1s_DataLeftShift2Bits((uint16_t *)s_pCurrentFrameBuffer, (uint16_t *)s_TempBuffer,
                                                   CAMERA_DEV_WIDTH, CAMERA_DEV_HEIGHT);
    if (MODE_DEPTH == parseFirstLine((uint16_t *)s_TempBuffer))
    {
        OrbbecDevice_shift9_2ToDepth((uint16_t *)s_TempBuffer, (uint16_t *)s_DepthBuffer, CAMERA_DEV_WIDTH,
                                     CAMERA_DEV_HEIGHT);
        *data   = (void *)s_DepthBuffer;
        *format = kPixelFormat_Depth16;
        LOGI("3D_depth");
    }
    else
    {
        // a9:a2 as low byte for display.
        *data   = (void *)s_pCurrentFrameBuffer;
        *format = kPixelFormat_Gray16;
        LOGI("3D_IR");
    }

    LOGI("--HAL_CameraDev_MipiOrbbecU1s_Dequeue");
    return ret;
}

const static camera_dev_operator_t s_CameraDev_MipiObU1SOps = {
    .init        = HAL_CameraDev_MipiOrbbecU1s_Init,
    .deinit      = HAL_CameraDev_MipiOrbbecU1s_Deinit,
    .start       = HAL_CameraDev_MipiOrbbec_U1s_Start,
    .enqueue     = HAL_CameraDev_MipiOrbbecU1s_Enqueue,
    .dequeue     = HAL_CameraDev_MipiOrbbecU1s_Dequeue,
    .inputNotify = NULL,
};

static camera_dev_t s_CameraDev_MipiObU1S = {
    .id   = 0,
    .name = CAMERA_NAME,
    .ops  = &s_CameraDev_MipiObU1SOps,
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

int HAL_CameraDev_MipiOrbbecU1s_Register()
{
    int error = 0;
    LOGD("HAL_CameraDev_MipiOrbbecU1s_Register");
    error = FWK_CameraManager_DeviceRegister(&s_CameraDev_MipiObU1S);
    return error;
}
