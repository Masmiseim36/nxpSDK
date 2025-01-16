/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "app.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_gpio.h"
#include "camera_task.h"
#include "vglite_window.h"
#include "display_support.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#if DEMO_BUFFER_COUNT > 3
#error This example does not support more than 3 frame buffers.
#endif

/*! @brief  vglite target scale resolution */
#define DEMO_IMG_WIDHT 640U
#define DEMO_IMG_HEIHT 480U
#define DEMO_VGLITE_FMT  VG_LITE_BGR565

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void vglite_task(void *pvParam);

/*******************************************************************************
 * Variables
 ******************************************************************************/
#if (CUSTOM_VGLITE_MEMORY_CONFIG != 1)
#error "Application must be compiled with CUSTOM_VGLITE_MEMORY_CONFIG=1"
#else
#define VGLITE_COMMAND_BUFFER_SZ (128 * 1024)
/* On RT595S */
#if defined(CPU_MIMXRT595SFFOC_cm33) || defined(CPU_MIMXRT798SGFOA_cm33_core0)
#define VGLITE_HEAP_SZ 0x100000 /* 1 MB */
/* On RT1170 */
#elif defined(CPU_MIMXRT1176DVMAA_cm7) || defined(CPU_MIMXRT1166DVM6A_cm7)
#define VGLITE_HEAP_SZ 8912896 /* 8.5 MB */
#else
#error "Unsupported CPU !"
#endif
#if (720 * 1280 == (DEMO_PANEL_WIDTH) * (DEMO_PANEL_HEIGHT))
#define TW 720
/* On RT595S */
#if defined(CPU_MIMXRT595SFFOC_cm33) || defined(CPU_MIMXRT798SGFOA_cm33_core0)
/* Tessellation window = 720 x 640 */
#define TH 640
/* On RT1170 */
#elif defined(CPU_MIMXRT1176DVMAA_cm7) || defined(CPU_MIMXRT1166DVM6A_cm7)
/* Tessellation window = 720 x 1280 */
#define TH 1280
#else
#error "Unsupported CPU !"
#endif
/* Panel RM67162. Supported only by platform RT595S. */
#elif (400 * 400 == (DEMO_PANEL_WIDTH) * (DEMO_PANEL_HEIGHT))
/* Tessellation window = 400 x 400 */
#define TW 400
#define TH 400
#else
/* Tessellation window = 256 x 256 */
#define TW 256
#define TH 256
#endif
/* Allocate the heap and set the command buffer(s) size */
AT_NONCACHEABLE_SECTION_ALIGN(uint8_t vglite_heap[VGLITE_HEAP_SZ], 64);
AT_NONCACHEABLE_SECTION(vg_lite_error_t vglite_error);
void *vglite_heap_base        = &vglite_heap;
uint32_t vglite_heap_size     = VGLITE_HEAP_SZ;
#endif

static vg_lite_matrix_t s_matrix;
static vg_lite_buffer_t s_blit_source;
static vg_lite_display_t display;
static vg_lite_window_t window;

/*******************************************************************************
 * Code
 ******************************************************************************/
int main(void)
{
    /* Init board hardware. */
    BOARD_InitHardware();
    BOARD_InitPsRamPins_Xspi2();

    BOARD_InitMipiPanelPinsEvk();

    BOARD_Init16bitsPsRam(XSPI2);

    PRINTF("EZH-V flexIO DVP example started\r\n");

    if (pdPASS != xTaskCreate(camera_task, "camera_task", 
                        configMINIMAL_STACK_SIZE + 200, NULL, 3, NULL))
    {
        PRINTF("Task creation failed!.\r\n");
        while (1)
            ;
    }

    if (pdPASS != xTaskCreate(vglite_task, "vglite_task",
                        configMINIMAL_STACK_SIZE + 2000, NULL, 1, NULL))
    {
        PRINTF("Task creation failed!.\r\n");
        while (1)
            ;
    }

    vTaskStartScheduler();
    for (;;)
        ;
}

