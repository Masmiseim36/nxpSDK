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
/*-----------------------------------------------------------*/
#include "vg_lite.h"
#include "vglite_support.h"
#include "vglite_window.h"
#include "Elm.h"

#include "clock_analog.h"
#include "hour_needle.h"
#include "minute_needle.h"

#include "fsl_gpio.h"
#include "display_support.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define APP_BUFFER_COUNT 2
#define DEFAULT_SIZE     256.0f;

typedef struct elm_render_buffer
{
    ElmBuffer handle;
    vg_lite_buffer_t *buffer;
} ElmRenderBuffer;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void vglite_task(void *pvParameters);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static vg_lite_display_t display;
static vg_lite_window_t window;

static vg_lite_matrix_t matrix;

static ElmHandle analogClockHandle  = ELM_NULL_HANDLE;
static ElmHandle hourNeedleHandle   = ELM_NULL_HANDLE;
static ElmHandle minuteNeedleHandle = ELM_NULL_HANDLE;
static ElmRenderBuffer elmFB[APP_BUFFER_COUNT];

extern unsigned int ClockAnalog_evo_len;
extern unsigned char ClockAnalog_evo[];

extern unsigned int HourNeedle_evo_len;
extern unsigned char HourNeedle_evo[];

extern unsigned int MinuteNeedle_evo_len;
extern unsigned char MinuteNeedle_evo[];
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

static ELM_BUFFER_FORMAT _buffer_format_to_Elm(vg_lite_buffer_format_t format)
{
    switch (format)
    {
        case VG_LITE_RGB565:
            return ELM_BUFFER_FORMAT_RGB565;
            break;
        case VG_LITE_BGR565:
            return ELM_BUFFER_FORMAT_BGR565;
            break;
        case VG_LITE_BGRX8888:
            return ELM_BUFFER_FORMAT_BGRX8888;
            break;
        default:
            return ELM_BUFFER_FORMAT_RGBA8888;
            break;
    }
}

static void cleanup(void)
{
    vg_lite_close();
}

static int load_clock()
{
    if (ClockAnalog_evo_len != 0)
    {
        analogClockHandle = ElmCreateObjectFromData(ELM_OBJECT_TYPE_EGO, (void *)ClockAnalog_evo, ClockAnalog_evo_len);
    }

    return (analogClockHandle != ELM_NULL_HANDLE);
}

static int load_hour()
{
    if (HourNeedle_evo_len != 0)
    {
        hourNeedleHandle = ElmCreateObjectFromData(ELM_OBJECT_TYPE_EGO, (void *)HourNeedle_evo, HourNeedle_evo_len);
    }

    return (hourNeedleHandle != ELM_NULL_HANDLE);
}

static int load_minute()
{
    if (MinuteNeedle_evo_len != 0)
    {
        minuteNeedleHandle =
            ElmCreateObjectFromData(ELM_OBJECT_TYPE_EGO, (void *)MinuteNeedle_evo, MinuteNeedle_evo_len);
    }

    return (minuteNeedleHandle != ELM_NULL_HANDLE);
}

static int load_texture()
{
    int ret = 0;
    ret     = load_clock();
    if (ret < 0)
    {
        PRINTF("load_clock\r\n");
        return ret;
    }
    ret = load_hour();
    if (ret < 0)
    {
        PRINTF("load_hour\r\n");
        return ret;
    }
    ret = load_minute();
    if (ret < 0)
    {
        PRINTF("load_minute\r\n");
    }
    return ret;
}

static vg_lite_error_t init_vg_lite(void)
{
    vg_lite_error_t error = VG_LITE_SUCCESS;
    int ret               = 0;

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
    ret = ElmInitialize(DEFAULT_VG_LITE_TW_WIDTH, DEFAULT_VG_LITE_TW_HEIGHT);
    if (!ret)
    {
        PRINTF("ElmInitialize failed\n");
        cleanup();
        return VG_LITE_OUT_OF_MEMORY;
    }
    // Set GPU command buffer size for this drawing task.
    error = vg_lite_set_command_buffer_size(VG_LITE_COMMAND_BUFFER_SIZE);
    if (error)
    {
        PRINTF("vg_lite_set_command_buffer_size() returned error %d\n", error);
        cleanup();
        return error;
    }

    // Setup a scale at center of buffer.
    vg_lite_identity(&matrix);
    vg_lite_translate(window.width / 2.0f, window.height / 2.0f, &matrix);
    // load the texture;
    ret = load_texture();
    if (ret < 0)
    {
        PRINTF("load_texture error\r\n");
        return VG_LITE_OUT_OF_MEMORY;
    }

    return error;
}

static ElmBuffer get_elm_buffer(vg_lite_buffer_t *buffer)
{
    for (int i = 0; i < APP_BUFFER_COUNT; i++)
    {
        if (elmFB[i].buffer == NULL)
        {
            elmFB[i].buffer = buffer;
            elmFB[i].handle = ElmWrapBuffer(buffer->width, buffer->height, buffer->stride, buffer->memory,
                                            buffer->address, _buffer_format_to_Elm(buffer->format));
            vg_lite_clear(buffer, NULL, 0x0);
            return elmFB[i].handle;
        }
        if (elmFB[i].buffer == buffer)
            return elmFB[i].handle;
    }
    return 0;
}
static int render(vg_lite_buffer_t *buffer, ElmHandle object)
{
    int status                = 0;
    ElmBuffer elmRenderBuffer = get_elm_buffer(buffer);
    status                    = ElmDraw(elmRenderBuffer, object);
    if (!status)
    {
        status = -1;
        return status;
    }
    ElmFinish();
    return status;
}

static void redraw()
{
    int status = 0;

    vg_lite_buffer_t *rt = VGLITE_GetRenderTarget(&window);
    if (rt == NULL)
    {
        PRINTF("vg_lite_get_renderTarget error\r\n");
        while (1)
            ;
    }
    static float angle = 0;

    // Draw the path using the matrix.
    status = render(rt, analogClockHandle);
    if (status == -1)
    {
        PRINTF("ELM Render analogClockHandle Failed");
        return;
    }

    ElmReset(hourNeedleHandle, ELM_PROP_TRANSFER_BIT);
    ElmTransfer(hourNeedleHandle, 200.0, 200.0);
    ElmRotate(hourNeedleHandle, angle);

    status = render(rt, hourNeedleHandle);
    if (status == -1)
    {
        PRINTF("ELM Render hourNeedleHandle Failed");
    }

    ElmReset(minuteNeedleHandle, ELM_PROP_TRANSFER_BIT);
    ElmTransfer(minuteNeedleHandle, 200.0, 200.0);
    ElmRotate(minuteNeedleHandle, -angle);

    status = render(rt, minuteNeedleHandle);
    if (status == -1)
    {
        PRINTF("ELM Render minuteNeedleHandle Failed");
    }

    angle += 0.5;

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
        if (n > 60)
        {
            time = getTime() - startTime;
            PRINTF("%d frames in %d seconds: %d fps\r\n", n, time / 1000, n * 1000 / time);
            n         = 0;
            startTime = getTime();
        }
    }
}
