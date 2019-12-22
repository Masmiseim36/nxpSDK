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

#ifndef _BezierClockDeviceClass_H
#define _BezierClockDeviceClass_H

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

/* Forward declaration of the class BezierClock::DeviceClass */
#ifndef _BezierClockDeviceClass_
  EW_DECLARE_CLASS( BezierClockDeviceClass )
#define _BezierClockDeviceClass_
#endif


/* This class implements the interface between the GUI application and the device. */
EW_DEFINE_FIELDS( BezierClockDeviceClass, XObject )
  EW_PROPERTY( CurrentBackground, XInt32 )
EW_END_OF_FIELDS( BezierClockDeviceClass )

/* Virtual Method Table (VMT) for the class : 'BezierClock::DeviceClass' */
EW_DEFINE_METHODS( BezierClockDeviceClass, XObject )
EW_END_OF_METHODS( BezierClockDeviceClass )

/* This method implements a device command that calls the underlying device driver 
   to change the RTC. */
void BezierClockDeviceClass_SetTime( BezierClockDeviceClass _this, XUInt32 aTime );

/* 'C' function for method : 'BezierClock::DeviceClass.OnSetCurrentBackground()' */
void BezierClockDeviceClass_OnSetCurrentBackground( BezierClockDeviceClass _this, 
  XInt32 value );

/* Default onget method for the property 'CurrentBackground' */
XInt32 BezierClockDeviceClass_OnGetCurrentBackground( BezierClockDeviceClass _this );

#ifdef __cplusplus
  }
#endif

#endif /* _BezierClockDeviceClass_H */

/* Embedded Wizard */
