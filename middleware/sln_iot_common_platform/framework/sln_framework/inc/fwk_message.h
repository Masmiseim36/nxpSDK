/*
 * Copyright 2020-2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
 * @brief message manager framework declaration.
 */

#ifndef _FWK_MESSAGE_H_
#define _FWK_MESSAGE_H_

#include "fwk_sln_platform.h"
#include "fwk_common.h"
#include "fwk_graphics.h"

#if defined(__cplusplus)
extern "C" {
#endif

/*! @brief List with framework internal messages. Not to be used from outside of framework */
typedef enum _fwk_message_id
{
    /* camera task send message */
    kFWKMessageID_CameraDequeue,
    kFWKMessageID_CameraSet,

    /* display task send message */
    kFWKMessageID_DisplayRequestFrame,
    kFWKMessageID_DisplayResponseFrame,

    /* vision algorithm manager message */
    kFWKMessageID_VAlgoRequestFrame,
    kFWKMessageID_VAlgoResponseFrame,
    kFWKMessageID_VAlgoResultUpdate,
    /* voice recognition message */
    kFWKMessageID_VAlgoASRInputProcess,
    kFWKMessageID_VAlgoASRResultUpdate,

    /* event dispatcher task send message */
    kFWKMessageID_DispatcherRequestShowOverlay,

    /* input task input triggered */
    kFWKMessageID_InputReceive,
    kFWKMessageID_InputNotify,
    kFWKMessageID_InputAudioReceived,
    kFWKMessageID_InputFrameworkReceived,
    kFWKMessageID_InputFrameworkGetComponents,
    kFWKMessageID_InputFrameworkGetDeviceConfigs,

    /* lpm timer message*/
    kFWKMessageID_LpmPreEnterSleep,

    /* raw message which is determined by the sender and receiver */
    kFWKMessageID_Raw,

    kFWKMessageID_Invalid,

} fwk_message_id_t;

/*! @brief Structure of o frame request message */
typedef struct
{
    int devId;
    /* frame buffer width, height, pitch */
    int height;
    int width;
    int pitch;
    /* active rect */
    int left;
    int top;
    int right;
    int bottom;
    /* rotate degree */
    cw_rotate_degree_t rotate;
    flip_mode_t flip;
    /* swap byte per two bytes */
    int swapByte;
    /* pixel format */
    pixel_format_t format;
    /* the source pixel format of the requested frame */
    pixel_format_t srcFormat;
    void *data;
} frame_msg_payload_t;

/*! @brief Structure of a raw message used to send various data */
typedef struct
{
    int devId;
    unsigned char freeAfterConsumed;
    void *data;
} raw_msg_payload_t;

/*! @brief Structure of a grafic message */
typedef struct
{
    gfx_surface_t *pSurface;
} overlay_msg_payload_t;

/*! @brief Structure of an input message */
typedef struct
{
    int devId;
    unsigned int receiverList;
    unsigned int length;
    void *data;
} input_msg_payload_t;

/*! @brief Structure of an audio message */
typedef struct
{
    int devId;
    unsigned int size;
    void *data;
} audio_msg_payload_t;

/*! @brief General structure of a message */
typedef struct
{
    fwk_message_id_t id;
    unsigned char freeAfterConsumed;
    union
    {
        frame_msg_payload_t frame;
        input_msg_payload_t input;
        audio_msg_payload_t audio;
        overlay_msg_payload_t overlay;
        raw_msg_payload_t raw;
        framework_request_t frameworkRequest;
    };
} fwk_message_t;

/**
 * @brief Init the internal structure of the messages
 *
 * @return int Return 0 if the init was successful
 */
int FWK_Message_Init();

/**
 * @brief Register a message queue and assigned it to a taskid
 * @param taskId Id of the task that owns the queue
 * @param queueHandle Queue Handle of the queue that will be linked with the task
 * @return BaseType_t pdTRUE if the registration was done
 */
BaseType_t FWK_Message_RegisterQueue(fwk_task_id_t taskId, QueueHandle_t queueHandle);

/**
 * @brief Add the message into the task queue
 * @param taskId Id of the task that owns the queue
 * @param ppMsg Double pointer to a message structure
 * @return BaseType_t pdTRUE if the message was added to the queue
 */
BaseType_t FWK_Message_Put(fwk_task_id_t taskId, fwk_message_t **ppMsg);

/**
 * @brief Fetch the message from the task queue
 * @param taskId Id of the task that owns the queue
 * @param ppMsg Double pointer to a message structure
 * @return BaseType_t pdTRUE if the message was added to the queue
 */
BaseType_t FWK_Message_Get(fwk_task_id_t taskId, fwk_message_t **ppMsg);

/**
 * @brief Add the message into the task queue from and irq context
 * @param taskId Id of the task that owns the queue
 * @param ppMsg Double pointer to a message structure
 * @return BaseType_t pdTRUE if the message was added to the queue
 */
BaseType_t FWK_Message_PutFromIsr(fwk_task_id_t taskId, fwk_message_t **ppMsg);

const char *FWK_Message_Name(fwk_message_id_t id);

#if defined(__cplusplus)
}
#endif

#endif /*_FWK_MESSAGE_H_*/
