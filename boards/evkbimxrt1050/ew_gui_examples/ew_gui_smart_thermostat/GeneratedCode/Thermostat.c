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
#include "_CorePropertyObserver.h"
#include "_CoreRotateTouchHandler.h"
#include "_CoreSimpleTouchHandler.h"
#include "_CoreTimer.h"
#include "_CoreView.h"
#include "_EffectsInt32Effect.h"
#include "_GraphicsPath.h"
#include "_ResourcesBitmap.h"
#include "_ResourcesFont.h"
#include "_ThermostatDeviceClass.h"
#include "_ThermostatTemperatureController.h"
#include "_ThermostatThermostat.h"
#include "_ViewsImage.h"
#include "_ViewsStrokePath.h"
#include "_ViewsText.h"
#include "_ViewsWallpaper.h"
#include "_ViewsWarpImage.h"
#include "_WidgetSetRadioButton.h"
#include "_WidgetSetRadioButtonConfig.h"
#include "_WidgetSetToggleButton.h"
#include "_WidgetSetToggleButtonConfig.h"
#include "_WidgetSetVerticalSlider.h"
#include "_WidgetSetVerticalSliderConfig.h"
#include "Core.h"
#include "Thermostat.h"
#include "Views.h"

/* Compressed strings for the language 'Default'. */
static const unsigned int _StringsDefault0[] =
{
  0x0000009A, /* ratio 77.92 % */
  0xB8001300, 0x00064452, 0x1C5C0018, 0xC00B0004, 0x09100010, 0x10A22C0C, 0x0D898162,
  0x3054421F, 0x001861F1, 0x01D00070, 0x1BC00690, 0x02206E00, 0x08913030, 0x54798F80,
  0x84E89822, 0x00338024, 0xB95488D0, 0x9795A260, 0x1D00C600, 0x4A64F259, 0x32260E88,
  0xC0049001, 0x8F35A6CF, 0x40010000, 0x5727800A, 0x39913401, 0xA640CA71, 0x1CAAA200,
  0x0000101B, 0x00000000
};

/* Constant values used in this 'C' module only. */
static const XPoint _Const0000 = { 46, 27 };
static const XColor _Const0001 = { 0xFF, 0xFF, 0xFF, 0xFF };
static const XPoint _Const0002 = { 270, 27 };
static const XPoint _Const0003 = { 27, 27 };
static const XPoint _Const0004 = { 16, 160 };
static const XRect _Const0005 = {{ 0, 0 }, { 210, 210 }};
static const XRect _Const0006 = {{ 10, 10 }, { 200, 200 }};
static const XPoint _Const0007 = { 161, 49 };
static const XPoint _Const0008 = { 175, 63 };
static const XPoint _Const0009 = { 184, 54 };
static const XPoint _Const000A = { 170, 40 };
static const XPoint _Const000B = { 10, 80 };
static const XRect _Const000C = {{ 0, 0 }, { 340, 340 }};
static const XColor _Const000D = { 0x00, 0x00, 0xFF, 0xFF };
static const XRect _Const000E = {{ 90, 156 }, { 120, 186 }};
static const XRect _Const000F = {{ 40, 80 }, { 160, 130 }};
static const XStringRes _Const0010 = { _StringsDefault0, 0x0002 };
static const XColor _Const0011 = { 0x00, 0x00, 0x00, 0xFF };
static const XPoint _Const0012 = { 80, 92 };
static const XPoint _Const0013 = { 128, 92 };
static const XPoint _Const0014 = { 128, 44 };
static const XPoint _Const0015 = { 80, 44 };
static const XPoint _Const0016 = { 80, 167 };
static const XPoint _Const0017 = { 129, 167 };
static const XPoint _Const0018 = { 129, 120 };
static const XPoint _Const0019 = { 80, 120 };
static const XPoint _Const001A = { 0, 0 };
static const XStringRes _Const001B = { _StringsDefault0, 0x000B };
static const XStringRes _Const001C = { _StringsDefault0, 0x000F };
static const XRect _Const001D = {{ 0, 0 }, { 480, 272 }};
static const XRect _Const001E = {{ 409, 210 }, { 455, 237 }};
static const XStringRes _Const001F = { _StringsDefault0, 0x0014 };
static const XRect _Const0020 = {{ 10, 80 }, { 130, 107 }};
static const XStringRes _Const0021 = { _StringsDefault0, 0x001E };
static const XRect _Const0022 = {{ 10, 120 }, { 130, 147 }};
static const XStringRes _Const0023 = { _StringsDefault0, 0x0024 };
static const XRect _Const0024 = {{ 10, 160 }, { 130, 187 }};
static const XStringRes _Const0025 = { _StringsDefault0, 0x002C };
static const XRect _Const0026 = {{ 438, 30 }, { 454, 190 }};
static const XRect _Const0027 = {{ 398, 30 }, { 428, 60 }};
static const XRect _Const0028 = {{ 402, 164 }, { 432, 194 }};
static const XRect _Const0029 = {{ 140, 30 }, { 350, 240 }};
static const XRect _Const002A = {{ 10, 0 }, { 180, 39 }};
static const XStringRes _Const002B = { _StringsDefault0, 0x0037 };
static const XRect _Const002C = {{ 400, 240 }, { 469, 265 }};
static const XStringRes _Const002D = { _StringsDefault0, 0x0045 };
static const XRect _Const002E = {{ 0, 230 }, { 100, 270 }};

/* User defined auto object: 'Thermostat::SwingSwitchConfig' */
EW_DEFINE_AUTOOBJECT( ThermostatSwingSwitchConfig, WidgetSetToggleButtonConfig )

/* Initializer for the auto object 'Thermostat::SwingSwitchConfig' */
void ThermostatSwingSwitchConfig__Init( WidgetSetToggleButtonConfig _this )
{
  WidgetSetToggleButtonConfig_OnSetFaceOnFrameActive( _this, 3 );
  WidgetSetToggleButtonConfig_OnSetFaceOnFrameFocused( _this, 1 );
  WidgetSetToggleButtonConfig_OnSetFaceOnFrameDisabled( _this, 1 );
  WidgetSetToggleButtonConfig_OnSetFaceOnFrameDefault( _this, 1 );
  WidgetSetToggleButtonConfig_OnSetFaceOnActive( _this, EwLoadResource( &ThermostatSwingSwitch, 
  ResourcesBitmap ));
  WidgetSetToggleButtonConfig_OnSetFaceOnFocused( _this, EwLoadResource( &ThermostatSwingSwitch, 
  ResourcesBitmap ));
  WidgetSetToggleButtonConfig_OnSetFaceOnDisabled( _this, EwLoadResource( &ThermostatSwingSwitch, 
  ResourcesBitmap ));
  WidgetSetToggleButtonConfig_OnSetFaceOnDefault( _this, EwLoadResource( &ThermostatSwingSwitch, 
  ResourcesBitmap ));
  WidgetSetToggleButtonConfig_OnSetFaceOffFrameActive( _this, 2 );
  WidgetSetToggleButtonConfig_OnSetFaceOffFrameFocused( _this, 0 );
  WidgetSetToggleButtonConfig_OnSetFaceOffFrameDisabled( _this, 0 );
  WidgetSetToggleButtonConfig_OnSetFaceOffFrameDefault( _this, 0 );
  WidgetSetToggleButtonConfig_OnSetFaceOffActive( _this, EwLoadResource( &ThermostatSwingSwitch, 
  ResourcesBitmap ));
  WidgetSetToggleButtonConfig_OnSetFaceOffFocused( _this, EwLoadResource( &ThermostatSwingSwitch, 
  ResourcesBitmap ));
  WidgetSetToggleButtonConfig_OnSetFaceOffDisabled( _this, EwLoadResource( &ThermostatSwingSwitch, 
  ResourcesBitmap ));
  WidgetSetToggleButtonConfig_OnSetFaceOffDefault( _this, EwLoadResource( &ThermostatSwingSwitch, 
  ResourcesBitmap ));
  WidgetSetToggleButtonConfig_OnSetWidgetMaxSize( _this, _Const0000 );
  WidgetSetToggleButtonConfig_OnSetWidgetMinSize( _this, _Const0000 );
}

/* Table with links to derived variants of the auto object : 'Thermostat::SwingSwitchConfig' */
EW_DEFINE_AUTOOBJECT_VARIANTS( ThermostatSwingSwitchConfig )
EW_END_OF_AUTOOBJECT_VARIANTS( ThermostatSwingSwitchConfig )

/* Include a file containing the bitmap resource : 'Thermostat::SwingSwitch' */
#include "_ThermostatSwingSwitch.h"

/* Table with links to derived variants of the bitmap resource : 'Thermostat::SwingSwitch' */
EW_RES_WITHOUT_VARIANTS( ThermostatSwingSwitch )

/* Include a file containing the bitmap resource : 'Thermostat::ButtonDay' */
#include "_ThermostatButtonDay.h"

/* Table with links to derived variants of the bitmap resource : 'Thermostat::ButtonDay' */
EW_RES_WITHOUT_VARIANTS( ThermostatButtonDay )

/* Include a file containing the bitmap resource : 'Thermostat::ButtonNight' */
#include "_ThermostatButtonNight.h"

/* Table with links to derived variants of the bitmap resource : 'Thermostat::ButtonNight' */
EW_RES_WITHOUT_VARIANTS( ThermostatButtonNight )

/* Include a file containing the bitmap resource : 'Thermostat::ButtonVacation' */
#include "_ThermostatButtonVacation.h"

/* Table with links to derived variants of the bitmap resource : 'Thermostat::ButtonVacation' */
EW_RES_WITHOUT_VARIANTS( ThermostatButtonVacation )

/* User defined auto object: 'Thermostat::RadioButtonDayConfig' */
EW_DEFINE_AUTOOBJECT( ThermostatRadioButtonDayConfig, WidgetSetRadioButtonConfig )

