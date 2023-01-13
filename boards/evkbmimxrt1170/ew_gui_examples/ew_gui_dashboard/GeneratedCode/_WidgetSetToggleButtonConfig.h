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

#ifndef _WidgetSetToggleButtonConfig_H
#define _WidgetSetToggleButtonConfig_H

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

#include "_WidgetSetWidgetConfig.h"

/* Forward declaration of the class Resources::Bitmap */
#ifndef _ResourcesBitmap_
  EW_DECLARE_CLASS( ResourcesBitmap )
#define _ResourcesBitmap_
#endif

/* Forward declaration of the class Resources::Font */
#ifndef _ResourcesFont_
  EW_DECLARE_CLASS( ResourcesFont )
#define _ResourcesFont_
#endif

/* Forward declaration of the class WidgetSet::ToggleButtonConfig */
#ifndef _WidgetSetToggleButtonConfig_
  EW_DECLARE_CLASS( WidgetSetToggleButtonConfig )
#define _WidgetSetToggleButtonConfig_
#endif


/* This class implements the functionality permitting you to simply customize the 
   look and feel of a bistable 'toggle button' widget (WidgetSet::ToggleButton). 
   In the practice, you will create an instance of this class, configure its properties 
   with bitmaps, colors and other relevant attributes according to your design expectations 
   and assign such prepared configuration object to the property 'Appearance' of 
   every affected toggle button widget. Thereupon the widgets will use the configuration 
   information provided in the object.
   During its lifetime the button remains always in one of the four states: 'disabled', 
   'default', 'focused' and 'active'. The state 'disabled' is true for every not 
   available button (the property 'Enabled' of the button is 'false'). Such buttons 
   will ignore all user inputs. The state 'default' determines a button, which is 
   ready to be touched by the user or ready to become focused. As soon as the button 
   becomes focused, it switches in the state 'focused'. In this state the user can 
   activate the button by pressing a key on the keyboard. Finally, the state 'active' 
   is true, if the user actually interacts with the button (the button is pressed). 
   At the same time, the toggle button distinguishes between two further states: 
   'on' (checked) and 'off' (unchecked). With the configuration object you can specify 
   the appearance of the button for every state combination individually. For this 
   purpose you should know from which views the button is composed of:
   - 'Face' is a bitmap frame view (Views::Frame) filling per default the entire 
   background of the button. In the configuration object you can individually specify 
   for every button state combination the desired bitmap (@FaceOffActive, @FaceOffDefault, 
   @FaceOffDisabled, @FaceOffFocused, @FaceOnActive, @FaceOnDefault, @FaceOnDisabled, 
   @FaceOnFocused), the frame number within the bitmap (@FaceOffFrameActive, @FaceOffFrameDefault, 
   @FaceOffFrameDisabled, @FaceOffFrameFocused, @FaceOnFrameActive, @FaceOnFrameDefault, 
   @FaceOnFrameDisabled, @FaceOnFrameFocused) and opacity or color to tint the bitmap 
   (@FaceOffTintActive, @FaceOffTintDefault, @FaceOffTintDisabled, @FaceOffTintFocused, 
   @FaceOnTintActive, @FaceOnTintDefault, @FaceOnTintDisabled, @FaceOnTintFocused). 
   The button can automatically play animated bitmaps if the corresponding frame 
   number is -1. If desired, with the property @FaceLayout you can also configure 
   the bitmap to be arranged horizontally and vertically instead of filling the 
   entire widget area.
   - 'Icon' is an image view (Views::Image) displayed per default in the center 
   of the button. The corresponding bitmap is determined in the property 'Icon' 
   in the toggle button widget. In the configuration object you can specify the 
   alignment (@IconAlignment) and margins (@IconMarginBottom, @IconMarginLeft, @IconMarginRight, 
   @IconMarginTop) how to arrange the 'Icon' bitmap within the button area. Furthermore, 
   for every button state combination the opacity or color to tint the bitmap can 
   be determined (@IconOffTintActive, @IconOffTintDefault, @IconOffTintDisabled, 
   @IconOffTintFocused, @IconOnTintActive, @IconOnTintDefault, @IconOnTintDisabled, 
   @IconOnTintFocused).
   - 'Label' is a text view (Views::Text) displayed per default in the center of 
   the button. The corresponding text is determined in the property 'LabelOff' or 
   'LabelOn' in the toggle button widget. In the configuration object you can specify 
   the font (@LabelOffFont, @LabelOnFont), alignment (@LabelAlignment) and margins 
   (@LabelMarginBottom, @LabelMarginLeft, @LabelMarginRight, @LabelMarginTop) to 
   use for the text view. For every button state combination you can specify individual 
   text color values (@LabelOffColorActive, @LabelOffColorDefault, @LabelOffColorDisabled, 
   @LabelOffColorFocused, @LabelOnColorActive, @LabelOnColorDefault, @LabelOnColorDisabled, 
   @LabelOnColorFocused).
   If the button is actually focused, it can also be activated by pressing the keyboard 
   key specified in the property @KeyCode. To prevent the button from being able 
   to be focused, initialize this property with the value Core::KeyCode.NoKey. The 
   property @PressedFeedbackDuration configures the duration how long the button 
   should appear active (pressed) even if it has been taped for a very short time.
   With the properties @WidgetMinSize and @WidgetMaxSize you can configure size 
   constraints for the widget itself. You can, for example, limit the button to 
   not shrink below a specified width or height. */
