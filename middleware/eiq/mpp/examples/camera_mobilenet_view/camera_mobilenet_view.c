/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* @brief This example application shows usage of MultiMedia Pipeline to build a simple graph:
 * 2D camera -> split -> image converter -> draw labeled rectangle -> display
 *                   +-> image converter -> inference engine (model: MobileNet v1)
 * The camera view finder is displayed on screen
 * The model performs classification among a list of 1000 object types
 *(see models/mobilenet_v1_0.25_128_quant_int8_cm7/mobilenetv1_labels.h),
 * the model output is displayed on UART console by application */

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "string.h"
#include "stdbool.h"
#include "atomic.h"

/* NXP includes. */
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "board_init.h"

/* MPP includes */
#include "mpp_api.h"
#include "board_config.h"

#include "models/mobilenet_v1_0.25_128_quant_int8_cm7/mobilenetv1_output_postproc.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

typedef struct _user_data_t {
    int inference_frame_num;
    mpp_t mp;
    mpp_elem_handle_t elem;
    mpp_labeled_rect_t labels[1];
    mobilenet_post_proc_data_t inf_out;
    uint32_t accessing; /* boolean protecting access to user data */
} user_data_t;

/*******************************************************************************
 * Variables declaration
 ******************************************************************************/

/* Use TensorFlowLite-Micro as an inference engine by default */
#if !defined(INFERENCE_ENGINE_DeepViewRT) && !defined(INFERENCE_ENGINE_GLOW)
#define INFERENCE_ENGINE_TFLM
#endif

/* Model data input (depends on inference engine) */
#if defined(INFERENCE_ENGINE_TFLM)
#include "models/mobilenet_v1_0.25_128_quant_int8_cm7/mobilenetv1_model_data_tflite.h"
#elif defined(INFERENCE_ENGINE_GLOW)
#include "models/mobilenet_v1_0.25_128_quant_int8_cm7/mobilenet_v1_weights_glow.h"
#include "models/mobilenet_v1_0.25_128_quant_int8_cm7/mobilenet_v1_glow.h"
#elif defined(INFERENCE_ENGINE_DeepViewRT)
#include <models/mobilenet_v1_0.25_128_quant_int8_cm7/mobilenet_model_data_dvrt.h>
#else
#error "ERROR: An inference engine must be selected"
#endif

#define DISPLAY_SMALL_DIM MIN(APP_DISPLAY_WIDTH, APP_DISPLAY_HEIGHT)
#define DISPLAY_LARGE_DIM MAX(APP_DISPLAY_WIDTH, APP_DISPLAY_HEIGHT)

#define CROP_TOP 0
#define CROP_LEFT (APP_CAMERA_WIDTH - APP_CAMERA_HEIGHT)/2
#define CROP_SIZE APP_CAMERA_HEIGHT

#define RECT_LINE_WIDTH 2
/* The detection zone rectangle is a square centered on the display */
/* The square length is the display smallest dimension minus the rectangle line width */
#define DETECTION_ZONE_RECT_TOP 0
#define DETECTION_ZONE_RECT_LEFT (DISPLAY_LARGE_DIM - DISPLAY_SMALL_DIM)/2
#define DETECTION_ZONE_RECT_LENGTH DISPLAY_SMALL_DIM - RECT_LINE_WIDTH

#define MODEL_WIDTH  128
#define MODEL_HEIGHT 128

static const char s_display_name[] = APP_DISPLAY_NAME;
static const char s_camera_name[] = APP_CAMERA_NAME;

#define STATS_PRINT_PERIOD_MS 1000

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
int main()
{
    BaseType_t ret;
    TaskHandle_t handle = NULL;

    /* Init board hardware. */
    BOARD_Init();

    ret = xTaskCreate(
          app_task,
          "app_task",
          configMINIMAL_STACK_SIZE + 1000,
          NULL,
          MPP_APP_MAX_PRIO,
          &handle);

    if (pdPASS != ret)
    {
        PRINTF("Failed to create app_task task");
        while (1);
    }

    vTaskStartScheduler();
    for (;;)
        vTaskSuspend(NULL);
    return 0;
}

