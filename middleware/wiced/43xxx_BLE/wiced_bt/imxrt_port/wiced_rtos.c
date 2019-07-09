/*
 * Copyright 2018, Cypress Semiconductor Corporation or a subsidiary of 
 * Cypress Semiconductor Corporation. All Rights Reserved.
 * 
 * This software, associated documentation and materials ("Software"),
 * is owned by Cypress Semiconductor Corporation
 * or one of its subsidiaries ("Cypress") and is protected by and subject to
 * worldwide patent protection (United States and foreign),
 * United States copyright laws and international treaty provisions.
 * Therefore, you may use this Software only as provided in the license
 * agreement accompanying the software package from which you
 * obtained this Software ("EULA").
 * If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
 * non-transferable license to copy, modify, and compile the Software
 * source code solely for use in connection with Cypress's
 * integrated circuit products. Any reproduction, modification, translation,
 * compilation, or representation of this Software except as specified
 * above is prohibited without the express written permission of Cypress.
 *
 * Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
 * reserves the right to make changes to the Software without notice. Cypress
 * does not assume any liability arising out of the application or use of the
 * Software or any product or circuit described in the Software. Cypress does
 * not authorize its products for use in any products where a malfunction or
 * failure of the Cypress product may reasonably be expected to result in
 * significant property damage, injury or death ("High Risk Product"). By
 * including Cypress's product in a High Risk Product, the manufacturer
 * of such system or application assumes all risk of such use and in doing
 * so agrees to indemnify Cypress against all liability.
 */

/** @file
 *
 */

#include <string.h>
#include <stdlib.h>
#include "wiced_rtos.h"
//#include "wiced_defaults.h"
#include "RTOS/wwd_rtos_interface.h"
//#include "RTOS/wiced_rtos_common.h"
#include "platform/wwd_platform_interface.h"
#include "queue.h"
#include "semphr.h"
#include "task.h"
#include "FreeRTOSConfig.h"
#include "timers.h"
#include "wiced_time.h"
//#include "internal/wiced_internal_api.h"
#include "wwd_assert.h"
//#include "platform_init.h"

/******************************************************
 *                      Macros
 ******************************************************/

/******************************************************
 *                    Constants
 ******************************************************/
#ifndef APPLICATION_STACK_SIZE
#define APPLICATION_STACK_SIZE WICED_DEFAULT_APPLICATION_STACK_SIZE
#endif

#ifndef TIMER_THREAD_STACK_SIZE
#define TIMER_THREAD_STACK_SIZE      1024
#endif
#define TIMER_QUEUE_LENGTH  5

#define SYSTEM_MONITOR_THREAD_STACK_SIZE   512


/* FreeRTOS Event Group definition is based on
 *  configUSE_16_BIT_TICKS
 *
 * See <FreeRTOS>/Source/event_roups.c for detail.
 */
#if configUSE_16_BIT_TICKS == 1
    #define eventEVENT_BITS_MASK    0xffU           /* 8-bit */
#else
    #define eventEVENT_BITS_MASK    0xffffffUL      /* 24-bit */
#endif

uint32_t wiced_time_offset;
extern const uint32_t ms_to_tick_ratio;

typedef TimerCallbackFunction_t native_timer_handler_t;

wiced_result_t wiced_rtos_create_thread( wiced_thread_t* thread, 
                                          uint8_t priority, 
                                          const char* name, 
                                          wiced_thread_function_t function, 
                                          uint32_t stack_size, 
                                          void* arg )
{
    /* Limit priority to default lib priority */
    if ( priority > RTOS_HIGHEST_PRIORITY )
    {
        priority = RTOS_HIGHEST_PRIORITY;
    }

    return host_rtos_create_thread_with_arg( thread , function, name,  NULL, 
                        stack_size, 
                        WICED_PRIORITY_TO_NATIVE_PRIORITY( priority ), 
                        (uint32_t) arg );
}

wiced_result_t wiced_rtos_create_thread_with_stack( wiced_thread_t* thread, uint8_t priority, const char* name, wiced_thread_function_t function, void* stack, uint32_t stack_size, void* arg )
{
    /* Limit priority to default lib priority */
    if ( priority > RTOS_HIGHEST_PRIORITY )
    {
        priority = RTOS_HIGHEST_PRIORITY;
    }

    return  host_rtos_create_thread_with_arg( thread , function, name, stack, stack_size, WICED_PRIORITY_TO_NATIVE_PRIORITY( priority ), (uint32_t)arg );
}

wiced_result_t wiced_rtos_delete_thread(wiced_thread_t* thread)
{
    //wiced_result_t result;

    //malloc_leak_check( &thread, LEAK_CHECK_THREAD );
    /***
    result = host_rtos_finish_thread( thread  );

    if ( result != WICED_WWD_SUCCESS )
    {
        return result;
    }
    ***/

    return host_rtos_delete_terminated_thread( thread  );
}

