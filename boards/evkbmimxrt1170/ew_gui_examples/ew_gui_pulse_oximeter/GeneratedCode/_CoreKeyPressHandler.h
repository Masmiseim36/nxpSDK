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

#ifndef _CoreKeyPressHandler_H
#define _CoreKeyPressHandler_H

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

/* Forward declaration of the class Core::KeyEvent */
#ifndef _CoreKeyEvent_
  EW_DECLARE_CLASS( CoreKeyEvent )
#define _CoreKeyEvent_
#endif

/* Forward declaration of the class Core::KeyPressHandler */
#ifndef _CoreKeyPressHandler_
  EW_DECLARE_CLASS( CoreKeyPressHandler )
#define _CoreKeyPressHandler_
#endif


/* The class Core::KeyPressHandler provides a universal keyboard event handler able 
   to process key press and release events. It compares the received key codes with 
   the value of the property @Filter and if the codes do match the filter condition, 
   sends signals to the slot methods stored in the @OnPress, @OnHold and @OnRelease 
   properties.
   Keyboard handlers can be created at the design time only. You can simply drag-and-drop 
   them from the Embedded Wizard Gallery into the Composer window where you are 
   editing the GUI component. If necessary several keyboard handlers can be added 
   to the component. For example, one handler can react to the 'Enter' key and the 
   other to the 'Exit' key. The order in which the handlers will process the events 
   corresponds to their Z-order. You can verify and modify this Z-order in the Embedded 
   Wizard Inspector. Please note, keyboard handlers created at the runtime with 
   the Chora 'new' operator will not work!
   The handler provides several variables with details of the received keyboard 
   event, like the key code (@Code or @CharCode), whether the user has pressed or 
   released the key (@Down), etc. These variables can be evaluated in the implementation 
   of the slot method.
   The property @Enabled can be used to activate/deactivate the keyboard handler. 
   Disabled handler will not react to the events. */
EW_DEFINE_FIELDS( CoreKeyPressHandler, XObject )
  EW_VARIABLE( next,            CoreKeyPressHandler )
  EW_PROPERTY( OnRelease,       XSlot )
  EW_PROPERTY( OnPress,         XSlot )
  EW_PROPERTY( OnHold,          XSlot )
  EW_VARIABLE( pressCounter,    XInt32 )
  EW_VARIABLE( Time,            XUInt32 )
  EW_VARIABLE( RepetitionCount, XInt32 )
  EW_PROPERTY( Filter,          XEnum )
  EW_VARIABLE( Code,            XEnum )
  EW_VARIABLE( CharCode,        XChar )
  EW_PROPERTY( Enabled,         XBool )
  EW_VARIABLE( Down,            XBool )
  EW_VARIABLE( Repetition,      XBool )
  EW_VARIABLE( Continue,        XBool )
EW_END_OF_FIELDS( CoreKeyPressHandler )

/* Virtual Method Table (VMT) for the class : 'Core::KeyPressHandler' */
EW_DEFINE_METHODS( CoreKeyPressHandler, XObject )
EW_END_OF_METHODS( CoreKeyPressHandler )

/* 'C' function for method : 'Core::KeyPressHandler.Init()' */
void CoreKeyPressHandler_Init( CoreKeyPressHandler _this, XHandle aArg );

/* 'C' function for method : 'Core::KeyPressHandler.HandleEvent()' */
XBool CoreKeyPressHandler_HandleEvent( CoreKeyPressHandler _this, CoreKeyEvent aEvent );

#ifdef __cplusplus
  }
#endif

#endif /* _CoreKeyPressHandler_H */

/* Embedded Wizard */
