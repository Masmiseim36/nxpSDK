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

#ifndef _CoreTimer_H
#define _CoreTimer_H

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

/* Forward declaration of the class Core::Timer */
#ifndef _CoreTimer_
  EW_DECLARE_CLASS( CoreTimer )
#define _CoreTimer_
#endif


/* The class Core::Timer provides a mechanism to trigger periodical operations in 
   predetermined intervals. The intervals are defined by the properties @Begin und 
   @Period. After an interval is elapsed, the timer sends a signal to the method 
   stored in the property OnTrigger. Optionally, the method @Trigger() can be overridden 
   and implemented in classes derived from Core::Timer.
   The timer will always start with the interval specified in @Begin. After the 
   begin interval is elapsed, the timer can continue running with the interval defined 
   in @Period.
   Whether the timer is running or not is controlled by its property @Enabled. It 
   can be used to start and stop the timer. The timer can also be controlled by 
   sending signals to its @StartTimer and @StopTimer slot methods. */
EW_DEFINE_FIELDS( CoreTimer, XObject )
  EW_PROPERTY( OnTrigger,       XSlot )
  EW_VARIABLE( timer,           XHandle )
  EW_VARIABLE( Time,            XUInt32 )
  EW_PROPERTY( Period,          XInt32 )
  EW_PROPERTY( Begin,           XInt32 )
  EW_PROPERTY( Enabled,         XBool )
EW_END_OF_FIELDS( CoreTimer )

/* Virtual Method Table (VMT) for the class : 'Core::Timer' */
EW_DEFINE_METHODS( CoreTimer, XObject )
  EW_METHOD( Trigger,           void )( CoreTimer _this )
EW_END_OF_METHODS( CoreTimer )

/* 'C' function for method : 'Core::Timer.Done()' */
void CoreTimer_Done( CoreTimer _this );

/* 'C' function for method : 'Core::Timer.restart()' */
void CoreTimer_restart( CoreTimer _this, XInt32 aBegin, XInt32 aPeriod );

/* 'C' function for method : 'Core::Timer.OnSetPeriod()' */
void CoreTimer_OnSetPeriod( CoreTimer _this, XInt32 value );

/* 'C' function for method : 'Core::Timer.OnSetBegin()' */
void CoreTimer_OnSetBegin( CoreTimer _this, XInt32 value );

/* 'C' function for method : 'Core::Timer.OnSetEnabled()' */
void CoreTimer_OnSetEnabled( CoreTimer _this, XBool value );

/* The method 'GetCurrentTime()' returns the current time expressed in milliseconds. 
   The value can be used e.g. to calculate the time span elapsed since the timer 
   was expired (see @Time). */
XUInt32 CoreTimer_GetCurrentTime( CoreTimer _this );

/* The method Trigger() will be invoked when the timer is expired (when the interval 
   defined in @Begin or @Period is elapsed). The method can be overridden and implemented 
   in derived classes. The default implementation of this method sends a signal 
   to the slot method stored in the @OnTrigger property. */
void CoreTimer_Trigger( CoreTimer _this );

/* Wrapper function for the virtual method : 'Core::Timer.Trigger()' */
void CoreTimer__Trigger( void* _this );

/* The following define announces the presence of the method Core::Timer.Trigger(). */
#define _CoreTimer__Trigger_

/* The slot method 'StopTimer' will stop the running timer if a signal is sent to 
   this slot method. */
void CoreTimer_StopTimer( CoreTimer _this, XObject sender );

/* The slot method 'StartTimer' will re-start the timer if a signal is sent to this 
   slot method. The timer will start with the interval specified in @Begin. After 
   the interval is elapsed, the timer continue running with the interval defined 
   in @Period. */
void CoreTimer_StartTimer( CoreTimer _this, XObject sender );

#ifdef __cplusplus
  }
#endif

#endif /* _CoreTimer_H */

/* Embedded Wizard */
