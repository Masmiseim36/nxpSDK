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

#ifndef _CoreEvent_H
#define _CoreEvent_H

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

/* Forward declaration of the class Core::Event */
#ifndef _CoreEvent_
  EW_DECLARE_CLASS( CoreEvent )
#define _CoreEvent_
#endif


/* The class Core::Event provides the base functionality for all events. The events 
   are used to feed GUI components with user inputs and notifications from the target 
   device, like an 'empty battery' event, etc. The events are dispatched and propagated 
   by the framework. The necessary infrastructure is provided in the classes Core::View 
   and Core::Group.
   The class Core::Event doesn't define any particular behavior. It exists as an 
   empty base for all derived event classes like the Core::KeyEvent or Core::LanguageEvent. */
EW_DEFINE_FIELDS( CoreEvent, XObject )
  EW_VARIABLE( Time,            XUInt32 )
  EW_VARIABLE( IsTimeExact,     XBool )
EW_END_OF_FIELDS( CoreEvent )

/* Virtual Method Table (VMT) for the class : 'Core::Event' */
EW_DEFINE_METHODS( CoreEvent, XObject )
EW_END_OF_METHODS( CoreEvent )

/* 'C' function for method : 'Core::Event.Init()' */
void CoreEvent_Init( CoreEvent _this, XHandle aArg );

/* The method 'GetCurrentTime()' returns the current time expressed in milliseconds. 
   The value can be used e.g. to calculate the time span elapsed since the event 
   was generated (see @Time). */
XUInt32 CoreEvent_GetCurrentTime( CoreEvent _this );

#ifdef __cplusplus
  }
#endif

#endif /* _CoreEvent_H */

/* Embedded Wizard */
