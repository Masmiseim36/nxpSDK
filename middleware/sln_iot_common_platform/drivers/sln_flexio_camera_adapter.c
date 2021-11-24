/*
 * Copyright 2020-2021 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * @brief flexio camera driver adapter implementation.
 */

#include "fsl_gpio.h"
#include "fsl_camera.h"
#include "fsl_camera_receiver.h"
#include "sln_flexio_camera_adapter.h"
#include "fsl_video_common.h"
#include "fsl_edma.h"
#include "fsl_dmamux.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
static flexio_camera_config_t s_flexio_camera_config;
static list_label_t s_flexio_camera_buffer_list;
static flexio_camera_list_element_t s_list_elements[FLEXIO_CAMERA_ADAPTER_BUFFER_MAX];
SDK_ALIGN(static edma_tcd_t s_dma_tcd[2], 32);

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static status_t FLEXIO_CSI_ADAPTER_Init(camera_receiver_handle_t *handle,
                                        const camera_config_t *config,
                                        camera_receiver_callback_t callback,
                                        void *userData);

static status_t FLEXIO_CSI_ADAPTER_Deinit(camera_receiver_handle_t *handle);

static status_t FLEXIO_CSI_ADAPTER_Start(camera_receiver_handle_t *handle);

static status_t FLEXIO_CSI_ADAPTER_Stop(camera_receiver_handle_t *handle);

static status_t FLEXIO_CSI_ADAPTER_SubmitEmptyBuffer(camera_receiver_handle_t *handle, uint32_t buffer);

static status_t FLEXIO_CSI_ADAPTER_GetFullBuffer(camera_receiver_handle_t *handle, uint32_t *buffer);

static status_t FLEXIO_CSI_ADAPTER_InitExt(camera_receiver_handle_t *handle,
                                           const camera_config_t *config,
                                           const void *specialConfig,
                                           camera_receiver_callback_t callback,
                                           void *userData);

static void FLEXIO_CameraCsiConfig(FLEXIO_CAMERA_Type *csiConfig);
static void FLEXIO_CameraXclkConfig(FLEXIO_CAMERA_Type *csiConfig, flexio_camera_xclk_t *xclkConfig);
static void FLEXIO_CameraDmaConfig(FLEXIO_CAMERA_Type *csiConfig,
                                   const camera_config_t *devConfig,
                                   flexio_camera_dma_t *dmaConfig);
static void FLEXIO_CameraVsyncConfig(flexio_camera_vsync_t *vsyncConfig);
static status_t FLEXIO_CameraBufferListInit(void);
static status_t FLEXIO_CameraAllocListElement(flexio_camera_list_element_t **pElement);
static status_t FLEXIO_CameraFreeListElement(flexio_camera_list_element_t *pElement);
/*******************************************************************************
 * Variables
 ******************************************************************************/
const camera_receiver_operations_t flexio_csi_ops = {.init              = FLEXIO_CSI_ADAPTER_Init,
                                                     .deinit            = FLEXIO_CSI_ADAPTER_Deinit,
                                                     .start             = FLEXIO_CSI_ADAPTER_Start,
                                                     .stop              = FLEXIO_CSI_ADAPTER_Stop,
                                                     .submitEmptyBuffer = FLEXIO_CSI_ADAPTER_SubmitEmptyBuffer,
                                                     .getFullBuffer     = FLEXIO_CSI_ADAPTER_GetFullBuffer,
                                                     .init_ext          = FLEXIO_CSI_ADAPTER_InitExt};

/*******************************************************************************
 * Code
 ******************************************************************************/
status_t FLEXIO_CSI_ADAPTER_Init(camera_receiver_handle_t *handle,
                                 const camera_config_t *config,
                                 camera_receiver_callback_t callback,
                                 void *userData)
{
    flexio_camera_status_t status = kStatus_Flexio_Camera_Success;

    FLEXIO_CAMERA_Type *csiConfig             = ((flexio_csi_resource_t *)handle->resource)->flexioCsi;
    flexio_camera_xclk_t *xclkConfig          = ((flexio_csi_resource_t *)handle->resource)->flexioXclk;
    flexio_camera_dma_t *dmaConfig            = ((flexio_csi_resource_t *)handle->resource)->flexioDma;
    flexio_camera_private_data_t *privateData = (flexio_camera_private_data_t *)handle->privateData;

    /* Camera shifter/timer configure. */
    FLEXIO_CameraCsiConfig(csiConfig);

    /* Camera XCLK timer configure. */
    FLEXIO_CameraXclkConfig(csiConfig, xclkConfig);

    /* Enable FlexIO. */
    FLEXIO_CAMERA_Enable(csiConfig, true);

    /* Camera DMA configure. */
    FLEXIO_CameraDmaConfig(csiConfig, config, dmaConfig);

    /* Enable FlexIO DMA request. */
    FLEXIO_CAMERA_EnableRxDMA(csiConfig, true);

    /* Init list data. */
    memset(&privateData->lists, 0, sizeof(flexio_camera_list_t));
    LIST_Init(&privateData->lists.emptyList, FLEXIO_CAMERA_ADAPTER_BUFFER_MAX);
    LIST_Init(&privateData->lists.fullList, FLEXIO_CAMERA_ADAPTER_BUFFER_MAX);

    FLEXIO_CameraBufferListInit();

    privateData->callback = callback;
    privateData->userData = userData;

    return status;
}

