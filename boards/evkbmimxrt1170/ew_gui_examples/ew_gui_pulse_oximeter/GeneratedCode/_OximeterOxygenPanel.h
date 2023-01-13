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

#ifndef _OximeterOxygenPanel_H
#define _OximeterOxygenPanel_H

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
#include "_CoreSimpleTouchHandler.h"
#include "_EffectsInt32Effect.h"
#include "_EffectsRectEffect.h"
#include "_OximeterAnalogDisplay.h"
#include "_OximeterPlotter.h"
#include "_OximeterValueDisplay.h"
#include "_ViewsImage.h"
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

/* Forward declaration of the class Graphics::Canvas */
#ifndef _GraphicsCanvas_
  EW_DECLARE_CLASS( GraphicsCanvas )
#define _GraphicsCanvas_
#endif

/* Forward declaration of the class Oximeter::OxygenPanel */
#ifndef _OximeterOxygenPanel_
  EW_DECLARE_CLASS( OximeterOxygenPanel )
#define _OximeterOxygenPanel_
#endif


/* Deklaration of class : 'Oximeter::OxygenPanel' */
EW_DEFINE_FIELDS( OximeterOxygenPanel, CoreGroup )
  EW_PROPERTY( OnChart,         XSlot )
  EW_PROPERTY( OnAdjust,        XSlot )
  EW_PROPERTY( OnOverview,      XSlot )
  EW_OBJECT  ( RectEffect,      EffectsRectEffect )
  EW_OBJECT  ( AnalogDisplay,   OximeterAnalogDisplay )
  EW_OBJECT  ( PercentDisplay,  OximeterValueDisplay )
  EW_OBJECT  ( FadeOutEffect,   EffectsInt32Effect )
  EW_OBJECT  ( FadeInEffect,    EffectsInt32Effect )
  EW_OBJECT  ( Plotter,         OximeterPlotter )
  EW_OBJECT  ( IconTop,         ViewsImage )
  EW_OBJECT  ( IconLeft,        ViewsImage )
  EW_OBJECT  ( TextTop,         ViewsText )
  EW_OBJECT  ( TextRight,       ViewsText )
  EW_OBJECT  ( SimpleTouchHandlerChart, CoreSimpleTouchHandler )
  EW_OBJECT  ( SimpleTouchHandlerAdjust, CoreSimpleTouchHandler )
  EW_OBJECT  ( SimpleTouchHandlerOverviewTop, CoreSimpleTouchHandler )
  EW_OBJECT  ( SimpleTouchHandlerOverviewSide, CoreSimpleTouchHandler )
  EW_VARIABLE( Mode,            XEnum )
EW_END_OF_FIELDS( OximeterOxygenPanel )

/* Virtual Method Table (VMT) for the class : 'Oximeter::OxygenPanel' */
EW_DEFINE_METHODS( OximeterOxygenPanel, CoreGroup )
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
EW_END_OF_METHODS( OximeterOxygenPanel )

/* The method Init() is invoked automatically after the component has been created. 
   This method can be overridden and filled with logic containing additional initialization 
   statements. */
void OximeterOxygenPanel_Init( OximeterOxygenPanel _this, XHandle aArg );

/* The function 'ChangeMode' is responsible to make the transission between the 
   different modes of the window and to start the corresponding animations. */
void OximeterOxygenPanel_ChangeMode( OximeterOxygenPanel _this, XEnum aMode );

/* 'C' function for method : 'Oximeter::OxygenPanel.fadeOut()' */
void OximeterOxygenPanel_fadeOut( OximeterOxygenPanel _this, XObject sender );

/* 'C' function for method : 'Oximeter::OxygenPanel.fadeIn()' */
void OximeterOxygenPanel_fadeIn( OximeterOxygenPanel _this, XObject sender );

/* 'C' function for method : 'Oximeter::OxygenPanel.finished()' */
void OximeterOxygenPanel_finished( OximeterOxygenPanel _this, XObject sender );

/* 'C' function for method : 'Oximeter::OxygenPanel.OnSetOnChart()' */
void OximeterOxygenPanel_OnSetOnChart( OximeterOxygenPanel _this, XSlot value );

/* 'C' function for method : 'Oximeter::OxygenPanel.OnSetOnAdjust()' */
void OximeterOxygenPanel_OnSetOnAdjust( OximeterOxygenPanel _this, XSlot value );

/* 'C' function for method : 'Oximeter::OxygenPanel.OnSetOnOverview()' */
void OximeterOxygenPanel_OnSetOnOverview( OximeterOxygenPanel _this, XSlot value );

#ifdef __cplusplus
  }
#endif

#endif /* _OximeterOxygenPanel_H */

/* Embedded Wizard */
