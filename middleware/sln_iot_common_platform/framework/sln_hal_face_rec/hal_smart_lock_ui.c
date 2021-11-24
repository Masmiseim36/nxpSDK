/*
 * Copyright 2019-2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
 * @brief Smart Lock UI device HAL driver implementation.
 */

#include <stdio.h>

#include "board_define.h"

#include "fonts/font.h"
#include "icons/ble_16x16.h"
#include "icons/greenlock_30x38.h"
#include "icons/redlock_30x38.h"
#include "icons/wifi_16x16.h"
#include "icons/sleep_logo_240x320.h"
#include "icons/process_bar_240x14.h"

#include "fwk_graphics.h"
#include "fwk_log.h"
#include "fwk_output_manager.h"
#include "fwk_perf.h"
#include "hal_event_descriptor_face_rec.h"
#include "hal_output_dev.h"
#include "hal_sln_timer.h"
#include "hal_vision_algo_oasis_lite.h"
#include "semphr.h"

#if defined(__cplusplus)
extern "C" {
#endif
int HAL_OutputDev_UiSmartlock_Register();
#if defined(__cplusplus)
}
#endif

/*******************************************************************************
 * Defines
 ******************************************************************************/
#define UI_BUFFER_BPP   2
#define UI_BUFFER_PITCH (UI_BUFFER_WIDTH * UI_BUFFER_BPP)
#define UI_SCALE_W      (640 / UI_BUFFER_WIDTH)
#define UI_SCALE_H      (640 / UI_BUFFER_HEIGHT)

#define RGB565_RED      0xf800
#define RGB565_GREEN    0x07e0
#define RGB565_BLUE     0x001f
#define RGB565_BLACK    0x0001
#define RGB565_NXPGREEN 0xBEA6
#define RGB565_NXPRED   0xFD83
#define RGB565_NXPBLUE  0x6D5B

/*
     |<------------480------------>|
___  _______________________________   ____
 |   |                              |    |
 |   |------------------------------|  --|<--- UI_TOPINFO_Y
 |   |       Top Info               |    |
 |   |------------------------------|  --|<--- UI_MAINWINDOW_Y = UI_TOPINFO_Y+UI_TOPINFO_H
 |   |                              |    |
 |   |                              |    |
 |   |                              |    |
 |   |                              |    |
640  |      Main Window             |    |
 |   |                              |    |
 |   |                              |    |
 |   |                              |    |
 |   |                              |    |
 |   |                              |    |
 |   |------------------------------|  --|<--- UI_BOTTOMINFO_H = UI_MAINWINDOW_Y+UI_MAINWINDOW_H
 |   |        Bottom Info           |    |
_|_  |______________________________|  __|_

*/

#define UI_TOPINFO_W    UI_BUFFER_WIDTH
#define UI_TOPINFO_H    30
#define UI_TOPINFO_X    0
#define UI_TOPINFO_Y    (4 / UI_SCALE_H)
#define UI_BOTTOMINFO_W UI_BUFFER_WIDTH
#define UI_BOTTOMINFO_H 20
#define UI_MAINWINDOW_W UI_BUFFER_WIDTH
#define UI_MAINWINDOW_H (UI_BUFFER_HEIGHT - UI_TOPINFO_H - UI_BOTTOMINFO_H - UI_TOPINFO_Y)
#define UI_MAINWINDOW_X 0
#define UI_MAINWINDOW_Y (UI_TOPINFO_Y + UI_TOPINFO_H)
#define UI_BOTTOMINFO_X 0
#define UI_BOTTOMINFO_Y (UI_MAINWINDOW_Y + UI_MAINWINDOW_H)

#define UI_MAINWINDOW_DEBUG_X 10
#define UI_MAINWINDOW_DEBUG_Y (UI_MAINWINDOW_Y + UI_MAINWINDOW_H / 4)

#define UI_MAINWINDOW_PROCESS_FG_X_OFFSET ((PROCESS_BAR_BG_W - PROCESS_BAR_FG_W) / 2)
#define UI_MAINWINDOW_PROCESS_FG_Y_OFFSET ((PROCESS_BAR_BG_H - PROCESS_BAR_FG_H) / 2)

