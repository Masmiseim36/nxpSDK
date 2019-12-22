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

#ifndef _CoreTaskQueue_H
#define _CoreTaskQueue_H

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
  EW_VARIABLE( toContinue,      CoreTask )
  EW_VARIABLE( current,         CoreTask )
  EW_VARIABLE( last,            CoreTask )
  EW_VARIABLE( first,           CoreTask )
  EW_PROPERTY( OnDone,          XSlot )
  EW_VARIABLE( isInOnStart,     XBool )
EW_END_OF_FIELDS( CoreTaskQueue )

/* Virtual Method Table (VMT) for the class : 'Core::TaskQueue' */
EW_DEFINE_METHODS( CoreTaskQueue, XObject )
EW_END_OF_METHODS( CoreTaskQueue )

/* 'C' function for method : 'Core::TaskQueue.completeTask()' */
void CoreTaskQueue_completeTask( CoreTaskQueue _this );

/* 'C' function for method : 'Core::TaskQueue.onDispatchNext()' */
void CoreTaskQueue_onDispatchNext( CoreTaskQueue _this, XObject sender );

/* 'C' function for method : 'Core::TaskQueue.onPreDispatchNext()' */
void CoreTaskQueue_onPreDispatchNext( CoreTaskQueue _this, XObject sender );

/* The method CancelTask() allows the application to remove a previously registered 
   task from the task queue. The affected task is determined by the parameter aTask.
   If the affected task is currently executed, the task is notified to immediately 
   finalize its work. Afterwards the queue starts the next available task. The method 
   will throw an error if you try to cancel a task not belonging to this queue. */
void CoreTaskQueue_CancelTask( CoreTaskQueue _this, CoreTask aTask );

/* The method ScheduleTask() registers the task passed in the parameter aTask for 
   later execution.
   The tasks are executed in the order in which they have been previously scheduled. 
   If the parameter aWithPriority is false, the new task will be arranged at the 
   end of the list with waiting tasks. If the parameter is true, the task is enqueued 
   in front of all waiting tasks.
   The method will throw an error if you try to schedule the same task twice. */
void CoreTaskQueue_ScheduleTask( CoreTaskQueue _this, CoreTask aTask, XBool aWithPriority );

#ifdef __cplusplus
  }
#endif

#endif /* _CoreTaskQueue_H */

/* Embedded Wizard */
