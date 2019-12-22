/*
 * Copyright 2017, Crank Software Inc. All Rights Reserved.
 *
 * For more information email info@cranksoftware.com.
 */

#include <FreeRTOS.h>
#include <task.h>
#include <gre/sdk/greal.h>

#ifdef GRE_TARGET_TOOLCHAIN_iar
#include <iar_dlmalloc.h>

xTaskHandle		idle_handle = NULL;
uint32_t  		cpu_usage = 0; 
uint32_t		cpu_idle_start = 0; 
uint32_t		cpu_idle_spent = 0; 
uint32_t		cpu_idle_total = 0;
uint32_t 		call_count = 0; 

size_t			initial_mem = 0; 
size_t			average_mem = 0; 
size_t 			highest_mem = 0; 
uint64_t		total_mem = 0; 


void 
vApplicationIdleHook(void) 
{
	if( idle_handle == NULL ) {
		idle_handle = xTaskGetCurrentTaskHandle();
	}
}

void 
vApplicationTickHook (void)
{
	static int tick_count = 0;
	
	if(tick_count++ > 1000) {
		tick_count = 0;
		
		if(cpu_idle_start != 0) {
			return; 
		}
		
		if(cpu_idle_total > 1000) {
			cpu_idle_total = 1000;
		}
	
		cpu_usage += (100 - (cpu_idle_total * 100) / 1000);
		cpu_idle_total = 0;
		call_count++; 
	}
}

void 
StartIdleMonitor (void)
{
  if(xTaskGetCurrentTaskHandle() == xTaskGetIdleTaskHandle()) {
    cpu_idle_start = xTaskGetTickCountFromISR();
  }
}

void 
EndIdleMonitor (void)
{
	if(xTaskGetCurrentTaskHandle() == idle_handle) {
		/* Store the handle to the idle task. */
    	cpu_idle_spent = xTaskGetTickCountFromISR() - cpu_idle_start;
		cpu_idle_start = 0; 
    	cpu_idle_total += cpu_idle_spent; 
  	}
}

void *greal_setup_metrics(void) {
	return NULL; 
}

void greal_teardown_metrics(void *data) {
	return; 
}


long greal_get_process_memory_usage(void *data) {
	struct mallinfo nm = __iar_dlmallinfo();
	return nm.uordblks; 
}

long greal_get_heap_memory_usage(void *data) {
	return 0; 
}

long greal_get_sbengine_cpu_time(void *data) {
	return cpu_usage * portTICK_RATE_MS ; 
}

#else

void *greal_setup_metrics(void) {
	return NULL; 
}

void greal_teardown_metrics(void *data) {
	return; 
}

long greal_get_process_memory_usage(void *data) {
	return 0; 
}

long greal_get_heap_memory_usage(void *data) {
	return 0; 
}

long  greal_get_sbengine_cpu_time(void *data) {
	return 0; 
}

void 
vApplicationIdleHook(void) 
{
}

void 
vApplicationTickHook (void)
{
}

void 
StartIdleMonitor (void)
{
}

void 
EndIdleMonitor (void)
{
}

#endif