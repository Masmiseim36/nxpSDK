
/**
 *  \file ethal_os.h
 *
 *  This header file is part of EtherMind OS Abstraction module.
 *  In this file the platform specific data types are abstracted and
 *  mapped to data types used within the EtherMind Stack.
 *
 *  Version: FreeRTOS
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_ETHAL_OS_
#define _H_ETHAL_OS_

/* --------------------------------------------------- Header File Inclusion */
#include "EM_platform.h"

/* -------------------------------------------- Global Definitions */
#define EM_SUCCESS                  0x0000U
#define EM_FAILURE                  0xFFFFU

#define EM_THREAD_RETURN_VAL_SUCCESS
#define EM_THREAD_RETURN_VAL_FAILURE

/* Declaration of 'static' variable/function */
#ifdef EM_HAVE_STATIC_DECL
#define DECL_STATIC                     static
#else  /* EM_HAVE_STATIC_DECL */
#define DECL_STATIC
#endif /* EM_HAVE_STATIC_DECL */

/* Declaration of 'const' variable */
#ifdef EM_HAVE_CONST_DECL
#define DECL_CONST                      const
#else  /* EM_HAVE_CONST_DECL */
#define DECL_CONST
#endif /* EM_HAVE_CONST_DECL */

/* Reentrant Function Declaration */
#ifdef EM_HAVE_REENTRANT_DECL
#define DECL_REENTRANT                  reentrant
#else /* EM_HAVE_REENTRANT_DECL */
#define DECL_REENTRANT
#endif /* EM_HAVE_REENTRANT_DECL */

#define DECL_CONST_QUALIFIER

/**
 * Default EM Task's preferred Stack Depth and Priority
 */
#define EM_OS_TASK_STACKDEPTH           (4U * 1024U)
#define EM_OS_TASK_PRIORITY             (6U)

/**
 * Macro for Default Task Name.
 * The default task name is mapped to the Stringized form of the Task routine
 * provided by the Upper Layer through EM_thread_create(...) Interface.
 */
#define EM_OS_DEFAULT_TASK_NAME(tname)  #tname

/* OS Specific Handle Sizes for Mutex, Semaphores and Tasks */
#define EM_THREAD_HANDLE_SIZE           (sizeof(uint32_t) * ((OSA_TASK_HANDLE_SIZE + sizeof(uint32_t) - 1U) / sizeof(uint32_t)))
#define EM_THREAD_MUTEX_SIZE            (sizeof(uint32_t) * ((OSA_MUTEX_HANDLE_SIZE + sizeof(uint32_t) - 1U) / sizeof(uint32_t)))
#define EM_THREAD_COND_SIZE             (sizeof(uint32_t) * ((OSA_SEM_HANDLE_SIZE + sizeof(uint32_t) - 1U) / sizeof(uint32_t)))

/* -------------------------------------------- Structures/Data Types */
/* 'signed' datatype of size '1 octet' */
typedef char CHAR;
/* 'signed' datatype of size '1 octet' */
typedef int8_t INT8;
/* 'unsigned' datatype of size '1 octet' */
typedef uint8_t UCHAR;
/* 'unsigned' datatype of size '1 octet' */
typedef uint8_t UINT8;
/* 'signed' datatype of size '2 octet' */
typedef int16_t INT16;
/* 'unsigned' datatype of size '2 octet' */
typedef uint16_t UINT16;
/* 'signed' datatype of size '4 octet' */
typedef int32_t INT32;
/* 'unsigned' datatype of size '4 octet' */
typedef uint32_t UINT32;
/* 'signed' datatype of size '8 octet' */
typedef int64_t INT64;
/* 'unsigned' datatype of size '8 octet' */
typedef uint64_t UINT64;

/* 'unsigned' datatype of size '1 octet' */
typedef bool BOOLEAN;

/*
 * Generic handle can used for the unknow
 * datatypes (e.g. Mutex Type, Conditional Type etc.)
 */
typedef void * EM_GENERIC_HANDLE;

/* Datatype to represent Thread or Task Identifier */
typedef osa_task_handle_t EM_thread_type;
/* Datatype to represent Thread or Task Attribute */
typedef struct EM_thread_attr_type_struct
{
    DECL_CONST CHAR  * thread_name;
    UINT32             thread_stack_size;
    UINT32             thread_priority;
} EM_thread_attr_type;

/* Datatype to represent Mutex object */
typedef osa_mutex_handle_t EM_thread_mutex_type;
/* Datatype to represent Attributes of a Mutex object */
typedef /* pthread_mutexattr_t */ void EM_thread_mutex_attr_type;
/* Datatype to represent Conditional Variable object */
typedef osa_semaphore_handle_t EM_thread_cond_type;
/* Datatype to represent Attributes of a Conditional Variable object */
typedef /* pthread_condattr_t */ void EM_thread_cond_attr_type;

/* Datatype to represent File Handle */
typedef EM_GENERIC_HANDLE EM_FILE_HANDLE;

/* Data type to represent the OS time for the platform */
typedef int              EM_time_type;

/* Data types for task parameters and retun types */
typedef void EM_THREAD_RETURN_TYPE;
typedef osa_task_param_t EM_THREAD_ARGS;
typedef osa_task_ptr_t EM_THREAD_START_ROUTINE;

