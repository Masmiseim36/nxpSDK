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

#ifndef _BezierClockClockScreen_H
#define _BezierClockClockScreen_H

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
#include "_CoreGroup.h"
#include "_CoreSimpleTouchHandler.h"
#include "_CoreTime.h"
#include "_CoreTimer.h"
#include "_GraphicsArcPath.h"
#include "_ViewsFillPath.h"
#include "_ViewsImage.h"
#include "_ViewsText.h"

/* Forward declaration of the class BezierClock::ClockScreen */
#ifndef _BezierClockClockScreen_
  EW_DECLARE_CLASS( BezierClockClockScreen )
#define _BezierClockClockScreen_
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


/* Deklaration of class : 'BezierClock::ClockScreen' */
EW_DEFINE_FIELDS( BezierClockClockScreen, CoreGroup )
  EW_OBJECT  ( Timer,           CoreTimer )
  EW_OBJECT  ( Time,            CoreTime )
  EW_OBJECT  ( DotPath,         GraphicsArcPath )
  EW_OBJECT  ( TouchHandlerDown, CoreSimpleTouchHandler )
  EW_OBJECT  ( TouchHandlerRight, CoreSimpleTouchHandler )
  EW_OBJECT  ( TouchHandlerUp,  CoreSimpleTouchHandler )
  EW_OBJECT  ( Date,            ViewsText )
  EW_OBJECT  ( Dot1,            ViewsFillPath )
  EW_OBJECT  ( Dot2,            ViewsFillPath )
  EW_OBJECT  ( Digit0,          BezierClockDigit )
  EW_OBJECT  ( Digit1,          BezierClockDigit )
  EW_OBJECT  ( Digit2,          BezierClockDigit )
  EW_OBJECT  ( Digit3,          BezierClockDigit )
  EW_OBJECT  ( Digit4,          BezierClockDigit )
  EW_OBJECT  ( Digit5,          BezierClockDigit )
  EW_OBJECT  ( ImageUp,         ViewsImage )
  EW_OBJECT  ( ImageDown,       ViewsImage )
  EW_OBJECT  ( ImageRight,      ViewsImage )
EW_END_OF_FIELDS( BezierClockClockScreen )

/* Virtual Method Table (VMT) for the class : 'BezierClock::ClockScreen' */
EW_DEFINE_METHODS( BezierClockClockScreen, CoreGroup )
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
  EW_METHOD( UpdateViewState,   void )( CoreGroup _this, XSet aState )
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
EW_END_OF_METHODS( BezierClockClockScreen )

/* The method Init() is invoked automatically after the component has been created. 
   This method can be overridden and filled with logic containing additional initialization 
   statements. */
void BezierClockClockScreen_Init( BezierClockClockScreen _this, XHandle aArg );

/* 'C' function for method : 'BezierClock::ClockScreen.onShowTimer()' */
void BezierClockClockScreen_onShowTimer( BezierClockClockScreen _this, XObject sender );

/* 'C' function for method : 'BezierClock::ClockScreen.onShowSettings()' */
void BezierClockClockScreen_onShowSettings( BezierClockClockScreen _this, XObject 
  sender );

/* 'C' function for method : 'BezierClock::ClockScreen.onShowStyle()' */
void BezierClockClockScreen_onShowStyle( BezierClockClockScreen _this, XObject sender );

/* 'C' function for method : 'BezierClock::ClockScreen.updateCurrentTime()' */
void BezierClockClockScreen_updateCurrentTime( BezierClockClockScreen _this, XObject 
  sender );

/* 'C' function for method : 'BezierClock::ClockScreen.frequentUpdate()' */
void BezierClockClockScreen_frequentUpdate( BezierClockClockScreen _this, XObject 
  sender );

/* 'C' function for method : 'BezierClock::ClockScreen.firstUpdate()' */
void BezierClockClockScreen_firstUpdate( BezierClockClockScreen _this, XObject sender );

#ifdef __cplusplus
  }
#endif

#endif /* _BezierClockClockScreen_H */

/* Embedded Wizard */
