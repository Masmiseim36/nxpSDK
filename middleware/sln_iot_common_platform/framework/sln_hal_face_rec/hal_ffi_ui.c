/*
 * Copyright 2019-2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
 * @brief ui device implementation.
 */

#include <stdio.h>

#include "board_define.h"

#include "fonts/font.h"
#include "icons/welcomehome_320x122.h"

#include "fwk_log.h"
#include "fwk_perf.h"
#include "fwk_graphics.h"
#include "fwk_output_manager.h"
#include "hal_output_dev.h"
#include "hal_vision_algo_oasis_lite.h"
#include "hal_event_descriptor_face_rec.h"
#include "hal_sln_timer.h"
#include "semphr.h"

#if defined(__cplusplus)
extern "C" {
#endif
int HAL_OutputDev_UiFfi_Register();
#if defined(__cplusplus)
}
#endif

/*******************************************************************************
 * Defines
 ******************************************************************************/
#define UI_BUFFER_BPP   2
#define UI_BUFFER_PITCH (UI_BUFFER_WIDTH * UI_BUFFER_BPP)
#define UI_SCALE_W      (480 / UI_BUFFER_WIDTH)
#define UI_SCALE_H      (640 / UI_BUFFER_HEIGHT)

#define RGB565_RED   0xf800
#define RGB565_GREEN 0x07e0
#define RGB565_BLUE  0x001f
#define RGB565_BLACK 0x0001

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
#define UI_BOTTOMINFO_H 40
#define UI_MAINWINDOW_W UI_BUFFER_WIDTH
#define UI_MAINWINDOW_H (UI_BUFFER_HEIGHT - UI_TOPINFO_H - UI_BOTTOMINFO_H - UI_TOPINFO_Y)
#define UI_MAINWINDOW_X 0
#define UI_MAINWINDOW_Y (UI_TOPINFO_Y + UI_TOPINFO_H)
#define UI_BOTTOMINFO_X 0
#define UI_BOTTOMINFO_Y (UI_MAINWINDOW_Y + UI_MAINWINDOW_H)

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

/*******************************************************************************
 * Internal Functions
 ******************************************************************************/
#if FWK_PERF
static void _draw_fps()
{
    char fpsStr[32];
    float camerafps  = fwk_get_fps(kFWKFPSType_Camera, 0);
    float displayfps = fwk_get_fps(kFWKFPSType_Display, 0);
    float algofps    = fwk_get_fps(kFWKFPSType_VAlgo, 0);
    snprintf(fpsStr, sizeof(fpsStr), "fps: c:%2d.%1d  d:%2d.%1d  a:%2d.%1d", (int)camerafps,
             (int)(((int)(camerafps * 10)) - (((int)camerafps) * 10)), (int)displayfps,
             (int)(((int)(displayfps * 10)) - (((int)displayfps) * 10)), (int)algofps,
             (int)(((int)(algofps * 10)) - (((int)algofps) * 10)));
    gfx_drawText(&s_UiSurface, 5, UI_BUFFER_HEIGHT - 32, RGB565_GREEN, 0x0, (int)kFont_OpenSans16, fpsStr);
}
#else
#define _draw_fps()
#endif

static void _draw_face_box(int x1, int x2, int y1, int y2, int dim, int color)
{
    // draw the face box
    if (((x1 >= 0) && (x2 < UI_BUFFER_WIDTH) && (x2 > 0)) && ((y1 >= 0) && (y2 < UI_BUFFER_HEIGHT) && (y2 > 0)))
    {
        // LOGD("box: %d, %d, %d, %d\r\n", x1, y1, x2, y2);
        gfx_drawRect(&s_UiSurface, x1, y1, x2 - x1, dim, color);
        gfx_drawRect(&s_UiSurface, x1, y1, dim, y2 - y1, color);
        gfx_drawRect(&s_UiSurface, x2, y1, dim, y2 - y1, color);
        gfx_drawRect(&s_UiSurface, x1, y2, x2 - x1, dim, color);
    }
}

static void _ui_eraseTopInfo()
{
    memset(s_UiSurface.buf + UI_TOPINFO_Y * UI_BUFFER_PITCH, 0x0, (UI_BUFFER_PITCH * UI_TOPINFO_H));
}

