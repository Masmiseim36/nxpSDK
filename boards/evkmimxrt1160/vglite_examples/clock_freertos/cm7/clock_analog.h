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


/*path id=path817*/
static data_mnemonic_t ClockDial_path_1_data[] = {
    {.cmd=VLC_OP_MOVE}, {.data=(int32_t) 200.00},{.data=(int32_t) 0.00},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 310.50},{.data=(int32_t) 0.00},{.data=(int32_t) 400.00},{.data=(int32_t) 89.50},{.data=(int32_t) 400.00},{.data=(int32_t) 200.00},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 400.00},{.data=(int32_t) 310.50},{.data=(int32_t) 310.50},{.data=(int32_t) 400.00},{.data=(int32_t) 200.00},{.data=(int32_t) 400.00},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 89.50},{.data=(int32_t) 400.00},{.data=(int32_t) 0.00},{.data=(int32_t) 310.50},{.data=(int32_t) 0.00},{.data=(int32_t) 200.00},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 0.00},{.data=(int32_t) 89.50},{.data=(int32_t) 89.50},{.data=(int32_t) 0.00},{.data=(int32_t) 200.00},{.data=(int32_t) 0.00},
    {.cmd=VLC_OP_END}
};

/*path id=path819*/
static data_mnemonic_t ClockDial_path_2_data[] = {
    {.cmd=VLC_OP_MOVE}, {.data=(int32_t) 200.00},{.data=(int32_t) 56.30},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 202.60},{.data=(int32_t) 56.30},{.data=(int32_t) 204.70},{.data=(int32_t) 58.40},{.data=(int32_t) 204.70},{.data=(int32_t) 61.00},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 204.70},{.data=(int32_t) 63.60},{.data=(int32_t) 202.60},{.data=(int32_t) 65.70},{.data=(int32_t) 200.00},{.data=(int32_t) 65.70},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 197.40},{.data=(int32_t) 65.70},{.data=(int32_t) 195.30},{.data=(int32_t) 63.60},{.data=(int32_t) 195.30},{.data=(int32_t) 61.00},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 195.30},{.data=(int32_t) 58.40},{.data=(int32_t) 197.40},{.data=(int32_t) 56.30},{.data=(int32_t) 200.00},{.data=(int32_t) 56.30},
    {.cmd=VLC_OP_END}
};

/*path id=path860-2*/
static data_mnemonic_t ClockDial_path_3_data[] = {
    {.cmd=VLC_OP_MOVE}, {.data=(int32_t) 278.50},{.data=(int32_t) 348.20},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 284.20},{.data=(int32_t) 345.20},{.data=(int32_t) 289.40},{.data=(int32_t) 340.10},{.data=(int32_t) 292.40},{.data=(int32_t) 334.50},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 294.50},{.data=(int32_t) 330.50},{.data=(int32_t) 294.90},{.data=(int32_t) 328.70},{.data=(int32_t) 294.90},{.data=(int32_t) 321.90},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 294.90},{.data=(int32_t) 315.10},{.data=(int32_t) 294.50},{.data=(int32_t) 313.30},{.data=(int32_t) 292.40},{.data=(int32_t) 309.30},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 289.40},{.data=(int32_t) 303.70},{.data=(int32_t) 284.30},{.data=(int32_t) 298.60},{.data=(int32_t) 278.50},{.data=(int32_t) 295.60},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 274.30},{.data=(int32_t) 293.40},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 200.00},{.data=(int32_t) 293.40},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 125.70},{.data=(int32_t) 293.40},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 121.50},{.data=(int32_t) 295.60},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 115.80},{.data=(int32_t) 298.60},{.data=(int32_t) 110.60},{.data=(int32_t) 303.70},{.data=(int32_t) 107.60},{.data=(int32_t) 309.30},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 105.50},{.data=(int32_t) 313.30},{.data=(int32_t) 105.10},{.data=(int32_t) 315.10},{.data=(int32_t) 105.10},{.data=(int32_t) 321.90},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 105.10},{.data=(int32_t) 328.40},{.data=(int32_t) 105.50},{.data=(int32_t) 330.60},{.data=(int32_t) 107.30},{.data=(int32_t) 334.00},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 111.20},{.data=(int32_t) 341.60},{.data=(int32_t) 118.30},{.data=(int32_t) 347.90},{.data=(int32_t) 126.10},{.data=(int32_t) 350.40},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 127.40},{.data=(int32_t) 350.80},{.data=(int32_t) 161.20},{.data=(int32_t) 350.50},{.data=(int32_t) 201.30},{.data=(int32_t) 350.40},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 274.20},{.data=(int32_t) 350.40},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 278.50},{.data=(int32_t) 348.20},
    {.cmd=VLC_OP_MOVE}, {.data=(int32_t) 127.10},{.data=(int32_t) 348.50},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 119.30},{.data=(int32_t) 346.10},{.data=(int32_t) 113.10},{.data=(int32_t) 340.40},{.data=(int32_t) 109.20},{.data=(int32_t) 333.10},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 106.00},{.data=(int32_t) 326.90},{.data=(int32_t) 106.10},{.data=(int32_t) 316.60},{.data=(int32_t) 109.50},{.data=(int32_t) 310.20},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 112.40},{.data=(int32_t) 304.60},{.data=(int32_t) 116.70},{.data=(int32_t) 300.50},{.data=(int32_t) 122.40},{.data=(int32_t) 297.50},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 126.60},{.data=(int32_t) 295.30},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 200.00},{.data=(int32_t) 295.30},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 273.40},{.data=(int32_t) 295.30},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 277.60},{.data=(int32_t) 297.50},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 283.30},{.data=(int32_t) 300.50},{.data=(int32_t) 287.60},{.data=(int32_t) 304.70},{.data=(int32_t) 290.50},{.data=(int32_t) 310.20},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 292.50},{.data=(int32_t) 314.00},{.data=(int32_t) 293.00},{.data=(int32_t) 316.10},{.data=(int32_t) 293.00},{.data=(int32_t) 321.90},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 293.00},{.data=(int32_t) 327.70},{.data=(int32_t) 292.60},{.data=(int32_t) 329.70},{.data=(int32_t) 290.50},{.data=(int32_t) 333.60},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 287.60},{.data=(int32_t) 339.20},{.data=(int32_t) 283.30},{.data=(int32_t) 343.30},{.data=(int32_t) 277.60},{.data=(int32_t) 346.30},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 273.40},{.data=(int32_t) 348.50},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 201.40},{.data=(int32_t) 348.50},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 161.80},{.data=(int32_t) 348.60},{.data=(int32_t) 128.40},{.data=(int32_t) 348.90},{.data=(int32_t) 127.10},{.data=(int32_t) 348.50},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 127.10},{.data=(int32_t) 348.50},
    {.cmd=VLC_OP_END}
};

/*path id=path6626*/
static data_mnemonic_t ClockDial_path_4_data[] = {
    {.cmd=VLC_OP_MOVE}, {.data=(int32_t) 167.50},{.data=(int32_t) 313.00},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 167.50},{.data=(int32_t) 319.30},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 168.10},{.data=(int32_t) 318.70},{.data=(int32_t) 168.70},{.data=(int32_t) 318.30},{.data=(int32_t) 169.40},{.data=(int32_t) 318.00},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 170.10},{.data=(int32_t) 317.70},{.data=(int32_t) 170.80},{.data=(int32_t) 317.60},{.data=(int32_t) 171.60},{.data=(int32_t) 317.60},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 173.30},{.data=(int32_t) 317.60},{.data=(int32_t) 174.70},{.data=(int32_t) 318.20},{.data=(int32_t) 175.80},{.data=(int32_t) 319.50},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 177.00},{.data=(int32_t) 320.80},{.data=(int32_t) 177.50},{.data=(int32_t) 322.40},{.data=(int32_t) 177.50},{.data=(int32_t) 324.30},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 177.50},{.data=(int32_t) 326.20},{.data=(int32_t) 176.90},{.data=(int32_t) 327.80},{.data=(int32_t) 175.70},{.data=(int32_t) 329.10},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 174.50},{.data=(int32_t) 330.40},{.data=(int32_t) 173.10},{.data=(int32_t) 331.00},{.data=(int32_t) 171.40},{.data=(int32_t) 331.00},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 170.70},{.data=(int32_t) 331.00},{.data=(int32_t) 170.00},{.data=(int32_t) 330.90},{.data=(int32_t) 169.40},{.data=(int32_t) 330.60},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 168.80},{.data=(int32_t) 330.30},{.data=(int32_t) 168.10},{.data=(int32_t) 329.90},{.data=(int32_t) 167.50},{.data=(int32_t) 329.30},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 167.50},{.data=(int32_t) 330.60},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 164.30},{.data=(int32_t) 330.60},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 164.30},{.data=(int32_t) 313.00},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 167.50},{.data=(int32_t) 313.00},
    {.cmd=VLC_OP_MOVE}, {.data=(int32_t) 170.90},{.data=(int32_t) 320.50},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 169.90},{.data=(int32_t) 320.50},{.data=(int32_t) 169.10},{.data=(int32_t) 320.80},{.data=(int32_t) 168.40},{.data=(int32_t) 321.50},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 167.70},{.data=(int32_t) 322.20},{.data=(int32_t) 167.40},{.data=(int32_t) 323.10},{.data=(int32_t) 167.40},{.data=(int32_t) 324.20},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 167.40},{.data=(int32_t) 325.30},{.data=(int32_t) 167.70},{.data=(int32_t) 326.20},{.data=(int32_t) 168.40},{.data=(int32_t) 326.90},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 169.10},{.data=(int32_t) 327.60},{.data=(int32_t) 169.90},{.data=(int32_t) 327.90},{.data=(int32_t) 170.90},{.data=(int32_t) 327.90},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 171.90},{.data=(int32_t) 327.90},{.data=(int32_t) 172.70},{.data=(int32_t) 327.50},{.data=(int32_t) 173.40},{.data=(int32_t) 326.80},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 174.10},{.data=(int32_t) 326.10},{.data=(int32_t) 174.40},{.data=(int32_t) 325.20},{.data=(int32_t) 174.40},{.data=(int32_t) 324.10},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 174.40},{.data=(int32_t) 323.00},{.data=(int32_t) 174.10},{.data=(int32_t) 322.10},{.data=(int32_t) 173.40},{.data=(int32_t) 321.40},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 172.70},{.data=(int32_t) 320.90},{.data=(int32_t) 171.90},{.data=(int32_t) 320.50},{.data=(int32_t) 170.90},{.data=(int32_t) 320.50},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 170.90},{.data=(int32_t) 320.50},
    {.cmd=VLC_OP_END}
};

/*path id=path6628*/
static data_mnemonic_t ClockDial_path_5_data[] = {
    {.cmd=VLC_OP_MOVE}, {.data=(int32_t) 180.20},{.data=(int32_t) 317.90},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 183.40},{.data=(int32_t) 317.90},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 183.40},{.data=(int32_t) 324.00},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 183.40},{.data=(int32_t) 325.20},{.data=(int32_t) 183.50},{.data=(int32_t) 326.00},{.data=(int32_t) 183.60},{.data=(int32_t) 326.50},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 183.80},{.data=(int32_t) 327.00},{.data=(int32_t) 184.00},{.data=(int32_t) 327.30},{.data=(int32_t) 184.40},{.data=(int32_t) 327.60},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 184.80},{.data=(int32_t) 327.90},{.data=(int32_t) 185.20},{.data=(int32_t) 328.00},{.data=(int32_t) 185.70},{.data=(int32_t) 328.00},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 186.20},{.data=(int32_t) 328.00},{.data=(int32_t) 186.70},{.data=(int32_t) 327.90},{.data=(int32_t) 187.00},{.data=(int32_t) 327.60},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 187.40},{.data=(int32_t) 327.30},{.data=(int32_t) 187.70},{.data=(int32_t) 327.00},{.data=(int32_t) 187.80},{.data=(int32_t) 326.50},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 187.90},{.data=(int32_t) 326.10},{.data=(int32_t) 188.00},{.data=(int32_t) 325.30},{.data=(int32_t) 188.00},{.data=(int32_t) 324.10},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 188.00},{.data=(int32_t) 317.90},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 191.20},{.data=(int32_t) 317.90},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 191.20},{.data=(int32_t) 323.30},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 191.20},{.data=(int32_t) 325.50},{.data=(int32_t) 191.00},{.data=(int32_t) 327.00},{.data=(int32_t) 190.70},{.data=(int32_t) 327.90},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 190.30},{.data=(int32_t) 328.90},{.data=(int32_t) 189.60},{.data=(int32_t) 329.70},{.data=(int32_t) 188.80},{.data=(int32_t) 330.20},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 188.00},{.data=(int32_t) 330.70},{.data=(int32_t) 186.90},{.data=(int32_t) 331.00},{.data=(int32_t) 185.60},{.data=(int32_t) 331.00},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 184.20},{.data=(int32_t) 331.00},{.data=(int32_t) 183.10},{.data=(int32_t) 330.70},{.data=(int32_t) 182.20},{.data=(int32_t) 330.10},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 181.30},{.data=(int32_t) 329.50},{.data=(int32_t) 180.70},{.data=(int32_t) 328.60},{.data=(int32_t) 180.40},{.data=(int32_t) 327.50},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 180.20},{.data=(int32_t) 326.70},{.data=(int32_t) 180.00},{.data=(int32_t) 325.30},{.data=(int32_t) 180.00},{.data=(int32_t) 323.30},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 180.20},{.data=(int32_t) 317.90},
    {.cmd=VLC_OP_END}
};

