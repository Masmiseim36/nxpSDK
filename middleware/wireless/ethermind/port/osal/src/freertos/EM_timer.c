
/**
 *  \file EM_timer.c
 *
 *  This File contains source codes for the EtherMind
 *  Timer Library Implementation for FreeRTOS.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* ----------------------------------------------- Header File Inclusion */
#include "EM_timer_internal.h"

#ifndef EM_ENABLE_PAL_TIMER

/* ----------------------------------------------- Global Definitions */
/* Timer Elements */
DECL_STATIC TIMER_ENTITY timer_entity[EM_TIMER_MAX_ENTITIES];
DECL_STATIC TIMER_ENTITY *timer_q_start = NULL;
DECL_STATIC TIMER_ENTITY *timer_q_end   = NULL;

/* Timer Library Mutex */
DECL_STATIC EM_thread_mutex_type timer_mutex;

/* ----------------------------------------------- Static Global Variables */


/* ----------------------------------------------- Functions */

void EM_timer_init (void)
{
    UINT16 index;

    EM_TIMER_TRC(
    "Initializing EtherMind Timer Library Module ...\n");

    /* Initialize Timer Mutex */
    /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
    (void) EM_thread_mutex_init(&timer_mutex, NULL);

    /* Initialize Timer Elements */
    for (index = 0U; index < EM_TIMER_MAX_ENTITIES; index ++)
    {
        (void) timer_init_entity(&timer_entity[index]);
        timer_entity[index].timer_id = NULL;
        timer_entity[index].handle = &timer_entity[index];
    }

    return;
}


void timer_em_init ( void )
{
    TIMER_ENTITY *timer;
    UINT16 index;
    TickType_t current_tick;

    /* Lock Timer */
    timer_lock();

    EM_TIMER_TRC(
    "Stack ON Initialization for Timer Library ...\n");

    /* Initialize Timer Entities */
    for (index = 0U; index < EM_TIMER_MAX_ENTITIES; index ++)
    {
        timer = &timer_entity[index];

        /* Create new one-shot timer */
        timer->timer_id = xTimerCreate
                          (
                              "Base Timer",
                              EM_TIMER_MS_2_TICKS(TIMER_TICK),
                              pdFALSE,
                              &current_tick,
                              timer_timeout_handler
                          );

        if (NULL == timer->timer_id)
        {
            EM_TIMER_ERR("*** FAILED to Create timer\n");
        }
    }

    /* Initialize Timer Q */
    timer_q_start = NULL;
    timer_q_end = NULL;

    timer_unlock();
    return;
}


void timer_em_shutdown ( void )
{
    UINT16 index;
    TIMER_ENTITY *timer;

    /* Lock Timer */
    timer_lock();

    /* Initialize Timer Q */
    timer_q_start = NULL;
    timer_q_end = NULL;

    /* Initialize Timer Entities */
    for (index = 0U; index < EM_TIMER_MAX_ENTITIES; index++)
    {
        timer = &timer_entity[index];
        if (TIMER_ENTITY_IN_USE == timer->in_use)
        {
            /* Stop Timer */
            (void) xTimerStop (timer->timer_id, 0U);

            if (EM_TIMER_STATIC_DATA_SIZE < timer->data_length)
            {
                timer_free(timer->allocated_data);
            }

            /* Delete Timer */
            (void) xTimerDelete(timer->timer_id, 0U);

            (void) timer_init_entity(timer);
            timer->timer_id = NULL;
        }
        else
        {
            /* Delete Timer */
            (void) xTimerDelete(timer->timer_id, 0U);
        }
    }

    EM_TIMER_TRC(
    "Stack OFF on Timer Library Module ...\n");

    timer_unlock();
    return;
}


