/**
 * @file lv_conf.h
 *
 */

#ifndef LV_CONF_H
#define LV_CONF_H
/* clang-format off */

#include <stdint.h>
#include "lvgl_support.h"

/*====================
   Graphical settings
 *====================*/

/* Color depth:
 * - 1:  1 byte per pixel
 * - 8:  RGB332
 * - 16: RGB565
 * - 32: ARGB8888
 */
#define LV_COLOR_DEPTH     16

/* Swap the 2 bytes of RGB565 color.
 * Useful if the display has a 8 bit interface (e.g. SPI)*/
#define LV_COLOR_16_SWAP   1

/* 1: Enable screen transparency.
 * Useful for OSD or other overlapping GUIs.
 * Requires `LV_COLOR_DEPTH = 32` colors and the screen's style should be modified: `style.body.opa = ...`*/
#define LV_COLOR_SCREEN_TRANSP    0

/*Images pixels with this color will not be drawn (if they are chroma keyed)*/
#define LV_COLOR_CHROMA_KEY    lv_color_hex(0x00ff00)         /*lv_color_hex(0x00ff00): pure green*/

/* Default display refresh period.
 * Can be changed in the display driver (`lv_disp_drv_t`).*/
#define LV_DISP_DEF_REFR_PERIOD      30      /*[ms]*/

/*=========================
   Memory manager settings
 *=========================*/

/* LittelvGL's internal memory manager's settings.
 * The graphical objects and other related data are stored here. */

/* 1: use custom malloc/free, 0: use the built-in `lv_mem_alloc` and `lv_mem_free` */
#define LV_MEM_CUSTOM 0
#if LV_MEM_CUSTOM == 0
/* Size of the memory used by `lv_mem_alloc` in bytes (>= 2kB)*/
#  define LV_MEM_SIZE    (32U * 1024U)

/* Set an address for the memory pool instead of allocating it as an array.
 * Can be in external SRAM too. */
#  define LV_MEM_ADR          0

#else       /*LV_MEM_CUSTOM*/
#  define LV_MEM_CUSTOM_INCLUDE "FreeRTOS.h"   /*Header for the dynamic memory function*/
#  define LV_MEM_CUSTOM_ALLOC   pvPortMalloc   /*Wrapper to malloc*/
#  define LV_MEM_CUSTOM_FREE    vPortFree      /*Wrapper to free*/
#endif     /*LV_MEM_CUSTOM*/

/* Use the standard memcpy and memset instead of LVGL's own functions.
 * The standard functions might or might not be faster depending on their implementation. */
#define LV_MEMCPY_MEMSET_STD    0

/* Garbage Collector settings
 * Used if lvgl is binded to higher level language and the memory is managed by that language */
#define LV_ENABLE_GC 0
#if LV_ENABLE_GC != 0
#  define LV_GC_INCLUDE "gc.h"                           /*Include Garbage Collector related things*/
#endif /* LV_ENABLE_GC */

/* Input device read period in milliseconds */
#define LV_INDEV_DEF_READ_PERIOD          30

/*==================
 * Feature usage
 *==================*/

/* 1: Enable complex draw engine*/
#define LV_DRAW_COMPLEX           1
#if LV_DRAW_COMPLEX
/* Allow buffering some shadow calculation
 * LV_SHADOW_CACHE_SIZE is the max. shadow size to buffer,
 * where shadow size is `shadow_width + radius`
 * Caching has LV_SHADOW_CACHE_SIZE^2 RAM cost*/
#define LV_SHADOW_CACHE_SIZE    0
#endif