// bottom info
#define LOCK_SPACING        (8 / UI_SCALE_W)
#define LOCK_WIDTH          greenlock_W
#define RECT_HEIGHT         20
#define RECT_WIDTH          (UI_BUFFER_WIDTH - LOCK_WIDTH - LOCK_SPACING) / 3
#define RED_RECT_WIDTH      (RECT_WIDTH + LOCK_WIDTH + LOCK_SPACING)
#define GREEN_RECT_WIDTH    RED_RECT_WIDTH
#define BLUE_RECT_WIDTH     (UI_BUFFER_WIDTH - RED_RECT_WIDTH - GREEN_RECT_WIDTH)
#define POS_RECT_Y          (UI_BUFFER_HEIGHT - RECT_HEIGHT)
#define POS_NXPRED_RECT_X   0
#define POS_NXPBLUE_RECT_X  (POS_NXPRED_RECT_X + RED_RECT_WIDTH)
#define POS_NXPGREEN_RECT_X (POS_NXPBLUE_RECT_X + BLUE_RECT_WIDTH)

#define WIFI_ICON_RELATIVE_X    (10 / UI_SCALE_W) - 4
#define BLE_ICON_RELATIVE_X     WIFI_ICON_RELATIVE_X + WIFI_W + LOCK_SPACING / 2
#define REGISTRATION_RELATIVE_X (2 / UI_SCALE_W)
#define MODE_RELATIVE_X         (LOCK_SPACING + LOCK_WIDTH)
#define REGISTRATION_RELATIVE_Y (4 / UI_SCALE_H)
#define MODE_RELATIVE_Y         (4 / UI_SCALE_H)
#define APP_RELATIVE_X          (LOCK_SPACING + LOCK_WIDTH)
#define APP_RELATIVE_Y          (4 / UI_SCALE_H)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static hal_output_status_t HAL_OutputDev_UiFfi_Init(const output_dev_t *dev);
static hal_output_status_t HAL_OutputDev_UiFfi_Start(const output_dev_t *dev);
static hal_output_status_t HAL_OutputDev_UiFfi_InferComplete(const output_dev_t *dev,
                                                             output_algo_source_t source,
                                                             void *infer_result);
static hal_output_status_t HAL_OutputDev_UiFfi_InputNotify(const output_dev_t *dev, void *data);

/*******************************************************************************
 * Global Variables
 ******************************************************************************/
static gfx_surface_t s_UiSurface;

SDK_ALIGN(static char s_AsBuffer[UI_BUFFER_WIDTH * UI_BUFFER_HEIGHT * UI_BUFFER_BPP], 32);

static oasis_lite_result_t s_LastResult;

const static output_dev_event_handler_t s_OutputDev_UiHandler = {
    .inferenceComplete = HAL_OutputDev_UiFfi_InferComplete,
    .inputNotify       = HAL_OutputDev_UiFfi_InputNotify,
};

static uint8_t s_BleIsOn;
static uint8_t s_LPMIsOn;
static uint8_t s_LogLevel;
static uint8_t s_RefreshUI;

/*******************************************************************************
 * Internal Functions
 ******************************************************************************/
static void _ui_eraseTopInfo(uint16_t eraseColor)
{
    if (eraseColor)
    {
        uint16_t *buf = (uint16_t *)(s_UiSurface.buf + UI_TOPINFO_Y * UI_BUFFER_PITCH);
        for (int i = 0; i < UI_BUFFER_WIDTH * UI_TOPINFO_H; i++)
        {
            buf[i] = eraseColor;
        }
    }
    else
    {
        memset(s_UiSurface.buf + UI_TOPINFO_Y * UI_BUFFER_PITCH, 0x0, (UI_BUFFER_PITCH * UI_TOPINFO_H));
    }
}

static void _ui_eraseMainWindow(uint16_t eraseColor)
{
    if (eraseColor)
    {
        uint16_t *buf = (uint16_t *)(s_UiSurface.buf + UI_TOPINFO_Y * UI_BUFFER_PITCH);
        for (int i = 0; i < UI_BUFFER_WIDTH * UI_MAINWINDOW_H; i++)
        {
            buf[i] = eraseColor;
        }
    }
    else
    {
        memset(s_UiSurface.buf + UI_MAINWINDOW_Y * UI_BUFFER_PITCH, 0x0, (UI_BUFFER_PITCH * UI_MAINWINDOW_H));
    }
}

