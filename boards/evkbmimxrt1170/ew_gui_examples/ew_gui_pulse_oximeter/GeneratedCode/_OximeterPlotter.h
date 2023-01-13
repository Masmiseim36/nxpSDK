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

#ifndef _OximeterPlotter_H
#define _OximeterPlotter_H

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
#include "_CorePropertyObserver.h"
#include "_GraphicsPath.h"
#include "_ViewsLine.h"
#include "_ViewsStrokePath.h"
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

/* Forward declaration of the class Oximeter::DataStorage */
#ifndef _OximeterDataStorage_
  EW_DECLARE_CLASS( OximeterDataStorage )
#define _OximeterDataStorage_
#endif

/* Forward declaration of the class Oximeter::Plotter */
#ifndef _OximeterPlotter_
  EW_DECLARE_CLASS( OximeterPlotter )
#define _OximeterPlotter_
#endif


/* Deklaration of class : 'Oximeter::Plotter' */
EW_DEFINE_FIELDS( OximeterPlotter, CoreGroup )
  EW_PROPERTY( DataStorage,     OximeterDataStorage )
  EW_PROPERTY( OutletMin,       XRef )
  EW_PROPERTY( OutletMax,       XRef )
  EW_OBJECT  ( StrokePath,      ViewsStrokePath )
  EW_OBJECT  ( Path,            GraphicsPath )
  EW_OBJECT  ( MaxRangeLine,    ViewsLine )
  EW_OBJECT  ( MaxValueLine,    ViewsLine )
  EW_OBJECT  ( MinValueLine,    ViewsLine )
  EW_OBJECT  ( MinRangeLine,    ViewsLine )
  EW_OBJECT  ( MaxValueText,    ViewsText )
  EW_OBJECT  ( MinValueText,    ViewsText )
  EW_OBJECT  ( DataObserver,    CorePropertyObserver )
  EW_OBJECT  ( Line1,           ViewsLine )
  EW_OBJECT  ( Line2,           ViewsLine )
  EW_OBJECT  ( Line3,           ViewsLine )
  EW_OBJECT  ( Line4,           ViewsLine )
  EW_OBJECT  ( Line5,           ViewsLine )
  EW_PROPERTY( LineWidth,       XFloat )
  EW_PROPERTY( MinValue,        XInt32 )
  EW_PROPERTY( MaxValue,        XInt32 )
  EW_PROPERTY( MinRange,        XInt32 )
  EW_PROPERTY( MaxRange,        XInt32 )
  EW_PROPERTY( PixelPerValue,   XInt32 )
EW_END_OF_FIELDS( OximeterPlotter )

/* Virtual Method Table (VMT) for the class : 'Oximeter::Plotter' */
EW_DEFINE_METHODS( OximeterPlotter, CoreGroup )
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
  EW_METHOD( UpdateLayout,      void )( OximeterPlotter _this, XPoint aSize )
  EW_METHOD( UpdateViewState,   void )( OximeterPlotter _this, XSet aState )
  EW_METHOD( InvalidateArea,    void )( CoreGroup _this, XRect aArea )
EW_END_OF_METHODS( OximeterPlotter )

/* The method UpdateLayout() is invoked automatically after the size of the component 
   has been changed. This method can be overridden and filled with logic to perform 
   a sophisticated arrangement calculation for one or more enclosed views. In this 
   case the parameter aSize can be used. It contains the current size of the component. 
   Usually, all enclosed views are arranged automatically accordingly to their @Layout 
   property. UpdateLayout() gives the derived components a chance to extend this 
   automatism by a user defined algorithm. */
void OximeterPlotter_UpdateLayout( OximeterPlotter _this, XPoint aSize );

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
void OximeterPlotter_UpdateViewState( OximeterPlotter _this, XSet aState );

/* 'C' function for method : 'Oximeter::Plotter.OnSetMinValue()' */
void OximeterPlotter_OnSetMinValue( OximeterPlotter _this, XInt32 value );

/* 'C' function for method : 'Oximeter::Plotter.OnSetMaxValue()' */
void OximeterPlotter_OnSetMaxValue( OximeterPlotter _this, XInt32 value );

/* This slot method will receive a signal, if the value of the property assigned 
   to @OutletMin has been changed by another widget or by the application logic. 
   In response to this notification, the widget will update itself. */
void OximeterPlotter_outletMinSlot( OximeterPlotter _this, XObject sender );

/* 'C' function for method : 'Oximeter::Plotter.OnSetOutletMin()' */
void OximeterPlotter_OnSetOutletMin( OximeterPlotter _this, XRef value );

/* This slot method will receive a signal, if the value of the property assigned 
   to @OutletMax has been changed by another widget or by the application logic. 
   In response to this notification, the widget will update itself. */
void OximeterPlotter_outletMaxSlot( OximeterPlotter _this, XObject sender );

/* 'C' function for method : 'Oximeter::Plotter.OnSetOutletMax()' */
void OximeterPlotter_OnSetOutletMax( OximeterPlotter _this, XRef value );

/* 'C' function for method : 'Oximeter::Plotter.OnSetMinRange()' */
void OximeterPlotter_OnSetMinRange( OximeterPlotter _this, XInt32 value );

/* 'C' function for method : 'Oximeter::Plotter.OnSetMaxRange()' */
void OximeterPlotter_OnSetMaxRange( OximeterPlotter _this, XInt32 value );

/* 'C' function for method : 'Oximeter::Plotter.arrangeMinMax()' */
void OximeterPlotter_arrangeMinMax( OximeterPlotter _this );

/* 'C' function for method : 'Oximeter::Plotter.OnSetDataStorage()' */
void OximeterPlotter_OnSetDataStorage( OximeterPlotter _this, OximeterDataStorage 
  value );

/* This slot method is executed when the associated property observer 'DataObserver' 
   is notified. */
void OximeterPlotter_onDataAdded( OximeterPlotter _this, XObject sender );

#ifdef __cplusplus
  }
#endif

#endif /* _OximeterPlotter_H */

/* Embedded Wizard */
