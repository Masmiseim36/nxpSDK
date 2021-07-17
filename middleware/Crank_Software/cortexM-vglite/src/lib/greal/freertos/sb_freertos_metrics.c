/*
 * Copyright 2017, Crank Software Inc. All Rights Reserved.
 *
 * For more information email info@cranksoftware.com.
 */

#include <gre/sdk/greal.h>
#include <FreeRTOS.h>
#include <task.h>

#ifdef GRE_TARGET_TOOLCHAIN_iar
#include <iar_dlmalloc.h>
#endif

#if (defined(GRE_TARGET_TOOLCHAIN_mcuxpresso) || defined(GRE_TARGET_TOOLCHAIN_cube))
#include <malloc.h>
#endif

xTaskHandle		idle_handle = NULL;
long 			tick_count = 0;
uint64_t  		cpu_usage = 0; 
uint32_t		cpu_idle_start = 0; 
uint32_t		cpu_idle_spent = 0; 
uint32_t		cpu_idle_total = 0;

void 
vApplicationIdleHook(void) {
	if( idle_handle == NULL ) {
		/**
		 * Store the handle to the idle task. 
		 */
		idle_handle = xTaskGetCurrentTaskHandle();
	}
}

void 
vApplicationTickHook (void) {
	tick_count++; 
}

void 
greal_task_switched_in (void) {
  if(xTaskGetCurrentTaskHandle() == idle_handle) {
    cpu_idle_start = xTaskGetTickCountFromISR();
  }
}

void 
greal_task_switched_out (void) {
	if(xTaskGetCurrentTaskHandle() == idle_handle) {
    	cpu_idle_spent = xTaskGetTickCountFromISR() - cpu_idle_start;
		cpu_idle_start = 0; 
    	cpu_idle_total += cpu_idle_spent; 
  	}
}

void *
greal_setup_metrics(void) {
	/**
	 * Not implemented as this is not needed on FreeRTOS platforms.
	 */
	return NULL; 
}

void 
greal_teardown_metrics(void *data) {
	/**
	 * Not implemented as this is not needed on FreeRTOS platforms.
	 */
	return; 
}


long 
greal_get_process_memory_usage(void *data) {
#if defined GRE_TARGET_TOOLCHAIN_iar
	struct mallinfo nm = __iar_dlmallinfo();
	return nm.uordblks; 
#elif (defined(GRE_TARGET_TOOLCHAIN_mcuxpresso) || defined(GRE_TARGET_TOOLCHAIN_cube))
	struct mallinfo minfo;
	long used;

	minfo = mallinfo();

	used = minfo.uordblks;

	//This field is mis-leading and depends significantly on the implementation
	used += minfo.usmblks;

	return used;
#else
	return 0; 
#endif
}

long 
greal_get_heap_memory_usage(void *data) {
	/**
	 * Not implemented as this value doesn't make sense on FreeRTOS platforms.
	 */
	return 0; 
}

long 
greal_get_sbengine_cpu_time(void *data) {
	//RJD:  This API is expecting the total number of milliseconds that 
	// the app has been running.  Also, this currently isn't a great measurement as it
	// just reports the time that the system is not idle. 
	cpu_usage += tick_count - cpu_idle_total;
	tick_count = 0; 
	cpu_idle_total = 0; 
	return cpu_usage * portTICK_PERIOD_MS; 
}