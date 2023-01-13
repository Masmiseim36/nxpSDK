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

#ifndef _WidgetSetHorizontalSliderConfig_H
#define _WidgetSetHorizontalSliderConfig_H

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

/* Forward declaration of the class WidgetSet::HorizontalSliderConfig */
#ifndef _WidgetSetHorizontalSliderConfig_
  EW_DECLARE_CLASS( WidgetSetHorizontalSliderConfig )
#define _WidgetSetHorizontalSliderConfig_
#endif


/* This class implements the functionality permitting you to simply customize the 
   look and feel of a 'horizontal slider' widget (WidgetSet::HorizontalSlider). 
   In the practice, you will create an instance of this class, configure its properties 
   with bitmaps, colors and other relevant attributes according to your design expectations 
   and assign such prepared configuration object to the property 'Appearance' of 
   every affected horizontal slider widget. Thereupon the widgets will use the configuration 
   information provided in the object.
   During its lifetime the slider remains always in one of the four states: 'disabled', 
   'default', 'focused' and 'active'. The state 'disabled' is true for every not 
   available slider (the property 'Enabled' of the slider is 'false'). Such sliders 
   will ignore all user inputs. The state 'default' determines a slider, which is 
   ready to be touched by the user or ready to become focused. As soon as the slider 
   becomes focused, it switches in the state 'focused'. In this state the user can 
   control the slider by pressing keys on the keyboard. Finally, the state 'active' 
   is true, if the user actually interacts with the slider (the slider's thumb is 
   pressed). With the configuration object you can specify the appearance of the 
   slider for every state individually. For this purpose you should know from which 
   views the slider is composed of:
   - 'Face' is a bitmap frame view (Views::Frame) filling vertically centered the 
   entire width in the background of the slider. In the configuration object you 
   can individually specify for every slider state the desired bitmap (@FaceActive, 
   @FaceDefault, @FaceDisabled, @FaceFocused), the frame number within the bitmap 
   (@FaceFrameActive, @FaceFrameDefault, @FaceFrameDisabled, @FaceFrameFocused) 
   and opacity or color to tint the bitmap (@FaceTintActive, @FaceTintDefault, @FaceTintDisabled, 
   @FaceTintFocused). The slider can automatically play animated bitmaps if the 
   specified frame number is -1.
   - 'TrackLeft' is a bitmap frame view (Views::Frame) filling vertically centered 
   the background of the slider between its left edge and the actual position of 
   the thumb. In the configuration object you can individually specify for every 
   slider state the desired bitmap (@TrackLeftActive, @TrackLeftDefault, @TrackLeftDisabled, 
   @TrackLeftFocused), the frame number within the bitmap (@TrackLeftFrameActive, 
   @TrackLeftFrameDefault, @TrackLeftFrameDisabled, @TrackLeftFrameFocused) and 
   opacity or color to tint the bitmap (@TrackLeftTintActive, @TrackLeftTintDefault, 
   @TrackLeftTintDisabled, @TrackLeftTintFocused). With the property @TrackLeftWithEdge 
   you can configure how the right edge of the track (the edge at the thumb position) 
   should appear. The slider can automatically play animated bitmaps if the specified 
   frame number is -1.
   - 'TrackRight' is a bitmap frame view (Views::Frame) filling vertically centered 
   the background of the slider between the actual position of the thumb and the 
   right edge of the slider widget. In the configuration object you can individually 
   specify for every slider state the desired bitmap (@TrackRightActive, @TrackRightDefault, 
   @TrackRightDisabled, @TrackRightFocused), the frame number within the bitmap 
   (@TrackRightFrameActive, @TrackRightFrameDefault, @TrackRightFrameDisabled, @TrackRightFrameFocused) 
   and opacity or color to tint the bitmap (@TrackRightTintActive, @TrackRightTintDefault, 
   @TrackRightTintDisabled, @TrackRightTintFocused). With the property @TrackRightWithEdge 
   you can configure how the left edge of the track (the edge at the thumb position) 
   should appear. The slider can automatically play animated bitmaps if the specified 
   frame number is -1.
   - 'Thumb' is an image view (Views::Image) displayed vertically centered at the 
   thumb position according to current value of the slider. In the configuration 
   object you can individually specify for every slider state the desired bitmap 
   (@ThumbActive, @ThumbDefault, @ThumbDisabled, @ThumbFocused), the frame number 
   within the bitmap (@ThumbFrameActive, @ThumbFrameDefault, @ThumbFrameDisabled, 
   @ThumbFrameFocused) and opacity or color to tint the bitmap (@ThumbTintActive, 
   @ThumbTintDefault, @ThumbTintDisabled, @ThumbTintFocused). If necessary, additional 
   margins on the left and on the right of the thumb can be specified by using the 
   properties @ThumbMarginLeft and @ThumbMarginRight. The slider can automatically 
   play animated bitmaps if the specified frame number is -1.
   - 'Cover' is a bitmap frame view (Views::Frame) filling vertically centered the 
   entire width of the slider and covering so eventually the thumb and track. In 
   the configuration object you can individually specify for every slider state 
   the desired bitmap (@CoverActive, @CoverDefault, @CoverDisabled, @CoverFocused), 
   the frame number within the bitmap (@CoverFrameActive, @CoverFrameDefault, @CoverFrameDisabled, 
   @CoverFrameFocused) and opacity or color to tint the bitmap (@CoverTintActive, 
   @CoverTintDefault, @CoverTintDisabled, @CoverTintFocused). The slider can automatically 
   play animated bitmaps if the specified frame number is -1.
   If the slider is actually focused, it can also be controlled by pressing the 
   keyboard keys specified in the properties @KeyCodeLeft and @KeyCodeRight causing 
   the slider's thumb to be moved left or right. To prevent the slider from being 
   able to be focused, initialize these properties with the value Core::KeyCode.NoKey. 
   With the properties @KeyRepeatDelay and @KeyRepeatPeriod you can configure whether 
   the thumb should be moved autonomously when the user holds the key pressed for 
   a while.
   With the properties @WidgetMinSize and @WidgetMaxSize you can configure size 
   constraints for the widget itself. You can, for example, limit the slider to 
   not shrink below a specified width or height. */
