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

#ifndef _OximeterGraph_H
#define _OximeterGraph_H

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
#include "_GraphicsPath.h"
#include "_ViewsFillPath.h"
#include "_ViewsStrokePath.h"

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

/* Forward declaration of the class Oximeter::Graph */
#ifndef _OximeterGraph_
  EW_DECLARE_CLASS( OximeterGraph )
#define _OximeterGraph_
#endif


/* Deklaration of class : 'Oximeter::Graph' */
EW_DEFINE_FIELDS( OximeterGraph, CoreGroup )
  EW_PROPERTY( DataStorage,     OximeterDataStorage )
  EW_OBJECT  ( FillPath,        ViewsFillPath )
  EW_OBJECT  ( StrokePath,      ViewsStrokePath )
  EW_OBJECT  ( Path,            GraphicsPath )
  EW_VARIABLE( current,         XInt32 )
  EW_VARIABLE( noOfData,        XInt32 )
  EW_PROPERTY( LineColorLeft,   XColor )
  EW_PROPERTY( LineWidth,       XFloat )
  EW_PROPERTY( PixelPerValue,   XInt32 )
  EW_PROPERTY( ScrollOffset,    XInt32 )
  EW_PROPERTY( MinRange,        XInt32 )
  EW_PROPERTY( MaxRange,        XInt32 )
  EW_PROPERTY( LineColorRight,  XColor )
  EW_PROPERTY( FillColorTop,    XColor )
  EW_PROPERTY( FillColorBottom, XColor )
  EW_PROPERTY( GraphWidth,      XInt32 )
  EW_PROPERTY( Filled,          XBool )
  EW_PROPERTY( Bezier,          XBool )
EW_END_OF_FIELDS( OximeterGraph )

/* Virtual Method Table (VMT) for the class : 'Oximeter::Graph' */
EW_DEFINE_METHODS( OximeterGraph, CoreGroup )
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
  EW_METHOD( UpdateLayout,      void )( OximeterGraph _this, XPoint aSize )
  EW_METHOD( UpdateViewState,   void )( OximeterGraph _this, XSet aState )
  EW_METHOD( InvalidateArea,    void )( CoreGroup _this, XRect aArea )
EW_END_OF_METHODS( OximeterGraph )

/* The method UpdateLayout() is invoked automatically after the size of the component 
   has been changed. This method can be overridden and filled with logic to perform 
   a sophisticated arrangement calculation for one or more enclosed views. In this 
   case the parameter aSize can be used. It contains the current size of the component. 
   Usually, all enclosed views are arranged automatically accordingly to their @Layout 
   property. UpdateLayout() gives the derived components a chance to extend this 
   automatism by a user defined algorithm. */
void OximeterGraph_UpdateLayout( OximeterGraph _this, XPoint aSize );

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
void OximeterGraph_UpdateViewState( OximeterGraph _this, XSet aState );

/* 'C' function for method : 'Oximeter::Graph.OnSetLineColorLeft()' */
void OximeterGraph_OnSetLineColorLeft( OximeterGraph _this, XColor value );

/* 'C' function for method : 'Oximeter::Graph.OnSetLineWidth()' */
void OximeterGraph_OnSetLineWidth( OximeterGraph _this, XFloat value );

/* 'C' function for method : 'Oximeter::Graph.OnSetPixelPerValue()' */
void OximeterGraph_OnSetPixelPerValue( OximeterGraph _this, XInt32 value );

/* 'C' function for method : 'Oximeter::Graph.OnSetDataStorage()' */
void OximeterGraph_OnSetDataStorage( OximeterGraph _this, OximeterDataStorage value );

/* 'C' function for method : 'Oximeter::Graph.OnSetScrollOffset()' */
void OximeterGraph_OnSetScrollOffset( OximeterGraph _this, XInt32 value );

/* 'C' function for method : 'Oximeter::Graph.OnSetMinRange()' */
void OximeterGraph_OnSetMinRange( OximeterGraph _this, XInt32 value );

/* 'C' function for method : 'Oximeter::Graph.OnSetMaxRange()' */
void OximeterGraph_OnSetMaxRange( OximeterGraph _this, XInt32 value );

/* 'C' function for method : 'Oximeter::Graph.OnSetBezier()' */
void OximeterGraph_OnSetBezier( OximeterGraph _this, XBool value );

/* 'C' function for method : 'Oximeter::Graph.OnSetLineColorRight()' */
void OximeterGraph_OnSetLineColorRight( OximeterGraph _this, XColor value );

/* 'C' function for method : 'Oximeter::Graph.OnSetFillColorTop()' */
void OximeterGraph_OnSetFillColorTop( OximeterGraph _this, XColor value );

/* 'C' function for method : 'Oximeter::Graph.OnSetFillColorBottom()' */
void OximeterGraph_OnSetFillColorBottom( OximeterGraph _this, XColor value );

/* 'C' function for method : 'Oximeter::Graph.OnSetFilled()' */
void OximeterGraph_OnSetFilled( OximeterGraph _this, XBool value );

/* 'C' function for method : 'Oximeter::Graph.OnGetGraphWidth()' */
XInt32 OximeterGraph_OnGetGraphWidth( OximeterGraph _this );

#ifdef __cplusplus
  }
#endif

#endif /* _OximeterGraph_H */

/* Embedded Wizard */
