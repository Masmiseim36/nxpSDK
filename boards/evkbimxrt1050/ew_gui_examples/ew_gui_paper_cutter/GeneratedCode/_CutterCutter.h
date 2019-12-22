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

#ifndef _CutterCutter_H
#define _CutterCutter_H

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
#include "_CoreTimer.h"
#include "_CutterBatchWindow.h"
#include "_CutterFeedWindow.h"
#include "_CutterFormatWindow.h"
#include "_CutterMainButton.h"
#include "_CutterPaperCut.h"
#include "_CutterRemainingWindow.h"
#include "_CutterSpeedWindow.h"
#include "_CutterStatusBar.h"
#include "_ViewsRectangle.h"

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

/* Forward declaration of the class Cutter::Cutter */
#ifndef _CutterCutter_
  EW_DECLARE_CLASS( CutterCutter )
#define _CutterCutter_
#endif

/* Forward declaration of the class Cutter::DeviceClass */
#ifndef _CutterDeviceClass_
  EW_DECLARE_CLASS( CutterDeviceClass )
#define _CutterDeviceClass_
#endif

/* Forward declaration of the class Graphics::Canvas */
#ifndef _GraphicsCanvas_
  EW_DECLARE_CLASS( GraphicsCanvas )
#define _GraphicsCanvas_
#endif


/* This is the main screen of the paper cutting machine. It arranges the different 
   setting windows. */
EW_DEFINE_FIELDS( CutterCutter, CoreGroup )
  EW_VARIABLE( Device,          CutterDeviceClass )
  EW_PROPERTY( PaperRemaining,  XRef )
  EW_OBJECT  ( Background,      ViewsRectangle )
  EW_OBJECT  ( FeedWindow,      CutterFeedWindow )
  EW_OBJECT  ( FormatWindow,    CutterFormatWindow )
  EW_OBJECT  ( BatchWindow,     CutterBatchWindow )
  EW_OBJECT  ( RemainingWindow, CutterRemainingWindow )
  EW_OBJECT  ( SpeedWindow,     CutterSpeedWindow )
  EW_OBJECT  ( MainButton,      CutterMainButton )
  EW_OBJECT  ( StatusBar,       CutterStatusBar )
  EW_OBJECT  ( PaperCut,        CutterPaperCut )
  EW_OBJECT  ( AutoDemoTimer,   CoreTimer )
  EW_VARIABLE( AutoState,       XInt32 )
EW_END_OF_FIELDS( CutterCutter )

/* Virtual Method Table (VMT) for the class : 'Cutter::Cutter' */
EW_DEFINE_METHODS( CutterCutter, CoreGroup )
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
  EW_METHOD( UpdateLayout,      void )( CutterCutter _this, XPoint aSize )
  EW_METHOD( UpdateViewState,   void )( CoreGroup _this, XSet aState )
  EW_METHOD( InvalidateArea,    void )( CoreGroup _this, XRect aArea )
  EW_METHOD( Restack,           void )( CoreGroup _this, CoreView aView, XInt32 
    aOrder )
  EW_METHOD( Add,               void )( CoreGroup _this, CoreView aView, XInt32 
    aOrder )
EW_END_OF_METHODS( CutterCutter )

/* The method UpdateLayout() is invoked automatically after the size of the component 
   has been changed. This method can be overridden and filled with logic to perform 
   a sophisticated arrangement calculation for one or more enclosed views. In this 
   case the parameter aSize can be used. It contains the current size of the component. 
   Usually, all enclosed views are arranged automatically accordingly to their @Layout 
   property. UpdateLayout() gives the derived components a chance to extend this 
   automatism by a user defined algorithm. */
void CutterCutter_UpdateLayout( CutterCutter _this, XPoint aSize );

/* The method Init() is invoked automatically after the component has been created. 
   This method can be overridden and filled with logic containing additional initialization 
   statements. */
void CutterCutter_Init( CutterCutter _this, XHandle aArg );

/* The slot method 'RunningMode' puts all members of the cutter into running mode. 
   The running mode is active, when the user has pressed the start button. */
void CutterCutter_RunningMode( CutterCutter _this, XObject sender );

/* The slot method 'ProgrammingMode' puts all members of the cutter into programming 
   mode. The programming mode is active, when the user has pressed the stop button. */
void CutterCutter_ProgrammingMode( CutterCutter _this, XObject sender );

/* The slot method 'ChangeFormat' puts the format window into exclusive mode. This 
   exclusive mode is active, when the user has pressed the window area of the format 
   window. */
void CutterCutter_ChangeFormat( CutterCutter _this, XObject sender );

/* The slot method 'ChangeFeed' puts the feed window into exclusive mode. This exclusive 
   mode is active, when the user has pressed the window area of the feed window. */
void CutterCutter_ChangeFeed( CutterCutter _this, XObject sender );

/* The slot method 'ChangeSpeed' puts the speed window into exclusive mode. This 
   exclusive mode is active, when the user has pressed the window area of the speed 
   window. */
void CutterCutter_ChangeSpeed( CutterCutter _this, XObject sender );

/* The slot method 'ChangeBatch' puts the batch window into exclusive mode. This 
   exclusive mode is active, when the user has pressed the window area of the batch 
   window. */
void CutterCutter_ChangeBatch( CutterCutter _this, XObject sender );

/* 'C' function for method : 'Cutter::Cutter.OnSetPaperRemaining()' */
void CutterCutter_OnSetPaperRemaining( CutterCutter _this, XRef value );

/* Slot method to update the display value. */
void CutterCutter_onUpdateRemaining( CutterCutter _this, XObject sender );

/* Slot method to trigger the next phase of the auto demo. */
void CutterCutter_AutoDemoSlot( CutterCutter _this, XObject sender );

#ifdef __cplusplus
  }
#endif

#endif /* _CutterCutter_H */

/* Embedded Wizard */
