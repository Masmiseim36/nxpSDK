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
/*********************************************************************
 *
 *       Static code
 *
 **********************************************************************
 */
/*********************************************************************
 *
 *       _cbInfo
 */
static void _cbInfo(WM_MESSAGE *pMsg)
{
    const char acQR_NXP[]    = "www.nxp.com";
    const char acQR_Segger[] = "www.segger.com";
    static GUI_HMEM ahQR[2];
    GUI_QR_INFO QRInfo;
    int xPos;
    int yPos;
    int xSize;
    int ySize;
    U32 i;
#if USE_MEMDEV
    static GUI_MEMDEV_Handle hMemSegger;
    static GUI_MEMDEV_Handle hMemNXP;
#endif

    switch (pMsg->MsgId)
    {
        case WM_CREATE:
#if USE_MEMDEV
            hMemSegger = CreateMemdevContainer(&bmSeggerLogo_110x55, NULL);
            hMemNXP    = CreateMemdevContainer(&bmNXP_110x44, NULL);
#endif
            //
            // Create QR codes
            //
            ahQR[0] = GUI_QR_Create(acQR_NXP, QR_PIXEL_SIZE, GUI_QR_ECLEVEL_H, 0);
            ahQR[1] = GUI_QR_Create(acQR_Segger, QR_PIXEL_SIZE, GUI_QR_ECLEVEL_H, 0);
            break;
        case WM_PAINT:
            //
            // Clear back ground with blue
            //
            GUI_SetBkColor(COLOR_BLUE);
            GUI_Clear();
            //
            // calculate positions and draw company logos
            //
            xSize = LCD_GetXSize();
            ySize = LCD_GetYSize();
            xPos  = (((xSize >> 1) - bmNXP_110x44.XSize) >> 1);
            yPos  = (((ySize >> 1) - bmNXP_110x44.YSize) >> 1) - OFFSET / 2;
#if USE_MEMDEV
            xPos += WM_GetWindowOrgX(pMsg->hWin);
            yPos += WM_GetWindowOrgY(pMsg->hWin);
            GUI_MEMDEV_WriteAt(hMemNXP, xPos, yPos);
#else
            GUI_DrawBitmap(&bmNXP_110x44, xPos, yPos);
#endif
            xPos = (xSize >> 1) + (((xSize >> 1) - bmSeggerLogo_110x55.XSize) >> 1);
            yPos = (((ySize >> 1) - bmSeggerLogo_110x55.YSize) >> 1) - OFFSET / 2;
#if USE_MEMDEV
            xPos += WM_GetWindowOrgX(pMsg->hWin);
            yPos += WM_GetWindowOrgY(pMsg->hWin);
            GUI_MEMDEV_WriteAt(hMemSegger, xPos, yPos);
#else
            GUI_DrawBitmap(&bmSeggerLogo_110x55, (xSize >> 1) + xPos, yPos);
#endif
            //
            // Draw QR codes
            //
            for (i = 0; i < GUI_COUNTOF(ahQR); i++)
            {
                GUI_QR_GetInfo(ahQR[i], &QRInfo);
                xPos = ((xSize >> 1) * i) + (((xSize >> 1) - QRInfo.Size) >> 1);
                yPos = (ySize >> 1) + (((ySize >> 1) - QRInfo.Size) >> 1) - OFFSET;
                //
                // Make sure the background of the QR code is white
                //
                GUI_SetColor(GUI_WHITE);
                GUI_FillRect(xPos - QR_PIXEL_SIZE, yPos - QR_PIXEL_SIZE, xPos + QRInfo.Size + QR_PIXEL_SIZE,
                             yPos + QRInfo.Size + QR_PIXEL_SIZE);
                GUI_QR_Draw(ahQR[i], xPos, yPos);
            }
            break;
#if USE_MEMDEV
        case WM_DELETE:
            GUI_MEMDEV_Delete(hMemSegger);
            GUI_MEMDEV_Delete(hMemNXP);
            break;
#endif
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
 *       CreateInfo
 */
WM_HWIN CreateInfo(WM_HWIN hParent, int x0, int y0, int xSize, int ySize)
{
    return WM_CreateWindowAsChild(x0, y0, xSize, ySize, hParent, WM_CF_SHOW, _cbInfo, 0);
}
