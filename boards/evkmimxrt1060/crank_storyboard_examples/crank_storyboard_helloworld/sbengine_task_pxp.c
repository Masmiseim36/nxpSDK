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

#include <fsl_elcdif.h>
#include <fsl_lpi2c.h>
#include <fsl_pxp.h>
#include <gre/iodefs.h>
#include <gre/sdk/greal.h>
#include "board.h"
#include "lcd.h"
#include "touch.h"
#include <fsl_cache.h>
#include "fsl_debug_console.h"
#include "board.h"
#include "peripherals.h"

#if (configGENERATE_RUN_TIME_STATS == 1U)
/*******************************************************************************
 * Support for FreeRTOS runtime stats using hi-res PIT timer
 ******************************************************************************/
#include <fsl_pit.h>
 
#define PIT_TICK_HANDLER PIT_IRQHandler
#define PIT_IRQ_ID PIT_IRQn
/* Get source clock for PIT driver */
#define PIT_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_OscClk)
 
void ShowRuntimeTaskStats( void );
#endif

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

#define APP_ELCDIF ELCDIF_PERIPHERAL
#define APP_PXP PXP
#define APP_BPP	2 // 16 bits RGB565
#define APP_IMG_HEIGHT 	ELCDIF_PANEL_HEIGHT	// 272 pixels
#define APP_IMG_WIDTH 	ELCDIF_PANEL_WIDTH 	// 480 pixels

/* PXP Output buffer config. */
static pxp_output_buffer_config_t outputBufferConfig;

gr_generic_display_layer_info_t main_layer;
gr_application_t *app;

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
#include "HelloWorld.h"

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

