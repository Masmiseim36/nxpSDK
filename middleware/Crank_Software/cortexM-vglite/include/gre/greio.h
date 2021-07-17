/*
 * Copyright 2007, Crank Software Inc. All Rights Reserved.
 *
 * For more information email info@cranksoftware.com.
 */
#ifndef GR_GREIO_H
#define GR_GREIO_H

/**
 * \file
 * This is an API for sending and receiving events to other GRE IO
 * based clients.  The data is sent/received along a unidirectional
 * queue that can be implemented on top of a pipe/mqueue/shared memory
 * or some other data channel.
 */

#include <stdint.h>

#define GRE_IO_TYPE_RDONLY			0x0         ///< Open channel for read access
#define GRE_IO_TYPE_XRDONLY			0x1         ///< Open channel for exclusive read access
#define GRE_IO_TYPE_WRONLY			0x2         ///< Open channel for write access
#define GRE_IO_TYPE_MASK			0xF         ///< Type mask

#define GRE_IO_FLAG_NONBLOCK		0x0010		///< Open channel nonblocking
#define GRE_IO_FLAG_MASK			0xFFF0		///< Special flag mask

#define GRE_IO_MAX_NAME_LEN			25          ///< Maximum name gre_io_open name length

#define GRE_IO_MAX_MSG_UNLIMITED	0

#if defined(GRE_TARGET_OS_vxworks)
#define GRE_IO_MAX_MSGS 			128
#define GRE_IO_MAX_MSG_SIZE 		4096
#endif

enum gre_io_error_codes {
	GREIO_ERROR_NO_ERROR = 0,
	GREIO_ERROR_UNKNOWN,
	GREIO_ERROR_OUT_OF_MEMORY,
	GREIO_ERROR_ACCESS,
	GREIO_ERROR_QUEUE_FULL,
	GREIO_ERROR_TIMEOUT,
	GREIO_ERROR_LAST_CODE			//keep at end
};

