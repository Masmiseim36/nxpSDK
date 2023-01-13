/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* @brief This example application shows usage of MultiMedia Pipeline to build a simple graph:
 * 2D camera -> split -> image converter -> draw labeled rectangles -> display
 *                   +-> image converter -> inference engine (model: Nanodet-M)
 * The camera view finder is displayed on screen
 * The model performs object detection among a list of 80 object types (see nanodet_labels.h),
 * the model output is displayed on UART console by application */

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "string.h"
#include "stdbool.h"
#include "stdio.h"
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
#include "mpp_config.h"
#include "board_config.h"
/* utility functions */
#include "models/utils.h"
#include "models/nanodet_m_320_quant_int8_cm7/nanodet_labels.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define MAX_LABEL_RECTS 10
#define NUM_BOXES_MAX 100   /* Nanodet max bounding boxes */

typedef struct _user_data_t {
    int inference_frame_num;
    mpp_t mp;
    mpp_elem_handle_t elem;
    mpp_labeled_rect_t labels[MAX_LABEL_RECTS];
    /* detected boxes */
    box_data  boxes[NUM_BOXES_MAX];
    /* detected boxes count */
    int count;
    uint32_t accessing; /* boolean protecting access */
} user_data_t;

/*******************************************************************************
 * Variables declaration
 ******************************************************************************/

/* Use TensorFlowLite-Micro as an inference engine by default */
#if !defined(INFERENCE_ENGINE_TFLM) && !defined(INFERENCE_ENGINE_GLOW)
#define INFERENCE_ENGINE_TFLM
#endif

/* Model data input (depends on inference engine) */
#if defined(INFERENCE_ENGINE_TFLM)
#if (HAL_TFLM_TENSOR_ARENA_SIZE_KB < 2048)
#error "Must set HAL_TFLM_TENSOR_ARENA_SIZE_KB >= 2048"
#endif
#include "models/nanodet_m_320_quant_int8_cm7/nanodet_m_0.5x_nhwc_tflite.h"
#elif defined(INFERENCE_ENGINE_GLOW)
#include "models/nanodet_m_320_quant_int8_cm7/nanodet_m_weights_glow.h"
#include "models/nanodet_m_320_quant_int8_cm7/nanodet_m_glow.h"
#else
#error "ERROR: An inference engine must be selected"
#endif

#include "models/nanodet_m_320_quant_int8_cm7/nanodet_m_output_postproc.h"

#include "images/skigirl_COCO_320_320_bgra.h"

#define DISPLAY_SMALL_DIM MIN(APP_DISPLAY_WIDTH, APP_DISPLAY_HEIGHT)

#define CROP_TOP 0
#define CROP_LEFT 0
#define CROP_SIZE SRC_IMAGE_WIDTH

#define RECT_LINE_WIDTH 2
/* The detection zone rectangle is a square in the top left corner of the display */
/* The square length is the display smallest dimension minus the rectangle line width */
#define DETECTION_ZONE_RECT_TOP 0
#define DETECTION_ZONE_RECT_LEFT 0
#define DETECTION_ZONE_RECT_LENGTH DISPLAY_SMALL_DIM - RECT_LINE_WIDTH

#define MODEL_WIDTH  320
#define MODEL_HEIGHT 320

#define STATS_PRINT_PERIOD_MS 1000

static const char s_display_name[] = APP_DISPLAY_NAME;

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

/* Translate boxes into labeled rectangles using display characteristics */
void boxes_to_rects(box_data* boxes[], uint32_t num_boxes, uint32_t max_boxes, mpp_labeled_rect_t *rects) {

    uint32_t box_counter = 1;

    /* other rectangles show detected objects */
    for (uint32_t i = 0; i < num_boxes && box_counter < max_boxes; i++) {
        if (!boxes[i])
            continue;
        /* input tensor preview is scaled and moved to fit on screen, and so its bounding boxes */
        rects[box_counter].left = (int)((boxes[i]->left * DISPLAY_SMALL_DIM)/ MODEL_WIDTH) + CROP_TOP;
        rects[box_counter].right = (int)((boxes[i]->right * DISPLAY_SMALL_DIM)/ MODEL_WIDTH) + CROP_TOP;
        rects[box_counter].bottom = (int)((boxes[i]->bottom * DISPLAY_SMALL_DIM)/MODEL_HEIGHT) + CROP_LEFT;
        rects[box_counter].top = (int)((boxes[i]->top * DISPLAY_SMALL_DIM)/MODEL_HEIGHT) + CROP_LEFT;
        rects[box_counter].line_width = RECT_LINE_WIDTH;
        rects[box_counter].line_color.rgb.B = 0xff;
        uint8_t label_size = sizeof(rects[box_counter].label);
        strncpy((char *) rects[box_counter].label, nanodet_labels[boxes[i]->label], label_size-1);
        rects[box_counter].label[label_size-1] = '\0';  /* in case label has been truncated */

        box_counter++;
    }
}

