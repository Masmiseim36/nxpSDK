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

#ifndef _BezierClockDeviceClass_H
#define _BezierClockDeviceClass_H

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
