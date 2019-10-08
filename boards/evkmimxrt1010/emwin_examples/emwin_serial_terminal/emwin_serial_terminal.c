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
/* Select OSC_CLK (24 MHz) as master lpspi clock source */
#define LPSPI_CLOCK_SOURCE_SELECT (0U)
/* Clock divider for master lpspi clock source */
#define LPSPI_CLOCK_SOURCE_DIVIDER (2U)
/* Select OSC_CLK (24 MHz) as master lpi2c clock source */
#define LPI2C_CLOCK_SOURCE_SELECT (1U)
/* Clock divider for master lpi2c clock source */
#define LPI2C_CLOCK_SOURCE_DIVIDER (1U)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

/* Get LPSPI1_CLK_Freq (8MHz).  */
uint32_t LPSPI1_GetFreq(void)
{
    return CLOCK_GetFreq(kCLOCK_OscClk) / (LPSPI_CLOCK_SOURCE_DIVIDER + 1);
}
/* Get LPI2C1_CLK_Freq (12MHz).  */
uint32_t LPI2C1_GetFreq(void)
{
    return CLOCK_GetFreq(kCLOCK_OscClk) / (LPI2C_CLOCK_SOURCE_DIVIDER + 1);
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
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /* Set LPSPI_PODF. */
    CLOCK_SetDiv(kCLOCK_LpspiDiv, LPSPI_CLOCK_SOURCE_DIVIDER);
    /* Set Lpspi clock source. */
    CLOCK_SetMux(kCLOCK_LpspiMux, LPSPI_CLOCK_SOURCE_SELECT);

    /* Set LPI2C_CLK_PODF. */
    CLOCK_SetDiv(kCLOCK_Lpi2cDiv, LPI2C_CLOCK_SOURCE_DIVIDER);
    /* Set Lpi2c clock source. */
    CLOCK_SetMux(kCLOCK_Lpi2cMux, LPI2C_CLOCK_SOURCE_SELECT);

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
