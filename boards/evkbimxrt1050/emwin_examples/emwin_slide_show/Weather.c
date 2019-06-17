/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2017  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

***** emWin - Graphical user interface for embedded applications *****
emWin is protected by international copyright laws.   Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with a license and should not be re-
distributed in any way. We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : Weather.c
Purpose     : Weather forecast demo
----------------------------------------------------------------------
*/
#include "Resource.h"

/*********************************************************************
 *
 *       Defines
 *
 **********************************************************************
 */
#define ID_WINDOW_0 (GUI_ID_USER + 0x00)

#define TEMP_OFFSET 20
#define TEMP_MULTIPLY 3

/*********************************************************************
 *
 *       Types
 *
 **********************************************************************
 */
typedef struct
{
    unsigned xSize;
    GUI_HMEM hResult;
    I16 ay[1];
} GUI_SPLINE;

/*********************************************************************
 *
 *       Static data
 *
 **********************************************************************
 */
/*********************************************************************
 *
 *       _aDialogCreate
 */
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    {WINDOW_CreateIndirect, "Window", ID_WINDOW_0, 0, 0, 480, 212, 0, 0x0, 0},
    {BUTTON_CreateIndirect, "Button", GUI_ID_BUTTON0, 195, 15, 90, 90, 0, 0x0, 0},
    {BUTTON_CreateIndirect, "Button", GUI_ID_BUTTON1, 290, 15, 90, 90, 0, 0x0, 0},
    {BUTTON_CreateIndirect, "Button", GUI_ID_BUTTON2, 385, 15, 90, 90, 0, 0x0, 0},
};

typedef struct
{
    const GUI_BITMAP *pBitmap;
    char *pWeather;
    char *pDay;
    char *pTemp;
    char *pHumi;
} FORECAST;

const int axi[] = {0, 20, 40, 60, 80, 100, 120, 140, 160, 180, 200, 220, 240};

const int ayi[3][13] = {{8, 7, 6, 7, 8, 11, 14, 16, 18, 16, 13, 9, 7},
                        {7, 7, 6, 6, 8, 11, 14, 15, 15, 14, 13, 8, 7},
                        {7, 6, 5, 4, 5, 8, 9, 12, 14, 12, 11, 8, 6}};

static FORECAST _aForecast[] = {
    {&bmSunny_45x34, "Sunny intervals", "Monday",
     "18"
     "\xc2\xb0"
     "C",
     "30%"},
    {&bmSunny_45x34, "Sunny intervals", "Monday",
     "18"
     "\xc2\xb0"
     "C",
     "30%"},
    {&bmRainy_45x34, "Rain", "Tuesday",
     "16"
     "\xc2\xb0"
     "C",
     "90%"},
    {&bmStormy_45x34, "Thunderstorm", "Wednesday",
     "14"
     "\xc2\xb0"
     "C",
     "80%"},
};

/*********************************************************************
 *
 *       Static code
 *
 **********************************************************************
 */
/*********************************************************************
 *
 *       _cbWinCurrent
 */
