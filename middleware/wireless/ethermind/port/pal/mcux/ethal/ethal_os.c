
/**
 *  \file ethal_os.c
 *
 *  This source file is part of EtherMind OS Abstraction module.
 *  In this file the platform dependent library calls are abstracted
 *  and mapped to calls used within the EtherMind Stack.
 *
 *  Version: FSL
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* -------------------------------------------- Header File Inclusion */
#include "ethal_os.h"

#ifdef EM_ENABLE_PAL_OS

/* -------------------------------------------- External Global Variables */

/* -------------------------------------------- Exported Global Variables */

/* -------------------------------------------- Static Global Variables */
/* Application signal notification handler */
/* static void(*em_process_term_handler)(void); */

/* Task stack overflow handler */
static void(*em_sof_handler)(void * task_name);

/* -------------------------------------------- Functions */

void EM_os_init (void)
{
    /**
     *  For FreeRTOS, this function needs to be called from BT_bluetooth_on
     *  and needed threads much have been created when the scheduler is started
     */
}

/**
 *  \fn EM_thread_create
 *
 *  \brief To create a task/thread
 *
 *  \par Description:
 *  This function implements the OS Wrapper for the Thread Create call.
 *  It creates a new thread of control  that  executes concurrently  with
 *  the  calling  thread.
 *
 *  \param [out] thread
 *         Caller allocated location to hold the Thread ID
 *         for the newly Created Thread
 *  \param [in] attr
 *         The Thread Attributes, pre-initialized using EM_thread_attr_init()
 *  \param [in] start_routine
 *         The Thread Start Routine for the newly created task/thread.
 *         Upon creation, the new task/thread calls this function,
 *         passing it 'thread_args' as the argument
 *  \param [in] thread_args
 *         This parameter points to a caller allocated "resident" memory
 *         location, containing the arguments to be passed to
 *         the newly created task's/thread's start routine
 *
 *  \return INT32 : 0 on Success. -1 on Failure.
 */
INT32 EM_thread_create
      (
          /* IN */  EM_thread_type *         thread,
          /* IN */  EM_thread_attr_type *    attr,
          /* IN */  EM_THREAD_START_ROUTINE  start_routine,
          /* IN */  EM_THREAD_ARGS           thread_args
      )
{
    osa_status_t   retval;
    osa_task_def_t l_task_attr_defs;

    /* Allocate memory for Task Handle */
    (*thread) = EM_alloc_mem(EM_THREAD_HANDLE_SIZE);

    if (NULL == (*thread))
    {
        /* Task Handle Allocation Failed! */
        return -1;
    }

    /* Assign values to OSA's task def parameter from Thread Attribute */
    if (NULL != attr)
    {
        /* Thread Start Routine */
        l_task_attr_defs.pthread   = start_routine;
        /* Thread Priority */
        l_task_attr_defs.tpriority = PRIORITY_RTOS_TO_OSA (attr->thread_priority);
        /* Num of Thread Instances */
        l_task_attr_defs.instances = 1U;
        /* Thread Stack Size */
        l_task_attr_defs.stacksize = attr->thread_stack_size;
        /* Thread Stack Pointer */
        l_task_attr_defs.tstack    = NULL;
        /* Thread Link Pointer */
        l_task_attr_defs.tlink     = NULL;
        /* Thread Name */
        l_task_attr_defs.tname     = (uint8_t *)attr->thread_name;
        /* Thread Use Float Flag */
        l_task_attr_defs.useFloat  = 0U;
    }
    else
    {
        /**
         * TODO:
         * Assign some default values if the Thread Attribute is not Passed!
         */
        /* Thread Start Routine */
        l_task_attr_defs.pthread   = start_routine;
        /* Thread Priority */
        l_task_attr_defs.tpriority = PRIORITY_RTOS_TO_OSA (EM_OS_TASK_PRIORITY);
        /* Num of Thread Instances */
        l_task_attr_defs.instances = 1U;
        /* Thread Stack Size */
        l_task_attr_defs.stacksize = EM_OS_TASK_STACKDEPTH;
        /* Thread Stack Pointer */
        l_task_attr_defs.tstack    = NULL;
        /* Thread Link Pointer */
        l_task_attr_defs.tlink     = NULL;
        /* Thread Name */
        l_task_attr_defs.tname     = (uint8_t *)EM_OS_DEFAULT_TASK_NAME(start_routine);
        /* Thread Use Float Flag */
        l_task_attr_defs.useFloat  = 0U;
    }

    /* Call to OSA task creation */
    retval = OSA_TaskCreate
             (
                 (osa_task_handle_t)(*thread),
                 &l_task_attr_defs,
                 thread_args
             );

    return (INT32)retval;
}

