/*
 * Copyright 2018 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef __SLN_PDM_MIC_H__
#define __SLN_PDM_MIC_H__

#include "fsl_common.h"
#include "fsl_edma.h"

#include "FreeRTOS.h"
#include "event_groups.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

typedef int32_t pdm_mic_status_t;

/**
 * @brief PDM microphone return codes
 */
typedef enum _pdm_mic_states
{
    kPdmMicNullPointer = -1,
    kPdmMicSuccess     = 0,
} pdm_mic_states_t;

/**
 * @brief Left rising edge or right falling edge
 */
typedef enum _mic_channel
{
    kLtOutputRisingEdge,
    kRtOutputFallingEdge
} pdm_mic_channel_t;

/**
 * @brief PDM microphone channel mask
 */
typedef enum _pdm_mic_mask
{
    kMicChannel1 = (1U << 0U),
    kMicChannel2 = (1U << 1U),
    kMicChannel3 = (1U << 2U),
    kMicChannel4 = (1U << 3U),
} pdm_mic_mask_t;

/**
 * @brief Microphone array configuration structure
 */
typedef struct _mic_config
{
    I2S_Type *sai;
    uint8_t saiChannelMask;
    uint32_t sampleRateHz;
    uint32_t pdmCaptureSize;
    uint32_t pdmCaptureCount;
    pdm_mic_channel_t micChannel;

} pdm_mic_config_t;

enum _tcd_count
{
    kTcdCount = 2U
};

/**
 * @brief Microphone handler structure
 */
typedef struct _pdm_mic_handle
{
    edma_tcd_t dmaTcd[kTcdCount]; /**< This structure is same as TCD register
                                       which is described in reference manual */
    pdm_mic_config_t *config;     /**< Microphone configuration */
    DMA_Type *dma;                /**< DMA interface */
    uint32_t dmaChannel;          /**< DMA channel */
    uint32_t dmaIrqNum;           /**< DMA intrerupt number */
    uint8_t dmaRequest;
    EventGroupHandle_t eventGroup; /**< Event group handler */
    EventBits_t pongFlag;
    EventBits_t pingFlag;
    EventBits_t errorFlag;
    uint32_t pingPongTracker;
    uint32_t *pingPongBuffer[kTcdCount];

} pdm_mic_handle_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /*_cplusplus*/

/**
 * @brief Initialize SAI and DMA for microphone capture
 *
 * @return pdm_mic_status_t     0 if success
 */
pdm_mic_status_t PDM_MIC_Init(void);

/**
 * @brief Configure the microphone from the handle. SAI bitclock and DMA channel TCDs will be configured
 *
 * @param handle            Microphone handler
 * @return pdm_mic_status_t 0 if success
 */
pdm_mic_status_t PDM_MIC_ConfigMic(pdm_mic_handle_t *handle);

/**
 * @brief Begin capture for given microphone handle
 *
 * @param handle    Microphone handler
 */
void PDM_MIC_StartMic(pdm_mic_handle_t *handle);

/**
 * @brief Stop capture for given microphone handle
 *
 * @param handle    Microphone handler
 */
void PDM_MIC_StopMic(pdm_mic_handle_t *handle);

pdm_mic_status_t PDM_MIC_SplitMutlChStream();

/**
 * @brief DeInitialize SAI and DMA for microphone capture
 *
 * @return pdm_mic_status_t     0 if success
 */
pdm_mic_status_t PDM_MIC_DeInit(void);

/**
 * @brief DMA Callback to be called from DMA IRQ Handler upon completion of data transfer
 *
 * @param handle    Microphone handler
 */
void PDM_MIC_DmaCallback(pdm_mic_handle_t *handle);

#if 0
/*!
 * @brief Initialize SAI and DMA for microphone capture
 * @param numSaiUsed [in]:     Number of SAI modules used for microphone array.
 * @param *micHandleIDs [out]: Number of assigned microphone handles. Handles start at 0 and increase as needed.
 * @returns  Status of intialization.
 */
pdm_mic_status_t PDM_MIC_Init(uint32_t numSaiUsed, uint32_t *micHandleIDs);

/*!
 * @brief Set DMA channel number for mic handle.
 * @pre  DMAMUX and EDMA Init must be called beforehand. PDM_MIC_Init must be called.
 * @post DMAMUX and EDMA channels are configured for external requests. NVIC is set for EDMA IRQ for channel.
 * @param micHandleID [in]:   Numeric ID to use for setting DMA channel and events.
 * @param dmaChannelNum [in]: DMA Channel number to be used for this handle.
 * @returns  Status of setting channel operation.
 */
pdm_mic_status_t PDM_MIC_SetDmaChannel(uint32_t micHandleID, DMA_Type *dma, uint32_t dmaChannelNum, edma_tcd_t *dmaTcdChain);

/*!
 * @brief Set FreeRTOS event group and flags for mic handle.
 * @param micHandleID [in]:  Numeric ID to use for setting DMA channel and events.
 * @param *eventGroup [in]:  FreeRTOS event group that application will use for mic handle.
 * @param pingPongFlags [in]:  Event bits to be used for ping/pong events. Lowest two bits are chosen as ping and pong respectively.
 * @returns  Status of setting event group operation.
 */
pdm_mic_status_t PDM_MIC_SetEventGroup(uint32_t micHandleID, EventGroupHandle_t *eventGroup, EventBits_t pingPongFlags);

/*!
 * @brief Configure microphone for mic handle. SAI bitclock and DMA channel TCDs will be configured.
 * @pre  SAI Clocking must be configured. PDM_MIC_SetDmaChannel must be called.
 * @post SAI is configure for DMA transfer, and is ready to begin; DMA TCD is configured and isntalled.
 * @param micHandleID [in]:  Numeric ID to use for setting DMA channel and events.
 * @param *config [in]:  Configuration structure for microphone.
 * @returns  Status of setting microphone configuration operation.
 */
pdm_mic_status_t PDM_MIC_ConfigMic(uint32_t micHandleID, pdm_mic_config_t *config);

/*!
 * @brief Begin capture for given microphone handle.
 * @pre  PDM_MIC_ConfigMic and PDM_MIC_SetEventGroup must be called.
 * @post SAI will begin capturing bitstream and generating DMA requests.
 * @param micHandleID [in]:  Numeric ID to use for setting DMA channel and events.
 * @returns  Status of start microphone request.
 */
pdm_mic_status_t PDM_MIC_StartMic(uint32_t micHandleID);

/*!
 * @brief Stop capture for given microphone handle.
 * @pre  PDM_MIC_ConfigMic and PDM_MIC_SetEventGroup must be called.
 * @post SAI will stop capturing bitstream and generating DMA requests.
 * @param micHandleID [in]:  Numeric ID to use for setting DMA channel and events.
 * @returns  Status of stop microphone request.
 */
pdm_mic_status_t PDM_MIC_StopMic(uint32_t micHandleID);

/*!
 *  @brief DeInitialize SAI and DMA for microphone capture
 *  @returns  Status of microphone driver de-init.
 */
pdm_mic_status_t PDM_MIC_DeInit(void);

/*!
 *  @brief DMA Callback to be called from DMA IRQ Handler upon completion of data transfer.
 *  @param micHandleID [in]: Numeric ID to use for setting DMA channel and events.
 */
void PDM_MIC_DmaCallback(uint32_t micHandleID);
#endif
#if defined(__cplusplus)
}
#endif /*_cplusplus*/

#endif //__SLN_PDM_MIC_H__