/* 1: Enable GPU interface*/
#define LV_USE_GPU_STM32_DMA2D  0
/*If enabling LV_USE_GPU_STM32_DMA2D, LV_GPU_DMA2D_CMSIS_INCLUDE must be defined to include path of CMSIS header of target processor
e.g. "stm32f769xx.h" or "stm32f429xx.h" */
#define LV_GPU_DMA2D_CMSIS_INCLUDE
/*Use NXP's PXP GPU iMX RTxxx platforms*/
#define LV_USE_GPU_NXP_PXP 0
#if LV_USE_GPU_NXP_PXP
/*1: Add default bare metal and FreeRTOS interrupt handling routines for PXP (lv_gpu_nxp_pxp_osa.c)
 *   and call lv_gpu_nxp_pxp_init() automatically during lv_init(). Note that symbol SDK_OS_FREE_RTOS
 *   has to be defined in order to use FreeRTOS OSA, otherwise bare-metal implementation is selected.
 *0: lv_gpu_nxp_pxp_init() has to be called manually before lv_init()
 */
#define LV_USE_GPU_NXP_PXP_AUTO_INIT 0
#endif

/*Use NXP's VG-Lite GPU iMX RTxxx platforms*/
#define LV_USE_GPU_NXP_VG_LITE 0

/*1: Add a `user_data` to drivers and objects*/
#define LV_USE_USER_DATA 1

/*1: Show CPU usage and FPS count in the right bottom corner*/
#define LV_USE_PERF_MONITOR 0

/* Show the used memory and the memory fragmentation  in the left bottom corner. Requires LV_MEM_CUSTOM = 0 */
#define LV_USE_MEM_MONITOR 0

/* Draw random colored rectangles over the redrawn areas */
#define LV_USE_REFR_DEBUG 0

/* Default image cache size. Image caching keeps the images opened.
 * If only the built-in image formats are used there is no real advantage of caching.
 * (I.e. no new image decoder is added)
 * With complex image decoders (e.g. PNG or JPG) caching can save the continuous open/decode of images.
 * However the opened images might consume additional RAM.
 * Set it to 0 to disable caching */
#define LV_IMG_CACHE_DEF_SIZE 1

/* Maximum buffer size to allocate for rotation. Only used if software rotation is enabled in the display driver. */
#define LV_DISP_ROT_MAX_BUF (10*1024)

/*=====================
 *  Compiler settings
 *====================*/

/* For big endian systems set to 1 */
#define LV_BIG_ENDIAN_SYSTEM    0

/* Define a custom attribute to `lv_tick_inc` function */
#define LV_ATTRIBUTE_TICK_INC

/* Define a custom attribute to `lv_timer_handler` function */
#define LV_ATTRIBUTE_TIMER_HANDLER

/* Define a custom attribute to `lv_disp_flush_ready` function */
#define LV_ATTRIBUTE_FLUSH_READY

/* Required alignment size for buffers */
#define LV_ATTRIBUTE_MEM_ALIGN_SIZE

/* With size optimization (-Os) the compiler might not align data to
 * 4 or 8 byte boundary. Some HW may need even 32 or 64 bytes.
 * This alignment will be explicitly applied where needed.
 * LV_ATTRIBUTE_MEM_ALIGN_SIZE should be used to specify required align size.
 * E.g. __attribute__((aligned(LV_ATTRIBUTE_MEM_ALIGN_SIZE))) */
#define LV_ATTRIBUTE_MEM_ALIGN

/* Attribute to mark large constant arrays for example
 * font's bitmaps */
#define LV_ATTRIBUTE_LARGE_CONST

/* Complier prefix for a big array declaration in RAM. */
#define LV_ATTRIBUTE_LARGE_RAM_ARRAY

/* Prefix performance critical functions to place them into a faster memory (e.g RAM)
 * Uses 15-20 kB extra memory */
#define LV_ATTRIBUTE_FAST_MEM

/* Export integer constant to binding.
 * This macro is used with constants in the form of LV_<CONST> that
 * should also appear on lvgl binding API such as Micropython
 *
 * The default value just prevents a GCC warning.
 */
#define LV_EXPORT_CONST_INT(int_value) struct _silence_gcc_warning

/* Extend the default -32k..32k coordinate range to -4M..4M by using int32_t for coordinates instead of int16_t. */
#define LV_USE_LARGE_COORD 0

