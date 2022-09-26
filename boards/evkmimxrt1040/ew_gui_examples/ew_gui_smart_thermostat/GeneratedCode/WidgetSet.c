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
#include "_WidgetSetRadioButton.h"
#include "_WidgetSetRadioButtonConfig.h"
#include "_WidgetSetToggleButton.h"
#include "_WidgetSetToggleButtonConfig.h"
#include "_WidgetSetVerticalSlider.h"
#include "_WidgetSetVerticalSliderConfig.h"
#include "_WidgetSetWidgetConfig.h"
#include "Core.h"
#include "Graphics.h"
#include "Views.h"
#include "WidgetSet.h"

/* Constant values used in this 'C' module only. */
static const XColor _Const0000 = { 0xFF, 0xFF, 0xFF, 0xFF };
static const XColor _Const0001 = { 0x00, 0x00, 0x00, 0xFF };
static const XRect _Const0002 = {{ 0, 0 }, { 50, 200 }};
static const XPoint _Const0003 = { 0, 200 };
static const XPoint _Const0004 = { 50, 200 };
static const XPoint _Const0005 = { 50, 0 };
static const XPoint _Const0006 = { 0, 0 };
static const XRect _Const0007 = {{ 0, 0 }, { 150, 50 }};
static const XPoint _Const0008 = { 0, 50 };
static const XPoint _Const0009 = { 150, 50 };
static const XPoint _Const000A = { 150, 0 };

/* Forward declaration of the class Resources::Font */
#ifndef _ResourcesFont_
  EW_DECLARE_CLASS( ResourcesFont )
#define _ResourcesFont_
#endif

/* Initializer for the class 'WidgetSet::VerticalSliderConfig' */
void WidgetSetVerticalSliderConfig__Init( WidgetSetVerticalSliderConfig _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  WidgetSetWidgetConfig__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( WidgetSetVerticalSliderConfig );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( WidgetSetVerticalSliderConfig );

  /* ... and initialize objects, variables, properties, etc. */
  _this->KeyCodeUp = CoreKeyCodeUp;
  _this->KeyCodeDown = CoreKeyCodeDown;
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
  _this->TrackAboveTintActive = _Const0000;
  _this->TrackAboveTintFocused = _Const0000;
  _this->TrackAboveTintDisabled = _Const0000;
  _this->TrackAboveTintDefault = _Const0000;
  _this->TrackAboveFrameActive = -1;
  _this->TrackAboveFrameFocused = -1;
  _this->TrackAboveFrameDisabled = -1;
  _this->TrackAboveFrameDefault = -1;
  _this->TrackBelowTintActive = _Const0000;
  _this->TrackBelowTintFocused = _Const0000;
  _this->TrackBelowTintDisabled = _Const0000;
  _this->TrackBelowTintDefault = _Const0000;
  _this->TrackBelowFrameActive = -1;
  _this->TrackBelowFrameFocused = -1;
  _this->TrackBelowFrameDisabled = -1;
  _this->TrackBelowFrameDefault = -1;
  _this->FaceTintActive = _Const0000;
  _this->FaceTintFocused = _Const0000;
  _this->FaceTintDisabled = _Const0000;
  _this->FaceTintDefault = _Const0000;
  _this->FaceFrameActive = -1;
  _this->FaceFrameFocused = -1;
  _this->FaceFrameDisabled = -1;
  _this->FaceFrameDefault = -1;
}

/* Re-Initializer for the class 'WidgetSet::VerticalSliderConfig' */
void WidgetSetVerticalSliderConfig__ReInit( WidgetSetVerticalSliderConfig _this )
{
  /* At first re-initialize the super class ... */
  WidgetSetWidgetConfig__ReInit( &_this->_Super );
}

/* Finalizer method for the class 'WidgetSet::VerticalSliderConfig' */
void WidgetSetVerticalSliderConfig__Done( WidgetSetVerticalSliderConfig _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( WidgetSetWidgetConfig );

  /* Don't forget to deinitialize the super class ... */
  WidgetSetWidgetConfig__Done( &_this->_Super );
}

