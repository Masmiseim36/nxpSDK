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

#ifndef _CoreSlideTouchHandler_H
#define _CoreSlideTouchHandler_H

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

#include "_CoreRectView.h"

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

/* Forward declaration of the class Core::SlideTouchHandler */
#ifndef _CoreSlideTouchHandler_
  EW_DECLARE_CLASS( CoreSlideTouchHandler )
#define _CoreSlideTouchHandler_
#endif

/* Forward declaration of the class Core::Timer */
#ifndef _CoreTimer_
  EW_DECLARE_CLASS( CoreTimer )
#define _CoreTimer_
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


/* The class Core::SlideTouchHandler provides a special kind of a view able to react 
   to touch screen events and to process slide gestures. Each time the user taps 
   inside the area of the handler and drags the finger, the handler modifies the 
   stored @Offset value accordingly to the direction and distance the user has stroke. 
   Each modification of this @Offset value causes the handler to send a signal to 
   a slot method stored in the property @OnSlide. The intention of this handler 
   is to cover the internal aspects of the cursor event handling.
   This slide handler is very useful to add the scrolling functionality to GUI components 
   containing large lists of items, text blocks, or other extensive contents. Even 
   the classes Core::Outline, Core::HorizontalList, Core::VerticalList, Views::Text 
   and Views::Image provide an interface to connect themselves with a such slide 
   handler. In this manner the user can scroll their content by simply touching 
   inside the handler area and dragging the finger.
   The slide handler implements fancy animation effects to simulate movements similar 
   to them in a real physical system with its inertia and the friction. For example, 
   after the user has finished the interaction the sliding can continue running 
   and its speed will decelerate accordingly to the @Friction factor. 
   The handler provides several variables useful to query e.g. the current handler 
   state (@Sliding), or the current slide position (@Offset), etc. These variables 
   can be simply evaluated in the implementation of the slot method.
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
   all handler appear as semitransparent rectangles. This allows you to interact 
   with the handlers during the design time. */
EW_DEFINE_FIELDS( CoreSlideTouchHandler, CoreRectView )
  EW_VARIABLE( timer,           CoreTimer )
  EW_VARIABLE( privateOnStart,  XSlot )
  EW_VARIABLE( privateOnEnd,    XSlot )
  EW_VARIABLE( privateOnSlide,  XSlot )
  EW_PROPERTY( OnSlide,         XSlot )
  EW_PROPERTY( OnEnd,           XSlot )
  EW_PROPERTY( OnStart,         XSlot )
  EW_VARIABLE( activeFinger,    XInt32 )
  EW_VARIABLE( frictFactor,     XFloat )
  EW_VARIABLE( refTime,         XUInt32 )
  EW_VARIABLE( initOffset,      XPoint )
  EW_VARIABLE( endY,            XInt32 )
  EW_VARIABLE( accelerationY,   XFloat )
  EW_VARIABLE( speedY,          XFloat )
  EW_VARIABLE( startY,          XFloat )
  EW_VARIABLE( startTimeY,      XUInt32 )
  EW_VARIABLE( endX,            XInt32 )
  EW_VARIABLE( accelerationX,   XFloat )
  EW_VARIABLE( speedX,          XFloat )
  EW_VARIABLE( startX,          XFloat )
  EW_VARIABLE( startTimeX,      XUInt32 )
  EW_VARIABLE( HittingPos,      XPoint )
  EW_VARIABLE( Delta,           XPoint )
  EW_PROPERTY( LimitToFinger,   XInt32 )
  EW_PROPERTY( MaxOffset,       XPoint )
  EW_PROPERTY( MinOffset,       XPoint )
  EW_PROPERTY( Offset,          XPoint )
  EW_PROPERTY( SnapLast,        XPoint )
  EW_PROPERTY( SnapNext,        XPoint )
  EW_PROPERTY( SnapFirst,       XPoint )
  EW_PROPERTY( RubberBandEffectElasticity, XFloat )
  EW_PROPERTY( RubberBandEffectDuration, XInt32 )
  EW_PROPERTY( RetargetDelay,   XInt32 )
  EW_PROPERTY( RetargetOffset,  XInt32 )
  EW_PROPERTY( RetargetCondition, XSet )
  EW_PROPERTY( ResetSpace,      XInt32 )
  EW_PROPERTY( ResetDelay,      XInt32 )
  EW_PROPERTY( SpeedLimit,      XFloat )
  EW_PROPERTY( Friction,        XFloat )
  EW_PROPERTY( RubberBandScrolling, XBool )
  EW_PROPERTY( SlideVert,       XBool )
  EW_PROPERTY( SlideHorz,       XBool )
  EW_VARIABLE( Sliding,         XBool )
  EW_VARIABLE( parkingX,        XBool )
  EW_VARIABLE( parkingY,        XBool )
  EW_VARIABLE( active,          XBool )