/* Initializer for the auto object 'Thermostat::RadioButtonDayConfig' */
void ThermostatRadioButtonDayConfig__Init( WidgetSetRadioButtonConfig _this )
{
  WidgetSetRadioButtonConfig_OnSetLabelOnColorActive( _this, _Const0001 );
  WidgetSetRadioButtonConfig_OnSetLabelOnColorFocused( _this, _Const0001 );
  WidgetSetRadioButtonConfig_OnSetLabelOnColorDefault( _this, _Const0001 );
  WidgetSetRadioButtonConfig_OnSetLabelOffColorActive( _this, _Const0001 );
  WidgetSetRadioButtonConfig_OnSetLabelOffColorFocused( _this, _Const0001 );
  WidgetSetRadioButtonConfig_OnSetLabelOffColorDefault( _this, _Const0001 );
  WidgetSetRadioButtonConfig_OnSetLabelMarginLeft( _this, 36 );
  WidgetSetRadioButtonConfig_OnSetLabelAlignment( _this, ViewsTextAlignmentAlignHorzLeft 
  | ViewsTextAlignmentAlignVertCenter );
  WidgetSetRadioButtonConfig_OnSetLabelOnFont( _this, EwLoadResource( &ThermostatFontL, 
  ResourcesFont ));
  WidgetSetRadioButtonConfig_OnSetLabelOffFont( _this, EwLoadResource( &ThermostatFontL, 
  ResourcesFont ));
  WidgetSetRadioButtonConfig_OnSetFaceLayout( _this, CoreLayoutAlignToBottom | CoreLayoutAlignToLeft 
  | CoreLayoutAlignToTop );
  WidgetSetRadioButtonConfig_OnSetFaceOnFrameActive( _this, 1 );
  WidgetSetRadioButtonConfig_OnSetFaceOnFrameFocused( _this, 1 );
  WidgetSetRadioButtonConfig_OnSetFaceOnFrameDisabled( _this, 1 );
  WidgetSetRadioButtonConfig_OnSetFaceOnFrameDefault( _this, 1 );
  WidgetSetRadioButtonConfig_OnSetFaceOnActive( _this, EwLoadResource( &ThermostatButtonDay, 
  ResourcesBitmap ));
  WidgetSetRadioButtonConfig_OnSetFaceOnFocused( _this, EwLoadResource( &ThermostatButtonDay, 
  ResourcesBitmap ));
  WidgetSetRadioButtonConfig_OnSetFaceOnDisabled( _this, EwLoadResource( &ThermostatButtonDay, 
  ResourcesBitmap ));
  WidgetSetRadioButtonConfig_OnSetFaceOnDefault( _this, EwLoadResource( &ThermostatButtonDay, 
  ResourcesBitmap ));
  WidgetSetRadioButtonConfig_OnSetFaceOffFrameActive( _this, 0 );
  WidgetSetRadioButtonConfig_OnSetFaceOffFrameFocused( _this, 0 );
  WidgetSetRadioButtonConfig_OnSetFaceOffFrameDisabled( _this, 0 );
  WidgetSetRadioButtonConfig_OnSetFaceOffFrameDefault( _this, 0 );
  WidgetSetRadioButtonConfig_OnSetFaceOffActive( _this, EwLoadResource( &ThermostatButtonDay, 
  ResourcesBitmap ));
  WidgetSetRadioButtonConfig_OnSetFaceOffFocused( _this, EwLoadResource( &ThermostatButtonDay, 
  ResourcesBitmap ));
  WidgetSetRadioButtonConfig_OnSetFaceOffDisabled( _this, EwLoadResource( &ThermostatButtonDay, 
  ResourcesBitmap ));
  WidgetSetRadioButtonConfig_OnSetFaceOffDefault( _this, EwLoadResource( &ThermostatButtonDay, 
  ResourcesBitmap ));
  WidgetSetRadioButtonConfig_OnSetWidgetMaxSize( _this, _Const0002 );
  WidgetSetRadioButtonConfig_OnSetWidgetMinSize( _this, _Const0003 );
}

/* Table with links to derived variants of the auto object : 'Thermostat::RadioButtonDayConfig' */
EW_DEFINE_AUTOOBJECT_VARIANTS( ThermostatRadioButtonDayConfig )
EW_END_OF_AUTOOBJECT_VARIANTS( ThermostatRadioButtonDayConfig )

/* User defined auto object: 'Thermostat::RadioButtonNightConfig' */
EW_DEFINE_AUTOOBJECT( ThermostatRadioButtonNightConfig, WidgetSetRadioButtonConfig )

/* Initializer for the auto object 'Thermostat::RadioButtonNightConfig' */
void ThermostatRadioButtonNightConfig__Init( WidgetSetRadioButtonConfig _this )
{
  WidgetSetRadioButtonConfig_OnSetLabelOnColorActive( _this, _Const0001 );
  WidgetSetRadioButtonConfig_OnSetLabelOnColorFocused( _this, _Const0001 );
  WidgetSetRadioButtonConfig_OnSetLabelOnColorDefault( _this, _Const0001 );
  WidgetSetRadioButtonConfig_OnSetLabelOffColorActive( _this, _Const0001 );
  WidgetSetRadioButtonConfig_OnSetLabelOffColorFocused( _this, _Const0001 );
  WidgetSetRadioButtonConfig_OnSetLabelOffColorDefault( _this, _Const0001 );
  WidgetSetRadioButtonConfig_OnSetLabelMarginLeft( _this, 36 );
  WidgetSetRadioButtonConfig_OnSetLabelAlignment( _this, ViewsTextAlignmentAlignHorzLeft 
  | ViewsTextAlignmentAlignVertCenter );
  WidgetSetRadioButtonConfig_OnSetLabelOnFont( _this, EwLoadResource( &ThermostatFontL, 
  ResourcesFont ));
  WidgetSetRadioButtonConfig_OnSetLabelOffFont( _this, EwLoadResource( &ThermostatFontL, 
  ResourcesFont ));
  WidgetSetRadioButtonConfig_OnSetFaceLayout( _this, CoreLayoutAlignToBottom | CoreLayoutAlignToLeft 
  | CoreLayoutAlignToTop );
  WidgetSetRadioButtonConfig_OnSetFaceOnFrameActive( _this, 1 );
  WidgetSetRadioButtonConfig_OnSetFaceOnFrameFocused( _this, 1 );
  WidgetSetRadioButtonConfig_OnSetFaceOnFrameDisabled( _this, 1 );
  WidgetSetRadioButtonConfig_OnSetFaceOnFrameDefault( _this, 1 );
  WidgetSetRadioButtonConfig_OnSetFaceOnActive( _this, EwLoadResource( &ThermostatButtonNight, 
  ResourcesBitmap ));
  WidgetSetRadioButtonConfig_OnSetFaceOnFocused( _this, EwLoadResource( &ThermostatButtonNight, 
  ResourcesBitmap ));
  WidgetSetRadioButtonConfig_OnSetFaceOnDisabled( _this, EwLoadResource( &ThermostatButtonNight, 
  ResourcesBitmap ));
  WidgetSetRadioButtonConfig_OnSetFaceOnDefault( _this, EwLoadResource( &ThermostatButtonNight, 
  ResourcesBitmap ));
  WidgetSetRadioButtonConfig_OnSetFaceOffFrameActive( _this, 0 );
  WidgetSetRadioButtonConfig_OnSetFaceOffFrameFocused( _this, 0 );
  WidgetSetRadioButtonConfig_OnSetFaceOffFrameDisabled( _this, 0 );
  WidgetSetRadioButtonConfig_OnSetFaceOffFrameDefault( _this, 0 );
  WidgetSetRadioButtonConfig_OnSetFaceOffActive( _this, EwLoadResource( &ThermostatButtonNight, 
  ResourcesBitmap ));
  WidgetSetRadioButtonConfig_OnSetFaceOffFocused( _this, EwLoadResource( &ThermostatButtonNight, 
  ResourcesBitmap ));
  WidgetSetRadioButtonConfig_OnSetFaceOffDisabled( _this, EwLoadResource( &ThermostatButtonNight, 
  ResourcesBitmap ));
  WidgetSetRadioButtonConfig_OnSetFaceOffDefault( _this, EwLoadResource( &ThermostatButtonNight, 
  ResourcesBitmap ));
  WidgetSetRadioButtonConfig_OnSetWidgetMaxSize( _this, _Const0002 );
  WidgetSetRadioButtonConfig_OnSetWidgetMinSize( _this, _Const0003 );
}

/* Table with links to derived variants of the auto object : 'Thermostat::RadioButtonNightConfig' */
EW_DEFINE_AUTOOBJECT_VARIANTS( ThermostatRadioButtonNightConfig )
EW_END_OF_AUTOOBJECT_VARIANTS( ThermostatRadioButtonNightConfig )

/* User defined auto object: 'Thermostat::RadioButtonVacationConfig' */
EW_DEFINE_AUTOOBJECT( ThermostatRadioButtonVacationConfig, WidgetSetRadioButtonConfig )