status_t FLEXIO_CSI_ADAPTER_Deinit(camera_receiver_handle_t *handle)
{
    flexio_camera_status_t status = kStatus_Flexio_Camera_Success;
    return status;
}

status_t FLEXIO_CSI_ADAPTER_Start(camera_receiver_handle_t *handle)
{
    flexio_camera_status_t status = kStatus_Flexio_Camera_Success;

    flexio_camera_vsync_t *vsyncConfig = ((flexio_csi_resource_t *)handle->resource)->flexioVsync;

    /* Init VSYNC pin, and enable the GPIO interrupt. */
    FLEXIO_CameraVsyncConfig(vsyncConfig);

    return status;
}

status_t FLEXIO_CSI_ADAPTER_Stop(camera_receiver_handle_t *handle)
{
    flexio_camera_status_t status = kStatus_Flexio_Camera_Success;
    return status;
}

status_t FLEXIO_CSI_ADAPTER_SubmitEmptyBuffer(camera_receiver_handle_t *handle, uint32_t buffer)
{
    flexio_camera_status_t status         = kStatus_Flexio_Camera_Success;
    list_status_t list_status             = kLIST_Ok;
    flexio_camera_list_element_t *element = NULL;

    assert(handle != NULL);
    assert(handle->privateData != NULL);

    flexio_camera_list_t *lists = &((flexio_camera_private_data_t *)handle->privateData)->lists;
    if (FLEXIO_CameraAllocListElement(&element) != kStatus_Flexio_Camera_Success)
    {
        return kStatus_Flexio_Camera_Error;
    }

    element->bufferAddress = (uint8_t *)buffer;
    list_status            = LIST_AddTail(&lists->emptyList, &element->listElement);
    if (kLIST_Full == list_status)
    {
        status = kStatus_Flexio_Camera_NoEmptyBuffer;
    }
    else if (kLIST_Ok != list_status)
    {
        status = kStatus_Flexio_Camera_Error;
    }

    return status;
}

status_t FLEXIO_CSI_ADAPTER_GetFullBuffer(camera_receiver_handle_t *handle, uint32_t *buffer)
{
    flexio_camera_status_t status = kStatus_Flexio_Camera_Success;

    assert(handle != NULL);
    assert(handle->privateData != NULL);

    flexio_camera_list_t *lists = &((flexio_camera_private_data_t *)handle->privateData)->lists;

    list_element_handle_t element = LIST_RemoveHead(&lists->fullList);
    if (element != NULL)
    {
        *buffer = (uint32_t)((flexio_camera_list_element_t *)element)->bufferAddress;
        if (FLEXIO_CameraFreeListElement((flexio_camera_list_element_t *)element) != kStatus_Flexio_Camera_Success)
        {
            return kStatus_Flexio_Camera_Error;
        }
    }
    else
    {
        status = kStatus_Flexio_Camera_NoFullBuffer;
    }

    return status;
}

status_t FLEXIO_CSI_ADAPTER_InitExt(camera_receiver_handle_t *handle,
                                    const camera_config_t *config,
                                    const void *specialConfig,
                                    camera_receiver_callback_t callback,
                                    void *userData)
{
    flexio_camera_status_t status = kStatus_Flexio_Camera_Success;
    return status;
}