int mpp_event_listener(mpp_t mpp, mpp_evt_t evt, void *evt_data, void *user_data) {
    status_t ret;
    const mpp_inference_cb_param_t *inf_output;
    mobilenet_post_proc_data_t out_data;

    /* user_data handle contains application private data */
    user_data_t *app_priv = (user_data_t *)user_data;

    switch(evt) {
    case MPP_EVENT_INFERENCE_OUTPUT_READY:
        /* cast evt_data pointer to correct structure matching the event */
        inf_output = (const mpp_inference_cb_param_t *) evt_data;
        ret = MOBILENETv1_ProcessOutput(
                inf_output,
                app_priv->mp,
                app_priv->elem,
                app_priv->labels,
                &out_data);
        if (ret != kStatus_Success)
            PRINTF("mpp_event_listener: process output error!");
        /* check that we can modify the user data (not accessed by other task) */
        if (Atomic_CompareAndSwap_u32(&app_priv->accessing, 1, 0) == ATOMIC_COMPARE_AND_SWAP_SUCCESS)
        {
            /* copy inference output */
            app_priv->inf_out = out_data;
            __atomic_store_n(&app_priv->accessing, 0, __ATOMIC_SEQ_CST);
        }
        app_priv->inference_frame_num++;
        break;
    case MPP_EVENT_INVALID:
    default:
        /* nothing to do */
        break;
    }

    return 0;
}

