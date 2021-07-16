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
* Platform : NXP.iMX_RT.RGB565
*
*******************************************************************************/

#ifndef _AnimatedListDeviceClass_H
#define _AnimatedListDeviceClass_H

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

/* Forward declaration of the class AnimatedList::DeviceClass */
#ifndef _AnimatedListDeviceClass_
  EW_DECLARE_CLASS( AnimatedListDeviceClass )
#define _AnimatedListDeviceClass_
#endif


/* Controller class, containing all data set values and all current values of the 
   device. */
EW_DEFINE_FIELDS( AnimatedListDeviceClass, XObject )
  EW_ARRAY   ( WeekDays,        XString, [7])
  EW_PROPERTY( Hour,            XInt32 )
  EW_PROPERTY( Minute,          XInt32 )
  EW_PROPERTY( WeekDay,         XInt32 )
EW_END_OF_FIELDS( AnimatedListDeviceClass )

/* Virtual Method Table (VMT) for the class : 'AnimatedList::DeviceClass' */
EW_DEFINE_METHODS( AnimatedListDeviceClass, XObject )
EW_END_OF_METHODS( AnimatedListDeviceClass )

/* 'C' function for method : 'AnimatedList::DeviceClass.OnSetHour()' */
void AnimatedListDeviceClass_OnSetHour( AnimatedListDeviceClass _this, XInt32 value );

/* 'C' function for method : 'AnimatedList::DeviceClass.OnSetMinute()' */
void AnimatedListDeviceClass_OnSetMinute( AnimatedListDeviceClass _this, XInt32 
  value );

/* 'C' function for method : 'AnimatedList::DeviceClass.OnSetWeekDay()' */
void AnimatedListDeviceClass_OnSetWeekDay( AnimatedListDeviceClass _this, XInt32 
  value );

/* Default onget method for the property 'Hour' */
XInt32 AnimatedListDeviceClass_OnGetHour( AnimatedListDeviceClass _this );

/* Default onget method for the property 'Minute' */
XInt32 AnimatedListDeviceClass_OnGetMinute( AnimatedListDeviceClass _this );

/* Default onget method for the property 'WeekDay' */
XInt32 AnimatedListDeviceClass_OnGetWeekDay( AnimatedListDeviceClass _this );

#ifdef __cplusplus
  }
#endif

#endif /* _AnimatedListDeviceClass_H */

/* Embedded Wizard */
