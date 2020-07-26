/*********************************************************************
*                SEGGER Microcontroller GmbH                         *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2019  SEGGER Microcontroller GmbH                *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V6.10 - Graphical user interface for embedded applications **
All  Intellectual Property rights  in the Software belongs to  SEGGER.
emWin is protected by  international copyright laws.  Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with the following terms:

The software has been licensed to  NXP Semiconductors USA, Inc.  whose
registered  office  is  situated  at 411 E. Plumeria Drive, San  Jose,
CA 95134, USA  solely for  the  purposes  of  creating  libraries  for
NXPs M0, M3/M4 and  ARM7/9 processor-based  devices,  sublicensed  and
distributed under the terms and conditions of the NXP End User License
Agreement.
Full source code is available at: www.segger.com

We appreciate your understanding and fairness.
----------------------------------------------------------------------
Licensing information
Licensor:                 SEGGER Microcontroller Systems LLC
Licensed to:              NXP Semiconductors, 1109 McKay Dr, M/S 76, San Jose, CA 95131, USA
Licensed SEGGER software: emWin
License number:           GUI-00186
License model:            emWin License Agreement, dated August 20th 2011 and Amendment No. 1, dated October 17th 2017 and Amendment No. 2, dated December 18th 2018
Licensed platform:        NXP's ARM 7/9, Cortex-M0, M3, M4, M7, A7, M33
----------------------------------------------------------------------
Support and Update Agreement (SUA)
SUA period:               2011-08-19 - 2020-09-02
Contact to extend SUA:    sales@segger.com
----------------------------------------------------------------------
File        : SWITCH.h
Purpose     : SWITCH public header file (API)
--------------------END-OF-HEADER-------------------------------------
*/

#ifndef SWITCH_H
#define SWITCH_H

#include "WM.h"
#include "DIALOG_Type.h"      /* Req. for Create indirect data structure */
#include "WIDGET.h"

#if (GUI_WINSUPPORT && GUI_SUPPORT_MEMDEV)

#if defined(__cplusplus)
  extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

/************************************************************
*
*       Defines
*
*************************************************************
*/
/*********************************************************************
*
*       Modes
*/
#define SWITCH_MODE_FADE     (1 << 0)
#define SWITCH_MODE_DISCLOSE (1 << 1)

/************************************************************
*
*       States
*/
#define SWITCH_STATE_PRESSED WIDGET_STATE_USER0

#define SWITCH_STATE_LEFT    0
#define SWITCH_STATE_RIGHT   1

/*********************************************************************
*
*       Color indices
*/
#define SWITCH_CI_LEFT     0
#define SWITCH_CI_RIGHT    1
#define SWITCH_CI_DISABLED 2

/*********************************************************************
*
*       Bitmap indices
*/
#define SWITCH_BI_BK_LEFT        0
#define SWITCH_BI_BK_RIGHT       1
#define SWITCH_BI_BK_DISABLED    2
#define SWITCH_BI_THUMB_LEFT     3
#define SWITCH_BI_THUMB_RIGHT    4
#define SWITCH_BI_THUMB_DISABLED 5

/*********************************************************************
*
*       Text indices
*/
#define SWITCH_TI_LEFT  0
#define SWITCH_TI_RIGHT 1

/*********************************************************************
*
*       Types
*
**********************************************************************
*/
typedef WM_HMEM SWITCH_Handle;

/*********************************************************************
*
*       Create function(s)

  Note: the parameters to a create function may vary.
         Some widgets may have multiple create functions
*/
SWITCH_Handle SWITCH_CreateUser    (int x0, int y0, int xSize, int ySize, WM_HWIN hParent, int WinFlags, int ExFlags, int Id, int NumExtraBytes);
SWITCH_Handle SWITCH_CreateIndirect(const GUI_WIDGET_CREATE_INFO * pCreateInfo, WM_HWIN hWinParent, int x0, int y0, WM_CALLBACK * cb);

/*********************************************************************
*
*       Managing default values
*
**********************************************************************
*/
GUI_COLOR        SWITCH_GetDefaultTextColor(unsigned Index);
const GUI_FONT * SWITCH_GetDefaultFont     (void);
unsigned         SWITCH_GetDefaultPeriod   (void);
void             SWITCH_SetDefaultTextColor(GUI_COLOR Color, unsigned Index);
void             SWITCH_SetDefaultFont     (const GUI_FONT * pFont);
void             SWITCH_SetDefaultPeriod   (unsigned Period);

/*********************************************************************
*
*       The callback ...
*
* Do not call it directly ! It is only to be used from within an
* overwritten callback.
*/
void SWITCH_Callback(WM_MESSAGE *pMsg);

/*********************************************************************
*
*       Member functions
*
**********************************************************************
*/
void SWITCH_AnimState   (SWITCH_Handle hObj, int State);
int  SWITCH_GetState    (SWITCH_Handle hObj);
int  SWITCH_GetUserData (SWITCH_Handle hObj, void * pDest, int NumBytes);
void SWITCH_SetBitmap   (SWITCH_Handle hObj, unsigned int Index, const GUI_BITMAP * pBitmap);
void SWITCH_SetFont     (SWITCH_Handle hObj, const GUI_FONT * pfont);
void SWITCH_SetMode     (SWITCH_Handle hObj, int Mode);
void SWITCH_SetPeriod   (SWITCH_Handle hObj, I32 Period);
void SWITCH_SetState    (SWITCH_Handle hObj, int State);
int  SWITCH_SetText     (SWITCH_Handle hObj, unsigned int Index, const char * pText);
void SWITCH_SetTextColor(SWITCH_Handle hObj, unsigned int Index, GUI_COLOR Color);
int  SWITCH_SetUserData (SWITCH_Handle hObj, const void * pSrc, int NumBytes);
void SWITCH_Toggle      (SWITCH_Handle hObj);

/*********************************************************************
*
*       Member functions: Skinning
*
**********************************************************************
*/
int  SWITCH_DrawSkin(const WIDGET_ITEM_DRAW_INFO * pDrawItemInfo);
void SWITCH_SetSkin (SWITCH_Handle hObj, WIDGET_DRAW_ITEM_FUNC * pfDrawSkin);

#define SWITCH_SKIN    SWITCH_DrawSkin

#if defined(__cplusplus)
  }
#endif

#endif  // (GUI_WINSUPPORT && GUI_SUPPORT_MEMDEV)
#endif  // SWITCH_H

/*************************** End of file ****************************/