EM_RESULT EM_start_timer
          (
              EM_timer_handle *handle,
              UINT32 timeout,
              void (* callback) (void *data, UINT16 datalen),
              void *data,
              UINT16 data_length
          )
{
    UCHAR *data_ptr = NULL;
    EM_RESULT retval;
    TIMER_ENTITY current_timer;

    if (NULL == handle)
    {
        EM_TIMER_ERR(
        "NULL Argument Unacceptable for Timer Handles.\n");

        return EM_TIMER_HANDLE_IS_NULL;
    }

    EM_TIMER_TRC(
    "Preparing to Add New Timer Entity. Timeout = %d, Data Size = %d.\n",
    timeout, data_length);

    /* Timer Library expects to have a valid callback */
    if (NULL == callback)
    {
        EM_TIMER_ERR(
        "FAILED to Add New Timer Element. NULL Callback.\n");

        return EM_TIMER_CALLBACK_IS_NULL;
    }

    /* Timer Library expects to have a valid Non-Zero timeout value */
    if (0U == timeout)
    {
        EM_TIMER_ERR(
        "FAILED to Add New Timer Element. Timeout Value set to ZERO!\n");

        return EM_TIMER_TIMEOUT_ZERO_NOT_ALLOWED;
    }

    if (0U != data_length)
    {
        if (data_length > EM_TIMER_STATIC_DATA_SIZE)
        {
            data_ptr = (UCHAR *) timer_malloc (data_length);
            if (NULL == data_ptr)
            {
                EM_TIMER_ERR(
                "FAILED to allocate Memory for Timer Handler Argument.\n");

                return EM_TIMER_MEMORY_ALLOCATION_FAILED;
            }
            current_timer.allocated_data = data_ptr;
        }
        else
        {
            data_ptr = current_timer.static_data;
        }

        /* Copy the Data to be passed to the Timer Handler */
        EM_mem_copy(data_ptr, data, data_length);
    }

    /* Store Timer Data Length, Callback & Timeout */
    current_timer.callback = callback;
    current_timer.data_length = data_length;
    current_timer.timeout = timeout;

    /* Lock Timer */
    timer_lock();

    /* Insert this Timer Entity into the List */
    retval = timer_add_entity(&current_timer);

    if (EM_SUCCESS != retval)
    {
        EM_TIMER_ERR(
        "FAILED to Add New Timer to Timer Queue. Error Code = 0x%04X\n",
        retval);

        if (EM_TIMER_STATIC_DATA_SIZE < current_timer.data_length)
        {
            timer_free (current_timer.allocated_data);
        }

        timer_unlock();
        return retval;
    }

    /* Store the Handle */
    *handle = current_timer.handle;

    EM_TIMER_TRC(
    "Successfully Added New Timer to Timer Queue. Handle = %p\n",
    (void *)*handle);

    timer_unlock();
    return EM_SUCCESS;
}


/* Callback registered with timer module */
void timer_timeout_handler (TimerHandle_t timer_id)
{
    EM_RESULT retval;
    TIMER_ENTITY *timer;

    EM_TIMER_TRC (
    "In Timer handler (Timer ID: 0x%04X)\n", timer_id);

    /* Lock Timer */
    timer_lock();

    /* Get the appropriate timer entity */
    retval = timer_search_entity_timer_id (&timer, timer_id);

    if (EM_SUCCESS != retval)
    {
        EM_TIMER_ERR(
        "*** UNKNOWN Spurious Timeout Callback?!?!\n");
        /* Unlock Timer */
        timer_unlock ();
        return;
    }

    /* Unlock Timer */
    timer_unlock ();

    if (timer->data_length > EM_TIMER_STATIC_DATA_SIZE)
    {
         /* Call the registered timeout handler */
         timer->callback (timer->allocated_data, timer->data_length);
    }
    else
    {
        /* Use Static Data */
        timer->callback (timer->static_data, timer->data_length);
    }

    /* Lock Timer */
    timer_lock ();

    /* Stop Timer */
    (void) xTimerStop  (timer->timer_id, 0U);

    /* Free the Timer */
    (void) timer_del_entity (timer, 1U);

    /* Unlock Timer */
    timer_unlock ();
    return;
}


EM_RESULT EM_stop_timer
          (
              EM_timer_handle handle
          )
{
    TIMER_ENTITY *timer;
    EM_RESULT retval;
    TimerHandle_t  timer_id;

    if (NULL == handle)
    {
        EM_TIMER_ERR(
        "NULL Argument Unacceptable for Timer Handles.\n");

        return EM_TIMER_HANDLE_IS_NULL;
    }

    /* Lock Timer */
    timer_lock();

    timer = (TIMER_ENTITY *)handle;

    /* Store the timer id before deleting entity */
    timer_id = timer->timer_id;

    retval = timer_del_entity(timer, 0x01U);

    if (EM_SUCCESS != retval)
    {
        EM_TIMER_ERR(
        "FAILED to find Timer Element. Handle = %p. Error Code = 0x%04X\n",
        (void *)handle, retval);
    }
    else
    {
        EM_TIMER_TRC(
        "Successfully Deleted Timer Element for Handle %p.\n",
        (void *)handle);

        /* Stop Timer */
        (void) xTimerStop (timer_id, 0U);

        EM_TIMER_TRC("*** Stopped Timer [ID: %04X]\n",
        timer_id);
    }

    /* Unlock Timer */
    timer_unlock();

    return retval;
}


