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

#ifndef _WidgetSetRotaryKnob_H
#define _WidgetSetRotaryKnob_H

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
#include "_CoreKeyPressHandler.h"
#include "_CoreRotateTouchHandler.h"
#include "_CoreSimpleTouchHandler.h"
#include "_CoreTimer.h"

/* Forward declaration of the class Core::DialogContext */
#ifndef _CoreDialogContext_
  EW_DECLARE_CLASS( CoreDialogContext )
#define _CoreDialogContext_
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

/* Forward declaration of the class Graphics::ArcPath */
#ifndef _GraphicsArcPath_
  EW_DECLARE_CLASS( GraphicsArcPath )
#define _GraphicsArcPath_
#endif

/* Forward declaration of the class Graphics::Canvas */
#ifndef _GraphicsCanvas_
  EW_DECLARE_CLASS( GraphicsCanvas )
#define _GraphicsCanvas_
#endif

/* Forward declaration of the class Views::FillPath */
#ifndef _ViewsFillPath_
  EW_DECLARE_CLASS( ViewsFillPath )
#define _ViewsFillPath_
#endif

/* Forward declaration of the class Views::Image */
#ifndef _ViewsImage_
  EW_DECLARE_CLASS( ViewsImage )
#define _ViewsImage_
#endif

/* Forward declaration of the class Views::WarpImage */
#ifndef _ViewsWarpImage_
  EW_DECLARE_CLASS( ViewsWarpImage )
#define _ViewsWarpImage_
#endif

/* Forward declaration of the class WidgetSet::RotaryKnob */
#ifndef _WidgetSetRotaryKnob_
  EW_DECLARE_CLASS( WidgetSetRotaryKnob )
#define _WidgetSetRotaryKnob_
#endif

/* Forward declaration of the class WidgetSet::RotaryKnobConfig */
#ifndef _WidgetSetRotaryKnobConfig_
  EW_DECLARE_CLASS( WidgetSetRotaryKnobConfig )
#define _WidgetSetRotaryKnobConfig_
#endif


/* This class implements a 'rotary knob' widget. When the user touches the thumb 
   and rotates it, the widget's current value represented by the int32 property 
   @CurrentValue is changed and signal is sent to the slot method stored in the 
   property @OnChange. The valid range for @CurrentValue is determined by the properties 
   @MinValue and @MaxValue. By connecting further slot methods to the properties 
   @OnStart and @OnEnd you can react to other events triggered while the user starts 
   and ends the interaction with the rotary knob.
   Alternatively the property @Outlet can refer to any other 'int32' property the 
   rotary knob widget should remain synchronized with. When the user rotates the 
   thumb, the affected property is automatically updated to reflect the widget's 
   current value. On the other hand, when the referred property is modified by another 
   one, the rotary knob is automatically notified to remain in sync with the property.
   This approach follows the Controller-View programming paradigm. Here the rotary 
   knob represents the 'View' and the property referred via 'Outlet' can be seen 
   as a part of the 'Controller'.
   During its lifetime the rotary knob remains always in one of the four states: 
   'disabled', 'default', 'focused' and 'active'. The state 'disabled' is true for 
   every not available rotary knob widget (the property @Enabled of the widget is 
   'false'). Such widgets will ignore all user inputs. The state 'default' determines 
   a rotary knob widget, which is ready to be touched by the user or ready to become 
   focused. As soon as the rotary knob widget becomes focused, it switches in the 
   state 'focused'. In this state the user can control the widget by pressing keys 
   on the keyboard. Finally, the state 'active' is true, if the user actually interacts 
   with the widget (the thumb is pressed).
   The exact look and feel of the rotary knob is determined by the 'Rotary Knob 
   Configuration' object assigned to the property @Appearance. The configuration 
   object provides bitmaps, colors and other configuration parameters needed to 
   construct and display the rotary knob widget. Usually, you will manage in your 
   project your own configuration objects and customize the rotary knob widgets 
   according to your design expectations. Depending on the information provided 
   in the associated configuration object, the rotary knob widget will be composed 
   of following views:
   - 'Scale' is an image view (Views::Image) displayed centered in the background 
   within the widget area. In the configuration object you can individually specify 
   for every widget state the desired bitmap, its opacity, frame number (if the 
   bitmap is multi-frame) and tint color (if the bitmap contains Alpha8 information 
   only). The rotary knob widget can automatically play animated bitmaps.
   - 'TrackLeft' is a vector graphic view (Views::FillPath) displaying a circle 
   segment between the thumb's leftmost position (clockwise) and its actual position. 
   From this results the effect of a track behind the thumb. In the configuration 
   object you can specify the track thickness as well as the appearance of its caps. 
   The colors of the track can be individually configured for every widget state.
   - 'TrackRight' is a vector graphic view (Views::FillPath) displaying a circle 
   segment between the actual position of the thumb and its rightmost position (clockwise). 
   From this results the effect of a track behind the thumb. In the configuration 
   object you can specify the track thickness as well as the appearance of its caps. 
   The colors of the track can be individually configured for every widget state.
   - 'Thumb' is a warp image view (Views::WarpImage) displayed and rotated around 
   predetermined pivot position. In the configuration object you can individually 
   specify for every widget state the desired bitmap, its opacity, frame number 
   (if the bitmap is multi-frame) and tint color (if the bitmap contains Alpha8 
   information only). The rotary knob widget can automatically play animated bitmaps. 
   The desired rotation range (the angles corresponding to the values @MinValue 
   and @MaxValue) and the pivot position are also specify in the configuration object.
   - 'Cover' is an image view (Views::Image) displayed centered within the widget 
   area and covering so eventually the scale and thumb images. In the configuration 
   object you can individually specify for every widget state the desired bitmap, 
   its opacity, frame number (if the bitmap is multi-frame) and tint color (if the 
   bitmap contains Alpha8 information only). The rotary knob widget can automatically 
   play animated bitmaps.
   If the rotary knob widget is actually focused, it can also be controlled by pressing 
   the keyboard keys Core::KeyCode.Left and Core::KeyCode.Right causing the thumb 
   to be rotated counterclockwise (key Left) or clockwise (key Right). In the configuration 
   object you can specify another key codes, if desired. To prevent the widget from 
   being able to be focused, specify in the configuration object the Core::KeyCode.NoKey 
   as codes to control the rotary knob. With the property @StepSize you can specify 
   a raster in which the rotary knob changes the value when user interacts with 
   it.
   The rotary knob widget provides an additional set of methods useful to query 
   the actual rotation angle of the thumb, its possible rotation range and the position 
   around which the thumb is rotated (@GetCenterPosition(), @GetThumbAngle(), @GetThumbMaxAngle(), 
   @GetThumbMinAngle()). These methods can be invoked from a slot method assigned 
   to the property @OnUpdate. In this manner you can automatically arrange and updated 
   additional decoration according to the actual state of the rotary knob widget. 
   For example, you can arrange a Views::Text view to follow the thumb and to display 
   the actual value of the widget (@CurrentValue).
   For more details regarding the customization of the rotary knob widget see the 
   description of WidgetSet::RotaryKnobConfig class. */
