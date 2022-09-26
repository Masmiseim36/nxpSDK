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

/* NXP includes. */
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "board_init.h"

/* MPP includes */
#include "mpp_api.h"

/* utility functions */
#include "models/utils.h"
#include "models/nanodet_m_320_quant_int8_cm7/nanodet_labels.h"

#include "examples_common.h"

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
} user_data_t;

/*******************************************************************************
 * Variables declaration
 ******************************************************************************/

/* set this flag to 1 in order to replace the camera source by static image */
#ifndef SOURCE_STATIC_IMAGE
#define SOURCE_STATIC_IMAGE 0
#endif

#define CAMERA_WIDTH  1280
#define CAMERA_HEIGHT 720

#define DISPLAY_WIDTH  720
#define DISPLAY_HEIGHT 1280

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

#if (SOURCE_STATIC_IMAGE == 1)
#include "images/skigirl_COCO_320_320_bgra.h"
#define CROP_TOP 0
#define CROP_LEFT 0
#define CROP_SIZE SRC_IMAGE_WIDTH
#define RECT_TOP_OFFSET 0
#else
#define CROP_TOP 0
#define CROP_LEFT (CAMERA_WIDTH - CAMERA_HEIGHT)/2
#define CROP_SIZE CAMERA_HEIGHT
#define RECT_TOP_OFFSET (DISPLAY_HEIGHT - DISPLAY_WIDTH)/2
#endif

#define MODEL_WIDTH  320
#define MODEL_HEIGHT 320

static const char s_display_name[] = "Lcdifv2Rk055ah";

#if (SOURCE_STATIC_IMAGE == 0)
static const char s_camera_name[] = "MipiOv5640";
#endif