wiced_result_t wiced_rtos_is_current_thread( wiced_thread_t* thread )
{
    if ( xTaskGetCurrentTaskHandle( ) == *thread )
    {
        return WICED_SUCCESS;
    }
    else
    {
        return WICED_ERROR;
    }
}

void *wiced_rtos_get_current_thread()
{
    return(void *) xTaskGetCurrentTaskHandle();
}


wiced_result_t wiced_rtos_check_stack( void )
{
    /* TODO: Add stack checking here. */

    return WICED_SUCCESS;
}

wiced_result_t wiced_rtos_thread_force_awake( wiced_thread_t* thread )
{
    xTaskAbortDelay(*thread);
    return WICED_SUCCESS;
}


wiced_result_t wiced_time_get_time(wiced_time_t* time_ptr)
{
    *time_ptr = (wiced_time_t) ( xTaskGetTickCount( ) * ms_to_tick_ratio ) + wiced_time_offset;
    return WICED_SUCCESS;
}

wiced_result_t wiced_time_set_time( const wiced_time_t* time_ptr )
{
    wiced_time_offset = *time_ptr - (wiced_time_t) ( xTaskGetTickCount( ) * ms_to_tick_ratio );
    return WICED_SUCCESS;
}

wiced_result_t wiced_rtos_init_mutex( wiced_mutex_t* mutex )
{
    wiced_assert("Bad args", mutex != NULL);

    /* Mutex uses priority inheritance */
    *mutex = xSemaphoreCreateRecursiveMutex( );
    if ( *mutex == NULL )
    {
        return WICED_ERROR;
    }

    return WICED_SUCCESS;
}

wiced_result_t wiced_rtos_lock_mutex( wiced_mutex_t* mutex )
{
    wiced_assert("Bad args", mutex != NULL);

    if ( xSemaphoreTakeRecursive( *mutex, WICED_WAIT_FOREVER ) != pdPASS )
    {
        return WICED_ERROR;
    }

    return WICED_SUCCESS;
}


wiced_result_t wiced_rtos_unlock_mutex( wiced_mutex_t* mutex )
{
    wiced_assert("Bad args", mutex != NULL);

    if ( xSemaphoreGiveRecursive( *mutex ) != pdPASS )
    {
        return WICED_ERROR;
    }

    return WICED_SUCCESS;
}


wiced_result_t wiced_rtos_deinit_mutex( wiced_mutex_t* mutex )
{
    wiced_assert("Bad args", mutex != NULL);

    vSemaphoreDelete( *mutex );
    return WICED_SUCCESS;
}


wiced_result_t wiced_rtos_init_queue( wiced_queue_t* queue, 
                                      const char* name, 
                                      uint32_t message_size, 
                                      uint32_t number_of_messages )
{
    UNUSED_PARAMETER( name );

    return host_rtos_init_queue( queue, NULL, message_size, number_of_messages);
}


wiced_result_t wiced_rtos_push_to_queue( wiced_queue_t* queue, void* message, uint32_t timeout_ms )
{
    return (wiced_result_t) host_rtos_push_to_queue( queue, message, timeout_ms );
}

wiced_result_t wiced_rtos_pop_from_queue( wiced_queue_t* queue, void* message, uint32_t timeout_ms )
{
    return (wiced_result_t) host_rtos_pop_from_queue(queue, message, timeout_ms );
}


 

/* Not yet implemented by other RTOSs */
wiced_result_t wiced_rtos_push_to_queue_front( wiced_queue_t* queue, void* message, uint32_t timeout_ms )
{
    if ( xQueueSendToFront( *queue, message, (TickType_t) ( timeout_ms / ms_to_tick_ratio ) ) != pdPASS )
    {
        return WICED_ERROR;
    }

    return WICED_SUCCESS;
}

wiced_result_t wiced_rtos_get_queue_occupancy( wiced_queue_t* queue, uint32_t *count )
{
    UNUSED_PARAMETER( queue );
    UNUSED_PARAMETER( count );
    return WICED_UNSUPPORTED;
}

wiced_result_t wiced_rtos_deinit_queue( wiced_queue_t* queue )
{
    return host_rtos_deinit_queue(queue );
}

wiced_result_t wiced_rtos_is_queue_empty( wiced_queue_t* queue )
{
    signed portBASE_TYPE result;

    taskENTER_CRITICAL();
    result = xQueueIsQueueEmptyFromISR( *queue );
    taskEXIT_CRITICAL();

    return ( result != 0 ) ? WICED_SUCCESS : WICED_ERROR;
}

