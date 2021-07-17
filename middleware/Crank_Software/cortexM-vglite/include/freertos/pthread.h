/*
 * Copyright 2012, Crank Software Inc. All Rights Reserved.
 * 
 * For more information email info@cranksoftware.com.
 */

#ifndef GR_PTHREAD_H
#define GR_PTHREAD_H
#include <time.h> 

#if !(defined(GRE_TARGET_TOOLCHAIN_cube) || defined(GRE_TARGET_TOOLCHAIN_dassgcc) || defined(GRE_TARGET_TOOLCHAIN_mcuxpresso) || defined(GRE_TARGET_TOOLCHAIN_sw4stm32))

struct _private_mutex;
struct _private_mutex_attr;
typedef struct _private_mutex * pthread_mutex_t;
typedef struct _private_mutex_attr * pthread_mutexattr_t;

struct _private_cond;
struct _private_cond_attr;
typedef struct _private_cond * pthread_cond_t;
typedef struct _private_cond_attr * pthread_condattr_t;

struct sched_param {
	int sched_priority;
};

#if !defined(GRE_TARGET_TOOLCHAIN_kpit_gcc) && !defined(GRE_TARGET_CPU_cortexm7) && !defined(GRE_TARGET_CPU_cortexm4) && !defined(GRE_TARGET_CPU_cortexm33) && !defined(GRE_TARGET_CPU_cortexa9)
struct timespec {
	unsigned int tv_sec;
	unsigned int tv_nsec;
};
#endif

int pthread_mutex_init(pthread_mutex_t *_mutex, const pthread_mutexattr_t *_attr);
int pthread_mutex_destroy(pthread_mutex_t *_mutex);

int pthread_mutex_lock(pthread_mutex_t *_mutex);
int pthread_mutex_unlock(pthread_mutex_t *_mutex);

int pthread_cond_init(pthread_cond_t *_cond, const pthread_condattr_t *_attr);
int pthread_cond_destroy(pthread_cond_t *_cond);

int pthread_cond_timedwait(pthread_cond_t *_cond, pthread_mutex_t * _mutex, const struct timespec * _abstime);
int pthread_cond_wait(pthread_cond_t *_cond, pthread_mutex_t *_mutex);

int pthread_cond_broadcast(pthread_cond_t *_cond);
int pthread_cond_signal(pthread_cond_t *_cond);

#ifndef PTHREAD_CREATE_JOINABLE
#define PTHREAD_CREATE_JOINABLE	0x00
#endif

struct _private_thread;
struct _private_thread_attr;
typedef struct _private_thread * pthread_t;
typedef struct _private_thread_attr * pthread_attr_t;

pthread_t pthread_self();
int pthread_equal(pthread_t _t1, pthread_t _t2);
int pthread_create(pthread_t *_thread, const pthread_attr_t *_attr, void *(*_start_routine)(void*), void *_arg);
int pthread_detach(pthread_t thread);
int pthread_join(pthread_t _thread, void **_value_ptr);

#endif

#endif /* GR_PTHREAD_H */