/* Initializer for the auto object 'Thermostat::RadioButtonVacationConfig' */
void ThermostatRadioButtonVacationConfig__Init( WidgetSetRadioButtonConfig _this )
{
  WidgetSetRadioButtonConfig_OnSetLabelOnColorActive( _this, _Const0001 );
  WidgetSetRadioButtonConfig_OnSetLabelOnColorFocused( _this, _Const0001 );
  WidgetSetRadioButtonConfig_OnSetLabelOnColorDefault( _this, _Const0001 );
  WidgetSetRadioButtonConfig_OnSetLabelOffColorActive( _this, _Const0001 );
  WidgetSetRadioButtonConfig_OnSetLabelOffColorFocused( _this, _Const0001 );
  WidgetSetRadioButtonConfig_OnSetLabelOffColorDefault( _this, _Const0001 );
  WidgetSetRadioButtonConfig_OnSetLabelMarginLeft( _this, 36 );
  WidgetSetRadioButtonConfig_OnSetLabelAlignment( _this, ViewsTextAlignmentAlignHorzLeft 
  | ViewsTextAlignmentAlignVertCenter );
  WidgetSetRadioButtonConfig_OnSetLabelOnFont( _this, EwLoadResource( &ThermostatFontL, 
  ResourcesFont ));
  WidgetSetRadioButtonConfig_OnSetLabelOffFont( _this, EwLoadResource( &ThermostatFontL, 
  ResourcesFont ));
  WidgetSetRadioButtonConfig_OnSetFaceLayout( _this, CoreLayoutAlignToBottom | CoreLayoutAlignToLeft 
  | CoreLayoutAlignToTop );
  WidgetSetRadioButtonConfig_OnSetFaceOnFrameActive( _this, 1 );
  WidgetSetRadioButtonConfig_OnSetFaceOnFrameFocused( _this, 1 );
  WidgetSetRadioButtonConfig_OnSetFaceOnFrameDisabled( _this, 1 );
  WidgetSetRadioButtonConfig_OnSetFaceOnFrameDefault( _this, 1 );
  WidgetSetRadioButtonConfig_OnSetFaceOnActive( _this, EwLoadResource( &ThermostatButtonVacation, 
  ResourcesBitmap ));
  WidgetSetRadioButtonConfig_OnSetFaceOnFocused( _this, EwLoadResource( &ThermostatButtonVacation, 
  ResourcesBitmap ));
  WidgetSetRadioButtonConfig_OnSetFaceOnDisabled( _this, EwLoadResource( &ThermostatButtonVacation, 
  ResourcesBitmap ));
  WidgetSetRadioButtonConfig_OnSetFaceOnDefault( _this, EwLoadResource( &ThermostatButtonVacation, 
  ResourcesBitmap ));
  WidgetSetRadioButtonConfig_OnSetFaceOffFrameActive( _this, 0 );
  WidgetSetRadioButtonConfig_OnSetFaceOffFrameFocused( _this, 0 );
  WidgetSetRadioButtonConfig_OnSetFaceOffFrameDisabled( _this, 0 );
  WidgetSetRadioButtonConfig_OnSetFaceOffFrameDefault( _this, 0 );
  WidgetSetRadioButtonConfig_OnSetFaceOffActive( _this, EwLoadResource( &ThermostatButtonVacation, 
  ResourcesBitmap ));
  WidgetSetRadioButtonConfig_OnSetFaceOffFocused( _this, EwLoadResource( &ThermostatButtonVacation, 
  ResourcesBitmap ));
  WidgetSetRadioButtonConfig_OnSetFaceOffDisabled( _this, EwLoadResource( &ThermostatButtonVacation, 
  ResourcesBitmap ));
  WidgetSetRadioButtonConfig_OnSetFaceOffDefault( _this, EwLoadResource( &ThermostatButtonVacation, 
  ResourcesBitmap ));
  WidgetSetRadioButtonConfig_OnSetWidgetMaxSize( _this, _Const0002 );
  WidgetSetRadioButtonConfig_OnSetWidgetMinSize( _this, _Const0003 );
}

/* Table with links to derived variants of the auto object : 'Thermostat::RadioButtonVacationConfig' */
EW_DEFINE_AUTOOBJECT_VARIANTS( ThermostatRadioButtonVacationConfig )
EW_END_OF_AUTOOBJECT_VARIANTS( ThermostatRadioButtonVacationConfig )

/* Include a file containing the font resource : 'Thermostat::FontL' */
#include "_ThermostatFontL.h"

/* Table with links to derived variants of the font resource : 'Thermostat::FontL' */
EW_RES_WITHOUT_VARIANTS( ThermostatFontL )

/* Include a file containing the bitmap resource : 'Thermostat::VertBar' */
#include "_ThermostatVertBar.h"

/* Table with links to derived variants of the bitmap resource : 'Thermostat::VertBar' */
EW_RES_WITHOUT_VARIANTS( ThermostatVertBar )

/* User defined auto object: 'Thermostat::VerticalSliderConfig' */
EW_DEFINE_AUTOOBJECT( ThermostatVerticalSliderConfig, WidgetSetVerticalSliderConfig )

/* Initializer for the auto object 'Thermostat::VerticalSliderConfig' */
void ThermostatVerticalSliderConfig__Init( WidgetSetVerticalSliderConfig _this )
{
  WidgetSetVerticalSliderConfig_OnSetTrackAboveFrameActive( _this, 0 );
  WidgetSetVerticalSliderConfig_OnSetTrackAboveFrameFocused( _this, 0 );
  WidgetSetVerticalSliderConfig_OnSetTrackAboveFrameDisabled( _this, 0 );
  WidgetSetVerticalSliderConfig_OnSetTrackAboveFrameDefault( _this, 0 );
  WidgetSetVerticalSliderConfig_OnSetTrackAboveActive( _this, EwLoadResource( &ThermostatVertBar, 
  ResourcesBitmap ));
  WidgetSetVerticalSliderConfig_OnSetTrackAboveFocused( _this, EwLoadResource( &ThermostatVertBar, 
  ResourcesBitmap ));
  WidgetSetVerticalSliderConfig_OnSetTrackAboveDisabled( _this, EwLoadResource( 
  &ThermostatVertBar, ResourcesBitmap ));
  WidgetSetVerticalSliderConfig_OnSetTrackAboveDefault( _this, EwLoadResource( &ThermostatVertBar, 
  ResourcesBitmap ));
  WidgetSetVerticalSliderConfig_OnSetTrackBelowFrameActive( _this, 1 );
  WidgetSetVerticalSliderConfig_OnSetTrackBelowFrameFocused( _this, 1 );
  WidgetSetVerticalSliderConfig_OnSetTrackBelowFrameDisabled( _this, 1 );
  WidgetSetVerticalSliderConfig_OnSetTrackBelowFrameDefault( _this, 1 );
  WidgetSetVerticalSliderConfig_OnSetTrackBelowActive( _this, EwLoadResource( &ThermostatVertBar, 
  ResourcesBitmap ));
  WidgetSetVerticalSliderConfig_OnSetTrackBelowFocused( _this, EwLoadResource( &ThermostatVertBar, 
  ResourcesBitmap ));
  WidgetSetVerticalSliderConfig_OnSetTrackBelowDisabled( _this, EwLoadResource( 
  &ThermostatVertBar, ResourcesBitmap ));
  WidgetSetVerticalSliderConfig_OnSetTrackBelowDefault( _this, EwLoadResource( &ThermostatVertBar, 
  ResourcesBitmap ));
  WidgetSetVerticalSliderConfig_OnSetWidgetMaxSize( _this, _Const0004 );
  WidgetSetVerticalSliderConfig_OnSetWidgetMinSize( _this, _Const0004 );
}

/* Table with links to derived variants of the auto object : 'Thermostat::VerticalSliderConfig' */
EW_DEFINE_AUTOOBJECT_VARIANTS( ThermostatVerticalSliderConfig )
EW_END_OF_AUTOOBJECT_VARIANTS( ThermostatVerticalSliderConfig )

