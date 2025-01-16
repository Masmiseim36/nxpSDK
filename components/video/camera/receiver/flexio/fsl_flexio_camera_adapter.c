/*
 * Copyright 2017, 2020, 2023 NXP
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_camera.h"
#include "fsl_camera_receiver.h"
#include "fsl_flexio_camera_adapter.h"
#include "fsl_gpio.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

static status_t FLEXIO_CAMERA_ADAPTER_Init(camera_receiver_handle_t *handle,
                                           const camera_config_t *config,
                                           camera_receiver_callback_t callback,
                                           void *userData);

static status_t FLEXIO_CAMERA_ADAPTER_Deinit(camera_receiver_handle_t *handle);

static status_t FLEXIO_CAMERA_ADAPTER_Start(camera_receiver_handle_t *handle);

static status_t FLEXIO_CAMERA_ADAPTER_Stop(camera_receiver_handle_t *handle);

static status_t FLEXIO_CAMERA_ADAPTER_SubmitEmptyBuffer(camera_receiver_handle_t *handle, uint32_t buffer);

static status_t FLEXIO_CAMERA_ADAPTER_GetFullBuffer(camera_receiver_handle_t *handle, uint32_t *buffer);

static status_t FLEXIO_CAMERA_ADAPTER_InitExt(camera_receiver_handle_t *handle,
                                              const camera_config_t *config,
                                              const void *specialConfig,
                                              camera_receiver_callback_t callback,
                                              void *userData);

/*******************************************************************************
 * Variables
 ******************************************************************************/
const camera_receiver_operations_t flexio_camera_ops = {
    .init              = FLEXIO_CAMERA_ADAPTER_Init,
    .deinit            = FLEXIO_CAMERA_ADAPTER_Deinit,
    .start             = FLEXIO_CAMERA_ADAPTER_Start,
    .stop              = FLEXIO_CAMERA_ADAPTER_Stop,
    .submitEmptyBuffer = FLEXIO_CAMERA_ADAPTER_SubmitEmptyBuffer,
    .getFullBuffer     = FLEXIO_CAMERA_ADAPTER_GetFullBuffer,
    .init_ext          = FLEXIO_CAMERA_ADAPTER_InitExt,
};

/*******************************************************************************
 * Code
 ******************************************************************************/

static uint32_t FLEXIO_CAMERA_TransferGetQueueDelta(uint32_t startIdx, uint32_t endIdx)
{
    if (endIdx >= startIdx)
    {
        return endIdx - startIdx;
    }
    else
    {
        return startIdx + FLEXIO_CAMERA_QUEUE_SIZE - endIdx;
    }
}

static uint32_t FLEXIO_CAMERA_TransferIncreaseQueueIdx(uint32_t idx)
{
    uint32_t ret;

    /*
     * Here not use the method:
     * ret = (idx+1) % FLEXIO_CAMERA_QUEUE_SIZE;
     *
     * Because the mod function might be slow.
     */

    ret = idx + 1U;

    if (ret >= FLEXIO_CAMERA_QUEUE_SIZE)
    {
        ret = 0U;
    }

    return ret;
}

static uint32_t FLEXIO_CAMERA_TransferGetEmptyBufferCount(flexio_camera_queue_t *cameraQueue)
{
    return FLEXIO_CAMERA_TransferGetQueueDelta(cameraQueue->queueDrvReadIdx, cameraQueue->queueUserWriteIdx);
}

static void FLEXIO_CAMERA_TransferLoadBufferToDevice(flexio_camera_resource_t *resource,
                                                     flexio_camera_private_data_t *privateData)
{
    flexio_camera_queue_t *cameraQueue = &(privateData->cameraQueue);

    /* Reset the EDMA source, dest address and transfer counter to sync picture frame */
    resource->dmaBase->TCD[resource->dmaChannel].SADDR = FLEXIO_CAMERA_GetRxBufferAddress(&(resource->device));
    resource->dmaBase->TCD[resource->dmaChannel].DADDR = cameraQueue->frameBufferQueue[cameraQueue->queueDrvReadIdx];
    resource->dmaBase->TCD[resource->dmaChannel].CITER_ELINKNO = (uint16_t)((privateData->frameBytes) >> 5);
    resource->dmaBase->TCD[resource->dmaChannel].BITER_ELINKNO = (uint16_t)((privateData->frameBytes) >> 5);

    cameraQueue->queueDrvReadIdx = (uint8_t)FLEXIO_CAMERA_TransferIncreaseQueueIdx(cameraQueue->queueDrvReadIdx);
}

