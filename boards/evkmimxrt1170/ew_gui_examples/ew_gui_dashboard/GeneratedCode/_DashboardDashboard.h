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

#ifndef _DashboardDashboard_H
#define _DashboardDashboard_H

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
#include "_CoreTimer.h"
#include "_DashboardBattery.h"
#include "_DashboardClock.h"
#include "_DashboardDashboardV1.h"
#include "_DashboardDashboardV2.h"
#include "_DashboardGaugeDisplay.h"
#include "_DashboardNavigation.h"
#include "_DashboardNavigationHints.h"
#include "_EffectsFloatEffect.h"
#include "_EffectsInt32Effect.h"
#include "_ViewsRectangle.h"
#include "_ViewsText.h"
#include "_ViewsWarpGroup.h"
#include "_WidgetSetPushButton.h"
#include "_WidgetSetToggleButton.h"
#include "_WidgetSetValueDisplay.h"

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

/* Forward declaration of the class Dashboard::Dashboard */
#ifndef _DashboardDashboard_
  EW_DECLARE_CLASS( DashboardDashboard )
#define _DashboardDashboard_
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


/* Deklaration of class : 'Dashboard::Dashboard' */
EW_DEFINE_FIELDS( DashboardDashboard, CoreGroup )
  EW_OBJECT  ( v1Effect,        EffectsFloatEffect )
  EW_OBJECT  ( ledOnEffect,     EffectsInt32Effect )
  EW_OBJECT  ( transitionEffect, EffectsFloatEffect )
  EW_OBJECT  ( navigationEffect, EffectsInt32Effect )
  EW_OBJECT  ( shutdownVariantEffect, EffectsFloatEffect )
  EW_OBJECT  ( shutdownNavigationEffect, EffectsInt32Effect )
  EW_OBJECT  ( gaugeDisplayEffect, EffectsFloatEffect )
  EW_OBJECT  ( shutdownGaugeDisplayEffect, EffectsFloatEffect )
  EW_OBJECT  ( gaugeCheckEffect, EffectsInt32Effect )
  EW_OBJECT  ( ledOffEffect,    EffectsInt32Effect )
  EW_OBJECT  ( Background,      ViewsRectangle )
  EW_OBJECT  ( Navigation,      DashboardNavigation )
  EW_OBJECT  ( ShutdownButton,  WidgetSetPushButton )
  EW_OBJECT  ( TransitionButton, WidgetSetPushButton )
  EW_OBJECT  ( DashboardV2,     DashboardDashboardV2 )
  EW_OBJECT  ( DashboardV1,     DashboardDashboardV1 )
  EW_OBJECT  ( NavigationHints, DashboardNavigationHints )
  EW_OBJECT  ( Clock,           DashboardClock )
  EW_OBJECT  ( Munich,          ViewsText )
  EW_OBJECT  ( Reach,           ViewsText )
  EW_OBJECT  ( Battery,         DashboardBattery )
  EW_OBJECT  ( EnergyDisplay,   DashboardGaugeDisplay )
  EW_OBJECT  ( EnergyWarpGroup, ViewsWarpGroup )
  EW_OBJECT  ( SpeedDisplay,    DashboardGaugeDisplay )
  EW_OBJECT  ( SpeedWarpGroup,  ViewsWarpGroup )
  EW_OBJECT  ( LedButton,       WidgetSetToggleButton )
  EW_OBJECT  ( LedButton1,      WidgetSetToggleButton )
  EW_OBJECT  ( LedButton2,      WidgetSetToggleButton )
  EW_OBJECT  ( LedButton3,      WidgetSetToggleButton )
  EW_OBJECT  ( LedButton4,      WidgetSetToggleButton )
  EW_OBJECT  ( LedButton5,      WidgetSetToggleButton )
  EW_OBJECT  ( RangeDisplay,    WidgetSetValueDisplay )
  EW_OBJECT  ( autoDemoModeTimer, CoreTimer )
  EW_PROPERTY( progress,        XFloat )
  EW_PROPERTY( progressReverse, XFloat )
  EW_VARIABLE( shutdown,        XBool )
EW_END_OF_FIELDS( DashboardDashboard )

/* Virtual Method Table (VMT) for the class : 'Dashboard::Dashboard' */
EW_DEFINE_METHODS( DashboardDashboard, CoreGroup )
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
  EW_METHOD( IsDialog,          XBool )( CoreGroup _this, XBool aRecursive )
  EW_METHOD( DispatchEvent,     XObject )( CoreGroup _this, CoreEvent aEvent )
  EW_METHOD( BroadcastEvent,    XObject )( CoreGroup _this, CoreEvent aEvent, XSet 
    aFilter )
  EW_METHOD( UpdateViewState,   void )( DashboardDashboard _this, XSet aState )
  EW_METHOD( InvalidateArea,    void )( CoreGroup _this, XRect aArea )
