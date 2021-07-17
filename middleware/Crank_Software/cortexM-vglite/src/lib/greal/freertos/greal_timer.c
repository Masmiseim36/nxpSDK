/*
 * Copyright 2016, Crank Software Inc. All Rights Reserved.
 * 
 * For more information email info@cranksoftware.com.
 */
/** \File: greal_timer.c
 *
 */

#include <gre/sdk/greal.h>
#include <errno.h>

//RJD:  This is only needed until the pthread library has been recompiled against the 2013 or higher version of 
// of Visual Studio.  When this is removed, fix the condition on line 94 as well. 
#if GR_OS(win32)
#define PTW32_ETIMEOUT 10060
#endif

uint32_t
greal_timespec_to_ms(greal_timespec_t*ts) {
	return (ts->tv_sec * 1000) + (ts->tv_nsec / 1000000);
}

/**
 * Compare two time values to one another
 *
 * @return <0 if ts1 < ts2, >0 if ts1 > ts2, 0 if equal
 */
int
greal_timespec_cmp(greal_timespec_t *ts1, greal_timespec_t *ts2) {
	int32_t diff;

	diff = ts1->tv_sec - ts2->tv_sec;
	if(diff == 0) {
		diff = ts1->tv_nsec - ts2->tv_nsec;
	}

	return diff;
}

/**
 * Add a ms value to a current timestamp.
 */
void
greal_timespec_add_ms(greal_timespec_t *ts, uint32_t ms) {
    uint32_t sec;

    sec = ms/1000;
    ms = ms - sec*1000;

    ts->tv_nsec += ms * 1000000;
    ts->tv_sec += ts->tv_nsec/1000000000 + sec;
    ts->tv_nsec = ts->tv_nsec % 1000000000;
}



#ifndef _GREAL_PLATFORM_TIMER

#define GREAL_TIMER_THREAD_STARTED	0x1	///< Flag indicating timer mgr thread started
#define GREAL_TIMER_THREAD_INITIALIZED		0x2	///< Flag indicating we passed through init
#define GREAL_TIMER_THREAD_TERMINATED		0x4	///< Flag indicating that we're quitting

struct _greal_timer_struct {
    pthread_t 			tid;						///< Thread handle for timer_callback_thread
	pthread_mutex_t	 	mutex;						///< Mutex for timer queue
	pthread_cond_t	 	condvar;					///< Condvar for timer callback thread
	void 				(*callback)(void *data);	///< Callback to fire when the timer fires 
	void 				*data;						///< Data to be passed in to the callback 
	greal_timespec_t   	next_fire_time; 			///< Time that should pass before firing the next timer 
	uint8_t				has_fire_time;				///< 0 if there is no fire time set, 1 if the fire time is set
	uint8_t				flags; 						///< The flags set for the timer 	
};

static void *
timer_callback_thread(void *arg) {
    greal_timespec_t start_time;
	int ret;
	greal_timer_struct_t *handle = (greal_timer_struct_t *) arg;

    greal_clock_gettime(GREAL_CLOCK_ID, &start_time);
    //Loop until the timer is destroyed/terminated
    while(1) {
		pthread_mutex_lock(&handle->mutex);

        //Wait until we are told to go, or told to quit
        while((handle->has_fire_time == 0) && !(handle->flags & GREAL_TIMER_THREAD_TERMINATED)) {
            pthread_cond_wait(&handle->condvar, &handle->mutex);
        }

        if (handle->flags & GREAL_TIMER_THREAD_TERMINATED) {
            pthread_mutex_unlock(&handle->mutex);
            break;
        } 

		//Either we are running behind or we need to wait for a timer or a reconfig to occur
    	greal_clock_gettime(GREAL_CLOCK_ID, &start_time);
		if(greal_timespec_cmp(&handle->next_fire_time, &start_time) > 0) {
			struct timespec ts;
			ts.tv_sec = handle->next_fire_time.tv_sec;
			ts.tv_nsec = handle->next_fire_time.tv_nsec;
			ret = pthread_cond_timedwait(&handle->condvar, &handle->mutex, &ts);
#if GR_OS(win32)
			if(ret != ETIMEDOUT && ret != PTW32_ETIMEOUT) { 
#else
			if(ret != ETIMEDOUT) { 
#endif
				pthread_mutex_unlock(&handle->mutex);
				continue;		//Something changed about our queue, revalidate
        	}
		}

		pthread_mutex_unlock(&handle->mutex);		
        if (handle->flags & GREAL_TIMER_THREAD_TERMINATED) {
			break;
		}

		handle->callback(handle->data);
    }
    return NULL;
}

greal_timer_struct_t *
greal_timer_create(void (*callback)(void *data), void *data) {
	greal_timer_struct_t *handle = NULL;
	int ret; 
#if GR_OS(linux) || GR_OS(qnx) || GR_OS(vxworks)
	pthread_condattr_t attr;
#endif

	handle = greal_calloc(1, sizeof(greal_timer_struct_t));
	if(handle == NULL) {
		return NULL; 
	}

	handle->callback = callback; 
	handle->data = data; 

	ret = pthread_mutex_init(&handle->mutex, NULL);
	if(ret != 0) {
		greal_free(handle);
		return NULL;
	}

#if GR_OS(linux) || GR_OS(qnx) || GR_OS(vxworks)
	pthread_condattr_init(&attr);
	pthread_condattr_setclock(&attr, GREAL_CLOCK_ID);
	ret = pthread_cond_init(&handle->condvar, &attr);
#else
	ret = pthread_cond_init(&handle->condvar, NULL);
#endif

	if(ret != 0) {
		greal_free(handle);
		return NULL;
	}
	
	ret = greal_pthread_create(&handle->tid, NULL, timer_callback_thread, handle, "greal timer thread");
	if(ret != 0) {
		greal_free(handle);
		return NULL;
	}
	handle->flags |= GREAL_TIMER_THREAD_STARTED;
	return handle; 
} 

void 
greal_timer_set_timeout(greal_timer_struct_t *handle, greal_timespec_t fire_time) {
	if(!handle) {
		return; 
	}
	pthread_mutex_lock(&handle->mutex);
	handle->next_fire_time.tv_nsec = fire_time.tv_nsec;
	handle->next_fire_time.tv_sec = fire_time.tv_sec; 
	handle->has_fire_time = 1;
 	pthread_mutex_unlock(&handle->mutex);
	pthread_cond_signal(&handle->condvar); 
} 

void 
greal_timer_cancel(greal_timer_struct_t *handle) {
	if(!handle) {
		return; 
	}

	pthread_mutex_lock(&handle->mutex);
	handle->next_fire_time.tv_nsec = 0; 
	handle->next_fire_time.tv_sec = 0;
	handle->has_fire_time = 0;
 	pthread_mutex_unlock(&handle->mutex);
	pthread_cond_signal(&handle->condvar);
}

void 
greal_timer_destroy(greal_timer_struct_t *handle) {
	if(!handle) {
		return; 
	}

	if (handle->flags & GREAL_TIMER_THREAD_STARTED) {
		pthread_mutex_lock(&handle->mutex);
		handle->has_fire_time = 0;
		handle->flags |= GREAL_TIMER_THREAD_TERMINATED;
		// The timer_callback thread may be waiting on this condition
		pthread_cond_signal(&handle->condvar);
		pthread_mutex_unlock(&handle->mutex);
		pthread_join(handle->tid, NULL);
	}
	pthread_mutex_destroy(&handle->mutex);
	pthread_cond_destroy(&handle->condvar);
	handle->callback = NULL;
	handle->data = NULL; 
	greal_free(handle);
}
#endif