/*
 * Copyright 2020-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
 * @brief vision algorithms device declaration. Vision algorithm devices include ML/AI models like the
 * TFLite inference engine.
 */

#ifndef _HAL_VALGO_DEV_H_
#define _HAL_VALGO_DEV_H_

#include "hal_os.h"
#include "mpp_api_types.h"
#include "hal_types.h"

/**
 * @brief declare the vision algorithm dev ##name
 */
#define HAL_VALGO_DEV_DECLARE(name) int HAL_VisionAlgoDev_##name##_Register();

/**
 * @brief register the vision algorithm dev ##name
 */
#define HAL_VALGO_DEV_REGISTER(name, ret)                             \
    {                                                                  \
        ret = HAL_VisionAlgoDev_##name##_Register();                       \
        if (ret != 0)                                                  \
        {                                                              \
            HAL_LOGE("HAL_VisionAlgoDev_%s_Register error %d\n", "##name", ret); \
            return ret;                                                \
        }                                                              \
    }

typedef enum _valgo_event
{
    /* Vision and voice algorithms result update */
    kVAlgoEvent_VisionResultUpdate,
    kVAlgoEvent_VoiceResultUpdate,
    /* Vision algorithm tries to control LED PWM or CAMERA EXPOSURE */
    kVAlgoEvent_VisionLedPwmControl,
    kVAlgoEvent_VisionCamExpControl,
    kVAlgoEvent_VisionRecordControl,

    kVAlgoEvent_RequestFrame,
    kVAlgoEvent_Count
} valgo_event_t;

/*!
 * @brief Callback function to notify managers the results of inference
 * valgo_dev* dev Pointer to an algorithm device
 * valgo_event_t event Event which took place
 * void* param Pointer to a struct of data that needs to be forwarded
 * unsigned int size Size of the struct that needs to be forwarded. If size = 0, param should be a pointer to a
 * persistent memory area.
 */

typedef int (*valgo_dev_callback_t)(int devId, valgo_event_t event, void *param, unsigned int size, uint8_t fromISR);

/*! @brief Structure passed to HAL as description of the binary model provided by user */
typedef struct _model_param_t {
    const void *model_data; /* pointer to model binary */
    int model_size;         /* model binary size */
    float model_input_mean; /* model 'mean' of input values, used for normalization */
    float model_input_std;  /* model 'standard deviation' of input values, used for normalization */
    mpp_inference_params_t inference_params; /* inference parameters */
    /* frame resolution */
    int height;
    int width;
    /* pixel format */
    mpp_pixel_format_t format;
    mpp_tensor_type_t inputType;
    mpp_tensor_order_t tensor_order;
    int (*evt_callback_f)(mpp_t mpp, mpp_evt_t evt, void *evt_data, void *user_data);    /* the callback to be called when model output is ready */
    void *cb_userdata;      /* pointer to user data, should be passed by callback */
} model_param_t;

/*! @brief Valgo Error codes for hal operations */
typedef enum _hal_valgo_status
{
    kStatus_HAL_ValgoSuccess = 0,   /*!< Successfully */
    kStatus_HAL_ValgoMallocError,   /*!< memory allocation failed for HAL algorithm */
    kStatus_HAL_ValgoInitError,     /*!< algorithm initialization error */
    kStatus_HAL_ValgoError,         /*!< Error occurs in HAL algorithm */
    kStatus_HAL_ValgoStop,          /*!< HAL algorithm stop */
} hal_valgo_status_t;

typedef struct _valgo_dev_private_capability
{
    /* callback */
    valgo_dev_callback_t callback;
    /* param for the callback */
    void *param;
} valgo_dev_private_capability_t;

/*! @brief Characteristics that need to be defined by a vision algo. */

typedef struct _vision_frame
{
    /* frame resolution */
    int height;
    int width;
    int pitch;
    /* pixel format */
    mpp_pixel_format_t format;
    /* pixel input buffer */
    void *input_buf;
} vision_frame_t;

typedef void *vision_algo_private_data_t;

typedef struct _vision_algo_dev vision_algo_dev_t;

/*! @brief Operation that needs to be implemented by a vision algorithm device */
typedef struct
{
    /* initialize the dev */
    hal_valgo_status_t (*init)(vision_algo_dev_t *dev, valgo_dev_callback_t callback, model_param_t *param);
    /* deinitialize the dev */
    hal_valgo_status_t (*deinit)(vision_algo_dev_t *dev);
    /* start the dev */
    hal_valgo_status_t (*run)(const vision_algo_dev_t *dev, void *data);
    /* read input parameters */
    hal_valgo_status_t (*get_buf_desc)(const vision_algo_dev_t *dev, hw_buf_desc_t *in_buf, mpp_memory_policy_t *policy);

} vision_algo_dev_operator_t;

/*! @brief Attributes of a vision algo device */
struct _vision_algo_dev
{
    /* unique id which is assigned by algorithm manager during the registration */
    int id;
    /* name to identify */
    char name[HAL_DEVICE_NAME_MAX_LENGTH];
    /* private capability */
    valgo_dev_private_capability_t cap;
    /* operations */
    const vision_algo_dev_operator_t *ops;
    /* private data */
    vision_algo_private_data_t priv_data;
};

#endif /*_HAL_VALGO_DEV_H_*/