EM_RESULT EM_restart_timer
          (
              EM_timer_handle handle,
              UINT16 new_timeout
          )
{
    TIMER_ENTITY *timer;
    BaseType_t   ret;
    EM_RESULT retval;

    if (NULL == handle)
    {
        EM_TIMER_ERR(
        "NULL Argument Unacceptable for Timer Handles.\n");

        return EM_TIMER_HANDLE_IS_NULL;
    }

    /* Lock Timer */
    timer_lock();

    timer = (TIMER_ENTITY *)handle;

    retval = timer_search_entity(timer);

    if (EM_SUCCESS != retval)
    {
        EM_TIMER_ERR(
        "FAILED to Find Timer ELement for Handle %p. Error Code = 0x%04X\n",
        (void *)handle, retval);
    }
    else
    {
        timer->timeout = new_timeout;

        /* Stop the existing timer */
        (void) xTimerStop (timer->timer_id, 0U);

        /* Change Timeout. This will also start the timer. */
        ret = xTimerChangePeriod
              (
                  timer->timer_id,
                  EM_TIMER_MS_2_TICKS
                  ((EM_TIMEOUT_MILLISEC & timer->timeout) ?
                   (timer->timeout & (UINT32)~(EM_TIMEOUT_MILLISEC)):
                   (timer->timeout * 1000U)),
                  0U
              );

        if (pdPASS != ret)
        {
            EM_TIMER_ERR("*** FAILED to Restart timer\n");
            (void) timer_del_entity (timer, 0x01U);
            return EM_TIMER_FAILED_SET_TIME_EVENT;
        }

        EM_TIMER_TRC(
        "Successfully restarted Timer [ID: %04X]\n", timer->timer_id);
    }

    timer_unlock();
    return retval;
}


EM_RESULT EM_is_active_timer
          (
              EM_timer_handle handle
          )
{
    TIMER_ENTITY *timer;
    EM_RESULT retval;

    if (NULL == handle)
    {
        EM_TIMER_ERR(
        "NULL Argument Unacceptable for Timer Handles.\n");

        return EM_TIMER_HANDLE_IS_NULL;
    }

    timer_lock();

    timer = (TIMER_ENTITY *)handle;

    retval = timer_search_entity(timer);

    if (EM_SUCCESS != retval)
    {
        EM_TIMER_ERR(
        "FAILED to Find the Timer Entity for Handle %p. Error Code = 0x%04X\n",
        (void *)handle, retval);
    }

    timer_unlock();

    return retval;
}


EM_RESULT timer_search_entity ( TIMER_ENTITY *timer )
{
    TIMER_ENTITY *current_timer;

    /* Is Queue Empty */
    if (NULL == timer_q_start)
    {
        return EM_TIMER_QUEUE_EMPTY;
    }

    /* Handle the first Element */
    if (timer == timer_q_start)
    {
        return EM_SUCCESS;
    }

    current_timer = timer_q_start->next;

    while (NULL != current_timer)
    {
        if (timer == current_timer)
        {
            return EM_SUCCESS;
        }

        current_timer = current_timer->next;
    }

    return EM_TIMER_ENTITY_SEARCH_FAILED;
}

/* Get the timer based on obtained timer id */
EM_RESULT timer_search_entity_timer_id
           (
               TIMER_ENTITY **timer,
               TimerHandle_t   timer_id
           )
{
    TIMER_ENTITY *current_timer;

    /* Is Queue Empty */
    if (NULL == timer_q_start)
    {
        return EM_TIMER_QUEUE_EMPTY;
    }

    /* Handle the first Element */
    if (timer_id == timer_q_start->timer_id)
    {
        /* Note the timer entity */
        *timer = timer_q_start;
        return EM_SUCCESS;
    }

    current_timer = timer_q_start->next;

    while (NULL != current_timer)
    {
        if (timer_id == current_timer->timer_id)
        {
            /* Note the timer entity */
            *timer = current_timer;
            return EM_SUCCESS;
        }

        current_timer = current_timer->next;
    }

    return EM_TIMER_ENTITY_SEARCH_FAILED;
}

