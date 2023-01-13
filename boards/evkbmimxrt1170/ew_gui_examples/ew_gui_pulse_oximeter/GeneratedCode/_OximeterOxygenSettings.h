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

#ifndef _OximeterOxygenSettings_H
#define _OximeterOxygenSettings_H

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
#include "_OximeterNumberDisplay.h"
#include "_ViewsText.h"
#include "_WidgetSetHorizontalSlider.h"
#include "_WidgetSetPushButton.h"
#include "_WidgetSetRotaryKnob.h"

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

/* Forward declaration of the class Oximeter::OxygenSettings */
#ifndef _OximeterOxygenSettings_
  EW_DECLARE_CLASS( OximeterOxygenSettings )
#define _OximeterOxygenSettings_
#endif


/* Deklaration of class : 'Oximeter::OxygenSettings' */
EW_DEFINE_FIELDS( OximeterOxygenSettings, CoreGroup )
  EW_PROPERTY( OnClose,         XSlot )
  EW_OBJECT  ( RotaryKnobMax,   WidgetSetRotaryKnob )
  EW_OBJECT  ( RotaryKnobMin,   WidgetSetRotaryKnob )
  EW_OBJECT  ( HorizontalSlider, WidgetSetHorizontalSlider )
  EW_OBJECT  ( TextAlarm,       ViewsText )
  EW_OBJECT  ( PushButtonCancel, WidgetSetPushButton )
  EW_OBJECT  ( PushButtonSave,  WidgetSetPushButton )
  EW_OBJECT  ( TextMax,         ViewsText )
  EW_OBJECT  ( TextMin,         ViewsText )
  EW_OBJECT  ( NumberDisplayMax, OximeterNumberDisplay )
  EW_OBJECT  ( NumberDisplayMin, OximeterNumberDisplay )
  EW_OBJECT  ( NumberDisplayAlarm, OximeterNumberDisplay )
  EW_VARIABLE( oldMin,          XInt32 )
  EW_VARIABLE( oldMax,          XInt32 )
  EW_VARIABLE( oldAlarm,        XInt32 )
EW_END_OF_FIELDS( OximeterOxygenSettings )

/* Virtual Method Table (VMT) for the class : 'Oximeter::OxygenSettings' */
EW_DEFINE_METHODS( OximeterOxygenSettings, CoreGroup )
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
  EW_METHOD( UpdateLayout,      void )( OximeterOxygenSettings _this, XPoint aSize )
  EW_METHOD( UpdateViewState,   void )( CoreGroup _this, XSet aState )
  EW_METHOD( InvalidateArea,    void )( CoreGroup _this, XRect aArea )
EW_END_OF_METHODS( OximeterOxygenSettings )

/* The method UpdateLayout() is invoked automatically after the size of the component 
   has been changed. This method can be overridden and filled with logic to perform 
   a sophisticated arrangement calculation for one or more enclosed views. In this 
   case the parameter aSize can be used. It contains the current size of the component. 
   Usually, all enclosed views are arranged automatically accordingly to their @Layout 
   property. UpdateLayout() gives the derived components a chance to extend this 
   automatism by a user defined algorithm. */
void OximeterOxygenSettings_UpdateLayout( OximeterOxygenSettings _this, XPoint aSize );

/* 'C' function for method : 'Oximeter::OxygenSettings.onCancel()' */
void OximeterOxygenSettings_onCancel( OximeterOxygenSettings _this, XObject sender );

/* 'C' function for method : 'Oximeter::OxygenSettings.OnSetOnClose()' */
void OximeterOxygenSettings_OnSetOnClose( OximeterOxygenSettings _this, XSlot value );

#ifdef __cplusplus
  }
#endif

#endif /* _OximeterOxygenSettings_H */

/* Embedded Wizard */
