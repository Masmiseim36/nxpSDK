/*
 * Copyright 2021-2022 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * API types public header
 */
#ifndef _MPP_API_TYPES_H
#define _MPP_API_TYPES_H


#include <stdbool.h>
#include <stdint.h>

/**
 * @defgroup MPP_TYPES MPP Types
 *
 * This section provides the detailed documentation for the MCU Media Processing Pipeline types
 *
 * @{
 */

/** Pipeline handle type */
typedef void* mpp_t ;
/** Element handle type */
typedef uintptr_t mpp_elem_handle_t ;
/**Invalid pipeline handle */
#define MPP_INVALID (NULL)

/** Pipeline generated events */
typedef enum {
    MPP_EVENT_INVALID,  /*!< invalid event */
    MPP_EVENT_INFERENCE_OUTPUT_READY,   /*!< inference out is ready */
    MPP_EVENT_NUM   /*!< DO NOT USE */
} mpp_evt_t;

/** Event mask for pipeline creation */
typedef unsigned int mpp_evt_mask_t;
#define MPP_EVENT_ALL 0xffffffff

/**
 * Execution parameters
 *
 * These parameters control the execution of the elements of an mpp. <br>
 * The "mpps" created using the flag MPP_EXEC_RC are  guaranteed to run up to
 * the completion of all processing elements, while not being preempted by other "mpps". <br>
 * The "mpps" created using the flag MPP_EXEC_PREEMPT are preempted after a given
 * time interval by "mpps" that will run-to-completion again. <br>
 * The "mpps" created with the MPP_EXEC_INHERIT flag inherit the same execution flag
 * as the parent(s) in case of split/join operation. <br>
 * Note: It is not possible to request run-to-completion execution when spliting/joining preemptable-execution "mpps".
 */
typedef enum {
    MPP_EXEC_INHERIT = 0,  /*!< inherit from parent(s) */
    MPP_EXEC_RC,           /*!< run-to-completion */
    MPP_EXEC_PREEMPT       /*!< preemptable */
} mpp_exec_flag_t;

/** Pipeline creation parameters */
typedef struct {
        int (*evt_callback_f)(mpp_t mpp, mpp_evt_t evt, void *evt_data, void *user_data);
        mpp_evt_mask_t mask ;
        mpp_exec_flag_t exec_flag;
        void *cb_userdata;
} mpp_params_t;

/** Rotation value */
typedef enum {
    ROTATE_0 = 0,   /*!< 0 degree */
    ROTATE_90,      /*!< 90 degrees */
    ROTATE_180,     /*!< 180 degrees */
    ROTATE_270      /*!< 270 degrees */
} mpp_rotate_degree_t;

/** Flip type */
typedef enum {
    FLIP_NONE = 0,      /*!< no flip */
    FLIP_HORIZONTAL,    /*!< horizontal flip */
    FLIP_VERTICAL,      /*!< vertical flip */
    FLIP_BOTH           /*!< vertical and horizontal flip */
} mpp_flip_mode_t;

/** The convert operations selector flags */
typedef enum {
    MPP_CONVERT_NONE = 0,           /*!< no frame conversion */
    MPP_CONVERT_ROTATE = 1,         /*!< frame rotation */
    MPP_CONVERT_SCALE = (1 << 1),   /*!< frame scaling */
    MPP_CONVERT_COLOR = (1 << 2),   /*!< frame color conversion */
    MPP_CONVERT_CROP = (1 << 3),    /*!< frame crop */
} mpp_convert_ops_t;

