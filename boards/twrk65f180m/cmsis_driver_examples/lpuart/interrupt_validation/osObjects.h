/*
 * Copyright (c) 2013-2016 ARM Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.

 * Copyright (c) 2016, Freescale Semiconductor, Inc.
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
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
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
 */
 /*----------------------------------------------------------------------------
 * osObjects.h: CMSIS-RTOS global object definitions for an application
 *----------------------------------------------------------------------------
 *
 * This header file defines global RTOS objects used throughout a project
 *
 * #define osObjectsPublic indicates that objects are defined; without that
 * definition the objects are defined as external symbols.
 *
 *--------------------------------------------------------------------------*/


#ifndef __osObjects
#define __osObjects

#if (!defined (osObjectsPublic))
#define osObjectsExternal          // define RTOS objects with extern attribute
#endif

#include <cmsis_os.h>              // CMSIS RTOS header file


// global 'thread' functions ---------------------------------------------------
/* 
Example:
extern void sample_name (void const *argument);          // thread function

osThreadId tid_sample_name;                              // thread id
osThreadDef (sample_name, osPriorityNormal, 1, 0);       // thread object
*/


// global 'semaphores' ----------------------------------------------------------
/* 
Example:
osSemaphoreId sid_sample_name;                           // semaphore id
osSemaphoreDef (sample_name);                            // semaphore object
*/


// global 'memory pools' --------------------------------------------------------
/* 
Example:
typedef struct sample_name type_sample_name;             // object data type

osPoolId mpid_sample_name;                               // memory pool id
osPoolDef (sample_name, 16, type_sample_name);           // memory pool object
*/


// global 'message queues' -------------------------------------------------------
/* 
Example:
typedef struct sample_name type_sample_name;             // object data type

osMessageQId mid_sample_name;                            // message queue id
osMessageQDef (sample_name, 16, type_sample_name);       // message queue object
*/


// global 'mail queues' ----------------------------------------------------------
/* 
Example:
typedef struct sample_name type_sample_name;             // object data type

osMailQId qid_sample_name;                               // mail queue id
osMailQDef (sample_name, 16, type_sample_name);          // mail queue object
*/

#endif  // __osObjects
