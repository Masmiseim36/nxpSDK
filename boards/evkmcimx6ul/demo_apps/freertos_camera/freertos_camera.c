/*
 * Copyright 2017 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "fsl_camera.h"
#include "fsl_camera_receiver.h"
#include "fsl_camera_device.h"

#include "board.h"
#include "demo_config.h"
#include "touch.h"
#include "lcd.h"
#include "ff.h"
#include "diskio.h"
#include "fsl_pxp.h"

#include "FreeRTOS.h"
#include "semphr.h"
#include "event_groups.h"
#include "task.h"

#include "fsl_debug_console.h"
#include <stdio.h>

#include "fsl_gpio.h"
#include "fsl_csi.h"
#include "fsl_csi_camera_adapter.h"
#include "fsl_ov5640.h"
#include "pin_mux.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Display. */
#define LCD_DISP_GPIO GPIO5
#define LCD_DISP_GPIO_PIN 9
/* Back light. */
#define LCD_BL_GPIO GPIO1
#define LCD_BL_GPIO_PIN 8

#define OV5640_I2C I2C2


#define MSEC_TO_TICK(msec) \
    (((uint32_t)(msec) + 500uL / (uint32_t)configTICK_RATE_HZ) * (uint32_t)configTICK_RATE_HZ / 1000uL)

/* Number of frame buffers used in camera receiving. */
#define APP_CAMERA_FRAME_BUFFER_COUNT 4

/* Save 10000 pictures to SD/MMC card at most. */
#define APP_MAX_SAVED_PICS 10000

/* Touch more than 1 second is long touch. */
#define TOUCH_LONG_THRESHOLD MSEC_TO_TICK(1000)

#define CAMERA_CAPTURE_BORDER 10

/* Camera input pixel format is YUYV, BPP=2 */
#define CAMERA_BPP 2

typedef enum _touch_gesture
{
    kTOUCH_GestureLongPress,
    kTOUCH_GestureShortPress,
} touch_gesture_t;

typedef enum _app_state
{
    kAPP_Preview,
    kAPP_Capture,
    kAPP_Browse,
} app_state_t;

/*
 * Object to sync the file operation between display task and SDMMC task.
 *
 * In this example, the display task runs as foreground task to show the pictures,
 * the SDMMC task runs as back ground task to save or read picture data. With the
 * file_sync_obj_t, display task could submit file operation to SDMMC task and
 * continue to run. To confirm the file operation is finished, application could
 * call APP_WaitFileOperation.
 */
typedef struct _file_sync_obj
{
    FRESULT result;
    EventGroupHandle_t event;
#define FS_EVENT_NEW_REQUEST (1 << 0)
#define FS_EVENT_RW_DONE (1 << 1)
    const char *fileName;
    char *data;
    uint32_t dataSize;
    uint8_t rw; /*!< 0 to read, 1 to write. */
} file_sync_obj_t;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
extern camera_device_handle_t cameraDevice;
extern camera_receiver_handle_t cameraReceiver;
static void APP_SdmmcTask(void *param);
static void APP_DisplayTask(void *param);
static void APP_InitTask(void *param);

/*******************************************************************************
* Variables
*******************************************************************************/
/* Semaphore indicate new frame received. */
static SemaphoreHandle_t s_cameraReceiveSema = NULL;
/* Semaphore indicate PXP process completed. */
static SemaphoreHandle_t s_pxpCompleteSema = NULL;
/* Queue to save the touch panel gestures. */
static QueueHandle_t s_touchGestureQueue = NULL;
/* Object to sync picture data between display task and SDMMC task. */
static file_sync_obj_t fileSyncObj;

AT_NONCACHEABLE_SECTION_ALIGN(static uint16_t s_sdmmcWritePicture[LCD_HEIGHT][LCD_WIDTH], FRAME_BUFFER_ALIGN);
AT_NONCACHEABLE_SECTION_ALIGN(
    static uint16_t s_cameraBuffer[APP_CAMERA_FRAME_BUFFER_COUNT][CAMERA_HEIGHT][CAMERA_WIDTH], FRAME_BUFFER_ALIGN);