static void ui_eraseWholeWindow(uint16_t eraseColor)
{
    if (eraseColor)
    {
        uint16_t *buf = (uint16_t *)(s_UiSurface.buf);
        for (int i = 0; i < UI_BUFFER_WIDTH * UI_BUFFER_HEIGHT; i++)
        {
            buf[i] = eraseColor;
        }
    }
    else
    {
        memset(s_UiSurface.buf, 0x0, (UI_BUFFER_PITCH * UI_BUFFER_WIDTH));
    }
}

static void _ui_drawTopInfo(oasis_lite_result_t *result)
{
    char txt[64];
    int bgColor = -1;

    memset(txt, 0x0, 64);
    switch (result->state)
    {
        case kOASISLiteState_Recognition:
        {
            switch (result->rec_result)
            {
                case kOASISLiteRecognitionResult_Success:
                {
                    sprintf(txt, "Recognition Successful");
                    bgColor = RGB565_GREEN;
                }
                break;
                case kOASISLiteRecognitionResult_Timeout:
                {
                    sprintf(txt, "Recognition Failed");
                    bgColor = RGB565_RED;
                }
                break;
                default:
                    if (result->qualityCheck == kOasisLiteQualityCheck_FakeFace)
                    {
                        /* The spaces help center the text */
                        /* TODO: Figure out a programmatic way to center text */
                        sprintf(txt, "          Fake Face!");
                        bgColor = RGB565_RED;
                        _ui_eraseTopInfo(bgColor);
                    }
                    break;
            }
        }
        break;
        case kOASISLiteState_Registration:
        {
            switch (result->reg_result)
            {
                case kOASISLiteRegistrationResult_Invalid:
                {
                    sprintf(txt, "Registering");
                    bgColor = RGB565_GREEN;
                }
                break;

                case kOASISLiteRegistrationResult_Success:
                {
                    sprintf(txt, "Registration Successful");
                    bgColor = RGB565_GREEN;
                }
                break;

                case kOASISLiteRegistrationResult_Duplicated:
                {
                    sprintf(txt, "User Already Exists");
                    bgColor = RGB565_RED;
                }
                break;

                case kOASISLiteRegistrationResult_Timeout:
                {
                    sprintf(txt, "Registration Timeout");
                    bgColor = RGB565_RED;
                }
                break;

                default:
                    break;
            }
        }
        break;

        case kOASISLiteState_DeRegistration:
        {
            switch (result->dereg_result)
            {
                case kOASISLiteDeregistrationResult_Invalid:
                {
                    sprintf(txt, "Deregistering");
                    bgColor = RGB565_GREEN;
                }
                break;

                case kOASISLiteDeregistrationResult_Success:
                {
                    sprintf(txt, "\"%s\" Removed", result->name);
                    bgColor = RGB565_GREEN;
                }
                break;

                case kOASISLiteDeregistrationResult_Timeout:
                {
                    sprintf(txt, "Deregistration Timeout");
                    bgColor = RGB565_RED;
                }
                break;

                default:
                    break;
            }
        }
        break;

        default:
            break;
    }

    if (bgColor != -1)
    {
        gfx_drawText(&s_UiSurface, (UI_TOPINFO_X + (UI_MAINWINDOW_W / 2 - 100) / UI_SCALE_W), UI_TOPINFO_Y, 0x0,
                     bgColor, (int)kFont_OpenSans16, txt);
    }
}

static void ui_drawGuideRect(int color)
{
    // registration prompt line
    int w = 300 / UI_SCALE_W;
    int h = 360 / UI_SCALE_H;
    int x = (UI_BUFFER_WIDTH - w) / 2;
    int y = (UI_BUFFER_HEIGHT - h) / 2;
    int l = 100 / UI_SCALE_W;
    int d = 4 / UI_SCALE_W;
    gfx_drawRect(&s_UiSurface, x, y, l, d, color);
    gfx_drawRect(&s_UiSurface, x, y, d, l, color);
    gfx_drawRect(&s_UiSurface, x + w - l, y, l, d, color);
    gfx_drawRect(&s_UiSurface, x + w, y, d, l, color);
    gfx_drawRect(&s_UiSurface, x, y + h, l, d, color);
    gfx_drawRect(&s_UiSurface, x, y + h - l, d, l, color);
    gfx_drawRect(&s_UiSurface, x + w - l, y + h, l, d, color);
    gfx_drawRect(&s_UiSurface, x + w, y + h - l, d, l, color);
}