static void _ui_drawTopInfo(oasis_lite_state_t state, uint32_t result)
{
    char txt[64];
    int bg_color = RGB565_GREEN;

    if (state == kOASISLiteState_Recognition)
    {
        _ui_eraseTopInfo();
        return;
    }

    memset(txt, 0x0, 64);
    switch (state)
    {
        case kOASISLiteState_Registration:
        {
            switch (result)
            {
                case kOASISLiteRegistrationResult_Invalid:
                {
                    sprintf(txt, "Registering");
                    bg_color = RGB565_GREEN;
                }
                break;

                case kOASISLiteRegistrationResult_Success:
                {
                    sprintf(txt, "Registered Success");
                    bg_color = RGB565_GREEN;
                }
                break;

                case kOASISLiteRegistrationResult_Duplicated:
                {
                    sprintf(txt, "Already Exist");
                    bg_color = RGB565_RED;
                }
                break;

                case kOASISLiteRegistrationResult_Timeout:
                {
                    sprintf(txt, "Registration Timeout");
                    bg_color = RGB565_RED;
                }
                break;

                default:
                    break;
            }
        }
        break;

        case kOASISLiteState_DeRegistration:
        {
            switch (result)
            {
                case kOASISLiteDeregistrationResult_Invalid:
                {
                    sprintf(txt, "Deregistering");
                    bg_color = RGB565_GREEN;
                }
                break;

                case kOASISLiteDeregistrationResult_Success:
                {
                    sprintf(txt, "Deregistering Success");
                    bg_color = RGB565_GREEN;
                }
                break;

                case kOASISLiteDeregistrationResult_Timeout:
                {
                    sprintf(txt, "Deregistration Timeout");
                    bg_color = RGB565_RED;
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

    _ui_eraseTopInfo();
    gfx_drawText(&s_UiSurface, (UI_TOPINFO_X + (UI_MAINWINDOW_W / 2 - 100) / UI_SCALE_W), UI_TOPINFO_Y, 0x0, bg_color,
                 (int)kFont_OpenSans16, txt);
}

static void _ui_eraseMainWindow()
{
    memset(s_UiSurface.buf + UI_MAINWINDOW_Y * UI_BUFFER_PITCH, 0x0, (UI_BUFFER_PITCH * UI_MAINWINDOW_H));
}

static void _ui_drawMainWindow(oasis_lite_result_t *pResult, int erase)
{
    int green_color = RGB565_GREEN;
    int red_color   = RGB565_RED;

    if (erase)
    {
        green_color = 0x0;
        red_color   = 0x0;
    }

    if ((pResult->state == kOASISLiteState_Registration) &&
        (pResult->reg_result != kOASISLiteRegistrationResult_Invalid))
    {
        return;
    }
    else if ((pResult->state == kOASISLiteState_DeRegistration) &&
             (pResult->dereg_result != kOASISLiteDeregistrationResult_Invalid))
    {
        return;
    }

    if (pResult->face_count == 1)
    {
        // LOGD("UI:FACE[%d]:[%s]:[%d,%d,%d,%d]\r\n", pResult->face_id, pResult->name,
        //     pResult->face_box.rect[0], pResult->face_box.rect[1], pResult->face_box.rect[2],
        //     pResult->face_box.rect[3]);
        int x1 = pResult->face_box.rect[0] / UI_SCALE_W;
        int y1 = pResult->face_box.rect[1] / UI_SCALE_H;
        int x2 = pResult->face_box.rect[2] / UI_SCALE_W;
        int y2 = pResult->face_box.rect[3] / UI_SCALE_H;

        if (y1 < UI_MAINWINDOW_Y)
        {
            y1 = UI_MAINWINDOW_Y + 1;
        }
        if (y2 > UI_BOTTOMINFO_Y)
        {
            y2 = UI_BOTTOMINFO_Y - 1;
        }

        if (pResult->face_recognized)
        {
            // rec face
            if (pResult->face_id >= 0)
            {
                // known face
                gfx_drawText(&s_UiSurface, x1 + 1, y1 + 1, green_color, 0x0, (int)kFont_OpenSans16, pResult->name);
                _draw_face_box(x1, x2, y1, y2, 2, green_color);
            }
            else
            {
                gfx_drawText(&s_UiSurface, x1 + 1, y1 + 1, red_color, 0x0, (int)kFont_OpenSans16, "unknown");
                _draw_face_box(x1, x2, y1, y2, 2, red_color);
            }
        }
        else
        {
            gfx_drawText(&s_UiSurface, x1 + 1, y1 + 1, red_color, 0x0, (int)kFont_OpenSans16, "unqualified");
            _draw_face_box(x1, x2, y1, y2, 2, red_color);
        }
    }
}

static void _ui_eraseBottomInfo()
{
    memset(s_UiSurface.buf + UI_BOTTOMINFO_Y * UI_BUFFER_PITCH, 0x0, (UI_BUFFER_PITCH * UI_BOTTOMINFO_H));
}

static void _ui_drawBottomInfo()
{
    _ui_eraseBottomInfo();
    _draw_fps();
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

    if (pResult != NULL)
    {
        // lock overlay surface to avoid conflict with PXP composing overlay surface
        if (s_UiSurface.lock)
        {
            xSemaphoreTake(s_UiSurface.lock, portMAX_DELAY);
        }

        // draw overlay surface begin====>
        _ui_drawTopInfo(pResult->state, pResult->reg_result);

        if (memcmp(&s_LastResult, pResult, sizeof(oasis_lite_result_t)) != 0)
        {
            // clear mainwindow
            _ui_drawMainWindow(&s_LastResult, 1);
            // draw update info
            _ui_drawMainWindow(pResult, 0);
        }

        _ui_drawBottomInfo();
        //<====draw overlay surface end

        // unlock overlay surface to avoid conflict with PXP composing overlay surface
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

    if (kEventFaceRecID_AddUser == eventBase.eventId)
    {
        // todo
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
int HAL_OutputDev_UiFfi_Register()
{
    int error = 0;
    LOGD("HAL_OutputDev_UiFfi_Register");
    error = FWK_OutputManager_DeviceRegister(&s_OutputDev_Ui);
    return error;
}
