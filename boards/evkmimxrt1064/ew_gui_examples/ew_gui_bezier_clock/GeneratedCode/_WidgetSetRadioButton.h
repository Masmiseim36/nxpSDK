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

#ifndef _WidgetSetRadioButton_H
#define _WidgetSetRadioButton_H

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

#include "_CoreGroup.h"
#include "_CoreKeyPressHandler.h"
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

/* Forward declaration of the class Views::Frame */
#ifndef _ViewsFrame_
  EW_DECLARE_CLASS( ViewsFrame )
#define _ViewsFrame_
#endif

/* Forward declaration of the class Views::Image */
#ifndef _ViewsImage_
  EW_DECLARE_CLASS( ViewsImage )
#define _ViewsImage_
#endif

/* Forward declaration of the class Views::Text */
#ifndef _ViewsText_
  EW_DECLARE_CLASS( ViewsText )
#define _ViewsText_
#endif

/* Forward declaration of the class WidgetSet::RadioButton */
#ifndef _WidgetSetRadioButton_
  EW_DECLARE_CLASS( WidgetSetRadioButton )
#define _WidgetSetRadioButton_
#endif

/* Forward declaration of the class WidgetSet::RadioButtonConfig */
#ifndef _WidgetSetRadioButtonConfig_
  EW_DECLARE_CLASS( WidgetSetRadioButtonConfig )
#define _WidgetSetRadioButtonConfig_
#endif


/* This class implements a bistable 'radio button' widget. When the user taps the 
   button, the current state of the button represented by the boolean property @Selected 
   is set and signal is sent to the slot method stored in the property @OnSelect. 
   Once selected, the radio button remains in this state until the property @Selected 
   is set to the value 'false' (e.g. in reaction to another radio button selection). 
   By connecting further slot methods to the properties @OnPress, @OnRelease, @OnEnter 
   and @OnLeave you can react to other events triggered while the user interacts 
   with the button.
   Alternatively the property @Outlet can refer to any other 'int32' property the 
   widget should remain synchronized with. When the user selects the radio button, 
   the affected property is automatically set to the value @OutletSelector. On the 
   other hand, when the referred property is modified by another one, the radio 
   button is automatically notified to remain in sync with the property.
   This approach follows the Controller-View programming paradigm. Here the radio 
   button represents the 'View' and the property referred via 'Outlet' can be seen 
   as a part of the 'Controller'.
   During its lifetime the button remains always in one of the four states: 'disabled', 
   'default', 'focused' and 'active'. The state 'disabled' is true for every not 
   available button (the property @Enabled of the button is 'false'). Such buttons 
   will ignore all user inputs. The state 'default' determines a button, which is 
   ready to be touched by the user or ready to become focused. As soon as the button 
   becomes focused, it switches in the state 'focused'. In this state the user can 
   activate the button by pressing a key on the keyboard. Finally, the state 'active' 
   is true, if the user actually interacts with the button (the button is pressed).
   The exact look and feel of the radio button is determined by the 'Radio Button 
   Configuration' object assigned to the property @Appearance. The configuration 
   object provides bitmaps, colors, fonts and other configuration parameters needed 
   to construct and display the radio button. Usually, you will manage in your project 
   your own configuration objects and customize the radio buttons according to your 
   design expectations. Depending on the information provided in the associated 
   configuration object, the radio button will be composed of following views:
   - 'Face' is a bitmap frame view (Views::Frame) filling per default the entire 
   background of the button. In the configuration object you can individually specify 
   for every button state the desired bitmap, its opacity, frame number (if the 
   bitmap is multi-frame) and tint color (if the bitmap contains Alpha8 information 
   only). The button can automatically play animated bitmaps. If desired, you can 
   also configure the bitmap to be arranged horizontally and vertically instead 
   of filling the entire widget area.
   - 'Icon' is an image view (Views::Image) displayed per default in the center 
   of the button. The corresponding bitmap is determined in the property @Icon. 
   If the bitmap is multi-frame, the desired frame number can be selected by using 
   the property @IconFrame or individually for every button state by using the properties 
   @IconOffFrame, @IconOnFrame, @IconOffFrameActive, @IconOffFrameDefault, @IconOffFrameDisabled, 
   @IconOffFrameFocused, @IconOnFrameActive, @IconOnFrameDefault, @IconOnFrameDisabled 
   and @IconOnFrameFocused In the configuration object you can specify the alignment 
   and margins how to arrange the @Icon bitmap within the button area. Furthermore, 
   for every button state the opacity and tint color (if the @Icon bitmap contains 
   Alpha8 information only) can be determined. The button can automatically play 
   animated bitmaps.
   - 'Label' is a text view (Views::Text) displayed per default in the center of 
   the button. The corresponding text is determined in the property @Label or @LabelOff 
   and @LabelOn depending on the current state of the button (property @Selected). 
   In the configuration object you can specify the font, alignment and margins to 
   use for the text view. For every button state you can specify individual text 
   color values.
   In particular application cases you can instruct the button to automatically 
   resign and retarget the actual touch interaction to another touch handler (e.g. 
   another widget) after the user has performed a horizontal wipe gesture (@ResignAfterHorizontalWipe) 
   or vertical wipe gesture (@ResignAfterVerticalWipe). Using these properties several 
   widgets can cooperate during an active user interaction even if these handler 
   overlap each other.
   If the button is actually focused, it can also be activated by pressing the keyboard 
   key Core::KeyCode.Enter. In the configuration object you can specify another 
   key code, if desired. To prevent the button from being able to be focused, specify 
   in the configuration object the Core::KeyCode.NoKey as the code to activate the 
   button.
   For more details regarding the customization of the button see the description 
   of WidgetSet::RadioButtonConfig class. */