/**
 *  \fn EM_thread_delete
 *
 *  \brief To delete a task/thread
 *
 *  \par Description:
 *  This function implements the OS Wrapper for the Thread Delete call.
 *
 *  \param [in] thread
 *         This parameter indicates the Thread Handle.
 *
 *  \return INT32 : 0 on Success. -1 on Failure.
 */
INT32 EM_thread_delete
      (
          /* IN */ EM_thread_type thread
      )
{

    osa_status_t ret;

    ret = OSA_TaskDestroy(thread);
    if (KOSA_StatusSuccess == ret)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}


/**
 *  \fn EM_thread_attr_init
 *
 *  \brief To initialize a task's/thread's attributes to default settings
 *
 *  \par Description:
 *  This function implements the OS Wrapper for the Thread attribute init
 *  call. It initializes the given thread attribute object with default
 *  values.
 *
 *  \param [out] attr
 *         Thread attribute object to be initialized
 *
 *  \return INT32 : 0 on Success. -1 on Failure.
 */
INT32 EM_thread_attr_init
      (
          /* OUT */ EM_thread_attr_type *    attr
      )
{
    (void) attr;
    return 0U;
}


/**
 *  \fn EM_thread_mutex_init
 *
 *  \brief To initialize a Mutex object
 *
 *  \par Description:
 *  This function implements the OS Wrapper for the Thread Mutex Init Call.
 *  It creates or initializes the mutex object and fills it with default
 *  values for the attributes.
 *
 *  \param [out] mutex
 *         The Mutex variable to be Initialized
 *  \param [in] mutex_attr
 *         Attribute of the mutex variable on initialization
 *
 *  \return INT32 : 0 on Success. -1 on Failure.
 */
INT32 EM_thread_mutex_init
      (
          /* OUT */ EM_thread_mutex_type *         mutex,
          /* IN */  EM_thread_mutex_attr_type *    mutex_attr
      )

{
    osa_status_t ret;

    (void) mutex_attr;

    if (NULL != mutex)
    {
        /* Allocate memory for Mutex */
        (*mutex) = EM_alloc_mem(EM_THREAD_MUTEX_SIZE);

        if (NULL != (*mutex))
        {
            ret = OSA_MutexCreate((*mutex));

            if (KOSA_StatusSuccess == ret)
            {
                return 0;
            }
        }
    }

    return -1;
}

/**
 *  \fn EM_thread_mutex_deinit
 *
 *  \brief To deinitialize a Mutex object
 *
 *  \par Description:
 *  This function implements the OS Wrapper for the Thread Mutex DeInit Call.
 *  It destroys or de-initializes the mutex object
 *
 *  \param [out] mutex
 *         The Mutex variable to be De-Initialized
 *
 *  \return INT32 : 0 on Success. -1 on Failure.
 */
INT32 EM_thread_mutex_deinit
      (
          /* IN */ EM_thread_mutex_type      * mutex
      )
{
    osa_status_t ret;

    if (NULL != mutex)
    {
        ret = OSA_MutexDestroy((*mutex));
        if (KOSA_StatusSuccess == ret)
        {
            return 0;
        }
    }

    return -1;
}


/**
 *  \fn EM_thread_mutex_lock
 *
 *  \brief To lock a Mutex object
 *
 *  \par Description:
 *  This function implements the OS Wrapper for the Thread Mutex Lock call.
 *  It locks the given mutex.
 *  If the given mutex is currently unlocked, it becomes locked and owned by
 *  the calling thread, and this routine returns immediately.
 *  If the mutex is already locked by another thread, this routine suspends
 *  the calling thread until the mutex is unlocked.
 *
 *  \param [in,out] mutex
 *         The Mutex variable to be locked
 *
 *  \return INT32 : 0 on Success. -1 on Failure.
 */
INT32 EM_thread_mutex_lock
      (
          /* INOUT */ EM_thread_mutex_type *    mutex
      )
{
    osa_status_t ret;

    if (NULL != mutex)
    {
        ret = OSA_MutexLock((*mutex), osaWaitForever_c);
        if (KOSA_StatusSuccess == ret)
        {
            return 0;
        }
    }

    return -1;
}


/**
 *  \fn EM_thread_mutex_unlock
 *
 *  \brief To unlock a Mutex object
 *
 *  \par Description:
 *  This function implements the OS Wrapper for the Mutex Unlock call.
 *  It unlocks the given mutex. The mutex is assumed to be locked and
 *  owned by the calling thread.
 *
 *  \param [in,out] mutex
 *         The Mutex variable to be unlocked
 *
 *  \return INT32 : 0 on Success. -1 on Failure.
 */