static void ui_drawProcessBar(float percent)
{
    /* process bar background */
    gfx_drawPicture(&s_UiSurface, 0, UI_BOTTOMINFO_Y - 36, PROCESS_BAR_BG_W, PROCESS_BAR_BG_H, 0xFFFF,
                    (char *)process_bar_240x14);

    /* process bar foreground */
    gfx_drawRect(&s_UiSurface, UI_MAINWINDOW_PROCESS_FG_X_OFFSET,
                 UI_BOTTOMINFO_Y - 36 + UI_MAINWINDOW_PROCESS_FG_Y_OFFSET, (int)(PROCESS_BAR_FG_W * percent),
                 PROCESS_BAR_FG_H, RGB565_NXPBLUE);
}

static void ui_drawDebugWindow(oasis_lite_debug_t debugInfo)
{
    char txt[64];
    uint8_t debugRow   = 0;
    uint16_t textColor = 0;
    memset(txt, 0, sizeof(txt));
    sprintf(txt, "sim: %d.%d%%  id:%d", (int)(debugInfo.sim / 100), (int)(debugInfo.sim % 100), debugInfo.faceID);
    textColor = RGB565_GREEN;
    gfx_drawText(&s_UiSurface, UI_MAINWINDOW_DEBUG_X, UI_MAINWINDOW_DEBUG_Y + debugRow, textColor, 0x0,
                 (int)kFont_OpenSans8, txt);
    debugRow += 15;

    memset(txt, 0, sizeof(txt));
    sprintf(txt, "blur: %d", debugInfo.isBlurry);
    textColor = debugInfo.isBlurry ? RGB565_RED : RGB565_GREEN;
    gfx_drawText(&s_UiSurface, UI_MAINWINDOW_DEBUG_X, UI_MAINWINDOW_DEBUG_Y + debugRow, textColor, 0x0,
                 (int)kFont_OpenSans8, txt);
    debugRow += 15;
    memset(txt, 0, sizeof(txt));
    sprintf(txt, "3d_fake: %d", debugInfo.is3dFake);
    textColor = debugInfo.is3dFake ? RGB565_RED : RGB565_GREEN;
    gfx_drawText(&s_UiSurface, UI_MAINWINDOW_DEBUG_X, UI_MAINWINDOW_DEBUG_Y + debugRow, textColor, 0x0,
                 (int)kFont_OpenSans8, txt);
    debugRow += 15;
    memset(txt, 0, sizeof(txt));
    sprintf(txt, "2d_fake: %d", debugInfo.is2dFake);
    textColor = debugInfo.is2dFake ? RGB565_RED : RGB565_GREEN;
    gfx_drawText(&s_UiSurface, UI_MAINWINDOW_DEBUG_X, UI_MAINWINDOW_DEBUG_Y + debugRow, textColor, 0x0,
                 (int)kFont_OpenSans8, txt);
    debugRow += 15;
    memset(txt, 0, sizeof(txt));
    sprintf(txt, "pose: %s  ", debugInfo.isSideFace ? "side" : "front");
    textColor = debugInfo.isSideFace ? RGB565_RED : RGB565_GREEN;
    gfx_drawText(&s_UiSurface, UI_MAINWINDOW_DEBUG_X, UI_MAINWINDOW_DEBUG_Y + debugRow, textColor, 0x0,
                 (int)kFont_OpenSans8, txt);
}

static void _ui_drawMainWindow(oasis_lite_result_t *pResult)
{
    if (pResult->state == kOASISLiteState_Registration)
    {
        if (pResult->reg_result == kOASISLiteRegistrationResult_Invalid)
        {
            ui_drawGuideRect(RGB565_GREEN);
        }
        else
        {
            if (pResult->face_count == 1)
            {
                // known face
                if (pResult->face_id >= 0)
                {
                    int w = 300 / UI_SCALE_W;
                    int h = 360 / UI_SCALE_H;
                    int x = (UI_BUFFER_WIDTH - w) / 2 + 50 / UI_SCALE_W;
                    int y = (UI_BUFFER_HEIGHT - h) / 2 + 200 / UI_SCALE_H;
                    char txt[1024];
                    int bgColor = RGB565_GREEN;
                    if (pResult->reg_result == kOASISLiteRegistrationResult_Success)
                    {
                        sprintf(txt, "%s added", pResult->name);
                        bgColor = RGB565_GREEN;
                    }
                    else if (pResult->reg_result == kOASISLiteRegistrationResult_Duplicated)
                    {
                        sprintf(txt, "%s exists", pResult->name);
                        bgColor = RGB565_RED;
                    }
                    gfx_drawText(&s_UiSurface, x, y, 0x0, bgColor, (int)kFont_OpenSans16, txt);
                }
            }
        }

        ui_drawProcessBar(pResult->process);
    }
    else if (pResult->state == kOASISLiteState_DeRegistration)
    {
        if (pResult->dereg_result == kOASISLiteDeregistrationResult_Invalid)
        {
            ui_drawGuideRect(RGB565_RED);
        }
        else
        {
        }

        ui_drawProcessBar(pResult->process);
    }
}

