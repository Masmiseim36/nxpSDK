/*********************************************************************
*                SEGGER Microcontroller GmbH                         *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2020  SEGGER Microcontroller GmbH                *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V6.14 - Graphical user interface for embedded applications **
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
File        : FRAMEWIN.h
Purpose     : Frame window include
--------------------END-OF-HEADER-------------------------------------
*/

#ifndef FRAMEWIN_H
#define FRAMEWIN_H

#include "WM.h"
#include "WIDGET.h"             /* Req. for WIDGET_DRAW_ITEM_FUNC */
#if GUI_WINSUPPORT
#include "DIALOG_Type.h"      /* Req. for Create indirect data structure */

#if defined(__cplusplus)
  extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
/*********************************************************************
*
*       Configuration
*/
#ifndef   FRAMEWIN_ALLOW_DRAG_ON_FRAME
  #define FRAMEWIN_ALLOW_DRAG_ON_FRAME 1
#endif

/*********************************************************************
*
*       FRAMEWIN states
*
*  Description
*    State of the FRAMEWIN used for various functions.
*/
#define FRAMEWIN_CI_INACTIVE 0    // When the FRAMEWIN is inactive.
#define FRAMEWIN_CI_ACTIVE   1    // When the FRAMEWIN is active.

/*********************************************************************
*
*       FRAMEWIN create flags
*
*  Description
*    Create flags that define the behavior of the FRAMEWIN widget.
*    These flags are OR-combinable and can be specified upon creation
*    of the widget via the \a{ExFlags} parameter of FRAMEWIN_CreateEx().
*/
#define FRAMEWIN_CF_ACTIVE     (1 << 3)    // Active-state of the frame window. See FRAMEWIN_SetActive().
#define FRAMEWIN_CF_MOVEABLE   (1 << 4)    // Sets the frame window to a moveable state. See FRAMEWIN_SetMoveable().
#define FRAMEWIN_CF_TITLEVIS   (1 << 5)    // Visibility of the frame window's title. See FRAMEWIN_SetTitleVis().
#define FRAMEWIN_CF_MINIMIZED  (1 << 6)    // Minimized-state of the frame window. See FRAMEWIN_Minimize().
#define FRAMEWIN_CF_MAXIMIZED  (1 << 7)    // Maximized-state of the frame window. See FRAMEWIN_Maximize().
#define FRAMEWIN_CF_DRAGGING   (1 << 8)

/* status flags */
#define FRAMEWIN_SF_ACTIVE     FRAMEWIN_CF_ACTIVE
#define FRAMEWIN_SF_MOVEABLE   FRAMEWIN_CF_MOVEABLE
#define FRAMEWIN_SF_TITLEVIS   FRAMEWIN_CF_TITLEVIS
#define FRAMEWIN_SF_MINIMIZED  FRAMEWIN_CF_MINIMIZED
#define FRAMEWIN_SF_MAXIMIZED  FRAMEWIN_CF_MAXIMIZED
#define FRAMEWIN_SF_DRAGGING   FRAMEWIN_CF_DRAGGING

/*********************************************************************
*
*       FRAMEWIN button flags
*
*  Description
*    These flags determine on which side of the FRAMEWIN widget a button
*    should be added.
*/
#define FRAMEWIN_BUTTON_RIGHT   (1 << 0)    // The BUTTON will be created at the right side.
#define FRAMEWIN_BUTTON_LEFT    (1 << 1)    // The BUTTON will be created at the left side.

/*********************************************************************
*
*       Skinning property indices
*/
#define FRAMEWIN_SKINFLEX_PI_ACTIVE   0
#define FRAMEWIN_SKINFLEX_PI_INACTIVE 1

/*********************************************************************
*
*       Getting border size
*/
#define FRAMEWIN_BORDERSIZE_T 0
#define FRAMEWIN_BORDERSIZE_L 1
#define FRAMEWIN_BORDERSIZE_B 2
#define FRAMEWIN_BORDERSIZE_R 3

/*********************************************************************
*
*       Types
*
**********************************************************************
*/
typedef WM_HMEM FRAMEWIN_Handle;

typedef struct {
  GUI_COLOR aColorFrame[3];
  GUI_COLOR aColorTitle[2];
  int Radius;
  int SpaceX;
  int BorderSizeL;
  int BorderSizeR;
  int BorderSizeT;
  int BorderSizeB;
} FRAMEWIN_SKINFLEX_PROPS;

