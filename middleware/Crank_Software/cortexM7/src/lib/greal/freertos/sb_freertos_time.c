/*
 * Copyright 2010, Crank Software Inc. All Rights Reserved.
 *
 * For more information email info@cranksoftware.com.
 */

#include <stdio.h>
#include <string.h>
#include <gre/sdk/greal.h>

#include <FreeRTOS.h>
#include <task.h>

int
greal_clock_gettime(int clockid, greal_timespec_t *ts) {
	TickType_t num_ticks;
	uint32_t num_ms;

	num_ticks = xTaskGetTickCount();
	num_ms = num_ticks * portTICK_RATE_MS;

	ts->tv_sec = num_ms / 1000;
	ts->tv_nsec = (num_ms - (ts->tv_sec * 1000)) * 1000000;

	return 0;
}

void
greal_nanosleep(greal_timespec_t *to, greal_timespec_t *remaining) {
	TickType_t num_ticks;
	uint32_t num_ms;

	if(remaining) {
		memset(remaining, 0, sizeof(*remaining));
	}

	num_ms = (to->tv_sec * 1000) + (to->tv_nsec / 1000000);
	num_ticks = num_ms / portTICK_RATE_MS;

	vTaskDelay(num_ticks);
}