void FLEXIO_CAMERA_VsyncHandleIRQ(camera_receiver_handle_t *handle)
{
    flexio_camera_resource_t *resource        = handle->resource;
    flexio_camera_private_data_t *privateData = (flexio_camera_private_data_t *)(handle->privateData);
    flexio_camera_queue_t *cameraQueue        = &(privateData->cameraQueue);

    FLEXIO_CAMERA_ClearStatusFlags(&(resource->device),
                                   (uint32_t)kFLEXIO_CAMERA_RxDataRegFullFlag | (uint32_t)kFLEXIO_CAMERA_RxErrorFlag);
    FLEXIO_CAMERA_EnableRxDMA(&(resource->device), false);

    cameraQueue->queueDrvWriteIdx = (uint8_t)FLEXIO_CAMERA_TransferIncreaseQueueIdx(cameraQueue->queueDrvWriteIdx);
    if ((cameraQueue->transferStarted) && (0U != FLEXIO_CAMERA_TransferGetEmptyBufferCount(cameraQueue)))
    {
        /* Reset the EDMA source, dest address and transfer counter to sync picture frame */
        FLEXIO_CAMERA_TransferLoadBufferToDevice(resource, privateData);
        FLEXIO_CAMERA_EnableRxDMA(&(resource->device), true);
    }
}

static void EDMA_ChannelTransferInit(DMA_Type *EDMAUsed, DMAMUX_Type *dmamux, uint8_t channelNum, uint8_t source)
{
    DMAMUX_SetSource(dmamux, channelNum, source);
    EDMA_SetModulo(EDMAUsed, channelNum, kEDMA_ModuloDisable, kEDMA_ModuloDisable);
    EDMA_ClearChannelStatusFlags(EDMAUsed, channelNum, kEDMA_DoneFlag);
    EDMA_EnableChannelRequest(EDMAUsed, channelNum);
    DMAMUX_EnableChannel(dmamux, channelNum);
}

static status_t FLEXIO_CAMERA_ADAPTER_Init(camera_receiver_handle_t *handle,
                                           const camera_config_t *config,
                                           camera_receiver_callback_t callback,
                                           void *userData)
{
    flexio_camera_resource_t *resource        = (flexio_camera_resource_t *)(handle->resource);
    flexio_camera_private_data_t *privateData = (flexio_camera_private_data_t *)(handle->privateData);

    privateData->callback = callback;
    privateData->userData = userData;

    /* Init FLEXIO CAMERA. */
    flexio_camera_config_t flexioCameraConfig;
    FLEXIO_CAMERA_GetDefaultConfig(&flexioCameraConfig);
    flexioCameraConfig.enablecamera = true;
    FLEXIO_CAMERA_Init(&(resource->device), &flexioCameraConfig);

    /* Calculate frameBytes. */
    privateData->frameBytes =
        (config->resolution & 0xFFFFU) * ((config->resolution & 0xFFFF0000U) >> 16U) * config->bytesPerPixel;

    /* Reset the camera queue. */
    (void)memset(&(privateData->cameraQueue), 0, sizeof(flexio_camera_queue_t));

    /* Clear all the flag. */
    FLEXIO_CAMERA_ClearStatusFlags(&(resource->device),
                                   (uint32_t)kFLEXIO_CAMERA_RxDataRegFullFlag | (uint32_t)kFLEXIO_CAMERA_RxErrorFlag);

    /* Enable the flexio edma request:  from frame buffer */
    FLEXIO_EnableShifterStatusDMA(resource->device.flexioBase, 0x02, true);

    resource->vsyncInit();

    return kStatus_Success;
}

static status_t FLEXIO_CAMERA_ADAPTER_Start(camera_receiver_handle_t *handle)
{
    assert(handle);

    flexio_camera_resource_t *resource        = (flexio_camera_resource_t *)(handle->resource);
    flexio_camera_private_data_t *privateData = (flexio_camera_private_data_t *)(handle->privateData);
    flexio_camera_queue_t *cameraQueue        = &(privateData->cameraQueue);
    uint32_t emptyBufferCount;

    emptyBufferCount = FLEXIO_CAMERA_TransferGetEmptyBufferCount(cameraQueue);

    if (emptyBufferCount < 1U)
    {
        return (status_t)kStatus_FLEXIO_CAMERA_QueueNoEmptyBuffer;
    }

    cameraQueue->transferStarted = true;

    resource->dmaBase->TCD[resource->dmaChannel].SADDR = FLEXIO_CAMERA_GetRxBufferAddress(&(resource->device));
    resource->dmaBase->TCD[resource->dmaChannel].SOFF  = 0;
    resource->dmaBase->TCD[resource->dmaChannel].ATTR =
        DMA_ATTR_SSIZE(kEDMA_TransferSize32Bytes) | DMA_ATTR_DSIZE(kEDMA_TransferSize32Bytes);
    resource->dmaBase->TCD[resource->dmaChannel].NBYTES_MLNO = 32;
    resource->dmaBase->TCD[resource->dmaChannel].SLAST       = 0;
    resource->dmaBase->TCD[resource->dmaChannel].DADDR = cameraQueue->frameBufferQueue[cameraQueue->queueDrvReadIdx];
    resource->dmaBase->TCD[resource->dmaChannel].DOFF  = 32;
    resource->dmaBase->TCD[resource->dmaChannel].CITER_ELINKNO = (uint16_t)((privateData->frameBytes >> 5));
    resource->dmaBase->TCD[resource->dmaChannel].DLAST_SGA     = (uint32_t)(-((int32_t)privateData->frameBytes));
    resource->dmaBase->TCD[resource->dmaChannel].CSR           = 0;
    resource->dmaBase->TCD[resource->dmaChannel].BITER_ELINKNO = (uint16_t)((privateData->frameBytes >> 5));

    cameraQueue->queueDrvReadIdx = (uint8_t)FLEXIO_CAMERA_TransferIncreaseQueueIdx(cameraQueue->queueDrvReadIdx);
    EDMA_ChannelTransferInit(resource->dmaBase, resource->dmamuxBase, resource->dmaChannel,
                             ((uint8_t)resource->device.shifterStartIdx + 1U));

    return kStatus_Success;
}

