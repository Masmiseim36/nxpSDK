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
* Platform : NXP.iMX_RT.RGB565
*
*******************************************************************************/

#ifndef _WidgetSetVerticalSliderConfig_H
#define _WidgetSetVerticalSliderConfig_H

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

/* Forward declaration of the class WidgetSet::VerticalSliderConfig */
#ifndef _WidgetSetVerticalSliderConfig_
  EW_DECLARE_CLASS( WidgetSetVerticalSliderConfig )
#define _WidgetSetVerticalSliderConfig_
#endif


/* This class implements the functionality permitting you to simply customize the 
   look and feel of a 'vertical slider' widget (WidgetSet::VerticalSlider). In the 
   practice, you will create an instance of this class, configure its properties 
   with bitmaps, colors and other relevant attributes according to your design expectations 
   and assign such prepared configuration object to the property 'Appearance' of 
   every affected vertical slider widget. Thereupon the widgets will use the configuration 
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
   - 'Face' is a bitmap frame view (Views::Frame) filling horizontally centered 
   the entire height in the background of the slider. In the configuration object 
   you can individually specify for every slider state the desired bitmap (@FaceActive, 
   @FaceDefault, @FaceDisabled, @FaceFocused), the frame number within the bitmap 
   (@FaceFrameActive, @FaceFrameDefault, @FaceFrameDisabled, @FaceFrameFocused) 
   and opacity or color to tint the bitmap (@FaceTintActive, @FaceTintDefault, @FaceTintDisabled, 
   @FaceTintFocused). The slider can automatically play animated bitmaps if the 
   specified frame number is -1.
   - 'TrackBelow' is a bitmap frame view (Views::Frame) filling horizontally centered 
   the background of the slider between its bottom edge and the actual position 
   of the thumb. In the configuration object you can individually specify for every 
   slider state the desired bitmap (@TrackBelowActive, @TrackBelowDefault, @TrackBelowDisabled, 
   @TrackBelowFocused), the frame number within the bitmap (@TrackBelowFrameActive, 
   @TrackBelowFrameDefault, @TrackBelowFrameDisabled, @TrackBelowFrameFocused) and 
   opacity or color to tint the bitmap (@TrackBelowTintActive, @TrackBelowTintDefault, 
   @TrackBelowTintDisabled, @TrackBelowTintFocused). With the property @TrackBelowWithEdge 
   you can configure how the top edge of the track (the edge at the thumb position) 
   should appear. The slider can automatically play animated bitmaps if the specified 
   frame number is -1.
   - 'TrackAbove' is a bitmap frame view (Views::Frame) filling horizontally centered 
   the background of the slider between the actual position of the thumb and the 
   top edge of the slider widget. In the configuration object you can individually 
   specify for every slider state the desired bitmap (@TrackAboveActive, @TrackAboveDefault, 
   @TrackAboveDisabled, @TrackAboveFocused), the frame number within the bitmap 
   (@TrackAboveFrameActive, @TrackAboveFrameDefault, @TrackAboveFrameDisabled, @TrackAboveFrameFocused) 
   and opacity or color to tint the bitmap (@TrackAboveTintActive, @TrackAboveTintDefault, 
   @TrackAboveTintDisabled, @TrackAboveTintFocused). With the property @TrackAboveWithEdge 
   you can configure how the bottom edge of the track (the edge at the thumb position) 
   should appear. The slider can automatically play animated bitmaps if the specified 
   frame number is -1.
   - 'Thumb' is an image view (Views::Image) displayed horizontally centered at 
   the thumb position according to current value of the slider. In the configuration 
   object you can individually specify for every slider state the desired bitmap 
   (@ThumbActive, @ThumbDefault, @ThumbDisabled, @ThumbFocused), the frame number 
   within the bitmap (@ThumbFrameActive, @ThumbFrameDefault, @ThumbFrameDisabled, 
   @ThumbFrameFocused) and opacity or color to tint the bitmap (@ThumbTintActive, 
   @ThumbTintDefault, @ThumbTintDisabled, @ThumbTintFocused). If necessary, additional 
   margins below and above the thumb can be specified by using the properties @ThumbMarginBelow 
   and @ThumbMarginAbove. The slider can automatically play animated bitmaps if 
   the specified frame number is -1.
   - 'Cover' is a bitmap frame view (Views::Frame) filling horizontally centered 
   the entire height of the slider and covering so eventually the thumb and track. 
   In the configuration object you can individually specify for every slider state 
   the desired bitmap (@CoverActive, @CoverDefault, @CoverDisabled, @CoverFocused), 
   the frame number within the bitmap (@CoverFrameActive, @CoverFrameDefault, @CoverFrameDisabled, 
   @CoverFrameFocused) and opacity or color to tint the bitmap (@CoverTintActive, 
   @CoverTintDefault, @CoverTintDisabled, @CoverTintFocused). The slider can automatically 
   play animated bitmaps if the specified frame number is -1.
   If the slider is actually focused, it can also be controlled by pressing the 
   keyboard keys specified in the properties @KeyCodeDown and @KeyCodeUp causing 
   the slider's thumb to be moved down or up. To prevent the slider from being able 
   to be focused, initialize these properties with the value Core::KeyCode.NoKey. 
   With the properties @KeyRepeatDelay and @KeyRepeatPeriod you can configure whether 
   the thumb should be moved autonomously when the user holds the key pressed for 
   a while.
   With the properties @WidgetMinSize and @WidgetMaxSize you can configure size 
   constraints for the widget itself. You can, for example, limit the slider to 
   not shrink below a specified width or height. */