EW_DEFINE_FIELDS( WidgetSetToggleButtonConfig, WidgetSetWidgetConfig )
  EW_PROPERTY( LabelOnFont,     ResourcesFont )
  EW_PROPERTY( LabelOffFont,    ResourcesFont )
  EW_PROPERTY( FaceOnActive,    ResourcesBitmap )
  EW_PROPERTY( FaceOnFocused,   ResourcesBitmap )
  EW_PROPERTY( FaceOnDisabled,  ResourcesBitmap )
  EW_PROPERTY( FaceOnDefault,   ResourcesBitmap )
  EW_PROPERTY( FaceOffActive,   ResourcesBitmap )
  EW_PROPERTY( FaceOffFocused,  ResourcesBitmap )
  EW_PROPERTY( FaceOffDisabled, ResourcesBitmap )
  EW_PROPERTY( FaceOffDefault,  ResourcesBitmap )
  EW_PROPERTY( PressedFeedbackDuration, XInt32 )
  EW_PROPERTY( LabelOnColorActive, XColor )
  EW_PROPERTY( LabelOnColorFocused, XColor )
  EW_PROPERTY( LabelOnColorDisabled, XColor )
  EW_PROPERTY( LabelOnColorDefault, XColor )
  EW_PROPERTY( LabelOffColorActive, XColor )
  EW_PROPERTY( LabelOffColorFocused, XColor )
  EW_PROPERTY( LabelOffColorDisabled, XColor )
  EW_PROPERTY( LabelOffColorDefault, XColor )
  EW_PROPERTY( LabelMarginBottom, XInt32 )
  EW_PROPERTY( LabelMarginTop,  XInt32 )
  EW_PROPERTY( LabelMarginRight, XInt32 )
  EW_PROPERTY( LabelMarginLeft, XInt32 )
  EW_PROPERTY( IconOnTintActive, XColor )
  EW_PROPERTY( IconOnTintFocused, XColor )
  EW_PROPERTY( IconOnTintDisabled, XColor )
  EW_PROPERTY( IconOnTintDefault, XColor )
  EW_PROPERTY( IconOffTintActive, XColor )
  EW_PROPERTY( IconOffTintFocused, XColor )
  EW_PROPERTY( IconOffTintDisabled, XColor )
  EW_PROPERTY( IconOffTintDefault, XColor )
  EW_PROPERTY( IconMarginBottom, XInt32 )
  EW_PROPERTY( IconMarginTop,   XInt32 )
  EW_PROPERTY( IconMarginRight, XInt32 )
  EW_PROPERTY( IconMarginLeft,  XInt32 )
  EW_PROPERTY( LabelAlignment,  XSet )
  EW_PROPERTY( IconAlignment,   XSet )
  EW_PROPERTY( FaceLayout,      XSet )
  EW_PROPERTY( FaceOnTintActive, XColor )
  EW_PROPERTY( FaceOnTintFocused, XColor )
  EW_PROPERTY( FaceOnTintDisabled, XColor )
  EW_PROPERTY( FaceOnTintDefault, XColor )
  EW_PROPERTY( FaceOnFrameActive, XInt32 )
  EW_PROPERTY( FaceOnFrameFocused, XInt32 )
  EW_PROPERTY( FaceOnFrameDisabled, XInt32 )
  EW_PROPERTY( FaceOnFrameDefault, XInt32 )
  EW_PROPERTY( FaceOffTintActive, XColor )
  EW_PROPERTY( FaceOffTintFocused, XColor )
  EW_PROPERTY( FaceOffTintDisabled, XColor )
  EW_PROPERTY( FaceOffTintDefault, XColor )
  EW_PROPERTY( FaceOffFrameActive, XInt32 )
  EW_PROPERTY( FaceOffFrameFocused, XInt32 )
  EW_PROPERTY( FaceOffFrameDisabled, XInt32 )
  EW_PROPERTY( FaceOffFrameDefault, XInt32 )
  EW_PROPERTY( WidgetMaxSize,   XPoint )
  EW_PROPERTY( WidgetMinSize,   XPoint )
  EW_PROPERTY( KeyCode,         XEnum )
