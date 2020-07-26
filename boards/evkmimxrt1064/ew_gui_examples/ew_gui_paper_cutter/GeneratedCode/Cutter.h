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
* Version  : 9.30
* Date     : 14.02.2020  8:00:50
* Profile  : iMX_RT
* Platform : NXP.iMX_RT.RGB565
*
*******************************************************************************/

#ifndef Cutter_H
#define Cutter_H

#ifdef __cplusplus
  extern "C"
  {
#endif

#include "ewrte.h"
#if EW_RTE_VERSION != 0x0009001E
  #error Wrong version of Embedded Wizard Runtime Environment.
#endif

#include "ewgfx.h"
#if EW_GFX_VERSION != 0x0009001E
  #error Wrong version of Embedded Wizard Graphics Engine.
#endif

#include "_CutterBarGraph.h"
#include "_CutterBatchDialog.h"
#include "_CutterBatchWindow.h"
#include "_CutterButton.h"
#include "_CutterCancelButton.h"
#include "_CutterCutter.h"
#include "_CutterDeviceClass.h"
#include "_CutterFeedButton.h"
#include "_CutterFeedDialog.h"
#include "_CutterFeedWindow.h"
#include "_CutterFormatDialog.h"
#include "_CutterFormatWindow.h"
#include "_CutterHorzSlider.h"
#include "_CutterMainButton.h"
#include "_CutterPaperCut.h"
#include "_CutterPaperFeed.h"
#include "_CutterRemainingWindow.h"
#include "_CutterSpeedButton.h"
#include "_CutterSpeedDialog.h"
#include "_CutterSpeedWindow.h"
#include "_CutterStatusBar.h"
#include "_CutterVertSlider.h"
#include "_CutterWindow.h"

/* This is a font resource. */
EW_DECLARE_FONT_RES( CutterCaptionFont )

/* This is a font resource. */
EW_DECLARE_FONT_RES( CutterSmallDigitFont )

/* This is a font resource. */
EW_DECLARE_FONT_RES( CutterDescrFont )

/* This is a bitmap resource. */
EW_DECLARE_BITMAP_RES( CutterPaperStripe )

/* This is a bitmap resource. */
EW_DECLARE_BITMAP_RES( CutterPaperFormat )

/* This is a bitmap resource. */
EW_DECLARE_BITMAP_RES( CutterPaperBatch )

/* This is a bitmap resource. */
EW_DECLARE_BITMAP_RES( CutterPaperSpeed )

/* This is a font resource. */
EW_DECLARE_FONT_RES( CutterLargeDigitFont )

/* This is a font resource. */
EW_DECLARE_FONT_RES( CutterMediumDigitFont )

/* This is a bitmap resource. */
EW_DECLARE_BITMAP_RES( CutterButtonBig )

/* This is a font resource. */
EW_DECLARE_FONT_RES( CutterMainButtonFont )

/* This is a bitmap resource. */
EW_DECLARE_BITMAP_RES( CutterStatusIcons )

/* This is a bitmap resource. */
EW_DECLARE_BITMAP_RES( CutterArrows )

/* This is a bitmap resource. */
EW_DECLARE_BITMAP_RES( CutterPaper )

/* This is a bitmap resource. */
EW_DECLARE_BITMAP_RES( CutterThumb )

/* This is a font resource. */
EW_DECLARE_FONT_RES( CutterButtonFont )

/* This is a bitmap resource. */
EW_DECLARE_BITMAP_RES( CutterBarRaster )

/* Color constant according UI design. */
extern const XColor CutterCaptionColor;

/* Color constant according UI design. */
extern const XColor CutterBackColor;

/* Color constant according UI design. */
extern const XColor CutterSelectColor;

/* Color constant according UI design. */
extern const XColor CutterStartColor;

/* Color constant according UI design. */
extern const XColor CutterStopColor;

/* This is an enumeration definition for indicating the current opeation mode of 
   the cutter. */
typedef enum
{
  CutterOperationModeProgramming        = 0,
  CutterOperationModeRunning            = 1,
  CutterOperationModeExclusive          = 2
} CutterOperationMode;

/* Constant to define the spacing between the windows. */
extern const XPoint CutterSpacing;

/* Constant to define the border around the screen. */
extern const XPoint CutterBorder;

/* Color constant according UI design. */
extern const XColor CutterTextColor;

/* Constant to define the size of the entire paper cutter application. */
extern const XPoint CutterScreenSize;

/* User defined auto object: 'Cutter::Device' */
EW_DECLARE_AUTOOBJECT( CutterDevice, CutterDeviceClass )

/* This is an enumeration definition for indicating the current mode of the button. */
typedef enum
{
  CutterButtonModeStart                 = 0,
  CutterButtonModeStop                  = 1,
  CutterButtonModeOk                    = 2
} CutterButtonMode;

/* Color constant according UI design. */
extern const XColor CutterButtonBgColor;

/* Color constant according UI design. */
extern const XColor CutterButtonTextColor;

#ifdef __cplusplus
  }
#endif

#endif /* Cutter_H */

/* Embedded Wizard */
