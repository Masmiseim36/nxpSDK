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

#ifndef _WasherTimeList_H
#define _WasherTimeList_H

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
#include "_CoreSimpleTouchHandler.h"
#include "_CoreSlideTouchHandler.h"
#include "_CoreVerticalList.h"
#include "_EffectsInt32Effect.h"
#include "_ViewsRectangle.h"
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

/* Forward declaration of the class Washer::TimeList */
#ifndef _WasherTimeList_
  EW_DECLARE_CLASS( WasherTimeList )
#define _WasherTimeList_
#endif


/* This is a GUI component. */
EW_DEFINE_FIELDS( WasherTimeList, CoreGroup )
  EW_PROPERTY( OnAppear,        XSlot )
  EW_PROPERTY( OnDisappear,     XSlot )
  EW_OBJECT  ( SlideTouchHandlerH, CoreSlideTouchHandler )
  EW_OBJECT  ( SimpleTouchHandlerH, CoreSimpleTouchHandler )
  EW_OBJECT  ( Rectangle,       ViewsRectangle )
  EW_OBJECT  ( BottomListH,     CoreVerticalList )
  EW_OBJECT  ( MainListH,       CoreVerticalList )
  EW_OBJECT  ( TopListH,        CoreVerticalList )
  EW_OBJECT  ( FadeListIn,      EffectsInt32Effect )
  EW_OBJECT  ( Caption,         ViewsText )
  EW_OBJECT  ( FadeCaption,     EffectsInt32Effect )
  EW_OBJECT  ( SlideTouchHandlerM, CoreSlideTouchHandler )
  EW_OBJECT  ( SimpleTouchHandlerM, CoreSimpleTouchHandler )
  EW_OBJECT  ( BottomListM,     CoreVerticalList )
  EW_OBJECT  ( MainListM,       CoreVerticalList )
  EW_OBJECT  ( TopListM,        CoreVerticalList )
  EW_OBJECT  ( Text,            ViewsText )
  EW_OBJECT  ( DurationText,    ViewsText )
  EW_OBJECT  ( FadeDuration,    EffectsInt32Effect )
  EW_OBJECT  ( FadeListOut,     EffectsInt32Effect )
  EW_OBJECT  ( Int32EffectH,    EffectsInt32Effect )
  EW_OBJECT  ( Int32EffectM,    EffectsInt32Effect )
EW_END_OF_FIELDS( WasherTimeList )

/* Virtual Method Table (VMT) for the class : 'Washer::TimeList' */
EW_DEFINE_METHODS( WasherTimeList, CoreGroup )
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
EW_END_OF_METHODS( WasherTimeList )

/* Slot method to load content of main scroll list. */
void WasherTimeList_OnLoadMainH( WasherTimeList _this, XObject sender );

/* Slot method to load content of upper scroll list. */
void WasherTimeList_OnLoadTopH( WasherTimeList _this, XObject sender );

/* Slot method to synchronize the scrolling of the upper/lower list with the current 
   scroll position of the center list. */
void WasherTimeList_OnUpdateH( WasherTimeList _this, XObject sender );

/* Slot method to load content of lower scroll list. */
void WasherTimeList_OnLoadBottomH( WasherTimeList _this, XObject sender );

/* Slot method to change the selection, when the item has reached its parking position. */
void WasherTimeList_OnEnd( WasherTimeList _this, XObject sender );

/* Slot method to make the upper/lower list visible. */
void WasherTimeList_OnStart( WasherTimeList _this, XObject sender );

/* This is a slot method. */
void WasherTimeList_onAnimate( WasherTimeList _this, XObject sender );

/* Slot method to load content of main scroll list. */
void WasherTimeList_OnLoadMainM( WasherTimeList _this, XObject sender );

/* Slot method to load content of upper scroll list. */
void WasherTimeList_OnLoadTopM( WasherTimeList _this, XObject sender );

/* Slot method to load content of lower scroll list. */
void WasherTimeList_OnLoadBottomM( WasherTimeList _this, XObject sender );

/* Slot method to synchronize the scrolling of the upper/lower list with the current 
   scroll position of the center list. */
void WasherTimeList_OnUpdateM( WasherTimeList _this, XObject sender );

/* This is a slot method. */
void WasherTimeList_onFinished( WasherTimeList _this, XObject sender );

/* 'C' function for method : 'Washer::TimeList.OnDragH()' */
void WasherTimeList_OnDragH( WasherTimeList _this, XObject sender );

/* 'C' function for method : 'Washer::TimeList.OnReleaseH()' */
void WasherTimeList_OnReleaseH( WasherTimeList _this, XObject sender );

/* 'C' function for method : 'Washer::TimeList.OnDragM()' */
void WasherTimeList_OnDragM( WasherTimeList _this, XObject sender );

/* 'C' function for method : 'Washer::TimeList.OnReleaseM()' */
void WasherTimeList_OnReleaseM( WasherTimeList _this, XObject sender );

#ifdef __cplusplus
  }
#endif

#endif /* _WasherTimeList_H */

/* Embedded Wizard */
