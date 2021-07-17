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

#ifndef _OximeterValueDisplay_H
#define _OximeterValueDisplay_H

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
#include "_ViewsImage.h"
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

/* Forward declaration of the class Oximeter::ValueDisplay */
#ifndef _OximeterValueDisplay_
  EW_DECLARE_CLASS( OximeterValueDisplay )
#define _OximeterValueDisplay_
#endif

/* Forward declaration of the class Resources::Bitmap */
#ifndef _ResourcesBitmap_
  EW_DECLARE_CLASS( ResourcesBitmap )
#define _ResourcesBitmap_
#endif


/* Deklaration of class : 'Oximeter::ValueDisplay' */
EW_DEFINE_FIELDS( OximeterValueDisplay, CoreGroup )
  EW_PROPERTY( Icon,            ResourcesBitmap )
  EW_PROPERTY( Outlet,          XRef )
  EW_PROPERTY( OutletMin,       XRef )
  EW_PROPERTY( OutletMax,       XRef )
  EW_OBJECT  ( CaptionText,     ViewsText )
  EW_OBJECT  ( Separator,       ViewsRectangle )
  EW_OBJECT  ( Separator1,      ViewsRectangle )
  EW_OBJECT  ( RangeText,       ViewsText )
  EW_OBJECT  ( Image,           ViewsImage )
  EW_OBJECT  ( DigitOnes,       ViewsImage )
  EW_OBJECT  ( DigitTens,       ViewsImage )
  EW_OBJECT  ( DigitHuns,       ViewsImage )
  EW_OBJECT  ( DigitPercent,    ViewsImage )
  EW_PROPERTY( Caption,         XString )
  EW_PROPERTY( CurrentValue,    XInt32 )
  EW_PROPERTY( MinValue,        XInt32 )
  EW_PROPERTY( MaxValue,        XInt32 )
  EW_PROPERTY( HeaderOpacity,   XInt32 )
  EW_PROPERTY( Percent,         XBool )
EW_END_OF_FIELDS( OximeterValueDisplay )

/* Virtual Method Table (VMT) for the class : 'Oximeter::ValueDisplay' */
EW_DEFINE_METHODS( OximeterValueDisplay, CoreGroup )
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
  EW_METHOD( UpdateViewState,   void )( OximeterValueDisplay _this, XSet aState )
  EW_METHOD( InvalidateArea,    void )( CoreGroup _this, XRect aArea )
EW_END_OF_METHODS( OximeterValueDisplay )

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
void OximeterValueDisplay_UpdateViewState( OximeterValueDisplay _this, XSet aState );

/* 'C' function for method : 'Oximeter::ValueDisplay.OnSetCurrentValue()' */
void OximeterValueDisplay_OnSetCurrentValue( OximeterValueDisplay _this, XInt32 
  value );

/* This slot method will receive a signal, if the value of the property assigned 
   to @Outlet has been changed by another widget or by the application logic. In 
   response to this notification, the widget will update itself. */
void OximeterValueDisplay_outletSlot( OximeterValueDisplay _this, XObject sender );

/* 'C' function for method : 'Oximeter::ValueDisplay.OnSetOutlet()' */
void OximeterValueDisplay_OnSetOutlet( OximeterValueDisplay _this, XRef value );

/* 'C' function for method : 'Oximeter::ValueDisplay.OnSetMinValue()' */
void OximeterValueDisplay_OnSetMinValue( OximeterValueDisplay _this, XInt32 value );

/* This slot method will receive a signal, if the value of the property assigned 
   to @OutletMin has been changed by another widget or by the application logic. 
   In response to this notification, the widget will update itself. */
void OximeterValueDisplay_outletMinSlot( OximeterValueDisplay _this, XObject sender );

/* 'C' function for method : 'Oximeter::ValueDisplay.OnSetOutletMin()' */
void OximeterValueDisplay_OnSetOutletMin( OximeterValueDisplay _this, XRef value );

/* 'C' function for method : 'Oximeter::ValueDisplay.OnSetMaxValue()' */
void OximeterValueDisplay_OnSetMaxValue( OximeterValueDisplay _this, XInt32 value );

/* This slot method will receive a signal, if the value of the property assigned 
   to @OutletMax has been changed by another widget or by the application logic. 
   In response to this notification, the widget will update itself. */
void OximeterValueDisplay_outletMaxSlot( OximeterValueDisplay _this, XObject sender );

/* 'C' function for method : 'Oximeter::ValueDisplay.OnSetOutletMax()' */
void OximeterValueDisplay_OnSetOutletMax( OximeterValueDisplay _this, XRef value );

/* 'C' function for method : 'Oximeter::ValueDisplay.OnSetIcon()' */
void OximeterValueDisplay_OnSetIcon( OximeterValueDisplay _this, ResourcesBitmap 
  value );

/* 'C' function for method : 'Oximeter::ValueDisplay.OnSetCaption()' */
void OximeterValueDisplay_OnSetCaption( OximeterValueDisplay _this, XString value );

/* 'C' function for method : 'Oximeter::ValueDisplay.OnSetHeaderOpacity()' */
void OximeterValueDisplay_OnSetHeaderOpacity( OximeterValueDisplay _this, XInt32 
  value );

/* 'C' function for method : 'Oximeter::ValueDisplay.OnSetPercent()' */
void OximeterValueDisplay_OnSetPercent( OximeterValueDisplay _this, XBool value );

#ifdef __cplusplus
  }
#endif

#endif /* _OximeterValueDisplay_H */

/* Embedded Wizard */
