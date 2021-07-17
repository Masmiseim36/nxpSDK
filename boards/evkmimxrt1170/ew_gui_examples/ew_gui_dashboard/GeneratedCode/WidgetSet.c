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
#include "_CoreSimpleTouchHandler.h"
#include "_CoreTimer.h"
#include "_CoreView.h"
#include "_ResourcesBitmap.h"
#include "_ViewsFrame.h"
#include "_ViewsImage.h"
#include "_ViewsText.h"
#include "_WidgetSetPushButton.h"
#include "_WidgetSetPushButtonConfig.h"
#include "_WidgetSetToggleButton.h"
#include "_WidgetSetToggleButtonConfig.h"
#include "_WidgetSetValueDisplay.h"
#include "_WidgetSetValueDisplayConfig.h"
#include "_WidgetSetWidgetConfig.h"
#include "Core.h"
#include "Views.h"
#include "WidgetSet.h"

/* Compressed strings for the language 'Default'. */
static const unsigned int _StringsDefault0[] =
{
  0x00000014, /* ratio 120.00 % */
  0xB8000900, 0x8005C452, 0xB6090283, 0x60F0C838, 0x00000040, 0x00000000
};

/* Constant values used in this 'C' module only. */
static const XColor _Const0000 = { 0x00, 0x00, 0x00, 0xFF };
static const XStringRes _Const0001 = { _StringsDefault0, 0x0002 };
static const XStringRes _Const0002 = { _StringsDefault0, 0x0006 };
static const XColor _Const0003 = { 0xFF, 0xFF, 0xFF, 0xFF };
static const XRect _Const0004 = {{ 0, 0 }, { 200, 30 }};
static const XRect _Const0005 = {{ 0, 0 }, { 150, 50 }};
static const XPoint _Const0006 = { 0, 50 };
static const XPoint _Const0007 = { 150, 50 };
static const XPoint _Const0008 = { 150, 0 };
static const XPoint _Const0009 = { 0, 0 };

/* Forward declaration of the class Resources::Font */
#ifndef _ResourcesFont_
  EW_DECLARE_CLASS( ResourcesFont )
#define _ResourcesFont_
#endif

/* Initializer for the class 'WidgetSet::ValueDisplayConfig' */
void WidgetSetValueDisplayConfig__Init( WidgetSetValueDisplayConfig _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  WidgetSetWidgetConfig__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( WidgetSetValueDisplayConfig );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( WidgetSetValueDisplayConfig );

  /* ... and initialize objects, variables, properties, etc. */
  _this->ValueColorNegative = _Const0000;
  _this->ValueColorPositive = _Const0000;
  _this->ValueAlignment = ViewsTextAlignmentAlignHorzCenter | ViewsTextAlignmentAlignVertCenter;
  _this->UnitColorNegative = _Const0000;
  _this->UnitColorPositive = _Const0000;
  _this->UnitAlignment = ViewsTextAlignmentAlignHorzCenter | ViewsTextAlignmentAlignVertCenter;
  _this->FormatDecimalSign = EwShareString( EwLoadString( &_Const0001 ));
  _this->FormatMinusSign = EwShareString( EwLoadString( &_Const0002 ));
  _this->Layout = WidgetSetValueDisplayLayoutUnitAndValueAreIndependent;
}

/* Re-Initializer for the class 'WidgetSet::ValueDisplayConfig' */
void WidgetSetValueDisplayConfig__ReInit( WidgetSetValueDisplayConfig _this )
{
  /* At first re-initialize the super class ... */
  WidgetSetWidgetConfig__ReInit( &_this->_Super );
}

/* Finalizer method for the class 'WidgetSet::ValueDisplayConfig' */
void WidgetSetValueDisplayConfig__Done( WidgetSetValueDisplayConfig _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( WidgetSetWidgetConfig );

  /* Don't forget to deinitialize the super class ... */
  WidgetSetWidgetConfig__Done( &_this->_Super );
}

