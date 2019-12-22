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

#ifndef _EffectsEffectTimerClass_H
#define _EffectsEffectTimerClass_H

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

#include "_CoreTimer.h"

/* Forward declaration of the class Effects::EffectTimerClass */
#ifndef _EffectsEffectTimerClass_
  EW_DECLARE_CLASS( EffectsEffectTimerClass )
#define _EffectsEffectTimerClass_
#endif


/* Deklaration of class : 'Effects::EffectTimerClass' */
EW_DEFINE_FIELDS( EffectsEffectTimerClass, CoreTimer )
EW_END_OF_FIELDS( EffectsEffectTimerClass )

/* Virtual Method Table (VMT) for the class : 'Effects::EffectTimerClass' */
EW_DEFINE_METHODS( EffectsEffectTimerClass, CoreTimer )
  EW_METHOD( Trigger,           void )( EffectsEffectTimerClass _this )
EW_END_OF_METHODS( EffectsEffectTimerClass )

/* The method Trigger() will be invoked when the timer is expired (when the interval 
   defined in @Begin or @Period is elapsed). The method can be overridden and implemented 
   in derived classes. The default implementation of this method sends a signal 
   to the slot method stored in the @OnTrigger property. */
void EffectsEffectTimerClass_Trigger( EffectsEffectTimerClass _this );

#ifdef __cplusplus
  }
#endif

#endif /* _EffectsEffectTimerClass_H */

/* Embedded Wizard */