/* Initializer for the class 'Thermostat::DeviceClass' */
void ThermostatDeviceClass__Init( ThermostatDeviceClass _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  TemplatesDeviceClass__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( ThermostatDeviceClass );

  /* ... then construct all embedded objects */
  CoreTimer__Init( &_this->ChangeTempTimer, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( ThermostatDeviceClass );

  /* ... and initialize objects, variables, properties, etc. */
  _this->ActualTemp = 155;
  _this->NominalTemp = 215;
  _this->FanSpeed = 30;
  CoreTimer_OnSetPeriod( &_this->ChangeTempTimer, 150 );
  CoreTimer_OnSetEnabled( &_this->ChangeTempTimer, 1 );
  _this->ChangeTempTimer.OnTrigger = EwNewSlot( _this, ThermostatDeviceClass_ChangeActualTemp );
}

/* Re-Initializer for the class 'Thermostat::DeviceClass' */
void ThermostatDeviceClass__ReInit( ThermostatDeviceClass _this )
{
  /* At first re-initialize the super class ... */
  TemplatesDeviceClass__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  CoreTimer__ReInit( &_this->ChangeTempTimer );
}

/* Finalizer method for the class 'Thermostat::DeviceClass' */
void ThermostatDeviceClass__Done( ThermostatDeviceClass _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( TemplatesDeviceClass );

  /* Finalize all embedded objects */
  CoreTimer__Done( &_this->ChangeTempTimer );

  /* Don't forget to deinitialize the super class ... */
  TemplatesDeviceClass__Done( &_this->_Super );
}

/* This method is intended to be called by the device to notify the GUI application 
   about an alternation of the property 'ActualTemp'. */
void ThermostatDeviceClass_UpdateActualTemp( ThermostatDeviceClass _this, XInt32 
  aNewValue )
{
  if ( aNewValue != _this->ActualTemp )
  {
    _this->ActualTemp = aNewValue;
    EwNotifyRefObservers( EwNewRef( _this, ThermostatDeviceClass_OnGetActualTemp, 
      ThermostatDeviceClass_OnSetActualTemp ), 0 );
  }
}

/* Wrapper function for the non virtual method : 'Thermostat::DeviceClass.UpdateActualTemp()' */
void ThermostatDeviceClass__UpdateActualTemp( void* _this, XInt32 aNewValue )
{
  ThermostatDeviceClass_UpdateActualTemp((ThermostatDeviceClass)_this, aNewValue );
}

/* 'C' function for method : 'Thermostat::DeviceClass.OnSetNominalTemp()' */
void ThermostatDeviceClass_OnSetNominalTemp( ThermostatDeviceClass _this, XInt32 
  value )
{
  if ( _this->NominalTemp == value )
    return;

  _this->NominalTemp = value;
  {
    /*
       TO DO:

       You can call a function of your own device API or you simply
       modify a variable existing in your middleware to reflect the
       new value:

       YourDevice_SetSomeValue( value );

       or

       YourDevice_SomeVariable = value;
    */
  }
  EwNotifyRefObservers( EwNewRef( _this, ThermostatDeviceClass_OnGetNominalTemp, 
    ThermostatDeviceClass_OnSetNominalTemp ), 0 );
}

/* 'C' function for method : 'Thermostat::DeviceClass.OnSetFanSpeed()' */
void ThermostatDeviceClass_OnSetFanSpeed( ThermostatDeviceClass _this, XInt32 value )
{
  if ( _this->FanSpeed == value )
    return;

  _this->FanSpeed = value;
  {
    /*
       TO DO:

       You can call a function of your own device API or you simply
       modify a variable existing in your middleware to reflect the
       new value:

       YourDevice_SetSomeValue( value );

       or

       YourDevice_SomeVariable = value;
    */
  }
  EwNotifyRefObservers( EwNewRef( _this, ThermostatDeviceClass_OnGetFanSpeed, ThermostatDeviceClass_OnSetFanSpeed ), 
    0 );
}

/* 'C' function for method : 'Thermostat::DeviceClass.OnSetSwingMode()' */
void ThermostatDeviceClass_OnSetSwingMode( ThermostatDeviceClass _this, XBool value )
{
  if ( _this->SwingMode == value )
    return;

  _this->SwingMode = value;
  {
    /*
       TO DO:

       You can call a function of your own device API or you simply
       modify a variable existing in your middleware to reflect the
       new value:

       YourDevice_SetSomeValue( value );

       or

       YourDevice_SomeVariable = value;
    */
  }
  EwNotifyRefObservers( EwNewRef( _this, ThermostatDeviceClass_OnGetSwingMode, ThermostatDeviceClass_OnSetSwingMode ), 
    0 );
}

/* 'C' function for method : 'Thermostat::DeviceClass.ChangeActualTemp()' */
void ThermostatDeviceClass_ChangeActualTemp( ThermostatDeviceClass _this, XObject 
  sender )
{
  XInt32 temp;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  temp = _this->ActualTemp;

  if ( temp < _this->NominalTemp )
    temp = temp + 1;
  else
    if ( temp > _this->NominalTemp )
      temp = temp - 1;

  ThermostatDeviceClass_UpdateActualTemp( _this, temp );
}

/* 'C' function for method : 'Thermostat::DeviceClass.OnSetSettingNo()' */
void ThermostatDeviceClass_OnSetSettingNo( ThermostatDeviceClass _this, XInt32 value )
{
  if ( value > 2 )
    value = 2;

  if ( value < 0 )
    value = 0;

  if ( _this->SettingNo == value )
    return;

  _this->SettingNo = value;
  {
    /*
       TO DO:

       You can call a function of your own device API or you simply
       modify a variable existing in your middleware to reflect the
       new value:

       YourDevice_SetSomeValue( value );

       or

       YourDevice_SomeVariable = value;
    */
  }
  EwNotifyRefObservers( EwNewRef( _this, ThermostatDeviceClass_OnGetSettingNo, ThermostatDeviceClass_OnSetSettingNo ), 
    0 );
}

/* Default onget method for the property 'ActualTemp' */
XInt32 ThermostatDeviceClass_OnGetActualTemp( ThermostatDeviceClass _this )
{
  return _this->ActualTemp;
}

/* Default onset method for the property 'ActualTemp' */
void ThermostatDeviceClass_OnSetActualTemp( ThermostatDeviceClass _this, XInt32 
  value )
{
  _this->ActualTemp = value;
}

/* Default onget method for the property 'NominalTemp' */
XInt32 ThermostatDeviceClass_OnGetNominalTemp( ThermostatDeviceClass _this )
{
  return _this->NominalTemp;
}

/* Default onget method for the property 'FanSpeed' */
XInt32 ThermostatDeviceClass_OnGetFanSpeed( ThermostatDeviceClass _this )
{
  return _this->FanSpeed;
}

/* Default onget method for the property 'SwingMode' */
XBool ThermostatDeviceClass_OnGetSwingMode( ThermostatDeviceClass _this )
{
  return _this->SwingMode;
}

/* Default onget method for the property 'SettingNo' */
XInt32 ThermostatDeviceClass_OnGetSettingNo( ThermostatDeviceClass _this )
{
  return _this->SettingNo;
}

/* Variants derived from the class : 'Thermostat::DeviceClass' */
EW_DEFINE_CLASS_VARIANTS( ThermostatDeviceClass )
EW_END_OF_CLASS_VARIANTS( ThermostatDeviceClass )

/* Virtual Method Table (VMT) for the class : 'Thermostat::DeviceClass' */
EW_DEFINE_CLASS( ThermostatDeviceClass, TemplatesDeviceClass, ChangeTempTimer, ChangeTempTimer, 
                 ChangeTempTimer, ChangeTempTimer, ActualTemp, ActualTemp, "Thermostat::DeviceClass" )
EW_END_OF_CLASS( ThermostatDeviceClass )

/* User defined auto object: 'Thermostat::Device' */
EW_DEFINE_AUTOOBJECT( ThermostatDevice, ThermostatDeviceClass )

/* Initializer for the auto object 'Thermostat::Device' */
void ThermostatDevice__Init( ThermostatDeviceClass _this )
{
  EW_UNUSED_ARG( _this );
}

/* Table with links to derived variants of the auto object : 'Thermostat::Device' */
EW_DEFINE_AUTOOBJECT_VARIANTS( ThermostatDevice )
EW_END_OF_AUTOOBJECT_VARIANTS( ThermostatDevice )

/* Include a file containing the bitmap resource : 'Thermostat::Wheels' */
#include "_ThermostatWheels.h"

/* Table with links to derived variants of the bitmap resource : 'Thermostat::Wheels' */
EW_RES_WITHOUT_VARIANTS( ThermostatWheels )

/* Include a file containing the bitmap resource : 'Thermostat::Controller' */
#include "_ThermostatController.h"

/* Table with links to derived variants of the bitmap resource : 'Thermostat::Controller' */
EW_RES_WITHOUT_VARIANTS( ThermostatController )

/* Initializer for the class 'Thermostat::TemperatureController' */
void ThermostatTemperatureController__Init( ThermostatTemperatureController _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( ThermostatTemperatureController );

  /* ... then construct all embedded objects */
  CoreRotateTouchHandler__Init( &_this->RotateTouchHandler, &_this->_XObject, 0 );
  ViewsImage__Init( &_this->Wheel, &_this->_XObject, 0 );
  ViewsImage__Init( &_this->Foreground, &_this->_XObject, 0 );
  ViewsWarpImage__Init( &_this->Needle, &_this->_XObject, 0 );
  GraphicsPath__Init( &_this->DeltaTempPath, &_this->_XObject, 0 );
  ViewsStrokePath__Init( &_this->DeltaTempView, &_this->_XObject, 0 );
  ViewsImage__Init( &_this->Symbol, &_this->_XObject, 0 );
  ViewsText__Init( &_this->ActualText, &_this->_XObject, 0 );
  ViewsText__Init( &_this->NominalText, &_this->_XObject, 0 );
  EffectsInt32Effect__Init( &_this->FadeTextEffect, &_this->_XObject, 0 );
  CoreSimpleTouchHandler__Init( &_this->SimpleTouchHandlerPlus, &_this->_XObject, 0 );
  CoreSimpleTouchHandler__Init( &_this->SimpleTouchHandlerMinus, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( ThermostatTemperatureController );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const0005 );
  CoreView_OnSetLayout((CoreView)&_this->RotateTouchHandler, CoreLayoutAlignToLeft 
  | CoreLayoutAlignToRight | CoreLayoutAlignToTop );
  CoreRectView__OnSetBounds( &_this->RotateTouchHandler, _Const0005 );
  CoreRectView__OnSetBounds( &_this->Wheel, _Const0005 );
  CoreRectView__OnSetBounds( &_this->Foreground, _Const0006 );
  CoreQuadView__OnSetPoint4( &_this->Needle, _Const0007 );
  CoreQuadView__OnSetPoint3( &_this->Needle, _Const0008 );
  CoreQuadView__OnSetPoint2( &_this->Needle, _Const0009 );
  CoreQuadView__OnSetPoint1( &_this->Needle, _Const000A );
  ViewsWarpView_OnSetSourceAnchor((ViewsWarpView)&_this->Needle, _Const000B );
  _this->NominalValue = 300;
  _this->ActualValue = 150;
  _this->MaxValue = 400;
  _this->MinValue = 100;
  CoreRectView__OnSetBounds( &_this->DeltaTempView, _Const000C );
  ViewsStrokePath_OnSetColor( &_this->DeltaTempView, _Const000D );
  ViewsStrokePath_OnSetWidth( &_this->DeltaTempView, 3.000000f );
  CoreRectView__OnSetBounds( &_this->Symbol, _Const000E );
  CoreView_OnSetLayout((CoreView)&_this->ActualText, CoreLayoutAlignToBottom | CoreLayoutAlignToRight 
  | CoreLayoutAlignToTop | CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->ActualText, _Const000F );
  ViewsText_OnSetAlignment( &_this->ActualText, ViewsTextAlignmentAlignHorzRight 
  | ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->ActualText, EwLoadString( &_Const0010 ));
  ViewsText_OnSetColor( &_this->ActualText, _Const0011 );
  CoreView_OnSetLayout((CoreView)&_this->NominalText, CoreLayoutAlignToBottom | 
  CoreLayoutAlignToRight | CoreLayoutAlignToTop | CoreLayoutResizeHorz | CoreLayoutResizeVert );
  CoreRectView__OnSetBounds( &_this->NominalText, _Const000F );
  ViewsText_OnSetAlignment( &_this->NominalText, ViewsTextAlignmentAlignHorzRight 
  | ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->NominalText, EwLoadString( &_Const0010 ));
  ViewsText_OnSetColor( &_this->NominalText, _Const0011 );
  ViewsText_OnSetOpacity( &_this->NominalText, 0 );
  EffectsEffect_OnSetNoOfCycles((EffectsEffect)&_this->FadeTextEffect, 1 );
  EffectsEffect_OnSetCycleDuration((EffectsEffect)&_this->FadeTextEffect, 500 );
  EffectsEffect_OnSetInitialDelay((EffectsEffect)&_this->FadeTextEffect, 1200 );
  CoreQuadView__OnSetPoint4( &_this->SimpleTouchHandlerPlus, _Const0012 );
  CoreQuadView__OnSetPoint3( &_this->SimpleTouchHandlerPlus, _Const0013 );
  CoreQuadView__OnSetPoint2( &_this->SimpleTouchHandlerPlus, _Const0014 );
  CoreQuadView__OnSetPoint1( &_this->SimpleTouchHandlerPlus, _Const0015 );
  CoreSimpleTouchHandler_OnSetMaxStrikeCount( &_this->SimpleTouchHandlerPlus, 100 );
  CoreQuadView__OnSetPoint4( &_this->SimpleTouchHandlerMinus, _Const0016 );
  CoreQuadView__OnSetPoint3( &_this->SimpleTouchHandlerMinus, _Const0017 );
  CoreQuadView__OnSetPoint2( &_this->SimpleTouchHandlerMinus, _Const0018 );
  CoreQuadView__OnSetPoint1( &_this->SimpleTouchHandlerMinus, _Const0019 );
  CoreSimpleTouchHandler_OnSetMaxStrikeCount( &_this->SimpleTouchHandlerMinus, 100 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->RotateTouchHandler ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Wheel ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Foreground ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Needle ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->DeltaTempView ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Symbol ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->ActualText ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->NominalText ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->SimpleTouchHandlerPlus ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->SimpleTouchHandlerMinus ), 
  0 );
  _this->RotateTouchHandler.OnRotate = EwNewSlot( _this, ThermostatTemperatureController_onRotateSlot );
  _this->RotateTouchHandler.OnEnd = EwNewSlot( _this, ThermostatTemperatureController_onEndSlot );
  _this->RotateTouchHandler.OnStart = EwNewSlot( _this, ThermostatTemperatureController_onStartSlot );
  ViewsImage_OnSetBitmap( &_this->Wheel, EwLoadResource( &ThermostatWheels, ResourcesBitmap ));
  ViewsImage_OnSetBitmap( &_this->Foreground, EwLoadResource( &ThermostatController, 
  ResourcesBitmap ));
  ViewsWarpImage_OnSetBitmap( &_this->Needle, EwLoadResource( &ThermostatNeedle, 
  ResourcesBitmap ));
  ViewsStrokePath_OnSetPath( &_this->DeltaTempView, &_this->DeltaTempPath );
  ViewsImage_OnSetBitmap( &_this->Symbol, EwLoadResource( &ThermostatSymbolHeadCool, 
  ResourcesBitmap ));
  ViewsText_OnSetFont( &_this->ActualText, EwLoadResource( &ThermostatFontXXL, ResourcesFont ));
  ViewsText_OnSetFont( &_this->NominalText, EwLoadResource( &ThermostatFontXXL, 
  ResourcesFont ));
  _this->FadeTextEffect.Super1.OnAnimate = EwNewSlot( _this, ThermostatTemperatureController_fadeText );
  _this->SimpleTouchHandlerPlus.OnPress = EwNewSlot( _this, ThermostatTemperatureController_incrNominal );
  _this->SimpleTouchHandlerMinus.OnPress = EwNewSlot( _this, ThermostatTemperatureController_decrNominal );
}

