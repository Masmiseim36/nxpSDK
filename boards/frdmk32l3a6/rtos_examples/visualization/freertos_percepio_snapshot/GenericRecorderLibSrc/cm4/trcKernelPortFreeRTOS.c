/*******************************************************************************
 * Tracealyzer / FreeRTOS+Trace - Snapshot Trace Recorder Library v3.0.9
 * Percepio AB, www.percepio.com
 *
 * trcKernelPort.c
 *
 * Kernel-specific functionality for FreeRTOS, used by the recorder library.
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

#include "trcKernelPort.h"

#if (USE_TRACEALYZER_RECORDER == 1)

#include <stdint.h>

#include "task.h"
#include "queue.h"

/* For classes implemented as FreeRTOS Queues:
This translates queue.type to the corresponding trace object class. */
traceObjectClass TraceObjectClassTable[5] = {TRACE_CLASS_QUEUE, TRACE_CLASS_MUTEX, TRACE_CLASS_SEMAPHORE,
                                             TRACE_CLASS_SEMAPHORE, TRACE_CLASS_MUTEX};

int uiInEventGroupSetBitsFromISR = 0;

#if (FREERTOS_VERSION < FREERTOS_VERSION_8_X)

objectHandleType prvTraceGetObjectNumber(void *handle)
{
    return (objectHandleType)ucQueueGetQueueNumber(handle);
}

#else

objectHandleType prvTraceGetObjectNumber(void *handle)
{
    return (objectHandleType)uxQueueGetQueueNumber(handle);
}

#endif

unsigned char prvTraceGetObjectType(void *handle)
{
    return ucQueueGetQueueType(handle);
}

objectHandleType prvTraceGetTaskNumber(void *handle)
{
    return (objectHandleType)uxTaskGetTaskNumber(handle);
}

unsigned char prvTraceIsSchedulerActive()
{
    return xTaskGetSchedulerState() == taskSCHEDULER_RUNNING;
}

unsigned char prvTraceIsSchedulerSuspended()
{
    return xTaskGetSchedulerState() == taskSCHEDULER_SUSPENDED;
}

unsigned char prvTraceIsSchedulerStarted()
{
    return xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED;
}

void *prvTraceGetCurrentTaskHandle()
{
    return xTaskGetCurrentTaskHandle();
}

/* Initialization of the object property table */
void vTraceInitObjectPropertyTable()
{
    RecorderDataPtr->ObjectPropertyTable.NumberOfObjectClasses          = TRACE_NCLASSES;
    RecorderDataPtr->ObjectPropertyTable.NumberOfObjectsPerClass[0]     = NQueue;
    RecorderDataPtr->ObjectPropertyTable.NumberOfObjectsPerClass[1]     = NSemaphore;
    RecorderDataPtr->ObjectPropertyTable.NumberOfObjectsPerClass[2]     = NMutex;
    RecorderDataPtr->ObjectPropertyTable.NumberOfObjectsPerClass[3]     = NTask;
    RecorderDataPtr->ObjectPropertyTable.NumberOfObjectsPerClass[4]     = NISR;
    RecorderDataPtr->ObjectPropertyTable.NumberOfObjectsPerClass[5]     = NTimer;
    RecorderDataPtr->ObjectPropertyTable.NumberOfObjectsPerClass[6]     = NEventGroup;
    RecorderDataPtr->ObjectPropertyTable.NameLengthPerClass[0]          = NameLenQueue;
    RecorderDataPtr->ObjectPropertyTable.NameLengthPerClass[1]          = NameLenSemaphore;
    RecorderDataPtr->ObjectPropertyTable.NameLengthPerClass[2]          = NameLenMutex;
    RecorderDataPtr->ObjectPropertyTable.NameLengthPerClass[3]          = NameLenTask;
    RecorderDataPtr->ObjectPropertyTable.NameLengthPerClass[4]          = NameLenISR;
    RecorderDataPtr->ObjectPropertyTable.NameLengthPerClass[5]          = NameLenTimer;
    RecorderDataPtr->ObjectPropertyTable.NameLengthPerClass[6]          = NameLenEventGroup;
    RecorderDataPtr->ObjectPropertyTable.TotalPropertyBytesPerClass[0]  = PropertyTableSizeQueue;
    RecorderDataPtr->ObjectPropertyTable.TotalPropertyBytesPerClass[1]  = PropertyTableSizeSemaphore;
    RecorderDataPtr->ObjectPropertyTable.TotalPropertyBytesPerClass[2]  = PropertyTableSizeMutex;
    RecorderDataPtr->ObjectPropertyTable.TotalPropertyBytesPerClass[3]  = PropertyTableSizeTask;
    RecorderDataPtr->ObjectPropertyTable.TotalPropertyBytesPerClass[4]  = PropertyTableSizeISR;
    RecorderDataPtr->ObjectPropertyTable.TotalPropertyBytesPerClass[5]  = PropertyTableSizeTimer;
    RecorderDataPtr->ObjectPropertyTable.TotalPropertyBytesPerClass[6]  = PropertyTableSizeEventGroup;
    RecorderDataPtr->ObjectPropertyTable.StartIndexOfClass[0]           = StartIndexQueue;
    RecorderDataPtr->ObjectPropertyTable.StartIndexOfClass[1]           = StartIndexSemaphore;
    RecorderDataPtr->ObjectPropertyTable.StartIndexOfClass[2]           = StartIndexMutex;
    RecorderDataPtr->ObjectPropertyTable.StartIndexOfClass[3]           = StartIndexTask;
    RecorderDataPtr->ObjectPropertyTable.StartIndexOfClass[4]           = StartIndexISR;
    RecorderDataPtr->ObjectPropertyTable.StartIndexOfClass[5]           = StartIndexTimer;
    RecorderDataPtr->ObjectPropertyTable.StartIndexOfClass[6]           = StartIndexEventGroup;
    RecorderDataPtr->ObjectPropertyTable.ObjectPropertyTableSizeInBytes = TRACE_OBJECT_TABLE_SIZE;
}