/*path id=path6630*/
static data_mnemonic_t ClockDial_path_6_data[] = {
    {.cmd=VLC_OP_MOVE}, {.data=(int32_t) 195.00},{.data=(int32_t) 313.20},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 198.20},{.data=(int32_t) 313.20},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 198.20},{.data=(int32_t) 317.90},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 200.10},{.data=(int32_t) 317.90},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 200.10},{.data=(int32_t) 320.60},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 198.20},{.data=(int32_t) 320.60},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 198.20},{.data=(int32_t) 330.60},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 195.00},{.data=(int32_t) 330.60},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 195.00},{.data=(int32_t) 320.60},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 193.40},{.data=(int32_t) 320.60},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 193.40},{.data=(int32_t) 317.90},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 195.00},{.data=(int32_t) 317.90},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 195.00},{.data=(int32_t) 313.20},
    {.cmd=VLC_OP_END}
};

/*path id=path6632*/
static data_mnemonic_t ClockDial_path_7_data[] = {
    {.cmd=VLC_OP_MOVE}, {.data=(int32_t) 202.20},{.data=(int32_t) 313.20},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 205.40},{.data=(int32_t) 313.20},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 205.40},{.data=(int32_t) 317.90},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 207.30},{.data=(int32_t) 317.90},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 207.30},{.data=(int32_t) 320.60},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 205.40},{.data=(int32_t) 320.60},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 205.40},{.data=(int32_t) 330.60},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 202.20},{.data=(int32_t) 330.60},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 202.20},{.data=(int32_t) 320.60},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 200.60},{.data=(int32_t) 320.60},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 200.60},{.data=(int32_t) 317.90},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 202.20},{.data=(int32_t) 317.90},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 202.20},{.data=(int32_t) 313.20},
    {.cmd=VLC_OP_END}
};

/*path id=path6634*/
static data_mnemonic_t ClockDial_path_8_data[] = {
    {.cmd=VLC_OP_MOVE}, {.data=(int32_t) 215.00},{.data=(int32_t) 317.60},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 216.20},{.data=(int32_t) 317.60},{.data=(int32_t) 217.30},{.data=(int32_t) 317.90},{.data=(int32_t) 218.40},{.data=(int32_t) 318.50},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 219.50},{.data=(int32_t) 319.10},{.data=(int32_t) 220.30},{.data=(int32_t) 319.90},{.data=(int32_t) 220.90},{.data=(int32_t) 320.90},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 221.50},{.data=(int32_t) 321.90},{.data=(int32_t) 221.80},{.data=(int32_t) 323.00},{.data=(int32_t) 221.80},{.data=(int32_t) 324.20},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 221.80},{.data=(int32_t) 325.40},{.data=(int32_t) 221.50},{.data=(int32_t) 326.50},{.data=(int32_t) 220.90},{.data=(int32_t) 327.60},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 220.30},{.data=(int32_t) 328.60},{.data=(int32_t) 219.50},{.data=(int32_t) 329.50},{.data=(int32_t) 218.50},{.data=(int32_t) 330.10},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 217.50},{.data=(int32_t) 330.70},{.data=(int32_t) 216.30},{.data=(int32_t) 331.00},{.data=(int32_t) 215.10},{.data=(int32_t) 331.00},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 213.30},{.data=(int32_t) 331.00},{.data=(int32_t) 211.70},{.data=(int32_t) 330.40},{.data=(int32_t) 210.40},{.data=(int32_t) 329.10},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 209.10},{.data=(int32_t) 327.80},{.data=(int32_t) 208.50},{.data=(int32_t) 326.20},{.data=(int32_t) 208.50},{.data=(int32_t) 324.40},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 208.50},{.data=(int32_t) 322.40},{.data=(int32_t) 209.20},{.data=(int32_t) 320.80},{.data=(int32_t) 210.70},{.data=(int32_t) 319.40},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 211.80},{.data=(int32_t) 318.20},{.data=(int32_t) 213.30},{.data=(int32_t) 317.60},{.data=(int32_t) 215.00},{.data=(int32_t) 317.60},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 215.00},{.data=(int32_t) 317.60},
    {.cmd=VLC_OP_MOVE}, {.data=(int32_t) 215.00},{.data=(int32_t) 320.60},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 214.00},{.data=(int32_t) 320.60},{.data=(int32_t) 213.20},{.data=(int32_t) 320.90},{.data=(int32_t) 212.50},{.data=(int32_t) 321.60},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 211.80},{.data=(int32_t) 322.30},{.data=(int32_t) 211.50},{.data=(int32_t) 323.20},{.data=(int32_t) 211.50},{.data=(int32_t) 324.20},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 211.50},{.data=(int32_t) 325.30},{.data=(int32_t) 211.80},{.data=(int32_t) 326.20},{.data=(int32_t) 212.50},{.data=(int32_t) 326.90},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 213.20},{.data=(int32_t) 327.60},{.data=(int32_t) 214.00},{.data=(int32_t) 327.90},{.data=(int32_t) 215.00},{.data=(int32_t) 327.90},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 216.00},{.data=(int32_t) 327.90},{.data=(int32_t) 216.80},{.data=(int32_t) 327.60},{.data=(int32_t) 217.50},{.data=(int32_t) 326.90},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 218.20},{.data=(int32_t) 326.20},{.data=(int32_t) 218.50},{.data=(int32_t) 325.30},{.data=(int32_t) 218.50},{.data=(int32_t) 324.20},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 218.50},{.data=(int32_t) 323.10},{.data=(int32_t) 218.20},{.data=(int32_t) 322.20},{.data=(int32_t) 217.50},{.data=(int32_t) 321.50},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 216.90},{.data=(int32_t) 321.00},{.data=(int32_t) 216.00},{.data=(int32_t) 320.60},{.data=(int32_t) 215.00},{.data=(int32_t) 320.60},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 215.00},{.data=(int32_t) 320.60},
    {.cmd=VLC_OP_END}
};

/*path id=path6636*/
static data_mnemonic_t ClockDial_path_9_data[] = {
    {.cmd=VLC_OP_MOVE}, {.data=(int32_t) 224.30},{.data=(int32_t) 317.90},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 227.50},{.data=(int32_t) 317.90},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 227.50},{.data=(int32_t) 319.20},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 228.20},{.data=(int32_t) 318.60},{.data=(int32_t) 228.90},{.data=(int32_t) 318.20},{.data=(int32_t) 229.50},{.data=(int32_t) 317.90},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 230.10},{.data=(int32_t) 317.70},{.data=(int32_t) 230.70},{.data=(int32_t) 317.50},{.data=(int32_t) 231.30},{.data=(int32_t) 317.50},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 232.60},{.data=(int32_t) 317.50},{.data=(int32_t) 233.60},{.data=(int32_t) 317.90},{.data=(int32_t) 234.50},{.data=(int32_t) 318.80},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 235.20},{.data=(int32_t) 319.50},{.data=(int32_t) 235.60},{.data=(int32_t) 320.70},{.data=(int32_t) 235.60},{.data=(int32_t) 322.10},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 235.60},{.data=(int32_t) 330.50},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 232.40},{.data=(int32_t) 330.50},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 232.40},{.data=(int32_t) 324.90},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 232.40},{.data=(int32_t) 323.40},{.data=(int32_t) 232.30},{.data=(int32_t) 322.40},{.data=(int32_t) 232.20},{.data=(int32_t) 321.90},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 232.10},{.data=(int32_t) 321.40},{.data=(int32_t) 231.80},{.data=(int32_t) 321.00},{.data=(int32_t) 231.50},{.data=(int32_t) 320.80},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 231.20},{.data=(int32_t) 320.50},{.data=(int32_t) 230.70},{.data=(int32_t) 320.40},{.data=(int32_t) 230.20},{.data=(int32_t) 320.40},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 229.60},{.data=(int32_t) 320.40},{.data=(int32_t) 229.00},{.data=(int32_t) 320.60},{.data=(int32_t) 228.50},{.data=(int32_t) 321.10},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 228.00},{.data=(int32_t) 321.50},{.data=(int32_t) 227.70},{.data=(int32_t) 322.10},{.data=(int32_t) 227.50},{.data=(int32_t) 322.90},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 227.40},{.data=(int32_t) 323.30},{.data=(int32_t) 227.40},{.data=(int32_t) 324.20},{.data=(int32_t) 227.40},{.data=(int32_t) 325.50},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 227.40},{.data=(int32_t) 330.60},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 224.20},{.data=(int32_t) 330.60},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 224.30},{.data=(int32_t) 317.90},
    {.cmd=VLC_OP_END}
};

/*path id=path6669*/
static data_mnemonic_t ClockDial_path_10_data[] = {
    {.cmd=VLC_OP_MOVE}, {.data=(int32_t) 20.10},{.data=(int32_t) 190.40},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 21.40},{.data=(int32_t) 190.40},{.data=(int32_t) 22.50},{.data=(int32_t) 190.70},{.data=(int32_t) 23.40},{.data=(int32_t) 191.30},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 24.30},{.data=(int32_t) 191.90},{.data=(int32_t) 25.10},{.data=(int32_t) 192.90},{.data=(int32_t) 25.60},{.data=(int32_t) 194.20},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 26.10},{.data=(int32_t) 195.50},{.data=(int32_t) 26.40},{.data=(int32_t) 197.30},{.data=(int32_t) 26.40},{.data=(int32_t) 199.60},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 26.40},{.data=(int32_t) 201.90},{.data=(int32_t) 26.10},{.data=(int32_t) 203.80},{.data=(int32_t) 25.60},{.data=(int32_t) 205.10},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 25.10},{.data=(int32_t) 206.40},{.data=(int32_t) 24.40},{.data=(int32_t) 207.40},{.data=(int32_t) 23.50},{.data=(int32_t) 208.00},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 22.60},{.data=(int32_t) 208.60},{.data=(int32_t) 21.50},{.data=(int32_t) 208.90},{.data=(int32_t) 20.20},{.data=(int32_t) 208.90},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 18.90},{.data=(int32_t) 208.90},{.data=(int32_t) 17.80},{.data=(int32_t) 208.60},{.data=(int32_t) 16.80},{.data=(int32_t) 208.00},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 15.90},{.data=(int32_t) 207.40},{.data=(int32_t) 15.20},{.data=(int32_t) 206.40},{.data=(int32_t) 14.70},{.data=(int32_t) 205.20},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 14.20},{.data=(int32_t) 203.90},{.data=(int32_t) 13.90},{.data=(int32_t) 202.20},{.data=(int32_t) 13.90},{.data=(int32_t) 199.80},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 13.90},{.data=(int32_t) 196.60},{.data=(int32_t) 14.40},{.data=(int32_t) 194.20},{.data=(int32_t) 15.40},{.data=(int32_t) 192.70},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 16.50},{.data=(int32_t) 191.20},{.data=(int32_t) 18.10},{.data=(int32_t) 190.40},{.data=(int32_t) 20.10},{.data=(int32_t) 190.40},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 20.10},{.data=(int32_t) 190.40},
    {.cmd=VLC_OP_MOVE}, {.data=(int32_t) 20.20},{.data=(int32_t) 193.60},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 19.60},{.data=(int32_t) 193.60},{.data=(int32_t) 19.10},{.data=(int32_t) 193.80},{.data=(int32_t) 18.70},{.data=(int32_t) 194.20},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 18.30},{.data=(int32_t) 194.60},{.data=(int32_t) 17.90},{.data=(int32_t) 195.20},{.data=(int32_t) 17.70},{.data=(int32_t) 196.10},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 17.50},{.data=(int32_t) 197.00},{.data=(int32_t) 17.40},{.data=(int32_t) 198.30},{.data=(int32_t) 17.40},{.data=(int32_t) 200.00},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 17.40},{.data=(int32_t) 202.20},{.data=(int32_t) 17.70},{.data=(int32_t) 203.80},{.data=(int32_t) 18.20},{.data=(int32_t) 204.60},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 18.70},{.data=(int32_t) 205.40},{.data=(int32_t) 19.40},{.data=(int32_t) 205.90},{.data=(int32_t) 20.30},{.data=(int32_t) 205.90},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 21.10},{.data=(int32_t) 205.90},{.data=(int32_t) 21.80},{.data=(int32_t) 205.50},{.data=(int32_t) 22.20},{.data=(int32_t) 204.70},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 22.80},{.data=(int32_t) 203.70},{.data=(int32_t) 23.10},{.data=(int32_t) 202.10},{.data=(int32_t) 23.10},{.data=(int32_t) 199.80},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 23.10},{.data=(int32_t) 197.50},{.data=(int32_t) 22.80},{.data=(int32_t) 195.90},{.data=(int32_t) 22.30},{.data=(int32_t) 195.00},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 21.70},{.data=(int32_t) 194.00},{.data=(int32_t) 21.00},{.data=(int32_t) 193.60},{.data=(int32_t) 20.20},{.data=(int32_t) 193.60},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 20.20},{.data=(int32_t) 193.60},
    {.cmd=VLC_OP_END}
};