int mpp_event_listener(mpp_t mpp, mpp_evt_t evt, void *evt_data, void *user_data) {
    status_t ret;
    const mpp_inference_cb_param_t *inf_output;
    box_data* finalboxes[NUM_BOXES_MAX];

    /* user_data handle contains application private data */
    user_data_t *app_priv = (user_data_t *)user_data;

    switch(evt) {
    case MPP_EVENT_INFERENCE_OUTPUT_READY:
        /* cast evt_data pointer to correct structure matching the event */
        inf_output = (const mpp_inference_cb_param_t *) evt_data;
        ret = NANODET_ProcessOutput(inf_output, finalboxes);
        if (ret != kStatus_Success)
            PRINTF("mpp_event_listener: process output error!");

        /* check that we can modify the user data (not accessed by other task) */
        if (Atomic_CompareAndSwap_u32(&app_priv->accessing, 1, 0) == ATOMIC_COMPARE_AND_SWAP_SUCCESS)
        {
            app_priv->count = 1;
            /* copy results */
            for (uint32_t i = 0; i < NUM_BOXES_MAX; i++)
            {
                if (!finalboxes[i])
                    continue;
                app_priv->boxes[app_priv->count] = *(finalboxes[i]);
                app_priv->count++;
            }
            /* end of modification of user data */
            __atomic_store_n(&app_priv->accessing, 0, __ATOMIC_SEQ_CST);
        }

        if ( (app_priv->mp != NULL) && (app_priv->elem != 0) && (app_priv->labels != NULL) ){
            mpp_element_params_t params;
            params.labels.detected_count = app_priv->count;
            params.labels.max_count = MAX_LABEL_RECTS;
            params.labels.rectangles = app_priv->labels;
            boxes_to_rects(finalboxes, NUM_BOXES_MAX, MAX_LABEL_RECTS, params.labels.rectangles);
            mpp_element_update(app_priv->mp, app_priv->elem, &params);
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
    static user_data_t user_data = {0};
    int ret;

    PRINTF("[%s]\r\n", mpp_get_version());
#if defined(INFERENCE_ENGINE_TFLM)
    PRINTF("Inference Engine: TensorFlow-Lite Micro \r\n");
#elif defined (INFERENCE_ENGINE_GLOW)
    PRINTF("Inference Engine: Glow \r\n");
#else
#error "Please select inference engine"
#endif

    ret = mpp_api_init(NULL);
    if (ret)
        goto err;

    mpp_t mp;
    mpp_params_t mpp_params;
    memset(&mpp_params, 0, sizeof(mpp_params));
    mpp_params.evt_callback_f = &mpp_event_listener;
    mpp_params.mask = MPP_EVENT_ALL;
    mpp_params.cb_userdata = &user_data;
    mpp_params.exec_flag = MPP_EXEC_RC;

    mp = mpp_create(&mpp_params, &ret);
    if (mp == MPP_INVALID)
        goto err;

    user_data.mp = mp;

    mpp_img_params_t img_params;
    memset(&img_params, 0, sizeof (mpp_img_params_t));
    img_params.format = SRC_IMAGE_FORMAT;
    img_params.width = SRC_IMAGE_WIDTH;
    img_params.height = SRC_IMAGE_HEIGHT;
    mpp_static_img_add(mp, &img_params, image_data);

    /* split the pipeline into 2 branches */
    mpp_t mp_split;
    mpp_params.exec_flag = MPP_EXEC_PREEMPT;

    ret = mpp_split(mp, 1 , &mpp_params, &mp_split);
    if (ret) {
        PRINTF("Failed to split pipeline\n");
        goto err;
    }

    /* On the preempt-able branch run the ML Inference (using a nanodet-m TF-Lite/Glow model) */
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

    /* configure TFlite/Glow element with model */
    mpp_element_params_t nanodet_params;
    static mpp_stats_t nanodet_stats;
    memset(&nanodet_params, 0 , sizeof(mpp_element_params_t));

#if defined(INFERENCE_ENGINE_TFLM)
    nanodet_params.ml_inference.model_data = nanodet_m_0_5x_nhwc_nopermute_tflite;
    nanodet_params.ml_inference.model_size = nanodet_m_0_5x_nhwc_nopermute_tflite_len;
    nanodet_params.ml_inference.tensor_order = MPP_TENSOR_ORDER_NHWC;
    nanodet_params.ml_inference.inference_params.num_outputs = 2;
    nanodet_params.ml_inference.model_input_mean = MODEL_INPUT_MEAN;
    nanodet_params.ml_inference.model_input_std = MODEL_INPUT_STD;
    nanodet_params.ml_inference.type = MPP_INFERENCE_TYPE_TFLITE;
#elif defined(INFERENCE_ENGINE_GLOW)
    nanodet_params.ml_inference.model_data = nanodet_m_weights_bin;
    nanodet_params.ml_inference.inference_params.constant_weight_MemSize = NANODET_M_CONSTANT_MEM_SIZE;
    nanodet_params.ml_inference.inference_params.mutable_weight_MemSize = NANODET_M_MUTABLE_MEM_SIZE;
    nanodet_params.ml_inference.inference_params.activations_MemSize = NANODET_M_ACTIVATIONS_MEM_SIZE;
    nanodet_params.ml_inference.inference_params.inputs_offsets[0] = NANODET_M_data;
    nanodet_params.ml_inference.inference_params.outputs_offsets[0] = NANODET_M_cls_pred;
    nanodet_params.ml_inference.inference_params.outputs_offsets[1] = NANODET_M_reg_pred;
    nanodet_params.ml_inference.inference_params.model_input_tensors_type = MPP_TENSOR_TYPE_INT8;
    nanodet_params.ml_inference.inference_params.model_entry_point = &nanodet_m;
    nanodet_params.ml_inference.type = MPP_INFERENCE_TYPE_GLOW ;
#endif
    nanodet_params.ml_inference.inference_params.num_inputs = 1;
    nanodet_params.ml_inference.inference_params.num_outputs = 2;
    nanodet_params.ml_inference.tensor_order = MPP_TENSOR_ORDER_NHWC;
    nanodet_params.stats = &nanodet_stats;

    ret = mpp_element_add(mp_split, MPP_ELEMENT_INFERENCE, &nanodet_params, NULL);
    if (ret) {
        PRINTF("Failed to add element MPP_ELEMENT_INFERENCE");
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
    if (APP_DISPLAY_WIDTH < APP_DISPLAY_HEIGHT) {
        /* display in portrait => scale to display width */
        elem_params.convert.width = APP_DISPLAY_WIDTH;
        elem_params.convert.height = APP_DISPLAY_WIDTH * SRC_IMAGE_HEIGHT / SRC_IMAGE_WIDTH;
    } else {
        /* display in landscape => scale to display height */
        elem_params.convert.height = APP_DISPLAY_HEIGHT;
        elem_params.convert.width  = APP_DISPLAY_HEIGHT * SRC_IMAGE_WIDTH / SRC_IMAGE_HEIGHT;
    }
    elem_params.convert.ops = MPP_CONVERT_COLOR | MPP_CONVERT_SCALE;
    ret = mpp_element_add(mp, MPP_ELEMENT_CONVERT, &elem_params, NULL);

    if (ret) {
        PRINTF("Failed to add element CONVERT\n");
        goto err;
    }

    /* add one label rectangle */
    memset(&elem_params, 0, sizeof(elem_params));
    memset(&user_data.labels, 0, sizeof(user_data.labels));

    /* params init */
    elem_params.labels.max_count = MAX_LABEL_RECTS;
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
    user_data.labels[0].line_color.rgb.G = 0xff;
    strcpy((char *)user_data.labels[0].label, "Detection zone");

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
    int i;
    const TickType_t xFrequency = STATS_PRINT_PERIOD_MS / portTICK_PERIOD_MS;
    xLastWakeTime = xTaskGetTickCount();
    for (;;) {
        xTaskDelayUntil( &xLastWakeTime, xFrequency );
        mpp_stats_disable(MPP_STATS_GRP_ELEMENT);
        PRINTF("Element stats --------------------------\r\n");
        PRINTF("nanodet : exec_time %u ms\r\n", nanodet_stats.elem.elem_exec_time);
        mpp_stats_enable(MPP_STATS_GRP_ELEMENT);

        if (Atomic_CompareAndSwap_u32(&user_data.accessing, 1, 0))
        {
            /* ignore rectangle of the Detection zone (user_data.boxes[0]) */
            for (i = 1; i < user_data.count; i++)
                PRINTF("nanodet : box %d label %s score %d(%%)\r\n", i,
                        nanodet_labels[user_data.boxes[i].label], (int)(user_data.boxes[i].score * 100.0f));
            __atomic_store_n(&user_data.accessing, 0, __ATOMIC_SEQ_CST);
        }
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

