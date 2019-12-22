/*******************************************************************************
*
* E M B E D D E D   W I Z A R D   P R O J E C T
*
*                                                Copyright (c) TARA Systems GmbH
*                                    written by Paul Banach and Manfred Schweyer
*
********************************************************************************
*
* This file was generated automatically by Embedded Wizard Studio.
*
* Please do not make any modifications of this file! The modifications are lost
* when the file is generated again by Embedded Wizard Studio!
*
* The template of this heading text can be found in the file 'head.ewt' in the
* directory 'Platforms' of your Embedded Wizard installation directory. If you
* wish to adapt this text, please copy the template file 'head.ewt' into your
* project directory and edit the copy only. Please avoid any modifications of
* the original template file!
*
* Version  : 9.20
* Profile  : iMX_RT
* Platform : NXP.iMX_RT.RGB565
*
*******************************************************************************/

#ifndef _WasherWasher_H
#define _WasherWasher_H

#ifdef __cplusplus
  extern "C"
  {
#endif

#include "ewrte.h"
#if EW_RTE_VERSION != 0x00090014
  #error Wrong version of Embedded Wizard Runtime Environment.
#endif

#include "ewgfx.h"
#if EW_GFX_VERSION != 0x00090014
  #error Wrong version of Embedded Wizard Graphics Engine.
#endif

#include "_CoreGroup.h"
#include "_CorePropertyObserver.h"
#include "_CoreTimer.h"
#include "_EffectsInt32Effect.h"
#include "_EffectsRectEffect.h"
#include "_ViewsBorder.h"
#include "_ViewsFrame.h"
#include "_ViewsImage.h"
#include "_ViewsRectangle.h"
#include "_ViewsText.h"
#include "_WasherOptList.h"
#include "_WasherProgList.h"
#include "_WasherSpinList.h"
#include "_WasherStartButton.h"
#include "_WasherTempList.h"
#include "_WasherTimeList.h"

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

/* Forward declaration of the class Washer::DeviceClass */
#ifndef _WasherDeviceClass_
  EW_DECLARE_CLASS( WasherDeviceClass )
#define _WasherDeviceClass_
#endif

/* Forward declaration of the class Washer::Washer */
#ifndef _WasherWasher_
  EW_DECLARE_CLASS( WasherWasher )
#define _WasherWasher_
#endif


/* This is a GUI component. */
EW_DEFINE_FIELDS( WasherWasher, CoreGroup )
  EW_VARIABLE( Device,          WasherDeviceClass )
  EW_OBJECT  ( Rectangle,       ViewsRectangle )
  EW_OBJECT  ( Image,           ViewsImage )
  EW_OBJECT  ( Frame,           ViewsFrame )
  EW_OBJECT  ( ProgList,        WasherProgList )
  EW_OBJECT  ( TempList,        WasherTempList )
  EW_OBJECT  ( SpinList,        WasherSpinList )
  EW_OBJECT  ( OptList,         WasherOptList )
  EW_OBJECT  ( TimeList,        WasherTimeList )
  EW_OBJECT  ( Border,          ViewsBorder )
  EW_OBJECT  ( Sep1,            ViewsRectangle )
  EW_OBJECT  ( Sep2,            ViewsRectangle )
  EW_OBJECT  ( Sep3,            ViewsRectangle )
  EW_OBJECT  ( Sep4,            ViewsRectangle )
  EW_OBJECT  ( FadeBgEffect,    EffectsInt32Effect )
  EW_OBJECT  ( StartButton,     WasherStartButton )
  EW_OBJECT  ( MoveStartEffect, EffectsRectEffect )
  EW_OBJECT  ( MessageEffect,   EffectsInt32Effect )
  EW_OBJECT  ( Message,         ViewsText )
  EW_OBJECT  ( ProgramObserver, CorePropertyObserver )
  EW_OBJECT  ( AutoDemoTimer,   CoreTimer )
  EW_VARIABLE( AutoState,       XInt32 )
EW_END_OF_FIELDS( WasherWasher )

/* Virtual Method Table (VMT) for the class : 'Washer::Washer' */
EW_DEFINE_METHODS( WasherWasher, CoreGroup )
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
  EW_METHOD( OnSetOpacity,      void )( CoreGroup _this, XInt32 value )
  EW_METHOD( DispatchEvent,     XObject )( CoreGroup _this, CoreEvent aEvent )
  EW_METHOD( BroadcastEvent,    XObject )( CoreGroup _this, CoreEvent aEvent, XSet 
    aFilter )
  EW_METHOD( UpdateViewState,   void )( CoreGroup _this, XSet aState )
  EW_METHOD( InvalidateArea,    void )( CoreGroup _this, XRect aArea )
  EW_METHOD( FindSiblingView,   CoreView )( CoreGroup _this, CoreView aView, XSet 
    aFilter )
  EW_METHOD( Restack,           void )( CoreGroup _this, CoreView aView, XInt32 
    aOrder )
  EW_METHOD( Add,               void )( CoreGroup _this, CoreView aView, XInt32 
    aOrder )
EW_END_OF_METHODS( WasherWasher )

/* The method Init() is invoked automatically after the component has been created. 
   This method can be overridden and filled with logic containing additional initialization 
   statements. */
void WasherWasher_Init( WasherWasher _this, XHandle aArg );

/* Slot method to update the display value. */
void WasherWasher_onUpdateProgram( WasherWasher _this, XObject sender );

/* This is a slot method. */
void WasherWasher_changeBackground( WasherWasher _this, XObject sender );

/* Slot to receive signal that time list will appear. */
void WasherWasher_OnListAppear( WasherWasher _this, XObject sender );

/* Slot to receive signal that time list has disappeared. */
void WasherWasher_OnListDisappear( WasherWasher _this, XObject sender );

/* 'C' function for method : 'Washer::Washer.onStart()' */
void WasherWasher_onStart( WasherWasher _this, XObject sender );

/* Slot method to trigger the next phase of the auto demo. */
void WasherWasher_AutoDemoSlot( WasherWasher _this, XObject sender );

#ifdef __cplusplus
  }
#endif

#endif /* _WasherWasher_H */

/* Embedded Wizard */
