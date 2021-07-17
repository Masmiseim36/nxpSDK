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

#ifndef _OximeterOximeter_H
#define _OximeterOximeter_H

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
#include "_EffectsInt32Effect.h"
#include "_OximeterBackground.h"
#include "_OximeterOxygenPanel.h"
#include "_OximeterPulsePanel.h"
#include "_ViewsImage.h"
#include "_WidgetSetPushButton.h"

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

/* Forward declaration of the class Oximeter::DeviceClass */
#ifndef _OximeterDeviceClass_
  EW_DECLARE_CLASS( OximeterDeviceClass )
#define _OximeterDeviceClass_
#endif

/* Forward declaration of the class Oximeter::GraphPanel */
#ifndef _OximeterGraphPanel_
  EW_DECLARE_CLASS( OximeterGraphPanel )
#define _OximeterGraphPanel_
#endif

/* Forward declaration of the class Oximeter::MainMenu */
#ifndef _OximeterMainMenu_
  EW_DECLARE_CLASS( OximeterMainMenu )
#define _OximeterMainMenu_
#endif

/* Forward declaration of the class Oximeter::Oximeter */
#ifndef _OximeterOximeter_
  EW_DECLARE_CLASS( OximeterOximeter )
#define _OximeterOximeter_
#endif

/* Forward declaration of the class Oximeter::OxygenSettings */
#ifndef _OximeterOxygenSettings_
  EW_DECLARE_CLASS( OximeterOxygenSettings )
#define _OximeterOxygenSettings_
#endif

/* Forward declaration of the class Oximeter::PulseSettings */
#ifndef _OximeterPulseSettings_
  EW_DECLARE_CLASS( OximeterPulseSettings )
#define _OximeterPulseSettings_
#endif


/* Deklaration of class : 'Oximeter::Oximeter' */
EW_DEFINE_FIELDS( OximeterOximeter, CoreGroup )
  EW_VARIABLE( Device,          OximeterDeviceClass )
  EW_VARIABLE( graphPanel,      OximeterGraphPanel )
  EW_VARIABLE( pulseSettings,   OximeterPulseSettings )
  EW_VARIABLE( oxygenSettings,  OximeterOxygenSettings )
  EW_VARIABLE( mainMenu,        OximeterMainMenu )
  EW_OBJECT  ( Background,      OximeterBackground )
  EW_OBJECT  ( PulsePanel,      OximeterPulsePanel )
  EW_OBJECT  ( OxygenPanel,     OximeterOxygenPanel )
  EW_OBJECT  ( PushButton,      WidgetSetPushButton )
  EW_OBJECT  ( ShowPanelEffect, EffectsInt32Effect )
  EW_OBJECT  ( RemovePanelEffect, EffectsInt32Effect )
  EW_OBJECT  ( Battery,         ViewsImage )
  EW_OBJECT  ( AutoDemoTimer,   CoreTimer )
  EW_VARIABLE( AutoState,       XInt32 )
  EW_VARIABLE( Mode,            XEnum )
EW_END_OF_FIELDS( OximeterOximeter )

/* Virtual Method Table (VMT) for the class : 'Oximeter::Oximeter' */
EW_DEFINE_METHODS( OximeterOximeter, CoreGroup )
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
  EW_METHOD( UpdateLayout,      void )( CoreGroup _this, XPoint aSize )
  EW_METHOD( UpdateViewState,   void )( CoreGroup _this, XSet aState )
  EW_METHOD( InvalidateArea,    void )( CoreGroup _this, XRect aArea )
EW_END_OF_METHODS( OximeterOximeter )

/* 'C' function for method : 'Oximeter::Oximeter.slotOverview()' */
void OximeterOximeter_slotOverview( OximeterOximeter _this, XObject sender );

/* 'C' function for method : 'Oximeter::Oximeter.slotGraphs()' */
void OximeterOximeter_slotGraphs( OximeterOximeter _this, XObject sender );

/* 'C' function for method : 'Oximeter::Oximeter.slotMenus()' */
void OximeterOximeter_slotMenus( OximeterOximeter _this, XObject sender );

/* 'C' function for method : 'Oximeter::Oximeter.slotOxiSettings()' */
void OximeterOximeter_slotOxiSettings( OximeterOximeter _this, XObject sender );

/* 'C' function for method : 'Oximeter::Oximeter.slotPulseSettings()' */
void OximeterOximeter_slotPulseSettings( OximeterOximeter _this, XObject sender );

/* 'C' function for method : 'Oximeter::Oximeter.removePanel()' */
void OximeterOximeter_removePanel( OximeterOximeter _this, XObject sender );

/* Slot method to trigger the next phase of the auto demo. */
void OximeterOximeter_AutoDemoSlot( OximeterOximeter _this, XObject sender );

#ifdef __cplusplus
  }
#endif

#endif /* _OximeterOximeter_H */

/* Embedded Wizard */
