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

#ifndef _BezierClockTimerWheel_H
#define _BezierClockTimerWheel_H

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

#include "_CoreRotateTouchHandler.h"
#include "_GraphicsArcPath.h"
#include "_GraphicsPath.h"
#include "_TemplatesRotaryKnob.h"
#include "_ViewsStrokePath.h"

/* Forward declaration of the class BezierClock::TimerWheel */
#ifndef _BezierClockTimerWheel_
  EW_DECLARE_CLASS( BezierClockTimerWheel )
#define _BezierClockTimerWheel_
#endif

/* Forward declaration of the class Core::DialogContext */
#ifndef _CoreDialogContext_
  EW_DECLARE_CLASS( CoreDialogContext )
#define _CoreDialogContext_
#endif

/* Forward declaration of the class Core::Group */
#ifndef _CoreGroup_
  EW_DECLARE_CLASS( CoreGroup )
#define _CoreGroup_
#endif

/* Forward declaration of the class Core::KeyPressHandler */
#ifndef _CoreKeyPressHandler_
  EW_DECLARE_CLASS( CoreKeyPressHandler )
#define _CoreKeyPressHandler_
#endif

/* Forward declaration of the class Core::LayoutContext */
#ifndef _CoreLayoutContext_
  EW_DECLARE_CLASS( CoreLayoutContext )
#define _CoreLayoutContext_
#endif

/* Forward declaration of the class Core::TaskQueue */
#ifndef _CoreTaskQueue_
  EW_DECLARE_CLASS( CoreTaskQueue )
#define _CoreTaskQueue_
#endif

/* Forward declaration of the class Core::View */
#ifndef _CoreView_
  EW_DECLARE_CLASS( CoreView )
#define _CoreView_
#endif

/* Forward declaration of the class Effects::Fader */
#ifndef _EffectsFader_
  EW_DECLARE_CLASS( EffectsFader )
#define _EffectsFader_
#endif

/* Forward declaration of the class Graphics::Canvas */
#ifndef _GraphicsCanvas_
  EW_DECLARE_CLASS( GraphicsCanvas )
#define _GraphicsCanvas_
#endif


/* This class implements a 'rotary knob' widget. The user can touch the knob and 
   rotate it causing the widget's value to be changed accordingly. The current value 
   of the widget is represented by the property @CurrentValue. The valid range for 
   this value is determined by the properties @MinValue and @MaxValue.
   While the user is dragging the knob, the widget sends signals to the slot method 
   stored in the property @OnChange. When the user terminates the interaction, a 
   signal is sent to the slot method stored in @OnApply.
   Alternatively the property @Outlet can refer to any other 'int32' property the 
   widget should remain synchronized with. When the user drags the knob, the affected 
   property is automatically updated to reflect the widget's current value. On the 
   other hand, when the referred property is modified by another one, the widget 
   is automatically notified to remain in sync with the property.
   This approach follows the Controller-View programming paradigm. Here the rotary 
   knob represents the 'View' and the property referred via 'Outlet' can be seen 
   as a part of the 'Controller'. */
EW_DEFINE_FIELDS( BezierClockTimerWheel, TemplatesRotaryKnob )
  EW_PROPERTY( OnApply,         XSlot )
  EW_PROPERTY( OnChange,        XSlot )
  EW_PROPERTY( Outlet,          XRef )
  EW_PROPERTY( OutletActive,    XRef )
  EW_OBJECT  ( WheelBackgroundPath, GraphicsArcPath )
  EW_OBJECT  ( WheelPath0,      GraphicsArcPath )
  EW_OBJECT  ( WheelPath1,      GraphicsArcPath )
  EW_OBJECT  ( WheelPath2,      GraphicsArcPath )
  EW_OBJECT  ( WheelPath3,      GraphicsArcPath )
  EW_OBJECT  ( ButtonPath,      GraphicsPath )
  EW_OBJECT  ( WheelBackground, ViewsStrokePath )
  EW_OBJECT  ( Wheel0,          ViewsStrokePath )
  EW_OBJECT  ( Wheel1,          ViewsStrokePath )
  EW_OBJECT  ( Wheel2,          ViewsStrokePath )
  EW_OBJECT  ( Wheel3,          ViewsStrokePath )
  EW_OBJECT  ( Button,          ViewsStrokePath )
  EW_OBJECT  ( TouchHandler,    CoreRotateTouchHandler )
  EW_VARIABLE( touchStartValue, XInt32 )
  EW_PROPERTY( MaxValue,        XInt32 )
  EW_PROPERTY( MinValue,        XInt32 )
  EW_PROPERTY( CurrentValue,    XInt32 )
  EW_PROPERTY( OutletFactor,    XInt32 )
  EW_PROPERTY( Active,          XBool )
  EW_VARIABLE( buttonTouched,   XBool )
  EW_VARIABLE( enabled,         XBool )
  EW_VARIABLE( selected,        XBool )
  EW_VARIABLE( rotating,        XBool )
