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

#ifndef _CoreDragEvent_H
#define _CoreDragEvent_H

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

#include "_CoreEvent.h"

/* Forward declaration of the class Core::DragEvent */
#ifndef _CoreDragEvent_
  EW_DECLARE_CLASS( CoreDragEvent )
#define _CoreDragEvent_
#endif


/* The class Core::DragEvent provides a specialized event object able to deliver 
   drag events. These events are sent when the user has touched the screen and then 
   moved the finger. See the method DriveCursorMovement() of the class Core::Root.
   The event contains information about the position where the user has tapped (@HittingPos), 
   the current position after the drag operation (@CurrentPos), the displacement 
   since the last drag event (@Offset), etc.
   The drag events are sent together with the cursor events Core::CursorEvent. */
EW_DEFINE_FIELDS( CoreDragEvent, CoreEvent )
  EW_VARIABLE( GlobalHittingPos, XPoint )
  EW_VARIABLE( GlobalCurrentPos, XPoint )
  EW_VARIABLE( StrikeCount,     XInt32 )
  EW_VARIABLE( HoldPeriod,      XInt32 )
  EW_VARIABLE( Offset,          XPoint )
  EW_VARIABLE( HittingPos,      XPoint )
  EW_VARIABLE( CurrentPos,      XPoint )
  EW_VARIABLE( Finger,          XInt32 )
EW_END_OF_FIELDS( CoreDragEvent )

/* Virtual Method Table (VMT) for the class : 'Core::DragEvent' */
EW_DEFINE_METHODS( CoreDragEvent, CoreEvent )
EW_END_OF_METHODS( CoreDragEvent )

/* The method Initialize() simplifies the initialization of this Core::DragEvent 
   object. This method stores the given parameters in the variables of this event 
   object and returns this event object to the caller. */
CoreDragEvent CoreDragEvent_Initialize( CoreDragEvent _this, XInt32 aFinger, XPoint 
  aCurrentPos, XPoint aHittingPos, XPoint aOffset, XInt32 aHoldPeriod, XInt32 aSequelCount, 
  XPoint aHitOffset, XPoint aGlobalCurrentPos, XPoint aGlobalHittingPos );

/* Wrapper function for the non virtual method : 'Core::DragEvent.Initialize()' */
CoreDragEvent CoreDragEvent__Initialize( void* _this, XInt32 aFinger, XPoint aCurrentPos, 
  XPoint aHittingPos, XPoint aOffset, XInt32 aHoldPeriod, XInt32 aSequelCount, XPoint 
  aHitOffset, XPoint aGlobalCurrentPos, XPoint aGlobalHittingPos );

/* The following define announces the presence of the method Core::DragEvent.Initialize(). */
#define _CoreDragEvent__Initialize_

#ifdef __cplusplus
  }
#endif

#endif /* _CoreDragEvent_H */

/* Embedded Wizard */