/*path id=path6671*/
static data_mnemonic_t ClockDial_path_11_data[] = {
    {.cmd=VLC_OP_MOVE}, {.data=(int32_t) 33.70},{.data=(int32_t) 208.90},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 30.80},{.data=(int32_t) 207.50},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 33.90},{.data=(int32_t) 201.50},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 32.40},{.data=(int32_t) 201.50},{.data=(int32_t) 31.10},{.data=(int32_t) 201.00},{.data=(int32_t) 30.10},{.data=(int32_t) 199.90},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 29.10},{.data=(int32_t) 198.80},{.data=(int32_t) 28.50},{.data=(int32_t) 197.50},{.data=(int32_t) 28.50},{.data=(int32_t) 195.90},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 28.50},{.data=(int32_t) 194.40},{.data=(int32_t) 29.00},{.data=(int32_t) 193.00},{.data=(int32_t) 30.10},{.data=(int32_t) 192.00},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 31.20},{.data=(int32_t) 190.90},{.data=(int32_t) 32.50},{.data=(int32_t) 190.40},{.data=(int32_t) 34.00},{.data=(int32_t) 190.40},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 35.60},{.data=(int32_t) 190.40},{.data=(int32_t) 36.90},{.data=(int32_t) 191.00},{.data=(int32_t) 38.00},{.data=(int32_t) 192.10},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 39.10},{.data=(int32_t) 193.20},{.data=(int32_t) 39.60},{.data=(int32_t) 194.50},{.data=(int32_t) 39.60},{.data=(int32_t) 195.90},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 39.60},{.data=(int32_t) 196.40},{.data=(int32_t) 39.50},{.data=(int32_t) 197.00},{.data=(int32_t) 39.30},{.data=(int32_t) 197.50},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 39.10},{.data=(int32_t) 198.00},{.data=(int32_t) 38.80},{.data=(int32_t) 198.90},{.data=(int32_t) 38.20},{.data=(int32_t) 200.00},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 33.70},{.data=(int32_t) 208.90},
    {.cmd=VLC_OP_MOVE}, {.data=(int32_t) 34.10},{.data=(int32_t) 198.30},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 34.70},{.data=(int32_t) 198.30},{.data=(int32_t) 35.20},{.data=(int32_t) 198.10},{.data=(int32_t) 35.70},{.data=(int32_t) 197.60},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 36.10},{.data=(int32_t) 197.10},{.data=(int32_t) 36.30},{.data=(int32_t) 196.60},{.data=(int32_t) 36.30},{.data=(int32_t) 195.90},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 36.30},{.data=(int32_t) 195.20},{.data=(int32_t) 36.10},{.data=(int32_t) 194.60},{.data=(int32_t) 35.60},{.data=(int32_t) 194.20},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 35.20},{.data=(int32_t) 193.70},{.data=(int32_t) 34.60},{.data=(int32_t) 193.50},{.data=(int32_t) 34.00},{.data=(int32_t) 193.50},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 33.40},{.data=(int32_t) 193.50},{.data=(int32_t) 32.80},{.data=(int32_t) 193.70},{.data=(int32_t) 32.40},{.data=(int32_t) 194.20},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 32.00},{.data=(int32_t) 194.70},{.data=(int32_t) 31.70},{.data=(int32_t) 195.20},{.data=(int32_t) 31.70},{.data=(int32_t) 195.90},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 31.70},{.data=(int32_t) 196.60},{.data=(int32_t) 31.90},{.data=(int32_t) 197.20},{.data=(int32_t) 32.30},{.data=(int32_t) 197.60},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 32.90},{.data=(int32_t) 198.10},{.data=(int32_t) 33.40},{.data=(int32_t) 198.30},{.data=(int32_t) 34.10},{.data=(int32_t) 198.30},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 34.10},{.data=(int32_t) 198.30},
    {.cmd=VLC_OP_END}
};

/*path id=path6664*/
static data_mnemonic_t ClockDial_path_12_data[] = {
    {.cmd=VLC_OP_MOVE}, {.data=(int32_t) 40.80},{.data=(int32_t) 106.80},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 45.80},{.data=(int32_t) 106.80},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 45.80},{.data=(int32_t) 124.50},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 42.50},{.data=(int32_t) 124.50},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 42.50},{.data=(int32_t) 110.00},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 38.90},{.data=(int32_t) 110.00},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 40.80},{.data=(int32_t) 106.80},
    {.cmd=VLC_OP_END}
};

/*path id=path6666*/
static data_mnemonic_t ClockDial_path_13_data[] = {
    {.cmd=VLC_OP_MOVE}, {.data=(int32_t) 57.40},{.data=(int32_t) 106.30},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 58.70},{.data=(int32_t) 106.30},{.data=(int32_t) 59.80},{.data=(int32_t) 106.60},{.data=(int32_t) 60.70},{.data=(int32_t) 107.20},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 61.60},{.data=(int32_t) 107.80},{.data=(int32_t) 62.40},{.data=(int32_t) 108.80},{.data=(int32_t) 62.90},{.data=(int32_t) 110.10},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 63.40},{.data=(int32_t) 111.40},{.data=(int32_t) 63.70},{.data=(int32_t) 113.20},{.data=(int32_t) 63.70},{.data=(int32_t) 115.50},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 63.70},{.data=(int32_t) 117.80},{.data=(int32_t) 63.40},{.data=(int32_t) 119.70},{.data=(int32_t) 62.90},{.data=(int32_t) 121.00},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 62.40},{.data=(int32_t) 122.30},{.data=(int32_t) 61.70},{.data=(int32_t) 123.30},{.data=(int32_t) 60.80},{.data=(int32_t) 123.90},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 59.90},{.data=(int32_t) 124.50},{.data=(int32_t) 58.80},{.data=(int32_t) 124.80},{.data=(int32_t) 57.50},{.data=(int32_t) 124.80},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 56.20},{.data=(int32_t) 124.80},{.data=(int32_t) 55.10},{.data=(int32_t) 124.50},{.data=(int32_t) 54.10},{.data=(int32_t) 123.90},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 53.20},{.data=(int32_t) 123.30},{.data=(int32_t) 52.50},{.data=(int32_t) 122.30},{.data=(int32_t) 52.00},{.data=(int32_t) 121.10},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 51.50},{.data=(int32_t) 119.80},{.data=(int32_t) 51.20},{.data=(int32_t) 118.10},{.data=(int32_t) 51.20},{.data=(int32_t) 115.70},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 51.20},{.data=(int32_t) 112.50},{.data=(int32_t) 51.70},{.data=(int32_t) 110.10},{.data=(int32_t) 52.70},{.data=(int32_t) 108.60},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 53.90},{.data=(int32_t) 107.10},{.data=(int32_t) 55.40},{.data=(int32_t) 106.30},{.data=(int32_t) 57.40},{.data=(int32_t) 106.30},
    {.cmd=VLC_OP_MOVE}, {.data=(int32_t) 57.50},{.data=(int32_t) 109.50},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 56.90},{.data=(int32_t) 109.50},{.data=(int32_t) 56.40},{.data=(int32_t) 109.70},{.data=(int32_t) 56.00},{.data=(int32_t) 110.10},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 55.60},{.data=(int32_t) 110.50},{.data=(int32_t) 55.20},{.data=(int32_t) 111.10},{.data=(int32_t) 55.00},{.data=(int32_t) 112.00},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 54.80},{.data=(int32_t) 112.90},{.data=(int32_t) 54.70},{.data=(int32_t) 114.20},{.data=(int32_t) 54.70},{.data=(int32_t) 115.90},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 54.70},{.data=(int32_t) 118.10},{.data=(int32_t) 55.00},{.data=(int32_t) 119.70},{.data=(int32_t) 55.50},{.data=(int32_t) 120.50},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 56.00},{.data=(int32_t) 121.30},{.data=(int32_t) 56.70},{.data=(int32_t) 121.80},{.data=(int32_t) 57.60},{.data=(int32_t) 121.80},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 58.40},{.data=(int32_t) 121.80},{.data=(int32_t) 59.10},{.data=(int32_t) 121.40},{.data=(int32_t) 59.50},{.data=(int32_t) 120.60},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 60.10},{.data=(int32_t) 119.60},{.data=(int32_t) 60.40},{.data=(int32_t) 118.00},{.data=(int32_t) 60.40},{.data=(int32_t) 115.70},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 60.40},{.data=(int32_t) 113.40},{.data=(int32_t) 60.10},{.data=(int32_t) 111.80},{.data=(int32_t) 59.60},{.data=(int32_t) 110.90},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 59.00},{.data=(int32_t) 110.00},{.data=(int32_t) 58.30},{.data=(int32_t) 109.50},{.data=(int32_t) 57.50},{.data=(int32_t) 109.50},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 57.50},{.data=(int32_t) 109.50},
    {.cmd=VLC_OP_END}
};

/*path id=path6659*/
static data_mnemonic_t ClockDial_path_14_data[] = {
    {.cmd=VLC_OP_MOVE}, {.data=(int32_t) 98.40},{.data=(int32_t) 48.10},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 103.40},{.data=(int32_t) 48.10},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 103.40},{.data=(int32_t) 65.80},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 100.10},{.data=(int32_t) 65.80},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 100.10},{.data=(int32_t) 51.20},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 96.50},{.data=(int32_t) 51.20},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 98.40},{.data=(int32_t) 48.10},
    {.cmd=VLC_OP_END}
};

/*path id=path6661*/
static data_mnemonic_t ClockDial_path_15_data[] = {
    {.cmd=VLC_OP_MOVE}, {.data=(int32_t) 112.20},{.data=(int32_t) 48.10},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 117.20},{.data=(int32_t) 48.10},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 117.20},{.data=(int32_t) 65.80},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 113.90},{.data=(int32_t) 65.80},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 113.90},{.data=(int32_t) 51.20},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 110.30},{.data=(int32_t) 51.20},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 112.20},{.data=(int32_t) 48.10},
    {.cmd=VLC_OP_END}
};

/*path id=path6654*/
static data_mnemonic_t ClockDial_path_16_data[] = {
    {.cmd=VLC_OP_MOVE}, {.data=(int32_t) 189.90},{.data=(int32_t) 21.60},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 194.90},{.data=(int32_t) 21.60},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 194.90},{.data=(int32_t) 39.30},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 191.60},{.data=(int32_t) 39.30},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 191.60},{.data=(int32_t) 24.70},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 188.00},{.data=(int32_t) 24.70},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 189.90},{.data=(int32_t) 21.60},
    {.cmd=VLC_OP_END}
};

/*path id=path6656*/
static data_mnemonic_t ClockDial_path_17_data[] = {
    {.cmd=VLC_OP_MOVE}, {.data=(int32_t) 203.90},{.data=(int32_t) 27.30},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 200.60},{.data=(int32_t) 27.30},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 200.70},{.data=(int32_t) 25.40},{.data=(int32_t) 201.30},{.data=(int32_t) 23.90},{.data=(int32_t) 202.40},{.data=(int32_t) 22.80},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 203.50},{.data=(int32_t) 21.70},{.data=(int32_t) 204.90},{.data=(int32_t) 21.20},{.data=(int32_t) 206.70},{.data=(int32_t) 21.20},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 207.80},{.data=(int32_t) 21.20},{.data=(int32_t) 208.70},{.data=(int32_t) 21.40},{.data=(int32_t) 209.50},{.data=(int32_t) 21.90},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 210.30},{.data=(int32_t) 22.30},{.data=(int32_t) 211.00},{.data=(int32_t) 23.00},{.data=(int32_t) 211.50},{.data=(int32_t) 23.90},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 212.00},{.data=(int32_t) 24.70},{.data=(int32_t) 212.20},{.data=(int32_t) 25.60},{.data=(int32_t) 212.20},{.data=(int32_t) 26.50},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 212.20},{.data=(int32_t) 27.50},{.data=(int32_t) 211.90},{.data=(int32_t) 28.70},{.data=(int32_t) 211.30},{.data=(int32_t) 29.90},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 210.70},{.data=(int32_t) 31.10},{.data=(int32_t) 209.60},{.data=(int32_t) 32.50},{.data=(int32_t) 208.00},{.data=(int32_t) 34.20},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 206.00},{.data=(int32_t) 36.30},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 212.30},{.data=(int32_t) 36.30},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 212.30},{.data=(int32_t) 39.40},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 200.10},{.data=(int32_t) 39.40},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 200.10},{.data=(int32_t) 37.80},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 205.60},{.data=(int32_t) 32.20},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 206.90},{.data=(int32_t) 30.90},{.data=(int32_t) 207.80},{.data=(int32_t) 29.80},{.data=(int32_t) 208.20},{.data=(int32_t) 29.00},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 208.60},{.data=(int32_t) 28.20},{.data=(int32_t) 208.90},{.data=(int32_t) 27.40},{.data=(int32_t) 208.90},{.data=(int32_t) 26.80},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 208.90},{.data=(int32_t) 26.10},{.data=(int32_t) 208.70},{.data=(int32_t) 25.60},{.data=(int32_t) 208.20},{.data=(int32_t) 25.10},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 207.80},{.data=(int32_t) 24.70},{.data=(int32_t) 207.20},{.data=(int32_t) 24.40},{.data=(int32_t) 206.50},{.data=(int32_t) 24.40},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 205.80},{.data=(int32_t) 24.40},{.data=(int32_t) 205.20},{.data=(int32_t) 24.70},{.data=(int32_t) 204.70},{.data=(int32_t) 25.20},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 204.20},{.data=(int32_t) 25.60},{.data=(int32_t) 203.90},{.data=(int32_t) 26.30},{.data=(int32_t) 203.90},{.data=(int32_t) 27.30},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 203.90},{.data=(int32_t) 27.30},
    {.cmd=VLC_OP_END}
};

