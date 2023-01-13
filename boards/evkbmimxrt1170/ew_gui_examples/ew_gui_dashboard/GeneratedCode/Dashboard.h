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

#ifndef Dashboard_H
#define Dashboard_H

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

#include "_DashboardAnimationClass.h"
#include "_DashboardBattery.h"
#include "_DashboardClock.h"
#include "_DashboardColorsClass.h"
#include "_DashboardDashboard.h"
#include "_DashboardDashboardV1.h"
#include "_DashboardDashboardV2.h"
#include "_DashboardDemoMode.h"
#include "_DashboardDeviceClass.h"
#include "_DashboardGauge.h"
#include "_DashboardGaugeDisplay.h"
#include "_DashboardGaugeIndicator.h"
#include "_DashboardGaugeLabels.h"
#include "_DashboardNavigation.h"
#include "_DashboardNavigationHints.h"
#include "_DashboardStartScreen.h"
#include "_DashboardVertValueBar.h"

/* Bitmap resource : 'Dashboard::StartEngine' */
EW_DECLARE_BITMAP_RES( DashboardStartEngine )

/* Bitmap resource : 'Dashboard::StartEngineGloom' */
EW_DECLARE_BITMAP_RES( DashboardStartEngineGloom )

/* Bitmap resource : 'Dashboard::LeftBottomGloomV1' */
EW_DECLARE_BITMAP_RES( DashboardLeftBottomGloomV1 )

/* Bitmap resource : 'Dashboard::RightBottomGloomV1' */
EW_DECLARE_BITMAP_RES( DashboardRightBottomGloomV1 )

/* Bitmap resource : 'Dashboard::TopGloomV1' */
EW_DECLARE_BITMAP_RES( DashboardTopGloomV1 )

/* Bitmap resource : 'Dashboard::Icons' */
EW_DECLARE_BITMAP_RES( DashboardIcons )

/* Bitmap resource : 'Dashboard::LeftWing2V2' */
EW_DECLARE_BITMAP_RES( DashboardLeftWing2V2 )

/* Bitmap resource : 'Dashboard::LeftWingV2' */
EW_DECLARE_BITMAP_RES( DashboardLeftWingV2 )

/* Bitmap resource : 'Dashboard::RightWing2V2' */
EW_DECLARE_BITMAP_RES( DashboardRightWing2V2 )

/* Bitmap resource : 'Dashboard::RightWingV2' */
EW_DECLARE_BITMAP_RES( DashboardRightWingV2 )

/* Bitmap resource : 'Dashboard::LeftGlowV2' */
EW_DECLARE_BITMAP_RES( DashboardLeftGlowV2 )

/* Bitmap resource : 'Dashboard::RightGlowV2' */
EW_DECLARE_BITMAP_RES( DashboardRightGlowV2 )

/* Bitmap resource : 'Dashboard::BatteryFrame' */
EW_DECLARE_BITMAP_RES( DashboardBatteryFrame )

/* Bitmap resource : 'Dashboard::BatteryEnergy' */
EW_DECLARE_BITMAP_RES( DashboardBatteryEnergy )

/* Bitmap resource : 'Dashboard::GaugeNeedle' */
EW_DECLARE_BITMAP_RES( DashboardGaugeNeedle )

/* Bitmap resource : 'Dashboard::Circle0' */
EW_DECLARE_BITMAP_RES( DashboardCircle0 )

/* Bitmap resource : 'Dashboard::Circle1' */
EW_DECLARE_BITMAP_RES( DashboardCircle1 )

/* Bitmap resource : 'Dashboard::Circle2' */
EW_DECLARE_BITMAP_RES( DashboardCircle2 )

/* Bitmap resource : 'Dashboard::Circle3' */
EW_DECLARE_BITMAP_RES( DashboardCircle3 )

/* Bitmap resource : 'Dashboard::Circle4' */
EW_DECLARE_BITMAP_RES( DashboardCircle4 )

/* Bitmap resource : 'Dashboard::Circle5' */
EW_DECLARE_BITMAP_RES( DashboardCircle5 )

/* Bitmap resource : 'Dashboard::Map' */
EW_DECLARE_BITMAP_RES( DashboardMap )

/* Bitmap resource : 'Dashboard::MapFading' */
EW_DECLARE_BITMAP_RES( DashboardMapFading )

/* Bitmap resource : 'Dashboard::Position' */
EW_DECLARE_BITMAP_RES( DashboardPosition )

/* Bitmap resource : 'Dashboard::Arrows' */
EW_DECLARE_BITMAP_RES( DashboardArrows )

/* Bitmap resource : 'Dashboard::GaugeGlowSegment' */
EW_DECLARE_BITMAP_RES( DashboardGaugeGlowSegment )

/* Bitmap resource : 'Dashboard::StartEngineStates' */
EW_DECLARE_BITMAP_RES( DashboardStartEngineStates )

