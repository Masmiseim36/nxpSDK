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
File        : KEYBOARD_Private.h
Purpose     : Internal header file
--------------------END-OF-HEADER-------------------------------------
*/

#ifndef KEYBOARD_PRIVATE_H
#define KEYBOARD_PRIVATE_H

#include "WM_Intern.h"
#include "KEYBOARD.h"

#if (GUI_WINSUPPORT)

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define KEYBOARD_STATE_CODES 0
#define KEYBOARD_STATE_SHIFT 1
#define KEYBOARD_STATE_SLOCK 2
#define KEYBOARD_STATE_EXTRA 3

#define FLAG_FIXED        (1 << 0)
#define FLAG_CODES        (1 << 1)
#define FLAG_SHIFT        (1 << 2)
#define FLAG_EXTRA        (1 << 3)
#define FLAG_KEYBACKSPACE (1 << 4)
#define FLAG_KEYENTER     (1 << 5)
#define FLAG_KEYSHIFT     (1 << 6)
#define FLAG_KEYSWITCH    (1 << 7)
#define FLAG_KEYSPACE     (1 << 8)

/*********************************************************************
*
*       Types
*
**********************************************************************
*/
typedef struct {
  GUI_RECT       Rect;
  U16            Code;
  U16            Long;
  U16            Flags;
  KEYBOARD_CODES cLong;
} KEYBOARD_KEY;

typedef struct {
  GUI_HMEM     hText;  // Handle to text
  const void * pBm;    // Pointer to GUI_BITMAP or pointer to streamed bitmap
  U32          Size;   // In case of a streamed bitmap 'Size' contains its size
} KEYBOARD_BITMAP_KEY;

/*********************************************************************
*
*       Object definition
*
**********************************************************************
*/
typedef struct {
  GUI_COLOR aColor[7];       // Colors to be used
  unsigned  aPeriod[2];      // Array for periods [0:long press, 1:repeat mode]
  unsigned  FrameRadius;     // Radius to be used for all keys
  unsigned  FrameSize;       // Frame size in pixels to be used for all keys
  unsigned  SpaceX, SpaceY;  // Space in /1000 between the keys
} KEYBOARD_PROPS;

typedef struct {
  WIDGET           Widget;
  KEYBOARD_PROPS   Props;
  const GUI_FONT * apFont[2];
  //const GUI_FONT * pFontCode;
  //const GUI_FONT * pFontLong;
  //
  // Common
  //
  U8           State;
  U8           IsPressed;
  WM_HTIMER    hTimer;
  KEYBOARD_KEY kPressed;
  //
  // Code keys
  //
  unsigned NumKeys;
  GUI_HMEM hKeys;
  //
  // Special keys
  //
  KEYBOARD_BITMAP_KEY aKeyShift[4];
  KEYBOARD_BITMAP_KEY aKeySwitch[2];
  KEYBOARD_BITMAP_KEY KeyBackspace;
  KEYBOARD_BITMAP_KEY KeyEnter;
  KEYBOARD_BITMAP_KEY KeySpace;
  //
  // Timer callback
  //
  void (* pfOnTimer)(KEYBOARD_Handle hObj);
  //
  // Size of longpress key
  //
  int xSizeKey, ySizeKey;
} KEYBOARD_OBJ;

/*********************************************************************
*
*       Macros for internal use
*
**********************************************************************
*/
#if GUI_DEBUG_LEVEL >= GUI_DEBUG_LEVEL_CHECK_ALL
  #define KEYBOARD_INIT_ID(p) (p->Widget.DebugId = KEYBOARD_ID)
#else
  #define KEYBOARD_INIT_ID(p)
#endif

#if GUI_DEBUG_LEVEL >= GUI_DEBUG_LEVEL_CHECK_ALL
  KEYBOARD_OBJ * KEYBOARD_LockH(KEYBOARD_Handle h);
  #define KEYBOARD_LOCK_H(h) KEYBOARD_LockH(h)
#else
  #define KEYBOARD_LOCK_H(h) (KEYBOARD_OBJ *)GUI_LOCK_H(h)
#endif

/*********************************************************************
*
*       Public data (internal defaults)
*
**********************************************************************
*/
extern KEYBOARD_PROPS KEYBOARD__DefaultProps;

#endif  // GUI_WINSUPPORT
#endif  // KEYBOARD_PRIVATE_H

/*************************** End of file ****************************/
