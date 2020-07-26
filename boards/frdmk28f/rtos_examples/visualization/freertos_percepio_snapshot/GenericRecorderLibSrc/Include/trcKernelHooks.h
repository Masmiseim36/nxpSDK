/*******************************************************************************
 * Tracealyzer / FreeRTOS+Trace - Snapshot Trace Recorder Library v3.0.9
 * Percepio AB, www.percepio.com
 *
 * trcKernelHooks.h
 *
 * Support functions for the kernel integration hooks, used by trcKernelPort.h.
 *
 *******************************************************************************
 * Copyright (c) 2016, Percepio AB.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************
 *
 * Tabs are used for indent in this file (1 tab = 4 spaces)
 ******************************************************************************/

#ifndef TRCKERNELHOOKS_H
#define TRCKERNELHOOKS_H

#if (USE_TRACEALYZER_RECORDER == 1)

#undef INCLUDE_xTaskGetSchedulerState
#define INCLUDE_xTaskGetSchedulerState 1

#undef INCLUDE_xTaskGetCurrentTaskHandle
#define INCLUDE_xTaskGetCurrentTaskHandle 1

#ifndef INCLUDE_OBJECT_DELETE
#define INCLUDE_OBJECT_DELETE 0
#endif

#ifndef INCLUDE_READY_EVENTS
#define INCLUDE_READY_EVENTS 1
#endif

#ifndef INCLUDE_NEW_TIME_EVENTS
#define INCLUDE_NEW_TIME_EVENTS 0
#endif

#if (INCLUDE_OBJECT_DELETE == 1)
/* This macro will remove the task and store it in the event buffer */
#undef trcKERNEL_HOOKS_TASK_DELETE
#define trcKERNEL_HOOKS_TASK_DELETE(SERVICE, pxTCB)                                                            \
    vTraceStoreKernelCall(TRACE_GET_TASK_EVENT_CODE(SERVICE, SUCCESS, CLASS, pxTCB), TRACE_CLASS_TASK,         \
                          TRACE_GET_TASK_NUMBER(pxTCB));                                                       \
    vTraceStoreObjectNameOnCloseEvent(TRACE_GET_TASK_NUMBER(pxTCB), TRACE_CLASS_TASK);                         \
    vTraceStoreObjectPropertiesOnCloseEvent(TRACE_GET_TASK_NUMBER(pxTCB), TRACE_CLASS_TASK);                   \
    vTraceSetPriorityProperty(TRACE_CLASS_TASK, TRACE_GET_TASK_NUMBER(pxTCB), TRACE_GET_TASK_PRIORITY(pxTCB)); \
    vTraceSetObjectState(TRACE_CLASS_TASK, TRACE_GET_TASK_NUMBER(pxTCB), TASK_STATE_INSTANCE_NOT_ACTIVE);      \
    vTraceFreeObjectHandle(TRACE_CLASS_TASK, TRACE_GET_TASK_NUMBER(pxTCB));
#else
#undef trcKERNEL_HOOKS_TASK_DELETE
#define trcKERNEL_HOOKS_TASK_DELETE(SERVICE, pxTCB)
#endif

#if (INCLUDE_OBJECT_DELETE == 1)
/* This macro will remove the object and store it in the event buffer */
#undef trcKERNEL_HOOKS_OBJECT_DELETE
#define trcKERNEL_HOOKS_OBJECT_DELETE(SERVICE, CLASS, pxObject)                                                     \
    vTraceStoreKernelCall(TRACE_GET_OBJECT_EVENT_CODE(SERVICE, SUCCESS, CLASS, pxObject),                           \
                          TRACE_GET_OBJECT_TRACE_CLASS(CLASS, pxObject), TRACE_GET_OBJECT_NUMBER(CLASS, pxObject)); \
    vTraceStoreObjectNameOnCloseEvent(TRACE_GET_OBJECT_NUMBER(CLASS, pxObject),                                     \
                                      TRACE_GET_OBJECT_TRACE_CLASS(CLASS, pxObject));                               \
    vTraceStoreObjectPropertiesOnCloseEvent(TRACE_GET_OBJECT_NUMBER(CLASS, pxObject),                               \
                                            TRACE_GET_OBJECT_TRACE_CLASS(CLASS, pxObject));                         \
    vTraceFreeObjectHandle(TRACE_GET_OBJECT_TRACE_CLASS(CLASS, pxObject), TRACE_GET_OBJECT_NUMBER(CLASS, pxObject));
#else
#undef trcKERNEL_HOOKS_OBJECT_DELETE
#define trcKERNEL_HOOKS_OBJECT_DELETE(SERVICE, CLASS, pxObject)
#endif