EW_END_OF_METHODS( DashboardDashboard )

/* The method Init() is invoked automatically after the component has been created. 
   This method can be overridden and filled with logic containing additional initialization 
   statements. */
void DashboardDashboard_Init( DashboardDashboard _this, XHandle aArg );

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
void DashboardDashboard_UpdateViewState( DashboardDashboard _this, XSet aState );

/* 'C' function for method : 'Dashboard::Dashboard.onV1Effect()' */
void DashboardDashboard_onV1Effect( DashboardDashboard _this, XObject sender );

/* 'C' function for method : 'Dashboard::Dashboard.onGaugeDisplayEffect()' */
void DashboardDashboard_onGaugeDisplayEffect( DashboardDashboard _this, XObject 
  sender );

/* 'C' function for method : 'Dashboard::Dashboard.onGaugeDisplayFinished()' */
void DashboardDashboard_onGaugeDisplayFinished( DashboardDashboard _this, XObject 
  sender );

/* 'C' function for method : 'Dashboard::Dashboard.onLedOnEffect()' */
void DashboardDashboard_onLedOnEffect( DashboardDashboard _this, XObject sender );

/* 'C' function for method : 'Dashboard::Dashboard.onLedOnEffectFinished()' */
void DashboardDashboard_onLedOnEffectFinished( DashboardDashboard _this, XObject 
  sender );

/* 'C' function for method : 'Dashboard::Dashboard.onStartTransition()' */
void DashboardDashboard_onStartTransition( DashboardDashboard _this, XObject sender );

/* 'C' function for method : 'Dashboard::Dashboard.onNavigationFinished()' */
void DashboardDashboard_onNavigationFinished( DashboardDashboard _this, XObject 
  sender );

/* 'C' function for method : 'Dashboard::Dashboard.OnSetprogress()' */
void DashboardDashboard_OnSetprogress( DashboardDashboard _this, XFloat value );

/* 'C' function for method : 'Dashboard::Dashboard.OnSetprogressReverse()' */
void DashboardDashboard_OnSetprogressReverse( DashboardDashboard _this, XFloat value );

/* 'C' function for method : 'Dashboard::Dashboard.onNavigationEffect()' */
void DashboardDashboard_onNavigationEffect( DashboardDashboard _this, XObject sender );

/* 'C' function for method : 'Dashboard::Dashboard.onShutdown()' */
void DashboardDashboard_onShutdown( DashboardDashboard _this, XObject sender );

/* 'C' function for method : 'Dashboard::Dashboard.onShutdownNavigationFinished()' */
void DashboardDashboard_onShutdownNavigationFinished( DashboardDashboard _this, 
  XObject sender );

/* 'C' function for method : 'Dashboard::Dashboard.onShutdownNavigationEffect()' */
void DashboardDashboard_onShutdownNavigationEffect( DashboardDashboard _this, XObject 
  sender );

/* 'C' function for method : 'Dashboard::Dashboard.onShutdownGaugeDisplayEffect()' */
void DashboardDashboard_onShutdownGaugeDisplayEffect( DashboardDashboard _this, 
  XObject sender );

/* 'C' function for method : 'Dashboard::Dashboard.onShutdownGaugeDisplayFinished()' */
void DashboardDashboard_onShutdownGaugeDisplayFinished( DashboardDashboard _this, 
  XObject sender );

/* 'C' function for method : 'Dashboard::Dashboard.onGaugeCheckEffect()' */
void DashboardDashboard_onGaugeCheckEffect( DashboardDashboard _this, XObject sender );

/* 'C' function for method : 'Dashboard::Dashboard.onGaugeCheckEffectFinished()' */
void DashboardDashboard_onGaugeCheckEffectFinished( DashboardDashboard _this, XObject 
  sender );

/* 'C' function for method : 'Dashboard::Dashboard.onLedOffEffect()' */
void DashboardDashboard_onLedOffEffect( DashboardDashboard _this, XObject sender );

/* 'C' function for method : 'Dashboard::Dashboard.onAutoDemoModeChanged()' */
void DashboardDashboard_onAutoDemoModeChanged( DashboardDashboard _this, XObject 
  sender );

/* Default onget method for the property 'progress' */
XFloat DashboardDashboard_OnGetprogress( DashboardDashboard _this );

/* Default onget method for the property 'progressReverse' */
XFloat DashboardDashboard_OnGetprogressReverse( DashboardDashboard _this );

#ifdef __cplusplus
  }
#endif

#endif /* _DashboardDashboard_H */

/* Embedded Wizard */
