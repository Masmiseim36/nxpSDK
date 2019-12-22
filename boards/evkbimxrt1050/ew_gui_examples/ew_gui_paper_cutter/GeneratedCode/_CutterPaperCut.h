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

#ifndef _CutterPaperCut_H
#define _CutterPaperCut_H

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
#include "_CutterHorzSlider.h"
#include "_CutterVertSlider.h"
#include "_EffectsInt32Effect.h"
#include "_ViewsImage.h"
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

/* Forward declaration of the class Cutter::PaperCut */
#ifndef _CutterPaperCut_
  EW_DECLARE_CLASS( CutterPaperCut )
#define _CutterPaperCut_
#endif

/* Forward declaration of the class Graphics::Canvas */
#ifndef _GraphicsCanvas_
  EW_DECLARE_CLASS( GraphicsCanvas )
#define _GraphicsCanvas_
#endif


/* The class 'PaperCut' implements a special widget to select the desired paper 
   format of the paper cutting machine. */
EW_DEFINE_FIELDS( CutterPaperCut, CoreGroup )
  EW_PROPERTY( PaperWidth,      XRef )
  EW_PROPERTY( PaperHeight,     XRef )
  EW_OBJECT  ( Image,           ViewsImage )
  EW_OBJECT  ( BorderBottom,    ViewsRectangle )
  EW_OBJECT  ( BorderTop,       ViewsRectangle )
  EW_OBJECT  ( BorderLeft,      ViewsRectangle )
  EW_OBJECT  ( BorderRight,     ViewsRectangle )
  EW_OBJECT  ( VertSlider,      CutterVertSlider )
  EW_OBJECT  ( HorzSlider,      CutterHorzSlider )
  EW_OBJECT  ( FadeEffect,      EffectsInt32Effect )
EW_END_OF_FIELDS( CutterPaperCut )

/* Virtual Method Table (VMT) for the class : 'Cutter::PaperCut' */
EW_DEFINE_METHODS( CutterPaperCut, CoreGroup )
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
  EW_METHOD( UpdateLayout,      void )( CoreGroup _this, XPoint aSize )
  EW_METHOD( UpdateViewState,   void )( CoreGroup _this, XSet aState )
  EW_METHOD( InvalidateArea,    void )( CoreGroup _this, XRect aArea )
  EW_METHOD( Restack,           void )( CoreGroup _this, CoreView aView, XInt32 
    aOrder )
  EW_METHOD( Add,               void )( CoreGroup _this, CoreView aView, XInt32 
    aOrder )
EW_END_OF_METHODS( CutterPaperCut )

/* 'C' function for method : 'Cutter::PaperCut.OnSetPaperWidth()' */
void CutterPaperCut_OnSetPaperWidth( CutterPaperCut _this, XRef value );

/* 'C' function for method : 'Cutter::PaperCut.OnSetPaperHeight()' */
void CutterPaperCut_OnSetPaperHeight( CutterPaperCut _this, XRef value );

/* Slot method to update the display value. */
void CutterPaperCut_onUpdate( CutterPaperCut _this, XObject sender );

/* Slot method to start fade-in of component. */
void CutterPaperCut_FadeIn( CutterPaperCut _this, XObject sender );

/* Slot method to start fade-out of component. */
void CutterPaperCut_FadeOut( CutterPaperCut _this, XObject sender );

#ifdef __cplusplus
  }
#endif

#endif /* _CutterPaperCut_H */

/* Embedded Wizard */
