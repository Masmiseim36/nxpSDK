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
#include "_EffectsEffectTimerClass.h"
#include "Effects.h"

/* Initializer for the class 'Effects::EffectTimerClass' */
void EffectsEffectTimerClass__Init( EffectsEffectTimerClass _this, XObject aLink, XHandle aArg )
{
  /* At first initialize the super class ... */
  CoreTimer__Init( &_this->_Super, aLink, aArg );

  /* Allow the Immediate Garbage Collection to evalute the members of this class. */
  _this->_GCT = EW_CLASS_GCT( EffectsEffectTimerClass );

  /* Setup the VMT pointer */
  _this->_VMT = EW_CLASS( EffectsEffectTimerClass );
}

/* Re-Initializer for the class 'Effects::EffectTimerClass' */
void EffectsEffectTimerClass__ReInit( EffectsEffectTimerClass _this )
{
  /* At first re-initialize the super class ... */
  CoreTimer__ReInit( &_this->_Super );
}

/* Finalizer method for the class 'Effects::EffectTimerClass' */
void EffectsEffectTimerClass__Done( EffectsEffectTimerClass _this )
{
  /* Finalize this class */
  _this->_Super._VMT = EW_CLASS( CoreTimer );

  /* Don't forget to deinitialize the super class ... */
  CoreTimer__Done( &_this->_Super );
}

/* The method Trigger() will be invoked when the timer is expired (when the interval 
   defined in @Begin or @Period is elapsed). The method can be overridden and implemented 
   in derived classes. The default implementation of this method sends a signal 
   to the slot method stored in the @OnTrigger property. */
void EffectsEffectTimerClass_Trigger( EffectsEffectTimerClass _this )
{
  CoreTimer_Trigger((CoreTimer)_this );
  EwNotifyObjObservers((XObject)_this, 0 );
}

/* Variants derived from the class : 'Effects::EffectTimerClass' */
EW_DEFINE_CLASS_VARIANTS( EffectsEffectTimerClass )
EW_END_OF_CLASS_VARIANTS( EffectsEffectTimerClass )

/* Virtual Method Table (VMT) for the class : 'Effects::EffectTimerClass' */
EW_DEFINE_CLASS( EffectsEffectTimerClass, CoreTimer, _None, _None, _None, _None, 
                 _None, _None, "Effects::EffectTimerClass" )
  EffectsEffectTimerClass_Trigger,
EW_END_OF_CLASS( EffectsEffectTimerClass )

/* The global autoobject Effects::EffectTimer triggers all actually active animation 
   effects. In this way all effects will run simultanously. Per default the timer 
   is configured with 15 ms period (~60 FPS). By overriding the Effects::EffectTimer 
   autoobject by a variant you can specify another resolution to drive the animations. */
EW_DEFINE_AUTOOBJECT( EffectsEffectTimer, EffectsEffectTimerClass )

/* Initializer for the auto object 'Effects::EffectTimer' */
void EffectsEffectTimer__Init( EffectsEffectTimerClass _this )
{
  CoreTimer_OnSetPeriod((CoreTimer)_this, 15 );
  CoreTimer_OnSetEnabled((CoreTimer)_this, 1 );
}

/* Table with links to derived variants of the auto object : 'Effects::EffectTimer' */
EW_DEFINE_AUTOOBJECT_VARIANTS( EffectsEffectTimer )
EW_END_OF_AUTOOBJECT_VARIANTS( EffectsEffectTimer )

/* Embedded Wizard */
