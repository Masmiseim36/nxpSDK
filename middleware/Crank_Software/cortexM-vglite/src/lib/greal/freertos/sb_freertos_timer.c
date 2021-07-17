/*
 * Copyright 2019, Crank Software Inc. All Rights Reserved.
 *
 * For more information email info@cranksoftware.com.
 */


#include <gre/sdk/greal.h>
#include <FreeRTOS.h>
#include <timers.h>

#ifdef _GREAL_PLATFORM_TIMER

struct _greal_timer_struct {
	TimerHandle_t 	timerID;					///< Timer ID of the system timer 
	void 			(*callback)(void *data);	///< Callback to fire when the timer fires 
	void			*data;						///< Data to be passed in to the callback 
};


void vCallbackFunction(TimerHandle_t xTimer) {
	greal_timer_struct_t *handle = NULL;
	handle = pvTimerGetTimerID(xTimer);
	if(handle == NULL) {
		return; 
	}
	handle->callback(handle->data);
}

greal_timer_struct_t *
greal_timer_create(void (*callback)(void *), void *data) {
	greal_timer_struct_t *handle = NULL;	

	handle = greal_calloc(1, sizeof(greal_timer_struct_t));
	if(handle == NULL) {
		return NULL; 
	}

	handle->callback = callback; 
	handle->data = data; 

	/**
	* 0 is not a valid time for a time value, so a default value of 100 is being used. 
	*/
	handle->timerID = xTimerCreate("Storyboard Timer", 100, pdFALSE, handle, &vCallbackFunction);
	
	/**
	 * The timer is just being created for now, so stop it, just in case, until it needs to be used.
	 */ 
	xTimerStop(handle->timerID, 0);
	return handle;
}

void 
greal_timer_set_timeout(greal_timer_struct_t *handle, greal_timespec_t fire_time) {
	uint32_t fire_ms;
	uint32_t current_ms; 
	uint32_t wait_ms;
	TickType_t wait_ticks; 
	greal_timespec_t current_time; 

	if(handle == NULL) {
		return; 
	}

	/**
	 * The timer heap manager stores the fire time as the absolute time in the future to fire.
	 * Therefore, to get the number of milliseconds to wait we have to subtract the current time from the 
	 * fire time.
	 */  
	greal_clock_gettime(GREAL_CLOCK_ID, &current_time);
	current_ms = greal_timespec_to_ms(&current_time);
	fire_ms = greal_timespec_to_ms(&fire_time);
	
    /* limit wait_ms to positive numbers */
	if(fire_ms < current_ms) { 
	    wait_ms = 1;
	} else {
		wait_ms = fire_ms - current_ms; 
	}	
	
	/**
	 * The FreeRTOS port can't accept a wait time of 0 ticks, so a value has to be passed in.  The callback can't be called from here because the 
	 * is currently in the main thread and this will deadlock because the callback uses trigger event which blocks. 
	 */
	wait_ticks = pdMS_TO_TICKS(wait_ms);
	if(wait_ticks <= 0) {
		wait_ticks = 1;
	}
	xTimerChangePeriod(handle->timerID, wait_ticks, 0);
	xTimerStart(handle->timerID, 0);

}

void 
greal_timer_cancel(greal_timer_struct_t *handle) {
	if(handle == NULL) {
		return; 
	}

	xTimerStop(handle->timerID, 0);
}

void 
greal_timer_destroy(greal_timer_struct_t *handle) {
	if(handle == NULL) {
		return; 
	}
 
	xTimerDelete(handle->timerID, 0);
	handle->callback = NULL; 
	handle->data = NULL; 
	greal_free(handle);
}

#endif //_GREAL_PLATFORM_TIMER