/* Bitmap resource : 'Dashboard::SwitchIcon' */
EW_DECLARE_BITMAP_RES( DashboardSwitchIcon )

/* Bitmap resource : 'Dashboard::OffIcon' */
EW_DECLARE_BITMAP_RES( DashboardOffIcon )

/* Bitmap resource : 'Dashboard::Logo' */
EW_DECLARE_BITMAP_RES( DashboardLogo )

/* Font resource used per default by 'Text' and 'Attributed Text' views to ensure 
   that new views don't remain empty on the screen. */
EW_DECLARE_FONT_RES( DashboardFontL )

/* Font resource used per default by 'Text' and 'Attributed Text' views to ensure 
   that new views don't remain empty on the screen. */
EW_DECLARE_FONT_RES( DashboardFontXXL )

/* Font resource used per default by 'Text' and 'Attributed Text' views to ensure 
   that new views don't remain empty on the screen. */
EW_DECLARE_FONT_RES( DashboardFontXL )

/* Font resource used per default by 'Text' and 'Attributed Text' views to ensure 
   that new views don't remain empty on the screen. */
EW_DECLARE_FONT_RES( DashboardFontLightL )

/* Font resource used per default by 'Text' and 'Attributed Text' views to ensure 
   that new views don't remain empty on the screen. */
EW_DECLARE_FONT_RES( DashboardFontLightXL )

/* Font resource used per default by 'Text' and 'Attributed Text' views to ensure 
   that new views don't remain empty on the screen. */
EW_DECLARE_FONT_RES( DashboardFontLightM )

/* Forward declaration of the class Effects::FadeInOutTransition */
#ifndef _EffectsFadeInOutTransition_
  EW_DECLARE_CLASS( EffectsFadeInOutTransition )
#define _EffectsFadeInOutTransition_
#endif

/* Forward declaration of the class WidgetSet::PushButtonConfig */
#ifndef _WidgetSetPushButtonConfig_
  EW_DECLARE_CLASS( WidgetSetPushButtonConfig )
#define _WidgetSetPushButtonConfig_
#endif

/* Forward declaration of the class WidgetSet::ToggleButtonConfig */
#ifndef _WidgetSetToggleButtonConfig_
  EW_DECLARE_CLASS( WidgetSetToggleButtonConfig )
#define _WidgetSetToggleButtonConfig_
#endif

/* Forward declaration of the class WidgetSet::ValueDisplayConfig */
#ifndef _WidgetSetValueDisplayConfig_
  EW_DECLARE_CLASS( WidgetSetValueDisplayConfig )
#define _WidgetSetValueDisplayConfig_
#endif

/* User defined auto object: 'Dashboard::Colors' */
EW_DECLARE_AUTOOBJECT( DashboardColors, DashboardColorsClass )

/* User defined auto object: 'Dashboard::Device' */
EW_DECLARE_AUTOOBJECT( DashboardDevice, DashboardDeviceClass )

/* User defined auto object: 'Dashboard::NavigationHintConfig' */
EW_DECLARE_AUTOOBJECT( DashboardNavigationHintConfig, WidgetSetValueDisplayConfig )

/* User defined auto object: 'Dashboard::Animation' */
EW_DECLARE_AUTOOBJECT( DashboardAnimation, DashboardAnimationClass )

/* User defined auto object: 'Dashboard::LedButtonOrangeConfig' */
EW_DECLARE_AUTOOBJECT( DashboardLedButtonOrangeConfig, WidgetSetToggleButtonConfig )

/* User defined auto object: 'Dashboard::RangeConfig' */
EW_DECLARE_AUTOOBJECT( DashboardRangeConfig, WidgetSetValueDisplayConfig )

/* User defined auto object: 'Dashboard::LedButtonGreenConfig' */
EW_DECLARE_AUTOOBJECT( DashboardLedButtonGreenConfig, WidgetSetToggleButtonConfig )

/* User defined auto object: 'Dashboard::StartEngineButtonConfig' */
EW_DECLARE_AUTOOBJECT( DashboardStartEngineButtonConfig, WidgetSetPushButtonConfig )

/* User defined auto object: 'Dashboard::DashboardButtonConfig' */
EW_DECLARE_AUTOOBJECT( DashboardDashboardButtonConfig, WidgetSetPushButtonConfig )

/* User defined auto object: 'Dashboard::FadeInOutBottom' */
EW_DECLARE_AUTOOBJECT( DashboardFadeInOutBottom, EffectsFadeInOutTransition )

/* User defined auto object: 'Dashboard::FadeInOutCentered' */
EW_DECLARE_AUTOOBJECT( DashboardFadeInOutCentered, EffectsFadeInOutTransition )

#ifdef __cplusplus
  }
#endif

#endif /* Dashboard_H */

/* Embedded Wizard */
