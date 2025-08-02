/*
 * Copyright 2019, 2021, 2024, 2025 NXP
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

#include "vglite_layer.h"
#include "svgt12_test.h"

#include "vglite_support.h"
#include "vglite_window.h"
/*-----------------------------------------------------------*/
#include "vg_lite.h"

#include "fsl_gpio.h"
#include "display_support.h"

#if defined(MIMXRT798S_cm33_core0_SERIES)
#include "fsl_lcdif.h"
#endif
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define APP_BUFFER_COUNT 2
#define DEFAULT_SIZE     256.0f;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void vglite_task(void *pvParameters);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static vg_lite_display_t display;
static vg_lite_window_t window;

#if (CUSTOM_VGLITE_MEMORY_CONFIG != 1)
#error "Application must be compiled with CUSTOM_VGLITE_MEMORY_CONFIG=1"
#else
#define VGLITE_COMMAND_BUFFER_SZ (128 * 1024)
/* On RT595S */
#if defined(MIMXRT595S_cm33_SERIES) || defined(MIMXRT798S_cm33_core0_SERIES)
#define VGLITE_HEAP_SZ 0x100000 /* 1 MB */
/* On RT1170 / RT1160 */
#elif defined(MIMXRT1176_cm7_SERIES) || defined(MIMXRT1175_cm7_SERIES) || \
    defined(MIMXRT1173_cm7_SERIES) || defined(MIMXRT1172_SERIES) || \
    defined(MIMXRT1171_SERIES) || defined(MIMXRT1166_cm7_SERIES) || \
    defined(MIMXRT1165_cm7_SERIES)
#define VGLITE_HEAP_SZ 8912896 /* 8.5 MB */
#else
#error "Unsupported CPU !"
#endif
#if (720 * 1280 == (DEMO_PANEL_WIDTH) * (DEMO_PANEL_HEIGHT))
#define TW 720
/* On RT595S */
#if defined(MIMXRT595S_cm33_SERIES)
/* Tessellation window = 720 x 128 */
#define TH 128
/* On RT798S */
#elif defined(MIMXRT798S_cm33_core0_SERIES)
/* Tessellation window = 720 x 640 */
#define TH 640
/* On RT1170 / RT1160 */
#elif defined(MIMXRT1176_cm7_SERIES) || defined(MIMXRT1175_cm7_SERIES) || \
    defined(MIMXRT1173_cm7_SERIES) || defined(MIMXRT1172_SERIES) || \
    defined(MIMXRT1171_SERIES) || defined(MIMXRT1166_cm7_SERIES) || \
    defined(MIMXRT1165_cm7_SERIES)
/* Tessellation window = 720 x 1280 */
#define TH 1280
#else
#error "Unsupported CPU !"
#endif
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
/* Allocate the heap and set the command buffer(s) size */
AT_NONCACHEABLE_SECTION_ALIGN(uint8_t vglite_heap[VGLITE_HEAP_SZ], 64);

void *vglite_heap_base        = &vglite_heap;
uint32_t vglite_heap_size     = VGLITE_HEAP_SZ;
#endif

/* svg specific variables */
static vg_lite_matrix_t g_transform_matrix;
static int win_cx, win_cy;

static UILayers_t g_layers = UI_LAYER_DATA(svgt12_test);

/*******************************************************************************
 * Code
 ******************************************************************************/

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
    vg_lite_error_t error;

    error = layer_free(&g_layers);
    if (error != VG_LITE_SUCCESS) {
        PRINTF("\r\nERROR: layer_free failed for %s\r\n",g_layers.img_info->image_name);
    }

    gradient_cache_free();
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
    // Initialize the draw.
    error = vg_lite_init(TW, TH);
    if (error)
    {
        PRINTF("vg_lite engine init failed: vg_lite_init() returned error %d\r\n", error);
        cleanup();
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

    return error;
}

static void redraw()
{
    vg_lite_buffer_t *rt = VGLITE_GetRenderTarget(&window);
    if (rt == NULL)
    {
        PRINTF("vg_lite_get_renderTarget error\r\n");
        while (1)
            ;
    }

    // Draw the path using the matrix.
    vg_lite_clear(rt, NULL, 0xFFFFFFFF);

    layer_draw(rt, &g_layers, &g_transform_matrix);
    VGLITE_SwapBuffers(&window);

    return;
}

void init_graphic_assets()
{
    int ret = 0;
    vg_lite_error_t error;
    static float sx, sy;

    win_cx = (int)(window.width / 2.0f);
    win_cy = (int)(window.height / 2.0f);

    sx = (window.width / 1.5f)/(g_layers.img_info->image_size[0]);
    sy = (window.height / 1.5f)/(g_layers.img_info->image_size[1]);

    float scale = (sx < sy) ? sx : sy;

    // Calculate scaled image size
    float scaled_width = g_layers.img_info->image_size[0] * scale;
    float scaled_height = g_layers.img_info->image_size[1] * scale;

    // Calculate top-left position to center the image
    float tx = win_cx - (scaled_width / 2.0f);
    float ty = win_cy - (scaled_height / 2.0f);

    vg_lite_identity(&g_transform_matrix);
    vg_lite_translate(tx, ty, &g_transform_matrix);
    vg_lite_scale(scale, scale, &g_transform_matrix);

    ret = layer_init(&g_layers);
    if (ret != VG_LITE_SUCCESS) {
        PRINTF("\r\nERROR: layer_init failed for %s\r\n",g_layers.img_info->image_name);
    }

    error = gradient_cache_init();
    if (error != 0)
    {
        PRINTF("init_cached_gradient failed: error %d\r\n", error);
        while (1)
            ;
    }
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

    init_graphic_assets();

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