/*path id=path6649*/
static data_mnemonic_t ClockDial_path_18_data[] = {
    {.cmd=VLC_OP_MOVE}, {.data=(int32_t) 289.30},{.data=(int32_t) 47.20},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 290.60},{.data=(int32_t) 47.20},{.data=(int32_t) 291.70},{.data=(int32_t) 47.50},{.data=(int32_t) 292.60},{.data=(int32_t) 48.10},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 293.50},{.data=(int32_t) 48.70},{.data=(int32_t) 294.30},{.data=(int32_t) 49.70},{.data=(int32_t) 294.80},{.data=(int32_t) 51.00},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 295.30},{.data=(int32_t) 52.30},{.data=(int32_t) 295.60},{.data=(int32_t) 54.10},{.data=(int32_t) 295.60},{.data=(int32_t) 56.40},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 295.60},{.data=(int32_t) 58.70},{.data=(int32_t) 295.30},{.data=(int32_t) 60.60},{.data=(int32_t) 294.80},{.data=(int32_t) 61.90},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 294.30},{.data=(int32_t) 63.20},{.data=(int32_t) 293.60},{.data=(int32_t) 64.20},{.data=(int32_t) 292.70},{.data=(int32_t) 64.80},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 291.80},{.data=(int32_t) 65.40},{.data=(int32_t) 290.70},{.data=(int32_t) 65.70},{.data=(int32_t) 289.40},{.data=(int32_t) 65.70},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 288.10},{.data=(int32_t) 65.70},{.data=(int32_t) 287.00},{.data=(int32_t) 65.40},{.data=(int32_t) 286.00},{.data=(int32_t) 64.80},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 285.10},{.data=(int32_t) 64.20},{.data=(int32_t) 284.40},{.data=(int32_t) 63.20},{.data=(int32_t) 283.90},{.data=(int32_t) 62.00},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 283.40},{.data=(int32_t) 60.70},{.data=(int32_t) 283.10},{.data=(int32_t) 59.00},{.data=(int32_t) 283.10},{.data=(int32_t) 56.60},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 283.10},{.data=(int32_t) 53.40},{.data=(int32_t) 283.60},{.data=(int32_t) 51.00},{.data=(int32_t) 284.60},{.data=(int32_t) 49.50},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 285.70},{.data=(int32_t) 48.00},{.data=(int32_t) 287.30},{.data=(int32_t) 47.20},{.data=(int32_t) 289.30},{.data=(int32_t) 47.20},
    {.cmd=VLC_OP_MOVE}, {.data=(int32_t) 289.30},{.data=(int32_t) 50.40},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 288.70},{.data=(int32_t) 50.40},{.data=(int32_t) 288.20},{.data=(int32_t) 50.60},{.data=(int32_t) 287.80},{.data=(int32_t) 51.00},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 287.40},{.data=(int32_t) 51.40},{.data=(int32_t) 287.00},{.data=(int32_t) 52.00},{.data=(int32_t) 286.80},{.data=(int32_t) 52.90},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 286.60},{.data=(int32_t) 53.80},{.data=(int32_t) 286.50},{.data=(int32_t) 55.10},{.data=(int32_t) 286.50},{.data=(int32_t) 56.80},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 286.50},{.data=(int32_t) 59.00},{.data=(int32_t) 286.80},{.data=(int32_t) 60.60},{.data=(int32_t) 287.30},{.data=(int32_t) 61.40},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 287.80},{.data=(int32_t) 62.20},{.data=(int32_t) 288.50},{.data=(int32_t) 62.70},{.data=(int32_t) 289.40},{.data=(int32_t) 62.70},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 290.20},{.data=(int32_t) 62.70},{.data=(int32_t) 290.90},{.data=(int32_t) 62.30},{.data=(int32_t) 291.30},{.data=(int32_t) 61.50},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 291.90},{.data=(int32_t) 60.50},{.data=(int32_t) 292.20},{.data=(int32_t) 58.90},{.data=(int32_t) 292.20},{.data=(int32_t) 56.60},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 292.20},{.data=(int32_t) 54.30},{.data=(int32_t) 291.90},{.data=(int32_t) 52.70},{.data=(int32_t) 291.40},{.data=(int32_t) 51.80},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 290.90},{.data=(int32_t) 50.80},{.data=(int32_t) 290.20},{.data=(int32_t) 50.40},{.data=(int32_t) 289.30},{.data=(int32_t) 50.40},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 289.30},{.data=(int32_t) 50.40},
    {.cmd=VLC_OP_END}
};

/*path id=path6651*/
static data_mnemonic_t ClockDial_path_19_data[] = {
    {.cmd=VLC_OP_MOVE}, {.data=(int32_t) 300.30},{.data=(int32_t) 47.60},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 305.30},{.data=(int32_t) 47.60},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 305.30},{.data=(int32_t) 65.30},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 302.00},{.data=(int32_t) 65.30},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 302.00},{.data=(int32_t) 50.80},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 298.40},{.data=(int32_t) 50.80},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 300.30},{.data=(int32_t) 47.60},
    {.cmd=VLC_OP_END}
};

/*path id=path6644*/
static data_mnemonic_t ClockDial_path_20_data[] = {
    {.cmd=VLC_OP_MOVE}, {.data=(int32_t) 342.00},{.data=(int32_t) 106.30},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 343.30},{.data=(int32_t) 106.30},{.data=(int32_t) 344.40},{.data=(int32_t) 106.60},{.data=(int32_t) 345.30},{.data=(int32_t) 107.20},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 346.20},{.data=(int32_t) 107.80},{.data=(int32_t) 347.00},{.data=(int32_t) 108.80},{.data=(int32_t) 347.50},{.data=(int32_t) 110.10},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 348.00},{.data=(int32_t) 111.40},{.data=(int32_t) 348.30},{.data=(int32_t) 113.20},{.data=(int32_t) 348.30},{.data=(int32_t) 115.50},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 348.30},{.data=(int32_t) 117.80},{.data=(int32_t) 348.00},{.data=(int32_t) 119.70},{.data=(int32_t) 347.50},{.data=(int32_t) 121.00},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 347.00},{.data=(int32_t) 122.30},{.data=(int32_t) 346.30},{.data=(int32_t) 123.30},{.data=(int32_t) 345.40},{.data=(int32_t) 123.90},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 344.50},{.data=(int32_t) 124.50},{.data=(int32_t) 343.40},{.data=(int32_t) 124.80},{.data=(int32_t) 342.10},{.data=(int32_t) 124.80},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 340.80},{.data=(int32_t) 124.80},{.data=(int32_t) 339.70},{.data=(int32_t) 124.50},{.data=(int32_t) 338.70},{.data=(int32_t) 123.90},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 337.80},{.data=(int32_t) 123.30},{.data=(int32_t) 337.10},{.data=(int32_t) 122.30},{.data=(int32_t) 336.60},{.data=(int32_t) 121.10},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 336.10},{.data=(int32_t) 119.80},{.data=(int32_t) 335.80},{.data=(int32_t) 118.10},{.data=(int32_t) 335.80},{.data=(int32_t) 115.70},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 335.80},{.data=(int32_t) 112.50},{.data=(int32_t) 336.30},{.data=(int32_t) 110.10},{.data=(int32_t) 337.30},{.data=(int32_t) 108.60},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 338.40},{.data=(int32_t) 107.10},{.data=(int32_t) 340.00},{.data=(int32_t) 106.30},{.data=(int32_t) 342.00},{.data=(int32_t) 106.30},
    {.cmd=VLC_OP_MOVE}, {.data=(int32_t) 342.00},{.data=(int32_t) 109.50},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 341.40},{.data=(int32_t) 109.50},{.data=(int32_t) 340.90},{.data=(int32_t) 109.70},{.data=(int32_t) 340.50},{.data=(int32_t) 110.10},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 340.10},{.data=(int32_t) 110.50},{.data=(int32_t) 339.70},{.data=(int32_t) 111.10},{.data=(int32_t) 339.50},{.data=(int32_t) 112.00},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 339.30},{.data=(int32_t) 112.90},{.data=(int32_t) 339.20},{.data=(int32_t) 114.20},{.data=(int32_t) 339.20},{.data=(int32_t) 115.90},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 339.20},{.data=(int32_t) 118.10},{.data=(int32_t) 339.50},{.data=(int32_t) 119.70},{.data=(int32_t) 340.00},{.data=(int32_t) 120.50},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 340.50},{.data=(int32_t) 121.30},{.data=(int32_t) 341.20},{.data=(int32_t) 121.80},{.data=(int32_t) 342.10},{.data=(int32_t) 121.80},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 342.90},{.data=(int32_t) 121.80},{.data=(int32_t) 343.60},{.data=(int32_t) 121.40},{.data=(int32_t) 344.00},{.data=(int32_t) 120.60},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 344.60},{.data=(int32_t) 119.60},{.data=(int32_t) 344.90},{.data=(int32_t) 118.00},{.data=(int32_t) 344.90},{.data=(int32_t) 115.70},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 344.90},{.data=(int32_t) 113.40},{.data=(int32_t) 344.60},{.data=(int32_t) 111.80},{.data=(int32_t) 344.10},{.data=(int32_t) 110.90},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 343.60},{.data=(int32_t) 110.00},{.data=(int32_t) 342.90},{.data=(int32_t) 109.50},{.data=(int32_t) 342.00},{.data=(int32_t) 109.50},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 342.00},{.data=(int32_t) 109.50},
    {.cmd=VLC_OP_END}
};

/*path id=path6646*/
static data_mnemonic_t ClockDial_path_21_data[] = {
    {.cmd=VLC_OP_MOVE}, {.data=(int32_t) 353.20},{.data=(int32_t) 112.50},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 349.90},{.data=(int32_t) 112.50},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 350.00},{.data=(int32_t) 110.60},{.data=(int32_t) 350.60},{.data=(int32_t) 109.10},{.data=(int32_t) 351.70},{.data=(int32_t) 108.00},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 352.80},{.data=(int32_t) 106.90},{.data=(int32_t) 354.20},{.data=(int32_t) 106.40},{.data=(int32_t) 356.00},{.data=(int32_t) 106.40},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 357.10},{.data=(int32_t) 106.40},{.data=(int32_t) 358.00},{.data=(int32_t) 106.60},{.data=(int32_t) 358.80},{.data=(int32_t) 107.10},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 359.60},{.data=(int32_t) 107.50},{.data=(int32_t) 360.30},{.data=(int32_t) 108.20},{.data=(int32_t) 360.80},{.data=(int32_t) 109.10},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 361.30},{.data=(int32_t) 109.90},{.data=(int32_t) 361.50},{.data=(int32_t) 110.80},{.data=(int32_t) 361.50},{.data=(int32_t) 111.70},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 361.50},{.data=(int32_t) 112.70},{.data=(int32_t) 361.20},{.data=(int32_t) 113.90},{.data=(int32_t) 360.60},{.data=(int32_t) 115.10},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 360.00},{.data=(int32_t) 116.30},{.data=(int32_t) 358.90},{.data=(int32_t) 117.70},{.data=(int32_t) 357.30},{.data=(int32_t) 119.40},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 355.30},{.data=(int32_t) 121.50},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 361.60},{.data=(int32_t) 121.50},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 361.60},{.data=(int32_t) 124.60},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 349.40},{.data=(int32_t) 124.60},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 349.40},{.data=(int32_t) 123.00},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 354.90},{.data=(int32_t) 117.40},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 356.20},{.data=(int32_t) 116.10},{.data=(int32_t) 357.10},{.data=(int32_t) 115.00},{.data=(int32_t) 357.50},{.data=(int32_t) 114.20},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 357.90},{.data=(int32_t) 113.40},{.data=(int32_t) 358.20},{.data=(int32_t) 112.60},{.data=(int32_t) 358.20},{.data=(int32_t) 112.00},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 358.20},{.data=(int32_t) 111.30},{.data=(int32_t) 358.00},{.data=(int32_t) 110.80},{.data=(int32_t) 357.50},{.data=(int32_t) 110.30},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 357.10},{.data=(int32_t) 109.90},{.data=(int32_t) 356.50},{.data=(int32_t) 109.60},{.data=(int32_t) 355.80},{.data=(int32_t) 109.60},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 355.10},{.data=(int32_t) 109.60},{.data=(int32_t) 354.50},{.data=(int32_t) 109.90},{.data=(int32_t) 354.00},{.data=(int32_t) 110.40},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 353.50},{.data=(int32_t) 110.80},{.data=(int32_t) 353.20},{.data=(int32_t) 111.60},{.data=(int32_t) 353.20},{.data=(int32_t) 112.50},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 353.20},{.data=(int32_t) 112.50},
    {.cmd=VLC_OP_END}
};

