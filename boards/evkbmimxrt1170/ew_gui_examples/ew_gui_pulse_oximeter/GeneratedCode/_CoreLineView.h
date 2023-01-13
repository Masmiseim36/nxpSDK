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

#ifndef _CoreLineView_H
#define _CoreLineView_H

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

#include "_CoreView.h"

/* Forward declaration of the class Core::Group */
#ifndef _CoreGroup_
  EW_DECLARE_CLASS( CoreGroup )
#define _CoreGroup_
#endif

/* Forward declaration of the class Core::LayoutContext */
#ifndef _CoreLayoutContext_
  EW_DECLARE_CLASS( CoreLayoutContext )
#define _CoreLayoutContext_
#endif

/* Forward declaration of the class Core::LineView */
#ifndef _CoreLineView_
  EW_DECLARE_CLASS( CoreLineView )
#define _CoreLineView_
#endif

/* Forward declaration of the class Core::Outline */
#ifndef _CoreOutline_
  EW_DECLARE_CLASS( CoreOutline )
#define _CoreOutline_
#endif


/* The class Core::LineView provides more specialized base functionality for all 
   view components with the shape of a simple line. It provides some few properties 
   to get and set the coordinates of the line end points @Point1 and @Point2.
   The class Core::LineView serves as base class for deriving line components only. 
   It doesn't define any particular behavior nor appearance. */
EW_DEFINE_FIELDS( CoreLineView, CoreView )
  EW_PROPERTY( Point2,          XPoint )
  EW_PROPERTY( Point1,          XPoint )
EW_END_OF_FIELDS( CoreLineView )

/* Virtual Method Table (VMT) for the class : 'Core::LineView' */
EW_DEFINE_METHODS( CoreLineView, CoreView )
  EW_METHOD( initLayoutContext, void )( CoreLineView _this, XRect aBounds, CoreOutline 
    aOutline )
  EW_METHOD( GetRoot,           CoreRoot )( CoreView _this )
  EW_METHOD( Draw,              void )( CoreView _this, GraphicsCanvas aCanvas, 
    XRect aClip, XPoint aOffset, XInt32 aOpacity, XBool aBlend )
  EW_METHOD( HandleEvent,       XObject )( CoreView _this, CoreEvent aEvent )
  EW_METHOD( CursorHitTest,     CoreCursorHit )( CoreView _this, XRect aArea, XInt32 
    aFinger, XInt32 aStrikeCount, CoreView aDedicatedView, XSet aRetargetReason )
  EW_METHOD( ArrangeView,       XPoint )( CoreLineView _this, XRect aBounds, XEnum 
    aFormation )
  EW_METHOD( MoveView,          void )( CoreLineView _this, XPoint aOffset, XBool 
    aFastMove )
  EW_METHOD( GetExtent,         XRect )( CoreLineView _this )
  EW_METHOD( ChangeViewState,   void )( CoreView _this, XSet aSetState, XSet aClearState )
EW_END_OF_METHODS( CoreLineView )

/* 'C' function for method : 'Core::LineView.initLayoutContext()' */
void CoreLineView_initLayoutContext( CoreLineView _this, XRect aBounds, CoreOutline 
  aOutline );

/* The method ArrangeView() is invoked automatically if the superior @Owner group 
   needs to re-arrange its views. For example, the changing of the owners size can 
   cause the enclosed views to adapt their position and size, so all views still 
   fit within the new owners area. This method provides the core functionality for 
   the automatic GUI layout mechanism.
   The arrangement is controlled primarily by the @Layout property of the view. 
   It specifies a set of alignment constraints and determines whether the view can 
   change its size. The bounds area where the view should be arranged, is passed 
   in the parameter aBounds. This is usually the current area of the views owner.
   The parameter aFormation defines the desired arrangement mode. Depending on the 
   value of this parameter, the views can be arranged in rows or columns. If aFormation 
   == Core::Formation.None, no automatic row/column arrangement is performed and 
   the view is moved and scaled only to fit inside the aBounds area.
   ArrangeView() is invoked automatically by the framework, so you never should 
   need to invoke it directly.
   The method returns the size of the view after it has been arranged. */
XPoint CoreLineView_ArrangeView( CoreLineView _this, XRect aBounds, XEnum aFormation );

/* The method MoveView() changes the position of the view by adding the value aOffset 
   to all corners of the view. For example, in case of a line view the value is 
   added to the both line end points.
   The parameter aFastMove serves for the optimization purpose. If it is 'true', 
   the corners are modified without performing any updates of the view and without 
   redrawing the screen. This is useful, when you wish to move or arrange a lot 
   of views at once. In this case it's up to you to request the finally screen update. 
   To do this you can use the method InvalidateArea() of the views @Owner.
   In the case aFastMove == false, the operation automatically requests the screen 
   redraw of the view areas before and after the movement. You don't need to take 
   care about it. */
void CoreLineView_MoveView( CoreLineView _this, XPoint aOffset, XBool aFastMove );

/* The method GetExtent() returns the position and the size of the view relative 
   to the origin of its @Owner. In case of views with a non rectangular shape the 
   method returns the rectangular boundary area enclosing the entire shape. */
XRect CoreLineView_GetExtent( CoreLineView _this );

/* 'C' function for method : 'Core::LineView.getExtent()' */
XRect CoreLineView_getExtent( CoreLineView _this );

/* Wrapper function for the non virtual method : 'Core::LineView.getExtent()' */
XRect CoreLineView__getExtent( void* _this );

/* The following define announces the presence of the method Core::LineView.getExtent(). */
#define _CoreLineView__getExtent_

/* 'C' function for method : 'Core::LineView.OnSetPoint2()' */
void CoreLineView_OnSetPoint2( CoreLineView _this, XPoint value );

/* 'C' function for method : 'Core::LineView.OnSetPoint1()' */
void CoreLineView_OnSetPoint1( CoreLineView _this, XPoint value );

#ifdef __cplusplus
  }
#endif

#endif /* _CoreLineView_H */

/* Embedded Wizard */
