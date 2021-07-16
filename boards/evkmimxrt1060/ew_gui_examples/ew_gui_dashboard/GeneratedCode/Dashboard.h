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
#include "_DashboardLeftBottomBar.h"
#include "_DashboardLeftTopBar.h"
#include "_DashboardRightValueBar.h"
#include "_DashboardStartScreen.h"

/* Bitmap resource : 'Dashboard::StartEngine' */
EW_DECLARE_BITMAP_RES( DashboardStartEngine )

/* Bitmap resource : 'Dashboard::StartEngineGloom' */
EW_DECLARE_BITMAP_RES( DashboardStartEngineGloom )

/* Bitmap resource : 'Dashboard::Icons' */
EW_DECLARE_BITMAP_RES( DashboardIcons )

/* Bitmap resource : 'Dashboard::LeftGlowV2' */
EW_DECLARE_BITMAP_RES( DashboardLeftGlowV2 )

/* Bitmap resource : 'Dashboard::RightGlowV2' */
EW_DECLARE_BITMAP_RES( DashboardRightGlowV2 )

/* Bitmap resource : 'Dashboard::BatteryFrame' */
EW_DECLARE_BITMAP_RES( DashboardBatteryFrame )

/* Bitmap resource : 'Dashboard::GaugeNeedle' */
EW_DECLARE_BITMAP_RES( DashboardGaugeNeedle )

/* Bitmap resource : 'Dashboard::Circle0' */
EW_DECLARE_BITMAP_RES( DashboardCircle0 )

/* Bitmap resource : 'Dashboard::Circle2' */
EW_DECLARE_BITMAP_RES( DashboardCircle2 )

/* Bitmap resource : 'Dashboard::Circle3' */
EW_DECLARE_BITMAP_RES( DashboardCircle3 )

/* Bitmap resource : 'Dashboard::Circle4' */
EW_DECLARE_BITMAP_RES( DashboardCircle4 )

/* Bitmap resource : 'Dashboard::Circle5' */
EW_DECLARE_BITMAP_RES( DashboardCircle5 )

/* Bitmap resource : 'Dashboard::GaugeGlowSegment' */
EW_DECLARE_BITMAP_RES( DashboardGaugeGlowSegment )

/* Bitmap resource : 'Dashboard::StartEngineStates' */
EW_DECLARE_BITMAP_RES( DashboardStartEngineStates )

/* Bitmap resource : 'Dashboard::SwitchIcon' */
EW_DECLARE_BITMAP_RES( DashboardSwitchIcon )

/* Bitmap resource : 'Dashboard::OffIcon' */
EW_DECLARE_BITMAP_RES( DashboardOffIcon )

/* Bitmap resource : 'Dashboard::LeftBottomGlowV1' */
EW_DECLARE_BITMAP_RES( DashboardLeftBottomGlowV1 )

/* Bitmap resource : 'Dashboard::RightBottomGlowV1' */
EW_DECLARE_BITMAP_RES( DashboardRightBottomGlowV1 )

/* Bitmap resource : 'Dashboard::LeftMiddleGlowV1' */
EW_DECLARE_BITMAP_RES( DashboardLeftMiddleGlowV1 )

/* Bitmap resource : 'Dashboard::RightMiddleGlowV1' */
EW_DECLARE_BITMAP_RES( DashboardRightMiddleGlowV1 )

/* Bitmap resource : 'Dashboard::TopGlowV1' */
EW_DECLARE_BITMAP_RES( DashboardTopGlowV1 )

/* Bitmap resource : 'Dashboard::CenterGlowV2' */
EW_DECLARE_BITMAP_RES( DashboardCenterGlowV2 )

/* Bitmap resource : 'Dashboard::KWIcon' */
EW_DECLARE_BITMAP_RES( DashboardKWIcon )

/* Bitmap resource : 'Dashboard::PWRIcon' */
EW_DECLARE_BITMAP_RES( DashboardPWRIcon )

/* Bitmap resource : 'Dashboard::LeftBricksV2' */
EW_DECLARE_BITMAP_RES( DashboardLeftBricksV2 )

/* Bitmap resource : 'Dashboard::RightBricksV2' */
EW_DECLARE_BITMAP_RES( DashboardRightBricksV2 )

/* Bitmap resource : 'Dashboard::Logo' */
EW_DECLARE_BITMAP_RES( DashboardLogo )

/* Bitmap resource : 'Dashboard::HorizontalLine' */
EW_DECLARE_BITMAP_RES( DashboardHorizontalLine )

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
EW_DECLARE_FONT_RES( DashboardFontLightM )

/* Font resource used per default by 'Text' and 'Attributed Text' views to ensure 
   that new views don't remain empty on the screen. */
EW_DECLARE_FONT_RES( DashboardFontS )

/* Font resource used per default by 'Text' and 'Attributed Text' views to ensure 
   that new views don't remain empty on the screen. */
EW_DECLARE_FONT_RES( DashboardFontLightS )

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

/* User defined auto object: 'Dashboard::Animation' */
EW_DECLARE_AUTOOBJECT( DashboardAnimation, DashboardAnimationClass )

/* User defined auto object: 'Dashboard::LedButtonOrangeConfig' */
EW_DECLARE_AUTOOBJECT( DashboardLedButtonOrangeConfig, WidgetSetToggleButtonConfig )

/* User defined auto object: 'Dashboard::RangeConfig' */
EW_DECLARE_AUTOOBJECT( DashboardRangeConfig, WidgetSetValueDisplayConfig )

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