EW_DEFINE_FIELDS( WidgetSetVerticalSliderConfig, WidgetSetWidgetConfig )
  EW_PROPERTY( CoverActive,     ResourcesBitmap )
  EW_PROPERTY( CoverFocused,    ResourcesBitmap )
  EW_PROPERTY( CoverDisabled,   ResourcesBitmap )
  EW_PROPERTY( CoverDefault,    ResourcesBitmap )
  EW_PROPERTY( ThumbActive,     ResourcesBitmap )
  EW_PROPERTY( ThumbFocused,    ResourcesBitmap )
  EW_PROPERTY( ThumbDisabled,   ResourcesBitmap )
  EW_PROPERTY( ThumbDefault,    ResourcesBitmap )
  EW_PROPERTY( TrackAboveActive, ResourcesBitmap )
  EW_PROPERTY( TrackAboveFocused, ResourcesBitmap )
  EW_PROPERTY( TrackAboveDisabled, ResourcesBitmap )
  EW_PROPERTY( TrackAboveDefault, ResourcesBitmap )
  EW_PROPERTY( TrackBelowActive, ResourcesBitmap )
  EW_PROPERTY( TrackBelowFocused, ResourcesBitmap )
  EW_PROPERTY( TrackBelowDisabled, ResourcesBitmap )
  EW_PROPERTY( TrackBelowDefault, ResourcesBitmap )
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
  EW_PROPERTY( ThumbMarginAbove, XInt32 )
  EW_PROPERTY( ThumbMarginBelow, XInt32 )
  EW_PROPERTY( ThumbTintActive, XColor )
  EW_PROPERTY( ThumbTintFocused, XColor )
  EW_PROPERTY( ThumbTintDisabled, XColor )
  EW_PROPERTY( ThumbTintDefault, XColor )
  EW_PROPERTY( ThumbFrameActive, XInt32 )
  EW_PROPERTY( ThumbFrameFocused, XInt32 )
  EW_PROPERTY( ThumbFrameDisabled, XInt32 )
  EW_PROPERTY( ThumbFrameDefault, XInt32 )
  EW_PROPERTY( TrackAboveTintActive, XColor )
  EW_PROPERTY( TrackAboveTintFocused, XColor )
  EW_PROPERTY( TrackAboveTintDisabled, XColor )
  EW_PROPERTY( TrackAboveTintDefault, XColor )
  EW_PROPERTY( TrackAboveFrameActive, XInt32 )
  EW_PROPERTY( TrackAboveFrameFocused, XInt32 )
  EW_PROPERTY( TrackAboveFrameDisabled, XInt32 )
  EW_PROPERTY( TrackAboveFrameDefault, XInt32 )
  EW_PROPERTY( TrackBelowTintActive, XColor )
  EW_PROPERTY( TrackBelowTintFocused, XColor )
  EW_PROPERTY( TrackBelowTintDisabled, XColor )
  EW_PROPERTY( TrackBelowTintDefault, XColor )
  EW_PROPERTY( TrackBelowFrameActive, XInt32 )
  EW_PROPERTY( TrackBelowFrameFocused, XInt32 )
  EW_PROPERTY( TrackBelowFrameDisabled, XInt32 )
  EW_PROPERTY( TrackBelowFrameDefault, XInt32 )
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
  EW_PROPERTY( KeyCodeDown,     XEnum )
  EW_PROPERTY( KeyCodeUp,       XEnum )
  EW_PROPERTY( TrackBelowWithEdge, XBool )
  EW_PROPERTY( TrackAboveWithEdge, XBool )
  EW_PROPERTY( ThumbUpdateAtTouch, XBool )
EW_END_OF_FIELDS( WidgetSetVerticalSliderConfig )

/* Virtual Method Table (VMT) for the class : 'WidgetSet::VerticalSliderConfig' */
EW_DEFINE_METHODS( WidgetSetVerticalSliderConfig, WidgetSetWidgetConfig )
EW_END_OF_METHODS( WidgetSetVerticalSliderConfig )

/* 'C' function for method : 'WidgetSet::VerticalSliderConfig.OnSetTrackAboveFrameActive()' */
void WidgetSetVerticalSliderConfig_OnSetTrackAboveFrameActive( WidgetSetVerticalSliderConfig _this, 
  XInt32 value );

