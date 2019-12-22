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

#ifndef _ViewsFrame_H
#define _ViewsFrame_H

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

/* Forward declaration of the class Core::Timer */
#ifndef _CoreTimer_
  EW_DECLARE_CLASS( CoreTimer )
#define _CoreTimer_
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

/* Forward declaration of the class Resources::Bitmap */
#ifndef _ResourcesBitmap_
  EW_DECLARE_CLASS( ResourcesBitmap )
#define _ResourcesBitmap_
#endif

/* Forward declaration of the class Views::Frame */
#ifndef _ViewsFrame_
  EW_DECLARE_CLASS( ViewsFrame )
#define _ViewsFrame_
#endif


/* The class Views::Frame provides a kind of view specialized to draw free scalable 
   frames by composing them of bitmap segments. These segments are used to draw 
   the frame's corners, to fill its edges and to fill its interior area. The bitmap 
   has thus to contain nine equal segments arranged in three rows and three columns. 
   The top-left segment e.g. is used to draw the top-left corner of the frame. In 
   contrast, the top-middle segment corresponds to the frame's top edge. If the 
   edge is wider than the segment, multiple copies of the segment are used to fill 
   the entire edge. In this manner the entire frame is composed by simply copying 
   bitmap segments. It's up to you to provide suitable bitmaps. The only thing you 
   have to take in account when designing them is, that their size have to be a 
   multiple of 3.
   The bitmap is determined by the property @Bitmap. In case of a multi-frame bitmap 
   the desired frame can be selected by the property @FrameNumber. Please don't 
   confuse these multi-frame bitmaps with the frames described above. Multi-frame 
   bitmaps, which are able to be animated can be controlled by the properties @Animated 
   and @Endless. Alternatively, the animation can be controlled by sending signals 
   to the slot methods @StartAnimation and @StopAnimation.
   The position and the size of the area to draw the frame is determined by the 
   property @Bounds. The properties @Color, @ColorTL, @ColorTR, @ColorBL and @ColorBR 
   can be used to tint the frame or to modulate its transparency. For each view's 
   corner different color can be set, so the frame can be shown with fancy color 
   or opacity gradients. The particular effect of the colors depends on the type 
   of the bitmap used to compose the frame:
   - Alpha8 bitmaps will be tinted with the given colors due to the fact they don't 
   provide their own color information. This allows one and the same Alpha8 bitmap 
   to be drawn with different colors.
   - In case of Native, Index8 or RGB565 bitmaps the opacity of the bitmap is modulated 
   by the color's alpha value. The original RGB information stored in the bitmap 
   are not affected. This allows one and the same bitmap to be drawn with different 
   transparencies.
   With the properties @Edges and @NoEdgesLimit you can individually configure which 
   edges of the frame view are displayed.
   The visibility of the frame is controlled by the properties @Visible, @AlphaBlended, 
   @Opacity and @Embedded. In particular the property @Embedded can determine whether 
   the corresponding view is limited (== embedded) to the boundary of a sibling 
   Core::Outline view or not. The embedding of views within a Core::Outline allows 
   a kind of sub-groups within the GUI component itself. Very useful for any kind 
   of scrollable lists, menus, etc. */
EW_DEFINE_FIELDS( ViewsFrame, CoreRectView )
  EW_VARIABLE( timer,           CoreTimer )
  EW_PROPERTY( Bitmap,          ResourcesBitmap )
  EW_PROPERTY( OnFinished,      XSlot )
  EW_VARIABLE( animFrameNumber, XInt32 )
  EW_PROPERTY( ColorBL,         XColor )
  EW_PROPERTY( ColorBR,         XColor )
  EW_PROPERTY( ColorTR,         XColor )
  EW_PROPERTY( ColorTL,         XColor )
  EW_PROPERTY( Edges,           XSet )
  EW_PROPERTY( FrameNumber,     XInt32 )
  EW_PROPERTY( Opacity,         XInt32 )
  EW_VARIABLE( startTime,       XUInt32 )
  EW_PROPERTY( NoEdgesLimit,    XPoint )
  EW_PROPERTY( Animated,        XBool )
  EW_PROPERTY( Endless,         XBool )
EW_END_OF_FIELDS( ViewsFrame )

/* Virtual Method Table (VMT) for the class : 'Views::Frame' */
EW_DEFINE_METHODS( ViewsFrame, CoreRectView )
  EW_METHOD( initLayoutContext, void )( CoreRectView _this, XRect aBounds, CoreOutline 
    aOutline )
  EW_METHOD( GetRoot,           CoreRoot )( CoreView _this )
  EW_METHOD( Draw,              void )( ViewsFrame _this, GraphicsCanvas aCanvas, 
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
  EW_METHOD( OnSetBounds,       void )( CoreRectView _this, XRect value )
EW_END_OF_METHODS( ViewsFrame )

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
void ViewsFrame_Draw( ViewsFrame _this, GraphicsCanvas aCanvas, XRect aClip, XPoint 
  aOffset, XInt32 aOpacity, XBool aBlend );

/* 'C' function for method : 'Views::Frame.observerSlot()' */
void ViewsFrame_observerSlot( ViewsFrame _this, XObject sender );

/* 'C' function for method : 'Views::Frame.timerSlot()' */
void ViewsFrame_timerSlot( ViewsFrame _this, XObject sender );

/* 'C' function for method : 'Views::Frame.OnSetColor()' */
void ViewsFrame_OnSetColor( ViewsFrame _this, XColor value );

/* 'C' function for method : 'Views::Frame.OnSetAnimated()' */
void ViewsFrame_OnSetAnimated( ViewsFrame _this, XBool value );

/* 'C' function for method : 'Views::Frame.OnSetFrameNumber()' */
void ViewsFrame_OnSetFrameNumber( ViewsFrame _this, XInt32 value );

/* 'C' function for method : 'Views::Frame.OnSetBitmap()' */
void ViewsFrame_OnSetBitmap( ViewsFrame _this, ResourcesBitmap value );

#ifdef __cplusplus
  }
#endif

#endif /* _ViewsFrame_H */

/* Embedded Wizard */