static status_t FLEXIO_CAMERA_ADAPTER_Stop(camera_receiver_handle_t *handle)
{
    assert(handle);

    flexio_camera_resource_t *resource        = (flexio_camera_resource_t *)(handle->resource);
    flexio_camera_private_data_t *privateData = (flexio_camera_private_data_t *)(handle->privateData);
    flexio_camera_queue_t *cameraQueue        = &(privateData->cameraQueue);

    FLEXIO_CAMERA_EnableRxDMA(&(resource->device), false);
    cameraQueue->transferStarted = false;

    /* Stoped, reset the state flags. */
    cameraQueue->queueDrvReadIdx = cameraQueue->queueDrvWriteIdx;

    return kStatus_Success;
}

static status_t FLEXIO_CAMERA_ADAPTER_SubmitEmptyBuffer(camera_receiver_handle_t *handle, uint32_t buffer)
{
    flexio_camera_private_data_t *privateData = (flexio_camera_private_data_t *)(handle->privateData);
    flexio_camera_queue_t *cameraQueue        = &(privateData->cameraQueue);

    if (FLEXIO_CAMERA_QUEUE_SIZE ==
        FLEXIO_CAMERA_TransferGetQueueDelta(cameraQueue->queueUserReadIdx, cameraQueue->queueUserWriteIdx))
    {
        return (status_t)kStatus_FLEXIO_CAMERA_QueueFull;
    }

    /* Save the empty frame buffer address to queue. */
    cameraQueue->frameBufferQueue[cameraQueue->queueUserWriteIdx] = buffer;
    cameraQueue->queueUserWriteIdx = (uint8_t)FLEXIO_CAMERA_TransferIncreaseQueueIdx(cameraQueue->queueUserWriteIdx);

    return kStatus_Success;
}

static status_t FLEXIO_CAMERA_ADAPTER_GetFullBuffer(camera_receiver_handle_t *handle, uint32_t *buffer)
{
    flexio_camera_resource_t *resource        = (flexio_camera_resource_t *)(handle->resource);
    flexio_camera_private_data_t *privateData = (flexio_camera_private_data_t *)(handle->privateData);
    flexio_camera_queue_t *cameraQueue        = &(privateData->cameraQueue);

    /* No full frame buffer. */
    if (cameraQueue->queueUserReadIdx == cameraQueue->queueDrvWriteIdx)
    {
        return (status_t)kStatus_FLEXIO_CAMERA_QueueNoFullBuffer;
    }

    /* Disable the DMA to protect the index information in camera queue. */
    FLEXIO_CAMERA_EnableRxDMA(&(resource->device), false);

    *buffer = cameraQueue->frameBufferQueue[cameraQueue->queueUserReadIdx];

    cameraQueue->queueUserReadIdx = (uint8_t)FLEXIO_CAMERA_TransferIncreaseQueueIdx(cameraQueue->queueUserReadIdx);

    return kStatus_Success;
}

static status_t FLEXIO_CAMERA_ADAPTER_Deinit(camera_receiver_handle_t *handle)
{
    return FLEXIO_CAMERA_ADAPTER_Stop(handle);
}

static status_t FLEXIO_CAMERA_ADAPTER_InitExt(camera_receiver_handle_t *handle,
                                              const camera_config_t *config,
                                              const void *specialConfig,
                                              camera_receiver_callback_t callback,
                                              void *userData)
{
    return FLEXIO_CAMERA_ADAPTER_Init(handle, config, callback, userData);
}