EM_RESULT timer_add_entity ( TIMER_ENTITY *timer )
{
    UINT16 index;
    BaseType_t   ret;
    TIMER_ENTITY *new_timer;

    new_timer = NULL;

    for (index = 0U; index < EM_TIMER_MAX_ENTITIES; index++)
    {
        new_timer = &timer_entity[index];
        if (TIMER_ENTITY_FREE == new_timer->in_use)
        {
            new_timer->in_use = TIMER_ENTITY_IN_USE;
            break;
        }
        else
        {
            new_timer = NULL;
        }
    }

    if (NULL == new_timer)
    {
        EM_TIMER_ERR(
        "FAILED to Allocate New Timer Entity. Timer List FULL !\n");

#ifdef EM_STATUS
        /* Timer List Full: Update EtherMind Status Flag */
        EM_status_set_bit (STATUS_BIT_TIMER_ENTITY_FULL, STATUS_BIT_SET);
#endif /* EM_STATUS */

        return EM_TIMER_QUEUE_FULL;
    }

    new_timer->next = NULL;
    new_timer->timeout = timer->timeout;
    new_timer->callback = timer->callback;
    new_timer->data_length = timer->data_length;
    if (new_timer->data_length > EM_TIMER_STATIC_DATA_SIZE)
    {
        new_timer->allocated_data = timer->allocated_data;
    }
    else
    {
        EM_mem_copy
        (
            new_timer->static_data,
            timer->static_data,
            new_timer->data_length
        );
    }

    /* Start timer. Set Timeout. This will also start the timer. */
#ifdef EM_TIMER_SUPPORT_REMAINING_TIME
    new_timer->start_timestamp = xTaskGetTickCount();
#endif /* EM_TIMER_SUPPORT_REMAINING_TIME */

    ret = xTimerChangePeriod
          (
              new_timer->timer_id,
              EM_TIMER_MS_2_TICKS
              ((EM_TIMEOUT_MILLISEC & new_timer->timeout) ?
               (new_timer->timeout & (UINT32)~(EM_TIMEOUT_MILLISEC)):
               (new_timer->timeout * 1000U)),
              0U
          );

    if (pdPASS != ret)
    {
        EM_TIMER_ERR("*** FAILED to Start timer\n");

        /* Mark the timer entity as Free */
        new_timer->in_use = TIMER_ENTITY_FREE;

        return EM_TIMER_FAILED_SET_TIME_EVENT;
    }

    EM_TIMER_TRC("Successfully started Timer [ID: %04X]\n",
    new_timer->timer_id);

    timer->handle = new_timer->handle;

    /* If the Timer Q Empty */
    if (NULL == timer_q_start)
    {
        timer_q_start = new_timer;
        timer_q_end = new_timer;
        return EM_SUCCESS;
    }

    timer_q_end->next = new_timer;
    timer_q_end = new_timer;

    return EM_SUCCESS;
}


EM_RESULT timer_del_entity
          (
              TIMER_ENTITY *timer,
              UCHAR free
          )
{
    TIMER_ENTITY *current_timer, *previous_timer;

    /* Either None or One Element */
    if (timer_q_start == timer_q_end)
    {
        if (NULL == timer_q_start)
        {
            /* Queue is Empty */
            return EM_TIMER_QUEUE_EMPTY;
        }
        else
        {
            if (timer == timer_q_start)
            {
                /* Queue has One Element */
                timer_q_start = NULL;
                timer_q_end = NULL;
            }
            else
            {
                /* Match NOT found in the Only element in Timer Queue */
                return EM_TIMER_ENTITY_SEARCH_FAILED;
            }
        }
    }
    else
    {
        /* Queue has more than One Element */
        if (timer == timer_q_start)
        {
            /* Match in the First Element */
            timer_q_start = timer_q_start->next;
        }
        else
        {
            previous_timer = timer_q_start;
            current_timer = timer_q_start->next;

            while (NULL != current_timer)
            {
                if (timer == current_timer)
                {
                    previous_timer->next = current_timer->next;

                    if (current_timer == timer_q_end)
                    {
                        timer_q_end = previous_timer;
                    }

                    break;
                }

                previous_timer = current_timer;
                current_timer  = current_timer->next;
            }

            if (NULL == current_timer)
            {
                return EM_TIMER_ENTITY_SEARCH_FAILED;
            }
        }
    }

    /* Free Allocated Data */
    if ((0x01U == free) &&
        (timer->data_length > EM_TIMER_STATIC_DATA_SIZE))
    {
        timer_free (timer->allocated_data);
    }

    (void) timer_init_entity(timer);
    return EM_SUCCESS;
}


