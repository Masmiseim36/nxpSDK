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

#include "ewlocale.h"
#include "_CoreKeyPressHandler.h"
#include "_CoreRoot.h"
#include "_CoreSimpleTouchHandler.h"
#include "_CoreTimer.h"
#include "_CoreView.h"
#include "_ResourcesBitmap.h"
#include "_ViewsFrame.h"
#include "_ViewsImage.h"
#include "_ViewsText.h"
#include "_WidgetSetHorizontalSlider.h"
#include "_WidgetSetHorizontalSliderConfig.h"
#include "_WidgetSetRadioButton.h"
#include "_WidgetSetRadioButtonConfig.h"
#include "_WidgetSetWidgetConfig.h"
#include "Core.h"
#include "Graphics.h"
#include "Views.h"
#include "WidgetSet.h"

/* Constant values used in this 'C' module only. */
static const XPoint _Const0000 = { 30, 26 };
static const XColor _Const0001 = { 0xFF, 0xFF, 0xFF, 0xFF };
static const XColor _Const0002 = { 0x00, 0x00, 0x00, 0xFF };
static const XRect _Const0003 = {{ 0, 0 }, { 200, 50 }};
static const XPoint _Const0004 = { 0, 50 };
static const XPoint _Const0005 = { 200, 50 };
static const XPoint _Const0006 = { 200, 0 };
static const XPoint _Const0007 = { 0, 0 };
static const XRect _Const0008 = {{ 0, 0 }, { 150, 50 }};
static const XPoint _Const0009 = { 150, 50 };
static const XPoint _Const000A = { 150, 0 };

/* Forward declaration of the class Resources::Font */
#ifndef _ResourcesFont_
  EW_DECLARE_CLASS( ResourcesFont )
#define _ResourcesFont_
#endif

/* Include a file containing the bitmap resource : 'WidgetSet::CheckBoxSmall' */
#include "_WidgetSetCheckBoxSmall.h"

/* Table with links to derived variants of the bitmap resource : 'WidgetSet::CheckBoxSmall' */
EW_RES_WITHOUT_VARIANTS( WidgetSetCheckBoxSmall )

/* Include a file containing the bitmap resource : 'WidgetSet::HorizontalSliderTrackSmall' */
#include "_WidgetSetHorizontalSliderTrackSmall.h"

/* Table with links to derived variants of the bitmap resource : 'WidgetSet::HorizontalSliderTrackSmall' */
EW_RES_WITHOUT_VARIANTS( WidgetSetHorizontalSliderTrackSmall )

/* Include a file containing the bitmap resource : 'WidgetSet::ThumbKnobSmall' */
#include "_WidgetSetThumbKnobSmall.h"

/* Table with links to derived variants of the bitmap resource : 'WidgetSet::ThumbKnobSmall' */
EW_RES_WITHOUT_VARIANTS( WidgetSetThumbKnobSmall )

/* This autoobject provides the default customization for the 'horizontal slider' 
   widget (WidgetSet::HorizontalSlider) in its small size variant. */
EW_DEFINE_AUTOOBJECT( WidgetSetHorizontalSlider_Small, WidgetSetHorizontalSliderConfig )

/* Initializer for the auto object 'WidgetSet::HorizontalSlider_Small' */
void WidgetSetHorizontalSlider_Small__Init( WidgetSetHorizontalSliderConfig _this )
{
  WidgetSetHorizontalSliderConfig_OnSetKeyRepeatPeriod( _this, 100 );
  WidgetSetHorizontalSliderConfig_OnSetKeyRepeatDelay( _this, 500 );
  WidgetSetHorizontalSliderConfig_OnSetThumbMarginRight( _this, -1 );
  WidgetSetHorizontalSliderConfig_OnSetThumbMarginLeft( _this, -1 );
  WidgetSetHorizontalSliderConfig_OnSetThumbFrameActive( _this, 3 );
  WidgetSetHorizontalSliderConfig_OnSetThumbFrameFocused( _this, 2 );
  WidgetSetHorizontalSliderConfig_OnSetThumbFrameDisabled( _this, 0 );
  WidgetSetHorizontalSliderConfig_OnSetThumbFrameDefault( _this, 1 );
  WidgetSetHorizontalSliderConfig_OnSetThumbActive( _this, EwLoadResource( &WidgetSetThumbKnobSmall, 
  ResourcesBitmap ));
  WidgetSetHorizontalSliderConfig_OnSetThumbFocused( _this, EwLoadResource( &WidgetSetThumbKnobSmall, 
  ResourcesBitmap ));
  WidgetSetHorizontalSliderConfig_OnSetThumbDisabled( _this, EwLoadResource( &WidgetSetThumbKnobSmall, 
  ResourcesBitmap ));
  WidgetSetHorizontalSliderConfig_OnSetThumbDefault( _this, EwLoadResource( &WidgetSetThumbKnobSmall, 
  ResourcesBitmap ));
  WidgetSetHorizontalSliderConfig_OnSetTrackRightFrameActive( _this, 0 );
  WidgetSetHorizontalSliderConfig_OnSetTrackRightFrameFocused( _this, 0 );
  WidgetSetHorizontalSliderConfig_OnSetTrackRightFrameDisabled( _this, 0 );
  WidgetSetHorizontalSliderConfig_OnSetTrackRightFrameDefault( _this, 0 );
  WidgetSetHorizontalSliderConfig_OnSetTrackRightActive( _this, EwLoadResource( 
  &WidgetSetHorizontalSliderTrackSmall, ResourcesBitmap ));
  WidgetSetHorizontalSliderConfig_OnSetTrackRightFocused( _this, EwLoadResource( 
  &WidgetSetHorizontalSliderTrackSmall, ResourcesBitmap ));
  WidgetSetHorizontalSliderConfig_OnSetTrackRightDisabled( _this, EwLoadResource( 
  &WidgetSetHorizontalSliderTrackSmall, ResourcesBitmap ));
  WidgetSetHorizontalSliderConfig_OnSetTrackRightDefault( _this, EwLoadResource( 
  &WidgetSetHorizontalSliderTrackSmall, ResourcesBitmap ));
  WidgetSetHorizontalSliderConfig_OnSetTrackLeftFrameActive( _this, 1 );
  WidgetSetHorizontalSliderConfig_OnSetTrackLeftFrameFocused( _this, 1 );
  WidgetSetHorizontalSliderConfig_OnSetTrackLeftFrameDisabled( _this, 1 );
  WidgetSetHorizontalSliderConfig_OnSetTrackLeftFrameDefault( _this, 1 );
  WidgetSetHorizontalSliderConfig_OnSetTrackLeftActive( _this, EwLoadResource( &WidgetSetHorizontalSliderTrackSmall, 
  ResourcesBitmap ));
  WidgetSetHorizontalSliderConfig_OnSetTrackLeftFocused( _this, EwLoadResource( 
  &WidgetSetHorizontalSliderTrackSmall, ResourcesBitmap ));
  WidgetSetHorizontalSliderConfig_OnSetTrackLeftDisabled( _this, EwLoadResource( 
  &WidgetSetHorizontalSliderTrackSmall, ResourcesBitmap ));
  WidgetSetHorizontalSliderConfig_OnSetTrackLeftDefault( _this, EwLoadResource( 
  &WidgetSetHorizontalSliderTrackSmall, ResourcesBitmap ));
  WidgetSetHorizontalSliderConfig_OnSetWidgetMinSize( _this, _Const0000 );
}

