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

#ifndef _EffectsShowHideTransition_H
#define _EffectsShowHideTransition_H

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

#include "_EffectsTransition.h"

/* Forward declaration of the class Effects::Fader */
#ifndef _EffectsFader_
  EW_DECLARE_CLASS( EffectsFader )
#define _EffectsFader_
#endif

/* Forward declaration of the class Effects::ShowHideTransition */
#ifndef _EffectsShowHideTransition_
  EW_DECLARE_CLASS( EffectsShowHideTransition )
#define _EffectsShowHideTransition_
#endif


/* The class Effects::ShowHideTransition provides functionality for the most primitive 
   fade-in/out operation affecting the pure visibility of a given GUI component. 
   When using the transition for the fade-in operation, the GUI component is automatically 
   arranged to appear (per default centered) within the area of its owner component. 
   This transition is thus ideal wherever one GUI component should instantly appear 
   or disappear in context of another component without any animation effects.
   Per default when the component is displayed, it is aligned in the center of its 
   owner. This can be modified by using the property @Alignment. Furthermore, with 
   the four properties @MarginLeft, @MarginRight, @MarginTop and @MarginBottom the 
   area to align the component within its owner can be limited.
   Please note, with this transition the fade-in operation is performed just at 
   the beginning of all other transitions running simultaneously, in other words, 
   the GUI component appears during the start phase of the transition. In turn, 
   to fade-out the component, the operation is executed during the end phase of 
   the transition. This behavior can be modified by using the properties @ShowAtStart 
   and @HideAtStart. */
EW_DEFINE_FIELDS( EffectsShowHideTransition, EffectsTransition )
  EW_PROPERTY( MarginBottom,    XInt32 )
  EW_PROPERTY( MarginRight,     XInt32 )
  EW_PROPERTY( MarginTop,       XInt32 )
  EW_PROPERTY( MarginLeft,      XInt32 )
  EW_PROPERTY( Alignment,       XSet )
  EW_PROPERTY( ShowAtStart,     XBool )
  EW_PROPERTY( HideAtStart,     XBool )
EW_END_OF_FIELDS( EffectsShowHideTransition )

/* Virtual Method Table (VMT) for the class : 'Effects::ShowHideTransition' */
EW_DEFINE_METHODS( EffectsShowHideTransition, EffectsTransition )
  EW_METHOD( CreatePresentFader, EffectsFader )( EffectsShowHideTransition _this )
  EW_METHOD( CreateDismissFader, EffectsFader )( EffectsShowHideTransition _this )
  EW_METHOD( CreateRestoreFader, EffectsFader )( EffectsShowHideTransition _this )
  EW_METHOD( CreateOverlayFader, EffectsFader )( EffectsShowHideTransition _this )
EW_END_OF_METHODS( EffectsShowHideTransition )

/* The method CreatePresentFader() creates an object of one of the classes descending 
   from Effects::Fader and configures it with parameters according to the desired 
   fade-in (present) transition. Finally the prepared fader object is returned to 
   the caller. Typically, the created fader is used to show (to present) a GUI component 
   when the user navigates to it. The method has to be overridden and implemented 
   in derived classes. The actual implementation does nothing and returns 'null'. */
EffectsFader EffectsShowHideTransition_CreatePresentFader( EffectsShowHideTransition _this );

/* The method CreateDismissFader() creates an object of one of the classes descending 
   from Effects::Fader and configures it with parameters according to the desired 
   fade-out (dismiss) transition. Finally the prepared fader object is returned 
   to the caller. Typically, the created fader is used to hide (to dismiss) a GUI 
   component when the user leaves it while he/she navigates in the GUI system. The 
   method has to be overridden and implemented in derived classes. The actual implementation 
   does nothing and returns 'null'. */
EffectsFader EffectsShowHideTransition_CreateDismissFader( EffectsShowHideTransition _this );

/* The method CreateRestoreFader() creates an object of one of the classes descending 
   from Effects::Fader and configures it with parameters according to the desired 
   fade-in (restore) transition. Finally the prepared fader object is returned to 
   the caller. Typically, the created fader is used to show (to restore the original 
   visible state) of a GUI component, which has been obscured (overlaid) by other 
   GUI components during the user navigates in the GUI system. The method has to 
   be overridden and implemented in derived classes. The actual implementation of 
   this method redirects the operation to the method @CreatePresentFader(). */
EffectsFader EffectsShowHideTransition_CreateRestoreFader( EffectsShowHideTransition _this );

/* The method CreateOverlayFader() creates an object of one of the classes descending 
   from Effects::Fader and configures it with parameters according to the desired 
   fade-out (overlay) transition. Finally the prepared fader object is returned 
   to the caller. Typically, the created fader is used to temporarily hide (to overlay) 
   a GUI component when the user navigates a deeper level in the GUI system. The 
   method has to be overridden and implemented in derived classes. The actual implementation 
   of this method redirects the operation to the method @CreateDismissFader(). */
EffectsFader EffectsShowHideTransition_CreateOverlayFader( EffectsShowHideTransition _this );

/* 'C' function for method : 'Effects::ShowHideTransition.onInitializeIn()' */
void EffectsShowHideTransition_onInitializeIn( EffectsShowHideTransition _this, 
  XObject sender );

#ifdef __cplusplus
  }
#endif

#endif /* _EffectsShowHideTransition_H */

/* Embedded Wizard */
