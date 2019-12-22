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

#ifndef _EffectsEffect_H
#define _EffectsEffect_H

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

/* Forward declaration of the class Core::Timer */
#ifndef _CoreTimer_
  EW_DECLARE_CLASS( CoreTimer )
#define _CoreTimer_
#endif

/* Forward declaration of the class Effects::Effect */
#ifndef _EffectsEffect_
  EW_DECLARE_CLASS( EffectsEffect )
#define _EffectsEffect_
#endif

/* Forward declaration of the class Effects::TimingList */
#ifndef _EffectsTimingList_
  EW_DECLARE_CLASS( EffectsTimingList )
#define _EffectsTimingList_
#endif


/* The class Effects::Effect provides the base functionality for all kinds of animation 
   effects. Effects allow the designer to enrich the GUI application with additional 
   fancy and eye candy animations. For example, the effect Effects::ColorEffect 
   performs the color fade animation. Effects::RectEffect in contrast can animate 
   the position and the size of a GUI component, etc. Animation effects are determined 
   by their duration and by the timing function.
   The duration is primarily controlled by the value of the property @CycleDuration. 
   This is the time the effect will take for a single animation run. Whether and 
   how often the animation is repeated is stored in the property @NoOfCycles. In 
   this case the property @InterCycleDelay can determine an optional interval to 
   wait between two consecutive animation runs. Finally the property @InitialDelay 
   determines the interval to wait at the very beginning of the effect still before 
   the first animation run began.
   The timing function determines the curve to run the animation. The class implements 
   an extensive set of various timing functions. In its simplest case the animation 
   can follow a straight line, or it can start slow and get faster afterwards. More 
   complex timings perform realistic spring elastic and bounce animations. Besides 
   it the user can configure his/her own animation curve. In such case the timing 
   function is based on a cubic Bezier curve. It gives the designer a lot of possibilities 
   to specify very fancy timing functions. The timing is controlled by the property 
   @Timing. In the case the designer wants to specify a new curve, the properties 
   @TimingCustom1 and @TimingCustom2 are available. With the property @Noise an 
   additional random noise can be added to the effect.
   During the runtime of the animation, the effect will send signals to the slot 
   method stored in the property @OnAnimate. When the effect is finished a signal 
   is send to the slot method stored in the @OnFinished property.
   Whether the effect is running or not is controlled by its property @Enabled. 
   It can be used to start and stop the effect. The effect can also be controlled 
   by sending signals to its @StartEffect and @StopEffect slot methods. With the 
   property @Reversed the playback direction can be determined. This allows to simply 
   rewind running effects or to configure an effect to be played in the reverse 
   direction. The slot method @ReverseEffect can also be used to switch the playback 
   direction. */
EW_DEFINE_FIELDS( EffectsEffect, XObject )
  EW_VARIABLE( timingList,      EffectsTimingList )
  EW_VARIABLE( timer,           CoreTimer )
  EW_VARIABLE( privateOnFinished, XSlot )
  EW_PROPERTY( OnFinished,      XSlot )
  EW_PROPERTY( OnAnimate,       XSlot )
  EW_VARIABLE( direction,       XInt32 )
  EW_VARIABLE( startDirection,  XInt32 )
  EW_VARIABLE( curveFactor2,    XFloat )
  EW_VARIABLE( curveFactor1,    XFloat )
  EW_VARIABLE( invCycleDuration, XFloat )
  EW_VARIABLE( lastFrame,       XFloat )
  EW_VARIABLE( cycleCounter,    XInt32 )
  EW_VARIABLE( startTime,       XUInt32 )
  EW_PROPERTY( Elasticity,      XFloat )
  EW_PROPERTY( Bounces,         XInt32 )
  EW_PROPERTY( Oscillations,    XInt32 )
  EW_PROPERTY( Amplitude,       XFloat )
  EW_PROPERTY( Exponent,        XFloat )
  EW_PROPERTY( Noise,           XFloat )
  EW_PROPERTY( TimingCustom2,   XFloat )
  EW_PROPERTY( TimingCustom1,   XFloat )
  EW_PROPERTY( NoOfCycles,      XInt32 )
  EW_PROPERTY( CycleDuration,   XInt32 )
  EW_PROPERTY( InterCycleDelay, XInt32 )
  EW_PROPERTY( InitialDelay,    XInt32 )
  EW_PROPERTY( Timing,          XEnum )
  EW_PROPERTY( Enabled,         XBool )
  EW_PROPERTY( Symmetric,       XBool )
  EW_PROPERTY( Reversed,        XBool )
  EW_VARIABLE( useBezier2,      XBool )
  EW_VARIABLE( useBezier3,      XBool )