AT_NONCACHEABLE_SECTION(static FATFS fileSystem);
AT_NONCACHEABLE_SECTION(static BYTE work[_MAX_SS]);
AT_NONCACHEABLE_SECTION(static FIL fileObject);

/*******************************************************************************
 * Code
 ******************************************************************************/

extern void CSI_DriverIRQHandler(void);
extern void LCD_IRQHandler(void);
extern void PXP_IRQHandler(void);
extern void GPIO_IRQ_Handler(void);
extern void USDHC1_DriverIRQHandler(void);

/* Initialize the LCD_DISP. */
void BOARD_InitLcd(void)
{
    volatile uint32_t i = 0x100U;

    gpio_pin_config_t config = {
        kGPIO_DigitalOutput, 0,
    };

    /* LCD power on. */
    BOARD_NXP74LV595_SetValue(kNXP74LV595_LCD_nPWREN, kSignal_NXP74LV595_Low);

    /* Reset the LCD. */
    GPIO_PinInit(LCD_DISP_GPIO, LCD_DISP_GPIO_PIN, &config);

    GPIO_WritePinOutput(LCD_DISP_GPIO, LCD_DISP_GPIO_PIN, 0);

    while (i--)
    {
    }

    GPIO_WritePinOutput(LCD_DISP_GPIO, LCD_DISP_GPIO_PIN, 1);

    /* Backlight. */
    config.outputLogic = 1;
    GPIO_PinInit(LCD_BL_GPIO, LCD_BL_GPIO_PIN, &config);
}

void BOARD_InitLcdifPixClock(void)
{
    /*
     * The desired output frame rate is 60Hz. So the pixel clock frequency is:
     * (480 + 41 + 4 + 18) * (272 + 10 + 4 + 2) * 60 = 9.2M.
     * Here set the LCDIF pixel clock to 9.3M.
     */

    /*
     * Initialize the Video PLL.
     * Video PLL output clock is OSC24M * (loopDivider + (denominator / numerator)) / postDivider = 93MHz.
     */
    clock_video_pll_config_t config = {
        .loopDivider = 31, .postDivider = 8, .numerator = 0, .denominator = 0,
    };

    CLOCK_InitVideoPll(&config);

    /*
     * 000 derive clock from PLL2
     * 001 derive clock from PLL3 PFD3
     * 010 derive clock from PLL5
     * 011 derive clock from PLL2 PFD0
     * 100 derive clock from PLL2 PFD1
     * 101 derive clock from PLL3 PFD1
     */
    CLOCK_SetMux(kCLOCK_Lcdif1PreMux, 2);

    CLOCK_SetDiv(kCLOCK_Lcdif1PreDiv, 4);

    CLOCK_SetDiv(kCLOCK_Lcdif1Div, 1);

    /*
     * 000 derive clock from divided pre-muxed lcdif1 clock
     * 001 derive clock from ipp_di0_clk
     * 010 derive clock from ipp_di1_clk
     * 011 derive clock from ldb_di0_clk
     * 100 derive clock from ldb_di1_clk
     */
    CLOCK_SetMux(kCLOCK_Lcdif1Mux, 0);
}

/* OV5640 connect to CSI. */
static csi_resource_t csiResource = {
    .csiBase = CSI,
};

static csi_private_data_t csiPrivateData;

camera_receiver_handle_t cameraReceiver = {
    .resource = &csiResource, .ops = &csi_ops, .privateData = &csiPrivateData,
};

static void BOARD_PullCameraResetPin(bool pullUp)
{
    if (pullUp)
    {
        BOARD_NXP74LV595_SetValue(kNXP74LV595_CSI_RST, kSignal_NXP74LV595_High);
    }
    else
    {
        BOARD_NXP74LV595_SetValue(kNXP74LV595_CSI_RST, kSignal_NXP74LV595_Low);
    }
}

