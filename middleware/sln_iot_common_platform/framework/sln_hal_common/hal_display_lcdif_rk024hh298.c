/*
 * Copyright 2020-2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
 * @brief display dev HAL driver implementation for lcdif and rk024hh298.
 */

#include <FreeRTOS.h>
#include <queue.h>

#include "sln_ili9341.h"
#include "fsl_elcdif.h"

#include "fwk_log.h"
#include "fwk_display_manager.h"
#include "hal_display_dev.h"
#include "hal_event_descriptor_common.h"
#include "./icons/nxp_logo_240x86.h"

#define DISPLAY_NAME               "rk024hh298"
#define DISPLAY_WIDTH              240
#define DISPLAY_HEIGHT             320
#define DISPLAY_BYTES_PER_PIXEL    2
#define DISPLAY_FRAME_BUFFER_COUNT 2

#define DISPLAY_HSW 10
#define DISPLAY_HFP 10
#define DISPLAY_HBP 20
#define DISPLAY_VSW 2
#define DISPLAY_VFP 4
#define DISPLAY_VBP 2

#define DISPLAY_LCDIF_BASE       LCDIF
#define DISPLAY_LCDIF_IRQn       eLCDIF_IRQn
#define DISPLAY_LCDIF_IRQHandler eLCDIF_IRQHandler

static hal_display_status_t HAL_DisplayDev_LcdifRk024hh2_InputNotify(const display_dev_t *receiver, void *data);
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */
void board_pull_elcdif_rk024hh298_reset_pin(bool pullUp);
void board_elcdif_rk024hh298_backlight_control(bool on);
#if defined(SDK_SW_SPI) && SDK_SW_SPI
void board_elcdif_rk024hh298_spi_send_cmd(uint8_t cmd);
void board_elcdif_rk024hh298_spi_send_data(uint8_t data);
void BOARD_InitElcdifRk024hh298Resource(void);
#else
status_t board_elcdif_rk024hh298_spi_send(uint8_t *txBuff, uint8_t txSize);
#endif
#if defined(__cplusplus)
}
#endif /* __cplusplus */

AT_NONCACHEABLE_SECTION_ALIGN(
    static uint8_t s_FrameBuffers[DISPLAY_FRAME_BUFFER_COUNT][DISPLAY_HEIGHT][DISPLAY_WIDTH * DISPLAY_BYTES_PER_PIXEL],
    32);
static uint8_t *s_pCurrentFrameBuffer = NULL;
static bool s_NewBufferSet            = 0;
static int8_t s_LCDFramesNum          = 0;

static void HAL_DisplayDev_LcdifRk024hh298_WriteCommand(uint8_t cmd)
{
#if defined(SDK_SW_SPI) && SDK_SW_SPI
    board_elcdif_rk024hh298_spi_send_cmd(cmd);
#else
    uint16_t tmp_data = cmd | 0x000;
    board_elcdif_rk024hh298_spi_send((uint8_t *)&tmp_data, 2);
#endif
}

static void HAL_DisplayDev_LcdifRk024hh298_WriteData(uint8_t data)
{
#if defined(SDK_SW_SPI) && SDK_SW_SPI
    board_elcdif_rk024hh298_spi_send_data(data);
#else
    uint16_t tmp_data = data | 0x100;
    board_elcdif_rk024hh298_spi_send((uint8_t *)&tmp_data, 2);
#endif
}

static void BOARD_InitLcdifClock(void)
{
    /* Set to 6Mhz. */
    const clock_root_config_t lcdifClockConfig = {
        .clockOff = false,
        .mux      = kCLOCK_LCDIF_ClockRoot_MuxOsc24MOut,
        .div      = 4,
    };

    CLOCK_SetRootClock(kCLOCK_Root_Lcdif, &lcdifClockConfig);
}

static void BOARD_InitDisplayInterface(void)
{
    CLOCK_EnableClock(kCLOCK_Video_Mux);

    /* ELCDIF output to MIPI DSI. */
    VIDEO_MUX->VID_MUX_CTRL.CLR = VIDEO_MUX_VID_MUX_CTRL_PARA_LCD_SEL_MASK;
}

