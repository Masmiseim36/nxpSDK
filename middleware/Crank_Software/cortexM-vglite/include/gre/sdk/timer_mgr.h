/*
* Copyright 2007, Crank Software Inc. All Rights Reserved.
*
* For more information email info@cranksoftware.com.
*/

/**
* \file string_pool.h
* Timer manager interface definition
*/

#ifndef GR_TIMERMGR_H
#define GR_TIMERMGR_H

#include <stdint.h>
#include <gre/gre.h>
#include <gre/sdk/greal.h>

#if defined(__cplusplus)
extern "C"{
#endif

#if !defined(TIMER_MGR_T)
#define TIMER_MGR_T gr_timer_mgr_t
#endif

#if GR_OS(linux) || GR_OS(qnx)
#define SB_CLOCK_ID	GREAL_CLOCK_MONOTONIC
#else
#define SB_CLOCK_ID	GREAL_CLOCK_REALTIME
#endif

typedef struct _gr_timer_mgr gr_timer_mgr_t;

/**
 * Timers: A timer provides a callback (not necessarily sequenced in the same thread
 * of execution as the caller) that can be triggered after a period of ms.
 */
typedef struct gr_timer gr_timer_t;

/**
 * Callback function invoked when timer is triggered.  If the function returns
 * a non-zero value, then the timer will be stopped (gr_timer_set(hdl, 0, 0)).
 * @param timer The timer being triggered
 * @param data The user supplied callback data
 * @param dropped The number of dropped/missed timer callbacks
 * @return 0 to continue the timer, non-zero to quit
 */
typedef int (*gr_timer_cb_t)(gr_timer_t *timer, void *data, int dropped);

/**
 * Callback function invoked when the timer is destroyed.
 * Use this routine to clear any user allocated data.
 * @param data The user supplied callback data
 */
typedef void (*gr_timer_free_t)(void *data);

struct _gr_timer_mgr {
	int					nbytes;
	int     			(*init)(TIMER_MGR_T *mgr, gr_application_t *app, char *options);
	void    			(*fini)(TIMER_MGR_T *mgr);

	/**
	 * Create a new timer object
	 *
	 * The timer callback function may be invoked in a different thread
	 * context than the original invoking thread.
	 *
	 * @param app The application this timer is associated with
	 * @param callback The callback function invoked when the timer fires
	 * @param free_cb The callback invoked when the timer is destroyed, may be NULL
	 * @param arg User defined data provided to callbacks, may be NULL
	 * @return A timer handle, or NULL if the timer creation fails
	 */
	gr_timer_t *    	(*create_timer)(gr_timer_mgr_t *timer_mgr, gr_timer_cb_t callback, gr_timer_free_t free_cb, void *arg);
};

extern TIMER_MGR_T * 	gr_timermgr_create();

/**
 * Destroy a timer object
 *
 * @param timer The timer handle returned from timer_mgr->gr_timer_create
 */
DLLExport void			gr_timer_destroy(gr_timer_t *timer);

/**
 * Set the timeout associated with a timer or cancel a timer
 *
 * To cancel a timer, set both the ms and rep_ms to 0
 *
 * @param timer The timer handle returned from timer_mgr->gr_timer_create
 * @param ms The number of ms before the callback is invoked
 * @param rep_ms The number of ms after the first firing the callback is invoked
 */
DLLExport void			gr_timer_set(gr_timer_t *timer, uint32_t ms, uint32_t rep_ms);

/**
 * Get the user data argument associated with the timer.
 *
 * @param timer The timer handle returned from timer_mgr->gr_timer_create
 * @return The user defined data provided in gr_timer_create
 */
DLLExport void *		gr_timer_get_args(gr_timer_t *timer);


/**
 * Snapshot the current time in ms
 * @return The system relative time (ie since EPOCH) in ms
 **/
DLLExport int64_t		gr_snapshot_time(void);

/**
 * Snapshot the current time in us
 * @return The system relative time (ie since EPOCH) in us
 **/
DLLExport int64_t		gr_snapshot_time_us(void);

/**
 * Snapshot the current time relative to application start in ms
 * @return The time relative to application start in ms
 **/
DLLExport int64_t		gr_snapshot_app_time(gr_application_t *app);

/**
 * Snapshot the current time relative to application start in us
 * @return The time relative to application start in us
 **/
DLLExport int64_t		gr_snapshot_app_time_us(gr_application_t *app);

/**
 * Snapshot the current time into a timespec structure. If a starting
 * timestamp is provided then subtract the time represented by the starting
 * timestamp before returning the value.
 * 
 * @param timestamp The structure to store the current timestamp into
 * @param start_timestamp If non-null the start time to remove after taking the current timestamp
 **/
DLLExport void gr_snapshot_time_and_diff(greal_timespec_t *timestamp, greal_timespec_t *start_timestamp);

/**
 * Convert a ms value to a timespec value
 * 
 * @param ms The time in milliseconds to convert
 * @param ts The structure to store the ms time in
 **/
DLLExport void gr_ms_to_timespec(int64_t ms, greal_timespec_t *ts);

/**
 * Convert a us value to a timespec value
 * 
 * @param us The time in microseconds to convert
 * @param ts The structure to store the ms time in
 **/
DLLExport void gr_us_to_timespec(int64_t us, greal_timespec_t *ts);

#define TS_TO_MS(_ts) (((int64_t)(_ts)->tv_sec * 1000LL) + ((int64_t)(_ts)->tv_nsec / 1000000LL))
#define TS_TO_US(_ts) (((int64_t)(_ts)->tv_sec * 1000000LL) + ((int64_t)(_ts)->tv_nsec / 1000LL))

#define MS_TO_TS(_ms, _ts) 												\
	(_ts)->tv_sec = (_ms) / 1000LL;										\
	(_ts)->tv_nsec = ((_ms) - ((_ts)->tv_sec * 1000LL)) * 1000000LL;	\

#define US_TO_TS(_us, _ts) 												\
	(_ts)->tv_sec = (_us) / 1000000LL;									\
	(_ts)->tv_nsec = ((_us) - ((_ts)->tv_sec * 1000000LL)) * 1000LL;	\


#if defined(__cplusplus)
}
#endif


#endif /* GR_TIMERMGR_H */
