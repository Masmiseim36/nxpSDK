/*
 * Copyright 2019-2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
 * @brief defines, enums, etc. common to all HAL devices
 */

#ifndef _FWK_COMMON_H_
#define _FWK_COMMON_H_

#include <stdint.h>

#define MAKE_FRAMEWORK_STATUS(group, code) ((((group)*100) + (code)))
#define MAKE_FRAMEWORK_EVENTS(group, code) ((((group)*10) + (code)))

typedef int32_t status_t;

typedef enum _framework_events
{
    kFrameworkEvents_GetManagerInfo,
    kFrameworkEvents_GetManagerComponents,
    kFrameworkEvents_GetDeviceConfigs,
} framework_events_t;

typedef enum _status_framework_groups
{
    kStatusFrameworkGroups_Camera          = 0,
    kStatusFrameworkGroups_Display         = 1,
    kStatusFrameworkGroups_VAlgo           = 2,
    kStatusFrameworkGroups_Output          = 3,
    kStatusFrameworkGroups_Input           = 4,
    kStatusFrameworkGroups_Config          = 5,
    kStatusFrameworkGroups_Flash           = 6,
    kStatusFrameworkGroups_AudioProcessing = 7,
    kStatusFrameworkGroups_Lpm             = 8,
    kStatusFrameworkGroups_Count,
} status_framework_groups_t;

typedef enum _log_level
{
    kLOGLevel_None = 0,
    kLOGLevel_Error,
    kLOGLevel_Debug,
    kLOGLevel_Info,
    kLOGLevel_Verbose,
    kLOGLevel_Invalid,
} log_level_t;

typedef enum _display_type
{
    kDisplayType_RGB = 0,
    kDisplayType_Gray,
    kDisplayType_DepthGray,
    kDisplayType_Invalid,
} display_type_t;

typedef enum _display_output
{
    kDisplayOutput_Panel = 0,
    kDisplayOutput_UVC,
    kDisplayOutput_Invalid,
} display_output_t;

typedef enum _pixel_format
{
    /* 2d frame format */
    kPixelFormat_RGB,
    kPixelFormat_RGB565,
    kPixelFormat_BGR,
    kPixelFormat_Gray888,
    kPixelFormat_Gray888X,
    kPixelFormat_Gray,
    kPixelFormat_Gray16,
    kPixelFormat_YUV1P444_RGB,   /* color camera sensor */
    kPixelFormat_YUV1P444_Gray,  /* ir camera sensor */
    kPixelFormat_UYVY1P422_RGB,  /* color camera sensor */
    kPixelFormat_UYVY1P422_Gray, /* ir camera sensor */
    kPixelFormat_VYUY1P422,

    /* 3d frame format */
    kPixelFormat_Depth16,
    kPixelFormat_Depth8,
    kPixelFormat_Invalid
} pixel_format_t;

typedef enum _cw_rotate_degree
{
    kCWRotateDegree_0 = 0,
    kCWRotateDegree_90,
    kCWRotateDegree_180,
    kCWRotateDegree_270
} cw_rotate_degree_t;

typedef enum _flip_mode
{
    kFlipMode_None = 0,
    kFlipMode_Horizontal,
    kFlipMode_Vertical,
    kFlipMode_Both
} flip_mode_t;

/* task slot count reserved for APP */
#define APP_TASK_COUNT 10

typedef enum _fwk_task_id
{
    kFWKTaskID_Camera = 0, /* This should always stay first */
    kFWKTaskID_Display,
    kFWKTaskID_VisionAlgo,
    kFWKTaskID_VoiceAlgo,
    kFWKTaskID_Output,
    kFWKTaskID_Input,
    kFWKTaskID_Audio,
    kFWKTaskID_APPStart, /* APP task ID should always start from here */
    kFWKTaskID_COUNT = (kFWKTaskID_APPStart + APP_TASK_COUNT)
} fwk_task_id_t;

typedef enum _valgo_frame_id
{
    kVAlgoFrameID_RGB = 0,
    kVAlgoFrameID_IR,
    kVAlgoFrameID_Depth,
    kVAlgoFrameID_Count
} valgo_frame_id_t;

typedef enum _hal_device_state
{
    kState_HAL_Created = 0,
    kState_HAL_Registered,
    kState_HAL_Initialized,
    kState_HAL_Started,
    kState_HAL_Stopped,
} hal_device_state_t;

