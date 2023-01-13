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

#ifndef _WidgetSetRotaryKnobConfig_H
#define _WidgetSetRotaryKnobConfig_H

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

/* Forward declaration of the class WidgetSet::RotaryKnobConfig */
#ifndef _WidgetSetRotaryKnobConfig_
  EW_DECLARE_CLASS( WidgetSetRotaryKnobConfig )
#define _WidgetSetRotaryKnobConfig_
#endif


/* This class implements the functionality permitting you to simply customize the 
   look and feel of a 'rotary knob' widget (WidgetSet::RotaryKnob). In the practice, 
   you will create an instance of this class, configure its properties with bitmaps, 
   colors and other relevant attributes according to your design expectations and 
   assign such prepared configuration object to the property 'Appearance' of every 
   affected rotary knob widget. Thereupon the widgets will use the configuration 
   information provided in the object.
   During its lifetime the rotary knob remains always in one of the four states: 
   'disabled', 'default', 'focused' and 'active'. The state 'disabled' is true for 
   every not available rotary knob widget (the property 'Enabled' of the widget 
   is 'false'). Such widgets will ignore all user inputs. The state 'default' determines 
   a rotary knob widget, which is ready to be touched by the user or ready to become 
   focused. As soon as the rotary knob widget becomes focused, it switches in the 
   state 'focused'. In this state the user can control the widget by pressing keys 
   on the keyboard. Finally, the state 'active' is true, if the user actually interacts 
   with the widget (the thumb is pressed). With the configuration object you can 
   specify the appearance of the rotary knob for every state individually. For this 
   purpose you should know from which views the rotary knob is composed of:
   - 'Scale' is an image view (Views::Image) displayed centered in the background 
   within the widget area. In the configuration object you can individually specify 
   for every widget state the desired bitmap (@ScaleActive, @ScaleDefault, @ScaleDisabled, 
   @ScaleFocused), the frame number within the bitmap (@ScaleFrameActive, @ScaleFrameDefault, 
   @ScaleFrameDisabled, @ScaleFrameFocused) and opacity or color to tint the bitmap 
   (@ScaleTintActive, @ScaleTintDefault, @ScaleTintDisabled, @ScaleTintFocused). 
   The rotary knob widget can automatically play animated bitmaps if the specified 
   frame number is -1.
   - 'TrackLeft' is a vector graphic view (Views::FillPath) displaying a circle 
   segment between the thumb's leftmost position (clockwise) and its actual position. 
   From this results the effect of a track behind the thumb. You can individually 
   configured for every widget state the track radius (@TrackLeftRadiusActive, @TrackLeftRadiusDefault, 
   @TrackLeftRadiusDisabled and @TrackLeftRadiusFocused), its thickness (@TrackLeftThicknessActive, 
   @TrackLeftThicknessDefault, @TrackLeftThicknessDisabled and @TrackLeftThicknessFocused) 
   and the colors of the track (@TrackLeftColorActive, @TrackLeftColorDefault, @TrackLeftColorDisabled, 
   @TrackLeftColorFocused). Also the appearance of the track caps (@TrackLeftRoundedStart 
   and @TrackLeftRoundedEnd) can be configured.
   - 'TrackRight' is a vector graphic view (Views::FillPath) displaying a circle 
   segment between the thumb's actual position and its rightmost position (clockwise). 
   From this results the effect of a track behind the thumb. You can individually 
   configured for every widget state the track radius (@TrackRightRadiusActive, 
   @TrackRightRadiusDefault, @TrackRightRadiusDisabled and @TrackRightRadiusFocused), 
   its thickness (@TrackRightThicknessActive, @TrackRightThicknessDefault, @TrackRightThicknessDisabled 
   and @TrackRightThicknessFocused) and the colors of the track (@TrackRightColorActive, 
   @TrackRightColorDefault, @TrackRightColorDisabled, @TrackRightColorFocused). 
   Also the appearance of the track caps (@TrackRightRoundedStart and @TrackRightRoundedEnd) 
   can be configured.
   - 'Thumb' is a warp image view (Views::WarpImage) displayed and rotated around 
   predetermined pivot position along a circle segment with given radius. In the 
   configuration object you can individually specify for every widget state the 
   desired bitmap (@ThumbActive, @ThumbDefault, @ThumbDisabled, @ThumbFocused), 
   the frame number within the bitmap (@ThumbFrameActive, @ThumbFrameDefault, @ThumbFrameDisabled, 
   @ThumbFrameFocused) and opacity or color to tint the bitmap (@ThumbTintActive, 
   @ThumbTintDefault, @ThumbTintDisabled, @ThumbTintFocused). The desired rotation 
   range (the angles corresponding to 'MinValue' and 'MaxValue' specified in the 
   rotary knob) are determined in the properties @ThumbMinAngle and @ThumbMaxAngle. 
   The position around it the thumb is rotated is determined in the properties @CenterOffset, 
   @ThumbRadius and @ThumbPivot. With the property @ThumbRotate you control whether 
   the bitmap should appear rotated or only moved along the circle segment. The 
   rotary knob widget can automatically play animated bitmaps if the specified frame 
   number is -1.
   - 'Cover' is an image view (Views::Image) displayed centered within the widget 
   area and covering so eventually the scale and thumb images. In the configuration 
   object you can individually specify for every widget state the desired bitmap 
   (@CoverActive, @CoverDefault, @CoverDisabled, @CoverFocused), the frame number 
   within the bitmap (@CoverFrameActive, @CoverFrameDefault, @CoverFrameDisabled, 
   @CoverFrameFocused) and opacity or color to tint the bitmap (@CoverTintActive, 
   @CoverTintDefault, @CoverTintDisabled, @CoverTintFocused). The rotary knob widget 
   can automatically play animated bitmaps if the specified frame number is -1.
   If the rotary knob is actually focused, it can also be controlled by pressing 
   the keyboard keys specified in the properties @KeyCodeLeft and @KeyCodeRight 
   causing the thumb to be rotated counterclockwise or clockwise. To prevent the 
   rotary knob from being able to be focused, initialize these properties with the 
   value Core::KeyCode.NoKey. With the properties @KeyRepeatDelay and @KeyRepeatPeriod 
   you can configure whether the thumb should be moved autonomously when the user 
   holds the key pressed for a while.
   With the properties @WidgetMinSize and @WidgetMaxSize you can configure size 
   constraints for the widget itself. You can, for example, limit the rotary knob 
   to not shrink below a specified width or height. */
