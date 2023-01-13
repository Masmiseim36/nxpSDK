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

#ifndef _EffectsVisibilityFader_H
#define _EffectsVisibilityFader_H

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

#include "_EffectsFader.h"

/* Forward declaration of the class Core::Group */
#ifndef _CoreGroup_
  EW_DECLARE_CLASS( CoreGroup )
#define _CoreGroup_
#endif

/* Forward declaration of the class Effects::FaderTask */
#ifndef _EffectsFaderTask_
  EW_DECLARE_CLASS( EffectsFaderTask )
#define _EffectsFaderTask_
#endif

/* Forward declaration of the class Effects::VisibilityFader */
#ifndef _EffectsVisibilityFader_
  EW_DECLARE_CLASS( EffectsVisibilityFader )
#define _EffectsVisibilityFader_
#endif


/* The class Effects::VisibilityFader provides functionality for the most primitive 
   fade-in and fade-out transitions affecting the pure visibility of a given GUI 
   component. This fader is thus ideal wherever one GUI component should instantly 
   appear or disappear in context of another component. In fact, this version of 
   a fader does not implement any animation effects to smoothly change the state 
   of the affected component.
   With the property @Visible you configure, whether the fader should show or hide 
   the GUI component. When you intend to show the GUI component, you can additionally 
   specify in the property @Position where it should appear. In such case set the 
   property @UseCurrentState to the value 'false'. If the property @UseCurrentState 
   is 'true', the GUI component will appear at position, the component stores already 
   in its own Bounds property.
   The fader automatically takes care of adding the GUI component to the intended 
   owner, when the component should appear and it is not yet a member within the 
   owner. Moreover, when showing the component, the fader automatically focuses 
   it and restacks it to appear above all other sibling views existing already within 
   the owner component. This behavior can be configured by modifying the properties 
   @AssignFocus and @RestackTopmost.
   The fader automatically removes the GUI component from its owner, when the component 
   disappears. This behavior can be changed by modifying the property @RemoveIfHidden.
   Please note, when the fader is configured to fade-in (show) the GUI component, 
   the affected operation is performed just at the beginning of all other transitions 
   running simultaneously, in other words, during the start phase of the transition. 
   In turn, if the fader is configured to fade-out (hide) the component, the operation 
   is executed during the end phase of the transition. This behavior can be modified 
   by using the properties @ShowAtStart and @HideAtStart.
   Applying the fade-out transition on a GUI component, which is already invisible 
   has no effect. Similarly, the fade-in transition has no effect if the GUI component 
   is already visible at the specified @Position. In both cases the affected component 
   retains its actual state.
   In practice, you create an instance of the fader class, configure its properties 
   (in particular the property @Visible) according to the desired kind of transition 
   and pass both, the fader instance and the affected GUI component as parameters 
   in the invocation of the method Core::Group.FadeGroup(). */
EW_DEFINE_FIELDS( EffectsVisibilityFader, EffectsFader )
  EW_PROPERTY( Position,        XPoint )
  EW_PROPERTY( ShowAtStart,     XBool )
  EW_PROPERTY( HideAtStart,     XBool )
  EW_VARIABLE( finished,        XBool )
EW_END_OF_FIELDS( EffectsVisibilityFader )

/* Virtual Method Table (VMT) for the class : 'Effects::VisibilityFader' */
EW_DEFINE_METHODS( EffectsVisibilityFader, EffectsFader )
  EW_METHOD( IsFinished,        XBool )( EffectsVisibilityFader _this )
  EW_METHOD( OnEnd,             void )( EffectsVisibilityFader _this )
  EW_METHOD( OnStart,           void )( EffectsVisibilityFader _this )
EW_END_OF_METHODS( EffectsVisibilityFader )

/* The method IsFinished() should return 'true' if the fader has finalized its fading 
   animation and 'false' if the animation is not started or it is still in progress. 
   This method should therefore be overridden in derived classes to evaluate the 
   state of animation effects used inside the fader. */
XBool EffectsVisibilityFader_IsFinished( EffectsVisibilityFader _this );

/* The method OnEnd() is invoked automatically just in the moment, when this fader 
   and all other faders belonging to the same fading task have signaled that they 
   are finished with their animations. The default implementation of this method 
   simply does nothing.
   Derived classes can override this method and implement there the code to complete 
   the animation (e.g. to remove the group from its owner after it has been faded-out, 
   etc.). The implementation of the overridden OnEnd() method has to use the variables 
   @Group and @Owner to determine on which group the fader has applied the animation. 
   Depending on the kind of the implemented animation, the OnEnd() method can also 
   evaluate the properties @UseCurrentState, @AssignFocus and @RestackTopmost. */
void EffectsVisibilityFader_OnEnd( EffectsVisibilityFader _this );

/* The method OnStart() is invoked automatically just in the moment, when the fader 
   is triggered to begin its animation. The default implementation of this method 
   simply calls the @Complete() method indicating so, that the fader is done with 
   its work. Thereupon the next fader waiting for its execution can start.
   Derived classes should override this method and implement there the code to prepare 
   and start the desired fade-in/out animation effects. The implementation of the 
   overridden OnStart() method has to use the variables @Group and @Owner to determine 
   on which group the fader should apply the animation. Depending on the kind of 
   the implemented animation, the OnStart() method can also evaluate the properties 
   @UseCurrentState, @AssignFocus and @RestackTopmost.
   Please note, if there is a slot method associated to the property @OnInitialize, 
   this slot method is signaled shortly before the method OnStart() is executed 
   given the slot method a possibility to apply more individual initialization steps 
   on the fader just before the animation begins.
   When the animation reaches its end (e.g. when the animation effects used inside 
   the fader do terminate), your implementation of the fader should invoke the method 
   @Complete() otherwise the animation is considered as still being in progress 
   causing other pending faders to wait. */
void EffectsVisibilityFader_OnStart( EffectsVisibilityFader _this );

#ifdef __cplusplus
  }
#endif

#endif /* _EffectsVisibilityFader_H */

/* Embedded Wizard */
