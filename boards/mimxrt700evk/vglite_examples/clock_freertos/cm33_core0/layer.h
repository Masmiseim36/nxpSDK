/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*
 * \file layer.h
 *
 * This file is intended to provide generic APIs to abstract and rendere SVG path drawing using VGLite APIs.
 */


/*******************************************************************************
 * Definitions
 ******************************************************************************/
#ifndef STATIC_PATH_DEFINES_H
#define STATIC_PATH_DEFINES_H

#include "vg_lite.h"

typedef union data_mnemonic {
    int32_t cmd;
    int32_t data;
} data_mnemonic_t;

typedef struct path_info {
    uint32_t  path_length;
    int32_t  *path_data;
    float bounding_box[4];
    uint8_t end_path_flag;
} path_info_t;

typedef struct stroke_info {
    uint32_t dashPatternCnt;
    float dashPhase;
    float *dashPattern;
    float strokeWidth;
    float miterlimit;
    uint32_t strokeColor;
    vg_lite_cap_style_t linecap;
    vg_lite_join_style_t linejoin;
} stroke_info_t;

typedef struct image_info {
    char *image_name;
    int  image_size[2];
    vg_lite_format_t data_format;
    float *transform;
    int path_count;
    stroke_info_t *stroke_info;
    path_info_t paths_info[];
} image_info_t;

typedef struct stopValue {
    float offset;
    uint32_t stop_color;
} stopValue_t;

typedef struct linearGradient {
    uint32_t num_stop_points;
    vg_lite_linear_gradient_parameter_t linear_gradient;
    stopValue_t *stops;
} linearGradient_t;

typedef struct radialGradient {
    uint32_t num_stop_points;
    vg_lite_radial_gradient_parameter_t radial_gradient;
    stopValue_t *stops;
} radialGradient_t;

typedef enum fill_mode {
    STROKE,
    FILL_CONSTANT,
    FILL_LINEAR_GRAD,
    FILL_RADIAL_GRAD,
    NO_FILL_MODE
} fill_mode_t;

typedef struct hybridPath {
    fill_mode_t fillType;
    vg_lite_draw_path_type_t pathType;
} hybridPath_t;

typedef struct gradient_mode {
    linearGradient_t **linearGrads;
    radialGradient_t **radialGrads;
    hybridPath_t *hybridPath;
    vg_lite_fill_t *fillRule;
}gradient_mode_t;

#define MAX_GRADIENT_CACHE (8)        /* Maximum number of gradient paths that can be rendered */
#define MAX_GRADIENT_STOP_POINTS (4)  /* Maximum number of stop points allowed per gradient */

typedef enum GradientCacheEntry {
	eInvalidCacheEntry = 0,
	eLinearGradientCacheEntry,
	eRadialGradientCacheEntry,
} GradientCacheEntry_t;

typedef struct gradient_cache_entry {
    void *g;
    GradientCacheEntry_t type;
    union {
        struct {
            vg_lite_linear_gradient_ext_t lGradient;
            vg_lite_linear_gradient_parameter_t params;
        } lg;
        struct {
            vg_lite_radial_gradient_t rGradient;
            vg_lite_radial_gradient_parameter_t params;
        } rg;
    } grad_data;
    /* Shared color ramp for both gradients */
    vg_lite_color_ramp_t vgColorRamp[MAX_GRADIENT_STOP_POINTS];
} gradient_cache_entry_t;

typedef struct UILayers {
	  image_info_t *img_info;
      vg_lite_path_t *handle;
      vg_lite_matrix_t *matrix;
      gradient_mode_t *mode;
      uint32_t *color;
} UILayers_t;

#define UI_LAYER_DATA(x) {&x, NULL, NULL, &x##_gradient_info, x##_color_data}

#endif

/**
 * layer_redraw()
 *
 * This API redraws given Layer into render target with user specified transform matrix
 *
 * rt [in] - Distination render buffer
 * layer [in] - layer which contains pre-initialized path commands
 * transforrm_matrix - matrix transform to apply on each path commands
 *
 * \retcode VG_LITE_SUCCESS - on successful rendering
 * \retcode VG_LITE_INVALID_ARGUMENT - if any arguments are invalid
 * \retcode VG_LITE_OUT_OF_MEMORY - if there is insufficient memory
 * \retcode -1 - For any other errror
 */
int layer_draw(vg_lite_buffer_t *rt, UILayers_t *layer, vg_lite_matrix_t *transform_matrix);

/**
 * layer_init()
 *
 * This API initializes layer internal datastrucutre based by pre-generated path commands
 * from VGLite toolkit application
 *
 * layer [in|out] - layer which contains pre-initialized path commands
 *
 * \retcode VG_LITE_SUCCESS - on successful rendering
 * \retcode VG_LITE_INVALID_ARGUMENT - if any arguments are invalid
 * \retcode VG_LITE_OUT_OF_MEMORY - if there is insufficient memory
 * \retcode -1 - For any other errror
 */
vg_lite_error_t layer_init(UILayers_t *layer);

/**
 * layer_free()
 *
 * This API frees VGLite internal memory
 *
 * layer [in|out] - layer which contains pre-initialized path commands
 *
 * \retcode VG_LITE_SUCCESS - on successful rendering
 * \retcode VG_LITE_INVALID_ARGUMENT - if any arguments are invalid
 * \retcode -1 - For any other errror
 */
vg_lite_error_t layer_free(UILayers_t *layer);

/**
 * gradient_cache_init()
 *
 * This API initializes internal gradient cache to reuse repeated gradients
 *
 * Cache memory utilization can be configured using MAX_GRADIENT_CACHE  and MAX_GRADIENT_STOP_POINTS
 * MAX_GRADIENT_CACHE defaults to 8
 * MAX_GRADIENT_STOP_POINTS defaults to 4
 *
 * \retcode VG_LITE_SUCCESS - on successful rendering
 * \retcode VG_LITE_OUT_OF_MEMORY - if there is insufficient memory
 */
vg_lite_error_t gradient_cache_init(void);

/**
 * gradient_cache_init()
 *
 * This API frees cached gradient entries
 *
 */
void gradient_cache_free(void);

/**
 * gradient_cache_find()
 *
 * This API lookups gradient cache for requested gradient. If gradient is found
 * it tries to find an unused entry. Initializes requested gradient and returns to application.
 * In case of error it sends appropriate error code.
 *
 * grad [in] - Pointer to requested gradient pointer
 * type [in] - Type of requested gradient (Linear/Radial)
 * transform_matrix [in] - tranform matrix that is used in current path
 * ppcachedEntry [out] - Pointer will be not-NULL in case of success
 *
 * \retcode VG_LITE_SUCCESS - in case of success
 * \retcode VG_LITE_OUT_OF_MEMORY - if gradient allocation fails.
 */
vg_lite_error_t gradient_cache_find(void *grad, GradientCacheEntry_t type,
    vg_lite_matrix_t *transform_matrix, gradient_cache_entry_t **ppcachedEntry);