/* Re-Initializer for the class 'Thermostat::TemperatureController' */
void ThermostatTemperatureController__ReInit( ThermostatTemperatureController _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  CoreRotateTouchHandler__ReInit( &_this->RotateTouchHandler );
  ViewsImage__ReInit( &_this->Wheel );
  ViewsImage__ReInit( &_this->Foreground );
  ViewsWarpImage__ReInit( &_this->Needle );
  GraphicsPath__ReInit( &_this->DeltaTempPath );
  ViewsStrokePath__ReInit( &_this->DeltaTempView );
  ViewsImage__ReInit( &_this->Symbol );
  ViewsText__ReInit( &_this->ActualText );
  ViewsText__ReInit( &_this->NominalText );
  EffectsInt32Effect__ReInit( &_this->FadeTextEffect );
  CoreSimpleTouchHandler__ReInit( &_this->SimpleTouchHandlerPlus );
  CoreSimpleTouchHandler__ReInit( &_this->SimpleTouchHandlerMinus );
}

/* Finalizer method for the class 'Thermostat::TemperatureController' */
void ThermostatTemperatureController__Done( ThermostatTemperatureController _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreGroup );

  /* Finalize all embedded objects */
  CoreRotateTouchHandler__Done( &_this->RotateTouchHandler );
  ViewsImage__Done( &_this->Wheel );
  ViewsImage__Done( &_this->Foreground );
  ViewsWarpImage__Done( &_this->Needle );
  GraphicsPath__Done( &_this->DeltaTempPath );
  ViewsStrokePath__Done( &_this->DeltaTempView );
  ViewsImage__Done( &_this->Symbol );
  ViewsText__Done( &_this->ActualText );
  ViewsText__Done( &_this->NominalText );
  EffectsInt32Effect__Done( &_this->FadeTextEffect );
  CoreSimpleTouchHandler__Done( &_this->SimpleTouchHandlerPlus );
  CoreSimpleTouchHandler__Done( &_this->SimpleTouchHandlerMinus );

  /* Don't forget to deinitialize the super class ... */
  CoreGroup__Done( &_this->_Super );
}

/* The method UpdateLayout() is invoked automatically after the size of the component 
   has been changed. This method can be overridden and filled with logic to perform 
   a sophisticated arrangement calculation for one or more enclosed views. In this 
   case the parameter aSize can be used. It contains the current size of the component. 
   Usually, all enclosed views are arranged automatically accordingly to their @Layout 
   property. UpdateLayout() gives the derived components a chance to extend this 
   automatism by a user defined algorithm. */
