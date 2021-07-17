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

#ifndef _ViewsWarpGroup_H
#define _ViewsWarpGroup_H

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

#include "_ViewsWarpView.h"

/* Forward declaration of the class Core::CursorHit */
#ifndef _CoreCursorHit_
  EW_DECLARE_CLASS( CoreCursorHit )
#define _CoreCursorHit_
#endif

/* Forward declaration of the class Core::Event */
#ifndef _CoreEvent_
  EW_DECLARE_CLASS( CoreEvent )
#define _CoreEvent_
#endif

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

/* Forward declaration of the class Graphics::WarpMatrix */
#ifndef _GraphicsWarpMatrix_
  EW_DECLARE_CLASS( GraphicsWarpMatrix )
#define _GraphicsWarpMatrix_
#endif

/* Forward declaration of the class Resources::Bitmap */
#ifndef _ResourcesBitmap_
  EW_DECLARE_CLASS( ResourcesBitmap )
#define _ResourcesBitmap_
#endif

/* Forward declaration of the class Views::WarpGroup */
#ifndef _ViewsWarpGroup_
  EW_DECLARE_CLASS( ViewsWarpGroup )
#define _ViewsWarpGroup_
#endif


/* The class Views::WarpGroup provides a kind of view able to show the aspect of 
   a foreign GUI component scaled and rotated in the 2D/3D space. In this manner 
   several warped copies of the component can be shown on the screen. Even the aspect 
   of an invisible component. This is very useful for fancy fade or mirror effects. 
   To work, the component has to be buffered. For this purpose set its property 
   Buffered to the value 'true'. Not buffered components will not appear in this 
   view. The GUI component to show its aspect is referred by the property @Group. 
   First of all, Embedded Wizard is not a 3D GUI development tool! Its lightweight 
   3D functionality is limited to the perspectively correct projection of an image 
   onto a quad (four corners polygon). Depending on this polygon's shape, different 
   kinds of transformations are possible, e.g. scaling, rotation or mirroring. The 
   polygon is determined by the four corners @Point1, @Point2, @Point3 and @Point4. 
   Moving these corners is thus sufficient to determine the desired transformation.
   More sophisticated way to control the transformations is provided by the methods 
   @RotateAndScale() and @Warp3D(). These methods derive the polygon coordinates 
   automatically from the given parameters.
   The properties @Opacity, @Opacity1, @Opacity2, @Opacity3 and @Opacity4 can be 
   used to modulate the transparency. For each polygon corner different opacity 
   can be set, so the component's aspect can be shown with fancy opacity gradients. 
   This view is also able to relay touch events to the associated GUI component. 
   This is determined by the property @RelayCursorEvents. 
   The visibility of the view is controlled by the properties @Visible, @AlphaBlended 
   and @Embedded. In particular the property @Embedded can determine whether the 
   corresponding view is limited (== embedded) to the boundary of a sibling Core::Outline 
   view or not. The embedding of views within a Core::Outline allows a kind of sub-groups 
   within the GUI component itself. Very useful for any kind of scrollable lists, 
   menus, etc. */
EW_DEFINE_FIELDS( ViewsWarpGroup, ViewsWarpView )
  EW_VARIABLE( cursorTargetView, CoreView )
  EW_VARIABLE( bitmap,          ResourcesBitmap )
  EW_PROPERTY( Group,           CoreGroup )
  EW_VARIABLE( i11,             XFloat )
  EW_VARIABLE( i12,             XFloat )
  EW_VARIABLE( i13,             XFloat )
  EW_VARIABLE( i21,             XFloat )
  EW_VARIABLE( i22,             XFloat )
  EW_VARIABLE( i23,             XFloat )
  EW_VARIABLE( i31,             XFloat )
  EW_VARIABLE( i32,             XFloat )
  EW_VARIABLE( i33,             XFloat )
  EW_PROPERTY( Opacity4,        XInt32 )
  EW_PROPERTY( Opacity3,        XInt32 )
  EW_PROPERTY( Opacity2,        XInt32 )
  EW_PROPERTY( Opacity1,        XInt32 )
  EW_VARIABLE( invMatrixValid,  XBool )
