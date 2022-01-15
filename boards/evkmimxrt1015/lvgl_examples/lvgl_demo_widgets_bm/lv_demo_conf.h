/**
 * @file lv_demo_conf.h
 *
 */
/*
 * COPY THIS FILE AS lv_demo_conf.h
 */

#ifndef LV_DEMO_CONF_H
#define LV_DEMO_CONF_H

/*******************
 * GENERAL SETTING
 *******************/
#define LV_EX_PRINTF   0 /*Enable printf-ing data*/
#define LV_EX_KEYBOARD 0 /*Add PC keyboard support to some examples (`lv_drivers` repository is required)*/
#define LV_EX_MOUSEWHEEL \
    0 /*Add 'encoder' (mouse wheel) support to some examples (`lv_drivers` repository is required)*/

/*********************
 * DEMO USAGE
 *********************/

/*Widgets demo*/
#define LV_USE_DEMO_WIDGETS 1
#if LV_USE_DEMO_WIDGETS != 0
#define LV_DEMO_WIDGETS_SLIDESHOW 0
#endif

#define LV_USE_DEMO_BENCHMARK 0

#define LV_USE_DEMO_STRESS 0

#define LV_USE_DEMO_KEYPAD_ENCODER 0

#define LV_USE_DEMO_PRINTER 0

#define LV_USE_EX_GET_STARTED 0

#define LV_USE_EX_STYLE 0

#define LV_USE_EX_WIDGETS 0

/*Music player demo*/
#define LV_USE_DEMO_MUSIC 0
#if LV_USE_DEMO_MUSIC
#define LV_DEMO_MUSIC_LANDSCAPE 0
#define LV_DEMO_MUSIC_LARGE 0
#define LV_DEMO_MUSIC_AUTO_PLAY 0
#endif

#endif /*LV_DEMO_CONF_H*/