static void _ui_eraseBottomInfo()
{
    memset(s_UiSurface.buf + UI_BOTTOMINFO_Y * UI_BUFFER_PITCH, 0x0, (UI_BUFFER_PITCH * UI_BOTTOMINFO_H));
}

static void _ui_drawBottomInfo(oasis_lite_result_t *pResult)
{
    char tstring[64];
    int dbCount     = facedb_get_count();
    uint16_t *pIcon = NULL;

    _ui_eraseBottomInfo();
    gfx_drawRect(&s_UiSurface, POS_NXPGREEN_RECT_X, POS_RECT_Y, GREEN_RECT_WIDTH, RECT_HEIGHT, RGB565_NXPGREEN);
    gfx_drawRect(&s_UiSurface, POS_NXPBLUE_RECT_X, POS_RECT_Y, BLUE_RECT_WIDTH, RECT_HEIGHT, RGB565_NXPBLUE);
    gfx_drawRect(&s_UiSurface, POS_NXPRED_RECT_X, POS_RECT_Y, RED_RECT_WIDTH, RECT_HEIGHT, RGB565_NXPRED);

    memset(tstring, 0x0, 64);
    sprintf(tstring, "Registered Users:%d", dbCount);
    gfx_drawText(&s_UiSurface, POS_NXPGREEN_RECT_X + REGISTRATION_RELATIVE_X, POS_RECT_Y + REGISTRATION_RELATIVE_Y,
                 RGB565_BLUE, RGB565_NXPGREEN, kFont_OpenSans8, tstring);

    memset(tstring, 0x0, 64);
#ifdef SMART_ACCESS_2D
    sprintf(tstring, "Access 2D");
#else
    sprintf(tstring, "Smart Lock");
#endif
    gfx_drawText(&s_UiSurface, POS_NXPRED_RECT_X + APP_RELATIVE_X, POS_RECT_Y + APP_RELATIVE_Y, RGB565_BLUE,
                 RGB565_NXPRED, kFont_OpenSans8, tstring);

    bool wifiIsConnected = 0; // todo
    if (wifiIsConnected)
    {
        pIcon = (uint16_t *)wifi16x16_data;
    }
    else
    {
        pIcon = (uint16_t *)no_wifi16x16_data;
    }
    gfx_drawPicture(&s_UiSurface, POS_NXPBLUE_RECT_X + WIFI_ICON_RELATIVE_X, POS_RECT_Y + 3 / UI_SCALE_H, WIFI_W,
                    WIFI_H, 0xE000, (char *)pIcon);

    if (s_BleIsOn)
    {
        pIcon = (uint16_t *)bluetooth16x16_data;
    }
    else
    {
        pIcon = (uint16_t *)no_bluetooth16x16_data;
    }
    gfx_drawPicture(&s_UiSurface, POS_NXPBLUE_RECT_X + BLE_ICON_RELATIVE_X, POS_RECT_Y + 3, BLE_W, BLE_H, 0xE000,
                    (char *)pIcon);
    if ((pResult->face_count == 1) && (pResult->face_id >= 0))
    {
        pIcon = (uint16_t *)greenlock_30x38;
    }
    else
    {
        pIcon = (uint16_t *)redlock_30x38;
    }
    gfx_drawPicture(&s_UiSurface, LOCK_SPACING / 2 + POS_NXPRED_RECT_X, POS_RECT_Y - 18, 30, 38, 0xfc00, (char *)pIcon);
}

