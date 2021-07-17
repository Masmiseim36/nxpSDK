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

#ifndef Oximeter_H
#define Oximeter_H

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

#include "_OximeterAnalogDisplay.h"
#include "_OximeterBackground.h"
#include "_OximeterDataStorage.h"
#include "_OximeterDeviceClass.h"
#include "_OximeterGraph.h"
#include "_OximeterGraphPanel.h"
#include "_OximeterMainMenu.h"
#include "_OximeterNumberDisplay.h"
#include "_OximeterOximeter.h"
#include "_OximeterOxygenPanel.h"
#include "_OximeterOxygenSettings.h"
#include "_OximeterPlotter.h"
#include "_OximeterPulsePanel.h"
#include "_OximeterPulseSettings.h"
#include "_OximeterValueDisplay.h"

/* Bitmap resource : 'Oximeter::Glow' */
EW_DECLARE_BITMAP_RES( OximeterGlow )

/* Font resource : 'Oximeter::FontCaption' */
EW_DECLARE_FONT_RES( OximeterFontCaption )

/* Font resource : 'Oximeter::FontM' */
EW_DECLARE_FONT_RES( OximeterFontM )

/* Bitmap resource : 'Oximeter::HeartLarge' */
EW_DECLARE_BITMAP_RES( OximeterHeartLarge )

/* Bitmap resource : 'Oximeter::HeartSmall' */
EW_DECLARE_BITMAP_RES( OximeterHeartSmall )

/* Bitmap resource : 'Oximeter::LungLarge' */
EW_DECLARE_BITMAP_RES( OximeterLungLarge )

/* Bitmap resource : 'Oximeter::LungSmall' */
EW_DECLARE_BITMAP_RES( OximeterLungSmall )

/* Bitmap resource : 'Oximeter::OxygenGradient' */
EW_DECLARE_BITMAP_RES( OximeterOxygenGradient )

/* Bitmap resource : 'Oximeter::PulseGradient' */
EW_DECLARE_BITMAP_RES( OximeterPulseGradient )

/* Bitmap resource : 'Oximeter::IconButton' */
EW_DECLARE_BITMAP_RES( OximeterIconButton )

/* Bitmap resource : 'Oximeter::RotaryBackground' */
EW_DECLARE_BITMAP_RES( OximeterRotaryBackground )

/* Bitmap resource : 'Oximeter::RotaryThumbMax' */
EW_DECLARE_BITMAP_RES( OximeterRotaryThumbMax )

/* Bitmap resource : 'Oximeter::RotaryThumbMin' */
EW_DECLARE_BITMAP_RES( OximeterRotaryThumbMin )

/* Bitmap resource : 'Oximeter::ToggleButton' */
EW_DECLARE_BITMAP_RES( OximeterToggleButton )

/* Font resource : 'Oximeter::FontL' */
EW_DECLARE_FONT_RES( OximeterFontL )

/* Bitmap resource : 'Oximeter::Thumb' */
EW_DECLARE_BITMAP_RES( OximeterThumb )

/* Bitmap resource : 'Oximeter::Track' */
EW_DECLARE_BITMAP_RES( OximeterTrack )

/* Bitmap resource : 'Oximeter::PushButton' */
EW_DECLARE_BITMAP_RES( OximeterPushButton )

/* Bitmap resource : 'Oximeter::Battery' */
EW_DECLARE_BITMAP_RES( OximeterBattery )

/* Bitmap resource : 'Oximeter::MenuIcons' */
EW_DECLARE_BITMAP_RES( OximeterMenuIcons )

/* Bitmap resource : 'Oximeter::MenuButton' */
EW_DECLARE_BITMAP_RES( OximeterMenuButton )

/* Bitmap resource : 'Oximeter::RotaryBackgroundOxygen' */
EW_DECLARE_BITMAP_RES( OximeterRotaryBackgroundOxygen )

/* Bitmap resource : 'Oximeter::RotaryBackgroundFlow' */
EW_DECLARE_BITMAP_RES( OximeterRotaryBackgroundFlow )

/* Bitmap resource : 'Oximeter::DigitsLarge' */
EW_DECLARE_BITMAP_RES( OximeterDigitsLarge )

