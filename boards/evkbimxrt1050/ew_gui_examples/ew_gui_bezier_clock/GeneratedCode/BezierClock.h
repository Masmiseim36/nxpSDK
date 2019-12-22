/*******************************************************************************
*
* E M B E D D E D   W I Z A R D   P R O J E C T
*
*                                                Copyright (c) TARA Systems GmbH
*                                    written by Paul Banach and Manfred Schweyer
*
********************************************************************************
*
* This file was generated automatically by Embedded Wizard Studio.
*
* Please do not make any modifications of this file! The modifications are lost
* when the file is generated again by Embedded Wizard Studio!
*
* The template of this heading text can be found in the file 'head.ewt' in the
* directory 'Platforms' of your Embedded Wizard installation directory. If you
* wish to adapt this text, please copy the template file 'head.ewt' into your
* project directory and edit the copy only. Please avoid any modifications of
* the original template file!
*
* Version  : 9.20
* Profile  : iMX_RT
* Platform : NXP.iMX_RT.RGB565
*
*******************************************************************************/

#ifndef BezierClock_H
#define BezierClock_H

#ifdef __cplusplus
  extern "C"
  {
#endif

#include "ewrte.h"
#if EW_RTE_VERSION != 0x00090014
  #error Wrong version of Embedded Wizard Runtime Environment.
#endif

#include "ewgfx.h"
#if EW_GFX_VERSION != 0x00090014
  #error Wrong version of Embedded Wizard Graphics Engine.
#endif

#include "_BezierClockBackground.h"
#include "_BezierClockBezierClock.h"
#include "_BezierClockBubble.h"
#include "_BezierClockClockScreen.h"
#include "_BezierClockDatePicker.h"
#include "_BezierClockDeviceClass.h"
#include "_BezierClockDigit.h"
#include "_BezierClockSettingsScreen.h"
#include "_BezierClockStyleScreen.h"
#include "_BezierClockTimePicker.h"
#include "_BezierClockTimerScreen.h"
#include "_BezierClockTimerWheel.h"

/* Font resource used per default by 'Text' and 'Attributed Text' views to ensure 
   that new views don't remain empty on the screen. */
EW_DECLARE_FONT_RES( BezierClockFontL )

/* Font resource used per default by 'Text' and 'Attributed Text' views to ensure 
   that new views don't remain empty on the screen. */
EW_DECLARE_FONT_RES( BezierClockFontXXL )

/* Bitmap resource : 'BezierClock::BackgroundPreview' */
EW_DECLARE_BITMAP_RES( BezierClockBackgroundPreview )

/* Bitmap resource : 'BezierClock::Blue' */
EW_DECLARE_BITMAP_RES( BezierClockBlue )

/* Bitmap resource : 'BezierClock::Dark' */
EW_DECLARE_BITMAP_RES( BezierClockDark )

/* Bitmap resource : 'BezierClock::Orange' */
EW_DECLARE_BITMAP_RES( BezierClockOrange )

/* This object represents an instance of the device interface implemented in the 
   class BezierClock::DeviceClass. */
EW_DECLARE_AUTOOBJECT( BezierClockDevice, BezierClockDeviceClass )

#ifdef __cplusplus
  }
#endif

#endif /* BezierClock_H */

/* Embedded Wizard */
