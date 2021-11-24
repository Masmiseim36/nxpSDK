/*
 * Copyright 2019-2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
 * @brief mipi_ov5640 camera module HAL camera driver implementation.
 */

#include <FreeRTOS.h>
#include <task.h>
#include <stdlib.h>

#include "board.h"
#include "fsl_camera.h"
#include "fsl_camera_receiver.h"
#include "fsl_camera_device.h"
#include "fsl_gpio.h"
#include "fsl_csi.h"
#include "fsl_csi_camera_adapter.h"
#include "fsl_ov5640.h"
#include "fsl_mipi_csi2rx.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "display_support.h"

#include "fwk_log.h"
#include "fwk_camera_manager.h"
#include "hal_camera_dev.h"

#if defined(__cplusplus)
extern "C" {
#endif

int HAL_CameraDev_MipiOv5640_Enqueue(const camera_dev_t *dev, void *data);
void HAL_CameraDev_MipiOv5640_ReceiverCallback(camera_receiver_handle_t *handle, status_t status, void *userData);
void BOARD_MIPICameraI2CInit(void);
status_t BOARD_MIPICameraI2CSend(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, const uint8_t *txBuff, uint8_t txBuffSize);
status_t BOARD_MIPICameraI2CReceive(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, uint8_t *rxBuff, uint8_t rxBuffSize);
#if defined(__cplusplus)
}
#endif

#define CAMERA_NAME             "mipi_ov5640"
#define CAMERA_PIXEL_FORMAT     kPixelFormat_YUV1P444_RGB
#define CAMERA_DEV_HEIGHT       480
#define CAMERA_DEV_WIDTH        640
#define CAMERA_DEV_BUFFER_COUNT 3
/* CSI output frame buffer is XYUV8888. */
#define CAMERA_DEV_BUFFER_BPP   4
#define CAMERA_DEV_BUFFER_ALIGN 64

AT_NONCACHEABLE_SECTION_ALIGN(
    static uint8_t s_cameraBuffer[CAMERA_DEV_BUFFER_COUNT][CAMERA_DEV_HEIGHT][CAMERA_DEV_WIDTH * CAMERA_DEV_BUFFER_BPP],
    CAMERA_DEV_BUFFER_ALIGN);