static void _ELCDIF_Init(void)
{
    memcpy((void *)&s_FrameBuffers[0] + DISPLAY_WIDTH * ((DISPLAY_HEIGHT - NXP_LOGO_H) / 2) * DISPLAY_BYTES_PER_PIXEL,
           nxp_logo_240x86, sizeof(nxp_logo_240x86));

    const elcdif_rgb_mode_config_t config = {
        .panelWidth    = DISPLAY_WIDTH,
        .panelHeight   = DISPLAY_HEIGHT,
        .hsw           = DISPLAY_HSW,
        .hfp           = DISPLAY_HFP,
        .hbp           = DISPLAY_HBP,
        .vsw           = DISPLAY_VSW,
        .vfp           = DISPLAY_VFP,
        .vbp           = DISPLAY_VBP,
        .polarityFlags = (kELCDIF_DataEnableActiveHigh | kELCDIF_VsyncActiveLow | kELCDIF_HsyncActiveLow |
                          kELCDIF_DriveDataOnRisingClkEdge),
        .bufferAddr    = (uint32_t)&s_FrameBuffers[0],
        .pixelFormat   = kELCDIF_PixelFormatRGB565,
        .dataBus       = kELCDIF_DataBus16Bit,
    };

    BOARD_InitLcdifClock();

    BOARD_InitDisplayInterface();

    ELCDIF_RgbModeInit(DISPLAY_LCDIF_BASE, &config);
}

hal_display_status_t HAL_DisplayDev_LcdifRk024hh2_Init(
    display_dev_t *dev, int width, int height, display_dev_callback_t callback, void *param)
{
    hal_display_status_t ret = kStatus_HAL_DisplaySuccess;
    LOGD("++display_dev_init");

    dev->cap.width       = width;
    dev->cap.height      = height;
    dev->cap.frameBuffer = (void *)&s_FrameBuffers[1];
    dev->cap.callback    = callback;

    BOARD_InitElcdifRk024hh298Resource();

    board_pull_elcdif_rk024hh298_reset_pin(1);
    vTaskDelay(50);
    board_pull_elcdif_rk024hh298_reset_pin(0);
    vTaskDelay(100);
    board_pull_elcdif_rk024hh298_reset_pin(1);
    vTaskDelay(120);

    FT9341_Init(HAL_DisplayDev_LcdifRk024hh298_WriteData, HAL_DisplayDev_LcdifRk024hh298_WriteCommand);

    _ELCDIF_Init();

    LOGD("--display_dev_init");
    return ret;
}

hal_display_status_t HAL_DisplayDev_LcdifRk024hh2_Uninit(const display_dev_t *dev)
{
    hal_display_status_t ret = kStatus_HAL_DisplaySuccess;
    return ret;
}

hal_display_status_t HAL_DisplayDev_LcdifRk024hh2_Start(const display_dev_t *dev)
{
    hal_display_status_t ret = kStatus_HAL_DisplaySuccess;
    LOGD("++display_dev_start");

    EnableIRQ(DISPLAY_LCDIF_IRQn);
    NVIC_SetPriority(DISPLAY_LCDIF_IRQn, configMAX_SYSCALL_INTERRUPT_PRIORITY - 1);
    ELCDIF_EnableInterrupts(DISPLAY_LCDIF_BASE, kELCDIF_CurFrameDoneInterruptEnable);
    ELCDIF_RgbModeStart(DISPLAY_LCDIF_BASE);

    LOGD("--display_dev_start");
    return ret;
}

hal_display_status_t HAL_DisplayDev_LcdifRk024hh2_Blit(const display_dev_t *dev, void *frame, int width, int height)
{
    hal_display_status_t ret = kStatus_HAL_DisplayNonBlocking;
    LOGI("++display_dev_blit");

    if (frame != NULL)
    {
        s_NewBufferSet        = true;
        s_pCurrentFrameBuffer = frame;
        ELCDIF_SetNextBufferAddr(DISPLAY_LCDIF_BASE, (uint32_t)frame);
    }

    LOGI("--display_dev_blit");
    return ret;
}

const static display_dev_operator_t s_DisplayDev_LcdifOps = {
    .init        = HAL_DisplayDev_LcdifRk024hh2_Init,
    .deinit      = HAL_DisplayDev_LcdifRk024hh2_Uninit,
    .start       = HAL_DisplayDev_LcdifRk024hh2_Start,
    .blit        = HAL_DisplayDev_LcdifRk024hh2_Blit,
    .inputNotify = HAL_DisplayDev_LcdifRk024hh2_InputNotify,
};