EW_DEFINE_FIELDS( WidgetSetHorizontalSliderConfig, WidgetSetWidgetConfig )
  EW_PROPERTY( CoverActive,     ResourcesBitmap )
  EW_PROPERTY( CoverFocused,    ResourcesBitmap )
  EW_PROPERTY( CoverDisabled,   ResourcesBitmap )
  EW_PROPERTY( CoverDefault,    ResourcesBitmap )
  EW_PROPERTY( ThumbActive,     ResourcesBitmap )
  EW_PROPERTY( ThumbFocused,    ResourcesBitmap )
  EW_PROPERTY( ThumbDisabled,   ResourcesBitmap )
  EW_PROPERTY( ThumbDefault,    ResourcesBitmap )
  EW_PROPERTY( TrackRightActive, ResourcesBitmap )
  EW_PROPERTY( TrackRightFocused, ResourcesBitmap )
  EW_PROPERTY( TrackRightDisabled, ResourcesBitmap )
  EW_PROPERTY( TrackRightDefault, ResourcesBitmap )
  EW_PROPERTY( TrackLeftActive, ResourcesBitmap )
  EW_PROPERTY( TrackLeftFocused, ResourcesBitmap )
  EW_PROPERTY( TrackLeftDisabled, ResourcesBitmap )
  EW_PROPERTY( TrackLeftDefault, ResourcesBitmap )
  EW_PROPERTY( FaceActive,      ResourcesBitmap )
  EW_PROPERTY( FaceFocused,     ResourcesBitmap )
  EW_PROPERTY( FaceDisabled,    ResourcesBitmap )
  EW_PROPERTY( FaceDefault,     ResourcesBitmap )
  EW_PROPERTY( KeyRepeatPeriod, XInt32 )
  EW_PROPERTY( KeyRepeatDelay,  XInt32 )
  EW_PROPERTY( CoverTintActive, XColor )
  EW_PROPERTY( CoverTintFocused, XColor )
  EW_PROPERTY( CoverTintDisabled, XColor )
  EW_PROPERTY( CoverTintDefault, XColor )
  EW_PROPERTY( CoverFrameActive, XInt32 )
  EW_PROPERTY( CoverFrameFocused, XInt32 )
  EW_PROPERTY( CoverFrameDisabled, XInt32 )
  EW_PROPERTY( CoverFrameDefault, XInt32 )
  EW_PROPERTY( ThumbMarginRight, XInt32 )
  EW_PROPERTY( ThumbMarginLeft, XInt32 )
  EW_PROPERTY( ThumbTintActive, XColor )
  EW_PROPERTY( ThumbTintFocused, XColor )
  EW_PROPERTY( ThumbTintDisabled, XColor )
  EW_PROPERTY( ThumbTintDefault, XColor )
  EW_PROPERTY( ThumbFrameActive, XInt32 )
  EW_PROPERTY( ThumbFrameFocused, XInt32 )
  EW_PROPERTY( ThumbFrameDisabled, XInt32 )
  EW_PROPERTY( ThumbFrameDefault, XInt32 )
  EW_PROPERTY( TrackRightTintActive, XColor )
  EW_PROPERTY( TrackRightTintFocused, XColor )
  EW_PROPERTY( TrackRightTintDisabled, XColor )
  EW_PROPERTY( TrackRightTintDefault, XColor )
  EW_PROPERTY( TrackRightFrameActive, XInt32 )
  EW_PROPERTY( TrackRightFrameFocused, XInt32 )
  EW_PROPERTY( TrackRightFrameDisabled, XInt32 )
  EW_PROPERTY( TrackRightFrameDefault, XInt32 )
  EW_PROPERTY( TrackLeftTintActive, XColor )
  EW_PROPERTY( TrackLeftTintFocused, XColor )
  EW_PROPERTY( TrackLeftTintDisabled, XColor )
  EW_PROPERTY( TrackLeftTintDefault, XColor )
  EW_PROPERTY( TrackLeftFrameActive, XInt32 )
  EW_PROPERTY( TrackLeftFrameFocused, XInt32 )
  EW_PROPERTY( TrackLeftFrameDisabled, XInt32 )
  EW_PROPERTY( TrackLeftFrameDefault, XInt32 )
  EW_PROPERTY( FaceTintActive,  XColor )
  EW_PROPERTY( FaceTintFocused, XColor )
  EW_PROPERTY( FaceTintDisabled, XColor )
  EW_PROPERTY( FaceTintDefault, XColor )
  EW_PROPERTY( FaceFrameActive, XInt32 )
  EW_PROPERTY( FaceFrameFocused, XInt32 )
  EW_PROPERTY( FaceFrameDisabled, XInt32 )
  EW_PROPERTY( FaceFrameDefault, XInt32 )
  EW_PROPERTY( WidgetMaxSize,   XPoint )
  EW_PROPERTY( WidgetMinSize,   XPoint )
  EW_PROPERTY( KeyCodeLeft,     XEnum )
  EW_PROPERTY( KeyCodeRight,    XEnum )
  EW_PROPERTY( TrackLeftWithEdge, XBool )
  EW_PROPERTY( TrackRightWithEdge, XBool )
  EW_PROPERTY( ThumbUpdateAtTouch, XBool )