/* Table with links to derived variants of the auto object : 'WidgetSet::HorizontalSlider_Small' */
EW_DEFINE_AUTOOBJECT_VARIANTS( WidgetSetHorizontalSlider_Small )
EW_END_OF_AUTOOBJECT_VARIANTS( WidgetSetHorizontalSlider_Small )

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
  _this->CoverTintActive = _Const0001;
  _this->CoverTintFocused = _Const0001;
  _this->CoverTintDisabled = _Const0001;
  _this->CoverTintDefault = _Const0001;
  _this->CoverFrameActive = -1;
  _this->CoverFrameFocused = -1;
  _this->CoverFrameDisabled = -1;
  _this->CoverFrameDefault = -1;
  _this->ThumbTintActive = _Const0001;
  _this->ThumbTintFocused = _Const0001;
  _this->ThumbTintDisabled = _Const0001;
  _this->ThumbTintDefault = _Const0001;
  _this->ThumbFrameActive = -1;
  _this->ThumbFrameFocused = -1;
  _this->ThumbFrameDisabled = -1;
  _this->ThumbFrameDefault = -1;
  _this->TrackRightTintActive = _Const0001;
  _this->TrackRightTintFocused = _Const0001;
  _this->TrackRightTintDisabled = _Const0001;
  _this->TrackRightTintDefault = _Const0001;
  _this->TrackRightFrameActive = -1;
  _this->TrackRightFrameFocused = -1;
  _this->TrackRightFrameDisabled = -1;
  _this->TrackRightFrameDefault = -1;
  _this->TrackLeftTintActive = _Const0001;
  _this->TrackLeftTintFocused = _Const0001;
  _this->TrackLeftTintDisabled = _Const0001;
  _this->TrackLeftTintDefault = _Const0001;
  _this->TrackLeftFrameActive = -1;
  _this->TrackLeftFrameFocused = -1;
  _this->TrackLeftFrameDisabled = -1;
  _this->TrackLeftFrameDefault = -1;
  _this->FaceTintActive = _Const0001;
  _this->FaceTintFocused = _Const0001;
  _this->FaceTintDisabled = _Const0001;
  _this->FaceTintDefault = _Const0001;
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

