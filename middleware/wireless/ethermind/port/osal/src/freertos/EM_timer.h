
/**
 *  \file EM_timer.h
 *
 *  This Header File contains the APIs and the ADTs exported by the
 *  EtherMind Timer Library for FreeRTOS.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_EM_TIMER_AL_
#define _H_EM_TIMER_AL_

/* --------------------------------------------------- Header File Inclusion */
#include "EM_os.h"

#define EM_TIMER_SUPPORT_REMAINING_TIME

/* --------------------------------------------------- Global Definitions */
/* Maximum number of timer entities */
#define EM_TIMER_MAX_ENTITIES                       10U

/* Mask to indicate millisecond timeout */
#define EM_TIMEOUT_MILLISEC                         0x80000000U

/* Timer Handles must be initialized to this value */
#define EM_TIMER_HANDLE_INIT_VAL                    NULL

#define EM_TIMER_STATIC_DATA_SIZE                   32U

/* Flag: Timer Entity State */
#define TIMER_ENTITY_FREE                           0x00U
#define TIMER_ENTITY_IN_USE                         0x01U
#define TIMER_ENTITY_IN_FREE                        0x02U

/* Flag: Timer Entity Data to be freed or not */
#define TIMER_ENTITY_HOLD_ALLOC_DATA                0x00U
#define TIMER_ENTITY_FREE_ALLOC_DATA                0x01U

/* Timer module ID and Error codes */
#define EM_TIMER_ERR_ID                            0xC000U

#define EM_TIMER_MUTEX_INIT_FAILED                 (0x0001U | EM_TIMER_ERR_ID)
#define EM_TIMER_COND_INIT_FAILED                  (0x0002U | EM_TIMER_ERR_ID)
#define EM_TIMER_MUTEX_LOCK_FAILED                 (0x0003U | EM_TIMER_ERR_ID)
#define EM_TIMER_MUTEX_UNLOCK_FAILED               (0x0004U | EM_TIMER_ERR_ID)
#define EM_TIMER_MEMORY_ALLOCATION_FAILED          (0x0005U | EM_TIMER_ERR_ID)

#define EM_TIMER_HANDLE_IS_NULL                    (0x0011U | EM_TIMER_ERR_ID)
#define EM_TIMER_CALLBACK_IS_NULL                  (0x0012U | EM_TIMER_ERR_ID)
#define EM_TIMER_QUEUE_EMPTY                       (0x0013U | EM_TIMER_ERR_ID)
#define EM_TIMER_QUEUE_FULL                        (0x0014U | EM_TIMER_ERR_ID)
#define EM_TIMER_ENTITY_SEARCH_FAILED              (0x0015U | EM_TIMER_ERR_ID)
#define EM_TIMER_NULL_PARAMETER_NOT_ALLOWED        (0x0016U | EM_TIMER_ERR_ID)
#define EM_TIMER_TIMEOUT_ZERO_NOT_ALLOWED          (0x0017U | EM_TIMER_ERR_ID)
#define EM_TIMER_FAILED_SET_TIME_EVENT             (0x0018U | EM_TIMER_ERR_ID)

/* ----------------------------------------------- Structures/Data Types */

/* Timer Entity */
typedef struct timer_entity_struct
{
    /* The Timer Handle */
    struct timer_entity_struct *handle;

    /* Callback to call when Timer expires */
    void (* callback) (void *data, UINT16 datalen);

    /**
     * Timer Callback Parameter if
     * data_length > EM_TIMER_STATIC_DATA_SIZE
     */
    UCHAR  *allocated_data;

    /* Next Element in the Timer Q */
    struct timer_entity_struct *next;

    /**
     * Timer Callback Parameter if
     * data_length <= EM_TIMER_STATIC_DATA_SIZE
     */
    UCHAR  static_data[EM_TIMER_STATIC_DATA_SIZE];

    /* Timeout Value asked by the User */
    UINT32 timeout;

#ifdef EM_TIMER_SUPPORT_REMAINING_TIME
    /* Start Time Stamp - used to calculate remaining time */
    TickType_t start_timestamp;
#endif /* EM_TIMER_SUPPORT_REMAINING_TIME */

    /* Length of the data */
    UINT16 data_length;

    /* Is this Entity Allocated ? */
    UCHAR in_use;

    /* FreeRTOS Timer ID */
    TimerHandle_t timer_id;

} TIMER_ENTITY;

typedef TIMER_ENTITY *  EM_timer_handle;


/* --------------------------------------------------- Macros */
/* Port conversion macros to convert between ticks and ms */
#define EM_TIMER_MS_2_TICKS(ms)         ((ms)/portTICK_PERIOD_MS)
#define EM_TIMER_TICKS_2_MS(ticks)      ((ticks) * portTICK_PERIOD_MS)

/* --------------------------------------------------- Function Declarations */

#ifdef __cplusplus
extern "C" {
#endif

void EM_timer_init ( void );
void timer_em_init ( void );
void timer_em_shutdown ( void );

EM_RESULT EM_start_timer
          (
              EM_timer_handle *handle,
              UINT32 timeout,
              void (* callback) (void *data, UINT16 datalen),
              void *data,
              UINT16 data_length
          );

EM_RESULT EM_restart_timer
          (
              EM_timer_handle handle,
              UINT16 new_timeout
          );

EM_RESULT EM_stop_timer ( EM_timer_handle handle );

#ifdef EM_TIMER_SUPPORT_REMAINING_TIME
EM_RESULT EM_timer_get_remaining_time
          (
              EM_timer_handle   handle,
              UINT32          * remaining_time_ms
          );
#else /* EM_TIMER_SUPPORT_REMAINING_TIME */
#define EM_timer_get_remaining_time(a, b)
#endif /* EM_TIMER_SUPPORT_REMAINING_TIME */

EM_RESULT EM_is_active_timer ( EM_timer_handle handle );

/* Debug Routine - Internal Use Only */
EM_RESULT EM_list_timer ( void );

#ifdef __cplusplus
};
#endif

#endif /* _H_EM_TIMER_AL_ */