/* 'C' function for method : 'WidgetSet::ValueDisplayConfig.OnSetValueMarginRight()' */
void WidgetSetValueDisplayConfig_OnSetValueMarginRight( WidgetSetValueDisplayConfig _this, 
  XInt32 value )
{
  if ( _this->ValueMarginRight == value )
    return;

  _this->ValueMarginRight = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::ValueDisplayConfig.OnSetValueColorNegative()' */
void WidgetSetValueDisplayConfig_OnSetValueColorNegative( WidgetSetValueDisplayConfig _this, 
  XColor value )
{
  if ( !EwCompColor( _this->ValueColorNegative, value ))
    return;

  _this->ValueColorNegative = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::ValueDisplayConfig.OnSetValueColorPositive()' */
void WidgetSetValueDisplayConfig_OnSetValueColorPositive( WidgetSetValueDisplayConfig _this, 
  XColor value )
{
  if ( !EwCompColor( _this->ValueColorPositive, value ))
    return;

  _this->ValueColorPositive = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::ValueDisplayConfig.OnSetValueAlignment()' */
void WidgetSetValueDisplayConfig_OnSetValueAlignment( WidgetSetValueDisplayConfig _this, 
  XSet value )
{
  if ( _this->ValueAlignment == value )
    return;

  _this->ValueAlignment = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::ValueDisplayConfig.OnSetValueFont()' */
void WidgetSetValueDisplayConfig_OnSetValueFont( WidgetSetValueDisplayConfig _this, 
  ResourcesFont value )
{
  if ( _this->ValueFont == value )
    return;

  _this->ValueFont = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::ValueDisplayConfig.OnSetUnitColorNegative()' */
void WidgetSetValueDisplayConfig_OnSetUnitColorNegative( WidgetSetValueDisplayConfig _this, 
  XColor value )
{
  if ( !EwCompColor( _this->UnitColorNegative, value ))
    return;

  _this->UnitColorNegative = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::ValueDisplayConfig.OnSetUnitColorPositive()' */
void WidgetSetValueDisplayConfig_OnSetUnitColorPositive( WidgetSetValueDisplayConfig _this, 
  XColor value )
{
  if ( !EwCompColor( _this->UnitColorPositive, value ))
    return;

  _this->UnitColorPositive = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::ValueDisplayConfig.OnSetUnitFont()' */
void WidgetSetValueDisplayConfig_OnSetUnitFont( WidgetSetValueDisplayConfig _this, 
  ResourcesFont value )
{
  if ( _this->UnitFont == value )
    return;

  _this->UnitFont = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::ValueDisplayConfig.OnSetFormatDecimalSign()' */
void WidgetSetValueDisplayConfig_OnSetFormatDecimalSign( WidgetSetValueDisplayConfig _this, 
  XString value )
{
  if ( !EwCompString( _this->FormatDecimalSign, value ))
    return;

  _this->FormatDecimalSign = EwShareString( value );
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::ValueDisplayConfig.OnSetLayout()' */
void WidgetSetValueDisplayConfig_OnSetLayout( WidgetSetValueDisplayConfig _this, 
  XEnum value )
{
  if ( _this->Layout == value )
    return;

  _this->Layout = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* Variants derived from the class : 'WidgetSet::ValueDisplayConfig' */
EW_DEFINE_CLASS_VARIANTS( WidgetSetValueDisplayConfig )
EW_END_OF_CLASS_VARIANTS( WidgetSetValueDisplayConfig )

/* Virtual Method Table (VMT) for the class : 'WidgetSet::ValueDisplayConfig' */
EW_DEFINE_CLASS( WidgetSetValueDisplayConfig, WidgetSetWidgetConfig, ValueFont, 
                 FormatDigitGroupSign, FormatDigitGroupSign, FormatDigitGroupSign, 
                 FormatDigitGroupSign, ValueMarginBottom, "WidgetSet::ValueDisplayConfig" )
EW_END_OF_CLASS( WidgetSetValueDisplayConfig )

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
  _this->LabelOnColorActive = _Const0000;
  _this->LabelOnColorFocused = _Const0000;
  _this->LabelOnColorDisabled = _Const0000;
  _this->LabelOnColorDefault = _Const0000;
  _this->LabelOffColorActive = _Const0000;
  _this->LabelOffColorFocused = _Const0000;
  _this->LabelOffColorDisabled = _Const0000;
  _this->LabelOffColorDefault = _Const0000;
  _this->IconOnTintActive = _Const0003;
  _this->IconOnTintFocused = _Const0003;
  _this->IconOnTintDisabled = _Const0003;
  _this->IconOnTintDefault = _Const0003;
  _this->IconOffTintActive = _Const0003;
  _this->IconOffTintFocused = _Const0003;
  _this->IconOffTintDisabled = _Const0003;
  _this->IconOffTintDefault = _Const0003;
  _this->LabelAlignment = ViewsTextAlignmentAlignHorzCenter | ViewsTextAlignmentAlignVertCenter;
  _this->IconAlignment = ViewsImageAlignmentAlignHorzCenter | ViewsImageAlignmentAlignVertCenter;
  _this->FaceLayout = CoreLayoutAlignToBottom | CoreLayoutAlignToLeft | CoreLayoutAlignToRight 
  | CoreLayoutAlignToTop | CoreLayoutResizeHorz | CoreLayoutResizeVert;
  _this->FaceOnTintActive = _Const0003;
  _this->FaceOnTintFocused = _Const0003;
  _this->FaceOnTintDisabled = _Const0003;
  _this->FaceOnTintDefault = _Const0003;
  _this->FaceOnFrameActive = -1;
  _this->FaceOnFrameFocused = -1;
  _this->FaceOnFrameDisabled = -1;
  _this->FaceOnFrameDefault = -1;
  _this->FaceOffTintActive = _Const0003;
  _this->FaceOffTintFocused = _Const0003;
  _this->FaceOffTintDisabled = _Const0003;
  _this->FaceOffTintDefault = _Const0003;
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

/* 'C' function for method : 'WidgetSet::ToggleButtonConfig.OnSetIconOnTintActive()' */
void WidgetSetToggleButtonConfig_OnSetIconOnTintActive( WidgetSetToggleButtonConfig _this, 
  XColor value )
{
  if ( !EwCompColor( _this->IconOnTintActive, value ))
    return;

  _this->IconOnTintActive = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::ToggleButtonConfig.OnSetIconOnTintFocused()' */
void WidgetSetToggleButtonConfig_OnSetIconOnTintFocused( WidgetSetToggleButtonConfig _this, 
  XColor value )
{
  if ( !EwCompColor( _this->IconOnTintFocused, value ))
    return;

  _this->IconOnTintFocused = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::ToggleButtonConfig.OnSetIconOnTintDisabled()' */
void WidgetSetToggleButtonConfig_OnSetIconOnTintDisabled( WidgetSetToggleButtonConfig _this, 
  XColor value )
{
  if ( !EwCompColor( _this->IconOnTintDisabled, value ))
    return;

  _this->IconOnTintDisabled = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::ToggleButtonConfig.OnSetIconOnTintDefault()' */
void WidgetSetToggleButtonConfig_OnSetIconOnTintDefault( WidgetSetToggleButtonConfig _this, 
  XColor value )
{
  if ( !EwCompColor( _this->IconOnTintDefault, value ))
    return;

  _this->IconOnTintDefault = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::ToggleButtonConfig.OnSetIconOffTintActive()' */
void WidgetSetToggleButtonConfig_OnSetIconOffTintActive( WidgetSetToggleButtonConfig _this, 
  XColor value )
{
  if ( !EwCompColor( _this->IconOffTintActive, value ))
    return;

  _this->IconOffTintActive = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::ToggleButtonConfig.OnSetIconOffTintFocused()' */
void WidgetSetToggleButtonConfig_OnSetIconOffTintFocused( WidgetSetToggleButtonConfig _this, 
  XColor value )
{
  if ( !EwCompColor( _this->IconOffTintFocused, value ))
    return;

  _this->IconOffTintFocused = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::ToggleButtonConfig.OnSetIconOffTintDisabled()' */
void WidgetSetToggleButtonConfig_OnSetIconOffTintDisabled( WidgetSetToggleButtonConfig _this, 
  XColor value )
{
  if ( !EwCompColor( _this->IconOffTintDisabled, value ))
    return;

  _this->IconOffTintDisabled = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::ToggleButtonConfig.OnSetIconOffTintDefault()' */
void WidgetSetToggleButtonConfig_OnSetIconOffTintDefault( WidgetSetToggleButtonConfig _this, 
  XColor value )
{
  if ( !EwCompColor( _this->IconOffTintDefault, value ))
    return;

  _this->IconOffTintDefault = value;
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
  _this->LabelColorActive = _Const0000;
  _this->LabelColorFocused = _Const0000;
  _this->LabelColorDisabled = _Const0000;
  _this->LabelColorDefault = _Const0000;
  _this->IconTintActive = _Const0003;
  _this->IconTintFocused = _Const0003;
  _this->IconTintDisabled = _Const0003;
  _this->IconTintDefault = _Const0003;
  _this->LabelAlignment = ViewsTextAlignmentAlignHorzCenter | ViewsTextAlignmentAlignVertCenter;
  _this->IconAlignment = ViewsImageAlignmentAlignHorzCenter | ViewsImageAlignmentAlignVertCenter;
  _this->FaceLayout = CoreLayoutAlignToBottom | CoreLayoutAlignToLeft | CoreLayoutAlignToRight 
  | CoreLayoutAlignToTop | CoreLayoutResizeHorz | CoreLayoutResizeVert;
  _this->FaceTintActive = _Const0003;
  _this->FaceTintFocused = _Const0003;
  _this->FaceTintDisabled = _Const0003;
  _this->FaceTintDefault = _Const0003;
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

/* 'C' function for method : 'WidgetSet::PushButtonConfig.OnSetIconTintActive()' */
void WidgetSetPushButtonConfig_OnSetIconTintActive( WidgetSetPushButtonConfig _this, 
  XColor value )
{
  if ( !EwCompColor( _this->IconTintActive, value ))
    return;

  _this->IconTintActive = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetWidgetConfig_onInvalidate ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::PushButtonConfig.OnSetIconTintDisabled()' */
void WidgetSetPushButtonConfig_OnSetIconTintDisabled( WidgetSetPushButtonConfig _this, 
  XColor value )
{
  if ( !EwCompColor( _this->IconTintDisabled, value ))
    return;

  _this->IconTintDisabled = value;
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

/* Initializer for the class 'WidgetSet::ValueDisplay' */
void WidgetSetValueDisplay__Init( WidgetSetValueDisplay _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( WidgetSetValueDisplay );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( WidgetSetValueDisplay );

  /* ... and initialize objects, variables, properties, etc. */
  _this->Super3.viewState = CoreViewStateAlphaBlended | CoreViewStateEnabled | CoreViewStateTouchable 
  | CoreViewStateVisible;
  CoreRectView__OnSetBounds( _this, _Const0004 );
  _this->CurrentFactor = 1.000000f;
}

/* Re-Initializer for the class 'WidgetSet::ValueDisplay' */
void WidgetSetValueDisplay__ReInit( WidgetSetValueDisplay _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );
}

/* Finalizer method for the class 'WidgetSet::ValueDisplay' */
void WidgetSetValueDisplay__Done( WidgetSetValueDisplay _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreGroup );

  /* Don't forget to deinitialize the super class ... */
  CoreGroup__Done( &_this->_Super );
}

/* 'C' function for method : 'WidgetSet::ValueDisplay.OnSetBounds()' */
void WidgetSetValueDisplay_OnSetBounds( WidgetSetValueDisplay _this, XRect value )
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
void WidgetSetValueDisplay_UpdateViewState( WidgetSetValueDisplay _this, XSet aState )
{
  XBool needsValue;
  XBool needsUnit;
  XRect area;

  CoreGroup_UpdateViewState((CoreGroup)_this, aState );
  needsValue = (XBool)(( _this->Appearance != 0 ) && ( _this->Appearance->ValueFont 
  != 0 ));
  needsUnit = (XBool)(( _this->Appearance != 0 ) && ( _this->Appearance->UnitFont 
  != 0 ));
  area = EwGetRectORect( _this->Super2.Bounds );

  if ( needsValue && ( _this->textView1 == 0 ))
  {
    _this->textView1 = EwNewObject( ViewsText, 0 );
    CoreGroup_Add((CoreGroup)_this, ((CoreView)_this->textView1 ), 0 );
  }
  else
    if ( !needsValue && ( _this->textView1 != 0 ))
    {
      CoreGroup_Remove((CoreGroup)_this, ((CoreView)_this->textView1 ));
      _this->textView1 = 0;
    }

  if ( needsUnit && ( _this->textView2 == 0 ))
  {
    _this->textView2 = EwNewObject( ViewsText, 0 );
    CoreGroup_Add((CoreGroup)_this, ((CoreView)_this->textView2 ), 0 );
    ViewsText_OnSetEnableBidiText( _this->textView2, 1 );
  }
  else
    if ( !needsUnit && ( _this->textView2 != 0 ))
    {
      CoreGroup_Remove((CoreGroup)_this, ((CoreView)_this->textView2 ));
      _this->textView2 = 0;
    }

  if ( _this->textView1 != 0 )
  {
    XColor clr = _Const0003;
    XRect r = area;

    if ( _this->isNegative )
      clr = _this->Appearance->ValueColorNegative;
    else
      clr = _this->Appearance->ValueColorPositive;

    if (( _this->Appearance->Layout == WidgetSetValueDisplayLayoutAlignUnitLeftToValue ) 
        || ( _this->Appearance->Layout == WidgetSetValueDisplayLayoutAlignUnitRightToValue ))
      ViewsText_OnSetOnUpdate( _this->textView1, EwNewSlot( _this, WidgetSetValueDisplay_onLayoutViews ));
    else
      ViewsText_OnSetOnUpdate( _this->textView1, EwNullSlot );

    if ((( _this->textView2 != 0 ) && ( EwCompString( _this->Unit, 0 ) != 0 )) && 
        !EwIsRectEmpty( ViewsText_GetContentArea( _this->textView2 )))
    {
      if ( _this->Appearance->Layout == WidgetSetValueDisplayLayoutAlignValueLeftToUnit )
        r.Point2.X = ViewsText_GetContentArea( _this->textView2 ).Point1.X;

      if ( _this->Appearance->Layout == WidgetSetValueDisplayLayoutAlignValueRightToUnit )
        r.Point1.X = ViewsText_GetContentArea( _this->textView2 ).Point2.X;
    }

    CoreRectView__OnSetBounds( _this->textView1, EwNewRect( r.Point1.X + _this->Appearance->ValueMarginLeft, 
    r.Point1.Y + _this->Appearance->ValueMarginTop, r.Point2.X - _this->Appearance->ValueMarginRight, 
    r.Point2.Y - _this->Appearance->ValueMarginBottom ));
    ViewsText_OnSetFont( _this->textView1, _this->Appearance->ValueFont );
    ViewsText_OnSetAlignment( _this->textView1, _this->Appearance->ValueAlignment );
    ViewsText_OnSetString( _this->textView1, _this->valueString );
    ViewsText_OnSetColor( _this->textView1, clr );
  }

  if ( _this->textView2 != 0 )
  {
    XColor clr = _Const0003;
    XRect r = area;

    if ( _this->isNegative )
      clr = _this->Appearance->UnitColorNegative;
    else
      clr = _this->Appearance->UnitColorPositive;

    if (( _this->Appearance->Layout == WidgetSetValueDisplayLayoutAlignValueLeftToUnit ) 
        || ( _this->Appearance->Layout == WidgetSetValueDisplayLayoutAlignValueRightToUnit ))
      ViewsText_OnSetOnUpdate( _this->textView2, EwNewSlot( _this, WidgetSetValueDisplay_onLayoutViews ));
    else
      ViewsText_OnSetOnUpdate( _this->textView2, EwNullSlot );

    if (( _this->textView1 != 0 ) && ( EwCompString( _this->valueString, 0 ) != 
        0 ))
    {
      if ( _this->Appearance->Layout == WidgetSetValueDisplayLayoutAlignUnitLeftToValue )
        r.Point2.X = ViewsText_GetContentArea( _this->textView1 ).Point1.X;

      if ( _this->Appearance->Layout == WidgetSetValueDisplayLayoutAlignUnitRightToValue )
        r.Point1.X = ViewsText_GetContentArea( _this->textView1 ).Point2.X;
    }

    CoreRectView__OnSetBounds( _this->textView2, EwNewRect( r.Point1.X + _this->Appearance->UnitMarginLeft, 
    r.Point1.Y + _this->Appearance->UnitMarginTop, r.Point2.X - _this->Appearance->UnitMarginRight, 
    r.Point2.Y - _this->Appearance->UnitMarginBottom ));
    ViewsText_OnSetFont( _this->textView2, _this->Appearance->UnitFont );
    ViewsText_OnSetAlignment( _this->textView2, _this->Appearance->UnitAlignment );
    ViewsText_OnSetString( _this->textView2, _this->Unit );
    ViewsText_OnSetColor( _this->textView2, clr );
  }
}

/* 'C' function for method : 'WidgetSet::ValueDisplay.onLayoutViews()' */
void WidgetSetValueDisplay_onLayoutViews( WidgetSetValueDisplay _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if (( _this->Appearance != 0 ) && ( _this->Appearance->Layout != WidgetSetValueDisplayLayoutUnitAndValueAreIndependent ))
    CoreGroup_InvalidateViewState((CoreGroup)_this );
}

/* 'C' function for method : 'WidgetSet::ValueDisplay.onFormatValue()' */
void WidgetSetValueDisplay_onFormatValue( WidgetSetValueDisplay _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( _this->Appearance != 0 )
  {
    XFloat theValue = ((XFloat)_this->CurrentValue * _this->CurrentFactor ) + _this->CurrentBias;
    XInt32 noOfDigits = _this->NoOfDigits;
    XString newValueString;
    XInt32 inx;

    if ( _this->Precision > 0 )
      noOfDigits = noOfDigits + 1;

    if ( theValue < 0.000000f )
      noOfDigits = noOfDigits + 1;

    newValueString = EwNewStringFloat( theValue, noOfDigits, _this->Precision );
    inx = EwStringFindChar( newValueString, '.', 0 );

    if (( inx >= 0 ) && ( EwCompString( _this->Appearance->FormatDecimalSign, EwLoadString( 
        &_Const0001 )) != 0 ))
      newValueString = EwConcatString( EwConcatString( EwStringLeft( newValueString, 
      inx ), _this->Appearance->FormatDecimalSign ), EwStringRemove( newValueString, 
      0, inx + 1 ));

    if ( theValue < 0.000000f )
    {
      newValueString = EwStringRemove( newValueString, 0, 1 );
      inx = inx - 1;
    }

    if ( EwCompString( _this->Appearance->FormatDigitGroupSign, 0 ) != 0 )
    {
      if ( inx < 0 )
        inx = EwGetStringLength( newValueString );

      for ( ; inx > 3; inx = inx - 3 )
        newValueString = EwConcatString( EwConcatString( EwStringLeft( newValueString, 
        inx - 3 ), _this->Appearance->FormatDigitGroupSign ), EwStringRemove( newValueString, 
        0, inx - 3 ));
    }

    if ( theValue >= 0.000000f )
      newValueString = EwConcatString( _this->Appearance->FormatPlusSign, newValueString );
    else
      newValueString = EwConcatString( _this->Appearance->FormatMinusSign, newValueString );

    if ( EwCompString( newValueString, _this->valueString ) != 0 )
    {
      _this->valueString = EwShareString( newValueString );
      _this->isNegative = (XBool)( theValue < 0.000000f );
      CoreGroup_InvalidateViewState((CoreGroup)_this );
    }
  }
  else
    if ( EwCompString( _this->valueString, 0 ) != 0 )
    {
      _this->valueString = 0;
      CoreGroup_InvalidateViewState((CoreGroup)_this );
    }
}

/* 'C' function for method : 'WidgetSet::ValueDisplay.onConfigChanged()' */
void WidgetSetValueDisplay_onConfigChanged( WidgetSetValueDisplay _this, XObject 
  sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( _this->Appearance != 0 )
    CoreRectView__OnSetBounds( _this, _this->Super2.Bounds );

  EwPostSignal( EwNewSlot( _this, WidgetSetValueDisplay_onFormatValue ), ((XObject)_this ));
  CoreGroup_InvalidateViewState((CoreGroup)_this );
}

/* 'C' function for method : 'WidgetSet::ValueDisplay.onOutlet()' */
void WidgetSetValueDisplay_onOutlet( WidgetSetValueDisplay _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( _this->Outlet.Object != 0 )
    WidgetSetValueDisplay_OnSetCurrentValue( _this, EwOnGetInt32( _this->Outlet ));
}

/* 'C' function for method : 'WidgetSet::ValueDisplay.OnSetOutlet()' */
void WidgetSetValueDisplay_OnSetOutlet( WidgetSetValueDisplay _this, XRef value )
{
  if ( !EwCompRef( _this->Outlet, value ))
    return;

  if ( _this->Outlet.Object != 0 )
    EwDetachRefObserver( EwNewSlot( _this, WidgetSetValueDisplay_onOutlet ), _this->Outlet, 
      0 );

  _this->Outlet = value;

  if ( value.Object != 0 )
    EwAttachRefObserver( EwNewSlot( _this, WidgetSetValueDisplay_onOutlet ), value, 
      0 );

  if ( value.Object != 0 )
    EwPostSignal( EwNewSlot( _this, WidgetSetValueDisplay_onOutlet ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::ValueDisplay.OnSetUnit()' */
void WidgetSetValueDisplay_OnSetUnit( WidgetSetValueDisplay _this, XString value )
{
  if ( !EwCompString( _this->Unit, value ))
    return;

  _this->Unit = EwShareString( value );
  CoreGroup_InvalidateViewState((CoreGroup)_this );
}

/* 'C' function for method : 'WidgetSet::ValueDisplay.OnSetNoOfDigits()' */
void WidgetSetValueDisplay_OnSetNoOfDigits( WidgetSetValueDisplay _this, XInt32 
  value )
{
  if ( _this->NoOfDigits == value )
    return;

  _this->NoOfDigits = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetValueDisplay_onFormatValue ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::ValueDisplay.OnSetCurrentFactor()' */
void WidgetSetValueDisplay_OnSetCurrentFactor( WidgetSetValueDisplay _this, XFloat 
  value )
{
  if ( _this->CurrentFactor == value )
    return;

  _this->CurrentFactor = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetValueDisplay_onFormatValue ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::ValueDisplay.OnSetCurrentValue()' */
void WidgetSetValueDisplay_OnSetCurrentValue( WidgetSetValueDisplay _this, XInt32 
  value )
{
  if ( _this->CurrentValue == value )
    return;

  _this->CurrentValue = value;
  EwPostSignal( EwNewSlot( _this, WidgetSetValueDisplay_onFormatValue ), ((XObject)_this ));
}

/* 'C' function for method : 'WidgetSet::ValueDisplay.OnSetAppearance()' */
void WidgetSetValueDisplay_OnSetAppearance( WidgetSetValueDisplay _this, WidgetSetValueDisplayConfig 
  value )
{
  if ( _this->Appearance == value )
    return;

  if ( _this->Appearance != 0 )
    EwDetachObjObserver( EwNewSlot( _this, WidgetSetValueDisplay_onConfigChanged ), 
      (XObject)_this->Appearance, 0 );

  _this->Appearance = value;

  if ( value != 0 )
    EwAttachObjObserver( EwNewSlot( _this, WidgetSetValueDisplay_onConfigChanged ), 
      (XObject)value, 0 );

  EwPostSignal( EwNewSlot( _this, WidgetSetValueDisplay_onConfigChanged ), ((XObject)_this ));
}

/* Variants derived from the class : 'WidgetSet::ValueDisplay' */
EW_DEFINE_CLASS_VARIANTS( WidgetSetValueDisplay )
EW_END_OF_CLASS_VARIANTS( WidgetSetValueDisplay )

/* Virtual Method Table (VMT) for the class : 'WidgetSet::ValueDisplay' */
EW_DEFINE_CLASS( WidgetSetValueDisplay, CoreGroup, textView2, Outlet, Outlet, valueString, 
                 valueString, Precision, "WidgetSet::ValueDisplay" )
  CoreRectView_initLayoutContext,
  CoreView_GetRoot,
  CoreGroup_Draw,
  CoreView_HandleEvent,
  CoreGroup_CursorHitTest,
  CoreRectView_ArrangeView,
  CoreRectView_MoveView,
  CoreRectView_GetExtent,
  CoreGroup_ChangeViewState,
  WidgetSetValueDisplay_OnSetBounds,
  CoreGroup_OnSetFocus,
  CoreGroup_OnSetBuffered,
  CoreGroup_OnSetOpacity,
  CoreGroup_IsDialog,
  CoreGroup_DispatchEvent,
  CoreGroup_BroadcastEvent,
  WidgetSetValueDisplay_UpdateViewState,
  CoreGroup_InvalidateArea,
EW_END_OF_CLASS( WidgetSetValueDisplay )

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
  CoreRectView__OnSetBounds( _this, _Const0005 );
  CoreTimer_OnSetPeriod( &_this->FlashTimer, 0 );
  CoreTimer_OnSetBegin( &_this->FlashTimer, 50 );
  _this->KeyHandler.Filter = CoreKeyCodeEnter;
  CoreView_OnSetLayout((CoreView)&_this->TouchHandler, CoreLayoutAlignToBottom | 
  CoreLayoutAlignToLeft | CoreLayoutAlignToRight | CoreLayoutAlignToTop | CoreLayoutResizeHorz 
  | CoreLayoutResizeVert );
  CoreQuadView__OnSetPoint4( &_this->TouchHandler, _Const0006 );
  CoreQuadView__OnSetPoint3( &_this->TouchHandler, _Const0007 );
  CoreQuadView__OnSetPoint2( &_this->TouchHandler, _Const0008 );
  CoreQuadView__OnSetPoint1( &_this->TouchHandler, _Const0009 );
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
    XColor tint = _Const0003;
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
    XColor clr = _Const0003;

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
      ViewsImage_OnSetColor( _this->imageView, _Const0003 );
    }

  if ( _this->textView != 0 )
  {
    XColor clr = _Const0003;
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

/* 'C' function for method : 'WidgetSet::ToggleButton.OnSetIconOffFrame()' */
void WidgetSetToggleButton_OnSetIconOffFrame( WidgetSetToggleButton _this, XInt32 
  value )
{
  if (((( value == _this->IconOffFrameDefault ) && ( value == _this->IconOffFrameDisabled )) 
      && ( value == _this->IconOffFrameFocused )) && ( value == _this->IconOffFrameActive ))
    return;

  _this->IconOffFrameDefault = value;
  _this->IconOffFrameDisabled = value;
  _this->IconOffFrameFocused = value;
  _this->IconOffFrameActive = value;
  CoreGroup_InvalidateViewState((CoreGroup)_this );
}

/* 'C' function for method : 'WidgetSet::ToggleButton.OnSetIconFrame()' */
void WidgetSetToggleButton_OnSetIconFrame( WidgetSetToggleButton _this, XInt32 value )
{
  if (((((((( value == _this->IconOffFrameDefault ) && ( value == _this->IconOffFrameDisabled )) 
      && ( value == _this->IconOffFrameFocused )) && ( value == _this->IconOffFrameActive )) 
      && ( value == _this->IconOnFrameDefault )) && ( value == _this->IconOnFrameDisabled )) 
      && ( value == _this->IconOnFrameFocused )) && ( value == _this->IconOnFrameActive ))
    return;

  _this->IconOffFrameDefault = value;
  _this->IconOffFrameDisabled = value;
  _this->IconOffFrameFocused = value;
  _this->IconOffFrameActive = value;
  _this->IconOnFrameDefault = value;
  _this->IconOnFrameDisabled = value;
  _this->IconOnFrameFocused = value;
  _this->IconOnFrameActive = value;
  CoreGroup_InvalidateViewState((CoreGroup)_this );
}

/* 'C' function for method : 'WidgetSet::ToggleButton.OnSetIcon()' */
void WidgetSetToggleButton_OnSetIcon( WidgetSetToggleButton _this, ResourcesBitmap 
  value )
{
  if ( _this->Icon == value )
    return;

  _this->Icon = value;
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
  CoreGroup_OnSetBuffered,
  CoreGroup_OnSetOpacity,
  CoreGroup_IsDialog,
  CoreGroup_DispatchEvent,
  CoreGroup_BroadcastEvent,
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
  CoreRectView__OnSetBounds( _this, _Const0005 );
  CoreTimer_OnSetPeriod( &_this->FlashTimer, 0 );
  CoreTimer_OnSetBegin( &_this->FlashTimer, 50 );
  _this->KeyHandler.Filter = CoreKeyCodeEnter;
  CoreView_OnSetLayout((CoreView)&_this->TouchHandler, CoreLayoutAlignToBottom | 
  CoreLayoutAlignToLeft | CoreLayoutAlignToRight | CoreLayoutAlignToTop | CoreLayoutResizeHorz 
  | CoreLayoutResizeVert );
  CoreQuadView__OnSetPoint4( &_this->TouchHandler, _Const0006 );
  CoreQuadView__OnSetPoint3( &_this->TouchHandler, _Const0007 );
  CoreQuadView__OnSetPoint2( &_this->TouchHandler, _Const0008 );
  CoreQuadView__OnSetPoint1( &_this->TouchHandler, _Const0009 );
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
    XColor tint = _Const0003;
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
    XColor clr = _Const0003;

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
      ViewsImage_OnSetColor( _this->imageView, _Const0003 );
    }

  if ( _this->textView != 0 )
  {
    XColor clr = _Const0003;

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
  CoreGroup_IsDialog,
  CoreGroup_DispatchEvent,
  CoreGroup_BroadcastEvent,
  WidgetSetPushButton_UpdateViewState,
  CoreGroup_InvalidateArea,
EW_END_OF_CLASS( WidgetSetPushButton )

/* Embedded Wizard */