INT32 EM_thread_mutex_unlock
      (
          /* INOUT */ EM_thread_mutex_type *    mutex
      )
{
    osa_status_t ret;

    if (NULL != mutex)
    {
        ret = OSA_MutexUnlock((*mutex));
        if (KOSA_StatusSuccess == ret)
        {
            return 0;
        }
    }

    return -1;
}


/**
 *  \fn EM_thread_cond_init
 *
 *  \brief To initialize a Conditional Variable object
 *
 *  \par Description:
 *  This function implements the Conditional Variable Initialisation call.
 *  It initializes the given conditional variable.
 *
 *  \param [out] cond
 *         The Conditional Variable
 *  \param [in] cond_attr
 *         Attributes of the conditional variable on initialization
 *
 *  \return INT32 : 0 on Success. -1 on Failure.
 *
 *  \note
 *  A conditional variable is a synchronization primitive that allows
 *  threads to suspend execution and relinquish the processors
 *  until some predicate on shared resources is satisfied.
 */
INT32 EM_thread_cond_init
      (
          /* OUT */ EM_thread_cond_type *         cond,
          /* IN */  EM_thread_cond_attr_type *    cond_attr
      )
{
    osa_status_t ret;

    (void) cond_attr;

    if (NULL != cond)
    {
        /* Allocate memory for Conditional Variable */
        (*cond) = EM_alloc_mem(EM_THREAD_COND_SIZE);

        if (NULL != (*cond))
        {
            ret = OSA_SemaphoreCreate ((*cond), 0x01U);
            if (KOSA_StatusSuccess == ret)
            {
                return 0;
            }
        }
    }

    return -1;
}

/**
 *  \fn EM_thread_cond_deinit
 *
 *  \brief To deinitialize a Condvar object
 *
 *  \par Description:
 *  This function implements the OS Wrapper for the Thread CondVar DeInit Call.
 *  It destroys or de-initializes the condvar object
 *
 *  \param [out] mutex
 *         The Conditional variable to be De-Initialized
 *
 *  \return INT32 : 0 on Success. -1 on Failure.
 */
INT32 EM_thread_cond_deinit
      (
          /* IN */ EM_thread_cond_type      * cond
      )
{
    osa_status_t ret;

    if (NULL != cond)
    {
        ret = OSA_SemaphoreDestroy((*cond));

        if (KOSA_StatusSuccess == ret)
        {
            return 0;
        }
    }

    return -1;
}


/**
 *  \fn EM_thread_cond_wait
 *
 *  \brief To wait on a Conditional Variable object
 *
 *  \par Description:
 *  This function implements the OS Wrapper for the Thread Wait on
 *  Conditional Variable call.
 *  It waits for the conditional variable to be signaled.
 *  This routine releases the  mutex and waits for the condition variable
 *  to  be signaled. The execution of the calling thread is suspended
 *  until the conditional variable is signaled.
 *  The mutex must be locked by the calling thread before
 *  calling this function. Before returning to the calling thread,
 *  it will re-acquire the mutex variable.
 *
 *  \param [in,out] cond
 *         The Conditional Variable
 *  \param [in,out] cond_mutex
 *         The Conditional Mutex Variable
 *
 *  \return INT32 : 0 on Success. -1 on Failure.
 */
INT32 EM_thread_cond_wait
      (
          /* INOUT */ EM_thread_cond_type *     cond,
          /* INOUT */ EM_thread_mutex_type *    cond_mutex
      )
{
    osa_status_t ret;

    if ((NULL != cond) &&
        (NULL != cond_mutex))
    {
        (void)EM_thread_mutex_unlock (cond_mutex);
        ret = OSA_SemaphoreWait((*cond), osaWaitForever_c);
        (void)EM_thread_mutex_lock (cond_mutex);

        if (KOSA_StatusSuccess == ret)
        {
            return 0;
        }
    }

    return -1;
}

/**
 *  \fn EM_thread_cond_wait_timeout
 *
 *  \brief To wait on a Conditional Variable object
 *
 *  \par Description:
 *  This function implements the OS Wrapper for the Thread Wait on
 *  Conditional Variable call.
 *  It waits for the conditional variable to be signaled.
 *  This routine releases the  mutex and waits for the condition variable
 *  to  be signaled. The execution of the calling thread is suspended
 *  until the conditional variable is signaled.
 *  The mutex must be locked by the calling thread before
 *  calling this function. Before returning to the calling thread,
 *  it will re-acquire the mutex variable.
 *
 *  \param [in,out] cond
 *         The Conditional Variable
 *  \param [in,out] cond_mutex
 *         The Conditional Mutex Variable
 *  \param [in] timeout
 *         Timeout of the action
 *
 *  \return INT32 : 0 on Success. -1 on Failure.
 */
