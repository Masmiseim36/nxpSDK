
/**
 *  \file eBT_os.h
 *
 *  This header file is part of EtherMind OS Abstraction module.
 *  In this file the platform specific data types are abstracted and
 *  mapped to data types used within the EtherMind Stack.
 */

/*
 *  Copyright (C) 2015. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_EBT_OS_
#define _H_EBT_OS_

/* --------------------------------------------- Header File Inclusion */
#include "EM_os.h"

/* --------------------------------------------- Global Definitions */
#define BT_THREAD_RETURN_VAL_SUCCESS    EM_THREAD_RETURN_VAL_SUCCESS
#define BT_THREAD_RETURN_VAL_FAILURE    EM_THREAD_RETURN_VAL_FAILURE

/**
 * Default Bluetooth Task's preferred Stack Depth and Priority
 */
#define BT_TASK_STACK_DEPTH             EM_OS_TASK_STACKDEPTH
#define BT_TASK_PRIORITY                EM_OS_TASK_PRIORITY

/* --------------------------------------------- Structures/Data Types */
/* Datatype to represent Thread or Task Identifier */
#define BT_thread_type                  EM_thread_type

/* Datatype to represent Thread or Task Attribute */
#define BT_thread_attr_type             EM_thread_attr_type

/* Datatype to represent Mutex object */
#define BT_thread_mutex_type            EM_thread_mutex_type

/* Datatype to represent Attributes of a Mutex object */
#define BT_thread_mutex_attr_type       EM_thread_mutex_attr_type

/* Datatype to represent Conditional Variable object */
#define BT_thread_cond_type             EM_thread_cond_type

/* Datatype to represent Attributes of a Conditional Variable object */
#define BT_thread_cond_attr_type        EM_thread_cond_attr_type

/* Datatype to represent File Handle */
#define BT_FILE_HANDLE                  EM_GENERIC_HANDLE

/* Data type to represent the OS time for the platform */
#define BT_time_type                    EM_time_type

/* Data type for thread arguments and return */
#define BT_THREAD_RETURN_TYPE           EM_THREAD_RETURN_TYPE
#define BT_THREAD_ARGS                  EM_THREAD_ARGS
#define BT_THREAD_START_ROUTINE         EM_THREAD_START_ROUTINE

/* --------------------------------------------- Macros */
/* Abstractions for String library functions */
#define BT_str_len(s)                   EM_str_len(s)
#define BT_str_n_len(s, sz)             EM_str_n_len((s), (sz))
#define BT_str_copy(d, s)               EM_str_copy((d), (s))
#define BT_str_n_copy(d, s, n)          EM_str_n_copy((d), (s), (n))
#define BT_str_cmp(s1, s2)              EM_str_cmp((s1), (s2))
#define BT_str_n_cmp(s1, s2, n)         EM_str_n_cmp((s1), (s2), (n))
#define BT_str_n_casecmp(s1, s2, n)     EM_str_n_casecmp((s1), (s2), (n))
#define BT_str_cat(d, s)                EM_str_cat((d), (s))
#define BT_str_n_cat(d, s, sz)          EM_str_n_cat((d), (s), (sz))
#define BT_str_str(s, ss)               EM_str_str((s), (ss))
#define BT_str_chr(s, ch)               EM_str_chr((s), (ch))
#define BT_str_rchr(s, ch)              EM_str_rchr((s), (ch))
#define BT_str_print(...)               EM_str_print(__VA_ARGS__)

/* Abstractions for memory functions */
#define BT_mem_move(d, s, n)            EM_mem_move((d), (s), (n))
#define BT_mem_cmp(p1, p2, n)           EM_mem_cmp((p1), (p2), (n))
#define BT_mem_set(p, v, n)             EM_mem_set((p), (v), (n))
#define BT_mem_copy(d, s, n)            EM_mem_copy((d), (s), (n))

/* Task/Thread Creation Primitives */
#define BT_thread_create(t, at, r, a)   EM_thread_create((t), (at), (r), (a))
#define BT_thread_attr_init(a)          EM_thread_attr_init(a)
#define BT_thread_delete(t)             EM_thread_delete((t))

/* Task/Thread Synchronization Primitives */
#define BT_thread_mutex_init(m, a)      EM_thread_mutex_init((m), (a))
#define BT_thread_mutex_deinit(m)       EM_thread_mutex_deinit((m))
#define BT_thread_mutex_lock(m)         EM_thread_mutex_lock(m)
#define BT_thread_mutex_unlock(m)       EM_thread_mutex_unlock(m)
#define BT_thread_cond_init(c, a)       EM_thread_cond_init((c), (a))
#define BT_thread_cond_deinit(c)        EM_thread_cond_deinit((c))
#define BT_thread_cond_wait(c, m)       EM_thread_cond_wait((c), (m))
#define BT_thread_cond_wait_timeout(c, m, t) \
                                        EM_thread_cond_wait_timeout((c), (m), (t))
#define BT_thread_cond_signal(c)        EM_thread_cond_signal(c)

/* Memory Management Primitives */
#define BT_alloc_mem(sz)                EM_alloc_mem(sz)
#define BT_free_mem(p)                  EM_free_mem(p)

/* Task/Thread Delay Primitives */
#define BT_sleep(tm)                    EM_sleep(tm)
#define BT_usleep(tm)                   EM_usleep(tm)
#define BT_get_current_time(ct)         EM_get_current_time(ct)
#define BT_get_local_time(local)        EM_get_local_time(local)
#define BT_get_time_ms()                EM_get_time_ms()
#define BT_get_us_timestamp             EM_get_us_timestamp

/* -------------------------------------------- Data Structures */
#define BT_LOCAL_TIME                   EM_LOCAL_TIME

/* --------------------------------------------- Internal Functions */

/* --------------------------------------------- API Declarations */


#endif /* _H_EBT_OS_ */