static display_dev_t s_DisplayDev_Lcdif = {.id   = 0,
                                           .name = DISPLAY_NAME,
                                           .ops  = &s_DisplayDev_LcdifOps,
                                           .cap  = {.width       = DISPLAY_WIDTH,
                                                   .height      = DISPLAY_HEIGHT,
                                                   .pitch       = DISPLAY_WIDTH * DISPLAY_BYTES_PER_PIXEL,
                                                   .left        = 0,
                                                   .top         = 0,
                                                   .right       = DISPLAY_WIDTH - 1,
                                                   .bottom      = DISPLAY_HEIGHT - 1,
                                                   .rotate      = kCWRotateDegree_0,
                                                   .format      = kPixelFormat_RGB565,
                                                   .srcFormat   = kPixelFormat_UYVY1P422_RGB,
                                                   .frameBuffer = NULL,
                                                   .callback    = NULL,
                                                   .param       = NULL}};

static hal_display_status_t HAL_DisplayDev_LcdifRk024hh2_InputNotify(const display_dev_t *receiver, void *data)
{
    hal_display_status_t error           = kStatus_HAL_DisplaySuccess;
    event_base_t eventBase               = *(event_base_t *)data;
    event_status_t event_response_status = kEventStatus_Ok;

    if (eventBase.eventId == kEventID_SetDisplayOutputSource)
    {
        event_common_t event             = *(event_common_t *)data;
        s_DisplayDev_Lcdif.cap.srcFormat = event.displayOutput.displayOutputSource;
        s_NewBufferSet                   = true;
        if (eventBase.respond != NULL)
        {
            eventBase.respond(eventBase.eventId, &event.displayOutput, event_response_status, true);
        }
        LOGI("[display_dev_inputNotify]: kEventID_SetDisplayOutputSource devID %d, srcFormat %d", receiver->id,
             event.displayOutput.displayOutputSource);
    }
    else if (eventBase.eventId == kEventID_GetDisplayOutputSource)
    {
        display_output_event_t display;
        display.displayOutputSource = s_DisplayDev_Lcdif.cap.srcFormat;
        if (eventBase.respond != NULL)
        {
            eventBase.respond(eventBase.eventId, &display, event_response_status, true);
        }
        LOGI("[display_dev_inputNotify]: kEventID_GetDisplayOutputSource devID %d, srcFormat %d", receiver->id,
             display.displayOutputSource);
    }

    return error;
}

void DISPLAY_LCDIF_IRQHandler(void)
{
    uint32_t intStatus;

    intStatus = ELCDIF_GetInterruptStatus(DISPLAY_LCDIF_BASE);

    ELCDIF_ClearInterruptStatus(DISPLAY_LCDIF_BASE, intStatus);

    if (s_LCDFramesNum >= 0)
    {
        if (intStatus & kELCDIF_CurFrameDone)
        {
            s_LCDFramesNum++;

            /* TODO: first 20 frames always white screen, workaround RTVZN-599. */
            if (s_LCDFramesNum > 20)
            {
                board_elcdif_rk024hh298_backlight_control(1);
                s_LCDFramesNum = -1;
            }
        }
    }

    if (intStatus & kELCDIF_CurFrameDone && s_NewBufferSet == true)
    {
        if (s_DisplayDev_Lcdif.cap.callback != NULL)
        {
            s_NewBufferSet  = false;
            uint8_t fromISR = __get_IPSR();

            if (s_pCurrentFrameBuffer == (uint8_t *)&s_FrameBuffers[0])
            {
                s_DisplayDev_Lcdif.cap.callback(&s_DisplayDev_Lcdif, kDisplayEvent_RequestFrame, &s_FrameBuffers[1],
                                                fromISR);
            }
            else if (s_pCurrentFrameBuffer == (uint8_t *)&s_FrameBuffers[1])
            {
                s_DisplayDev_Lcdif.cap.callback(&s_DisplayDev_Lcdif, kDisplayEvent_RequestFrame, &s_FrameBuffers[0],
                                                fromISR);
            }
        }
    }
}

int HAL_DisplayDev_LcdifRk024hh298_Register()
{
    int ret = 0;
    LOGD("s_DisplayDev_Lcdif_rk024hh298_register");
    ret = FWK_DisplayManager_DeviceRegister(&s_DisplayDev_Lcdif);
    return ret;
}