/* Initialization of the handle mechanism, see e.g, xTraceGetObjectHandle */
void vTraceInitObjectHandleStack()
{
    objectHandleStacks.indexOfNextAvailableHandle[0] = objectHandleStacks.lowestIndexOfClass[0] = 0;
    objectHandleStacks.indexOfNextAvailableHandle[1] = objectHandleStacks.lowestIndexOfClass[1] = NQueue;
    objectHandleStacks.indexOfNextAvailableHandle[2] = objectHandleStacks.lowestIndexOfClass[2] = NQueue + NSemaphore;
    objectHandleStacks.indexOfNextAvailableHandle[3] = objectHandleStacks.lowestIndexOfClass[3] =
        NQueue + NSemaphore + NMutex;
    objectHandleStacks.indexOfNextAvailableHandle[4] = objectHandleStacks.lowestIndexOfClass[4] =
        NQueue + NSemaphore + NMutex + NTask;
    objectHandleStacks.indexOfNextAvailableHandle[5] = objectHandleStacks.lowestIndexOfClass[5] =
        NQueue + NSemaphore + NMutex + NTask + NISR;
    objectHandleStacks.indexOfNextAvailableHandle[6] = objectHandleStacks.lowestIndexOfClass[6] =
        NQueue + NSemaphore + NMutex + NTask + NISR + NTimer;

    objectHandleStacks.highestIndexOfClass[0] = NQueue - 1;
    objectHandleStacks.highestIndexOfClass[1] = NQueue + NSemaphore - 1;
    objectHandleStacks.highestIndexOfClass[2] = NQueue + NSemaphore + NMutex - 1;
    objectHandleStacks.highestIndexOfClass[3] = NQueue + NSemaphore + NMutex + NTask - 1;
    objectHandleStacks.highestIndexOfClass[4] = NQueue + NSemaphore + NMutex + NTask + NISR - 1;
    objectHandleStacks.highestIndexOfClass[5] = NQueue + NSemaphore + NMutex + NTask + NISR + NTimer - 1;
    objectHandleStacks.highestIndexOfClass[6] = NQueue + NSemaphore + NMutex + NTask + NISR + NTimer + NEventGroup - 1;
}

/* Returns the "Not enough handles" error message for this object class */
const char *pszTraceGetErrorNotEnoughHandles(traceObjectClass objectclass)
{
    switch (objectclass)
    {
        case TRACE_CLASS_TASK:
            return "Not enough TASK handles - increase NTask in trcConfig.h";
        case TRACE_CLASS_ISR:
            return "Not enough ISR handles - increase NISR in trcConfig.h";
        case TRACE_CLASS_SEMAPHORE:
            return "Not enough SEMAPHORE handles - increase NSemaphore in trcConfig.h";
        case TRACE_CLASS_MUTEX:
            return "Not enough MUTEX handles - increase NMutex in trcConfig.h";
        case TRACE_CLASS_QUEUE:
            return "Not enough QUEUE handles - increase NQueue in trcConfig.h";
        case TRACE_CLASS_TIMER:
            return "Not enough TIMER handles - increase NTimer in trcConfig.h";
        case TRACE_CLASS_EVENTGROUP:
            return "Not enough EVENTGROUP handles - increase NEventGroup in trcConfig.h";
        default:
            return "pszTraceGetErrorHandles: Invalid objectclass!";
    }
}

/* Returns the exclude state of the object */
uint8_t uiTraceIsObjectExcluded(traceObjectClass objectclass, objectHandleType handle)
{
    TRACE_ASSERT(objectclass < TRACE_NCLASSES, "prvTraceIsObjectExcluded: objectclass >= TRACE_NCLASSES", 1);
    TRACE_ASSERT(handle <= RecorderDataPtr->ObjectPropertyTable.NumberOfObjectsPerClass[objectclass],
                 "uiTraceIsObjectExcluded: Invalid value for handle", 1);

    switch (objectclass)
    {
        case TRACE_CLASS_TASK:
            return TRACE_GET_TASK_FLAG_ISEXCLUDED(handle);
        case TRACE_CLASS_SEMAPHORE:
            return TRACE_GET_SEMAPHORE_FLAG_ISEXCLUDED(handle);
        case TRACE_CLASS_MUTEX:
            return TRACE_GET_MUTEX_FLAG_ISEXCLUDED(handle);
        case TRACE_CLASS_QUEUE:
            return TRACE_GET_QUEUE_FLAG_ISEXCLUDED(handle);
        case TRACE_CLASS_TIMER:
            return TRACE_GET_TIMER_FLAG_ISEXCLUDED(handle);
        case TRACE_CLASS_EVENTGROUP:
            return TRACE_GET_EVENTGROUP_FLAG_ISEXCLUDED(handle);
    }

    vTraceError("Invalid object class ID in uiTraceIsObjectExcluded!");

    /* Must never reach */
    return 1;
}

#endif