/* Prefix variables that are used in GPU accelerated operations, often these need to be
 * placed in RAM sections that are DMA accessible */
#define LV_ATTRIBUTE_DMA

/*===================
 *  HAL settings
 *==================*/

/* 1: use a custom tick source.
 * It removes the need to manually update the tick with `lv_tick_inc`) */
#define LV_TICK_CUSTOM     0
#if LV_TICK_CUSTOM == 1
#define LV_TICK_CUSTOM_INCLUDE  "something.h"       /*Header for the sys time function*/
#define LV_TICK_CUSTOM_SYS_TIME_EXPR (millis())     /*Expression evaluating to current systime in ms*/
#endif   /*LV_TICK_CUSTOM*/

/* Default Dot Per Inch. Used to initialize default sizes such as widgets sized, style paddings.
 * (Not so important, you can adjust it to modify default sizes and spaces) */
#define LV_DPI_DEF 130 /* [px/inch] */

/*================
 * Log settings
 *===============*/

/*1: Enable the log module*/
#define LV_USE_LOG      0
#if LV_USE_LOG
/* How important log should be added:
 * LV_LOG_LEVEL_TRACE       A lot of logs to give detailed information
 * LV_LOG_LEVEL_INFO        Log important events
 * LV_LOG_LEVEL_WARN        Log if something unwanted happened but didn't cause a problem
 * LV_LOG_LEVEL_ERROR       Only critical issue, when the system may fail
 * LV_LOG_LEVEL_USER        Only logs added by the user
 * LV_LOG_LEVEL_NONE        Do not log anything
 */
#  define LV_LOG_LEVEL    LV_LOG_LEVEL_WARN

/* 1: Print the log with 'printf';
 * 0: user need to register a callback with `lv_log_register_print_cb`*/
#  define LV_LOG_PRINTF 0

/* Enable/disable LV_LOG_TRACE in modules that produces a huge number of logs. */
#define LV_LOG_TRACE_MEM 1
#define LV_LOG_TRACE_TIMER 1
#define LV_LOG_TRACE_INDEV 1
#define LV_LOG_TRACE_DISP_REFR 1
#define LV_LOG_TRACE_EVENT 1
#define LV_LOG_TRACE_OBJ_CREATE 1
#define LV_LOG_TRACE_LAYOUT 1
#define LV_LOG_TRACE_ANIM 1
#endif  /*LV_USE_LOG*/

/*=================
 * Debug settings
 *================*/

/*Check if the parameter is NULL. (Quite fast) */
#define LV_USE_ASSERT_NULL 1

/* Checks is the memory is successfully allocated or no. (Very fast, recommended) */
#define LV_USE_ASSERT_MALLOC 1

/*Check the integrity of `lv_mem` after critical operations. (Slow)*/
#define LV_USE_ASSERT_MEM_INTEGRITY       0

/* Check NULL, the object's type and existence (e.g. not deleted). (Quite slow)
 * If disabled `LV_USE_ASSERT_NULL` will be performed instead (if it's enabled) */
#define LV_USE_ASSERT_OBJ 0

/* Add a custom handler when assert happens e.g. to restart the MCU */
#define LV_ASSERT_HANDLER_INCLUDE <stdint.h>
#define LV_ASSERT_HANDLER while(1); /* Halt by default */

/*Check if the styles are properly initialized. (Fast)*/
#define LV_USE_ASSERT_STYLE     1

/*==================
 *    FONT USAGE
 *===================*/

/* The built-in fonts contains the ASCII range and some Symbols with  4 bit-per-pixel.
 * The symbols are available via `LV_SYMBOL_...` defines
 * More info about fonts: https://docs.lvgl.com/#Fonts
 * To create a new font go to: https://lvgl.com/ttf-font-to-c-array
 */

/* Montserrat fonts with bpp = 4
 * https://fonts.google.com/specimen/Montserrat  */