/*path id=path6639*/
static data_mnemonic_t ClockDial_path_22_data[] = {
    {.cmd=VLC_OP_MOVE}, {.data=(int32_t) 364.80},{.data=(int32_t) 190.40},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 366.10},{.data=(int32_t) 190.40},{.data=(int32_t) 367.20},{.data=(int32_t) 190.70},{.data=(int32_t) 368.10},{.data=(int32_t) 191.30},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 369.00},{.data=(int32_t) 191.90},{.data=(int32_t) 369.80},{.data=(int32_t) 192.90},{.data=(int32_t) 370.30},{.data=(int32_t) 194.20},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 370.80},{.data=(int32_t) 195.50},{.data=(int32_t) 371.10},{.data=(int32_t) 197.30},{.data=(int32_t) 371.10},{.data=(int32_t) 199.60},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 371.10},{.data=(int32_t) 201.90},{.data=(int32_t) 370.80},{.data=(int32_t) 203.80},{.data=(int32_t) 370.30},{.data=(int32_t) 205.10},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 369.80},{.data=(int32_t) 206.40},{.data=(int32_t) 369.10},{.data=(int32_t) 207.40},{.data=(int32_t) 368.20},{.data=(int32_t) 208.00},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 367.30},{.data=(int32_t) 208.60},{.data=(int32_t) 366.20},{.data=(int32_t) 208.90},{.data=(int32_t) 364.90},{.data=(int32_t) 208.90},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 363.60},{.data=(int32_t) 208.90},{.data=(int32_t) 362.50},{.data=(int32_t) 208.60},{.data=(int32_t) 361.50},{.data=(int32_t) 208.00},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 360.60},{.data=(int32_t) 207.40},{.data=(int32_t) 359.90},{.data=(int32_t) 206.40},{.data=(int32_t) 359.40},{.data=(int32_t) 205.20},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 358.90},{.data=(int32_t) 203.90},{.data=(int32_t) 358.60},{.data=(int32_t) 202.20},{.data=(int32_t) 358.60},{.data=(int32_t) 199.80},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 358.60},{.data=(int32_t) 196.60},{.data=(int32_t) 359.10},{.data=(int32_t) 194.20},{.data=(int32_t) 360.10},{.data=(int32_t) 192.70},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 361.30},{.data=(int32_t) 191.20},{.data=(int32_t) 362.80},{.data=(int32_t) 190.40},{.data=(int32_t) 364.80},{.data=(int32_t) 190.40},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 364.80},{.data=(int32_t) 190.40},
    {.cmd=VLC_OP_MOVE}, {.data=(int32_t) 364.90},{.data=(int32_t) 193.60},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 364.30},{.data=(int32_t) 193.60},{.data=(int32_t) 363.80},{.data=(int32_t) 193.80},{.data=(int32_t) 363.40},{.data=(int32_t) 194.20},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 363.00},{.data=(int32_t) 194.60},{.data=(int32_t) 362.60},{.data=(int32_t) 195.20},{.data=(int32_t) 362.40},{.data=(int32_t) 196.10},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 362.20},{.data=(int32_t) 197.00},{.data=(int32_t) 362.10},{.data=(int32_t) 198.30},{.data=(int32_t) 362.10},{.data=(int32_t) 200.00},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 362.10},{.data=(int32_t) 202.20},{.data=(int32_t) 362.40},{.data=(int32_t) 203.80},{.data=(int32_t) 362.90},{.data=(int32_t) 204.60},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 363.40},{.data=(int32_t) 205.40},{.data=(int32_t) 364.10},{.data=(int32_t) 205.90},{.data=(int32_t) 365.00},{.data=(int32_t) 205.90},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 365.80},{.data=(int32_t) 205.90},{.data=(int32_t) 366.50},{.data=(int32_t) 205.50},{.data=(int32_t) 366.90},{.data=(int32_t) 204.70},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 367.50},{.data=(int32_t) 203.70},{.data=(int32_t) 367.80},{.data=(int32_t) 202.10},{.data=(int32_t) 367.80},{.data=(int32_t) 199.80},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 367.80},{.data=(int32_t) 197.50},{.data=(int32_t) 367.50},{.data=(int32_t) 195.90},{.data=(int32_t) 367.00},{.data=(int32_t) 195.00},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 366.40},{.data=(int32_t) 194.00},{.data=(int32_t) 365.70},{.data=(int32_t) 193.60},{.data=(int32_t) 364.90},{.data=(int32_t) 193.60},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 364.90},{.data=(int32_t) 193.60},
    {.cmd=VLC_OP_END}
};

/*path id=path6641*/
static data_mnemonic_t ClockDial_path_23_data[] = {
    {.cmd=VLC_OP_MOVE}, {.data=(int32_t) 376.40},{.data=(int32_t) 195.20},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 373.20},{.data=(int32_t) 195.20},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 373.40},{.data=(int32_t) 193.90},{.data=(int32_t) 373.80},{.data=(int32_t) 192.80},{.data=(int32_t) 374.60},{.data=(int32_t) 192.10},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 375.70},{.data=(int32_t) 191.00},{.data=(int32_t) 377.00},{.data=(int32_t) 190.40},{.data=(int32_t) 378.70},{.data=(int32_t) 190.40},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 380.20},{.data=(int32_t) 190.40},{.data=(int32_t) 381.40},{.data=(int32_t) 190.90},{.data=(int32_t) 382.30},{.data=(int32_t) 191.80},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 383.30},{.data=(int32_t) 192.70},{.data=(int32_t) 383.80},{.data=(int32_t) 193.80},{.data=(int32_t) 383.80},{.data=(int32_t) 195.10},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 383.80},{.data=(int32_t) 195.90},{.data=(int32_t) 383.60},{.data=(int32_t) 196.60},{.data=(int32_t) 383.20},{.data=(int32_t) 197.30},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 382.80},{.data=(int32_t) 197.90},{.data=(int32_t) 382.20},{.data=(int32_t) 198.50},{.data=(int32_t) 381.30},{.data=(int32_t) 198.90},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 382.40},{.data=(int32_t) 199.20},{.data=(int32_t) 383.20},{.data=(int32_t) 199.80},{.data=(int32_t) 383.80},{.data=(int32_t) 200.60},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 384.40},{.data=(int32_t) 201.40},{.data=(int32_t) 384.70},{.data=(int32_t) 202.30},{.data=(int32_t) 384.70},{.data=(int32_t) 203.40},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 384.70},{.data=(int32_t) 205.00},{.data=(int32_t) 384.10},{.data=(int32_t) 206.30},{.data=(int32_t) 382.90},{.data=(int32_t) 207.40},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 381.70},{.data=(int32_t) 208.50},{.data=(int32_t) 380.20},{.data=(int32_t) 209.00},{.data=(int32_t) 378.40},{.data=(int32_t) 209.00},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 376.70},{.data=(int32_t) 209.00},{.data=(int32_t) 375.30},{.data=(int32_t) 208.50},{.data=(int32_t) 374.20},{.data=(int32_t) 207.40},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 373.10},{.data=(int32_t) 206.40},{.data=(int32_t) 372.50},{.data=(int32_t) 204.90},{.data=(int32_t) 372.40},{.data=(int32_t) 203.20},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 375.70},{.data=(int32_t) 203.20},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 375.80},{.data=(int32_t) 204.10},{.data=(int32_t) 376.20},{.data=(int32_t) 204.80},{.data=(int32_t) 376.60},{.data=(int32_t) 205.20},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 377.10},{.data=(int32_t) 205.60},{.data=(int32_t) 377.70},{.data=(int32_t) 205.90},{.data=(int32_t) 378.50},{.data=(int32_t) 205.90},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 379.30},{.data=(int32_t) 205.90},{.data=(int32_t) 379.90},{.data=(int32_t) 205.60},{.data=(int32_t) 380.40},{.data=(int32_t) 205.10},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 380.90},{.data=(int32_t) 204.60},{.data=(int32_t) 381.20},{.data=(int32_t) 204.00},{.data=(int32_t) 381.20},{.data=(int32_t) 203.20},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 381.20},{.data=(int32_t) 202.40},{.data=(int32_t) 380.90},{.data=(int32_t) 201.70},{.data=(int32_t) 380.20},{.data=(int32_t) 201.20},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 379.50},{.data=(int32_t) 200.60},{.data=(int32_t) 378.50},{.data=(int32_t) 200.40},{.data=(int32_t) 377.20},{.data=(int32_t) 200.30},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 377.20},{.data=(int32_t) 197.40},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 378.00},{.data=(int32_t) 197.30},{.data=(int32_t) 378.60},{.data=(int32_t) 197.20},{.data=(int32_t) 379.00},{.data=(int32_t) 197.00},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 379.40},{.data=(int32_t) 196.80},{.data=(int32_t) 379.70},{.data=(int32_t) 196.50},{.data=(int32_t) 379.90},{.data=(int32_t) 196.20},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 380.10},{.data=(int32_t) 195.90},{.data=(int32_t) 380.20},{.data=(int32_t) 195.50},{.data=(int32_t) 380.20},{.data=(int32_t) 195.20},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 380.20},{.data=(int32_t) 194.70},{.data=(int32_t) 380.00},{.data=(int32_t) 194.30},{.data=(int32_t) 379.70},{.data=(int32_t) 194.00},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 379.40},{.data=(int32_t) 193.70},{.data=(int32_t) 378.90},{.data=(int32_t) 193.50},{.data=(int32_t) 378.40},{.data=(int32_t) 193.50},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 377.90},{.data=(int32_t) 193.50},{.data=(int32_t) 377.50},{.data=(int32_t) 193.60},{.data=(int32_t) 377.10},{.data=(int32_t) 193.90},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 376.80},{.data=(int32_t) 194.30},{.data=(int32_t) 376.60},{.data=(int32_t) 194.70},{.data=(int32_t) 376.40},{.data=(int32_t) 195.20},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 376.40},{.data=(int32_t) 195.20},
    {.cmd=VLC_OP_END}
};

/*path id=path1373*/
static data_mnemonic_t ClockDial_path_24_data[] = {
    {.cmd=VLC_OP_MOVE}, {.data=(int32_t) 121.10},{.data=(int32_t) 243.70},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 124.80},{.data=(int32_t) 243.70},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 129.50},{.data=(int32_t) 257.00},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 134.30},{.data=(int32_t) 243.70},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 138.00},{.data=(int32_t) 243.70},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 131.30},{.data=(int32_t) 262.40},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 127.90},{.data=(int32_t) 262.40},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 121.10},{.data=(int32_t) 243.70},
    {.cmd=VLC_OP_END}
};

/*path id=path1375*/
static data_mnemonic_t ClockDial_path_25_data[] = {
    {.cmd=VLC_OP_MOVE}, {.data=(int32_t) 158.00},{.data=(int32_t) 246.80},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 155.50},{.data=(int32_t) 249.30},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 153.90},{.data=(int32_t) 247.60},{.data=(int32_t) 152.10},{.data=(int32_t) 246.80},{.data=(int32_t) 150.00},{.data=(int32_t) 246.80},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 148.20},{.data=(int32_t) 246.80},{.data=(int32_t) 146.60},{.data=(int32_t) 247.40},{.data=(int32_t) 145.40},{.data=(int32_t) 248.60},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 144.20},{.data=(int32_t) 249.80},{.data=(int32_t) 143.50},{.data=(int32_t) 251.30},{.data=(int32_t) 143.50},{.data=(int32_t) 253.00},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 143.50},{.data=(int32_t) 254.80},{.data=(int32_t) 144.10},{.data=(int32_t) 256.30},{.data=(int32_t) 145.40},{.data=(int32_t) 257.60},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 146.70},{.data=(int32_t) 258.90},{.data=(int32_t) 148.30},{.data=(int32_t) 259.50},{.data=(int32_t) 150.20},{.data=(int32_t) 259.50},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 151.40},{.data=(int32_t) 259.50},{.data=(int32_t) 152.50},{.data=(int32_t) 259.20},{.data=(int32_t) 153.30},{.data=(int32_t) 258.70},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 154.10},{.data=(int32_t) 258.20},{.data=(int32_t) 154.80},{.data=(int32_t) 257.40},{.data=(int32_t) 155.30},{.data=(int32_t) 256.30},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 149.90},{.data=(int32_t) 256.30},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 149.90},{.data=(int32_t) 253.00},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 159.30},{.data=(int32_t) 253.00},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 159.30},{.data=(int32_t) 253.80},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 159.30},{.data=(int32_t) 255.40},{.data=(int32_t) 158.90},{.data=(int32_t) 257.00},{.data=(int32_t) 158.00},{.data=(int32_t) 258.40},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 157.20},{.data=(int32_t) 259.90},{.data=(int32_t) 156.10},{.data=(int32_t) 261.00},{.data=(int32_t) 154.70},{.data=(int32_t) 261.70},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 153.40},{.data=(int32_t) 262.50},{.data=(int32_t) 151.80},{.data=(int32_t) 262.90},{.data=(int32_t) 150.00},{.data=(int32_t) 262.90},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 148.10},{.data=(int32_t) 262.90},{.data=(int32_t) 146.30},{.data=(int32_t) 262.50},{.data=(int32_t) 144.80},{.data=(int32_t) 261.60},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 143.30},{.data=(int32_t) 260.80},{.data=(int32_t) 142.10},{.data=(int32_t) 259.60},{.data=(int32_t) 141.20},{.data=(int32_t) 258.00},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 140.30},{.data=(int32_t) 256.40},{.data=(int32_t) 139.90},{.data=(int32_t) 254.80},{.data=(int32_t) 139.90},{.data=(int32_t) 253.00},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 139.90},{.data=(int32_t) 250.50},{.data=(int32_t) 140.70},{.data=(int32_t) 248.40},{.data=(int32_t) 142.30},{.data=(int32_t) 246.60},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 144.20},{.data=(int32_t) 244.40},{.data=(int32_t) 146.80},{.data=(int32_t) 243.30},{.data=(int32_t) 149.90},{.data=(int32_t) 243.30},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 151.50},{.data=(int32_t) 243.30},{.data=(int32_t) 153.00},{.data=(int32_t) 243.60},{.data=(int32_t) 154.50},{.data=(int32_t) 244.20},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 155.70},{.data=(int32_t) 244.70},{.data=(int32_t) 156.90},{.data=(int32_t) 245.50},{.data=(int32_t) 158.00},{.data=(int32_t) 246.80},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 158.00},{.data=(int32_t) 246.80},
    {.cmd=VLC_OP_END}
};

/*path id=path1377*/
static data_mnemonic_t ClockDial_path_26_data[] = {
    {.cmd=VLC_OP_MOVE}, {.data=(int32_t) 169.80},{.data=(int32_t) 243.70},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 173.40},{.data=(int32_t) 243.70},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 173.40},{.data=(int32_t) 259.00},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 178.60},{.data=(int32_t) 259.00},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 178.60},{.data=(int32_t) 262.40},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 169.90},{.data=(int32_t) 262.40},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 169.80},{.data=(int32_t) 243.70},
    {.cmd=VLC_OP_END}
};

/*path id=path1379*/
static data_mnemonic_t ClockDial_path_27_data[] = {
    {.cmd=VLC_OP_MOVE}, {.data=(int32_t) 181.00},{.data=(int32_t) 243.70},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 184.50},{.data=(int32_t) 243.70},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 184.50},{.data=(int32_t) 262.40},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 181.00},{.data=(int32_t) 262.40},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 181.00},{.data=(int32_t) 243.70},
    {.cmd=VLC_OP_END}
};

