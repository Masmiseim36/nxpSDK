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

#ifndef _WaveformChannelPanel_H
#define _WaveformChannelPanel_H

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
#include "_ViewsFrame.h"
#include "_ViewsText.h"
#include "_WidgetSetHorizontalSlider.h"
#include "_WidgetSetRadioButton.h"
#include "_WidgetSetRadioButtonConfig.h"

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

/* Forward declaration of the class Waveform::ChannelPanel */
#ifndef _WaveformChannelPanel_
  EW_DECLARE_CLASS( WaveformChannelPanel )
#define _WaveformChannelPanel_
#endif


/* The 'ChannelPanel' class implements a complete UI components for making all user 
   settings of a waveform generator channel. */
EW_DEFINE_FIELDS( WaveformChannelPanel, CoreGroup )
  EW_PROPERTY( OnChanged,       XSlot )
  EW_OBJECT  ( Frame,           ViewsFrame )
  EW_OBJECT  ( CaptionText,     ViewsText )
  EW_OBJECT  ( SliderFrequency, WidgetSetHorizontalSlider )
  EW_OBJECT  ( SliderAmplitude, WidgetSetHorizontalSlider )
  EW_OBJECT  ( Text1,           ViewsText )
  EW_OBJECT  ( Text2,           ViewsText )
  EW_OBJECT  ( ButtonSinus,     WidgetSetRadioButton )
  EW_OBJECT  ( ButtonSquare,    WidgetSetRadioButton )
  EW_OBJECT  ( ButtonRamp,      WidgetSetRadioButton )
  EW_OBJECT  ( CheckBox,        WidgetSetRadioButtonConfig )
  EW_PROPERTY( Caption,         XString )
  EW_PROPERTY( Frequency,       XInt32 )
  EW_PROPERTY( Amplitude,       XInt32 )
  EW_PROPERTY( Waveform,        XEnum )
EW_END_OF_FIELDS( WaveformChannelPanel )

/* Virtual Method Table (VMT) for the class : 'Waveform::ChannelPanel' */
EW_DEFINE_METHODS( WaveformChannelPanel, CoreGroup )
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
  EW_METHOD( UpdateViewState,   void )( CoreGroup _this, XSet aState )
  EW_METHOD( InvalidateArea,    void )( CoreGroup _this, XRect aArea )
EW_END_OF_METHODS( WaveformChannelPanel )

/* Slot method, called when the user changed a slider value. */
void WaveformChannelPanel_OnSlider( WaveformChannelPanel _this, XObject sender );

/* Slot method, called when the user pressed the corresponding waveform selection 
   button. */
void WaveformChannelPanel_OnSinus( WaveformChannelPanel _this, XObject sender );

/* 'C' function for method : 'Waveform::ChannelPanel.OnSetCaption()' */
void WaveformChannelPanel_OnSetCaption( WaveformChannelPanel _this, XString value );

/* Slot method, called when the user pressed the corresponding waveform selection 
   button. */
void WaveformChannelPanel_OnSquare( WaveformChannelPanel _this, XObject sender );

/* Slot method, called when the user pressed the corresponding waveform selection 
   button. */
void WaveformChannelPanel_OnRamp( WaveformChannelPanel _this, XObject sender );

/* 'C' function for method : 'Waveform::ChannelPanel.OnSetWaveform()' */
void WaveformChannelPanel_OnSetWaveform( WaveformChannelPanel _this, XEnum value );

/* Default onget method for the property 'Frequency' */
XInt32 WaveformChannelPanel_OnGetFrequency( WaveformChannelPanel _this );

/* Default onset method for the property 'Frequency' */
void WaveformChannelPanel_OnSetFrequency( WaveformChannelPanel _this, XInt32 value );

/* Default onget method for the property 'Amplitude' */
XInt32 WaveformChannelPanel_OnGetAmplitude( WaveformChannelPanel _this );

/* Default onset method for the property 'Amplitude' */
void WaveformChannelPanel_OnSetAmplitude( WaveformChannelPanel _this, XInt32 value );

#ifdef __cplusplus
  }
#endif

#endif /* _WaveformChannelPanel_H */

/* Embedded Wizard */
