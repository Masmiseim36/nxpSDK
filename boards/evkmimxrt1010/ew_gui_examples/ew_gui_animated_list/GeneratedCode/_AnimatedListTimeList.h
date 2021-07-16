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

#ifndef _AnimatedListTimeList_H
#define _AnimatedListTimeList_H

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
#include "_CoreSimpleTouchHandler.h"
#include "_CoreSlideTouchHandler.h"
#include "_CoreVerticalList.h"
#include "_EffectsInt32Effect.h"
#include "_ViewsRectangle.h"
#include "_ViewsText.h"

/* Forward declaration of the class AnimatedList::TimeList */
#ifndef _AnimatedListTimeList_
  EW_DECLARE_CLASS( AnimatedListTimeList )
#define _AnimatedListTimeList_
#endif

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


/* This is a GUI component. */
EW_DEFINE_FIELDS( AnimatedListTimeList, CoreGroup )
  EW_PROPERTY( OnAppear,        XSlot )
  EW_PROPERTY( OnDisappear,     XSlot )
  EW_OBJECT  ( SlideTouchHandlerH, CoreSlideTouchHandler )
  EW_OBJECT  ( SimpleTouchHandlerH, CoreSimpleTouchHandler )
  EW_OBJECT  ( SlideTouchHandlerM, CoreSlideTouchHandler )
  EW_OBJECT  ( SimpleTouchHandlerM, CoreSimpleTouchHandler )
  EW_OBJECT  ( Rectangle,       ViewsRectangle )
  EW_OBJECT  ( BottomListH,     CoreVerticalList )
  EW_OBJECT  ( MainListH,       CoreVerticalList )
  EW_OBJECT  ( TopListH,        CoreVerticalList )
  EW_OBJECT  ( BottomListM,     CoreVerticalList )
  EW_OBJECT  ( MainListM,       CoreVerticalList )
  EW_OBJECT  ( TopListM,        CoreVerticalList )
  EW_OBJECT  ( Caption,         ViewsText )
  EW_OBJECT  ( DotText,         ViewsText )
  EW_OBJECT  ( TimezoneText,    ViewsText )
  EW_OBJECT  ( FadeListIn,      EffectsInt32Effect )
  EW_OBJECT  ( FadeCaption,     EffectsInt32Effect )
  EW_OBJECT  ( FadeDuration,    EffectsInt32Effect )
  EW_OBJECT  ( FadeListOut,     EffectsInt32Effect )
  EW_OBJECT  ( Int32EffectH,    EffectsInt32Effect )
  EW_OBJECT  ( Int32EffectM,    EffectsInt32Effect )
  EW_OBJECT  ( AutoDemoEffectH, EffectsInt32Effect )
  EW_OBJECT  ( AutoDemoEffectM, EffectsInt32Effect )
EW_END_OF_FIELDS( AnimatedListTimeList )

/* Virtual Method Table (VMT) for the class : 'AnimatedList::TimeList' */
EW_DEFINE_METHODS( AnimatedListTimeList, CoreGroup )
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
  EW_METHOD( Add,               void )( CoreGroup _this, CoreView aView, XInt32 
    aOrder )
EW_END_OF_METHODS( AnimatedListTimeList )

/* Slot method to load content of main scroll list. */
void AnimatedListTimeList_OnLoadMainH( AnimatedListTimeList _this, XObject sender );

/* Slot method to load content of upper scroll list. */
void AnimatedListTimeList_OnLoadTopH( AnimatedListTimeList _this, XObject sender );

/* Slot method to synchronize the scrolling of the upper/lower list with the current 
   scroll position of the center list. */
void AnimatedListTimeList_OnUpdateH( AnimatedListTimeList _this, XObject sender );

/* Slot method to load content of lower scroll list. */
void AnimatedListTimeList_OnLoadBottomH( AnimatedListTimeList _this, XObject sender );

/* Slot method to change the selection, when the item has reached its parking position. */
void AnimatedListTimeList_OnEnd( AnimatedListTimeList _this, XObject sender );

/* Slot method to make the upper/lower list visible. */
void AnimatedListTimeList_OnStart( AnimatedListTimeList _this, XObject sender );

/* This is a slot method. */
void AnimatedListTimeList_onAnimate( AnimatedListTimeList _this, XObject sender );

/* Slot method to load content of main scroll list. */
void AnimatedListTimeList_OnLoadMainM( AnimatedListTimeList _this, XObject sender );

/* Slot method to load content of upper scroll list. */
void AnimatedListTimeList_OnLoadTopM( AnimatedListTimeList _this, XObject sender );

/* Slot method to load content of lower scroll list. */
void AnimatedListTimeList_OnLoadBottomM( AnimatedListTimeList _this, XObject sender );

/* Slot method to synchronize the scrolling of the upper/lower list with the current 
   scroll position of the center list. */
void AnimatedListTimeList_OnUpdateM( AnimatedListTimeList _this, XObject sender );

/* This is a slot method. */
void AnimatedListTimeList_onFinished( AnimatedListTimeList _this, XObject sender );

/* 'C' function for method : 'AnimatedList::TimeList.OnReleaseH()' */
void AnimatedListTimeList_OnReleaseH( AnimatedListTimeList _this, XObject sender );

/* 'C' function for method : 'AnimatedList::TimeList.OnReleaseM()' */
void AnimatedListTimeList_OnReleaseM( AnimatedListTimeList _this, XObject sender );

/* Slot method to trigger the next phase of the auto demo. */
void AnimatedListTimeList_AutoDemo( AnimatedListTimeList _this, XObject sender );

#ifdef __cplusplus
  }
#endif

#endif /* _AnimatedListTimeList_H */

/* Embedded Wizard */