EW_END_OF_FIELDS( ViewsWarpGroup )

/* Virtual Method Table (VMT) for the class : 'Views::WarpGroup' */
EW_DEFINE_METHODS( ViewsWarpGroup, ViewsWarpView )
  EW_METHOD( initLayoutContext, void )( CoreQuadView _this, XRect aBounds, CoreOutline 
    aOutline )
  EW_METHOD( GetRoot,           CoreRoot )( CoreView _this )
  EW_METHOD( Draw,              void )( ViewsWarpGroup _this, GraphicsCanvas aCanvas, 
    XRect aClip, XPoint aOffset, XInt32 aOpacity, XBool aBlend )
  EW_METHOD( HandleEvent,       XObject )( ViewsWarpGroup _this, CoreEvent aEvent )
  EW_METHOD( CursorHitTest,     CoreCursorHit )( ViewsWarpGroup _this, XRect aArea, 
    XInt32 aFinger, XInt32 aStrikeCount, CoreView aDedicatedView, XSet aRetargetReason )
  EW_METHOD( ArrangeView,       XPoint )( CoreQuadView _this, XRect aBounds, XEnum 
    aFormation )
  EW_METHOD( MoveView,          void )( CoreQuadView _this, XPoint aOffset, XBool 
    aFastMove )
  EW_METHOD( GetExtent,         XRect )( CoreQuadView _this )
  EW_METHOD( ChangeViewState,   void )( CoreView _this, XSet aSetState, XSet aClearState )
  EW_METHOD( OnSetPoint4,       void )( ViewsWarpView _this, XPoint value )
  EW_METHOD( OnSetPoint3,       void )( ViewsWarpView _this, XPoint value )
  EW_METHOD( OnSetPoint2,       void )( ViewsWarpView _this, XPoint value )
  EW_METHOD( OnSetPoint1,       void )( ViewsWarpView _this, XPoint value )
  EW_METHOD( update,            void )( ViewsWarpGroup _this, XObject sender )
  EW_METHOD( RotateAndScale,    void )( ViewsWarpGroup _this, XPoint aDstPos, XFloat 
    aAngle, XFloat aScaleX, XFloat aScaleY )
EW_END_OF_METHODS( ViewsWarpGroup )

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
void ViewsWarpGroup_Draw( ViewsWarpGroup _this, GraphicsCanvas aCanvas, XRect aClip, 
  XPoint aOffset, XInt32 aOpacity, XBool aBlend );

/* The method HandleEvent() is invoked automatically if the view has received an 
   event. For example, touching the view on the touch screen can cause the view 
   to receive a Core::CursorEvent event. Within this method the view can evaluate 
   the event and react to it.
   Whether the event has been handled by the view or not is determined by the return 
   value. To sign an event as handled HandleEvent() should return a valid object 
   (e.g. 'this'). If the event has not been handled, 'null' should be returned.
   Depending on the kind of the event, the framework can continue dispatching of 
   still unhandled events. For example, keyboard events (Core::KeyEvent class) are 
   automatically delivered to the superior @Owner of the receiver view if this view 
   has ignored the event.
   HandleEvent() is invoked automatically by the framework, so you never should 
   need to invoke it directly. However you can prepare and post new events by using 
   the methods DispatchEvent() and BroadcastEvent() of the application class Core::Root. */
XObject ViewsWarpGroup_HandleEvent( ViewsWarpGroup _this, CoreEvent aEvent );

