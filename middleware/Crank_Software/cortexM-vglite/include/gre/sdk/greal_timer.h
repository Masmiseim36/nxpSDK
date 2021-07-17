/*
 * Copyright 2016, Crank Software Inc. All Rights Reserved.
 * 
 * For more information email info@cranksoftware.com.
 */

/**
* This file contains descriptions to OS/HW abstraction layers
* for time manipulation and tracking
*/

#ifndef GR_GREAL_TIMER_H
#define GR_GREAL_TIMER_H

#include <stdint.h> 

#ifndef GREAL_CLOCK_ID
#if GR_OS(linux) || GR_OS(qnx)
#define GREAL_CLOCK_ID	GREAL_CLOCK_MONOTONIC
#else
#define GREAL_CLOCK_ID	GREAL_CLOCK_REALTIME
#endif
#endif

typedef struct _greal_timer_struct greal_timer_struct_t;

/**
 * Converts a greal timespec value to a millisecond value
 * 
 * @param ts A pointer greal timespec to convent to a millisecond value
 * @return The number of milliseconds that are in the greal timespec value
 **/
uint32_t greal_timespec_to_ms(greal_timespec_t *ts);

/**
 * Compares two greal timespec values for equality 
 * 
 * @param ts1 A pointer to the first timespec value to compare  
 * @param ts2 A pointer to the second timespec value to compare
 * @return 0 if the two timespec values are the same, greater than 0 if the first value is greater than the second value, less
 *         than 0 if second value is greater than the first value 
 **/
int greal_timespec_cmp(greal_timespec_t *ts1, greal_timespec_t *ts2);

/**
 * Adds the number of milliseconds specified to the greal timespec value
 *  
 * @param ts A pointer to the greal timespec value to add the milliseconds to
 * @param ms The number of milliseconds to add to the timespec value
 **/
void greal_timespec_add_ms(greal_timespec_t *ts, uint32_t ms);


/**
 * Creates a timer to keep track of when the next Storyboard timer should fire 
 * 
 * @param callback The function to call when the next timer should fire  
 * @param data The data to pass the callback function when it fires
 * @return A pointer to the timer handle to be used with other timer functions 
 **/
greal_timer_struct_t *greal_timer_create(void (*callback)(void *), void *data); 

/**
 * Sets the time for when the timer callback should be called. 
 * 
 * @param handle A pointer to a timer handle that was created using greal_timer_create
 * @param fire_time A greal timespec value representing when in the future the timer should fire 
 **/
void greal_timer_set_timeout(greal_timer_struct_t *handle, greal_timespec_t fire_time); 

/**
 * Cancels the current timer timeout
 * 
 * @param handle The handle to the timer structure to cancel the timeout on
 **/
void greal_timer_cancel(greal_timer_struct_t *handle);

/**
 * Destroys the greal timer and cleans up any system resources associated with the timer
 * 
 * @param handle The handle to the timer that should be destroyed
 **/
void greal_timer_destroy(greal_timer_struct_t *handle); 

#endif /* GR_GREAL_TIMER_H */