EW_DEFINE_FIELDS( WidgetSetRotaryKnobConfig, WidgetSetWidgetConfig )
  EW_PROPERTY( CoverActive,     ResourcesBitmap )
  EW_PROPERTY( CoverFocused,    ResourcesBitmap )
  EW_PROPERTY( CoverDisabled,   ResourcesBitmap )
  EW_PROPERTY( CoverDefault,    ResourcesBitmap )
  EW_PROPERTY( ThumbActive,     ResourcesBitmap )
  EW_PROPERTY( ThumbFocused,    ResourcesBitmap )
  EW_PROPERTY( ThumbDisabled,   ResourcesBitmap )
  EW_PROPERTY( ThumbDefault,    ResourcesBitmap )
  EW_PROPERTY( ScaleActive,     ResourcesBitmap )
  EW_PROPERTY( ScaleFocused,    ResourcesBitmap )
  EW_PROPERTY( ScaleDisabled,   ResourcesBitmap )
  EW_PROPERTY( ScaleDefault,    ResourcesBitmap )
  EW_PROPERTY( KeyRepeatPeriod, XInt32 )
  EW_PROPERTY( KeyRepeatDelay,  XInt32 )
  EW_PROPERTY( TrackRightColorActive, XColor )
  EW_PROPERTY( TrackRightColorFocused, XColor )
  EW_PROPERTY( TrackRightColorDisabled, XColor )
  EW_PROPERTY( TrackRightColorDefault, XColor )
  EW_PROPERTY( TrackRightThicknessActive, XFloat )
  EW_PROPERTY( TrackRightThicknessFocused, XFloat )
  EW_PROPERTY( TrackRightThicknessDisabled, XFloat )
  EW_PROPERTY( TrackRightThicknessDefault, XFloat )
  EW_PROPERTY( TrackRightRadiusActive, XFloat )
  EW_PROPERTY( TrackRightRadiusFocused, XFloat )
  EW_PROPERTY( TrackRightRadiusDisabled, XFloat )
  EW_PROPERTY( TrackRightRadiusDefault, XFloat )
  EW_PROPERTY( TrackLeftColorActive, XColor )
  EW_PROPERTY( TrackLeftColorFocused, XColor )
  EW_PROPERTY( TrackLeftColorDisabled, XColor )
  EW_PROPERTY( TrackLeftColorDefault, XColor )
  EW_PROPERTY( TrackLeftThicknessActive, XFloat )
  EW_PROPERTY( TrackLeftThicknessFocused, XFloat )
  EW_PROPERTY( TrackLeftThicknessDisabled, XFloat )
  EW_PROPERTY( TrackLeftThicknessDefault, XFloat )
  EW_PROPERTY( TrackLeftRadiusActive, XFloat )
  EW_PROPERTY( TrackLeftRadiusFocused, XFloat )
  EW_PROPERTY( TrackLeftRadiusDisabled, XFloat )
  EW_PROPERTY( TrackLeftRadiusDefault, XFloat )
  EW_PROPERTY( CoverTintActive, XColor )
  EW_PROPERTY( CoverTintFocused, XColor )
  EW_PROPERTY( CoverTintDisabled, XColor )
  EW_PROPERTY( CoverTintDefault, XColor )
  EW_PROPERTY( CoverFrameActive, XInt32 )
  EW_PROPERTY( CoverFrameFocused, XInt32 )
  EW_PROPERTY( CoverFrameDisabled, XInt32 )
  EW_PROPERTY( CoverFrameDefault, XInt32 )
  EW_PROPERTY( ThumbMaxAngle,   XFloat )
  EW_PROPERTY( ThumbMinAngle,   XFloat )
  EW_PROPERTY( ThumbRadius,     XFloat )
  EW_PROPERTY( ThumbPivot,      XPoint )
  EW_PROPERTY( CenterOffset,    XPoint )
  EW_PROPERTY( ThumbTintActive, XColor )
  EW_PROPERTY( ThumbTintFocused, XColor )
  EW_PROPERTY( ThumbTintDisabled, XColor )
  EW_PROPERTY( ThumbTintDefault, XColor )
  EW_PROPERTY( ThumbFrameActive, XInt32 )
  EW_PROPERTY( ThumbFrameFocused, XInt32 )
  EW_PROPERTY( ThumbFrameDisabled, XInt32 )
  EW_PROPERTY( ThumbFrameDefault, XInt32 )
  EW_PROPERTY( ScaleTintActive, XColor )
  EW_PROPERTY( ScaleTintFocused, XColor )
  EW_PROPERTY( ScaleTintDisabled, XColor )
  EW_PROPERTY( ScaleTintDefault, XColor )
  EW_PROPERTY( ScaleFrameActive, XInt32 )
  EW_PROPERTY( ScaleFrameFocused, XInt32 )
  EW_PROPERTY( ScaleFrameDisabled, XInt32 )
  EW_PROPERTY( ScaleFrameDefault, XInt32 )
  EW_PROPERTY( WidgetMaxSize,   XPoint )
  EW_PROPERTY( WidgetMinSize,   XPoint )
  EW_PROPERTY( KeyCodeLeft,     XEnum )
  EW_PROPERTY( KeyCodeRight,    XEnum )
  EW_PROPERTY( ThumbRotate,     XBool )
  EW_PROPERTY( TrackLeftRoundedStart, XBool )
  EW_PROPERTY( TrackLeftRoundedEnd, XBool )
  EW_PROPERTY( TrackRightRoundedStart, XBool )
  EW_PROPERTY( TrackRightRoundedEnd, XBool )