#define LV_FONT_MONTSERRAT_8     0
#define LV_FONT_MONTSERRAT_10    0
#define LV_FONT_MONTSERRAT_12    1
#define LV_FONT_MONTSERRAT_14    0
#define LV_FONT_MONTSERRAT_16    0
#define LV_FONT_MONTSERRAT_18    0
#define LV_FONT_MONTSERRAT_20    0
#define LV_FONT_MONTSERRAT_22    0
#define LV_FONT_MONTSERRAT_24    0
#define LV_FONT_MONTSERRAT_26    0
#define LV_FONT_MONTSERRAT_28    0
#define LV_FONT_MONTSERRAT_30    0
#define LV_FONT_MONTSERRAT_32    0
#define LV_FONT_MONTSERRAT_34    0
#define LV_FONT_MONTSERRAT_36    0
#define LV_FONT_MONTSERRAT_38    0
#define LV_FONT_MONTSERRAT_40    0
#define LV_FONT_MONTSERRAT_42    0
#define LV_FONT_MONTSERRAT_44    0
#define LV_FONT_MONTSERRAT_46    0
#define LV_FONT_MONTSERRAT_48    0

/* Demonstrate special features */
#define LV_FONT_MONTSERRAT_12_SUBPX      0
#define LV_FONT_MONTSERRAT_28_COMPRESSED 0  /*bpp = 3*/
#define LV_FONT_DEJAVU_16_PERSIAN_HEBREW 0  /*Hebrew, Arabic, PErisan letters and all their forms*/
#define LV_FONT_SIMSUN_16_CJK            0  /*1000 most common CJK radicals*/

/*Pixel perfect monospace font
 * http://pelulamu.net/unscii/ */
#define LV_FONT_UNSCII_8     0
#define LV_FONT_UNSCII_16    0

/* Optionally declare your custom fonts here.
 * You can use these fonts as default font too
 * and they will be available globally. E.g.
 * #define LV_FONT_CUSTOM_DECLARE LV_FONT_DECLARE(my_font_1) \
 *                                LV_FONT_DECLARE(my_font_2)
 */
#define LV_FONT_CUSTOM_DECLARE

/* Always set a default font. */
#define LV_FONT_DEFAULT &lv_font_montserrat_12

/* Enable it if you have fonts with a lot of characters.
 * The limit depends on the font size, font face and bpp
 * but with > 10,000 characters if you see issues probably you need to enable it.*/
#define LV_FONT_FMT_TXT_LARGE   0

/* Enables/disables support for compressed fonts. If it's disabled, compressed
 * glyphs cannot be processed by the library and won't be rendered.
 */
#define LV_USE_FONT_COMPRESSED 1

/* Enable subpixel rendering */
#define LV_USE_FONT_SUBPX 1
#if LV_USE_FONT_SUBPX
/* Set the pixel order of the display.
 * Important only if "subpx fonts" are used.
 * With "normal" font it doesn't matter.
 */
#define LV_FONT_SUBPX_BGR    0
#endif

/*================
 *  THEME USAGE
 *================*/

/* Mono-color theme for monochrome displays.
 * If LV_THEME_DEFAULT_COLOR_PRIMARY is LV_COLOR_BLACK the
 * texts and borders will be black and the background will be
 * white. Else the colors are inverted.
 * No flags. Set LV_THEME_DEFAULT_FLAG 0 */
 #define LV_USE_THEME_MONO 0

/* A simple, impressive and very complete theme */
#define LV_USE_THEME_DEFAULT 1
#if LV_USE_THEME_DEFAULT
/* 0: Light mode; 1: Dark mode */
#define LV_THEME_DEFAULT_DARK 0
/* 1: Enable grow on press */
#define LV_THEME_DEFAULT_GROW 1
/* Default transition time in [ms] */
#define LV_THEME_DEFAULT_TRANSITON_TIME 80
#endif /* LV_USE_THEME_DEFAULT */

