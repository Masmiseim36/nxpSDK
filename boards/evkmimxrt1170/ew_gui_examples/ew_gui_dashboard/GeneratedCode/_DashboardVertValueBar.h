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

#ifndef _DashboardVertValueBar_H
#define _DashboardVertValueBar_H

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

#include "_GraphicsPath.h"
#include "_TemplatesVerticalValueBar.h"
#include "_ViewsFillPath.h"
#include "_ViewsImage.h"

/* Forward declaration of the class Core::DialogContext */
#ifndef _CoreDialogContext_
  EW_DECLARE_CLASS( CoreDialogContext )
#define _CoreDialogContext_
#endif

/* Forward declaration of the class Core::Group */
#ifndef _CoreGroup_
  EW_DECLARE_CLASS( CoreGroup )
#define _CoreGroup_
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

/* Forward declaration of the class Dashboard::VertValueBar */
#ifndef _DashboardVertValueBar_
  EW_DECLARE_CLASS( DashboardVertValueBar )
#define _DashboardVertValueBar_
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

/* Forward declaration of the class Views::Text */
#ifndef _ViewsText_
  EW_DECLARE_CLASS( ViewsText )
#define _ViewsText_
#endif


/* This class implements a simple 'value bar' instrument intended to display a vertical 
   bar. The height of the bar corresponds to the value specified in the property 
   @CurrentValue, which lies in range determined by the properties @MinValue and 
   @MaxValue.
   Alternatively the property @Outlet can refer to any other 'int32' property the 
   widget should remain synchronized with. When the referred property is modified 
   by another one, the widget is automatically notified to remain in sync with the 
   property.
   This approach follows the Controller-View programming paradigm. Here the 'value 
   bar' widget represents the 'View' and the property referred via 'Outlet' can 
   be seen as a part of the 'Controller'. */
EW_DEFINE_FIELDS( DashboardVertValueBar, TemplatesVerticalValueBar )
  EW_ARRAY   ( label,           ViewsText, [10])
  EW_PROPERTY( Outlet,          XRef )
  EW_OBJECT  ( verticalValuePath, GraphicsPath )
  EW_OBJECT  ( needlePath,      GraphicsPath )
  EW_OBJECT  ( Wing,            ViewsImage )
  EW_OBJECT  ( ValueStrokePath, ViewsFillPath )
  EW_OBJECT  ( NeedleStrokePath, ViewsFillPath )
  EW_PROPERTY( MaxValue,        XInt32 )
  EW_PROPERTY( MinValue,        XInt32 )
  EW_PROPERTY( CurrentValue,    XInt32 )
  EW_VARIABLE( increment,       XInt32 )
  EW_PROPERTY( RightWing,       XBool )
EW_END_OF_FIELDS( DashboardVertValueBar )

/* Virtual Method Table (VMT) for the class : 'Dashboard::VertValueBar' */
EW_DEFINE_METHODS( DashboardVertValueBar, TemplatesVerticalValueBar )
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
  EW_METHOD( IsDialog,          XBool )( CoreGroup _this, XBool aRecursive )
  EW_METHOD( DispatchEvent,     XObject )( CoreGroup _this, CoreEvent aEvent )
  EW_METHOD( BroadcastEvent,    XObject )( CoreGroup _this, CoreEvent aEvent, XSet 
    aFilter )
  EW_METHOD( UpdateViewState,   void )( DashboardVertValueBar _this, XSet aState )
  EW_METHOD( InvalidateArea,    void )( CoreGroup _this, XRect aArea )
EW_END_OF_METHODS( DashboardVertValueBar )

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
void DashboardVertValueBar_UpdateViewState( DashboardVertValueBar _this, XSet aState );

/* The method Init() is invoked automatically after the component has been created. 
   This method can be overridden and filled with logic containing additional initialization 
   statements. */
void DashboardVertValueBar_Init( DashboardVertValueBar _this, XHandle aArg );

/* This slot method will receive a signal, if the value of the property assigned 
   to @Outlet has been changed by another widget or by the application logic. In 
   response to this notification, the widget will update itself. */
void DashboardVertValueBar_onOutlet( DashboardVertValueBar _this, XObject sender );

/* 'C' function for method : 'Dashboard::VertValueBar.OnSetOutlet()' */
void DashboardVertValueBar_OnSetOutlet( DashboardVertValueBar _this, XRef value );

/* 'C' function for method : 'Dashboard::VertValueBar.OnGetCurrentValue()' */
XInt32 DashboardVertValueBar_OnGetCurrentValue( DashboardVertValueBar _this );

/* 'C' function for method : 'Dashboard::VertValueBar.OnSetCurrentValue()' */
void DashboardVertValueBar_OnSetCurrentValue( DashboardVertValueBar _this, XInt32 
  value );

/* 'C' function for method : 'Dashboard::VertValueBar.setup()' */
void DashboardVertValueBar_setup( DashboardVertValueBar _this, XObject sender );

/* 'C' function for method : 'Dashboard::VertValueBar.OnSetRightWing()' */
void DashboardVertValueBar_OnSetRightWing( DashboardVertValueBar _this, XBool value );

/* 'C' function for method : 'Dashboard::VertValueBar.setupLabels()' */
void DashboardVertValueBar_setupLabels( DashboardVertValueBar _this, XObject sender );

/* 'C' function for method : 'Dashboard::VertValueBar.adjustLabels()' */
void DashboardVertValueBar_adjustLabels( DashboardVertValueBar _this, XObject sender );

#ifdef __cplusplus
  }
#endif

#endif /* _DashboardVertValueBar_H */

/* Embedded Wizard */
