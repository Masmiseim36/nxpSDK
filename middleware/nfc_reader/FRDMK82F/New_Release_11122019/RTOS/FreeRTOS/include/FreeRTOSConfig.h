/*
 * FreeRTOS Kernel V10.2.1
 * Copyright (C) 2019 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#if defined(PHFL_APP_NXBUILD_CONFIG) || defined(NXPBUILD_CUSTOMER_HEADER_INCLUDED)
#   include <ph_NxpBuild.h>
#endif
#ifdef __GNUC__
#  include <stdint.h>
#endif
/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE.
 *----------------------------------------------------------*/

#if defined(__GNUC__) || defined (__ARMCC_VERSION) || defined (__ICCARM__)
extern uint32_t SystemCoreClock;
#endif

#define configUSE_PREEMPTION		1
#define configUSE_IDLE_HOOK			0
#define configMAX_PRIORITIES        ( 6 )
#define configUSE_TICK_HOOK			0
#define configCPU_CLOCK_HZ          ( ( unsigned long ) SystemCoreClock )
#define configTICK_RATE_HZ			( ( TickType_t ) 1000 )
#define configMINIMAL_STACK_SIZE	( ( unsigned short ) 100 )
#define configSUPPORT_STATIC_ALLOCATION 0
#if defined (__PN74XXXX__) || defined (__PN73XXXX__)
#   if ( configSUPPORT_STATIC_ALLOCATION == 1 )
#      error "Not supported for PN7462AU"
#   else
#       ifndef configTOTAL_HEAP_SIZE
#      		define configTOTAL_HEAP_SIZE       ( ( size_t ) ( 5 * 1024 ) )
#       endif
#   endif
#else
#   if ( configSUPPORT_STATIC_ALLOCATION == 1 )
#      define configTOTAL_HEAP_SIZE       ( ( size_t ) ( 0 * 1024 ) )
#   else
#      define configTOTAL_HEAP_SIZE       ( ( size_t ) ( 12 * 1024 ) )
#   endif
#endif
#if ( configSUPPORT_STATIC_ALLOCATION == 1 )
#define configSUPPORT_DYNAMIC_ALLOCATION    0
#else
#define configSUPPORT_DYNAMIC_ALLOCATION    1
#endif
#define configMAX_TASK_NAME_LEN     ( 25 )
#define configUSE_TRACE_FACILITY	0
#define configUSE_16_BIT_TICKS		0
#define configIDLE_SHOULD_YIELD     1
#define configUSE_CO_ROUTINES 		0
#define configUSE_MUTEXES			1

#define configMAX_CO_ROUTINE_PRIORITIES ( 2 )

#define configUSE_COUNTING_SEMAPHORES 	1
#define configUSE_ALTERNATIVE_API 		0
#define configCHECK_FOR_STACK_OVERFLOW	2
#define configUSE_RECURSIVE_MUTEXES     0
#define configQUEUE_REGISTRY_SIZE       8
#define configGENERATE_RUN_TIME_STATS	0
#define configUSE_MALLOC_FAILED_HOOK	1

/* Software timer definitions. */
#define configUSE_TIMERS                1
#define configTIMER_TASK_PRIORITY       ( 5 )
#if defined (__PN74XXXX__) || defined (__PN73XXXX__)
/* Since PN7462 I2cm, RF, SPIM, CT and other hals are all Blocking calls.
 * Shall use the event mechanism in ISR contexts. This would lead to Message posting to
 * Timer Daemon Task at run time, message queue length of 2 is not enough.
 * So making the queue length to 8.
 *  */
#    define configTIMER_QUEUE_LENGTH        8
#else
#    define configTIMER_QUEUE_LENGTH        2
#endif

#define configTIMER_TASK_STACK_DEPTH    ( 100 )
/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */

#define INCLUDE_vTaskPrioritySet			1
#define INCLUDE_uxTaskPriorityGet			1
#define INCLUDE_vTaskDelete					1
#define INCLUDE_vTaskCleanUpResources       1
#define INCLUDE_vTaskSuspend				1
#define INCLUDE_vTaskDelayUntil				1
#define INCLUDE_vTaskDelay					1
#define INCLUDE_uxTaskGetStackHighWaterMark 0

#define INCLUDE_xEventGroupSetBitFromISR    1
#define INCLUDE_xTimerPendFunctionCall      1
#define INCLUDE_xSemaphoreGetMutexHolder    1

/**
 * Configure the number of priority bits. This is normally
 * __NVIC_PRIO_BITS but PSoC Creator beta 5 contained a larger
 * value for the priority than is implemented in the hardware so
 * set it here to what the data sheet describes.
 */
/* Use the system definition, if there is one */
#ifdef __NVIC_PRIO_BITS
    #define configPRIO_BITS       __NVIC_PRIO_BITS
#else
    #define configPRIO_BITS       5        /* 32 priority levels */
#endif

/* The lowest priority. */
#define configKERNEL_INTERRUPT_PRIORITY 	( 31 << (8 - configPRIO_BITS) )
/* Priority 5, or 160 as only the top three bits are implemented. */
/* !!!! configMAX_SYSCALL_INTERRUPT_PRIORITY must not be set to zero !!!!
See http://www.FreeRTOS.org/RTOS-Cortex-M3-M4.html. */
#define configMAX_SYSCALL_INTERRUPT_PRIORITY 	( 5 << (8 - configPRIO_BITS) )

#ifdef CORE_M3
/*
 * Use the Cortex-M3 optimisations, rather than the generic C implementation.
 */
#define configUSE_PORT_OPTIMISED_TASK_SELECTION 1
#endif   /* CORE_M3 */

#ifdef PH_OSAL_FREERTOS
#   define vPortSVCHandler     SVC_Handler
#   define xPortPendSVHandler  PendSV_Handler
#   define xPortSysTickHandler SysTick_Handler
#endif

/*
#define configASSERT( x )   if( ( x ) == 0 ) {printf("File %s Line %d :", __FILE__, __LINE__); }
*/

#endif /* FREERTOS_CONFIG_H */
