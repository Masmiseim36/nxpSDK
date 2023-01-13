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

#ifndef _DashboardGaugeDisplay_H
#define _DashboardGaugeDisplay_H

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
#include "_ViewsLine.h"
#include "_ViewsText.h"

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

/* Forward declaration of the class Dashboard::GaugeDisplay */
#ifndef _DashboardGaugeDisplay_
  EW_DECLARE_CLASS( DashboardGaugeDisplay )
#define _DashboardGaugeDisplay_
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


/* Deklaration of class : 'Dashboard::GaugeDisplay' */
EW_DEFINE_FIELDS( DashboardGaugeDisplay, CoreGroup )
  EW_PROPERTY( Outlet,          XRef )
  EW_PROPERTY( OutletDesc,      XRef )
  EW_OBJECT  ( DescText,        ViewsText )
  EW_OBJECT  ( Line,            ViewsLine )
  EW_OBJECT  ( UnitText,        ViewsText )
  EW_OBJECT  ( ValueText,       ViewsText )
  EW_PROPERTY( Unit,            XString )
  EW_PROPERTY( Desc,            XString )
  EW_PROPERTY( DescUnit,        XString )
EW_END_OF_FIELDS( DashboardGaugeDisplay )

/* Virtual Method Table (VMT) for the class : 'Dashboard::GaugeDisplay' */
EW_DEFINE_METHODS( DashboardGaugeDisplay, CoreGroup )
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
EW_END_OF_METHODS( DashboardGaugeDisplay )

/* 'C' function for method : 'Dashboard::GaugeDisplay.OnSetOutlet()' */
void DashboardGaugeDisplay_OnSetOutlet( DashboardGaugeDisplay _this, XRef value );

/* 'C' function for method : 'Dashboard::GaugeDisplay.onOutlet()' */
void DashboardGaugeDisplay_onOutlet( DashboardGaugeDisplay _this, XObject sender );

/* 'C' function for method : 'Dashboard::GaugeDisplay.OnSetUnit()' */
void DashboardGaugeDisplay_OnSetUnit( DashboardGaugeDisplay _this, XString value );

/* 'C' function for method : 'Dashboard::GaugeDisplay.OnSetDesc()' */
void DashboardGaugeDisplay_OnSetDesc( DashboardGaugeDisplay _this, XString value );

/* 'C' function for method : 'Dashboard::GaugeDisplay.OnSetOutletDesc()' */
void DashboardGaugeDisplay_OnSetOutletDesc( DashboardGaugeDisplay _this, XRef value );

/* 'C' function for method : 'Dashboard::GaugeDisplay.onOutletDesc()' */
void DashboardGaugeDisplay_onOutletDesc( DashboardGaugeDisplay _this, XObject sender );

/* 'C' function for method : 'Dashboard::GaugeDisplay.OnSetDescUnit()' */
void DashboardGaugeDisplay_OnSetDescUnit( DashboardGaugeDisplay _this, XString value );

#ifdef __cplusplus
  }
#endif

#endif /* _DashboardGaugeDisplay_H */

/* Embedded Wizard */