INT32 EM_thread_cond_wait_timeout
      (
          /* INOUT */ EM_thread_cond_type *     cond,
          /* INOUT */ EM_thread_mutex_type *    cond_mutex,
          /* IN */    UINT32 timeout
      )
{
    osa_status_t ret;

    if ((NULL != cond) &&
        (NULL != cond_mutex))
    {
        (void)EM_thread_mutex_unlock (cond_mutex);
        ret = OSA_SemaphoreWait((*cond), timeout);
        (void)EM_thread_mutex_lock (cond_mutex);

        if (KOSA_StatusSuccess == ret)
        {
            return 0;
        }
    }

    return -1;
}

/**
 *  \fn EM_thread_cond_signal
 *
 *  \brief To signal a Conditional Variable object
 *
 *  \par Description:
 *  This function implements the OS Wrapper for the Thread Signalling on
 *  Conditional Variable Call.
 *  It will signal and restart one of the threads that are waiting on
 *  the given conditional variable.
 *
 *  \param [in,out] cond
 *         The Conditional Variable to signal
 *
 *  \return INT32 : 0 on Success. -1 on Failure.
 */
INT32 EM_thread_cond_signal
      (
          /* INOUT */ EM_thread_cond_type *    cond
      )
{
    osa_status_t ret;

    if (NULL != cond)
    {
        ret = OSA_SemaphorePost((*cond));
        if (KOSA_StatusSuccess == ret)
        {
            return 0;
        }
    }

    return -1;
}


/**
 *  \fn EM_alloc_mem
 *
 *  \brief To allocate memory dynamically
 *
 *  \par Description:
 *  This function implements the OS Wrapper for the Memory Allocation call.
 *  It allocates memory of 'size' bytes and returns a pointer to
 *  the allocated memory.
 *
 *  \param [in] size
 *         The number of bytes to allocate
 *
 *  \return void * :  Pointer to the newly Allocated Memory segment.
 *                    NULL, in case of failure
 */
void * EM_alloc_mem ( /* IN */ UINT32 size )
{
    void * buf;

    buf = OSA_MemoryAllocate (size);

    return buf;
}


/**
 *  \fn EM_free_mem
 *
 *  \brief To free dynamically allocated memory
 *
 *  \par Description:
 *  This function implements the OS Wrapper for the Memory Free call.
 *  It frees the memory space specified by the given pointer.
 *
 *  \param [in] ptr
 *         Pointer to the Memory location to be freed
 *
 *  \return None
 */
void EM_free_mem ( /* IN */ void * ptr )
{
    OSA_MemoryFree (ptr);
    return;
}


/**
 *  \fn EM_sleep
 *
 *  \brief To delay execution for a specified number of seconds
 *
 *  \par Description:
 *  This function implements the OS Wrapper for a Thread or task to sleep
 *  for specified number of seconds.
 *
 *  \param [in] tm
 *         Number of seconds the calling task or thread wants to sleep
 *
 *  \return None
 */
void EM_sleep( /* IN */ UINT32 tm )
{
    OSA_TimeDelay(tm * 1000U);
}


/**
 *  \fn EM_usleep
 *
 *  \brief To delay execution for a specified number of microseconds
 *
 *  \par Description:
 *  This function implements the OS Wrapper for a Thread or task to sleep
 *  for specified number of micro-seconds.
 *
 *  \param [in] tm
 *         Number of micro-seconds the calling task or thread wants to sleep
 *
 *  \return None
 */
void EM_usleep( /* IN */ UINT32 tm )
{
    /* TODO: Check API if available for Microsecond */
    OSA_TimeDelay((tm + 999U)/1000U);
}


/**
 *  \fn EM_get_current_time
 *
 *  \brief To get the current system time
 *
 *  \par Description:
 *  This function implements the OS Wrapper to get current time
 *  from the system.
 *
 *  \param [out] curtime
 *         Current Time
 *
 *  \return None
 */
void EM_get_current_time (/* OUT */ EM_time_type * curtime)
{
    /**
     * Port this section to fetch the current system time.
     * By default this value is set to 0.
     */
    (* curtime) = 0U;

    return;
}


/**
 *  \fn EM_get_local_time
 *
 *  \brief To get the local time
 *
 *  \par Description:
 *  This function implements the OS Wrapper to get local time
 *  from the system.
 *
 *  \param [out] local
 *         Abstracted local time pointer to hold the system local time
 *
 *  \return None
 */