static void BOARD_PullCameraPowerDownPin(bool pullUp)
{
    if (pullUp)
    {
        BOARD_NXP74LV595_SetValue(kNXP74LV595_CSI_PWDN, kSignal_NXP74LV595_High);
    }
    else
    {
        BOARD_NXP74LV595_SetValue(kNXP74LV595_CSI_PWDN, kSignal_NXP74LV595_Low);
    }
}

static ov5640_resource_t ov5640Resource = {
    .sccbI2C = OV5640_I2C, .pullResetPin = BOARD_PullCameraResetPin, .pullPowerDownPin = BOARD_PullCameraPowerDownPin,
};

camera_device_handle_t cameraDevice = {
    .resource = &ov5640Resource, .ops = &ov5640_ops,
};

void BOARD_InitCameraResource(void)
{
    i2c_master_config_t masterConfig;
    uint32_t sourceClock;

    /*
     * masterConfig->baudRate_Bps = 100000U;
     * masterConfig->enableHighDrive = false;
     * masterConfig->enableStopHold = false;
     * masterConfig->glitchFilterWidth = 0U;
     * masterConfig->enableMaster = true;
     */
    I2C_MasterGetDefaultConfig(&masterConfig);
    masterConfig.baudRate_Bps = 100000;

    sourceClock = (CLOCK_GetFreq(kCLOCK_IpgClk) / (CLOCK_GetDiv(kCLOCK_PerclkDiv) + 1U));

    I2C_MasterInit(OV5640_I2C, &masterConfig, sourceClock);

    /* CSI MCLK select 24M. */
    CLOCK_SetMux(kCLOCK_CsiMux, 0);
    CLOCK_SetDiv(kCLOCK_CsiDiv, 0);
    CLOCK_EnableClock(kCLOCK_CsiMclk);
}


/*Provides own implementation of vApplicationIRQHandler() to call SystemIrqHandler()
 *directly without saving the FPU registers on interrupt entry.
 */
void vApplicationIRQHandler(uint32_t ulICCIAR)
{
    SystemIrqHandler(ulICCIAR);
}
/*
 * This function processes the touch panel event, decode the gesture.
 */
void APP_TouchCallback(touch_event_t event, uint16_t x, uint16_t y, void *userData)
{
    static TickType_t down_tick;
    /* Current touch has been reported to gesture queue. */
    static bool isTouchReported = false;
    TickType_t tick_delta;
    touch_gesture_t gesture;

    if (kTOUCH_Down == event)
    {
        isTouchReported = false;
        down_tick = xTaskGetTickCount();
    }
    else
    {
        if (!isTouchReported)
        {
            if (kTOUCH_Up == event)
            {
                gesture = kTOUCH_GestureShortPress;
                xQueueSendToBack(s_touchGestureQueue, &gesture, 0);
            }
            else
            {
                tick_delta = xTaskGetTickCount() - down_tick;

                if (tick_delta > TOUCH_LONG_THRESHOLD)
                {
                    gesture = kTOUCH_GestureLongPress;
                    isTouchReported = true;
                    xQueueSendToBack(s_touchGestureQueue, &gesture, 0);
                }
            }
        }
    }
}

/* The SD/MMC task to handle file read and write. */
static void APP_SdmmcTask(void *param)
{
    FRESULT error;

    while (xEventGroupWaitBits(fileSyncObj.event, FS_EVENT_NEW_REQUEST, pdTRUE, pdTRUE, portMAX_DELAY))
    {
        UINT processedDataSizeBytes;

        if (fileSyncObj.rw)
        {
            error = f_open(&fileObject, _T(fileSyncObj.fileName), (FA_WRITE | FA_CREATE_ALWAYS));

            while ((FR_OK == error) && (fileSyncObj.dataSize > 0))
            {
                error = f_write(&fileObject, fileSyncObj.data, fileSyncObj.dataSize, &processedDataSizeBytes);
                fileSyncObj.data += processedDataSizeBytes;
                fileSyncObj.dataSize -= processedDataSizeBytes;
            }
        }
        else
        {
            error = f_open(&fileObject, _T(fileSyncObj.fileName), FA_READ);

            while ((FR_OK == error) && (fileSyncObj.dataSize > 0))
            {
                error = f_read(&fileObject, fileSyncObj.data, fileSyncObj.dataSize, &processedDataSizeBytes);
                fileSyncObj.data += processedDataSizeBytes;
                fileSyncObj.dataSize -= processedDataSizeBytes;
            }
        }

        f_close(&fileObject);

        fileSyncObj.result = error;
        xEventGroupSetBits(fileSyncObj.event, FS_EVENT_RW_DONE);
    }
}

