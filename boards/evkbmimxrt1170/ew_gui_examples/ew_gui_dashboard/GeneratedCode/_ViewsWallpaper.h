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

#ifndef _ViewsWallpaper_H
#define _ViewsWallpaper_H

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

/* Forward declaration of the class Views::Wallpaper */
#ifndef _ViewsWallpaper_
  EW_DECLARE_CLASS( ViewsWallpaper )
#define _ViewsWallpaper_
#endif


/* The class Views::Wallpaper provides a kind of view specialized to fill an area 
   with multiple copies of a bitmap. The bitmap is determined by the property @Bitmap. 
   In case of a multi-frame bitmap the desired frame can be selected by the property 
   @FrameNumber. The animation of bitmaps containing short video sequences is controlled 
   by the properties @Animated and @Endless. Alternatively, the animation can be 
   controlled by sending signals to the slot methods @StartAnimation and @StopAnimation.
   The position and the size of the area to fill with the bitmap copies is determined 
   by the property @Bounds. The properties @Color, @ColorTL, @ColorTR, @ColorBL 
   and @ColorBR can be used to tint the copied bitmaps or to modulate their opacity. 
   For each view's corner different color can be set, so the wallpaper can be shown 
   with fancy color or opacity gradients. The particular effect of the colors depends 
   on the type of the bitmap:
   - Alpha8 bitmaps will be tinted with the given colors due to the fact they don't 
   provide their own color information. This allows one and the same Alpha8 bitmap 
   to be drawn with different colors.
   - In case of Native, Index8 or RGB565 bitmaps the opacity of the bitmap is modulated 
   by the color's alpha value. The original RGB information stored in the bitmap 
   are not affected. This allows one and the same bitmap to be drawn with different 
   transparencies.
   The property @ScrollOffset scrolls the bitmaps within the view's area. The visibility 
   of the wallpaper is controlled by the properties @Visible, @AlphaBlended, @Opacity 
   and @Embedded. In particular the property @Embedded can determine whether the 
   corresponding view is limited (== embedded) to the boundary of a sibling Core::Outline 
   view or not. The embedding of views within a Core::Outline allows a kind of sub-groups 
   within the GUI component itself. Very useful for any kind of scrollable lists, 
   menus, etc. */
EW_DEFINE_FIELDS( ViewsWallpaper, CoreRectView )
  EW_VARIABLE( timer,           CoreTimer )
  EW_PROPERTY( Bitmap,          ResourcesBitmap )
  EW_PROPERTY( OnFinished,      XSlot )
  EW_VARIABLE( animFrameNumber, XInt32 )
  EW_PROPERTY( ColorBL,         XColor )
  EW_PROPERTY( ColorBR,         XColor )
  EW_PROPERTY( ColorTR,         XColor )
  EW_PROPERTY( ColorTL,         XColor )
  EW_PROPERTY( ScrollOffset,    XPoint )
  EW_PROPERTY( FrameNumber,     XInt32 )
  EW_PROPERTY( Opacity,         XInt32 )
  EW_VARIABLE( startTime,       XUInt32 )
  EW_PROPERTY( Animated,        XBool )
  EW_PROPERTY( Endless,         XBool )
EW_END_OF_FIELDS( ViewsWallpaper )

/* Virtual Method Table (VMT) for the class : 'Views::Wallpaper' */
EW_DEFINE_METHODS( ViewsWallpaper, CoreRectView )
  EW_METHOD( initLayoutContext, void )( CoreRectView _this, XRect aBounds, CoreOutline 
    aOutline )
  EW_METHOD( GetRoot,           CoreRoot )( CoreView _this )
  EW_METHOD( Draw,              void )( ViewsWallpaper _this, GraphicsCanvas aCanvas, 
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
EW_END_OF_METHODS( ViewsWallpaper )

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
void ViewsWallpaper_Draw( ViewsWallpaper _this, GraphicsCanvas aCanvas, XRect aClip, 
  XPoint aOffset, XInt32 aOpacity, XBool aBlend );

/* 'C' function for method : 'Views::Wallpaper.observerSlot()' */
void ViewsWallpaper_observerSlot( ViewsWallpaper _this, XObject sender );

/* 'C' function for method : 'Views::Wallpaper.timerSlot()' */
void ViewsWallpaper_timerSlot( ViewsWallpaper _this, XObject sender );

/* 'C' function for method : 'Views::Wallpaper.OnSetAnimated()' */
void ViewsWallpaper_OnSetAnimated( ViewsWallpaper _this, XBool value );

/* 'C' function for method : 'Views::Wallpaper.OnSetScrollOffset()' */
void ViewsWallpaper_OnSetScrollOffset( ViewsWallpaper _this, XPoint value );

/* 'C' function for method : 'Views::Wallpaper.OnSetBitmap()' */
void ViewsWallpaper_OnSetBitmap( ViewsWallpaper _this, ResourcesBitmap value );

/* 'C' function for method : 'Views::Wallpaper.OnSetAlphaBlended()' */
void ViewsWallpaper_OnSetAlphaBlended( ViewsWallpaper _this, XBool value );

#ifdef __cplusplus
  }
#endif

#endif /* _ViewsWallpaper_H */

/* Embedded Wizard */
