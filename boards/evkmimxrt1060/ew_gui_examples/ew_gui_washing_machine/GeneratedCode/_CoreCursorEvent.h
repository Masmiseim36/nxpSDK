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

#ifndef _CoreCursorEvent_H
#define _CoreCursorEvent_H

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

/* Forward declaration of the class Core::CursorEvent */
#ifndef _CoreCursorEvent_
  EW_DECLARE_CLASS( CoreCursorEvent )
#define _CoreCursorEvent_
#endif


/* The class Core::CursorEvent provides a specialized event object able to deliver 
   touch screen pressure and release events. Each time the user clicks with the 
   mouse cursor on the screen, hits it with a pen or touches it with his finger, 
   a cursor event is prepared and dispatched to the view which is willing to handle 
   it. See the method DriveCursorHitting() of the class Core::Root.
   The event contains information about the position where the user has tapped (@HittingPos 
   and @CurrentPos), whether the user has pressed or released the finger (@Down), 
   how often has the user tapped at the same position (@StrikeCount), etc.
   The dispatching of cursor events will always take place in so called 'grab cycles'. 
   A grab cycle starts when the user touches the screen and exists until the user 
   has released it. At the beginning the framework determines the view which is 
   lying at the touched position and is willing to receive cursor events. This view 
   will become the target for all following events while this grab cycle -> the 
   view 'grabs' the cursor for this time.
   Afterwards the target view will receive the first Core::CursorEvent with its 
   variable @Down == 'true'. At the end of the grab cycle, after the user has released 
   the finger, the target view will receive the last Core::CursorEvent with the 
   variable @Down == 'false'. In the meantime the framework will feed the target 
   view with more events: the hold and the drag events.
   The hold events are generated periodically in constant intervals. They store 
   the current cursor position and the time since the grab cycle began @HoldPeriod. 
   Please note, these hold events are sent with the variable @Down == 'true'. In 
   order to distinguish between the first event and the following hold events, the 
   @HoldPeriod variable should be evaluated.
   Beside the hold events, the target view will also receive drag events as soon 
   as the user strokes the screen. This is communicated by events of the class Core::DragEvent. */
EW_DEFINE_FIELDS( CoreCursorEvent, CoreEvent )
  EW_VARIABLE( GlobalHittingPos, XPoint )
  EW_VARIABLE( GlobalCurrentPos, XPoint )
  EW_VARIABLE( StrikeCount,     XInt32 )
  EW_VARIABLE( HoldPeriod,      XInt32 )
  EW_VARIABLE( HittingPos,      XPoint )
  EW_VARIABLE( CurrentPos,      XPoint )
  EW_VARIABLE( Finger,          XInt32 )
  EW_VARIABLE( Down,            XBool )
  EW_VARIABLE( AutoDeflected,   XBool )
EW_END_OF_FIELDS( CoreCursorEvent )

/* Virtual Method Table (VMT) for the class : 'Core::CursorEvent' */
EW_DEFINE_METHODS( CoreCursorEvent, CoreEvent )
EW_END_OF_METHODS( CoreCursorEvent )

/* The method InitializeHold() simplifies the initialization of this Core::CursorEvent 
   object. This method stores the given parameters in the variables of this event 
   object, sets its variable @Down 'true' and returns this event object to the caller. */
CoreCursorEvent CoreCursorEvent_InitializeHold( CoreCursorEvent _this, XInt32 aFinger, 
  XPoint aCurrentPos, XPoint aHittingPos, XInt32 aHoldPeriod, XInt32 aStrikeCount, 
  XPoint aHitOffset, XPoint aGlobalCurrentPos, XPoint aGlobalHittingPos );

/* Wrapper function for the non virtual method : 'Core::CursorEvent.InitializeHold()' */
CoreCursorEvent CoreCursorEvent__InitializeHold( void* _this, XInt32 aFinger, XPoint 
  aCurrentPos, XPoint aHittingPos, XInt32 aHoldPeriod, XInt32 aStrikeCount, XPoint 
  aHitOffset, XPoint aGlobalCurrentPos, XPoint aGlobalHittingPos );

/* The following define announces the presence of the method Core::CursorEvent.InitializeHold(). */
#define _CoreCursorEvent__InitializeHold_

/* The method InitializeUp() simplifies the initialization of this Core::CursorEvent 
   object. This method stores the given parameters in the variables of this event 
   object, sets its variable @Down 'false' and returns this event object to the 
   caller. */
CoreCursorEvent CoreCursorEvent_InitializeUp( CoreCursorEvent _this, XInt32 aFinger, 
  XPoint aCurrentPos, XPoint aHittingPos, XInt32 aHoldPeriod, XInt32 aStrikeCount, 
  XPoint aHitOffset, XBool aAutoDeflected, XPoint aGlobalCurrentPos, XPoint aGlobalHittingPos );

/* Wrapper function for the non virtual method : 'Core::CursorEvent.InitializeUp()' */
CoreCursorEvent CoreCursorEvent__InitializeUp( void* _this, XInt32 aFinger, XPoint 
  aCurrentPos, XPoint aHittingPos, XInt32 aHoldPeriod, XInt32 aStrikeCount, XPoint 
  aHitOffset, XBool aAutoDeflected, XPoint aGlobalCurrentPos, XPoint aGlobalHittingPos );

/* The following define announces the presence of the method Core::CursorEvent.InitializeUp(). */
#define _CoreCursorEvent__InitializeUp_

/* The method InitializeDown() simplifies the initialization of this Core::CursorEvent 
   object. This method stores the given parameters in the variables of this event 
   object, sets its variable @Down 'true' and returns this event object to the caller. */
CoreCursorEvent CoreCursorEvent_InitializeDown( CoreCursorEvent _this, XInt32 aFinger, 
  XPoint aCurrentPos, XInt32 aStrikeCount, XPoint aHitOffset, XBool aAutoDeflected, 
  XPoint aGlobalCurrentPos );

/* Wrapper function for the non virtual method : 'Core::CursorEvent.InitializeDown()' */
CoreCursorEvent CoreCursorEvent__InitializeDown( void* _this, XInt32 aFinger, XPoint 
  aCurrentPos, XInt32 aStrikeCount, XPoint aHitOffset, XBool aAutoDeflected, XPoint 
  aGlobalCurrentPos );

/* The following define announces the presence of the method Core::CursorEvent.InitializeDown(). */
#define _CoreCursorEvent__InitializeDown_

#ifdef __cplusplus
  }
#endif

#endif /* _CoreCursorEvent_H */

/* Embedded Wizard */
