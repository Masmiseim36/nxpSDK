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

#ifndef _DashboardDemoMode_H
#define _DashboardDemoMode_H

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

#include "_EffectsInt32Effect.h"

/* Forward declaration of the class Dashboard::DemoMode */
#ifndef _DashboardDemoMode_
  EW_DECLARE_CLASS( DashboardDemoMode )
#define _DashboardDemoMode_
#endif


/* Deklaration of class : 'Dashboard::DemoMode' */
EW_DEFINE_FIELDS( DashboardDemoMode, XObject )
  EW_OBJECT  ( DemoModeTimer,   EffectsInt32Effect )
  EW_OBJECT  ( SpeedEffect,     EffectsInt32Effect )
  EW_OBJECT  ( PowerEffect,     EffectsInt32Effect )
  EW_OBJECT  ( BatteryEffect,   EffectsInt32Effect )
  EW_OBJECT  ( ODOEffect,       EffectsInt32Effect )
  EW_VARIABLE( DemoStep,        XInt32 )
EW_END_OF_FIELDS( DashboardDemoMode )

/* Virtual Method Table (VMT) for the class : 'Dashboard::DemoMode' */
EW_DEFINE_METHODS( DashboardDemoMode, XObject )
EW_END_OF_METHODS( DashboardDemoMode )

/* 'C' function for method : 'Dashboard::DemoMode.onDemoModeCycleFinished()' */
void DashboardDemoMode_onDemoModeCycleFinished( DashboardDemoMode _this, XObject 
  sender );

/* 'C' function for method : 'Dashboard::DemoMode.StartEffect()' */
void DashboardDemoMode_StartEffect( DashboardDemoMode _this, EffectsInt32Effect 
  aEffect, XInt32 aValue1, XInt32 aValue2, XEnum aTiming, XInt32 aCycleDuration );

/* The method Init() is invoked automatically after the component has been created. 
   This method can be overridden and filled with logic containing additional initialization 
   statements. */
void DashboardDemoMode_Init( DashboardDemoMode _this, XHandle aArg );

/* 'C' function for method : 'Dashboard::DemoMode.onDemoModeChanged()' */
void DashboardDemoMode_onDemoModeChanged( DashboardDemoMode _this, XObject sender );

#ifdef __cplusplus
  }
#endif

#endif /* _DashboardDemoMode_H */

/* Embedded Wizard */