/* 'C' function for method : 'WidgetSet::VerticalSliderConfig.OnSetTrackAboveFrameFocused()' */
void WidgetSetVerticalSliderConfig_OnSetTrackAboveFrameFocused( WidgetSetVerticalSliderConfig _this, 
  XInt32 value );

/* 'C' function for method : 'WidgetSet::VerticalSliderConfig.OnSetTrackAboveFrameDisabled()' */
void WidgetSetVerticalSliderConfig_OnSetTrackAboveFrameDisabled( WidgetSetVerticalSliderConfig _this, 
  XInt32 value );

/* 'C' function for method : 'WidgetSet::VerticalSliderConfig.OnSetTrackAboveFrameDefault()' */
void WidgetSetVerticalSliderConfig_OnSetTrackAboveFrameDefault( WidgetSetVerticalSliderConfig _this, 
  XInt32 value );

/* 'C' function for method : 'WidgetSet::VerticalSliderConfig.OnSetTrackAboveActive()' */
void WidgetSetVerticalSliderConfig_OnSetTrackAboveActive( WidgetSetVerticalSliderConfig _this, 
  ResourcesBitmap value );

/* 'C' function for method : 'WidgetSet::VerticalSliderConfig.OnSetTrackAboveFocused()' */
void WidgetSetVerticalSliderConfig_OnSetTrackAboveFocused( WidgetSetVerticalSliderConfig _this, 
  ResourcesBitmap value );

/* 'C' function for method : 'WidgetSet::VerticalSliderConfig.OnSetTrackAboveDisabled()' */
void WidgetSetVerticalSliderConfig_OnSetTrackAboveDisabled( WidgetSetVerticalSliderConfig _this, 
  ResourcesBitmap value );

/* 'C' function for method : 'WidgetSet::VerticalSliderConfig.OnSetTrackAboveDefault()' */
void WidgetSetVerticalSliderConfig_OnSetTrackAboveDefault( WidgetSetVerticalSliderConfig _this, 
  ResourcesBitmap value );

/* 'C' function for method : 'WidgetSet::VerticalSliderConfig.OnSetTrackBelowFrameActive()' */
void WidgetSetVerticalSliderConfig_OnSetTrackBelowFrameActive( WidgetSetVerticalSliderConfig _this, 
  XInt32 value );

/* 'C' function for method : 'WidgetSet::VerticalSliderConfig.OnSetTrackBelowFrameFocused()' */
void WidgetSetVerticalSliderConfig_OnSetTrackBelowFrameFocused( WidgetSetVerticalSliderConfig _this, 
  XInt32 value );

/* 'C' function for method : 'WidgetSet::VerticalSliderConfig.OnSetTrackBelowFrameDisabled()' */
void WidgetSetVerticalSliderConfig_OnSetTrackBelowFrameDisabled( WidgetSetVerticalSliderConfig _this, 
  XInt32 value );

/* 'C' function for method : 'WidgetSet::VerticalSliderConfig.OnSetTrackBelowFrameDefault()' */
void WidgetSetVerticalSliderConfig_OnSetTrackBelowFrameDefault( WidgetSetVerticalSliderConfig _this, 
  XInt32 value );

/* 'C' function for method : 'WidgetSet::VerticalSliderConfig.OnSetTrackBelowActive()' */
void WidgetSetVerticalSliderConfig_OnSetTrackBelowActive( WidgetSetVerticalSliderConfig _this, 
  ResourcesBitmap value );

/* 'C' function for method : 'WidgetSet::VerticalSliderConfig.OnSetTrackBelowFocused()' */
void WidgetSetVerticalSliderConfig_OnSetTrackBelowFocused( WidgetSetVerticalSliderConfig _this, 
  ResourcesBitmap value );

/* 'C' function for method : 'WidgetSet::VerticalSliderConfig.OnSetTrackBelowDisabled()' */
void WidgetSetVerticalSliderConfig_OnSetTrackBelowDisabled( WidgetSetVerticalSliderConfig _this, 
  ResourcesBitmap value );

/* 'C' function for method : 'WidgetSet::VerticalSliderConfig.OnSetTrackBelowDefault()' */
void WidgetSetVerticalSliderConfig_OnSetTrackBelowDefault( WidgetSetVerticalSliderConfig _this, 
  ResourcesBitmap value );

/* 'C' function for method : 'WidgetSet::VerticalSliderConfig.OnSetWidgetMaxSize()' */
void WidgetSetVerticalSliderConfig_OnSetWidgetMaxSize( WidgetSetVerticalSliderConfig _this, 
  XPoint value );

/* 'C' function for method : 'WidgetSet::VerticalSliderConfig.OnSetWidgetMinSize()' */
void WidgetSetVerticalSliderConfig_OnSetWidgetMinSize( WidgetSetVerticalSliderConfig _this, 
  XPoint value );

#ifdef __cplusplus
  }
#endif

#endif /* _WidgetSetVerticalSliderConfig_H */

/* Embedded Wizard */