status_t FLEXIO_CSI_ADAPTER_Callback(camera_receiver_handle_t *handle)
{
    flexio_camera_status_t status = kStatus_Flexio_Camera_Success;

    flexio_camera_private_data_t *privateData = (flexio_camera_private_data_t *)handle->privateData;

    flexio_camera_list_t *lists = &((flexio_camera_private_data_t *)handle->privateData)->lists;
    flexio_camera_dma_t *dma    = ((flexio_csi_resource_t *)handle->resource)->flexioDma;
    uint32_t bufferAddr;

    assert(handle != NULL);
    assert(handle->privateData != NULL);

    if (lists->currentElement != NULL)
    {
        LIST_AddTail(&lists->fullList, lists->currentElement);
        lists->currentElement = NULL;
    }

    lists->currentElement = LIST_RemoveHead(&lists->emptyList);
    if (lists->currentElement != NULL)
    {
        bufferAddr = (uint32_t)(((flexio_camera_list_element_t *)lists->currentElement)->bufferAddress);
    }
    else
    {
        return kStatus_Flexio_Camera_NoEmptyBuffer;
    }

    FLEXIO_CAMERA_ClearStatusFlags(((flexio_csi_resource_t *)handle->resource)->flexioCsi,
                                   kFLEXIO_CAMERA_RxDataRegFullFlag | kFLEXIO_CAMERA_RxErrorFlag);

    s_dma_tcd[0].DADDR = (uint32_t)bufferAddr;
    s_dma_tcd[1].DADDR = (uint32_t)bufferAddr + dma->tcdLinkOffset;

    EDMA_InstallTCD(DMA0, dma->dmaCh, &s_dma_tcd[0]);

    /* Enable DMA channel request. */
    DMA0->SERQ = DMA_SERQ_SERQ(dma->dmaCh);

    if (privateData->callback != NULL)
    {
        privateData->callback(handle, 0, privateData->userData);
    }

    return status;
}

static void FLEXIO_CameraCsiConfig(FLEXIO_CAMERA_Type *csiConfig)
{
    /* Init flexio camera mode */
    FLEXIO_CAMERA_GetDefaultConfig(&s_flexio_camera_config);
    FLEXIO_CAMERA_Init(csiConfig, &s_flexio_camera_config);

    /* Clear all the flag. */
    FLEXIO_CAMERA_ClearStatusFlags(csiConfig, kFLEXIO_CAMERA_RxDataRegFullFlag | kFLEXIO_CAMERA_RxErrorFlag);
}

static void FLEXIO_CameraXclkConfig(FLEXIO_CAMERA_Type *csiConfig, flexio_camera_xclk_t *xclkConfig)
{
    flexio_timer_config_t timerConfig;

    timerConfig.triggerSelect   = 0u;
    timerConfig.triggerPolarity = kFLEXIO_TimerTriggerPolarityActiveHigh;
    timerConfig.triggerSource   = kFLEXIO_TimerTriggerSourceInternal;
    timerConfig.pinConfig       = kFLEXIO_PinConfigOutput;
    timerConfig.pinSelect       = xclkConfig->pinIdx;
    timerConfig.pinPolarity     = kFLEXIO_PinActiveHigh;
    timerConfig.timerMode       = kFLEXIO_TimerModeDual8BitPWM;
    timerConfig.timerOutput     = kFLEXIO_TimerOutputZeroNotAffectedByReset;
    timerConfig.timerDecrement  = kFLEXIO_TimerDecSrcOnFlexIOClockShiftTimerOutput;
    timerConfig.timerReset      = kFLEXIO_TimerResetNever;
    timerConfig.timerDisable    = kFLEXIO_TimerDisableNever;
    timerConfig.timerEnable     = kFLEXIO_TimerEnabledAlways;
    timerConfig.timerStop       = kFLEXIO_TimerStopBitDisabled;
    timerConfig.timerStart      = kFLEXIO_TimerStartBitDisabled;
    timerConfig.timerCompare    = 0x0404; /* 120MHz clock source generates 24MHz clock.*/

    FLEXIO_SetTimerConfig(csiConfig->flexioBase, xclkConfig->timerIdx, &timerConfig);
}