static void gpu_blit(vg_lite_buffer_t *rt, vg_lite_buffer_t * source)
{
    vg_lite_buffer_t *blitBuffer = source;
    int8_t error = 0;
    
    if (rt == NULL)
    {
        PRINTF("vg_lite_get_renderTarget error\r\n");
        while (1)
            ;
    }

    error = vg_lite_blit(rt, blitBuffer, &s_matrix, VG_LITE_BLEND_NONE,
                    0, VG_LITE_FILTER_POINT);
    if (error) 
    {
      PRINTF("Error****%d:%s\n", __LINE__, __func__);
      return;
    }
    vg_lite_finish();
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
        PRINTF("VGLITE_CreateDisplay() returned error %d\r\n", error);
        return error;
    }
    /* Initialize the window */
    error = VGLITE_CreateWindow(&display, &window);
    if (error)
    {
        PRINTF("VGLITE_CreateWindow() returned error %d\r\n", error);
        return error;
    }
    /* Initialize the draw */ 
    error = vg_lite_init(TW, TH);
    if (error)
    {
        PRINTF("vg_lite_init() returned error %d\r\n", error);
        cleanup();
        return error;
    }

    /* Set GPU command buffer size for this drawing task */
    error = vg_lite_set_command_buffer_size(VGLITE_COMMAND_BUFFER_SZ);
    if (error)
    {
        PRINTF("vg_lite_set_command_buffer_size() returned error %d\n", error);
        cleanup();
        return error;
    }

    vg_lite_identity(&s_matrix);
    vg_lite_scale((DEMO_IMG_WIDHT*1.0f) / (CAMERA_WIDTH*1.0f),
                  (DEMO_IMG_HEIHT*1.0f) / (CAMERA_HEIGHT*1.0f),
                  &s_matrix);

    return error;
}

static void redraw()
{
    /* update blit source address */
    if (g_dvpTransfer.userIdx == g_dvpTransfer.driverIdx)
    {
        return;
    }
    s_blit_source.memory  = 
        (vg_lite_pointer)g_dvpTransfer.queue[g_dvpTransfer.userIdx].pBuf;
    s_blit_source.address = (uint32_t)s_blit_source.memory;
    g_dvpTransfer.userIdx = (g_dvpTransfer.userIdx+1)%QUEUE_SIZE;

    vg_lite_buffer_t *rt = VGLITE_GetRenderTarget(&window);
    if (rt == NULL)
    {
        PRINTF("vg_lite_get_renderTarget error\r\n");
        while (1)
            ;
    }
    vg_lite_clear(rt, NULL, 0x0);
    gpu_blit(rt, &s_blit_source);
    VGLITE_SwapBuffers(&window);

    return;
}

static void vglite_blit_resources(void)
{
    s_blit_source.format	 = DEMO_VGLITE_FMT;
    s_blit_source.width	     = CAMERA_WIDTH;
    s_blit_source.height	 = CAMERA_HEIGHT;
    s_blit_source.stride	 = CAMERA_WIDTH * CAMERA_BPP;
    s_blit_source.memory     = (vg_lite_pointer)g_dvpTransfer.queue[0].pBuf;
    s_blit_source.address	 = (uint32_t)s_blit_source.memory;
    s_blit_source.tiled	     = VG_LITE_LINEAR;
    s_blit_source.compress_mode = VG_LITE_DEC_DISABLE;
}

static uint32_t DEMO_GetTimeMs()
{
    return (uint32_t)(xTaskGetTickCount() * portTICK_PERIOD_MS);
}

static void vglite_task(void *pvParam)
{
    status_t status;
    vg_lite_error_t error;

    status = BOARD_PrepareVGLiteController();
    if (status != kStatus_Success)
    {
        PRINTF("Prepare VGlite contolor error\r\n");
        while (1)
            ;
    }

    vglite_blit_resources();

    error = init_vg_lite();
    if (error)
    {
        PRINTF("init_vg_lite failed: returned error %d\r\n", error);
        while (1)
            ;
    }

    uint32_t startTime, time, n = 0;
    startTime = DEMO_GetTimeMs();
    while (1)
    {
        if (1 == g_newVideoFrame)
        {
            g_newVideoFrame = 0;
            redraw();
            /*
             * Show the frame update rate.
             *
             * NOTE: The project spends time with filling frame buffer, 
             * so the calculated frame update rate
             * might be smaller the actual LCD frame refresh rate, 
             * especially when frame buffer is large.
             */
            n++;
            if (n > 60)
            {
                time = DEMO_GetTimeMs() - startTime;
                PRINTF("Frame Update Rate: %d fps\r\n", n * 1000 / time);
                n = 0;
                startTime = DEMO_GetTimeMs();
            }
        }
    }
}
