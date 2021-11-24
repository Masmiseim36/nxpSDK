/*
 * Copyright 2020-2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
 * @brief SLN Oasis-Lite Vision algorithm HAL driver declaration for the smart lock application.
 */

#ifndef _HAL_VISION_ALGO_OASIS_LITE_H_
#define _HAL_VISION_ALGO_OASIS_LITE_H_

#include "hal_sln_facedb.h"

#ifdef SMART_LOCK_3D
#include "oasislite3D_runtime.h"
#elif defined(SMART_LOCK_2D) || defined(SMART_ACCESS_2D)
#include "oasislite2D_runtime.h"
#else
#error "***Invalid demo APP definition! Supported definition [SMART_LOCK_3D | SMART_LOCK_2D | SMART_ACCESS_2D]***"
#endif

#define OASIS_RGB_FRAME_WIDTH          480
#define OASIS_RGB_FRAME_HEIGHT         640
#define OASIS_RGB_FRAME_BYTE_PER_PIXEL 3

#define OASIS_IR_FRAME_WIDTH          480
#define OASIS_IR_FRAME_HEIGHT         640
#define OASIS_IR_FRAME_BYTE_PER_PIXEL 3

#define OASIS_FRAME_WIDTH  540
#define OASIS_FRAME_HEIGHT 640

#define OASIS_DETECT_MIN_FACE 100
#ifndef OASIS_STATIC_MEM_BUFFER
#define OASIS_STATIC_MEM_BUFFER 0
#endif

#if OASIS_STATIC_MEM_BUFFER
#define OASIS_STATIC_MEM_POOL 0x100000
#endif

/*dtc buffer for inference engine optimization*/
#define DTC_OPTIMIZE_BUFFER_SIZE (128*1024)

typedef enum _oasis_lite_mode
{
    kOASISLiteMode_SmartLock,
    kOASISLiteMode_FFI,
    kOASISLiteMode_Count
} oasis_lite_mode_t;

typedef enum _oasis_lite_state
{
    kOASISLiteState_Recognition,
    kOASISLiteState_Registration,
    kOASISLiteState_DeRegistration,
    kOASISLiteState_RemoteRegistration,
    kOASISLiteState_Count
} oasis_lite_state_t;

typedef enum _oasis_lite_recognition_result
{
    kOASISLiteRecognitionResult_Invalid,
    kOASISLiteRecognitionResult_Success,
    kOASISLiteRecognitionResult_Unknown,
    kOASISLiteRecognitionResult_Timeout,
    kOASISLiteRecognitionResult_Count
} oasis_lite_recognition_result_t;

typedef enum _oasis_lite_registration_result
{
    kOASISLiteRegistrationResult_Invalid,
    kOASISLiteRegistrationResult_Success,
    kOASISLiteRegistrationResult_Duplicated,
    kOASISLiteRegistrationResult_Timeout,
    kOASISLiteRegistrationResult_Count
} oasis_lite_registration_result_t;

typedef enum _oasis_lite_deregistration_result
{
    kOASISLiteDeregistrationResult_Invalid,
    kOASISLiteDeregistrationResult_Success,
    kOASISLiteDeregistrationResult_Timeout,
    kOASISLiteDeregistrationResult_Count
} oasis_lite_deregistration_result_t;

typedef enum _oasis_lite_quality_check_result
{
    kOasisLiteQualityCheck_Ok,
    kOasisLiteQualityCheck_FakeFace,
    kOasisLiteQualityCheck_NonFrontalFace,
    kOasisLiteQualityCheck_Blurry,
    kOasisLiteQualityCheck_Count
}oasis_lite_quality_check_result_t;

#if HEADLESS_ENABLE
typedef enum _oasisi_lite_headless_reg_process
{
    OASIS_LITE_HEADLESS_REG_START = 0,
    OASIS_LITE_HEADLESS_REG_FRONT_FACE,
    OASIS_LITE_HEADLESS_REG_LEFT_FACE,
    OASIS_LITE_HEADLESS_REG_RIGHT_FACE,
    OASIS_LITE_HEADLESS_REG_COMPLETE,
} oasis_lite_headless_reg_process_t;
#endif

typedef struct _oasis_lite_debug
{
    uint32_t sim;
    /* the face id with this sim value */
    uint32_t faceID;
    uint8_t isOk;
    uint8_t is3dFake;
    uint8_t is2dFake;
    uint8_t isBlurry;
    uint8_t isSideFace;
    uint8_t hasMask;
    uint8_t hasGlasses;
    uint8_t irBrightness;
    uint8_t rgbBrightness;
} oasis_lite_debug_t;

// oasis lite algorithm result
typedef struct _oasis_lite_result
{
    oasis_lite_state_t state;

    /* detect result */
    uint8_t face_count;
    FaceBox_t face_box;

    /* recognition result */
    int face_recognized;
    int face_id;
    char name[FACE_NAME_MAX_LEN];

    /* quality check results */
    oasis_lite_quality_check_result_t qualityCheck;

    // registration result
    union
    {
        uint32_t result;
        oasis_lite_recognition_result_t rec_result;
        oasis_lite_registration_result_t reg_result;
        oasis_lite_deregistration_result_t dereg_result;
    };
    float process;
#if HEADLESS_ENABLE
    uint8_t headless_reg_status;
#endif
    oasis_lite_debug_t debug_info;
} oasis_lite_result_t;

#endif /* _HAL_VISION_ALGO_OASIS_LITE_H_ */