#ifdef __cplusplus
extern "C"{
#endif


/**
 * Opaque handle for IO operations.
 */
typedef struct _gre_io_ gre_io_t;

typedef struct _gre_io_serialized_data {
		char    *buffer;            ///< Serialized buffer data
		int      buffer_nbytes;     ///< Number of bytes allocated for the buffer (capacity)
		int      data_nbytes;       ///< Number of bytes the data is occupying (size)
} gre_io_serialized_data_t;

typedef struct _multi_data_set {
	uint16_t	size;
	uint16_t	data_offset;
	uint32_t	data_size;
	char		format[8];         // this may need to be revisited if the the format string become bigger than 3 bytes
} gre_io_mdata_t;

typedef struct _gre_io_error {
	int32_t		error_code;			//our error code
	int32_t		sys_code;			//system errno
} gre_io_error_t;

/*
 * GREIO Implementations must not have the following symbols defined when configured for GREIO_MULTI
 * gre_io_open
 * gre_io_close
 * gre_io_send
 * gre_io_receive
 * gre_io_grow_buffer
 * gre_io_max_message
 * gre_io_get_error_codes
 * gre_io_get_error_message
 */
#if !defined(GREIO_MULTI) || !GREIO_MULTI

/**
 * Open an IO connection using a named connection.
 *
 * @param io_name The name of the io-channel to use
 * @param flag The mode you want to open the queue in
 * @return handle to greio channel or NULL on failure.
 */
gre_io_t *gre_io_open(const char *io_name, int flag, ...);

/**
 * Close an io connection.  Any pending clients will return
 *
 * with an error on their action.
 * @param handle A valid handle created with gre_io_open()
 */
void gre_io_close(gre_io_t *handle);

/**
 * Send a serialized buffer to the handle.
 * @param handle A valid handle created with gre_io_open()
 * @param buffer A data buffer containing a serialized event
 * @return -1 on failure anything else is success
 */
int gre_io_send(gre_io_t *handle, gre_io_serialized_data_t *buffer);

/**
 * Receive a serialized event.  This call blocks until an event is received
 * or until the channel is destroyed.
 *
 * @param handle A valid handle created with gre_io_open()
 * @param buffer A pointer to a serialized buffer pointer.  If the
 *  buffer is NULL then a new buffer is allocated otherwise the buffer
 *  provided is used to store the received event.
 * @return The size of the message received or -1 on failure
 */
int gre_io_receive(gre_io_t *handle, gre_io_serialized_data_t **buffer);

/**
 * Set the max message size that may be sent over an IO channel to match the size specified by the provided serialized buffer.
 * This is not available on all greio implementations and will fail by returning -1 if it is not supported.
 *
 * @param handle A valid handle created with gre_io_open()
 * @param buffer A data buffer
 * @return -1 on failure anything else is success
 */
int gre_io_grow_buffer(gre_io_t *handle, gre_io_serialized_data_t *buffer);

/**
 * Get the maximum size of a message that may be sent over IO channel.
 *
 * @param handle A valid handle created with gre_io_open()
 * @return -1 on error, success the size in bytes
 */
int gre_io_max_message(gre_io_t *handle);

/**
 * Get the error codes if greio experiences an error.
 *
 * @param handle A valid handle created with gre_io_open()
 * @param errorcodes A valid gre_io_error_t structure to be filled out
*/
void gre_io_get_error_codes(gre_io_t *handle, gre_io_error_t *errorcodes);

/**
 * Get the error message for an error code returned by a call to gre_io_get_error_codes.
 *
 * @param errorcodes A valid gre_io_error_t structure containing the errors returned 
 *  from a call to gre_io_get_error_codes
 * @return The error message
*/
const char * gre_io_get_error_message(gre_io_error_t *errorcodes);

#endif /* !GREIO_MULTI */

/**
 * This creates a data buffer to hold the serialized event data required
 * for transmission.  This allows a resizable buffer to be allocated once
 * and then re-used for multiple event transmissions.
 *
 * If the provided buffer is not NULL and already contains data within
 * its buffers, then the resizing of the buffer will transfer the contents
 * of the old internal buffer to the new buffer, similar to the realloc()
 * function.  The buffer may be used in this fashion when data is being
 * streamed so that multiple buffers are not required.
 *
 * @param buffer The buffer to be sized, or NULL to allocate a new buffer
 * @param nbytes The number of bytes this buffer should be able to support
 * @return A buffer with room for a message nbytes in size or NULL if no space available
 */
gre_io_serialized_data_t * gre_io_size_buffer(gre_io_serialized_data_t *buffer, int nbytes);

/**
 * This sets the data payload of the buffer to zero (0) bytes.
 * The buffer is not resized or free'ed, but data content in
 * the buffer should be considered lost.
 *
 * @param buffer The buffer to be de-allocated
 */
void gre_io_zero_buffer(gre_io_serialized_data_t *buffer);

/**
 * This de-allocates a buffer that has been allocated through the
 * calls to gre_io_* functions.
 *
 * @param buffer The buffer to be de-allocated
 */
void                       gre_io_free_buffer(gre_io_serialized_data_t *buffer);

/**
 * Transform event data elements (see <io_mgr.h>) into a serialized data buffer.
 *
 * @param buffer The buffer that will contain the serialized data or NULL if a new buffer should be allocated
 * @param event_target The name of the event target, or NULL to send to the default target
 * @param event_name The name of the event to send, or NULL to send an empty event
 * @param event_format The format of the data (see <data_format.h>, or NULL if no data is being sent
 * @param event_data A pointer do the data to transmit, or NULL if no data is transmitted
 * @param event_nbytes The number of data bytes to transmit, or NULL if no data is transmitted
 * @return A buffer with the serialized data or NULL on error (errno is set)
 */
gre_io_serialized_data_t * gre_io_serialize(gre_io_serialized_data_t *buffer,
                                            const char *event_target,
                                            const char *event_name,
                                            const char *event_format,
                                            const void *event_data, int event_nbytes);

/**
 * Transform a serialized buffer into event data elements (see <io_mgr.h>).  The
 * pointers returned point back into the content of the serialized buffer so the
 * buffer can't be de-allocated until clients are finished manipulating the data
 * elements.
 *
 * @param buffer The buffer containing the serialized data
 * @param event_target Location to store the event target
 * @param event_name Location to store the event name
 * @param event_format  Location to store the event format
 * @param event_data  Location to store the event data
 * @return The number of bytes in the event_data structure
 */
int gre_io_unserialize(gre_io_serialized_data_t *buffer,
                        char **event_target,
                        char **event_name,
                        char **event_format,
                        void **event_data);

/**
 * Add a data key/value pair to a serialized buffer.  
 * This call can be used to add serialize multiple data
 * sets into a single send. This buffer can be later sent via
 * the gre_io_send_mdata call.
 *
 * @param mbuffer The buffer containing the serialized data
 * @param key_name Name of the variable to be set in data manager
 * @param data_format The format of the data (see <data_format.h>, or NULL if no data is being sent
 * @param data  A pointer do the data to transmit
 * @param data_nbytes  The number of data bytes to transmit, or NULL if no data is transmitted
 * @return -1 on failure anything else is success
 */
int gre_io_add_mdata(gre_io_serialized_data_t **mbuffer,
				const char *key_name,
				const char *data_format,
				const void *data, int data_nbytes);

/**
 * Send a serialized buffer of mdata (data manager key/value pairs) to the handle.
 *
 * @param handle A valid handle created with gre_io_open()
 * @param md_buffer A data buffer containing a serialized event
 * @return -1 on failure anything else is success
 */
int gre_io_send_mdata(gre_io_t *handle, gre_io_serialized_data_t *md_buffer);

/**
 * Event Data Conversion Routines.
 */

/**
 * Convert an array of parsable string segments (ie [0]4s1:value [1]20 [3]1s0:name [4]Crank ...)
 * into a memory block and event format string.  Clients are responsible for the memory 
 * associated with the returned format string and the event data and should call free() to 
 * deallocated it when they are finished.
 *
 * @param argv  A pointer to format/value segments
 * @param argc  The number of segments in argv
 * @param event_data   A pointer to a pointer where the event data can be stored.
 * @param event_data_nbytes   The number of bytes of data event_data points at
 * @return A format string describing the memory block or NULL on failure
 */
char * greio_array_to_event_data(char **argv, int argc, void **event_data, int *event_data_nbytes);

/**
 * Convert a parsable data string to a memory block and event format string. Clients are
 * responsible for the memory associated with the returned format string and the event 
 * data and should call free() to deallocated it when they are finished.
 *
 * @param data_string  A non-nul data and format string (from greio_event_data_to_string)
 * @param event_data   A pointer to a pointer where the event data can be stored.
 * @param event_data_nbytes   The number of bytes of data event_data points at
 * @return A format string describing the memory block or NULL on failure
 */
char * greio_string_to_event_data(char *data_string, void **event_data, int *event_data_nbytes);

/**
 * Convert an user data event payload to a parseable data string.  Client are expected to
 * expect a NULL return code if the format string or data is empty. Clients are responsible for
 * the memory associated with the string returned and should call free() to deallocate it.
 *
 * @param event_format 	A nul terminated string that contains a standard format string
 * @param event_data    A pointer to a block of memory to be interpreted via event_format
 * @param event_data_nbytes The number of bytes that event_data points to
 * @return An allocated string parsable via greio_string_to_event_data or NULL on failure
 */
char * greio_event_data_to_string(char *event_format, void *event_data, int event_data_nbytes);

#ifdef __cplusplus
}
#endif

#endif /* GR_GREIO_H */
