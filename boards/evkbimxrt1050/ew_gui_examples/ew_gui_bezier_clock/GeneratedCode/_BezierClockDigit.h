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

#ifndef _BezierClockDigit_H
#define _BezierClockDigit_H

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

#include "_CoreGroup.h"
#include "_EffectsFloatEffect.h"
#include "_GraphicsPath.h"
#include "_ViewsStrokePath.h"

/* Forward declaration of the class BezierClock::Digit */
#ifndef _BezierClockDigit_
  EW_DECLARE_CLASS( BezierClockDigit )
#define _BezierClockDigit_
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


/* Deklaration of class : 'BezierClock::Digit' */
EW_DEFINE_FIELDS( BezierClockDigit, CoreGroup )
  EW_OBJECT  ( FloatEffect,     EffectsFloatEffect )
  EW_OBJECT  ( Path,            GraphicsPath )
  EW_OBJECT  ( StrokePath,      ViewsStrokePath )
  EW_PROPERTY( Value,           XInt32 )
  EW_ARRAY   ( Points,          XPoint, [13])
  EW_PROPERTY( Edges,           XInt32 )
  EW_PROPERTY( Color,           XColor )
  EW_PROPERTY( StrokeWidth,     XFloat )
  EW_ARRAY   ( Points0,         XPoint, [13])
  EW_ARRAY   ( Points1,         XPoint, [13])
  EW_ARRAY   ( Points2,         XPoint, [13])
  EW_ARRAY   ( Points3,         XPoint, [13])
  EW_ARRAY   ( Points4,         XPoint, [13])
  EW_ARRAY   ( Points5,         XPoint, [13])
  EW_ARRAY   ( Points6,         XPoint, [13])
  EW_ARRAY   ( Points7,         XPoint, [13])
  EW_ARRAY   ( Points8,         XPoint, [13])
  EW_ARRAY   ( Points9,         XPoint, [13])
  EW_VARIABLE( oldDigit,        XInt32 )
  EW_PROPERTY( AnimationDuration, XInt32 )
  EW_PROPERTY( Helpers,         XBool )
EW_END_OF_FIELDS( BezierClockDigit )

/* Virtual Method Table (VMT) for the class : 'BezierClock::Digit' */
EW_DEFINE_METHODS( BezierClockDigit, CoreGroup )
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
  EW_METHOD( drawContent,       void )( BezierClockDigit _this, GraphicsCanvas aCanvas, 
    XRect aClip, XPoint aOffset, XInt32 aOpacity, XBool aBlend )
  EW_METHOD( OnSetFocus,        void )( CoreGroup _this, CoreView value )
  EW_METHOD( OnSetBuffered,     void )( CoreGroup _this, XBool value )
  EW_METHOD( OnSetOpacity,      void )( CoreGroup _this, XInt32 value )
  EW_METHOD( IsDialog,          XBool )( CoreGroup _this, XBool aRecursive )
  EW_METHOD( DispatchEvent,     XObject )( CoreGroup _this, CoreEvent aEvent )
  EW_METHOD( BroadcastEvent,    XObject )( CoreGroup _this, CoreEvent aEvent, XSet 
    aFilter )
  EW_METHOD( UpdateLayout,      void )( BezierClockDigit _this, XPoint aSize )
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
EW_END_OF_METHODS( BezierClockDigit )

/* 'C' function for method : 'BezierClock::Digit.drawContent()' */
void BezierClockDigit_drawContent( BezierClockDigit _this, GraphicsCanvas aCanvas, 
  XRect aClip, XPoint aOffset, XInt32 aOpacity, XBool aBlend );

/* The method UpdateLayout() is invoked automatically after the size of the component 
   has been changed. This method can be overridden and filled with logic to perform 
   a sophisticated arrangement calculation for one or more enclosed views. In this 
   case the parameter aSize can be used. It contains the current size of the component. 
   Usually, all enclosed views are arranged automatically accordingly to their @Layout 
   property. UpdateLayout() gives the derived components a chance to extend this 
   automatism by a user defined algorithm. */
void BezierClockDigit_UpdateLayout( BezierClockDigit _this, XPoint aSize );

/* The method Init() is invoked automatically after the component has been created. 
   This method can be overridden and filled with logic containing additional initialization 
   statements. */
void BezierClockDigit_Init( BezierClockDigit _this, XHandle aArg );

/* 'C' function for method : 'BezierClock::Digit.OnSetValue()' */
void BezierClockDigit_OnSetValue( BezierClockDigit _this, XInt32 value );

/* 'C' function for method : 'BezierClock::Digit.OnSetEdges()' */
void BezierClockDigit_OnSetEdges( BezierClockDigit _this, XInt32 value );

/* 'C' function for method : 'BezierClock::Digit.OnSetColor()' */
void BezierClockDigit_OnSetColor( BezierClockDigit _this, XColor value );

/* 'C' function for method : 'BezierClock::Digit.OnSetStrokeWidth()' */
void BezierClockDigit_OnSetStrokeWidth( BezierClockDigit _this, XFloat value );

/* 'C' function for method : 'BezierClock::Digit.drawPoint()' */
void BezierClockDigit_drawPoint( BezierClockDigit _this, GraphicsCanvas aCanvas, 
  XRect aClip, XPoint aOffset, XInt32 aOpacity, XBool aBlend, XPoint aPoint );

/* 'C' function for method : 'BezierClock::Digit.OnSetHelpers()' */
void BezierClockDigit_OnSetHelpers( BezierClockDigit _this, XBool value );

/* 'C' function for method : 'BezierClock::Digit.updatePoints()' */
void BezierClockDigit_updatePoints( BezierClockDigit _this, XObject sender );

/* 'C' function for method : 'BezierClock::Digit.OnSetAnimationDuration()' */
void BezierClockDigit_OnSetAnimationDuration( BezierClockDigit _this, XInt32 value );

/* 'C' function for method : 'BezierClock::Digit.updatePath()' */
void BezierClockDigit_updatePath( BezierClockDigit _this, XObject sender );

#ifdef __cplusplus
  }
#endif

#endif /* _BezierClockDigit_H */

/* Embedded Wizard */
