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
File        : ICONVIEW_Private.h
Purpose     : ICONVIEW private header file
--------------------END-OF-HEADER-------------------------------------
*/

#ifndef ICONVIEW_PRIVATE_H
#define ICONVIEW_PRIVATE_H

#include "WM.h"
#include "GUI_ARRAY.h"
#include "ICONVIEW.h"

#if GUI_WINSUPPORT

#if defined(__cplusplus)
  extern "C" {             // Make sure we have C-declarations in C++ programs
#endif

/*********************************************************************
*
*       Types
*
**********************************************************************
*/
typedef struct {
  const GUI_FONT * pFont;
  GUI_COLOR        aBkColor[3];
  GUI_COLOR        aTextColor[3];
  int              FrameX, FrameY;
  int              SpaceX, SpaceY;
  int              TextAlign;
  int              IconAlign;
  GUI_WRAPMODE     WrapMode;
} ICONVIEW_PROPS;

typedef struct {
  WIDGET          Widget;
  WM_SCROLL_STATE ScrollStateV;
  WM_SCROLL_STATE ScrollStateH;
  ICONVIEW_PROPS  Props;
  GUI_ARRAY       ItemArray;
  int             xSizeItems;
  int             ySizeItems;
  int             Sel;
  U16             Flags;
} ICONVIEW_OBJ;

typedef void tDrawImage    (const void * pData, GUI_GET_DATA_FUNC * pfGetData, int xPos, int yPos);
typedef void tDrawText     (ICONVIEW_OBJ * pObj, GUI_RECT * pRect, const char * s);
typedef void tGetImageSizes(const void * pData, GUI_GET_DATA_FUNC * pfGetData, int * xSize, int * ySize);

typedef struct {
  tDrawImage        * pfDrawImage;
  tDrawText         * pfDrawText;
  tGetImageSizes    * pfGetImageSizes;
  GUI_GET_DATA_FUNC * pfGetData;
  const void        * pData;
  U32                 UserData;
  int                 SizeOfData;
  char                acText[1];
} ICONVIEW_ITEM;

/*********************************************************************
*
*       Function pointer(s)
*
**********************************************************************
*/
extern void (* ICONVIEW__pfDrawStreamedBitmap)(const void * p, int x, int y);

/*********************************************************************
*
*       Macros for internal use
*
**********************************************************************
*/
#if GUI_DEBUG_LEVEL >= GUI_DEBUG_LEVEL_CHECK_ALL
  #define ICONVIEW_INIT_ID(p) (p->Widget.DebugId = ICONVIEW_ID)
#else
  #define ICONVIEW_INIT_ID(p)
#endif

#if GUI_DEBUG_LEVEL >= GUI_DEBUG_LEVEL_CHECK_ALL
  ICONVIEW_OBJ * ICONVIEW_LockH(ICONVIEW_Handle h);
  #define ICONVIEW_LOCK_H(h)   ICONVIEW_LockH(h)
#else
  #define ICONVIEW_LOCK_H(h)   (ICONVIEW_OBJ *)GUI_LOCK_H(h)
#endif

/*********************************************************************
*
*       Private functions
*
**********************************************************************
*/
void ICONVIEW__DrawText        (ICONVIEW_OBJ    * pObj, GUI_RECT * pRect, const char * pText);
void ICONVIEW__ManageAutoScroll(ICONVIEW_Handle   hObj);

#if defined(__cplusplus)
  }
#endif

#endif  // GUI_WINSUPPORT
#endif  // ICONVIEW_H

/*************************** End of file ****************************/