/* An very simple them that is a good starting point for a custom theme */
#define LV_USE_THEME_BASIC 1

/*=================
 *  Text settings
 *=================*/

/* Select a character encoding for strings.
 * Your IDE or editor should have the same character encoding
 * - LV_TXT_ENC_UTF8
 * - LV_TXT_ENC_ASCII
 * */
#define LV_TXT_ENC LV_TXT_ENC_UTF8

 /*Can break (wrap) texts on these chars*/
#define LV_TXT_BREAK_CHARS                  " ,.;:-_"

/* If a word is at least this long, will break wherever "prettiest"
 * To disable, set to a value <= 0 */
#define LV_TXT_LINE_BREAK_LONG_LEN          0

/* Minimum number of characters in a long word to put on a line before a break.
 * Depends on LV_TXT_LINE_BREAK_LONG_LEN. */
#define LV_TXT_LINE_BREAK_LONG_PRE_MIN_LEN  3

/* Minimum number of characters in a long word to put on a line after a break.
 * Depends on LV_TXT_LINE_BREAK_LONG_LEN. */
#define LV_TXT_LINE_BREAK_LONG_POST_MIN_LEN 3

/* The control character to use for signalling text recoloring. */
#define LV_TXT_COLOR_CMD "#"

/* Support bidirectional texts.
 * Allows mixing Left-to-Right and Right-to-Left texts.
 * The direction will be processed according to the Unicode Bidirectioanl Algorithm:
 * https://www.w3.org/International/articles/inline-bidi-markup/uba-basics*/
#define LV_USE_BIDI     0
#if LV_USE_BIDI
/* Set the default direction. Supported values:
 * `LV_BASE_DIR_LTR` Left-to-Right
 * `LV_BASE_DIR_RTL` Right-to-Left
 * `LV_BASE_DIR_AUTO` detect texts base direction */
#define LV_BIDI_BASE_DIR_DEF  LV_BASE_DIR_AUTO
#endif

/* Enable Arabic/Persian processing
 * In these languages characters should be replaced with
 * an other form based on their position in the text */
#define LV_USE_ARABIC_PERSIAN_CHARS 0

/*Change the built in (v)snprintf functions*/
#define LV_SPRINTF_CUSTOM   0
#if LV_SPRINTF_CUSTOM
#  define LV_SPRINTF_INCLUDE <stdio.h>
#  define lv_snprintf     snprintf
#  define lv_vsnprintf    vsnprintf
#else   /*!LV_SPRINTF_CUSTOM*/
#  define LV_SPRINTF_USE_FLOAT 0
#endif  /*LV_SPRINTF_CUSTOM*/

/*==================
 *  LV OBJ X USAGE
 *================*/
/*
 * Documentation of the object types: https://docs.lvgl.com/#Object-types
 */

/*Arc (dependencies: -)*/
#define LV_USE_ARC 1

#define LV_USE_ANIMIMG 1

/*Bar (dependencies: -)*/
#define LV_USE_BAR      1

/*Button (dependencies: lv_cont*/
#define LV_USE_BTN      1

/*Button matrix (dependencies: -)*/
#define LV_USE_BTNMATRIX     1

/*Calendar (dependencies: -)*/
#define LV_USE_CALENDAR 1
#if LV_USE_CALENDAR
#  define LV_CALENDAR_WEEK_STARTS_MONDAY 0

#if LV_CALENDAR_WEEK_STARTS_MONDAY
#define LV_CALENDAR_DEFAULT_DAY_NAMES {"Mo", "Tu", "We", "Th", "Fr", "Sa", "Su"}
#else
#define LV_CALENDAR_DEFAULT_DAY_NAMES {"Su", "Mo", "Tu", "We", "Th", "Fr", "Sa"}
#endif
#define LV_CALENDAR_DEFAULT_MONTH_NAMES {"January", "February", "March",  "April", "May",  "June", "July", "August", "September", "October", "November", "December"}
#define LV_USE_CALENDAR_HEADER_ARROW 1
#define LV_USE_CALENDAR_HEADER_DROPDOWN 1
#endif

