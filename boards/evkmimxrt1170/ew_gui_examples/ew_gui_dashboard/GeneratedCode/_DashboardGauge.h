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

#ifndef _DashboardGauge_H
#define _DashboardGauge_H

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
#include "_DashboardGaugeIndicator.h"
#include "_DashboardGaugeLabels.h"
#include "_GraphicsArcPath.h"
#include "_ViewsFillPath.h"
#include "_ViewsImage.h"

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

/* Forward declaration of the class Dashboard::Gauge */
#ifndef _DashboardGauge_
  EW_DECLARE_CLASS( DashboardGauge )
#define _DashboardGauge_
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


/* Deklaration of class : 'Dashboard::Gauge' */
EW_DEFINE_FIELDS( DashboardGauge, CoreGroup )
  EW_PROPERTY( Outlet,          XRef )
  EW_OBJECT  ( background,      GraphicsArcPath )
  EW_OBJECT  ( Bg,              ViewsFillPath )
  EW_OBJECT  ( Circle5,         ViewsImage )
  EW_OBJECT  ( Circle4,         ViewsImage )
  EW_OBJECT  ( Circle3,         ViewsImage )
  EW_OBJECT  ( Circle2,         ViewsImage )
  EW_OBJECT  ( Circle1,         ViewsImage )
  EW_OBJECT  ( Circle0,         ViewsImage )
  EW_OBJECT  ( GaugeLabels,     DashboardGaugeLabels )
  EW_OBJECT  ( GaugeIndicator,  DashboardGaugeIndicator )
  EW_PROPERTY( OpacityGauge,    XUInt8 )
  EW_PROPERTY( OpacityLabels,   XUInt8 )
  EW_PROPERTY( Clockwise,       XBool )
EW_END_OF_FIELDS( DashboardGauge )

/* Virtual Method Table (VMT) for the class : 'Dashboard::Gauge' */
EW_DEFINE_METHODS( DashboardGauge, CoreGroup )
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
  EW_METHOD( UpdateViewState,   void )( CoreGroup _this, XSet aState )
  EW_METHOD( InvalidateArea,    void )( CoreGroup _this, XRect aArea )
EW_END_OF_METHODS( DashboardGauge )

/* 'C' function for method : 'Dashboard::Gauge.OnSetOutlet()' */
void DashboardGauge_OnSetOutlet( DashboardGauge _this, XRef value );

/* 'C' function for method : 'Dashboard::Gauge.OnSetClockwise()' */
void DashboardGauge_OnSetClockwise( DashboardGauge _this, XBool value );

/* 'C' function for method : 'Dashboard::Gauge.OnSetOpacityLabels()' */
void DashboardGauge_OnSetOpacityLabels( DashboardGauge _this, XUInt8 value );

/* 'C' function for method : 'Dashboard::Gauge.OnSetOpacityGauge()' */
void DashboardGauge_OnSetOpacityGauge( DashboardGauge _this, XUInt8 value );

#ifdef __cplusplus
  }
#endif

#endif /* _DashboardGauge_H */

/* Embedded Wizard */