static void _cbWinPreview(WM_MESSAGE *pMsg)
{
    GUI_RECT Rect;
    int Index;
    U32 i;
    U32 j;
    int x0, y0;
    WM_HWIN hWinFocus;
    static GUI_MEMDEV_Handle ahMem[3];
    GUI_HMEM hSpline;
    U32 NumPoints;
    int aTemp[3][25];

    switch (pMsg->MsgId)
    {
        case WM_CREATE:
            for (i = 0; i < GUI_COUNTOF(ayi); i++)
            {
                for (j = 0; j < GUI_COUNTOF(ayi[i]); j++)
                {
                    aTemp[i][j] = ayi[i][j] * TEMP_MULTIPLY * -1;
                }
            }
            //
            // On window creation create three memory devices
            //
            for (j = 0; j < GUI_COUNTOF(ahMem); j++)
            {
                //
                // Since WM_GetClientRect() is not available yet, we noeed to do it on our own.
                //
                Rect.x0 = 0;
                Rect.y0 = 0;
                Rect.x1 = WM_GetWindowSizeX(pMsg->hWin);
                Rect.y1 = WM_GetWindowSizeY(pMsg->hWin);
                //
                // Create memory device
                //
                ahMem[j] = GUI_MEMDEV_Create(Rect.x0, Rect.y0, Rect.x1, Rect.y1);
                //
                // Select memory device and draw
                //
                GUI_MEMDEV_Select(ahMem[j]);
                //
                // Fill with color
                //
                GUI_SetBkColor(COLOR_SPLINE_BG);
                GUI_Clear();
                //
                // Draw the graph
                //
                GUI_SetColor(COLOR_SPLINE_FG);
                GUI_SetFont(&GUI_Font_13_R);
                Rect.y1 -= TEMP_OFFSET;
                NumPoints = GUI_COUNTOF(axi);
                hSpline   = GUI_SPLINE_Create(axi, aTemp[j], NumPoints);
                GUI_SPLINE_DrawAA(hSpline, 20, Rect.y1, 0);
                GUI_SPLINE_Delete(hSpline);
                //
                // Draw scale
                //
                GUI_DrawVLine(TEMP_OFFSET, 10, Rect.y1 + 5);
                GUI_DrawHLine(Rect.y1, BORDER_SIZE, Rect.x1 - BORDER_SIZE);
                GUI_DrawHLine(Rect.y1 - BORDER_SIZE * TEMP_MULTIPLY, 15, TEMP_OFFSET);
                GUI_DispStringAt("10", BORDER_SIZE / 2, Rect.y1 - BORDER_SIZE * TEMP_MULTIPLY - 7);
                GUI_DispStringAt(
                    "\xc2\xb0"
                    "C",
                    BORDER_SIZE / 2, BORDER_SIZE / 2);
                for (i = 0; i < GUI_COUNTOF(axi); i++)
                {
                    x0 = TEMP_OFFSET + axi[i];
                    //
                    // Draw a vertical line every second point
                    //
                    GUI_DrawLine(x0, Rect.y1, x0, Rect.y1 + BORDER_SIZE / 2);
                }
                //
                // Set up text rect
                //
                Rect.x0 += BORDER_SIZE / 2;
                Rect.y0 += BORDER_SIZE / 2;
                Rect.x1 -= BORDER_SIZE / 3;
                Rect.y1 += 15;
                //
                // Name the scale
                //
                GUI_DispStringInRect("h", &Rect, GUI_TA_RIGHT | GUI_TA_BOTTOM);
                GUI_DispStringInRect("12", &Rect, GUI_TA_HCENTER | GUI_TA_BOTTOM);
            }
            //
            // Deselect memory device, this is important, otherwise any further drawing
            // operations would be performed into the selected device.
            //
            GUI_MEMDEV_Select(0);
            break;
        case WM_PAINT:
            //
            // Get the currently focused Button and calculate an index with its Id
            //
            hWinFocus = WM_GetFocusedWindow();
            Index     = WM_GetId(hWinFocus) - GUI_ID_BUTTON0;
            x0        = WM_GetWindowOrgX(pMsg->hWin);
            y0        = WM_GetWindowOrgY(pMsg->hWin);
            //
            // Since we already filled the memory devices, we just have to show them.
            // This safes a lot of performance because we don't need to perform any AA drawings anymore.
            // Drawing a memory device is nothing more than drawing a bitmap.
            //
            GUI_MEMDEV_WriteAt(ahMem[Index], x0, y0);
            break;
        default:
            WM_DefaultProc(pMsg);
            break;
    }
}

/*********************************************************************
 *
 *       _MemdevChangeColor
 */
static void _MemdevChangeColor(GUI_MEMDEV_Handle hMem, GUI_COLOR Color)
{
    U32 *pData;
    U32 *pWork;
    int xSize;
    int ySize;
    int x;
    int y;

    xSize = GUI_MEMDEV_GetXSize(hMem);
    ySize = GUI_MEMDEV_GetYSize(hMem);
    pData = (U32 *)GUI_MEMDEV_GetDataPtr(hMem);
    y     = 0;
    while (y < ySize)
    {
        x = 0;
        while (x < xSize)
        {
            pWork = pData + (y * xSize) + x;
            *pWork &= ~0x00FFFFFF;
            *pWork |= (Color & 0x00FFFFFF);
            x++;
        }
        y++;
    }
}

/*********************************************************************
 *
 *       _cbWinCurrent
 */