/*Canvas (dependencies: lv_img)*/
#define LV_USE_CANVAS   1

/*Check box (dependencies: lv_btn, lv_label)*/
#define LV_USE_CHECKBOX       1

/*Chart (dependencies: -)*/
#define LV_USE_CHART 1

#define LV_USE_COLORWHEEL 1

/*Requires: lv_label*/
#define LV_USE_DROPDOWN    1

/*Image (dependencies: lv_label*/
#define LV_USE_IMG      1

/*Image Button (dependencies: lv_btn*/
#define LV_USE_IMGBTN   1

/*Keyboard (dependencies: lv_btnm)*/
#define LV_USE_KEYBOARD       1

/*Label (dependencies: -*/
#define LV_USE_LABEL    1
#if LV_USE_LABEL != 0

/*Enable selecting text of the label */
#  define LV_LABEL_TEXT_SELECTION               0

/*Store extra some info in labels (12 bytes) to speed up drawing of very long texts*/
#  define LV_LABEL_LONG_TXT_HINT          0
#endif

/*LED (dependencies: -)*/
#define LV_USE_LED      1

/*Line (dependencies: -*/
#define LV_USE_LINE     1

/*List (dependencies: lv_page, lv_btn, lv_label, (lv_img optionally for icons ))*/
#define LV_USE_LIST 1

#define LV_USE_METER 1

/*Message box (dependencies: lv_rect, lv_btnm, lv_label)*/
#define LV_USE_MSGBOX     1

/*Preload (dependencies: lv_arc, lv_anim)*/
#define LV_USE_SPINNER      1

/*Requires: lv_label*/
#define LV_USE_ROLLER    1
#if LV_USE_ROLLER != 0

/*Number of extra "pages" when the roller is infinite*/
#  define LV_ROLLER_INF_PAGES         7
#endif

/*Slider (dependencies: lv_bar)*/
#define LV_USE_SLIDER    1

/*Spinbox (dependencies: lv_ta)*/
#define LV_USE_SPINBOX       1

/*Switch (dependencies: lv_slider)*/
#define LV_USE_SWITCH       1

/*Text area (dependencies: lv_label, lv_page)*/
#define LV_USE_TEXTAREA       1
#if LV_USE_TEXTAREA != 0
#  define LV_TEXTAREA_DEF_PWD_SHOW_TIME     1500    /*ms*/
#endif

#define LV_USE_TABLE    1

/*Tab (dependencies: lv_page, lv_btnm)*/
#define LV_USE_TABVIEW      1
#  if LV_USE_TABVIEW != 0
/*Time of slide animation [ms] (0: no animation)*/
#  define LV_TABVIEW_DEF_ANIM_TIME    0
#endif

/*Tileview (dependencies: lv_page) */
#define LV_USE_TILEVIEW     1

/*Window (dependencies: lv_cont, lv_btn, lv_label, lv_img, lv_page)*/
#define LV_USE_WIN 1

#define LV_USE_SPAN 1
#if LV_USE_SPAN
/* A line text can contain maximum num of span descriptor */
#define LV_SPAN_SNIPPET_STACK_SIZE 64
#endif

/*-----------
 * Layouts
 *----------*/

/* A layout similar to Flexbox in CSS. */
#define LV_USE_FLEX 1

/* A layout similar to Grid in CSS. */
#define LV_USE_GRID 1

/*==================
* EXAMPLES
*==================*/

/* Enable the examples to be built with the library */
#define LV_BUILD_EXAMPLES 1

/*==================
 * Non-user section
 *==================*/

#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)    /* Disable warnings for Visual Studio*/
#  define _CRT_SECURE_NO_WARNINGS
#endif

/*--END OF LV_CONF_H--*/

/* clang-format on */
#endif /*LV_CONF_H*/
