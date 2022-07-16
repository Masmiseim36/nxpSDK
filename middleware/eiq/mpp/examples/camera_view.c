/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "string.h"
#include "stdbool.h"

/* Freescale includes. */
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "board_init.h"

//#define SOURCE_STATIC_IMAGE 1

/* MPP includes */
#include "mpp_api.h"

/* Source parameters */
#ifdef SOURCE_STATIC_IMAGE
/* static image */
#include "stopwatch168_208_vuyx.h"
#else
/* camera */
#define IMAGE_WIDTH  1280
#define IMAGE_HEIGHT 720
#endif
#define IMAGE_FORMAT MPP_PIXEL_YUV1P444
/* display dimensions */
#define DISPLAY_WIDTH  720
#define DISPLAY_HEIGHT 1280

/*******************************************************************************
 * Definitions
 ******************************************************************************/

typedef struct _args_t {
#ifdef SOURCE_STATIC_IMAGE
    void *image_buffer;
#else
    char camera_name[32];
#endif
    char display_name[32];
    mpp_pixel_format_t src_format;
    mpp_pixel_format_t display_format;
} args_t;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void app_task(void *params);

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Application entry point.
 */
int main(int argc, char *argv[])
{
    BaseType_t ret = pdFAIL;
    TaskHandle_t handle = NULL;

    /* Init board hardware. */
    BOARD_Init();
    args_t *args = pvPortMalloc(sizeof(args_t));
    if (!args) {
        PRINTF("Allocation failed\n");
        goto err;
    }

    strcpy(args->display_name, "Lcdifv2Rk055ah");
#ifdef SOURCE_STATIC_IMAGE
    /* Set input arguments*/
    args->image_buffer = image_data;
    if (!args->image_buffer ){
        PRINTF("Failed to get input buffer\n");
        goto err;
    }
#else
    strcpy(args->camera_name, "MipiOv5640");
#endif
    args->display_format = MPP_PIXEL_RGB565;
    args->src_format = IMAGE_FORMAT;

    /* Create application task */
    ret = xTaskCreate(
          app_task,
          "app_task",
          configMINIMAL_STACK_SIZE + 1000,
          (void *) args,
          tskIDLE_PRIORITY + 1,
          &handle);

err:
    if (pdPASS != ret)
    {
        PRINTF("Failed to create app_task task");
        while (1);
    }

    /* MPP API requires RTOS scheduler to be started */
    vTaskStartScheduler();
    for (;;)
        vTaskSuspend(NULL);
    return 0;
}

/* Application task function */
static void app_task(void *params) {
    int ret;
    args_t *args = (args_t *) params;

    PRINTF("[%s]\n", mpp_get_version());

    /* init API */
    ret = mpp_api_init();
    if (ret)
        goto err;

    /* create mpp */
    mpp_t mp;
    mpp_params_t mpp_params;
    memset(&mpp_params, 0, sizeof(mpp_params));
    mpp_params.exec_flag = MPP_EXEC_RC;
    mp = mpp_create(&mpp_params, &ret);
    if (mp == MPP_INVALID)
        goto err;

#ifdef SOURCE_STATIC_IMAGE
    /* set static image element parameters*/
    mpp_img_params_t img_params;
    memset(&img_params, 0, sizeof (mpp_img_params_t));
    img_params.format = args->src_format;
    img_params.width = INPUT_IMAGE_WIDTH;
    img_params.height = INPUT_IMAGE_HEIGHT;
    mpp_static_img_add(mp, &img_params, image_data);

    /* add convert element for color conversion and scaling */
    mpp_element_params_t elem_params;
    memset(&elem_params, 0, sizeof(elem_params));
    elem_params.convert.pixel_format = args->display_format;
    elem_params.convert.width = DISPLAY_WIDTH;
    elem_params.convert.height = DISPLAY_WIDTH * INPUT_IMAGE_HEIGHT / INPUT_IMAGE_WIDTH;
    elem_params.convert.ops = MPP_CONVERT_COLOR | MPP_CONVERT_SCALE;
     ret = mpp_element_add(mp, MPP_ELEMENT_CONVERT, &elem_params, NULL);
    if (ret) {
        PRINTF("Failed to add element CONVERT - op COLOR|ROTATE\n");
        goto err;
    }
#else
    /* add camera */
    mpp_camera_params_t cam_params;
    memset(&cam_params, 0 , sizeof(cam_params));
    cam_params.height = IMAGE_HEIGHT;
    cam_params.width =  IMAGE_WIDTH;
    cam_params.format = args->src_format;
    cam_params.fps    = 30;
    ret = mpp_camera_add(mp, args->camera_name, &cam_params, false);
    if (ret) {
        PRINTF("Failed to add camera %s\n", args->camera_name);
        goto err;
    }

    /* add convert element for color conversion and rotation
       as required by the display */
    mpp_element_params_t elem_params;
    memset(&elem_params, 0, sizeof(elem_params));
    elem_params.convert.angle = ROTATE_90;
    elem_params.convert.pixel_format = args->display_format;
    elem_params.convert.ops = MPP_CONVERT_COLOR | MPP_CONVERT_ROTATE;
    ret = mpp_element_add(mp, MPP_ELEMENT_CONVERT, &elem_params, NULL);
    if (ret) {
        PRINTF("Failed to add element CONVERT - op COLOR|ROTATE\n");
        goto err;
    }
#endif

    /* add display */
    mpp_display_params_t disp_params;
    memset(&disp_params, 0 , sizeof(disp_params));
    disp_params.format = args->display_format;
    ret = mpp_display_add(mp, args->display_name, &disp_params);
    if (ret) {
        PRINTF("Failed to add display %s\n", args->display_name);
        goto err;
    }

    /* start mpp and run application pipeline */
    ret += mpp_start(mp, 1);
    if (ret) {
        PRINTF("Failed to start pipeline\n");
        goto err;
    }

    /* pause application task */
    vTaskSuspend(NULL);

err:
    for (;;)
    {
        PRINTF("Error building application pipeline : ret %d\r\n", ret);
        vTaskSuspend(NULL);
    }
}