EW_DEFINE_FIELDS( WidgetSetRotaryKnob, CoreGroup )
  EW_VARIABLE( pathView1,       ViewsFillPath )
  EW_VARIABLE( pathView2,       ViewsFillPath )
  EW_VARIABLE( path1,           GraphicsArcPath )
  EW_VARIABLE( path2,           GraphicsArcPath )
  EW_VARIABLE( warpView,        ViewsWarpImage )
  EW_VARIABLE( imageView2,      ViewsImage )
  EW_VARIABLE( imageView1,      ViewsImage )
  EW_PROPERTY( Appearance,      WidgetSetRotaryKnobConfig )
  EW_PROPERTY( OnUpdate,        XSlot )
  EW_PROPERTY( OnChange,        XSlot )
  EW_PROPERTY( OnEnd,           XSlot )
  EW_PROPERTY( OnStart,         XSlot )
  EW_PROPERTY( Outlet,          XRef )
  EW_OBJECT  ( RepetitionTimer, CoreTimer )
  EW_OBJECT  ( KeyHandlerRight, CoreKeyPressHandler )
  EW_OBJECT  ( KeyHandlerLeft,  CoreKeyPressHandler )
  EW_OBJECT  ( TouchHandler,    CoreRotateTouchHandler )
  EW_OBJECT  ( TouchHandlerThumb, CoreSimpleTouchHandler )
  EW_VARIABLE( touchStartValue, XInt32 )
  EW_PROPERTY( StepSize,        XInt32 )
  EW_PROPERTY( MaxValue,        XInt32 )
  EW_PROPERTY( MinValue,        XInt32 )
  EW_PROPERTY( CurrentValue,    XInt32 )
EW_END_OF_FIELDS( WidgetSetRotaryKnob )