static hal_output_status_t HAL_OutputDev_UiFfi_Init(const output_dev_t *dev)
{
    hal_output_status_t error = kStatus_HAL_OutputSuccess;

    s_UiSurface.left   = 0;
    s_UiSurface.top    = 0;
    s_UiSurface.right  = UI_BUFFER_WIDTH - 1;
    s_UiSurface.bottom = UI_BUFFER_HEIGHT - 1;
    s_UiSurface.height = UI_BUFFER_HEIGHT;
    s_UiSurface.width  = UI_BUFFER_WIDTH;
    s_UiSurface.pitch  = UI_BUFFER_WIDTH * 2;
    s_UiSurface.format = kPixelFormat_RGB565;
    s_UiSurface.buf    = s_AsBuffer;
    s_UiSurface.lock   = xSemaphoreCreateMutex();

    memset(&s_LastResult, 0x0, sizeof(oasis_lite_result_t));
    s_LogLevel = FWK_ConfigGetLogLevel();
    return error;
}

static hal_output_status_t HAL_OutputDev_UiFfi_Start(const output_dev_t *dev)
{
    hal_output_status_t error = kStatus_HAL_OutputSuccess;
    if (FWK_OutputManager_RegisterEventHandler(dev, &s_OutputDev_UiHandler) != 0)
        error = kStatus_HAL_OutputError;
    return error;
}

static hal_output_status_t HAL_OutputDev_UiFfi_InferComplete(const output_dev_t *dev,
                                                             output_algo_source_t source,
                                                             void *infer_result)
{
    hal_output_status_t error    = kStatus_HAL_OutputSuccess;
    oasis_lite_result_t *pResult = (oasis_lite_result_t *)infer_result;

    if (s_LPMIsOn)
    {
        return error;
    }

    if (source == kOutputAlgoSource_LPM)
    {
        memcpy(s_AsBuffer, sleep_logo_240x320, sizeof(sleep_logo_240x320));
        s_LPMIsOn = 1;
        return error;
    }

    if (pResult != NULL)
    {
        // lock overlay surface to avoid conflict with PXP composing overlay surface
        if (s_UiSurface.lock)
        {
            xSemaphoreTake(s_UiSurface.lock, portMAX_DELAY);
        }

        // draw overlay surface begin====>

        uint8_t new_results =
            memcmp(&s_LastResult, pResult, (sizeof(oasis_lite_result_t) - sizeof(oasis_lite_debug_t)));

        if ((new_results != 0) || (s_RefreshUI))
        {
            /* erase top info */
            _ui_eraseTopInfo(0);
            // clear mainwindow
            _ui_eraseMainWindow(0);

            _ui_drawTopInfo(pResult);
            // draw update info
            _ui_drawMainWindow(pResult);
        }

        if (s_LogLevel >= kLOGLevel_Debug)
        {
            ui_drawDebugWindow(pResult->debug_info);
        }

        _ui_drawBottomInfo(pResult);
        //<====draw overlay surface end

        // unlock overlay surface to avoid conflict with PXP composing overlay
        // surface
        if (s_UiSurface.lock)
        {
            xSemaphoreGive(s_UiSurface.lock);
        }

        memcpy(&s_LastResult, pResult, sizeof(oasis_lite_result_t));
    }

    return error;
}

static hal_output_status_t HAL_OutputDev_UiFfi_InputNotify(const output_dev_t *dev, void *data)
{
    hal_output_status_t error = kStatus_HAL_OutputSuccess;
    event_base_t eventBase    = *(event_base_t *)data;

    if (eventBase.eventId == kEventID_SetBLEConnection)
    {
        event_face_rec_t event = *(event_face_rec_t *)data;
        s_BleIsOn              = event.wuart.status;
    }
    else if (eventBase.eventId == kEventID_SetLogLevel)
    {
        event_common_t event = *(event_common_t *)data;
        s_LogLevel           = event.logLevel.logLevel;
        s_RefreshUI          = true;
    }

    return error;
}

const static output_dev_operator_t s_OutputDev_UiOps = {
    .init   = HAL_OutputDev_UiFfi_Init,
    .deinit = NULL,
    .start  = HAL_OutputDev_UiFfi_Start,
    .stop   = NULL,
};

static output_dev_t s_OutputDev_Ui = {
    .name          = "ui",
    .attr.type     = kOutputDevType_UI,
    .attr.pSurface = &s_UiSurface,
    .ops           = &s_OutputDev_UiOps,
};

/*******************************************************************************
 * Interface
 ******************************************************************************/
int HAL_OutputDev_UiSmartlock_Register()
{
    int error = 0;
    LOGD("output_dev_ui_smartlock_register");
    error = FWK_OutputManager_DeviceRegister(&s_OutputDev_Ui);
    return error;
}