EW_END_OF_FIELDS( BezierClockTimerWheel )

/* Virtual Method Table (VMT) for the class : 'BezierClock::TimerWheel' */
EW_DEFINE_METHODS( BezierClockTimerWheel, TemplatesRotaryKnob )
  EW_METHOD( initLayoutContext, void )( CoreRectView _this, XRect aBounds, CoreOutline 
    aOutline )
  EW_METHOD( GetRoot,           CoreRoot )( CoreView _this )
  EW_METHOD( Draw,              void )( CoreGroup _this, GraphicsCanvas aCanvas, 
    XRect aClip, XPoint aOffset, XInt32 aOpacity, XBool aBlend )
  EW_METHOD( HandleEvent,       XObject )( CoreView _this, CoreEvent aEvent )
  EW_METHOD( CursorHitTest,     CoreCursorHit )( CoreGroup _this, XRect aArea, XInt32 
    aFinger, XInt32 aStrikeCount, CoreView aDedicatedView, XSet aRetargetReason )
  EW_METHOD( ArrangeView,       XPoint )( CoreRectView _this, XRect aBounds, XEnum 
    aFormation )
  EW_METHOD( MoveView,          void )( CoreRectView _this, XPoint aOffset, XBool 
    aFastMove )
  EW_METHOD( GetExtent,         XRect )( CoreRectView _this )
  EW_METHOD( ChangeViewState,   void )( CoreGroup _this, XSet aSetState, XSet aClearState )
  EW_METHOD( OnSetBounds,       void )( CoreGroup _this, XRect value )
  EW_METHOD( drawContent,       void )( CoreGroup _this, GraphicsCanvas aCanvas, 
    XRect aClip, XPoint aOffset, XInt32 aOpacity, XBool aBlend )
  EW_METHOD( OnSetFocus,        void )( CoreGroup _this, CoreView value )
  EW_METHOD( OnSetBuffered,     void )( CoreGroup _this, XBool value )
  EW_METHOD( OnSetOpacity,      void )( CoreGroup _this, XInt32 value )
  EW_METHOD( IsDialog,          XBool )( CoreGroup _this, XBool aRecursive )
  EW_METHOD( DispatchEvent,     XObject )( CoreGroup _this, CoreEvent aEvent )
  EW_METHOD( BroadcastEvent,    XObject )( CoreGroup _this, CoreEvent aEvent, XSet 
    aFilter )
  EW_METHOD( UpdateLayout,      void )( BezierClockTimerWheel _this, XPoint aSize )
  EW_METHOD( UpdateViewState,   void )( BezierClockTimerWheel _this, XSet aState )
  EW_METHOD( InvalidateArea,    void )( CoreGroup _this, XRect aArea )
  EW_METHOD( FindSiblingView,   CoreView )( CoreGroup _this, CoreView aView, XSet 
    aFilter )
  EW_METHOD( RestackBehind,     void )( CoreGroup _this, CoreView aView, CoreView 
    aSibling )
  EW_METHOD( RestackTop,        void )( CoreGroup _this, CoreView aView )
  EW_METHOD( Restack,           void )( CoreGroup _this, CoreView aView, XInt32 
    aOrder )
  EW_METHOD( Remove,            void )( CoreGroup _this, CoreView aView )
  EW_METHOD( Add,               void )( CoreGroup _this, CoreView aView, XInt32 
    aOrder )
EW_END_OF_METHODS( BezierClockTimerWheel )

/* The method UpdateLayout() is invoked automatically after the size of the component 
   has been changed. This method can be overridden and filled with logic to perform 
   a sophisticated arrangement calculation for one or more enclosed views. In this 
   case the parameter aSize can be used. It contains the current size of the component. 
   Usually, all enclosed views are arranged automatically accordingly to their @Layout 
   property. UpdateLayout() gives the derived components a chance to extend this 
   automatism by a user defined algorithm. */
