/*
 * Copyright 2010, Crank Software Inc. All Rights Reserved.
 *
 * For more information email info@cranksoftware.com.
 */

#include <gre/sdk/greal.h>

#include <pthread.h>

#include <FreeRTOS.h>
#include "task.h"
#include "semphr.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static xSemaphoreHandle pthread_semTask;
static int sem_created = 0;

struct _private_thread {
	struct _private_thread *	next;
	void *		(*cb_func)(void *arg);
	void *		 cb_data;
	xTaskHandle  task_handle;
};

struct _private_thread_attr {
	int 	zero;
};

/**
 * Global thread list, integrity maintained by ENTER/EXIT_CRITICAL() calls
 */
static struct _private_thread *g_thread_list = NULL;

/**
 * Since the main thread is not actually created through this API, if we run
 * into a situation where this thread is not in the list, then we 'leak' a
 * thread entry and create a thread for ourselves.
 */
pthread_t
pthread_self(void) {
	struct _private_thread *pt;
	xTaskHandle task_handle;

	if (!sem_created) {
		sem_created = 1;
		pthread_semTask = xSemaphoreCreateMutex();
	}
    xSemaphoreTake(pthread_semTask, portMAX_DELAY);

	task_handle = xTaskGetCurrentTaskHandle();

	for(pt = g_thread_list; pt != NULL; pt = pt->next) {
		if(pt->task_handle == task_handle) {
			break;
		}
	}

	if(pt == NULL) {
		pt = greal_calloc(1, sizeof(*pt));
		if(pt != NULL) {
			pt->task_handle = xTaskGetCurrentTaskHandle();

			taskENTER_CRITICAL();
			pt->next = g_thread_list;
			g_thread_list = pt;
			taskEXIT_CRITICAL();
		}
	}
	xSemaphoreGive(pthread_semTask);

	return pt;
}

int
pthread_equal(pthread_t t1, pthread_t t2) {
	struct _private_thread *pt1 = t1;
	struct _private_thread *pt2 = t2;

	//Invalid thread id's are undefined, define them as unequal
	if(!pt1 || !pt2) {
		return 0;
	}

	return (pt1->task_handle == pt2->task_handle) ? 1 : 0;
}

/**
 * We need to use a wrapper task around the function because the FreeRTOS tasks
 * can never return to their caller, so we need to allow them to spin indefinitely.
 */
static void
wrapper_task(void *arg) {
	struct _private_thread *pt = (struct _private_thread *)arg;

	pt->cb_func(pt->cb_data);

	/**
	 * Should the routine fail, we end up here
	 */
#if ( INCLUDE_vTaskDelete == 1 )
    vTaskDelete(NULL);
#endif
    while(1) { ; }
}

int
pthread_create_named(pthread_t *thread, const pthread_attr_t *_attr, void *(*start_routine)(void*), void *arg, const char *name) {
	struct _private_thread *pt;
	int 	 				priority;

	if (!sem_created) {
		sem_created = 1;
		pthread_semTask = xSemaphoreCreateMutex();
	}
    xSemaphoreTake(pthread_semTask, portMAX_DELAY);

	pt = greal_calloc(1, sizeof(*pt));
	if (pt == NULL) {
	  	xSemaphoreGive(pthread_semTask);
	 	return 1; 
	}
	pt->cb_func = start_routine;
	pt->cb_data = arg;

	pt->next = g_thread_list;
	g_thread_list = pt;

	priority = configMAX_PRIORITIES / 2;

	xTaskCreate(wrapper_task, name, 2048, pt, priority, &pt->task_handle);

	*thread = pt;

	xSemaphoreGive(pthread_semTask);

	return 0;
}

int
pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void*), void *arg) {
	int result;
	char name[16];
	static int tid_index = 0;

    sprintf(name, "tid-%d", tid_index++);
	result = pthread_create_named(thread, attr, start_routine, arg, name);
	if (0 != result) {
		--tid_index;
	}
	return result;
}

int
greal_pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void *), void *arg, const char *name) {
	if (NULL == name) {
		return pthread_create(thread, attr, start_routine, arg);
	}

	return pthread_create_named(thread, attr, start_routine, arg, name);
}

int
pthread_detach(pthread_t thread) {
	/**
	 * Not implemented.
	 */
	return 0;
}

int
pthread_join(pthread_t thread, void **_value_ptr) {
	/**
	 * yield with a non blocking delay for thread to delete itself
	 */

	while (eTaskGetState( ((struct _private_thread*)thread)->task_handle) != eDeleted ) {
		vTaskDelay( 10 );
	}
	return 0;
}

int
pthread_kill(pthread_t tid, int sig) {
	/**
	 * Not implemented.
	 */
	return 0;
}


void
taskYield(void) {
	 const portTickType xDelay = 100 / portTICK_RATE_MS;
	 vTaskDelay( xDelay );
}
