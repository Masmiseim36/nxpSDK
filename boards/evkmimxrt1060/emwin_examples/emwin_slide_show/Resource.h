/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*                        The Embedded Experts                        *
**********************************************************************
*                                                                    *
*       (c) 2003 - 2016  SEGGER Microcontroller GmbH & Co. KG        *
*                                                                    *
*       www.segger.com     Support: support@segger.com               *
*                                                                    *
**********************************************************************
----------------------------------------------------------------------
File    : Resource.h
Purpose : Includes required resources.
--------  END-OF-HEADER  ---------------------------------------------
*/
#ifndef RESOURCE_H
#define RESOURCE_H

#include "DIALOG.h"

#include <stdio.h>

/*********************************************************************
 *
 *       Configuration defines
 *
 **********************************************************************
 */
#define USE_MEMDEV 1

/*********************************************************************
 *
 *       Externals
 *
 **********************************************************************
 */
extern GUI_CONST_STORAGE GUI_BITMAP bmNXP_110x44;
extern GUI_CONST_STORAGE GUI_BITMAP bmSeggerLogo_110x55;

extern GUI_CONST_STORAGE GUI_BITMAP bmBell_40x40;
extern GUI_CONST_STORAGE GUI_BITMAP bmBell_60x60;

extern GUI_CONST_STORAGE GUI_BITMAP bmArrowLeft_23x24;
extern GUI_CONST_STORAGE GUI_BITMAP bmArrowRight_23x24;

// extern GUI_CONST_STORAGE GUI_BITMAP bmRainy_45x34;
// extern GUI_CONST_STORAGE GUI_BITMAP bmStormy_45x34;
// extern GUI_CONST_STORAGE GUI_BITMAP bmSunny_45x34;

extern GUI_BITMAP bmRainy_45x34;
extern GUI_BITMAP bmStormy_45x34;
extern GUI_BITMAP bmSunny_45x34;

extern GUI_CONST_STORAGE GUI_FONT GUI_FontAA4_B_20;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontAA4_B_40;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontAA4_B_100;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font_13_R;

/*********************************************************************
 *
 *       Defines
 *
 **********************************************************************
 */
#define FLYIN 0
#define RESIZE 1
#define ANIMATION FLYIN

#define QR_PIXEL_SIZE 3
#define OFFSET 60

#define BUTTON_SIZE 60 // Button are quadratic, one size is sufficient
#define FRAME_SIZEX 240
#define FRAME_SIZEY 110

#define LISTWHEEL_SIZEX 90

#define BORDER_SIZE 10

#define ANIM_DIR_UP 0x01
#define ANIM_DIR_DOWN 0x02
#define ANIM_DIR_RIGHT 0x04
#define ANIM_DIR_LEFT 0x08
#define ANIM_RESIZE 0x10

#define TIMER_ID0 (WM_USER + 0)
#define TIMER_ID1 (WM_USER + 1)

#define MESSAGE_ALARM (WM_USER + 2)
#define MESSAGE_ALARM_STOP (WM_USER + 3)

#define COLOR_LEMON GUI_MAKE_COLOR(0x0014b6a9)
#define COLOR_BLUE GUI_MAKE_COLOR(0x00623700)
#define COLOR_LIGHTBLUE GUI_MAKE_COLOR(0x00AA7D67)
#define COLOR_RED GUI_MAKE_COLOR(0x002196EF)
#define COLOR_WHITE GUI_MAKE_COLOR(0x00F2F2F2)
#define COLOR_LIGHTGRAY GUI_MAKE_COLOR(0x00C0C0C0)
#define COLOR_DARKGRAY GUI_MAKE_COLOR(0x00404040)
#define COLOR_BLACK GUI_MAKE_COLOR(0x00000000)

#define COLOR_BG COLOR_BLUE
#define COLOR_BUTTON COLOR_LEMON
#define COLOR_PAGEIDC_0 COLOR_LIGHTBLUE
#define COLOR_PAGEIDC_1 COLOR_LEMON
#define COLOR_BELL_0 COLOR_LEMON
#define COLOR_BELL_1 COLOR_BLUE
#define COLOR_START_0 COLOR_BLUE
#define COLOR_START_1 COLOR_LEMON
#define COLOR_WHEEL_TEXT COLOR_LEMON
#define COLOR_WHEEL_BG COLOR_DARKGRAY
#define COLOR_WHEEL_FRAME COLOR_LIGHTBLUE
#define COLOR_ALARM_TEXT COLOR_LEMON
#define COLOR_SPLINE_BG COLOR_RED
#define COLOR_SPLINE_FG COLOR_WHITE
#define COLOR_CURRENT_BG COLOR_WHITE
#define COLOR_CURRENT_TX COLOR_DARKGRAY
#define COLOR_TOP_BG COLOR_LEMON
#define COLOR_TOP_FG COLOR_DARKGRAY
#define COLOR_SMALL_0 COLOR_BLUE
#define COLOR_SMALL_1 COLOR_WHITE
#define COLOR_SMALL_2 COLOR_DARKGRAY
#define COLOR_ANIM_TEXT COLOR_LEMON
#define COLOR_RISE_TEXT COLOR_WHITE
#define COLOR_SMALL_TEXT COLOR_RED

#define ANIMATION_TIME 300

/*********************************************************************
 *
 *       Types
 *
 **********************************************************************
 */
typedef struct
{
    WM_HWIN hWin;
    int xOrg;
} WINDOW_INFO;

typedef struct
{
    WINDOW_INFO *pInfo;
    int NumItems;
    int Dir;
    int xPos;
} ANIM_DATA;

/*********************************************************************
 *
 *       Prototypes
 *
 **********************************************************************
 */
WM_HWIN CreateAlarm(WM_HWIN hParent, int x0, int y0, int xSize, int ySize);
WM_HWIN CreateInfo(WM_HWIN hParent, int x0, int y0, int xSize, int ySize);
WM_HWIN CreateWeather(WM_HWIN hParent, int x0, int y0, int xSize, int ySize);
GUI_MEMDEV_Handle CreateMemdevContainer(const GUI_BITMAP *pBitmap, GUI_COLOR *pColor);

#endif

/****** End of File *************************************************/
