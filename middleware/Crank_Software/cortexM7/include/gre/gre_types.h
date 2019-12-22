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

typedef void (*gr_event_listener_t)(gr_application_t *app, gr_event_t *event, void *arg);

#endif /* GR_GRE_TYPE_H */
