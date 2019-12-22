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

#ifndef _CoreEvent_H
#define _CoreEvent_H

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

/* The method 'GetCurrentTime()' returns the current time expressed in milliseconds. 
   The value can be used e.g. to calculate the time span elapsed since the event 
   was generated (see @Time). */
XUInt32 CoreEvent_GetCurrentTime( CoreEvent _this );

/* 'C' function for method : 'Core::Event.Init()' */
void CoreEvent_Init( CoreEvent _this, XHandle aArg );

#ifdef __cplusplus
  }
#endif

#endif /* _CoreEvent_H */

/* Embedded Wizard */
