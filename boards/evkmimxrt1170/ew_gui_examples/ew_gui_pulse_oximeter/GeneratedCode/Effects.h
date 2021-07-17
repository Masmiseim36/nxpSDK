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

#ifndef Effects_H
#define Effects_H

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

#include "_EffectsEffect.h"
#include "_EffectsEffectTimerClass.h"
#include "_EffectsFloatEffect.h"
#include "_EffectsInt32Effect.h"
#include "_EffectsRectEffect.h"
#include "_EffectsTimingList.h"

/* The definition Effects::Timing determines the available timing functions for 
   animation effects. Except the 'Custom' item, all items of this definition represent 
   build-in timing functions. These can be simply selected in the effect property 
   Timing. */
typedef enum
{
  EffectsTimingLinear                   = 0,
  EffectsTimingPower_In                 = 1,
  EffectsTimingPower_Out                = 2,
  EffectsTimingPower_InOut              = 3,
  EffectsTimingExp_In                   = 4,
  EffectsTimingExp_Out                  = 5,
  EffectsTimingExp_InOut                = 6,
  EffectsTimingSine_In                  = 7,
  EffectsTimingSine_Out                 = 8,
  EffectsTimingSine_InOut               = 9,
  EffectsTimingCircle_In                = 10,
  EffectsTimingCircle_Out               = 11,
  EffectsTimingCircle_InOut             = 12,
  EffectsTimingBack_In                  = 13,
  EffectsTimingBack_Out                 = 14,
  EffectsTimingBack_InOut               = 15,
  EffectsTimingElastic_In               = 16,
  EffectsTimingElastic_Out              = 17,
  EffectsTimingElastic_InOut            = 18,
  EffectsTimingBounce_In                = 19,
  EffectsTimingBounce_Out               = 20,
  EffectsTimingBounce_InOut             = 21,
  EffectsTimingEaseIn_FastOut           = 22,
  EffectsTimingFastIn_EaseOut           = 23,
  EffectsTimingEaseIn_EaseOut           = 24,
  EffectsTimingFastIn_FastOut           = 25,
  EffectsTimingCustom                   = 26
} EffectsTiming;

/* The global autoobject Effects::EffectTimer triggers all actually active animation 
   effects. In this way all effects will run simultanously. Per default the timer 
   is configured with 15 ms period (~60 FPS). By overriding the Effects::EffectTimer 
   autoobject by a variant you can specify another resolution to drive the animations. */
EW_DECLARE_AUTOOBJECT( EffectsEffectTimer, EffectsEffectTimerClass )

#ifdef __cplusplus
  }
#endif

#endif /* Effects_H */

/* Embedded Wizard */
