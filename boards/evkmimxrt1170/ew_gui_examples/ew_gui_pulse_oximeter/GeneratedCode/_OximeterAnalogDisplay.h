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

#ifndef _OximeterAnalogDisplay_H
#define _OximeterAnalogDisplay_H

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

#include "_CoreGroup.h"
#include "_EffectsFloatEffect.h"
#include "_GraphicsArcPath.h"
#include "_GraphicsPath.h"
#include "_ViewsStrokePath.h"
#include "_ViewsWarpImage.h"

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

/* Forward declaration of the class Graphics::Canvas */
#ifndef _GraphicsCanvas_
  EW_DECLARE_CLASS( GraphicsCanvas )
#define _GraphicsCanvas_
#endif

/* Forward declaration of the class Oximeter::AnalogDisplay */
#ifndef _OximeterAnalogDisplay_
  EW_DECLARE_CLASS( OximeterAnalogDisplay )
#define _OximeterAnalogDisplay_
#endif


/* Deklaration of class : 'Oximeter::AnalogDisplay' */
EW_DEFINE_FIELDS( OximeterAnalogDisplay, CoreGroup )
  EW_PROPERTY( Outlet,          XRef )
  EW_PROPERTY( OutletMin,       XRef )
  EW_PROPERTY( OutletMax,       XRef )
  EW_OBJECT  ( RotateEffect,    EffectsFloatEffect )
  EW_OBJECT  ( BackPath,        GraphicsArcPath )
  EW_OBJECT  ( BackView,        ViewsStrokePath )
  EW_OBJECT  ( IndicatorPath,   GraphicsArcPath )
  EW_OBJECT  ( IndicatorView,   ViewsStrokePath )
  EW_OBJECT  ( Glow,            ViewsWarpImage )
  EW_OBJECT  ( MinMaxPath,      GraphicsPath )
  EW_OBJECT  ( MinMaxView,      ViewsStrokePath )
  EW_PROPERTY( MinRange,        XInt32 )
  EW_PROPERTY( MaxRange,        XInt32 )
  EW_PROPERTY( CurrentValue,    XInt32 )
  EW_PROPERTY( MinValue,        XInt32 )
  EW_PROPERTY( MaxValue,        XInt32 )
  EW_VARIABLE( radius,          XInt32 )
EW_END_OF_FIELDS( OximeterAnalogDisplay )

/* Virtual Method Table (VMT) for the class : 'Oximeter::AnalogDisplay' */
EW_DEFINE_METHODS( OximeterAnalogDisplay, CoreGroup )
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
  EW_METHOD( OnSetFocus,        void )( CoreGroup _this, CoreView value )
  EW_METHOD( OnSetBuffered,     void )( CoreGroup _this, XBool value )
  EW_METHOD( OnSetOpacity,      void )( CoreGroup _this, XInt32 value )
  EW_METHOD( DispatchEvent,     XObject )( CoreGroup _this, CoreEvent aEvent )
  EW_METHOD( BroadcastEvent,    XObject )( CoreGroup _this, CoreEvent aEvent, XSet 
    aFilter )
  EW_METHOD( UpdateLayout,      void )( OximeterAnalogDisplay _this, XPoint aSize )
  EW_METHOD( UpdateViewState,   void )( OximeterAnalogDisplay _this, XSet aState )
  EW_METHOD( InvalidateArea,    void )( CoreGroup _this, XRect aArea )
EW_END_OF_METHODS( OximeterAnalogDisplay )

/* The method UpdateLayout() is invoked automatically after the size of the component 
   has been changed. This method can be overridden and filled with logic to perform 
   a sophisticated arrangement calculation for one or more enclosed views. In this 
   case the parameter aSize can be used. It contains the current size of the component. 
   Usually, all enclosed views are arranged automatically accordingly to their @Layout 
   property. UpdateLayout() gives the derived components a chance to extend this 
   automatism by a user defined algorithm. */
void OximeterAnalogDisplay_UpdateLayout( OximeterAnalogDisplay _this, XPoint aSize );

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
void OximeterAnalogDisplay_UpdateViewState( OximeterAnalogDisplay _this, XSet aState );

/* 'C' function for method : 'Oximeter::AnalogDisplay.OnSetMaxRange()' */
void OximeterAnalogDisplay_OnSetMaxRange( OximeterAnalogDisplay _this, XInt32 value );

/* 'C' function for method : 'Oximeter::AnalogDisplay.OnSetCurrentValue()' */
void OximeterAnalogDisplay_OnSetCurrentValue( OximeterAnalogDisplay _this, XInt32 
  value );

/* This slot method will receive a signal, if the value of the property assigned 
   to @Outlet has been changed by another widget or by the application logic. In 
   response to this notification, the widget will update itself. */
void OximeterAnalogDisplay_outletSlot( OximeterAnalogDisplay _this, XObject sender );

/* 'C' function for method : 'Oximeter::AnalogDisplay.OnSetOutlet()' */
void OximeterAnalogDisplay_OnSetOutlet( OximeterAnalogDisplay _this, XRef value );

/* Slot method to rotate the needle according calculated value from float effect. */
void OximeterAnalogDisplay_rotateSlot( OximeterAnalogDisplay _this, XObject sender );

/* 'C' function for method : 'Oximeter::AnalogDisplay.OnSetMinValue()' */
void OximeterAnalogDisplay_OnSetMinValue( OximeterAnalogDisplay _this, XInt32 value );

/* This slot method will receive a signal, if the value of the property assigned 
   to @OutletMin has been changed by another widget or by the application logic. 
   In response to this notification, the widget will update itself. */
void OximeterAnalogDisplay_outletMinSlot( OximeterAnalogDisplay _this, XObject sender );

/* 'C' function for method : 'Oximeter::AnalogDisplay.OnSetOutletMin()' */
void OximeterAnalogDisplay_OnSetOutletMin( OximeterAnalogDisplay _this, XRef value );

/* 'C' function for method : 'Oximeter::AnalogDisplay.OnSetMaxValue()' */
void OximeterAnalogDisplay_OnSetMaxValue( OximeterAnalogDisplay _this, XInt32 value );

/* This slot method will receive a signal, if the value of the property assigned 
   to @OutletMax has been changed by another widget or by the application logic. 
   In response to this notification, the widget will update itself. */
void OximeterAnalogDisplay_outletMaxSlot( OximeterAnalogDisplay _this, XObject sender );

/* 'C' function for method : 'Oximeter::AnalogDisplay.OnSetOutletMax()' */
void OximeterAnalogDisplay_OnSetOutletMax( OximeterAnalogDisplay _this, XRef value );

/* 'C' function for method : 'Oximeter::AnalogDisplay.updateMinMaxPath()' */
void OximeterAnalogDisplay_updateMinMaxPath( OximeterAnalogDisplay _this );

#ifdef __cplusplus
  }
#endif

#endif /* _OximeterAnalogDisplay_H */

/* Embedded Wizard */