EW_END_OF_FIELDS( WidgetSetHorizontalSliderConfig )

/* Virtual Method Table (VMT) for the class : 'WidgetSet::HorizontalSliderConfig' */
EW_DEFINE_METHODS( WidgetSetHorizontalSliderConfig, WidgetSetWidgetConfig )
EW_END_OF_METHODS( WidgetSetHorizontalSliderConfig )

/* 'C' function for method : 'WidgetSet::HorizontalSliderConfig.OnSetThumbActive()' */
void WidgetSetHorizontalSliderConfig_OnSetThumbActive( WidgetSetHorizontalSliderConfig _this, 
  ResourcesBitmap value );

/* 'C' function for method : 'WidgetSet::HorizontalSliderConfig.OnSetThumbFocused()' */
void WidgetSetHorizontalSliderConfig_OnSetThumbFocused( WidgetSetHorizontalSliderConfig _this, 
  ResourcesBitmap value );

/* 'C' function for method : 'WidgetSet::HorizontalSliderConfig.OnSetThumbDefault()' */
void WidgetSetHorizontalSliderConfig_OnSetThumbDefault( WidgetSetHorizontalSliderConfig _this, 
  ResourcesBitmap value );

/* 'C' function for method : 'WidgetSet::HorizontalSliderConfig.OnSetTrackRightFrameActive()' */
void WidgetSetHorizontalSliderConfig_OnSetTrackRightFrameActive( WidgetSetHorizontalSliderConfig _this, 
  XInt32 value );

/* 'C' function for method : 'WidgetSet::HorizontalSliderConfig.OnSetTrackRightFrameFocused()' */
void WidgetSetHorizontalSliderConfig_OnSetTrackRightFrameFocused( WidgetSetHorizontalSliderConfig _this, 
  XInt32 value );

/* 'C' function for method : 'WidgetSet::HorizontalSliderConfig.OnSetTrackRightFrameDisabled()' */
void WidgetSetHorizontalSliderConfig_OnSetTrackRightFrameDisabled( WidgetSetHorizontalSliderConfig _this, 
  XInt32 value );