#define CAMERA_DEV_FRAME_RATE    30
#define CAMERA_DEV_CONTROL_FLAGS (kCAMERA_HrefActiveHigh | kCAMERA_DataLatchOnRisingEdge)
#define CAMERA_MIPI_CSI_LANE     2

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_CSI_CLK_FREQ          (CLOCK_GetFreqFromObs(CCM_OBS_BUS_CLK_ROOT))
#define DEMO_MIPI_CSI2_UI_CLK_FREQ (CLOCK_GetFreqFromObs(CCM_OBS_CSI2_UI_CLK_ROOT))

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void BOARD_PullCameraPowerDownPin(bool pullUp);
static void BOARD_PullCameraResetPin(bool pullUp);
static status_t BOARD_VerifyCameraClockSource(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* Camera connect to CSI. */
static csi_resource_t csiResource = {
    .csiBase = CSI,
    .dataBus = kCSI_DataBus24Bit,
};

static csi_private_data_t csiPrivateData;

camera_receiver_handle_t cameraReceiver = {
    .resource    = &csiResource,
    .ops         = &csi_ops,
    .privateData = &csiPrivateData,
};

static ov5640_resource_t ov5640Resource = {
    .i2cSendFunc      = BOARD_MIPICameraI2CSend,
    .i2cReceiveFunc   = BOARD_MIPICameraI2CReceive,
    .pullResetPin     = BOARD_PullCameraResetPin,
    .pullPowerDownPin = BOARD_PullCameraPowerDownPin,
};

camera_device_handle_t cameraDevice = {
    .resource = &ov5640Resource,
    .ops      = &ov5640_ops,
};

/*******************************************************************************
 * Code
 ******************************************************************************/
extern void CSI_DriverIRQHandler(void);

void CSI_IRQHandler(void)
{
    CSI_DriverIRQHandler();
    __DSB();
}

static void BOARD_PullCameraResetPin(bool pullUp)
{
    if (pullUp)
    {
        GPIO_PinWrite(BOARD_MIPI_CAMERA_RESET_GPIO, BOARD_MIPI_CAMERA_RESET_PIN, 1);
    }
    else
    {
        GPIO_PinWrite(BOARD_MIPI_CAMERA_RESET_GPIO, BOARD_MIPI_CAMERA_RESET_PIN, 0);
    }
}

static void BOARD_PullCameraPowerDownPin(bool pullUp)
{
    if (pullUp)
    {
        GPIO_PinWrite(BOARD_MIPI_CAMERA_PWDN_GPIO, BOARD_MIPI_CAMERA_PWDN_PIN, 1);
    }
    else
    {
        GPIO_PinWrite(BOARD_MIPI_CAMERA_PWDN_GPIO, BOARD_MIPI_CAMERA_PWDN_PIN, 0);
    }
}

void BOARD_EarlyInitCamera(void)
{
    /* If the camera I2C bus should be released by sending I2C sequence,
     * add the code here.
     */
}

void BOARD_InitCameraResource(void)
{
    BOARD_MIPICameraI2CInit();

    /* CSI MCLK is connect to dedicated 24M OSC, so don't need to configure it. */
}

void BOARD_InitMipiCsi(void)
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
        .div      = 7,
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
        .div      = 7,
    };

    if (kStatus_Success != BOARD_VerifyCameraClockSource())
    {
        PRINTF("MIPI CSI clock source not valid\r\n");

        while (1)
        {
        }
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
        PRINTF("CSI clock should be faster than MIPI CSI2 ui clock.\r\n");

        while (1)
        {
        }
    }

    /*
     * Initialize the MIPI CSI2
     *
     * From D-PHY specification, the T-HSSETTLE should in the range of 85ns+6*UI to 145ns+10*UI
     * UI is Unit Interval, equal to the duration of any HS state on the Clock Lane
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
    csi2rxConfig.tHsSettle_EscClk = 0x12;

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

hal_camera_status_t HAL_CameraDev_MipiOv5640_Init(
    camera_dev_t *dev, int width, int height, camera_dev_callback_t callback, void *param)
{
    hal_camera_status_t ret = kStatus_HAL_CameraSuccess;
    LOGD("++HAL_CameraDev_MipiOv5640_Init( param[%p]", param);

    dev->config.width  = width;
    dev->config.height = height;
    dev->config.pitch  = CAMERA_DEV_WIDTH * CAMERA_DEV_BUFFER_BPP;

    dev->cap.callback = callback;
    dev->cap.param    = param;

    camera_config_t cameraConfig;

    memset(&cameraConfig, 0, sizeof(cameraConfig));

    /*TODO
     * RTVZ-160 temporary fix
     */
    BOARD_PrepareDisplayController();

    BOARD_InitCameraResource();

    /* CSI input data bus is 24-bit, and save as XYUV8888.. */
    cameraConfig.pixelFormat                = kVIDEO_PixelFormatXYUV;
    cameraConfig.bytesPerPixel              = CAMERA_DEV_BUFFER_BPP;
    cameraConfig.resolution                 = FSL_VIDEO_RESOLUTION(CAMERA_DEV_WIDTH, CAMERA_DEV_HEIGHT);
    cameraConfig.frameBufferLinePitch_Bytes = CAMERA_DEV_WIDTH * CAMERA_DEV_BUFFER_BPP;
    cameraConfig.interface                  = kCAMERA_InterfaceGatedClock;
    cameraConfig.controlFlags               = CAMERA_DEV_CONTROL_FLAGS;
    cameraConfig.framePerSec                = CAMERA_DEV_FRAME_RATE;

    NVIC_SetPriority(CSI_IRQn, configMAX_SYSCALL_INTERRUPT_PRIORITY - 1);
    CAMERA_RECEIVER_Init(&cameraReceiver, &cameraConfig, HAL_CameraDev_MipiOv5640_ReceiverCallback, NULL);

    BOARD_InitMipiCsi();

    cameraConfig.pixelFormat   = kVIDEO_PixelFormatYUYV;
    cameraConfig.bytesPerPixel = 2;
    cameraConfig.resolution    = FSL_VIDEO_RESOLUTION(CAMERA_DEV_WIDTH, CAMERA_DEV_HEIGHT);
    cameraConfig.interface     = kCAMERA_InterfaceMIPI;
    cameraConfig.controlFlags  = CAMERA_DEV_CONTROL_FLAGS;
    cameraConfig.framePerSec   = CAMERA_DEV_FRAME_RATE;
    cameraConfig.csiLanes      = CAMERA_MIPI_CSI_LANE;
    CAMERA_DEVICE_Init(&cameraDevice, &cameraConfig);

    CAMERA_DEVICE_Start(&cameraDevice);

    /* Submit the empty frame buffers to buffer queue. */
    for (uint32_t i = 0; i < CAMERA_DEV_BUFFER_COUNT; i++)
    {
        CAMERA_RECEIVER_SubmitEmptyBuffer(&cameraReceiver, (uint32_t)(s_cameraBuffer[i]));
    }

    LOGD("--HAL_CameraDev_MipiOv5640_Init");
    return ret;
}

