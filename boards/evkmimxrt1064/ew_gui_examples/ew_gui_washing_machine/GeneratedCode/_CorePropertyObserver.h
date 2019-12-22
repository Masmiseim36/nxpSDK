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

#ifndef _CorePropertyObserver_H
#define _CorePropertyObserver_H

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