EW_END_OF_FIELDS( WidgetSetRotaryKnobConfig )

/* Virtual Method Table (VMT) for the class : 'WidgetSet::RotaryKnobConfig' */
EW_DEFINE_METHODS( WidgetSetRotaryKnobConfig, WidgetSetWidgetConfig )
EW_END_OF_METHODS( WidgetSetRotaryKnobConfig )

/* 'C' function for method : 'WidgetSet::RotaryKnobConfig.OnSetThumbMaxAngle()' */
void WidgetSetRotaryKnobConfig_OnSetThumbMaxAngle( WidgetSetRotaryKnobConfig _this, 
  XFloat value );

/* 'C' function for method : 'WidgetSet::RotaryKnobConfig.OnSetThumbMinAngle()' */
void WidgetSetRotaryKnobConfig_OnSetThumbMinAngle( WidgetSetRotaryKnobConfig _this, 
  XFloat value );

/* 'C' function for method : 'WidgetSet::RotaryKnobConfig.OnSetThumbPivot()' */
void WidgetSetRotaryKnobConfig_OnSetThumbPivot( WidgetSetRotaryKnobConfig _this, 
  XPoint value );

/* 'C' function for method : 'WidgetSet::RotaryKnobConfig.OnSetThumbActive()' */
void WidgetSetRotaryKnobConfig_OnSetThumbActive( WidgetSetRotaryKnobConfig _this, 
  ResourcesBitmap value );