void ThermostatTemperatureController_UpdateLayout( ThermostatTemperatureController _this, 
  XPoint aSize )
{
  XInt32 cX;
  XInt32 cY;

  CoreGroup_UpdateLayout((CoreGroup)_this, aSize );
  cX = aSize.X / 2;
  cY = aSize.Y / 2;
  CoreRectView__OnSetBounds( &_this->DeltaTempView, EwNewRect2Point( _Const001A, 
  aSize ));
  ViewsStrokePath_OnSetOffset( &_this->DeltaTempView, EwNewPoint( cX, cY ));
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
void ThermostatTemperatureController_UpdateViewState( ThermostatTemperatureController _this, 
  XSet aState )
{
  XInt32 alpha;
  XInt32 actual;
  XFloat actAngle;
  XInt32 nominal;
  XFloat nomAngle;
  XInt32 inner;
  XInt32 outer;
  XColor hot;
  XColor cold;

  CoreGroup_UpdateViewState((CoreGroup)_this, aState );
  alpha = 0;
  actual = _this->ActualValue;
  actAngle = 0.000000f;
  nominal = _this->NominalValue;
  nomAngle = 0.000000f;

  if ( actual < _this->MinValue )
    actual = _this->MinValue;

  if ( actual > _this->MaxValue )
    actual = _this->MaxValue;

  if ( nominal < _this->MinValue )
    nominal = _this->MinValue;

  if ( nominal > _this->MaxValue )
    nominal = _this->MaxValue;

  if ( _this->MaxValue > _this->MinValue )
  {
    actAngle = 135.000000f - (((XFloat)( actual - _this->MinValue ) * 270.000000f ) 
    / (XFloat)( _this->MaxValue - _this->MinValue ));
    nomAngle = 135.000000f - (((XFloat)( nominal - _this->MinValue ) * 270.000000f ) 
    / (XFloat)( _this->MaxValue - _this->MinValue ));
  }

  ViewsWarpView_RotateAndScale((ViewsWarpView)&_this->Needle, EwGetRectCenter( _this->Foreground.Super1.Bounds ), 
  nomAngle, 1.000000f, 1.000000f );
  inner = 54;
  outer = 61;

  if (( _this->MaxValue > _this->MinValue ) && ( actual != nominal ))
  {
    GraphicsPath_InitSubPath( &_this->DeltaTempPath, 0, 84 );
    GraphicsPath_Rotate( &_this->DeltaTempPath, -actAngle - 90.000000f );
    GraphicsPath_Begin( &_this->DeltaTempPath, 0, (XFloat)inner, 0.000000f );
    GraphicsPath_AddLine( &_this->DeltaTempPath, 0, (XFloat)outer, 0.000000f );
    GraphicsPath_AddArc( &_this->DeltaTempPath, 0, 0.000000f, 0.000000f, (XFloat)outer, 
    (XFloat)outer, 0.000000f, -nomAngle + actAngle, 80 );
    GraphicsPath_Rotate( &_this->DeltaTempPath, actAngle + 90.000000f );
  }
  else
  {
    GraphicsPath_InitSubPath( &_this->DeltaTempPath, 0, 84 );
  }

  if ( actual < nominal )
  {
    ViewsImage_OnSetFrameNumber( &_this->Symbol, 0 );
    ViewsImage_OnSetColor( &_this->Symbol, ThermostatColorHot );
    ViewsStrokePath_OnSetColor( &_this->DeltaTempView, ThermostatColorHot );
    alpha = ( nominal - actual ) * 10;
  }
  else
    if ( actual > nominal )
    {
      ViewsImage_OnSetFrameNumber( &_this->Symbol, 1 );
      ViewsImage_OnSetColor( &_this->Symbol, ThermostatColorCold );
      ViewsStrokePath_OnSetColor( &_this->DeltaTempView, ThermostatColorCold );
      alpha = ( actual - nominal ) * 10;
    }

  if ( alpha > 255 )
    alpha = 255;

  ViewsImage_OnSetOpacity( &_this->Symbol, alpha );
  ViewsText_OnSetString( &_this->ActualText, EwConcatString( EwConcatString( EwConcatString( 
  EwNewStringInt( actual / 10, 0, 10 ), EwLoadString( &_Const001B )), EwNewStringInt( 
  actual % 10, 0, 10 )), EwLoadString( &_Const001C )));
  ViewsText_OnSetString( &_this->NominalText, EwConcatString( EwConcatString( EwConcatString( 
  EwNewStringInt( nominal / 10, 0, 10 ), EwLoadString( &_Const001B )), EwNewStringInt( 
  nominal % 10, 0, 10 )), EwLoadString( &_Const001C )));
  alpha = (( actual - _this->MinValue ) * 255 ) / ( _this->MaxValue - _this->MinValue );
  hot = EwBlendColorInt( ThermostatColorHot, alpha );
  cold = EwBlendColorInt( ThermostatColorCold, (XUInt8)( 255 - alpha ));
  ViewsText_OnSetColor( &_this->ActualText, EwPlusColor( hot, cold ));
}

/* 'C' function for method : 'Thermostat::TemperatureController.OnSetNominalValue()' */
void ThermostatTemperatureController_OnSetNominalValue( ThermostatTemperatureController _this, 
  XInt32 value )
{
  if ( _this->NominalValue == value )
    return;

  _this->NominalValue = value;
  CoreGroup_InvalidateViewState((CoreGroup)_this );
}

/* 'C' function for method : 'Thermostat::TemperatureController.OnSetActualValue()' */
void ThermostatTemperatureController_OnSetActualValue( ThermostatTemperatureController _this, 
  XInt32 value )
{
  if ( _this->ActualValue == value )
    return;

  _this->ActualValue = value;
  CoreGroup_InvalidateViewState((CoreGroup)_this );
}

/* This slot method will receive a signal, if the value of the property assigned 
   to @Outlet has been changed by another widget or by the application logic. In 
   response to this notification, the widget will update itself. */
void ThermostatTemperatureController_outletNominalSlot( ThermostatTemperatureController _this, 
  XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( _this->OutletNominal.Object != 0 )
    ThermostatTemperatureController_OnSetNominalValue( _this, EwOnGetInt32( _this->OutletNominal ));
}

/* 'C' function for method : 'Thermostat::TemperatureController.OnSetOutletNominal()' */
void ThermostatTemperatureController_OnSetOutletNominal( ThermostatTemperatureController _this, 
  XRef value )
{
  if ( !EwCompRef( _this->OutletNominal, value ))
    return;

  if ( _this->OutletNominal.Object != 0 )
    EwDetachRefObserver( EwNewSlot( _this, ThermostatTemperatureController_outletNominalSlot ), 
      _this->OutletNominal, 0 );

  _this->OutletNominal = value;

  if ( value.Object != 0 )
    EwAttachRefObserver( EwNewSlot( _this, ThermostatTemperatureController_outletNominalSlot ), 
      value, 0 );

  if ( value.Object != 0 )
    EwPostSignal( EwNewSlot( _this, ThermostatTemperatureController_outletNominalSlot ), 
      ((XObject)_this ));
}

/* This slot method will receive a signal, if the value of the property assigned 
   to @Outlet has been changed by another widget or by the application logic. In 
   response to this notification, the widget will update itself. */
void ThermostatTemperatureController_outletCurrentSlot( ThermostatTemperatureController _this, 
  XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  if ( _this->OutletActual.Object != 0 )
    ThermostatTemperatureController_OnSetActualValue( _this, EwOnGetInt32( _this->OutletActual ));
}

/* 'C' function for method : 'Thermostat::TemperatureController.OnSetOutletActual()' */
void ThermostatTemperatureController_OnSetOutletActual( ThermostatTemperatureController _this, 
  XRef value )
{
  if ( !EwCompRef( _this->OutletActual, value ))
    return;

  if ( _this->OutletActual.Object != 0 )
    EwDetachRefObserver( EwNewSlot( _this, ThermostatTemperatureController_outletCurrentSlot ), 
      _this->OutletActual, 0 );

  _this->OutletActual = value;

  if ( value.Object != 0 )
    EwAttachRefObserver( EwNewSlot( _this, ThermostatTemperatureController_outletCurrentSlot ), 
      value, 0 );

  if ( value.Object != 0 )
    EwPostSignal( EwNewSlot( _this, ThermostatTemperatureController_outletCurrentSlot ), 
      ((XObject)_this ));
}

/* 'C' function for method : 'Thermostat::TemperatureController.onStartSlot()' */
void ThermostatTemperatureController_onStartSlot( ThermostatTemperatureController _this, 
  XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  CoreGroup_InvalidateViewState((CoreGroup)_this );
  _this->startValue = _this->NominalValue;
  ViewsText_OnSetOpacity( &_this->NominalText, 255 );
  ViewsText_OnSetOpacity( &_this->ActualText, 0 );
  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeTextEffect, 0 );
}

/* 'C' function for method : 'Thermostat::TemperatureController.onRotateSlot()' */
void ThermostatTemperatureController_onRotateSlot( ThermostatTemperatureController _this, 
  XObject sender )
{
  XInt32 temp;
  XInt32 delta;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  temp = 0;
  delta = 0;

  if ((( _this->MaxValue > _this->MinValue ) && ( _this->NominalValue >= _this->MinValue )) 
      && ( _this->NominalValue <= _this->MaxValue ))
  {
    delta = ( _this->RotateTouchHandler.Relative * ( _this->MaxValue - _this->MinValue )) 
    / 720;
  }

  temp = _this->startValue - delta;

  if ( temp < _this->MinValue )
    temp = _this->MinValue;

  if ( temp > _this->MaxValue )
    temp = _this->MaxValue;

  if ( _this->NominalValue != temp )
  {
    XInt32 inx = _this->Wheel.FrameNumber;

    if (( _this->NominalValue - temp ) > 0 )
      inx = inx - 1;
    else
      inx = inx + 1;

    if ( inx < 0 )
      inx = 3;

    if ( inx > 3 )
      inx = 0;

    ViewsImage_OnSetFrameNumber( &_this->Wheel, inx );
    ThermostatTemperatureController_OnSetNominalValue( _this, temp );

    if ( _this->OutletNominal.Object != 0 )
      EwOnSetInt32( _this->OutletNominal, _this->NominalValue );

    EwNotifyRefObservers( _this->OutletNominal, 0 );
  }
}

/* 'C' function for method : 'Thermostat::TemperatureController.onEndSlot()' */
void ThermostatTemperatureController_onEndSlot( ThermostatTemperatureController _this, 
  XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  CoreGroup_InvalidateViewState((CoreGroup)_this );
  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeTextEffect, 1 );
}

/* 'C' function for method : 'Thermostat::TemperatureController.fadeText()' */
void ThermostatTemperatureController_fadeText( ThermostatTemperatureController _this, 
  XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  ViewsText_OnSetOpacity( &_this->NominalText, 255 - _this->FadeTextEffect.Value );
  ViewsText_OnSetOpacity( &_this->ActualText, _this->FadeTextEffect.Value );
}

/* 'C' function for method : 'Thermostat::TemperatureController.incrNominal()' */
void ThermostatTemperatureController_incrNominal( ThermostatTemperatureController _this, 
  XObject sender )
{
  XInt32 temp;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  temp = _this->NominalValue + 5;
  ViewsText_OnSetOpacity( &_this->NominalText, 255 );
  ViewsText_OnSetOpacity( &_this->ActualText, 0 );
  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeTextEffect, 0 );

  if ( temp > _this->MaxValue )
    temp = _this->MaxValue;

  ThermostatTemperatureController_OnSetNominalValue( _this, temp );

  if ( _this->OutletNominal.Object != 0 )
    EwOnSetInt32( _this->OutletNominal, _this->NominalValue );

  EwNotifyRefObservers( _this->OutletNominal, 0 );
  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeTextEffect, 1 );
}

/* 'C' function for method : 'Thermostat::TemperatureController.decrNominal()' */
void ThermostatTemperatureController_decrNominal( ThermostatTemperatureController _this, 
  XObject sender )
{
  XInt32 temp;

  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  temp = _this->NominalValue - 5;
  ViewsText_OnSetOpacity( &_this->NominalText, 255 );
  ViewsText_OnSetOpacity( &_this->ActualText, 0 );
  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeTextEffect, 0 );

  if ( temp < _this->MinValue )
    temp = _this->MinValue;

  ThermostatTemperatureController_OnSetNominalValue( _this, temp );

  if ( _this->OutletNominal.Object != 0 )
    EwOnSetInt32( _this->OutletNominal, _this->NominalValue );

  EwNotifyRefObservers( _this->OutletNominal, 0 );
  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeTextEffect, 1 );
}

/* Variants derived from the class : 'Thermostat::TemperatureController' */
EW_DEFINE_CLASS_VARIANTS( ThermostatTemperatureController )
EW_END_OF_CLASS_VARIANTS( ThermostatTemperatureController )

/* Virtual Method Table (VMT) for the class : 'Thermostat::TemperatureController' */
EW_DEFINE_CLASS( ThermostatTemperatureController, CoreGroup, OutletNominal, OutletNominal, 
                 OutletNominal, RotateTouchHandler, NominalValue, NominalValue, 
                 "Thermostat::TemperatureController" )
  CoreRectView_initLayoutContext,
  CoreView_GetRoot,
  CoreGroup_Draw,
  CoreView_HandleEvent,
  CoreGroup_CursorHitTest,
  CoreRectView_ArrangeView,
  CoreRectView_MoveView,
  CoreRectView_GetExtent,
  CoreGroup_ChangeViewState,
  CoreGroup_OnSetBounds,
  CoreGroup_OnSetFocus,
  CoreGroup_DispatchEvent,
  CoreGroup_BroadcastEvent,
  ThermostatTemperatureController_UpdateLayout,
  ThermostatTemperatureController_UpdateViewState,
  CoreGroup_InvalidateArea,
EW_END_OF_CLASS( ThermostatTemperatureController )

/* Include a file containing the bitmap resource : 'Thermostat::Needle' */
#include "_ThermostatNeedle.h"