EW_DEFINE_FIELDS( WidgetSetRadioButton, CoreGroup )
  EW_VARIABLE( textView,        ViewsText )
  EW_VARIABLE( imageView,       ViewsImage )
  EW_VARIABLE( frameView,       ViewsFrame )
  EW_PROPERTY( Icon,            ResourcesBitmap )
  EW_PROPERTY( Appearance,      WidgetSetRadioButtonConfig )
  EW_PROPERTY( OnLeave,         XSlot )
  EW_PROPERTY( OnEnter,         XSlot )
  EW_PROPERTY( OnRelease,       XSlot )
  EW_PROPERTY( OnPress,         XSlot )
  EW_PROPERTY( OnSelect,        XSlot )
  EW_PROPERTY( Outlet,          XRef )
  EW_OBJECT  ( FlashTimer,      CoreTimer )
  EW_OBJECT  ( KeyHandler,      CoreKeyPressHandler )
  EW_OBJECT  ( TouchHandler,    CoreSimpleTouchHandler )
  EW_VARIABLE( onPressKeyTime,  XUInt32 )
  EW_PROPERTY( OutletSelector,  XInt32 )
  EW_PROPERTY( IconOnFrameActive, XInt32 )
  EW_PROPERTY( IconOnFrameFocused, XInt32 )
  EW_PROPERTY( IconOnFrameDisabled, XInt32 )
  EW_PROPERTY( IconOnFrameDefault, XInt32 )
  EW_PROPERTY( IconOffFrameActive, XInt32 )
  EW_PROPERTY( IconOffFrameFocused, XInt32 )
  EW_PROPERTY( IconOffFrameDisabled, XInt32 )
  EW_PROPERTY( IconOffFrameDefault, XInt32 )
  EW_PROPERTY( LabelOn,         XString )
  EW_PROPERTY( LabelOff,        XString )
  EW_PROPERTY( Selected,        XBool )
EW_END_OF_FIELDS( WidgetSetRadioButton )

/* Virtual Method Table (VMT) for the class : 'WidgetSet::RadioButton' */
EW_DEFINE_METHODS( WidgetSetRadioButton, CoreGroup )
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
  EW_METHOD( OnSetBounds,       void )( WidgetSetRadioButton _this, XRect value )
  EW_METHOD( drawContent,       void )( CoreGroup _this, GraphicsCanvas aCanvas, 
    XRect aClip, XPoint aOffset, XInt32 aOpacity, XBool aBlend )
  EW_METHOD( OnSetFocus,        void )( CoreGroup _this, CoreView value )
  EW_METHOD( OnSetBuffered,     void )( CoreGroup _this, XBool value )
  EW_METHOD( OnSetOpacity,      void )( CoreGroup _this, XInt32 value )
  EW_METHOD( IsDialog,          XBool )( CoreGroup _this, XBool aRecursive )
  EW_METHOD( DispatchEvent,     XObject )( CoreGroup _this, CoreEvent aEvent )
  EW_METHOD( BroadcastEvent,    XObject )( CoreGroup _this, CoreEvent aEvent, XSet 
    aFilter )
  EW_METHOD( UpdateLayout,      void )( CoreGroup _this, XPoint aSize )
  EW_METHOD( UpdateViewState,   void )( WidgetSetRadioButton _this, XSet aState )
  EW_METHOD( InvalidateArea,    void )( CoreGroup _this, XRect aArea )
  EW_METHOD( FindSiblingView,   CoreView )( CoreGroup _this, CoreView aView, XSet 
    aFilter )
  EW_METHOD( RestackBehind,     void )( CoreGroup _this, CoreView aView, CoreView 
    aSibling )
  EW_METHOD( RestackTop,        void )( CoreGroup _this, CoreView aView )
  EW_METHOD( Restack,           void )( CoreGroup _this, CoreView aView, XInt32 
    aOrder )
  EW_METHOD( Remove,            void )( CoreGroup _this, CoreView aView )
  EW_METHOD( Add,               void )( CoreGroup _this, CoreView aView, XInt32 
    aOrder )