/* The method CursorHitTest() is invoked automatically in order to determine whether 
   the view is interested in the receipt of cursor events or not. This method will 
   be invoked immediately after the user has tapped the visible area of the view. 
   If the view is not interested in the cursor event, the framework repeats this 
   procedure for the next behind view until a willing view has been found or all 
   views are evaluated.
   In the implementation of the method the view can evaluate the passed aArea parameter. 
   It determines the place where the user has tapped the view with his fingertip 
   expressed in the coordinates of the views @Owner. The method can test e.g. whether 
   the tapped area does intersect any touchable areas within the view, etc. The 
   affected finger is identified in the parameter aFinger. The first finger (or 
   the first mouse device button) has the number 0.
   The parameter aStrikeCount determines how many times the same area has been tapped 
   in series. This is useful to detect series of multiple touches, e.g. in case 
   of the double click, aStrikeCount == 2.
   The parameter aDedicatedView, if it is not 'null', restricts the event to be 
   handled by this view only. If aDedicatedView == null, no special restriction 
   exists.
   This method is also invoked if during an existing grab cycle the current target 
   view has decided to resign and deflect the cursor events to another view. This 
   is usually the case after the user has performed a gesture the current target 
   view is not interested to process. Thereupon, the system looks for another view 
   willing to take over the cursor event processing after the performed gesture. 
   Which gesture has caused the operation, is specified in the parameter aRetargetReason.
   If the view is willing to process the event, the method should create, initialize 
   and return a new Core::CursorHit object. This object identify the willing view. 
   Otherwise the method should return 'null'.
   CursorHitTest() is invoked automatically by the framework, so you never should 
   need to invoke it directly. This method is predetermined for the hit-test only. 
   The proper processing of events should take place in the @HandleEvent() method 
   by reacting to Core::CursorEvent and Core::DragEvent events. */
CoreCursorHit ViewsWarpGroup_CursorHitTest( ViewsWarpGroup _this, XRect aArea, XInt32 
  aFinger, XInt32 aStrikeCount, CoreView aDedicatedView, XSet aRetargetReason );

/* 'C' function for method : 'Views::WarpGroup.update()' */
void ViewsWarpGroup_update( ViewsWarpGroup _this, XObject sender );

/* The method RotateAndScale() performs a 2D rotation and scaling of the source 
   image. The transformation is performed around a reference position specified 
   in the property @SourceAnchor. This allows e.g. a rotation around the center 
   of the image, etc. The rotation angle is passed in the parameter aAngle as a 
   value in the range 0 .. 360°. The values passed in the parameters aScaleX and 
   aScaleY determine additional scaling factors for the image in the horizontal 
   and vertical direction. If no scaling should be performed, the value 1.0 can 
   be passed to these parameters. In contrast, if no rotation is desired, 0.0 can 
   be passed to the parameter aAngle.
   The transformed image appears on the screen at the position aDstPos relative 
   to the top-left corner of this view's @Owner. The view itself takes the shape 
   resulting from this transformation and adapts its @Point1 .. @Point4 coordinates 
   accordingly. More sophisticated 2D and 3D transformations are performed by the 
   method @Warp3D(). */
void ViewsWarpGroup_RotateAndScale( ViewsWarpGroup _this, XPoint aDstPos, XFloat 
  aAngle, XFloat aScaleX, XFloat aScaleY );

/* 'C' function for method : 'Views::WarpGroup.updateInvMatrix()' */
void ViewsWarpGroup_updateInvMatrix( ViewsWarpGroup _this );

/* 'C' function for method : 'Views::WarpGroup.mapPosition()' */
XPoint ViewsWarpGroup_mapPosition( ViewsWarpGroup _this, XPoint aPos );

/* 'C' function for method : 'Views::WarpGroup.observerSlot()' */
void ViewsWarpGroup_observerSlot( ViewsWarpGroup _this, XObject sender );

/* 'C' function for method : 'Views::WarpGroup.OnSetOpacity()' */
void ViewsWarpGroup_OnSetOpacity( ViewsWarpGroup _this, XInt32 value );

/* 'C' function for method : 'Views::WarpGroup.OnSetGroup()' */
void ViewsWarpGroup_OnSetGroup( ViewsWarpGroup _this, CoreGroup value );

#ifdef __cplusplus
  }
#endif

#endif /* _ViewsWarpGroup_H */

/* Embedded Wizard */