/** Pixel format */
typedef enum {
    /* 2d frame format */
    MPP_PIXEL_ARGB,         /*!< ARGB 32 bits */
    MPP_PIXEL_RGB,          /*!< RGB 24 bits */
    MPP_PIXEL_RGB565,       /*!< RGB 16 bits */
    MPP_PIXEL_BGR,          /*!< BGR 24 bits */
    MPP_PIXEL_GRAY888,      /*!< gray 3x8 bits */
    MPP_PIXEL_GRAY888X,     /*!< gray 3x8 bits +8 unused bits */
    MPP_PIXEL_GRAY,         /*!< gray 8 bits */
    MPP_PIXEL_GRAY16,       /*!< gray 16 bits */
    MPP_PIXEL_YUV1P444,     /*!< YUVX interleaved 4:4:4 */
    MPP_PIXEL_VYUY1P422,    /*!< VYUY interleaved 4:2:2 */
    MPP_PIXEL_UYVY1P422,    /*!< UYVY interleaved 4:2:2 */
    MPP_PIXEL_YUYV,         /*!< YUYV interleaved 4:2:2 */

    /* 3d frame format */
    MPP_PIXEL_DEPTH16,      /*!< depth 16 bits */
    MPP_PIXEL_DEPTH8,       /*!< depth 8 bits */

    MPP_PIXEL_YUV420P,      /*!< YUV planar 4:2:0 */

    MPP_PIXEL_INVALID       /*!< invalid pixel format */
} mpp_pixel_format_t;

/** Camera parameters */
typedef struct {
    int height; /*!< buffer height */
    int width;  /*!< buffer width */
    mpp_pixel_format_t format; /*!< pixel format */
    int fps;    /*!< frames per second */
} mpp_camera_params_t;

/** Static image parameters */
typedef struct {
    int height;
    int width;
    mpp_pixel_format_t format;
} mpp_img_params_t;

/** Display parameters */
typedef struct {
    int height; /*!< buffer resolution: setting to 0 will default to panel physical resolution */
    int width;  /*!< buffer resolution: setting to 0 will default to panel physical resolution */
    int pitch;  /*!< buffer resolution: setting to 0 will default to panel physical resolution */
    int left;   /*!< active rect: setting to 0 will default to fullscreen */
    int top;    /*!< active rect: setting to 0 will default to fullscreen */
    int right;  /*!< active rect: setting to 0 will default to fullscreen */
    int bottom; /*!< active rect: setting to 0 will default to fullscreen */
    mpp_rotate_degree_t rotate; /*!< rotate degree */
    mpp_pixel_format_t format;  /*!< pixel format */
} mpp_display_params_t;

/** Processing element ids */
typedef enum {
    MPP_ELEMENT_INVALID,    /*!< Invalid element */
    MPP_ELEMENT_COMPOSE,    /*!< Image composition - NOT IMPLEMENTED YET */
    MPP_ELEMENT_LABELED_RECTANGLE, /*!< Labeled rectangle - bounding box */
    MPP_ELEMENT_TEST,       /*!< Test inplace element - NOT FOR USE */
    MPP_ELEMENT_INFERENCE,  /*!< Inference engine */
    MPP_ELEMENT_CONVERT,    /*!< Image conversion: resolution, orientation, color format */
    MPP_ELEMENT_NUM         /*!< DO NOT USE */
} mpp_element_id_t;

/* Inference parameter types */
#define MAX_TENSOR_DIMS 4

/** Inference tensor dimensions */
typedef struct
{
  uint32_t size;
  uint32_t data[MAX_TENSOR_DIMS];
} mpp_tensor_dims_t;

/** Inference tensor type */
typedef enum {
    MPP_TENSOR_TYPE_FLOAT32 = 0,/*!< floating point 32 bits */
    MPP_TENSOR_TYPE_UINT8 = 1,  /*!< unsigned integer 8 bits */
    MPP_TENSOR_TYPE_INT8 = 2    /*!< signed integer 8 bits */
} mpp_tensor_type_t;

/** Inference type */
typedef enum {
    MPP_INFERENCE_TYPE_TFLITE = 0,      /*!< TensorFlow-Lite */
    MPP_INFERENCE_TYPE_DEEPVIEWRT = 1   /*!< DeepViewRT */
} mpp_inference_type_t;

/** Inference callback parameters */
typedef struct {
    void *user_data;        /*!< callback will pass this pointer */
    const uint8_t* tensor;  /*!< output tensor data */
    mpp_tensor_dims_t dims; /*!< tensor data dimensions */
    mpp_tensor_type_t type; /*!< tensor data type */
    int inference_time_ms;  /*!< inference run time measurement - output to user */
} mpp_inference_cb_param_t;

