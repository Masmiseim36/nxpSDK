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

#ifndef _CoreSimpleTouchHandler_H
#define _CoreSimpleTouchHandler_H

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

#include "_CoreQuadView.h"

/* Forward declaration of the class Core::CursorHit */
#ifndef _CoreCursorHit_
  EW_DECLARE_CLASS( CoreCursorHit )
#define _CoreCursorHit_
#endif

/* Forward declaration of the class Core::Event */
#ifndef _CoreEvent_
  EW_DECLARE_CLASS( CoreEvent )
#define _CoreEvent_
#endif

/* Forward declaration of the class Core::Group */
#ifndef _CoreGroup_
  EW_DECLARE_CLASS( CoreGroup )
#define _CoreGroup_
#endif

/* Forward declaration of the class Core::LayoutContext */
#ifndef _CoreLayoutContext_
  EW_DECLARE_CLASS( CoreLayoutContext )
#define _CoreLayoutContext_
#endif

/* Forward declaration of the class Core::SimpleTouchHandler */
#ifndef _CoreSimpleTouchHandler_
  EW_DECLARE_CLASS( CoreSimpleTouchHandler )
#define _CoreSimpleTouchHandler_
#endif

/* Forward declaration of the class Core::View */
#ifndef _CoreView_
  EW_DECLARE_CLASS( CoreView )
#define _CoreView_
#endif

/* Forward declaration of the class Graphics::Canvas */
#ifndef _GraphicsCanvas_
  EW_DECLARE_CLASS( GraphicsCanvas )
#define _GraphicsCanvas_
#endif


/* The class Core::SimpleTouchHandler provides a special kind of a view able to 
   react to touch screen events. Each time the user taps inside the area of the 
   handler view, the handler send signals to slot methods stored in its various 
   properties. For example, if there is a slot method assigned to the property @OnPress, 
   the handler will send a signal to this slot method as soon as the user has touched 
   the handler view area. The intention of this handler is to cover the internal 
   aspects of the cursor event handling.
   The simple touch handler recognizes the tap and drag operations only. More sophisticated 
   gesture handlers are available in other classes, e.g. Core::RotateTouchHandler 
   is able to recognize the rotation gesture.
   The touch area can assume any quad shape (4 corners polygon). Each corner can 
   be placed separately by the @Point1 .. @Point4 properties. In this manner fancy, 
   non rectangular touch areas can be determined.
   The handler provides several variables useful to query e.g. the current touch 
   position (@CurrentPos), etc. These variables can be simply evaluated in the implementation 
   of the slot method. If this handler is intended to process multi-touch events 
   (the property @EnableMultiTouch is 'true' and the target system can feed the 
   application with multi-touch events) you should additionally evaluate the variable 
   @Finger, which helps you to distinguish the touch events.
   The property @Enabled can be used to activate/deactivate the touch handler. Disabled 
   handler will not react to user taps.
   With the properties @RetargetCondition, @RetargetOffset and @RetargetDelay the 
   handler can be configured to automatically deflect the current event processing 
   to another handler as soon as the user has performed a simple gesture, e.g. wipe 
   down. Using these properties several handler can cooperate during an active user 
   interaction even if these handler overlap each other. For example, one handler 
   can process horizontal gestures while another handler will limit to vertical 
   gestures, etc.
   Due to its ancestry from the Core::View class, the touch handler can be arranged 
   within its @Owner or if @Embedded == 'true' within the boundary area of an Core::Outline 
   view in the same manner as it is done with all regular views.
   The touch handler itself is invisible except the Embedded Wizard Composer, where 
   all handler appear as semitransparent quads. This allows you to interact with 
   the handlers during the design time. */
