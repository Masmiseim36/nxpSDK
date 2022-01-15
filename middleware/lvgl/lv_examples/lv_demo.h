/**
 * @file lv_examples.h
 *
 */

#ifndef LV_DEMO_H
#define LV_DEMO_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#if defined(LV_LVGL_H_INCLUDE_SIMPLE)
#include "lvgl.h"
#else
#include "../lvgl/lvgl.h"
#endif

#if defined(LV_DEMO_CONF_PATH)
#define __LV_TO_STR_AUX(x) #x
#define __LV_TO_STR(x) __LV_TO_STR_AUX(x)
#include __LV_TO_STR(LV_DEMO_CONF_PATH)
#undef __LV_TO_STR_AUX
#undef __LV_TO_STR
#elif defined(LV_DEMO_CONF_INCLUDE_SIMPLE)
#include "lv_demo_conf.h"
#else
#include "../lv_demo_conf.h"
#endif

#if LV_USE_DEMO_WIDGETS != 0
#include "src/lv_demo_widgets/lv_demo_widgets.h"
#endif

#if LV_USE_DEMO_BENCHMARK != 0
#include "src/lv_demo_benchmark/lv_demo_benchmark.h"
#endif

#if LV_USE_DEMO_STRESS != 0
#include "src/lv_demo_stress/lv_demo_stress.h"
#endif

#if LV_USE_DEMO_KEYPAD_AND_ENCODER != 0
#include "src/lv_demo_keypad_encoder/lv_demo_keypad_encoder.h"
#endif

#if LV_USE_DEMO_MUSIC != 0
#include "src/lv_demo_music/lv_demo_music.h"
#endif

/*********************
 *      DEFINES
 *********************/
/*Test  lvgl version*/
#if LV_VERSION_CHECK(8, 0, 0) == 0
#error "lv_demo: Wrong lvgl version"
#endif

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/


/**********************
 *      MACROS
 **********************/


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*LV_DEMO_H*/