/* Table with links to derived variants of the bitmap resource : 'Thermostat::Needle' */
EW_RES_WITHOUT_VARIANTS( ThermostatNeedle )

/* User defined constant: 'Thermostat::ColorCold' */
const XColor ThermostatColorCold = { 0x14, 0x88, 0xD7, 0xFF };

/* User defined constant: 'Thermostat::ColorHot' */
const XColor ThermostatColorHot = { 0xC7, 0x0B, 0x6A, 0xFF };

/* Include a file containing the bitmap resource : 'Thermostat::SymbolHeadCool' */
#include "_ThermostatSymbolHeadCool.h"

/* Table with links to derived variants of the bitmap resource : 'Thermostat::SymbolHeadCool' */
EW_RES_WITHOUT_VARIANTS( ThermostatSymbolHeadCool )

/* Include a file containing the font resource : 'Thermostat::FontXXL' */
#include "_ThermostatFontXXL.h"

/* Table with links to derived variants of the font resource : 'Thermostat::FontXXL' */
EW_RES_WITHOUT_VARIANTS( ThermostatFontXXL )

/* Initializer for the class 'Thermostat::Thermostat' */
void ThermostatThermostat__Init( ThermostatThermostat _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreGroup__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( ThermostatThermostat );

  /* ... then construct all embedded objects */
  ViewsWallpaper__Init( &_this->Background, &_this->_XObject, 0 );
  ViewsWallpaper__Init( &_this->Overlay, &_this->_XObject, 0 );
  WidgetSetToggleButton__Init( &_this->ToggleButton, &_this->_XObject, 0 );
  WidgetSetRadioButton__Init( &_this->RadioDay, &_this->_XObject, 0 );
  WidgetSetRadioButton__Init( &_this->RadioNight, &_this->_XObject, 0 );
  WidgetSetRadioButton__Init( &_this->RadioVacation, &_this->_XObject, 0 );
  WidgetSetVerticalSlider__Init( &_this->VerticalSlider, &_this->_XObject, 0 );
  ViewsImage__Init( &_this->FanHi, &_this->_XObject, 0 );
  ViewsImage__Init( &_this->FanHi1, &_this->_XObject, 0 );
  ThermostatTemperatureController__Init( &_this->TemperatureController, &_this->_XObject, 0 );
  ViewsText__Init( &_this->Title, &_this->_XObject, 0 );
  ViewsText__Init( &_this->OnOffButtonText, &_this->_XObject, 0 );
  ViewsImage__Init( &_this->ErgosignLogo, &_this->_XObject, 0 );
  EffectsInt32Effect__Init( &_this->FadeEffect, &_this->_XObject, 0 );
  CorePropertyObserver__Init( &_this->SettingsObserver, &_this->_XObject, 0 );
  CoreTimer__Init( &_this->AutoDemoTimer, &_this->_XObject, 0 );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( ThermostatThermostat );

  /* ... and initialize objects, variables, properties, etc. */
  CoreRectView__OnSetBounds( _this, _Const001D );
  CoreRectView__OnSetBounds( &_this->Background, _Const001D );
  CoreRectView__OnSetBounds( &_this->Overlay, _Const001D );
  ViewsWallpaper_OnSetOpacity( &_this->Overlay, 0 );
  CoreRectView__OnSetBounds( &_this->ToggleButton, _Const001E );
  WidgetSetToggleButton_OnSetLabel( &_this->ToggleButton, EwLoadString( &_Const001F ));
  CoreRectView__OnSetBounds( &_this->RadioDay, _Const0020 );
  WidgetSetRadioButton_OnSetLabel( &_this->RadioDay, EwLoadString( &_Const0021 ));
  CoreRectView__OnSetBounds( &_this->RadioNight, _Const0022 );
  WidgetSetRadioButton_OnSetOutletSelector( &_this->RadioNight, 1 );
  WidgetSetRadioButton_OnSetLabel( &_this->RadioNight, EwLoadString( &_Const0023 ));
  CoreRectView__OnSetBounds( &_this->RadioVacation, _Const0024 );
  WidgetSetRadioButton_OnSetOutletSelector( &_this->RadioVacation, 2 );
  WidgetSetRadioButton_OnSetLabel( &_this->RadioVacation, EwLoadString( &_Const0025 ));
  CoreRectView__OnSetBounds( &_this->VerticalSlider, _Const0026 );
  CoreRectView__OnSetBounds( &_this->FanHi, _Const0027 );
  ViewsImage_OnSetFrameNumber( &_this->FanHi, 1 );
  CoreRectView__OnSetBounds( &_this->FanHi1, _Const0028 );
  ViewsImage_OnSetFrameNumber( &_this->FanHi1, 0 );
  CoreRectView__OnSetBounds( &_this->TemperatureController, _Const0029 );
  CoreRectView__OnSetBounds( &_this->Title, _Const002A );
  ViewsText_OnSetAlignment( &_this->Title, ViewsTextAlignmentAlignHorzLeft | ViewsTextAlignmentAlignVertCenter );
  ViewsText_OnSetString( &_this->Title, EwLoadString( &_Const002B ));
  CoreRectView__OnSetBounds( &_this->OnOffButtonText, _Const002C );
  ViewsText_OnSetString( &_this->OnOffButtonText, EwLoadString( &_Const002D ));
  CoreRectView__OnSetBounds( &_this->ErgosignLogo, _Const002E );
  EffectsEffect_OnSetNoOfCycles((EffectsEffect)&_this->FadeEffect, 1 );
  CoreTimer_OnSetPeriod( &_this->AutoDemoTimer, 4000 );
  CoreTimer_OnSetBegin( &_this->AutoDemoTimer, 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Background ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Overlay ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->ToggleButton ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->RadioDay ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->RadioNight ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->RadioVacation ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->VerticalSlider ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->FanHi ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->FanHi1 ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->TemperatureController ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->Title ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->OnOffButtonText ), 0 );
  CoreGroup_Add((CoreGroup)_this, ((CoreView)&_this->ErgosignLogo ), 0 );
  ViewsWallpaper_OnSetBitmap( &_this->Background, EwLoadResource( &ThermostatBackgroundDay, 
  ResourcesBitmap ));
  ViewsWallpaper_OnSetBitmap( &_this->Overlay, EwLoadResource( &ThermostatBackgroundDay, 
  ResourcesBitmap ));
  WidgetSetToggleButton_OnSetOutlet( &_this->ToggleButton, EwNewRef( EwGetAutoObject( 
  &ThermostatDevice, ThermostatDeviceClass ), ThermostatDeviceClass_OnGetSwingMode, 
  ThermostatDeviceClass_OnSetSwingMode ));
  WidgetSetToggleButton_OnSetAppearance( &_this->ToggleButton, EwGetAutoObject( 
  &ThermostatSwingSwitchConfig, WidgetSetToggleButtonConfig ));
  WidgetSetRadioButton_OnSetOutlet( &_this->RadioDay, EwNewRef( EwGetAutoObject( 
  &ThermostatDevice, ThermostatDeviceClass ), ThermostatDeviceClass_OnGetSettingNo, 
  ThermostatDeviceClass_OnSetSettingNo ));
  WidgetSetRadioButton_OnSetAppearance( &_this->RadioDay, EwGetAutoObject( &ThermostatRadioButtonDayConfig, 
  WidgetSetRadioButtonConfig ));
  WidgetSetRadioButton_OnSetOutlet( &_this->RadioNight, EwNewRef( EwGetAutoObject( 
  &ThermostatDevice, ThermostatDeviceClass ), ThermostatDeviceClass_OnGetSettingNo, 
  ThermostatDeviceClass_OnSetSettingNo ));
  WidgetSetRadioButton_OnSetAppearance( &_this->RadioNight, EwGetAutoObject( &ThermostatRadioButtonNightConfig, 
  WidgetSetRadioButtonConfig ));
  WidgetSetRadioButton_OnSetOutlet( &_this->RadioVacation, EwNewRef( EwGetAutoObject( 
  &ThermostatDevice, ThermostatDeviceClass ), ThermostatDeviceClass_OnGetSettingNo, 
  ThermostatDeviceClass_OnSetSettingNo ));
  WidgetSetRadioButton_OnSetAppearance( &_this->RadioVacation, EwGetAutoObject( 
  &ThermostatRadioButtonVacationConfig, WidgetSetRadioButtonConfig ));
  WidgetSetVerticalSlider_OnSetOutlet( &_this->VerticalSlider, EwNewRef( EwGetAutoObject( 
  &ThermostatDevice, ThermostatDeviceClass ), ThermostatDeviceClass_OnGetFanSpeed, 
  ThermostatDeviceClass_OnSetFanSpeed ));
  WidgetSetVerticalSlider_OnSetAppearance( &_this->VerticalSlider, EwGetAutoObject( 
  &ThermostatVerticalSliderConfig, WidgetSetVerticalSliderConfig ));
  _this->Device = EwGetAutoObject( &ThermostatDevice, ThermostatDeviceClass );
  ViewsImage_OnSetBitmap( &_this->FanHi, EwLoadResource( &ThermostatSymbolFan, ResourcesBitmap ));
  ViewsImage_OnSetBitmap( &_this->FanHi1, EwLoadResource( &ThermostatSymbolFan, 
  ResourcesBitmap ));
  ThermostatTemperatureController_OnSetOutletNominal( &_this->TemperatureController, 
  EwNewRef( EwGetAutoObject( &ThermostatDevice, ThermostatDeviceClass ), ThermostatDeviceClass_OnGetNominalTemp, 
  ThermostatDeviceClass_OnSetNominalTemp ));
  ThermostatTemperatureController_OnSetOutletActual( &_this->TemperatureController, 
  EwNewRef( EwGetAutoObject( &ThermostatDevice, ThermostatDeviceClass ), ThermostatDeviceClass_OnGetActualTemp, 
  ThermostatDeviceClass_OnSetActualTemp ));
  ViewsText_OnSetFont( &_this->Title, EwLoadResource( &ThermostatFontL, ResourcesFont ));
  ViewsText_OnSetFont( &_this->OnOffButtonText, EwLoadResource( &ThermostatFontM, 
  ResourcesFont ));
  ViewsImage_OnSetBitmap( &_this->ErgosignLogo, EwLoadResource( &ThermostatErgosign, 
  ResourcesBitmap ));
  _this->FadeEffect.Super1.OnFinished = EwNewSlot( _this, ThermostatThermostat_changeBackFinished );
  _this->FadeEffect.Outlet = EwNewRef( &_this->Overlay, ViewsWallpaper_OnGetOpacity, 
  ViewsWallpaper_OnSetOpacity );
  _this->SettingsObserver.OnEvent = EwNewSlot( _this, ThermostatThermostat_onSettingChanged );
  CorePropertyObserver_OnSetOutlet( &_this->SettingsObserver, EwNewRef( EwGetAutoObject( 
  &ThermostatDevice, ThermostatDeviceClass ), ThermostatDeviceClass_OnGetSettingNo, 
  ThermostatDeviceClass_OnSetSettingNo ));
  _this->AutoDemoTimer.OnTrigger = EwNewSlot( _this, ThermostatThermostat_AutoDemoSlot );
}