/* Function Return Value typwe */
typedef UINT16 EM_RESULT;

/* --------------------------------------------------- Macros */

/* Abstractions for String library functions */
#define EM_str_len(s)                 strlen((char *)(s))
#define EM_str_n_len(s, sz)           strnlen((char *)(s), (sz))
#define EM_str_copy(d, s)             (void)strcpy((char *)(d), (char *)(s))
#define EM_str_n_copy(d, s, n)        (void)strncpy((char *)(d), (char *)(s), n)
#define EM_str_cmp(s1, s2)            strcmp((char *)(s1), (char *)(s2))
#define EM_str_n_cmp(s1, s2, n)       strncmp((char *)(s1), (char *)(s2), n)
#define EM_str_cat(d, s)              (void)strcat((char *)(d), (char *)(s))
#define EM_str_n_cat(d, s, sz)        (void)strncat((char *)(d), (char *)(s), (sz))
#define EM_str_str(s, ss)             strstr((char *)(s), (char *)(ss))
#define EM_str_chr(s, ch)             strchr((char *)(s), (ch))
#define EM_str_rchr(s, ch)            strrchr((char *)(s), (ch))
#define EM_str_n_casecmp(s1, s2, n)   _strnicmp ((char *)(s1), (char *)(s2), n)
#define EM_str_print(...)             (void)sprintf(__VA_ARGS__)

/* Abstractions for memory functions */
#define EM_mem_move(d, s, n)          (void)memmove((d), (s), (n))
#define EM_mem_cmp(p1, p2, n)         memcmp((p1), (p2), (n))
#define EM_mem_set(p, v, n)           (void)memset((p), (v), (n))
#define EM_mem_copy(p1, p2, n)        (void)memcpy((p1), (p2), (n))

/* -------------------------------------------- Data Structures */
/** Structure to have the local time */
typedef struct _EM_LOCAL_TIME
{
    UINT16  dyear;
    UINT16  dmonth;
    UINT16  dday;
    UINT16  dwday;

    UINT16  thour;
    UINT16  tmin;
    UINT16  tsec;
    UINT16  tmsec;

} EM_LOCAL_TIME;

/* -------------------------------------------- Function Declarations */
#ifdef __cplusplus
extern "C" {
#endif

/** Initialize the OS */
void EM_os_init(void);

/* Task/Thread Creation Primitives */
INT32 EM_thread_create
      (
          /* OUT */ EM_thread_type *         thread,
          /* IN */  EM_thread_attr_type *    attr,
          /* IN */  EM_THREAD_START_ROUTINE  start_routine,
          /* IN */  EM_THREAD_ARGS           thread_args
      );

INT32 EM_thread_delete
      (
          /* IN */ EM_thread_type thread
      );

INT32 EM_thread_attr_init
      (
          /* OUT */ EM_thread_attr_type *    attr
      );

/* Task/Thread Synchronization Primitives */
INT32 EM_thread_mutex_init
      (
          /* OUT */ EM_thread_mutex_type *         mutex,
          /* IN */  EM_thread_mutex_attr_type *    mutex_attr
      );

INT32 EM_thread_mutex_deinit
      (
          /* IN */ EM_thread_mutex_type *         mutex
      );

INT32 EM_thread_mutex_lock
      (
          /* INOUT */ EM_thread_mutex_type *    mutex
      );

INT32 EM_thread_mutex_unlock
      (
          /* INOUT */ EM_thread_mutex_type *    mutex
      );

INT32 EM_thread_cond_init
      (
          /* OUT */ EM_thread_cond_type *         cond,
          /* IN */  EM_thread_cond_attr_type *    cond_attr
      );

INT32 EM_thread_cond_deinit
      (
          /* IN */ EM_thread_cond_type *         cond
      );

INT32 EM_thread_cond_wait
      (
          /* INOUT */ EM_thread_cond_type *     cond,
          /* INOUT */ EM_thread_mutex_type *    cond_mutex
      );

INT32 EM_thread_cond_wait_timeout
      (
          /* INOUT */ EM_thread_cond_type *     cond,
          /* INOUT */ EM_thread_mutex_type *    cond_mutex,
          /* IN */    UINT32 timeout
      );

INT32 EM_thread_cond_signal
      (
          /* INOUT */ EM_thread_cond_type *    cond
      );

/* Memory Management Primitives */
void * EM_alloc_mem (/* IN */ UINT32 size);
void EM_free_mem (/* IN */ void * ptr);

/* Task/Thread Delay Primitives */
void EM_sleep ( /* IN */ UINT32 tm );
void EM_usleep ( /* IN */ UINT32 tm );
void EM_get_current_time (/* OUT */ EM_time_type * curtime);
void EM_get_local_time( /* OUT */ EM_LOCAL_TIME *local);
INT32 EM_get_time_ms(void);
UINT64 EM_get_us_timestamp(void);

/* Process termination handling */
void EM_process_term_notify(void(*handler)(void));

void EM_register_sof_handler (void(*handler)(void * task_name));
UINT32 EM_thread_get_stack_unused (void);

#ifdef __cplusplus
};
#endif

#endif /* _H_ETHAL_OS_ */