/* This macro will create a task in the object table */
#undef trcKERNEL_HOOKS_TASK_CREATE
#define trcKERNEL_HOOKS_TASK_CREATE(SERVICE, CLASS, pxTCB)                                                     \
    TRACE_SET_TASK_NUMBER(pxTCB)                                                                               \
    vTraceSetObjectName(TRACE_CLASS_TASK, TRACE_GET_TASK_NUMBER(pxTCB), TRACE_GET_TASK_NAME(pxTCB));           \
    vTraceSetPriorityProperty(TRACE_CLASS_TASK, TRACE_GET_TASK_NUMBER(pxTCB), TRACE_GET_TASK_PRIORITY(pxTCB)); \
    vTraceStoreKernelCall(TRACE_GET_TASK_EVENT_CODE(SERVICE, SUCCESS, CLASS, pxTCB), TRACE_CLASS_TASK,         \
                          TRACE_GET_TASK_NUMBER(pxTCB));

/* This macro will create a failed create call to create a task */
#undef trcKERNEL_HOOKS_TASK_CREATE_FAILED
#define trcKERNEL_HOOKS_TASK_CREATE_FAILED(SERVICE, CLASS) \
    vTraceStoreKernelCall(TRACE_GET_TASK_EVENT_CODE(SERVICE, FAILED, CLASS, 0), TRACE_CLASS_TASK, 0);

/* This macro will setup a task in the object table */
#undef trcKERNEL_HOOKS_OBJECT_CREATE
#define trcKERNEL_HOOKS_OBJECT_CREATE(SERVICE, CLASS, pxObject)                                                     \
    TRACE_SET_OBJECT_NUMBER(CLASS, pxObject);                                                                       \
    vTraceStoreKernelCall(TRACE_GET_OBJECT_EVENT_CODE(SERVICE, SUCCESS, CLASS, pxObject),                           \
                          TRACE_GET_OBJECT_TRACE_CLASS(CLASS, pxObject), TRACE_GET_OBJECT_NUMBER(CLASS, pxObject)); \
    vTraceSetObjectState(TRACE_GET_OBJECT_TRACE_CLASS(CLASS, pxObject), TRACE_GET_OBJECT_NUMBER(CLASS, pxObject), 0);

/* This macro will create a failed create call to create an object */
#undef trcKERNEL_HOOKS_OBJECT_CREATE_FAILED
#define trcKERNEL_HOOKS_OBJECT_CREATE_FAILED(SERVICE, CLASS, kernelClass)                  \
    vTraceStoreKernelCall(TRACE_GET_CLASS_EVENT_CODE(SERVICE, FAILED, CLASS, kernelClass), \
                          TRACE_GET_CLASS_TRACE_CLASS(CLASS, kernelClass), 0);

/* This macro will create a call to a kernel service with a certain result, with an object as parameter */
#undef trcKERNEL_HOOKS_KERNEL_SERVICE
#define trcKERNEL_HOOKS_KERNEL_SERVICE(SERVICE, RESULT, CLASS, pxObject)                 \
    vTraceStoreKernelCall(TRACE_GET_OBJECT_EVENT_CODE(SERVICE, RESULT, CLASS, pxObject), \
                          TRACE_GET_OBJECT_TRACE_CLASS(CLASS, pxObject), TRACE_GET_OBJECT_NUMBER(CLASS, pxObject));

/* This macro will set the state for an object */
#undef trcKERNEL_HOOKS_SET_OBJECT_STATE
#define trcKERNEL_HOOKS_SET_OBJECT_STATE(CLASS, pxObject, STATE)                                                  \
    vTraceSetObjectState(TRACE_GET_OBJECT_TRACE_CLASS(CLASS, pxObject), TRACE_GET_OBJECT_NUMBER(CLASS, pxObject), \
                         STATE);

/* This macro will flag a certain task as a finished instance */
#undef trcKERNEL_HOOKS_SET_TASK_INSTANCE_FINISHED
#define trcKERNEL_HOOKS_SET_TASK_INSTANCE_FINISHED() \
    vTraceSetTaskInstanceFinished(TRACE_GET_TASK_NUMBER(TRACE_GET_CURRENT_TASK()));

#if INCLUDE_READY_EVENTS == 1
/* This macro will create an event to indicate that a task became Ready */
#undef trcKERNEL_HOOKS_MOVED_TASK_TO_READY_STATE
#define trcKERNEL_HOOKS_MOVED_TASK_TO_READY_STATE(pxTCB) vTraceStoreTaskReady(TRACE_GET_TASK_NUMBER(pxTCB));
#else
#undef trcKERNEL_HOOKS_MOVED_TASK_TO_READY_STATE
#define trcKERNEL_HOOKS_MOVED_TASK_TO_READY_STATE(pxTCB)
#endif

/* This macro will update the internal tick counter and call vTracePortGetTimeStamp(0) to update the internal counters
 */
#undef trcKERNEL_HOOKS_INCREMENT_TICK
#define trcKERNEL_HOOKS_INCREMENT_TICK()  \
    {                                     \
        extern uint32_t uiTraceTickCount; \
        uiTraceTickCount++;               \
        vTracePortGetTimeStamp(0);        \
    }

