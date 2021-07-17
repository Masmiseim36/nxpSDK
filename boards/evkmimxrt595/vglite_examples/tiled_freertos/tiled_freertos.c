/*
 * Copyright 2019, 2021 NXP
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

#include "vglite_support.h"
#include "vglite_window.h"
/*-----------------------------------------------------------*/
#include "vg_lite.h"

#include "fsl_gpio.h"
#include "display_support.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define DEFAULT_SIZE       256.0f;
#define TILED_BUFFER1_ADDR 0x28400000U

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void vglite_task(void *pvParameters);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static vg_lite_display_t display;
static vg_lite_window_t window;

/*
            *-----*
           /       \
          /         \
         *           *
         |          /
         |         X
         |          \
         *           *
          \         /
           \       /
            *-----*
 */
static int8_t path_data[] = {
    2, -5,  -10, // moveto   -5,-10
    4, 5,   -10, // lineto    5,-10
    4, 10,  -5,  // lineto   10, -5
    4, 0,   0,   // lineto    0,  0
    4, 10,  5,   // lineto   10,  5
    4, 5,   10,  // lineto    5, 10
    4, -5,  10,  // lineto   -5, 10
    4, -10, 5,   // lineto  -10,  5
    4, -10, -5,  // lineto  -10, -5
    0,           // end
};

static vg_lite_path_t path = {{-10, -10,         // left,top
                               10, 10},          // right,bottom
                              VG_LITE_HIGH,      // quality
                              VG_LITE_S8,        // -128 to 127 coordinate range
                              {0},               // uploaded
                              sizeof(path_data), // path length
                              path_data,         // path data
                              1};

static vg_lite_matrix_t matrix;
static vg_lite_buffer_t tiled_buffer;

/*******************************************************************************
 * Code
 ******************************************************************************/
#if (DEMO_PANEL_RM67162 == DEMO_PANEL)
void GPIO_INTA_IRQHandler(void)
{
    uint32_t intStat;

    intStat = GPIO_PortGetInterruptStatus(GPIO, BOARD_MIPI_TE_PORT, 0);

    GPIO_PortClearInterruptFlags(GPIO, BOARD_MIPI_TE_PORT, 0, intStat);

    if (intStat & (1U << BOARD_MIPI_TE_PIN))
    {
        BOARD_DisplayTEPinHandler();
    }
}
#endif


int main(void)
{
    /* Init board hardware. */
    status_t status;

    BOARD_InitUARTPins();
    BOARD_InitPsRamPins();

#if (DEMO_PANEL_TFT_PROTO_5 == DEMO_PANEL)
    BOARD_InitFlexIOPanelPins();

    GPIO_PortInit(GPIO, BOARD_SSD1963_RST_PORT);
    GPIO_PortInit(GPIO, BOARD_SSD1963_CS_PORT);
    GPIO_PortInit(GPIO, BOARD_SSD1963_RS_PORT);
#else
    BOARD_InitMipiPanelPins();

    GPIO_PortInit(GPIO, BOARD_MIPI_POWER_PORT);
    GPIO_PortInit(GPIO, BOARD_MIPI_BL_PORT);
    GPIO_PortInit(GPIO, BOARD_MIPI_RST_PORT);

#if (DEMO_PANEL_RM67162 == DEMO_PANEL)
    GPIO_PortInit(GPIO, BOARD_MIPI_TE_PORT);
#endif

#endif

    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    status = BOARD_InitPsRam();
    if (status != kStatus_Success)
    {
        assert(false);
    }

    if (xTaskCreate(vglite_task, "vglite_task", configMINIMAL_STACK_SIZE + 200, NULL, configMAX_PRIORITIES - 1, NULL) !=
        pdPASS)
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
    vg_lite_clear_path(&path);
    vg_lite_close();
}

static vg_lite_error_t init_vg_lite(void)
{
    vg_lite_error_t error = VG_LITE_SUCCESS;

    error = VGLITE_CreateDisplay(&display);
    if (error)
    {
        PRINTF("VGLITE_CreateDisplay failed: VGLITE_CreateDisplay() returned error %d\n", error);
        return error;
    }
    // Initialize the window.
    error = VGLITE_CreateWindow(&display, &window);
    if (error)
    {
        PRINTF("VGLITE_CreateWindow failed: VGLITE_CreateWindow() returned error %d\n", error);
        return error;
    }
    // Initialize the draw.
    error = vg_lite_init(DEFAULT_VG_LITE_TW_WIDTH, DEFAULT_VG_LITE_TW_HEIGHT);
    if (error)
    {
        PRINTF("vg_lite engine init failed: vg_lite_init() returned error %d\n", error);
        cleanup();
        return error;
    }
    // Set GPU command buffer size for this drawing task.
    error = vg_lite_set_command_buffer_size(VG_LITE_COMMAND_BUFFER_SIZE);
    if (error)
    {
        PRINTF("vg_lite_set_command_buffer_size() returned error %d\n", error);
        cleanup();
        return error;
    }

    // Initialize tiled buffer
    memset(&tiled_buffer, 0, sizeof(tiled_buffer));
    tiled_buffer.width   = DEMO_BUFFER_WIDTH;
    tiled_buffer.height  = DEMO_BUFFER_HEIGHT;
    tiled_buffer.stride  = DEMO_BUFFER_WIDTH * 2;
    tiled_buffer.tiled   = VG_LITE_TILED;
    tiled_buffer.format  = VG_LITE_RGB565;
    tiled_buffer.memory  = (void *)TILED_BUFFER1_ADDR;
    tiled_buffer.address = TILED_BUFFER1_ADDR;

    /* Align stride to 64 bytes (required for the tiled raster images) */
    if (tiled_buffer.stride & 0x3f)
        tiled_buffer.stride = (tiled_buffer.stride & (~(uint32_t)0x3f)) + 64;

    vg_lite_clear(&tiled_buffer, NULL, 0x0);

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
    vg_lite_identity(&matrix);
    vg_lite_translate(window.width / 2.0f, window.height / 2.0f, &matrix);
    vg_lite_scale(10, 10, &matrix);

    // Draw the path using the matrix.
    error = vg_lite_draw(&tiled_buffer, &path, VG_LITE_FILL_EVEN_ODD, &matrix, VG_LITE_BLEND_NONE, 0xFF0000FF);
    if (error)
    {
        PRINTF("vg_lite_draw() returned error %d\n", error);
        cleanup();
        return;
    }

    vg_lite_clear(rt, NULL, 0x0);
    vg_lite_identity(&matrix);
    error = vg_lite_blit(rt, &tiled_buffer, &matrix, VG_LITE_BLEND_NONE, 0, VG_LITE_FILTER_BI_LINEAR);
    if (error)
    {
        PRINTF("vg_lite_blit() returned error %d\n", error);
        cleanup();
        return;
    }

    VGLITE_SwapBuffers(&window);

    return;
}

uint32_t getTime()
{
    return (uint32_t)(xTaskGetTickCount() * portTICK_PERIOD_MS);
}

static void vglite_task(void *pvParameters)
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

    error = init_vg_lite();
    if (error)
    {
        PRINTF("init_vg_lite failed: init_vg_lite() returned error %d\n", error);
        while (1)
            ;
    }

    uint32_t startTime, time, n = 0;
    startTime = getTime();
    while (1)
    {
        redraw();
        n++;
        if (n >= 60)
        {
            time = getTime() - startTime;
            PRINTF("%d frames in %d seconds: %d fps\r\n", n, time / 1000, n * 1000 / time);
            n         = 0;
            startTime = getTime();
        }
    }
}
