/*
 * Copyright 2021-2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_CORECM33_H_
#define _APP_CORECM33_H_

/*${header:start}*/
#include "dsp_support.h"
#include "fsl_sema42.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define SDCARD_SWITCH_VOLTAGE_FUNCTION_EXIST
#define DEMO_CODEC_VOLUME  70U
#define APP_SEMA42         SEMA42_4
#define SEMA_PRINTF_NUM    0
#define SEMA_STARTUP_NUM   1
#define SEMA_CORE_ID_CM33  0
#define SEMA_LOCKED_BY_DSP kSEMA42_LockedByProc4
#define DEMO_I2C_CLK_FREQ  24000000U

#if defined(__CC_ARM)
extern uint32_t Image$$DSP_REGION$$Base;
extern uint32_t Image$$DSP_REGION$$Length;
#define HIFI1_IMAGE_START &Image$$DSP_REGION$$Base
#elif defined(__ICCARM__)
#define HIFI1_IMAGE_VECTOR_START (uint32_t *)__section_begin("__hifi1_vector_section")
#define HIFI1_IMAGE_VECTOR_SIZE \
    (int32_t) __section_end("__hifi1_vector_section") - (uint32_t)__section_begin("__hifi1_vector_section");
#define HIFI1_IMAGE_TEXT_START (uint32_t *)__section_begin("__hifi1_text_section")
#define HIFI1_IMAGE_TEXT_SIZE \
    (int32_t) __section_end("__hifi1_text_section") - (uint32_t)__section_begin("__hifi1_text_section");
#define HIFI1_IMAGE_DATA_START (uint32_t *)__section_begin("__hifi1_data_section")
#define HIFI1_IMAGE_DATA_SIZE \
    (int32_t) __section_end("__hifi1_data_section") - (uint32_t)__section_begin("__hifi1_data_section");
#elif (defined(__ARMCC_VERSION))
extern const char hifi1_vector_image_start[];
extern const char hifi1_vector_image_end[];
extern const char hifi1_text_image_start[];
extern const char hifi1_text_image_end[];
extern const char hifi1_data_image_start[];
extern const char hifi1_data_image_end[];
#define HIFI1_IMAGE_VECTOR_START  ((uint32_t *)hifi1_vector_image_start)
#define HIFI1_IMAGE_VECTOR_SIZE   ((uint32_t)hifi1_vector_image_end - (uint32_t)hifi1_vector_image_start)
#define HIFI1_IMAGE_TEXT_START    ((uint32_t *)hifi1_text_image_start)
#define HIFI1_IMAGE_TEXT_SIZE     ((uint32_t)hifi1_text_image_end - (uint32_t)hifi1_text_image_start)
#define HIFI1_IMAGE_DATA_START    ((uint32_t *)hifi1_data_image_start)
#define HIFI1_IMAGE_DATA_SIZE     ((uint32_t)hifi1_data_image_end - (uint32_t)hifi1_data_image_start)
#elif defined(__GNUC__)
extern const char hifi1_vector_image_start[];
extern int hifi1_vector_image_size;
extern const char hifi1_text_image_start[];
extern int hifi1_text_image_size;
extern const char hifi1_data_image_start[];
extern int hifi1_data_image_size;
#define HIFI1_IMAGE_VECTOR_START  ((uint32_t *)hifi1_vector_image_start)
#define HIFI1_IMAGE_VECTOR_SIZE   ((int32_t)hifi1_vector_image_size)
#define HIFI1_IMAGE_TEXT_START    ((uint32_t *)hifi1_text_image_start)
#define HIFI1_IMAGE_TEXT_SIZE     ((int32_t)hifi1_text_image_size)
#define HIFI1_IMAGE_DATA_START    ((uint32_t *)hifi1_data_image_start)
#define HIFI1_IMAGE_DATA_SIZE     ((int32_t)hifi1_data_image_size)
#endif
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
int BOARD_CODEC_Init(void);
void BOARD_PowerOffSDCARD(void);
void BOARD_PowerOnSDCARD(void);
void BOARD_USDHC_Switch_VoltageTo1V8(void);
void BOARD_USDHC_Switch_VoltageTo3V3(void);
/*${prototype:end}*/

#endif /* _APP_CORECM33_H_ */
