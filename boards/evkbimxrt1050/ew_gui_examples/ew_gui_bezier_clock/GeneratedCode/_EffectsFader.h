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

#ifndef _EffectsFader_H
#define _EffectsFader_H

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

/* Forward declaration of the class Core::Group */
#ifndef _CoreGroup_
  EW_DECLARE_CLASS( CoreGroup )
#define _CoreGroup_
#endif

/* Forward declaration of the class Effects::Fader */
#ifndef _EffectsFader_
  EW_DECLARE_CLASS( EffectsFader )
#define _EffectsFader_
#endif

/* Forward declaration of the class Effects::FaderTask */
#ifndef _EffectsFaderTask_
  EW_DECLARE_CLASS( EffectsFaderTask )
#define _EffectsFaderTask_
#endif


/* The class Effects::Fader provides the basic infrastructure, which can be enhanced 
   in derived classes in order to implement sophisticated transitions for fading-in 
   and/or fading-out GUI components. At the runtime, an instance of the fader class 
   represents a transition you can simply apply on a GUI component by passing both, 
   the fader instance and the affected GUI component as parameters in the invocation 
   of the method Core::Group.FadeGroup(). The infrastructure consists of:
   - The method @OnStart(), which is invoked automatically as soon as the fader 
   is scheduled to start with its transition. Usually, in derived classes this method 
   has then the job to create, initialize and start animation effects (instances 
   of classes descending from Effects::Effect) to fade-in or fade-out the GUI component 
   @Group within another component @Owner.
   - Whether the fader is intended to perform a fade-in or fade-out transition is 
   determined in the property @Visible. Initializing this property with 'true' indicates, 
   that the affected component should become or/and remain visible during and after 
   the transition.
   - As soon as all involved animation effects has finalized their work, the fader 
   should invoke its method @Complete().
   - By overriding the method @OnEnd() the implementation of the fader can additionally 
   be adapted to perform further operation just at the end of the transition, for 
   example to remove @Group from its @Owner after it has been faded-out, etc.
   - The method @IsFinished() should be overridden in derived class to return 'true' 
   if all animation effects has finalized their work.
   - Furthermore, the properties @AddToOwner, @AssignFocus, @RemoveIfHidden, @RestackTopmost 
   and @UseCurrentState are available to provide common parameters how the fader 
   should act. These properties should be evaluated in the implementation of the 
   @OnStart() and/or @OnEnd() method.
   - The property @OnInitialize can refer to a slot method containing more individual 
   code to initialize the fader just in the moment it is started.
   Being the basic class, Effects::Fader doesn't implement any particular transition 
   effects. This class is intended to be overridden and enhanced accordingly. */
EW_DEFINE_FIELDS( EffectsFader, XObject )
  EW_VARIABLE( task,            EffectsFaderTask )
  EW_VARIABLE( prev,            EffectsFader )
  EW_VARIABLE( next,            EffectsFader )
  EW_VARIABLE( Owner,           CoreGroup )
  EW_VARIABLE( Group,           CoreGroup )
  EW_VARIABLE( onCancel,        XSlot )
  EW_VARIABLE( onComplete,      XSlot )
  EW_PROPERTY( OnInitialize,    XSlot )
  EW_PROPERTY( Visible,         XBool )
  EW_PROPERTY( UseCurrentState, XBool )
  EW_PROPERTY( AddToOwner,      XBool )
  EW_PROPERTY( AssignFocus,     XBool )
  EW_PROPERTY( RestackTopmost,  XBool )
  EW_PROPERTY( RemoveIfHidden,  XBool )
EW_END_OF_FIELDS( EffectsFader )

/* Virtual Method Table (VMT) for the class : 'Effects::Fader' */
EW_DEFINE_METHODS( EffectsFader, XObject )
  EW_METHOD( IsFinished,        XBool )( EffectsFader _this )
  EW_METHOD( OnEnd,             void )( EffectsFader _this )
  EW_METHOD( OnStart,           void )( EffectsFader _this )
EW_END_OF_METHODS( EffectsFader )

/* The method Complete() is intended to be called by the implementation of your 
   adapted fader as soon as the animation effects used inside the fader has finalized 
   their work. This is essential in order to ensure, that next faders waiting for 
   execution can start with their animations. */
void EffectsFader_Complete( EffectsFader _this );

/* The method IsFinished() should return 'true' if the fader has finalized its fading 
   animation and 'false' if the animation is not started or it is still in progress. 
   This method should therefore be overridden in derived classes to evaluate the 
   state of animation effects used inside the fader. */
XBool EffectsFader_IsFinished( EffectsFader _this );

/* Wrapper function for the virtual method : 'Effects::Fader.IsFinished()' */
XBool EffectsFader__IsFinished( void* _this );

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
void EffectsFader_OnEnd( EffectsFader _this );

/* Wrapper function for the virtual method : 'Effects::Fader.OnEnd()' */
void EffectsFader__OnEnd( void* _this );

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
void EffectsFader_OnStart( EffectsFader _this );

/* Wrapper function for the virtual method : 'Effects::Fader.OnStart()' */
void EffectsFader__OnStart( void* _this );

#ifdef __cplusplus
  }
#endif

#endif /* _EffectsFader_H */

/* Embedded Wizard */