wiced_result_t wiced_rtos_is_queue_full( wiced_queue_t* queue )
{
    signed portBASE_TYPE result;

    taskENTER_CRITICAL();
    result = xQueueIsQueueFullFromISR( *queue );
    taskEXIT_CRITICAL();

    return ( result != 0 ) ? WICED_SUCCESS : WICED_ERROR;
}

wiced_result_t wiced_rtos_init_semaphore( wiced_semaphore_t* semaphore )
{
    return (wiced_result_t) host_rtos_init_semaphore( (host_semaphore_type_t*) semaphore );
}

wiced_result_t wiced_rtos_set_semaphore( wiced_semaphore_t* semaphore )
{
    return (wiced_result_t) host_rtos_set_semaphore( (host_semaphore_type_t*) semaphore, host_platform_is_in_interrupt_context( ) );
}

wiced_result_t wiced_rtos_get_semaphore( wiced_semaphore_t* semaphore, uint32_t timeout_ms )
{
    return (wiced_result_t) host_rtos_get_semaphore( (host_semaphore_type_t*) semaphore, timeout_ms, WICED_FALSE );
}

wiced_result_t wiced_rtos_deinit_semaphore( wiced_semaphore_t* semaphore )
{
    return (wiced_result_t) host_rtos_deinit_semaphore( (host_semaphore_type_t*) semaphore );
}

wiced_result_t wiced_rtos_delay_milliseconds( uint32_t milliseconds )
{
    return (wiced_result_t) host_rtos_delay_milliseconds( milliseconds );
}

static void timer_callback( TimerHandle_t handle )
{
    wiced_timer_t* timer = (wiced_timer_t*) pvTimerGetTimerID( handle );

    if ( timer->function )
    {
        timer->function( timer->arg );
    }
}

wiced_result_t wiced_rtos_init_timer( wiced_timer_t* timer, 
                        uint32_t time_ms, timer_handler_t function, void* arg )
{
    wiced_assert("Bad args", timer != NULL);

    timer->function = function;
    timer->arg      = arg;

    timer->handle = xTimerCreate(  (const char *)"", (TickType_t)( time_ms / ms_to_tick_ratio ), pdTRUE, timer, (native_timer_handler_t) timer_callback );
    if ( timer->handle == NULL )
    {
        return WICED_ERROR;
    }
    return WICED_SUCCESS;
}


wiced_result_t wiced_rtos_start_timer( wiced_timer_t* timer )
{
    if ( timer->handle == NULL )
    {
        return WICED_ERROR;
    }

    if (host_platform_is_in_interrupt_context() == WICED_TRUE)
    {   /* in Interrupt */
        signed portBASE_TYPE result;
        signed portBASE_TYPE xHigherPriorityTaskWoken;

        result = xTimerResetFromISR( timer->handle, &xHigherPriorityTaskWoken );
        portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
        if ( result != pdPASS )
        {
            return WICED_ERROR;
        }

        result = xTimerStartFromISR( timer->handle, &xHigherPriorityTaskWoken );
        portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
        if ( result != pdPASS )
        {
            return WICED_ERROR;
        }
    }
    else
    {   /* in Task */
        if ( xTimerReset( timer->handle, WICED_WAIT_FOREVER ) != pdPASS )
        {
            return WICED_ERROR;
        }

        if ( xTimerStart( timer->handle, WICED_WAIT_FOREVER ) != pdPASS )
        {
            return WICED_ERROR;
        }
    }

    return WICED_SUCCESS;
}


wiced_result_t wiced_rtos_stop_timer( wiced_timer_t* timer )
{
    if ( timer->handle == NULL )
    {
        return WICED_ERROR;
    }

    if (host_platform_is_in_interrupt_context() == WICED_TRUE)
    {   /* in Interrupt */
        signed portBASE_TYPE xHigherPriorityTaskWoken;

        if ( xTimerStopFromISR( timer->handle, &xHigherPriorityTaskWoken ) != pdPASS )
        {
            return WICED_ERROR;
        }
        portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
    }
    else
    {   /* in Task */
        if ( xTimerStop( timer->handle, WICED_WAIT_FOREVER ) != pdPASS )
        {
            return WICED_ERROR;
        }
    }

    return WICED_SUCCESS;
}


wiced_result_t wiced_rtos_deinit_timer( wiced_timer_t* timer )
{
    if ( timer->handle == NULL )
    {
        return WICED_ERROR;
    }

    if ( xTimerDelete( timer->handle, WICED_WAIT_FOREVER ) != pdPASS )
    {
        return WICED_ERROR;
    }

    timer->handle = NULL;

    return WICED_SUCCESS;
}

wiced_result_t wiced_rtos_is_timer_running( wiced_timer_t* timer )
{
    if (timer->handle == NULL)
    {
        return WICED_ERROR;
    }

    return ( xTimerIsTimerActive( timer->handle ) != 0 ) ? WICED_SUCCESS : WICED_ERROR;
}


