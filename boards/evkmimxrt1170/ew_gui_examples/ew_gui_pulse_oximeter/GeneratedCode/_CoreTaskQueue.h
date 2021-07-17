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

#ifndef _CoreTaskQueue_H
#define _CoreTaskQueue_H

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

/* Forward declaration of the class Core::Task */
#ifndef _CoreTask_
  EW_DECLARE_CLASS( CoreTask )
#define _CoreTask_
#endif

/* Forward declaration of the class Core::TaskQueue */
#ifndef _CoreTaskQueue_
  EW_DECLARE_CLASS( CoreTaskQueue )
#define _CoreTaskQueue_
#endif


/* The class Core::TaskQueue provides a simple task scheduler component. The component 
   implements functionality to register, manage and perform tasks. A task can be 
   e.g. an animated transition between two GUI components. With the task queue, 
   several independent transitions (and another operations) can be executed strictly 
   one after another.
   Usually the tasks are executed in the order in which they were previously added 
   to the queue (the order in which they are scheduled). The queue takes care that 
   at the same time only one task is executed. This task is called the 'current' 
   task. As soon as the current task is completed, the queue activates the next 
   available task.
   A single task is represented by an instance of a class derived from Core::Task. 
   To schedule a task, create an instance of the task class and call the method 
   @ScheduleTask() with the task as parameter.
   With the method @CancelTask() you can remove a previously scheduled task from 
   the queue again or if the task is already running, stop it and let the queue 
   continue with the next task.
   The method @GetCurrentTask() is useful to determine which task is currently in 
   progress by the affected queue.
   It is essential to understand, that the entire 'task' functionality has nothing 
   to do with multi-threading or multi-tasking features known from operating systems. 
   Applications developed with Chora are limited to a single-thread environment. 
   There is no real background thread activity. Accordingly your implementation 
   of a task should behave cooperatively. A well designed task should perform its 
   job quickly, use timers or effects to delay execution and when the job is done 
   inform the queue about its completion. */
EW_DEFINE_FIELDS( CoreTaskQueue, XObject )
  EW_VARIABLE( current,         CoreTask )
  EW_VARIABLE( first,           CoreTask )
EW_END_OF_FIELDS( CoreTaskQueue )

/* Virtual Method Table (VMT) for the class : 'Core::TaskQueue' */
EW_DEFINE_METHODS( CoreTaskQueue, XObject )
EW_END_OF_METHODS( CoreTaskQueue )

#ifdef __cplusplus
  }
#endif

#endif /* _CoreTaskQueue_H */

/* Embedded Wizard */
