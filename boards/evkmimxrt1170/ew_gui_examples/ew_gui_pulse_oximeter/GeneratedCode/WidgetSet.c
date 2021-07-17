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

#include "ewlocale.h"
#include "_CoreKeyPressHandler.h"
#include "_CoreRoot.h"
#include "_CoreRotateTouchHandler.h"
#include "_CoreSimpleTouchHandler.h"
#include "_CoreTimer.h"
#include "_CoreView.h"
#include "_GraphicsArcPath.h"
#include "_GraphicsPath.h"
#include "_GraphicsWarpMatrix.h"
#include "_ResourcesBitmap.h"
#include "_ViewsFillPath.h"
#include "_ViewsFrame.h"
#include "_ViewsImage.h"
#include "_ViewsText.h"
#include "_ViewsWarpImage.h"
#include "_WidgetSetHorizontalSlider.h"
#include "_WidgetSetHorizontalSliderConfig.h"
#include "_WidgetSetPushButton.h"
#include "_WidgetSetPushButtonConfig.h"
#include "_WidgetSetRotaryKnob.h"
#include "_WidgetSetRotaryKnobConfig.h"
#include "_WidgetSetToggleButton.h"
#include "_WidgetSetToggleButtonConfig.h"
#include "_WidgetSetWidgetConfig.h"
#include "Core.h"
#include "Graphics.h"
#include "Views.h"
#include "WidgetSet.h"

/* Constant values used in this 'C' module only. */
static const XColor _Const0000 = { 0xFF, 0xFF, 0xFF, 0xFF };
static const XColor _Const0001 = { 0x00, 0x00, 0x00, 0xFF };
static const XRect _Const0002 = {{ 0, 0 }, { 120, 120 }};
static const XPoint _Const0003 = { 0, 0 };
static const XPoint _Const0004 = { 60, 0 };
static const XPoint _Const0005 = { 60, 50 };
static const XPoint _Const0006 = { 0, 50 };
static const XRect _Const0007 = {{ 0, 0 }, { 200, 50 }};
static const XPoint _Const0008 = { 200, 50 };
static const XPoint _Const0009 = { 200, 0 };
static const XRect _Const000A = {{ 0, 0 }, { 150, 50 }};
static const XPoint _Const000B = { 150, 50 };
static const XPoint _Const000C = { 150, 0 };

/* Forward declaration of the class Resources::Font */
#ifndef _ResourcesFont_
  EW_DECLARE_CLASS( ResourcesFont )
#define _ResourcesFont_
#endif

/* Initializer for the class 'WidgetSet::RotaryKnobConfig' */
void WidgetSetRotaryKnobConfig__Init( WidgetSetRotaryKnobConfig _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  WidgetSetWidgetConfig__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( WidgetSetRotaryKnobConfig );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( WidgetSetRotaryKnobConfig );

  /* ... and initialize objects, variables, properties, etc. */
  _this->KeyCodeRight = CoreKeyCodeRight;
  _this->KeyCodeLeft = CoreKeyCodeLeft;
  _this->TrackRightColorActive = _Const0000;
  _this->TrackRightColorFocused = _Const0000;
  _this->TrackRightColorDisabled = _Const0000;
  _this->TrackRightColorDefault = _Const0000;
  _this->TrackLeftColorActive = _Const0000;
  _this->TrackLeftColorFocused = _Const0000;
  _this->TrackLeftColorDisabled = _Const0000;
  _this->TrackLeftColorDefault = _Const0000;
  _this->CoverTintActive = _Const0000;
  _this->CoverTintFocused = _Const0000;
  _this->CoverTintDisabled = _Const0000;
  _this->CoverTintDefault = _Const0000;
  _this->CoverFrameActive = -1;
  _this->CoverFrameFocused = -1;
  _this->CoverFrameDisabled = -1;
  _this->CoverFrameDefault = -1;
  _this->ThumbMaxAngle = 45.000000f;
  _this->ThumbMinAngle = 135.000000f;
  _this->ThumbRotate = 1;
  _this->ThumbTintActive = _Const0000;
  _this->ThumbTintFocused = _Const0000;
  _this->ThumbTintDisabled = _Const0000;
  _this->ThumbTintDefault = _Const0000;
  _this->ThumbFrameActive = -1;
  _this->ThumbFrameFocused = -1;
  _this->ThumbFrameDisabled = -1;
  _this->ThumbFrameDefault = -1;
  _this->ScaleTintActive = _Const0000;
  _this->ScaleTintFocused = _Const0000;
  _this->ScaleTintDisabled = _Const0000;
  _this->ScaleTintDefault = _Const0000;
  _this->ScaleFrameActive = -1;
  _this->ScaleFrameFocused = -1;
  _this->ScaleFrameDisabled = -1;
  _this->ScaleFrameDefault = -1;
}

/* Re-Initializer for the class 'WidgetSet::RotaryKnobConfig' */
void WidgetSetRotaryKnobConfig__ReInit( WidgetSetRotaryKnobConfig _this )
{
  /* At first re-initialize the super class ... */
  WidgetSetWidgetConfig__ReInit( &_this->_Super );
}

/* Finalizer method for the class 'WidgetSet::RotaryKnobConfig' */
void WidgetSetRotaryKnobConfig__Done( WidgetSetRotaryKnobConfig _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( WidgetSetWidgetConfig );

  /* Don't forget to deinitialize the super class ... */
  WidgetSetWidgetConfig__Done( &_this->_Super );
}

