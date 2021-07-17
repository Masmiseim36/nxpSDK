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

#ifndef _ViewsWarpView_H
#define _ViewsWarpView_H

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

#include "_CoreQuadView.h"

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

/* Forward declaration of the class Graphics::WarpMatrix */
#ifndef _GraphicsWarpMatrix_
  EW_DECLARE_CLASS( GraphicsWarpMatrix )
#define _GraphicsWarpMatrix_
#endif

/* Forward declaration of the class Views::WarpView */
#ifndef _ViewsWarpView_
  EW_DECLARE_CLASS( ViewsWarpView )
#define _ViewsWarpView_
#endif


/* The class Views::WarpView provides the base functionality for views able to show 
   images scaled and rotated in the 2D/3D space. First of all, Embedded Wizard is 
   not a 3D GUI development tool! Its lightweight 3D functionality is limited to 
   the perspectively correct projection of an image onto a quad (four corners polygon). 
   Depending on this polygon's shape, different kinds of transformations are possible, 
   e.g. scaling, rotation or mirroring. The polygon is determined by the four corners 
   @Point1, @Point2, @Point3 and @Point4. Moving these corners is thus sufficient 
   to determine the desired transformation.
   More sophisticated way to control the transformations is provided by the methods 
   @RotateAndScale() and @Warp3D(). These methods derive the polygon coordinates 
   automatically from the given parameters.
   The visibility of the view is controlled by the properties @Visible, @AlphaBlended, 
   and @Embedded. In particular the property @Embedded can determine whether the 
   corresponding view is limited (== embedded) to the boundary of a sibling Core::Outline 
   view or not. The embedding of views within a Core::Outline allows a kind of sub-groups 
   within the GUI component itself. Very useful for any kind of scrollable lists, 
   menus, etc.
   The class Views::WarpView doesn't define any particular appearance. It exists 
   as base for derived view classes like the Views::WarpImage and Views::WarpGroup. */
EW_DEFINE_FIELDS( ViewsWarpView, CoreQuadView )
  EW_VARIABLE( matrix,          GraphicsWarpMatrix )
  EW_VARIABLE( dstPosY,         XFloat )
  EW_VARIABLE( dstPosX,         XFloat )
  EW_VARIABLE( bitmapSize,      XPoint )
  EW_ARRAY   ( lighting,        XInt32, [4])
  EW_ARRAY   ( vertices,        XFloat, [4][3])
  EW_PROPERTY( Shininess,       XFloat )
  EW_PROPERTY( LightingEffect,  XFloat )
  EW_PROPERTY( Clipping,        XRect )
  EW_PROPERTY( SourceAnchor,    XPoint )
  EW_PROPERTY( SourceArea,      XRect )
  EW_VARIABLE( newUpdateCase,   XChar )
  EW_VARIABLE( oldUpdateCase,   XChar )
  EW_PROPERTY( Quality,         XBool )
EW_END_OF_FIELDS( ViewsWarpView )

/* Virtual Method Table (VMT) for the class : 'Views::WarpView' */
EW_DEFINE_METHODS( ViewsWarpView, CoreQuadView )
  EW_METHOD( initLayoutContext, void )( CoreQuadView _this, XRect aBounds, CoreOutline 
    aOutline )
  EW_METHOD( GetRoot,           CoreRoot )( CoreView _this )
  EW_METHOD( Draw,              void )( CoreView _this, GraphicsCanvas aCanvas, 
    XRect aClip, XPoint aOffset, XInt32 aOpacity, XBool aBlend )
  EW_METHOD( HandleEvent,       XObject )( CoreView _this, CoreEvent aEvent )
  EW_METHOD( CursorHitTest,     CoreCursorHit )( CoreView _this, XRect aArea, XInt32 
    aFinger, XInt32 aStrikeCount, CoreView aDedicatedView, XSet aRetargetReason )
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
  EW_METHOD( update,            void )( ViewsWarpView _this, XObject sender )
  EW_METHOD( RotateAndScale,    void )( ViewsWarpView _this, XPoint aDstPos, XFloat 
    aAngle, XFloat aScaleX, XFloat aScaleY )
EW_END_OF_METHODS( ViewsWarpView )

/* 'C' function for method : 'Views::WarpView.OnSetPoint4()' */
void ViewsWarpView_OnSetPoint4( ViewsWarpView _this, XPoint value );

/* 'C' function for method : 'Views::WarpView.OnSetPoint3()' */
void ViewsWarpView_OnSetPoint3( ViewsWarpView _this, XPoint value );

/* 'C' function for method : 'Views::WarpView.OnSetPoint2()' */
void ViewsWarpView_OnSetPoint2( ViewsWarpView _this, XPoint value );

/* 'C' function for method : 'Views::WarpView.OnSetPoint1()' */
void ViewsWarpView_OnSetPoint1( ViewsWarpView _this, XPoint value );

/* 'C' function for method : 'Views::WarpView.calculateLight()' */
void ViewsWarpView_calculateLight( ViewsWarpView _this );

/* 'C' function for method : 'Views::WarpView.update()' */
void ViewsWarpView_update( ViewsWarpView _this, XObject sender );

/* Wrapper function for the virtual method : 'Views::WarpView.update()' */
void ViewsWarpView__update( void* _this, XObject sender );

/* 'C' function for method : 'Views::WarpView.OnSetSourceAnchor()' */
void ViewsWarpView_OnSetSourceAnchor( ViewsWarpView _this, XPoint value );

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
void ViewsWarpView_RotateAndScale( ViewsWarpView _this, XPoint aDstPos, XFloat aAngle, 
  XFloat aScaleX, XFloat aScaleY );

/* Wrapper function for the virtual method : 'Views::WarpView.RotateAndScale()' */
void ViewsWarpView__RotateAndScale( void* _this, XPoint aDstPos, XFloat aAngle, 
  XFloat aScaleX, XFloat aScaleY );

#ifdef __cplusplus
  }
#endif

#endif /* _ViewsWarpView_H */

/* Embedded Wizard */