static void _cbWinCurrent(WM_MESSAGE *pMsg)
{
    GUI_RECT Rect;
#if USE_MEMDEV
    int xPos;
    int yPos;
    GUI_COLOR Color;
    static GUI_MEMDEV_Handle hMem;
#endif

    switch (pMsg->MsgId)
    {
#if USE_MEMDEV
        case WM_CREATE:
            Color = COLOR_WHITE;
            hMem  = CreateMemdevContainer(_aForecast[0].pBitmap, &Color);
            break;
#endif
        case WM_PAINT:
            //
            // Fill background with white
            //
            GUI_SetBkColor(COLOR_WHITE);
            GUI_Clear();
            //
            // Get the client rectangle
            //
            WM_GetClientRect(&Rect);
            GUI_SetColor(COLOR_LEMON);
            //
            // Fill a rectangel at the top of the window
            //
            GUI_FillRect(Rect.x0, Rect.y0, Rect.x1, _aForecast[0].pBitmap->YSize);
            //
            // Set other foreground and background colors and modify the client rectangle
            //
            GUI_SetColor(COLOR_WHITE);
            GUI_SetBkColor(COLOR_LEMON);
            Rect.x0 += BORDER_SIZE / 2;
            Rect.y1 = _aForecast[0].pBitmap->YSize;
            GUI_SetFont(&GUI_FontAA4_B_20);
            //
            // Display a string at the top and draw an alpha bitmap. Alpha bitmaps have the
            // advantage that they can be drawn with a set color and are quite small.
            //
            GUI_DispStringInRect(_aForecast[0].pWeather, &Rect, GUI_TA_LEFT | GUI_TA_VCENTER);
#if USE_MEMDEV
            xPos = WM_GetWindowOrgX(pMsg->hWin) + 180 - 45;
            yPos = WM_GetWindowOrgY(pMsg->hWin);
            GUI_MEMDEV_WriteAt(hMem, xPos, yPos);
#else
            GUI_DrawBitmap(_aForecast[0].pBitmap, 180 - 45, 0);
#endif
            //
            // Get the client area again since we have modified the las received rectangle
            //
            WM_GetClientRect(&Rect);
            Rect.x0 += BORDER_SIZE / 2;
            Rect.y0 += _aForecast[0].pBitmap->YSize + 1;
            Rect.y1 -= BORDER_SIZE * 2;
            GUI_SetBkColor(COLOR_WHITE);
            GUI_SetColor(GUI_DARKGRAY);
            GUI_SetFont(&GUI_FontAA4_B_40);
            //
            // Display information about the day, temperature and rainfall probability
            //
            GUI_DispStringInRect(_aForecast[0].pDay, &Rect, GUI_TA_LEFT | GUI_TA_TOP);
            GUI_DispStringInRect(_aForecast[0].pTemp, &Rect, GUI_TA_LEFT | GUI_TA_VCENTER);
            GUI_DispStringInRect(_aForecast[0].pHumi, &Rect, GUI_TA_LEFT | GUI_TA_BOTTOM);
            break;
#if USE_MEMDEV
        case WM_DELETE:
            GUI_MEMDEV_Delete(hMem);
            break;
#endif
        default:
            WM_DefaultProc(pMsg);
            break;
    }
}

/*********************************************************************
 *
 *       _cbButton
 */