/* 'C' function for method : 'WidgetSet::HorizontalSliderConfig.OnSetKeyRepeatPeriod()' */
void WidgetSetHorizontalSliderConfig_OnSetKeyRepeatPeriod( WidgetSetHorizontalSliderConfig _this, 
  XInt32 value )
{
  if ( _this->KeyRepeatPeriod == value )
    return;

  _this->KeyRepeatPeriod = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::HorizontalSliderConfig.OnSetKeyRepeatDelay()' */
void WidgetSetHorizontalSliderConfig_OnSetKeyRepeatDelay( WidgetSetHorizontalSliderConfig _this, 
  XInt32 value )
{
  if ( _this->KeyRepeatDelay == value )
    return;

  _this->KeyRepeatDelay = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::HorizontalSliderConfig.OnSetThumbMarginRight()' */
void WidgetSetHorizontalSliderConfig_OnSetThumbMarginRight( WidgetSetHorizontalSliderConfig _this, 
  XInt32 value )
{
  if ( _this->ThumbMarginRight == value )
    return;

  _this->ThumbMarginRight = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::HorizontalSliderConfig.OnSetThumbMarginLeft()' */
void WidgetSetHorizontalSliderConfig_OnSetThumbMarginLeft( WidgetSetHorizontalSliderConfig _this, 
  XInt32 value )
{
  if ( _this->ThumbMarginLeft == value )
    return;

  _this->ThumbMarginLeft = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::HorizontalSliderConfig.OnSetThumbFrameActive()' */
void WidgetSetHorizontalSliderConfig_OnSetThumbFrameActive( WidgetSetHorizontalSliderConfig _this, 
  XInt32 value )
{
  if ( _this->ThumbFrameActive == value )
    return;

  _this->ThumbFrameActive = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::HorizontalSliderConfig.OnSetThumbFrameFocused()' */
void WidgetSetHorizontalSliderConfig_OnSetThumbFrameFocused( WidgetSetHorizontalSliderConfig _this, 
  XInt32 value )
{
  if ( _this->ThumbFrameFocused == value )
    return;

  _this->ThumbFrameFocused = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::HorizontalSliderConfig.OnSetThumbFrameDisabled()' */
void WidgetSetHorizontalSliderConfig_OnSetThumbFrameDisabled( WidgetSetHorizontalSliderConfig _this, 
  XInt32 value )
{
  if ( _this->ThumbFrameDisabled == value )
    return;

  _this->ThumbFrameDisabled = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::HorizontalSliderConfig.OnSetThumbFrameDefault()' */
void WidgetSetHorizontalSliderConfig_OnSetThumbFrameDefault( WidgetSetHorizontalSliderConfig _this, 
  XInt32 value )
{
  if ( _this->ThumbFrameDefault == value )
    return;

  _this->ThumbFrameDefault = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
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

/* 'C' function for method : 'WidgetSet::HorizontalSliderConfig.OnSetThumbDisabled()' */
void WidgetSetHorizontalSliderConfig_OnSetThumbDisabled( WidgetSetHorizontalSliderConfig _this, 
  ResourcesBitmap value )
{
  if ( _this->ThumbDisabled == value )
    return;

  _this->ThumbDisabled = value;
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

/* Initializer for the class 'WidgetSet::RadioButtonConfig' */
void WidgetSetRadioButtonConfig__Init( WidgetSetRadioButtonConfig _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  WidgetSetWidgetConfig__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( WidgetSetRadioButtonConfig );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( WidgetSetRadioButtonConfig );

  /* ... and initialize objects, variables, properties, etc. */
  _this->PressedFeedbackDuration = 50;
  _this->KeyCode = CoreKeyCodeEnter;
  _this->LabelOnColorActive = _Const0002;
  _this->LabelOnColorFocused = _Const0002;
  _this->LabelOnColorDisabled = _Const0002;
  _this->LabelOnColorDefault = _Const0002;
  _this->LabelOffColorActive = _Const0002;
  _this->LabelOffColorFocused = _Const0002;
  _this->LabelOffColorDisabled = _Const0002;
  _this->LabelOffColorDefault = _Const0002;
  _this->IconOnTintActive = _Const0001;
  _this->IconOnTintFocused = _Const0001;
  _this->IconOnTintDisabled = _Const0001;
  _this->IconOnTintDefault = _Const0001;
  _this->IconOffTintActive = _Const0001;
  _this->IconOffTintFocused = _Const0001;
  _this->IconOffTintDisabled = _Const0001;
  _this->IconOffTintDefault = _Const0001;
  _this->LabelAlignment = ViewsTextAlignmentAlignHorzCenter | ViewsTextAlignmentAlignVertCenter;
  _this->IconAlignment = ViewsImageAlignmentAlignHorzCenter | ViewsImageAlignmentAlignVertCenter;
  _this->FaceLayout = CoreLayoutAlignToBottom | CoreLayoutAlignToLeft | CoreLayoutAlignToRight 
  | CoreLayoutAlignToTop | CoreLayoutResizeHorz | CoreLayoutResizeVert;
  _this->FaceOnTintActive = _Const0001;
  _this->FaceOnTintFocused = _Const0001;
  _this->FaceOnTintDisabled = _Const0001;
  _this->FaceOnTintDefault = _Const0001;
  _this->FaceOnFrameActive = -1;
  _this->FaceOnFrameFocused = -1;
  _this->FaceOnFrameDisabled = -1;
  _this->FaceOnFrameDefault = -1;
  _this->FaceOffTintActive = _Const0001;
  _this->FaceOffTintFocused = _Const0001;
  _this->FaceOffTintDisabled = _Const0001;
  _this->FaceOffTintDefault = _Const0001;
  _this->FaceOffFrameActive = -1;
  _this->FaceOffFrameFocused = -1;
  _this->FaceOffFrameDisabled = -1;
  _this->FaceOffFrameDefault = -1;
}

/* Re-Initializer for the class 'WidgetSet::RadioButtonConfig' */
void WidgetSetRadioButtonConfig__ReInit( WidgetSetRadioButtonConfig _this )
{
  /* At first re-initialize the super class ... */
  WidgetSetWidgetConfig__ReInit( &_this->_Super );
}

/* Finalizer method for the class 'WidgetSet::RadioButtonConfig' */
void WidgetSetRadioButtonConfig__Done( WidgetSetRadioButtonConfig _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( WidgetSetWidgetConfig );

  /* Don't forget to deinitialize the super class ... */
  WidgetSetWidgetConfig__Done( &_this->_Super );
}

/* 'C' function for method : 'WidgetSet::RadioButtonConfig.OnSetLabelOnColorActive()' */
void WidgetSetRadioButtonConfig_OnSetLabelOnColorActive( WidgetSetRadioButtonConfig _this, 
  XColor value )
{
  if ( !EwCompColor( _this->LabelOnColorActive, value ))
    return;

  _this->LabelOnColorActive = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::RadioButtonConfig.OnSetLabelOnColorFocused()' */
void WidgetSetRadioButtonConfig_OnSetLabelOnColorFocused( WidgetSetRadioButtonConfig _this, 
  XColor value )
{
  if ( !EwCompColor( _this->LabelOnColorFocused, value ))
    return;

  _this->LabelOnColorFocused = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::RadioButtonConfig.OnSetLabelOnColorDisabled()' */
void WidgetSetRadioButtonConfig_OnSetLabelOnColorDisabled( WidgetSetRadioButtonConfig _this, 
  XColor value )
{
  if ( !EwCompColor( _this->LabelOnColorDisabled, value ))
    return;

  _this->LabelOnColorDisabled = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::RadioButtonConfig.OnSetLabelOnColorDefault()' */
void WidgetSetRadioButtonConfig_OnSetLabelOnColorDefault( WidgetSetRadioButtonConfig _this, 
  XColor value )
{
  if ( !EwCompColor( _this->LabelOnColorDefault, value ))
    return;

  _this->LabelOnColorDefault = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::RadioButtonConfig.OnSetLabelOffColorActive()' */
void WidgetSetRadioButtonConfig_OnSetLabelOffColorActive( WidgetSetRadioButtonConfig _this, 
  XColor value )
{
  if ( !EwCompColor( _this->LabelOffColorActive, value ))
    return;

  _this->LabelOffColorActive = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::RadioButtonConfig.OnSetLabelOffColorFocused()' */
void WidgetSetRadioButtonConfig_OnSetLabelOffColorFocused( WidgetSetRadioButtonConfig _this, 
  XColor value )
{
  if ( !EwCompColor( _this->LabelOffColorFocused, value ))
    return;

  _this->LabelOffColorFocused = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::RadioButtonConfig.OnSetLabelOffColorDisabled()' */
void WidgetSetRadioButtonConfig_OnSetLabelOffColorDisabled( WidgetSetRadioButtonConfig _this, 
  XColor value )
{
  if ( !EwCompColor( _this->LabelOffColorDisabled, value ))
    return;

  _this->LabelOffColorDisabled = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::RadioButtonConfig.OnSetLabelOffColorDefault()' */
void WidgetSetRadioButtonConfig_OnSetLabelOffColorDefault( WidgetSetRadioButtonConfig _this, 
  XColor value )
{
  if ( !EwCompColor( _this->LabelOffColorDefault, value ))
    return;

  _this->LabelOffColorDefault = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::RadioButtonConfig.OnSetLabelMarginLeft()' */
void WidgetSetRadioButtonConfig_OnSetLabelMarginLeft( WidgetSetRadioButtonConfig _this, 
  XInt32 value )
{
  if ( _this->LabelMarginLeft == value )
    return;

  _this->LabelMarginLeft = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::RadioButtonConfig.OnSetIconOnTintActive()' */
void WidgetSetRadioButtonConfig_OnSetIconOnTintActive( WidgetSetRadioButtonConfig _this, 
  XColor value )
{
  if ( !EwCompColor( _this->IconOnTintActive, value ))
    return;

  _this->IconOnTintActive = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::RadioButtonConfig.OnSetIconOnTintFocused()' */
void WidgetSetRadioButtonConfig_OnSetIconOnTintFocused( WidgetSetRadioButtonConfig _this, 
  XColor value )
{
  if ( !EwCompColor( _this->IconOnTintFocused, value ))
    return;

  _this->IconOnTintFocused = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::RadioButtonConfig.OnSetIconOnTintDisabled()' */
void WidgetSetRadioButtonConfig_OnSetIconOnTintDisabled( WidgetSetRadioButtonConfig _this, 
  XColor value )
{
  if ( !EwCompColor( _this->IconOnTintDisabled, value ))
    return;

  _this->IconOnTintDisabled = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::RadioButtonConfig.OnSetIconOnTintDefault()' */
void WidgetSetRadioButtonConfig_OnSetIconOnTintDefault( WidgetSetRadioButtonConfig _this, 
  XColor value )
{
  if ( !EwCompColor( _this->IconOnTintDefault, value ))
    return;

  _this->IconOnTintDefault = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::RadioButtonConfig.OnSetIconOffTintActive()' */
void WidgetSetRadioButtonConfig_OnSetIconOffTintActive( WidgetSetRadioButtonConfig _this, 
  XColor value )
{
  if ( !EwCompColor( _this->IconOffTintActive, value ))
    return;

  _this->IconOffTintActive = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::RadioButtonConfig.OnSetIconOffTintFocused()' */
void WidgetSetRadioButtonConfig_OnSetIconOffTintFocused( WidgetSetRadioButtonConfig _this, 
  XColor value )
{
  if ( !EwCompColor( _this->IconOffTintFocused, value ))
    return;

  _this->IconOffTintFocused = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::RadioButtonConfig.OnSetIconOffTintDisabled()' */
void WidgetSetRadioButtonConfig_OnSetIconOffTintDisabled( WidgetSetRadioButtonConfig _this, 
  XColor value )
{
  if ( !EwCompColor( _this->IconOffTintDisabled, value ))
    return;

  _this->IconOffTintDisabled = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::RadioButtonConfig.OnSetIconOffTintDefault()' */
void WidgetSetRadioButtonConfig_OnSetIconOffTintDefault( WidgetSetRadioButtonConfig _this, 
  XColor value )
{
  if ( !EwCompColor( _this->IconOffTintDefault, value ))
    return;

  _this->IconOffTintDefault = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::RadioButtonConfig.OnSetIconMarginTop()' */
void WidgetSetRadioButtonConfig_OnSetIconMarginTop( WidgetSetRadioButtonConfig _this, 
  XInt32 value )
{
  if ( _this->IconMarginTop == value )
    return;

  _this->IconMarginTop = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::RadioButtonConfig.OnSetLabelAlignment()' */
void WidgetSetRadioButtonConfig_OnSetLabelAlignment( WidgetSetRadioButtonConfig _this, 
  XSet value )
{
  if ( _this->LabelAlignment == value )
    return;

  _this->LabelAlignment = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::RadioButtonConfig.OnSetLabelOnFont()' */
void WidgetSetRadioButtonConfig_OnSetLabelOnFont( WidgetSetRadioButtonConfig _this, 
  ResourcesFont value )
{
  if ( _this->LabelOnFont == value )
    return;

  _this->LabelOnFont = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::RadioButtonConfig.OnSetLabelOffFont()' */
void WidgetSetRadioButtonConfig_OnSetLabelOffFont( WidgetSetRadioButtonConfig _this, 
  ResourcesFont value )
{
  if ( _this->LabelOffFont == value )
    return;

  _this->LabelOffFont = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::RadioButtonConfig.OnSetIconAlignment()' */
void WidgetSetRadioButtonConfig_OnSetIconAlignment( WidgetSetRadioButtonConfig _this, 
  XSet value )
{
  if ( _this->IconAlignment == value )
    return;

  _this->IconAlignment = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::RadioButtonConfig.OnSetFaceLayout()' */
void WidgetSetRadioButtonConfig_OnSetFaceLayout( WidgetSetRadioButtonConfig _this, 
  XSet value )
{
  if ( _this->FaceLayout == value )
    return;

  _this->FaceLayout = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::RadioButtonConfig.OnSetFaceOnFrameActive()' */
void WidgetSetRadioButtonConfig_OnSetFaceOnFrameActive( WidgetSetRadioButtonConfig _this, 
  XInt32 value )
{
  if ( _this->FaceOnFrameActive == value )
    return;

  _this->FaceOnFrameActive = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::RadioButtonConfig.OnSetFaceOnFrameFocused()' */
void WidgetSetRadioButtonConfig_OnSetFaceOnFrameFocused( WidgetSetRadioButtonConfig _this, 
  XInt32 value )
{
  if ( _this->FaceOnFrameFocused == value )
    return;

  _this->FaceOnFrameFocused = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::RadioButtonConfig.OnSetFaceOnFrameDisabled()' */
void WidgetSetRadioButtonConfig_OnSetFaceOnFrameDisabled( WidgetSetRadioButtonConfig _this, 
  XInt32 value )
{
  if ( _this->FaceOnFrameDisabled == value )
    return;

  _this->FaceOnFrameDisabled = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::RadioButtonConfig.OnSetFaceOnFrameDefault()' */
void WidgetSetRadioButtonConfig_OnSetFaceOnFrameDefault( WidgetSetRadioButtonConfig _this, 
  XInt32 value )
{
  if ( _this->FaceOnFrameDefault == value )
    return;

  _this->FaceOnFrameDefault = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::RadioButtonConfig.OnSetFaceOnActive()' */
void WidgetSetRadioButtonConfig_OnSetFaceOnActive( WidgetSetRadioButtonConfig _this, 
  ResourcesBitmap value )
{
  if ( _this->FaceOnActive == value )
    return;

  _this->FaceOnActive = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::RadioButtonConfig.OnSetFaceOnFocused()' */
void WidgetSetRadioButtonConfig_OnSetFaceOnFocused( WidgetSetRadioButtonConfig _this, 
  ResourcesBitmap value )
{
  if ( _this->FaceOnFocused == value )
    return;

  _this->FaceOnFocused = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::RadioButtonConfig.OnSetFaceOnDisabled()' */
void WidgetSetRadioButtonConfig_OnSetFaceOnDisabled( WidgetSetRadioButtonConfig _this, 
  ResourcesBitmap value )
{
  if ( _this->FaceOnDisabled == value )
    return;

  _this->FaceOnDisabled = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::RadioButtonConfig.OnSetFaceOnDefault()' */
void WidgetSetRadioButtonConfig_OnSetFaceOnDefault( WidgetSetRadioButtonConfig _this, 
  ResourcesBitmap value )
{
  if ( _this->FaceOnDefault == value )
    return;

  _this->FaceOnDefault = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::RadioButtonConfig.OnSetFaceOffFrameActive()' */
void WidgetSetRadioButtonConfig_OnSetFaceOffFrameActive( WidgetSetRadioButtonConfig _this, 
  XInt32 value )
{
  if ( _this->FaceOffFrameActive == value )
    return;

  _this->FaceOffFrameActive = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::RadioButtonConfig.OnSetFaceOffFrameFocused()' */
void WidgetSetRadioButtonConfig_OnSetFaceOffFrameFocused( WidgetSetRadioButtonConfig _this, 
  XInt32 value )
{
  if ( _this->FaceOffFrameFocused == value )
    return;

  _this->FaceOffFrameFocused = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::RadioButtonConfig.OnSetFaceOffFrameDisabled()' */
void WidgetSetRadioButtonConfig_OnSetFaceOffFrameDisabled( WidgetSetRadioButtonConfig _this, 
  XInt32 value )
{
  if ( _this->FaceOffFrameDisabled == value )
    return;

  _this->FaceOffFrameDisabled = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::RadioButtonConfig.OnSetFaceOffFrameDefault()' */
void WidgetSetRadioButtonConfig_OnSetFaceOffFrameDefault( WidgetSetRadioButtonConfig _this, 
  XInt32 value )
{
  if ( _this->FaceOffFrameDefault == value )
    return;

  _this->FaceOffFrameDefault = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::RadioButtonConfig.OnSetFaceOffActive()' */
void WidgetSetRadioButtonConfig_OnSetFaceOffActive( WidgetSetRadioButtonConfig _this, 
  ResourcesBitmap value )
{
  if ( _this->FaceOffActive == value )
    return;

  _this->FaceOffActive = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::RadioButtonConfig.OnSetFaceOffFocused()' */
void WidgetSetRadioButtonConfig_OnSetFaceOffFocused( WidgetSetRadioButtonConfig _this, 
  ResourcesBitmap value )
{
  if ( _this->FaceOffFocused == value )
    return;

  _this->FaceOffFocused = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::RadioButtonConfig.OnSetFaceOffDisabled()' */
void WidgetSetRadioButtonConfig_OnSetFaceOffDisabled( WidgetSetRadioButtonConfig _this, 
  ResourcesBitmap value )
{
  if ( _this->FaceOffDisabled == value )
    return;

  _this->FaceOffDisabled = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::RadioButtonConfig.OnSetFaceOffDefault()' */
void WidgetSetRadioButtonConfig_OnSetFaceOffDefault( WidgetSetRadioButtonConfig _this, 
  ResourcesBitmap value )
{
  if ( _this->FaceOffDefault == value )
    return;

  _this->FaceOffDefault = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::RadioButtonConfig.OnSetWidgetMinSize()' */
void WidgetSetRadioButtonConfig_OnSetWidgetMinSize( WidgetSetRadioButtonConfig _this, 
  XPoint value )
{
  if ( !EwCompPoint( _this->WidgetMinSize, value ))
    return;

  _this->WidgetMinSize = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* Variants derived from the class : 'WidgetSet::RadioButtonConfig' */
EW_DEFINE_CLASS_VARIANTS( WidgetSetRadioButtonConfig )
EW_END_OF_CLASS_VARIANTS( WidgetSetRadioButtonConfig )

/* Virtual Method Table (VMT) for the class : 'WidgetSet::RadioButtonConfig' */
EW_DEFINE_CLASS( WidgetSetRadioButtonConfig, WidgetSetWidgetConfig, LabelOnFont, 
                 PressedFeedbackDuration, PressedFeedbackDuration, PressedFeedbackDuration, 
                 PressedFeedbackDuration, PressedFeedbackDuration, "WidgetSet::RadioButtonConfig" )
EW_END_OF_CLASS( WidgetSetRadioButtonConfig )

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
  CoreRectView__OnSetBounds( _this, _Const0003 );
  CoreTimer_OnSetPeriod( &_this->RepetitionTimer, 0 );
  CoreTimer_OnSetBegin( &_this->RepetitionTimer, 50 );
  _this->KeyHandlerLeft.Filter = CoreKeyCodeLeft;
  _this->KeyHandlerRight.Filter = CoreKeyCodeRight;
  CoreView_OnSetLayout((CoreView)&_this->TouchHandler, CoreLayoutAlignToBottom | 
  CoreLayoutAlignToLeft | CoreLayoutAlignToRight | CoreLayoutAlignToTop | CoreLayoutResizeHorz 
  | CoreLayoutResizeVert );
  CoreQuadView_OnSetPoint4((CoreQuadView)&_this->TouchHandler, _Const0004 );
  CoreQuadView_OnSetPoint3((CoreQuadView)&_this->TouchHandler, _Const0005 );
  CoreQuadView_OnSetPoint2((CoreQuadView)&_this->TouchHandler, _Const0006 );
  CoreQuadView_OnSetPoint1((CoreQuadView)&_this->TouchHandler, _Const0007 );
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
  thumbSize = _Const0007;

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
    XColor tint = _Const0001;

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
    XColor tint = _Const0001;
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
    XColor tint = _Const0001;
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
    XColor tint = _Const0001;

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
    XColor tint = _Const0001;

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
    XRect thumbArea = EwNewRect2Point( _Const0007, thumbSize );
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
  CoreGroup_DispatchEvent,
  CoreGroup_BroadcastEvent,
  WidgetSetHorizontalSlider_UpdateViewState,
  CoreGroup_InvalidateArea,
EW_END_OF_CLASS( WidgetSetHorizontalSlider )

/* Initializer for the class 'WidgetSet::RadioButton' */
void WidgetSetRadioButton__Init( WidgetSetRadioButton _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( WidgetSetRadioButton );

  /* ... then construct all embedded objects */
  CoreTimer__Init( &_this->FlashTimer, &_this->_XObject, 0 );
  CoreKeyPressHandler__Init( &_this->KeyHandler, &_this->_XObject, 0 );
  CoreSimpleTouchHandler__Init( &_this->TouchHandler, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( WidgetSetRadioButton );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const0008 );
  CoreTimer_OnSetPeriod( &_this->FlashTimer, 0 );
  CoreTimer_OnSetBegin( &_this->FlashTimer, 50 );
  _this->KeyHandler.Filter = CoreKeyCodeEnter;
  CoreView_OnSetLayout((CoreView)&_this->TouchHandler, CoreLayoutAlignToBottom | 
  CoreLayoutAlignToLeft | CoreLayoutAlignToRight | CoreLayoutAlignToTop | CoreLayoutResizeHorz 
  | CoreLayoutResizeVert );
  CoreQuadView_OnSetPoint4((CoreQuadView)&_this->TouchHandler, _Const0004 );
  CoreQuadView_OnSetPoint3((CoreQuadView)&_this->TouchHandler, _Const0009 );
  CoreQuadView_OnSetPoint2((CoreQuadView)&_this->TouchHandler, _Const000A );
  CoreQuadView_OnSetPoint1((CoreQuadView)&_this->TouchHandler, _Const0007 );
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
  _this->FlashTimer.OnTrigger = EwNewSlot( _this, WidgetSetRadioButton_onFlashTimer );
  _this->KeyHandler.OnRelease = EwNewSlot( _this, WidgetSetRadioButton_onReleaseKey );
  _this->KeyHandler.OnPress = EwNewSlot( _this, WidgetSetRadioButton_onPressKey );
  _this->TouchHandler.OnLeave = EwNewSlot( _this, WidgetSetRadioButton_onLeaveTouch );
  _this->TouchHandler.OnEnter = EwNewSlot( _this, WidgetSetRadioButton_onEnterTouch );
  _this->TouchHandler.OnRelease = EwNewSlot( _this, WidgetSetRadioButton_onReleaseTouch );
  _this->TouchHandler.OnPress = EwNewSlot( _this, WidgetSetRadioButton_onPressTouch );
}

/* Re-Initializer for the class 'WidgetSet::RadioButton' */
void WidgetSetRadioButton__ReInit( WidgetSetRadioButton _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  CoreTimer__ReInit( &_this->FlashTimer );
  CoreKeyPressHandler__ReInit( &_this->KeyHandler );
  CoreSimpleTouchHandler__ReInit( &_this->TouchHandler );
}

/* Finalizer method for the class 'WidgetSet::RadioButton' */
void WidgetSetRadioButton__Done( WidgetSetRadioButton _this )
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

/* 'C' function for method : 'WidgetSet::RadioButton.OnSetBounds()' */
void WidgetSetRadioButton_OnSetBounds( WidgetSetRadioButton _this, XRect value )
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
void WidgetSetRadioButton_UpdateViewState( WidgetSetRadioButton _this, XSet aState )
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
  isChecked = _this->Selected;

  if ( _this->frameView != 0 )
  {
    ResourcesBitmap bitmap = 0;
    XInt32 frameNo = -1;
    XColor tint = _Const0001;
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
    XColor clr = _Const0001;

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
      ViewsImage_OnSetColor( _this->imageView, _Const0001 );
    }

  if ( _this->textView != 0 )
  {
    XColor clr = _Const0001;
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

/* 'C' function for method : 'WidgetSet::RadioButton.onConfigChanged()' */
void WidgetSetRadioButton_onConfigChanged( WidgetSetRadioButton _this, XObject sender )
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

/* 'C' function for method : 'WidgetSet::RadioButton.onFlashTimer()' */
void WidgetSetRadioButton_onFlashTimer( WidgetSetRadioButton _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  CoreGroup_InvalidateViewState((CoreGroup)_this );

  if ( !_this->Selected )
  {
    WidgetSetRadioButton_OnSetSelected( _this, 1 );
    EwPostSignal( _this->OnSelect, ((XObject)_this ));

    if ( _this->Outlet.Object != 0 )
    {
      EwOnSetInt32( _this->Outlet, _this->OutletSelector );
      EwNotifyRefObservers( _this->Outlet, 0 );
    }
  }
}

/* 'C' function for method : 'WidgetSet::RadioButton.onReleaseKey()' */
void WidgetSetRadioButton_onReleaseKey( WidgetSetRadioButton _this, XObject sender )
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
    if ( !_this->Selected )
    {
      WidgetSetRadioButton_OnSetSelected( _this, 1 );
      EwPostSignal( _this->OnSelect, ((XObject)_this ));

      if ( _this->Outlet.Object != 0 )
      {
        EwOnSetInt32( _this->Outlet, _this->OutletSelector );
        EwNotifyRefObservers( _this->Outlet, 0 );
      }
    }
  }
  else
  {
    CoreTimer_OnSetBegin( &_this->FlashTimer, pressFeedbackDuration - (XInt32)( 
    _this->KeyHandler.Time - _this->onPressKeyTime ));
    CoreTimer_OnSetEnabled( &_this->FlashTimer, 1 );
  }
}

/* 'C' function for method : 'WidgetSet::RadioButton.onPressKey()' */
void WidgetSetRadioButton_onPressKey( WidgetSetRadioButton _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  CoreSimpleTouchHandler_OnSetEnabled( &_this->TouchHandler, 0 );
  CoreGroup_InvalidateViewState((CoreGroup)_this );

  if ( _this->FlashTimer.Enabled )
  {
    CoreTimer_OnSetEnabled( &_this->FlashTimer, 0 );

    if ( !_this->Selected )
    {
      WidgetSetRadioButton_OnSetSelected( _this, 1 );
      EwPostSignal( _this->OnSelect, ((XObject)_this ));

      if ( _this->Outlet.Object != 0 )
      {
        EwOnSetInt32( _this->Outlet, _this->OutletSelector );
        EwNotifyRefObservers( _this->Outlet, 0 );
      }
    }
  }

  _this->onPressKeyTime = _this->KeyHandler.Time;
  EwPostSignal( _this->OnPress, ((XObject)_this ));
  EwPostSignal( _this->OnEnter, ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::RadioButton.onLeaveTouch()' */
void WidgetSetRadioButton_onLeaveTouch( WidgetSetRadioButton _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  CoreGroup_InvalidateViewState((CoreGroup)_this );
  EwPostSignal( _this->OnLeave, ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::RadioButton.onEnterTouch()' */
void WidgetSetRadioButton_onEnterTouch( WidgetSetRadioButton _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  CoreGroup_InvalidateViewState((CoreGroup)_this );
  EwPostSignal( _this->OnEnter, ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::RadioButton.onReleaseTouch()' */
void WidgetSetRadioButton_onReleaseTouch( WidgetSetRadioButton _this, XObject sender )
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
    if ( !_this->Selected )
    {
      WidgetSetRadioButton_OnSetSelected( _this, 1 );
      EwPostSignal( _this->OnSelect, ((XObject)_this ));

      if ( _this->Outlet.Object != 0 )
      {
        EwOnSetInt32( _this->Outlet, _this->OutletSelector );
        EwNotifyRefObservers( _this->Outlet, 0 );
      }
    }
  }
  else
  {
    CoreTimer_OnSetBegin( &_this->FlashTimer, pressFeedbackDuration - _this->TouchHandler.HoldPeriod );
    CoreTimer_OnSetEnabled( &_this->FlashTimer, 1 );
  }
}

/* 'C' function for method : 'WidgetSet::RadioButton.onPressTouch()' */
void WidgetSetRadioButton_onPressTouch( WidgetSetRadioButton _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  _this->KeyHandler.Enabled = 0;

  if ( _this->FlashTimer.Enabled )
  {
    CoreTimer_OnSetEnabled( &_this->FlashTimer, 0 );

    if ( !_this->Selected )
    {
      WidgetSetRadioButton_OnSetSelected( _this, 1 );
      EwPostSignal( _this->OnSelect, ((XObject)_this ));

      if ( _this->Outlet.Object != 0 )
      {
        EwOnSetInt32( _this->Outlet, _this->OutletSelector );
        EwNotifyRefObservers( _this->Outlet, 0 );
      }
    }
  }

  EwPostSignal( _this->OnPress, ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::RadioButton.OnSetSelected()' */
void WidgetSetRadioButton_OnSetSelected( WidgetSetRadioButton _this, XBool value )
{
  if ( _this->Selected == value )
    return;

  _this->Selected = value;
  CoreGroup_InvalidateViewState((CoreGroup)_this );
}

/* 'C' function for method : 'WidgetSet::RadioButton.OnSetIconOnFrameActive()' */
void WidgetSetRadioButton_OnSetIconOnFrameActive( WidgetSetRadioButton _this, XInt32 
  value )
{
  if ( _this->IconOnFrameActive == value )
    return;

  _this->IconOnFrameActive = value;
  CoreGroup_InvalidateViewState((CoreGroup)_this );
}

/* 'C' function for method : 'WidgetSet::RadioButton.OnSetIconOnFrameFocused()' */
void WidgetSetRadioButton_OnSetIconOnFrameFocused( WidgetSetRadioButton _this, XInt32 
  value )
{
  if ( _this->IconOnFrameFocused == value )
    return;

  _this->IconOnFrameFocused = value;
  CoreGroup_InvalidateViewState((CoreGroup)_this );
}

/* 'C' function for method : 'WidgetSet::RadioButton.OnSetIconOnFrameDisabled()' */
void WidgetSetRadioButton_OnSetIconOnFrameDisabled( WidgetSetRadioButton _this, 
  XInt32 value )
{
  if ( _this->IconOnFrameDisabled == value )
    return;

  _this->IconOnFrameDisabled = value;
  CoreGroup_InvalidateViewState((CoreGroup)_this );
}

/* 'C' function for method : 'WidgetSet::RadioButton.OnSetIconOnFrameDefault()' */
void WidgetSetRadioButton_OnSetIconOnFrameDefault( WidgetSetRadioButton _this, XInt32 
  value )
{
  if ( _this->IconOnFrameDefault == value )
    return;

  _this->IconOnFrameDefault = value;
  CoreGroup_InvalidateViewState((CoreGroup)_this );
}

/* 'C' function for method : 'WidgetSet::RadioButton.OnSetIconOffFrameActive()' */
void WidgetSetRadioButton_OnSetIconOffFrameActive( WidgetSetRadioButton _this, XInt32 
  value )
{
  if ( _this->IconOffFrameActive == value )
    return;

  _this->IconOffFrameActive = value;
  CoreGroup_InvalidateViewState((CoreGroup)_this );
}

/* 'C' function for method : 'WidgetSet::RadioButton.OnSetIconOffFrameFocused()' */
void WidgetSetRadioButton_OnSetIconOffFrameFocused( WidgetSetRadioButton _this, 
  XInt32 value )
{
  if ( _this->IconOffFrameFocused == value )
    return;

  _this->IconOffFrameFocused = value;
  CoreGroup_InvalidateViewState((CoreGroup)_this );
}

/* 'C' function for method : 'WidgetSet::RadioButton.OnSetIconOffFrameDisabled()' */
void WidgetSetRadioButton_OnSetIconOffFrameDisabled( WidgetSetRadioButton _this, 
  XInt32 value )
{
  if ( _this->IconOffFrameDisabled == value )
    return;

  _this->IconOffFrameDisabled = value;
  CoreGroup_InvalidateViewState((CoreGroup)_this );
}

/* 'C' function for method : 'WidgetSet::RadioButton.OnSetIconOffFrameDefault()' */
void WidgetSetRadioButton_OnSetIconOffFrameDefault( WidgetSetRadioButton _this, 
  XInt32 value )
{
  if ( _this->IconOffFrameDefault == value )
    return;

  _this->IconOffFrameDefault = value;
  CoreGroup_InvalidateViewState((CoreGroup)_this );
}

/* 'C' function for method : 'WidgetSet::RadioButton.OnSetIcon()' */
void WidgetSetRadioButton_OnSetIcon( WidgetSetRadioButton _this, ResourcesBitmap 
  value )
{
  if ( _this->Icon == value )
    return;

  _this->Icon = value;
  CoreGroup_InvalidateViewState((CoreGroup)_this );
}

/* 'C' function for method : 'WidgetSet::RadioButton.OnSetAppearance()' */
void WidgetSetRadioButton_OnSetAppearance( WidgetSetRadioButton _this, WidgetSetRadioButtonConfig 
  value )
{
  if ( _this->Appearance == value )
    return;

  if ( _this->Appearance != 0 )
    EwDetachObjObserver( EwNewSlot( _this, WidgetSetRadioButton_onConfigChanged ), 
      (XObject)_this->Appearance, 0 );

  _this->Appearance = value;

  if ( value != 0 )
    EwAttachObjObserver( EwNewSlot( _this, WidgetSetRadioButton_onConfigChanged ), 
      (XObject)value, 0 );

  EwPostSignal( EwNewSlot( _this, WidgetSetRadioButton_onConfigChanged ), ((XObject)_this ));
}

/* Variants derived from the class : 'WidgetSet::RadioButton' */
EW_DEFINE_CLASS_VARIANTS( WidgetSetRadioButton )
EW_END_OF_CLASS_VARIANTS( WidgetSetRadioButton )

/* Virtual Method Table (VMT) for the class : 'WidgetSet::RadioButton' */
EW_DEFINE_CLASS( WidgetSetRadioButton, CoreGroup, textView, OnLeave, Outlet, FlashTimer, 
                 LabelOn, onPressKeyTime, "WidgetSet::RadioButton" )
  CoreRectView_initLayoutContext,
  CoreView_GetRoot,
  CoreGroup_Draw,
  CoreView_HandleEvent,
  CoreGroup_CursorHitTest,
  CoreRectView_ArrangeView,
  CoreRectView_MoveView,
  CoreRectView_GetExtent,
  CoreGroup_ChangeViewState,
  WidgetSetRadioButton_OnSetBounds,
  CoreGroup_OnSetFocus,
  CoreGroup_DispatchEvent,
  CoreGroup_BroadcastEvent,
  WidgetSetRadioButton_UpdateViewState,
  CoreGroup_InvalidateArea,
EW_END_OF_CLASS( WidgetSetRadioButton )

/* Embedded Wizard */
