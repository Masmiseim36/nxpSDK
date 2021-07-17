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

#ifndef _CorePropertyObserver_H
#define _CorePropertyObserver_H

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

/* Forward declaration of the class Core::PropertyObserver */
#ifndef _CorePropertyObserver_
  EW_DECLARE_CLASS( CorePropertyObserver )
#define _CorePropertyObserver_
#endif


/* The class Core::PropertyObserver provides a convenient component able to react 
   to notifications when an associated property changes its value. The component 
   provides a simple interface to register a slot method which will receive a signal 
   when a property is notified as changed. The observed property is determined in 
   @Outlet. The slot method can be stored in the property @OnEvent. */
EW_DEFINE_FIELDS( CorePropertyObserver, XObject )
  EW_PROPERTY( OnEvent,         XSlot )
  EW_PROPERTY( Outlet,          XRef )
  EW_PROPERTY( Enabled,         XBool )
EW_END_OF_FIELDS( CorePropertyObserver )

/* Virtual Method Table (VMT) for the class : 'Core::PropertyObserver' */
EW_DEFINE_METHODS( CorePropertyObserver, XObject )
EW_END_OF_METHODS( CorePropertyObserver )

/* 'C' function for method : 'Core::PropertyObserver.onEvent()' */
void CorePropertyObserver_onEvent( CorePropertyObserver _this, XObject sender );

/* 'C' function for method : 'Core::PropertyObserver.OnSetOutlet()' */
void CorePropertyObserver_OnSetOutlet( CorePropertyObserver _this, XRef value );

#ifdef __cplusplus
  }
#endif

#endif /* _CorePropertyObserver_H */

/* Embedded Wizard */
