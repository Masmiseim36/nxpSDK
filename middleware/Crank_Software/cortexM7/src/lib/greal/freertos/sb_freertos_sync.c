/*
 * Copyright 2010, Crank Software Inc. All Rights Reserved.
 *
 * For more information email info@cranksoftware.com.
 */

#include <gre/sdk/greal.h>

#include <pthread.h>

#include <FreeRTOS.h>
#include "semphr.h"
#include <task.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct _private_mutex {
	xSemaphoreHandle	handle;
};

struct _private_mutex_attr {
	int 	zero;
};

struct _private_cond {
	pthread_mutex_t		mutex;
	int 				wait_count;
	xSemaphoreHandle	wait_sem;
};

struct _private_cond_attr {
	int 	zero;
};

int
pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr) {
	struct _private_mutex *pm;

#if !defined(configUSE_MUTEXES)
#warning This port requires mutex enablement
#endif

	pm = greal_calloc(1, sizeof(*pm));
	if (pm == NULL) {
	 	return 1; 
	}
	pm->handle = xSemaphoreCreateMutex();
	if(pm->handle == NULL) {
		greal_free(pm);
		return 1;
	}

	*mutex = pm;

	return 0;
}

int
pthread_mutex_destroy(pthread_mutex_t *mutex) {
	/**
	 * Not implemented.
	 **/
	return 0;
}

int
pthread_mutex_lock(pthread_mutex_t *mutex) {
	struct _private_mutex *pm;
	portBASE_TYPE 		   ret;

	pm = *mutex;

	ret = xSemaphoreTake(pm->handle, portMAX_DELAY);

	return (ret == pdPASS) ? 0 : 1;
}

int
pthread_mutex_unlock(pthread_mutex_t *mutex) {
	struct _private_mutex *pm;
	portBASE_TYPE 		   ret;

	pm = *mutex;

	ret = xSemaphoreGive(pm->handle);

	return (ret == pdPASS) ? 0 : 1;
}

int
pthread_cond_init(pthread_cond_t *cond, const pthread_condattr_t *attr) {
	struct _private_cond *pc;
	int					 ret;

	pc = greal_calloc(1, sizeof(*pc));
	if(pc == NULL) {
		return 1;
	}

	ret = pthread_mutex_init(&pc->mutex, NULL);
	if(ret != 0) {
		greal_free(pc);
		return 1;
	}

	pc->wait_sem = xSemaphoreCreateCounting(50, 0);
	if(pc->wait_sem == NULL) {
		pthread_mutex_destroy(&pc->mutex);
		greal_free(pc);
		return 1;
	}

	*cond = pc;

	return 0;
}

int
pthread_cond_destroy(pthread_cond_t *cond) {
	/**
	 * Not implemented.
	 **/
	return 0;
}

int
pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex) {      
    return pthread_cond_timedwait(cond, mutex, NULL); 
}

int
pthread_cond_timedwait(pthread_cond_t *cond, pthread_mutex_t * mutex, const struct timespec * abstime) {  
	struct _private_cond *pc;
	portBASE_TYPE		 ret;
    int absticks;
    uint32_t timeout_ticks;
    TickType_t  now_ticks;
    int num_ms; 

	pc = *cond;

    if(abstime) {
        num_ms = (abstime->tv_sec * 1000) + (abstime->tv_nsec / 1000000);
		absticks = num_ms / portTICK_RATE_MS;
     
        now_ticks = xTaskGetTickCount();
        if(absticks > now_ticks) {
            timeout_ticks = absticks - now_ticks;
        } else {
            timeout_ticks = 1;	
        }
    } else {
        timeout_ticks =  portMAX_DELAY-1;
    }
    
	pthread_mutex_lock(&pc->mutex);
	pc->wait_count++;
	pthread_mutex_unlock(&pc->mutex);

	pthread_mutex_unlock(mutex);
    ret = xSemaphoreTake(pc->wait_sem, timeout_ticks);
	pthread_mutex_lock(mutex);

	pthread_mutex_lock(&pc->mutex);
	pc->wait_count--;
	pthread_mutex_unlock(&pc->mutex);

    if (ret == pdPASS)
       return 0; 
    if (abstime)
      return ETIMEDOUT; 
    else 
       return 1; 

}

int
pthread_cond_signal(pthread_cond_t *cond) {
	struct _private_cond *pc;
	int					 count;
	pc = *cond;

	pthread_mutex_lock(&pc->mutex);
	count = pc->wait_count;
	pthread_mutex_unlock(&pc->mutex);
    
	if(count > 0) {
		xSemaphoreGive(pc->wait_sem);
	}

	return 0;
}

int
pthread_cond_broadcast(pthread_cond_t *cond) {
	struct _private_cond *pc;
	int					 count;
	pc = *cond;

	pthread_mutex_lock(&pc->mutex);
	count = pc->wait_count;
	pthread_mutex_unlock(&pc->mutex);

	while(count > 0) {
		xSemaphoreGive(pc->wait_sem);
		count--;
	}

	return 0;
}


