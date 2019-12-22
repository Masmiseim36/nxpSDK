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

#ifndef _EffectsTransition_H
#define _EffectsTransition_H

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

/* Forward declaration of the class Effects::Fader */
#ifndef _EffectsFader_
  EW_DECLARE_CLASS( EffectsFader )
#define _EffectsFader_
#endif

/* Forward declaration of the class Effects::Transition */
#ifndef _EffectsTransition_
  EW_DECLARE_CLASS( EffectsTransition )
#define _EffectsTransition_
#endif


/* The class Effects::Transition provides the infrastructure to implement sophisticated 
   transitions for fading-in/out GUI components. At the runtime, you associate instances 
   of the desired transition classes with the affected GUI component and determine 
   so the desired animations to perform when the component is presented, dismissed 
   or when the user switches between components forth and back. For more see the 
   methods Core::Group.PresentDialog(), Core::Group.DismissDialog() and Core::Group.SwitchToDialog(). 
   The infrastructure consists of:
   - The method @CreatePresentFader(), which has the job to create and configure 
   a fader (an instance of a class descending from Effects::Fader class) to perform 
   the desired present (fade-in) transition.
   - The method @CreateDismissFader(), which has the job to create and configure 
   a fader (an instance of a class descending from Effects::Fader class) to perform 
   the desired dismiss (fade-out) transition.
   - The method @CreateOverlayFader(), which has the job to create and configure 
   a fader (an instance of a class descending from Effects::Fader class) to perform 
   the desired overlay (temporarily fade-out) transition.
   - The method @CreateRestoreFader(), which has the job to create and configure 
   a fader (an instance of a class descending from Effects::Fader class) to perform 
   the desired restore (fade-in) transition after the preceding overlay transition.
   The class Effects::Transition and all classes descending from it can be considered 
   as kind of factories to conveniently create and configure often used fader objects. 
   The default implementation of Effects::Transition does nothing. Derived classes 
   has to override and implement at least the both methods @CreatePresentFader() 
   and @CreateDismissFader(). */
EW_DEFINE_FIELDS( EffectsTransition, XObject )
EW_END_OF_FIELDS( EffectsTransition )

/* Virtual Method Table (VMT) for the class : 'Effects::Transition' */
EW_DEFINE_METHODS( EffectsTransition, XObject )
  EW_METHOD( CreatePresentFader, EffectsFader )( EffectsTransition _this )
  EW_METHOD( CreateDismissFader, EffectsFader )( EffectsTransition _this )
  EW_METHOD( CreateRestoreFader, EffectsFader )( EffectsTransition _this )
  EW_METHOD( CreateOverlayFader, EffectsFader )( EffectsTransition _this )
EW_END_OF_METHODS( EffectsTransition )

/* The method CreatePresentFader() creates an object of one of the classes descending 
   from Effects::Fader and configures it with parameters according to the desired 
   fade-in (present) transition. Finally the prepared fader object is returned to 
   the caller. Typically, the created fader is used to show (to present) a GUI component 
   when the user navigates to it. The method has to be overridden and implemented 
   in derived classes. The actual implementation does nothing and returns 'null'. */
EffectsFader EffectsTransition_CreatePresentFader( EffectsTransition _this );

/* Wrapper function for the virtual method : 'Effects::Transition.CreatePresentFader()' */
EffectsFader EffectsTransition__CreatePresentFader( void* _this );

/* The method CreateDismissFader() creates an object of one of the classes descending 
   from Effects::Fader and configures it with parameters according to the desired 
   fade-out (dismiss) transition. Finally the prepared fader object is returned 
   to the caller. Typically, the created fader is used to hide (to dismiss) a GUI 
   component when the user leaves it while he/she navigates in the GUI system. The 
   method has to be overridden and implemented in derived classes. The actual implementation 
   does nothing and returns 'null'. */
EffectsFader EffectsTransition_CreateDismissFader( EffectsTransition _this );

/* Wrapper function for the virtual method : 'Effects::Transition.CreateDismissFader()' */
EffectsFader EffectsTransition__CreateDismissFader( void* _this );

/* The method CreateRestoreFader() creates an object of one of the classes descending 
   from Effects::Fader and configures it with parameters according to the desired 
   fade-in (restore) transition. Finally the prepared fader object is returned to 
   the caller. Typically, the created fader is used to show (to restore the original 
   visible state) of a GUI component, which has been obscured (overlaid) by other 
   GUI components during the user navigates in the GUI system. The method has to 
   be overridden and implemented in derived classes. The actual implementation of 
   this method redirects the operation to the method @CreatePresentFader(). */
EffectsFader EffectsTransition_CreateRestoreFader( EffectsTransition _this );

/* Wrapper function for the virtual method : 'Effects::Transition.CreateRestoreFader()' */
EffectsFader EffectsTransition__CreateRestoreFader( void* _this );

/* The method CreateOverlayFader() creates an object of one of the classes descending 
   from Effects::Fader and configures it with parameters according to the desired 
   fade-out (overlay) transition. Finally the prepared fader object is returned 
   to the caller. Typically, the created fader is used to temporarily hide (to overlay) 
   a GUI component when the user navigates a deeper level in the GUI system. The 
   method has to be overridden and implemented in derived classes. The actual implementation 
   of this method redirects the operation to the method @CreateDismissFader(). */
EffectsFader EffectsTransition_CreateOverlayFader( EffectsTransition _this );

/* Wrapper function for the virtual method : 'Effects::Transition.CreateOverlayFader()' */
EffectsFader EffectsTransition__CreateOverlayFader( void* _this );

#ifdef __cplusplus
  }
#endif

#endif /* _EffectsTransition_H */

/* Embedded Wizard */
