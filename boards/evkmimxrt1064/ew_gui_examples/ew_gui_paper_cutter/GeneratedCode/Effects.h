/*******************************************************************************
*
* E M B E D D E D   W I Z A R D   P R O J E C T
*
*                                                Copyright (c) TARA Systems GmbH
*                                    written by Paul Banach and Manfred Schweyer
*
********************************************************************************
*
* This file was generated automatically by Embedded Wizard Studio.
*
* Please do not make any modifications of this file! The modifications are lost
* when the file is generated again by Embedded Wizard Studio!
*
* The template of this heading text can be found in the file 'head.ewt' in the
* directory 'Platforms' of your Embedded Wizard installation directory. If you
* wish to adapt this text, please copy the template file 'head.ewt' into your
* project directory and edit the copy only. Please avoid any modifications of
* the original template file!
*
* Version  : 9.20
* Profile  : iMX_RT
* Platform : NXP.iMX_RT.RGB565
*
*******************************************************************************/

#ifndef Effects_H
#define Effects_H

#ifdef __cplusplus
  extern "C"
  {
#endif

#include "ewrte.h"
#if EW_RTE_VERSION != 0x00090014
  #error Wrong version of Embedded Wizard Runtime Environment.
#endif

#include "ewgfx.h"
#if EW_GFX_VERSION != 0x00090014
  #error Wrong version of Embedded Wizard Graphics Engine.
#endif

#include "_EffectsEffect.h"
#include "_EffectsEffectTimerClass.h"
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

/* User defined auto object: 'Effects::EffectTimer' */
EW_DECLARE_AUTOOBJECT( EffectsEffectTimer, EffectsEffectTimerClass )

#ifdef __cplusplus
  }
#endif

#endif /* Effects_H */

/* Embedded Wizard */
