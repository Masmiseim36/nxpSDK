/*
 * Copyright 2020-2023 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include "fsl_debug_console.h"
#include <stdio.h>
#include <stdarg.h>

/* TODO - fix pixel format definition issue */
#include "hal_utils.h"
#include "mpp_config.h"
#include "hal_debug.h"
#include "hal_static_image.h"
#include "hal_os.h"

#define LOG_STRING_MAX_SIZE 128

#ifndef HAL_LOG_LEVEL
#define HAL_LOG_LEVEL LOG_DEFAULT_LEVEL
#endif

static void LOG_STR(const char* module, const char* func, int line, const char* lvl_str, const char* str)
{
    PRINTF("\r[%u]", (unsigned int)GET_TICK());
    PRINTF(":%s:%s:(%s:%u)", module, lvl_str, func, line);
    PRINTF(":%s", str);
}

#if (LOG_LVL_ERR <= HAL_LOG_LEVEL)
void LOGE(const char* module, const char* func, int line, const char* format, ...)
{
    char args_buffer[LOG_STRING_MAX_SIZE];
    va_list args;
    va_start(args, format);
    vsprintf(args_buffer, format, args);
    LOG_STR(module, func, line, "ERR", args_buffer);
    va_end(args);
}
#else
void LOGE(const char* module, const char* func, int line, const char* format, ...)
{
    return;
}
#endif

#if (LOG_LVL_INFO <= HAL_LOG_LEVEL)
void LOGI(const char* module, const char* func, int line, const char* format, ...)
{
    char args_buffer[LOG_STRING_MAX_SIZE];
    va_list args;
    va_start(args, format);
    vsprintf(args_buffer, format, args);
    LOG_STR(module, func, line, "INFO", args_buffer);
    va_end(args);
}
#else
void LOGI(const char* module, const char* func, int line, const char* format, ...)
{
    return;
}
#endif

#if (LOG_LVL_DEBUG <= HAL_LOG_LEVEL)
void LOGD(const char* module, const char* func, int line, const char* format, ...)
{
    char args_buffer[LOG_STRING_MAX_SIZE];
    va_list args;
    va_start(args, format);
    vsprintf(args_buffer, format, args);
    LOG_STR(module, func, line, "DBG", args_buffer);
    va_end(args);
}
#else
void LOGD(const char* module, const char* func, int line, const char* format, ...)
{
    return;
}
#endif

int setup_graphic_dev(hal_graphics_setup_t gfx_setup[], int graphic_nb,
                      const char *name, gfx_dev_t *dev)
{
    int i, found = 0;

    /* search name */
    if ((name == NULL) && (graphic_nb)) {
        /* pick prefered first graphics device of the list */
        found = 1;
        i = 0;
    }
    else if ((name == NULL) || (!strcmp(name, HAL_GFX_DEV_CPU_NAME))) {
        /* pick CPU graphics */
        return HAL_GfxDev_CPU_Register(dev);
    }
    else {
        for (i = 0; i < graphic_nb; i++)
            if (!strcmp(name, gfx_setup[i].gfx_dev_name)) {
                found = 1;
                break;
            }
    }
    if (!found) {
        HAL_LOGE("Graphic device %s not found\n", name);
        return MPP_INVALID_PARAM;
    }

    /* call name-specific graphic setup function*/
    graphic_setup_func_t gfx_setup_f;
    gfx_setup_f = gfx_setup[i].gfx_setup_func;
    if (gfx_setup_f)
        return gfx_setup_f(dev);

    return MPP_ERROR;
}

int setup_display_dev(hal_display_setup_t display_setup[], int display_nb,
		      const char *name, display_dev_t *dev)
{
    int i, found = 0;

    if (name == NULL) {
        HAL_LOGE("name is NULL\n");
        return MPP_INVALID_PARAM;
    }

    for (i = 0; i < display_nb; i++)
        if (!strcmp(name, display_setup[i].display_name)) {
            found = 1;
            break;
        }
    if (!found) {
        HAL_LOGE("Display device %s not found\n", name);
        return MPP_INVALID_PARAM;
    }

    display_setup_func_t disp_setup_f;
    disp_setup_f = display_setup[i].display_setup_func;
    if (disp_setup_f)
        return disp_setup_f(dev);

    return MPP_ERROR;

}

int setup_camera_dev(hal_camera_setup_t camera_setup[], int camera_nb,
		     const char *name, camera_dev_t *dev)
{
    int i, found = 0;

    if (name == NULL) {
        HAL_LOGE("name is NULL\n");
        return MPP_INVALID_PARAM;
    }

    /* search name */
    for (i = 0; i < camera_nb; i++)
        if (!strcmp(name, camera_setup[i].camera_name)) {
            found = 1;
            break;
        }
    if (!found) {
        HAL_LOGE("Camera device %s not found\n", name);
        return MPP_INVALID_PARAM;
    }

    /* call name-specific camera setup function*/
    camera_setup_func_t cam_setup_f;
    cam_setup_f = camera_setup[i].camera_setup_func;
    if (cam_setup_f)
        return cam_setup_f(name, dev);

    return MPP_ERROR;
}

int setup_static_image(static_image_t *);

int setup_static_image_elt(static_image_t *elt)
{
    return setup_static_image(elt);
}

/* Computes the checksum of the buffer
 * using Pisano with End-Around Carry algorithm
 * which is almost as reliable but faster than CRC32.
 * See https://hackaday.io/project/178998-peac-pisano-with-end-around-carry-algorithm
 * Note: with odd buffer size, last byte is ignored.
 * param: 'size_b': size in bytes of buffer
 * param: 'pbuf': pointer to buffer
 * return: a 32bit checksum
 **/
uint32_t calc_checksum(int size_b, void *pbuf)
{
    uint16_t *pw = (uint16_t *)pbuf;  /* pointer to 16b word in input buffer */
    uint16_t x = 0x1234;
    uint32_t y = 0xABCD;
    uint32_t c = size_b;
    int w_cnt = size_b/2; /* 16b word count */
    while(w_cnt > 0)
    {
        c += x;
        c += y;
        y  = x + *pw;
        x  = (uint16_t) c;
        c >>= 16;
        w_cnt--;
        pw++;
    }
    return (x | (y << 16));
}

#ifdef __cplusplus
}
#endif