/*path id=path1381*/
static data_mnemonic_t ClockDial_path_28_data[] = {
    {.cmd=VLC_OP_MOVE}, {.data=(int32_t) 186.70},{.data=(int32_t) 243.70},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 197.00},{.data=(int32_t) 243.70},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 197.00},{.data=(int32_t) 247.20},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 193.60},{.data=(int32_t) 247.20},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 193.60},{.data=(int32_t) 262.40},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 190.00},{.data=(int32_t) 262.40},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 190.00},{.data=(int32_t) 247.20},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 186.70},{.data=(int32_t) 247.20},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 186.70},{.data=(int32_t) 243.70},
    {.cmd=VLC_OP_END}
};

/*path id=path1383*/
static data_mnemonic_t ClockDial_path_29_data[] = {
    {.cmd=VLC_OP_MOVE}, {.data=(int32_t) 199.40},{.data=(int32_t) 243.70},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 209.60},{.data=(int32_t) 243.70},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 209.60},{.data=(int32_t) 247.20},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 203.00},{.data=(int32_t) 247.20},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 203.00},{.data=(int32_t) 250.60},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 209.70},{.data=(int32_t) 250.60},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 209.70},{.data=(int32_t) 254.00},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 203.00},{.data=(int32_t) 254.00},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 203.00},{.data=(int32_t) 258.90},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 209.70},{.data=(int32_t) 258.90},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 209.70},{.data=(int32_t) 262.40},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 199.50},{.data=(int32_t) 262.40},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 199.50},{.data=(int32_t) 243.70},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 199.40},{.data=(int32_t) 243.70},
    {.cmd=VLC_OP_END}
};

/*path id=path1385*/
static data_mnemonic_t ClockDial_path_30_data[] = {
    {.cmd=VLC_OP_MOVE}, {.data=(int32_t) 229.90},{.data=(int32_t) 246.30},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 227.30},{.data=(int32_t) 248.60},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 226.40},{.data=(int32_t) 247.30},{.data=(int32_t) 225.40},{.data=(int32_t) 246.70},{.data=(int32_t) 224.50},{.data=(int32_t) 246.70},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 224.00},{.data=(int32_t) 246.70},{.data=(int32_t) 223.70},{.data=(int32_t) 246.80},{.data=(int32_t) 223.40},{.data=(int32_t) 247.10},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 223.10},{.data=(int32_t) 247.30},{.data=(int32_t) 223.00},{.data=(int32_t) 247.60},{.data=(int32_t) 223.00},{.data=(int32_t) 247.90},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 223.00},{.data=(int32_t) 248.20},{.data=(int32_t) 223.10},{.data=(int32_t) 248.50},{.data=(int32_t) 223.30},{.data=(int32_t) 248.80},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 223.60},{.data=(int32_t) 249.20},{.data=(int32_t) 224.50},{.data=(int32_t) 250.00},{.data=(int32_t) 225.90},{.data=(int32_t) 251.20},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 227.30},{.data=(int32_t) 252.30},{.data=(int32_t) 228.10},{.data=(int32_t) 253.00},{.data=(int32_t) 228.40},{.data=(int32_t) 253.30},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 229.10},{.data=(int32_t) 254.00},{.data=(int32_t) 229.60},{.data=(int32_t) 254.70},{.data=(int32_t) 229.90},{.data=(int32_t) 255.40},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 230.20},{.data=(int32_t) 256.10},{.data=(int32_t) 230.40},{.data=(int32_t) 256.80},{.data=(int32_t) 230.40},{.data=(int32_t) 257.60},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 230.40},{.data=(int32_t) 259.10},{.data=(int32_t) 229.90},{.data=(int32_t) 260.40},{.data=(int32_t) 228.80},{.data=(int32_t) 261.40},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 227.70},{.data=(int32_t) 262.40},{.data=(int32_t) 226.40},{.data=(int32_t) 262.90},{.data=(int32_t) 224.70},{.data=(int32_t) 262.90},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 223.40},{.data=(int32_t) 262.90},{.data=(int32_t) 222.20},{.data=(int32_t) 262.60},{.data=(int32_t) 221.20},{.data=(int32_t) 261.90},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 220.20},{.data=(int32_t) 261.20},{.data=(int32_t) 219.40},{.data=(int32_t) 260.20},{.data=(int32_t) 218.70},{.data=(int32_t) 258.80},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 221.70},{.data=(int32_t) 257.00},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 222.60},{.data=(int32_t) 258.60},{.data=(int32_t) 223.60},{.data=(int32_t) 259.50},{.data=(int32_t) 224.80},{.data=(int32_t) 259.50},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 225.40},{.data=(int32_t) 259.50},{.data=(int32_t) 225.90},{.data=(int32_t) 259.30},{.data=(int32_t) 226.30},{.data=(int32_t) 259.00},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 226.70},{.data=(int32_t) 258.60},{.data=(int32_t) 226.90},{.data=(int32_t) 258.20},{.data=(int32_t) 226.90},{.data=(int32_t) 257.80},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 226.90},{.data=(int32_t) 257.40},{.data=(int32_t) 226.70},{.data=(int32_t) 257.00},{.data=(int32_t) 226.40},{.data=(int32_t) 256.50},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 226.10},{.data=(int32_t) 256.10},{.data=(int32_t) 225.40},{.data=(int32_t) 255.40},{.data=(int32_t) 224.30},{.data=(int32_t) 254.60},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 222.30},{.data=(int32_t) 252.90},{.data=(int32_t) 221.00},{.data=(int32_t) 251.70},{.data=(int32_t) 220.40},{.data=(int32_t) 250.80},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 219.80},{.data=(int32_t) 249.90},{.data=(int32_t) 219.50},{.data=(int32_t) 249.00},{.data=(int32_t) 219.50},{.data=(int32_t) 248.10},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 219.50},{.data=(int32_t) 246.80},{.data=(int32_t) 220.00},{.data=(int32_t) 245.70},{.data=(int32_t) 221.00},{.data=(int32_t) 244.80},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 222.00},{.data=(int32_t) 243.90},{.data=(int32_t) 223.20},{.data=(int32_t) 243.40},{.data=(int32_t) 224.70},{.data=(int32_t) 243.40},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 225.60},{.data=(int32_t) 243.40},{.data=(int32_t) 226.50},{.data=(int32_t) 243.60},{.data=(int32_t) 227.40},{.data=(int32_t) 244.00},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 228.10},{.data=(int32_t) 244.30},{.data=(int32_t) 229.00},{.data=(int32_t) 245.10},{.data=(int32_t) 229.90},{.data=(int32_t) 246.30},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 229.90},{.data=(int32_t) 246.30},
    {.cmd=VLC_OP_END}
};

/*path id=path1387*/
static data_mnemonic_t ClockDial_path_31_data[] = {
    {.cmd=VLC_OP_MOVE}, {.data=(int32_t) 233.60},{.data=(int32_t) 243.70},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 237.40},{.data=(int32_t) 243.70},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 239.40},{.data=(int32_t) 243.70},{.data=(int32_t) 240.90},{.data=(int32_t) 243.90},{.data=(int32_t) 241.80},{.data=(int32_t) 244.30},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 242.70},{.data=(int32_t) 244.70},{.data=(int32_t) 243.40},{.data=(int32_t) 245.30},{.data=(int32_t) 243.90},{.data=(int32_t) 246.10},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 244.40},{.data=(int32_t) 246.90},{.data=(int32_t) 244.70},{.data=(int32_t) 247.90},{.data=(int32_t) 244.70},{.data=(int32_t) 249.10},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 244.70},{.data=(int32_t) 250.40},{.data=(int32_t) 244.40},{.data=(int32_t) 251.50},{.data=(int32_t) 243.70},{.data=(int32_t) 252.30},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 243.00},{.data=(int32_t) 253.20},{.data=(int32_t) 242.10},{.data=(int32_t) 253.80},{.data=(int32_t) 240.90},{.data=(int32_t) 254.10},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 240.20},{.data=(int32_t) 254.30},{.data=(int32_t) 239.00},{.data=(int32_t) 254.40},{.data=(int32_t) 237.20},{.data=(int32_t) 254.40},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 237.20},{.data=(int32_t) 262.30},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 233.60},{.data=(int32_t) 262.30},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 233.60},{.data=(int32_t) 243.70},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 233.60},{.data=(int32_t) 243.70},
    {.cmd=VLC_OP_MOVE}, {.data=(int32_t) 237.20},{.data=(int32_t) 251.00},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 238.30},{.data=(int32_t) 251.00},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 239.20},{.data=(int32_t) 251.00},{.data=(int32_t) 239.80},{.data=(int32_t) 250.90},{.data=(int32_t) 240.20},{.data=(int32_t) 250.80},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 240.50},{.data=(int32_t) 250.70},{.data=(int32_t) 240.80},{.data=(int32_t) 250.50},{.data=(int32_t) 241.00},{.data=(int32_t) 250.20},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 241.20},{.data=(int32_t) 249.90},{.data=(int32_t) 241.30},{.data=(int32_t) 249.50},{.data=(int32_t) 241.30},{.data=(int32_t) 249.10},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 241.30},{.data=(int32_t) 248.40},{.data=(int32_t) 241.00},{.data=(int32_t) 247.90},{.data=(int32_t) 240.50},{.data=(int32_t) 247.50},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 240.10},{.data=(int32_t) 247.30},{.data=(int32_t) 239.30},{.data=(int32_t) 247.10},{.data=(int32_t) 238.20},{.data=(int32_t) 247.10},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 237.20},{.data=(int32_t) 247.10},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 237.20},{.data=(int32_t) 251.00},
    {.cmd=VLC_OP_END}
};

/*path id=path1389*/
static data_mnemonic_t ClockDial_path_32_data[] = {
    {.cmd=VLC_OP_MOVE}, {.data=(int32_t) 248.20},{.data=(int32_t) 243.70},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 258.40},{.data=(int32_t) 243.70},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 258.40},{.data=(int32_t) 247.20},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 251.70},{.data=(int32_t) 247.20},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 251.70},{.data=(int32_t) 250.60},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 258.40},{.data=(int32_t) 250.60},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 258.40},{.data=(int32_t) 254.00},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 251.70},{.data=(int32_t) 254.00},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 251.70},{.data=(int32_t) 258.90},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 258.40},{.data=(int32_t) 258.90},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 258.40},{.data=(int32_t) 262.40},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 248.20},{.data=(int32_t) 262.40},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 248.20},{.data=(int32_t) 243.70},
    {.cmd=VLC_OP_END}
};

/*path id=path1391*/
static data_mnemonic_t ClockDial_path_33_data[] = {
    {.cmd=VLC_OP_MOVE}, {.data=(int32_t) 278.90},{.data=(int32_t) 247.00},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 276.40},{.data=(int32_t) 249.40},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 274.70},{.data=(int32_t) 247.60},{.data=(int32_t) 272.80},{.data=(int32_t) 246.70},{.data=(int32_t) 270.70},{.data=(int32_t) 246.70},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 268.90},{.data=(int32_t) 246.70},{.data=(int32_t) 267.40},{.data=(int32_t) 247.30},{.data=(int32_t) 266.20},{.data=(int32_t) 248.50},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 265.00},{.data=(int32_t) 249.70},{.data=(int32_t) 264.40},{.data=(int32_t) 251.20},{.data=(int32_t) 264.40},{.data=(int32_t) 253.00},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 264.40},{.data=(int32_t) 254.20},{.data=(int32_t) 264.70},{.data=(int32_t) 255.30},{.data=(int32_t) 265.20},{.data=(int32_t) 256.30},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 265.70},{.data=(int32_t) 257.30},{.data=(int32_t) 266.50},{.data=(int32_t) 258.00},{.data=(int32_t) 267.50},{.data=(int32_t) 258.60},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 268.50},{.data=(int32_t) 259.10},{.data=(int32_t) 269.60},{.data=(int32_t) 259.40},{.data=(int32_t) 270.80},{.data=(int32_t) 259.40},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 271.80},{.data=(int32_t) 259.40},{.data=(int32_t) 272.80},{.data=(int32_t) 259.20},{.data=(int32_t) 273.60},{.data=(int32_t) 258.80},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 274.50},{.data=(int32_t) 258.40},{.data=(int32_t) 275.40},{.data=(int32_t) 257.70},{.data=(int32_t) 276.40},{.data=(int32_t) 256.70},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 278.80},{.data=(int32_t) 259.20},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 277.40},{.data=(int32_t) 260.50},{.data=(int32_t) 276.10},{.data=(int32_t) 261.50},{.data=(int32_t) 274.90},{.data=(int32_t) 262.00},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 273.70},{.data=(int32_t) 262.50},{.data=(int32_t) 272.30},{.data=(int32_t) 262.80},{.data=(int32_t) 270.70},{.data=(int32_t) 262.80},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 267.80},{.data=(int32_t) 262.80},{.data=(int32_t) 265.40},{.data=(int32_t) 261.90},{.data=(int32_t) 263.60},{.data=(int32_t) 260.00},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 261.80},{.data=(int32_t) 258.20},{.data=(int32_t) 260.80},{.data=(int32_t) 255.80},{.data=(int32_t) 260.80},{.data=(int32_t) 252.90},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 260.80},{.data=(int32_t) 251.00},{.data=(int32_t) 261.20},{.data=(int32_t) 249.40},{.data=(int32_t) 262.10},{.data=(int32_t) 247.90},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 262.90},{.data=(int32_t) 246.50},{.data=(int32_t) 264.20},{.data=(int32_t) 245.30},{.data=(int32_t) 265.70},{.data=(int32_t) 244.40},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 267.30},{.data=(int32_t) 243.50},{.data=(int32_t) 269.00},{.data=(int32_t) 243.10},{.data=(int32_t) 270.80},{.data=(int32_t) 243.10},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 272.30},{.data=(int32_t) 243.10},{.data=(int32_t) 273.80},{.data=(int32_t) 243.40},{.data=(int32_t) 275.20},{.data=(int32_t) 244.10},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 276.60},{.data=(int32_t) 244.90},{.data=(int32_t) 277.80},{.data=(int32_t) 245.80},{.data=(int32_t) 278.90},{.data=(int32_t) 247.00},
    {.cmd=VLC_OP_LINE}, {.data=(int32_t) 278.90},{.data=(int32_t) 247.00},
    {.cmd=VLC_OP_END}
};

