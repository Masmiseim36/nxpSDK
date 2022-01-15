
/**
 *  \file EM_timer_internal.h
 *
 *  This Header File contains Internal Declarations of Structures,
 *  Functions and Global Definitions for FreeRTOS.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_EM_TIMER_AL_INTERNAL_
#define _H_EM_TIMER_AL_INTERNAL_

/* ----------------------------------------------- Header File Inclusion */
#include "EM_timer.h"
#include "EM_debug.h"

/* ----------------------------------------------- Timer Debug Macros */

#ifdef EM_TIMERL_DEBUG
#define EM_TIMERL_TRC(...)      EM_debug_trace(bt_debug_fd, __VA_ARGS__)
#else  /* EM_TIMERL_DEBUG */
#define EM_TIMERL_TRC           EM_debug_null
#endif /* EM_TIMERL_DEBUG */

#ifdef EM_TIMERT_DEBUG
#define EM_TIMERT_TRC(...)      EM_debug_trace(bt_debug_fd, __VA_ARGS__)
#else  /* EM_TIMERT_DEBUG */
#define EM_TIMERT_TRC           EM_debug_null
#endif /* EM_TIMERT_DEBUG */

#ifdef EM_TIMER_DEBUG

#define EM_TIMER_ERR(...)       EM_debug_error(bt_debug_fd, __VA_ARGS__)
#define EM_TIMER_TRC(...)       EM_debug_trace(bt_debug_fd, __VA_ARGS__)
#define EM_TIMER_INF(...)       EM_debug_info(bt_debug_fd, __VA_ARGS__)

#else  /* EM_TIMER_DEBUG */

#define EM_TIMER_ERR            EM_debug_null
#define EM_TIMER_TRC            EM_debug_null
#define EM_TIMER_INF            EM_debug_null

#endif /* EM_TIMER_DEBUG */


/* ----------------------------------------------- Global Definitions */

/* Timer Malloc/Free Related --------- */
#define timer_malloc            EM_alloc_mem
#define timer_free              EM_free_mem

/* Lock/Unlock Timer Library */
#define timer_lock()            (void) EM_thread_mutex_lock(&timer_mutex)
#define timer_unlock()          (void) EM_thread_mutex_unlock(&timer_mutex)

/* Null Check for Timer Handles */
#define timer_null_check(p)     if (NULL == (p)) return -1;

/* Timer Task State Values */
#define TIMER_INIT              0x00U
#define TIMER_WAITING           0x01U
#define TIMER_RUNNING           0x02U
#define TIMER_SHUTDOWN          0x03U
#define TIMER_IMMEDIATE         0x04U

/* One Timer Tick => 10 MilliSecond */
#define TIMER_TICK              10U

/* Timer task yield period (in Microsecond) */
#define TIMER_YIELD_PERIOD      (1 * 1000)

/* ----------------------------------------------- Structures/Data Types */


/* ----------------------------------------------- Internal Functions */

#ifdef __cplusplus
extern "C" {
#endif

EM_RESULT timer_add_entity ( TIMER_ENTITY *timer );
EM_RESULT timer_del_entity ( TIMER_ENTITY *timer, UCHAR free );
EM_RESULT timer_search_entity ( TIMER_ENTITY *timer );
EM_RESULT timer_init_entity ( TIMER_ENTITY *timer );

/* Get the timer based on obtained timer id */
EM_RESULT timer_search_entity_timer_id
           (
               TIMER_ENTITY **timer,
               TimerHandle_t   timer_id
           );

/* Callback registered with timer module */
void timer_timeout_handler (TimerHandle_t timer_id);

#ifdef __cplusplus
};
#endif

#endif /* _H_EM_TIMER_AL_INTERNAL_ */

