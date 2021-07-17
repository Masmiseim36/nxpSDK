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

#ifndef _CoreOutline_H
#define _CoreOutline_H

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

/* Forward declaration of the class Core::Outline */
#ifndef _CoreOutline_
  EW_DECLARE_CLASS( CoreOutline )
#define _CoreOutline_
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


/* The class Core::Outline provides a special kind of a view able to unite and control 
   its sibling views. It acts like a lightweight container for the views without 
   being the owner of them:
   - Outline can control only views lying in front of it (views with higher Z-order). 
   Furthermore the affected views have to be signed explicitly with the state 'Embedded'. 
   Views lying behind the outline or views not signed as embedded are ignored by 
   the outline - they are controlled by their @Owner.
   - Outline limits the visibility of the embedded views to its own boundary area. 
   In this manner the views are clipped at the outlines edges.
   - Moving the outline causes the embedded views to be moved accordingly. Together 
   with the outline the views behave like a single union.
   - The embedded views can be scrolled within the outline boundary area. The property 
   @ScrollOffset determines the desired displacement.
   - A Core::SlideTouchHandler can be attached to the outline so the user can scroll 
   the outline content interactively by touching the screen. The property @SlideHandler 
   allows it. 
   - If the size of the outline changes, all embedded views are automatically arranged 
   within the outline boundary accordingly to their @Layout properties.
   - The property @Formation allows more sophisticated arrangement of embedded views 
   in columns and rows. Even table-like grid is possible. It is useful to create 
   scrollable menus or lists. With the properties @SpaceHorz, @SpaceVert and @Space 
   the padding between the automatically arranged views is controlled.
   - To find and enumerate the embedded views, the methods like @FindNextView(), 
   @FindPrevView(), @FindViewAtPosition, @FindViewInDirection() or @GetViewAtIndex() 
   are available. 
   - The method @EnsureVisible() has the job to scroll the outline contents until 
   the given view is visible. The method @AdjustContent() in contrast tries to scroll 
   the outline so the entire outline area is filled with views. Usually this is 
   used after the outline size changes.
   The outline itself is invisible except the Embedded Wizard Composer, where all 
   outlines appear as semitransparent rectangles. This allows you to interact with 
   the outlines during the design time. */
EW_DEFINE_FIELDS( CoreOutline, CoreRectView )
  EW_PROPERTY( OnUpdate,        XSlot )
  EW_PROPERTY( ScrollOffset,    XPoint )
  EW_PROPERTY( SpaceVert,       XInt32 )
  EW_PROPERTY( SpaceHorz,       XInt32 )
  EW_PROPERTY( Space,           XInt32 )
  EW_PROPERTY( Formation,       XEnum )
EW_END_OF_FIELDS( CoreOutline )

/* Virtual Method Table (VMT) for the class : 'Core::Outline' */
EW_DEFINE_METHODS( CoreOutline, CoreRectView )
  EW_METHOD( initLayoutContext, void )( CoreRectView _this, XRect aBounds, CoreOutline 
    aOutline )
  EW_METHOD( GetRoot,           CoreRoot )( CoreView _this )
  EW_METHOD( Draw,              void )( CoreOutline _this, GraphicsCanvas aCanvas, 
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
  EW_METHOD( OnSetBounds,       void )( CoreOutline _this, XRect value )
EW_END_OF_METHODS( CoreOutline )

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
void CoreOutline_Draw( CoreOutline _this, GraphicsCanvas aCanvas, XRect aClip, XPoint 
  aOffset, XInt32 aOpacity, XBool aBlend );

/* 'C' function for method : 'Core::Outline.OnSetBounds()' */
void CoreOutline_OnSetBounds( CoreOutline _this, XRect value );

#ifdef __cplusplus
  }
#endif

#endif /* _CoreOutline_H */

/* Embedded Wizard */