/* Application should make sure the SDMMC has finished the previous file operation before calling this. */
static void APP_SubmitFileRequest(const char *fileName, char *data, uint32_t bytes, uint8_t write)
{
    fileSyncObj.fileName = fileName;
    fileSyncObj.data = data;
    fileSyncObj.dataSize = bytes;
    fileSyncObj.rw = write;

    xEventGroupSetBits(fileSyncObj.event, FS_EVENT_NEW_REQUEST);
}

/* Wait for file operation finished. */
static FRESULT APP_WaitFileOperation(void)
{
    xEventGroupWaitBits(fileSyncObj.event, FS_EVENT_RW_DONE, pdTRUE, pdTRUE, portMAX_DELAY);

    return fileSyncObj.result;
}

void APP_CameraReceiveCallback(camera_receiver_handle_t *handle, status_t status, void *userData)
{
    BaseType_t xHigherPriorityTaskWoken;

    if (kStatus_Success == status)
    {
        xHigherPriorityTaskWoken = pdFALSE;
        xSemaphoreGiveFromISR(s_cameraReceiveSema, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

void APP_InitCamera(void)
{
    /* Initialize the camera. */
    const camera_config_t cameraConfig = {
        .pixelFormat = kVIDEO_PixelFormatYUYV,
        .bytesPerPixel = CAMERA_BPP,
        .resolution = FSL_VIDEO_RESOLUTION(CAMERA_WIDTH, CAMERA_HEIGHT),
        .frameBufferLinePitch_Bytes = CAMERA_WIDTH * CAMERA_BPP,
        .interface = kCAMERA_InterfaceCCIR656,
        .controlFlags = CAMERA_CONTROL_FLAGS,
        .framePerSec = 30,
    };

    s_cameraReceiveSema = xSemaphoreCreateCounting(APP_CAMERA_FRAME_BUFFER_COUNT, 0);

    configASSERT(NULL != s_cameraReceiveSema);

    INIT_CAMERA_INT();

    CAMERA_DEVICE_Init(&cameraDevice, &cameraConfig);

    CAMERA_DEVICE_Start(&cameraDevice);

    CAMERA_RECEIVER_Init(&cameraReceiver, &cameraConfig, APP_CameraReceiveCallback, NULL);

    /* Submit the empty frame buffers to buffer queue. */
    for (uint32_t i = 0; i < APP_CAMERA_FRAME_BUFFER_COUNT; i++)
    {
        CAMERA_RECEIVER_SubmitEmptyBuffer(&cameraReceiver, (uint32_t)(s_cameraBuffer[i]));
    }

    CAMERA_RECEIVER_Start(&cameraReceiver);
}

/*
 * Flush the camera frame buffer queue.
 */
void APP_FlushCameraFrames(void)
{
    uint32_t frameBuffer;

    while (pdTRUE == xSemaphoreTake(s_cameraReceiveSema, 0))
    {
        CAMERA_RECEIVER_GetFullBuffer(&cameraReceiver, &frameBuffer);
        /* Return the camera buffer to camera queue. */
        CAMERA_RECEIVER_SubmitEmptyBuffer(&cameraReceiver, frameBuffer);
    }
}

void APP_InitPxp(void)
{
    /*
     * Configure the PXP for color space conversion.
     */
    PXP_Init(DISPLAY_PXP);

    PXP_SetProcessSurfaceBackGroundColor(DISPLAY_PXP, 0xFFFFFFFFU);

    PXP_SetProcessSurfacePosition(DISPLAY_PXP, 0U, 0U, LCD_WIDTH - 1U, LCD_HEIGHT - 1U);
    PXP_SetProcessSurfaceScaler(DISPLAY_PXP, CAMERA_WIDTH, CAMERA_HEIGHT, LCD_WIDTH, LCD_HEIGHT);

    /* Disable AS. */
    PXP_SetAlphaSurfacePosition(DISPLAY_PXP, 0xFFFFU, 0xFFFFU, 0U, 0U);

    PXP_SetCsc1Mode(DISPLAY_PXP, kPXP_Csc1YCbCr2RGB);
    PXP_EnableCsc1(DISPLAY_PXP, true);

    INIT_PXP_INT();
    PXP_EnableInterrupts(DISPLAY_PXP, kPXP_CompleteInterruptEnable);

    s_pxpCompleteSema = xSemaphoreCreateBinary();
    configASSERT(NULL != s_pxpCompleteSema);
}

/* The source frame is scaled down and add border then saved to the destination frame. */
void APP_ConvertFrame(uint32_t srcFrame, uint32_t destFrame, uint16_t border)
{
    static pxp_ps_buffer_config_t psBufferConfig = {
        .pixelFormat = kPXP_PsPixelFormatUYVY1P422,
        .swapByte = false,
        .bufferAddrU = 0U,
        .bufferAddrV = 0U,
        .pitchBytes = CAMERA_WIDTH * CAMERA_BPP,
    };

    static pxp_output_buffer_config_t outputBufferConfig = {
        .pixelFormat = kPXP_OutputPixelFormatRGB565,
        .interlacedMode = kPXP_OutputProgressive,
        .buffer1Addr = 0U,
        .pitchBytes = LCD_WIDTH * 2,
        .width = LCD_WIDTH,
        .height = LCD_HEIGHT,
    };

    psBufferConfig.bufferAddr = srcFrame;
    PXP_SetProcessSurfaceBufferConfig(DISPLAY_PXP, &psBufferConfig);

    outputBufferConfig.buffer0Addr = destFrame;
    PXP_SetOutputBufferConfig(DISPLAY_PXP, &outputBufferConfig);

    PXP_SetProcessSurfaceScaler(DISPLAY_PXP, CAMERA_WIDTH, CAMERA_HEIGHT, LCD_WIDTH - 2 * border,
                                LCD_HEIGHT - 2 * border);
    PXP_SetProcessSurfacePosition(DISPLAY_PXP, border, border, LCD_WIDTH - border - 1, LCD_HEIGHT - border - 1);

    PXP_Start(DISPLAY_PXP);
    /* Wait for PXP complete. */
    xSemaphoreTake(s_pxpCompleteSema, portMAX_DELAY);
}

void PXP_IRQHandler(void)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    if (kPXP_CompleteFlag & PXP_GetStatusFlags(DISPLAY_PXP))
    {
        PXP_ClearStatusFlags(DISPLAY_PXP, kPXP_CompleteFlag);
        xSemaphoreGiveFromISR(s_pxpCompleteSema, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

/* Display task, this task shows camera input or saved pictures in LCD. */
static void APP_DisplayTask(void *param)
{
    int curPicIdx = 0;
    int savedPicNum = 0;
    uint32_t lcdInactiveBuf;
    uint32_t fullCameraBufferAddr;
    uint8_t fileTryTime;
    char fileName[16];
    touch_gesture_t gesture;
    app_state_t appState = kAPP_Preview;

    while (1)
    {
        switch (appState)
        {
            case kAPP_Preview:

                while (1)
                {
                    if (pdTRUE == xQueueReceive(s_touchGestureQueue, &gesture, 0))
                    {
                        if (kTOUCH_GestureShortPress == gesture)
                        {
                            if (savedPicNum >= APP_MAX_SAVED_PICS)
                            {
                                PRINTF("Could not save more pictures...\r\n");
                            }
                            else
                            {
                                appState = kAPP_Capture;
                            }
                        }
                        else if (kTOUCH_GestureLongPress == gesture)
                        {
                            if (savedPicNum > 0)
                            {
                                appState = kAPP_Browse;
                                break;
                            }
                            else
                            {
                                PRINTF("No picture captured and saved...\r\n");
                            }
                        }
                        else
                        {
                        }
                    }

                    /* Wait for new input frame ready. */
                    xSemaphoreTake(s_cameraReceiveSema, portMAX_DELAY);
                    CAMERA_RECEIVER_GetFullBuffer(&cameraReceiver, &fullCameraBufferAddr);

                    if (kAPP_Capture == appState)
                    {
                        /* Convert the picture to RGB format and save to SD card. */
                        APP_ConvertFrame(fullCameraBufferAddr, (uint32_t)s_sdmmcWritePicture, 0);

                        sprintf(fileName, "IMG%05d", savedPicNum);
                        APP_SubmitFileRequest(fileName, (void *)s_sdmmcWritePicture, LCD_WIDTH * LCD_HEIGHT * 2, 1);

                        /* Animation. */
                        for (int32_t border = 0; border < CAMERA_CAPTURE_BORDER; border++)
                        {
                            lcdInactiveBuf = LCD_BeginDraw(false);

                            APP_ConvertFrame(fullCameraBufferAddr, (uint32_t)lcdInactiveBuf, border);

                            LCD_EndDraw();
                        }

                        if (FR_OK == APP_WaitFileOperation())
                        {
                            savedPicNum++;
                            PRINTF("Picture %s is saved...\n\r", fileName);
                        }
                        else
                        {
                            PRINTF("Could not save the picture...\n\r");
                        }

                        APP_FlushCameraFrames();
                        appState = kAPP_Preview;
                    }
                    else
                    {
                        lcdInactiveBuf = LCD_BeginDraw(false);

                        /* Start the color space conversion using PXP. */
                        APP_ConvertFrame(fullCameraBufferAddr, lcdInactiveBuf, 0);

                        LCD_EndDraw();
                    }

                    /* Return the camera buffer to camera queue. */
                    CAMERA_RECEIVER_SubmitEmptyBuffer(&cameraReceiver, fullCameraBufferAddr);
                }

                break;

            case kAPP_Browse:
                curPicIdx = 0;

                while (1)
                {
                    /* Get frame buffer to draw. */
                    lcdInactiveBuf = LCD_BeginDraw(false);

                    /* Submit the request to load pictures from SD card. */
                    sprintf(fileName, "IMG%05d", curPicIdx);

                    /* Try three times if failed. */
                    for (fileTryTime = 0; fileTryTime < 3; fileTryTime++)
                    {
                        APP_SubmitFileRequest(fileName, (void *)lcdInactiveBuf, LCD_WIDTH * LCD_HEIGHT * 2, 0);

                        if (FR_OK == APP_WaitFileOperation())
                        {
                            break;
                        }
                    }

                    if (3 == fileTryTime)
                    {
                        PRINTF("Could not read the picture: %s\n\r", fileName);
                    }
                    else
                    {
                        PRINTF("Picture %s is read and shown.\n\r", fileName);
                    }

                    /* Pass the frame buffer to LCD. */
                    LCD_EndDraw();

                    /* Wait touch event. */
                    while (pdTRUE != xQueueReceive(s_touchGestureQueue, &gesture, portMAX_DELAY))
                    {
                    }

                    if (kTOUCH_GestureLongPress == gesture)
                    {
                        /* Return to preview state. */
                        appState = kAPP_Preview;
                        APP_FlushCameraFrames();
                        break;
                    }
                    else if (kTOUCH_GestureShortPress == gesture)
                    {
                        curPicIdx++;

                        if (curPicIdx >= savedPicNum)
                        {
                            curPicIdx = 0;
                        }
                    }
                    else
                    {
                    }
                }

                break;

            default:
                appState = kAPP_Preview;
                break;
        }
    }
}

/* This task initializes the resources and creates the other tasks when resources ready. */
static void APP_InitTask(void *param)
{
    const TCHAR driverNumberBuffer[3U] = {SDDISK + '0', ':', '/'};

    APP_InitPxp();

    /* Initialize the LCD. */
    LCD_Init();

    /* Initialize the touch. */
    s_touchGestureQueue = xQueueCreate(4, sizeof(touch_gesture_t));

    configASSERT(NULL != s_touchGestureQueue);

    TOUCH_Init(APP_TouchCallback, NULL);

    /* Initialise and start the camera. */
    APP_InitCamera();

    memset(&fileSyncObj, 0, sizeof(file_sync_obj_t));
    fileSyncObj.event = xEventGroupCreate();
    configASSERT(NULL != fileSyncObj.event);

    PRINTF("\r\nPlease insert a card into board.\r\n");

    INIT_SDMMC_INT();

#if _USE_MKFS
    PRINTF("\r\nAfter card inserted, the card will be formated.\r\n");
    PRINTF("The time may be long if the card capacity is big.\r\n");
    if (f_mkfs(driverNumberBuffer, FM_ANY, 0U, work, sizeof(work)))
    {
        PRINTF("Make file system failed.\r\n");
        configASSERT(pdFALSE);
    }
    PRINTF("\r\nFile system ready......\r\n");
#endif /* _USE_MKFS */

    if (f_mount(&fileSystem, driverNumberBuffer, 0U))
    {
        PRINTF("Mount volume failed.\r\n");
        configASSERT(pdFALSE);
    }

#if (_FS_RPATH >= 2U)
    if (f_chdrive((char const *)driverNumberBuffer))
    {
        PRINTF("Change drive failed.\r\n");
        configASSERT(pdFALSE);
    }
#endif

    xTaskCreate(APP_DisplayTask, "Display Task", DISPLAY_TASK_STACK_SIZE, NULL, DISPLAY_TASK_PRIORITY, NULL);
    xTaskCreate(APP_SdmmcTask, "SDMMC Task", SDMMC_TASK_STACK_SIZE, NULL, SDMMC_TASK_PRIORITY, NULL);

    vTaskDelete(NULL);
}

/*!
 * @brief Application entry point.
 */
int main(void)
{
    /* Init board hardware. */
    BOARD_InitPins();
    BOARD_LCDIF_ConfigurePins();
    BOARD_CSI_ConfigurePins();
    BOARD_USDHC_ConfigurePins();
    BOARD_BootClockRUN();
    BOARD_InitLcdifPixClock();
    BOARD_InitMemory();
    BOARD_InitDebugConsole();
    BOARD_InitLcd();
    BOARD_InitCameraResource();

    /* Install IRQ Handler */
    SystemInitIrqTable();
    /* Tricky here: As IRQ handler in example doesn't use parameters, just ignore them */
    SystemInstallIrqHandler(CSI_IRQn, (system_irq_handler_t)(uint32_t)CSI_DriverIRQHandler, NULL);
    SystemInstallIrqHandler(LCDIF_IRQn, (system_irq_handler_t)(uint32_t)LCD_IRQHandler, NULL);
    SystemInstallIrqHandler(PXP_IRQn, (system_irq_handler_t)(uint32_t)PXP_IRQHandler, NULL);
    SystemInstallIrqHandler(GPIO1_Combined_16_31_IRQn, (system_irq_handler_t)GPIO_IRQ_Handler, NULL);
    SystemInstallIrqHandler(USDHC1_IRQn, (system_irq_handler_t)(uint32_t)USDHC1_DriverIRQHandler, NULL);

    xTaskCreate(APP_InitTask, "Init Task", INIT_TASK_STACK_SIZE, NULL, INIT_TASK_PRIORITY, NULL);

    vTaskStartScheduler();
    for (;;)
    {
    }
}
