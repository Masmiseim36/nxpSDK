/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "emwin_support.h"

#include "DIALOG.h"
#include "Resource.h"

#include <stdio.h>

#include "pin_mux.h"
#include "fsl_gpio.h"
#include "fsl_gpt.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define APP_PXP PXP
#define EXAMPLE_GPT GPT2
#define EXAMPLE_GPT_TICK_TO_MS 25

#define INDICATOR_SIZE_X 60
#define ALARM_PERIOD 500

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void BOARD_EnableLcdInterrupt(void);

/*******************************************************************************
 * Code
 ******************************************************************************/

/* Initialize the LCD_DISP. */
void BOARD_InitLcd(void)
{
    volatile uint32_t i = 0x100U;

    gpio_pin_config_t config = {
        kGPIO_DigitalOutput,
        0,
    };

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

    /*Clock setting for LPI2C*/
    CLOCK_SetMux(kCLOCK_Lpi2cMux, LPI2C_CLOCK_SOURCE_SELECT);
    CLOCK_SetDiv(kCLOCK_Lpi2cDiv, LPI2C_CLOCK_SOURCE_DIVIDER);
}

void BOARD_InitLcdifPixelClock(void)
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
        .loopDivider = 31,
        .postDivider = 8,
        .numerator   = 0,
        .denominator = 0,
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
    CLOCK_SetMux(kCLOCK_LcdifPreMux, 2);

    CLOCK_SetDiv(kCLOCK_LcdifPreDiv, 4);

    CLOCK_SetDiv(kCLOCK_LcdifDiv, 1);
}

void BOARD_InitGPT(void)
{
    gpt_config_t gptConfig;

    GPT_GetDefaultConfig(&gptConfig);

    gptConfig.enableFreeRun = true;
    gptConfig.divider       = 3000;

    /* Initialize GPT module */
    GPT_Init(EXAMPLE_GPT, &gptConfig);
    GPT_StartTimer(EXAMPLE_GPT);
}



/*********************************************************************
 *
 *       _AnimateWin
 */
static void _AnimateWin(GUI_ANIM_INFO *pInfo, void *pVoid)
{
    int xPos;
    int yPos;
    int xSize;
    ANIM_DATA *pData;
    WINDOW_INFO *pWinInfo;
    int i;

    pData    = (ANIM_DATA *)pVoid;
    pWinInfo = pData->pInfo;
    xSize    = LCD_GetXSize();
    //
    // Itterate over all items, since we need to move all windows.
    //
    for (i = 0; i < pData->NumItems; i++)
    {
        //
        // Depending on the direction we need to calculate differently.
        //
        if (pData->Dir)
        {
            xPos = pWinInfo->xOrg - ((xSize * pInfo->Pos) / GUI_ANIM_RANGE);
        }
        else
        {
            xPos = pWinInfo->xOrg + ((xSize * pInfo->Pos) / GUI_ANIM_RANGE);
        }
        //
        // Update window positions
        //
        yPos = BUTTON_SIZE;
        WM_MoveTo(pWinInfo->hWin, xPos, yPos);
        pWinInfo++;
    }
    if (pInfo->State == GUI_ANIM_END)
    {
        //
        // at the end of an animation we update the info structure.
        //
        pData    = (ANIM_DATA *)pVoid;
        pWinInfo = pData->pInfo;
        for (i = 0; i < pData->NumItems; i++)
        {
            pWinInfo->xOrg = WM_GetWindowOrgX(pWinInfo->hWin);
            pWinInfo++;
        }
    }
}

/*********************************************************************
 *
 *       _AnimateCreate
 */
