/*
 * Copyright 2007, Crank Software Inc. All Rights Reserved.
 *
 * For more information email info@cranksoftware.com.
 */

/**
* \file gre.h
*  Top level Graphic Runtime Engine defintions 
*/

#ifndef GR_GRE_H
#define GR_GRE_H

#include <stdint.h>
#include <stdarg.h>
#include <gre/gre_types.h>

#ifdef _WIN32
#define DLLExport __declspec( dllexport )
#else 
#define DLLExport
#endif

#if !defined(GR_MAJOR_VERSION)
#define GR_MAJOR_VERSION 				6
#endif
#if !defined(GR_MINOR_VERSION)
#define GR_MINOR_VERSION 				0
#endif
#if !defined(GR_SERVICE_VERSION)
#define GR_SERVICE_VERSION 				0
#endif
#if !defined(GR_BUILD_VERSION)
#define GR_BUILD_VERSION 				0
#endif

#define GR_APP_LOAD_FILE				0x0			///< The "bundle" points to a filename to be loaded
#define GR_APP_LOAD_STRING				0x1			///< The "bundle" points to a nul terminated string to be loaded,
   	   	   	   	   	   	   	   	   	   	   	   	   	///< The string must remain valid until after gr_application_quit()

#define GR_DEBUG_CMD_VERBOSITY      	0x0			///< Handle: No;  Args: a GR_LOG_* value Return: Previous value
#define GR_DEBUG_CMD_DUMP_MODEL     	0x1     	///< Handle: Yes; Args: None Return: None
#define GR_DEBUG_CMD_SELF_TEST_PRE  	0x2			///< Handle: Yes; Args: None Return: None
#define GR_DEBUG_CMD_SELF_TEST_POST 	0x3			///< Handle: Yes; Args: None Return: None
#define GR_DEBUG_CMD_SYSTEM_INFO        0x4			///< Handle: Yes; Args: None Return: None
#define GR_DEBUG_MAP_SHARED_DATA        0x5			///< Handle: Yes; Args: char *, void * Return: None