void EM_get_local_time
     (
         /* OUT */ EM_LOCAL_TIME * local
     )
{
    /* Get Local time. */

    local->dyear = 0U;
    local->dmonth = 0U;
    local->dday = 0U;
    local->dwday = 0U;
    local->thour = 0U;
    local->tmin = 0U;
    local->tsec = 0U;
    local->tmsec = 0U;

    return;
}

/**
 *  \fn EM_get_time_ms
 *
 *  \brief To get the system time in Millisec
 *
 *  \par Description:
 *  This function implements the OS Wrapper to get system time in millisec.
 *
 *  \return System time in Millisec
 */
INT32 EM_get_time_ms(void)
{
    return OSA_TimeGetMsec();
}

/* Microseconds from 01-Jan-0000 till epoch(in ISO 8601: 1970 - 01 - 01T00 : 00 : 00Z). */
#define EM_uS_TILL_EPOCH           0x00DCDDB30F2F8000ULL
/* Microseconds from 01-Jan-0000 till epoch(in ISO 8601: 2021 - 04 - 30T00 : 00 : 00Z). */
#define EM_uS_TIMESTAMP_PSEUDO     0x00E29EE5C36CE000ULL
/**
 *  \fn EM_get_us_timestamp
 *
 *  \brief To get the microsecond timestamp
 *
 *  \par Description:
 *  This function implements the OS Wrapper to get system time in microseconds
 *
 *  \return System time in microseconds
 */
UINT64 EM_get_us_timestamp(void)
{
    UINT64 timestamp;
    timestamp = OSA_TimeGetMsec() * (UINT64)1000UL;

#if 0
    /* FILETIME till epoch in 100 nano seconds */
#define EM_FILETIME_TILL_EPOCH     0x019DB1DED53E8000ULL

    timestamp -= EM_FILETIME_TILL_EPOCH;

    /* Divide by 10 as the unit is 100 nano second */
    timestamp = timestamp / 10;
    timestamp += EM_uS_TILL_EPOCH;
#endif

    timestamp = timestamp + EM_uS_TIMESTAMP_PSEUDO;

    return timestamp;
}

/**
 *  \fn EM_process_term_notify
 *
 *  \brief To register handler for process termination
 *
 *  \par Description:
 *  This function implements the OS Wrapper to get process termination
 *  signal from system
 *
 *  \param handler (IN)
 *         Handler to be called for notification
 *
 *  \return None
 */
void EM_process_term_notify (void(*handler)(void))
{
    if (NULL == handler)
    {
        return;
    }
}

/**
 *  \fn EM_register_sof_handler
 *
 *  \brief To register handler Task Stack Overflow notification
 *
 *  \par Description:
 *  This function registers the handler to be notified on stack overflow.
 *
 *  \param handler (IN)
 *         Handler to be called for notification
 *
 *  \return None
 */
void EM_register_sof_handler (void(*handler)(void* task_name))
{
    /* Save the stack overflow handler */
    em_sof_handler = handler;
}

/**
 *  \fn EM_thread_get_stack_unused
 *
 *  \brief To get the stack size unused in the current stack
 *
 *  \par Description:
 *  This function returns the number of unused stack bytes of the current task
 *  for the entire duration of existence
 *
 *  \param
 *
 *  \return Unused stack size in number of bytes
 */
UINT32 EM_thread_get_stack_unused (void)
{
#if (1 == INCLUDE_uxTaskGetStackHighWaterMark)
    UBaseType_t uxHighWaterMark;

    /**
     * Get the stack high watermark for the current task
     */
    uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);

    /**
     * The value returned is the high water mark in words
     * (for example, on a 32 bit machine a return value of
     * 1 would indicate that 4 bytes of stack were unused).
     *
     * Ref: https://www.freertos.org/uxTaskGetStackHighWaterMark.html
     */
    return (uxHighWaterMark * sizeof(int));
#else /* (1 == INCLUDE_uxTaskGetStackHighWaterMark) */

    return 0U;
#endif /* (1 == INCLUDE_uxTaskGetStackHighWaterMark) */
}

/* -------------------------------------------- Static OS Hooks */
void vApplicationStackOverflowHook
     (
         TaskHandle_t pxTask,
         char *pcTaskName
     )
{
    (void)pxTask;

    /* Disable interuupts to avoid any context switch */
    taskDISABLE_INTERRUPTS();

    /* Invoke handler if not NULL */
    if (NULL != em_sof_handler)
    {
        em_sof_handler(pcTaskName);
    }

    for(;;) {}
}

#endif /* EM_ENABLE_PAL_OS */
