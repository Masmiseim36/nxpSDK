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

#ifndef _CoreKeyEvent_H
#define _CoreKeyEvent_H

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

#include "_CoreEvent.h"

/* Forward declaration of the class Core::KeyEvent */
#ifndef _CoreKeyEvent_
  EW_DECLARE_CLASS( CoreKeyEvent )
#define _CoreKeyEvent_
#endif


/* The class Core::KeyEvent provides a specialized event object able to deliver 
   key pressure and release events. Each time a key event occurs, the code of the 
   pressed or released key will be passed in the Core::KeyEvent object to the application. 
   On its part the application dispatches the event to the GUI components along 
   the focus path. See the method DispatchEvent() of the class Core::Group.
   The Mosaic framework defines a fixed set of the most important key codes (Menu, 
   Left, Up, Ok, Exit, Play, Record, etc.). The usage of these predefined key codes 
   increases the platform independence of your GUI application. For special key 
   codes, which are not included in this set, a range of UserXX key codes is provided. 
   The codes are defined in the enum Core::KeyCode.
   Beside the predefined fixed set of key codes the Core::KeyEvent object can also 
   deliver a UNICODE character codes. This is very useful if your target device 
   supports an alpha numerical keyboard.
   Usually the key codes and the character codes are used exclusively in the Core::KeyEvent 
   object. If an event contains a valid key code (Menu, Left, Ok, etc.), the character 
   code is set to '\x0000'. In the case, a character code is passed in the event, 
   the key code is set to NoKey. Digits, alpha signs 'A' .. 'Z' and some few special 
   signs like '+', '-', '*' are handled in a special manner, they are passed as 
   well as key codes Key0 .. Key9, KeyA .. KeyZ, KeyPlus, KeyMinus, KeyMultiply 
   and as character codes '0' .. '9', 'A' .. 'Z', '+', '-', '*'. */
EW_DEFINE_FIELDS( CoreKeyEvent, CoreEvent )
  EW_VARIABLE( Code,            XEnum )
  EW_VARIABLE( CharCode,        XChar )
  EW_VARIABLE( Down,            XBool )
EW_END_OF_FIELDS( CoreKeyEvent )

/* Virtual Method Table (VMT) for the class : 'Core::KeyEvent' */
EW_DEFINE_METHODS( CoreKeyEvent, CoreEvent )
EW_END_OF_METHODS( CoreKeyEvent )

/* The method Initialize2() initializes this Core::KeyEvent object with the given 
   parameters. This method stores the UNICODE character code aCode in the variable 
   @CharCode and returns this event object to the caller. The @Code variable will 
   be initialized with the Core::KeyCode.NoKey value unless a '0' .. '9', 'A' .. 
   'Z' character code is passed to this method. In this case the variable @Code 
   is initialized with the corresponding key code Key0 .. Key9 or KeyA .. KeyZ. */
CoreKeyEvent CoreKeyEvent_Initialize2( CoreKeyEvent _this, XChar aCode, XBool aDown );

/* Wrapper function for the non virtual method : 'Core::KeyEvent.Initialize2()' */
CoreKeyEvent CoreKeyEvent__Initialize2( void* _this, XChar aCode, XBool aDown );

/* The following define announces the presence of the method Core::KeyEvent.Initialize2(). */
#define _CoreKeyEvent__Initialize2_

/* The method Initialize() initializes this Core::KeyEvent object with the given 
   parameters. This method stores the key code aCode in the variable @Code and returns 
   this event object to the caller. The @CharCode variable will be initialized with 
   the zero character '\x0000' unless a Key0 .. Key9; KeyA .. KeyZ code is passed 
   to this method. In this case the variable @CharCode is initialized with the corresponding 
   UNICODE character code '0' .. '9' or 'A' .. 'Z'. */
CoreKeyEvent CoreKeyEvent_Initialize( CoreKeyEvent _this, XEnum aCode, XBool aDown );

/* Wrapper function for the non virtual method : 'Core::KeyEvent.Initialize()' */
CoreKeyEvent CoreKeyEvent__Initialize( void* _this, XEnum aCode, XBool aDown );

/* The following define announces the presence of the method Core::KeyEvent.Initialize(). */
#define _CoreKeyEvent__Initialize_

/* The method IsCode() compares the key code stored within the event with the code 
   passed in the parameter aCodeOrCategory and returns 'true' or 'false' depending 
   on the result of this operation.
   The method is able to test whether a code does belong to a code category. Categories 
   are special codes defined in the Core::KeyCode enumeration, like AlphaKeys, DigitKeys 
   or CursorKeys. They stand for an entire range of key codes. */
XBool CoreKeyEvent_IsCode( CoreKeyEvent _this, XEnum aCodeOrCategory );

#ifdef __cplusplus
  }
#endif

#endif /* _CoreKeyEvent_H */

/* Embedded Wizard */