EW_DEFINE_FIELDS( CoreSimpleTouchHandler, CoreQuadView )
  EW_PROPERTY( OnDrag,          XSlot )
  EW_PROPERTY( OnLeave,         XSlot )
  EW_PROPERTY( OnEnter,         XSlot )
  EW_PROPERTY( OnHold,          XSlot )
  EW_PROPERTY( OnRelease,       XSlot )
  EW_PROPERTY( OnPress,         XSlot )
  EW_VARIABLE( activeFinger,    XInt32 )
  EW_VARIABLE( Finger,          XInt32 )
  EW_VARIABLE( Time,            XUInt32 )
  EW_VARIABLE( StrikeCount,     XInt32 )
  EW_VARIABLE( HoldPeriod,      XInt32 )
  EW_VARIABLE( Offset,          XPoint )
  EW_VARIABLE( HittingPos,      XPoint )
  EW_VARIABLE( CurrentPos,      XPoint )
  EW_PROPERTY( RetargetDelay,   XInt32 )
  EW_PROPERTY( RetargetOffset,  XInt32 )
  EW_PROPERTY( RetargetCondition, XSet )
  EW_PROPERTY( LimitToFinger,   XInt32 )
  EW_PROPERTY( MaxStrikeCount,  XInt32 )
  EW_PROPERTY( MinStrikeCount,  XInt32 )
  EW_PROPERTY( EnableMultiTouch, XBool )
  EW_VARIABLE( Down,            XBool )
  EW_VARIABLE( Inside,          XBool )
  EW_VARIABLE( AutoDeflected,   XBool )
  EW_VARIABLE( active,          XBool )
EW_END_OF_FIELDS( CoreSimpleTouchHandler )

/* Virtual Method Table (VMT) for the class : 'Core::SimpleTouchHandler' */
EW_DEFINE_METHODS( CoreSimpleTouchHandler, CoreQuadView )
  EW_METHOD( initLayoutContext, void )( CoreQuadView _this, XRect aBounds, CoreOutline 
    aOutline )
  EW_METHOD( GetRoot,           CoreRoot )( CoreView _this )
  EW_METHOD( Draw,              void )( CoreSimpleTouchHandler _this, GraphicsCanvas 
    aCanvas, XRect aClip, XPoint aOffset, XInt32 aOpacity, XBool aBlend )
  EW_METHOD( HandleEvent,       XObject )( CoreSimpleTouchHandler _this, CoreEvent 
    aEvent )
  EW_METHOD( CursorHitTest,     CoreCursorHit )( CoreSimpleTouchHandler _this, XRect 
    aArea, XInt32 aFinger, XInt32 aStrikeCount, CoreView aDedicatedView, XSet aRetargetReason )
  EW_METHOD( ArrangeView,       XPoint )( CoreQuadView _this, XRect aBounds, XEnum 
    aFormation )
  EW_METHOD( MoveView,          void )( CoreQuadView _this, XPoint aOffset, XBool 
    aFastMove )
  EW_METHOD( GetExtent,         XRect )( CoreQuadView _this )
  EW_METHOD( ChangeViewState,   void )( CoreView _this, XSet aSetState, XSet aClearState )
EW_END_OF_METHODS( CoreSimpleTouchHandler )

/* The method Draw() is invoked automatically if parts of the view should be redrawn 
   on the screen. This can occur when e.g. the view has been moved or the appearance 
   of the view has changed before.
   Draw() is invoked automatically by the framework, you never will need to invoke 
   this method directly. However you can request an invocation of this method by 
   calling the method InvalidateArea() of the views @Owner. Usually this is also 
   unnecessary unless you are developing your own view.
   The passed parameters determine the drawing destination aCanvas and the area 
   to redraw aClip in the coordinate space of the canvas. The parameter aOffset 
   contains the displacement between the origin of the views owner and the origin 
   of the canvas. You will need it to convert views coordinates into these of the 
   canvas.
   The parameter aOpacity contains the opacity descended from this view's @Owner. 
   It lies in range 0 .. 255. If the view implements its own 'Opacity', 'Color', 
   etc. properties, the Draw() method should calculate the resulting real opacity 
   by mixing the values of these properties with the one passed in aOpacity parameter.
   The parameter aBlend contains the blending mode descended from this view's @Owner. 
   It determines, whether the view should be drawn with alpha-blending active or 
   not. If aBlend is false, the outputs of the view should overwrite the corresponding 
   pixel in the drawing destination aCanvas. If aBlend is true, the outputs should 
   be mixed with the pixel already stored in aCanvas. For this purpose all Graphics 
   Engine functions provide a parameter to specify the mode for the respective drawing 
   operation. If the view implements its own 'Blend' property, the Draw() method 
   should calculate the resulting real blend mode by using logical AND operation 
   of the value of the property and the one passed in aBlend parameter. */
