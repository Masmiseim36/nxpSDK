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

#ifndef _CoreCursorGrabEvent_H
#define _CoreCursorGrabEvent_H

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

/* Forward declaration of the class Core::CursorGrabEvent */
#ifndef _CoreCursorGrabEvent_
  EW_DECLARE_CLASS( CoreCursorGrabEvent )
#define _CoreCursorGrabEvent_
#endif

/* Forward declaration of the class Core::View */
#ifndef _CoreView_
  EW_DECLARE_CLASS( CoreView )
#define _CoreView_
#endif


/* The class Core::CursorGrabEvent provides a specialized event object able to deliver 
   touch screen events associated with the beginning and the end of a cursor grab 
   cycle.
   Each time the user clicks with the mouse cursor on the screen, hits it with a 
   pen or touches it with his finger, the Mosaic searches for the view, which is 
   lying at the touched screen position and willing to handle the user interaction. 
   This found view will become the target for all subsequent cursor events related 
   to this interaction. This relation is called the 'grab cycle'.
   The event Core::CursorGrabEvent is used to notify all other views about the beginning 
   and the end of the grab cycle. With the event the views receive the information 
   where the user has touched the screen and which view has been selected as the 
   target.
   In this manner, although not being the target of the cursor event, the view can 
   perform operations like to stop a running slide animation etc. */
EW_DEFINE_FIELDS( CoreCursorGrabEvent, CoreEvent )
  EW_VARIABLE( Target,          CoreView )
  EW_VARIABLE( GlobalCurrentPos, XPoint )
  EW_VARIABLE( StrikeCount,     XInt32 )
  EW_VARIABLE( Finger,          XInt32 )
  EW_VARIABLE( Down,            XBool )
  EW_VARIABLE( AutoDeflected,   XBool )
EW_END_OF_FIELDS( CoreCursorGrabEvent )

/* Virtual Method Table (VMT) for the class : 'Core::CursorGrabEvent' */
EW_DEFINE_METHODS( CoreCursorGrabEvent, CoreEvent )
EW_END_OF_METHODS( CoreCursorGrabEvent )

/* The method InitializeUp() simplifies the initialization of this Core::CursorGrabEvent 
   object. This method stores the given parameters in the variables of this event 
   object, sets its variable @Down 'false' and returns this event object to the 
   caller. */
CoreCursorGrabEvent CoreCursorGrabEvent_InitializeUp( CoreCursorGrabEvent _this, 
  XInt32 aFinger, XInt32 aStrikeCount, XBool aAutoDeflected, CoreView aTarget, XPoint 
  aGlobalCurrentPos );

/* Wrapper function for the non virtual method : 'Core::CursorGrabEvent.InitializeUp()' */
CoreCursorGrabEvent CoreCursorGrabEvent__InitializeUp( void* _this, XInt32 aFinger, 
  XInt32 aStrikeCount, XBool aAutoDeflected, CoreView aTarget, XPoint aGlobalCurrentPos );

/* The following define announces the presence of the method Core::CursorGrabEvent.InitializeUp(). */
#define _CoreCursorGrabEvent__InitializeUp_

/* The method InitializeDown() simplifies the initialization of this Core::CursorGrabEvent 
   object. This method stores the given parameters in the variables of this event 
   object, sets its variable @Down 'true' and returns this event object to the caller. */
CoreCursorGrabEvent CoreCursorGrabEvent_InitializeDown( CoreCursorGrabEvent _this, 
  XInt32 aFinger, XInt32 aStrikeCount, XBool aAutoDeflected, CoreView aTarget, XPoint 
  aGlobalCurrentPos );

/* Wrapper function for the non virtual method : 'Core::CursorGrabEvent.InitializeDown()' */
CoreCursorGrabEvent CoreCursorGrabEvent__InitializeDown( void* _this, XInt32 aFinger, 
  XInt32 aStrikeCount, XBool aAutoDeflected, CoreView aTarget, XPoint aGlobalCurrentPos );

/* The following define announces the presence of the method Core::CursorGrabEvent.InitializeDown(). */
#define _CoreCursorGrabEvent__InitializeDown_

#ifdef __cplusplus
  }
#endif

#endif /* _CoreCursorGrabEvent_H */

/* Embedded Wizard */