void BezierClockTimerWheel_UpdateLayout( BezierClockTimerWheel _this, XPoint aSize );

/* The method UpdateViewState() is invoked automatically after the state of the 
   component has been changed. This method can be overridden and filled with logic 
   to ensure the visual aspect of the component does reflect its current state. 
   For example, the 'enabled' state of the component can affect its colors (disabled 
   components may appear pale). In this case the logic of the method should modify 
   the respective color properties accordingly to the current 'enabled' state. 
   The current state of the component is passed as a set in the parameter aState. 
   It reflects the very basic component state like its visibility or the ability 
   to react to user inputs. Beside this common state, the method can also involve 
   any other variables used in the component as long as they reflect its current 
   state. For example, the toggle switch component can take in account its toggle 
   state 'on' or 'off' and change accordingly the location of the slider, etc.
   Usually, this method will be invoked automatically by the framework. Optionally 
   you can request its invocation by using the method @InvalidateViewState(). */
void BezierClockTimerWheel_UpdateViewState( BezierClockTimerWheel _this, XSet aState );

/* This slot method will receive a signal, if the value of the property assigned 
   to @Outlet has been changed by another widget or by the application logic. In 
   response to this notification, the widget will update itself. */
void BezierClockTimerWheel_onOutlet( BezierClockTimerWheel _this, XObject sender );

/* This internal slot method is called when the user drags the finger while pressing 
   the widget. The method calculates from the made displacement the rotation angle 
   and thus the alternation of the widget's value and updates the widget accordingly. */
void BezierClockTimerWheel_onRotateTouch( BezierClockTimerWheel _this, XObject sender );

/* This internal slot method is called when the user releases the screen after touching 
   and evtl. rotating the knob --> when the interaction ends. */
void BezierClockTimerWheel_onReleaseTouch( BezierClockTimerWheel _this, XObject 
  sender );

/* This internal slot method is called when the user touches the widget's area --> 
   when the rotation interaction begins. */
void BezierClockTimerWheel_onPressTouch( BezierClockTimerWheel _this, XObject sender );

/* 'C' function for method : 'BezierClock::TimerWheel.OnSetOutlet()' */
void BezierClockTimerWheel_OnSetOutlet( BezierClockTimerWheel _this, XRef value );

/* 'C' function for method : 'BezierClock::TimerWheel.OnSetMaxValue()' */
void BezierClockTimerWheel_OnSetMaxValue( BezierClockTimerWheel _this, XInt32 value );

/* 'C' function for method : 'BezierClock::TimerWheel.OnGetCurrentValue()' */
XInt32 BezierClockTimerWheel_OnGetCurrentValue( BezierClockTimerWheel _this );

/* 'C' function for method : 'BezierClock::TimerWheel.OnSetCurrentValue()' */
void BezierClockTimerWheel_OnSetCurrentValue( BezierClockTimerWheel _this, XInt32 
  value );

/* 'C' function for method : 'BezierClock::TimerWheel.OnSetOutletFactor()' */
void BezierClockTimerWheel_OnSetOutletFactor( BezierClockTimerWheel _this, XInt32 
  value );

/* 'C' function for method : 'BezierClock::TimerWheel.updateButton()' */
void BezierClockTimerWheel_updateButton( BezierClockTimerWheel _this, XObject sender );

/* 'C' function for method : 'BezierClock::TimerWheel.OnSetOutletActive()' */
void BezierClockTimerWheel_OnSetOutletActive( BezierClockTimerWheel _this, XRef 
  value );

/* This slot method will receive a signal, if the value of the property assigned 
   to @Outlet has been changed by another widget or by the application logic. In 
   response to this notification, the widget will update itself. */
void BezierClockTimerWheel_onOutletActive( BezierClockTimerWheel _this, XObject 
  sender );

/* 'C' function for method : 'BezierClock::TimerWheel.OnSetActive()' */
void BezierClockTimerWheel_OnSetActive( BezierClockTimerWheel _this, XBool value );

#ifdef __cplusplus
  }
#endif

#endif /* _BezierClockTimerWheel_H */

/* Embedded Wizard */
