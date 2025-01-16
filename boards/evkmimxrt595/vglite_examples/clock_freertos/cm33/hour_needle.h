/*
 * Copyright 2019, 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "layer.h"

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

#endif


/*path id=path826-3*/
static data_mnemonic_t HourNeedle_path_1_data[] = {
    {.cmd=VLC_OP_MOVE}, {.data=(int32_t) -0.26},{.data=(int32_t) 115.57},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) -2.36},{.data=(int32_t) 115.57},{.data=(int32_t) -4.36},{.data=(int32_t) 113.57},{.data=(int32_t) -5.46},{.data=(int32_t) 109.47},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) -5.46},{.data=(int32_t) 10.87},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) -3.86},{.data=(int32_t) 11.67},{.data=(int32_t) -1.96},{.data=(int32_t) 12.17},{.data=(int32_t) -0.26},{.data=(int32_t) 12.17},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 1.64},{.data=(int32_t) 12.17},{.data=(int32_t) 3.34},{.data=(int32_t) 11.77},{.data=(int32_t) 4.94},{.data=(int32_t) 10.87},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 4.94},{.data=(int32_t) 109.47},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 3.84},{.data=(int32_t) 113.57},{.data=(int32_t) 1.74},{.data=(int32_t) 115.57},{.data=(int32_t) -0.26},{.data=(int32_t) 115.57},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) -0.26},{.data=(int32_t) 115.57},
    {.cmd=VLC_OP_END}
};

/*path id=path848*/
static data_mnemonic_t HourNeedle_path_2_data[] = {
    {.cmd=VLC_OP_MOVE}, {.data=(int32_t) -0.26},{.data=(int32_t) 11.77},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 5.94},{.data=(int32_t) 11.77},{.data=(int32_t) 11.14},{.data=(int32_t) 6.67},{.data=(int32_t) 11.14},{.data=(int32_t) 0.27},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 11.14},{.data=(int32_t) -6.03},{.data=(int32_t) 6.14},{.data=(int32_t) -11.23},{.data=(int32_t) -0.26},{.data=(int32_t) -11.23},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) -6.66},{.data=(int32_t) -11.23},{.data=(int32_t) -11.66},{.data=(int32_t) -6.03},{.data=(int32_t) -11.66},{.data=(int32_t) 0.27},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) -11.66},{.data=(int32_t) 6.57},{.data=(int32_t) -6.66},{.data=(int32_t) 11.77},{.data=(int32_t) -0.26},{.data=(int32_t) 11.77},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) -0.26},{.data=(int32_t) 11.77},
    {.cmd=VLC_OP_END}
};

/*path id=path851*/
static data_mnemonic_t HourNeedle_path_3_data[] = {
    {.cmd=VLC_OP_MOVE}, {.data=(int32_t) -0.26},{.data=(int32_t) 4.77},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 2.44},{.data=(int32_t) 4.77},{.data=(int32_t) 4.54},{.data=(int32_t) 2.67},{.data=(int32_t) 4.54},{.data=(int32_t) -0.03},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 4.54},{.data=(int32_t) -2.73},{.data=(int32_t) 2.24},{.data=(int32_t) -4.73},{.data=(int32_t) -0.26},{.data=(int32_t) -4.73},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) -2.76},{.data=(int32_t) -4.73},{.data=(int32_t) -5.06},{.data=(int32_t) -2.73},{.data=(int32_t) -5.06},{.data=(int32_t) -0.03},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) -5.06},{.data=(int32_t) 2.67},{.data=(int32_t) -2.96},{.data=(int32_t) 4.77},{.data=(int32_t) -0.26},{.data=(int32_t) 4.77},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) -0.26},{.data=(int32_t) 4.77},
    {.cmd=VLC_OP_END}
};

hybridPath_t HourNeedle_hybrid_path[] = {
    { .fillType = FILL_CONSTANT, .pathType = VG_LITE_DRAW_FILL_PATH },
    { .fillType = NO_FILL_MODE, .pathType = VG_LITE_DRAW_ZERO },
    { .fillType = FILL_CONSTANT, .pathType = VG_LITE_DRAW_FILL_PATH },
    { .fillType = NO_FILL_MODE, .pathType = VG_LITE_DRAW_ZERO },
    { .fillType = FILL_CONSTANT, .pathType = VG_LITE_DRAW_FILL_PATH },
    { .fillType = NO_FILL_MODE, .pathType = VG_LITE_DRAW_ZERO },

};

static vg_lite_fill_t HourNeedle_fill_rule[] = {
VG_LITE_FILL_EVEN_ODD,
VG_LITE_FILL_EVEN_ODD,
VG_LITE_FILL_EVEN_ODD
};

static gradient_mode_t HourNeedle_gradient_info = {
    .linearGrads = NULL,
    .radialGrads = NULL,
    .hybridPath = HourNeedle_hybrid_path,
    .fillRule = HourNeedle_fill_rule
};

static float HourNeedle_transform_matrix[] = {
1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f
};

static image_info_t HourNeedle = {
    .image_name ="HourNeedle",
    .image_size = {400, 400},
    .data_format = VG_LITE_S32,
    .transform = HourNeedle_transform_matrix,
    .path_count = 3,
    .stroke_info = NULL,
    .paths_info = {
        {.path_length = sizeof(HourNeedle_path_1_data), .path_data=(int32_t*)HourNeedle_path_1_data, .end_path_flag=0, .bounding_box = {0.26, 10.87, 5.46, 115.57} },
        {.path_length = sizeof(HourNeedle_path_2_data), .path_data=(int32_t*)HourNeedle_path_2_data, .end_path_flag=0, .bounding_box = {0.26, 6.03, 11.66, 11.77} },
        {.path_length = sizeof(HourNeedle_path_3_data), .path_data=(int32_t*)HourNeedle_path_3_data, .end_path_flag=0, .bounding_box = {0.26, 2.67, 5.06, 4.77} }
    },
};

uint32_t HourNeedle_color_data[] = {
    0xff8a3523, 0xff0b0d31, 0xff41e8df
};