void CoreSimpleTouchHandler_Draw( CoreSimpleTouchHandler _this, GraphicsCanvas aCanvas, 
  XRect aClip, XPoint aOffset, XInt32 aOpacity, XBool aBlend );

/* The method HandleEvent() is invoked automatically if the view has received an 
   event. For example, touching the view on the touch screen can cause the view 
   to receive a Core::CursorEvent event. Within this method the view can evaluate 
   the event and react to it.
   Whether the event has been handled by the view or not is determined by the return 
   value. To sign an event as handled HandleEvent() should return a valid object 
   (e.g. 'this'). If the event has not been handled, 'null' should be returned.
   Depending on the kind of the event, the framework can continue dispatching of 
   still unhandled events. For example, keyboard events (Core::KeyEvent class) are 
   automatically delivered to the superior @Owner of the receiver view if this view 
   has ignored the event.
   HandleEvent() is invoked automatically by the framework, so you never should 
   need to invoke it directly. However you can prepare and post new events by using 
   the methods DispatchEvent() and BroadcastEvent() of the application class Core::Root. */
XObject CoreSimpleTouchHandler_HandleEvent( CoreSimpleTouchHandler _this, CoreEvent 
  aEvent );

/* The method CursorHitTest() is invoked automatically in order to determine whether 
   the view is interested in the receipt of cursor events or not. This method will 
   be invoked immediately after the user has tapped the visible area of the view. 
   If the view is not interested in the cursor event, the framework repeats this 
   procedure for the next behind view until a willing view has been found or all 
   views are evaluated.
   In the implementation of the method the view can evaluate the passed aArea parameter. 
   It determines the place where the user has tapped the view with his fingertip 
   expressed in the coordinates of the views @Owner. The method can test e.g. whether 
   the tapped area does intersect any touchable areas within the view, etc. The 
   affected finger is identified in the parameter aFinger. The first finger (or 
   the first mouse device button) has the number 0.
   The parameter aStrikeCount determines how many times the same area has been tapped 
   in series. This is useful to detect series of multiple touches, e.g. in case 
   of the double click, aStrikeCount == 2.
   The parameter aDedicatedView, if it is not 'null', restricts the event to be 
   handled by this view only. If aDedicatedView == null, no special restriction 
   exists.
   This method is also invoked if during an existing grab cycle the current target 
   view has decided to resign and deflect the cursor events to another view. This 
   is usually the case after the user has performed a gesture the current target 
   view is not interested to process. Thereupon, the system looks for another view 
   willing to take over the cursor event processing after the performed gesture. 
   Which gesture has caused the operation, is specified in the parameter aRetargetReason.
   If the view is willing to process the event, the method should create, initialize 
   and return a new Core::CursorHit object. This object identify the willing view. 
   Otherwise the method should return 'null'.
   CursorHitTest() is invoked automatically by the framework, so you never should 
   need to invoke it directly. This method is predetermined for the hit-test only. 
   The proper processing of events should take place in the @HandleEvent() method 
   by reacting to Core::CursorEvent and Core::DragEvent events. */
CoreCursorHit CoreSimpleTouchHandler_CursorHitTest( CoreSimpleTouchHandler _this, 
  XRect aArea, XInt32 aFinger, XInt32 aStrikeCount, CoreView aDedicatedView, XSet 
  aRetargetReason );

/* 'C' function for method : 'Core::SimpleTouchHandler.OnSetEnabled()' */
void CoreSimpleTouchHandler_OnSetEnabled( CoreSimpleTouchHandler _this, XBool value );

#ifdef __cplusplus
  }
#endif

#endif /* _CoreSimpleTouchHandler_H */

/* Embedded Wizard */
