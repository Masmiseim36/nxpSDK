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

#ifndef _ViewsStrokePath_H
#define _ViewsStrokePath_H

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

#include "_CoreRectView.h"

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

/* Forward declaration of the class Graphics::Path */
#ifndef _GraphicsPath_
  EW_DECLARE_CLASS( GraphicsPath )
#define _GraphicsPath_
#endif

/* Forward declaration of the class Views::StrokePath */
#ifndef _ViewsStrokePath_
  EW_DECLARE_CLASS( ViewsStrokePath )
#define _ViewsStrokePath_
#endif


/* The class Views::StrokePath provides a kind of view specialized to stroke a path. 
   The coordinates of the path line segments are provided in a Graphics::Path object 
   assigned to the property @Path. The position and the size of the area to display 
   the path are determined by the property @Bounds. The color to use when stroking 
   the path is defined by the property @Color. Alternatively, the path can be stroked 
   with a color gradient specified by the properties @ColorTL, @ColorTR, @ColorBL 
   and @ColorBR. For each view's corner different color can be set. 
   The thickness of the stroked path is controlled by the property @Width. With 
   the properties @StartCap and @EndCap the appearance of the path ends, if the 
   path is opened, can be determined. The properties @JoinPoints and @MiterLimit 
   control the appearance of path corners, the join points between path line segments.
   With the property @FlipY and @Offset you can control the orientation and the 
   origin position of the path coordinate system. Finally, the property @Quality 
   determines whether the paths is rasterized with or without antialiasing.
   The visibility of the stroked path is controlled by the properties @Visible and 
   @Embedded. In particular the property @Embedded can determine whether the corresponding 
   view is limited (== embedded) to the boundary of a sibling Core::Outline view 
   or not. The embedding of views within a Core::Outline allows a kind of sub-groups 
   within the GUI component itself. Very useful for any kind of scrollable lists, 
   menus, etc.
   Finally, the property @Buffered controls whether the view should store its aspect 
   in an off-screen bitmap. This feature is useful to optimize screen update avoiding 
   the rasterization of the path. */
EW_DEFINE_FIELDS( ViewsStrokePath, CoreRectView )
  EW_PROPERTY( Path,            GraphicsPath )
  EW_VARIABLE( buffer,          XHandle )
  EW_PROPERTY( ColorBL,         XColor )
  EW_PROPERTY( ColorBR,         XColor )
  EW_PROPERTY( ColorTR,         XColor )
  EW_PROPERTY( ColorTL,         XColor )
  EW_PROPERTY( Offset,          XPoint )
  EW_PROPERTY( MiterLimit,      XFloat )
  EW_PROPERTY( Width,           XFloat )
  EW_PROPERTY( StartCap,        XEnum )
  EW_PROPERTY( EndCap,          XEnum )
  EW_PROPERTY( JoinPoints,      XEnum )
  EW_PROPERTY( Buffered,        XBool )
  EW_PROPERTY( Quality,         XBool )
  EW_PROPERTY( FlipY,           XBool )
EW_END_OF_FIELDS( ViewsStrokePath )

/* Virtual Method Table (VMT) for the class : 'Views::StrokePath' */
EW_DEFINE_METHODS( ViewsStrokePath, CoreRectView )
  EW_METHOD( initLayoutContext, void )( CoreRectView _this, XRect aBounds, CoreOutline 
    aOutline )
  EW_METHOD( GetRoot,           CoreRoot )( CoreView _this )
  EW_METHOD( Draw,              void )( ViewsStrokePath _this, GraphicsCanvas aCanvas, 
    XRect aClip, XPoint aOffset, XInt32 aOpacity, XBool aBlend )
  EW_METHOD( HandleEvent,       XObject )( CoreView _this, CoreEvent aEvent )
  EW_METHOD( CursorHitTest,     CoreCursorHit )( CoreView _this, XRect aArea, XInt32 
    aFinger, XInt32 aStrikeCount, CoreView aDedicatedView, XSet aRetargetReason )
  EW_METHOD( ArrangeView,       XPoint )( CoreRectView _this, XRect aBounds, XEnum 
    aFormation )
  EW_METHOD( MoveView,          void )( CoreRectView _this, XPoint aOffset, XBool 
    aFastMove )
  EW_METHOD( GetExtent,         XRect )( CoreRectView _this )
  EW_METHOD( ChangeViewState,   void )( CoreView _this, XSet aSetState, XSet aClearState )
  EW_METHOD( OnSetBounds,       void )( ViewsStrokePath _this, XRect value )
