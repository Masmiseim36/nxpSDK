#include "Resource.h"

/*********************************************************************
 *
 *       Defines
 *
 **********************************************************************
 */

/*********************************************************************
 *
 *       Static data
 *
 **********************************************************************
 */
static const char *_apMinutes[] = {
    "00", "01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12", "13", "14", "15",
    "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30", "31",
    "32", "33", "34", "35", "36", "37", "38", "39", "40", "41", "42", "43", "44", "45", "46", "47",
    "48", "49", "50", "51", "52", "53", "54", "55", "56", "57", "58", "59", NULL,
};

static const char *_apSeconds[] = {
    "00", "01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12", "13", "14", "15",
    "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30", "31",
    "32", "33", "34", "35", "36", "37", "38", "39", "40", "41", "42", "43", "44", "45", "46", "47",
    "48", "49", "50", "51", "52", "53", "54", "55", "56", "57", "58", "59", NULL,
};

/*********************************************************************
 *
 *       Static code
 *
 **********************************************************************
 */

/*********************************************************************
 *
 *       _cbStartStop
 */
static void _cbStartStop(WM_MESSAGE *pMsg)
{
    GUI_RECT Rect;
    char acBuffer[10];
    GUI_COLOR ColorBK;
    GUI_COLOR ColorFG;

    switch (pMsg->MsgId)
    {
        case WM_PAINT:
            if (BUTTON_IsPressed(pMsg->hWin))
            {
                ColorBK = COLOR_LEMON;
                ColorFG = COLOR_BLUE;
            }
            else
            {
                ColorBK = COLOR_BLUE;
                ColorFG = COLOR_LEMON;
            }
            GUI_SetBkColor(ColorBK);
            GUI_Clear();
            BUTTON_GetText(pMsg->hWin, acBuffer, sizeof(acBuffer));
            WM_GetClientRect(&Rect);
            GUI_SetFont(&GUI_FontAA4_B_40);
            GUI_SetColor(ColorFG);
            GUI_DispStringInRect(acBuffer, &Rect, GUI_TA_HCENTER | GUI_TA_VCENTER);
            break;
        default:
            BUTTON_Callback(pMsg);
            break;
    }
}

/*********************************************************************
 *
 *       _cbListwheel
 */
static void _cbListwheel(WM_MESSAGE *pMsg)
{
    WM_MOTION_INFO *pInfo;

    switch (pMsg->MsgId)
    {
        case WM_MOTION:
            pInfo = (WM_MOTION_INFO *)pMsg->Data.p;
            switch (pInfo->Cmd)
            {
                case WM_MOTION_INIT:
                    pInfo->Flags = WM_MOTION_MANAGE_BY_WINDOW;
                    break;
            }
            break;
        default:
            LISTWHEEL_Callback(pMsg);
            break;
    }
}

/*********************************************************************
 *
 *       _cbClock
 */