/*********************************************************************
*
*       Create functions
*
**********************************************************************
*/
FRAMEWIN_Handle FRAMEWIN_Create        (const char * pTitle, WM_CALLBACK * cb, int Flags, int x0, int y0, int xSize, int ySize);
FRAMEWIN_Handle FRAMEWIN_CreateAsChild (int x0, int y0, int xSize, int ySize, WM_HWIN hParent, const char * pText, WM_CALLBACK * cb, int Flags);
FRAMEWIN_Handle FRAMEWIN_CreateEx      (int x0, int y0, int xSize, int ySize, WM_HWIN hParent, int WinFlags, int ExFlags, int Id, const char * pTitle, WM_CALLBACK * cb);
FRAMEWIN_Handle FRAMEWIN_CreateUser    (int x0, int y0, int xSize, int ySize, WM_HWIN hParent, int WinFlags, int ExFlags, int Id, const char * pTitle, WM_CALLBACK * cb, int NumExtraBytes);
FRAMEWIN_Handle FRAMEWIN_CreateIndirect(const GUI_WIDGET_CREATE_INFO * pCreateInfo, WM_HWIN hWinParent, int x0, int y0, WM_CALLBACK * cb);

/*********************************************************************
*
*       The callback ...
*
* Do not call it directly ! It is only to be used from within an
* overwritten callback.
*/
void FRAMEWIN_Callback(WM_MESSAGE * pMsg);

/*********************************************************************
*
*       Member functions: Set Properties
*
**********************************************************************
*/
WM_HWIN FRAMEWIN_AddButton     (FRAMEWIN_Handle hObj, int Flags, int Off, int Id);
WM_HWIN FRAMEWIN_AddCloseButton(FRAMEWIN_Handle hObj, int Flags, int Off);
WM_HWIN FRAMEWIN_AddMaxButton  (FRAMEWIN_Handle hObj, int Flags, int Off);
void    FRAMEWIN_AddMenu       (FRAMEWIN_Handle hObj, WM_HWIN hMenu);
WM_HWIN FRAMEWIN_AddMinButton  (FRAMEWIN_Handle hObj, int Flags, int Off);
void    FRAMEWIN_Minimize      (FRAMEWIN_Handle hObj);
void    FRAMEWIN_Maximize      (FRAMEWIN_Handle hObj);
void    FRAMEWIN_Restore       (FRAMEWIN_Handle hObj);
void    FRAMEWIN_SetActive     (FRAMEWIN_Handle hObj, int State);
void    FRAMEWIN_SetBarColor   (FRAMEWIN_Handle hObj, unsigned Index, GUI_COLOR Color);
void    FRAMEWIN_SetBorderSize (FRAMEWIN_Handle hObj, unsigned Size);
void    FRAMEWIN_SetClientColor(FRAMEWIN_Handle hObj, GUI_COLOR Color);
void    FRAMEWIN_SetFont       (FRAMEWIN_Handle hObj, const GUI_FONT * pFont);
void    FRAMEWIN_SetMoveable   (FRAMEWIN_Handle hObj, int State);
void    FRAMEWIN_SetOwnerDraw  (FRAMEWIN_Handle hObj, WIDGET_DRAW_ITEM_FUNC * pfDrawItem);
void    FRAMEWIN_SetResizeable (FRAMEWIN_Handle hObj, int State);
void    FRAMEWIN_SetText       (FRAMEWIN_Handle hObj, const char* s);
void    FRAMEWIN_SetTextAlign  (FRAMEWIN_Handle hObj, int Align);
void    FRAMEWIN_SetTextColor  (FRAMEWIN_Handle hObj, GUI_COLOR Color);
void    FRAMEWIN_SetTextColorEx(FRAMEWIN_Handle hObj, unsigned Index, GUI_COLOR Color);
void    FRAMEWIN_SetTitleVis   (FRAMEWIN_Handle hObj, int Show);
int     FRAMEWIN_SetTitleHeight(FRAMEWIN_Handle hObj, int Height);
int     FRAMEWIN_SetUserData   (FRAMEWIN_Handle hObj, const void * pSrc, int NumBytes);