static void _AnimateCreate(GUI_ANIM_HANDLE *pAnim, ANIM_DATA *pAnimData)
{
    //
    // Set up an animation
    //
    *pAnim = GUI_ANIM_Create(ANIMATION_TIME, 10, (void *)pAnimData, 0);
    GUI_ANIM_AddItem(*pAnim, 0, ANIMATION_TIME, ANIM_ACCELDECEL, (void *)pAnimData, _AnimateWin);
    //
    // Execute the animation once.
    //
    GUI_ANIM_StartEx(*pAnim, 1, NULL);
}

/*********************************************************************
 *
 *       _ButtonSkin
 */
static int _ButtonSkin(const WIDGET_ITEM_DRAW_INFO *pDrawItemInfo)
{
    switch (pDrawItemInfo->Cmd)
    {
        case WIDGET_ITEM_DRAW_BACKGROUND:
            //
            // Make sure the button has no background.
            // So we catch the message and simply do nothing.
            //
            return 0;
        default:
            //
            // Anything else gets done by the default skinning function (like drawing the bitmap)
            //
            return BUTTON_DrawSkinFlex(pDrawItemInfo);
    }
}

/*********************************************************************
 *
 *       _UpdateIndicator
 */
static void _UpdateIndicator(WM_HWIN hWin)
{
    GUI_RECT Rect;
    int xSize;

    //
    // Set up a rectangle and invalidate only the rectangle.
    // This way we just update the area of the indicator and not the whole screen.
    //
    xSize   = LCD_GetXSize();
    Rect.x0 = xSize / 2 - INDICATOR_SIZE_X / 2;
    Rect.y0 = 0;
    Rect.x1 = xSize / 2 + INDICATOR_SIZE_X / 2;
    Rect.y1 = BUTTON_SIZE;
    WM_InvalidateRect(hWin, &Rect);
}

/*********************************************************************
 *
 *       _cbAlarm
 */
static void _cbAlarm(WM_MESSAGE *pMsg)
{
    static int OnOff;
    static WM_HTIMER hTimer;
#if USE_MEMDEV
    int xPos;
    int yPos;
    GUI_COLOR Color;
    static GUI_MEMDEV_Handle hMemBell;
#endif

    switch (pMsg->MsgId)
    {
        case WM_CREATE:
#if USE_MEMDEV
            Color    = COLOR_LEMON;
            hMemBell = CreateMemdevContainer(&bmBell_40x40, &Color);
#endif
            //
            // Create a timer on window creation
            //
            hTimer = WM_CreateTimer(pMsg->hWin, 0, ALARM_PERIOD, 0);
            break;
        case WM_TIMER:
            //
            // If the timer expires, restart it and invalidate this window
            //
            WM_RestartTimer(hTimer, ALARM_PERIOD);
            OnOff ^= 1;
            WM_Invalidate(pMsg->hWin);
            break;
        case WM_PAINT:
            //
            // Clear background
            //
            GUI_SetBkColor(COLOR_BLUE);
            GUI_Clear();
            //
            // Depending on the variable we draw a bell or don't.
            //
            if (OnOff)
            {
#if USE_MEMDEV
                xPos = WM_GetWindowOrgX(pMsg->hWin);
                yPos = WM_GetWindowOrgY(pMsg->hWin);
                GUI_MEMDEV_WriteAt(hMemBell, xPos, yPos);
#else
                GUI_DrawBitmap(&bmBell_40x40, 0, 0);
#endif
            }
            break;
        case WM_TOUCH:
            //
            // If this window gets touched, we delete it.
            //
            WM_DeleteWindow(pMsg->hWin);
#if USE_MEMDEV
            GUI_MEMDEV_Delete(hMemBell);
#endif
            break;
        case WM_DELETE:
            //
            // Clean up on deletion
            //
            WM_DeleteTimer(hTimer);
            hTimer = 0;
            OnOff  = 0;
            break;
        default:
            WM_DefaultProc(pMsg);
            break;
    }
}

/*********************************************************************
 *
 *       _cbBk
 */