EW_END_OF_FIELDS( WidgetSetToggleButtonConfig )

/* Virtual Method Table (VMT) for the class : 'WidgetSet::ToggleButtonConfig' */
EW_DEFINE_METHODS( WidgetSetToggleButtonConfig, WidgetSetWidgetConfig )
EW_END_OF_METHODS( WidgetSetToggleButtonConfig )

/* 'C' function for method : 'WidgetSet::ToggleButtonConfig.OnSetIconOnTintActive()' */
void WidgetSetToggleButtonConfig_OnSetIconOnTintActive( WidgetSetToggleButtonConfig _this, 
  XColor value );

/* 'C' function for method : 'WidgetSet::ToggleButtonConfig.OnSetIconOnTintFocused()' */
void WidgetSetToggleButtonConfig_OnSetIconOnTintFocused( WidgetSetToggleButtonConfig _this, 
  XColor value );

/* 'C' function for method : 'WidgetSet::ToggleButtonConfig.OnSetIconOnTintDisabled()' */
void WidgetSetToggleButtonConfig_OnSetIconOnTintDisabled( WidgetSetToggleButtonConfig _this, 
  XColor value );

/* 'C' function for method : 'WidgetSet::ToggleButtonConfig.OnSetIconOnTintDefault()' */
void WidgetSetToggleButtonConfig_OnSetIconOnTintDefault( WidgetSetToggleButtonConfig _this, 
  XColor value );

/* 'C' function for method : 'WidgetSet::ToggleButtonConfig.OnSetIconOffTintActive()' */
void WidgetSetToggleButtonConfig_OnSetIconOffTintActive( WidgetSetToggleButtonConfig _this, 
  XColor value );

/* 'C' function for method : 'WidgetSet::ToggleButtonConfig.OnSetIconOffTintFocused()' */
void WidgetSetToggleButtonConfig_OnSetIconOffTintFocused( WidgetSetToggleButtonConfig _this, 
  XColor value );

/* 'C' function for method : 'WidgetSet::ToggleButtonConfig.OnSetIconOffTintDisabled()' */
void WidgetSetToggleButtonConfig_OnSetIconOffTintDisabled( WidgetSetToggleButtonConfig _this, 
  XColor value );

/* 'C' function for method : 'WidgetSet::ToggleButtonConfig.OnSetIconOffTintDefault()' */
void WidgetSetToggleButtonConfig_OnSetIconOffTintDefault( WidgetSetToggleButtonConfig _this, 
  XColor value );

#ifdef __cplusplus
  }
#endif

#endif /* _WidgetSetToggleButtonConfig_H */

/* Embedded Wizard */