/* 'C' function for method : 'WidgetSet::RotaryKnobConfig.OnSetThumbMaxAngle()' */
void WidgetSetRotaryKnobConfig_OnSetThumbMaxAngle( WidgetSetRotaryKnobConfig _this, 
  XFloat value )
{
  if ( _this->ThumbMaxAngle == value )
    return;

  _this->ThumbMaxAngle = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::RotaryKnobConfig.OnSetThumbMinAngle()' */
void WidgetSetRotaryKnobConfig_OnSetThumbMinAngle( WidgetSetRotaryKnobConfig _this, 
  XFloat value )
{
  if ( _this->ThumbMinAngle == value )
    return;

  _this->ThumbMinAngle = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::RotaryKnobConfig.OnSetThumbPivot()' */
void WidgetSetRotaryKnobConfig_OnSetThumbPivot( WidgetSetRotaryKnobConfig _this, 
  XPoint value )
{
  if ( !EwCompPoint( _this->ThumbPivot, value ))
    return;

  _this->ThumbPivot = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::RotaryKnobConfig.OnSetThumbActive()' */
void WidgetSetRotaryKnobConfig_OnSetThumbActive( WidgetSetRotaryKnobConfig _this, 
  ResourcesBitmap value )
{
  if ( _this->ThumbActive == value )
    return;

  _this->ThumbActive = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::RotaryKnobConfig.OnSetThumbFocused()' */
void WidgetSetRotaryKnobConfig_OnSetThumbFocused( WidgetSetRotaryKnobConfig _this, 
  ResourcesBitmap value )
{
  if ( _this->ThumbFocused == value )
    return;

  _this->ThumbFocused = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::RotaryKnobConfig.OnSetThumbDefault()' */
void WidgetSetRotaryKnobConfig_OnSetThumbDefault( WidgetSetRotaryKnobConfig _this, 
  ResourcesBitmap value )
{
  if ( _this->ThumbDefault == value )
    return;

  _this->ThumbDefault = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::RotaryKnobConfig.OnSetScaleActive()' */
void WidgetSetRotaryKnobConfig_OnSetScaleActive( WidgetSetRotaryKnobConfig _this, 
  ResourcesBitmap value )
{
  if ( _this->ScaleActive == value )
    return;

  _this->ScaleActive = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::RotaryKnobConfig.OnSetScaleFocused()' */
void WidgetSetRotaryKnobConfig_OnSetScaleFocused( WidgetSetRotaryKnobConfig _this, 
  ResourcesBitmap value )
{
  if ( _this->ScaleFocused == value )
    return;

  _this->ScaleFocused = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::RotaryKnobConfig.OnSetScaleDisabled()' */
void WidgetSetRotaryKnobConfig_OnSetScaleDisabled( WidgetSetRotaryKnobConfig _this, 
  ResourcesBitmap value )
{
  if ( _this->ScaleDisabled == value )
    return;

  _this->ScaleDisabled = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::RotaryKnobConfig.OnSetScaleDefault()' */
void WidgetSetRotaryKnobConfig_OnSetScaleDefault( WidgetSetRotaryKnobConfig _this, 
  ResourcesBitmap value )
{
  if ( _this->ScaleDefault == value )
    return;

  _this->ScaleDefault = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::RotaryKnobConfig.OnSetWidgetMaxSize()' */
void WidgetSetRotaryKnobConfig_OnSetWidgetMaxSize( WidgetSetRotaryKnobConfig _this, 
  XPoint value )
{
  if ( !EwCompPoint( _this->WidgetMaxSize, value ))
    return;

  _this->WidgetMaxSize = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::RotaryKnobConfig.OnSetWidgetMinSize()' */
void WidgetSetRotaryKnobConfig_OnSetWidgetMinSize( WidgetSetRotaryKnobConfig _this, 
  XPoint value )
{
  if ( !EwCompPoint( _this->WidgetMinSize, value ))
    return;

  _this->WidgetMinSize = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* Variants derived from the class : 'WidgetSet::RotaryKnobConfig' */
EW_DEFINE_CLASS_VARIANTS( WidgetSetRotaryKnobConfig )
EW_END_OF_CLASS_VARIANTS( WidgetSetRotaryKnobConfig )

/* Virtual Method Table (VMT) for the class : 'WidgetSet::RotaryKnobConfig' */
EW_DEFINE_CLASS( WidgetSetRotaryKnobConfig, WidgetSetWidgetConfig, CoverActive, 
                 KeyRepeatPeriod, KeyRepeatPeriod, KeyRepeatPeriod, KeyRepeatPeriod, 
                 KeyRepeatPeriod, "WidgetSet::RotaryKnobConfig" )
EW_END_OF_CLASS( WidgetSetRotaryKnobConfig )

/* Initializer for the class 'WidgetSet::HorizontalSliderConfig' */
void WidgetSetHorizontalSliderConfig__Init( WidgetSetHorizontalSliderConfig _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  WidgetSetWidgetConfig__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( WidgetSetHorizontalSliderConfig );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( WidgetSetHorizontalSliderConfig );

  /* ... and initialize objects, variables, properties, etc. */
  _this->KeyCodeRight = CoreKeyCodeRight;
  _this->KeyCodeLeft = CoreKeyCodeLeft;
  _this->CoverTintActive = _Const0000;
  _this->CoverTintFocused = _Const0000;
  _this->CoverTintDisabled = _Const0000;
  _this->CoverTintDefault = _Const0000;
  _this->CoverFrameActive = -1;
  _this->CoverFrameFocused = -1;
  _this->CoverFrameDisabled = -1;
  _this->CoverFrameDefault = -1;
  _this->ThumbTintActive = _Const0000;
  _this->ThumbTintFocused = _Const0000;
  _this->ThumbTintDisabled = _Const0000;
  _this->ThumbTintDefault = _Const0000;
  _this->ThumbFrameActive = -1;
  _this->ThumbFrameFocused = -1;
  _this->ThumbFrameDisabled = -1;
  _this->ThumbFrameDefault = -1;
  _this->TrackRightTintActive = _Const0000;
  _this->TrackRightTintFocused = _Const0000;
  _this->TrackRightTintDisabled = _Const0000;
  _this->TrackRightTintDefault = _Const0000;
  _this->TrackRightFrameActive = -1;
  _this->TrackRightFrameFocused = -1;
  _this->TrackRightFrameDisabled = -1;
  _this->TrackRightFrameDefault = -1;
  _this->TrackLeftTintActive = _Const0000;
  _this->TrackLeftTintFocused = _Const0000;
  _this->TrackLeftTintDisabled = _Const0000;
  _this->TrackLeftTintDefault = _Const0000;
  _this->TrackLeftFrameActive = -1;
  _this->TrackLeftFrameFocused = -1;
  _this->TrackLeftFrameDisabled = -1;
  _this->TrackLeftFrameDefault = -1;
  _this->FaceTintActive = _Const0000;
  _this->FaceTintFocused = _Const0000;
  _this->FaceTintDisabled = _Const0000;
  _this->FaceTintDefault = _Const0000;
  _this->FaceFrameActive = -1;
  _this->FaceFrameFocused = -1;
  _this->FaceFrameDisabled = -1;
  _this->FaceFrameDefault = -1;
}

/* Re-Initializer for the class 'WidgetSet::HorizontalSliderConfig' */
void WidgetSetHorizontalSliderConfig__ReInit( WidgetSetHorizontalSliderConfig _this )
{
  /* At first re-initialize the super class ... */
  WidgetSetWidgetConfig__ReInit( &_this->_Super );
}

/* Finalizer method for the class 'WidgetSet::HorizontalSliderConfig' */
void WidgetSetHorizontalSliderConfig__Done( WidgetSetHorizontalSliderConfig _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( WidgetSetWidgetConfig );

  /* Don't forget to deinitialize the super class ... */
  WidgetSetWidgetConfig__Done( &_this->_Super );
}

/* 'C' function for method : 'WidgetSet::HorizontalSliderConfig.OnSetThumbActive()' */
void WidgetSetHorizontalSliderConfig_OnSetThumbActive( WidgetSetHorizontalSliderConfig _this, 
  ResourcesBitmap value )
{
  if ( _this->ThumbActive == value )
    return;

  _this->ThumbActive = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::HorizontalSliderConfig.OnSetThumbFocused()' */
void WidgetSetHorizontalSliderConfig_OnSetThumbFocused( WidgetSetHorizontalSliderConfig _this, 
  ResourcesBitmap value )
{
  if ( _this->ThumbFocused == value )
    return;

  _this->ThumbFocused = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::HorizontalSliderConfig.OnSetThumbDefault()' */
void WidgetSetHorizontalSliderConfig_OnSetThumbDefault( WidgetSetHorizontalSliderConfig _this, 
  ResourcesBitmap value )
{
  if ( _this->ThumbDefault == value )
    return;

  _this->ThumbDefault = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::HorizontalSliderConfig.OnSetTrackRightFrameActive()' */
void WidgetSetHorizontalSliderConfig_OnSetTrackRightFrameActive( WidgetSetHorizontalSliderConfig _this, 
  XInt32 value )
{
  if ( _this->TrackRightFrameActive == value )
    return;

  _this->TrackRightFrameActive = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::HorizontalSliderConfig.OnSetTrackRightFrameFocused()' */
void WidgetSetHorizontalSliderConfig_OnSetTrackRightFrameFocused( WidgetSetHorizontalSliderConfig _this, 
  XInt32 value )
{
  if ( _this->TrackRightFrameFocused == value )
    return;

  _this->TrackRightFrameFocused = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::HorizontalSliderConfig.OnSetTrackRightFrameDisabled()' */
void WidgetSetHorizontalSliderConfig_OnSetTrackRightFrameDisabled( WidgetSetHorizontalSliderConfig _this, 
  XInt32 value )
{
  if ( _this->TrackRightFrameDisabled == value )
    return;

  _this->TrackRightFrameDisabled = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::HorizontalSliderConfig.OnSetTrackRightFrameDefault()' */
void WidgetSetHorizontalSliderConfig_OnSetTrackRightFrameDefault( WidgetSetHorizontalSliderConfig _this, 
  XInt32 value )
{
  if ( _this->TrackRightFrameDefault == value )
    return;

  _this->TrackRightFrameDefault = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::HorizontalSliderConfig.OnSetTrackRightActive()' */
void WidgetSetHorizontalSliderConfig_OnSetTrackRightActive( WidgetSetHorizontalSliderConfig _this, 
  ResourcesBitmap value )
{
  if ( _this->TrackRightActive == value )
    return;

  _this->TrackRightActive = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::HorizontalSliderConfig.OnSetTrackRightFocused()' */
void WidgetSetHorizontalSliderConfig_OnSetTrackRightFocused( WidgetSetHorizontalSliderConfig _this, 
  ResourcesBitmap value )
{
  if ( _this->TrackRightFocused == value )
    return;

  _this->TrackRightFocused = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::HorizontalSliderConfig.OnSetTrackRightDisabled()' */
void WidgetSetHorizontalSliderConfig_OnSetTrackRightDisabled( WidgetSetHorizontalSliderConfig _this, 
  ResourcesBitmap value )
{
  if ( _this->TrackRightDisabled == value )
    return;

  _this->TrackRightDisabled = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::HorizontalSliderConfig.OnSetTrackRightDefault()' */
void WidgetSetHorizontalSliderConfig_OnSetTrackRightDefault( WidgetSetHorizontalSliderConfig _this, 
  ResourcesBitmap value )
{
  if ( _this->TrackRightDefault == value )
    return;

  _this->TrackRightDefault = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::HorizontalSliderConfig.OnSetTrackLeftFrameActive()' */
void WidgetSetHorizontalSliderConfig_OnSetTrackLeftFrameActive( WidgetSetHorizontalSliderConfig _this, 
  XInt32 value )
{
  if ( _this->TrackLeftFrameActive == value )
    return;

  _this->TrackLeftFrameActive = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::HorizontalSliderConfig.OnSetTrackLeftFrameFocused()' */
void WidgetSetHorizontalSliderConfig_OnSetTrackLeftFrameFocused( WidgetSetHorizontalSliderConfig _this, 
  XInt32 value )
{
  if ( _this->TrackLeftFrameFocused == value )
    return;

  _this->TrackLeftFrameFocused = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::HorizontalSliderConfig.OnSetTrackLeftFrameDisabled()' */
void WidgetSetHorizontalSliderConfig_OnSetTrackLeftFrameDisabled( WidgetSetHorizontalSliderConfig _this, 
  XInt32 value )
{
  if ( _this->TrackLeftFrameDisabled == value )
    return;

  _this->TrackLeftFrameDisabled = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::HorizontalSliderConfig.OnSetTrackLeftFrameDefault()' */
void WidgetSetHorizontalSliderConfig_OnSetTrackLeftFrameDefault( WidgetSetHorizontalSliderConfig _this, 
  XInt32 value )
{
  if ( _this->TrackLeftFrameDefault == value )
    return;

  _this->TrackLeftFrameDefault = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::HorizontalSliderConfig.OnSetTrackLeftActive()' */
void WidgetSetHorizontalSliderConfig_OnSetTrackLeftActive( WidgetSetHorizontalSliderConfig _this, 
  ResourcesBitmap value )
{
  if ( _this->TrackLeftActive == value )
    return;

  _this->TrackLeftActive = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::HorizontalSliderConfig.OnSetTrackLeftFocused()' */
void WidgetSetHorizontalSliderConfig_OnSetTrackLeftFocused( WidgetSetHorizontalSliderConfig _this, 
  ResourcesBitmap value )
{
  if ( _this->TrackLeftFocused == value )
    return;

  _this->TrackLeftFocused = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::HorizontalSliderConfig.OnSetTrackLeftDisabled()' */
void WidgetSetHorizontalSliderConfig_OnSetTrackLeftDisabled( WidgetSetHorizontalSliderConfig _this, 
  ResourcesBitmap value )
{
  if ( _this->TrackLeftDisabled == value )
    return;

  _this->TrackLeftDisabled = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::HorizontalSliderConfig.OnSetTrackLeftDefault()' */
void WidgetSetHorizontalSliderConfig_OnSetTrackLeftDefault( WidgetSetHorizontalSliderConfig _this, 
  ResourcesBitmap value )
{
  if ( _this->TrackLeftDefault == value )
    return;

  _this->TrackLeftDefault = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::HorizontalSliderConfig.OnSetWidgetMaxSize()' */
void WidgetSetHorizontalSliderConfig_OnSetWidgetMaxSize( WidgetSetHorizontalSliderConfig _this, 
  XPoint value )
{
  if ( !EwCompPoint( _this->WidgetMaxSize, value ))
    return;

  _this->WidgetMaxSize = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::HorizontalSliderConfig.OnSetWidgetMinSize()' */
void WidgetSetHorizontalSliderConfig_OnSetWidgetMinSize( WidgetSetHorizontalSliderConfig _this, 
  XPoint value )
{
  if ( !EwCompPoint( _this->WidgetMinSize, value ))
    return;

  _this->WidgetMinSize = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* Variants derived from the class : 'WidgetSet::HorizontalSliderConfig' */
EW_DEFINE_CLASS_VARIANTS( WidgetSetHorizontalSliderConfig )
EW_END_OF_CLASS_VARIANTS( WidgetSetHorizontalSliderConfig )

/* Virtual Method Table (VMT) for the class : 'WidgetSet::HorizontalSliderConfig' */
EW_DEFINE_CLASS( WidgetSetHorizontalSliderConfig, WidgetSetWidgetConfig, CoverActive, 
                 KeyRepeatPeriod, KeyRepeatPeriod, KeyRepeatPeriod, KeyRepeatPeriod, 
                 KeyRepeatPeriod, "WidgetSet::HorizontalSliderConfig" )
EW_END_OF_CLASS( WidgetSetHorizontalSliderConfig )

/* Initializer for the class 'WidgetSet::ToggleButtonConfig' */
void WidgetSetToggleButtonConfig__Init( WidgetSetToggleButtonConfig _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  WidgetSetWidgetConfig__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( WidgetSetToggleButtonConfig );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( WidgetSetToggleButtonConfig );

  /* ... and initialize objects, variables, properties, etc. */
  _this->PressedFeedbackDuration = 50;
  _this->KeyCode = CoreKeyCodeEnter;
  _this->LabelOnColorActive = _Const0001;
  _this->LabelOnColorFocused = _Const0001;
  _this->LabelOnColorDisabled = _Const0001;
  _this->LabelOnColorDefault = _Const0001;
  _this->LabelOffColorActive = _Const0001;
  _this->LabelOffColorFocused = _Const0001;
  _this->LabelOffColorDisabled = _Const0001;
  _this->LabelOffColorDefault = _Const0001;
  _this->IconOnTintActive = _Const0000;
  _this->IconOnTintFocused = _Const0000;
  _this->IconOnTintDisabled = _Const0000;
  _this->IconOnTintDefault = _Const0000;
  _this->IconOffTintActive = _Const0000;
  _this->IconOffTintFocused = _Const0000;
  _this->IconOffTintDisabled = _Const0000;
  _this->IconOffTintDefault = _Const0000;
  _this->LabelAlignment = ViewsTextAlignmentAlignHorzCenter | ViewsTextAlignmentAlignVertCenter;
  _this->IconAlignment = ViewsImageAlignmentAlignHorzCenter | ViewsImageAlignmentAlignVertCenter;
  _this->FaceLayout = CoreLayoutAlignToBottom | CoreLayoutAlignToLeft | CoreLayoutAlignToRight 
  | CoreLayoutAlignToTop | CoreLayoutResizeHorz | CoreLayoutResizeVert;
  _this->FaceOnTintActive = _Const0000;
  _this->FaceOnTintFocused = _Const0000;
  _this->FaceOnTintDisabled = _Const0000;
  _this->FaceOnTintDefault = _Const0000;
  _this->FaceOnFrameActive = -1;
  _this->FaceOnFrameFocused = -1;
  _this->FaceOnFrameDisabled = -1;
  _this->FaceOnFrameDefault = -1;
  _this->FaceOffTintActive = _Const0000;
  _this->FaceOffTintFocused = _Const0000;
  _this->FaceOffTintDisabled = _Const0000;
  _this->FaceOffTintDefault = _Const0000;
  _this->FaceOffFrameActive = -1;
  _this->FaceOffFrameFocused = -1;
  _this->FaceOffFrameDisabled = -1;
  _this->FaceOffFrameDefault = -1;
}

/* Re-Initializer for the class 'WidgetSet::ToggleButtonConfig' */
void WidgetSetToggleButtonConfig__ReInit( WidgetSetToggleButtonConfig _this )
{
  /* At first re-initialize the super class ... */
  WidgetSetWidgetConfig__ReInit( &_this->_Super );
}

/* Finalizer method for the class 'WidgetSet::ToggleButtonConfig' */
void WidgetSetToggleButtonConfig__Done( WidgetSetToggleButtonConfig _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( WidgetSetWidgetConfig );

  /* Don't forget to deinitialize the super class ... */
  WidgetSetWidgetConfig__Done( &_this->_Super );
}

/* 'C' function for method : 'WidgetSet::ToggleButtonConfig.OnSetFaceOnFrameActive()' */
void WidgetSetToggleButtonConfig_OnSetFaceOnFrameActive( WidgetSetToggleButtonConfig _this, 
  XInt32 value )
{
  if ( _this->FaceOnFrameActive == value )
    return;

  _this->FaceOnFrameActive = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::ToggleButtonConfig.OnSetFaceOnFrameFocused()' */
void WidgetSetToggleButtonConfig_OnSetFaceOnFrameFocused( WidgetSetToggleButtonConfig _this, 
  XInt32 value )
{
  if ( _this->FaceOnFrameFocused == value )
    return;

  _this->FaceOnFrameFocused = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::ToggleButtonConfig.OnSetFaceOnFrameDefault()' */
void WidgetSetToggleButtonConfig_OnSetFaceOnFrameDefault( WidgetSetToggleButtonConfig _this, 
  XInt32 value )
{
  if ( _this->FaceOnFrameDefault == value )
    return;

  _this->FaceOnFrameDefault = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::ToggleButtonConfig.OnSetFaceOnActive()' */
void WidgetSetToggleButtonConfig_OnSetFaceOnActive( WidgetSetToggleButtonConfig _this, 
  ResourcesBitmap value )
{
  if ( _this->FaceOnActive == value )
    return;

  _this->FaceOnActive = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::ToggleButtonConfig.OnSetFaceOnFocused()' */
void WidgetSetToggleButtonConfig_OnSetFaceOnFocused( WidgetSetToggleButtonConfig _this, 
  ResourcesBitmap value )
{
  if ( _this->FaceOnFocused == value )
    return;

  _this->FaceOnFocused = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::ToggleButtonConfig.OnSetFaceOnDefault()' */
void WidgetSetToggleButtonConfig_OnSetFaceOnDefault( WidgetSetToggleButtonConfig _this, 
  ResourcesBitmap value )
{
  if ( _this->FaceOnDefault == value )
    return;

  _this->FaceOnDefault = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::ToggleButtonConfig.OnSetFaceOffFrameActive()' */
void WidgetSetToggleButtonConfig_OnSetFaceOffFrameActive( WidgetSetToggleButtonConfig _this, 
  XInt32 value )
{
  if ( _this->FaceOffFrameActive == value )
    return;

  _this->FaceOffFrameActive = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::ToggleButtonConfig.OnSetFaceOffFrameFocused()' */
void WidgetSetToggleButtonConfig_OnSetFaceOffFrameFocused( WidgetSetToggleButtonConfig _this, 
  XInt32 value )
{
  if ( _this->FaceOffFrameFocused == value )
    return;

  _this->FaceOffFrameFocused = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::ToggleButtonConfig.OnSetFaceOffFrameDefault()' */
void WidgetSetToggleButtonConfig_OnSetFaceOffFrameDefault( WidgetSetToggleButtonConfig _this, 
  XInt32 value )
{
  if ( _this->FaceOffFrameDefault == value )
    return;

  _this->FaceOffFrameDefault = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::ToggleButtonConfig.OnSetFaceOffActive()' */
void WidgetSetToggleButtonConfig_OnSetFaceOffActive( WidgetSetToggleButtonConfig _this, 
  ResourcesBitmap value )
{
  if ( _this->FaceOffActive == value )
    return;

  _this->FaceOffActive = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::ToggleButtonConfig.OnSetFaceOffFocused()' */
void WidgetSetToggleButtonConfig_OnSetFaceOffFocused( WidgetSetToggleButtonConfig _this, 
  ResourcesBitmap value )
{
  if ( _this->FaceOffFocused == value )
    return;

  _this->FaceOffFocused = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::ToggleButtonConfig.OnSetFaceOffDefault()' */
void WidgetSetToggleButtonConfig_OnSetFaceOffDefault( WidgetSetToggleButtonConfig _this, 
  ResourcesBitmap value )
{
  if ( _this->FaceOffDefault == value )
    return;

  _this->FaceOffDefault = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::ToggleButtonConfig.OnSetWidgetMaxSize()' */
void WidgetSetToggleButtonConfig_OnSetWidgetMaxSize( WidgetSetToggleButtonConfig _this, 
  XPoint value )
{
  if ( !EwCompPoint( _this->WidgetMaxSize, value ))
    return;

  _this->WidgetMaxSize = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::ToggleButtonConfig.OnSetWidgetMinSize()' */
void WidgetSetToggleButtonConfig_OnSetWidgetMinSize( WidgetSetToggleButtonConfig _this, 
  XPoint value )
{
  if ( !EwCompPoint( _this->WidgetMinSize, value ))
    return;

  _this->WidgetMinSize = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* Variants derived from the class : 'WidgetSet::ToggleButtonConfig' */
EW_DEFINE_CLASS_VARIANTS( WidgetSetToggleButtonConfig )
EW_END_OF_CLASS_VARIANTS( WidgetSetToggleButtonConfig )

/* Virtual Method Table (VMT) for the class : 'WidgetSet::ToggleButtonConfig' */
EW_DEFINE_CLASS( WidgetSetToggleButtonConfig, WidgetSetWidgetConfig, LabelOnFont, 
                 PressedFeedbackDuration, PressedFeedbackDuration, PressedFeedbackDuration, 
                 PressedFeedbackDuration, PressedFeedbackDuration, "WidgetSet::ToggleButtonConfig" )
EW_END_OF_CLASS( WidgetSetToggleButtonConfig )

/* Initializer for the class 'WidgetSet::PushButtonConfig' */
void WidgetSetPushButtonConfig__Init( WidgetSetPushButtonConfig _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  WidgetSetWidgetConfig__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( WidgetSetPushButtonConfig );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( WidgetSetPushButtonConfig );

  /* ... and initialize objects, variables, properties, etc. */
  _this->PressedFeedbackDuration = 50;
  _this->KeyCode = CoreKeyCodeEnter;
  _this->LabelColorActive = _Const0001;
  _this->LabelColorFocused = _Const0001;
  _this->LabelColorDisabled = _Const0001;
  _this->LabelColorDefault = _Const0001;
  _this->IconTintActive = _Const0000;
  _this->IconTintFocused = _Const0000;
  _this->IconTintDisabled = _Const0000;
  _this->IconTintDefault = _Const0000;
  _this->LabelAlignment = ViewsTextAlignmentAlignHorzCenter | ViewsTextAlignmentAlignVertCenter;
  _this->IconAlignment = ViewsImageAlignmentAlignHorzCenter | ViewsImageAlignmentAlignVertCenter;
  _this->FaceLayout = CoreLayoutAlignToBottom | CoreLayoutAlignToLeft | CoreLayoutAlignToRight 
  | CoreLayoutAlignToTop | CoreLayoutResizeHorz | CoreLayoutResizeVert;
  _this->FaceTintActive = _Const0000;
  _this->FaceTintFocused = _Const0000;
  _this->FaceTintDisabled = _Const0000;
  _this->FaceTintDefault = _Const0000;
  _this->FaceFrameActive = -1;
  _this->FaceFrameFocused = -1;
  _this->FaceFrameDisabled = -1;
  _this->FaceFrameDefault = -1;
}

/* Re-Initializer for the class 'WidgetSet::PushButtonConfig' */
void WidgetSetPushButtonConfig__ReInit( WidgetSetPushButtonConfig _this )
{
  /* At first re-initialize the super class ... */
  WidgetSetWidgetConfig__ReInit( &_this->_Super );
}

/* Finalizer method for the class 'WidgetSet::PushButtonConfig' */
void WidgetSetPushButtonConfig__Done( WidgetSetPushButtonConfig _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( WidgetSetWidgetConfig );

  /* Don't forget to deinitialize the super class ... */
  WidgetSetWidgetConfig__Done( &_this->_Super );
}

/* 'C' function for method : 'WidgetSet::PushButtonConfig.OnSetLabelColorActive()' */
void WidgetSetPushButtonConfig_OnSetLabelColorActive( WidgetSetPushButtonConfig _this, 
  XColor value )
{
  if ( !EwCompColor( _this->LabelColorActive, value ))
    return;

  _this->LabelColorActive = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::PushButtonConfig.OnSetLabelColorFocused()' */
void WidgetSetPushButtonConfig_OnSetLabelColorFocused( WidgetSetPushButtonConfig _this, 
  XColor value )
{
  if ( !EwCompColor( _this->LabelColorFocused, value ))
    return;

  _this->LabelColorFocused = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::PushButtonConfig.OnSetLabelColorDefault()' */
void WidgetSetPushButtonConfig_OnSetLabelColorDefault( WidgetSetPushButtonConfig _this, 
  XColor value )
{
  if ( !EwCompColor( _this->LabelColorDefault, value ))
    return;

  _this->LabelColorDefault = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::PushButtonConfig.OnSetLabelMarginTop()' */
void WidgetSetPushButtonConfig_OnSetLabelMarginTop( WidgetSetPushButtonConfig _this, 
  XInt32 value )
{
  if ( _this->LabelMarginTop == value )
    return;

  _this->LabelMarginTop = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::PushButtonConfig.OnSetIconTintActive()' */
void WidgetSetPushButtonConfig_OnSetIconTintActive( WidgetSetPushButtonConfig _this, 
  XColor value )
{
  if ( !EwCompColor( _this->IconTintActive, value ))
    return;

  _this->IconTintActive = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::PushButtonConfig.OnSetIconTintDefault()' */
void WidgetSetPushButtonConfig_OnSetIconTintDefault( WidgetSetPushButtonConfig _this, 
  XColor value )
{
  if ( !EwCompColor( _this->IconTintDefault, value ))
    return;

  _this->IconTintDefault = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::PushButtonConfig.OnSetIconMarginBottom()' */
void WidgetSetPushButtonConfig_OnSetIconMarginBottom( WidgetSetPushButtonConfig _this, 
  XInt32 value )
{
  if ( _this->IconMarginBottom == value )
    return;

  _this->IconMarginBottom = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::PushButtonConfig.OnSetLabelAlignment()' */
void WidgetSetPushButtonConfig_OnSetLabelAlignment( WidgetSetPushButtonConfig _this, 
  XSet value )
{
  if ( _this->LabelAlignment == value )
    return;

  _this->LabelAlignment = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::PushButtonConfig.OnSetLabelFont()' */
void WidgetSetPushButtonConfig_OnSetLabelFont( WidgetSetPushButtonConfig _this, 
  ResourcesFont value )
{
  if ( _this->LabelFont == value )
    return;

  _this->LabelFont = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::PushButtonConfig.OnSetIconAlignment()' */
void WidgetSetPushButtonConfig_OnSetIconAlignment( WidgetSetPushButtonConfig _this, 
  XSet value )
{
  if ( _this->IconAlignment == value )
    return;

  _this->IconAlignment = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::PushButtonConfig.OnSetFaceTintActive()' */
void WidgetSetPushButtonConfig_OnSetFaceTintActive( WidgetSetPushButtonConfig _this, 
  XColor value )
{
  if ( !EwCompColor( _this->FaceTintActive, value ))
    return;

  _this->FaceTintActive = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::PushButtonConfig.OnSetFaceTintFocused()' */
void WidgetSetPushButtonConfig_OnSetFaceTintFocused( WidgetSetPushButtonConfig _this, 
  XColor value )
{
  if ( !EwCompColor( _this->FaceTintFocused, value ))
    return;

  _this->FaceTintFocused = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::PushButtonConfig.OnSetFaceTintDisabled()' */
void WidgetSetPushButtonConfig_OnSetFaceTintDisabled( WidgetSetPushButtonConfig _this, 
  XColor value )
{
  if ( !EwCompColor( _this->FaceTintDisabled, value ))
    return;

  _this->FaceTintDisabled = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::PushButtonConfig.OnSetFaceTintDefault()' */
void WidgetSetPushButtonConfig_OnSetFaceTintDefault( WidgetSetPushButtonConfig _this, 
  XColor value )
{
  if ( !EwCompColor( _this->FaceTintDefault, value ))
    return;

  _this->FaceTintDefault = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::PushButtonConfig.OnSetFaceFrameActive()' */
void WidgetSetPushButtonConfig_OnSetFaceFrameActive( WidgetSetPushButtonConfig _this, 
  XInt32 value )
{
  if ( _this->FaceFrameActive == value )
    return;

  _this->FaceFrameActive = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::PushButtonConfig.OnSetFaceFrameDisabled()' */
void WidgetSetPushButtonConfig_OnSetFaceFrameDisabled( WidgetSetPushButtonConfig _this, 
  XInt32 value )
{
  if ( _this->FaceFrameDisabled == value )
    return;

  _this->FaceFrameDisabled = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::PushButtonConfig.OnSetFaceFrameDefault()' */
void WidgetSetPushButtonConfig_OnSetFaceFrameDefault( WidgetSetPushButtonConfig _this, 
  XInt32 value )
{
  if ( _this->FaceFrameDefault == value )
    return;

  _this->FaceFrameDefault = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::PushButtonConfig.OnSetFaceActive()' */
void WidgetSetPushButtonConfig_OnSetFaceActive( WidgetSetPushButtonConfig _this, 
  ResourcesBitmap value )
{
  if ( _this->FaceActive == value )
    return;

  _this->FaceActive = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::PushButtonConfig.OnSetFaceFocused()' */
void WidgetSetPushButtonConfig_OnSetFaceFocused( WidgetSetPushButtonConfig _this, 
  ResourcesBitmap value )
{
  if ( _this->FaceFocused == value )
    return;

  _this->FaceFocused = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::PushButtonConfig.OnSetFaceDisabled()' */
void WidgetSetPushButtonConfig_OnSetFaceDisabled( WidgetSetPushButtonConfig _this, 
  ResourcesBitmap value )
{
  if ( _this->FaceDisabled == value )
    return;

  _this->FaceDisabled = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::PushButtonConfig.OnSetFaceDefault()' */
void WidgetSetPushButtonConfig_OnSetFaceDefault( WidgetSetPushButtonConfig _this, 
  ResourcesBitmap value )
{
  if ( _this->FaceDefault == value )
    return;

  _this->FaceDefault = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::PushButtonConfig.OnSetWidgetMaxSize()' */
void WidgetSetPushButtonConfig_OnSetWidgetMaxSize( WidgetSetPushButtonConfig _this, 
  XPoint value )
{
  if ( !EwCompPoint( _this->WidgetMaxSize, value ))
    return;

  _this->WidgetMaxSize = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::PushButtonConfig.OnSetWidgetMinSize()' */
void WidgetSetPushButtonConfig_OnSetWidgetMinSize( WidgetSetPushButtonConfig _this, 
  XPoint value )
{
  if ( !EwCompPoint( _this->WidgetMinSize, value ))
    return;

  _this->WidgetMinSize = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* Variants derived from the class : 'WidgetSet::PushButtonConfig' */
EW_DEFINE_CLASS_VARIANTS( WidgetSetPushButtonConfig )
EW_END_OF_CLASS_VARIANTS( WidgetSetPushButtonConfig )

/* Virtual Method Table (VMT) for the class : 'WidgetSet::PushButtonConfig' */
EW_DEFINE_CLASS( WidgetSetPushButtonConfig, WidgetSetWidgetConfig, LabelFont, PressedFeedbackDuration, 
                 PressedFeedbackDuration, PressedFeedbackDuration, PressedFeedbackDuration, 
                 PressedFeedbackDuration, "WidgetSet::PushButtonConfig" )
EW_END_OF_CLASS( WidgetSetPushButtonConfig )

/* Initializer for the class 'WidgetSet::WidgetConfig' */
void WidgetSetWidgetConfig__Init( WidgetSetWidgetConfig _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  XObject__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( WidgetSetWidgetConfig );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( WidgetSetWidgetConfig );
}

/* Re-Initializer for the class 'WidgetSet::WidgetConfig' */
void WidgetSetWidgetConfig__ReInit( WidgetSetWidgetConfig _this )
{
  /* At first re-initialize the super class ... */
  XObject__ReInit( &_this->_Super );
}

/* Finalizer method for the class 'WidgetSet::WidgetConfig' */
void WidgetSetWidgetConfig__Done( WidgetSetWidgetConfig _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( XObject );

  /* Don't forget to deinitialize the super class ... */
  XObject__Done( &_this->_Super );
}

/* 'C' function for method : 'WidgetSet::WidgetConfig.onInvalidate()' */
void WidgetSetWidgetConfig_onInvalidate( WidgetSetWidgetConfig _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  EwNotifyObjObservers((XObject)_this, 0 );
}

/* Variants derived from the class : 'WidgetSet::WidgetConfig' */
EW_DEFINE_CLASS_VARIANTS( WidgetSetWidgetConfig )
EW_END_OF_CLASS_VARIANTS( WidgetSetWidgetConfig )

/* Virtual Method Table (VMT) for the class : 'WidgetSet::WidgetConfig' */
EW_DEFINE_CLASS( WidgetSetWidgetConfig, XObject, _None, _None, _None, _None, _None, 
                 _None, "WidgetSet::WidgetConfig" )
EW_END_OF_CLASS( WidgetSetWidgetConfig )

/* Initializer for the class 'WidgetSet::RotaryKnob' */
void WidgetSetRotaryKnob__Init( WidgetSetRotaryKnob _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( WidgetSetRotaryKnob );

  /* ... then construct all embedded objects */
  CoreTimer__Init( &_this->RepetitionTimer, &_this->_XObject, 0 );
  CoreKeyPressHandler__Init( &_this->KeyHandlerRight, &_this->_XObject, 0 );
  CoreKeyPressHandler__Init( &_this->KeyHandlerLeft, &_this->_XObject, 0 );
  CoreRotateTouchHandler__Init( &_this->TouchHandler, &_this->_XObject, 0 );
  CoreSimpleTouchHandler__Init( &_this->TouchHandlerThumb, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( WidgetSetRotaryKnob );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const0002 );
  CoreTimer_OnSetPeriod( &_this->RepetitionTimer, 0 );
  CoreTimer_OnSetBegin( &_this->RepetitionTimer, 50 );
  _this->KeyHandlerRight.Filter = CoreKeyCodeRight;
  _this->KeyHandlerLeft.Filter = CoreKeyCodeLeft;
  CoreView_OnSetLayout((CoreView)&_this->TouchHandler, CoreLayoutAlignToBottom | 
  CoreLayoutAlignToLeft | CoreLayoutAlignToRight | CoreLayoutAlignToTop | CoreLayoutResizeHorz 
  | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->TouchHandler, _Const0002 );
  CoreQuadView__OnSetPoint4( &_this->TouchHandlerThumb, _Const0003 );
  CoreQuadView__OnSetPoint3( &_this->TouchHandlerThumb, _Const0004 );
  CoreQuadView__OnSetPoint2( &_this->TouchHandlerThumb, _Const0005 );
  CoreQuadView__OnSetPoint1( &_this->TouchHandlerThumb, _Const0006 );
  CoreSimpleTouchHandler_OnSetEnabled( &_this->TouchHandlerThumb, 0 );
  _this->StepSize = 1;
  _this->MaxValue = 100;
  _this->CurrentValue = 50;
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->TouchHandler ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->TouchHandlerThumb ), 0 );
  _this->RepetitionTimer.OnTrigger = EwNewSlot( _this, WidgetSetRotaryKnob_onRepetitionTimer );
  _this->KeyHandlerRight.OnRelease = EwNewSlot( _this, WidgetSetRotaryKnob_onReleaseKey );
  _this->KeyHandlerRight.OnPress = EwNewSlot( _this, WidgetSetRotaryKnob_onPressKey );
  _this->KeyHandlerLeft.OnRelease = EwNewSlot( _this, WidgetSetRotaryKnob_onReleaseKey );
  _this->KeyHandlerLeft.OnPress = EwNewSlot( _this, WidgetSetRotaryKnob_onPressKey );
  _this->TouchHandler.OnRotate = EwNewSlot( _this, WidgetSetRotaryKnob_onRotateTouch );
  _this->TouchHandler.OnEnd = EwNewSlot( _this, WidgetSetRotaryKnob_onReleaseTouch );
  _this->TouchHandler.OnStart = EwNewSlot( _this, WidgetSetRotaryKnob_onPressTouch );
  _this->TouchHandlerThumb.OnPress = EwNewSlot( _this, WidgetSetRotaryKnob_onPressTouchThumb );
}

/* Re-Initializer for the class 'WidgetSet::RotaryKnob' */
void WidgetSetRotaryKnob__ReInit( WidgetSetRotaryKnob _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  CoreTimer__ReInit( &_this->RepetitionTimer );
  CoreKeyPressHandler__ReInit( &_this->KeyHandlerRight );
  CoreKeyPressHandler__ReInit( &_this->KeyHandlerLeft );
  CoreRotateTouchHandler__ReInit( &_this->TouchHandler );
  CoreSimpleTouchHandler__ReInit( &_this->TouchHandlerThumb );
}

/* Finalizer method for the class 'WidgetSet::RotaryKnob' */
void WidgetSetRotaryKnob__Done( WidgetSetRotaryKnob _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreGroup );

  /* Finalize all embedded objects */
  CoreTimer__Done( &_this->RepetitionTimer );
  CoreKeyPressHandler__Done( &_this->KeyHandlerRight );
  CoreKeyPressHandler__Done( &_this->KeyHandlerLeft );
  CoreRotateTouchHandler__Done( &_this->TouchHandler );
  CoreSimpleTouchHandler__Done( &_this->TouchHandlerThumb );

  /* Don't forget to deinitialize the super class ... */
  CoreGroup__Done( &_this->_Super );
}

/* 'C' function for method : 'WidgetSet::RotaryKnob.OnSetBounds()' */
void WidgetSetRotaryKnob_OnSetBounds( WidgetSetRotaryKnob _this, XRect value )
{
  if ( EwCompRect( _this->Super2.Bounds, value ))
    EwPostSignal( _this->OnUpdate, ((XObject)_this ));

  if ( _this->Appearance != 0 )
  {
    XPoint cs = EwGetRectSize( value );
    XPoint ns = cs;
    XPoint delta;

    if ( ns.X < _this->Appearance->WidgetMinSize.X )
      ns.X = _this->Appearance->WidgetMinSize.X;

    if ( ns.Y < _this->Appearance->WidgetMinSize.Y )
      ns.Y = _this->Appearance->WidgetMinSize.Y;

    if (( _this->Appearance->WidgetMaxSize.X > 0 ) && ( ns.X > _this->Appearance->WidgetMaxSize.X ))
      ns.X = _this->Appearance->WidgetMaxSize.X;

    if (( _this->Appearance->WidgetMaxSize.Y > 0 ) && ( ns.Y > _this->Appearance->WidgetMaxSize.Y ))
      ns.Y = _this->Appearance->WidgetMaxSize.Y;

    delta = EwMovePointNeg( ns, cs );

    if ( delta.X != 0 )
    {
      XBool alignToLeft = (( _this->Super3.Layout & CoreLayoutAlignToLeft ) == CoreLayoutAlignToLeft );
      XBool alignToRight = (( _this->Super3.Layout & CoreLayoutAlignToRight ) == 
        CoreLayoutAlignToRight );

      if ( alignToLeft && !alignToRight )
        value.Point2.X = ( value.Point2.X + delta.X );
      else
        if ( !alignToLeft && alignToRight )
          value.Point1.X = ( value.Point1.X - delta.X );
        else
        {
          value.Point1.X = ( value.Point1.X - ( delta.X / 2 ));
          value.Point2.X = ( value.Point1.X + ns.X );
        }
    }

    if ( delta.Y != 0 )
    {
      XBool alignToTop = (( _this->Super3.Layout & CoreLayoutAlignToTop ) == CoreLayoutAlignToTop );
      XBool alignToBottom = (( _this->Super3.Layout & CoreLayoutAlignToBottom ) 
        == CoreLayoutAlignToBottom );

      if ( alignToTop && !alignToBottom )
        value.Point2.Y = ( value.Point2.Y + delta.Y );
      else
        if ( !alignToTop && alignToBottom )
          value.Point1.Y = ( value.Point1.Y - delta.Y );
        else
        {
          value.Point1.Y = ( value.Point1.Y - ( delta.Y / 2 ));
          value.Point2.Y = ( value.Point1.Y + ns.Y );
        }
    }
  }

  CoreGroup_OnSetBounds((CoreGroup)_this, value );
}

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
void WidgetSetRotaryKnob_UpdateViewState( WidgetSetRotaryKnob _this, XSet aState )
{
  XBool needsScale;
  XBool needsKnob;
  XBool needsCover;
  XBool needsPath1;
  XBool needsPath2;
  XBool restack;
  XRect area;
  XFloat angleMin;
  XFloat angleMax;
  XFloat angle;
  XBool isEnabled;
  XBool isFocused;
  XBool isPressed;

  CoreGroup_UpdateViewState((CoreGroup)_this, aState );
  needsScale = (XBool)(( _this->Appearance != 0 ) && (((( _this->Appearance->ScaleDefault 
  != 0 ) || ( _this->Appearance->ScaleDisabled != 0 )) || ( _this->Appearance->ScaleFocused 
  != 0 )) || ( _this->Appearance->ScaleActive != 0 )));
  needsKnob = (XBool)(( _this->Appearance != 0 ) && (((( _this->Appearance->ThumbDefault 
  != 0 ) || ( _this->Appearance->ThumbDisabled != 0 )) || ( _this->Appearance->ThumbFocused 
  != 0 )) || ( _this->Appearance->ThumbActive != 0 )));
  needsCover = (XBool)(( _this->Appearance != 0 ) && (((( _this->Appearance->CoverDefault 
  != 0 ) || ( _this->Appearance->CoverDisabled != 0 )) || ( _this->Appearance->CoverFocused 
  != 0 )) || ( _this->Appearance->CoverActive != 0 )));
  needsPath1 = (XBool)(( _this->Appearance != 0 ) && (((( _this->Appearance->TrackLeftThicknessDefault 
  > 0.000000f ) || ( _this->Appearance->TrackLeftThicknessDisabled > 0.000000f )) 
  || ( _this->Appearance->TrackLeftThicknessFocused > 0.000000f )) || ( _this->Appearance->TrackLeftThicknessActive 
  > 0.000000f )));
  needsPath2 = (XBool)(( _this->Appearance != 0 ) && (((( _this->Appearance->TrackRightThicknessDefault 
  > 0.000000f ) || ( _this->Appearance->TrackRightThicknessDisabled > 0.000000f )) 
  || ( _this->Appearance->TrackRightThicknessFocused > 0.000000f )) || ( _this->Appearance->TrackRightThicknessActive 
  > 0.000000f )));
  restack = 0;
  area = EwGetRectORect( _this->Super2.Bounds );
  angleMin = 135.000000f;
  angleMax = 45.000000f;
  angle = angleMin;

  if ( needsScale && ( _this->imageView1 == 0 ))
  {
    _this->imageView1 = EwNewObject( ViewsImage, 0 );
    CoreGroup_Add((CoreGroup)_this, ((CoreView)_this->imageView1 ), 0 );
    restack = 1;
  }
  else
    if ( !needsScale && ( _this->imageView1 != 0 ))
    {
      CoreGroup_Remove((CoreGroup)_this, ((CoreView)_this->imageView1 ));
      _this->imageView1 = 0;
    }

  if ( needsCover && ( _this->imageView2 == 0 ))
  {
    _this->imageView2 = EwNewObject( ViewsImage, 0 );
    CoreGroup_Add((CoreGroup)_this, ((CoreView)_this->imageView2 ), 0 );
    restack = 1;
  }
  else
    if ( !needsCover && ( _this->imageView2 != 0 ))
    {
      CoreGroup_Remove((CoreGroup)_this, ((CoreView)_this->imageView2 ));
      _this->imageView2 = 0;
    }

  if ( needsKnob && ( _this->warpView == 0 ))
  {
    _this->warpView = EwNewObject( ViewsWarpImage, 0 );
    CoreGroup_Add((CoreGroup)_this, ((CoreView)_this->warpView ), 0 );
    restack = 1;
  }
  else
    if ( !needsKnob && ( _this->warpView != 0 ))
    {
      CoreGroup_Remove((CoreGroup)_this, ((CoreView)_this->warpView ));
      _this->warpView = 0;
    }

  if ( needsPath1 && ( _this->path1 == 0 ))
  {
    _this->path1 = EwNewObject( GraphicsArcPath, 0 );
    _this->pathView1 = EwNewObject( ViewsFillPath, 0 );
    CoreGroup_Add((CoreGroup)_this, ((CoreView)_this->pathView1 ), 0 );
    ViewsFillPath_OnSetPath( _this->pathView1, ((GraphicsPath)_this->path1 ));
    ViewsFillPath_OnSetFillRule( _this->pathView1, GraphicsFillRuleNonZero );
    restack = 1;
  }
  else
    if ( !needsPath1 && ( _this->path1 != 0 ))
    {
      CoreGroup_Remove((CoreGroup)_this, ((CoreView)_this->pathView1 ));
      _this->pathView1 = 0;
      _this->path1 = 0;
    }

  if ( needsPath2 && ( _this->path2 == 0 ))
  {
    _this->path2 = EwNewObject( GraphicsArcPath, 0 );
    _this->pathView2 = EwNewObject( ViewsFillPath, 0 );
    CoreGroup_Add((CoreGroup)_this, ((CoreView)_this->pathView2 ), 0 );
    ViewsFillPath_OnSetPath( _this->pathView2, ((GraphicsPath)_this->path2 ));
    ViewsFillPath_OnSetFillRule( _this->pathView2, GraphicsFillRuleNonZero );
    restack = 1;
  }
  else
    if ( !needsPath2 && ( _this->path2 != 0 ))
    {
      CoreGroup_Remove((CoreGroup)_this, ((CoreView)_this->pathView2 ));
      _this->pathView2 = 0;
      _this->path2 = 0;
    }

  if ( restack )
  {
    if ( _this->imageView1 != 0 )
      CoreGroup_RestackTop((CoreGroup)_this, ((CoreView)_this->imageView1 ));

    if ( _this->pathView1 != 0 )
      CoreGroup_RestackTop((CoreGroup)_this, ((CoreView)_this->pathView1 ));

    if ( _this->pathView2 != 0 )
      CoreGroup_RestackTop((CoreGroup)_this, ((CoreView)_this->pathView2 ));

    if ( _this->warpView != 0 )
      CoreGroup_RestackTop((CoreGroup)_this, ((CoreView)_this->warpView ));

    if ( _this->imageView2 != 0 )
      CoreGroup_RestackTop((CoreGroup)_this, ((CoreView)_this->imageView2 ));
  }

  if ( _this->Appearance != 0 )
  {
    angleMin = _this->Appearance->ThumbMinAngle;
    angleMax = _this->Appearance->ThumbMaxAngle;
  }

  isEnabled = (( aState & CoreViewStateEnabled ) == CoreViewStateEnabled );
  isFocused = (( aState & CoreViewStateFocused ) == CoreViewStateFocused );
  isPressed = (XBool)(( _this->TouchHandler.Down || _this->KeyHandlerLeft.Down ) 
  || _this->KeyHandlerRight.Down );

  if ( _this->imageView1 != 0 )
  {
    ResourcesBitmap bitmap = 0;
    XInt32 frameNo = -1;
    XColor tint = _Const0000;

    if ( !isEnabled )
    {
      bitmap = _this->Appearance->ScaleDisabled;
      frameNo = _this->Appearance->ScaleFrameDisabled;
      tint = _this->Appearance->ScaleTintDisabled;
    }
    else
      if ( isPressed )
      {
        bitmap = _this->Appearance->ScaleActive;
        frameNo = _this->Appearance->ScaleFrameActive;
        tint = _this->Appearance->ScaleTintActive;
      }
      else
        if ( isFocused )
        {
          bitmap = _this->Appearance->ScaleFocused;
          frameNo = _this->Appearance->ScaleFrameFocused;
          tint = _this->Appearance->ScaleTintFocused;
        }
        else
        {
          bitmap = _this->Appearance->ScaleDefault;
          frameNo = _this->Appearance->ScaleFrameDefault;
          tint = _this->Appearance->ScaleTintDefault;
        }

    CoreRectView__OnSetBounds( _this->imageView1, area );
    ViewsImage_OnSetAnimated( _this->imageView1, (XBool)( frameNo < 0 ));
    ViewsImage_OnSetColor( _this->imageView1, tint );

    if ( frameNo < 0 )
      frameNo = 0;

    ViewsImage_OnSetFrameNumber( _this->imageView1, frameNo );
    ViewsImage_OnSetBitmap( _this->imageView1, bitmap );
  }

  if ( _this->imageView2 != 0 )
  {
    ResourcesBitmap bitmap = 0;
    XInt32 frameNo = -1;
    XColor tint = _Const0000;

    if ( !isEnabled )
    {
      bitmap = _this->Appearance->CoverDisabled;
      frameNo = _this->Appearance->CoverFrameDisabled;
      tint = _this->Appearance->CoverTintDisabled;
    }
    else
      if ( isPressed )
      {
        bitmap = _this->Appearance->CoverActive;
        frameNo = _this->Appearance->CoverFrameActive;
        tint = _this->Appearance->CoverTintActive;
      }
      else
        if ( isFocused )
        {
          bitmap = _this->Appearance->CoverFocused;
          frameNo = _this->Appearance->CoverFrameFocused;
          tint = _this->Appearance->CoverTintFocused;
        }
        else
        {
          bitmap = _this->Appearance->CoverDefault;
          frameNo = _this->Appearance->CoverFrameDefault;
          tint = _this->Appearance->CoverTintDefault;
        }

    CoreRectView__OnSetBounds( _this->imageView2, area );
    ViewsImage_OnSetAnimated( _this->imageView2, (XBool)( frameNo < 0 ));
    ViewsImage_OnSetColor( _this->imageView2, tint );

    if ( frameNo < 0 )
      frameNo = 0;

    ViewsImage_OnSetFrameNumber( _this->imageView2, frameNo );
    ViewsImage_OnSetBitmap( _this->imageView2, bitmap );
  }

  if ( _this->MaxValue != _this->MinValue )
    angle = (((XFloat)( WidgetSetRotaryKnob_OnGetCurrentValue( _this ) - _this->MinValue ) 
    * ( angleMax - angleMin )) / (XFloat)( _this->MaxValue - _this->MinValue )) 
    + angleMin;

  if ( _this->warpView != 0 )
  {
    ResourcesBitmap bitmap = 0;
    XInt32 frameNo = -1;
    XColor tint = _Const0000;
    GraphicsWarpMatrix matrix;

    if ( !isEnabled )
    {
      bitmap = _this->Appearance->ThumbDisabled;
      frameNo = _this->Appearance->ThumbFrameDisabled;
      tint = _this->Appearance->ThumbTintDisabled;
    }
    else
      if ( isPressed )
      {
        bitmap = _this->Appearance->ThumbActive;
        frameNo = _this->Appearance->ThumbFrameActive;
        tint = _this->Appearance->ThumbTintActive;
      }
      else
        if ( isFocused )
        {
          bitmap = _this->Appearance->ThumbFocused;
          frameNo = _this->Appearance->ThumbFrameFocused;
          tint = _this->Appearance->ThumbTintFocused;
        }
        else
        {
          bitmap = _this->Appearance->ThumbDefault;
          frameNo = _this->Appearance->ThumbFrameDefault;
          tint = _this->Appearance->ThumbTintDefault;
        }

    ViewsWarpImage_OnSetAnimated( _this->warpView, (XBool)( frameNo < 0 ));
    ViewsWarpImage_OnSetColor( _this->warpView, tint );

    if ( frameNo < 0 )
      frameNo = 0;

    matrix = EwNewObject( GraphicsWarpMatrix, 0 );
    GraphicsWarpMatrix_Rotate( matrix, 0.000000f, 0.000000f, -angle );

    if ( _this->Appearance->ThumbRadius != 0.000000f )
      GraphicsWarpMatrix_Translate( matrix, _this->Appearance->ThumbRadius, 0.000000f, 
      0.000000f );

    if ( !_this->Appearance->ThumbRotate )
      GraphicsWarpMatrix_Rotate( matrix, 0.000000f, 0.000000f, angle );

    ViewsWarpImage_OnSetFrameNumber( _this->warpView, frameNo );
    ViewsWarpImage_OnSetBitmap( _this->warpView, bitmap );
    ViewsWarpView_OnSetSourceAnchor((ViewsWarpView)_this->warpView, _this->Appearance->ThumbPivot );
    ViewsWarpView_Warp3D((ViewsWarpView)_this->warpView, EwMovePointPos( EwGetRectCenter( 
    area ), _this->Appearance->CenterOffset ), matrix );
  }

  if ( _this->pathView1 != 0 )
  {
    XColor clr = _Const0000;

    if ( !isEnabled )
      clr = _this->Appearance->TrackLeftColorDisabled;
    else
      if ( isPressed )
        clr = _this->Appearance->TrackLeftColorActive;
      else
        if ( isFocused )
          clr = _this->Appearance->TrackLeftColorFocused;
        else
          clr = _this->Appearance->TrackLeftColorDefault;

    CoreRectView__OnSetBounds( _this->pathView1, area );
    ViewsFillPath_OnSetOffset( _this->pathView1, EwMovePointPos( EwGetRectCenter( 
    area ), _this->Appearance->CenterOffset ));
    ViewsFillPath_OnSetColor( _this->pathView1, clr );
  }

  if ( _this->pathView2 != 0 )
  {
    XColor clr = _Const0000;

    if ( !isEnabled )
      clr = _this->Appearance->TrackRightColorDisabled;
    else
      if ( isPressed )
        clr = _this->Appearance->TrackRightColorActive;
      else
        if ( isFocused )
          clr = _this->Appearance->TrackRightColorFocused;
        else
          clr = _this->Appearance->TrackRightColorDefault;

    CoreRectView__OnSetBounds( _this->pathView2, area );
    ViewsFillPath_OnSetOffset( _this->pathView2, EwMovePointPos( EwGetRectCenter( 
    area ), _this->Appearance->CenterOffset ));
    ViewsFillPath_OnSetColor( _this->pathView2, clr );
  }

  if ( _this->path1 != 0 )
  {
    XFloat radius = 0.000000f;
    XFloat thickness = 0.000000f;

    if ( !isEnabled )
    {
      radius = _this->Appearance->TrackLeftRadiusDisabled;
      thickness = _this->Appearance->TrackLeftThicknessDisabled;
    }
    else
      if ( isPressed )
      {
        radius = _this->Appearance->TrackLeftRadiusActive;
        thickness = _this->Appearance->TrackLeftThicknessActive;
      }
      else
        if ( isFocused )
        {
          radius = _this->Appearance->TrackLeftRadiusFocused;
          thickness = _this->Appearance->TrackLeftThicknessFocused;
        }
        else
        {
          radius = _this->Appearance->TrackLeftRadiusDefault;
          thickness = _this->Appearance->TrackLeftThicknessDefault;
        }

    GraphicsArcPath_OnSetStyle( _this->path1, GraphicsArcStylePie );
    GraphicsArcPath_OnSetInnerRadius( _this->path1, radius - ( thickness * 0.500000f ));
    GraphicsArcPath_OnSetRadius( _this->path1, GraphicsArcPath_OnGetInnerRadius( 
    _this->path1 ) + thickness );
    GraphicsArcPath_OnSetStartAngle( _this->path1, -_this->Appearance->ThumbMinAngle );
    GraphicsArcPath_OnSetEndAngle( _this->path1, -angle );

    if ( _this->Appearance->TrackLeftRoundedStart && _this->Appearance->TrackLeftRoundedEnd )
      GraphicsArcPath_OnSetStyle( _this->path1, GraphicsArcStylePieRounded );
    else
      if ( _this->Appearance->TrackLeftRoundedStart )
        GraphicsArcPath_OnSetStyle( _this->path1, GraphicsArcStylePieRoundedStart );
      else
        if ( _this->Appearance->TrackLeftRoundedEnd )
          GraphicsArcPath_OnSetStyle( _this->path1, GraphicsArcStylePieRoundedEnd );
        else
          GraphicsArcPath_OnSetStyle( _this->path1, GraphicsArcStylePie );
  }

  if ( _this->path2 != 0 )
  {
    XFloat radius = 0.000000f;
    XFloat thickness = 0.000000f;

    if ( !isEnabled )
    {
      radius = _this->Appearance->TrackRightRadiusDisabled;
      thickness = _this->Appearance->TrackRightThicknessDisabled;
    }
    else
      if ( isPressed )
      {
        radius = _this->Appearance->TrackRightRadiusActive;
        thickness = _this->Appearance->TrackRightThicknessActive;
      }
      else
        if ( isFocused )
        {
          radius = _this->Appearance->TrackRightRadiusFocused;
          thickness = _this->Appearance->TrackRightThicknessFocused;
        }
        else
        {
          radius = _this->Appearance->TrackRightRadiusDefault;
          thickness = _this->Appearance->TrackRightThicknessDefault;
        }

    GraphicsArcPath_OnSetStyle( _this->path2, GraphicsArcStylePie );
    GraphicsArcPath_OnSetInnerRadius( _this->path2, radius - ( thickness * 0.500000f ));
    GraphicsArcPath_OnSetRadius( _this->path2, GraphicsArcPath_OnGetInnerRadius( 
    _this->path2 ) + thickness );
    GraphicsArcPath_OnSetStartAngle( _this->path2, -angle );
    GraphicsArcPath_OnSetEndAngle( _this->path2, -_this->Appearance->ThumbMaxAngle );

    if ( _this->Appearance->TrackRightRoundedStart && _this->Appearance->TrackRightRoundedEnd )
      GraphicsArcPath_OnSetStyle( _this->path2, GraphicsArcStylePieRounded );
    else
      if ( _this->Appearance->TrackRightRoundedStart )
        GraphicsArcPath_OnSetStyle( _this->path2, GraphicsArcStylePieRoundedStart );
      else
        if ( _this->Appearance->TrackRightRoundedEnd )
          GraphicsArcPath_OnSetStyle( _this->path2, GraphicsArcStylePieRoundedEnd );
        else
          GraphicsArcPath_OnSetStyle( _this->path2, GraphicsArcStylePie );
  }

  if (( _this->warpView != 0 ) && ( _this->warpView->Bitmap != 0 ))
  {
    CoreQuadView__OnSetPoint1( &_this->TouchHandlerThumb, _this->warpView->Super2.Point1 );
    CoreQuadView__OnSetPoint2( &_this->TouchHandlerThumb, _this->warpView->Super2.Point2 );
    CoreQuadView__OnSetPoint3( &_this->TouchHandlerThumb, _this->warpView->Super2.Point3 );
    CoreQuadView__OnSetPoint4( &_this->TouchHandlerThumb, _this->warpView->Super2.Point4 );
    CoreSimpleTouchHandler_OnSetEnabled( &_this->TouchHandlerThumb, 1 );
    CoreRotateTouchHandler_OnSetEnabled( &_this->TouchHandler, 0 );
  }
  else
  {
    CoreSimpleTouchHandler_OnSetEnabled( &_this->TouchHandlerThumb, 0 );
    CoreRotateTouchHandler_OnSetEnabled( &_this->TouchHandler, 1 );
  }
}

/* 'C' function for method : 'WidgetSet::RotaryKnob.onConfigChanged()' */
void WidgetSetRotaryKnob_onConfigChanged( WidgetSetRotaryKnob _this, XObject sender )
{
  XEnum oldKeyCodeLeft;
  XEnum oldKeyCodeRight;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  oldKeyCodeLeft = _this->KeyHandlerLeft.Filter;
  oldKeyCodeRight = _this->KeyHandlerRight.Filter;

  if ( _this->Appearance != 0 )
  {
    _this->KeyHandlerLeft.Filter = _this->Appearance->KeyCodeLeft;
    _this->KeyHandlerRight.Filter = _this->Appearance->KeyCodeRight;
  }
  else
  {
    _this->KeyHandlerLeft.Filter = CoreKeyCodeLeft;
    _this->KeyHandlerRight.Filter = CoreKeyCodeRight;
  }

  if (((( oldKeyCodeLeft == CoreKeyCodeNoKey ) || ( oldKeyCodeRight == CoreKeyCodeNoKey )) 
      && ( _this->KeyHandlerLeft.Filter != CoreKeyCodeNoKey )) && ( _this->KeyHandlerRight.Filter 
      != CoreKeyCodeNoKey ))
    CoreView__ChangeViewState( _this, CoreViewStateFocusable, 0 );

  if ((( oldKeyCodeLeft != CoreKeyCodeNoKey ) && ( oldKeyCodeRight != CoreKeyCodeNoKey )) 
      && (( _this->KeyHandlerLeft.Filter == CoreKeyCodeNoKey ) || ( _this->KeyHandlerRight.Filter 
      == CoreKeyCodeNoKey )))
    CoreView__ChangeViewState( _this, 0, CoreViewStateFocusable );

  if ( _this->Appearance != 0 )
  {
    CoreTimer_OnSetBegin( &_this->RepetitionTimer, _this->Appearance->KeyRepeatDelay );
    CoreTimer_OnSetPeriod( &_this->RepetitionTimer, _this->Appearance->KeyRepeatPeriod );
  }
  else
  {
    CoreTimer_OnSetBegin( &_this->RepetitionTimer, 0 );
    CoreTimer_OnSetPeriod( &_this->RepetitionTimer, 0 );
  }

  if ( _this->Appearance != 0 )
    CoreRectView__OnSetBounds( _this, _this->Super2.Bounds );

  if ( _this->Appearance != 0 )
    CoreRotateTouchHandler_OnSetCenterOffset( &_this->TouchHandler, _this->Appearance->CenterOffset );
  else
    CoreRotateTouchHandler_OnSetCenterOffset( &_this->TouchHandler, _Const0003 );

  CoreGroup_InvalidateViewState((CoreGroup)_this );
  EwPostSignal( _this->OnUpdate, ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::RotaryKnob.onOutlet()' */
void WidgetSetRotaryKnob_onOutlet( WidgetSetRotaryKnob _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( _this->Outlet.Object != 0 )
    WidgetSetRotaryKnob_OnSetCurrentValue( _this, EwOnGetInt32( _this->Outlet ));
}

/* 'C' function for method : 'WidgetSet::RotaryKnob.onRepetitionTimer()' */
void WidgetSetRotaryKnob_onRepetitionTimer( WidgetSetRotaryKnob _this, XObject sender )
{
  XInt32 oldValue;
  XInt32 newValue;
  XInt32 delta;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  oldValue = WidgetSetRotaryKnob_OnGetCurrentValue( _this );
  newValue = oldValue;
  delta = _this->StepSize;

  if (( _this->KeyHandlerLeft.Down && ( _this->MaxValue > _this->MinValue )) || 
      ( _this->KeyHandlerRight.Down && ( _this->MaxValue < _this->MinValue )))
    delta = -delta;

  if ( oldValue < 0 )
  {
    newValue = -newValue;
    delta = -delta;
  }

  if ( delta > 0 )
    newValue = ( newValue + delta ) - (( newValue + delta ) % delta );

  if ( delta < 0 )
  {
    if (( newValue % -delta ) != 0 )
      newValue = ( newValue - delta ) - ( newValue % -delta );

    newValue = newValue + delta;
  }

  if ( oldValue < 0 )
    newValue = -newValue;

  WidgetSetRotaryKnob_OnSetCurrentValue( _this, newValue );

  if ( oldValue == WidgetSetRotaryKnob_OnGetCurrentValue( _this ))
    return;

  EwPostSignal( _this->OnChange, ((XObject)_this ));

  if ( _this->Outlet.Object != 0 )
  {
    EwOnSetInt32( _this->Outlet, WidgetSetRotaryKnob_OnGetCurrentValue( _this ));
    EwNotifyRefObservers( _this->Outlet, 0 );
  }
}

/* 'C' function for method : 'WidgetSet::RotaryKnob.onReleaseKey()' */
void WidgetSetRotaryKnob_onReleaseKey( WidgetSetRotaryKnob _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  CoreRotateTouchHandler_OnSetEnabled( &_this->TouchHandler, 1 );
  CoreGroup_InvalidateViewState((CoreGroup)_this );
  CoreTimer_OnSetEnabled( &_this->RepetitionTimer, 0 );
  EwPostSignal( _this->OnEnd, ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::RotaryKnob.onPressKey()' */
void WidgetSetRotaryKnob_onPressKey( WidgetSetRotaryKnob _this, XObject sender )
{
  XInt32 oldValue;
  XInt32 newValue;
  XInt32 delta;

  CoreRotateTouchHandler_OnSetEnabled( &_this->TouchHandler, 0 );
  CoreGroup_InvalidateViewState((CoreGroup)_this );
  EwPostSignal( _this->OnStart, ((XObject)_this ));
  CoreTimer_OnSetEnabled( &_this->RepetitionTimer, 1 );
  oldValue = WidgetSetRotaryKnob_OnGetCurrentValue( _this );
  newValue = oldValue;
  delta = _this->StepSize;

  if ((( sender == ((XObject)&_this->KeyHandlerLeft )) && ( _this->MaxValue > _this->MinValue )) 
      || (( sender == ((XObject)&_this->KeyHandlerRight )) && ( _this->MaxValue 
      < _this->MinValue )))
    delta = -delta;

  if ( oldValue < 0 )
  {
    newValue = -newValue;
    delta = -delta;
  }

  if ( delta > 0 )
    newValue = ( newValue + delta ) - (( newValue + delta ) % delta );

  if ( delta < 0 )
  {
    if (( newValue % -delta ) != 0 )
      newValue = ( newValue - delta ) - ( newValue % -delta );

    newValue = newValue + delta;
  }

  if ( oldValue < 0 )
    newValue = -newValue;

  WidgetSetRotaryKnob_OnSetCurrentValue( _this, newValue );

  if ( oldValue == WidgetSetRotaryKnob_OnGetCurrentValue( _this ))
    return;

  EwPostSignal( _this->OnChange, ((XObject)_this ));

  if ( _this->Outlet.Object != 0 )
  {
    EwOnSetInt32( _this->Outlet, WidgetSetRotaryKnob_OnGetCurrentValue( _this ));
    EwNotifyRefObservers( _this->Outlet, 0 );
  }
}

/* 'C' function for method : 'WidgetSet::RotaryKnob.onRotateTouch()' */
void WidgetSetRotaryKnob_onRotateTouch( WidgetSetRotaryKnob _this, XObject sender )
{
  XFloat angleMin;
  XFloat angleMax;
  XInt32 oldValue;
  XInt32 newValue;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  angleMin = 135.000000f;
  angleMax = 45.000000f;
  oldValue = WidgetSetRotaryKnob_OnGetCurrentValue( _this );
  newValue = oldValue;

  if ( _this->Appearance != 0 )
  {
    angleMin = _this->Appearance->ThumbMinAngle;
    angleMax = _this->Appearance->ThumbMaxAngle;
  }

  if ( angleMin > angleMax )
    newValue = (XInt32)((((XFloat)-_this->TouchHandler.Relative ) * (XFloat)( _this->MaxValue 
    - _this->MinValue )) / ( angleMin - angleMax )) + _this->touchStartValue;

  newValue = newValue - ( newValue % _this->StepSize );

  if (( newValue > _this->MaxValue ) && ( _this->MaxValue > _this->MinValue ))
    _this->touchStartValue = _this->touchStartValue - ( newValue - _this->MaxValue );

  if (( newValue > _this->MinValue ) && ( _this->MinValue > _this->MaxValue ))
    _this->touchStartValue = _this->touchStartValue - ( newValue - _this->MinValue );

  if (( newValue < _this->MinValue ) && ( _this->MaxValue > _this->MinValue ))
    _this->touchStartValue = _this->touchStartValue - ( newValue - _this->MinValue );

  if (( newValue < _this->MaxValue ) && ( _this->MinValue > _this->MaxValue ))
    _this->touchStartValue = _this->touchStartValue - ( newValue - _this->MaxValue );

  WidgetSetRotaryKnob_OnSetCurrentValue( _this, newValue );

  if ( oldValue == WidgetSetRotaryKnob_OnGetCurrentValue( _this ))
    return;

  EwPostSignal( _this->OnChange, ((XObject)_this ));

  if ( _this->Outlet.Object != 0 )
  {
    EwOnSetInt32( _this->Outlet, WidgetSetRotaryKnob_OnGetCurrentValue( _this ));
    EwNotifyRefObservers( _this->Outlet, 0 );
  }
}

/* 'C' function for method : 'WidgetSet::RotaryKnob.onReleaseTouch()' */
void WidgetSetRotaryKnob_onReleaseTouch( WidgetSetRotaryKnob _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  _this->KeyHandlerLeft.Enabled = 1;
  _this->KeyHandlerRight.Enabled = 1;
  CoreGroup_InvalidateViewState((CoreGroup)_this );
  EwPostSignal( _this->OnEnd, ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::RotaryKnob.onPressTouch()' */
void WidgetSetRotaryKnob_onPressTouch( WidgetSetRotaryKnob _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  _this->KeyHandlerLeft.Enabled = 0;
  _this->KeyHandlerRight.Enabled = 0;
  EwPostSignal( _this->OnStart, ((XObject)_this ));
  CoreGroup_InvalidateViewState((CoreGroup)_this );
  _this->touchStartValue = WidgetSetRotaryKnob_OnGetCurrentValue( _this );
}

/* 'C' function for method : 'WidgetSet::RotaryKnob.onPressTouchThumb()' */
void WidgetSetRotaryKnob_onPressTouchThumb( WidgetSetRotaryKnob _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  CoreRoot_DeflectCursor( CoreView__GetRoot( _this ), ((CoreView)&_this->TouchHandler ), 
  _Const0003 );
}

/* 'C' function for method : 'WidgetSet::RotaryKnob.OnSetOutlet()' */
void WidgetSetRotaryKnob_OnSetOutlet( WidgetSetRotaryKnob _this, XRef value )
{
  if ( !EwCompRef( _this->Outlet, value ))
    return;

  if ( _this->Outlet.Object != 0 )
    EwDetachRefObserver( EwNewSlot( _this, WidgetSetRotaryKnob_onOutlet ), _this->Outlet, 
      0 );

  _this->Outlet = value;

  if ( value.Object != 0 )
    EwAttachRefObserver( EwNewSlot( _this, WidgetSetRotaryKnob_onOutlet ), value, 
      0 );

  if ( value.Object != 0 )
    EwPostSignal( EwNewSlot( _this, WidgetSetRotaryKnob_onOutlet ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::RotaryKnob.OnSetMaxValue()' */
void WidgetSetRotaryKnob_OnSetMaxValue( WidgetSetRotaryKnob _this, XInt32 value )
{
  if ( _this->MaxValue == value )
    return;

  _this->MaxValue = value;
  CoreGroup_InvalidateViewState((CoreGroup)_this );
  EwPostSignal( _this->OnUpdate, ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::RotaryKnob.OnSetMinValue()' */
void WidgetSetRotaryKnob_OnSetMinValue( WidgetSetRotaryKnob _this, XInt32 value )
{
  if ( _this->MinValue == value )
    return;

  _this->MinValue = value;
  CoreGroup_InvalidateViewState((CoreGroup)_this );
  EwPostSignal( _this->OnUpdate, ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::RotaryKnob.OnGetCurrentValue()' */
XInt32 WidgetSetRotaryKnob_OnGetCurrentValue( WidgetSetRotaryKnob _this )
{
  XInt32 value = _this->CurrentValue;

  if ( _this->MinValue > _this->MaxValue )
  {
    if ( value < _this->MaxValue )
      value = _this->MaxValue;

    if ( value > _this->MinValue )
      value = _this->MinValue;
  }
  else
  {
    if ( value < _this->MinValue )
      value = _this->MinValue;

    if ( value > _this->MaxValue )
      value = _this->MaxValue;
  }

  return value;
}

/* 'C' function for method : 'WidgetSet::RotaryKnob.OnSetCurrentValue()' */
void WidgetSetRotaryKnob_OnSetCurrentValue( WidgetSetRotaryKnob _this, XInt32 value )
{
  if ( _this->CurrentValue == value )
    return;

  _this->CurrentValue = value;
  CoreGroup_InvalidateViewState((CoreGroup)_this );
  EwPostSignal( _this->OnUpdate, ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::RotaryKnob.OnSetAppearance()' */
void WidgetSetRotaryKnob_OnSetAppearance( WidgetSetRotaryKnob _this, WidgetSetRotaryKnobConfig 
  value )
{
  if ( _this->Appearance == value )
    return;

  if ( _this->Appearance != 0 )
    EwDetachObjObserver( EwNewSlot( _this, WidgetSetRotaryKnob_onConfigChanged ), 
      (XObject)_this->Appearance, 0 );

  _this->Appearance = value;

  if ( value != 0 )
    EwAttachObjObserver( EwNewSlot( _this, WidgetSetRotaryKnob_onConfigChanged ), 
      (XObject)value, 0 );

  EwPostSignal( EwNewSlot( _this, WidgetSetRotaryKnob_onConfigChanged ), ((XObject)_this ));
}

/* Variants derived from the class : 'WidgetSet::RotaryKnob' */
EW_DEFINE_CLASS_VARIANTS( WidgetSetRotaryKnob )
EW_END_OF_CLASS_VARIANTS( WidgetSetRotaryKnob )

/* Virtual Method Table (VMT) for the class : 'WidgetSet::RotaryKnob' */
EW_DEFINE_CLASS( WidgetSetRotaryKnob, CoreGroup, pathView1, OnUpdate, Outlet, RepetitionTimer, 
                 touchStartValue, touchStartValue, "WidgetSet::RotaryKnob" )
  CoreRectView_initLayoutContext,
  CoreView_GetRoot,
  CoreGroup_Draw,
  CoreView_HandleEvent,
  CoreGroup_CursorHitTest,
  CoreRectView_ArrangeView,
  CoreRectView_MoveView,
  CoreRectView_GetExtent,
  CoreGroup_ChangeViewState,
  WidgetSetRotaryKnob_OnSetBounds,
  CoreGroup_OnSetFocus,
  CoreGroup_OnSetBuffered,
  CoreGroup_OnSetOpacity,
  CoreGroup_DispatchEvent,
  CoreGroup_BroadcastEvent,
  CoreGroup_UpdateLayout,
  WidgetSetRotaryKnob_UpdateViewState,
  CoreGroup_InvalidateArea,
EW_END_OF_CLASS( WidgetSetRotaryKnob )

/* Initializer for the class 'WidgetSet::HorizontalSlider' */
void WidgetSetHorizontalSlider__Init( WidgetSetHorizontalSlider _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( WidgetSetHorizontalSlider );

  /* ... then construct all embedded objects */
  CoreTimer__Init( &_this->RepetitionTimer, &_this->_XObject, 0 );
  CoreKeyPressHandler__Init( &_this->KeyHandlerLeft, &_this->_XObject, 0 );
  CoreKeyPressHandler__Init( &_this->KeyHandlerRight, &_this->_XObject, 0 );
  CoreSimpleTouchHandler__Init( &_this->TouchHandler, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( WidgetSetHorizontalSlider );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const0007 );
  CoreTimer_OnSetPeriod( &_this->RepetitionTimer, 0 );
  CoreTimer_OnSetBegin( &_this->RepetitionTimer, 50 );
  _this->KeyHandlerLeft.Filter = CoreKeyCodeLeft;
  _this->KeyHandlerRight.Filter = CoreKeyCodeRight;
  CoreView_OnSetLayout((CoreView)&_this->TouchHandler, CoreLayoutAlignToBottom | 
  CoreLayoutAlignToLeft | CoreLayoutAlignToRight | CoreLayoutAlignToTop | CoreLayoutResizeHorz 
  | CoreLayoutResizeVert );
  CoreQuadView__OnSetPoint4( &_this->TouchHandler, _Const0006 );
  CoreQuadView__OnSetPoint3( &_this->TouchHandler, _Const0008 );
  CoreQuadView__OnSetPoint2( &_this->TouchHandler, _Const0009 );
  CoreQuadView__OnSetPoint1( &_this->TouchHandler, _Const0003 );
  CoreSimpleTouchHandler_OnSetRetargetOffset( &_this->TouchHandler, 16 );
  CoreSimpleTouchHandler_OnSetMaxStrikeCount( &_this->TouchHandler, 100 );
  _this->StepSize = 1;
  _this->MaxValue = 100;
  _this->CurrentValue = 50;
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->TouchHandler ), 0 );
  _this->RepetitionTimer.OnTrigger = EwNewSlot( _this, WidgetSetHorizontalSlider_onRepetitionTimer );
  _this->KeyHandlerLeft.OnRelease = EwNewSlot( _this, WidgetSetHorizontalSlider_onReleaseKey );
  _this->KeyHandlerLeft.OnPress = EwNewSlot( _this, WidgetSetHorizontalSlider_onPressKey );
  _this->KeyHandlerRight.OnRelease = EwNewSlot( _this, WidgetSetHorizontalSlider_onReleaseKey );
  _this->KeyHandlerRight.OnPress = EwNewSlot( _this, WidgetSetHorizontalSlider_onPressKey );
  _this->TouchHandler.OnDrag = EwNewSlot( _this, WidgetSetHorizontalSlider_onDragTouch );
  _this->TouchHandler.OnRelease = EwNewSlot( _this, WidgetSetHorizontalSlider_onReleaseTouch );
  _this->TouchHandler.OnPress = EwNewSlot( _this, WidgetSetHorizontalSlider_onPressTouch );
}

/* Re-Initializer for the class 'WidgetSet::HorizontalSlider' */
void WidgetSetHorizontalSlider__ReInit( WidgetSetHorizontalSlider _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  CoreTimer__ReInit( &_this->RepetitionTimer );
  CoreKeyPressHandler__ReInit( &_this->KeyHandlerLeft );
  CoreKeyPressHandler__ReInit( &_this->KeyHandlerRight );
  CoreSimpleTouchHandler__ReInit( &_this->TouchHandler );
}

/* Finalizer method for the class 'WidgetSet::HorizontalSlider' */
void WidgetSetHorizontalSlider__Done( WidgetSetHorizontalSlider _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreGroup );

  /* Finalize all embedded objects */
  CoreTimer__Done( &_this->RepetitionTimer );
  CoreKeyPressHandler__Done( &_this->KeyHandlerLeft );
  CoreKeyPressHandler__Done( &_this->KeyHandlerRight );
  CoreSimpleTouchHandler__Done( &_this->TouchHandler );

  /* Don't forget to deinitialize the super class ... */
  CoreGroup__Done( &_this->_Super );
}

/* 'C' function for method : 'WidgetSet::HorizontalSlider.OnSetBounds()' */
void WidgetSetHorizontalSlider_OnSetBounds( WidgetSetHorizontalSlider _this, XRect 
  value )
{
  if ( EwCompRect( _this->Super2.Bounds, value ))
    EwPostSignal( _this->OnUpdate, ((XObject)_this ));

  if ( _this->Appearance != 0 )
  {
    XPoint cs = EwGetRectSize( value );
    XPoint ns = cs;
    XPoint delta;

    if ( ns.X < _this->Appearance->WidgetMinSize.X )
      ns.X = _this->Appearance->WidgetMinSize.X;

    if ( ns.Y < _this->Appearance->WidgetMinSize.Y )
      ns.Y = _this->Appearance->WidgetMinSize.Y;

    if (( _this->Appearance->WidgetMaxSize.X > 0 ) && ( ns.X > _this->Appearance->WidgetMaxSize.X ))
      ns.X = _this->Appearance->WidgetMaxSize.X;

    if (( _this->Appearance->WidgetMaxSize.Y > 0 ) && ( ns.Y > _this->Appearance->WidgetMaxSize.Y ))
      ns.Y = _this->Appearance->WidgetMaxSize.Y;

    delta = EwMovePointNeg( ns, cs );

    if ( delta.X != 0 )
    {
      XBool alignToLeft = (( _this->Super3.Layout & CoreLayoutAlignToLeft ) == CoreLayoutAlignToLeft );
      XBool alignToRight = (( _this->Super3.Layout & CoreLayoutAlignToRight ) == 
        CoreLayoutAlignToRight );

      if ( alignToLeft && !alignToRight )
        value.Point2.X = ( value.Point2.X + delta.X );
      else
        if ( !alignToLeft && alignToRight )
          value.Point1.X = ( value.Point1.X - delta.X );
        else
        {
          value.Point1.X = ( value.Point1.X - ( delta.X / 2 ));
          value.Point2.X = ( value.Point1.X + ns.X );
        }
    }

    if ( delta.Y != 0 )
    {
      XBool alignToTop = (( _this->Super3.Layout & CoreLayoutAlignToTop ) == CoreLayoutAlignToTop );
      XBool alignToBottom = (( _this->Super3.Layout & CoreLayoutAlignToBottom ) 
        == CoreLayoutAlignToBottom );

      if ( alignToTop && !alignToBottom )
        value.Point2.Y = ( value.Point2.Y + delta.Y );
      else
        if ( !alignToTop && alignToBottom )
          value.Point1.Y = ( value.Point1.Y - delta.Y );
        else
        {
          value.Point1.Y = ( value.Point1.Y - ( delta.Y / 2 ));
          value.Point2.Y = ( value.Point1.Y + ns.Y );
        }
    }
  }

  CoreGroup_OnSetBounds((CoreGroup)_this, value );
}

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
void WidgetSetHorizontalSlider_UpdateViewState( WidgetSetHorizontalSlider _this, 
  XSet aState )
{
  XBool needsFace;
  XBool needsTrackLeft;
  XBool needsTrackRight;
  XBool needsThumb;
  XBool needsCover;
  XBool restack;
  XRect area;
  XPoint thumbSize;
  XBool isEnabled;
  XBool isFocused;
  XBool isPressed;
  XInt32 marginLeft;
  XInt32 marginRight;
  XInt32 minPos;
  XInt32 maxPos;
  XInt32 newThumbPos;

  CoreGroup_UpdateViewState((CoreGroup)_this, aState );
  needsFace = (XBool)(( _this->Appearance != 0 ) && (((( _this->Appearance->FaceDefault 
  != 0 ) || ( _this->Appearance->FaceDisabled != 0 )) || ( _this->Appearance->FaceFocused 
  != 0 )) || ( _this->Appearance->FaceActive != 0 )));
  needsTrackLeft = (XBool)(( _this->Appearance != 0 ) && (((( _this->Appearance->TrackLeftDefault 
  != 0 ) || ( _this->Appearance->TrackLeftDisabled != 0 )) || ( _this->Appearance->TrackLeftFocused 
  != 0 )) || ( _this->Appearance->TrackLeftActive != 0 )));
  needsTrackRight = (XBool)(( _this->Appearance != 0 ) && (((( _this->Appearance->TrackRightDefault 
  != 0 ) || ( _this->Appearance->TrackRightDisabled != 0 )) || ( _this->Appearance->TrackRightFocused 
  != 0 )) || ( _this->Appearance->TrackRightActive != 0 )));
  needsThumb = (XBool)(( _this->Appearance != 0 ) && (((( _this->Appearance->ThumbDefault 
  != 0 ) || ( _this->Appearance->ThumbDisabled != 0 )) || ( _this->Appearance->ThumbFocused 
  != 0 )) || ( _this->Appearance->ThumbActive != 0 )));
  needsCover = (XBool)(( _this->Appearance != 0 ) && (((( _this->Appearance->CoverDefault 
  != 0 ) || ( _this->Appearance->CoverDisabled != 0 )) || ( _this->Appearance->CoverFocused 
  != 0 )) || ( _this->Appearance->CoverActive != 0 )));
  restack = 0;
  area = EwGetRectORect( _this->Super2.Bounds );
  thumbSize = _Const0003;

  if ( needsFace && ( _this->frameView1 == 0 ))
  {
    _this->frameView1 = EwNewObject( ViewsFrame, 0 );
    CoreGroup_Add((CoreGroup)_this, ((CoreView)_this->frameView1 ), 0 );
    restack = 1;
  }
  else
    if ( !needsFace && ( _this->frameView1 != 0 ))
    {
      CoreGroup_Remove((CoreGroup)_this, ((CoreView)_this->frameView1 ));
      _this->frameView1 = 0;
    }

  if ( needsTrackLeft && ( _this->frameView2 == 0 ))
  {
    _this->frameView2 = EwNewObject( ViewsFrame, 0 );
    CoreGroup_Add((CoreGroup)_this, ((CoreView)_this->frameView2 ), 0 );
    restack = 1;
  }
  else
    if ( !needsTrackLeft && ( _this->frameView2 != 0 ))
    {
      CoreGroup_Remove((CoreGroup)_this, ((CoreView)_this->frameView2 ));
      _this->frameView2 = 0;
    }

  if ( needsTrackRight && ( _this->frameView3 == 0 ))
  {
    _this->frameView3 = EwNewObject( ViewsFrame, 0 );
    CoreGroup_Add((CoreGroup)_this, ((CoreView)_this->frameView3 ), 0 );
    restack = 1;
  }
  else
    if ( !needsTrackRight && ( _this->frameView3 != 0 ))
    {
      CoreGroup_Remove((CoreGroup)_this, ((CoreView)_this->frameView3 ));
      _this->frameView3 = 0;
    }

  if ( needsThumb && ( _this->imageView == 0 ))
  {
    _this->imageView = EwNewObject( ViewsImage, 0 );
    CoreGroup_Add((CoreGroup)_this, ((CoreView)_this->imageView ), 0 );
    restack = 1;
  }
  else
    if ( !needsThumb && ( _this->imageView != 0 ))
    {
      CoreGroup_Remove((CoreGroup)_this, ((CoreView)_this->imageView ));
      _this->imageView = 0;
    }

  if ( needsCover && ( _this->frameView4 == 0 ))
  {
    _this->frameView4 = EwNewObject( ViewsFrame, 0 );
    CoreGroup_Add((CoreGroup)_this, ((CoreView)_this->frameView4 ), 0 );
    restack = 1;
  }
  else
    if ( !needsCover && ( _this->frameView4 != 0 ))
    {
      CoreGroup_Remove((CoreGroup)_this, ((CoreView)_this->frameView4 ));
      _this->frameView4 = 0;
    }

  if ( restack )
  {
    if ( _this->frameView1 != 0 )
      CoreGroup_RestackTop((CoreGroup)_this, ((CoreView)_this->frameView1 ));

    if ( _this->frameView2 != 0 )
      CoreGroup_RestackTop((CoreGroup)_this, ((CoreView)_this->frameView2 ));

    if ( _this->frameView3 != 0 )
      CoreGroup_RestackTop((CoreGroup)_this, ((CoreView)_this->frameView3 ));

    if ( _this->imageView != 0 )
      CoreGroup_RestackTop((CoreGroup)_this, ((CoreView)_this->imageView ));

    if ( _this->frameView4 != 0 )
      CoreGroup_RestackTop((CoreGroup)_this, ((CoreView)_this->frameView4 ));
  }

  isEnabled = (( aState & CoreViewStateEnabled ) == CoreViewStateEnabled );
  isFocused = (( aState & CoreViewStateFocused ) == CoreViewStateFocused );
  isPressed = (XBool)(( _this->touchActive || _this->KeyHandlerRight.Down ) || _this->KeyHandlerLeft.Down );

  if ( _this->frameView1 != 0 )
  {
    ResourcesBitmap bitmap = 0;
    XInt32 frameNo = -1;
    XColor tint = _Const0000;

    if ( !isEnabled )
    {
      bitmap = _this->Appearance->FaceDisabled;
      frameNo = _this->Appearance->FaceFrameDisabled;
      tint = _this->Appearance->FaceTintDisabled;
    }
    else
      if ( isPressed )
      {
        bitmap = _this->Appearance->FaceActive;
        frameNo = _this->Appearance->FaceFrameActive;
        tint = _this->Appearance->FaceTintActive;
      }
      else
        if ( isFocused )
        {
          bitmap = _this->Appearance->FaceFocused;
          frameNo = _this->Appearance->FaceFrameFocused;
          tint = _this->Appearance->FaceTintFocused;
        }
        else
        {
          bitmap = _this->Appearance->FaceDefault;
          frameNo = _this->Appearance->FaceFrameDefault;
          tint = _this->Appearance->FaceTintDefault;
        }

    ViewsFrame_OnSetAnimated( _this->frameView1, (XBool)( frameNo < 0 ));
    ViewsFrame_OnSetColor( _this->frameView1, tint );

    if ( frameNo < 0 )
      frameNo = 0;

    ViewsFrame_OnSetFrameNumber( _this->frameView1, frameNo );
    ViewsFrame_OnSetBitmap( _this->frameView1, bitmap );
  }

  if ( _this->frameView2 != 0 )
  {
    ResourcesBitmap bitmap = 0;
    XInt32 frameNo = -1;
    XColor tint = _Const0000;
    XSet edges = GraphicsEdgesBottom | GraphicsEdgesInterior | GraphicsEdgesLeft 
      | GraphicsEdgesTop;

    if ( !isEnabled )
    {
      bitmap = _this->Appearance->TrackLeftDisabled;
      frameNo = _this->Appearance->TrackLeftFrameDisabled;
      tint = _this->Appearance->TrackLeftTintDisabled;
    }
    else
      if ( isPressed )
      {
        bitmap = _this->Appearance->TrackLeftActive;
        frameNo = _this->Appearance->TrackLeftFrameActive;
        tint = _this->Appearance->TrackLeftTintActive;
      }
      else
        if ( isFocused )
        {
          bitmap = _this->Appearance->TrackLeftFocused;
          frameNo = _this->Appearance->TrackLeftFrameFocused;
          tint = _this->Appearance->TrackLeftTintFocused;
        }
        else
        {
          bitmap = _this->Appearance->TrackLeftDefault;
          frameNo = _this->Appearance->TrackLeftFrameDefault;
          tint = _this->Appearance->TrackLeftTintDefault;
        }

    ViewsFrame_OnSetAnimated( _this->frameView2, (XBool)( frameNo < 0 ));
    ViewsFrame_OnSetColor( _this->frameView2, tint );

    if ( frameNo < 0 )
      frameNo = 0;

    if ( _this->Appearance->TrackLeftWithEdge )
      edges = GraphicsEdgesBottom | GraphicsEdgesInterior | GraphicsEdgesLeft | 
      GraphicsEdgesRight | GraphicsEdgesTop;

    ViewsFrame_OnSetFrameNumber( _this->frameView2, frameNo );
    ViewsFrame_OnSetBitmap( _this->frameView2, bitmap );
    ViewsFrame_OnSetEdges( _this->frameView2, edges );
  }

  if ( _this->frameView3 != 0 )
  {
    ResourcesBitmap bitmap = 0;
    XInt32 frameNo = -1;
    XColor tint = _Const0000;
    XSet edges = GraphicsEdgesBottom | GraphicsEdgesInterior | GraphicsEdgesRight 
      | GraphicsEdgesTop;

    if ( !isEnabled )
    {
      bitmap = _this->Appearance->TrackRightDisabled;
      frameNo = _this->Appearance->TrackRightFrameDisabled;
      tint = _this->Appearance->TrackRightTintDisabled;
    }
    else
      if ( isPressed )
      {
        bitmap = _this->Appearance->TrackRightActive;
        frameNo = _this->Appearance->TrackRightFrameActive;
        tint = _this->Appearance->TrackRightTintActive;
      }
      else
        if ( isFocused )
        {
          bitmap = _this->Appearance->TrackRightFocused;
          frameNo = _this->Appearance->TrackRightFrameFocused;
          tint = _this->Appearance->TrackRightTintFocused;
        }
        else
        {
          bitmap = _this->Appearance->TrackRightDefault;
          frameNo = _this->Appearance->TrackRightFrameDefault;
          tint = _this->Appearance->TrackRightTintDefault;
        }

    ViewsFrame_OnSetAnimated( _this->frameView3, (XBool)( frameNo < 0 ));
    ViewsFrame_OnSetColor( _this->frameView3, tint );

    if ( frameNo < 0 )
      frameNo = 0;

    if ( _this->Appearance->TrackRightWithEdge )
      edges = GraphicsEdgesBottom | GraphicsEdgesInterior | GraphicsEdgesLeft | 
      GraphicsEdgesRight | GraphicsEdgesTop;

    ViewsFrame_OnSetFrameNumber( _this->frameView3, frameNo );
    ViewsFrame_OnSetBitmap( _this->frameView3, bitmap );
    ViewsFrame_OnSetEdges( _this->frameView3, edges );
  }

  if ( _this->frameView4 != 0 )
  {
    ResourcesBitmap bitmap = 0;
    XInt32 frameNo = -1;
    XColor tint = _Const0000;

    if ( !isEnabled )
    {
      bitmap = _this->Appearance->CoverDisabled;
      frameNo = _this->Appearance->CoverFrameDisabled;
      tint = _this->Appearance->CoverTintDisabled;
    }
    else
      if ( isPressed )
      {
        bitmap = _this->Appearance->CoverActive;
        frameNo = _this->Appearance->CoverFrameActive;
        tint = _this->Appearance->CoverTintActive;
      }
      else
        if ( isFocused )
        {
          bitmap = _this->Appearance->CoverFocused;
          frameNo = _this->Appearance->CoverFrameFocused;
          tint = _this->Appearance->CoverTintFocused;
        }
        else
        {
          bitmap = _this->Appearance->CoverDefault;
          frameNo = _this->Appearance->CoverFrameDefault;
          tint = _this->Appearance->CoverTintDefault;
        }

    ViewsFrame_OnSetAnimated( _this->frameView4, (XBool)( frameNo < 0 ));
    ViewsFrame_OnSetColor( _this->frameView4, tint );

    if ( frameNo < 0 )
      frameNo = 0;

    ViewsFrame_OnSetFrameNumber( _this->frameView4, frameNo );
    ViewsFrame_OnSetBitmap( _this->frameView4, bitmap );
  }

  if ( _this->imageView != 0 )
  {
    ResourcesBitmap bitmap = 0;
    XInt32 frameNo = -1;
    XColor tint = _Const0000;

    if ( !isEnabled )
    {
      bitmap = _this->Appearance->ThumbDisabled;
      frameNo = _this->Appearance->ThumbFrameDisabled;
      tint = _this->Appearance->ThumbTintDisabled;
    }
    else
      if ( isPressed )
      {
        bitmap = _this->Appearance->ThumbActive;
        frameNo = _this->Appearance->ThumbFrameActive;
        tint = _this->Appearance->ThumbTintActive;
      }
      else
        if ( isFocused )
        {
          bitmap = _this->Appearance->ThumbFocused;
          frameNo = _this->Appearance->ThumbFrameFocused;
          tint = _this->Appearance->ThumbTintFocused;
        }
        else
        {
          bitmap = _this->Appearance->ThumbDefault;
          frameNo = _this->Appearance->ThumbFrameDefault;
          tint = _this->Appearance->ThumbTintDefault;
        }

    ViewsImage_OnSetAnimated( _this->imageView, (XBool)( frameNo < 0 ));
    ViewsImage_OnSetColor( _this->imageView, tint );

    if ( frameNo < 0 )
      frameNo = 0;

    ViewsImage_OnSetFrameNumber( _this->imageView, frameNo );
    ViewsImage_OnSetBitmap( _this->imageView, bitmap );
  }

  if (( _this->imageView != 0 ) && ( _this->imageView->Bitmap != 0 ))
    thumbSize = _this->imageView->Bitmap->FrameSize;

  marginLeft = thumbSize.X / 2;
  marginRight = thumbSize.X - marginLeft;

  if ( _this->Appearance != 0 )
  {
    marginLeft = marginLeft + _this->Appearance->ThumbMarginLeft;
    marginRight = marginRight + _this->Appearance->ThumbMarginRight;
  }

  minPos = marginLeft;
  maxPos = EwGetRectW( area ) - marginRight;
  newThumbPos = marginLeft;

  if ( _this->MaxValue != _this->MinValue )
    newThumbPos = (XInt32)(((XFloat)( WidgetSetHorizontalSlider_OnGetCurrentValue( 
    _this ) - _this->MinValue ) * (XFloat)( maxPos - minPos )) / (XFloat)( _this->MaxValue 
    - _this->MinValue )) + minPos;

  if ( newThumbPos < 0 )
    newThumbPos = 0;

  if ( newThumbPos > EwGetRectW( area ))
    newThumbPos = EwGetRectW( area );

  if (( _this->frameView1 != 0 ) && ( _this->frameView1->Bitmap != 0 ))
  {
    XInt32 h = _this->frameView1->Bitmap->FrameSize.Y;
    CoreRectView__OnSetBounds( _this->frameView1, EwNewRect( 0, ( EwGetRectH( area ) 
    / 2 ) - ( h / 2 ), EwGetRectW( area ), (( EwGetRectH( area ) / 2 ) - ( h / 2 )) 
    + h ));
  }

  if (( _this->frameView2 != 0 ) && ( _this->frameView2->Bitmap != 0 ))
  {
    XPoint s = _this->frameView2->Bitmap->FrameSize;
    CoreRectView__OnSetBounds( _this->frameView2, EwNewRect( 0, ( EwGetRectH( area ) 
    / 2 ) - ( s.Y / 2 ), newThumbPos, (( EwGetRectH( area ) / 2 ) - ( s.Y / 2 )) 
    + s.Y ));
    ViewsFrame_OnSetNoEdgesLimit( _this->frameView2, EwSetPointX( _this->frameView2->NoEdgesLimit, 
    EwGetRectW( area ) - ( s.X / 3 )));
    ViewsFrame_OnSetVisible( _this->frameView2, (XBool)( !_this->Appearance->TrackLeftWithEdge 
    || ( newThumbPos >= (( s.X / 3 ) * 2 ))));
  }

  if (( _this->frameView3 != 0 ) && ( _this->frameView3->Bitmap != 0 ))
  {
    XPoint s = _this->frameView3->Bitmap->FrameSize;
    CoreRectView__OnSetBounds( _this->frameView3, EwNewRect( newThumbPos, ( EwGetRectH( 
    area ) / 2 ) - ( s.Y / 2 ), EwGetRectW( area ), (( EwGetRectH( area ) / 2 ) 
    - ( s.Y / 2 )) + s.Y ));
    ViewsFrame_OnSetNoEdgesLimit( _this->frameView3, EwSetPointX( _this->frameView3->NoEdgesLimit, 
    EwGetRectW( area ) - ( s.X / 3 )));
    ViewsFrame_OnSetVisible( _this->frameView3, (XBool)( !_this->Appearance->TrackRightWithEdge 
    || (( EwGetRectW( area ) - newThumbPos ) >= (( s.X / 3 ) * 2 ))));
  }

  if (( _this->frameView4 != 0 ) && ( _this->frameView4->Bitmap != 0 ))
  {
    XInt32 h = _this->frameView4->Bitmap->FrameSize.Y;
    CoreRectView__OnSetBounds( _this->frameView4, EwNewRect( 0, ( EwGetRectH( area ) 
    / 2 ) - ( h / 2 ), EwGetRectW( area ), (( EwGetRectH( area ) / 2 ) - ( h / 2 )) 
    + h ));
  }

  if (( _this->imageView != 0 ) && ( _this->imageView->Bitmap != 0 ))
  {
    XRect thumbArea = EwNewRect2Point( _Const0003, thumbSize );
    CoreRectView__OnSetBounds( _this->imageView, EwMoveRectPos( EwMoveRectNeg( thumbArea, 
    EwGetRectCenter( thumbArea )), EwNewPoint( newThumbPos, EwGetRectH( area ) / 
    2 )));
  }
}

/* 'C' function for method : 'WidgetSet::HorizontalSlider.onConfigChanged()' */
void WidgetSetHorizontalSlider_onConfigChanged( WidgetSetHorizontalSlider _this, 
  XObject sender )
{
  XEnum oldKeyCodeRight;
  XEnum oldKeyCodeLeft;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  oldKeyCodeRight = _this->KeyHandlerRight.Filter;
  oldKeyCodeLeft = _this->KeyHandlerLeft.Filter;

  if ( _this->Appearance != 0 )
  {
    _this->KeyHandlerRight.Filter = _this->Appearance->KeyCodeRight;
    _this->KeyHandlerLeft.Filter = _this->Appearance->KeyCodeLeft;
  }
  else
  {
    _this->KeyHandlerRight.Filter = CoreKeyCodeRight;
    _this->KeyHandlerLeft.Filter = CoreKeyCodeLeft;
  }

  if (((( oldKeyCodeRight == CoreKeyCodeNoKey ) || ( oldKeyCodeLeft == CoreKeyCodeNoKey )) 
      && ( _this->KeyHandlerRight.Filter != CoreKeyCodeNoKey )) && ( _this->KeyHandlerLeft.Filter 
      != CoreKeyCodeNoKey ))
    CoreView__ChangeViewState( _this, CoreViewStateFocusable, 0 );

  if ((( oldKeyCodeRight != CoreKeyCodeNoKey ) && ( oldKeyCodeLeft != CoreKeyCodeNoKey )) 
      && (( _this->KeyHandlerRight.Filter == CoreKeyCodeNoKey ) || ( _this->KeyHandlerLeft.Filter 
      == CoreKeyCodeNoKey )))
    CoreView__ChangeViewState( _this, 0, CoreViewStateFocusable );

  if ( _this->Appearance != 0 )
  {
    CoreTimer_OnSetBegin( &_this->RepetitionTimer, _this->Appearance->KeyRepeatDelay );
    CoreTimer_OnSetPeriod( &_this->RepetitionTimer, _this->Appearance->KeyRepeatPeriod );
  }
  else
  {
    CoreTimer_OnSetBegin( &_this->RepetitionTimer, 0 );
    CoreTimer_OnSetPeriod( &_this->RepetitionTimer, 0 );
  }

  if ( _this->Appearance != 0 )
    CoreRectView__OnSetBounds( _this, _this->Super2.Bounds );

  CoreGroup_InvalidateViewState((CoreGroup)_this );
  EwPostSignal( _this->OnUpdate, ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::HorizontalSlider.onOutlet()' */
void WidgetSetHorizontalSlider_onOutlet( WidgetSetHorizontalSlider _this, XObject 
  sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( _this->Outlet.Object != 0 )
    WidgetSetHorizontalSlider_OnSetCurrentValue( _this, EwOnGetInt32( _this->Outlet ));
}

/* 'C' function for method : 'WidgetSet::HorizontalSlider.onRepetitionTimer()' */
void WidgetSetHorizontalSlider_onRepetitionTimer( WidgetSetHorizontalSlider _this, 
  XObject sender )
{
  XInt32 oldValue;
  XInt32 newValue;
  XInt32 delta;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  oldValue = WidgetSetHorizontalSlider_OnGetCurrentValue( _this );
  newValue = oldValue;
  delta = _this->StepSize;

  if (( _this->KeyHandlerLeft.Down && ( _this->MaxValue > _this->MinValue )) || 
      ( _this->KeyHandlerRight.Down && ( _this->MaxValue < _this->MinValue )))
    delta = -delta;

  if ( oldValue < 0 )
  {
    newValue = -newValue;
    delta = -delta;
  }

  if ( delta > 0 )
    newValue = ( newValue + delta ) - (( newValue + delta ) % delta );

  if ( delta < 0 )
  {
    if (( newValue % -delta ) != 0 )
      newValue = ( newValue - delta ) - ( newValue % -delta );

    newValue = newValue + delta;
  }

  if ( oldValue < 0 )
    newValue = -newValue;

  WidgetSetHorizontalSlider_OnSetCurrentValue( _this, newValue );

  if ( oldValue == WidgetSetHorizontalSlider_OnGetCurrentValue( _this ))
    return;

  EwPostSignal( _this->OnChange, ((XObject)_this ));

  if ( _this->Outlet.Object != 0 )
  {
    EwOnSetInt32( _this->Outlet, WidgetSetHorizontalSlider_OnGetCurrentValue( _this ));
    EwNotifyRefObservers( _this->Outlet, 0 );
  }
}

/* 'C' function for method : 'WidgetSet::HorizontalSlider.onReleaseKey()' */
void WidgetSetHorizontalSlider_onReleaseKey( WidgetSetHorizontalSlider _this, XObject 
  sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  CoreSimpleTouchHandler_OnSetEnabled( &_this->TouchHandler, 1 );
  CoreGroup_InvalidateViewState((CoreGroup)_this );
  CoreTimer_OnSetEnabled( &_this->RepetitionTimer, 0 );
  EwPostSignal( _this->OnEnd, ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::HorizontalSlider.onPressKey()' */
void WidgetSetHorizontalSlider_onPressKey( WidgetSetHorizontalSlider _this, XObject 
  sender )
{
  XInt32 oldValue;
  XInt32 newValue;
  XInt32 delta;

  CoreSimpleTouchHandler_OnSetEnabled( &_this->TouchHandler, 0 );
  CoreGroup_InvalidateViewState((CoreGroup)_this );
  EwPostSignal( _this->OnStart, ((XObject)_this ));
  CoreTimer_OnSetEnabled( &_this->RepetitionTimer, 1 );
  oldValue = WidgetSetHorizontalSlider_OnGetCurrentValue( _this );
  newValue = oldValue;
  delta = _this->StepSize;

  if ((( sender == ((XObject)&_this->KeyHandlerLeft )) && ( _this->MaxValue > _this->MinValue )) 
      || (( sender == ((XObject)&_this->KeyHandlerRight )) && ( _this->MaxValue 
      < _this->MinValue )))
    delta = -delta;

  if ( oldValue < 0 )
  {
    newValue = -newValue;
    delta = -delta;
  }

  if ( delta > 0 )
    newValue = ( newValue + delta ) - (( newValue + delta ) % delta );

  if ( delta < 0 )
  {
    if (( newValue % -delta ) != 0 )
      newValue = ( newValue - delta ) - ( newValue % -delta );

    newValue = newValue + delta;
  }

  if ( oldValue < 0 )
    newValue = -newValue;

  WidgetSetHorizontalSlider_OnSetCurrentValue( _this, newValue );

  if ( oldValue == WidgetSetHorizontalSlider_OnGetCurrentValue( _this ))
    return;

  EwPostSignal( _this->OnChange, ((XObject)_this ));

  if ( _this->Outlet.Object != 0 )
  {
    EwOnSetInt32( _this->Outlet, WidgetSetHorizontalSlider_OnGetCurrentValue( _this ));
    EwNotifyRefObservers( _this->Outlet, 0 );
  }
}

/* 'C' function for method : 'WidgetSet::HorizontalSlider.onDragTouch()' */
void WidgetSetHorizontalSlider_onDragTouch( WidgetSetHorizontalSlider _this, XObject 
  sender )
{
  XInt32 marginLeft;
  XInt32 marginRight;
  XPoint delta;
  XInt32 minPos;
  XInt32 maxPos;
  XInt32 oldValue;
  XInt32 newValue;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( !_this->touchActive )
    return;

  marginLeft = 0;
  marginRight = 0;

  if ( _this->Appearance != 0 )
  {
    marginLeft = _this->Appearance->ThumbMarginLeft;
    marginRight = _this->Appearance->ThumbMarginRight;
  }

  delta = EwMovePointNeg( _this->TouchHandler.CurrentPos, _this->TouchHandler.HittingPos );
  minPos = marginLeft;
  maxPos = EwGetRectW( _this->Super2.Bounds ) - marginRight;
  oldValue = WidgetSetHorizontalSlider_OnGetCurrentValue( _this );
  newValue = oldValue;

  if (( _this->imageView != 0 ) && ( _this->imageView->Bitmap != 0 ))
    maxPos = maxPos - EwGetRectW( _this->imageView->Super1.Bounds );

  if ( maxPos > minPos )
    newValue = (XInt32)(((XFloat)delta.X * (XFloat)( _this->MaxValue - _this->MinValue )) 
    / (XFloat)( maxPos - minPos )) + _this->touchStartValue;

  WidgetSetHorizontalSlider_OnSetCurrentValue( _this, newValue - ( newValue % _this->StepSize ));

  if ( oldValue == WidgetSetHorizontalSlider_OnGetCurrentValue( _this ))
    return;

  EwPostSignal( _this->OnChange, ((XObject)_this ));

  if ( _this->Outlet.Object != 0 )
  {
    EwOnSetInt32( _this->Outlet, WidgetSetHorizontalSlider_OnGetCurrentValue( _this ));
    EwNotifyRefObservers( _this->Outlet, 0 );
  }
}

/* 'C' function for method : 'WidgetSet::HorizontalSlider.onReleaseTouch()' */
void WidgetSetHorizontalSlider_onReleaseTouch( WidgetSetHorizontalSlider _this, 
  XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( !_this->touchActive )
    return;

  _this->touchActive = 0;
  _this->KeyHandlerRight.Enabled = 1;
  _this->KeyHandlerLeft.Enabled = 1;
  CoreGroup_InvalidateViewState((CoreGroup)_this );
  EwPostSignal( _this->OnEnd, ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::HorizontalSlider.onPressTouch()' */
void WidgetSetHorizontalSlider_onPressTouch( WidgetSetHorizontalSlider _this, XObject 
  sender )
{
  XInt32 dragLimit;
  XRect touchArea;
  XBool hasThumb;
  XBool insideThumb;
  XBool adjustAtTouch;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  dragLimit = CoreView__GetRoot( _this )->CursorDragLimit;
  touchArea = EwMoveRectPos( EwNewRect2Point( EwNewPoint( -dragLimit, -dragLimit ), 
  EwNewPoint( dragLimit + 1, dragLimit + 1 )), _this->TouchHandler.CurrentPos );
  hasThumb = (XBool)(( _this->imageView != 0 ) && ( _this->imageView->Bitmap != 
  0 ));
  insideThumb = (XBool)( hasThumb && !EwIsRectEmpty( EwIntersectRect( _this->imageView->Super1.Bounds, 
  touchArea )));
  adjustAtTouch = (XBool)(( _this->Appearance != 0 ) && _this->Appearance->ThumbUpdateAtTouch );

  if (( hasThumb && !insideThumb ) && !adjustAtTouch )
  {
    CoreRoot root = CoreView__GetRoot( _this );
    CoreSimpleTouchHandler_OnSetEnabled( &_this->TouchHandler, 0 );
    CoreRoot_RetargetCursor( root, ((CoreView)root ), 0 );
    CoreSimpleTouchHandler_OnSetEnabled( &_this->TouchHandler, 1 );
    return;
  }

  _this->touchActive = 1;
  _this->KeyHandlerRight.Enabled = 0;
  _this->KeyHandlerLeft.Enabled = 0;
  EwPostSignal( _this->OnStart, ((XObject)_this ));
  CoreGroup_InvalidateViewState((CoreGroup)_this );
  _this->touchStartValue = WidgetSetHorizontalSlider_OnGetCurrentValue( _this );

  if ( adjustAtTouch && !insideThumb )
  {
    XInt32 marginLeft = 0;
    XInt32 marginRight = 0;
    XInt32 thumbWidth = 0;
    XInt32 hitPos;
    XInt32 minPos;
    XInt32 maxPos;
    XInt32 oldValue;
    XInt32 newValue;

    if ( _this->Appearance != 0 )
    {
      marginLeft = _this->Appearance->ThumbMarginLeft;
      marginRight = _this->Appearance->ThumbMarginRight;
    }

    if (( _this->imageView != 0 ) && ( _this->imageView->Bitmap != 0 ))
      thumbWidth = EwGetRectW( _this->imageView->Super1.Bounds );

    hitPos = _this->TouchHandler.HittingPos.X - ( thumbWidth / 2 );
    minPos = marginLeft;
    maxPos = ( EwGetRectW( _this->Super2.Bounds ) - marginRight ) - thumbWidth;
    oldValue = WidgetSetHorizontalSlider_OnGetCurrentValue( _this );
    newValue = oldValue;

    if ( maxPos > minPos )
      newValue = (XInt32)(((XFloat)( hitPos - minPos ) * (XFloat)( _this->MaxValue 
      - _this->MinValue )) / (XFloat)( maxPos - minPos )) + _this->MinValue;

    WidgetSetHorizontalSlider_OnSetCurrentValue( _this, newValue - ( newValue % 
    _this->StepSize ));

    if ( oldValue == WidgetSetHorizontalSlider_OnGetCurrentValue( _this ))
      return;

    EwPostSignal( _this->OnChange, ((XObject)_this ));

    if ( _this->Outlet.Object != 0 )
    {
      EwOnSetInt32( _this->Outlet, WidgetSetHorizontalSlider_OnGetCurrentValue( 
      _this ));
      EwNotifyRefObservers( _this->Outlet, 0 );
    }

    _this->touchStartValue = WidgetSetHorizontalSlider_OnGetCurrentValue( _this );
  }
}

/* 'C' function for method : 'WidgetSet::HorizontalSlider.OnSetOutlet()' */
void WidgetSetHorizontalSlider_OnSetOutlet( WidgetSetHorizontalSlider _this, XRef 
  value )
{
  if ( !EwCompRef( _this->Outlet, value ))
    return;

  if ( _this->Outlet.Object != 0 )
    EwDetachRefObserver( EwNewSlot( _this, WidgetSetHorizontalSlider_onOutlet ), 
      _this->Outlet, 0 );

  _this->Outlet = value;

  if ( value.Object != 0 )
    EwAttachRefObserver( EwNewSlot( _this, WidgetSetHorizontalSlider_onOutlet ), 
      value, 0 );

  if ( value.Object != 0 )
    EwPostSignal( EwNewSlot( _this, WidgetSetHorizontalSlider_onOutlet ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::HorizontalSlider.OnSetMaxValue()' */
void WidgetSetHorizontalSlider_OnSetMaxValue( WidgetSetHorizontalSlider _this, XInt32 
  value )
{
  if ( _this->MaxValue == value )
    return;

  _this->MaxValue = value;
  CoreGroup_InvalidateViewState((CoreGroup)_this );
  EwPostSignal( _this->OnUpdate, ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::HorizontalSlider.OnSetMinValue()' */
void WidgetSetHorizontalSlider_OnSetMinValue( WidgetSetHorizontalSlider _this, XInt32 
  value )
{
  if ( _this->MinValue == value )
    return;

  _this->MinValue = value;
  CoreGroup_InvalidateViewState((CoreGroup)_this );
  EwPostSignal( _this->OnUpdate, ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::HorizontalSlider.OnGetCurrentValue()' */
XInt32 WidgetSetHorizontalSlider_OnGetCurrentValue( WidgetSetHorizontalSlider _this )
{
  XInt32 value = _this->CurrentValue;

  if ( _this->MinValue > _this->MaxValue )
  {
    if ( value < _this->MaxValue )
      value = _this->MaxValue;

    if ( value > _this->MinValue )
      value = _this->MinValue;
  }
  else
  {
    if ( value < _this->MinValue )
      value = _this->MinValue;

    if ( value > _this->MaxValue )
      value = _this->MaxValue;
  }

  return value;
}

/* 'C' function for method : 'WidgetSet::HorizontalSlider.OnSetCurrentValue()' */
void WidgetSetHorizontalSlider_OnSetCurrentValue( WidgetSetHorizontalSlider _this, 
  XInt32 value )
{
  if ( _this->CurrentValue == value )
    return;

  _this->CurrentValue = value;
  CoreGroup_InvalidateViewState((CoreGroup)_this );
  EwPostSignal( _this->OnUpdate, ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::HorizontalSlider.OnSetAppearance()' */
void WidgetSetHorizontalSlider_OnSetAppearance( WidgetSetHorizontalSlider _this, 
  WidgetSetHorizontalSliderConfig value )
{
  if ( _this->Appearance == value )
    return;

  if ( _this->Appearance != 0 )
    EwDetachObjObserver( EwNewSlot( _this, WidgetSetHorizontalSlider_onConfigChanged ), 
      (XObject)_this->Appearance, 0 );

  _this->Appearance = value;

  if ( value != 0 )
    EwAttachObjObserver( EwNewSlot( _this, WidgetSetHorizontalSlider_onConfigChanged ), 
      (XObject)value, 0 );

  EwPostSignal( EwNewSlot( _this, WidgetSetHorizontalSlider_onConfigChanged ), ((XObject)_this ));
}

/* Variants derived from the class : 'WidgetSet::HorizontalSlider' */
EW_DEFINE_CLASS_VARIANTS( WidgetSetHorizontalSlider )
EW_END_OF_CLASS_VARIANTS( WidgetSetHorizontalSlider )

/* Virtual Method Table (VMT) for the class : 'WidgetSet::HorizontalSlider' */
EW_DEFINE_CLASS( WidgetSetHorizontalSlider, CoreGroup, frameView4, OnUpdate, Outlet, 
                 RepetitionTimer, touchStartValue, touchStartValue, "WidgetSet::HorizontalSlider" )
  CoreRectView_initLayoutContext,
  CoreView_GetRoot,
  CoreGroup_Draw,
  CoreView_HandleEvent,
  CoreGroup_CursorHitTest,
  CoreRectView_ArrangeView,
  CoreRectView_MoveView,
  CoreRectView_GetExtent,
  CoreGroup_ChangeViewState,
  WidgetSetHorizontalSlider_OnSetBounds,
  CoreGroup_OnSetFocus,
  CoreGroup_OnSetBuffered,
  CoreGroup_OnSetOpacity,
  CoreGroup_DispatchEvent,
  CoreGroup_BroadcastEvent,
  CoreGroup_UpdateLayout,
  WidgetSetHorizontalSlider_UpdateViewState,
  CoreGroup_InvalidateArea,
EW_END_OF_CLASS( WidgetSetHorizontalSlider )

/* Initializer for the class 'WidgetSet::ToggleButton' */
void WidgetSetToggleButton__Init( WidgetSetToggleButton _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( WidgetSetToggleButton );

  /* ... then construct all embedded objects */
  CoreTimer__Init( &_this->FlashTimer, &_this->_XObject, 0 );
  CoreKeyPressHandler__Init( &_this->KeyHandler, &_this->_XObject, 0 );
  CoreSimpleTouchHandler__Init( &_this->TouchHandler, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( WidgetSetToggleButton );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const000A );
  CoreTimer_OnSetPeriod( &_this->FlashTimer, 0 );
  CoreTimer_OnSetBegin( &_this->FlashTimer, 50 );
  _this->KeyHandler.Filter = CoreKeyCodeEnter;
  CoreView_OnSetLayout((CoreView)&_this->TouchHandler, CoreLayoutAlignToBottom | 
  CoreLayoutAlignToLeft | CoreLayoutAlignToRight | CoreLayoutAlignToTop | CoreLayoutResizeHorz 
  | CoreLayoutResizeVert );
  CoreQuadView__OnSetPoint4( &_this->TouchHandler, _Const0006 );
  CoreQuadView__OnSetPoint3( &_this->TouchHandler, _Const000B );
  CoreQuadView__OnSetPoint2( &_this->TouchHandler, _Const000C );
  CoreQuadView__OnSetPoint1( &_this->TouchHandler, _Const0003 );
  CoreSimpleTouchHandler_OnSetRetargetOffset( &_this->TouchHandler, 16 );
  CoreSimpleTouchHandler_OnSetMaxStrikeCount( &_this->TouchHandler, 100 );
  _this->IconOnFrameActive = -1;
  _this->IconOnFrameFocused = -1;
  _this->IconOnFrameDisabled = -1;
  _this->IconOnFrameDefault = -1;
  _this->IconOffFrameActive = -1;
  _this->IconOffFrameFocused = -1;
  _this->IconOffFrameDisabled = -1;
  _this->IconOffFrameDefault = -1;
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->TouchHandler ), 0 );
  _this->FlashTimer.OnTrigger = EwNewSlot( _this, WidgetSetToggleButton_onFlashTimer );
  _this->KeyHandler.OnRelease = EwNewSlot( _this, WidgetSetToggleButton_onReleaseKey );
  _this->KeyHandler.OnPress = EwNewSlot( _this, WidgetSetToggleButton_onPressKey );
  _this->TouchHandler.OnLeave = EwNewSlot( _this, WidgetSetToggleButton_onLeaveTouch );
  _this->TouchHandler.OnEnter = EwNewSlot( _this, WidgetSetToggleButton_onEnterTouch );
  _this->TouchHandler.OnRelease = EwNewSlot( _this, WidgetSetToggleButton_onReleaseTouch );
  _this->TouchHandler.OnPress = EwNewSlot( _this, WidgetSetToggleButton_onPressTouch );
}

/* Re-Initializer for the class 'WidgetSet::ToggleButton' */
void WidgetSetToggleButton__ReInit( WidgetSetToggleButton _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  CoreTimer__ReInit( &_this->FlashTimer );
  CoreKeyPressHandler__ReInit( &_this->KeyHandler );
  CoreSimpleTouchHandler__ReInit( &_this->TouchHandler );
}

/* Finalizer method for the class 'WidgetSet::ToggleButton' */
void WidgetSetToggleButton__Done( WidgetSetToggleButton _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreGroup );

  /* Finalize all embedded objects */
  CoreTimer__Done( &_this->FlashTimer );
  CoreKeyPressHandler__Done( &_this->KeyHandler );
  CoreSimpleTouchHandler__Done( &_this->TouchHandler );

  /* Don't forget to deinitialize the super class ... */
  CoreGroup__Done( &_this->_Super );
}

/* 'C' function for method : 'WidgetSet::ToggleButton.OnSetBounds()' */
void WidgetSetToggleButton_OnSetBounds( WidgetSetToggleButton _this, XRect value )
{
  if ( _this->Appearance != 0 )
  {
    XPoint cs = EwGetRectSize( value );
    XPoint ns = cs;
    XPoint delta;

    if ( ns.X < _this->Appearance->WidgetMinSize.X )
      ns.X = _this->Appearance->WidgetMinSize.X;

    if ( ns.Y < _this->Appearance->WidgetMinSize.Y )
      ns.Y = _this->Appearance->WidgetMinSize.Y;

    if (( _this->Appearance->WidgetMaxSize.X > 0 ) && ( ns.X > _this->Appearance->WidgetMaxSize.X ))
      ns.X = _this->Appearance->WidgetMaxSize.X;

    if (( _this->Appearance->WidgetMaxSize.Y > 0 ) && ( ns.Y > _this->Appearance->WidgetMaxSize.Y ))
      ns.Y = _this->Appearance->WidgetMaxSize.Y;

    delta = EwMovePointNeg( ns, cs );

    if ( delta.X != 0 )
    {
      XBool alignToLeft = (( _this->Super3.Layout & CoreLayoutAlignToLeft ) == CoreLayoutAlignToLeft );
      XBool alignToRight = (( _this->Super3.Layout & CoreLayoutAlignToRight ) == 
        CoreLayoutAlignToRight );

      if ( alignToLeft && !alignToRight )
        value.Point2.X = ( value.Point2.X + delta.X );
      else
        if ( !alignToLeft && alignToRight )
          value.Point1.X = ( value.Point1.X - delta.X );
        else
        {
          value.Point1.X = ( value.Point1.X - ( delta.X / 2 ));
          value.Point2.X = ( value.Point1.X + ns.X );
        }
    }

    if ( delta.Y != 0 )
    {
      XBool alignToTop = (( _this->Super3.Layout & CoreLayoutAlignToTop ) == CoreLayoutAlignToTop );
      XBool alignToBottom = (( _this->Super3.Layout & CoreLayoutAlignToBottom ) 
        == CoreLayoutAlignToBottom );

      if ( alignToTop && !alignToBottom )
        value.Point2.Y = ( value.Point2.Y + delta.Y );
      else
        if ( !alignToTop && alignToBottom )
          value.Point1.Y = ( value.Point1.Y - delta.Y );
        else
        {
          value.Point1.Y = ( value.Point1.Y - ( delta.Y / 2 ));
          value.Point2.Y = ( value.Point1.Y + ns.Y );
        }
    }
  }

  CoreGroup_OnSetBounds((CoreGroup)_this, value );
}

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
void WidgetSetToggleButton_UpdateViewState( WidgetSetToggleButton _this, XSet aState )
{
  XBool needsFace;
  XBool needsIcon;
  XBool needsLabel;
  XBool restack;
  XRect area;
  XBool isEnabled;
  XBool isFocused;
  XBool isPressed;
  XBool isChecked;

  CoreGroup_UpdateViewState((CoreGroup)_this, aState );
  needsFace = (XBool)(( _this->Appearance != 0 ) && (((((((( _this->Appearance->FaceOffDefault 
  != 0 ) || ( _this->Appearance->FaceOffDisabled != 0 )) || ( _this->Appearance->FaceOffFocused 
  != 0 )) || ( _this->Appearance->FaceOffActive != 0 )) || ( _this->Appearance->FaceOnDefault 
  != 0 )) || ( _this->Appearance->FaceOnDisabled != 0 )) || ( _this->Appearance->FaceOnFocused 
  != 0 )) || ( _this->Appearance->FaceOnActive != 0 )));
  needsIcon = (XBool)( _this->Icon != 0 );
  needsLabel = (XBool)(( _this->Appearance != 0 ) && ((( EwCompString( _this->LabelOff, 
  0 ) != 0 ) && ( _this->Appearance->LabelOffFont != 0 )) || (( EwCompString( _this->LabelOn, 
  0 ) != 0 ) && ( _this->Appearance->LabelOnFont != 0 ))));
  restack = 0;
  area = EwGetRectORect( _this->Super2.Bounds );

  if ( needsFace && ( _this->frameView == 0 ))
  {
    _this->frameView = EwNewObject( ViewsFrame, 0 );
    CoreGroup_Add((CoreGroup)_this, ((CoreView)_this->frameView ), 0 );
    restack = 1;
  }
  else
    if ( !needsFace && ( _this->frameView != 0 ))
    {
      CoreGroup_Remove((CoreGroup)_this, ((CoreView)_this->frameView ));
      _this->frameView = 0;
    }

  if ( needsIcon && ( _this->imageView == 0 ))
  {
    _this->imageView = EwNewObject( ViewsImage, 0 );
    CoreGroup_Add((CoreGroup)_this, ((CoreView)_this->imageView ), 0 );
    restack = 1;
  }
  else
    if ( !needsIcon && ( _this->imageView != 0 ))
    {
      CoreGroup_Remove((CoreGroup)_this, ((CoreView)_this->imageView ));
      _this->imageView = 0;
    }

  if ( needsLabel && ( _this->textView == 0 ))
  {
    _this->textView = EwNewObject( ViewsText, 0 );
    CoreGroup_Add((CoreGroup)_this, ((CoreView)_this->textView ), 0 );
    restack = 1;
    ViewsText_OnSetEnableBidiText( _this->textView, 1 );
  }
  else
    if ( !needsLabel && ( _this->textView != 0 ))
    {
      CoreGroup_Remove((CoreGroup)_this, ((CoreView)_this->textView ));
      _this->textView = 0;
    }

  if ( restack )
  {
    if ( _this->frameView != 0 )
      CoreGroup_RestackTop((CoreGroup)_this, ((CoreView)_this->frameView ));

    if ( _this->imageView != 0 )
      CoreGroup_RestackTop((CoreGroup)_this, ((CoreView)_this->imageView ));

    if ( _this->textView != 0 )
      CoreGroup_RestackTop((CoreGroup)_this, ((CoreView)_this->textView ));
  }

  isEnabled = (( aState & CoreViewStateEnabled ) == CoreViewStateEnabled );
  isFocused = (( aState & CoreViewStateFocused ) == CoreViewStateFocused );
  isPressed = (XBool)((( _this->TouchHandler.Down && _this->TouchHandler.Inside ) 
  || _this->KeyHandler.Down ) || _this->FlashTimer.Enabled );
  isChecked = _this->Checked;

  if ( _this->frameView != 0 )
  {
    ResourcesBitmap bitmap = 0;
    XInt32 frameNo = -1;
    XColor tint = _Const0000;
    XSet layout = _this->Appearance->FaceLayout;
    XRect r = area;

    if ( isChecked )
    {
      if ( !isEnabled )
      {
        bitmap = _this->Appearance->FaceOnDisabled;
        frameNo = _this->Appearance->FaceOnFrameDisabled;
        tint = _this->Appearance->FaceOnTintDisabled;
      }
      else
        if ( isPressed )
        {
          bitmap = _this->Appearance->FaceOnActive;
          frameNo = _this->Appearance->FaceOnFrameActive;
          tint = _this->Appearance->FaceOnTintActive;
        }
        else
          if ( isFocused )
          {
            bitmap = _this->Appearance->FaceOnFocused;
            frameNo = _this->Appearance->FaceOnFrameFocused;
            tint = _this->Appearance->FaceOnTintFocused;
          }
          else
          {
            bitmap = _this->Appearance->FaceOnDefault;
            frameNo = _this->Appearance->FaceOnFrameDefault;
            tint = _this->Appearance->FaceOnTintDefault;
          }
    }
    else
    {
      if ( !isEnabled )
      {
        bitmap = _this->Appearance->FaceOffDisabled;
        frameNo = _this->Appearance->FaceOffFrameDisabled;
        tint = _this->Appearance->FaceOffTintDisabled;
      }
      else
        if ( isPressed )
        {
          bitmap = _this->Appearance->FaceOffActive;
          frameNo = _this->Appearance->FaceOffFrameActive;
          tint = _this->Appearance->FaceOffTintActive;
        }
        else
          if ( isFocused )
          {
            bitmap = _this->Appearance->FaceOffFocused;
            frameNo = _this->Appearance->FaceOffFrameFocused;
            tint = _this->Appearance->FaceOffTintFocused;
          }
          else
          {
            bitmap = _this->Appearance->FaceOffDefault;
            frameNo = _this->Appearance->FaceOffFrameDefault;
            tint = _this->Appearance->FaceOffTintDefault;
          }
    }

    ViewsFrame_OnSetAnimated( _this->frameView, (XBool)( frameNo < 0 ));
    ViewsFrame_OnSetColor( _this->frameView, tint );

    if ( frameNo < 0 )
      frameNo = 0;

    if (( bitmap != 0 ) && !(( layout & CoreLayoutResizeHorz ) == CoreLayoutResizeHorz ))
    {
      XBool alignToLeft = (( layout & CoreLayoutAlignToLeft ) == CoreLayoutAlignToLeft );
      XBool alignToRight = (( layout & CoreLayoutAlignToRight ) == CoreLayoutAlignToRight );
      XInt32 w = bitmap->FrameSize.X;

      if ( alignToLeft && !alignToRight )
        r.Point2.X = ( r.Point1.X + w );
      else
        if ( !alignToLeft && alignToRight )
          r.Point1.X = ( r.Point2.X - w );
        else
        {
          r.Point1.X = (( EwGetRectW( r ) / 2 ) - ( w / 2 ));
          r.Point2.X = ( r.Point1.X + w );
        }
    }

    if (( bitmap != 0 ) && !(( layout & CoreLayoutResizeVert ) == CoreLayoutResizeVert ))
    {
      XBool alignToTop = (( layout & CoreLayoutAlignToTop ) == CoreLayoutAlignToTop );
      XBool alignToBottom = (( layout & CoreLayoutAlignToBottom ) == CoreLayoutAlignToBottom );
      XInt32 h = bitmap->FrameSize.Y;

      if ( alignToTop && !alignToBottom )
        r.Point2.Y = ( r.Point1.Y + h );
      else
        if ( !alignToTop && alignToBottom )
          r.Point1.Y = ( r.Point2.Y - h );
        else
        {
          r.Point1.Y = (( EwGetRectH( r ) / 2 ) - ( h / 2 ));
          r.Point2.Y = ( r.Point1.Y + h );
        }
    }

    ViewsFrame_OnSetFrameNumber( _this->frameView, frameNo );
    ViewsFrame_OnSetBitmap( _this->frameView, bitmap );
    CoreRectView__OnSetBounds( _this->frameView, r );
  }

  if (( _this->imageView != 0 ) && ( _this->Appearance != 0 ))
  {
    XInt32 frameNo = -1;
    XColor clr = _Const0000;

    if ( isChecked )
    {
      if ( !isEnabled )
      {
        frameNo = _this->IconOnFrameDisabled;
        clr = _this->Appearance->IconOnTintDisabled;
      }
      else
        if ( isPressed )
        {
          frameNo = _this->IconOnFrameActive;
          clr = _this->Appearance->IconOnTintActive;
        }
        else
          if ( isFocused )
          {
            frameNo = _this->IconOnFrameFocused;
            clr = _this->Appearance->IconOnTintFocused;
          }
          else
          {
            frameNo = _this->IconOnFrameDefault;
            clr = _this->Appearance->IconOnTintDefault;
          }
    }
    else
    {
      if ( !isEnabled )
      {
        frameNo = _this->IconOffFrameDisabled;
        clr = _this->Appearance->IconOffTintDisabled;
      }
      else
        if ( isPressed )
        {
          frameNo = _this->IconOffFrameActive;
          clr = _this->Appearance->IconOffTintActive;
        }
        else
          if ( isFocused )
          {
            frameNo = _this->IconOffFrameFocused;
            clr = _this->Appearance->IconOffTintFocused;
          }
          else
          {
            frameNo = _this->IconOffFrameDefault;
            clr = _this->Appearance->IconOffTintDefault;
          }
    }

    ViewsImage_OnSetAnimated( _this->imageView, (XBool)( frameNo < 0 ));

    if ( frameNo < 0 )
      frameNo = 0;

    CoreRectView__OnSetBounds( _this->imageView, EwNewRect( area.Point1.X + _this->Appearance->IconMarginLeft, 
    area.Point1.Y + _this->Appearance->IconMarginTop, area.Point2.X - _this->Appearance->IconMarginRight, 
    area.Point2.Y - _this->Appearance->IconMarginBottom ));
    ViewsImage_OnSetAlignment( _this->imageView, _this->Appearance->IconAlignment );
    ViewsImage_OnSetBitmap( _this->imageView, _this->Icon );
    ViewsImage_OnSetFrameNumber( _this->imageView, frameNo );
    ViewsImage_OnSetColor( _this->imageView, clr );
  }
  else
    if ( _this->imageView != 0 )
    {
      XInt32 frameNo = -1;

      if ( isChecked )
      {
        if ( !isEnabled )
          frameNo = _this->IconOnFrameDisabled;
        else
          if ( isPressed )
            frameNo = _this->IconOnFrameActive;
          else
            if ( isFocused )
              frameNo = _this->IconOnFrameFocused;
            else
              frameNo = _this->IconOnFrameDefault;
      }
      else
      {
        if ( !isEnabled )
          frameNo = _this->IconOffFrameDisabled;
        else
          if ( isPressed )
            frameNo = _this->IconOffFrameActive;
          else
            if ( isFocused )
              frameNo = _this->IconOffFrameFocused;
            else
              frameNo = _this->IconOffFrameDefault;
      }

      CoreRectView__OnSetBounds( _this->imageView, area );
      ViewsImage_OnSetAlignment( _this->imageView, ViewsImageAlignmentAlignHorzCenter 
      | ViewsImageAlignmentAlignVertCenter );
      ViewsImage_OnSetBitmap( _this->imageView, _this->Icon );
      ViewsImage_OnSetFrameNumber( _this->imageView, frameNo );
      ViewsImage_OnSetColor( _this->imageView, _Const0000 );
    }

  if ( _this->textView != 0 )
  {
    XColor clr = _Const0000;
    ResourcesFont font = 0;
    XString label = 0;

    if ( isChecked )
    {
      font = _this->Appearance->LabelOnFont;
      label = _this->LabelOn;

      if ( !isEnabled )
        clr = _this->Appearance->LabelOnColorDisabled;
      else
        if ( isPressed )
          clr = _this->Appearance->LabelOnColorActive;
        else
          if ( isFocused )
            clr = _this->Appearance->LabelOnColorFocused;
          else
            clr = _this->Appearance->LabelOnColorDefault;
    }
    else
    {
      font = _this->Appearance->LabelOffFont;
      label = _this->LabelOff;

      if ( !isEnabled )
        clr = _this->Appearance->LabelOffColorDisabled;
      else
        if ( isPressed )
          clr = _this->Appearance->LabelOffColorActive;
        else
          if ( isFocused )
            clr = _this->Appearance->LabelOffColorFocused;
          else
            clr = _this->Appearance->LabelOffColorDefault;
    }

    CoreRectView__OnSetBounds( _this->textView, EwNewRect( area.Point1.X + _this->Appearance->LabelMarginLeft, 
    area.Point1.Y + _this->Appearance->LabelMarginTop, area.Point2.X - _this->Appearance->LabelMarginRight, 
    area.Point2.Y - _this->Appearance->LabelMarginBottom ));
    ViewsText_OnSetAlignment( _this->textView, _this->Appearance->LabelAlignment );
    ViewsText_OnSetFont( _this->textView, font );
    ViewsText_OnSetString( _this->textView, label );
    ViewsText_OnSetColor( _this->textView, clr );
    ViewsText_OnSetWrapText( _this->textView, 1 );
    ViewsText_OnSetEllipsis( _this->textView, 1 );
  }
}

/* 'C' function for method : 'WidgetSet::ToggleButton.onConfigChanged()' */
void WidgetSetToggleButton_onConfigChanged( WidgetSetToggleButton _this, XObject 
  sender )
{
  XEnum oldKeyCode;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  oldKeyCode = _this->KeyHandler.Filter;

  if ( _this->Appearance != 0 )
    _this->KeyHandler.Filter = _this->Appearance->KeyCode;
  else
    _this->KeyHandler.Filter = CoreKeyCodeEnter;

  if (( oldKeyCode == CoreKeyCodeNoKey ) && ( _this->KeyHandler.Filter != CoreKeyCodeNoKey ))
    CoreView__ChangeViewState( _this, CoreViewStateFocusable, 0 );

  if (( oldKeyCode != CoreKeyCodeNoKey ) && ( _this->KeyHandler.Filter == CoreKeyCodeNoKey ))
    CoreView__ChangeViewState( _this, 0, CoreViewStateFocusable );

  if ( _this->Appearance != 0 )
    CoreRectView__OnSetBounds( _this, _this->Super2.Bounds );

  CoreGroup_InvalidateViewState((CoreGroup)_this );
}

/* 'C' function for method : 'WidgetSet::ToggleButton.onFlashTimer()' */
void WidgetSetToggleButton_onFlashTimer( WidgetSetToggleButton _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  CoreGroup_InvalidateViewState((CoreGroup)_this );
  WidgetSetToggleButton_OnSetChecked( _this, (XBool)!_this->Checked );

  if ( _this->Checked )
    EwPostSignal( _this->OnSwitchOn, ((XObject)_this ));
  else
    EwPostSignal( _this->OnSwitchOff, ((XObject)_this ));

  if ( _this->Outlet.Object != 0 )
  {
    EwOnSetBool( _this->Outlet, _this->Checked );
    EwNotifyRefObservers( _this->Outlet, 0 );
  }
}

/* 'C' function for method : 'WidgetSet::ToggleButton.onReleaseKey()' */
void WidgetSetToggleButton_onReleaseKey( WidgetSetToggleButton _this, XObject sender )
{
  XInt32 pressFeedbackDuration;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  pressFeedbackDuration = 0;

  if ( _this->Appearance != 0 )
    pressFeedbackDuration = _this->Appearance->PressedFeedbackDuration;

  CoreSimpleTouchHandler_OnSetEnabled( &_this->TouchHandler, 1 );
  EwPostSignal( _this->OnLeave, ((XObject)_this ));
  EwPostSignal( _this->OnRelease, ((XObject)_this ));
  CoreGroup_InvalidateViewState((CoreGroup)_this );

  if ((XInt32)( _this->KeyHandler.Time - _this->onPressKeyTime ) >= pressFeedbackDuration )
  {
    WidgetSetToggleButton_OnSetChecked( _this, (XBool)!_this->Checked );

    if ( _this->Checked )
      EwPostSignal( _this->OnSwitchOn, ((XObject)_this ));
    else
      EwPostSignal( _this->OnSwitchOff, ((XObject)_this ));

    if ( _this->Outlet.Object != 0 )
    {
      EwOnSetBool( _this->Outlet, _this->Checked );
      EwNotifyRefObservers( _this->Outlet, 0 );
    }
  }
  else
  {
    CoreTimer_OnSetBegin( &_this->FlashTimer, pressFeedbackDuration - (XInt32)( 
    _this->KeyHandler.Time - _this->onPressKeyTime ));
    CoreTimer_OnSetEnabled( &_this->FlashTimer, 1 );
  }
}

/* 'C' function for method : 'WidgetSet::ToggleButton.onPressKey()' */
void WidgetSetToggleButton_onPressKey( WidgetSetToggleButton _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  CoreSimpleTouchHandler_OnSetEnabled( &_this->TouchHandler, 0 );
  CoreGroup_InvalidateViewState((CoreGroup)_this );

  if ( _this->FlashTimer.Enabled )
  {
    CoreTimer_OnSetEnabled( &_this->FlashTimer, 0 );
    WidgetSetToggleButton_OnSetChecked( _this, (XBool)!_this->Checked );

    if ( _this->Checked )
      EwPostSignal( _this->OnSwitchOn, ((XObject)_this ));
    else
      EwPostSignal( _this->OnSwitchOff, ((XObject)_this ));

    if ( _this->Outlet.Object != 0 )
    {
      EwOnSetBool( _this->Outlet, _this->Checked );
      EwNotifyRefObservers( _this->Outlet, 0 );
    }
  }

  _this->onPressKeyTime = _this->KeyHandler.Time;
  EwPostSignal( _this->OnPress, ((XObject)_this ));
  EwPostSignal( _this->OnEnter, ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::ToggleButton.onLeaveTouch()' */
void WidgetSetToggleButton_onLeaveTouch( WidgetSetToggleButton _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  CoreGroup_InvalidateViewState((CoreGroup)_this );
  EwPostSignal( _this->OnLeave, ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::ToggleButton.onEnterTouch()' */
void WidgetSetToggleButton_onEnterTouch( WidgetSetToggleButton _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  CoreGroup_InvalidateViewState((CoreGroup)_this );
  EwPostSignal( _this->OnEnter, ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::ToggleButton.onReleaseTouch()' */
void WidgetSetToggleButton_onReleaseTouch( WidgetSetToggleButton _this, XObject 
  sender )
{
  XInt32 pressFeedbackDuration;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  pressFeedbackDuration = 0;

  if ( _this->Appearance != 0 )
    pressFeedbackDuration = _this->Appearance->PressedFeedbackDuration;

  _this->KeyHandler.Enabled = 1;
  EwPostSignal( _this->OnRelease, ((XObject)_this ));

  if ( !_this->TouchHandler.Inside )
    return;

  if ( _this->TouchHandler.AutoDeflected )
    return;

  if ( _this->TouchHandler.HoldPeriod >= pressFeedbackDuration )
  {
    WidgetSetToggleButton_OnSetChecked( _this, (XBool)!_this->Checked );

    if ( _this->Checked )
      EwPostSignal( _this->OnSwitchOn, ((XObject)_this ));
    else
      EwPostSignal( _this->OnSwitchOff, ((XObject)_this ));

    if ( _this->Outlet.Object != 0 )
    {
      EwOnSetBool( _this->Outlet, _this->Checked );
      EwNotifyRefObservers( _this->Outlet, 0 );
    }
  }
  else
  {
    CoreTimer_OnSetBegin( &_this->FlashTimer, pressFeedbackDuration - _this->TouchHandler.HoldPeriod );
    CoreTimer_OnSetEnabled( &_this->FlashTimer, 1 );
  }
}

/* 'C' function for method : 'WidgetSet::ToggleButton.onPressTouch()' */
void WidgetSetToggleButton_onPressTouch( WidgetSetToggleButton _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  _this->KeyHandler.Enabled = 0;

  if ( _this->FlashTimer.Enabled )
  {
    CoreTimer_OnSetEnabled( &_this->FlashTimer, 0 );
    WidgetSetToggleButton_OnSetChecked( _this, (XBool)!_this->Checked );

    if ( _this->Checked )
      EwPostSignal( _this->OnSwitchOn, ((XObject)_this ));
    else
      EwPostSignal( _this->OnSwitchOff, ((XObject)_this ));

    if ( _this->Outlet.Object != 0 )
    {
      EwOnSetBool( _this->Outlet, _this->Checked );
      EwNotifyRefObservers( _this->Outlet, 0 );
    }
  }

  EwPostSignal( _this->OnPress, ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::ToggleButton.OnSetChecked()' */
void WidgetSetToggleButton_OnSetChecked( WidgetSetToggleButton _this, XBool value )
{
  if ( _this->Checked == value )
    return;

  _this->Checked = value;
  CoreGroup_InvalidateViewState((CoreGroup)_this );
}

/* 'C' function for method : 'WidgetSet::ToggleButton.OnSetAppearance()' */
void WidgetSetToggleButton_OnSetAppearance( WidgetSetToggleButton _this, WidgetSetToggleButtonConfig 
  value )
{
  if ( _this->Appearance == value )
    return;

  if ( _this->Appearance != 0 )
    EwDetachObjObserver( EwNewSlot( _this, WidgetSetToggleButton_onConfigChanged ), 
      (XObject)_this->Appearance, 0 );

  _this->Appearance = value;

  if ( value != 0 )
    EwAttachObjObserver( EwNewSlot( _this, WidgetSetToggleButton_onConfigChanged ), 
      (XObject)value, 0 );

  EwPostSignal( EwNewSlot( _this, WidgetSetToggleButton_onConfigChanged ), ((XObject)_this ));
}

/* Variants derived from the class : 'WidgetSet::ToggleButton' */
EW_DEFINE_CLASS_VARIANTS( WidgetSetToggleButton )
EW_END_OF_CLASS_VARIANTS( WidgetSetToggleButton )

/* Virtual Method Table (VMT) for the class : 'WidgetSet::ToggleButton' */
EW_DEFINE_CLASS( WidgetSetToggleButton, CoreGroup, textView, OnLeave, Outlet, FlashTimer, 
                 LabelOn, onPressKeyTime, "WidgetSet::ToggleButton" )
  CoreRectView_initLayoutContext,
  CoreView_GetRoot,
  CoreGroup_Draw,
  CoreView_HandleEvent,
  CoreGroup_CursorHitTest,
  CoreRectView_ArrangeView,
  CoreRectView_MoveView,
  CoreRectView_GetExtent,
  CoreGroup_ChangeViewState,
  WidgetSetToggleButton_OnSetBounds,
  CoreGroup_OnSetFocus,
  CoreGroup_OnSetBuffered,
  CoreGroup_OnSetOpacity,
  CoreGroup_DispatchEvent,
  CoreGroup_BroadcastEvent,
  CoreGroup_UpdateLayout,
  WidgetSetToggleButton_UpdateViewState,
  CoreGroup_InvalidateArea,
EW_END_OF_CLASS( WidgetSetToggleButton )

/* Initializer for the class 'WidgetSet::PushButton' */
void WidgetSetPushButton__Init( WidgetSetPushButton _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( WidgetSetPushButton );

  /* ... then construct all embedded objects */
  CoreTimer__Init( &_this->FlashTimer, &_this->_XObject, 0 );
  CoreKeyPressHandler__Init( &_this->KeyHandler, &_this->_XObject, 0 );
  CoreSimpleTouchHandler__Init( &_this->TouchHandler, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( WidgetSetPushButton );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const000A );
  CoreTimer_OnSetPeriod( &_this->FlashTimer, 0 );
  CoreTimer_OnSetBegin( &_this->FlashTimer, 50 );
  _this->KeyHandler.Filter = CoreKeyCodeEnter;
  CoreView_OnSetLayout((CoreView)&_this->TouchHandler, CoreLayoutAlignToBottom | 
  CoreLayoutAlignToLeft | CoreLayoutAlignToRight | CoreLayoutAlignToTop | CoreLayoutResizeHorz 
  | CoreLayoutResizeVert );
  CoreQuadView__OnSetPoint4( &_this->TouchHandler, _Const0006 );
  CoreQuadView__OnSetPoint3( &_this->TouchHandler, _Const000B );
  CoreQuadView__OnSetPoint2( &_this->TouchHandler, _Const000C );
  CoreQuadView__OnSetPoint1( &_this->TouchHandler, _Const0003 );
  CoreSimpleTouchHandler_OnSetRetargetOffset( &_this->TouchHandler, 16 );
  CoreSimpleTouchHandler_OnSetMaxStrikeCount( &_this->TouchHandler, 100 );
  _this->IconFrameActive = -1;
  _this->IconFrameFocused = -1;
  _this->IconFrameDisabled = -1;
  _this->IconFrameDefault = -1;
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->TouchHandler ), 0 );
  _this->FlashTimer.OnTrigger = EwNewSlot( _this, WidgetSetPushButton_onFlashTimer );
  _this->KeyHandler.OnRelease = EwNewSlot( _this, WidgetSetPushButton_onReleaseKey );
  _this->KeyHandler.OnPress = EwNewSlot( _this, WidgetSetPushButton_onPressKey );
  _this->TouchHandler.OnLeave = EwNewSlot( _this, WidgetSetPushButton_onLeaveTouch );
  _this->TouchHandler.OnEnter = EwNewSlot( _this, WidgetSetPushButton_onEnterTouch );
  _this->TouchHandler.OnRelease = EwNewSlot( _this, WidgetSetPushButton_onReleaseTouch );
  _this->TouchHandler.OnPress = EwNewSlot( _this, WidgetSetPushButton_onPressTouch );
}

/* Re-Initializer for the class 'WidgetSet::PushButton' */
void WidgetSetPushButton__ReInit( WidgetSetPushButton _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  CoreTimer__ReInit( &_this->FlashTimer );
  CoreKeyPressHandler__ReInit( &_this->KeyHandler );
  CoreSimpleTouchHandler__ReInit( &_this->TouchHandler );
}

/* Finalizer method for the class 'WidgetSet::PushButton' */
void WidgetSetPushButton__Done( WidgetSetPushButton _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreGroup );

  /* Finalize all embedded objects */
  CoreTimer__Done( &_this->FlashTimer );
  CoreKeyPressHandler__Done( &_this->KeyHandler );
  CoreSimpleTouchHandler__Done( &_this->TouchHandler );

  /* Don't forget to deinitialize the super class ... */
  CoreGroup__Done( &_this->_Super );
}

/* 'C' function for method : 'WidgetSet::PushButton.OnSetBounds()' */
void WidgetSetPushButton_OnSetBounds( WidgetSetPushButton _this, XRect value )
{
  if ( _this->Appearance != 0 )
  {
    XPoint cs = EwGetRectSize( value );
    XPoint ns = cs;
    XPoint delta;

    if ( ns.X < _this->Appearance->WidgetMinSize.X )
      ns.X = _this->Appearance->WidgetMinSize.X;

    if ( ns.Y < _this->Appearance->WidgetMinSize.Y )
      ns.Y = _this->Appearance->WidgetMinSize.Y;

    if (( _this->Appearance->WidgetMaxSize.X > 0 ) && ( ns.X > _this->Appearance->WidgetMaxSize.X ))
      ns.X = _this->Appearance->WidgetMaxSize.X;

    if (( _this->Appearance->WidgetMaxSize.Y > 0 ) && ( ns.Y > _this->Appearance->WidgetMaxSize.Y ))
      ns.Y = _this->Appearance->WidgetMaxSize.Y;

    delta = EwMovePointNeg( ns, cs );

    if ( delta.X != 0 )
    {
      XBool alignToLeft = (( _this->Super3.Layout & CoreLayoutAlignToLeft ) == CoreLayoutAlignToLeft );
      XBool alignToRight = (( _this->Super3.Layout & CoreLayoutAlignToRight ) == 
        CoreLayoutAlignToRight );

      if ( alignToLeft && !alignToRight )
        value.Point2.X = ( value.Point2.X + delta.X );
      else
        if ( !alignToLeft && alignToRight )
          value.Point1.X = ( value.Point1.X - delta.X );
        else
        {
          value.Point1.X = ( value.Point1.X - ( delta.X / 2 ));
          value.Point2.X = ( value.Point1.X + ns.X );
        }
    }

    if ( delta.Y != 0 )
    {
      XBool alignToTop = (( _this->Super3.Layout & CoreLayoutAlignToTop ) == CoreLayoutAlignToTop );
      XBool alignToBottom = (( _this->Super3.Layout & CoreLayoutAlignToBottom ) 
        == CoreLayoutAlignToBottom );

      if ( alignToTop && !alignToBottom )
        value.Point2.Y = ( value.Point2.Y + delta.Y );
      else
        if ( !alignToTop && alignToBottom )
          value.Point1.Y = ( value.Point1.Y - delta.Y );
        else
        {
          value.Point1.Y = ( value.Point1.Y - ( delta.Y / 2 ));
          value.Point2.Y = ( value.Point1.Y + ns.Y );
        }
    }
  }

  CoreGroup_OnSetBounds((CoreGroup)_this, value );
}

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
void WidgetSetPushButton_UpdateViewState( WidgetSetPushButton _this, XSet aState )
{
  XBool needsFace;
  XBool needsIcon;
  XBool needsLabel;
  XBool restack;
  XRect area;
  XBool isEnabled;
  XBool isFocused;
  XBool isPressed;

  CoreGroup_UpdateViewState((CoreGroup)_this, aState );
  needsFace = (XBool)(( _this->Appearance != 0 ) && (((( _this->Appearance->FaceDefault 
  != 0 ) || ( _this->Appearance->FaceDisabled != 0 )) || ( _this->Appearance->FaceFocused 
  != 0 )) || ( _this->Appearance->FaceActive != 0 )));
  needsIcon = (XBool)( _this->Icon != 0 );
  needsLabel = (XBool)((( _this->Appearance != 0 ) && ( EwCompString( _this->Label, 
  0 ) != 0 )) && ( _this->Appearance->LabelFont != 0 ));
  restack = 0;
  area = EwGetRectORect( _this->Super2.Bounds );

  if ( needsFace && ( _this->frameView == 0 ))
  {
    _this->frameView = EwNewObject( ViewsFrame, 0 );
    CoreGroup_Add((CoreGroup)_this, ((CoreView)_this->frameView ), 0 );
    restack = 1;
  }
  else
    if ( !needsFace && ( _this->frameView != 0 ))
    {
      CoreGroup_Remove((CoreGroup)_this, ((CoreView)_this->frameView ));
      _this->frameView = 0;
    }

  if ( needsIcon && ( _this->imageView == 0 ))
  {
    _this->imageView = EwNewObject( ViewsImage, 0 );
    CoreGroup_Add((CoreGroup)_this, ((CoreView)_this->imageView ), 0 );
    restack = 1;
  }
  else
    if ( !needsIcon && ( _this->imageView != 0 ))
    {
      CoreGroup_Remove((CoreGroup)_this, ((CoreView)_this->imageView ));
      _this->imageView = 0;
    }

  if ( needsLabel && ( _this->textView == 0 ))
  {
    _this->textView = EwNewObject( ViewsText, 0 );
    CoreGroup_Add((CoreGroup)_this, ((CoreView)_this->textView ), 0 );
    restack = 1;
    ViewsText_OnSetEnableBidiText( _this->textView, 1 );
  }
  else
    if ( !needsLabel && ( _this->textView != 0 ))
    {
      CoreGroup_Remove((CoreGroup)_this, ((CoreView)_this->textView ));
      _this->textView = 0;
    }

  if ( restack )
  {
    if ( _this->frameView != 0 )
      CoreGroup_RestackTop((CoreGroup)_this, ((CoreView)_this->frameView ));

    if ( _this->imageView != 0 )
      CoreGroup_RestackTop((CoreGroup)_this, ((CoreView)_this->imageView ));

    if ( _this->textView != 0 )
      CoreGroup_RestackTop((CoreGroup)_this, ((CoreView)_this->textView ));
  }

  isEnabled = (( aState & CoreViewStateEnabled ) == CoreViewStateEnabled );
  isFocused = (( aState & CoreViewStateFocused ) == CoreViewStateFocused );
  isPressed = (XBool)((( _this->TouchHandler.Down && _this->TouchHandler.Inside ) 
  || _this->KeyHandler.Down ) || _this->FlashTimer.Enabled );

  if ( _this->frameView != 0 )
  {
    ResourcesBitmap bitmap = 0;
    XInt32 frameNo = -1;
    XColor tint = _Const0000;
    XSet layout = _this->Appearance->FaceLayout;
    XRect r = area;

    if ( !isEnabled )
    {
      bitmap = _this->Appearance->FaceDisabled;
      frameNo = _this->Appearance->FaceFrameDisabled;
      tint = _this->Appearance->FaceTintDisabled;
    }
    else
      if ( isPressed )
      {
        bitmap = _this->Appearance->FaceActive;
        frameNo = _this->Appearance->FaceFrameActive;
        tint = _this->Appearance->FaceTintActive;
      }
      else
        if ( isFocused )
        {
          bitmap = _this->Appearance->FaceFocused;
          frameNo = _this->Appearance->FaceFrameFocused;
          tint = _this->Appearance->FaceTintFocused;
        }
        else
        {
          bitmap = _this->Appearance->FaceDefault;
          frameNo = _this->Appearance->FaceFrameDefault;
          tint = _this->Appearance->FaceTintDefault;
        }

    ViewsFrame_OnSetAnimated( _this->frameView, (XBool)( frameNo < 0 ));
    ViewsFrame_OnSetColor( _this->frameView, tint );

    if ( frameNo < 0 )
      frameNo = 0;

    if (( bitmap != 0 ) && !(( layout & CoreLayoutResizeHorz ) == CoreLayoutResizeHorz ))
    {
      XBool alignToLeft = (( layout & CoreLayoutAlignToLeft ) == CoreLayoutAlignToLeft );
      XBool alignToRight = (( layout & CoreLayoutAlignToRight ) == CoreLayoutAlignToRight );
      XInt32 w = bitmap->FrameSize.X;

      if ( alignToLeft && !alignToRight )
        r.Point2.X = ( r.Point1.X + w );
      else
        if ( !alignToLeft && alignToRight )
          r.Point1.X = ( r.Point2.X - w );
        else
        {
          r.Point1.X = (( EwGetRectW( r ) / 2 ) - ( w / 2 ));
          r.Point2.X = ( r.Point1.X + w );
        }
    }

    if (( bitmap != 0 ) && !(( layout & CoreLayoutResizeVert ) == CoreLayoutResizeVert ))
    {
      XBool alignToTop = (( layout & CoreLayoutAlignToTop ) == CoreLayoutAlignToTop );
      XBool alignToBottom = (( layout & CoreLayoutAlignToBottom ) == CoreLayoutAlignToBottom );
      XInt32 h = bitmap->FrameSize.Y;

      if ( alignToTop && !alignToBottom )
        r.Point2.Y = ( r.Point1.Y + h );
      else
        if ( !alignToTop && alignToBottom )
          r.Point1.Y = ( r.Point2.Y - h );
        else
        {
          r.Point1.Y = (( EwGetRectH( r ) / 2 ) - ( h / 2 ));
          r.Point2.Y = ( r.Point1.Y + h );
        }
    }

    ViewsFrame_OnSetFrameNumber( _this->frameView, frameNo );
    ViewsFrame_OnSetBitmap( _this->frameView, bitmap );
    CoreRectView__OnSetBounds( _this->frameView, r );
  }

  if (( _this->imageView != 0 ) && ( _this->Appearance != 0 ))
  {
    XInt32 frameNo = -1;
    XColor clr = _Const0000;

    if ( !isEnabled )
    {
      frameNo = _this->IconFrameDisabled;
      clr = _this->Appearance->IconTintDisabled;
    }
    else
      if ( isPressed )
      {
        frameNo = _this->IconFrameActive;
        clr = _this->Appearance->IconTintActive;
      }
      else
        if ( isFocused )
        {
          frameNo = _this->IconFrameFocused;
          clr = _this->Appearance->IconTintFocused;
        }
        else
        {
          frameNo = _this->IconFrameDefault;
          clr = _this->Appearance->IconTintDefault;
        }

    ViewsImage_OnSetAnimated( _this->imageView, (XBool)( frameNo < 0 ));

    if ( frameNo < 0 )
      frameNo = 0;

    CoreRectView__OnSetBounds( _this->imageView, EwNewRect( area.Point1.X + _this->Appearance->IconMarginLeft, 
    area.Point1.Y + _this->Appearance->IconMarginTop, area.Point2.X - _this->Appearance->IconMarginRight, 
    area.Point2.Y - _this->Appearance->IconMarginBottom ));
    ViewsImage_OnSetAlignment( _this->imageView, _this->Appearance->IconAlignment );
    ViewsImage_OnSetBitmap( _this->imageView, _this->Icon );
    ViewsImage_OnSetFrameNumber( _this->imageView, frameNo );
    ViewsImage_OnSetColor( _this->imageView, clr );
  }
  else
    if ( _this->imageView != 0 )
    {
      XInt32 frameNo = -1;

      if ( !isEnabled )
      {
        frameNo = _this->IconFrameDisabled;
      }
      else
        if ( isPressed )
        {
          frameNo = _this->IconFrameActive;
        }
        else
          if ( isFocused )
          {
            frameNo = _this->IconFrameFocused;
          }
          else
          {
            frameNo = _this->IconFrameDefault;
          }

      CoreRectView__OnSetBounds( _this->imageView, area );
      ViewsImage_OnSetAlignment( _this->imageView, ViewsImageAlignmentAlignHorzCenter 
      | ViewsImageAlignmentAlignVertCenter );
      ViewsImage_OnSetBitmap( _this->imageView, _this->Icon );
      ViewsImage_OnSetFrameNumber( _this->imageView, frameNo );
      ViewsImage_OnSetColor( _this->imageView, _Const0000 );
    }

  if ( _this->textView != 0 )
  {
    XColor clr = _Const0000;

    if ( !isEnabled )
      clr = _this->Appearance->LabelColorDisabled;
    else
      if ( isPressed )
        clr = _this->Appearance->LabelColorActive;
      else
        if ( isFocused )
          clr = _this->Appearance->LabelColorFocused;
        else
          clr = _this->Appearance->LabelColorDefault;

    CoreRectView__OnSetBounds( _this->textView, EwNewRect( area.Point1.X + _this->Appearance->LabelMarginLeft, 
    area.Point1.Y + _this->Appearance->LabelMarginTop, area.Point2.X - _this->Appearance->LabelMarginRight, 
    area.Point2.Y - _this->Appearance->LabelMarginBottom ));
    ViewsText_OnSetAlignment( _this->textView, _this->Appearance->LabelAlignment );
    ViewsText_OnSetWrapText( _this->textView, 1 );
    ViewsText_OnSetEllipsis( _this->textView, 1 );
    ViewsText_OnSetFont( _this->textView, _this->Appearance->LabelFont );
    ViewsText_OnSetString( _this->textView, _this->Label );
    ViewsText_OnSetColor( _this->textView, clr );
  }
}

/* 'C' function for method : 'WidgetSet::PushButton.onConfigChanged()' */
void WidgetSetPushButton_onConfigChanged( WidgetSetPushButton _this, XObject sender )
{
  XEnum oldKeyCode;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  oldKeyCode = _this->KeyHandler.Filter;

  if ( _this->Appearance != 0 )
    _this->KeyHandler.Filter = _this->Appearance->KeyCode;
  else
    _this->KeyHandler.Filter = CoreKeyCodeEnter;

  if (( oldKeyCode == CoreKeyCodeNoKey ) && ( _this->KeyHandler.Filter != CoreKeyCodeNoKey ))
    CoreView__ChangeViewState( _this, CoreViewStateFocusable, 0 );

  if (( oldKeyCode != CoreKeyCodeNoKey ) && ( _this->KeyHandler.Filter == CoreKeyCodeNoKey ))
    CoreView__ChangeViewState( _this, 0, CoreViewStateFocusable );

  if ( _this->Appearance != 0 )
    CoreRectView__OnSetBounds( _this, _this->Super2.Bounds );

  CoreGroup_InvalidateViewState((CoreGroup)_this );
}

/* 'C' function for method : 'WidgetSet::PushButton.onFlashTimer()' */
void WidgetSetPushButton_onFlashTimer( WidgetSetPushButton _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  CoreGroup_InvalidateViewState((CoreGroup)_this );
  EwPostSignal( _this->OnActivate, ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::PushButton.onReleaseKey()' */
void WidgetSetPushButton_onReleaseKey( WidgetSetPushButton _this, XObject sender )
{
  XInt32 pressFeedbackDuration;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  pressFeedbackDuration = 0;

  if ( _this->Appearance != 0 )
    pressFeedbackDuration = _this->Appearance->PressedFeedbackDuration;

  CoreSimpleTouchHandler_OnSetEnabled( &_this->TouchHandler, 1 );
  EwPostSignal( _this->OnLeave, ((XObject)_this ));
  EwPostSignal( _this->OnRelease, ((XObject)_this ));
  CoreGroup_InvalidateViewState((CoreGroup)_this );

  if ((XInt32)( _this->KeyHandler.Time - _this->onPressKeyTime ) >= pressFeedbackDuration )
    EwPostSignal( _this->OnActivate, ((XObject)_this ));
  else
  {
    CoreTimer_OnSetBegin( &_this->FlashTimer, pressFeedbackDuration - (XInt32)( 
    _this->KeyHandler.Time - _this->onPressKeyTime ));
    CoreTimer_OnSetEnabled( &_this->FlashTimer, 1 );
  }
}

/* 'C' function for method : 'WidgetSet::PushButton.onPressKey()' */
void WidgetSetPushButton_onPressKey( WidgetSetPushButton _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  CoreSimpleTouchHandler_OnSetEnabled( &_this->TouchHandler, 0 );
  CoreGroup_InvalidateViewState((CoreGroup)_this );

  if ( _this->FlashTimer.Enabled )
  {
    EwPostSignal( _this->OnActivate, ((XObject)_this ));
    CoreTimer_OnSetEnabled( &_this->FlashTimer, 0 );
  }

  _this->onPressKeyTime = _this->KeyHandler.Time;
  EwPostSignal( _this->OnPress, ((XObject)_this ));
  EwPostSignal( _this->OnEnter, ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::PushButton.onLeaveTouch()' */
void WidgetSetPushButton_onLeaveTouch( WidgetSetPushButton _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  CoreGroup_InvalidateViewState((CoreGroup)_this );
  EwPostSignal( _this->OnLeave, ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::PushButton.onEnterTouch()' */
void WidgetSetPushButton_onEnterTouch( WidgetSetPushButton _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  CoreGroup_InvalidateViewState((CoreGroup)_this );
  EwPostSignal( _this->OnEnter, ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::PushButton.onReleaseTouch()' */
void WidgetSetPushButton_onReleaseTouch( WidgetSetPushButton _this, XObject sender )
{
  XInt32 pressFeedbackDuration;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  pressFeedbackDuration = 0;

  if ( _this->Appearance != 0 )
    pressFeedbackDuration = _this->Appearance->PressedFeedbackDuration;

  _this->KeyHandler.Enabled = 1;
  EwPostSignal( _this->OnRelease, ((XObject)_this ));

  if ( !_this->TouchHandler.Inside )
    return;

  if ( _this->TouchHandler.AutoDeflected )
    return;

  if ( _this->TouchHandler.HoldPeriod >= pressFeedbackDuration )
    EwPostSignal( _this->OnActivate, ((XObject)_this ));
  else
  {
    CoreTimer_OnSetBegin( &_this->FlashTimer, pressFeedbackDuration - _this->TouchHandler.HoldPeriod );
    CoreTimer_OnSetEnabled( &_this->FlashTimer, 1 );
  }
}

/* 'C' function for method : 'WidgetSet::PushButton.onPressTouch()' */
void WidgetSetPushButton_onPressTouch( WidgetSetPushButton _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  _this->KeyHandler.Enabled = 0;

  if ( _this->FlashTimer.Enabled )
  {
    EwPostSignal( _this->OnActivate, ((XObject)_this ));
    CoreTimer_OnSetEnabled( &_this->FlashTimer, 0 );
  }

  EwPostSignal( _this->OnPress, ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::PushButton.OnSetIconFrameActive()' */
void WidgetSetPushButton_OnSetIconFrameActive( WidgetSetPushButton _this, XInt32 
  value )
{
  if ( _this->IconFrameActive == value )
    return;

  _this->IconFrameActive = value;
  CoreGroup_InvalidateViewState((CoreGroup)_this );
}

/* 'C' function for method : 'WidgetSet::PushButton.OnSetIconFrameFocused()' */
void WidgetSetPushButton_OnSetIconFrameFocused( WidgetSetPushButton _this, XInt32 
  value )
{
  if ( _this->IconFrameFocused == value )
    return;

  _this->IconFrameFocused = value;
  CoreGroup_InvalidateViewState((CoreGroup)_this );
}

/* 'C' function for method : 'WidgetSet::PushButton.OnSetIconFrameDisabled()' */
void WidgetSetPushButton_OnSetIconFrameDisabled( WidgetSetPushButton _this, XInt32 
  value )
{
  if ( _this->IconFrameDisabled == value )
    return;

  _this->IconFrameDisabled = value;
  CoreGroup_InvalidateViewState((CoreGroup)_this );
}

/* 'C' function for method : 'WidgetSet::PushButton.OnSetIconFrameDefault()' */
void WidgetSetPushButton_OnSetIconFrameDefault( WidgetSetPushButton _this, XInt32 
  value )
{
  if ( _this->IconFrameDefault == value )
    return;

  _this->IconFrameDefault = value;
  CoreGroup_InvalidateViewState((CoreGroup)_this );
}

/* 'C' function for method : 'WidgetSet::PushButton.OnSetIcon()' */
void WidgetSetPushButton_OnSetIcon( WidgetSetPushButton _this, ResourcesBitmap value )
{
  if ( _this->Icon == value )
    return;

  _this->Icon = value;
  CoreGroup_InvalidateViewState((CoreGroup)_this );
}

/* 'C' function for method : 'WidgetSet::PushButton.OnSetLabel()' */
void WidgetSetPushButton_OnSetLabel( WidgetSetPushButton _this, XString value )
{
  if ( !EwCompString( _this->Label, value ))
    return;

  _this->Label = EwShareString( value );
  CoreGroup_InvalidateViewState((CoreGroup)_this );
}

/* 'C' function for method : 'WidgetSet::PushButton.OnSetAppearance()' */
void WidgetSetPushButton_OnSetAppearance( WidgetSetPushButton _this, WidgetSetPushButtonConfig 
  value )
{
  if ( _this->Appearance == value )
    return;

  if ( _this->Appearance != 0 )
    EwDetachObjObserver( EwNewSlot( _this, WidgetSetPushButton_onConfigChanged ), 
      (XObject)_this->Appearance, 0 );

  _this->Appearance = value;

  if ( value != 0 )
    EwAttachObjObserver( EwNewSlot( _this, WidgetSetPushButton_onConfigChanged ), 
      (XObject)value, 0 );

  EwPostSignal( EwNewSlot( _this, WidgetSetPushButton_onConfigChanged ), ((XObject)_this ));
}

/* Variants derived from the class : 'WidgetSet::PushButton' */
EW_DEFINE_CLASS_VARIANTS( WidgetSetPushButton )
EW_END_OF_CLASS_VARIANTS( WidgetSetPushButton )

/* Virtual Method Table (VMT) for the class : 'WidgetSet::PushButton' */
EW_DEFINE_CLASS( WidgetSetPushButton, CoreGroup, textView, OnLeave, FlashTimer, 
                 FlashTimer, Label, onPressKeyTime, "WidgetSet::PushButton" )
  CoreRectView_initLayoutContext,
  CoreView_GetRoot,
  CoreGroup_Draw,
  CoreView_HandleEvent,
  CoreGroup_CursorHitTest,
  CoreRectView_ArrangeView,
  CoreRectView_MoveView,
  CoreRectView_GetExtent,
  CoreGroup_ChangeViewState,
  WidgetSetPushButton_OnSetBounds,
  CoreGroup_OnSetFocus,
  CoreGroup_OnSetBuffered,
  CoreGroup_OnSetOpacity,
  CoreGroup_DispatchEvent,
  CoreGroup_BroadcastEvent,
  CoreGroup_UpdateLayout,
  WidgetSetPushButton_UpdateViewState,
  CoreGroup_InvalidateArea,
EW_END_OF_CLASS( WidgetSetPushButton )

/* Embedded Wizard */