/* 'C' function for method : 'WidgetSet::RotaryKnobConfig.OnSetThumbFocused()' */
void WidgetSetRotaryKnobConfig_OnSetThumbFocused( WidgetSetRotaryKnobConfig _this, 
  ResourcesBitmap value );

/* 'C' function for method : 'WidgetSet::RotaryKnobConfig.OnSetThumbDefault()' */
void WidgetSetRotaryKnobConfig_OnSetThumbDefault( WidgetSetRotaryKnobConfig _this, 
  ResourcesBitmap value );

/* 'C' function for method : 'WidgetSet::RotaryKnobConfig.OnSetScaleActive()' */
void WidgetSetRotaryKnobConfig_OnSetScaleActive( WidgetSetRotaryKnobConfig _this, 
  ResourcesBitmap value );

/* 'C' function for method : 'WidgetSet::RotaryKnobConfig.OnSetScaleFocused()' */
void WidgetSetRotaryKnobConfig_OnSetScaleFocused( WidgetSetRotaryKnobConfig _this, 
  ResourcesBitmap value );

/* 'C' function for method : 'WidgetSet::RotaryKnobConfig.OnSetScaleDisabled()' */
void WidgetSetRotaryKnobConfig_OnSetScaleDisabled( WidgetSetRotaryKnobConfig _this, 
  ResourcesBitmap value );

/* 'C' function for method : 'WidgetSet::RotaryKnobConfig.OnSetScaleDefault()' */
void WidgetSetRotaryKnobConfig_OnSetScaleDefault( WidgetSetRotaryKnobConfig _this, 
  ResourcesBitmap value );

/* 'C' function for method : 'WidgetSet::RotaryKnobConfig.OnSetWidgetMaxSize()' */
void WidgetSetRotaryKnobConfig_OnSetWidgetMaxSize( WidgetSetRotaryKnobConfig _this, 
  XPoint value );

/* 'C' function for method : 'WidgetSet::RotaryKnobConfig.OnSetWidgetMinSize()' */
void WidgetSetRotaryKnobConfig_OnSetWidgetMinSize( WidgetSetRotaryKnobConfig _this, 
  XPoint value );

#ifdef __cplusplus
  }
#endif

#endif /* _WidgetSetRotaryKnobConfig_H */

/* Embedded Wizard */