#ifdef CPU_MIMXRT1062DVL6A
static void FLEXIO_CameraDmaConfig(FLEXIO_CAMERA_Type *csiConfig,
                                   const camera_config_t *devConfig,
                                   flexio_camera_dma_t *dmaConfig)
{
    uint32_t dmaCh = dmaConfig->dmaCh;
    uint32_t smod  = 0;
    uint32_t soff = 8, size = 3u; /* 8bytes dma offset, 64bits DMA burst.*/

    uint32_t dmaMinorLoopSize = csiConfig->shifterCount * 32 / 8;
    uint32_t dmaMajorLoopSize = FSL_VIDEO_EXTRACT_WIDTH(devConfig->resolution) *
                                FSL_VIDEO_EXTRACT_HEIGHT(devConfig->resolution) * devConfig->bytesPerPixel /
                                dmaMinorLoopSize;

    dma_request_source_t dmaRequestSource;

    dmaConfig->majorLoop     = dmaMajorLoopSize;
    dmaConfig->tcdLinkOffset = FSL_VIDEO_EXTRACT_WIDTH(devConfig->resolution) *
                               FSL_VIDEO_EXTRACT_HEIGHT(devConfig->resolution) * devConfig->bytesPerPixel / 2;

    /* smod = log2(dmaMinorLoopSize) */
    while (1u << smod < dmaMinorLoopSize)
    {
        smod++;
    }

    s_dma_tcd[0].SADDR     = FLEXIO_CAMERA_GetRxBufferAddress(csiConfig);
    s_dma_tcd[0].SOFF      = soff;
    s_dma_tcd[0].ATTR      = DMA_ATTR_SMOD(smod) | DMA_ATTR_SSIZE(size) | DMA_ATTR_DMOD(0u) | DMA_ATTR_DSIZE(size);
    s_dma_tcd[0].NBYTES    = dmaMinorLoopSize;
    s_dma_tcd[0].SLAST     = 0u;
    s_dma_tcd[0].DADDR     = 0u;
    s_dma_tcd[0].DOFF      = soff;
    s_dma_tcd[0].CITER     = dmaMajorLoopSize / 2;
    s_dma_tcd[0].DLAST_SGA = (uint32_t)&s_dma_tcd[1];
    s_dma_tcd[0].CSR       = DMA_CSR_ESG_MASK;
    s_dma_tcd[0].BITER     = dmaMajorLoopSize / 2;

    s_dma_tcd[1].SADDR     = FLEXIO_CAMERA_GetRxBufferAddress(csiConfig);
    s_dma_tcd[1].SOFF      = soff;
    s_dma_tcd[1].ATTR      = DMA_ATTR_SMOD(smod) | DMA_ATTR_SSIZE(size) | DMA_ATTR_DMOD(0u) | DMA_ATTR_DSIZE(size);
    s_dma_tcd[1].NBYTES    = dmaMinorLoopSize;
    s_dma_tcd[1].SLAST     = 0u;
    s_dma_tcd[1].DADDR     = 0u;
    s_dma_tcd[1].DOFF      = soff;
    s_dma_tcd[1].CITER     = dmaMajorLoopSize / 2;
    s_dma_tcd[1].DLAST_SGA = (uint32_t)&s_dma_tcd[0];
    s_dma_tcd[1].CSR       = DMA_CSR_DREQ_MASK | DMA_CSR_ESG_MASK;
    s_dma_tcd[1].BITER     = dmaMajorLoopSize / 2;

    EDMA_InstallTCD(DMA0, dmaCh, &s_dma_tcd[0]);

    if (csiConfig->flexioBase == FLEXIO1)
    {
        dmaRequestSource = kDmaRequestMuxFlexIO1Request0Request1;
    }
    else if (csiConfig->flexioBase == FLEXIO2)
    {
        dmaRequestSource = kDmaRequestMuxFlexIO2Request0Request1;
    }

    /* Configure DMA MUX Source */
    DMAMUX->CHCFG[dmaCh] = ((DMAMUX->CHCFG[dmaCh] & ~DMAMUX_CHCFG_SOURCE_MASK) | DMAMUX_CHCFG_SOURCE(dmaRequestSource));

    /* Enable DMA channel. */
    DMAMUX->CHCFG[dmaCh] |= DMAMUX_CHCFG_ENBL_MASK;
}
#endif

