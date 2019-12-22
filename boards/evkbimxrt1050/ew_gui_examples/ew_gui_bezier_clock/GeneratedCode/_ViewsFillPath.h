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

#ifndef _ViewsFillPath_H
#define _ViewsFillPath_H

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

/* Forward declaration of the class Views::FillPath */
#ifndef _ViewsFillPath_
  EW_DECLARE_CLASS( ViewsFillPath )
#define _ViewsFillPath_
#endif


/* The class Views::FillPath provides a kind of view specialized to draw a filled 
   polygon. The shape of the polygon (the coordinates of the line segments) is provided 
   in a Graphics::Path object assigned to the property @Path. The position and the 
   size of the area to display the path are determined by the property @Bounds. 
   The color to fill the path is defined by the property @Color. Alternatively, 
   the path can be filled with a color gradient specified by the properties @ColorTL, 
   @ColorTR, @ColorBL and @ColorBR. For each view's corner different color can be 
   set. 
   The property @FillRule determines the algorithm to decide which parts of complex 
   nested paths should be considered as filled and which are empty. With the property 
   @FlipY and @Offset you can control the orientation and the origin position of 
   the path coordinate system. Finally, the property @Quality determines whether 
   the paths is rasterized with or without antialiasing.
   The visibility of the filled path is controlled by the properties @Visible and 
   @Embedded. In particular the property @Embedded can determine whether the corresponding 
   view is limited (== embedded) to the boundary of a sibling Core::Outline view 
   or not. The embedding of views within a Core::Outline allows a kind of sub-groups 
   within the GUI component itself. Very useful for any kind of scrollable lists, 
   menus, etc.
   Finally, the property @Buffered controls whether the view should store its aspect 
   in an off-screen bitmap. This feature is useful to optimize screen update avoiding 
   the rasterization of the path. */
EW_DEFINE_FIELDS( ViewsFillPath, CoreRectView )
  EW_PROPERTY( Path,            GraphicsPath )
  EW_VARIABLE( buffer,          XHandle )
  EW_PROPERTY( ColorBL,         XColor )
  EW_PROPERTY( ColorBR,         XColor )
  EW_PROPERTY( ColorTR,         XColor )
  EW_PROPERTY( ColorTL,         XColor )
  EW_PROPERTY( Offset,          XPoint )
  EW_PROPERTY( FillRule,        XEnum )
  EW_PROPERTY( Buffered,        XBool )
  EW_PROPERTY( Quality,         XBool )
  EW_PROPERTY( FlipY,           XBool )
EW_END_OF_FIELDS( ViewsFillPath )

/* Virtual Method Table (VMT) for the class : 'Views::FillPath' */
EW_DEFINE_METHODS( ViewsFillPath, CoreRectView )
  EW_METHOD( initLayoutContext, void )( CoreRectView _this, XRect aBounds, CoreOutline 
    aOutline )
  EW_METHOD( GetRoot,           CoreRoot )( CoreView _this )
  EW_METHOD( Draw,              void )( ViewsFillPath _this, GraphicsCanvas aCanvas, 
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
  EW_METHOD( OnSetBounds,       void )( ViewsFillPath _this, XRect value )
EW_END_OF_METHODS( ViewsFillPath )

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
void ViewsFillPath_Draw( ViewsFillPath _this, GraphicsCanvas aCanvas, XRect aClip, 
  XPoint aOffset, XInt32 aOpacity, XBool aBlend );

/* 'C' function for method : 'Views::FillPath.OnSetBounds()' */
void ViewsFillPath_OnSetBounds( ViewsFillPath _this, XRect value );

/* 'C' function for method : 'Views::FillPath.Done()' */
void ViewsFillPath_Done( ViewsFillPath _this );

/* 'C' function for method : 'Views::FillPath.destroyBuffer()' */
void ViewsFillPath_destroyBuffer( ViewsFillPath _this );

/* 'C' function for method : 'Views::FillPath.updateBuffer()' */
void ViewsFillPath_updateBuffer( ViewsFillPath _this, XObject sender );

/* 'C' function for method : 'Views::FillPath.updatePath()' */
void ViewsFillPath_updatePath( ViewsFillPath _this, XObject sender );

/* 'C' function for method : 'Views::FillPath.OnSetColor()' */
void ViewsFillPath_OnSetColor( ViewsFillPath _this, XColor value );

/* 'C' function for method : 'Views::FillPath.OnSetOffset()' */
void ViewsFillPath_OnSetOffset( ViewsFillPath _this, XPoint value );

/* 'C' function for method : 'Views::FillPath.OnSetPath()' */
void ViewsFillPath_OnSetPath( ViewsFillPath _this, GraphicsPath value );

#ifdef __cplusplus
  }
#endif

#endif /* _ViewsFillPath_H */

/* Embedded Wizard */