hal_camera_status_t HAL_CameraDev_MipiOv5640_Deinit(camera_dev_t *dev)
{
    hal_camera_status_t ret = kStatus_HAL_CameraSuccess;
    return ret;
}

hal_camera_status_t HAL_CameraDev_MipiOv5640_Start(const camera_dev_t *dev)
{
    hal_camera_status_t ret = kStatus_HAL_CameraSuccess;
    LOGD("++HAL_CameraDev_MipiOv5640_Start");
    CAMERA_RECEIVER_Start(&cameraReceiver);
    LOGD("--HAL_CameraDev_MipiOv5640_Start");
    return ret;
}

static uint32_t gCurrentBufferAddr = 0;

hal_camera_status_t HAL_CameraDev_MipiOv5640_Dequeue(const camera_dev_t *dev, void **data, pixel_format_t *format)
{
    hal_camera_status_t ret = kStatus_HAL_CameraSuccess;
    LOGI("++HAL_CameraDev_MipiOv5640_Dequeue");

    // get one frame
    while (kStatus_Success != CAMERA_RECEIVER_GetFullBuffer(&cameraReceiver, &gCurrentBufferAddr))
    {
    }

    *data   = (void *)gCurrentBufferAddr;
    *format = CAMERA_PIXEL_FORMAT;
    LOGI("--HAL_CameraDev_MipiOv5640_Dequeue");
    return ret;
}

const static camera_dev_operator_t camera_dev_mipi_ov5640_ops = {
    .init        = HAL_CameraDev_MipiOv5640_Init,
    .deinit      = HAL_CameraDev_MipiOv5640_Deinit,
    .start       = HAL_CameraDev_MipiOv5640_Start,
    .enqueue     = HAL_CameraDev_MipiOv5640_Enqueue,
    .dequeue     = HAL_CameraDev_MipiOv5640_Dequeue,
    .inputNotify = NULL,
};

static camera_dev_t camera_dev_mipi_ov5640 = {
    .id   = 1,
    .name = CAMERA_NAME,
    .ops  = &camera_dev_mipi_ov5640_ops,
    .config =
        {
            .height   = CAMERA_DEV_HEIGHT,
            .width    = CAMERA_DEV_WIDTH,
            .pitch    = CAMERA_DEV_WIDTH * CAMERA_DEV_BUFFER_BPP,
            .left     = 0,
            .top      = 0,
            .right    = CAMERA_DEV_WIDTH - 1,
            .bottom   = CAMERA_DEV_HEIGHT - 1,
            .rotate   = kCWRotateDegree_90,
            .flip     = kFlipMode_None,
            .swapByte = 0,
        },
    .cap = {.callback = NULL, .param = NULL},
};

void HAL_CameraDev_MipiOv5640_ReceiverCallback(camera_receiver_handle_t *handle, status_t status, void *userData)
{
    if (camera_dev_mipi_ov5640.cap.callback != NULL)
    {
        uint8_t fromISR = __get_IPSR();
        camera_dev_mipi_ov5640.cap.callback(&camera_dev_mipi_ov5640, kCameraEvent_SendFrame,
                                            camera_dev_mipi_ov5640.cap.param, fromISR);
    }
}

int HAL_CameraDev_MipiOv5640_Enqueue(const camera_dev_t *dev, void *data)
{
    int error = 0;
    LOGI("++HAL_CameraDev_MipiOv5640_Enqueue");

    if (gCurrentBufferAddr != 0)
    {
        LOGI("Submitting empty buffer");
        CAMERA_RECEIVER_SubmitEmptyBuffer(&cameraReceiver, (uint32_t)gCurrentBufferAddr);
        gCurrentBufferAddr = 0;
    }

    LOGI("--HAL_CameraDev_MipiOv5640_Enqueue");
    return error;
}

int HAL_CameraDev_MipiOv5640_Register()
{
    int error = 0;
    LOGD("camera_dev_mipi_ov5640_register");
    error = FWK_CameraManager_DeviceRegister(&camera_dev_mipi_ov5640);
    return error;
}
