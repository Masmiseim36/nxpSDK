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

#ifndef _CoreTask_H
#define _CoreTask_H

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

/* Forward declaration of the class Core::Task */
#ifndef _CoreTask_
  EW_DECLARE_CLASS( CoreTask )
#define _CoreTask_
#endif


/* The class Core::Task provides the basic functionality for implementing tasks 
   which then can be scheduled for later execution. The execution of tasks is controlled 
   by instances of the class Core::TaskQueue.
   Usually you will use this class to derive your own task class. In your task class 
   you can implement what to do when the task is started, canceled or completed. 
   For this purpose you can override the methods @OnStart(), @OnCancel(), @OnComplete() 
   and @OnContinue().
   It is essential to understand, that the entire 'task' functionality has nothing 
   to do with multi-threading or multi-tasking features known from operating systems. 
   Applications developed with Chora are limited to a single-thread environment. 
   There is no real background thread activity. Accordingly your implementation 
   of a task should behave cooperatively. A well designed task should perform its 
   job quickly, use timers or effects to delay execution and when the job is done 
   reports its completion by calling the method @Complete(). */
EW_DEFINE_FIELDS( CoreTask, XObject )
EW_END_OF_FIELDS( CoreTask )

/* Virtual Method Table (VMT) for the class : 'Core::Task' */
EW_DEFINE_METHODS( CoreTask, XObject )
EW_END_OF_METHODS( CoreTask )

#ifdef __cplusplus
  }
#endif

#endif /* _CoreTask_H */

/* Embedded Wizard */
