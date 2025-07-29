/*
 * Copyright 2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"

#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"

#include "vg_lite.h"
#include "vglite_support.h"
#include "vglite_window.h"
#include "256x256_etc2_img.h"

#include "fsl_gpio.h"
#include "display_support.h"
#include "fsl_lcdif.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define VGLITE_COMMAND_BUFFER_SZ (128 * 1024)
#define VGLITE_HEAP_SZ 0x100000 /* 1 MB */
#if (720 * 1280 == (DEMO_PANEL_WIDTH) * (DEMO_PANEL_HEIGHT))
/* Tessellation window = 720 x 640 */
#define TW 720
#define TH 640
/* Panel RM67162. Supported only by platform RT595S. */
#elif (400 * 400 == (DEMO_PANEL_WIDTH) * (DEMO_PANEL_HEIGHT))
/* Tessellation window = 400 x 400 */
#define TW 400
#define TH 256
#else
/* Tessellation window = 256 x 256 */
#define TW 256
#define TH 256
#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void vglite_task(void *pvParameters);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static vg_lite_display_t display;
static vg_lite_window_t window;
static vg_lite_buffer_t src;

static vg_lite_matrix_t matrix;
volatile bool s_frameDone = false;

/* Allocate the heap and set the command buffer(s) size */
AT_NONCACHEABLE_SECTION_ALIGN(uint8_t vglite_heap[VGLITE_HEAP_SZ], 64);

void *vglite_heap_base        = &vglite_heap;
uint32_t vglite_heap_size     = VGLITE_HEAP_SZ;

/*******************************************************************************
 * Code
 *******************************/

int main(void)
{
    /* Init board hardware. */
    BOARD_InitHardware();

    if (xTaskCreate(vglite_task, "vglite_task", configMINIMAL_STACK_SIZE + 2000, NULL, configMAX_PRIORITIES - 1, NULL) != pdPASS)
    {
        PRINTF("Task creation failed!.\r\n");
        while (1)
            ;
    }

    vTaskStartScheduler();
    for (;;)
        ;
}

static void cleanup(void)
{
    vg_lite_close();
}

static vg_lite_error_t init_vg_lite(void)
{
    vg_lite_error_t error = VG_LITE_SUCCESS;

    error = VGLITE_CreateDisplay(&display);
    if (error)
    {
        PRINTF("VGLITE_CreateDisplay failed: VGLITE_CreateDisplay() returned error %d\r\n", error);
        return error;
    }
    // Initialize the window.
    error = VGLITE_CreateWindow(&display, &window);
    if (error)
    {
        PRINTF("VGLITE_CreateWindow failed: VGLITE_CreateWindow() returned error %d\r\n", error);
        return error;
    }
    // Set GPU command buffer size for this drawing task.
    error = vg_lite_set_command_buffer_size(VGLITE_COMMAND_BUFFER_SZ);
    if (error)
    {
        PRINTF("vg_lite_set_command_buffer_size() returned error %d\n", error);
        cleanup();
        return error;
    }
    // Initialize the draw.
    error = vg_lite_init(TW, TH);
    if (error)
    {
        PRINTF("vg_lite engine init failed: vg_lite_init() returned error %d\r\n", error);
        cleanup();
        return error;
    }

    return error;
}

static void redraw()
{
    vg_lite_error_t error = VG_LITE_SUCCESS;
    vg_lite_buffer_t *rt = VGLITE_GetRenderTarget(&window);

    if (rt == NULL)
    {
        PRINTF("vg_lite_get_renderTarget error\r\n");
        while (1)
            ;
    }

    vg_lite_clear(rt, NULL, 0xFFFFFFFF);
    vg_lite_identity(&matrix);
    vg_lite_translate((window.width - src.width)/ 2.0f, (window.height - src.height) / 2.0f, &matrix);
    error = vg_lite_blit(rt, &src, &matrix, VG_LITE_BLEND_NONE, 0, VG_LITE_FILTER_POINT);
    if (error)
    {
        PRINTF("vg_lite_blit() returned error %d\n", error);
        cleanup();
        return;
    }

    VGLITE_SwapBuffers(&window);

    return;
}

static void prepare_decompress_etc2_assets()
{
    src.memory = img_etc2;
    src.address = (uint32_t)img_etc2;
    src.width = ETC2_W;
    src.height = ETC2_H;
    src.stride = ETC2_STRIDE;
    src.format = VG_LITE_RGBA8888_ETC2_EAC;
    src.tiled = VG_LITE_TILED;
    src.image_mode = VG_LITE_NORMAL_IMAGE_MODE;
    src.transparency_mode = VG_LITE_IMAGE_OPAQUE;
    src.compress_mode = VG_LITE_DEC_DISABLE;
}


uint32_t getTime()
{
    return (uint32_t)(xTaskGetTickCount() * portTICK_PERIOD_MS);
}

static void vglite_task(void *pvParameters)
{
    status_t status;
    vg_lite_error_t error;
    uint32_t startTime, time, n = 0, fps_x_1000;

    status = BOARD_PrepareVGLiteController();
    if (status != kStatus_Success)
    {
        PRINTF("Prepare VGlite contolor error\r\n");
        while (1)
            ;
    }

    error = init_vg_lite();
    if (error)
    {
        PRINTF("init_vg_lite failed: init_vg_lite() returned error %d\r\n", error);
        while (1)
            ;
    }

    prepare_decompress_etc2_assets();

    startTime = getTime();
    while (1)
    {
        redraw();

        n++;
        if (n >= 60)
        {
            time       = getTime() - startTime;
            fps_x_1000 = (n * 1000 * 1000) / time;
            PRINTF("%d frames in %d mSec: %d.%d FPS\r\n", n, time, fps_x_1000 / 1000, fps_x_1000 % 1000);
            n         = 0;
            startTime = getTime();
        }
    }
}