/* Bitmap resource : 'Oximeter::DigitsSmall' */
EW_DECLARE_BITMAP_RES( OximeterDigitsSmall )

/* Forward declaration of the class WidgetSet::HorizontalSliderConfig */
#ifndef _WidgetSetHorizontalSliderConfig_
  EW_DECLARE_CLASS( WidgetSetHorizontalSliderConfig )
#define _WidgetSetHorizontalSliderConfig_
#endif

/* Forward declaration of the class WidgetSet::PushButtonConfig */
#ifndef _WidgetSetPushButtonConfig_
  EW_DECLARE_CLASS( WidgetSetPushButtonConfig )
#define _WidgetSetPushButtonConfig_
#endif

/* Forward declaration of the class WidgetSet::RotaryKnobConfig */
#ifndef _WidgetSetRotaryKnobConfig_
  EW_DECLARE_CLASS( WidgetSetRotaryKnobConfig )
#define _WidgetSetRotaryKnobConfig_
#endif

/* Forward declaration of the class WidgetSet::ToggleButtonConfig */
#ifndef _WidgetSetToggleButtonConfig_
  EW_DECLARE_CLASS( WidgetSetToggleButtonConfig )
#define _WidgetSetToggleButtonConfig_
#endif

/* This is an enumeration definition for indicating the current opeation mode of 
   the oximeter. */
typedef enum
{
  OximeterOperationModeOverview         = 0,
  OximeterOperationModePulseSettings    = 1,
  OximeterOperationModeOxygenSettings   = 2,
  OximeterOperationModeGraphs           = 3,
  OximeterOperationModeMenus            = 4
} OximeterOperationMode;

/* User defined auto object: 'Oximeter::Device' */
EW_DECLARE_AUTOOBJECT( OximeterDevice, OximeterDeviceClass )

/* User defined auto object: 'Oximeter::IconButtonConfig' */
EW_DECLARE_AUTOOBJECT( OximeterIconButtonConfig, WidgetSetPushButtonConfig )

/* User defined auto object: 'Oximeter::RotaryMaxConfig' */
EW_DECLARE_AUTOOBJECT( OximeterRotaryMaxConfig, WidgetSetRotaryKnobConfig )

/* User defined auto object: 'Oximeter::RotaryMinConfig' */
EW_DECLARE_AUTOOBJECT( OximeterRotaryMinConfig, WidgetSetRotaryKnobConfig )

/* Color constants for panels, text items and graphs */
extern const XColor OximeterColorOxygenLight;

/* Color constants for panels, text items and graphs */
extern const XColor OximeterColorOxygenDark;

/* Color constants for panels, text items and graphs */
extern const XColor OximeterColorPulseLight;

/* Color constants for panels, text items and graphs */
extern const XColor OximeterColorPulseDark;

/* User defined auto object: 'Oximeter::ToggleButtonConfig' */
EW_DECLARE_AUTOOBJECT( OximeterToggleButtonConfig, WidgetSetToggleButtonConfig )

/* User defined auto object: 'Oximeter::HorizontalSliderConfig' */
EW_DECLARE_AUTOOBJECT( OximeterHorizontalSliderConfig, WidgetSetHorizontalSliderConfig )

/* User defined auto object: 'Oximeter::PushButtonConfig' */
EW_DECLARE_AUTOOBJECT( OximeterPushButtonConfig, WidgetSetPushButtonConfig )

/* User defined auto object: 'Oximeter::MenuItemConfig' */
EW_DECLARE_AUTOOBJECT( OximeterMenuItemConfig, WidgetSetPushButtonConfig )

/* User defined auto object: 'Oximeter::RotaryOxiConfig' */
EW_DECLARE_AUTOOBJECT( OximeterRotaryOxiConfig, WidgetSetRotaryKnobConfig )

/* User defined auto object: 'Oximeter::RotaryFlowConfig' */
EW_DECLARE_AUTOOBJECT( OximeterRotaryFlowConfig, WidgetSetRotaryKnobConfig )

#ifdef __cplusplus
  }
#endif

#endif /* Oximeter_H */

/* Embedded Wizard */
