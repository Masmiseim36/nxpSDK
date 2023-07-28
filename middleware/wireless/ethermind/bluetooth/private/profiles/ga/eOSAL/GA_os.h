
/**
 *  \file GA_os.h
 *
 *  This Header File contains the mapping to generic OSAL APIs.
 */

/*
 *  Copyright (C) 2015. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_GA_OS_
#define _H_GA_OS_

/* --------------------------------------------- Header File Inclusion */
#include "EM_os.h"

/* --------------------------------------------- Global Definitions */
#define GA_THREAD_RETURN_VAL_SUCCESS    EM_THREAD_RETURN_VAL_SUCCESS
#define GA_THREAD_RETURN_VAL_FAILURE    EM_THREAD_RETURN_VAL_FAILURE

/* --------------------------------------------- Structures/Data Types */
/* Datatype to represent Thread or Task Identifier */
#define GA_thread_type                  EM_thread_type

/* Datatype to represent Thread or Task Attribute */
#define GA_thread_attr_type             EM_thread_attr_type

/* Datatype to represent Mutex object */
#define GA_thread_mutex_type            EM_thread_mutex_type

/* Datatype to represent Attributes of a Mutex object */
#define GA_thread_mutex_attr_type       EM_thread_mutex_attr_type

/* Datatype to represent Conditional Variable object */
#define GA_thread_cond_type             EM_thread_cond_type

/* Datatype to represent Attributes of a Conditional Variable object */
#define GA_thread_cond_attr_type        EM_thread_cond_attr_type

/* Data type to represent the OS time for the platform */
#define GA_time_type                    EM_time_type

/* Data type for thread arguments and return */
#define GA_THREAD_RETURN_TYPE           EM_THREAD_RETURN_TYPE
#define GA_THREAD_ARGS                  EM_THREAD_ARGS
#define GA_THREAD_START_ROUTINE         EM_THREAD_START_ROUTINE

/* --------------------------------------------- Macros */
/* Abstractions for String library functions */
#define GA_str_len(s)                   EM_str_len(s)
#define GA_str_n_len(s, sz)             EM_str_n_len((s), (sz))
#define GA_str_copy(d, s)               EM_str_copy((d), (s))
#define GA_str_n_copy(d, s, n)          EM_str_n_copy((d), (s), (n))
#define GA_str_cmp(s1, s2)              EM_str_cmp((s1), (s2))
#define GA_str_n_cmp(s1, s2, n)         EM_str_n_cmp((s1), (s2), (n))
#define GA_str_n_casecmp(s1, s2, n)     EM_str_n_casecmp((s1), (s2), (n))
#define GA_str_cat(d, s)                EM_str_cat((d), (s))
#define GA_str_n_cat(d, s, sz)          EM_str_n_cat((d), (s), (sz))
#define GA_str_str(s, ss)               EM_str_str((s), (ss))
#define GA_str_chr(s, ch)               EM_str_chr((s), (ch))
#define GA_str_rchr(s, ch)              EM_str_rchr((s), (ch))
#define GA_str_print(...)               EM_str_print(__VA_ARGS__)

/* Abstractions for memory functions */
#define GA_mem_move(d, s, n)            EM_mem_move((d), (s), (n))
#define GA_mem_cmp(p1, p2, n)           EM_mem_cmp((p1), (p2), (n))
#define GA_mem_set(p, v, n)             EM_mem_set((p), (v), (n))
#define GA_mem_copy(d, s, n)            EM_mem_copy((d), (s), (n))

/* Task/Thread Creation Primitives */
#define GA_thread_create(t, at, r, a)   EM_thread_create((t), (at), (r), (a))
#define GA_thread_attr_init(a)          EM_thread_attr_init(a)
#define GA_thread_delete(t)             EM_thread_delete((t))

/* Task/Thread Synchronization Primitives */
#define GA_thread_mutex_init(m, a)      EM_thread_mutex_init((m), (a))
#define GA_thread_mutex_deinit(m)       EM_thread_mutex_deinit((m))
#define GA_thread_mutex_lock(m)         EM_thread_mutex_lock(m)
#define GA_thread_mutex_unlock(m)       EM_thread_mutex_unlock(m)
#define GA_thread_cond_init(c, a)       EM_thread_cond_init((c), (a))
#define GA_thread_cond_deinit(c)        EM_thread_cond_deinit((c))
#define GA_thread_cond_wait(c, m)       EM_thread_cond_wait((c), (m))
#define GA_thread_cond_signal(c)        EM_thread_cond_signal(c)

/* Memory Management Primitives */
#define GA_alloc_mem(sz)                EM_alloc_mem(sz)
#define GA_free_mem(p)                  EM_free_mem(p)

/* Task/Thread Delay Primitives */
#define GA_sleep(tm)                    EM_sleep(tm)
#define GA_usleep(tm)                   EM_usleep(tm)
#define GA_get_current_time(ct)         EM_get_current_time(ct)
#define GA_get_local_time(local)        EM_get_local_time(local)
#define GA_get_time_ms()                EM_get_time_ms()
#define GA_get_us_timestamp             EM_get_us_timestamp

/* -------------------------------------------- Data Structures */
#define GA_LOCAL_TIME                   EM_LOCAL_TIME

/* --------------------------------------------- Internal Functions */

/* --------------------------------------------- API Declarations */


#endif /* _H_GA_OS_ */

