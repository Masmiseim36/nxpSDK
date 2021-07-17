/*******************************************************************************
*
* E M B E D D E D   W I Z A R D   P R O J E C T
*
*                                                Copyright (c) TARA Systems GmbH
*                                    written by Paul Banach and Manfred Schweyer
*
********************************************************************************
*
* This software is delivered "as is" and shows the usage of other software 
* components. It is provided as an example software which is intended to be 
* modified and extended according to particular requirements.
* 
* TARA Systems hereby disclaims all warranties and conditions with regard to the
* software, including all implied warranties and conditions of merchantability 
* and non-infringement of any third party IPR or other rights which may result 
* from the use or the inability to use the software.
*
* This file was generated automatically by Embedded Wizard Studio.
*
* Please do not make any modifications of this file! The modifications are lost
* when the file is generated again by Embedded Wizard Studio!
*
* Version  : 10.0
* Date     : 17.02.2021  8:00:50
* Profile  : iMX_RT
* Platform : NXP.iMX_RT_VGLite.RGB565
*
*******************************************************************************/

#ifndef Views_H
#define Views_H

#ifdef __cplusplus
  extern "C"
  {
#endif

#include "ewrte.h"
#if EW_RTE_VERSION != 0x000A0000
  #error Wrong version of Embedded Wizard Runtime Environment.
#endif

#include "ewgfx.h"
#if EW_GFX_VERSION != 0x000A0000
  #error Wrong version of Embedded Wizard Graphics Engine.
#endif

#include "_ViewsFillPath.h"
#include "_ViewsFrame.h"
#include "_ViewsImage.h"
#include "_ViewsLine.h"
#include "_ViewsRectangle.h"
#include "_ViewsStrokePath.h"
#include "_ViewsText.h"
#include "_ViewsWallpaper.h"
#include "_ViewsWarpImage.h"
#include "_ViewsWarpView.h"

/* The definition Views::ImageAlignment determines the set of constraints to apply 
   on a bitmap if its size differ from the size of the view where the bitmap is 
   shown. Depending on the active constraints a bitmap can be aligned or stretched 
   within the view's area. */
typedef enum
{
  ViewsImageAlignmentAlignHorzLeft      = 0x00000001,
  ViewsImageAlignmentAlignHorzCenter    = 0x00000002,
  ViewsImageAlignmentAlignHorzRight     = 0x00000004,
  ViewsImageAlignmentAlignVertTop       = 0x00000008,
  ViewsImageAlignmentAlignVertCenter    = 0x00000010,
  ViewsImageAlignmentAlignVertBottom    = 0x00000020,
  ViewsImageAlignmentScaleToFill        = 0x00000040,
  ViewsImageAlignmentScaleToFit         = 0x00000080,
  ViewsImageAlignmentStretchToFill      = 0x00000100
} ViewsImageAlignment;

/* The definition Views::TextAlignment determines the set of constraints to apply 
   on text rows and blocks if their size differ from the size of the view where 
   the text is shown. Depending on the active constraints the text rows can be aligned 
   horizontally and text blocks vertically. */
typedef enum
{
  ViewsTextAlignmentAlignHorzLeft       = 0x00000001,
  ViewsTextAlignmentAlignHorzCenter     = 0x00000002,
  ViewsTextAlignmentAlignHorzRight      = 0x00000004,
  ViewsTextAlignmentAlignVertTop        = 0x00000008,
  ViewsTextAlignmentAlignVertCenter     = 0x00000010,
  ViewsTextAlignmentAlignVertBottom     = 0x00000020,
  ViewsTextAlignmentAlignHorzJustified  = 0x00000040,
  ViewsTextAlignmentAlignHorzAuto       = 0x00000080
} ViewsTextAlignment;

/* The definition Views::Orientation determines the possible options to control 
   the orientation of the content displayed in the Views::Image, Views::Group and 
   Views::Text views. Depending on the selected option the view displays the content 
   rotated. */
typedef enum
{
  ViewsOrientationNormal                = 0,
  ViewsOrientationRotated_90            = 1,
  ViewsOrientationRotated_180           = 2,
  ViewsOrientationRotated_270           = 3
} ViewsOrientation;

#ifdef __cplusplus
  }
#endif

#endif /* Views_H */

/* Embedded Wizard */