EW_END_OF_FIELDS( EffectsEffect )

/* Virtual Method Table (VMT) for the class : 'Effects::Effect' */
EW_DEFINE_METHODS( EffectsEffect, XObject )
  EW_METHOD( Animate,           void )( EffectsEffect _this, XFloat aProgress )
EW_END_OF_METHODS( EffectsEffect )

/* 'C' function for method : 'Effects::Effect.timerSlot()' */
void EffectsEffect_timerSlot( EffectsEffect _this, XObject sender );

/* 'C' function for method : 'Effects::Effect.run()' */
void EffectsEffect_run( EffectsEffect _this, XFloat aFrame );

/* 'C' function for method : 'Effects::Effect.runRevRev()' */
XBool EffectsEffect_runRevRev( EffectsEffect _this );

/* 'C' function for method : 'Effects::Effect.runRevFwd()' */
XBool EffectsEffect_runRevFwd( EffectsEffect _this );

/* 'C' function for method : 'Effects::Effect.runFwdRev()' */
XBool EffectsEffect_runFwdRev( EffectsEffect _this );

/* 'C' function for method : 'Effects::Effect.runFwdFwd()' */
XBool EffectsEffect_runFwdFwd( EffectsEffect _this );

/* 'C' function for method : 'Effects::Effect.OnSetReversed()' */
void EffectsEffect_OnSetReversed( EffectsEffect _this, XBool value );

/* 'C' function for method : 'Effects::Effect.OnSetElasticity()' */
void EffectsEffect_OnSetElasticity( EffectsEffect _this, XFloat value );

/* 'C' function for method : 'Effects::Effect.OnSetBounces()' */
void EffectsEffect_OnSetBounces( EffectsEffect _this, XInt32 value );

/* 'C' function for method : 'Effects::Effect.OnSetOscillations()' */
void EffectsEffect_OnSetOscillations( EffectsEffect _this, XInt32 value );

/* 'C' function for method : 'Effects::Effect.OnSetAmplitude()' */
void EffectsEffect_OnSetAmplitude( EffectsEffect _this, XFloat value );

/* 'C' function for method : 'Effects::Effect.OnSetExponent()' */
void EffectsEffect_OnSetExponent( EffectsEffect _this, XFloat value );

/* 'C' function for method : 'Effects::Effect.OnSetTimingCustom2()' */
void EffectsEffect_OnSetTimingCustom2( EffectsEffect _this, XFloat value );

/* 'C' function for method : 'Effects::Effect.OnSetTimingCustom1()' */
void EffectsEffect_OnSetTimingCustom1( EffectsEffect _this, XFloat value );

/* 'C' function for method : 'Effects::Effect.OnSetTiming()' */
void EffectsEffect_OnSetTiming( EffectsEffect _this, XEnum value );

/* 'C' function for method : 'Effects::Effect.OnSetNoOfCycles()' */
void EffectsEffect_OnSetNoOfCycles( EffectsEffect _this, XInt32 value );

/* 'C' function for method : 'Effects::Effect.OnSetCycleDuration()' */
void EffectsEffect_OnSetCycleDuration( EffectsEffect _this, XInt32 value );

/* 'C' function for method : 'Effects::Effect.OnSetEnabled()' */
void EffectsEffect_OnSetEnabled( EffectsEffect _this, XBool value );

/* 'C' function for method : 'Effects::Effect.Animate()' */
void EffectsEffect_Animate( EffectsEffect _this, XFloat aProgress );

/* Wrapper function for the virtual method : 'Effects::Effect.Animate()' */
void EffectsEffect__Animate( void* _this, XFloat aProgress );

/* The slot method 'StartEffect' re-starts the effect if a signal is sent to this 
   slot method. The effect will start from the beginning. */
void EffectsEffect_StartEffect( EffectsEffect _this, XObject sender );

#ifdef __cplusplus
  }
#endif

#endif /* _EffectsEffect_H */

/* Embedded Wizard */