box_data* g_finalboxes[NUM_BOXES_MAX] = {0};

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
          tskIDLE_PRIORITY + 1,
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
    /* first rectangle shows detection zone */
    rects[0].top = RECT_TOP_OFFSET;
    rects[0].left = 1;
    rects[0].bottom = RECT_TOP_OFFSET + DISPLAY_WIDTH;
    rects[0].right = DISPLAY_WIDTH - 1;
    rects[0].line_width = 2;
    rects[0].line_color.rgb.G = 0xff;
    strcpy((char *)rects[0].label, "Detection zone");

    /* other rectangles show detected objects */
    for (uint32_t i = 0; i < num_boxes && box_counter < max_boxes; i++) {
        if (!boxes[i])
            continue;

        PRINTF("Displaying box %d: label '%s' (%d%%) \r\n",
               (int)i, nanodet_labels[boxes[i]->label], (int) (boxes[i]->score * 100.0f));
        /* input tensor preview is scaled and moved to fit on screen, and so its bounding boxes */
        rects[box_counter].left = (int)((boxes[i]->left * DISPLAY_WIDTH)/ MODEL_WIDTH) + CROP_TOP;
        rects[box_counter].right = (int)((boxes[i]->right * DISPLAY_WIDTH)/ MODEL_WIDTH) + CROP_TOP;
        rects[box_counter].bottom = (int)((boxes[i]->bottom * DISPLAY_WIDTH)/MODEL_HEIGHT) + CROP_LEFT;
        rects[box_counter].top = (int)((boxes[i]->top * DISPLAY_WIDTH)/MODEL_HEIGHT) + CROP_LEFT;
        rects[box_counter].line_width = 2;
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
    int count;

    /* user_data handle contains application private data */
    user_data_t *app_priv = (user_data_t *)user_data;
    PRINTF("mpp_event_listener: START!\n");

    switch(evt) {
    case MPP_EVENT_INFERENCE_OUTPUT_READY:
        /* cast evt_data pointer to correct structure matching the event */
        inf_output = (const mpp_inference_cb_param_t *) evt_data;
        ret = NANODET_ProcessOutput(
                inf_output,
                g_finalboxes);
        if (ret != kStatus_Success)
            PRINTF("mpp_event_listener: process output error!");

        int inferenceTime = inf_output->inference_time_ms;
        PRINTF("----------------------------------------\r\n");
        PRINTF("     Inference time: %d ms \r\n", inferenceTime);
        PRINTF("----------------------------------------\r\n");

        if ( (app_priv->mp != NULL) && (app_priv->elem != 0) && (app_priv->labels != NULL) ){
            mpp_element_params_t params;
            params.labels.detected_count = 1; /* start at 1 for the detection zone */
            for(count = 0; count < NUM_BOXES_MAX; count++)
            {
                if (g_finalboxes[count] == NULL) continue;
                params.labels.detected_count++;
            }
            params.labels.max_count = MAX_LABEL_RECTS;
            params.labels.rectangles = app_priv->labels;
            boxes_to_rects(g_finalboxes, NUM_BOXES_MAX, MAX_LABEL_RECTS, params.labels.rectangles);
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
    user_data_t user_data = {0};
    int ret;

    PRINTF("[%s]\r\n", mpp_get_version());
#if defined(INFERENCE_ENGINE_TFLM)
    PRINTF("Inference Engine: TensorFlow-Lite Micro \r\n");
#elif defined (INFERENCE_ENGINE_GLOW)
    PRINTF("Inference Engine: Glow \r\n");
#else
#error "Please select inference engine"
#endif

    ret = mpp_api_init();
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

#if (SOURCE_STATIC_IMAGE == 1)
    mpp_img_params_t img_params;
    memset(&img_params, 0, sizeof (mpp_img_params_t));
    img_params.format = SRC_IMAGE_FORMAT;
    img_params.width = SRC_IMAGE_WIDTH;
    img_params.height = SRC_IMAGE_HEIGHT;
    mpp_static_img_add(mp, &img_params, image_data);
#else
    mpp_camera_params_t cam_params;
    memset(&cam_params, 0 , sizeof(cam_params));
    cam_params.height = CAMERA_HEIGHT;
    cam_params.width =  CAMERA_WIDTH;
    cam_params.format = MPP_PIXEL_YUV1P444;
    cam_params.fps    = 30;
    ret = mpp_camera_add(mp, s_camera_name, &cam_params, false);
    if (ret) {
        PRINTF("Failed to add camera %s\n", s_camera_name);
        goto err;
    }
#endif

    /* split the pipeline into 2 branches */
    mpp_t mp_split;
    mpp_exec_flag_t exec_flag = MPP_EXEC_PREEMPT;
    ret = mpp_split(mp, 1 , &exec_flag, &mp_split);
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
    nanodet_params.ml_inference.model_size = nanodet_m_weights_bin_len;
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
    elem_params.convert.pixel_format = MPP_PIXEL_RGB565;
#if (SOURCE_STATIC_IMAGE == 1)
    elem_params.convert.width = DISPLAY_WIDTH;
    elem_params.convert.height = DISPLAY_WIDTH * SRC_IMAGE_HEIGHT / SRC_IMAGE_WIDTH;
    elem_params.convert.ops = MPP_CONVERT_COLOR | MPP_CONVERT_SCALE;
#else
    /* rotation is needed because camera is landscape, display RK055 is portrait */
    elem_params.convert.angle = ROTATE_90;
    elem_params.convert.flip = FLIP_HORIZONTAL;
    elem_params.convert.ops = MPP_CONVERT_COLOR | MPP_CONVERT_ROTATE;
#endif
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
    user_data.labels[0].top = RECT_TOP_OFFSET;
    user_data.labels[0].left = 1;
    user_data.labels[0].bottom = RECT_TOP_OFFSET + DISPLAY_WIDTH;
    user_data.labels[0].right = DISPLAY_WIDTH - 1;
    user_data.labels[0].line_width = 2;
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
    disp_params.format = MPP_PIXEL_RGB565;
    ret = mpp_display_add(mp, s_display_name, &disp_params);
    if (ret) {
        PRINTF("Failed to add display %s\n", s_display_name);
        goto err;
    }

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

    /* pause application task */
    vTaskSuspend(NULL);

err:
    for (;;)
    {
        PRINTF("Error building application pipeline : ret %d\r\n", ret);
        vTaskSuspend(NULL);
    }
}

