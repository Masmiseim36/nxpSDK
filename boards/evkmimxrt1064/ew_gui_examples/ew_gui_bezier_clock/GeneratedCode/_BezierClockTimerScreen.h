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

#ifndef _BezierClockTimerScreen_H
#define _BezierClockTimerScreen_H

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

#include "_BezierClockDigit.h"
#include "_BezierClockTimerWheel.h"
#include "_CoreGroup.h"
#include "_CoreSimpleTouchHandler.h"
#include "_EffectsInt32Effect.h"
#include "_GraphicsArcPath.h"
#include "_ViewsFillPath.h"
#include "_ViewsImage.h"

/* Forward declaration of the class BezierClock::TimerScreen */
#ifndef _BezierClockTimerScreen_
  EW_DECLARE_CLASS( BezierClockTimerScreen )
#define _BezierClockTimerScreen_
#endif

/* Forward declaration of the class Core::DialogContext */
#ifndef _CoreDialogContext_
  EW_DECLARE_CLASS( CoreDialogContext )
#define _CoreDialogContext_
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


/* Deklaration of class : 'BezierClock::TimerScreen' */
EW_DEFINE_FIELDS( BezierClockTimerScreen, CoreGroup )
  EW_OBJECT  ( Int32Effect,     EffectsInt32Effect )
  EW_OBJECT  ( DotPath,         GraphicsArcPath )
  EW_OBJECT  ( Dot1,            ViewsFillPath )
  EW_OBJECT  ( Dot2,            ViewsFillPath )
  EW_OBJECT  ( Digit0,          BezierClockDigit )
  EW_OBJECT  ( Digit1,          BezierClockDigit )
  EW_OBJECT  ( Digit2,          BezierClockDigit )
  EW_OBJECT  ( Digit3,          BezierClockDigit )
  EW_OBJECT  ( Digit4,          BezierClockDigit )
  EW_OBJECT  ( Digit5,          BezierClockDigit )
  EW_OBJECT  ( RotaryKnob,      BezierClockTimerWheel )
  EW_OBJECT  ( TouchHandlerLeft, CoreSimpleTouchHandler )
  EW_OBJECT  ( ImageLeft,       ViewsImage )
  EW_PROPERTY( TimerCurrent,    XInt32 )
  EW_PROPERTY( Active,          XBool )
EW_END_OF_FIELDS( BezierClockTimerScreen )

/* Virtual Method Table (VMT) for the class : 'BezierClock::TimerScreen' */
EW_DEFINE_METHODS( BezierClockTimerScreen, CoreGroup )
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
  EW_METHOD( UpdateLayout,      void )( CoreGroup _this, XPoint aSize )
  EW_METHOD( UpdateViewState,   void )( BezierClockTimerScreen _this, XSet aState )
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
EW_END_OF_METHODS( BezierClockTimerScreen )

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
void BezierClockTimerScreen_UpdateViewState( BezierClockTimerScreen _this, XSet 
  aState );

/* 'C' function for method : 'BezierClock::TimerScreen.onDismiss()' */
void BezierClockTimerScreen_onDismiss( BezierClockTimerScreen _this, XObject sender );

/* 'C' function for method : 'BezierClock::TimerScreen.OnSetActive()' */
void BezierClockTimerScreen_OnSetActive( BezierClockTimerScreen _this, XBool value );

/* 'C' function for method : 'BezierClock::TimerScreen.OnSetTimerCurrent()' */
void BezierClockTimerScreen_OnSetTimerCurrent( BezierClockTimerScreen _this, XInt32 
  value );

/* Default onget method for the property 'Active' */
XBool BezierClockTimerScreen_OnGetActive( BezierClockTimerScreen _this );

/* Default onget method for the property 'TimerCurrent' */
XInt32 BezierClockTimerScreen_OnGetTimerCurrent( BezierClockTimerScreen _this );

#ifdef __cplusplus
  }
#endif

#endif /* _BezierClockTimerScreen_H */

/* Embedded Wizard */
