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

#ifndef _WaveformWaveform_H
#define _WaveformWaveform_H

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
#include "_CoreOutline.h"
#include "_CoreSlideTouchHandler.h"
#include "_CoreTimer.h"
#include "_EffectsInt32Effect.h"
#include "_GraphicsPath.h"
#include "_ViewsFrame.h"
#include "_ViewsLine.h"
#include "_ViewsRectangle.h"
#include "_ViewsStrokePath.h"
#include "_ViewsText.h"
#include "_WaveformChannelPanel.h"

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

/* Forward declaration of the class Waveform::Waveform */
#ifndef _WaveformWaveform_
  EW_DECLARE_CLASS( WaveformWaveform )
#define _WaveformWaveform_
#endif


/* This is the root component of the entire GUI application. */
EW_DEFINE_FIELDS( WaveformWaveform, CoreGroup )
  EW_OBJECT  ( Rectangle,       ViewsRectangle )
  EW_OBJECT  ( Frame,           ViewsFrame )
  EW_OBJECT  ( SlideTouchHandler, CoreSlideTouchHandler )
  EW_OBJECT  ( Outline,         CoreOutline )
  EW_OBJECT  ( ChannelPanel1,   WaveformChannelPanel )
  EW_OBJECT  ( ChannelPanel2,   WaveformChannelPanel )
  EW_OBJECT  ( RecalcTimer,     CoreTimer )
  EW_OBJECT  ( AutoDemoTimer,   EffectsInt32Effect )
  EW_OBJECT  ( LineCenter,      ViewsLine )
  EW_OBJECT  ( LineTop,         ViewsLine )
  EW_OBJECT  ( LineBotom,       ViewsLine )
  EW_OBJECT  ( Path,            GraphicsPath )
  EW_OBJECT  ( StrokePath,      ViewsStrokePath )
  EW_OBJECT  ( TextCh1,         ViewsText )
  EW_OBJECT  ( TextCh2,         ViewsText )
  EW_OBJECT  ( Caption,         ViewsText )
EW_END_OF_FIELDS( WaveformWaveform )

/* Virtual Method Table (VMT) for the class : 'Waveform::Waveform' */
EW_DEFINE_METHODS( WaveformWaveform, CoreGroup )
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
EW_END_OF_METHODS( WaveformWaveform )

/* The method Init() is invoked automatically after the component has been created. 
   This method can be overridden and filled with logic containing additional initialization 
   statements. */
void WaveformWaveform_Init( WaveformWaveform _this, XHandle aArg );

/* The slot method 'SettingsChanged' is called when the user has changed the current 
   waveform gernator channel settings. */
void WaveformWaveform_SettingsChanged( WaveformWaveform _this, XObject sender );

/* Slot method to force the recalculation of coordinates in case of a noise waveform. */
void WaveformWaveform_TimerSlot( WaveformWaveform _this, XObject sender );

/* This is a slot method. */
void WaveformWaveform_AnimateFrequency( WaveformWaveform _this, XObject sender );

/* The method 'CalculatePath' contains all the mathematics to calculate the waveform 
   path (= all graph coordinates) according the current waveform generator settings. */
void WaveformWaveform_CalculatePath( WaveformWaveform _this );

#ifdef __cplusplus
  }
#endif

#endif /* _WaveformWaveform_H */

/* Embedded Wizard */