#if INCLUDE_NEW_TIME_EVENTS == 1
/* This macro will create an event indicating that the OS tick count has increased */
#undef trcKERNEL_HOOKS_NEW_TIME
#define trcKERNEL_HOOKS_NEW_TIME(SERVICE, xValue) vTraceStoreKernelCallWithNumericParamOnly(SERVICE, xValue);
#else
#undef trcKERNEL_HOOKS_NEW_TIME
#define trcKERNEL_HOOKS_NEW_TIME(SERVICE, xValue)
#endif

/* This macro will create a task switch event to the currently executing task */
#undef trcKERNEL_HOOKS_TASK_SWITCH
#define trcKERNEL_HOOKS_TASK_SWITCH(pxTCB) vTraceStoreTaskswitch(TRACE_GET_TASK_NUMBER(pxTCB));

/* This macro will create an event to indicate that the task has been suspended */
#undef trcKERNEL_HOOKS_TASK_SUSPEND
#define trcKERNEL_HOOKS_TASK_SUSPEND(SERVICE, pxTCB)                                \
    vTraceStoreKernelCall(SERVICE, TRACE_CLASS_TASK, TRACE_GET_TASK_NUMBER(pxTCB)); \
    vTraceSetTaskInstanceFinished((uint8_t)TRACE_GET_TASK_NUMBER(pxTCB));

/* This macro will create an event to indicate that a task has called a wait/delay function */
#undef trcKERNEL_HOOKS_TASK_DELAY
#define trcKERNEL_HOOKS_TASK_DELAY(SERVICE, pxTCB, xValue)      \
    vTraceStoreKernelCallWithNumericParamOnly(SERVICE, xValue); \
    vTraceSetTaskInstanceFinished((uint8_t)TRACE_GET_TASK_NUMBER(pxTCB));

/* This macro will create an event to indicate that a task has gotten its priority changed */
#undef trcKERNEL_HOOKS_TASK_PRIORITY_CHANGE
#define trcKERNEL_HOOKS_TASK_PRIORITY_CHANGE(SERVICE, pxTCB, uxNewPriority)                                     \
    vTraceStoreKernelCallWithParam(SERVICE, TRACE_CLASS_TASK, TRACE_GET_TASK_NUMBER(pxTCB),                     \
                                   uiTraceGetPriorityProperty(TRACE_CLASS_TASK, TRACE_GET_TASK_NUMBER(pxTCB))); \
    vTraceSetPriorityProperty(TRACE_CLASS_TASK, TRACE_GET_TASK_NUMBER(pxTCB), (uint8_t)uxNewPriority);

/* This macro will create an event to indicate that the task has been resumed */
#undef trcKERNEL_HOOKS_TASK_RESUME
#define trcKERNEL_HOOKS_TASK_RESUME(SERVICE, pxTCB) \
    vTraceStoreKernelCall(SERVICE, TRACE_CLASS_TASK, TRACE_GET_TASK_NUMBER(pxTCB));

#undef trcKERNEL_HOOKS_TIMER_EVENT
#define trcKERNEL_HOOKS_TIMER_EVENT(SERVICE, pxTimer) \
    vTraceStoreKernelCall(SERVICE, TRACE_CLASS_TIMER, TRACE_GET_TIMER_NUMBER(pxTimer));

/* This macro will create a timer in the object table and assign the timer a trace handle (timer number).*/
#undef trcKERNEL_HOOKS_TIMER_CREATE
#define trcKERNEL_HOOKS_TIMER_CREATE(SERVICE, pxTimer)                                                      \
    TRACE_SET_TIMER_NUMBER(pxTimer);                                                                        \
    vTraceSetObjectName(TRACE_CLASS_TIMER, TRACE_GET_TIMER_NUMBER(pxTimer), TRACE_GET_TIMER_NAME(pxTimer)); \
    vTraceStoreKernelCall(SERVICE, TRACE_CLASS_TIMER, TRACE_GET_TIMER_NUMBER(pxTimer));
#endif

#undef trcKERNEL_HOOKS_TIMER_DELETE
#define trcKERNEL_HOOKS_TIMER_DELETE(SERVICE, pxTimer)                                           \
    vTraceStoreKernelCall(SERVICE, TRACE_CLASS_TIMER, TRACE_GET_TIMER_NUMBER(pxTimer));          \
    vTraceStoreObjectNameOnCloseEvent(TRACE_GET_TIMER_NUMBER(pxTimer), TRACE_CLASS_TIMER);       \
    vTraceStoreObjectPropertiesOnCloseEvent(TRACE_GET_TIMER_NUMBER(pxTimer), TRACE_CLASS_TIMER); \
    vTraceFreeObjectHandle(TRACE_CLASS_TIMER, TRACE_GET_TIMER_NUMBER(pxTimer));

#endif /* TRCKERNELHOOKS_H */
