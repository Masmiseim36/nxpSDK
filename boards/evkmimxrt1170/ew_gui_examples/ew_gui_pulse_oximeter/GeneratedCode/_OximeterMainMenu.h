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

#ifndef _OximeterMainMenu_H
#define _OximeterMainMenu_H

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
#include "_EffectsInt32Effect.h"
#include "_ViewsText.h"
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

/* Forward declaration of the class Oximeter::MainMenu */
#ifndef _OximeterMainMenu_
  EW_DECLARE_CLASS( OximeterMainMenu )
#define _OximeterMainMenu_
#endif


/* Deklaration of class : 'Oximeter::MainMenu' */
EW_DEFINE_FIELDS( OximeterMainMenu, CoreGroup )
  EW_PROPERTY( OnClose,         XSlot )
  EW_OBJECT  ( CloseButton,     WidgetSetPushButton )
  EW_OBJECT  ( TextMax,         ViewsText )
  EW_OBJECT  ( AlertText,       ViewsText )
  EW_OBJECT  ( MenuItem1,       WidgetSetPushButton )
  EW_OBJECT  ( MenuItem2,       WidgetSetPushButton )
  EW_OBJECT  ( MenuItem3,       WidgetSetPushButton )
  EW_OBJECT  ( MenuItem4,       WidgetSetPushButton )
  EW_OBJECT  ( MenuItem5,       WidgetSetPushButton )
  EW_OBJECT  ( MenuItem6,       WidgetSetPushButton )
  EW_OBJECT  ( MenuItem7,       WidgetSetPushButton )
  EW_OBJECT  ( MenuItem8,       WidgetSetPushButton )
  EW_OBJECT  ( TextMax1,        ViewsText )
  EW_OBJECT  ( MenuItem9,       WidgetSetPushButton )
  EW_OBJECT  ( TextMax2,        ViewsText )
  EW_OBJECT  ( AlertEffect,     EffectsInt32Effect )
EW_END_OF_FIELDS( OximeterMainMenu )

/* Virtual Method Table (VMT) for the class : 'Oximeter::MainMenu' */
EW_DEFINE_METHODS( OximeterMainMenu, CoreGroup )
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
EW_END_OF_METHODS( OximeterMainMenu )

/* 'C' function for method : 'Oximeter::MainMenu.OnSetOnClose()' */
void OximeterMainMenu_OnSetOnClose( OximeterMainMenu _this, XSlot value );

/* 'C' function for method : 'Oximeter::MainMenu.menuClick()' */
void OximeterMainMenu_menuClick( OximeterMainMenu _this, XObject sender );

#ifdef __cplusplus
  }
#endif

#endif /* _OximeterMainMenu_H */

/* Embedded Wizard */