static void _cbButton(WM_MESSAGE *pMsg)
{
    GUI_RECT Rect;
    GUI_COLOR ColorBK;
    GUI_COLOR ColorFG;
    int Id;
    int Index;
#if USE_MEMDEV
    int xPos;
    int yPos;
    GUI_MEMDEV_Handle *pMem;
#endif

    switch (pMsg->MsgId)
    {
        case WM_PAINT:
            //
            // Depending on if the button has the focus or not we chosse different colors
            //
            if (WM_HasFocus(pMsg->hWin))
            {
                ColorBK = COLOR_LEMON;
                ColorFG = COLOR_WHITE;
            }
            else
            {
                ColorBK = COLOR_WHITE;
                ColorFG = GUI_DARKGRAY;
            }
            //
            // Clear background
            //
            GUI_SetBkColor(ColorBK);
            GUI_Clear();
            //
            // Get the Id of the button to calculate an index
            //
            Id    = WM_GetId(pMsg->hWin);
            Index = Id - GUI_ID_BUTTON0 + 1;
            //
            // Use the set foreground color to draw alpha bitmap
            //
            GUI_SetColor(ColorFG);
#if USE_MEMDEV
            WM_GetUserData(pMsg->hWin, &pMem, sizeof(void *));
            xPos = WM_GetWindowOrgX(pMsg->hWin) + 45;
            yPos = WM_GetWindowOrgY(pMsg->hWin);
            _MemdevChangeColor(*pMem, ColorFG);
            GUI_MEMDEV_WriteAt(*pMem, xPos, yPos);
#else
            GUI_DrawBitmap(_aForecast[Index].pBitmap, 45, 0);
#endif
            //
            // Get the client rectangle and modify it
            //
            WM_GetClientRect(&Rect);
            Rect.y0 += _aForecast[Index].pBitmap->YSize;
            Rect.x0 += BORDER_SIZE / 2;
            //
            // Display strings with the font GUI_FontAA4_B_20
            //
            GUI_SetFont(&GUI_FontAA4_B_20);
            GUI_DispStringInRect(_aForecast[Index].pDay, &Rect, GUI_TA_LEFT | GUI_TA_TOP);
            GUI_DispStringInRect(_aForecast[Index].pTemp, &Rect, GUI_TA_LEFT | GUI_TA_VCENTER);
            GUI_DispStringInRect(_aForecast[Index].pHumi, &Rect, GUI_TA_LEFT | GUI_TA_BOTTOM);
            break;
        default:
            BUTTON_Callback(pMsg);
            break;
    }
}

/*********************************************************************
 *
 *       _cbDialog
 */
static void _cbDialog(WM_MESSAGE *pMsg)
{
    WM_HWIN hItem;
    int NCode;
    int Id;
    int i;
    static WM_HWIN hWinSelect;
    static WM_HWIN hWinTemperature;
    int Index;
#if USE_MEMDEV
    GUI_COLOR Color;
    GUI_MEMDEV_Handle *pMem;
    static GUI_MEMDEV_Handle ahMem[GUI_COUNTOF(_aForecast)];
#endif

    switch (pMsg->MsgId)
    {
        case WM_INIT_DIALOG:
//
// Initialize buttons
//
#if USE_MEMDEV
            Color = GUI_GRAY;
#endif
            for (i = 0; i < 3; i++)
            {
                hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON0 + i);
#if USE_MEMDEV
                ahMem[i] = CreateMemdevContainer(_aForecast[i + 1].pBitmap, &Color);
                pMem     = &ahMem[i];
                WM_SetUserData(hItem, &pMem, sizeof(void *));
#endif
                WM_SetCallback(hItem, _cbButton);
            }
            //
            // Create a window which shows the curretnly selected day
            //
            hWinSelect = WM_CreateWindowAsChild(10, 15, 180, 180, pMsg->hWin, WM_CF_SHOW, _cbWinCurrent, 0);
            //
            // This window shows a temperature curve
            //
            hWinTemperature = WM_CreateWindowAsChild(195, 110, 280, 85, pMsg->hWin, WM_CF_SHOW, _cbWinPreview, 0);
            break;
        case WM_PAINT:
            //
            // Clear background
            //
            GUI_SetBkColor(COLOR_BG);
            GUI_Clear();
            break;
        case WM_NOTIFY_PARENT:
            Id    = WM_GetId(pMsg->hWinSrc);
            NCode = pMsg->Data.v;
            switch (Id)
            {
                case GUI_ID_BUTTON0: // Notifications sent by 'Button'
                case GUI_ID_BUTTON1:
                case GUI_ID_BUTTON2:
                    switch (NCode)
                    {
                        case WM_NOTIFICATION_CLICKED:
                            //
                            // All buttons should do the same just with another index
                            //
                            Index         = Id - GUI_ID_BUTTON0 + 1;
                            _aForecast[0] = _aForecast[Index];
                            hItem         = WM_GetDialogItem(pMsg->hWin, Id);
                            WM_SetFocus(hItem);
                            WM_Invalidate(hWinSelect);
                            WM_Invalidate(hWinTemperature);
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
 *       Static code
 *
 **********************************************************************
 */
/*********************************************************************
 *
 *       CreateWeather
 */
WM_HWIN CreateWeather(WM_HWIN hParent, int x0, int y0, int xSize, int ySize)
{
    GUI_USE_PARA(xSize);
    GUI_USE_PARA(ySize);
    return GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, hParent, x0, y0);
}

/*************************** End of file ****************************/
