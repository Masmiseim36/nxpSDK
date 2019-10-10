/*******************************************************************************
 * Tracealyzer / FreeRTOS+Trace - Snapshot Trace Recorder Library v3.0.9
 * Percepio AB, www.percepio.com
 *
 * trcKernel.h
 *
 * Functions used by trcKernelHooks.h.
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

#ifndef TRCKERNEL_H
#define TRCKERNEL_H

#include "trcKernelPort.h"

#if (USE_TRACEALYZER_RECORDER == 1)

/* Internal functions */

#if !defined INCLUDE_READY_EVENTS || INCLUDE_READY_EVENTS == 1
void vTraceSetReadyEventsEnabled(int status);
void vTraceStoreTaskReady(objectHandleType handle);
#else
#define vTraceSetReadyEventsEnabled(status)
#endif

void vTraceStoreLowPower(uint32_t flag);

void vTraceStoreTaskswitch(objectHandleType task_handle);

void vTraceStoreKernelCall(uint32_t eventcode, traceObjectClass objectClass, uint32_t byteParam);

void vTraceStoreKernelCallWithNumericParamOnly(uint32_t evtcode, uint32_t param);

void vTraceStoreKernelCallWithParam(uint32_t evtcode,
                                    traceObjectClass objectClass,
                                    uint32_t objectNumber,
                                    uint32_t param);

void vTraceSetTaskInstanceFinished(objectHandleType handle);

void vTraceSetPriorityProperty(uint8_t objectclass, objectHandleType id, uint8_t value);

uint8_t uiTraceGetPriorityProperty(uint8_t objectclass, objectHandleType id);

void vTraceSetObjectState(uint8_t objectclass, objectHandleType id, uint8_t value);

uint8_t uiTraceGetObjectState(uint8_t objectclass, objectHandleType id);

#if (INCLUDE_OBJECT_DELETE == 1)

void vTraceStoreObjectNameOnCloseEvent(objectHandleType handle, traceObjectClass objectclass);

void vTraceStoreObjectPropertiesOnCloseEvent(objectHandleType handle, traceObjectClass objectclass);
#endif

/* Internal constants for task state */
#define TASK_STATE_INSTANCE_NOT_ACTIVE 0
#define TASK_STATE_INSTANCE_ACTIVE 1

#endif

#endif
