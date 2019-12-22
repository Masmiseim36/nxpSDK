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

#ifndef _CutterFormatDialog_H
#define _CutterFormatDialog_H

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
#include "_CutterButton.h"
#include "_CutterCancelButton.h"
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

/* Forward declaration of the class Cutter::FormatDialog */
#ifndef _CutterFormatDialog_
  EW_DECLARE_CLASS( CutterFormatDialog )
#define _CutterFormatDialog_
#endif

/* Forward declaration of the class Graphics::Canvas */
#ifndef _GraphicsCanvas_
  EW_DECLARE_CLASS( GraphicsCanvas )
#define _GraphicsCanvas_
#endif


/* Dialog class to define the paper format produced by the paper cutting machine. */
EW_DEFINE_FIELDS( CutterFormatDialog, CoreGroup )
  EW_PROPERTY( OnCancel,        XSlot )
  EW_PROPERTY( PaperWidth,      XRef )
  EW_PROPERTY( PaperHeight,     XRef )
  EW_OBJECT  ( ButtonA3,        CutterButton )
  EW_OBJECT  ( TextW,           ViewsText )
  EW_OBJECT  ( TextH,           ViewsText )
  EW_OBJECT  ( TextWidth,       ViewsText )
  EW_OBJECT  ( TextHeight,      ViewsText )
  EW_OBJECT  ( CancelButton,    CutterCancelButton )
  EW_OBJECT  ( TextW0,          ViewsText )
  EW_OBJECT  ( ButtonA4,        CutterButton )
  EW_OBJECT  ( ButtonA5,        CutterButton )
  EW_OBJECT  ( ButtonA6,        CutterButton )
  EW_OBJECT  ( ButtonA7,        CutterButton )
  EW_OBJECT  ( ButtonA8,        CutterButton )
EW_END_OF_FIELDS( CutterFormatDialog )

/* Virtual Method Table (VMT) for the class : 'Cutter::FormatDialog' */
EW_DEFINE_METHODS( CutterFormatDialog, CoreGroup )
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
EW_END_OF_METHODS( CutterFormatDialog )

/* The method Init() is invoked automatically after the component has been created. 
   This method can be overridden and filled with logic containing additional initialization 
   statements. */
void CutterFormatDialog_Init( CutterFormatDialog _this, XHandle aArg );

/* 'C' function for method : 'Cutter::FormatDialog.OnSetPaperWidth()' */
void CutterFormatDialog_OnSetPaperWidth( CutterFormatDialog _this, XRef value );

/* 'C' function for method : 'Cutter::FormatDialog.OnSetPaperHeight()' */
void CutterFormatDialog_OnSetPaperHeight( CutterFormatDialog _this, XRef value );

/* Slot method to update the display value. */
void CutterFormatDialog_onUpdate( CutterFormatDialog _this, XObject sender );

/* Slot method to forward the cancel event. */
void CutterFormatDialog_onCancel( CutterFormatDialog _this, XObject sender );

/* Slot method to set corresponding paper width and height. */
void CutterFormatDialog_selectA3( CutterFormatDialog _this, XObject sender );

/* Slot method to set corresponding paper width and height. */
void CutterFormatDialog_selectA4( CutterFormatDialog _this, XObject sender );

/* Slot method to set corresponding paper width and height. */
void CutterFormatDialog_selectA5( CutterFormatDialog _this, XObject sender );

/* Slot method to set corresponding paper width and height. */
void CutterFormatDialog_selectA6( CutterFormatDialog _this, XObject sender );

/* Slot method to set corresponding paper width and height. */
void CutterFormatDialog_selectA7( CutterFormatDialog _this, XObject sender );

/* Slot method to set corresponding paper width and height. */
void CutterFormatDialog_selectA8( CutterFormatDialog _this, XObject sender );

#ifdef __cplusplus
  }
#endif

#endif /* _CutterFormatDialog_H */

/* Embedded Wizard */