EM_RESULT timer_init_entity (TIMER_ENTITY *timer)
{
    timer->in_use = TIMER_ENTITY_FREE;
    timer->timeout = 0U;
    timer->callback = NULL;
    timer->allocated_data = NULL;
    timer->data_length = 0U;
    timer->next = NULL;

#ifdef EM_TIMER_SUPPORT_REMAINING_TIME
    timer->start_timestamp = 0U;
#endif /* EM_TIMER_SUPPORT_REMAINING_TIME */

    return EM_SUCCESS;
}

#ifdef EM_TIMER_SUPPORT_REMAINING_TIME
EM_RESULT EM_timer_get_remaining_time
          (
              EM_timer_handle   handle,
              UINT32          * remaining_time_ms
          )
{
    TIMER_ENTITY *timer;
    EM_RESULT     retval;
    TickType_t    current_timestamp, diff_in_ticks;
    UINT32        time_ms, elapsed_time_ms;

    if (NULL == handle)
    {
        EM_TIMER_ERR(
        "NULL Argument Unacceptable for Timer Handles.\n");

        return EM_TIMER_HANDLE_IS_NULL;
    }

    /* Lock Timer */
    timer_lock();

    timer = (TIMER_ENTITY *)handle;

    retval = timer_search_entity(timer);

    if (EM_SUCCESS != retval)
    {
        EM_TIMER_ERR(
        "FAILED to Find Timer ELement for Handle %p. Error Code = 0x%04X\n",
        (void *)handle, retval);
    }
    else
    {
        time_ms = ((EM_TIMEOUT_MILLISEC & timer->timeout) ?
            (timer->timeout & (UINT32)~(EM_TIMEOUT_MILLISEC)) :
            (timer->timeout * 1000U));

        /* Get Current Time */

        /* Remaining Time = (Timeout in ms) - (Current Time - Timer Start Time) */
        current_timestamp = xTaskGetTickCount();

        /* Get the Ticks difference */
        if (current_timestamp >= timer->start_timestamp)
        {
            /* Difference */
            diff_in_ticks = (current_timestamp - timer->start_timestamp);
        }
        else
        {
            /* Difference */
            diff_in_ticks = (0xFFFFFFFFU - timer->start_timestamp) + current_timestamp;
        }

        /* Convert Ticks to ms */
        elapsed_time_ms = EM_TIMER_TICKS_2_MS(diff_in_ticks);

        /* Calculate remaining time */

        if (time_ms < elapsed_time_ms)
        {
            EM_TIMER_ERR(
            "FAILED to Find Remaining Time.TO:%d < Elapsed Time:%d\n",
            time_ms, elapsed_time_ms);
        }
        else
        {
            time_ms -= elapsed_time_ms;
            *remaining_time_ms = time_ms;

            EM_TIMER_TRC(
            "[EM_TIMER] Remaining Time (ms): %d\n", time_ms);

            retval = EM_SUCCESS;
        }
    }

    timer_unlock();

    return retval;
}
#endif /* EM_TIMER_SUPPORT_REMAINING_TIME */

EM_RESULT EM_list_timer ( void )
{
#ifdef EM_TIMERL_DEBUG

    UINT16 index, free;
    TIMER_ENTITY *timer;

    timer_lock();
    EM_TIMERL_TRC("\n");
    EM_TIMERL_TRC("========================================= \n");

    EM_TIMERL_TRC("Start Q = %p\n", timer_q_start);

    timer = timer_q_start;
    while(NULL != timer)
    {
        EM_TIMERL_TRC("    Handle = %p",
        timer->handle);

        timer = timer->next;
    }

    EM_TIMERL_TRC("End   Q = %p\n", timer_q_end);

    free = 0U;
    for (index = 0U; index < EM_TIMER_MAX_ENTITIES; index ++)
    {
        if (TIMER_ENTITY_FREE == timer_entity[index].in_use)
        {
            free ++;
        }
    }

    EM_TIMERL_TRC("Max Q Entity = %d, Free = %d\n",
    EM_TIMER_MAX_ENTITIES, free);

    EM_TIMERL_TRC("========================================= \n");
    EM_TIMERL_TRC("\n");

    timer_unlock();

#endif /* EM_TIMERL_DEBUG */

    return EM_SUCCESS;
}

#endif /* EM_ENABLE_PAL_TIMER */