/* 'C' function for method : 'WidgetSet::HorizontalSliderConfig.OnSetTrackRightFrameDefault()' */
void WidgetSetHorizontalSliderConfig_OnSetTrackRightFrameDefault( WidgetSetHorizontalSliderConfig _this, 
  XInt32 value );

/* 'C' function for method : 'WidgetSet::HorizontalSliderConfig.OnSetTrackRightActive()' */
void WidgetSetHorizontalSliderConfig_OnSetTrackRightActive( WidgetSetHorizontalSliderConfig _this, 
  ResourcesBitmap value );

/* 'C' function for method : 'WidgetSet::HorizontalSliderConfig.OnSetTrackRightFocused()' */
void WidgetSetHorizontalSliderConfig_OnSetTrackRightFocused( WidgetSetHorizontalSliderConfig _this, 
  ResourcesBitmap value );

/* 'C' function for method : 'WidgetSet::HorizontalSliderConfig.OnSetTrackRightDisabled()' */
void WidgetSetHorizontalSliderConfig_OnSetTrackRightDisabled( WidgetSetHorizontalSliderConfig _this, 
  ResourcesBitmap value );

/* 'C' function for method : 'WidgetSet::HorizontalSliderConfig.OnSetTrackRightDefault()' */
void WidgetSetHorizontalSliderConfig_OnSetTrackRightDefault( WidgetSetHorizontalSliderConfig _this, 
  ResourcesBitmap value );

/* 'C' function for method : 'WidgetSet::HorizontalSliderConfig.OnSetTrackLeftFrameActive()' */
void WidgetSetHorizontalSliderConfig_OnSetTrackLeftFrameActive( WidgetSetHorizontalSliderConfig _this, 
  XInt32 value );

/* 'C' function for method : 'WidgetSet::HorizontalSliderConfig.OnSetTrackLeftFrameFocused()' */
void WidgetSetHorizontalSliderConfig_OnSetTrackLeftFrameFocused( WidgetSetHorizontalSliderConfig _this, 
  XInt32 value );

/* 'C' function for method : 'WidgetSet::HorizontalSliderConfig.OnSetTrackLeftFrameDisabled()' */
void WidgetSetHorizontalSliderConfig_OnSetTrackLeftFrameDisabled( WidgetSetHorizontalSliderConfig _this, 
  XInt32 value );

/* 'C' function for method : 'WidgetSet::HorizontalSliderConfig.OnSetTrackLeftFrameDefault()' */
void WidgetSetHorizontalSliderConfig_OnSetTrackLeftFrameDefault( WidgetSetHorizontalSliderConfig _this, 
  XInt32 value );

/* 'C' function for method : 'WidgetSet::HorizontalSliderConfig.OnSetTrackLeftActive()' */
void WidgetSetHorizontalSliderConfig_OnSetTrackLeftActive( WidgetSetHorizontalSliderConfig _this, 
  ResourcesBitmap value );

/* 'C' function for method : 'WidgetSet::HorizontalSliderConfig.OnSetTrackLeftFocused()' */
void WidgetSetHorizontalSliderConfig_OnSetTrackLeftFocused( WidgetSetHorizontalSliderConfig _this, 
  ResourcesBitmap value );

/* 'C' function for method : 'WidgetSet::HorizontalSliderConfig.OnSetTrackLeftDisabled()' */
void WidgetSetHorizontalSliderConfig_OnSetTrackLeftDisabled( WidgetSetHorizontalSliderConfig _this, 
  ResourcesBitmap value );

/* 'C' function for method : 'WidgetSet::HorizontalSliderConfig.OnSetTrackLeftDefault()' */
void WidgetSetHorizontalSliderConfig_OnSetTrackLeftDefault( WidgetSetHorizontalSliderConfig _this, 
  ResourcesBitmap value );

/* 'C' function for method : 'WidgetSet::HorizontalSliderConfig.OnSetWidgetMaxSize()' */
void WidgetSetHorizontalSliderConfig_OnSetWidgetMaxSize( WidgetSetHorizontalSliderConfig _this, 
  XPoint value );

/* 'C' function for method : 'WidgetSet::HorizontalSliderConfig.OnSetWidgetMinSize()' */
void WidgetSetHorizontalSliderConfig_OnSetWidgetMinSize( WidgetSetHorizontalSliderConfig _this, 
  XPoint value );

#ifdef __cplusplus
  }
#endif

#endif /* _WidgetSetHorizontalSliderConfig_H */

/* Embedded Wizard */
