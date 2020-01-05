/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*  Standard C Included Files */
#include <stdio.h>
#include <string.h>
#include "board.h"
#include "fsl_debug_console.h"
#include "emwin_support.h"

#include "GUI.h"
#include "GUIDRV_Lin.h"
#include "BUTTON.h"

#include "pin_mux.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

uint32_t DSPI0_GetFreq(void)
{
    return CLOCK_GetBusClkFreq();
}

uint32_t I2C0_GetFreq(void)
{
    return CLOCK_GetFreq(I2C0_CLK_SRC);
}

uint32_t I2C1_GetFreq(void)
{
    return CLOCK_GetFreq(I2C1_CLK_SRC);
}

int main(void)
{
    int termWidth;
    int termHeight;

    int charWidth;
    int lineHeight;

    int c      = 0;
    int c_prev = 0;

    /* Board pin, clock, debug console init */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /* emWin start */
    GUI_Init();

    WM_SetSize(WM_HBKWIN, LCD_WIDTH, LCD_HEIGHT);

    /* Solid color display */
    GUI_SetBkColor(GUI_WHITE);
    GUI_Clear();

    GUI_MULTIBUF_Begin();
    GUI_Exec();
    GUI_MULTIBUF_End();

    GUI_SetColor(GUI_BLACK);

    termWidth  = WM_GetWindowSizeX(0);
    termHeight = WM_GetWindowSizeY(0);

    GUI_SetFont(GUI_FONT_8X16);
    lineHeight = GUI_GetFontDistY();
    charWidth  = GUI_GetCharDistX('_');

    GUI_DispString("Waiting for data - connect using serial terminal\n>");

    for (;;)
    {
        /* show cursor */
        GUI_MULTIBUF_Begin();
        GUI_DispChar('_');
        GUI_GotoX(GUI_GetDispPosX() - charWidth);
        GUI_MULTIBUF_End();

        c = GETCHAR();
        if (c == '\r')
        {
            /* translate CR to LF */
            c_prev = c;
            c      = '\n';
        }
        else if (c == '\n' && c_prev == '\r')
        {
            /* ignore LF following CR */
            c_prev = c;
            continue;
        }
        c_prev = c;

        /* hide cursor */
        GUI_MULTIBUF_Begin();
        GUI_DispChar(' ');
        GUI_GotoX(GUI_GetDispPosX() - charWidth);

        GUI_DispChar(c);

        if (GUI_GetDispPosX() >= termWidth)
        {
            GUI_DispNextLine();
        }

        if (GUI_GetDispPosY() + lineHeight > termHeight)
        {
            GUI_Clear();
            GUI_GotoY(0);
        }
        GUI_MULTIBUF_End();
    }
}