EW_END_OF_FIELDS( CoreSlideTouchHandler )

/* Virtual Method Table (VMT) for the class : 'Core::SlideTouchHandler' */
EW_DEFINE_METHODS( CoreSlideTouchHandler, CoreRectView )
  EW_METHOD( initLayoutContext, void )( CoreRectView _this, XRect aBounds, CoreOutline 
    aOutline )
  EW_METHOD( GetRoot,           CoreRoot )( CoreView _this )
  EW_METHOD( Draw,              void )( CoreSlideTouchHandler _this, GraphicsCanvas 
    aCanvas, XRect aClip, XPoint aOffset, XInt32 aOpacity, XBool aBlend )
  EW_METHOD( HandleEvent,       XObject )( CoreSlideTouchHandler _this, CoreEvent 
    aEvent )
  EW_METHOD( CursorHitTest,     CoreCursorHit )( CoreSlideTouchHandler _this, XRect 
    aArea, XInt32 aFinger, XInt32 aStrikeCount, CoreView aDedicatedView, XSet aRetargetReason )
  EW_METHOD( ArrangeView,       XPoint )( CoreRectView _this, XRect aBounds, XEnum 
    aFormation )
  EW_METHOD( MoveView,          void )( CoreRectView _this, XPoint aOffset, XBool 
    aFastMove )
  EW_METHOD( GetExtent,         XRect )( CoreRectView _this )
  EW_METHOD( ChangeViewState,   void )( CoreView _this, XSet aSetState, XSet aClearState )
  EW_METHOD( OnSetBounds,       void )( CoreRectView _this, XRect value )
EW_END_OF_METHODS( CoreSlideTouchHandler )

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
void CoreSlideTouchHandler_Draw( CoreSlideTouchHandler _this, GraphicsCanvas aCanvas, 
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
XObject CoreSlideTouchHandler_HandleEvent( CoreSlideTouchHandler _this, CoreEvent 
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
CoreCursorHit CoreSlideTouchHandler_CursorHitTest( CoreSlideTouchHandler _this, 
  XRect aArea, XInt32 aFinger, XInt32 aStrikeCount, CoreView aDedicatedView, XSet 
  aRetargetReason );

/* 'C' function for method : 'Core::SlideTouchHandler.stopAnimation()' */
void CoreSlideTouchHandler_stopAnimation( CoreSlideTouchHandler _this );

/* 'C' function for method : 'Core::SlideTouchHandler.startAnimation()' */
void CoreSlideTouchHandler_startAnimation( CoreSlideTouchHandler _this );

/* 'C' function for method : 'Core::SlideTouchHandler.timerSlot()' */
void CoreSlideTouchHandler_timerSlot( CoreSlideTouchHandler _this, XObject sender );

/* 'C' function for method : 'Core::SlideTouchHandler.OnSetSnapNext()' */
void CoreSlideTouchHandler_OnSetSnapNext( CoreSlideTouchHandler _this, XPoint value );

/* 'C' function for method : 'Core::SlideTouchHandler.OnSetFriction()' */
void CoreSlideTouchHandler_OnSetFriction( CoreSlideTouchHandler _this, XFloat value );

#ifdef __cplusplus
  }
#endif

#endif /* _CoreSlideTouchHandler_H */

/* Embedded Wizard */
