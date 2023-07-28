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
* Platform : NXP.iMX_RT.RGB565
*
*******************************************************************************/

#ifndef _ThermostatTemperatureController_H
#define _ThermostatTemperatureController_H

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
#include "_CoreRotateTouchHandler.h"
#include "_CoreSimpleTouchHandler.h"
#include "_EffectsInt32Effect.h"
#include "_GraphicsPath.h"
#include "_ViewsImage.h"
#include "_ViewsStrokePath.h"
#include "_ViewsText.h"
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

/* Forward declaration of the class Thermostat::TemperatureController */
#ifndef _ThermostatTemperatureController_
  EW_DECLARE_CLASS( ThermostatTemperatureController )
#define _ThermostatTemperatureController_
#endif


/* Deklaration of class : 'Thermostat::TemperatureController' */
EW_DEFINE_FIELDS( ThermostatTemperatureController, CoreGroup )
  EW_PROPERTY( OutletNominal,   XRef )
  EW_PROPERTY( OutletActual,    XRef )
  EW_OBJECT  ( RotateTouchHandler, CoreRotateTouchHandler )
  EW_OBJECT  ( Wheel,           ViewsImage )
  EW_OBJECT  ( Foreground,      ViewsImage )
  EW_OBJECT  ( Needle,          ViewsWarpImage )
  EW_OBJECT  ( DeltaTempPath,   GraphicsPath )
  EW_OBJECT  ( DeltaTempView,   ViewsStrokePath )
  EW_OBJECT  ( Symbol,          ViewsImage )
  EW_OBJECT  ( ActualText,      ViewsText )
  EW_OBJECT  ( NominalText,     ViewsText )
  EW_OBJECT  ( FadeTextEffect,  EffectsInt32Effect )
  EW_OBJECT  ( SimpleTouchHandlerPlus, CoreSimpleTouchHandler )
  EW_OBJECT  ( SimpleTouchHandlerMinus, CoreSimpleTouchHandler )
  EW_PROPERTY( NominalValue,    XInt32 )
  EW_PROPERTY( ActualValue,     XInt32 )
  EW_PROPERTY( MaxValue,        XInt32 )
  EW_PROPERTY( MinValue,        XInt32 )
  EW_VARIABLE( startValue,      XInt32 )
EW_END_OF_FIELDS( ThermostatTemperatureController )

/* Virtual Method Table (VMT) for the class : 'Thermostat::TemperatureController' */
EW_DEFINE_METHODS( ThermostatTemperatureController, CoreGroup )
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
  EW_METHOD( DispatchEvent,     XObject )( CoreGroup _this, CoreEvent aEvent )
  EW_METHOD( BroadcastEvent,    XObject )( CoreGroup _this, CoreEvent aEvent, XSet 
    aFilter )
  EW_METHOD( UpdateLayout,      void )( ThermostatTemperatureController _this, XPoint 
    aSize )
  EW_METHOD( UpdateViewState,   void )( ThermostatTemperatureController _this, XSet 
    aState )
  EW_METHOD( InvalidateArea,    void )( CoreGroup _this, XRect aArea )
EW_END_OF_METHODS( ThermostatTemperatureController )

/* The method UpdateLayout() is invoked automatically after the size of the component 
   has been changed. This method can be overridden and filled with logic to perform 
   a sophisticated arrangement calculation for one or more enclosed views. In this 
   case the parameter aSize can be used. It contains the current size of the component. 
   Usually, all enclosed views are arranged automatically accordingly to their @Layout 
   property. UpdateLayout() gives the derived components a chance to extend this 
   automatism by a user defined algorithm. */
void ThermostatTemperatureController_UpdateLayout( ThermostatTemperatureController _this, 
  XPoint aSize );

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
void ThermostatTemperatureController_UpdateViewState( ThermostatTemperatureController _this, 
  XSet aState );

/* 'C' function for method : 'Thermostat::TemperatureController.OnSetNominalValue()' */
void ThermostatTemperatureController_OnSetNominalValue( ThermostatTemperatureController _this, 
  XInt32 value );

/* 'C' function for method : 'Thermostat::TemperatureController.OnSetActualValue()' */
void ThermostatTemperatureController_OnSetActualValue( ThermostatTemperatureController _this, 
  XInt32 value );

/* This slot method will receive a signal, if the value of the property assigned 
   to @Outlet has been changed by another widget or by the application logic. In 
   response to this notification, the widget will update itself. */
void ThermostatTemperatureController_outletNominalSlot( ThermostatTemperatureController _this, 
  XObject sender );

/* 'C' function for method : 'Thermostat::TemperatureController.OnSetOutletNominal()' */
void ThermostatTemperatureController_OnSetOutletNominal( ThermostatTemperatureController _this, 
  XRef value );

/* This slot method will receive a signal, if the value of the property assigned 
   to @Outlet has been changed by another widget or by the application logic. In 
   response to this notification, the widget will update itself. */
void ThermostatTemperatureController_outletCurrentSlot( ThermostatTemperatureController _this, 
  XObject sender );

/* 'C' function for method : 'Thermostat::TemperatureController.OnSetOutletActual()' */
void ThermostatTemperatureController_OnSetOutletActual( ThermostatTemperatureController _this, 
  XRef value );

/* 'C' function for method : 'Thermostat::TemperatureController.onStartSlot()' */
void ThermostatTemperatureController_onStartSlot( ThermostatTemperatureController _this, 
  XObject sender );

/* 'C' function for method : 'Thermostat::TemperatureController.onRotateSlot()' */
void ThermostatTemperatureController_onRotateSlot( ThermostatTemperatureController _this, 
  XObject sender );

/* 'C' function for method : 'Thermostat::TemperatureController.onEndSlot()' */
void ThermostatTemperatureController_onEndSlot( ThermostatTemperatureController _this, 
  XObject sender );

/* 'C' function for method : 'Thermostat::TemperatureController.fadeText()' */
void ThermostatTemperatureController_fadeText( ThermostatTemperatureController _this, 
  XObject sender );

/* 'C' function for method : 'Thermostat::TemperatureController.incrNominal()' */
void ThermostatTemperatureController_incrNominal( ThermostatTemperatureController _this, 
  XObject sender );

/* 'C' function for method : 'Thermostat::TemperatureController.decrNominal()' */
void ThermostatTemperatureController_decrNominal( ThermostatTemperatureController _this, 
  XObject sender );

#ifdef __cplusplus
  }
#endif

#endif /* _ThermostatTemperatureController_H */

/* Embedded Wizard */