static void _cbClock(WM_MESSAGE *pMsg)
{
    GUI_RECT Rect;
    static int Minutes;
    static int Seconds;
    int xSize;
    int ySize;
    int xPos;
    int yPos;
    WM_HWIN hItem;
    int NCode;
    int Id;
    static WM_HTIMER hTimer;
    static int StartStop;
    static char *apText[] = {"Start", "Stop"};
    static GUI_MEMDEV_Handle hMemFrame;
    static GUI_MEMDEV_Handle hMemBell;

    switch (pMsg->MsgId)
    {
        case WM_TIMER:
            if (Seconds > 0)
            {
                //
                // Seconds left? Decrement
                //
                Seconds--;
            }
            else if (Minutes > 0)
            {
                //
                // If no seconds but minutes left, decrement minutes and set set seconds to max
                //
                Seconds = 59;
                Minutes--;
            }
            if (Minutes >= 0)
            {
                //
                // Move listwheel to proper position and update selection
                //
                hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTWHEEL0);
                LISTWHEEL_MoveToPos(hItem, Minutes);
                LISTWHEEL_SetSel(hItem, Minutes);
            }
            //
            // Move listwheel to proper position and update selection
            //
            hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTWHEEL1);
            LISTWHEEL_MoveToPos(hItem, Seconds);
            LISTWHEEL_SetSel(hItem, Seconds);
            if (Seconds || Minutes)
            {
                //
                // If minutes or seconds left, restart timer
                //
                WM_RestartTimer(pMsg->Data.v, 1000);
            }
            else
            {
                //
                // No minutes and seconds left, update button text, do not restart timer and enable alarm window
                //
                StartStop = 0;
                hItem     = WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON0);
                BUTTON_SetText(hItem, apText[StartStop]);
                WM_SendMessageNoPara(WM_GetParent(pMsg->hWin), MESSAGE_ALARM);
                WM_DeleteTimer(hTimer);
                hTimer = 0;
            }
            break;
        case WM_CREATE:
            xSize = LCD_GetXSize();
            ySize = LCD_GetYSize() - BUTTON_SIZE;
            xPos  = xSize / 2;
            yPos  = ySize / 2 - ySize / 4 + 2;
            ySize = (ySize / 2 + ySize / 4) - yPos - 3;
            //
            // Set up listwheel for minutes
            //
            hItem = LISTWHEEL_CreateEx(xPos - LISTWHEEL_SIZEX - BORDER_SIZE, yPos, LISTWHEEL_SIZEX, ySize, pMsg->hWin,
                                       WM_CF_SHOW | WM_CF_MOTION_X, 0, GUI_ID_LISTWHEEL0, _apMinutes);
            WM_SetCallback(hItem, _cbListwheel);
            LISTWHEEL_SetFont(hItem, &GUI_FontAA4_B_100);
            LISTWHEEL_SetTextAlign(hItem, GUI_TA_HCENTER);
            LISTWHEEL_SetTextColor(hItem, LISTWHEEL_CI_SEL, COLOR_LEMON);
            LISTWHEEL_SetTextColor(hItem, LISTWHEEL_CI_UNSEL, COLOR_LEMON);
            LISTWHEEL_SetBkColor(hItem, LISTWHEEL_CI_SEL, GUI_DARKGRAY);
            LISTWHEEL_SetBkColor(hItem, LISTWHEEL_CI_UNSEL, GUI_DARKGRAY);
            LISTWHEEL_SetDeceleration(hItem, 30);
            //
            // Set up listwheel for minutes
            //
            hItem = LISTWHEEL_CreateEx(xPos + BORDER_SIZE, yPos, LISTWHEEL_SIZEX, ySize, pMsg->hWin,
                                       WM_CF_SHOW | WM_CF_MOTION_X, 0, GUI_ID_LISTWHEEL1, _apSeconds);
            WM_SetCallback(hItem, _cbListwheel);
            LISTWHEEL_SetFont(hItem, &GUI_FontAA4_B_100);
            LISTWHEEL_SetTextAlign(hItem, GUI_TA_HCENTER);
            LISTWHEEL_SetTextColor(hItem, LISTWHEEL_CI_SEL, COLOR_LEMON);
            LISTWHEEL_SetTextColor(hItem, LISTWHEEL_CI_UNSEL, COLOR_LEMON);
            LISTWHEEL_SetBkColor(hItem, LISTWHEEL_CI_SEL, GUI_DARKGRAY);
            LISTWHEEL_SetBkColor(hItem, LISTWHEEL_CI_UNSEL, GUI_DARKGRAY);
            LISTWHEEL_SetDeceleration(hItem, 30);
            //
            // Create button to start and stop the timer
            //
            hItem = BUTTON_CreateAsChild(xPos - BUTTON_SIZE, yPos + ySize + BORDER_SIZE, BUTTON_SIZE * 2, 40,
                                         pMsg->hWin, GUI_ID_BUTTON0, WM_CF_SHOW);
            BUTTON_SetText(hItem, "Start");
#if GUI_VERSION >= 54000
            BUTTON_SetFocusable(hItem, 0);
#else
            BUTTON_SetFocussable(hItem, 0);
