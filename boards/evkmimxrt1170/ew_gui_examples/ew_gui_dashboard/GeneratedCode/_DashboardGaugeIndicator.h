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

#ifndef _DashboardGaugeIndicator_H
#define _DashboardGaugeIndicator_H

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
#include "_ViewsWarpImage.h"

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

/* Forward declaration of the class Dashboard::GaugeIndicator */
#ifndef _DashboardGaugeIndicator_
  EW_DECLARE_CLASS( DashboardGaugeIndicator )
#define _DashboardGaugeIndicator_
#endif

/* Forward declaration of the class Effects::Fader */
#ifndef _EffectsFader_
  EW_DECLARE_CLASS( EffectsFader )
#define _EffectsFader_
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


/* The class 'PieChart' implements a GUI component for displaying a complete pie 
   chart diagram. The data for the single pie segments is defined within a @RecordList. */
EW_DEFINE_FIELDS( DashboardGaugeIndicator, CoreGroup )
  EW_PROPERTY( SegmentImage,    ResourcesBitmap )
  EW_VARIABLE( tempImage,       GraphicsCanvas )
  EW_PROPERTY( Outlet,          XRef )
  EW_OBJECT  ( Needle,          ViewsWarpImage )
  EW_PROPERTY( StartAngle,      XFloat )
  EW_PROPERTY( Scaling,         XFloat )
  EW_PROPERTY( Displacement,    XFloat )
  EW_PROPERTY( MaxValue,        XInt32 )
  EW_PROPERTY( MinValue,        XInt32 )
  EW_PROPERTY( CurrentValue,    XInt32 )
  EW_PROPERTY( EndAngle,        XFloat )
  EW_VARIABLE( newRotationAngle, XFloat )
EW_END_OF_FIELDS( DashboardGaugeIndicator )

/* Virtual Method Table (VMT) for the class : 'Dashboard::GaugeIndicator' */
EW_DEFINE_METHODS( DashboardGaugeIndicator, CoreGroup )
  EW_METHOD( initLayoutContext, void )( CoreRectView _this, XRect aBounds, CoreOutline 
    aOutline )
  EW_METHOD( GetRoot,           CoreRoot )( CoreView _this )
  EW_METHOD( Draw,              void )( DashboardGaugeIndicator _this, GraphicsCanvas 
    aCanvas, XRect aClip, XPoint aOffset, XInt32 aOpacity, XBool aBlend )
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
  EW_METHOD( IsDialog,          XBool )( CoreGroup _this, XBool aRecursive )
  EW_METHOD( DispatchEvent,     XObject )( CoreGroup _this, CoreEvent aEvent )
  EW_METHOD( BroadcastEvent,    XObject )( CoreGroup _this, CoreEvent aEvent, XSet 
    aFilter )
  EW_METHOD( UpdateViewState,   void )( CoreGroup _this, XSet aState )
  EW_METHOD( InvalidateArea,    void )( CoreGroup _this, XRect aArea )
EW_END_OF_METHODS( DashboardGaugeIndicator )

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
void DashboardGaugeIndicator_Draw( DashboardGaugeIndicator _this, GraphicsCanvas 
  aCanvas, XRect aClip, XPoint aOffset, XInt32 aOpacity, XBool aBlend );

/* The method Init() is invoked automatically after the component has been created. 
   This method can be overridden and filled with logic containing additional initialization 
   statements. */
void DashboardGaugeIndicator_Init( DashboardGaugeIndicator _this, XHandle aArg );

/* 'C' function for method : 'Dashboard::GaugeIndicator.OnSetStartAngle()' */
void DashboardGaugeIndicator_OnSetStartAngle( DashboardGaugeIndicator _this, XFloat 
  value );

/* 'C' function for method : 'Dashboard::GaugeIndicator.drawPie()' */
void DashboardGaugeIndicator_drawPie( DashboardGaugeIndicator _this, GraphicsCanvas 
  aCanvas, XRect aClip, XInt32 aX, XInt32 aY, XFloat aStartAngle, XFloat aDeltaAngle, 
  XFloat aScaling, XFloat aDisplacement, XColor aColor, XInt32 aOpacity, XBool aBlend );

/* 'C' function for method : 'Dashboard::GaugeIndicator.OnSetOutlet()' */
void DashboardGaugeIndicator_OnSetOutlet( DashboardGaugeIndicator _this, XRef value );

/* This slot method will receive a signal, if the value of the property assigned 
   to @Outlet has been changed by another widget or by the application logic. In 
   response to this notification, the widget will update itself. */
void DashboardGaugeIndicator_onOutlet( DashboardGaugeIndicator _this, XObject sender );

/* 'C' function for method : 'Dashboard::GaugeIndicator.OnSetCurrentValue()' */
void DashboardGaugeIndicator_OnSetCurrentValue( DashboardGaugeIndicator _this, XInt32 
  value );

/* 'C' function for method : 'Dashboard::GaugeIndicator.OnSetEndAngle()' */
void DashboardGaugeIndicator_OnSetEndAngle( DashboardGaugeIndicator _this, XFloat 
  value );

/* 'C' function for method : 'Dashboard::GaugeIndicator.drawNeedle()' */
void DashboardGaugeIndicator_drawNeedle( DashboardGaugeIndicator _this );

#ifdef __cplusplus
  }
#endif

#endif /* _DashboardGaugeIndicator_H */

/* Embedded Wizard */