static void _cbBk(WM_MESSAGE *pMsg)
{
    WM_HWIN hItem;
    int NCode;
    int Id;
    int xSize;
    int ySize;
    static GUI_ANIM_HANDLE hAnim;
    static ANIM_DATA AnimData;
    static WINDOW_INFO aInfo[3];
    static int Index;
    static int i;
    GUI_COLOR ColorIndex;
    GUI_RECT Rect;
    static WM_HWIN hAlarm;
    WM_MOTION_INFO *pInfo;
    static int xPosMotion;

    switch (pMsg->MsgId)
    {
        case WM_MOTION:
            //
            // get Motion information
            //
            pInfo = (WM_MOTION_INFO *)pMsg->Data.p;
            switch (pInfo->Cmd)
            {
                case WM_MOTION_INIT:
                    //
                    // Set motion flags, tell the motion module that we handle the motion on our own
                    //
                    pInfo->Flags = WM_CF_MOTION_X | WM_MOTION_MANAGE_BY_WINDOW;
                    //
                    // Get client rect and set a snap position
                    //
                    WM_GetClientRectEx(pMsg->hWin, &Rect);
                    pInfo->SnapX = Rect.x1 + 1;
                    break;
                case WM_MOTION_MOVE:
                    //
                    // Set y new x position with delta value we receive from the motion module
                    //
                    xPosMotion += pInfo->dx;
                    xSize = LCD_GetXSize();
//
// Make sure we stop at the min and max positions
//
#if GUI_VERSION < 53800
                    if (xPosMotion < -xSize)
                    {
                        xPosMotion        = -xSize;
                        pInfo->StopMotion = 1;
                    }
                    else if (xPosMotion > xSize)
                    {
                        xPosMotion        = xSize;
                        pInfo->StopMotion = 1;
                    }
#else
                    if (xPosMotion < -xSize)
                    {
                        xPosMotion = -xSize;
                    }
                    else if (xPosMotion > xSize)
                    {
                        xPosMotion = xSize;
                    }
#endif
                    //
                    // update position of all three windows
                    //
                    for (i = 0; i < GUI_COUNTOF(aInfo); i++)
                    {
                        WM_MoveTo(aInfo[i].hWin, xPosMotion + xSize * (-1 + i), BUTTON_SIZE);
                    }
                    //
                    // Manage the final move
                    //
                    if (pInfo->FinalMove == 1)
                    {
                        //
                        // On the final move update the info structure with the new window position.
                        // This is important since we handle the movement in two different ways.
                        //
                        for (i = 0; i < GUI_COUNTOF(aInfo); i++)
                        {
                            aInfo[i].xOrg = WM_GetWindowOrgX(aInfo[i].hWin);
                            if (aInfo[i].xOrg == 0)
                            {
                                //
                                // The window which is located at 0 in x direction is the curretnly shown.
                                // So we set the index to this window.
                                //
                                Index = i;
                            }
                        }
                        //
                        // Update indicator
                        //
                        _UpdateIndicator(pMsg->hWin);
                    }
                    break;
                case WM_MOTION_GETPOS:
                    //
                    // tell the motiuon module where we are now.
                    //
                    pInfo->xPos = xPosMotion;
                    break;
            }
            break;
        case WM_CREATE:
            xSize = LCD_GetXSize();
            ySize = LCD_GetYSize();
            //
            // Button right
            //
            hItem = BUTTON_CreateAsChild(xSize - BUTTON_SIZE, 0, BUTTON_SIZE, BUTTON_SIZE, pMsg->hWin, GUI_ID_BUTTON0,
                                         WM_CF_SHOW);
            BUTTON_SetBitmapEx(hItem, BUTTON_CI_PRESSED, &bmArrowRight_23x24,
                               (BUTTON_SIZE - bmArrowRight_23x24.XSize) / 2,
                               (BUTTON_SIZE - bmArrowRight_23x24.YSize) / 2);
            BUTTON_SetBitmapEx(hItem, BUTTON_CI_UNPRESSED, &bmArrowRight_23x24,
                               (BUTTON_SIZE - bmArrowRight_23x24.XSize) / 2,
                               (BUTTON_SIZE - bmArrowRight_23x24.YSize) / 2);
            BUTTON_SetSkin(hItem, _ButtonSkin);
#if GUI_VERSION >= 54000
            BUTTON_SetFocusable(hItem, 0);
#else
            BUTTON_SetFocussable(hItem, 0);
#endif
            //
            // Button left
            //
            hItem = BUTTON_CreateAsChild(0, 0, BUTTON_SIZE, BUTTON_SIZE, pMsg->hWin, GUI_ID_BUTTON1, WM_CF_SHOW);
            BUTTON_SetBitmapEx(hItem, BUTTON_CI_PRESSED, &bmArrowLeft_23x24,
                               (BUTTON_SIZE - bmArrowLeft_23x24.XSize) / 2,
                               (BUTTON_SIZE - bmArrowLeft_23x24.YSize) / 2);
            BUTTON_SetBitmapEx(hItem, BUTTON_CI_UNPRESSED, &bmArrowLeft_23x24,
                               (BUTTON_SIZE - bmArrowLeft_23x24.XSize) / 2,
                               (BUTTON_SIZE - bmArrowLeft_23x24.YSize) / 2);
            BUTTON_SetSkin(hItem, _ButtonSkin);
#if GUI_VERSION >= 54000
            BUTTON_SetFocusable(hItem, 0);
#else
            BUTTON_SetFocussable(hItem, 0);
#endif
            //
            // Create other views
            //
            aInfo[0].hWin = CreateAlarm(pMsg->hWin, -xSize, BUTTON_SIZE, xSize, ySize - BUTTON_SIZE);
            aInfo[1].hWin = CreateInfo(pMsg->hWin, 0, BUTTON_SIZE, xSize, ySize - BUTTON_SIZE);
            aInfo[2].hWin = CreateWeather(pMsg->hWin, xSize, BUTTON_SIZE, xSize, ySize - BUTTON_SIZE);

            aInfo[0].xOrg     = -xSize;
            aInfo[1].xOrg     = 0;
            aInfo[2].xOrg     = xSize;
            Index             = 1;
            AnimData.pInfo    = aInfo;
            AnimData.NumItems = GUI_COUNTOF(aInfo);
            break;
        case MESSAGE_ALARM:
            //
            // Create alarm indicator
            //
            xSize = LCD_GetXSize();
            hAlarm =
                WM_CreateWindowAsChild(xSize - BUTTON_SIZE - bmBell_40x40.XSize, (BUTTON_SIZE - bmBell_40x40.YSize) / 2,
                                       bmBell_40x40.XSize, bmBell_40x40.YSize, pMsg->hWin, WM_CF_SHOW, _cbAlarm, 0);
            break;
        case MESSAGE_ALARM_STOP:
            //
            // Delete alarm indicator
            //
            if (WM_IsWindow(hAlarm))
            {
                WM_DeleteWindow(hAlarm);
            }
            break;
        case WM_PAINT:
            //
            // Clear back ground with blue
            //
            GUI_SetBkColor(COLOR_BLUE);
            GUI_Clear();
            xSize = LCD_GetXSize();
            //
            // Draw page indicator
            //
            for (i = 0; i < GUI_COUNTOF(aInfo); i++)
            {
                if (i == (unsigned)Index)
                {
                    ColorIndex = COLOR_LEMON;
                }
                else
                {
                    ColorIndex = COLOR_LIGHTBLUE;
                }
                GUI_SetColor(ColorIndex);
                GUI_AA_FillCircle(xSize / 2 + 20 * (-1 + i), BUTTON_SIZE / 2, 5);
            }
            break;
        case WM_NOTIFY_PARENT:
            Id    = WM_GetId(pMsg->hWinSrc);
            NCode = pMsg->Data.v;
            switch (Id)
            {
                case GUI_ID_BUTTON0: // Notifications sent by right button
                    switch (NCode)
                    {
                        case WM_NOTIFICATION_RELEASED:
                            //
                            // Start animation to move windows left
                            //
                            if ((unsigned)Index < GUI_COUNTOF(aInfo) - 1)
                            {
                                AnimData.Dir = 1;
                                _AnimateCreate(&hAnim, &AnimData);
                                Index = ((unsigned)Index < GUI_COUNTOF(aInfo) - 1) ? (unsigned)Index + 1 :
                                                                                     GUI_COUNTOF(aInfo) - 1;
                                //
                                // Update indicator
                                //
                                _UpdateIndicator(pMsg->hWin);
                                //
                                // Update x position used for motion module
                                //
                                xSize      = LCD_GetXSize();
                                xPosMotion = xSize * (1 - Index);
                            }
                            break;
                    }
                    break;
                case GUI_ID_BUTTON1: // Notifications sent by left button
                    switch (NCode)
                    {
                        case WM_NOTIFICATION_RELEASED:
                            //
                            // Start animation to move windows left
                            //
                            if (Index > 0)
                            {
                                AnimData.Dir = 0;
                                _AnimateCreate(&hAnim, &AnimData);
                                Index = (Index > 0) ? Index - 1 : 0;
                                //
                                // Update indicator
                                //
                                _UpdateIndicator(pMsg->hWin);
                                //
                                // Update x position used for motion module
                                //
                                xSize      = LCD_GetXSize();
                                xPosMotion = xSize * (1 - Index);
                            }
                            break;
                    }
                    break;
            }
            break;
        default:
            WM_DefaultProc(pMsg);
            break;
    }
}

