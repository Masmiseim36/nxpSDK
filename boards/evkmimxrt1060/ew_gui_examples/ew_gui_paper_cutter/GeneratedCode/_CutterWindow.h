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

#ifndef _CutterWindow_H
#define _CutterWindow_H

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
#include "_EffectsInt32Effect.h"
#include "_EffectsRectEffect.h"
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

/* Forward declaration of the class Cutter::Window */
#ifndef _CutterWindow_
  EW_DECLARE_CLASS( CutterWindow )
#define _CutterWindow_
#endif

/* Forward declaration of the class Graphics::Canvas */
#ifndef _GraphicsCanvas_
  EW_DECLARE_CLASS( GraphicsCanvas )
#define _GraphicsCanvas_
#endif


/* Base class of all paper cutter windows. */
EW_DEFINE_FIELDS( CutterWindow, CoreGroup )
  EW_PROPERTY( OnChange,        XSlot )
  EW_PROPERTY( OnCancel,        XSlot )
  EW_OBJECT  ( TouchHandler,    CoreSimpleTouchHandler )
  EW_OBJECT  ( Background,      ViewsRectangle )
  EW_OBJECT  ( CaptionRect,     ViewsRectangle )
  EW_OBJECT  ( CaptionText,     ViewsText )
  EW_OBJECT  ( RectEffect,      EffectsRectEffect )
  EW_OBJECT  ( FadeEffect,      EffectsInt32Effect )
  EW_VARIABLE( Mode,            XEnum )
EW_END_OF_FIELDS( CutterWindow )

/* Virtual Method Table (VMT) for the class : 'Cutter::Window' */
EW_DEFINE_METHODS( CutterWindow, CoreGroup )
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
  EW_METHOD( FadeAnimation,     void )( CutterWindow _this, XEnum aMode, XInt32 
    aValue )
EW_END_OF_METHODS( CutterWindow )

/* The method UpdateLayout() is invoked automatically after the size of the component 
   has been changed. This method can be overridden and filled with logic to perform 
   a sophisticated arrangement calculation for one or more enclosed views. In this 
   case the parameter aSize can be used. It contains the current size of the component. 
   Usually, all enclosed views are arranged automatically accordingly to their @Layout 
   property. UpdateLayout() gives the derived components a chance to extend this 
   automatism by a user defined algorithm. */
void CutterWindow_UpdateLayout( CutterWindow _this, XPoint aSize );

/* The method UpdateViewState() is invoked automatically after the state of the 
   component has been changed. This method can be overridden and filled with logic 
   to ensure the visual aspect of the component does reflect its current state. 
   For example, the 'enabled' state of the component can affect its colors (disabled 
   components may appear pale). In this case the logic of the method should modify 
   the respective color properties accordingly to the current 'enabled' state. 
   The current state of the component is passed as a set in the parameter aState. 
   It reflects the very basic component state like its visibility or the ability 
   to react to user inputs. Beside this common state, the method can also involve 
   any other variables used in the component as long as they reflect its current 
   state. For example, the toggle switch component can take in account its toggle 
   state 'on' or 'off' and change accordingly the location of the slider, etc.
   Usually, this method will be invoked automatically by the framework. Optionally 
   you can request its invocation by using the method @InvalidateViewState(). */
void CutterWindow_UpdateViewState( CutterWindow _this, XSet aState );

/* This internal slot method is used to receive the corresponding signals form the 
   touch handler. */
void CutterWindow_enterLeaveSlot( CutterWindow _this, XObject sender );

/* This internal slot method is used to receive the corresponding signals form the 
   touch handler. */
void CutterWindow_pressReleaseSlot( CutterWindow _this, XObject sender );

/* The function 'ChangeMode' is responsible to make the transission between the 
   different modes of the window and to start the corresponding animations. */
void CutterWindow_ChangeMode( CutterWindow _this, XEnum aMode, XRect aDestBounds );

/* Wrapper function for the virtual method : 'Cutter::Window.ChangeMode()' */
void CutterWindow__ChangeMode( void* _this, XEnum aMode, XRect aDestBounds );

/* This is a slot method. */
void CutterWindow_onFade( CutterWindow _this, XObject sender );

/* Method for doing a fade animation within the derived window classes. The parameter 
   aValue runs from 0 to 255 during the animation. */
void CutterWindow_FadeAnimation( CutterWindow _this, XEnum aMode, XInt32 aValue );

/* Wrapper function for the virtual method : 'Cutter::Window.FadeAnimation()' */
void CutterWindow__FadeAnimation( void* _this, XEnum aMode, XInt32 aValue );

/* Slot method to forward a signal from  an embedded Cancel button. */
void CutterWindow_onCancel( CutterWindow _this, XObject sender );

#ifdef __cplusplus
  }
#endif

#endif /* _CutterWindow_H */

/* Embedded Wizard */