/* 'C' function for method : 'WidgetSet::VerticalSliderConfig.OnSetTrackAboveFrameActive()' */
void WidgetSetVerticalSliderConfig_OnSetTrackAboveFrameActive( WidgetSetVerticalSliderConfig _this, 
  XInt32 value )
{
  if ( _this->TrackAboveFrameActive == value )
    return;

  _this->TrackAboveFrameActive = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::VerticalSliderConfig.OnSetTrackAboveFrameFocused()' */
void WidgetSetVerticalSliderConfig_OnSetTrackAboveFrameFocused( WidgetSetVerticalSliderConfig _this, 
  XInt32 value )
{
  if ( _this->TrackAboveFrameFocused == value )
    return;

  _this->TrackAboveFrameFocused = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::VerticalSliderConfig.OnSetTrackAboveFrameDisabled()' */
void WidgetSetVerticalSliderConfig_OnSetTrackAboveFrameDisabled( WidgetSetVerticalSliderConfig _this, 
  XInt32 value )
{
  if ( _this->TrackAboveFrameDisabled == value )
    return;

  _this->TrackAboveFrameDisabled = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::VerticalSliderConfig.OnSetTrackAboveFrameDefault()' */
void WidgetSetVerticalSliderConfig_OnSetTrackAboveFrameDefault( WidgetSetVerticalSliderConfig _this, 
  XInt32 value )
{
  if ( _this->TrackAboveFrameDefault == value )
    return;

  _this->TrackAboveFrameDefault = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::VerticalSliderConfig.OnSetTrackAboveActive()' */
void WidgetSetVerticalSliderConfig_OnSetTrackAboveActive( WidgetSetVerticalSliderConfig _this, 
  ResourcesBitmap value )
{
  if ( _this->TrackAboveActive == value )
    return;

  _this->TrackAboveActive = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::VerticalSliderConfig.OnSetTrackAboveFocused()' */
void WidgetSetVerticalSliderConfig_OnSetTrackAboveFocused( WidgetSetVerticalSliderConfig _this, 
  ResourcesBitmap value )
{
  if ( _this->TrackAboveFocused == value )
    return;

  _this->TrackAboveFocused = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::VerticalSliderConfig.OnSetTrackAboveDisabled()' */
void WidgetSetVerticalSliderConfig_OnSetTrackAboveDisabled( WidgetSetVerticalSliderConfig _this, 
  ResourcesBitmap value )
{
  if ( _this->TrackAboveDisabled == value )
    return;

  _this->TrackAboveDisabled = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::VerticalSliderConfig.OnSetTrackAboveDefault()' */
void WidgetSetVerticalSliderConfig_OnSetTrackAboveDefault( WidgetSetVerticalSliderConfig _this, 
  ResourcesBitmap value )
{
  if ( _this->TrackAboveDefault == value )
    return;

  _this->TrackAboveDefault = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::VerticalSliderConfig.OnSetTrackBelowFrameActive()' */
void WidgetSetVerticalSliderConfig_OnSetTrackBelowFrameActive( WidgetSetVerticalSliderConfig _this, 
  XInt32 value )
{
  if ( _this->TrackBelowFrameActive == value )
    return;

  _this->TrackBelowFrameActive = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::VerticalSliderConfig.OnSetTrackBelowFrameFocused()' */
void WidgetSetVerticalSliderConfig_OnSetTrackBelowFrameFocused( WidgetSetVerticalSliderConfig _this, 
  XInt32 value )
{
  if ( _this->TrackBelowFrameFocused == value )
    return;

  _this->TrackBelowFrameFocused = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::VerticalSliderConfig.OnSetTrackBelowFrameDisabled()' */
void WidgetSetVerticalSliderConfig_OnSetTrackBelowFrameDisabled( WidgetSetVerticalSliderConfig _this, 
  XInt32 value )
{
  if ( _this->TrackBelowFrameDisabled == value )
    return;

  _this->TrackBelowFrameDisabled = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::VerticalSliderConfig.OnSetTrackBelowFrameDefault()' */
void WidgetSetVerticalSliderConfig_OnSetTrackBelowFrameDefault( WidgetSetVerticalSliderConfig _this, 
  XInt32 value )
{
  if ( _this->TrackBelowFrameDefault == value )
    return;

  _this->TrackBelowFrameDefault = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::VerticalSliderConfig.OnSetTrackBelowActive()' */
void WidgetSetVerticalSliderConfig_OnSetTrackBelowActive( WidgetSetVerticalSliderConfig _this, 
  ResourcesBitmap value )
{
  if ( _this->TrackBelowActive == value )
    return;

  _this->TrackBelowActive = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::VerticalSliderConfig.OnSetTrackBelowFocused()' */
void WidgetSetVerticalSliderConfig_OnSetTrackBelowFocused( WidgetSetVerticalSliderConfig _this, 
  ResourcesBitmap value )
{
  if ( _this->TrackBelowFocused == value )
    return;

  _this->TrackBelowFocused = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::VerticalSliderConfig.OnSetTrackBelowDisabled()' */
void WidgetSetVerticalSliderConfig_OnSetTrackBelowDisabled( WidgetSetVerticalSliderConfig _this, 
  ResourcesBitmap value )
{
  if ( _this->TrackBelowDisabled == value )
    return;

  _this->TrackBelowDisabled = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::VerticalSliderConfig.OnSetTrackBelowDefault()' */
void WidgetSetVerticalSliderConfig_OnSetTrackBelowDefault( WidgetSetVerticalSliderConfig _this, 
  ResourcesBitmap value )
{
  if ( _this->TrackBelowDefault == value )
    return;

  _this->TrackBelowDefault = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::VerticalSliderConfig.OnSetWidgetMaxSize()' */
void WidgetSetVerticalSliderConfig_OnSetWidgetMaxSize( WidgetSetVerticalSliderConfig _this, 
  XPoint value )
{
  if ( !EwCompPoint( _this->WidgetMaxSize, value ))
    return;

  _this->WidgetMaxSize = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::VerticalSliderConfig.OnSetWidgetMinSize()' */
void WidgetSetVerticalSliderConfig_OnSetWidgetMinSize( WidgetSetVerticalSliderConfig _this, 
  XPoint value )
{
  if ( !EwCompPoint( _this->WidgetMinSize, value ))
    return;

  _this->WidgetMinSize = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* Variants derived from the class : 'WidgetSet::VerticalSliderConfig' */
EW_DEFINE_CLASS_VARIANTS( WidgetSetVerticalSliderConfig )
EW_END_OF_CLASS_VARIANTS( WidgetSetVerticalSliderConfig )

/* Virtual Method Table (VMT) for the class : 'WidgetSet::VerticalSliderConfig' */
EW_DEFINE_CLASS( WidgetSetVerticalSliderConfig, WidgetSetWidgetConfig, CoverActive, 
                 KeyRepeatPeriod, KeyRepeatPeriod, KeyRepeatPeriod, KeyRepeatPeriod, 
                 KeyRepeatPeriod, "WidgetSet::VerticalSliderConfig" )
EW_END_OF_CLASS( WidgetSetVerticalSliderConfig )

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

/* 'C' function for method : 'WidgetSet::RadioButtonConfig.OnSetWidgetMaxSize()' */
void WidgetSetRadioButtonConfig_OnSetWidgetMaxSize( WidgetSetRadioButtonConfig _this, 
  XPoint value )
{
  if ( !EwCompPoint( _this->WidgetMaxSize, value ))
    return;

  _this->WidgetMaxSize = value;
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

/* 'C' function for method : 'WidgetSet::ToggleButtonConfig.OnSetFaceOnFrameDisabled()' */
void WidgetSetToggleButtonConfig_OnSetFaceOnFrameDisabled( WidgetSetToggleButtonConfig _this, 
  XInt32 value )
{
  if ( _this->FaceOnFrameDisabled == value )
    return;

  _this->FaceOnFrameDisabled = value;
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

/* 'C' function for method : 'WidgetSet::ToggleButtonConfig.OnSetFaceOnDisabled()' */
void WidgetSetToggleButtonConfig_OnSetFaceOnDisabled( WidgetSetToggleButtonConfig _this, 
  ResourcesBitmap value )
{
  if ( _this->FaceOnDisabled == value )
    return;

  _this->FaceOnDisabled = value;
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

/* 'C' function for method : 'WidgetSet::ToggleButtonConfig.OnSetFaceOffFrameDisabled()' */
void WidgetSetToggleButtonConfig_OnSetFaceOffFrameDisabled( WidgetSetToggleButtonConfig _this, 
  XInt32 value )
{
  if ( _this->FaceOffFrameDisabled == value )
    return;

  _this->FaceOffFrameDisabled = value;
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

/* 'C' function for method : 'WidgetSet::ToggleButtonConfig.OnSetFaceOffDisabled()' */
void WidgetSetToggleButtonConfig_OnSetFaceOffDisabled( WidgetSetToggleButtonConfig _this, 
  ResourcesBitmap value )
{
  if ( _this->FaceOffDisabled == value )
    return;

  _this->FaceOffDisabled = value;
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

/* Initializer for the class 'WidgetSet::VerticalSlider' */
void WidgetSetVerticalSlider__Init( WidgetSetVerticalSlider _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( WidgetSetVerticalSlider );

  /* ... then construct all embedded objects */
  CoreTimer__Init( &_this->RepetitionTimer, &_this->_XObject, 0 );
  CoreKeyPressHandler__Init( &_this->KeyHandlerDown, &_this->_XObject, 0 );
  CoreKeyPressHandler__Init( &_this->KeyHandlerUp, &_this->_XObject, 0 );
  CoreSimpleTouchHandler__Init( &_this->TouchHandler, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( WidgetSetVerticalSlider );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const0002 );
  CoreTimer_OnSetPeriod( &_this->RepetitionTimer, 0 );
  CoreTimer_OnSetBegin( &_this->RepetitionTimer, 50 );
  _this->KeyHandlerDown.Filter = CoreKeyCodeDown;
  _this->KeyHandlerUp.Filter = CoreKeyCodeUp;
  CoreView_OnSetLayout((CoreView)&_this->TouchHandler, CoreLayoutAlignToBottom | 
  CoreLayoutAlignToLeft | CoreLayoutAlignToRight | CoreLayoutAlignToTop | CoreLayoutResizeHorz 
  | CoreLayoutResizeVert );
  CoreQuadView__OnSetPoint4( &_this->TouchHandler, _Const0003 );
  CoreQuadView__OnSetPoint3( &_this->TouchHandler, _Const0004 );
  CoreQuadView__OnSetPoint2( &_this->TouchHandler, _Const0005 );
  CoreQuadView__OnSetPoint1( &_this->TouchHandler, _Const0006 );
  CoreSimpleTouchHandler_OnSetRetargetOffset( &_this->TouchHandler, 16 );
  CoreSimpleTouchHandler_OnSetMaxStrikeCount( &_this->TouchHandler, 100 );
  _this->StepSize = 1;
  _this->MaxValue = 100;
  _this->CurrentValue = 50;
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->TouchHandler ), 0 );
  _this->RepetitionTimer.OnTrigger = EwNewSlot( _this, WidgetSetVerticalSlider_onRepetitionTimer );
  _this->KeyHandlerDown.OnRelease = EwNewSlot( _this, WidgetSetVerticalSlider_onReleaseKey );
  _this->KeyHandlerDown.OnPress = EwNewSlot( _this, WidgetSetVerticalSlider_onPressKey );
  _this->KeyHandlerUp.OnRelease = EwNewSlot( _this, WidgetSetVerticalSlider_onReleaseKey );
  _this->KeyHandlerUp.OnPress = EwNewSlot( _this, WidgetSetVerticalSlider_onPressKey );
  _this->TouchHandler.OnDrag = EwNewSlot( _this, WidgetSetVerticalSlider_onDragTouch );
  _this->TouchHandler.OnRelease = EwNewSlot( _this, WidgetSetVerticalSlider_onReleaseTouch );
  _this->TouchHandler.OnPress = EwNewSlot( _this, WidgetSetVerticalSlider_onPressTouch );
}

/* Re-Initializer for the class 'WidgetSet::VerticalSlider' */
void WidgetSetVerticalSlider__ReInit( WidgetSetVerticalSlider _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  CoreTimer__ReInit( &_this->RepetitionTimer );
  CoreKeyPressHandler__ReInit( &_this->KeyHandlerDown );
  CoreKeyPressHandler__ReInit( &_this->KeyHandlerUp );
  CoreSimpleTouchHandler__ReInit( &_this->TouchHandler );
}

/* Finalizer method for the class 'WidgetSet::VerticalSlider' */
void WidgetSetVerticalSlider__Done( WidgetSetVerticalSlider _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreGroup );

  /* Finalize all embedded objects */
  CoreTimer__Done( &_this->RepetitionTimer );
  CoreKeyPressHandler__Done( &_this->KeyHandlerDown );
  CoreKeyPressHandler__Done( &_this->KeyHandlerUp );
  CoreSimpleTouchHandler__Done( &_this->TouchHandler );

  /* Don't forget to deinitialize the super class ... */
  CoreGroup__Done( &_this->_Super );
}

/* 'C' function for method : 'WidgetSet::VerticalSlider.OnSetBounds()' */
void WidgetSetVerticalSlider_OnSetBounds( WidgetSetVerticalSlider _this, XRect value )
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
void WidgetSetVerticalSlider_UpdateViewState( WidgetSetVerticalSlider _this, XSet 
  aState )
{
  XBool needsFace;
  XBool needsTrackBelow;
  XBool needsTrackAbove;
  XBool needsThumb;
  XBool needsCover;
  XBool restack;
  XRect area;
  XPoint thumbSize;
  XBool isEnabled;
  XBool isFocused;
  XBool isPressed;
  XInt32 marginBelow;
  XInt32 marginAbove;
  XInt32 minPos;
  XInt32 maxPos;
  XInt32 newThumbPos;

  CoreGroup_UpdateViewState((CoreGroup)_this, aState );
  needsFace = (XBool)(( _this->Appearance != 0 ) && (((( _this->Appearance->FaceDefault 
  != 0 ) || ( _this->Appearance->FaceDisabled != 0 )) || ( _this->Appearance->FaceFocused 
  != 0 )) || ( _this->Appearance->FaceActive != 0 )));
  needsTrackBelow = (XBool)(( _this->Appearance != 0 ) && (((( _this->Appearance->TrackBelowDefault 
  != 0 ) || ( _this->Appearance->TrackBelowDisabled != 0 )) || ( _this->Appearance->TrackBelowFocused 
  != 0 )) || ( _this->Appearance->TrackBelowActive != 0 )));
  needsTrackAbove = (XBool)(( _this->Appearance != 0 ) && (((( _this->Appearance->TrackAboveDefault 
  != 0 ) || ( _this->Appearance->TrackAboveDisabled != 0 )) || ( _this->Appearance->TrackAboveFocused 
  != 0 )) || ( _this->Appearance->TrackAboveActive != 0 )));
  needsThumb = (XBool)(( _this->Appearance != 0 ) && (((( _this->Appearance->ThumbDefault 
  != 0 ) || ( _this->Appearance->ThumbDisabled != 0 )) || ( _this->Appearance->ThumbFocused 
  != 0 )) || ( _this->Appearance->ThumbActive != 0 )));
  needsCover = (XBool)(( _this->Appearance != 0 ) && (((( _this->Appearance->CoverDefault 
  != 0 ) || ( _this->Appearance->CoverDisabled != 0 )) || ( _this->Appearance->CoverFocused 
  != 0 )) || ( _this->Appearance->CoverActive != 0 )));
  restack = 0;
  area = EwGetRectORect( _this->Super2.Bounds );
  thumbSize = _Const0006;

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

  if ( needsTrackBelow && ( _this->frameView2 == 0 ))
  {
    _this->frameView2 = EwNewObject( ViewsFrame, 0 );
    CoreGroup_Add((CoreGroup)_this, ((CoreView)_this->frameView2 ), 0 );
    restack = 1;
  }
  else
    if ( !needsTrackBelow && ( _this->frameView2 != 0 ))
    {
      CoreGroup_Remove((CoreGroup)_this, ((CoreView)_this->frameView2 ));
      _this->frameView2 = 0;
    }

  if ( needsTrackAbove && ( _this->frameView3 == 0 ))
  {
    _this->frameView3 = EwNewObject( ViewsFrame, 0 );
    CoreGroup_Add((CoreGroup)_this, ((CoreView)_this->frameView3 ), 0 );
    restack = 1;
  }
  else
    if ( !needsTrackAbove && ( _this->frameView3 != 0 ))
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
  isPressed = (XBool)(( _this->touchActive || _this->KeyHandlerUp.Down ) || _this->KeyHandlerDown.Down );

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
      | GraphicsEdgesRight;

    if ( !isEnabled )
    {
      bitmap = _this->Appearance->TrackBelowDisabled;
      frameNo = _this->Appearance->TrackBelowFrameDisabled;
      tint = _this->Appearance->TrackBelowTintDisabled;
    }
    else
      if ( isPressed )
      {
        bitmap = _this->Appearance->TrackBelowActive;
        frameNo = _this->Appearance->TrackBelowFrameActive;
        tint = _this->Appearance->TrackBelowTintActive;
      }
      else
        if ( isFocused )
        {
          bitmap = _this->Appearance->TrackBelowFocused;
          frameNo = _this->Appearance->TrackBelowFrameFocused;
          tint = _this->Appearance->TrackBelowTintFocused;
        }
        else
        {
          bitmap = _this->Appearance->TrackBelowDefault;
          frameNo = _this->Appearance->TrackBelowFrameDefault;
          tint = _this->Appearance->TrackBelowTintDefault;
        }

    ViewsFrame_OnSetAnimated( _this->frameView2, (XBool)( frameNo < 0 ));
    ViewsFrame_OnSetColor( _this->frameView2, tint );

    if ( frameNo < 0 )
      frameNo = 0;

    if ( _this->Appearance->TrackBelowWithEdge )
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
    XSet edges = GraphicsEdgesInterior | GraphicsEdgesLeft | GraphicsEdgesRight 
      | GraphicsEdgesTop;

    if ( !isEnabled )
    {
      bitmap = _this->Appearance->TrackAboveDisabled;
      frameNo = _this->Appearance->TrackAboveFrameDisabled;
      tint = _this->Appearance->TrackAboveTintDisabled;
    }
    else
      if ( isPressed )
      {
        bitmap = _this->Appearance->TrackAboveActive;
        frameNo = _this->Appearance->TrackAboveFrameActive;
        tint = _this->Appearance->TrackAboveTintActive;
      }
      else
        if ( isFocused )
        {
          bitmap = _this->Appearance->TrackAboveFocused;
          frameNo = _this->Appearance->TrackAboveFrameFocused;
          tint = _this->Appearance->TrackAboveTintFocused;
        }
        else
        {
          bitmap = _this->Appearance->TrackAboveDefault;
          frameNo = _this->Appearance->TrackAboveFrameDefault;
          tint = _this->Appearance->TrackAboveTintDefault;
        }

    ViewsFrame_OnSetAnimated( _this->frameView3, (XBool)( frameNo < 0 ));
    ViewsFrame_OnSetColor( _this->frameView3, tint );

    if ( frameNo < 0 )
      frameNo = 0;

    if ( _this->Appearance->TrackAboveWithEdge )
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

  marginBelow = thumbSize.Y / 2;
  marginAbove = thumbSize.Y - marginBelow;

  if ( _this->Appearance != 0 )
  {
    marginBelow = marginBelow + _this->Appearance->ThumbMarginBelow;
    marginAbove = marginAbove + _this->Appearance->ThumbMarginAbove;
  }

  minPos = marginBelow;
  maxPos = EwGetRectH( area ) - marginAbove;
  newThumbPos = marginBelow;

  if ( _this->MaxValue != _this->MinValue )
    newThumbPos = EwGetRectH( area ) - ((XInt32)(((XFloat)( WidgetSetVerticalSlider_OnGetCurrentValue( 
    _this ) - _this->MinValue ) * (XFloat)( maxPos - minPos )) / (XFloat)( _this->MaxValue 
    - _this->MinValue )) + minPos );

  if ( newThumbPos < 0 )
    newThumbPos = 0;

  if ( newThumbPos > EwGetRectH( area ))
    newThumbPos = EwGetRectH( area );

  if (( _this->frameView1 != 0 ) && ( _this->frameView1->Bitmap != 0 ))
  {
    XInt32 w = _this->frameView1->Bitmap->FrameSize.X;
    CoreRectView__OnSetBounds( _this->frameView1, EwNewRect(( EwGetRectW( area ) 
    / 2 ) - ( w / 2 ), 0, (( EwGetRectW( area ) / 2 ) - ( w / 2 )) + w, EwGetRectH( 
    area )));
  }

  if (( _this->frameView2 != 0 ) && ( _this->frameView2->Bitmap != 0 ))
  {
    XPoint s = _this->frameView2->Bitmap->FrameSize;
    CoreRectView__OnSetBounds( _this->frameView2, EwNewRect(( EwGetRectW( area ) 
    / 2 ) - ( s.X / 2 ), newThumbPos, (( EwGetRectW( area ) / 2 ) - ( s.X / 2 )) 
    + s.X, EwGetRectH( area )));
    ViewsFrame_OnSetNoEdgesLimit( _this->frameView2, EwSetPointY( _this->frameView2->NoEdgesLimit, 
    EwGetRectH( area ) - ( s.Y / 3 )));
    ViewsFrame_OnSetVisible( _this->frameView2, (XBool)( !_this->Appearance->TrackBelowWithEdge 
    || (( EwGetRectH( area ) - newThumbPos ) >= (( s.Y / 3 ) * 2 ))));
  }

  if (( _this->frameView3 != 0 ) && ( _this->frameView3->Bitmap != 0 ))
  {
    XPoint s = _this->frameView3->Bitmap->FrameSize;
    CoreRectView__OnSetBounds( _this->frameView3, EwNewRect(( EwGetRectW( area ) 
    / 2 ) - ( s.X / 2 ), 0, (( EwGetRectW( area ) / 2 ) - ( s.X / 2 )) + s.X, newThumbPos ));
    ViewsFrame_OnSetNoEdgesLimit( _this->frameView3, EwSetPointY( _this->frameView3->NoEdgesLimit, 
    EwGetRectH( area ) - ( s.Y / 3 )));
    ViewsFrame_OnSetVisible( _this->frameView3, (XBool)( !_this->Appearance->TrackAboveWithEdge 
    || ( newThumbPos >= (( s.Y / 3 ) * 2 ))));
  }

  if (( _this->frameView4 != 0 ) && ( _this->frameView4->Bitmap != 0 ))
  {
    XInt32 w = _this->frameView4->Bitmap->FrameSize.X;
    CoreRectView__OnSetBounds( _this->frameView4, EwNewRect(( EwGetRectW( area ) 
    / 2 ) - ( w / 2 ), 0, (( EwGetRectW( area ) / 2 ) - ( w / 2 )) + w, EwGetRectH( 
    area )));
  }

  if (( _this->imageView != 0 ) && ( _this->imageView->Bitmap != 0 ))
  {
    XRect thumbArea = EwNewRect2Point( _Const0006, thumbSize );
    CoreRectView__OnSetBounds( _this->imageView, EwMoveRectPos( EwMoveRectNeg( thumbArea, 
    EwGetRectCenter( thumbArea )), EwNewPoint( EwGetRectW( area ) / 2, newThumbPos )));
  }
}

/* 'C' function for method : 'WidgetSet::VerticalSlider.onConfigChanged()' */
void WidgetSetVerticalSlider_onConfigChanged( WidgetSetVerticalSlider _this, XObject 
  sender )
{
  XEnum oldKeyCodeUp;
  XEnum oldKeyCodeDown;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  oldKeyCodeUp = _this->KeyHandlerUp.Filter;
  oldKeyCodeDown = _this->KeyHandlerDown.Filter;

  if ( _this->Appearance != 0 )
  {
    _this->KeyHandlerUp.Filter = _this->Appearance->KeyCodeUp;
    _this->KeyHandlerDown.Filter = _this->Appearance->KeyCodeDown;
  }
  else
  {
    _this->KeyHandlerUp.Filter = CoreKeyCodeUp;
    _this->KeyHandlerDown.Filter = CoreKeyCodeDown;
  }

  if (((( oldKeyCodeUp == CoreKeyCodeNoKey ) || ( oldKeyCodeDown == CoreKeyCodeNoKey )) 
      && ( _this->KeyHandlerUp.Filter != CoreKeyCodeNoKey )) && ( _this->KeyHandlerDown.Filter 
      != CoreKeyCodeNoKey ))
    CoreView__ChangeViewState( _this, CoreViewStateFocusable, 0 );

  if ((( oldKeyCodeUp != CoreKeyCodeNoKey ) && ( oldKeyCodeDown != CoreKeyCodeNoKey )) 
      && (( _this->KeyHandlerUp.Filter == CoreKeyCodeNoKey ) || ( _this->KeyHandlerDown.Filter 
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

/* 'C' function for method : 'WidgetSet::VerticalSlider.onOutlet()' */
void WidgetSetVerticalSlider_onOutlet( WidgetSetVerticalSlider _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( _this->Outlet.Object != 0 )
    WidgetSetVerticalSlider_OnSetCurrentValue( _this, EwOnGetInt32( _this->Outlet ));
}

/* 'C' function for method : 'WidgetSet::VerticalSlider.onRepetitionTimer()' */
void WidgetSetVerticalSlider_onRepetitionTimer( WidgetSetVerticalSlider _this, XObject 
  sender )
{
  XInt32 oldValue;
  XInt32 newValue;
  XInt32 delta;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  oldValue = WidgetSetVerticalSlider_OnGetCurrentValue( _this );
  newValue = oldValue;
  delta = _this->StepSize;

  if (( _this->KeyHandlerDown.Down && ( _this->MaxValue > _this->MinValue )) || 
      ( _this->KeyHandlerUp.Down && ( _this->MaxValue < _this->MinValue )))
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

  WidgetSetVerticalSlider_OnSetCurrentValue( _this, newValue );

  if ( oldValue == WidgetSetVerticalSlider_OnGetCurrentValue( _this ))
    return;

  EwPostSignal( _this->OnChange, ((XObject)_this ));

  if ( _this->Outlet.Object != 0 )
  {
    EwOnSetInt32( _this->Outlet, WidgetSetVerticalSlider_OnGetCurrentValue( _this ));
    EwNotifyRefObservers( _this->Outlet, 0 );
  }
}

/* 'C' function for method : 'WidgetSet::VerticalSlider.onReleaseKey()' */
void WidgetSetVerticalSlider_onReleaseKey( WidgetSetVerticalSlider _this, XObject 
  sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  CoreSimpleTouchHandler_OnSetEnabled( &_this->TouchHandler, 1 );
  CoreGroup_InvalidateViewState((CoreGroup)_this );
  CoreTimer_OnSetEnabled( &_this->RepetitionTimer, 0 );
  EwPostSignal( _this->OnEnd, ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::VerticalSlider.onPressKey()' */
void WidgetSetVerticalSlider_onPressKey( WidgetSetVerticalSlider _this, XObject 
  sender )
{
  XInt32 oldValue;
  XInt32 newValue;
  XInt32 delta;

  CoreSimpleTouchHandler_OnSetEnabled( &_this->TouchHandler, 0 );
  CoreGroup_InvalidateViewState((CoreGroup)_this );
  EwPostSignal( _this->OnStart, ((XObject)_this ));
  CoreTimer_OnSetEnabled( &_this->RepetitionTimer, 1 );
  oldValue = WidgetSetVerticalSlider_OnGetCurrentValue( _this );
  newValue = oldValue;
  delta = _this->StepSize;

  if ((( sender == ((XObject)&_this->KeyHandlerDown )) && ( _this->MaxValue > _this->MinValue )) 
      || (( sender == ((XObject)&_this->KeyHandlerUp )) && ( _this->MaxValue < _this->MinValue )))
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

  WidgetSetVerticalSlider_OnSetCurrentValue( _this, newValue );

  if ( oldValue == WidgetSetVerticalSlider_OnGetCurrentValue( _this ))
    return;

  EwPostSignal( _this->OnChange, ((XObject)_this ));

  if ( _this->Outlet.Object != 0 )
  {
    EwOnSetInt32( _this->Outlet, WidgetSetVerticalSlider_OnGetCurrentValue( _this ));
    EwNotifyRefObservers( _this->Outlet, 0 );
  }
}

/* 'C' function for method : 'WidgetSet::VerticalSlider.onDragTouch()' */
void WidgetSetVerticalSlider_onDragTouch( WidgetSetVerticalSlider _this, XObject 
  sender )
{
  XInt32 marginBelow;
  XInt32 marginAbove;
  XPoint delta;
  XInt32 minPos;
  XInt32 maxPos;
  XInt32 oldValue;
  XInt32 newValue;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( !_this->touchActive )
    return;

  marginBelow = 0;
  marginAbove = 0;

  if ( _this->Appearance != 0 )
  {
    marginBelow = _this->Appearance->ThumbMarginBelow;
    marginAbove = _this->Appearance->ThumbMarginAbove;
  }

  delta = EwMovePointNeg( _this->TouchHandler.CurrentPos, _this->TouchHandler.HittingPos );
  minPos = marginBelow;
  maxPos = EwGetRectH( _this->Super2.Bounds ) - marginAbove;
  oldValue = WidgetSetVerticalSlider_OnGetCurrentValue( _this );
  newValue = oldValue;

  if (( _this->imageView != 0 ) && ( _this->imageView->Bitmap != 0 ))
    maxPos = maxPos - EwGetRectH( _this->imageView->Super1.Bounds );

  if ( maxPos > minPos )
    newValue = (XInt32)((((XFloat)-delta.Y ) * (XFloat)( _this->MaxValue - _this->MinValue )) 
    / (XFloat)( maxPos - minPos )) + _this->touchStartValue;

  WidgetSetVerticalSlider_OnSetCurrentValue( _this, newValue - ( newValue % _this->StepSize ));

  if ( oldValue == WidgetSetVerticalSlider_OnGetCurrentValue( _this ))
    return;

  EwPostSignal( _this->OnChange, ((XObject)_this ));

  if ( _this->Outlet.Object != 0 )
  {
    EwOnSetInt32( _this->Outlet, WidgetSetVerticalSlider_OnGetCurrentValue( _this ));
    EwNotifyRefObservers( _this->Outlet, 0 );
  }
}

/* 'C' function for method : 'WidgetSet::VerticalSlider.onReleaseTouch()' */
void WidgetSetVerticalSlider_onReleaseTouch( WidgetSetVerticalSlider _this, XObject 
  sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( !_this->touchActive )
    return;

  _this->touchActive = 0;
  _this->KeyHandlerUp.Enabled = 1;
  _this->KeyHandlerDown.Enabled = 1;
  CoreGroup_InvalidateViewState((CoreGroup)_this );
  EwPostSignal( _this->OnEnd, ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::VerticalSlider.onPressTouch()' */
void WidgetSetVerticalSlider_onPressTouch( WidgetSetVerticalSlider _this, XObject 
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
  _this->KeyHandlerUp.Enabled = 0;
  _this->KeyHandlerDown.Enabled = 0;
  EwPostSignal( _this->OnStart, ((XObject)_this ));
  CoreGroup_InvalidateViewState((CoreGroup)_this );
  _this->touchStartValue = WidgetSetVerticalSlider_OnGetCurrentValue( _this );

  if ( adjustAtTouch && !insideThumb )
  {
    XInt32 marginBelow = 0;
    XInt32 marginAbove = 0;
    XInt32 thumbHeight = 0;
    XInt32 hitPos;
    XInt32 minPos;
    XInt32 maxPos;
    XInt32 oldValue;
    XInt32 newValue;

    if ( _this->Appearance != 0 )
    {
      marginBelow = _this->Appearance->ThumbMarginBelow;
      marginAbove = _this->Appearance->ThumbMarginAbove;
    }

    if (( _this->imageView != 0 ) && ( _this->imageView->Bitmap != 0 ))
      thumbHeight = EwGetRectH( _this->imageView->Super1.Bounds );

    hitPos = ( EwGetRectH( _this->Super2.Bounds ) - _this->TouchHandler.HittingPos.Y ) 
    - ( thumbHeight / 2 );
    minPos = marginBelow;
    maxPos = ( EwGetRectH( _this->Super2.Bounds ) - marginAbove ) - thumbHeight;
    oldValue = WidgetSetVerticalSlider_OnGetCurrentValue( _this );
    newValue = oldValue;

    if ( maxPos > minPos )
      newValue = (XInt32)(((XFloat)( hitPos - minPos ) * (XFloat)( _this->MaxValue 
      - _this->MinValue )) / (XFloat)( maxPos - minPos )) + _this->MinValue;

    WidgetSetVerticalSlider_OnSetCurrentValue( _this, newValue - ( newValue % _this->StepSize ));

    if ( oldValue == WidgetSetVerticalSlider_OnGetCurrentValue( _this ))
      return;

    EwPostSignal( _this->OnChange, ((XObject)_this ));

    if ( _this->Outlet.Object != 0 )
    {
      EwOnSetInt32( _this->Outlet, WidgetSetVerticalSlider_OnGetCurrentValue( _this ));
      EwNotifyRefObservers( _this->Outlet, 0 );
    }

    _this->touchStartValue = WidgetSetVerticalSlider_OnGetCurrentValue( _this );
  }
}

/* 'C' function for method : 'WidgetSet::VerticalSlider.OnSetOutlet()' */
void WidgetSetVerticalSlider_OnSetOutlet( WidgetSetVerticalSlider _this, XRef value )
{
  if ( !EwCompRef( _this->Outlet, value ))
    return;

  if ( _this->Outlet.Object != 0 )
    EwDetachRefObserver( EwNewSlot( _this, WidgetSetVerticalSlider_onOutlet ), _this->Outlet, 
      0 );

  _this->Outlet = value;

  if ( value.Object != 0 )
    EwAttachRefObserver( EwNewSlot( _this, WidgetSetVerticalSlider_onOutlet ), value, 
      0 );

  if ( value.Object != 0 )
    EwPostSignal( EwNewSlot( _this, WidgetSetVerticalSlider_onOutlet ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::VerticalSlider.OnGetCurrentValue()' */
XInt32 WidgetSetVerticalSlider_OnGetCurrentValue( WidgetSetVerticalSlider _this )
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

/* 'C' function for method : 'WidgetSet::VerticalSlider.OnSetCurrentValue()' */
void WidgetSetVerticalSlider_OnSetCurrentValue( WidgetSetVerticalSlider _this, XInt32 
  value )
{
  if ( _this->CurrentValue == value )
    return;

  _this->CurrentValue = value;
  CoreGroup_InvalidateViewState((CoreGroup)_this );
  EwPostSignal( _this->OnUpdate, ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::VerticalSlider.OnSetAppearance()' */
void WidgetSetVerticalSlider_OnSetAppearance( WidgetSetVerticalSlider _this, WidgetSetVerticalSliderConfig 
  value )
{
  if ( _this->Appearance == value )
    return;

  if ( _this->Appearance != 0 )
    EwDetachObjObserver( EwNewSlot( _this, WidgetSetVerticalSlider_onConfigChanged ), 
      (XObject)_this->Appearance, 0 );

  _this->Appearance = value;

  if ( value != 0 )
    EwAttachObjObserver( EwNewSlot( _this, WidgetSetVerticalSlider_onConfigChanged ), 
      (XObject)value, 0 );

  EwPostSignal( EwNewSlot( _this, WidgetSetVerticalSlider_onConfigChanged ), ((XObject)_this ));
}

/* Variants derived from the class : 'WidgetSet::VerticalSlider' */
EW_DEFINE_CLASS_VARIANTS( WidgetSetVerticalSlider )
EW_END_OF_CLASS_VARIANTS( WidgetSetVerticalSlider )

/* Virtual Method Table (VMT) for the class : 'WidgetSet::VerticalSlider' */
EW_DEFINE_CLASS( WidgetSetVerticalSlider, CoreGroup, frameView4, OnUpdate, Outlet, 
                 RepetitionTimer, touchStartValue, touchStartValue, "WidgetSet::VerticalSlider" )
  CoreRectView_initLayoutContext,
  CoreView_GetRoot,
  CoreGroup_Draw,
  CoreView_HandleEvent,
  CoreGroup_CursorHitTest,
  CoreRectView_ArrangeView,
  CoreRectView_MoveView,
  CoreRectView_GetExtent,
  CoreGroup_ChangeViewState,
  WidgetSetVerticalSlider_OnSetBounds,
  CoreGroup_OnSetFocus,
  CoreGroup_DispatchEvent,
  CoreGroup_BroadcastEvent,
  CoreGroup_UpdateLayout,
  WidgetSetVerticalSlider_UpdateViewState,
  CoreGroup_InvalidateArea,
EW_END_OF_CLASS( WidgetSetVerticalSlider )

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
  CoreRectView__OnSetBounds( _this, _Const0007 );
  CoreTimer_OnSetPeriod( &_this->FlashTimer, 0 );
  CoreTimer_OnSetBegin( &_this->FlashTimer, 50 );
  _this->KeyHandler.Filter = CoreKeyCodeEnter;
  CoreView_OnSetLayout((CoreView)&_this->TouchHandler, CoreLayoutAlignToBottom | 
  CoreLayoutAlignToLeft | CoreLayoutAlignToRight | CoreLayoutAlignToTop | CoreLayoutResizeHorz 
  | CoreLayoutResizeVert );
  CoreQuadView__OnSetPoint4( &_this->TouchHandler, _Const0008 );
  CoreQuadView__OnSetPoint3( &_this->TouchHandler, _Const0009 );
  CoreQuadView__OnSetPoint2( &_this->TouchHandler, _Const000A );
  CoreQuadView__OnSetPoint1( &_this->TouchHandler, _Const0006 );
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

/* 'C' function for method : 'WidgetSet::RadioButton.onOutlet()' */
void WidgetSetRadioButton_onOutlet( WidgetSetRadioButton _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( _this->Outlet.Object != 0 )
    WidgetSetRadioButton_OnSetSelected( _this, (XBool)( EwOnGetInt32( _this->Outlet ) 
    == _this->OutletSelector ));
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

/* 'C' function for method : 'WidgetSet::RadioButton.OnSetOutletSelector()' */
void WidgetSetRadioButton_OnSetOutletSelector( WidgetSetRadioButton _this, XInt32 
  value )
{
  if ( _this->OutletSelector == value )
    return;

  _this->OutletSelector = value;

  if ( _this->Outlet.Object != 0 )
    WidgetSetRadioButton_OnSetSelected( _this, (XBool)( EwOnGetInt32( _this->Outlet ) 
    == _this->OutletSelector ));
}

/* 'C' function for method : 'WidgetSet::RadioButton.OnSetOutlet()' */
void WidgetSetRadioButton_OnSetOutlet( WidgetSetRadioButton _this, XRef value )
{
  if ( !EwCompRef( _this->Outlet, value ))
    return;

  if ( _this->Outlet.Object != 0 )
    EwDetachRefObserver( EwNewSlot( _this, WidgetSetRadioButton_onOutlet ), _this->Outlet, 
      0 );

  _this->Outlet = value;

  if ( value.Object != 0 )
    EwAttachRefObserver( EwNewSlot( _this, WidgetSetRadioButton_onOutlet ), value, 
      0 );

  if ( value.Object != 0 )
    EwPostSignal( EwNewSlot( _this, WidgetSetRadioButton_onOutlet ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::RadioButton.OnSetSelected()' */
void WidgetSetRadioButton_OnSetSelected( WidgetSetRadioButton _this, XBool value )
{
  if ( _this->Selected == value )
    return;

  _this->Selected = value;
  CoreGroup_InvalidateViewState((CoreGroup)_this );
}

/* 'C' function for method : 'WidgetSet::RadioButton.OnSetLabel()' */
void WidgetSetRadioButton_OnSetLabel( WidgetSetRadioButton _this, XString value )
{
  if ( !EwCompString( value, _this->LabelOff ) && !EwCompString( value, _this->LabelOn ))
    return;

  _this->LabelOff = EwShareString( value );
  _this->LabelOn = EwShareString( value );
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
  CoreGroup_UpdateLayout,
  WidgetSetRadioButton_UpdateViewState,
  CoreGroup_InvalidateArea,
EW_END_OF_CLASS( WidgetSetRadioButton )

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
  CoreRectView__OnSetBounds( _this, _Const0007 );
  CoreTimer_OnSetPeriod( &_this->FlashTimer, 0 );
  CoreTimer_OnSetBegin( &_this->FlashTimer, 50 );
  _this->KeyHandler.Filter = CoreKeyCodeEnter;
  CoreView_OnSetLayout((CoreView)&_this->TouchHandler, CoreLayoutAlignToBottom | 
  CoreLayoutAlignToLeft | CoreLayoutAlignToRight | CoreLayoutAlignToTop | CoreLayoutResizeHorz 
  | CoreLayoutResizeVert );
  CoreQuadView__OnSetPoint4( &_this->TouchHandler, _Const0008 );
  CoreQuadView__OnSetPoint3( &_this->TouchHandler, _Const0009 );
  CoreQuadView__OnSetPoint2( &_this->TouchHandler, _Const000A );
  CoreQuadView__OnSetPoint1( &_this->TouchHandler, _Const0006 );
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

/* 'C' function for method : 'WidgetSet::ToggleButton.onOutlet()' */
void WidgetSetToggleButton_onOutlet( WidgetSetToggleButton _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( _this->Outlet.Object != 0 )
    WidgetSetToggleButton_OnSetChecked( _this, EwOnGetBool( _this->Outlet ));
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

/* 'C' function for method : 'WidgetSet::ToggleButton.OnSetOutlet()' */
void WidgetSetToggleButton_OnSetOutlet( WidgetSetToggleButton _this, XRef value )
{
  if ( !EwCompRef( _this->Outlet, value ))
    return;

  if ( _this->Outlet.Object != 0 )
    EwDetachRefObserver( EwNewSlot( _this, WidgetSetToggleButton_onOutlet ), _this->Outlet, 
      0 );

  _this->Outlet = value;

  if ( value.Object != 0 )
    EwAttachRefObserver( EwNewSlot( _this, WidgetSetToggleButton_onOutlet ), value, 
      0 );

  if ( value.Object != 0 )
    EwPostSignal( EwNewSlot( _this, WidgetSetToggleButton_onOutlet ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::ToggleButton.OnSetChecked()' */
void WidgetSetToggleButton_OnSetChecked( WidgetSetToggleButton _this, XBool value )
{
  if ( _this->Checked == value )
    return;

  _this->Checked = value;
  CoreGroup_InvalidateViewState((CoreGroup)_this );
}

/* 'C' function for method : 'WidgetSet::ToggleButton.OnSetLabel()' */
void WidgetSetToggleButton_OnSetLabel( WidgetSetToggleButton _this, XString value )
{
  if ( !EwCompString( value, _this->LabelOff ) && !EwCompString( value, _this->LabelOn ))
    return;

  _this->LabelOff = EwShareString( value );
  _this->LabelOn = EwShareString( value );
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
  CoreGroup_DispatchEvent,
  CoreGroup_BroadcastEvent,
  CoreGroup_UpdateLayout,
  WidgetSetToggleButton_UpdateViewState,
  CoreGroup_InvalidateArea,
EW_END_OF_CLASS( WidgetSetToggleButton )

/* Embedded Wizard */
