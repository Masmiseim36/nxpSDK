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
File        : SWITCH_Private.h
Purpose     : SWITCH private header file
--------------------END-OF-HEADER-------------------------------------
*/

#ifndef SWITCH_PRIVATE_H
#define SWITCH_PRIVATE_H

#include "WM_Intern.h"
#include "SWITCH.h"

#if (GUI_WINSUPPORT && GUI_SUPPORT_MEMDEV)

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
/*********************************************************************
*
*       SWITCH Flags
*/
#define SWITCH_ANIM_ACTIVE   (1 << 2)
#define SWITCH_MOTION_ACTIVE (1 << 3)
#define SWITCH_THUMB_TOUCHED (1 << 4)

/*********************************************************************
*
*       Object definition
*
**********************************************************************
*/
typedef struct {
  WIDGET_DRAW_ITEM_FUNC * pfDrawSkin;
} SWITCH_SKIN_PRIVATE;

typedef struct {
  GUI_COLOR           aTextColor[3];
  int                 Period;
  const GUI_FONT    * pFont;
  SWITCH_SKIN_PRIVATE SkinPrivate;
} SWITCH_PROPS;

typedef struct {
  WIDGET              Widget;
  SWITCH_PROPS        Props;
  WIDGET_SKIN const * pWidgetSkin;
  WM_HMEM             hContext;      // Motion context
  int                 PosThumb;      // A value between 0 (left) and GUI_ANIM_RANGE (right)
  int                 IndexThumb;    // Index to be used to draw thumb
  WM_HMEM             ahpText[2];    // Left- and right rext
  WM_HMEM             ahDrawObj[6];  // Up to 6 images are required
  int                 xSizeThumb;    // (obvious)
  int                 ySizeThumb;    // (obvious)
  int                 xAreaThumb;    // Size in pixels to be used for moving the thumb
  int                 AnimStart;     // Start value of animation
  int                 AnimEnd;       // End value of animation
  GUI_ANIM_HANDLE     hAnim;         // Animation handle
  GUI_MEMDEV_Handle   hMem_L;        // Memory device for fading mode containing left image
  GUI_MEMDEV_Handle   hMem_R;        // Memory device for fading mode containing right image
  GUI_MEMDEV_Handle   hMem_Work;     // Memory device for fading mode containing result
  U8                  Flags;         // (obvious)
} SWITCH_Obj;

/*********************************************************************
*
*       Macros for internal use
*
**********************************************************************
*/
#if GUI_DEBUG_LEVEL >= GUI_DEBUG_LEVEL_CHECK_ALL
  #define SWITCH_INIT_ID(p) (p->Widget.DebugId = SWITCH_ID)
#else
  #define SWITCH_INIT_ID(p)
#endif

#if GUI_DEBUG_LEVEL >= GUI_DEBUG_LEVEL_CHECK_ALL
  SWITCH_Obj * SWITCH_LockH(SWITCH_Handle h);
  #define SWITCH_LOCK_H(h)   SWITCH_LockH(h)
#else
  #define SWITCH_LOCK_H(h)   (SWITCH_Obj *)GUI_LOCK_H(h)
#endif

/*********************************************************************
*
*       Private interface
*
**********************************************************************
*/
void SWITCH__SetDrawObj  (SWITCH_Handle hObj, int Index, GUI_DRAW_HANDLE hDrawObj);

/*********************************************************************
*
*       Public data (internal defaults)
*
**********************************************************************
*/
extern SWITCH_PROPS SWITCH__DefaultProps;

extern const WIDGET_SKIN SWITCH__SkinClassic;
extern       WIDGET_SKIN SWITCH__Skin;

extern WIDGET_SKIN const * SWITCH__pSkinDefault;


#endif   /* (GUI_WINSUPPORT && GUI_SUPPORT_MEMDEV) */
#endif   /* SWITCH_PRIVATE_H */

/*************************** End of file ****************************/