static void app_task(void *params)
{
    user_data_t user_data = {0};
    int ret;

    PRINTF("[%s]\n", mpp_get_version());
#if defined(INFERENCE_ENGINE_TFLM)
    PRINTF("Inference Engine: TensorFlow-Lite Micro \r\n");
#elif defined (INFERENCE_ENGINE_GLOW)
    PRINTF("Inference Engine: Glow \r\n");
#elif defined(INFERENCE_ENGINE_DeepViewRT)
    PRINTF("---INFERENCE ENGINE: DeepViewRT---\n");
#else
#error "Please select inference engine"
#endif

    mpp_api_params_t api_params;
    mpp_stats_t api_stats;
    memset(&api_stats, 0, sizeof(api_stats));
    api_params.stats = &api_stats;
    ret = mpp_api_init(&api_params);
    if (ret)
        goto err;

    mpp_t mp;
    mpp_stats_t mpp_stats;
    mpp_params_t mpp_params;
    memset(&mpp_params, 0, sizeof(mpp_params));
    mpp_params.evt_callback_f = &mpp_event_listener;
    mpp_params.mask = MPP_EVENT_ALL;
    mpp_params.cb_userdata = &user_data;
    mpp_params.exec_flag = MPP_EXEC_RC;
    mpp_params.stats = &mpp_stats;

    mp = mpp_create(&mpp_params, &ret);
    if (mp == MPP_INVALID)
        goto err;

    user_data.mp = mp;

    mpp_camera_params_t cam_params;
    memset(&cam_params, 0 , sizeof(cam_params));
    cam_params.height = APP_CAMERA_HEIGHT;
    cam_params.width =  APP_CAMERA_WIDTH;
    cam_params.format = APP_CAMERA_FORMAT;
    cam_params.fps    = 30;
    ret = mpp_camera_add(mp, s_camera_name, &cam_params, false);
    if (ret) {
        PRINTF("Failed to add camera %s\n", s_camera_name);
        goto err;
    }

    /* split the pipeline into 2 branches */
    mpp_t mp_split;
    mpp_stats_t split_stats;
    mpp_params.exec_flag = MPP_EXEC_PREEMPT;
    mpp_params.stats = &split_stats;
    ret = mpp_split(mp, 1 , &mpp_params, &mp_split);
    if (ret) {
        PRINTF("Failed to split pipeline\n");
        goto err;
    }

    /* On the preempt-able branch run the ML Inference (using a mobilenet_v1 TF-Lite model) */
    /* First do crop + resize + color convert */
    mpp_element_params_t elem_params;
    memset(&elem_params, 0, sizeof(elem_params));
    /* color convert */
    elem_params.convert.pixel_format = MPP_PIXEL_RGB;
    elem_params.convert.ops = MPP_CONVERT_COLOR;
    /* resize */
    elem_params.convert.height = MODEL_HEIGHT;
    elem_params.convert.width  = MODEL_WIDTH;
    elem_params.convert.ops |= MPP_CONVERT_SCALE;
    /* crop center of image */
    elem_params.convert.crop.top = CROP_TOP;
    elem_params.convert.crop.bottom = CROP_TOP + CROP_SIZE - 1;
    elem_params.convert.crop.left = CROP_LEFT;
    elem_params.convert.crop.right = CROP_LEFT + CROP_SIZE - 1;
    elem_params.convert.out_area.top = 0;
    elem_params.convert.out_area.bottom = MODEL_HEIGHT - 1;
    elem_params.convert.out_area.left = 0;
    elem_params.convert.out_area.right = MODEL_WIDTH - 1;
    elem_params.convert.ops |= MPP_CONVERT_CROP;

    ret = mpp_element_add(mp_split, MPP_ELEMENT_CONVERT, &elem_params, NULL);
    if (ret ) {
        PRINTF("Failed to add element CONVERT\n");
        goto err;
    }

    /* configure TFlite element with model */
    mpp_element_params_t mobilenet_params;
    static mpp_stats_t mobilenet_stats;
    memset(&mobilenet_params, 0 , sizeof(mpp_element_params_t));

#if defined(INFERENCE_ENGINE_TFLM)
    mobilenet_params.ml_inference.model_data = model_data;
    mobilenet_params.ml_inference.model_size = model_data_len;
    mobilenet_params.ml_inference.model_input_mean = MODEL_INPUT_MEAN;
    mobilenet_params.ml_inference.model_input_std = MODEL_INPUT_STD;
    mobilenet_params.ml_inference.type = MPP_INFERENCE_TYPE_TFLITE;
#elif defined(INFERENCE_ENGINE_GLOW)
    mobilenet_params.ml_inference.model_data = mobilenet_v1_weights_bin;
    mobilenet_params.ml_inference.inference_params.constant_weight_MemSize = MOBILENET_V1_CONSTANT_MEM_SIZE;
    mobilenet_params.ml_inference.inference_params.mutable_weight_MemSize = MOBILENET_V1_MUTABLE_MEM_SIZE;
    mobilenet_params.ml_inference.inference_params.activations_MemSize = MOBILENET_V1_ACTIVATIONS_MEM_SIZE;
    mobilenet_params.ml_inference.inference_params.num_inputs = 1;
    mobilenet_params.ml_inference.inference_params.inputs_offsets[0] = MOBILENET_V1_input;
    mobilenet_params.ml_inference.inference_params.outputs_offsets[0] = MOBILENET_V1_MobilenetV1_Predictions_Reshape_1;
    mobilenet_params.ml_inference.inference_params.model_input_tensors_type = MPP_TENSOR_TYPE_INT8;
    mobilenet_params.ml_inference.inference_params.model_entry_point = &mobilenet_v1;
    mobilenet_params.ml_inference.type = MPP_INFERENCE_TYPE_GLOW ;
#elif defined(INFERENCE_ENGINE_DeepViewRT)
    mobilenet_params.ml_inference.model_data = model_data;
    mobilenet_params.ml_inference.model_size = model_data_len;
    mobilenet_params.ml_inference.type = MPP_INFERENCE_TYPE_DEEPVIEWRT ;
#endif

    mobilenet_params.ml_inference.inference_params.num_inputs = 1;
    mobilenet_params.ml_inference.inference_params.num_outputs = 1;
    mobilenet_params.ml_inference.tensor_order = MPP_TENSOR_ORDER_NHWC;
    mobilenet_params.stats = &mobilenet_stats;

    ret = mpp_element_add(mp_split, MPP_ELEMENT_INFERENCE, &mobilenet_params, NULL);
    if (ret) {
        PRINTF("Failed to add element VALGO_TFLite");
        goto err;
    }
    /* close the pipeline with a null sink */
    ret = mpp_nullsink_add(mp_split);
    if (ret) {
        PRINTF("Failed to add NULL sink\n");
        goto err;
    }

    /* On the main branch of the pipeline, send the frame to the display */
    /* First do color-convert + rotate */
    memset(&elem_params, 0, sizeof(elem_params));
    elem_params.convert.pixel_format = APP_DISPLAY_FORMAT;
    elem_params.convert.angle = APP_CAMERA_DISPLAY_ROTATE;
    elem_params.convert.flip = FLIP_HORIZONTAL;
    elem_params.convert.ops = MPP_CONVERT_COLOR | MPP_CONVERT_ROTATE;
    ret = mpp_element_add(mp, MPP_ELEMENT_CONVERT, &elem_params, NULL);

    if (ret) {
        PRINTF("Failed to add element CONVERT\n");
        goto err;
    }

    /* add one label rectangle */
    memset(&elem_params, 0, sizeof(elem_params));
    memset(&user_data.labels, 0, sizeof(user_data.labels));

    /* params init */
    elem_params.labels.max_count = 1;
    elem_params.labels.detected_count = 1;
    elem_params.labels.rectangles = user_data.labels;

    /* first */
    if ((APP_CAMERA_DISPLAY_ROTATE != ROTATE_0) &&
        (APP_CAMERA_DISPLAY_ROTATE != ROTATE_180)) {
        /* camera and display have different orientation */
        user_data.labels[0].top    = DETECTION_ZONE_RECT_LEFT;
        user_data.labels[0].left   = DETECTION_ZONE_RECT_TOP;
        user_data.labels[0].bottom = DETECTION_ZONE_RECT_LEFT + DETECTION_ZONE_RECT_LENGTH;
        user_data.labels[0].right  = DETECTION_ZONE_RECT_TOP + DETECTION_ZONE_RECT_LENGTH;
    } else {
        user_data.labels[0].top    = DETECTION_ZONE_RECT_TOP;
        user_data.labels[0].left   = DETECTION_ZONE_RECT_LEFT;
        user_data.labels[0].bottom = DETECTION_ZONE_RECT_TOP + DETECTION_ZONE_RECT_LENGTH;
        user_data.labels[0].right  = DETECTION_ZONE_RECT_LEFT + DETECTION_ZONE_RECT_LENGTH;
    }
    user_data.labels[0].line_width = RECT_LINE_WIDTH;
    user_data.labels[0].line_color.rgb.B = 0xff;
    strcpy((char *)user_data.labels[0].label, "no label");

    /* retrieve the element handle while add api */
    ret = mpp_element_add(mp, MPP_ELEMENT_LABELED_RECTANGLE, &elem_params, &user_data.elem);
    if (ret) {
        PRINTF("Failed to add element LABELED_RECTANGLE (0x%x)\r\n", ret);
        goto err;
    }

    mpp_display_params_t disp_params;
    memset(&disp_params, 0 , sizeof(disp_params));
    disp_params.format = APP_DISPLAY_FORMAT;
    disp_params.width  = APP_DISPLAY_WIDTH;
    disp_params.height = APP_DISPLAY_HEIGHT;
    ret = mpp_display_add(mp, s_display_name, &disp_params);
    if (ret) {
        PRINTF("Failed to add display %s\n", s_display_name);
        goto err;
    }

    mpp_stats_enable(MPP_STATS_GRP_API);
    mpp_stats_enable(MPP_STATS_GRP_MPP);
    mpp_stats_enable(MPP_STATS_GRP_ELEMENT);

    /* start preempt-able pipeline branch */
    ret = mpp_start(mp_split, 0);
    if (ret) {
        PRINTF("Failed to start pipeline");
        goto err;
    }
    /* start main pipeline branch */
    ret = mpp_start(mp, 1);
    if (ret) {
        PRINTF("Failed to start pipeline");
        goto err;
    }

    TickType_t xLastWakeTime;
    const TickType_t xFrequency = STATS_PRINT_PERIOD_MS / portTICK_PERIOD_MS;
    xLastWakeTime = xTaskGetTickCount();
#if (configGENERATE_RUN_TIME_STATS == 1)
    static char task_stats_buf[512];
#endif
    for (;;) {
        xTaskDelayUntil( &xLastWakeTime, xFrequency );
        mpp_stats_disable(MPP_STATS_GRP_API);
        mpp_stats_disable(MPP_STATS_GRP_MPP);
        mpp_stats_disable(MPP_STATS_GRP_ELEMENT);
        PRINTF("API stats ------------------------------\r\n");
        PRINTF("rc_cycle = %u ms rc_cycle_max %u ms\r\n",
                api_stats.api.rc_cycle, api_stats.api.rc_cycle_max);
        PRINTF("pr_slot  = %u ms pr_rounds %u app_slot %u ms\r\n",
                api_stats.api.pr_slot, api_stats.api.pr_rounds, api_stats.api.app_slot);
        PRINTF("MPP stats ------------------------------\r\n");
        PRINTF("mpp %p exec_time %u ms\r\n", mpp_stats.mpp.mpp, mpp_stats.mpp.mpp_exec_time);
        PRINTF("mpp %p exec_time %u ms\r\n", split_stats.mpp.mpp, split_stats.mpp.mpp_exec_time);
        PRINTF("Element stats --------------------------\r\n");
        PRINTF("mobilenet : exec_time %u ms\r\n", mobilenet_stats.elem.elem_exec_time);
        if (Atomic_CompareAndSwap_u32(&user_data.accessing, 1, 0) == ATOMIC_COMPARE_AND_SWAP_SUCCESS)
        {
            PRINTF("mobilenet : %s (%d%%)\r\n", user_data.inf_out.label, user_data.inf_out.score);
            __atomic_store_n(&user_data.accessing, 0, __ATOMIC_SEQ_CST);
        }
        mpp_stats_enable(MPP_STATS_GRP_MPP);
        mpp_stats_enable(MPP_STATS_GRP_API);
        mpp_stats_enable(MPP_STATS_GRP_ELEMENT);

#if (configGENERATE_RUN_TIME_STATS == 1)
        vTaskGetRunTimeStats(task_stats_buf);
        PRINTF(task_stats_buf);
#endif
    }

err:
    for (;;)
    {
        PRINTF("Error building application pipeline : ret %d\r\n", ret);
        vTaskSuspend(NULL);
    }
}

