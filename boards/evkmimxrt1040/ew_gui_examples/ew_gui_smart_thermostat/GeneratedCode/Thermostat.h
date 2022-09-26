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

#ifndef Thermostat_H
#define Thermostat_H

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

#include "_ThermostatDeviceClass.h"
#include "_ThermostatTemperatureController.h"
#include "_ThermostatThermostat.h"

/* Bitmap resource : 'Thermostat::SwingSwitch' */
EW_DECLARE_BITMAP_RES( ThermostatSwingSwitch )

/* Bitmap resource : 'Thermostat::ButtonDay' */
EW_DECLARE_BITMAP_RES( ThermostatButtonDay )

/* Bitmap resource : 'Thermostat::ButtonNight' */
EW_DECLARE_BITMAP_RES( ThermostatButtonNight )

/* Bitmap resource : 'Thermostat::ButtonVacation' */
EW_DECLARE_BITMAP_RES( ThermostatButtonVacation )

/* Font resources used for the different widgets. If you want to change the fonts 
   within your project, just create a variant of these font resources and define 
   the attributes of the font resources according your design ideas. */
EW_DECLARE_FONT_RES( ThermostatFontL )

/* Bitmap resource : 'Thermostat::VertBar' */
EW_DECLARE_BITMAP_RES( ThermostatVertBar )

/* Bitmap resource : 'Thermostat::Wheels' */
EW_DECLARE_BITMAP_RES( ThermostatWheels )

/* Bitmap resource : 'Thermostat::Controller' */
EW_DECLARE_BITMAP_RES( ThermostatController )

/* Bitmap resource : 'Thermostat::Needle' */
EW_DECLARE_BITMAP_RES( ThermostatNeedle )

/* Bitmap resource : 'Thermostat::SymbolHeadCool' */
EW_DECLARE_BITMAP_RES( ThermostatSymbolHeadCool )

/* Font resources used for the different widgets. If you want to change the fonts 
   within your project, just create a variant of these font resources and define 
   the attributes of the font resources according your design ideas. */
EW_DECLARE_FONT_RES( ThermostatFontXXL )

/* Bitmap resource : 'Thermostat::BackgroundDay' */
EW_DECLARE_BITMAP_RES( ThermostatBackgroundDay )

/* Bitmap resource : 'Thermostat::BackgroundNight' */
EW_DECLARE_BITMAP_RES( ThermostatBackgroundNight )

/* Bitmap resource : 'Thermostat::SymbolFan' */
EW_DECLARE_BITMAP_RES( ThermostatSymbolFan )

/* Font resources used for the different widgets. If you want to change the fonts 
   within your project, just create a variant of these font resources and define 
   the attributes of the font resources according your design ideas. */
EW_DECLARE_FONT_RES( ThermostatFontM )

/* Bitmap resource : 'Thermostat::BackgroundDefault' */
EW_DECLARE_BITMAP_RES( ThermostatBackgroundDefault )

/* Bitmap resource : 'Thermostat::Ergosign' */
EW_DECLARE_BITMAP_RES( ThermostatErgosign )

/* Forward declaration of the class WidgetSet::RadioButtonConfig */
#ifndef _WidgetSetRadioButtonConfig_
  EW_DECLARE_CLASS( WidgetSetRadioButtonConfig )
#define _WidgetSetRadioButtonConfig_
#endif

/* Forward declaration of the class WidgetSet::ToggleButtonConfig */
#ifndef _WidgetSetToggleButtonConfig_
  EW_DECLARE_CLASS( WidgetSetToggleButtonConfig )
#define _WidgetSetToggleButtonConfig_
#endif

/* Forward declaration of the class WidgetSet::VerticalSliderConfig */
#ifndef _WidgetSetVerticalSliderConfig_
  EW_DECLARE_CLASS( WidgetSetVerticalSliderConfig )
#define _WidgetSetVerticalSliderConfig_
#endif

/* User defined auto object: 'Thermostat::SwingSwitchConfig' */
EW_DECLARE_AUTOOBJECT( ThermostatSwingSwitchConfig, WidgetSetToggleButtonConfig )

/* User defined auto object: 'Thermostat::RadioButtonDayConfig' */
EW_DECLARE_AUTOOBJECT( ThermostatRadioButtonDayConfig, WidgetSetRadioButtonConfig )

/* User defined auto object: 'Thermostat::RadioButtonNightConfig' */
EW_DECLARE_AUTOOBJECT( ThermostatRadioButtonNightConfig, WidgetSetRadioButtonConfig )

/* User defined auto object: 'Thermostat::RadioButtonVacationConfig' */
EW_DECLARE_AUTOOBJECT( ThermostatRadioButtonVacationConfig, WidgetSetRadioButtonConfig )

/* User defined auto object: 'Thermostat::VerticalSliderConfig' */
EW_DECLARE_AUTOOBJECT( ThermostatVerticalSliderConfig, WidgetSetVerticalSliderConfig )

/* User defined auto object: 'Thermostat::Device' */
EW_DECLARE_AUTOOBJECT( ThermostatDevice, ThermostatDeviceClass )

/* User defined constant: 'Thermostat::ColorCold' */
extern const XColor ThermostatColorCold;

/* User defined constant: 'Thermostat::ColorHot' */
extern const XColor ThermostatColorHot;

#ifdef __cplusplus
  }
#endif

#endif /* Thermostat_H */

/* Embedded Wizard */
