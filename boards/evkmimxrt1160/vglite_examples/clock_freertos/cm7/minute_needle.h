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


/*path id=path951*/
static data_mnemonic_t MinuteNeedle_path_1_data[] = {
    {.cmd=VLC_OP_MOVE}, {.data=(int32_t) -0.02},{.data=(int32_t) 12.30},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 6.28},{.data=(int32_t) 12.30},{.data=(int32_t) 11.38},{.data=(int32_t) 7.20},{.data=(int32_t) 11.38},{.data=(int32_t) 0.90},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 11.38},{.data=(int32_t) -5.40},{.data=(int32_t) 6.28},{.data=(int32_t) -10.50},{.data=(int32_t) -0.02},{.data=(int32_t) -10.50},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) -6.32},{.data=(int32_t) -10.50},{.data=(int32_t) -11.42},{.data=(int32_t) -5.50},{.data=(int32_t) -11.42},{.data=(int32_t) 0.80},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) -11.42},{.data=(int32_t) 7.10},{.data=(int32_t) -6.32},{.data=(int32_t) 12.30},{.data=(int32_t) -0.02},{.data=(int32_t) 12.30},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) -0.02},{.data=(int32_t) 12.30},
    {.cmd=VLC_OP_END}
};

/*path id=path826-3-3*/
static data_mnemonic_t MinuteNeedle_path_2_data[] = {
    {.cmd=VLC_OP_MOVE}, {.data=(int32_t) -0.02},{.data=(int32_t) 150.40},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) -2.02},{.data=(int32_t) 150.40},{.data=(int32_t) -4.12},{.data=(int32_t) 148.40},{.data=(int32_t) -5.22},{.data=(int32_t) 144.30},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) -5.22},{.data=(int32_t) 11.40},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) -3.62},{.data=(int32_t) 12.20},{.data=(int32_t) -1.82},{.data=(int32_t) 12.70},{.data=(int32_t) -0.02},{.data=(int32_t) 12.70},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 1.78},{.data=(int32_t) 12.70},{.data=(int32_t) 3.58},{.data=(int32_t) 12.30},{.data=(int32_t) 5.18},{.data=(int32_t) 11.40},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 5.18},{.data=(int32_t) 144.30},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 4.18},{.data=(int32_t) 148.40},{.data=(int32_t) 2.08},{.data=(int32_t) 150.40},{.data=(int32_t) -0.02},{.data=(int32_t) 150.40},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) -0.02},{.data=(int32_t) 150.40},
    {.cmd=VLC_OP_END}
};

/*path id=path953*/
static data_mnemonic_t MinuteNeedle_path_3_data[] = {
    {.cmd=VLC_OP_MOVE}, {.data=(int32_t) 0.19},{.data=(int32_t) 5.28},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 2.79},{.data=(int32_t) 5.28},{.data=(int32_t) 4.89},{.data=(int32_t) 3.18},{.data=(int32_t) 4.89},{.data=(int32_t) 0.58},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 4.89},{.data=(int32_t) -2.02},{.data=(int32_t) 2.79},{.data=(int32_t) -4.12},{.data=(int32_t) 0.19},{.data=(int32_t) -4.12},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) -2.41},{.data=(int32_t) -4.12},{.data=(int32_t) -4.51},{.data=(int32_t) -2.02},{.data=(int32_t) -4.51},{.data=(int32_t) 0.58},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) -4.51},{.data=(int32_t) 3.08},{.data=(int32_t) -2.41},{.data=(int32_t) 5.28},{.data=(int32_t) 0.19},{.data=(int32_t) 5.28},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 0.19},{.data=(int32_t) 5.28},
    {.cmd=VLC_OP_END}
};

hybridPath_t MinuteNeedle_hybrid_path[] = {
    { .fillType = FILL_CONSTANT, .pathType = VG_LITE_DRAW_FILL_PATH },
    { .fillType = NO_FILL_MODE, .pathType = VG_LITE_DRAW_ZERO },
    { .fillType = FILL_CONSTANT, .pathType = VG_LITE_DRAW_FILL_PATH },
    { .fillType = NO_FILL_MODE, .pathType = VG_LITE_DRAW_ZERO },
    { .fillType = FILL_CONSTANT, .pathType = VG_LITE_DRAW_FILL_PATH },
    { .fillType = NO_FILL_MODE, .pathType = VG_LITE_DRAW_ZERO },

};

static vg_lite_fill_t MinuteNeedle_fill_rule[] = {
VG_LITE_FILL_EVEN_ODD,
VG_LITE_FILL_EVEN_ODD,
VG_LITE_FILL_EVEN_ODD
};

static gradient_mode_t MinuteNeedle_gradient_info = {
    .linearGrads = NULL,
    .radialGrads = NULL,
    .hybridPath = MinuteNeedle_hybrid_path,
    .fillRule = MinuteNeedle_fill_rule
};

static float MinuteNeedle_transform_matrix[] = {
1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f
};

static image_info_t MinuteNeedle = {
    .image_name ="MinuteNeedle",
    .image_size = {400, 400},
    .data_format = VG_LITE_S32,
    .transform = MinuteNeedle_transform_matrix,
    .path_count = 3,
    .stroke_info = NULL,
    .paths_info = {
        {.path_length = sizeof(MinuteNeedle_path_1_data), .path_data=(int32_t*)MinuteNeedle_path_1_data, .end_path_flag=0, .bounding_box = {0.02, 5.40, 11.42, 12.30} },
        {.path_length = sizeof(MinuteNeedle_path_2_data), .path_data=(int32_t*)MinuteNeedle_path_2_data, .end_path_flag=0, .bounding_box = {0.02, 11.40, 5.22, 150.40} },
        {.path_length = sizeof(MinuteNeedle_path_3_data), .path_data=(int32_t*)MinuteNeedle_path_3_data, .end_path_flag=0, .bounding_box = {0.19, 2.02, 4.89, 5.28} }
    },
};

uint32_t MinuteNeedle_color_data[] = {
    0xff0b0d31, 0xffe2d945, 0xff41e8df
};