EW_END_OF_METHODS( ViewsStrokePath )

/* The method Draw() is invoked automatically if parts of the view should be redrawn 
   on the screen. This can occur when e.g. the view has been moved or the appearance 
   of the view has changed before.
   Draw() is invoked automatically by the framework, you never will need to invoke 
   this method directly. However you can request an invocation of this method by 
   calling the method InvalidateArea() of the views @Owner. Usually this is also 
   unnecessary unless you are developing your own view.
   The passed parameters determine the drawing destination aCanvas and the area 
   to redraw aClip in the coordinate space of the canvas. The parameter aOffset 
   contains the displacement between the origin of the views owner and the origin 
   of the canvas. You will need it to convert views coordinates into these of the 
   canvas.
   The parameter aOpacity contains the opacity descended from this view's @Owner. 
   It lies in range 0 .. 255. If the view implements its own 'Opacity', 'Color', 
   etc. properties, the Draw() method should calculate the resulting real opacity 
   by mixing the values of these properties with the one passed in aOpacity parameter.
   The parameter aBlend contains the blending mode descended from this view's @Owner. 
   It determines, whether the view should be drawn with alpha-blending active or 
   not. If aBlend is false, the outputs of the view should overwrite the corresponding 
   pixel in the drawing destination aCanvas. If aBlend is true, the outputs should 
   be mixed with the pixel already stored in aCanvas. For this purpose all Graphics 
   Engine functions provide a parameter to specify the mode for the respective drawing 
   operation. If the view implements its own 'Blend' property, the Draw() method 
   should calculate the resulting real blend mode by using logical AND operation 
   of the value of the property and the one passed in aBlend parameter. */
void ViewsStrokePath_Draw( ViewsStrokePath _this, GraphicsCanvas aCanvas, XRect 
  aClip, XPoint aOffset, XInt32 aOpacity, XBool aBlend );

/* 'C' function for method : 'Views::StrokePath.OnSetBounds()' */
void ViewsStrokePath_OnSetBounds( ViewsStrokePath _this, XRect value );

/* 'C' function for method : 'Views::StrokePath.Done()' */
void ViewsStrokePath_Done( ViewsStrokePath _this );

/* 'C' function for method : 'Views::StrokePath.destroyBuffer()' */
void ViewsStrokePath_destroyBuffer( ViewsStrokePath _this );

/* 'C' function for method : 'Views::StrokePath.updateBuffer()' */
void ViewsStrokePath_updateBuffer( ViewsStrokePath _this, XObject sender );

/* 'C' function for method : 'Views::StrokePath.updatePath()' */
void ViewsStrokePath_updatePath( ViewsStrokePath _this, XObject sender );

/* 'C' function for method : 'Views::StrokePath.OnSetColor()' */
void ViewsStrokePath_OnSetColor( ViewsStrokePath _this, XColor value );

/* 'C' function for method : 'Views::StrokePath.OnSetMiterLimit()' */
void ViewsStrokePath_OnSetMiterLimit( ViewsStrokePath _this, XFloat value );

/* 'C' function for method : 'Views::StrokePath.OnSetJoinPoints()' */
void ViewsStrokePath_OnSetJoinPoints( ViewsStrokePath _this, XEnum value );

/* 'C' function for method : 'Views::StrokePath.OnSetStartCap()' */
void ViewsStrokePath_OnSetStartCap( ViewsStrokePath _this, XEnum value );

/* 'C' function for method : 'Views::StrokePath.OnSetWidth()' */
void ViewsStrokePath_OnSetWidth( ViewsStrokePath _this, XFloat value );

/* 'C' function for method : 'Views::StrokePath.OnSetPath()' */
void ViewsStrokePath_OnSetPath( ViewsStrokePath _this, GraphicsPath value );

#ifdef __cplusplus
  }
#endif

#endif /* _ViewsStrokePath_H */

/* Embedded Wizard */
