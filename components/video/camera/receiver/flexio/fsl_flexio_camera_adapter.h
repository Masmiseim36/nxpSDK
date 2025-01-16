/*
 * Copyright 2017, 2020, 2023 NXP
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _FSL_FLEXIO_CAMERA_ADAPTER_H_
#define _FSL_FLEXIO_CAMERA_ADAPTER_H_

#include "fsl_common.h"
#include "fsl_flexio_camera.h"
#include "fsl_dmamux.h"
#include "fsl_edma.h"
#include "fsl_camera_receiver.h"

/*
 * Change log:
 *
 *   1.0.2
 *     - Fixed build errors.
 *
 *   1.0.1
 *     - Fixed MISRA-C 2012 issues.
 *
 *   1.0.0
 *     - Initial version
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief Size of the frame buffer queue used in FLEXIO CAMERA Queue. */
#ifndef FLEXIO_CAMERA_QUEUE_SIZE
#define FLEXIO_CAMERA_QUEUE_SIZE 4U
#endif

/*! @brief Error codes for the Camera driver. */
enum _flexio_camera_queue_status
{
    kStatus_FLEXIO_CAMERA_QueueFrameDone = MAKE_STATUS(kStatusGroup_FLEXIO_CAMERA, 2),
    /*!< Frame ceceive done in queue. */
    kStatus_FLEXIO_CAMERA_QueueNoEmptyBuffer = MAKE_STATUS(kStatusGroup_FLEXIO_CAMERA, 3),
    /*!< No empty buffer in the queue. */
    kStatus_FLEXIO_CAMERA_QueueFull = MAKE_STATUS(kStatusGroup_FLEXIO_CAMERA, 4),
    /*!< Queue full. */
    kStatus_FLEXIO_CAMERA_QueueNoFullBuffer = MAKE_STATUS(kStatusGroup_FLEXIO_CAMERA, 5),
    /*!< No full buffer ready in the queue. */
};

/*!
 * @brief The resources used by the FLEXIO camera queue.
 *
 */
typedef struct _flexio_camera_queue
{
    uint32_t frameBufferQueue[FLEXIO_CAMERA_QUEUE_SIZE + 1]; /*!< Frame buffer queue. */

    volatile uint8_t queueUserReadIdx;  /*!< Application gets full-filled frame buffer from this index. */
    volatile uint8_t queueUserWriteIdx; /*!< Application puts empty frame buffer to this index. */
    volatile uint8_t queueDrvReadIdx;   /*!< Driver gets empty frame buffer from this index. */
    volatile uint8_t queueDrvWriteIdx;  /*!< Driver puts the full-filled frame buffer to this index. */
    volatile bool transferStarted; /*!< User has called @ref FLEXIO_CAMERA_ADAPTER_Start to start frame receiving. */
} flexio_camera_queue_t;

/*! @brief The private data used by the FLEXIO camera receiver. */
typedef struct _flexio_camera_private_data
{
    flexio_camera_queue_t cameraQueue;   /*!< FLEXIO camera queue. */
    uint32_t frameBytes;                 /*!< Frame bytes in each frame. */
    camera_receiver_callback_t callback; /*!< Save the callback. */
    void *userData;                      /*!< Parameter for the callback. */
} flexio_camera_private_data_t;

/*!
 * @brief The resources used by the FLEXIO camera receiver.
 *
 * Don't need to initialize the resource before FLEXIO initialization.
 */
typedef struct _flexio_camera_resource
{
    FLEXIO_CAMERA_Type device; /*!< FLEXIO camera device. */
    DMAMUX_Type *dmamuxBase;   /*!< DMAMUX base address. */
    DMA_Type *dmaBase;         /*!< EDMA base address. */
    uint8_t dmaChannel;        /*!< FLEXIO DMA Channel number. */
    void (*vsyncInit)(void);   /*!< Configure vsync pin interrupt. */
} flexio_camera_resource_t;

/*! @brief FLEXIO camera receiver operations structure. */
extern const camera_receiver_operations_t flexio_camera_ops;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

void FLEXIO_CAMERA_VsyncHandleIRQ(camera_receiver_handle_t *handle);

#if defined(__cplusplus)
}
#endif

#endif /* _FSL_FLEXIO_CAMERA_ADAPTER_H_ */