#ifdef CPU_MIMXRT117HDVMAA
static void FLEXIO_CameraDmaConfig(FLEXIO_CAMERA_Type *csiConfig,
                                   const camera_config_t *devConfig,
                                   flexio_camera_dma_t *dmaConfig)
{
    uint32_t dmaCh = dmaConfig->dmaCh;
    uint32_t smod  = 0;
    uint32_t soff = 32, size = 5u; /* 32bytes dma offset, 32-byte(4 beats of 64 bits) DMA burst.*/

    uint32_t dmaMinorLoopSize = csiConfig->shifterCount * 32 / 8;
    uint32_t dmaMajorLoopSize = FSL_VIDEO_EXTRACT_WIDTH(devConfig->resolution) *
                                FSL_VIDEO_EXTRACT_HEIGHT(devConfig->resolution) * devConfig->bytesPerPixel /
                                dmaMinorLoopSize;

    dma_request_source_t dmaRequestSource;

    dmaConfig->majorLoop     = dmaMajorLoopSize;
    dmaConfig->tcdLinkOffset = FSL_VIDEO_EXTRACT_WIDTH(devConfig->resolution) *
                               FSL_VIDEO_EXTRACT_HEIGHT(devConfig->resolution) * devConfig->bytesPerPixel / 2;

    /* smod = log2(dmaMinorLoopSize) */
    while (1u << smod < dmaMinorLoopSize)
    {
        smod++;
    }

    s_dma_tcd[0].SADDR     = FLEXIO_CAMERA_GetRxBufferAddress(csiConfig);
    s_dma_tcd[0].SOFF      = soff;
    s_dma_tcd[0].ATTR      = DMA_ATTR_SMOD(smod) | DMA_ATTR_SSIZE(size) | DMA_ATTR_DMOD(0u) | DMA_ATTR_DSIZE(size);
    s_dma_tcd[0].NBYTES    = dmaMinorLoopSize;
    s_dma_tcd[0].SLAST     = 0u;
    s_dma_tcd[0].DADDR     = 0u;
    s_dma_tcd[0].DOFF      = soff;
    s_dma_tcd[0].CITER     = dmaMajorLoopSize;
    s_dma_tcd[0].DLAST_SGA = 0;
    s_dma_tcd[0].CSR       = DMA_CSR_DREQ_MASK;
    s_dma_tcd[0].BITER     = dmaMajorLoopSize;

    EDMA_InstallTCD(DMA0, dmaCh, &s_dma_tcd[0]);

    if (csiConfig->flexioBase == FLEXIO1)
    {
        dmaRequestSource = kDmaRequestMuxFlexIO1Request0Request1;
    }
    else if (csiConfig->flexioBase == FLEXIO2)
    {
        dmaRequestSource = kDmaRequestMuxFlexIO2Request0Request1;
    }

    /* Configure DMA MUX Source */
    DMAMUX0->CHCFG[dmaCh] =
        ((DMAMUX0->CHCFG[dmaCh] & ~DMAMUX_CHCFG_SOURCE_MASK) | DMAMUX_CHCFG_SOURCE(dmaRequestSource));

    /* Enable DMA channel. */
    DMAMUX0->CHCFG[dmaCh] |= DMAMUX_CHCFG_ENBL_MASK;
}
#endif

static void FLEXIO_CameraVsyncConfig(flexio_camera_vsync_t *vsyncConfig)
{
    gpio_pin_config_t gpioPinConfig = {.direction = kGPIO_DigitalInput, .interruptMode = kGPIO_IntRisingEdge};

    GPIO_PinInit(vsyncConfig->portBase, vsyncConfig->pinIdx, &gpioPinConfig);
    GPIO_ClearPinsInterruptFlags(vsyncConfig->portBase, 1 << vsyncConfig->pinIdx);
    GPIO_EnableInterrupts(vsyncConfig->portBase, 1u << vsyncConfig->pinIdx);
    NVIC_SetPriority(vsyncConfig->irq, vsyncConfig->irqPriority);
    EnableIRQ(vsyncConfig->irq);
}

static status_t FLEXIO_CameraBufferListInit(void)
{
    flexio_camera_status_t status = kStatus_Flexio_Camera_Success;

    LIST_Init(&s_flexio_camera_buffer_list, FLEXIO_CAMERA_ADAPTER_BUFFER_MAX);
    for (int i = 0; i < FLEXIO_CAMERA_ADAPTER_BUFFER_MAX; i++)
    {
        LIST_AddTail(&s_flexio_camera_buffer_list, &(s_list_elements[i].listElement));
    }

    return status;
}

static status_t FLEXIO_CameraAllocListElement(flexio_camera_list_element_t **pElement)
{
    flexio_camera_status_t status = kStatus_Flexio_Camera_Success;

    *pElement = (flexio_camera_list_element_t *)LIST_RemoveHead(&s_flexio_camera_buffer_list);
    if (*pElement == NULL)
    {
        status = kStatus_Flexio_Camera_Error;
    }

    return status;
}

static status_t FLEXIO_CameraFreeListElement(flexio_camera_list_element_t *pElement)
{
    flexio_camera_status_t status = kStatus_Flexio_Camera_Success;

    if (LIST_AddTail(&s_flexio_camera_buffer_list, &(pElement->listElement)) != kLIST_Ok)
    {
        status = kStatus_Flexio_Camera_Error;
    }

    return status;
}