/** mpp color encoding */
typedef union {
    uint32_t raw;   /*!< Raw color */
    /** rgb color values */
    struct {
        uint8_t R;      /*!< Red byte */
        uint8_t G;      /*!< Green byte */
        uint8_t B;      /*!< Blue byte */
        uint8_t pad;    /*!< padding byte */
    } rgb;  /*!< RGB color */
} mpp_color_t;

/** mpp labeled rectangle element structure */
typedef struct {
    uint8_t label[64];      /*!< label to print */
    uint16_t clear;         /*!< clear rectangle */
    uint16_t line_width;    /*!< rectangle line thickness */
    mpp_color_t line_color; /*!< rectangle line color */
    uint16_t top;           /*!< rectangle top position */
    uint16_t left;          /*!< rectangle left position */
    uint16_t bottom;        /*!< rectangle bottom position */
    uint16_t right;         /*!< rectangle right position */
    uint16_t tag;           /*!< labeled rectangle tag */
    uint16_t reserved;      /*!< pad for 32 bits alignment */
} mpp_labeled_rect_t;

/** Image area coordinates */
typedef struct {
    int top;
    int left;
    int bottom;
    int right;
} mpp_area_t;

/** Processing element parameters */
typedef union {
    /** Compose element's parameters - NOT IMPLEMENTED YET */
    struct {
        float a;
        float b;
    } compose;
    /** Labeled Rectangle element's parameters */
    struct {
        uint32_t max_count;                 /*!< maximum number of rectangles */
        uint32_t detected_count;            /*!< detected rectangles */
        mpp_labeled_rect_t *rectangles;     /*!< array of rectangle data */
    } labels;
    /** Convert element's parameters */
    struct {
        unsigned int width;                 /*!< output image width */
        unsigned int height;                /*!< output image height */
        mpp_pixel_format_t pixel_format;    /*!< new pixel format */
        mpp_rotate_degree_t angle;          /*!< rotation angle */
        mpp_area_t crop;                    /*!< input crop area */
        mpp_area_t out_area;                /*!< output window area */
        mpp_convert_ops_t ops;              /*!< operation selector mask */
    } convert;
    /** Resize element's parameters */
    struct {
    	unsigned int width;
    	unsigned int height;
    } resize;
    /** Color convert element's parameters */
    struct {
    	mpp_pixel_format_t pixel_format;
    } color_conv;
    /** Rotate element's parameters */
    struct {
        mpp_rotate_degree_t angle;
    } rotate;
    /** Test element's parameters */
    struct {
        _Bool inp;
        /* output parameters */
        unsigned int width;
        unsigned int height;
        mpp_pixel_format_t format;
    } test;
    /** ML inference element's parameters */
    struct {
        const void *model_data; /*!< pointer to model binary */
        mpp_inference_type_t type; /*!< inference type */
        int model_size;         /*!< model binary size */
        float model_input_mean; /*!< model 'mean' of input values, used for normalization */
        float model_input_std;  /*!< model 'standard deviation' of input values, used for normalization */
    } ml_inference;

} mpp_element_params_t;

/** @}*/

/** @defgroup return_codes
 *  MPP APIs return status definitions
 * @{
 */
#define MPP_SUCCESS         (0)     /*!< Success return code */
#define MPP_ERROR           (10)    /*!< A generic error occured */
#define MPP_INVALID_ELEM    (20)    /*!< Invalid element provided */
#define MPP_INVALID_PARAM   (30)    /*!< Invalid parameter provided */
#define MPP_ERR_ALLOC_MUTEX (40)    /*!< Error occured while allocating mutex */
#define MPP_INVALID_MUTEX   (41)    /*!< Invalid mutex provided */
#define MPP_MUTEX_TIMEOUT   (42)    /*!< Mutex timeout occured */
#define MPP_MUTEX_ERROR     (43)    /*!< Mutex error occured */
#define MPP_MALLOC_ERROR    (50)    /*!< Memory allocation error occured */

/** @} */

#endif
