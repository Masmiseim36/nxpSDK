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

#ifndef _ThermostatThermostat_H
#define _ThermostatThermostat_H

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
#include "_CorePropertyObserver.h"
#include "_CoreTimer.h"
#include "_EffectsInt32Effect.h"
#include "_ThermostatTemperatureController.h"
#include "_ViewsImage.h"
#include "_ViewsText.h"
#include "_ViewsWallpaper.h"
#include "_WidgetSetRadioButton.h"
#include "_WidgetSetToggleButton.h"
#include "_WidgetSetVerticalSlider.h"

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

/* Forward declaration of the class Thermostat::DeviceClass */
#ifndef _ThermostatDeviceClass_
  EW_DECLARE_CLASS( ThermostatDeviceClass )
#define _ThermostatDeviceClass_
#endif

/* Forward declaration of the class Thermostat::Thermostat */
#ifndef _ThermostatThermostat_
  EW_DECLARE_CLASS( ThermostatThermostat )
#define _ThermostatThermostat_
#endif


/* Deklaration of class : 'Thermostat::Thermostat' */
EW_DEFINE_FIELDS( ThermostatThermostat, CoreGroup )
  EW_VARIABLE( Device,          ThermostatDeviceClass )
  EW_OBJECT  ( Background,      ViewsWallpaper )
  EW_OBJECT  ( Overlay,         ViewsWallpaper )
  EW_OBJECT  ( ToggleButton,    WidgetSetToggleButton )
  EW_OBJECT  ( RadioDay,        WidgetSetRadioButton )
  EW_OBJECT  ( RadioNight,      WidgetSetRadioButton )
  EW_OBJECT  ( RadioVacation,   WidgetSetRadioButton )
  EW_OBJECT  ( VerticalSlider,  WidgetSetVerticalSlider )
  EW_OBJECT  ( FanHi,           ViewsImage )
  EW_OBJECT  ( FanHi1,          ViewsImage )
  EW_OBJECT  ( TemperatureController, ThermostatTemperatureController )
  EW_OBJECT  ( Title,           ViewsText )
  EW_OBJECT  ( OnOffButtonText, ViewsText )
  EW_OBJECT  ( ErgosignLogo,    ViewsImage )
  EW_OBJECT  ( FadeEffect,      EffectsInt32Effect )
  EW_OBJECT  ( SettingsObserver, CorePropertyObserver )
  EW_OBJECT  ( AutoDemoTimer,   CoreTimer )
  EW_VARIABLE( AutoState,       XInt32 )
EW_END_OF_FIELDS( ThermostatThermostat )

/* Virtual Method Table (VMT) for the class : 'Thermostat::Thermostat' */
EW_DEFINE_METHODS( ThermostatThermostat, CoreGroup )
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
  EW_METHOD( UpdateLayout,      void )( CoreGroup _this, XPoint aSize )
  EW_METHOD( UpdateViewState,   void )( CoreGroup _this, XSet aState )
  EW_METHOD( InvalidateArea,    void )( CoreGroup _this, XRect aArea )
EW_END_OF_METHODS( ThermostatThermostat )

/* 'C' function for method : 'Thermostat::Thermostat.changeBackFinished()' */
void ThermostatThermostat_changeBackFinished( ThermostatThermostat _this, XObject 
  sender );

/* This slot method is executed when the associated property observer 'PropertyObserver' 
   is notified. */
void ThermostatThermostat_onSettingChanged( ThermostatThermostat _this, XObject 
  sender );

/* Slot method to trigger the next phase of the auto demo. */
void ThermostatThermostat_AutoDemoSlot( ThermostatThermostat _this, XObject sender );

#ifdef __cplusplus
  }
#endif

#endif /* _ThermostatThermostat_H */

/* Embedded Wizard */
