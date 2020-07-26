/*
 * Copyright 2018 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#include "sln_pdm_mic.h"
#include <limits.h>
#include "fsl_dmamux.h"
#include "fsl_sai.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#ifdef FSL_RTOS_FREE_RTOS
#define PDM_MIC_MALLOC(x) pvPortMalloc(x)
#define PDM_MIC_FREE(x) vPortFree(x)
#define PDM_MIC_DMA_IRQ_PRIO (configMAX_SYSCALL_INTERRUPT_PRIORITY - 1)
#else
#define PDM_MIC_MALLOC(x) malloc(x)
#define PDM_MIC_FREE(x) free(x)
#define PDM_MIC_DMA_IRQ_PRIO (3U)
#endif

/* Select Audio/Video PLL (786.48 MHz) as SAI clock source */
#define PDM_SAI_CLOCK_SOURCE_SELECT (2U)
/* Clock pre divider for SAI clock source */
#define PDM_SAI_CLOCK_SOURCE_PRE_DIVIDER (0U) //(1U)
/* Clock divider for SAI clock source */
#define PDM_SAI_CLOCK_SOURCE_DIVIDER (63U)
/* Get frequency of SAI clock */
#define PDM_SAI_CLK_FREQ \
    (CLOCK_GetFreq(kCLOCK_AudioPllClk) / (PDM_SAI_CLOCK_SOURCE_DIVIDER + 1U) / (PDM_SAI_CLOCK_SOURCE_PRE_DIVIDER + 1U))

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

void PDM_MIC_DmaCallback(pdm_mic_handle_t *handle)
{
    handle->dma->INT |= (1 << handle->dmaChannel);

    handle->dma->TCD[handle->dmaChannel].CSR &= ~DMA_CSR_DONE_MASK;

    handle->dma->TCD[handle->dmaChannel].CSR |= DMA_CSR_ESG_MASK;

    handle->dma->SERQ = DMA_SERQ_SERQ(handle->dmaChannel);

    BaseType_t xHigherPriorityTaskWoken;

    xHigherPriorityTaskWoken = pdFALSE;

    if (handle->pingPongTracker & 0x01U)
    {
        if (xEventGroupGetBitsFromISR(handle->eventGroup) & handle->pongFlag)
        {
            xEventGroupSetBitsFromISR((handle->eventGroup), handle->errorFlag, &xHigherPriorityTaskWoken);
        }
        xEventGroupSetBitsFromISR((handle->eventGroup), handle->pongFlag, &xHigherPriorityTaskWoken);
    }
    else
    {
        if (xEventGroupGetBitsFromISR(handle->eventGroup) & handle->pingFlag)
        {
            xEventGroupSetBitsFromISR((handle->eventGroup), handle->errorFlag, &xHigherPriorityTaskWoken);
        }
        xEventGroupSetBitsFromISR((handle->eventGroup), handle->pingFlag, &xHigherPriorityTaskWoken);
    }

    handle->pingPongTracker++;
}

pdm_mic_status_t PDM_MIC_GetSAIConfiguration(uint8_t channelMask,
                                             uint32_t *burstSize,
                                             uint32_t *burstBytes,
                                             uint32_t *startIdx)
{
    // We will assign start index, burt size enum and bust byte count for each channel mapping.
    switch (channelMask)
    {
        // Determine all single channel mapping start index
        case kMicChannel1:
            *startIdx = 0;
            goto set_4byte_burst;
        case kMicChannel2:
            *startIdx = 1;
            goto set_4byte_burst;
        case kMicChannel3:
            *startIdx = 2;
            goto set_4byte_burst;
        case kMicChannel4:
            *startIdx = 3;

        // All byte channel mapping configurations will have these burstSize and burstBytes settings
        set_4byte_burst:
            *burstSize  = kEDMA_TransferSize4Bytes;
            *burstBytes = 4U;
            break;

        // Determine two channel mapping start index
        case kMicChannel1 | kMicChannel2:
            *startIdx = 0;
            goto set_8byte_burst;
        case kMicChannel2 | kMicChannel3:
            *startIdx = 1;
            goto set_8byte_burst;
        case kMicChannel3 | kMicChannel4:
            *startIdx = 2;

        // All two channel mapping configurations will have these burstSize and burstBytes settings
        set_8byte_burst:
            *burstSize = kEDMA_TransferSize8Bytes;
            // burstSize = kEDMA_TransferSize4Bytes;
            *burstBytes = 8U;
            break;

        // Any 3 or 4 channel mic maps
        default:
            *startIdx  = 0;
            *burstSize = kEDMA_TransferSize16Bytes;
            // burstSize = kEDMA_TransferSize4Bytes;
            *burstBytes = 16U;
            break;
    };

    return kPdmMicSuccess;
}