sb_ccallback_t *
sbvfs_get_ccallback_list() {
  return (sb_ccallback_t *) sb_ccallback_list;
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

	// args[n++] = "screen_mgr";
	// args[n++] = "fps";
            
    // args[n++] = "logger";
    // args[n++] = "perf=1";


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

static volatile bool s_frame_done = false;

static void APP_LCDIF_IRQHandler(void)
{
	uint32_t intStatus;
 
	intStatus = ELCDIF_GetInterruptStatus(APP_ELCDIF);
 
	ELCDIF_ClearInterruptStatus(APP_ELCDIF, intStatus);
 
	if (intStatus & kELCDIF_CurFrameDone)
	{
        if(s_frame_done == false)
        {  
          s_frame_done = true;
        }
	}
    if (intStatus & kELCDIF_TxFifoUnderflow)
    {
      // LCD FiFo underflow caused by bus or task priorities
      PRINTF("WARN: LCDIF_CTRL1[UNDERFLOW_IRQ] - check priorities!\r\n");         
    }

// Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
// exception return operation might vector to incorrect interrupt
#if defined __CORTEX_M && (__CORTEX_M == 4U)
	__DSB();
#endif
}

// This is required to be an externally linked function. If this is made
// static (internal linkage), then it won't be seen by the IRQ assembly code.
void LCDIF_IRQHandler(void)
{
	APP_LCDIF_IRQHandler();
}

/*
 * Configure the PXP block to handle the framebuffer transfer in hardware.
 *
 * We will assume 16bit RGB565 based framebuffer
 * 	- The LCD peripheral is setup to use eLCDIF_Buffer[0]
 * 	- Storyboard is configured to render single buffer, eLCDIF_Buffer[1]
 * 	- The PXP will be configured to transfer from eLCDIF_Buffer[1] to eLCDIF_Buffer[0]
 */
static void APP_InitPxp(void)
{
    PXP_Init(APP_PXP);

    /* PS configure. */
    const pxp_ps_buffer_config_t psBufferConfig = {
        .pixelFormat = kPXP_PsPixelFormatRGB565,
        .swapByte = false,
        .bufferAddr = (uint32_t)eLCDIF_Buffer[1],
        .bufferAddrU = 0U,
        .bufferAddrV = 0U,
        .pitchBytes = APP_IMG_WIDTH * APP_BPP,
    };

    PXP_SetProcessSurfaceBackGroundColor(APP_PXP, 0x1f);
    PXP_SetProcessSurfaceBufferConfig(APP_PXP, &psBufferConfig);
    PXP_SetProcessSurfacePosition(APP_PXP, 0, 0, APP_IMG_WIDTH, APP_IMG_HEIGHT);

    /* Output config. */
    outputBufferConfig.pixelFormat = kPXP_OutputPixelFormatRGB565;
    outputBufferConfig.interlacedMode = kPXP_OutputProgressive;
    outputBufferConfig.buffer0Addr = (uint32_t)eLCDIF_Buffer[0];
    outputBufferConfig.buffer1Addr = 0U;
    outputBufferConfig.pitchBytes = APP_IMG_WIDTH * APP_BPP;
    outputBufferConfig.width = APP_IMG_WIDTH;
    outputBufferConfig.height = APP_IMG_HEIGHT;

    PXP_SetOutputBufferConfig(APP_PXP, &outputBufferConfig);

    /* Disable CSC1, it is enabled by default. */
    PXP_EnableCsc1(APP_PXP, false);
    /* Process 16x16 pixel blocks - Note: make sure this is appropriate for the display geometry */
    PXP_SetProcessBlockSize(APP_PXP, kPXP_BlockSize16);
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
#define GPV0_BASE       0x41000000
#define read_qos_LCD    (0x44000 + 0x100)
#define write_qos_LCD   (0x44000 + 0x104)

int
gr_generic_display_init(gr_generic_display_info_t *info) {
    
	BOARD_InitLcdifPixelClock();

	// Increase read_qos bus arbitration for LCD to avoid underflow
    // so that the LCDIF has SDRAM bus read priority over the core
    // which has default priority 4.  This prevents flicker and LCD
    // buffer disturbance when running code in XIP from SPI Flash 
    // See i.MX RT1050 Reference Manual sec 41.3, SIM_MAIN
    *(uint32_t *)(GPV0_BASE + read_qos_LCD) = 5;

	/* enable backlight */
	BOARD_InitLcd();
    
    info->num_layers = 1;
    main_layer.num_buffers = 1; 		// Single render buffer for Storyboard
    info->layer_info = &main_layer;
 
    main_layer.buffer[0] = (void *)eLCDIF_Buffer[1];
    main_layer.render_format = GR_RENDER_FMT_RGB565;
    main_layer.width = (uint16_t)APP_IMG_WIDTH;;
    main_layer.height = (uint16_t)APP_IMG_HEIGHT;
    main_layer.stride = (uint16_t)(main_layer.width * GR_RENDER_FMT_BYTESPP(main_layer.render_format));

    /* configure PXP for hardware assisted framebuffer transfer */
    APP_InitPxp();
	ELCDIF_RgbModeInit(APP_ELCDIF, &eLCDIF_rgbConfig);
	ELCDIF_SetNextBufferAddr(APP_ELCDIF, (uint32_t)eLCDIF_Buffer[0]);
	ELCDIF_RgbModeStart(APP_ELCDIF);

	return 0;
}


/*
 * Used to perform any device or API-specific calls that may be necessary
 * when a screen update occurs (such as disabling a framebuffer or enabling another).
 */
int
gr_generic_display_update(const gr_generic_display_info_t *info) {

    /* invalidate cache on the buffer about to be being written to LCD by by pxp*/
	DCACHE_CleanInvalidateByRange((uint32_t)info->layer_info[0].buffer[info->layer_info[0].buffer_draw_index], APP_IMG_HEIGHT * APP_IMG_WIDTH * GR_RENDER_FMT_BYTESPP(info->layer_info[0].render_format) );

 	/* Start PXP. */
	PXP_Start(APP_PXP);

	/* Wait for process complete. */
	while (!(kPXP_CompleteFlag & PXP_GetStatusFlags(APP_PXP))) {}

	PXP_ClearStatusFlags(APP_PXP, kPXP_CompleteFlag);
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
 
		// De-bounce inputs
		if (previous_touch_state.x == touch_state.x &&
		    previous_touch_state.y == touch_state.y &&
		    previous_touch_state.pressed == touch_state.pressed) {
			greal_nanosleep(&sleep_time, NULL);
			continue;
		}
 
		if (touch_state.pressed) {
			gr_ptr_event_t event = {
				// No, this isn't a typo. The axes are literally inverted at the driver level.
				.x = touch_state.y,
				.y = touch_state.x,
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
				// No, this isn't a typo. The axes are literally inverted at the driver level.
				.x = previous_touch_state.y,
				.y = previous_touch_state.x,
				.z = 1,
				.timestamp = snapshot_time(),
			};
 
			pressed = false;
			gr_application_send_event(app, NULL, GR_EVENT_RELEASE, GR_EVENT_PTR_FMT, &event, sizeof(event));
			previous_touch_state = touch_state;
#if (configGENERATE_RUN_TIME_STATS == 1U)
			ShowRuntimeTaskStats();
#endif

		}
 
		greal_nanosleep(&sleep_time, NULL);
	}
}

#if (configGENERATE_RUN_TIME_STATS == 1U)
static uint32_t runtimeTimerValue=0;
 
void PIT_TICK_HANDLER(void)
{
    /* Clear interrupt flag.*/
    PIT_ClearStatusFlags(PIT, kPIT_Chnl_0, kPIT_TimerFlag);
    
    /* Increment our counter */
    runtimeTimerValue++;
}
 
/* Require hi-res timer for FreeRTOS runtime stats  */
void vConfigureTimerForRunTimeStats( void )
{
    /* Structure of initialize PIT */
    pit_config_t pitConfig;
    /*
     * pitConfig.enableRunInDebug = false;
     */
    PIT_GetDefaultConfig(&pitConfig);
 
    /* Init pit module */
    PIT_Init(PIT, &pitConfig);
 
    /* Set timer period for channel 0 = 10 times FreeRTOS tick rate*/
    PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, USEC_TO_COUNT(configTICK_RATE_HZ * 10, PIT_SOURCE_CLOCK));
 
    /* Enable timer interrupts for channel 0 */
    PIT_EnableInterrupts(PIT, kPIT_Chnl_0, kPIT_TimerInterruptEnable);
 
    /* Enable at the NVIC */
    EnableIRQ(PIT_IRQ_ID);
 
    /* Start channel 0 */
    PRINTF("\r\nvConfigureTimerForRunTimeStats() Starting PIT channel No.0...\r\n");
    PIT_StartTimer(PIT, kPIT_Chnl_0);
 
    return;
}
 
uint32_t vGetRunTimeStatsTimerValue( void )
{
  return runtimeTimerValue;
}
 
void ShowRuntimeTaskStats( void )
{
  //A buffer into which the execution times will be written, in ASCII form. 
  //This buffer is assumed to be large enough to contain the generated report. 
  //Approximately 40 bytes per task should be sufficient.
  char cWriteBuffer[10*40];
  
  // Note: configGENERATE_RUN_TIME_STATS and configUSE_STATS_FORMATTING_FUNCTIONS 
  // must both be defined as 1 for this function to be available.
  vTaskGetRunTimeStats(&cWriteBuffer[0]);
  PRINTF("Task                 Abs Time         %%Time\r\n");
  PRINTF("=============================================\r\n");
  PRINTF("%s\r\n",cWriteBuffer);         
 
  // Note: configUSE_TRACE_FACILITY and configUSE_STATS_FORMATTING_FUNCTIONS 
  //must be defined as 1 in FreeRTOSConfig.h for this function to be available.
  vTaskList(&cWriteBuffer[0]);
  PRINTF("Name                  State   Priority Stack  Num\r\n");         
  PRINTF("==================================================\r\n");
  PRINTF("%s\r\n",cWriteBuffer);         
    
}
#endif