/*path id=path819-1*/
static data_mnemonic_t ClockDial_path_34_data[] = {
    {.cmd=VLC_OP_MOVE}, {.data=(int32_t) 127.00},{.data=(int32_t) 80.30},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 129.60},{.data=(int32_t) 80.30},{.data=(int32_t) 131.70},{.data=(int32_t) 82.40},{.data=(int32_t) 131.70},{.data=(int32_t) 85.00},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 131.70},{.data=(int32_t) 87.60},{.data=(int32_t) 129.60},{.data=(int32_t) 89.70},{.data=(int32_t) 127.00},{.data=(int32_t) 89.70},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 124.40},{.data=(int32_t) 89.70},{.data=(int32_t) 122.30},{.data=(int32_t) 87.60},{.data=(int32_t) 122.30},{.data=(int32_t) 85.00},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 122.30},{.data=(int32_t) 82.40},{.data=(int32_t) 124.40},{.data=(int32_t) 80.30},{.data=(int32_t) 127.00},{.data=(int32_t) 80.30},
    {.cmd=VLC_OP_END}
};

/*path id=path819-9*/
static data_mnemonic_t ClockDial_path_35_data[] = {
    {.cmd=VLC_OP_MOVE}, {.data=(int32_t) 85.00},{.data=(int32_t) 130.30},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 87.60},{.data=(int32_t) 130.30},{.data=(int32_t) 89.70},{.data=(int32_t) 132.40},{.data=(int32_t) 89.70},{.data=(int32_t) 135.00},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 89.70},{.data=(int32_t) 137.60},{.data=(int32_t) 87.60},{.data=(int32_t) 139.70},{.data=(int32_t) 85.00},{.data=(int32_t) 139.70},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 82.40},{.data=(int32_t) 139.70},{.data=(int32_t) 80.30},{.data=(int32_t) 137.60},{.data=(int32_t) 80.30},{.data=(int32_t) 135.00},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 80.30},{.data=(int32_t) 132.40},{.data=(int32_t) 82.40},{.data=(int32_t) 130.30},{.data=(int32_t) 85.00},{.data=(int32_t) 130.30},
    {.cmd=VLC_OP_END}
};

/*path id=path819-3*/
static data_mnemonic_t ClockDial_path_36_data[] = {
    {.cmd=VLC_OP_MOVE}, {.data=(int32_t) 61.00},{.data=(int32_t) 191.80},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 63.60},{.data=(int32_t) 191.80},{.data=(int32_t) 65.70},{.data=(int32_t) 193.90},{.data=(int32_t) 65.70},{.data=(int32_t) 196.50},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 65.70},{.data=(int32_t) 199.10},{.data=(int32_t) 63.60},{.data=(int32_t) 201.20},{.data=(int32_t) 61.00},{.data=(int32_t) 201.20},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 58.40},{.data=(int32_t) 201.20},{.data=(int32_t) 56.30},{.data=(int32_t) 199.10},{.data=(int32_t) 56.30},{.data=(int32_t) 196.50},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 56.30},{.data=(int32_t) 193.90},{.data=(int32_t) 58.40},{.data=(int32_t) 191.80},{.data=(int32_t) 61.00},{.data=(int32_t) 191.80},
    {.cmd=VLC_OP_END}
};

/*path id=path819-3-9*/
static data_mnemonic_t ClockDial_path_37_data[] = {
    {.cmd=VLC_OP_MOVE}, {.data=(int32_t) 339.00},{.data=(int32_t) 191.80},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 341.60},{.data=(int32_t) 191.80},{.data=(int32_t) 343.70},{.data=(int32_t) 193.90},{.data=(int32_t) 343.70},{.data=(int32_t) 196.50},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 343.70},{.data=(int32_t) 199.10},{.data=(int32_t) 341.60},{.data=(int32_t) 201.20},{.data=(int32_t) 339.00},{.data=(int32_t) 201.20},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 336.40},{.data=(int32_t) 201.20},{.data=(int32_t) 334.30},{.data=(int32_t) 199.10},{.data=(int32_t) 334.30},{.data=(int32_t) 196.50},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 334.30},{.data=(int32_t) 193.90},{.data=(int32_t) 336.40},{.data=(int32_t) 191.80},{.data=(int32_t) 339.00},{.data=(int32_t) 191.80},
    {.cmd=VLC_OP_END}
};

/*path id=path819-9-9*/
static data_mnemonic_t ClockDial_path_38_data[] = {
    {.cmd=VLC_OP_MOVE}, {.data=(int32_t) 315.00},{.data=(int32_t) 130.30},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 317.60},{.data=(int32_t) 130.30},{.data=(int32_t) 319.70},{.data=(int32_t) 132.40},{.data=(int32_t) 319.70},{.data=(int32_t) 135.00},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 319.70},{.data=(int32_t) 137.60},{.data=(int32_t) 317.60},{.data=(int32_t) 139.70},{.data=(int32_t) 315.00},{.data=(int32_t) 139.70},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 312.40},{.data=(int32_t) 139.70},{.data=(int32_t) 310.30},{.data=(int32_t) 137.60},{.data=(int32_t) 310.30},{.data=(int32_t) 135.00},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 310.30},{.data=(int32_t) 132.40},{.data=(int32_t) 312.40},{.data=(int32_t) 130.30},{.data=(int32_t) 315.00},{.data=(int32_t) 130.30},
    {.cmd=VLC_OP_END}
};

/*path id=path819-1-7*/
static data_mnemonic_t ClockDial_path_39_data[] = {
    {.cmd=VLC_OP_MOVE}, {.data=(int32_t) 273.00},{.data=(int32_t) 80.30},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 275.60},{.data=(int32_t) 80.30},{.data=(int32_t) 277.70},{.data=(int32_t) 82.40},{.data=(int32_t) 277.70},{.data=(int32_t) 85.00},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 277.70},{.data=(int32_t) 87.60},{.data=(int32_t) 275.60},{.data=(int32_t) 89.70},{.data=(int32_t) 273.00},{.data=(int32_t) 89.70},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 270.40},{.data=(int32_t) 89.70},{.data=(int32_t) 268.30},{.data=(int32_t) 87.60},{.data=(int32_t) 268.30},{.data=(int32_t) 85.00},
    {.cmd=VLC_OP_CUBIC}, {.data=(int32_t) 268.30},{.data=(int32_t) 82.40},{.data=(int32_t) 270.40},{.data=(int32_t) 80.30},{.data=(int32_t) 273.00},{.data=(int32_t) 80.30},
    {.cmd=VLC_OP_END}
};

hybridPath_t ClockDial_hybrid_path[] = {
    { .fillType = FILL_CONSTANT, .pathType = VG_LITE_DRAW_FILL_PATH },
    { .fillType = NO_FILL_MODE, .pathType = VG_LITE_DRAW_ZERO },
    { .fillType = FILL_CONSTANT, .pathType = VG_LITE_DRAW_FILL_PATH },
    { .fillType = NO_FILL_MODE, .pathType = VG_LITE_DRAW_ZERO },
    { .fillType = FILL_CONSTANT, .pathType = VG_LITE_DRAW_FILL_PATH },
    { .fillType = NO_FILL_MODE, .pathType = VG_LITE_DRAW_ZERO },
    { .fillType = FILL_CONSTANT, .pathType = VG_LITE_DRAW_FILL_PATH },
    { .fillType = NO_FILL_MODE, .pathType = VG_LITE_DRAW_ZERO },
    { .fillType = FILL_CONSTANT, .pathType = VG_LITE_DRAW_FILL_PATH },
    { .fillType = NO_FILL_MODE, .pathType = VG_LITE_DRAW_ZERO },
    { .fillType = FILL_CONSTANT, .pathType = VG_LITE_DRAW_FILL_PATH },
    { .fillType = NO_FILL_MODE, .pathType = VG_LITE_DRAW_ZERO },
    { .fillType = FILL_CONSTANT, .pathType = VG_LITE_DRAW_FILL_PATH },
    { .fillType = NO_FILL_MODE, .pathType = VG_LITE_DRAW_ZERO },
    { .fillType = FILL_CONSTANT, .pathType = VG_LITE_DRAW_FILL_PATH },
    { .fillType = NO_FILL_MODE, .pathType = VG_LITE_DRAW_ZERO },
    { .fillType = FILL_CONSTANT, .pathType = VG_LITE_DRAW_FILL_PATH },
    { .fillType = NO_FILL_MODE, .pathType = VG_LITE_DRAW_ZERO },
    { .fillType = FILL_CONSTANT, .pathType = VG_LITE_DRAW_FILL_PATH },
    { .fillType = NO_FILL_MODE, .pathType = VG_LITE_DRAW_ZERO },
    { .fillType = FILL_CONSTANT, .pathType = VG_LITE_DRAW_FILL_PATH },
    { .fillType = NO_FILL_MODE, .pathType = VG_LITE_DRAW_ZERO },
    { .fillType = FILL_CONSTANT, .pathType = VG_LITE_DRAW_FILL_PATH },
    { .fillType = NO_FILL_MODE, .pathType = VG_LITE_DRAW_ZERO },
    { .fillType = FILL_CONSTANT, .pathType = VG_LITE_DRAW_FILL_PATH },
    { .fillType = NO_FILL_MODE, .pathType = VG_LITE_DRAW_ZERO },
    { .fillType = FILL_CONSTANT, .pathType = VG_LITE_DRAW_FILL_PATH },
    { .fillType = NO_FILL_MODE, .pathType = VG_LITE_DRAW_ZERO },
    { .fillType = FILL_CONSTANT, .pathType = VG_LITE_DRAW_FILL_PATH },
    { .fillType = NO_FILL_MODE, .pathType = VG_LITE_DRAW_ZERO },
    { .fillType = FILL_CONSTANT, .pathType = VG_LITE_DRAW_FILL_PATH },
    { .fillType = NO_FILL_MODE, .pathType = VG_LITE_DRAW_ZERO },
    { .fillType = FILL_CONSTANT, .pathType = VG_LITE_DRAW_FILL_PATH },
    { .fillType = NO_FILL_MODE, .pathType = VG_LITE_DRAW_ZERO },
    { .fillType = FILL_CONSTANT, .pathType = VG_LITE_DRAW_FILL_PATH },
    { .fillType = NO_FILL_MODE, .pathType = VG_LITE_DRAW_ZERO },
    { .fillType = FILL_CONSTANT, .pathType = VG_LITE_DRAW_FILL_PATH },
    { .fillType = NO_FILL_MODE, .pathType = VG_LITE_DRAW_ZERO },
    { .fillType = FILL_CONSTANT, .pathType = VG_LITE_DRAW_FILL_PATH },
    { .fillType = NO_FILL_MODE, .pathType = VG_LITE_DRAW_ZERO },
    { .fillType = FILL_CONSTANT, .pathType = VG_LITE_DRAW_FILL_PATH },
    { .fillType = NO_FILL_MODE, .pathType = VG_LITE_DRAW_ZERO },
    { .fillType = FILL_CONSTANT, .pathType = VG_LITE_DRAW_FILL_PATH },
    { .fillType = NO_FILL_MODE, .pathType = VG_LITE_DRAW_ZERO },
    { .fillType = FILL_CONSTANT, .pathType = VG_LITE_DRAW_FILL_PATH },
    { .fillType = NO_FILL_MODE, .pathType = VG_LITE_DRAW_ZERO },
    { .fillType = FILL_CONSTANT, .pathType = VG_LITE_DRAW_FILL_PATH },
    { .fillType = NO_FILL_MODE, .pathType = VG_LITE_DRAW_ZERO },
    { .fillType = FILL_CONSTANT, .pathType = VG_LITE_DRAW_FILL_PATH },
    { .fillType = NO_FILL_MODE, .pathType = VG_LITE_DRAW_ZERO },
    { .fillType = FILL_CONSTANT, .pathType = VG_LITE_DRAW_FILL_PATH },
    { .fillType = NO_FILL_MODE, .pathType = VG_LITE_DRAW_ZERO },
    { .fillType = FILL_CONSTANT, .pathType = VG_LITE_DRAW_FILL_PATH },
    { .fillType = NO_FILL_MODE, .pathType = VG_LITE_DRAW_ZERO },
    { .fillType = FILL_CONSTANT, .pathType = VG_LITE_DRAW_FILL_PATH },
    { .fillType = NO_FILL_MODE, .pathType = VG_LITE_DRAW_ZERO },
    { .fillType = FILL_CONSTANT, .pathType = VG_LITE_DRAW_FILL_PATH },
    { .fillType = NO_FILL_MODE, .pathType = VG_LITE_DRAW_ZERO },
    { .fillType = FILL_CONSTANT, .pathType = VG_LITE_DRAW_FILL_PATH },
    { .fillType = NO_FILL_MODE, .pathType = VG_LITE_DRAW_ZERO },
    { .fillType = FILL_CONSTANT, .pathType = VG_LITE_DRAW_FILL_PATH },
    { .fillType = NO_FILL_MODE, .pathType = VG_LITE_DRAW_ZERO },
    { .fillType = FILL_CONSTANT, .pathType = VG_LITE_DRAW_FILL_PATH },
    { .fillType = NO_FILL_MODE, .pathType = VG_LITE_DRAW_ZERO },
    { .fillType = FILL_CONSTANT, .pathType = VG_LITE_DRAW_FILL_PATH },
    { .fillType = NO_FILL_MODE, .pathType = VG_LITE_DRAW_ZERO },
    { .fillType = FILL_CONSTANT, .pathType = VG_LITE_DRAW_FILL_PATH },
    { .fillType = NO_FILL_MODE, .pathType = VG_LITE_DRAW_ZERO },
    { .fillType = FILL_CONSTANT, .pathType = VG_LITE_DRAW_FILL_PATH },
    { .fillType = NO_FILL_MODE, .pathType = VG_LITE_DRAW_ZERO },
    { .fillType = FILL_CONSTANT, .pathType = VG_LITE_DRAW_FILL_PATH },
    { .fillType = NO_FILL_MODE, .pathType = VG_LITE_DRAW_ZERO },
    { .fillType = FILL_CONSTANT, .pathType = VG_LITE_DRAW_FILL_PATH },
    { .fillType = NO_FILL_MODE, .pathType = VG_LITE_DRAW_ZERO },
    { .fillType = FILL_CONSTANT, .pathType = VG_LITE_DRAW_FILL_PATH },
    { .fillType = NO_FILL_MODE, .pathType = VG_LITE_DRAW_ZERO },
    { .fillType = FILL_CONSTANT, .pathType = VG_LITE_DRAW_FILL_PATH },
    { .fillType = NO_FILL_MODE, .pathType = VG_LITE_DRAW_ZERO },

};