/*********************************************************************
*
*       Member functions: Skinning
*
**********************************************************************
*/
void FRAMEWIN_GetSkinFlexProps     (FRAMEWIN_SKINFLEX_PROPS * pProps, int Index);
void FRAMEWIN_SetSkinClassic       (FRAMEWIN_Handle hObj);
void FRAMEWIN_SetSkin              (FRAMEWIN_Handle hObj, WIDGET_DRAW_ITEM_FUNC * pfDrawSkin);
int  FRAMEWIN_DrawSkinFlex         (const WIDGET_ITEM_DRAW_INFO * pDrawItemInfo);
void FRAMEWIN_SetSkinFlexProps     (const FRAMEWIN_SKINFLEX_PROPS * pProps, int Index);
void FRAMEWIN_SetDefaultSkinClassic(void);
WIDGET_DRAW_ITEM_FUNC * FRAMEWIN_SetDefaultSkin(WIDGET_DRAW_ITEM_FUNC * pfDrawSkin);

#define FRAMEWIN_SKIN_FLEX    FRAMEWIN_DrawSkinFlex

/*********************************************************************
*
*       Member functions: Get Properties
*
**********************************************************************
*/
const GUI_FONT * FRAMEWIN_GetFont(FRAMEWIN_Handle hObj);

int       FRAMEWIN_GetActive      (FRAMEWIN_Handle hObj);
int       FRAMEWIN_GetTitleHeight (FRAMEWIN_Handle hObj);
GUI_COLOR FRAMEWIN_GetBarColor    (FRAMEWIN_Handle hObj, unsigned Index);
int       FRAMEWIN_GetBorderSize  (FRAMEWIN_Handle hObj);
int       FRAMEWIN_GetBorderSizeEx(FRAMEWIN_Handle hObj, unsigned Edge);
void      FRAMEWIN_GetText        (FRAMEWIN_Handle hObj, char * pBuffer, int MaxLen);
int       FRAMEWIN_GetTextAlign   (FRAMEWIN_Handle hObj);
int       FRAMEWIN_GetUserData    (FRAMEWIN_Handle hObj, void * pDest, int NumBytes);
int       FRAMEWIN_IsMinimized    (FRAMEWIN_Handle hObj);
int       FRAMEWIN_IsMaximized    (FRAMEWIN_Handle hObj);

/*********************************************************************
*
*       Managing default values
*
**********************************************************************
*/
GUI_COLOR        FRAMEWIN_GetDefaultBarColor   (unsigned Index);
int              FRAMEWIN_GetDefaultBorderSize (void);
int              FRAMEWIN_GetDefaultTitleHeight(void);
GUI_COLOR        FRAMEWIN_GetDefaultClientColor(void);
const GUI_FONT * FRAMEWIN_GetDefaultFont       (void);
GUI_COLOR        FRAMEWIN_GetDefaultTextColor  (unsigned Index);
int              FRAMEWIN_OwnerDraw            (const WIDGET_ITEM_DRAW_INFO * pDrawItemInfo);
void             FRAMEWIN_SetDefaultBarColor   (unsigned Index, GUI_COLOR Color);
void             FRAMEWIN_SetDefaultBorderSize (int DefaultBorderSize);
void             FRAMEWIN_SetDefaultTitleHeight(int DefaultTitleHeight);
void             FRAMEWIN_SetDefaultClientColor(GUI_COLOR Color);
void             FRAMEWIN_SetDefaultFont       (const GUI_FONT * pFont);
int              FRAMEWIN_SetDefaultTextAlign  (int TextAlign);
void             FRAMEWIN_SetDefaultTextColor  (unsigned Index, GUI_COLOR Color);

/*********************************************************************
*
*       Macros for compatibility
*
**********************************************************************
*/
#define         FRAMEWIN_SetDefaultCaptionSize(Height)       FRAMEWIN_SetDefaultTitleHeight(Height)
#define         FRAMEWIN_GetDefaultCaptionSize()             FRAMEWIN_GetDefaultTitleHeight()
#define         FRAMEWIN_CreateButton(hObj, Flags, Off, Id)  FRAMEWIN_AddButton(hObj, Flags, Off, Id) 
#define         FRAMEWIN_CreateCloseButton(hObj, Flags, Off) FRAMEWIN_AddCloseButton(hObj, Flags, Off)
#define         FRAMEWIN_CreateMaxButton(hObj, Flags, Off)   FRAMEWIN_AddMaxButton(hObj, Flags, Off)  
#define         FRAMEWIN_CreateMinButton(hObj, Flags, Off)   FRAMEWIN_AddMinButton(hObj, Flags, Off)  

#if defined(__cplusplus)
  }
#endif

#endif  // GUI_WINSUPPORT
#endif  // FRAMEWIN_H

/*************************** End of file ****************************/