#if defined(__cplusplus)
extern "C"{
#endif

struct _gr_version {
	uint16_t	major;					///< Major version number for API
	uint16_t	minor;					///< Minor version number for API
	uint16_t	service;				///< Service version number for API
	uint16_t 	build;					///< Build revision
};
typedef struct _gr_version gr_version_t;

/**
 * Create an application handle.
 * The deployment bundle name is generally a path to a file in the
 * filesystem or a memory handle.
 *
 * Additional plugins can be loaded by setting the GRE_PLUGINS environment
 * variable.  Its value should be a colon (:) delimited list of plugins
 * (file paths) that should be loaded after the core engine has been
 * initialized.
 *
 * NOTE: The application will change directory to the directory containing
 * the deployment bundle and this may affect the operation of other threads
 * within this process.
 *
 * Options are provided to plugins and internal components through the
 * option argument array as a pairs of entries:
 *  options[n]   = option_target
 *  options[n+1] = option_argument in getsubopt() format
 *
 * These are valid ways to invoke:
 *  option_arg = { "sdl", "softwarerender", sdl, "noinput" }
 *  option_arg = { "sdl", "softwarerender,noinput" }
 *
 * @param bundle The name of the deployment bundle
 * @param flags Application flags (GR_APP_*)
 * @param options Option argument array
 * @param option_count Number of elements in options
 * @return The application handle or NULL if the application could not be created.
 */
DLLExport gr_application_t * gr_application_create_args(const char *bundle, int flags, char * const *options, int option_count);

/**
 * Create an application handle.
 * The deployment bundle name is generally a path to a file in the
 * filesystem or a memory handle.
 *
 * Additional plugins can be loaded by setting the GRE_PLUGINS environment
 * variable.  Its value should be a colon (:) delimited list of plugins
 * (file paths) that should be loaded after the core engine has been
 * initialized.
 *
 * NOTE: The application will change directory to the directory containing
 * the deployment bundle and this may affect the operation of other threads
 * within this process.
 *
 * @param bundle The name of the deployment bundle
 * @return The application handle or NULL if the application could not be created.
 */
DLLExport gr_application_t * gr_application_create(const char *bundle);

/**
 * Destroy and free an application handle.  This will stop
 * the execution of the application and free any resources
 * that are associated with it.
 *
 * @param handle A non-null handle created with gr_application_create
 */
DLLExport void gr_application_free(gr_application_t *handle);

/**
 * Run the application associated with handle.  This is a blocking
 * call and will not return until the application is terminated.
 *
 * @param handle A non-null handle created with gr_application_create
 * @return A zero value on success or non-zero value on failure
 */
DLLExport int gr_application_run(gr_application_t *handle);

/**
 * Terminate the application.  This call can be made by another
 * thread of execution in order to terminate an application that
 * has been started by a call to gr_application_run().  This call
 * does not block waiting for gr_application_run() to return.
 *
 * @param handle A non-null handle created with gr_application_create
 * @return A zero value on success or non-zero on failure
 */
DLLExport int gr_application_quit(gr_application_t *handle);

/**
 * Execute a debugging operation.  This function can be used to
 * control various execution parameters and dump the internal application
 * state for inspection.
 *
 * @param handle An application handle, or NULL if a handle is not relevant
 * @param command One of GRE_DEBUG_CMD_* with associated data
 * @return A status value related to the command specified
 */
DLLExport int gr_application_debug(gr_application_t *handle, unsigned command, ...);

/**
 * Debug and tracing log message API
 */
enum {
	GR_LOG_ALWAYS = -1,     //Always go to stdout
    GR_LOG_ERROR = 0,       //Errors (fatal and non-fatal)
    GR_LOG_WARNING,         //Warnings
    GR_LOG_INFO,          	//Information, one time, non-repetitive
    GR_LOG_EVENT1,          //Event delivery, excluding motion/mtevent/redraws
    GR_LOG_ACTION,          //Action execution
    GR_LOG_DIAG1,           //Storyboard diagnostic informative
    GR_LOG_DIAG2,           //Storyboard diagnostic detailed
    GR_LOG_EVENT2,			//Motion/mtevent/redraw event delivery
    GR_LOG_TRACE1,          //Storyboard minimal tracing
    GR_LOG_TRACE2			//Storyboard maximum tracing
};

/**
 * Log a diagnostic message.
 * @param app The application handle
 * @param level A value indicating the severity of the message, one of GR_LOG_*
 * @param format A printf' style format string
 */
DLLExport void gre_app_log(gr_application_t *app, int level, const char *format, ...);

/**
 * Log a diagnostic message using a variable argument list.
 * @param app The application handle
 * @param level A value indicating the severity of the message, one of GR_LOG_*
 * @param format A printf' style format string
 * @param arglist A value identifying a variable arguments list initialized with va_start
 */
DLLExport void gre_app_vlog(gr_application_t *app, int level, const char *format, va_list arglist);

/**
 * Set a data variable
 * @param app The application handle
 * @param key The data key variable to set
 * @param fmt The format describing the data argument: 1s0 (string) | 4u1 (uint32_t) | 4s1 (int32_t) ...
 * @param data A pointer to the data to set that can be passed to free() upon overwrite
 * @return -1 on error, 0 on success
 */
DLLExport int gr_application_set_data(gr_application_t *app, const char *key, const char *fmt, void *data);

/**
 * Set a data variable that is statically allocated for the life of the program 
 * Data stored in ROM/flash should use this API. 
 * @param app The application handle
 * @param key The data key variable to set
 * @param fmt The format describing the data argument: 1s0 (string) | 4u1 (uint32_t) | 4s1 (int32_t) ...
 * @param data A pointer to the data to set 
 * @return -1 on error, 0 on success
 */
DLLExport int gr_application_set_data_static(gr_application_t *app, const char *key, const char *fmt, void *data);

/**
 * Get a data variable
 * @param app The application handle
 * @param key The data key to get
 * @param data Location to store the returned data, this is a pointer into the database and should not be released
 * @param format Location to store the returned data format, this is a pointer into the database and should not be released
 * @return -1 on error, 0 on success
 */
DLLExport int gr_application_get_data(gr_application_t *app, const char *key, void **data, char **format);

/**
 * Add an event listener
 * @param app The application handle
 * @param event The event to add a listener for
 * @param handler The handler to call for this event
 * @param data User defined data which is passed to the handler
 * @return -1 on error, 0 on success
 */
DLLExport gr_application_event_listener_t *gr_application_add_event_listener(gr_application_t *app, const char *event,
		gr_event_listener_t handler, void *data);

/**
 * Remove an event listener
 * @param app The application handle
 * @param listener The event listener to remove
 * @return -1 on error, 0 on success
 */
DLLExport int gr_application_rem_event_listener(gr_application_t *app, gr_application_event_listener_t *listener);

/**
 * Inject an event into the application event queue.  
 *
 * After making this call the arguments are copied into the internal event queue and 
 * the content can be freely modified without affecting the queue'ed event. 
 *
 * @param app The application handle
 * @param event_target The name of the event target model element (screen,layer, control, render extension), 
 * 						or NULL to send to the default target (application level)
 * @param event_name The name of the event to send, must not be NULL
 * @param event_format The format of the data (see <data_format.h>, or NULL if no data is being sent
 * @param event_data A pointer do the data to transmit, or NULL if no data is transmitted
 * @param event_nbytes The number of data bytes to transmit, or NULL if no data is transmitted
 * @return 0 on success otherwise an error.
 */
DLLExport int gr_application_send_event(gr_application_t *app, 
							const char *event_target, 
							const char *event_name,
							const char *event_format,
							const void *event_data, 
							int event_nbytes);

/**
 * Get the Storyboard application handle from an action context
 *
 * @param action_context The action context to get the application pointer from
 * @return The Storyboard application handle from the action context
 */
DLLExport gr_application_t *gr_context_get_application(gr_action_context_t *app);  

/**
 * Estimate the maximum length a fully qualified name contained in an action context 
 * 
 * @param action_context The action context to estimate the fully qualified name for
 * @return The estimated length for the maximum fully qualified name
 */
DLLExport int gr_context_max_fqn(gr_action_context_t *action_context);

/**
 * Get the context row information from the action context.
 * 
 * @param action_context The action context to get the row information from 
 * @return The row number if control context for the action context is a table template, -1 otherwise
 */
DLLExport int gr_context_get_row(gr_action_context_t *action_context);

/**
 * Get the context column information from the action context.
 * 
 * @param action_context The action context to get the row information from 
 * @return The column number if control context for the action context is a table template, -1 otherwise
 */
DLLExport int gr_context_get_column(gr_action_context_t *action_context);

/**
 * Get the fully qualified name of the control from the action context if there is one
 * 
 * @param action_context The action context to get the fully qualified name from 
 * @param buffer The character buffer to store the controls fully qualifed name in, the first character may be set to null, if there 
 *               is no control in the action context
 * @param buffer_len The storage space that the buffer has to store the name, see function gr_context_max_fqn
 * @return The length of the control name if there is a control in the context, -1 otherwise
 */
DLLExport int gr_context_get_control(gr_action_context_t *action_context, char *buffer, int buffer_len);

/**
 * Get the fully qualified name of the group from the action context if there is one
 * 
 * @param action_context The action context to get the fully qualified name from 
 * @param buffer The character buffer to store the groups fully qualifed name in, the first character may be set to null, if there 
 *               is no group in the action context
 * @param buffer_len The storage space that the buffer has to store the name, see function gr_context_max_fqn
 * @return The length of the control name if there is a control in the context, -1 otherwise
 */
DLLExport int gr_context_get_group(gr_action_context_t *action_context, char *buffer, int buffer_len);

/**
 * Get the fully qualified name of the layer from the action context if there is one
 * 
 * @param action_context The action context to get the fully qualified name from 
 * @param buffer The character buffer to store the layers fully qualifed name in, the first character may be set to null, if there 
 *               is no layer in the action context
 * @param buffer_len The storage space that the buffer has to store the name, see function gr_context_max_fqn
 * @return The length of the layer name if there is a layer in the context, -1 otherwise
 */
DLLExport int gr_context_get_layer(gr_action_context_t *action_context, char *buffer, int buffer_len);

/**
 * Get the fully qualified name of the screen from the action context if there is one
 * 
 * @param action_context The action context to get the fully qualified name from 
 * @param buffer The character buffer to store the screens fully qualifed name in, the first character may be set to null, if there 
 *               is no screen in the action context
 * @param buffer_len The storage space that the buffer has to store the name, see function gr_context_max_fqn
 * @return The length of the screen name if there is a screen in the context, -1 otherwise
 */
DLLExport int gr_context_get_screen(gr_action_context_t *action_context, char *buffer, int buffer_len);

/**
 * Get the fully qualified name of the active context from the action context if there is one
 * 
 * @param action_context The action context to get the fully qualified name from 
 * @param buffer The character buffer to store the screens fully qualifed name in, the first character may be set to null, if there 
 *               is no screen in the action context
 * @param buffer_len The storage space that the buffer has to store the name, see function gr_context_max_fqn
 * @return The length of the screen name if there is a screen in the context, -1 otherwise
 */
DLLExport int gr_context_get_fqn(gr_action_context_t *action_context, char *buffer, int buffer_len);

/**
 * Get the name of the event from the action context if there is one.  Note that the event name pointer is 
 * only valid within the scope of the function this is being called from. 
 * 
 * @param action_context The action context to get the fully qualified name from 
 * @return The name of the event if there is an event name in the context, null otherwise
 */
DLLExport char * gr_context_get_event_name(gr_action_context_t *action_context);

/**
 * Get the event data from the action context if there is one.  Note that the event data pointer is 
 * only valid within the scope of the function this is being called from. 
 * 
 * @param action_context The action context to get the fully qualified name from 
 * @param nbytes A pointer to an integer to store the size of the event data, it may be null 
 * @return The name of the event if there is an event name in the context, null otherwise
 */
DLLExport void * gr_context_get_event_data(gr_action_context_t *action_context, int *nbytes);



#if defined(__cplusplus)
}
#endif

#endif /* GR_GRE_H */
