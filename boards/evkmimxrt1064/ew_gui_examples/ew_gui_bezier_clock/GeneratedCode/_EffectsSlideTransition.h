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

#ifndef _EffectsSlideTransition_H
#define _EffectsSlideTransition_H

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

#include "_EffectsTransition.h"

/* Forward declaration of the class Effects::Fader */
#ifndef _EffectsFader_
  EW_DECLARE_CLASS( EffectsFader )
#define _EffectsFader_
#endif

/* Forward declaration of the class Effects::SlideTransition */
#ifndef _EffectsSlideTransition_
  EW_DECLARE_CLASS( EffectsSlideTransition )
#define _EffectsSlideTransition_
#endif


/* The class Effects::SlideTransition provides functionality for the fade-in/out 
   operation affecting the position and the opacity of the given GUI component. 
   By using the property @Direction you determine in which direction the component 
   should be moved during the fade-in/out animation (e.g. up, down, left, right, 
   etc.). When using the transition for the fade-in operation, the GUI component 
   starts outside of the visible area of its owner and continues moving until it 
   reaches the predetermined (per default the center) position of its owner. When 
   using the transition for the fade-out operation, the component is moved until 
   it leaves the visible area of its owner component. This transition is thus ideal 
   wherever one GUI component should smoothly slide-in/out in context of another 
   component.
   Per default when the component is displayed, it is aligned in the center of its 
   owner. This can be modified by using the property @Alignment. Furthermore, with 
   the four properties @MarginLeft, @MarginRight, @MarginTop and @MarginBottom the 
   area to align the component within its owner can be limited.
   How long the transition should take is determined in the property @Duration, 
   which is per default 500 ms. The exact timing (easing) is configured with the 
   property @Timing. The class implements an extensive set of various timing functions. 
   In its simplest case the animation can follow a straight line, or it can start 
   slow and get faster afterwards. More complex timings perform realistic spring 
   elastic and bounce animations. In its default configuration, the transition uses 
   the 'FastIn_EaseOut' timing (it starts fast and slows down until the end position 
   is reached).
   With the property @Buffered you can configure, whether the transition should 
   additionally enable the buffering mode for the affected GUI component while it 
   performs the animation. */
EW_DEFINE_FIELDS( EffectsSlideTransition, EffectsTransition )
  EW_PROPERTY( MarginBottom,    XInt32 )
  EW_PROPERTY( MarginRight,     XInt32 )
  EW_PROPERTY( MarginTop,       XInt32 )
  EW_PROPERTY( MarginLeft,      XInt32 )
  EW_PROPERTY( Alignment,       XSet )
  EW_PROPERTY( Elasticity,      XFloat )
  EW_PROPERTY( Bounces,         XInt32 )
  EW_PROPERTY( Oscillations,    XInt32 )
  EW_PROPERTY( Amplitude,       XFloat )
  EW_PROPERTY( Exponent,        XFloat )
  EW_PROPERTY( TimingCustom2,   XFloat )
  EW_PROPERTY( TimingCustom1,   XFloat )
  EW_PROPERTY( Duration,        XInt32 )
  EW_PROPERTY( Direction,       XEnum )
  EW_PROPERTY( Timing,          XEnum )
  EW_PROPERTY( Buffered,        XBool )
EW_END_OF_FIELDS( EffectsSlideTransition )

/* Virtual Method Table (VMT) for the class : 'Effects::SlideTransition' */
EW_DEFINE_METHODS( EffectsSlideTransition, EffectsTransition )
  EW_METHOD( CreatePresentFader, EffectsFader )( EffectsSlideTransition _this )
  EW_METHOD( CreateDismissFader, EffectsFader )( EffectsSlideTransition _this )
  EW_METHOD( CreateRestoreFader, EffectsFader )( EffectsSlideTransition _this )
  EW_METHOD( CreateOverlayFader, EffectsFader )( EffectsSlideTransition _this )
EW_END_OF_METHODS( EffectsSlideTransition )

/* The method CreatePresentFader() creates an object of one of the classes descending 
   from Effects::Fader and configures it with parameters according to the desired 
   fade-in (present) transition. Finally the prepared fader object is returned to 
   the caller. Typically, the created fader is used to show (to present) a GUI component 
   when the user navigates to it. The method has to be overridden and implemented 
   in derived classes. The actual implementation does nothing and returns 'null'. */
EffectsFader EffectsSlideTransition_CreatePresentFader( EffectsSlideTransition _this );

/* The method CreateDismissFader() creates an object of one of the classes descending 
   from Effects::Fader and configures it with parameters according to the desired 
   fade-out (dismiss) transition. Finally the prepared fader object is returned 
   to the caller. Typically, the created fader is used to hide (to dismiss) a GUI 
   component when the user leaves it while he/she navigates in the GUI system. The 
   method has to be overridden and implemented in derived classes. The actual implementation 
   does nothing and returns 'null'. */
EffectsFader EffectsSlideTransition_CreateDismissFader( EffectsSlideTransition _this );

/* The method CreateRestoreFader() creates an object of one of the classes descending 
   from Effects::Fader and configures it with parameters according to the desired 
   fade-in (restore) transition. Finally the prepared fader object is returned to 
   the caller. Typically, the created fader is used to show (to restore the original 
   visible state) of a GUI component, which has been obscured (overlaid) by other 
   GUI components during the user navigates in the GUI system. The method has to 
   be overridden and implemented in derived classes. The actual implementation of 
   this method redirects the operation to the method @CreatePresentFader(). */
EffectsFader EffectsSlideTransition_CreateRestoreFader( EffectsSlideTransition _this );

/* The method CreateOverlayFader() creates an object of one of the classes descending 
   from Effects::Fader and configures it with parameters according to the desired 
   fade-out (overlay) transition. Finally the prepared fader object is returned 
   to the caller. Typically, the created fader is used to temporarily hide (to overlay) 
   a GUI component when the user navigates a deeper level in the GUI system. The 
   method has to be overridden and implemented in derived classes. The actual implementation 
   of this method redirects the operation to the method @CreateDismissFader(). */
EffectsFader EffectsSlideTransition_CreateOverlayFader( EffectsSlideTransition _this );

/* 'C' function for method : 'Effects::SlideTransition.onInitializeIn()' */
void EffectsSlideTransition_onInitializeIn( EffectsSlideTransition _this, XObject 
  sender );

/* 'C' function for method : 'Effects::SlideTransition.onInitializeOut()' */
void EffectsSlideTransition_onInitializeOut( EffectsSlideTransition _this, XObject 
  sender );

#ifdef __cplusplus
  }
#endif

#endif /* _EffectsSlideTransition_H */

/* Embedded Wizard */