/*********************************************************************
 *
 *       Public code
 *
 **********************************************************************
 */
#if USE_MEMDEV
/*********************************************************************
 *
 *       CreateMemdevContainer
 */
GUI_MEMDEV_Handle CreateMemdevContainer(const GUI_BITMAP *pBitmap, GUI_COLOR *pColor)
{
    GUI_MEMDEV_Handle hMem;
    GUI_MEMDEV_Handle hMemOld;

    hMem    = GUI_MEMDEV_CreateFixed32(0, 0, pBitmap->XSize, pBitmap->YSize);
    hMemOld = GUI_MEMDEV_Select(hMem);
    GUI_SetBkColor(GUI_TRANSPARENT);
    GUI_Clear();
    if (pColor)
    {
        GUI_SetColor(*pColor);
    }
    GUI_DrawBitmap(pBitmap, 0, 0);
    GUI_MEMDEV_Select(hMemOld);
    return hMem;
}
#endif

/*!
 * @brief Main function
 */
int main(void)
{
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_InitI2C1Pins();
    BOARD_InitSemcPins();
    BOARD_BootClockRUN();
    BOARD_InitLcdifPixelClock();
    BOARD_InitDebugConsole();
    BOARD_InitLcd();
    BOARD_InitGPT();

    int xSize;
    int ySize;

    GUI_Init();
    //
    // Enable multibuffering, this has no effect in the simulation.
    //
    WM_MULTIBUF_Enable(1);
    //
    // Enable the motion support
    //
    WM_MOTION_Enable(1);
    //
    // Get screen dimension
    //
    xSize = LCD_GetXSize();
    ySize = LCD_GetYSize();
    //
    // Create a window and make it moveable in x direction (WM_CF_MOTION_X)
    //
    WM_CreateWindowAsChild(0, 0, xSize, ySize, WM_HBKWIN, WM_CF_SHOW | WM_CF_MOTION_X, _cbBk, 0);
    WM_Exec();

    while (1)
    {
        /* Poll touch controller for update */
        if (BOARD_Touch_Poll())
        {
            GUI_Delay(25);
        }
    }
}
