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

** emWin V6.16 - Graphical user interface for embedded applications **
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
SUA period:               2011-08-19 - 2021-09-02
Contact to extend SUA:    sales@segger.com
----------------------------------------------------------------------
File        : MULTIEDIT_Private.h
Purpose     : MULTIEDIT include
--------------------END-OF-HEADER-------------------------------------
*/

#ifndef MULTIEDIT_PRIVATE_H
#define MULTIEDIT_PRIVATE_H

#include "GUI_Private.h"
#include "WM_Intern.h"

#if GUI_WINSUPPORT

#include <stddef.h>

#include "MULTIEDIT.h"

#if defined(__cplusplus)
  extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

/*********************************************************************
*
*       Object definition
*
**********************************************************************
*/

#define NUM_DISP_MODES 2

#define INVALID_NUMCHARS (1 << 0)
#define INVALID_NUMLINES (1 << 1)
#define INVALID_TEXTSIZE (1 << 2)
#define INVALID_CURSORXY (1 << 3)
#define INVALID_LINEPOSB (1 << 4)

typedef struct {
  WIDGET           Widget;
  GUI_COLOR        aBkColor[NUM_DISP_MODES];
  GUI_COLOR        aColor[NUM_DISP_MODES];
  GUI_COLOR        aCursorColor[2];
  WM_HMEM          hText;
  U16              MaxNumChars;         /* Maximum number of characters including the prompt */
  U16              NumChars;            /* Number of characters (text and prompt) in object */
  U16              NumCharsPrompt;      /* Number of prompt characters */
  U16              NumLines;            /* Number of text lines needed to show all data */
  U16              TextSizeX;           /* Size in X of text depending of wrapping mode */
  U16              BufferSize;
  U16              CursorLine;          /* Number of current cursor line */
  U16              CursorPosChar;       /* Character offset number of cursor */
  U16              CursorPosByte;       /* Byte offset number of cursor */
  I16              CursorPosX;          /* Cursor position in X */
  U16              CursorPosY;          /* Cursor position in Y */
  U16              CacheLinePosByte;    /*  */
  U16              CacheLineNumber;     /*  */
  U16              CacheFirstVisibleLine;
  U16              CacheFirstVisibleByte;
  U16              Align;
  WM_SCROLL_STATE  ScrollStateV;
  WM_SCROLL_STATE  ScrollStateH;
  const GUI_FONT * pFont;
  U8               Flags;
  U8               InvalidFlags;         /* Flags to save validation status */
  U8               EditMode;
  U8               HBorder;
  U8               Radius;               // Currently only used by AppWizard
  WM_HTIMER        hTimer;
  U8               CursorVis;            /* Indicates whether cursor is visible or not*/
  U8               InvertCursor;
  GUI_WRAPMODE     WrapMode;
  int              MotionPosX;
  int              MotionPosY;
  U8               MotionActive;
} MULTIEDIT_OBJ;

#if defined(__cplusplus)
  }
#endif

#endif  // GUI_WINSUPPORT
#endif  // MULTIEDIT_PRIVATE_H

/*************************** End of file ****************************/