typedef struct _fwk_task_info
{
    unsigned int priority;
    fwk_task_id_t managerId;
    char *name;
} fwk_task_info_t;

#define AUDIO_PROCESSING_TASK_NAME     "audio_processing"
#define AUDIO_PROCESSING_TASK_STACK    1024
#define AUDIO_PROCESSING_TASK_PRIORITY 2

#define CAMERA_MANAGER_TASK_NAME     "camera_manager"
#define CAMERA_MANAGER_TASK_STACK    1024
#define CAMERA_MANAGER_TASK_PRIORITY 2

#define DISPLAY_MANAGER_TASK_NAME     "display_manager"
#define DISPLAY_MANAGER_TASK_STACK    1024
#define DISPLAY_MANAGER_TASK_PRIORITY 3

#define VISION_ALGO_MANAGER_TASK_NAME     "vision_algo_manager"
#define VISION_ALGO_MANAGER_TASK_STACK    (10 * 1024)
#define VISION_ALGO_MANAGER_TASK_PRIORITY 4

#define VOICE_ALGO_MANAGER_TASK_NAME     "voice_algo_manager"
#define VOICE_ALGO_MANAGER_TASK_STACK    1024
#define VOICE_ALGO_MANAGER_TASK_PRIORITY 3

#define OUTPUT_MANAGER_TASK_NAME     "output_manager"
#define OUTPUT_MANAGER_TASK_STACK    1024
#define OUTPUT_MANAGER_TASK_PRIORITY 4

#define INPUT_MANAGER_TASK_NAME     "input_manager"
#define INPUT_MANAGER_TASK_STACK    1024
#define INPUT_MANAGER_TASK_PRIORITY 1

#define MAXIMUM_CAMERA_DEV    2
#define MAXIMUM_CAMERA_BUFFER 2

#define MAXIMUM_DISPLAY_DEV          2
#define MAXIMUM_INPUT_DEV            10
#define MAXIMUM_OUTPUT_DEV           10
#define MAXIMUM_VISION_ALGO_DEV      1
#define MAXIMUM_VOICE_ALGO_DEV       1
#define MAXIMUM_AUDIO_PROCESSING_DEV 1

#define MAXIMUM_CONFIGS_PER_DEVICE 5

#define FROM_ISR_TRUE  1
#define FROM_ISR_FALSE 0

#define DEVICE_NAME_MAX_LENGTH                16
#define DEVICE_CONFIG_NAME_MAX_LENGTH         16
#define DEVICE_CONFIG_EXPECTED_VAL_MAX_LENGTH 32
#define DEVICE_CONFIG_DESCRIPTION_MAX_LENGTH  32
#define LPM_REQUEST_NAME_MAX_LENGTH           32

typedef struct _hal_device_config
{
    char name[DEVICE_CONFIG_NAME_MAX_LENGTH];
    char expectedValue[DEVICE_CONFIG_EXPECTED_VAL_MAX_LENGTH];
    char description[DEVICE_CONFIG_DESCRIPTION_MAX_LENGTH];
    uint32_t value;

    status_t (*get)(char *valueToString);
    status_t (*set)(char *configName, uint32_t value);
} hal_device_config;

typedef struct _fwk_task_component
{
    fwk_task_id_t managerId;
    unsigned int deviceId;
    char *deviceName;
    hal_device_config *configs;
} fwk_task_component_t;

typedef struct _framework_response
{
    union
    {
        /* Valid only when eventId is kFrameworkEvents_GetManagerInfo */
        fwk_task_info_t fwkTaskInfo;
        fwk_task_component_t fwkTaskComponent;
    };
} framework_response_t;

typedef struct _framework_request
{
    framework_events_t frameworkEventId;
    fwk_task_id_t managerId;
    unsigned int deviceId;
    char deviceName[DEVICE_NAME_MAX_LENGTH];
    int (*respond)(framework_events_t eventId, framework_response_t *response, unsigned char isFinished);

} framework_request_t;

#ifndef FWK_SUPPORT_STATIC_ALLOCATION
#define FWK_SUPPORT_STATIC_ALLOCATION 0
#endif /* FWK_SUPPORT_STATIC_ALLOCATION */

#ifndef FWK_SUPPORT_ASYNC_CAMERA_INIT
#define FWK_SUPPORT_ASYNC_CAMERA_INIT 1
#endif /* FWK_SUPPORT_ASYNC_CAMERA_INIT */

#endif /*_FWK_COMMON_H_*/