EW_END_OF_METHODS( WidgetSetRadioButton )

/* 'C' function for method : 'WidgetSet::RadioButton.OnSetBounds()' */
void WidgetSetRadioButton_OnSetBounds( WidgetSetRadioButton _this, XRect value );

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
void WidgetSetRadioButton_UpdateViewState( WidgetSetRadioButton _this, XSet aState );

/* 'C' function for method : 'WidgetSet::RadioButton.onConfigChanged()' */
void WidgetSetRadioButton_onConfigChanged( WidgetSetRadioButton _this, XObject sender );

/* 'C' function for method : 'WidgetSet::RadioButton.onOutlet()' */
void WidgetSetRadioButton_onOutlet( WidgetSetRadioButton _this, XObject sender );

/* 'C' function for method : 'WidgetSet::RadioButton.onFlashTimer()' */
void WidgetSetRadioButton_onFlashTimer( WidgetSetRadioButton _this, XObject sender );

/* 'C' function for method : 'WidgetSet::RadioButton.onReleaseKey()' */
void WidgetSetRadioButton_onReleaseKey( WidgetSetRadioButton _this, XObject sender );

/* 'C' function for method : 'WidgetSet::RadioButton.onPressKey()' */
void WidgetSetRadioButton_onPressKey( WidgetSetRadioButton _this, XObject sender );

/* 'C' function for method : 'WidgetSet::RadioButton.onLeaveTouch()' */
void WidgetSetRadioButton_onLeaveTouch( WidgetSetRadioButton _this, XObject sender );

/* 'C' function for method : 'WidgetSet::RadioButton.onEnterTouch()' */
void WidgetSetRadioButton_onEnterTouch( WidgetSetRadioButton _this, XObject sender );

/* 'C' function for method : 'WidgetSet::RadioButton.onReleaseTouch()' */
void WidgetSetRadioButton_onReleaseTouch( WidgetSetRadioButton _this, XObject sender );

/* 'C' function for method : 'WidgetSet::RadioButton.onPressTouch()' */
void WidgetSetRadioButton_onPressTouch( WidgetSetRadioButton _this, XObject sender );

/* 'C' function for method : 'WidgetSet::RadioButton.OnSetOutletSelector()' */
void WidgetSetRadioButton_OnSetOutletSelector( WidgetSetRadioButton _this, XInt32 
  value );

/* 'C' function for method : 'WidgetSet::RadioButton.OnSetOutlet()' */
void WidgetSetRadioButton_OnSetOutlet( WidgetSetRadioButton _this, XRef value );

/* 'C' function for method : 'WidgetSet::RadioButton.OnSetSelected()' */
void WidgetSetRadioButton_OnSetSelected( WidgetSetRadioButton _this, XBool value );

/* 'C' function for method : 'WidgetSet::RadioButton.OnSetIconFrame()' */
void WidgetSetRadioButton_OnSetIconFrame( WidgetSetRadioButton _this, XInt32 value );

/* 'C' function for method : 'WidgetSet::RadioButton.OnSetIcon()' */
void WidgetSetRadioButton_OnSetIcon( WidgetSetRadioButton _this, ResourcesBitmap 
  value );

/* 'C' function for method : 'WidgetSet::RadioButton.OnSetAppearance()' */
void WidgetSetRadioButton_OnSetAppearance( WidgetSetRadioButton _this, WidgetSetRadioButtonConfig 
  value );

#ifdef __cplusplus
  }
#endif

#endif /* _WidgetSetRadioButton_H */

/* Embedded Wizard */