/* Re-Initializer for the class 'Thermostat::Thermostat' */
void ThermostatThermostat__ReInit( ThermostatThermostat _this )
{
  /* At first re-initialize the super class ... */
  CoreGroup__ReInit( &_this->_Super );

  /* ... then re-construct all embedded objects */
  ViewsWallpaper__ReInit( &_this->Background );
  ViewsWallpaper__ReInit( &_this->Overlay );
  WidgetSetToggleButton__ReInit( &_this->ToggleButton );
  WidgetSetRadioButton__ReInit( &_this->RadioDay );
  WidgetSetRadioButton__ReInit( &_this->RadioNight );
  WidgetSetRadioButton__ReInit( &_this->RadioVacation );
  WidgetSetVerticalSlider__ReInit( &_this->VerticalSlider );
  ViewsImage__ReInit( &_this->FanHi );
  ViewsImage__ReInit( &_this->FanHi1 );
  ThermostatTemperatureController__ReInit( &_this->TemperatureController );
  ViewsText__ReInit( &_this->Title );
  ViewsText__ReInit( &_this->OnOffButtonText );
  ViewsImage__ReInit( &_this->ErgosignLogo );
  EffectsInt32Effect__ReInit( &_this->FadeEffect );
  CorePropertyObserver__ReInit( &_this->SettingsObserver );
  CoreTimer__ReInit( &_this->AutoDemoTimer );
}

/* Finalizer method for the class 'Thermostat::Thermostat' */
void ThermostatThermostat__Done( ThermostatThermostat _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreGroup );

  /* Finalize all embedded objects */
  ViewsWallpaper__Done( &_this->Background );
  ViewsWallpaper__Done( &_this->Overlay );
  WidgetSetToggleButton__Done( &_this->ToggleButton );
  WidgetSetRadioButton__Done( &_this->RadioDay );
  WidgetSetRadioButton__Done( &_this->RadioNight );
  WidgetSetRadioButton__Done( &_this->RadioVacation );
  WidgetSetVerticalSlider__Done( &_this->VerticalSlider );
  ViewsImage__Done( &_this->FanHi );
  ViewsImage__Done( &_this->FanHi1 );
  ThermostatTemperatureController__Done( &_this->TemperatureController );
  ViewsText__Done( &_this->Title );
  ViewsText__Done( &_this->OnOffButtonText );
  ViewsImage__Done( &_this->ErgosignLogo );
  EffectsInt32Effect__Done( &_this->FadeEffect );
  CorePropertyObserver__Done( &_this->SettingsObserver );
  CoreTimer__Done( &_this->AutoDemoTimer );

  /* Don't forget to deinitialize the super class ... */
  CoreGroup__Done( &_this->_Super );
}

/* 'C' function for method : 'Thermostat::Thermostat.changeBackFinished()' */
void ThermostatThermostat_changeBackFinished( ThermostatThermostat _this, XObject 
  sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  ViewsWallpaper_OnSetBitmap( &_this->Background, _this->Overlay.Bitmap );
  ViewsWallpaper_OnSetOpacity( &_this->Overlay, 0 );
}

/* This slot method is executed when the associated property observer 'PropertyObserver' 
   is notified. */
void ThermostatThermostat_onSettingChanged( ThermostatThermostat _this, XObject 
  sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  ViewsWallpaper_OnSetOpacity( &_this->Overlay, 0 );
  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeEffect, 0 );

  switch ( _this->Device->SettingNo )
  {
    case 0 :
      ViewsWallpaper_OnSetBitmap( &_this->Overlay, EwLoadResource( &ThermostatBackgroundDay, 
      ResourcesBitmap ));
    break;

    case 1 :
      ViewsWallpaper_OnSetBitmap( &_this->Overlay, EwLoadResource( &ThermostatBackgroundNight, 
      ResourcesBitmap ));
    break;

    default : 
      ViewsWallpaper_OnSetBitmap( &_this->Overlay, EwLoadResource( &ThermostatBackgroundDefault, 
      ResourcesBitmap ));
  }

  EffectsEffect_OnSetEnabled((EffectsEffect)&_this->FadeEffect, 1 );
}

/* Slot method to trigger the next phase of the auto demo. */
void ThermostatThermostat_AutoDemoSlot( ThermostatThermostat _this, XObject sender )
{
  /* Dummy expressions to avoid the 'C' warning 'unused argument'. */
  EW_UNUSED_ARG( sender );

  switch ( _this->AutoState )
  {
    case 0 :
      ThermostatDeviceClass_OnSetNominalTemp( _this->Device, 335 );
    break;

    case 1 :
      ThermostatDeviceClass_OnSetSettingNo( _this->Device, 1 );
    break;

    case 2 :
      ThermostatDeviceClass_OnSetNominalTemp( _this->Device, 185 );
    break;

    case 3 :
      ThermostatDeviceClass_OnSetSettingNo( _this->Device, 2 );
    break;

    case 4 :
      ThermostatDeviceClass_OnSetSettingNo( _this->Device, 0 );
    break;

    case 5 :
    {
      ThermostatDeviceClass_OnSetNominalTemp( _this->Device, 225 );
      ThermostatDeviceClass_OnSetFanSpeed( _this->Device, 80 );
      ThermostatDeviceClass_OnSetSwingMode( _this->Device, 1 );
    }
    break;

    case 6 :
    {
      ThermostatDeviceClass_OnSetNominalTemp( _this->Device, 190 );
      ThermostatDeviceClass_OnSetFanSpeed( _this->Device, 30 );
      ThermostatDeviceClass_OnSetSwingMode( _this->Device, 0 );
    }
    break;

    default : 
      ;
  }

  _this->AutoState = _this->AutoState + 1;

  if ( _this->AutoState > 6 )
    _this->AutoState = 0;
}

/* Variants derived from the class : 'Thermostat::Thermostat' */
EW_DEFINE_CLASS_VARIANTS( ThermostatThermostat )
EW_END_OF_CLASS_VARIANTS( ThermostatThermostat )

/* Virtual Method Table (VMT) for the class : 'Thermostat::Thermostat' */
EW_DEFINE_CLASS( ThermostatThermostat, CoreGroup, Device, Background, Background, 
                 Background, AutoState, AutoState, "Thermostat::Thermostat" )
  CoreRectView_initLayoutContext,
  CoreView_GetRoot,
  CoreGroup_Draw,
  CoreView_HandleEvent,
  CoreGroup_CursorHitTest,
  CoreRectView_ArrangeView,
  CoreRectView_MoveView,
  CoreRectView_GetExtent,
  CoreGroup_ChangeViewState,
  CoreGroup_OnSetBounds,
  CoreGroup_OnSetFocus,
  CoreGroup_DispatchEvent,
  CoreGroup_BroadcastEvent,
  CoreGroup_UpdateLayout,
  CoreGroup_UpdateViewState,
  CoreGroup_InvalidateArea,
EW_END_OF_CLASS( ThermostatThermostat )

/* Include a file containing the bitmap resource : 'Thermostat::BackgroundDay' */
#include "_ThermostatBackgroundDay.h"

/* Table with links to derived variants of the bitmap resource : 'Thermostat::BackgroundDay' */
EW_RES_WITHOUT_VARIANTS( ThermostatBackgroundDay )

/* Include a file containing the bitmap resource : 'Thermostat::BackgroundNight' */
#include "_ThermostatBackgroundNight.h"

/* Table with links to derived variants of the bitmap resource : 'Thermostat::BackgroundNight' */
EW_RES_WITHOUT_VARIANTS( ThermostatBackgroundNight )

/* Include a file containing the bitmap resource : 'Thermostat::SymbolFan' */
#include "_ThermostatSymbolFan.h"

/* Table with links to derived variants of the bitmap resource : 'Thermostat::SymbolFan' */
EW_RES_WITHOUT_VARIANTS( ThermostatSymbolFan )

/* Include a file containing the font resource : 'Thermostat::FontM' */
#include "_ThermostatFontM.h"

/* Table with links to derived variants of the font resource : 'Thermostat::FontM' */
EW_RES_WITHOUT_VARIANTS( ThermostatFontM )

/* Include a file containing the bitmap resource : 'Thermostat::BackgroundDefault' */
#include "_ThermostatBackgroundDefault.h"

/* Table with links to derived variants of the bitmap resource : 'Thermostat::BackgroundDefault' */
EW_RES_WITHOUT_VARIANTS( ThermostatBackgroundDefault )

/* Include a file containing the bitmap resource : 'Thermostat::Ergosign' */
#include "_ThermostatErgosign.h"

/* Table with links to derived variants of the bitmap resource : 'Thermostat::Ergosign' */
EW_RES_WITHOUT_VARIANTS( ThermostatErgosign )

/* Embedded Wizard */