/* Virtual Method Table (VMT) for the class : 'WidgetSet::RotaryKnob' */
EW_DEFINE_METHODS( WidgetSetRotaryKnob, CoreGroup )
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
  EW_METHOD( OnSetBounds,       void )( WidgetSetRotaryKnob _this, XRect value )
  EW_METHOD( OnSetFocus,        void )( CoreGroup _this, CoreView value )
  EW_METHOD( OnSetBuffered,     void )( CoreGroup _this, XBool value )
  EW_METHOD( OnSetOpacity,      void )( CoreGroup _this, XInt32 value )
  EW_METHOD( DispatchEvent,     XObject )( CoreGroup _this, CoreEvent aEvent )
  EW_METHOD( BroadcastEvent,    XObject )( CoreGroup _this, CoreEvent aEvent, XSet 
    aFilter )
  EW_METHOD( UpdateLayout,      void )( CoreGroup _this, XPoint aSize )
  EW_METHOD( UpdateViewState,   void )( WidgetSetRotaryKnob _this, XSet aState )
  EW_METHOD( InvalidateArea,    void )( CoreGroup _this, XRect aArea )
EW_END_OF_METHODS( WidgetSetRotaryKnob )

/* 'C' function for method : 'WidgetSet::RotaryKnob.OnSetBounds()' */
void WidgetSetRotaryKnob_OnSetBounds( WidgetSetRotaryKnob _this, XRect value );

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
void WidgetSetRotaryKnob_UpdateViewState( WidgetSetRotaryKnob _this, XSet aState );

/* 'C' function for method : 'WidgetSet::RotaryKnob.onConfigChanged()' */
void WidgetSetRotaryKnob_onConfigChanged( WidgetSetRotaryKnob _this, XObject sender );

/* 'C' function for method : 'WidgetSet::RotaryKnob.onOutlet()' */
void WidgetSetRotaryKnob_onOutlet( WidgetSetRotaryKnob _this, XObject sender );

/* 'C' function for method : 'WidgetSet::RotaryKnob.onRepetitionTimer()' */
void WidgetSetRotaryKnob_onRepetitionTimer( WidgetSetRotaryKnob _this, XObject sender );

/* 'C' function for method : 'WidgetSet::RotaryKnob.onReleaseKey()' */
void WidgetSetRotaryKnob_onReleaseKey( WidgetSetRotaryKnob _this, XObject sender );

/* 'C' function for method : 'WidgetSet::RotaryKnob.onPressKey()' */
void WidgetSetRotaryKnob_onPressKey( WidgetSetRotaryKnob _this, XObject sender );

/* 'C' function for method : 'WidgetSet::RotaryKnob.onRotateTouch()' */
void WidgetSetRotaryKnob_onRotateTouch( WidgetSetRotaryKnob _this, XObject sender );

/* 'C' function for method : 'WidgetSet::RotaryKnob.onReleaseTouch()' */
void WidgetSetRotaryKnob_onReleaseTouch( WidgetSetRotaryKnob _this, XObject sender );

/* 'C' function for method : 'WidgetSet::RotaryKnob.onPressTouch()' */
void WidgetSetRotaryKnob_onPressTouch( WidgetSetRotaryKnob _this, XObject sender );

/* 'C' function for method : 'WidgetSet::RotaryKnob.onPressTouchThumb()' */
void WidgetSetRotaryKnob_onPressTouchThumb( WidgetSetRotaryKnob _this, XObject sender );

/* 'C' function for method : 'WidgetSet::RotaryKnob.OnSetOutlet()' */
void WidgetSetRotaryKnob_OnSetOutlet( WidgetSetRotaryKnob _this, XRef value );

/* 'C' function for method : 'WidgetSet::RotaryKnob.OnSetMaxValue()' */
void WidgetSetRotaryKnob_OnSetMaxValue( WidgetSetRotaryKnob _this, XInt32 value );

/* 'C' function for method : 'WidgetSet::RotaryKnob.OnSetMinValue()' */
void WidgetSetRotaryKnob_OnSetMinValue( WidgetSetRotaryKnob _this, XInt32 value );

/* 'C' function for method : 'WidgetSet::RotaryKnob.OnGetCurrentValue()' */
XInt32 WidgetSetRotaryKnob_OnGetCurrentValue( WidgetSetRotaryKnob _this );

/* 'C' function for method : 'WidgetSet::RotaryKnob.OnSetCurrentValue()' */
void WidgetSetRotaryKnob_OnSetCurrentValue( WidgetSetRotaryKnob _this, XInt32 value );

/* 'C' function for method : 'WidgetSet::RotaryKnob.OnSetAppearance()' */
void WidgetSetRotaryKnob_OnSetAppearance( WidgetSetRotaryKnob _this, WidgetSetRotaryKnobConfig 
  value );

#ifdef __cplusplus
  }
#endif

#endif /* _WidgetSetRotaryKnob_H */

/* Embedded Wizard */
