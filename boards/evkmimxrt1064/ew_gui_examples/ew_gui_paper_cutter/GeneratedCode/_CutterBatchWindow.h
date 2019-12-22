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

#ifndef _CutterBatchWindow_H
#define _CutterBatchWindow_H

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

#include "_CoreSimpleTouchHandler.h"
#include "_CutterWindow.h"
#include "_EffectsInt32Effect.h"
#include "_EffectsRectEffect.h"
#include "_ViewsImage.h"
#include "_ViewsRectangle.h"
#include "_ViewsText.h"

/* Forward declaration of the class Core::DialogContext */
#ifndef _CoreDialogContext_
  EW_DECLARE_CLASS( CoreDialogContext )
#define _CoreDialogContext_
#endif

/* Forward declaration of the class Core::Group */
#ifndef _CoreGroup_
  EW_DECLARE_CLASS( CoreGroup )
#define _CoreGroup_
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

/* Forward declaration of the class Cutter::BatchDialog */
#ifndef _CutterBatchDialog_
  EW_DECLARE_CLASS( CutterBatchDialog )
#define _CutterBatchDialog_
#endif

/* Forward declaration of the class Cutter::BatchWindow */
#ifndef _CutterBatchWindow_
  EW_DECLARE_CLASS( CutterBatchWindow )
#define _CutterBatchWindow_
#endif

/* Forward declaration of the class Graphics::Canvas */
#ifndef _GraphicsCanvas_
  EW_DECLARE_CLASS( GraphicsCanvas )
#define _GraphicsCanvas_
#endif


/* Window class to host paper batch settings. */
EW_DEFINE_FIELDS( CutterBatchWindow, CutterWindow )
  EW_VARIABLE( dialog,          CutterBatchDialog )
  EW_PROPERTY( PaperBatch,      XRef )
  EW_OBJECT  ( Icon,            ViewsImage )
  EW_OBJECT  ( Text,            ViewsText )
  EW_OBJECT  ( TextBatchLarge,  ViewsText )
  EW_OBJECT  ( TextBatchMedium, ViewsText )
EW_END_OF_FIELDS( CutterBatchWindow )

/* Virtual Method Table (VMT) for the class : 'Cutter::BatchWindow' */
EW_DEFINE_METHODS( CutterBatchWindow, CutterWindow )
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
  EW_METHOD( UpdateLayout,      void )( CutterWindow _this, XPoint aSize )
  EW_METHOD( UpdateViewState,   void )( CutterWindow _this, XSet aState )
  EW_METHOD( InvalidateArea,    void )( CoreGroup _this, XRect aArea )
  EW_METHOD( Restack,           void )( CoreGroup _this, CoreView aView, XInt32 
    aOrder )
  EW_METHOD( Add,               void )( CoreGroup _this, CoreView aView, XInt32 
    aOrder )
  EW_METHOD( ChangeMode,        void )( CutterWindow _this, XEnum aMode, XRect aDestBounds )
  EW_METHOD( FadeAnimation,     void )( CutterBatchWindow _this, XEnum aMode, XInt32 
    aValue )
EW_END_OF_METHODS( CutterBatchWindow )

/* Method for doing a fade animation within the derived window classes. The parameter 
   aValue runs from 0 to 255 during the animation. */
void CutterBatchWindow_FadeAnimation( CutterBatchWindow _this, XEnum aMode, XInt32 
  aValue );

/* 'C' function for method : 'Cutter::BatchWindow.OnSetPaperBatch()' */
void CutterBatchWindow_OnSetPaperBatch( CutterBatchWindow _this, XRef value );

/* Slot method to update the display value. */
void CutterBatchWindow_onUpdate( CutterBatchWindow _this, XObject sender );

#ifdef __cplusplus
  }
#endif

#endif /* _CutterBatchWindow_H */

/* Embedded Wizard */
