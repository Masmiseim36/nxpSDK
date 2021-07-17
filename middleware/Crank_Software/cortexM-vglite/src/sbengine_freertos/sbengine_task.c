/*
 * Copyright 2013, Crank Software Inc. All Rights Reserved.
 *
 * For more information email info@cranksoftware.com.
 */

/**
 * A sample application driver for the Storyboard engine.
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <FreeRTOS.h>
#include "task.h"

#include <gre/gre.h>

/*
 * Static plugin configuration
 */
#include "sbengine_plugins.h"

/*
 * Storyboard will call this function on initialization in order to load defined features
 */
const gr_plugin_create_func_t *
gr_application_get_plugins(gr_application_t *app) {
	return sb_plugins;
}

/*
 * Storyboard will call this function on to get resources
 */
#if defined(GRE_FEATURE_VFS_RESOURCES)
#include <gre/sdk/sbresource_vfs.h>
#include "sbengine_model.h"

extern const struct _sb_resources sb_model_resources[];

sbvfs_resource_t **
sbvfs_get_resource_roots(int *nroots) {
	static const sbvfs_resource_t *roots[1];

	if(roots[0] == NULL) {
		roots[0] = sb_model_resources;
	}

	*nroots = 1;

	return (sbvfs_resource_t **)roots;
}

sb_ccallback_t * sbvfs_get_ccallback_list() {
	return (sb_ccallback_t *) sb_ccallback_list;
}
#endif

static void
run_storyboard_app(const char *bundle, int flags, char * const *options, int option_count) {
	gr_application_t *app;
	app = gr_application_create_args(bundle, flags, options, option_count);

	if(!app) {
		return;
	}

	// Sets the application logging verbosity. For more logging verbosities, refer to gre.h.
	gr_application_debug(app, GR_DEBUG_CMD_VERBOSITY, GR_LOG_ERROR);

	// Start the application
	gr_application_run(app);

	// Free the application instance and close any resources.
	gr_application_free(app);
}

/**
 * This is the main runtime task for Storyboard.  It should be possible to call this
 * task directly from within your FreeRTOS core application.
 */
void
sbengine_main_task(void *arg) {
	char *args[20];
	int n = 0;

	// args[n++] = "greio";
	// args[n++] = "channel=sbiotest";
	//
	// args[n++] = "screen_mgr";
	// args[n++] = "fps";

	// If using VFS, the Storyboard application model will be loaded via string data
	// which is generally present in an sbengine_model.h file named 'sb_model'. This
	// string data would be passed to gr_application_create_args with the GR_APP_LOAD_STRING
	// option flag.
	//
	// In the case a filesystem is present, then the Storyboard model will be loaded
	// from a file on the filesystem. The path to the file should be passed to
	// gr_application_create_args with the GR_APP_LOAD_FILE option flag.

#if defined(GRE_FEATURE_VFS_RESOURCES)
	run_storyboard_app(sb_model, GR_APP_LOAD_STRING, args, n);
#else
	const char example_model_path[] = "sbapp/sbapp.gapp";
	run_storyboard_app(example_model_path, GR_APP_LOAD_FILE, args, n);
#endif

	// Should the initialization fail or app exit, we end up here
#if (INCLUDE_vTaskDelete == 1)
	vTaskDelete(NULL);
#endif
	while(1) { ; }
}

/*
 * The following functions are used to customize display initialization and display updating.
 *
 * Refer to the Storyboard SDK documentation for additional details.
 */

/*
 * Used to initialize the display with any relevant information about the framebuffers
 * being used. The renderer will use the information provided to the gr_generic_display_info_t
 * struct.
 *
 * 0 is returned to indicate success, while any other return value is considered to be an error.
 */
int
gr_generic_display_init(gr_generic_display_info_t *info) {
	return 0;
}

/*
 * Used to perform any device or API-specific calls that may be necessary
 * when a screen update occurs (such as disabling a framebuffer or enabling another).
 */
int
gr_generic_display_update(const gr_generic_display_info_t *info) {
	return 0;
}