pdm_mic_status_t PDM_MIC_Init(void)
{
    // Coming Soon to A MCU Voice board near you!

    // TODO: Implement driver private memory, set to known values

    return kPdmMicSuccess;
}

pdm_mic_status_t PDM_MIC_ConfigMic(pdm_mic_handle_t *handle)
{
    // Map channel mask to DMA transfer config
    uint32_t burstSize  = 0U;
    uint32_t startIndex = 0U;
    uint32_t burstBytes = 0U;

    /* Zero ping pong buffers */
    uint32_t bufferLen = handle->config->pdmCaptureSize * handle->config->pdmCaptureCount;
    memset(handle->pingPongBuffer[0], 0, bufferLen);
    memset(handle->pingPongBuffer[1], 0, bufferLen);

    if (handle->config->sai == SAI1)
    {
        CLOCK_SetMux(kCLOCK_Sai1Mux, PDM_SAI_CLOCK_SOURCE_SELECT);
        CLOCK_SetDiv(kCLOCK_Sai1PreDiv, PDM_SAI_CLOCK_SOURCE_PRE_DIVIDER);
        CLOCK_SetDiv(kCLOCK_Sai1Div, PDM_SAI_CLOCK_SOURCE_DIVIDER);
        CLOCK_EnableClock(kCLOCK_Sai1);
    }
    else if (handle->config->sai == SAI2)
    {
        CLOCK_SetMux(kCLOCK_Sai2Mux, PDM_SAI_CLOCK_SOURCE_SELECT);
        CLOCK_SetDiv(kCLOCK_Sai2PreDiv, PDM_SAI_CLOCK_SOURCE_PRE_DIVIDER);
        CLOCK_SetDiv(kCLOCK_Sai2Div, PDM_SAI_CLOCK_SOURCE_DIVIDER);
        CLOCK_EnableClock(kCLOCK_Sai2);
    }
    else if (handle->config->sai == SAI3)
    {
        CLOCK_SetMux(kCLOCK_Sai3Mux, PDM_SAI_CLOCK_SOURCE_SELECT);
        CLOCK_SetDiv(kCLOCK_Sai3PreDiv, PDM_SAI_CLOCK_SOURCE_PRE_DIVIDER);
        CLOCK_SetDiv(kCLOCK_Sai3Div, PDM_SAI_CLOCK_SOURCE_DIVIDER);
        CLOCK_EnableClock(kCLOCK_Sai3);
    }

    PDM_MIC_GetSAIConfiguration(handle->config->saiChannelMask, &burstSize, &burstBytes, &startIndex);

    handle->dmaTcd[0].SADDR = (uint32_t)(&handle->config->sai->RDR[startIndex]);
    handle->dmaTcd[1].SADDR = (uint32_t)(&handle->config->sai->RDR[startIndex]);

    handle->dmaTcd[0].SOFF = 0U;
    handle->dmaTcd[1].SOFF = 0U;

    handle->dmaTcd[0].ATTR = (DMA_ATTR_SSIZE(burstSize) | DMA_ATTR_DSIZE(burstSize));
    handle->dmaTcd[1].ATTR = (DMA_ATTR_SSIZE(burstSize) | DMA_ATTR_DSIZE(burstSize));

    handle->dmaTcd[0].NBYTES = burstBytes;
    handle->dmaTcd[1].NBYTES = burstBytes;

    handle->dmaTcd[0].SLAST = 0U;
    handle->dmaTcd[1].SLAST = 0U;

    handle->dmaTcd[0].DADDR = (uint32_t)handle->pingPongBuffer[0];
    handle->dmaTcd[1].DADDR = (uint32_t)handle->pingPongBuffer[1];

    handle->dmaTcd[0].DOFF = burstBytes;
    handle->dmaTcd[1].DOFF = burstBytes;

    handle->dmaTcd[0].CITER = handle->config->pdmCaptureCount;
    handle->dmaTcd[1].CITER = handle->config->pdmCaptureCount;

    handle->dmaTcd[0].DLAST_SGA = (uint32_t)&handle->dmaTcd[1];
    handle->dmaTcd[1].DLAST_SGA = (uint32_t)&handle->dmaTcd[0];

    handle->dmaTcd[0].CSR = (DMA_CSR_INTMAJOR_MASK | DMA_CSR_ESG_MASK);
    handle->dmaTcd[1].CSR = (DMA_CSR_INTMAJOR_MASK | DMA_CSR_ESG_MASK);

    handle->dmaTcd[0].BITER = handle->config->pdmCaptureCount;
    handle->dmaTcd[1].BITER = handle->config->pdmCaptureCount;

    EDMA_InstallTCD(handle->dma, handle->dmaChannel, &handle->dmaTcd[0]);

    DMAMUX_SetSource(DMAMUX, handle->dmaChannel, handle->dmaRequest);
    DMAMUX_EnableChannel(DMAMUX, handle->dmaChannel);

    NVIC_SetPriority(handle->dmaIrqNum, PDM_MIC_DMA_IRQ_PRIO);
    NVIC_EnableIRQ(handle->dmaIrqNum);

    uint32_t val               = 0;
    uint32_t bclk              = 0;
    uint32_t mclkSourceClockHz = 0U;
    mclkSourceClockHz          = PDM_SAI_CLK_FREQ;
    bclk                       = handle->config->sampleRateHz;

    handle->config->sai->RCR2 |= I2S_RCR2_BCP_MASK;
    handle->config->sai->RCR3 &= ~I2S_RCR3_WDFL_MASK;
    handle->config->sai->RCR4 =
        I2S_RCR4_MF(0U) | I2S_RCR4_SYWD(31U) | I2S_RCR4_FSE(0U) | I2S_RCR4_FSP(0U) | I2S_RCR4_FRSZ(1U);

    handle->config->sai->RCR2 |= I2S_RCR2_BCD_MASK;
    handle->config->sai->RCR4 |= I2S_RCR4_FSD_MASK;

    val                       = handle->config->sai->RCR2 & (~I2S_RCR2_MSEL_MASK);
    handle->config->sai->RCR2 = (val | I2S_RCR2_MSEL(kSAI_BclkSourceMclkDiv));
    val                       = 0;

    val = handle->config->sai->RCR2;
    val &= ~I2S_RCR2_SYNC_MASK;
    handle->config->sai->RCR2 = (val | I2S_RCR2_SYNC(0U));
    val                       = 0;

    handle->config->sai->RCR2 &= ~I2S_RCR2_BCP_MASK; // 0, sample on falling

    val = 0;
    val = (handle->config->sai->RCR4 & (~I2S_RCR4_SYWD_MASK));
    val |= I2S_RCR4_SYWD(kSAI_WordWidth32bits - 1U);
    handle->config->sai->RCR4 = val;

    handle->config->sai->RCR2 &= ~I2S_RCR2_DIV_MASK;
    handle->config->sai->RCR2 |= I2S_RCR2_DIV((mclkSourceClockHz / bclk) / 2U - 1U);

    handle->config->sai->RCR5 = I2S_RCR5_WNW(31U) | I2S_RCR5_W0W(31U) | I2S_RCR5_FBT(0U);

    handle->config->sai->RMR = 0U;

    handle->config->sai->RCR3 &= ~I2S_RCR3_RCE_MASK;
    handle->config->sai->RCR3 |= I2S_RCR3_RCE(handle->config->saiChannelMask);

    handle->config->sai->RCR1 = FSL_FEATURE_SAI_FIFO_COUNT / 2U;

    handle->dma->EEI |= (1 << handle->dmaChannel);
    handle->dma->SERQ = DMA_SERQ_SERQ(handle->dmaChannel);

#ifndef NDEBUG
    handle->config->sai->RCSR = ((handle->config->sai->RCSR & 0xFFE3FFFFU) | I2S_RCSR_DBGE_MASK);
#endif

    handle->config->sai->RCSR = ((handle->config->sai->RCSR & 0xFFE3FFFFU) | kSAI_FIFORequestDMAEnable);

    return kPdmMicSuccess;
}

void PDM_MIC_StartMic(pdm_mic_handle_t *handle)
{
    handle->config->sai->RCSR = ((handle->config->sai->RCSR & 0xFFE3FFFFU) | I2S_RCSR_RE_MASK);
    handle->config->sai->RCSR = ((handle->config->sai->RCSR & 0xFFE3FFFFU) | kSAI_FIFOErrorFlag);
}

void PDM_MIC_StopMic(pdm_mic_handle_t *handle)
{
    handle->config->sai->RCSR = ((handle->config->sai->RCSR & 0xFFE3FFFFU) | 0x0);
    handle->config->sai->RCSR = ((handle->config->sai->RCSR & 0xFFE3FFFFU) | kSAI_FIFOErrorFlag);
}
