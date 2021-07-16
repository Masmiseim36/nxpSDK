/*
 * Copyright 2011, Crank Software Inc. All Rights Reserved.
 *
 * For more information email info@cranksoftware.com.
 */

/**
* \file gre_types.h
* Declare common and forward type definitions
*/

#ifndef GR_GRE_TYPE_H
#define GR_GRE_TYPE_H

//The expectation is that the native float is a 4 byte value

typedef float gr_float_t;

typedef struct _gr_application      gr_application_t;

typedef struct gr_model_context     gr_action_context_t; 

typedef struct _gr_application_event_listener gr_application_event_listener_t;

typedef struct _gr_rotation       	gr_rotation_t;

typedef struct _gr_event gr_event_t;
struct _gr_event {
    char       *name;           ///< Non-null, event name (null terminated)
    char       *target;         ///< Target for event, or NULL for non-targetted events
    char       *format;         ///< The event data format, or NULL if the format isn't known
    int32_t	    timestamp;	    ///< The timestamp for the event, generated when it is pulled from the event queue
    uint32_t    nbytes;         ///< Number of bytes of data in the event
    void       *data;           ///< Event data, nbytes
};

/**
* Reference counted strings are normal character pointers,
* but have a reference ID associated, mark them with their
* own type for API clarity.
*/
typedef char * gr_str_t;

typedef union _gre_data_union {
	void	   *ptr;			///< May be allocated memory to be deallocated according to a specified policy 
	gr_str_t    cptr;			///< May be allocated memory to be deallocated according to a specified policy 
	int64_t     i64;	
	uint64_t    u64;
	double		f64;
	gr_float_t  f32;			///< Placeholder only, not currently supported in all cases.
	int32_t     i32;
	uint32_t    u32;
	int16_t     i16;
	uint16_t    u16;
	int8_t      i8;
	uint8_t     u8;
} gr_data_union_t;

typedef enum DATA_FORMAT_TYPE_INDEX {
	GR_DATA_FORMAT_UNKNOWN=-1,
	GR_DATA_FORMAT_INVALID,
	GR_DATA_FORMAT_1s0,
	GR_DATA_FORMAT_1s1,
	GR_DATA_FORMAT_2s1,
	GR_DATA_FORMAT_4s1,	
	GR_DATA_FORMAT_8s1,	 
	GR_DATA_FORMAT_1u1,
	GR_DATA_FORMAT_2u1,
	GR_DATA_FORMAT_4u1,	
	GR_DATA_FORMAT_8u1,
	GR_DATA_FORMAT_4f1,
	GR_DATA_FORMAT_MAX
} gr_data_format_t;

typedef struct _gre_wrapped_data_t {
	gr_data_union_t 	v;			///< The data storage
	gr_data_format_t	fmt;		///< The data format
	int					policy;		///< (Internal Use Only)
} gr_wrapped_data_t;

typedef void (*gr_event_listener_t)(gr_application_t *app, gr_event_t *event, void *arg);

/**
 * Function to log trace messages
 *
 * @param app A handle to the application
 * @param level Debug logging level, see gre.h
 * @param logmsg The debug message
 * @param udata Userdata to be passed to the callback function
 */
typedef void (*gr_log_writer_cb_t)(gr_application_t *app, int level, const char *logmsg, void *udata);

typedef struct _sb_ccallback {
    const char *name;
    void (*c_callback)(gr_action_context_t *);
} sb_ccallback_t;

#endif /* GR_GRE_TYPE_H */