static vg_lite_fill_t ClockDial_fill_rule[] = {
VG_LITE_FILL_EVEN_ODD,
VG_LITE_FILL_EVEN_ODD,
VG_LITE_FILL_EVEN_ODD,
VG_LITE_FILL_EVEN_ODD,
VG_LITE_FILL_EVEN_ODD,
VG_LITE_FILL_EVEN_ODD,
VG_LITE_FILL_EVEN_ODD,
VG_LITE_FILL_EVEN_ODD,
VG_LITE_FILL_EVEN_ODD,
VG_LITE_FILL_EVEN_ODD,
VG_LITE_FILL_EVEN_ODD,
VG_LITE_FILL_EVEN_ODD,
VG_LITE_FILL_EVEN_ODD,
VG_LITE_FILL_EVEN_ODD,
VG_LITE_FILL_EVEN_ODD,
VG_LITE_FILL_EVEN_ODD,
VG_LITE_FILL_EVEN_ODD,
VG_LITE_FILL_EVEN_ODD,
VG_LITE_FILL_EVEN_ODD,
VG_LITE_FILL_EVEN_ODD,
VG_LITE_FILL_EVEN_ODD,
VG_LITE_FILL_EVEN_ODD,
VG_LITE_FILL_EVEN_ODD,
VG_LITE_FILL_EVEN_ODD,
VG_LITE_FILL_EVEN_ODD,
VG_LITE_FILL_EVEN_ODD,
VG_LITE_FILL_EVEN_ODD,
VG_LITE_FILL_EVEN_ODD,
VG_LITE_FILL_EVEN_ODD,
VG_LITE_FILL_EVEN_ODD,
VG_LITE_FILL_EVEN_ODD,
VG_LITE_FILL_EVEN_ODD,
VG_LITE_FILL_EVEN_ODD,
VG_LITE_FILL_EVEN_ODD,
VG_LITE_FILL_EVEN_ODD,
VG_LITE_FILL_EVEN_ODD,
VG_LITE_FILL_EVEN_ODD,
VG_LITE_FILL_EVEN_ODD,
VG_LITE_FILL_EVEN_ODD
};

static gradient_mode_t ClockDial_gradient_info = {
    .linearGrads = NULL,
    .radialGrads = NULL,
    .hybridPath = ClockDial_hybrid_path,
    .fillRule = ClockDial_fill_rule
};

static float ClockDial_transform_matrix[] = {
1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f
};

static image_info_t ClockDial = {
    .image_name ="ClockDial",
    .image_size = {400, 400},
    .data_format = VG_LITE_S32,
    .transform = ClockDial_transform_matrix,
    .path_count = 39,
    .stroke_info = NULL,
    .paths_info = {
        {.path_length = sizeof(ClockDial_path_1_data), .path_data=(int32_t*)ClockDial_path_1_data, .end_path_flag=0, .bounding_box = {0.00, 0.00, 400.00, 400.00} },
        {.path_length = sizeof(ClockDial_path_2_data), .path_data=(int32_t*)ClockDial_path_2_data, .end_path_flag=0, .bounding_box = {195.30, 56.30, 204.70, 65.70} },
        {.path_length = sizeof(ClockDial_path_3_data), .path_data=(int32_t*)ClockDial_path_3_data, .end_path_flag=0, .bounding_box = {105.10, 293.40, 294.90, 350.80} },
        {.path_length = sizeof(ClockDial_path_4_data), .path_data=(int32_t*)ClockDial_path_4_data, .end_path_flag=0, .bounding_box = {164.30, 313.00, 177.50, 331.00} },
        {.path_length = sizeof(ClockDial_path_5_data), .path_data=(int32_t*)ClockDial_path_5_data, .end_path_flag=0, .bounding_box = {180.20, 317.90, 191.20, 331.00} },
        {.path_length = sizeof(ClockDial_path_6_data), .path_data=(int32_t*)ClockDial_path_6_data, .end_path_flag=0, .bounding_box = {193.40, 313.20, 200.10, 330.60} },
        {.path_length = sizeof(ClockDial_path_7_data), .path_data=(int32_t*)ClockDial_path_7_data, .end_path_flag=0, .bounding_box = {200.60, 313.20, 207.30, 330.60} },
        {.path_length = sizeof(ClockDial_path_8_data), .path_data=(int32_t*)ClockDial_path_8_data, .end_path_flag=0, .bounding_box = {208.50, 317.60, 221.80, 331.00} },
        {.path_length = sizeof(ClockDial_path_9_data), .path_data=(int32_t*)ClockDial_path_9_data, .end_path_flag=0, .bounding_box = {224.20, 317.50, 235.60, 330.60} },
        {.path_length = sizeof(ClockDial_path_10_data), .path_data=(int32_t*)ClockDial_path_10_data, .end_path_flag=0, .bounding_box = {13.90, 190.40, 26.40, 208.90} },
        {.path_length = sizeof(ClockDial_path_11_data), .path_data=(int32_t*)ClockDial_path_11_data, .end_path_flag=0, .bounding_box = {28.50, 190.40, 39.60, 208.90} },
        {.path_length = sizeof(ClockDial_path_12_data), .path_data=(int32_t*)ClockDial_path_12_data, .end_path_flag=0, .bounding_box = {38.90, 106.80, 45.80, 124.50} },
        {.path_length = sizeof(ClockDial_path_13_data), .path_data=(int32_t*)ClockDial_path_13_data, .end_path_flag=0, .bounding_box = {51.20, 106.30, 63.70, 124.80} },
        {.path_length = sizeof(ClockDial_path_14_data), .path_data=(int32_t*)ClockDial_path_14_data, .end_path_flag=0, .bounding_box = {96.50, 48.10, 103.40, 65.80} },
        {.path_length = sizeof(ClockDial_path_15_data), .path_data=(int32_t*)ClockDial_path_15_data, .end_path_flag=0, .bounding_box = {110.30, 48.10, 117.20, 65.80} },
        {.path_length = sizeof(ClockDial_path_16_data), .path_data=(int32_t*)ClockDial_path_16_data, .end_path_flag=0, .bounding_box = {188.00, 21.60, 194.90, 39.30} },
        {.path_length = sizeof(ClockDial_path_17_data), .path_data=(int32_t*)ClockDial_path_17_data, .end_path_flag=0, .bounding_box = {200.10, 21.20, 212.30, 39.40} },
        {.path_length = sizeof(ClockDial_path_18_data), .path_data=(int32_t*)ClockDial_path_18_data, .end_path_flag=0, .bounding_box = {283.10, 47.20, 295.60, 65.70} },
        {.path_length = sizeof(ClockDial_path_19_data), .path_data=(int32_t*)ClockDial_path_19_data, .end_path_flag=0, .bounding_box = {298.40, 47.60, 305.30, 65.30} },
        {.path_length = sizeof(ClockDial_path_20_data), .path_data=(int32_t*)ClockDial_path_20_data, .end_path_flag=0, .bounding_box = {335.80, 106.30, 348.30, 124.80} },
        {.path_length = sizeof(ClockDial_path_21_data), .path_data=(int32_t*)ClockDial_path_21_data, .end_path_flag=0, .bounding_box = {349.40, 106.40, 361.60, 124.60} },
        {.path_length = sizeof(ClockDial_path_22_data), .path_data=(int32_t*)ClockDial_path_22_data, .end_path_flag=0, .bounding_box = {358.60, 190.40, 371.10, 208.90} },
        {.path_length = sizeof(ClockDial_path_23_data), .path_data=(int32_t*)ClockDial_path_23_data, .end_path_flag=0, .bounding_box = {373.10, 190.40, 384.70, 209.00} },
        {.path_length = sizeof(ClockDial_path_24_data), .path_data=(int32_t*)ClockDial_path_24_data, .end_path_flag=0, .bounding_box = {121.10, 243.70, 138.00, 262.40} },
        {.path_length = sizeof(ClockDial_path_25_data), .path_data=(int32_t*)ClockDial_path_25_data, .end_path_flag=0, .bounding_box = {139.90, 243.30, 159.30, 262.90} },
        {.path_length = sizeof(ClockDial_path_26_data), .path_data=(int32_t*)ClockDial_path_26_data, .end_path_flag=0, .bounding_box = {169.80, 243.70, 178.60, 262.40} },
        {.path_length = sizeof(ClockDial_path_27_data), .path_data=(int32_t*)ClockDial_path_27_data, .end_path_flag=0, .bounding_box = {181.00, 243.70, 184.50, 262.40} },
        {.path_length = sizeof(ClockDial_path_28_data), .path_data=(int32_t*)ClockDial_path_28_data, .end_path_flag=0, .bounding_box = {186.70, 243.70, 197.00, 262.40} },
        {.path_length = sizeof(ClockDial_path_29_data), .path_data=(int32_t*)ClockDial_path_29_data, .end_path_flag=0, .bounding_box = {199.40, 243.70, 209.70, 262.40} },
        {.path_length = sizeof(ClockDial_path_30_data), .path_data=(int32_t*)ClockDial_path_30_data, .end_path_flag=0, .bounding_box = {219.50, 243.40, 230.40, 262.90} },
        {.path_length = sizeof(ClockDial_path_31_data), .path_data=(int32_t*)ClockDial_path_31_data, .end_path_flag=0, .bounding_box = {233.60, 243.70, 244.70, 262.30} },
        {.path_length = sizeof(ClockDial_path_32_data), .path_data=(int32_t*)ClockDial_path_32_data, .end_path_flag=0, .bounding_box = {248.20, 243.70, 258.40, 262.40} },
        {.path_length = sizeof(ClockDial_path_33_data), .path_data=(int32_t*)ClockDial_path_33_data, .end_path_flag=0, .bounding_box = {260.80, 243.10, 278.90, 262.80} },
        {.path_length = sizeof(ClockDial_path_34_data), .path_data=(int32_t*)ClockDial_path_34_data, .end_path_flag=0, .bounding_box = {122.30, 80.30, 131.70, 89.70} },
        {.path_length = sizeof(ClockDial_path_35_data), .path_data=(int32_t*)ClockDial_path_35_data, .end_path_flag=0, .bounding_box = {80.30, 130.30, 89.70, 139.70} },
        {.path_length = sizeof(ClockDial_path_36_data), .path_data=(int32_t*)ClockDial_path_36_data, .end_path_flag=0, .bounding_box = {56.30, 191.80, 65.70, 201.20} },
        {.path_length = sizeof(ClockDial_path_37_data), .path_data=(int32_t*)ClockDial_path_37_data, .end_path_flag=0, .bounding_box = {334.30, 191.80, 343.70, 201.20} },
        {.path_length = sizeof(ClockDial_path_38_data), .path_data=(int32_t*)ClockDial_path_38_data, .end_path_flag=0, .bounding_box = {310.30, 130.30, 319.70, 139.70} },
        {.path_length = sizeof(ClockDial_path_39_data), .path_data=(int32_t*)ClockDial_path_39_data, .end_path_flag=0, .bounding_box = {268.30, 80.30, 277.70, 89.70} }
    },
};

uint32_t ClockDial_color_data[] = {
    0xffdf9600, 0xff41e8df, 0xff00ff40, 0xffffffff, 
    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 
    0xffffffff, 0xffdffdff, 0xffdffdff, 0xffdffdff, 
    0xffdffdff, 0xffdffdff, 0xffdffdff, 0xffdffdff, 
    0xffdffdff, 0xffdffdff, 0xffdffdff, 0xffdffdff, 
    0xffdffdff, 0xffdffdff, 0xffdffdff, 0xffdffdff, 
    0xffdffdff, 0xffdffdff, 0xffdffdff, 0xffdffdff, 
    0xffdffdff, 0xffdffdff, 0xffdffdff, 0xffdffdff, 
    0xffdffdff, 0xff41e8df, 0xff41e8df, 0xff41e8df, 
    0xff41e8df, 0xff41e8df, 0xff41e8df
};

