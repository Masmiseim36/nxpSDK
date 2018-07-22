/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2016  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.38 - Graphical user interface for embedded applications **
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
License model:            emWin License Agreement, dated August 20th 2011
Licensed product:         -
Licensed platform:        NXP's ARM 7/9, Cortex-M0,M3,M4
Licensed number of seats: -
----------------------------------------------------------------------
File        : DROPDOWN_Private.h
Purpose     : DROPDOWN private header file
--------------------END-OF-HEADER-------------------------------------
*/

#ifndef DROPDOWN_PRIVATE_H
#define DROPDOWN_PRIVATE_H

#include "DROPDOWN.h"
#include "WIDGET.h"
#include "GUI_ARRAY.h"

#if GUI_WINSUPPORT

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define DROPDOWN_SF_AUTOSCROLLBAR DROPDOWN_CF_AUTOSCROLLBAR

/*********************************************************************
*
*       Object definition
*
**********************************************************************
*/
typedef struct {
  WIDGET_DRAW_ITEM_FUNC * pfDrawSkin;
} DROPDOWN_SKIN_PRIVATE;

typedef struct {
  const GUI_FONT * pFont;
  GUI_COLOR aBackColor[3];
  GUI_COLOR aTextColor[3];
  GUI_COLOR aColor[2];
  GUI_COLOR aScrollbarColor[3];
  DROPDOWN_SKIN_PRIVATE SkinPrivate;
  I16       TextBorderSize;
  I16       Align;
} DROPDOWN_PROPS;

typedef struct {
  WIDGET  Widget;
  I16     Sel;        // Current selection
  I16     ySizeLB;    // ySize of assigned LISTBOX in expanded state
  I16     TextHeight;
  GUI_ARRAY Handles;
  WM_SCROLL_STATE ScrollState;
  DROPDOWN_PROPS Props;
  WIDGET_SKIN const * pWidgetSkin;
  WM_HWIN hListWin;
  U8      Flags;
  U16     ItemSpacing;
  U8      ScrollbarWidth;
  char  IsPressed;
  WM_HMEM hDisabled;
} DROPDOWN_Obj;

/*********************************************************************
*
*       Macros for internal use
*
**********************************************************************
*/
#if GUI_DEBUG_LEVEL >= GUI_DEBUG_LEVEL_CHECK_ALL
  #define DROPDOWN_INIT_ID(p) (p->Widget.DebugId = DROPDOWN_ID)
#else
  #define DROPDOWN_INIT_ID(p)
#endif

#if GUI_DEBUG_LEVEL >= GUI_DEBUG_LEVEL_CHECK_ALL
  DROPDOWN_Obj * DROPDOWN_LockH(DROPDOWN_Handle h);
  #define DROPDOWN_LOCK_H(h)   DROPDOWN_LockH(h)
#else
  #define DROPDOWN_LOCK_H(h)   (DROPDOWN_Obj *)GUI_LOCK_H(h)
#endif

/*********************************************************************
*
*       Private (module internal) data
*
**********************************************************************
*/

extern DROPDOWN_PROPS DROPDOWN__DefaultProps;

extern const WIDGET_SKIN DROPDOWN__SkinClassic;
extern       WIDGET_SKIN DROPDOWN__Skin;

extern WIDGET_SKIN const * DROPDOWN__pSkinDefault;

/*********************************************************************
*
*       Private functions
*
**********************************************************************
*/

void DROPDOWN__AdjustHeight(DROPDOWN_Handle hObj);
int  DROPDOWN__GetNumItems (DROPDOWN_Obj * pObj);
const char * DROPDOWN__GetpItemLocked(DROPDOWN_Handle hObj, int Index);

#endif // GUI_WINSUPPORT
#endif // DROPDOWN_PRIVATE_H
