/*
 * Copyright 2007, Crank Software Inc. All Rights Reserved.
 *
 * For more information email info@cranksoftware.com.
 */

/**
 * \file
 * Definition of the greio plugin input output data structures
 */


#ifndef GR_IODATA_H
#define GR_IODATA_H

#include <gre/greio.h>

/**
 * Key into the application that points to the data channel that the greio
 * plugin has created.  This is a colon separated list of GREIO channels
 * that have been created.
 */
#define GREIO_CHANNEL_SHARED_DATA_KEY 	"greio_channels"

/**
 * The data that is being set is conditional on the target address
 * that is specified in the event.
 */
#define GREIO_EVENT_DATA_SET            "greio.iodata_set"
typedef struct iodata_set_event {
    char            data[1];        //The data of the event is based on db setting
} iodata_set_event_t;

/**
 * The data that is being extracted is conditional on the target
 * address that is specified in the event.  The address is the
 * parameter that should be passed to gre_open() by the server to
 * reply back to the client.
 *
 * This is a pure data manager operation and will not resolve any
 * links in the data.  It is the client's responsability to serialize
 * or dispatch information returned by multiple threads.
 */
#define GREIO_EVENT_DATA_GET            "greio.iodata_get"
typedef struct iodata_get_event {
    char            reply_channel[GRE_IO_MAX_NAME_LEN];
} iodata_get_event_t;

/**
 * This is the reply of the data from the server to the client based
 * on what was requested.  The target address contains the target
 * information that was requested and the format field contains the
 * information that was available in the database.
 */
#define GREIO_EVENT_DATA_GET_REPLY      "greio.iodata_get_reply"
typedef struct iodata_get_reply_event {
    char            data[1];        //The data of the event is based on db setting
} iodata_get_reply_event_t;

/**
 * This is the event which can set the debugging verbosity
 * on the engine at runtime.
 */
#define GREIO_EVENT_VERBOSITY           "greio.verbosity"
typedef struct  iodata_set_verbosity {
	int              verbosity;      //new verbosity level
} iodata_set_verbosity_t;

#endif /* GR_IODATA_H */