#endif
            WM_SetCallback(hItem, _cbStartStop);
            //
            // Create memory devices, we do it with memory devices to avoid
            // excessive use of AA functions and converting bitmap too often.
            //
            Rect.x0 = 0;
            Rect.y0 = 0;
            Rect.x1 = FRAME_SIZEX;
            Rect.y1 = FRAME_SIZEY;
            //
            // Memory device used as background
            //
            hMemFrame = GUI_MEMDEV_Create(Rect.x0, Rect.y0, Rect.x1, Rect.y1);
            GUI_MEMDEV_Select(hMemFrame);
            GUI_SetBkColor(COLOR_BG);
            GUI_Clear();
            GUI_SetColor(GUI_DARKGRAY);
            GUI_FillRoundedRect(Rect.x0, Rect.y0, Rect.x1 - 1, Rect.y1 - 1, 5);
            GUI_SetColor(GUI_LIGHTGRAY);
            GUI_AA_DrawRoundedRect(Rect.x0, Rect.y0, Rect.x1 - 1, Rect.y1 - 1, 5);
            GUI_MEMDEV_Select(0);
            //
            // Memory device containing the bell
            //
            hMemBell = GUI_MEMDEV_Create(Rect.x0, Rect.y0, bmBell_60x60.XSize, bmBell_60x60.YSize);
            GUI_MEMDEV_Select(hMemBell);
            GUI_DrawBitmap(&bmBell_60x60, Rect.x0, Rect.y0);
            GUI_MEMDEV_Select(0);
            break;
        case WM_PAINT:
            GUI_SetBkColor(COLOR_BLUE);
            GUI_Clear();
            //
            // Draw background
            //
            xSize = LCD_GetXSize();
            ySize = LCD_GetYSize() - BUTTON_SIZE;
            xPos  = (xSize - FRAME_SIZEX) / 2 + WM_GetWindowOrgX(pMsg->hWin);
            yPos  = (ySize - FRAME_SIZEY) / 2 + WM_GetWindowOrgY(pMsg->hWin);
            GUI_MEMDEV_WriteAt(hMemFrame, xPos, yPos);
            //
            // Draw bell
            //
            xPos = (xSize - FRAME_SIZEX) / 2 - bmBell_60x60.XSize - BORDER_SIZE + WM_GetWindowOrgX(pMsg->hWin);
            yPos = (ySize - bmBell_60x60.YSize) / 2 + WM_GetWindowOrgY(pMsg->hWin);
            GUI_MEMDEV_WriteAt(hMemBell, xPos, yPos);
            //
            // Set up text properties
            //
            GUI_SetColor(COLOR_LEMON);
            GUI_SetBkColor(GUI_DARKGRAY);
            GUI_SetFont(&GUI_FontAA4_B_100);
            //
            // Draw separator
            //
            WM_GetClientRect(&Rect);
            Rect.y0 += BORDER_SIZE / 2;
            Rect.y1 -= BORDER_SIZE;
            GUI_DispStringInRect(":", &Rect, GUI_TA_HCENTER | GUI_TA_VCENTER);
            GUI_SetFont(&GUI_FontAA4_B_40);
            //
            // Draw title
            //
            WM_GetClientRect(&Rect);
            GUI_SetBkColor(COLOR_BLUE);
            GUI_DispStringInRect("Alarm", &Rect, GUI_TA_TOP | GUI_TA_HCENTER);
            break;
        case WM_NOTIFY_PARENT:
            Id    = WM_GetId(pMsg->hWinSrc);
            NCode = pMsg->Data.v;
            hItem = WM_GetDialogItem(pMsg->hWin, Id);
            switch (Id)
            {
                case GUI_ID_LISTWHEEL0:
                    switch (NCode)
                    {
                        case WM_NOTIFICATION_SEL_CHANGED:
                            Minutes = LISTWHEEL_GetPos(hItem);
                            break;
                    }
                    break;
                case GUI_ID_LISTWHEEL1:
                    switch (NCode)
                    {
                        case WM_NOTIFICATION_SEL_CHANGED:
                            Seconds = LISTWHEEL_GetPos(hItem);
                            break;
                    }
                    break;
                case GUI_ID_BUTTON0:
                    switch (NCode)
                    {
                        case WM_NOTIFICATION_RELEASED:
                            //
                            // Check for StartStop flag
                            //
                            if (StartStop)
                            {
                                //
                                // If the flag is set cancel timer execution
                                //
                                StartStop = 0;
                                BUTTON_SetText(hItem, apText[StartStop]);
                                WM_DeleteTimer(hTimer);
                                hTimer = 0;
                            }
                            else
                            {
                                //
                                // If the flag is set and the user has entered a valid time, start timer execution
                                //
                                if ((Seconds > 0) || (Minutes > 0))
                                {
                                    StartStop = 1;
                                    BUTTON_SetText(hItem, apText[StartStop]);
                                    if (hTimer == 0)
                                    {
                                        hTimer = WM_CreateTimer(pMsg->hWin, 0, 1000, 0);
                                    }
                                    //
                                    // Send stop alarm message
                                    //
                                    WM_SendMessageNoPara(WM_GetParent(pMsg->hWin), MESSAGE_ALARM_STOP);
                                }
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
 *       Static code
 *
 **********************************************************************
 */
/*********************************************************************
 *
 *       CreateAlarm
 */
WM_HWIN CreateAlarm(WM_HWIN hParent, int x0, int y0, int xSize, int ySize)
{
    return WM_CreateWindowAsChild(x0, y0, xSize, ySize, hParent, WM_CF_SHOW, _cbClock, 0);
}
