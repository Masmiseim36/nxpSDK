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
#include <gre/sdk/generic_display.h> 

#include <gre/iodefs.h>
#include <gre/sdk/greal.h>
#include "board.h"
#include "touch.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "display_support.h"
#include "fsl_fbdev.h"
#include "vglite_support.h"
#include "vg_lite.h"

gr_application_t *app;

/*
 * Storyboard will call this function on initialization in order to load defined features
 */
const gr_plugin_create_func_t *
gr_application_get_plugins(gr_application_t *app) {
	return sb_plugins;
}

#ifdef __NEWLIB__
//Locking functions for newlib
void __malloc_lock(struct _reent *reent) {
	vTaskSuspendAll();
}

void __malloc_unlock(struct _reent *reent) {
	(void)xTaskResumeAll();
}

void __env_lock() {
	vTaskSuspendAll();
}

void __env_unlock() {
	(void)xTaskResumeAll();
}
#endif

/*
 * Storyboard will call this function on to get resources
 */
#if defined(GRE_FEATURE_VFS_RESOURCES)
#include <gre/sdk/sbresource_vfs.h>
#include "WatchDemo.h"

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
#endif

static void
run_storyboard_app(const char *bundle, int flags, char * const *options, int option_count) {
	
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

#define APP_BUFFER_COUNT 2
#if !DEMO_BUFFER_FIXED_ADDRESS
AT_NONCACHEABLE_SECTION_ALIGN(
    static uint8_t s_frameBuffer[APP_BUFFER_COUNT][DEMO_BUFFER_HEIGHT][DEMO_BUFFER_WIDTH][DEMO_BUFFER_BYTE_PER_PIXEL],
    FRAME_BUFFER_ALIGN);

#define DEMO_BUFFER0_ADDR (uint32_t) s_frameBuffer[0]

#if APP_BUFFER_COUNT > 1
#define DEMO_BUFFER1_ADDR (uint32_t) s_frameBuffer[1]
#endif

#if APP_BUFFER_COUNT > 2
#define DEMO_BUFFER2_ADDR (uint32_t) s_frameBuffer[2]
#endif

#endif

static const uint32_t s_frameBufferAddress[APP_BUFFER_COUNT] = {DEMO_BUFFER0_ADDR,
#if APP_BUFFER_COUNT > 1
                                                                DEMO_BUFFER1_ADDR,
#endif
#if APP_BUFFER_COUNT > 2
                                                                DEMO_BUFFER2_ADDR
#endif
};

fbdev_t g_fbdev;
fbdev_fb_info_t g_fbInfo;
gr_generic_display_layer_info_t main_layer;
int display_inited = 0;

int
gr_generic_display_init(gr_generic_display_info_t *info) {
    status_t status;
    void * buffer;
    fbdev_t *fbdev_ptr          = &g_fbdev;
    fbdev_fb_info_t *fbInfo_ptr = &g_fbInfo;
    vg_lite_error_t 	error;	

    BOARD_PrepareDisplayController();
    display_inited = 1;
    FBDEV_Open(fbdev_ptr, &g_dc, 0);
    
    status = BOARD_PrepareVGLiteController();
    if (status != kStatus_Success)
    {
        PRINTF("Prepare VGlite controller error\r\n");
        while (1);
    }
    
    info->num_layers = 1;
    main_layer.num_buffers = 2;
    info->layer_info = &main_layer;
 
    main_layer.buffer[0] = (void *)s_frameBufferAddress[0];
    main_layer.render_format = GR_RENDER_FMT_RGB565;
    main_layer.width = (uint16_t) DEMO_PANEL_WIDTH;
    main_layer.height = (uint16_t)DEMO_BUFFER_HEIGHT;
    main_layer.stride = (uint16_t)(main_layer.width * GR_RENDER_FMT_BYTESPP(main_layer.render_format));
    main_layer.buffer[1] = (void *)s_frameBufferAddress[1];

    FBDEV_GetFrameBufferInfo(fbdev_ptr, fbInfo_ptr);
    fbInfo_ptr->bufInfo.pixelFormat = DEMO_BUFFER_PIXEL_FORMAT;
    fbInfo_ptr->bufInfo.startX      = DEMO_BUFFER_START_X;
    fbInfo_ptr->bufInfo.startY      = DEMO_BUFFER_START_Y;
    fbInfo_ptr->bufInfo.width       = DEMO_PANEL_WIDTH;
    fbInfo_ptr->bufInfo.height      = DEMO_BUFFER_HEIGHT;
    fbInfo_ptr->bufInfo.strideBytes = DEMO_BUFFER_STRIDE_BYTE;

    fbInfo_ptr->bufferCount = 2;
    fbInfo_ptr->buffers[0] = (void *) s_frameBufferAddress[0];
    fbInfo_ptr->buffers[1] = (void *) s_frameBufferAddress[1];

    memset(main_layer.buffer[0], 0x00, fbInfo_ptr->bufInfo.height * fbInfo_ptr->bufInfo.strideBytes);
    memset(main_layer.buffer[1], 0x00, fbInfo_ptr->bufInfo.height * fbInfo_ptr->bufInfo.strideBytes);
 
    status = FBDEV_SetFrameBufferInfo(fbdev_ptr, fbInfo_ptr);
    if (status != kStatus_Success) {
        while (1);
    }

    buffer = FBDEV_GetFrameBuffer(fbdev_ptr, 0);
    if(!buffer) {
        PRINTF("No framebuffer\n");
        return -1;
    }
    FBDEV_SetFrameBuffer(fbdev_ptr, fbInfo_ptr->buffers[0], 0);
    FBDEV_Enable(fbdev_ptr);
    
    error = vg_lite_init(64,64);
    if (error) {
      PRINTF("VGLite init failed (%i)\n", error);
      return -1;   
    }
    return 0;
}


/*
 * Used to perform any device or API-specific calls that may be necessary
 * when a screen update occurs (such as disabling a framebuffer or enabling another).
 */
int
gr_generic_display_update(const gr_generic_display_info_t *info) {
    FBDEV_SetFrameBuffer(&g_fbdev, info->layer_info[0].buffer[info->layer_info[0].buffer_draw_index], 0);
    return 0;
}

/**
 * Convenience function to grab current time
 */
int64_t
snapshot_time() {
	greal_timespec_t ts;
    greal_clock_gettime(0, &ts);
    return (ts.tv_sec * 1000LL) + (ts.tv_nsec / 1000000LL);
}


/*
 * Task to manage polling of the touch-screen driver for touch events and
 * forwarding to the Storyboard application using API gr_application_send_event()
 *
 * Detected events:
 * 	GR_EVENT_MOTION
 * 	GR_EVENT_PRESS
 * 	GR_EVENT_RELEASE
 */
void
sbengine_input_task(void *arg) {
	const int sleep_msec = 100;
	greal_timespec_t sleep_time = {
		.tv_sec = 0,
		.tv_nsec = sleep_msec * 1000000
	};
 

	while(!display_inited) {
		greal_nanosleep(&sleep_time, NULL);
	}
	/* initialise touch */
	BOARD_Touch_Init();

	touch_poll_state_t previous_touch_state = {0};
	touch_poll_state_t touch_state;
	bool pressed = false;
 
	while (1) {
		if (kStatus_Success != BOARD_Touch_Poll(&touch_state)) {
			greal_nanosleep(&sleep_time, NULL);
			continue;
		}
 
#if (DEMO_PANEL_RM67162 == DEMO_PANEL)
		touch_state.x = (touch_state.x - DEMO_BUFFER_WIDTH) * -1;
		touch_state.y = (touch_state.y - DEMO_BUFFER_HEIGHT) * -1;
#endif

		// De-bounce inputs
		if (previous_touch_state.x == touch_state.x &&
		    previous_touch_state.y == touch_state.y &&
		    previous_touch_state.pressed == touch_state.pressed) {
			continue;
		}
 
		if (touch_state.pressed) {
			gr_ptr_event_t event = {
				.x = touch_state.x,
				.y = touch_state.y,
				.z = 1,
				.timestamp = snapshot_time(),
			};
 
			if (pressed) {
				gr_application_send_event(app, NULL, GR_EVENT_MOTION, GR_EVENT_PTR_FMT, &event, sizeof(event));
			} else {
				pressed = true;
				gr_application_send_event(app, NULL, GR_EVENT_PRESS, GR_EVENT_PTR_FMT, &event, sizeof(event));
			}
 
			previous_touch_state = touch_state;
		} else if (pressed) {
			gr_ptr_event_t event = {
				.x = previous_touch_state.x,
				.y = previous_touch_state.y,
				.z = 1,
				.timestamp = snapshot_time(),
			};
 
			pressed = false;
			gr_application_send_event(app, NULL, GR_EVENT_RELEASE, GR_EVENT_PTR_FMT, &event, sizeof(event));
			previous_touch_state = touch_state;
		}
		greal_nanosleep(&sleep_time, NULL);
	}
}
