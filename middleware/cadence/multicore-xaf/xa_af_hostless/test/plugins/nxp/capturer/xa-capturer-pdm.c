/*
 * Copyright 2018 NXP
 * Copyright (c) 2015-2018 Cadence Design Systems, Inc.
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/*******************************************************************************
 * xa-capturer_pdm.c
 *
 * Hardware capturer implementation using PDM
 ******************************************************************************/
#ifdef CAPTURER_PDM

#define MODULE_TAG CAPTURER

/*******************************************************************************
 * Includes
 ******************************************************************************/
#ifdef HAVE_FREERTOS
#include "FreeRTOS.h"
#include "task.h"
#else
#include <xtensa/xos.h>
#endif

#include "audio/xa-capturer-api.h"
#include "xf-debug.h"

#include "fsl_pdm_edma.h"

/*******************************************************************************
 * Tracing configuration
 ******************************************************************************/
// TRACE_TAG(INIT, 1);
// TRACE_TAG(PROCESS, 1);
// TRACE_TAG(UNDERRUN, 1);

/*******************************************************************************
 * Hardware parameters
 ******************************************************************************/
#define DEMO_DMA DMA0
#define DEMO_PDM_EDMA_CHANNEL         (1U)
#define DEMO_PDM_ENABLE_CHANNEL_LEFT  (0U)
#define DEMO_PDM_EDMA_SOURCE          kDmaRequestMuxMicfil

#define BUFFER_LENGTH   256    // In samples
#define BUFFER_COUNT    2U
#define PDM_CH_COUNT   FSL_FEATURE_PDM_CHANNEL_NUM
//#define EDMAREQ_PDM_CH0 16U

#define PDM16KHZ 16000

#define REF_MIC0 0U
#define REF_MIC1 1U
#define REF_MIC2 2U
#define REF_MIC3 3U
#define REF_MIC4 4U
#define REF_MIC5 5U
#define REF_MIC6 6U
#define REF_MIC7 7U

typedef struct _circular_buf_t
{
    int32_t (*buffer)[BUFFER_LENGTH];
    uint8_t pingpong;
    bool full;
} circular_buf_t;

typedef struct XACapturer
{
    /***************************************************************************
     * Internal stuff
     **************************************************************************/

    /* ...component state */
    UWORD32 state;

    /* ...notification callback pointer */
    xa_capturer_cb_t *cdata;

    /* ...input buffer pointer */
    void *output;

    /* ...number of samples produced */
    UWORD32 produced;

    /***************************************************************************
     * Run-time data
     **************************************************************************/

    /* ...size of PCM sample (respects channels and PCM width) */
    UWORD32 sample_size;

    /* ...number of channels */
    UWORD32 channels;

    /* ...interleave */
    UWORD32 interleave;

    /* ...sample width */
    UWORD32 pcm_width;

    /* ...current sampling rate */
    UWORD32 rate;

    UWORD32 over_flow_flag;

    /* total bytes produced by the component*/
    UWORD64 tot_bytes_produced;

    /* total bytes to be produced*/
    UWORD64 bytes_end;

    circular_buf_t circular_buf_h;
    WORD32 newDataAvailable;

    /* ...PDM edma handle for 8 channel */
    pdm_edma_handle_t s_pdmEdmaHandle[FSL_FEATURE_PDM_CHANNEL_NUM];

#ifdef HAVE_FREERTOS
    TaskHandle_t irq_thread;
#else
    XosThread irq_thread;
    XosSem irq_sem;
    UWORD8 irq_stack[1024];
#endif

} XACapturer;

/*******************************************************************************
 * Operating flags
 ******************************************************************************/
#define XA_CAPTURER_FLAG_PREINIT_DONE  (1 << 0)
#define XA_CAPTURER_FLAG_POSTINIT_DONE (1 << 1)
#define XA_CAPTURER_FLAG_IDLE          (1 << 2)
#define XA_CAPTURER_FLAG_RUNNING       (1 << 3)
#define XA_CAPTURER_FLAG_PAUSED        (1 << 4)

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* Receive ping pong buffer and descriptor */
#if (XCHAL_DCACHE_SIZE > 0)
AT_NONCACHEABLE_SECTION_ALIGN(
#else
SDK_ALIGN(
#endif
    static int32_t g_rxBuffer[PDM_CH_COUNT * BUFFER_COUNT][BUFFER_LENGTH], 4);

AT_NONCACHEABLE_SECTION_ALIGN(edma_tcd_t s_edmaTcd[4], 32U);
extern edma_handle_t g_pdmDmaHandle;

//#if (XCHAL_DCACHE_SIZE > 0)
//AT_NONCACHEABLE_SECTION_ALIGN(
//#else
//SDK_ALIGN(
//#endif
//    static edma_descriptor_t s_edmaDescriptorPingpong[FSL_FEATURE_PDM_CHANNEL_NUM * 2], 16);

static pdm_edma_transfer_t s_receiveXfer[FSL_FEATURE_PDM_CHANNEL_NUM * 2] = {
    /* transfer configurations for channel0 */
    {
        .data                   = (uint8_t *)&g_rxBuffer[0][0],
        .dataSize               = FSL_FEATURE_PDM_FIFO_WIDTH * BUFFER_LENGTH,
        .linkTransfer           = &s_receiveXfer[1],
    },

    {
        .data                   = (uint8_t *)&g_rxBuffer[1][0],
        .dataSize               = FSL_FEATURE_PDM_FIFO_WIDTH * BUFFER_LENGTH,
        .linkTransfer           = &s_receiveXfer[0],
    },

    /* transfer configurations for channel1 */
    {
        .data                   = (uint8_t *)&g_rxBuffer[2][0],
        .dataSize               = FSL_FEATURE_PDM_FIFO_WIDTH * BUFFER_LENGTH,
        .linkTransfer           = &s_receiveXfer[3],
    },

    {
        .data                   = (uint8_t *)&g_rxBuffer[3][0],
        .dataSize               = FSL_FEATURE_PDM_FIFO_WIDTH * BUFFER_LENGTH,
        .linkTransfer           = &s_receiveXfer[2],
    },

    /* transfer configurations for channel2 */
    {
        .data                   = (uint8_t *)&g_rxBuffer[4][0],
        .dataSize               = FSL_FEATURE_PDM_FIFO_WIDTH * BUFFER_LENGTH,
        .linkTransfer           = &s_receiveXfer[5],
    },

    {
        .data                   = (uint8_t *)&g_rxBuffer[5][0],
        .dataSize               = FSL_FEATURE_PDM_FIFO_WIDTH * BUFFER_LENGTH,
        .linkTransfer           = &s_receiveXfer[4],
    },

    /* transfer configurations for channel3 */
    {
        .data                   = (uint8_t *)&g_rxBuffer[6][0],
        .dataSize               = FSL_FEATURE_PDM_FIFO_WIDTH * BUFFER_LENGTH,
        .linkTransfer           = &s_receiveXfer[7],
    },

    {
        .data                   = (uint8_t *)&g_rxBuffer[7][0],
        .dataSize               = FSL_FEATURE_PDM_FIFO_WIDTH * BUFFER_LENGTH,
        .linkTransfer           = &s_receiveXfer[6],
    },

    /* transfer configurations for channel4 */
    {
        .data                   = (uint8_t *)&g_rxBuffer[8][0],
        .dataSize               = FSL_FEATURE_PDM_FIFO_WIDTH * BUFFER_LENGTH,
        .linkTransfer           = &s_receiveXfer[9],
    },

    {
        .data                   = (uint8_t *)&g_rxBuffer[9][0],
        .dataSize               = FSL_FEATURE_PDM_FIFO_WIDTH * BUFFER_LENGTH,
        .linkTransfer           = &s_receiveXfer[8],
    },

    /* transfer configurations for channel5 */
    {
        .data                   = (uint8_t *)&g_rxBuffer[10][0],
        .dataSize               = FSL_FEATURE_PDM_FIFO_WIDTH * BUFFER_LENGTH,
        .linkTransfer           = &s_receiveXfer[11],
    },

    {
        .data                   = (uint8_t *)&g_rxBuffer[11][0],
        .dataSize               = FSL_FEATURE_PDM_FIFO_WIDTH * BUFFER_LENGTH,
        .linkTransfer           = &s_receiveXfer[10],
    },

    /* transfer configurations for channel6 */
    {
        .data                   = (uint8_t *)&g_rxBuffer[12][0],
        .dataSize               = FSL_FEATURE_PDM_FIFO_WIDTH * BUFFER_LENGTH,
        .linkTransfer           = &s_receiveXfer[13],
    },

    {
        .data                   = (uint8_t *)&g_rxBuffer[13][0],
        .dataSize               = FSL_FEATURE_PDM_FIFO_WIDTH * BUFFER_LENGTH,
        .linkTransfer           = &s_receiveXfer[12],
    },

    /* transfer configurations for channel7 */
    {
        .data                   = (uint8_t *)&g_rxBuffer[14][0],
        .dataSize               = FSL_FEATURE_PDM_FIFO_WIDTH * BUFFER_LENGTH,
        .linkTransfer           = &s_receiveXfer[15],
    },

    {
        .data                   = (uint8_t *)&g_rxBuffer[15][0],
        .dataSize               = FSL_FEATURE_PDM_FIFO_WIDTH * BUFFER_LENGTH,
        .linkTransfer           = &s_receiveXfer[14],
    },
};

static const pdm_config_t pdmConfig = {
#if defined(FSL_FEATURE_PDM_HAS_DECIMATION_FILTER_BYPASS) && FSL_FEATURE_PDM_HAS_DECIMATION_FILTER_BYPASS
    .enableFilterBypass = false,
#endif
    .enableDoze        = false,
    .fifoWatermark     = (FSL_FEATURE_PDM_FIFO_DEPTH / 2U),
    .qualityMode       = kPDM_QualityModeHigh,
    .cicOverSampleRate = (16U),
};
static const pdm_channel_config_t channelConfig = {
#if (defined(FSL_FEATURE_PDM_HAS_DC_OUT_CTRL) && (FSL_FEATURE_PDM_HAS_DC_OUT_CTRL))
    .outputCutOffFreq = kPDM_DcRemoverCutOff40Hz,
#else
    .cutOffFreq = kPDM_DcRemoverCutOff152Hz,
#endif
#ifdef DEMO_PDM_CHANNEL_GAIN
    .gain = DEMO_PDM_CHANNEL_GAIN,
#else
    .gain       = kPDM_DfOutputGain7,
#endif
};

/*******************************************************************************
 * Local functions
 ******************************************************************************/
#ifdef HAVE_FREERTOS
void PDM_CaptureCallback(void *arg)
#else
int PDM_CaptureCallback(void *arg, int wake_value)
#endif
{
    XACapturer *d = (XACapturer *)arg;
    int32_t err   = XOS_OK;

    while (1)
    {
#ifdef HAVE_FREERTOS
        xTaskNotifyWait(pdFALSE, 0xffffff, NULL, portMAX_DELAY);
#else
        err = xos_sem_get(&d->irq_sem);
        if (err == XOS_ERR_INVALID_PARAMETER)
        {
            return -1;
        }
#endif
        d->cdata->cb(d->cdata, 0);
    }
}

void PDM_CallbackISR(PDM_Type *base, pdm_edma_handle_t *handle, status_t status, void *userData)
{
    XACapturer *capturer = (XACapturer *)userData;
#ifdef HAVE_FREERTOS
    BaseType_t woken = pdFALSE;
#endif
    uint32_t i;

    if (status == kStatus_PDM_Idle)
    {
        for (i = 0; i < FSL_FEATURE_PDM_CHANNEL_NUM; i++)
        {
            capturer->newDataAvailable |= (1 << i);
        }

        if (capturer->over_flow_flag == 0)
        {
            capturer->over_flow_flag = 1;

#ifdef HAVE_FREERTOS
            xTaskNotifyFromISR(capturer->irq_thread, 0, eNoAction, &woken);
            portYIELD_FROM_ISR(woken);
#else
            xos_sem_put(&capturer->irq_sem);
#endif
        }
    }
}

/* Hardware-specific initialization
 * Start PDM EDMA transfers */
static void evk_pdm_edma_start(XACapturer *d)
{
    uint32_t i;

//    PDM_EnableChannnel(PDM0, PDM_CHANEN_EN_CH0_MASK | PDM_CHANEN_EN_CH1_MASK | PDM_CHANEN_EN_CH2_MASK |
//                                   PDM_CHANEN_EN_CH3_MASK | PDM_CHANEN_EN_CH4_MASK | PDM_CHANEN_EN_CH5_MASK |
//                                   PDM_CHANEN_EN_CH6_MASK | PDM_CHANEN_EN_CH7_MASK);
//
//    for (i = 0; i < FSL_FEATURE_PDM_CHANNEL_NUM; i++)
//    {
//        PDM_TransferReceiveEDMA(PDM0, &d->s_pdmEdmaHandle[i], &s_receiveXfer[i * 2], kPDM_Channel0 + i);
//    }

    PDM_TransferReceiveEDMA(PDM, &d->s_pdmEdmaHandle[0], &s_receiveXfer[0]);
}

/* Hardware-specific initialization
 * Setup PDM and EDMA */
static void evk_pdm_edma_config(void *ptr)
{
    uint32_t i;

    XACapturer *capturer = (XACapturer *)ptr;
    RESET_ClearPeripheralReset(kPDM_RST_SHIFT_RSTn);
    EDMA_EnableRequest(DEMO_DMA, DEMO_PDM_EDMA_SOURCE);

    EDMA_CreateHandle(&g_pdmDmaHandle, DEMO_DMA, DEMO_PDM_EDMA_CHANNEL);
#if defined(FSL_FEATURE_EDMA_HAS_CHANNEL_MUX) && FSL_FEATURE_EDMA_HAS_CHANNEL_MUX
    EDMA_SetChannelMux(DEMO_DMA, DEMO_PDM_EDMA_CHANNEL, DEMO_PDM_EDMA_SOURCE);
#endif

    PDM_Init(PDM, &pdmConfig);
    PDM_TransferCreateHandleEDMA(PDM, &capturer->s_pdmEdmaHandle[0], PDM_CallbackISR, capturer, &g_pdmDmaHandle);
    PDM_TransferInstallEDMATCDMemory(&capturer->s_pdmEdmaHandle[0], s_edmaTcd, 2);
    PDM_TransferSetChannelConfigEDMA(PDM, &capturer->s_pdmEdmaHandle[0], 0, &channelConfig);
    if (PDM_SetSampleRateConfig(PDM, CLOCK_GetMicfilClkFreq(), PDM16KHZ) != kStatus_Success)
    {
        return;
    }
    PDM_Reset(PDM);
}

static void evk_hw_capturer_init(void *ptr)
{
    XACapturer *d = (XACapturer *)ptr;

    d->circular_buf_h.buffer   = g_rxBuffer;
    d->circular_buf_h.pingpong = 0;
    d->circular_buf_h.full     = false;

#ifdef HAVE_FREERTOS
    xTaskCreate(PDM_CaptureCallback, "PDM_CaptureCallback", 1024, d, configMAX_PRIORITIES - 1, &d->irq_thread);
#else
    xos_sem_create(&d->irq_sem, 0, 0);
    xos_thread_create(&d->irq_thread, NULL, PDM_CaptureCallback, d, "PDM_CaptureCallback", d->irq_stack,
                      sizeof(d->irq_stack), XOS_MAX_PRIORITY - 1, 0, 0);
#endif

    evk_pdm_edma_config(ptr);
}

/*******************************************************************************
 * Codec access functions
 ******************************************************************************/
static inline void xa_hw_capturer_close(XACapturer *d)
{
    /* Disable EDMA channels when closing PDM device. */
     PDM_TransferAbortReceiveEDMA(PDM, &d->s_pdmEdmaHandle[0]);

#ifdef HAVE_FREERTOS
    vTaskDelete(d->irq_thread);
#else
    xos_sem_delete(&d->irq_sem);

    xos_thread_abort(&d->irq_thread, 0);
    xos_thread_join(&d->irq_thread, NULL);
    xos_thread_delete(&d->irq_thread);
#endif
}

/* ...submit data (in samples) into internal capturer ring-buffer */

/*******************************************************************************
 * API command hooks
 ******************************************************************************/
/* ...standard codec initialization routine */
static XA_ERRORCODE xa_capturer_get_api_size(XACapturer *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...check parameters are sane */
    XF_CHK_ERR(pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...retrieve API structure size */
    *(WORD32 *)pv_value = sizeof(*d);

    return XA_NO_ERROR;
}

static XA_ERRORCODE xa_hw_capturer_init(XACapturer *d)
{
    d->produced           = 0;
    d->tot_bytes_produced = 0;
    d->over_flow_flag     = 0;

    evk_hw_capturer_init(d);

    return XA_NO_ERROR;
}

/* ...standard codec initialization routine */
static XA_ERRORCODE xa_capturer_init(XACapturer *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...sanity check - pointer must be valid */
    XF_CHK_ERR(d, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...process particular initialization type */
    switch (i_idx)
    {
        case XA_CMD_TYPE_INIT_API_PRE_CONFIG_PARAMS:
        {
            /* ...pre-configuration initialization; reset internal data */
            memset(d, 0, sizeof(*d));

            /* ...set default capturer parameters - 16-bit stereo @ 16KHz */
            d->sample_size = 2;
            d->channels    = 2;
            d->pcm_width   = 16;
            d->interleave  = 1;
            d->rate        = 16000;

            /* ...and mark capturer has been created */
            d->state = XA_CAPTURER_FLAG_PREINIT_DONE;

            return XA_NO_ERROR;
        }

        case XA_CMD_TYPE_INIT_API_POST_CONFIG_PARAMS:
        {
            /* ...post-configuration initialization (all parameters are set) */

            XF_CHK_ERR(d->state & XA_CAPTURER_FLAG_PREINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

            XF_CHK_ERR(xa_hw_capturer_init(d) == 0, XA_CAPTURER_CONFIG_FATAL_HW);

            /* ...mark post-initialization is complete */
            d->state |= XA_CAPTURER_FLAG_POSTINIT_DONE;

            return XA_NO_ERROR;
        }

        case XA_CMD_TYPE_INIT_PROCESS:
        {
            /* ...kick run-time initialization process; make sure setup is complete */
            XF_CHK_ERR(d->state & XA_CAPTURER_FLAG_POSTINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);
            /* ...mark capturer is in idle state */
            d->state |= XA_CAPTURER_FLAG_IDLE;

            evk_pdm_edma_start(d);

            return XA_NO_ERROR;
        }

        case XA_CMD_TYPE_INIT_DONE_QUERY:
        {
            /* ...check if initialization is done; make sure pointer is sane */
            XF_CHK_ERR(pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

            /* ...put current status */
            *(WORD32 *)pv_value = (d->state & XA_CAPTURER_FLAG_IDLE ? 1 : 0);

            d->state ^= XA_CAPTURER_FLAG_IDLE | XA_CAPTURER_FLAG_RUNNING;

            return XA_NO_ERROR;
        }

        default:
            /* ...unrecognized command type */
            return XF_CHK_ERR(0, XA_API_FATAL_INVALID_CMD_TYPE);
    }
}

/* ...HW-capturer control function */
static inline XA_ERRORCODE xa_hw_capturer_control(XACapturer *d, UWORD32 state)
{
    switch (state)
    {
        case XA_CAPTURER_STATE_START:
            /* ...capturer must be in idle state */
            //XF_CHK_ERR(d->state & XA_CAPTURER_FLAG_IDLE, XA_CAPTURER_EXEC_NONFATAL_STATE);

            /* ...mark capturer is runnning */
            d->state ^= XA_CAPTURER_FLAG_IDLE | XA_CAPTURER_FLAG_RUNNING;

            return XA_NO_ERROR;
        case XA_CAPTURER_STATE_RUN:
            /* ...capturer must be in paused state */
            XF_CHK_ERR(d->state & XA_CAPTURER_FLAG_PAUSED, XA_CAPTURER_EXEC_NONFATAL_STATE);

            /* ...mark capturer is running */
            d->state ^= XA_CAPTURER_FLAG_RUNNING | XA_CAPTURER_FLAG_PAUSED;

            return XA_NO_ERROR;

        case XA_CAPTURER_STATE_PAUSE:
            /* ...capturer must be in running state */
            XF_CHK_ERR(d->state & XA_CAPTURER_FLAG_RUNNING, XA_CAPTURER_EXEC_NONFATAL_STATE);

            /* ...pause capturer operation */
            /* TODO: implement pause functionality */

            /* ...mark capturer is paused */
            d->state ^= XA_CAPTURER_FLAG_RUNNING | XA_CAPTURER_FLAG_PAUSED;

            return XA_NO_ERROR;

        case XA_CAPTURER_STATE_IDLE:
            /* ...command is valid in any active state; stop capturer operation */
            xa_hw_capturer_close(d);

            /* ...reset capturer flags */
            d->state &= ~(XA_CAPTURER_FLAG_RUNNING | XA_CAPTURER_FLAG_PAUSED);

            return XA_NO_ERROR;

        default:
            /* ...unrecognized command */
            return XF_CHK_ERR(0, XA_API_FATAL_INVALID_CMD_TYPE);
    }
}

/* ...set capturer configuration parameter */
static XA_ERRORCODE xa_capturer_set_config_param(XACapturer *d, WORD32 i_idx, pVOID pv_value)
{
    UWORD32 i_value;

    /* ...sanity check - pointers must be sane */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...pre-initialization must be completed */
    XF_CHK_ERR(d->state & XA_CAPTURER_FLAG_PREINIT_DONE, XA_CAPTURER_CONFIG_FATAL_STATE);

    /* ...process individual configuration parameter */
    switch (i_idx)
    {
        case XA_CAPTURER_CONFIG_PARAM_PCM_WIDTH:
            /* ...command is valid only in configuration state */
            XF_CHK_ERR((d->state & XA_CAPTURER_FLAG_POSTINIT_DONE) == 0, XA_CAPTURER_CONFIG_FATAL_STATE);

            /* ...get requested PCM width */
            i_value = (UWORD32) * (WORD32 *)pv_value;

            /* ...check value is permitted (16 bits only) */
            XF_CHK_ERR((i_value == 16 || i_value == 32), XA_CAPTURER_CONFIG_NONFATAL_RANGE);

            /* ...apply setting */
            d->pcm_width = i_value;

            return XA_NO_ERROR;

        case XA_CAPTURER_CONFIG_PARAM_CHANNELS:
            /* ...command is valid only in configuration state */
            XF_CHK_ERR((d->state & XA_CAPTURER_FLAG_POSTINIT_DONE) == 0, XA_CAPTURER_CONFIG_FATAL_STATE);

            /* ...get requested channel number */
            i_value = (UWORD32) * (WORD32 *)pv_value;

            /* ...allow up to 8 channels */
            XF_CHK_ERR((i_value >= 1 && i_value <= 8), XA_CAPTURER_CONFIG_NONFATAL_RANGE);

            /* ...apply setting */
            d->channels = (UWORD32)i_value;

            return XA_NO_ERROR;

        case XA_CAPTURER_CONFIG_PARAM_INTERLEAVE:
            /* ...command is valid only in configuration state */
            XF_CHK_ERR((d->state & XA_CAPTURER_FLAG_POSTINIT_DONE) == 0, XA_CAPTURER_CONFIG_FATAL_STATE);

            i_value = (UWORD32) * (WORD32 *)pv_value;

            XF_CHK_ERR((i_value >= 0 && i_value <= 1), XA_CAPTURER_CONFIG_NONFATAL_RANGE);

            /* ...apply setting */
            d->interleave = (UWORD32)i_value;

            return XA_NO_ERROR;

        case XA_CAPTURER_CONFIG_PARAM_SAMPLE_RATE:
            /* ...command is valid only in configuration state */
            XF_CHK_ERR((d->state & XA_CAPTURER_FLAG_POSTINIT_DONE) == 0, XA_CAPTURER_CONFIG_FATAL_STATE);

            /* ...get requested sampling rate */
            i_value = (UWORD32) * (WORD32 *)pv_value;

            /* ...allow 16 kHz only */
            XF_CHK_ERR(i_value == 16000, XA_CAPTURER_CONFIG_NONFATAL_RANGE);

            /* ...apply setting */
            d->rate = (UWORD32)i_value;

            return XA_NO_ERROR;

        case XA_CAPTURER_CONFIG_PARAM_FRAME_SIZE:
            /* ...command is valid only in configuration state */
            XF_CHK_ERR((d->state & XA_CAPTURER_FLAG_POSTINIT_DONE) == 0, XA_CAPTURER_CONFIG_FATAL_STATE);

            /* ...get requested frame size */
            i_value = (UWORD32) * (WORD32 *)pv_value;

            /* ...check it is equal to the only frame size we support */
            XF_CHK_ERR(i_value == FSL_FEATURE_PDM_FIFO_WIDTH * BUFFER_LENGTH, XA_CAPTURER_CONFIG_NONFATAL_RANGE);

            return XA_NO_ERROR;

        case XA_CAPTURER_CONFIG_PARAM_SAMPLE_END:
            /* ...command is valid only in configuration state */
            XF_CHK_ERR((d->state & XA_CAPTURER_FLAG_POSTINIT_DONE) == 0, XA_CAPTURER_CONFIG_FATAL_STATE);

            /* ...get requested frame size */
            d->bytes_end = ((UWORD64) * (UWORD64 *)pv_value) * (d->sample_size) * (d->channels);

            return XA_NO_ERROR;

        case XA_CAPTURER_CONFIG_PARAM_CB:
            /* ...set opaque callback data function */
            d->cdata = (xa_capturer_cb_t *)pv_value;

            return XA_NO_ERROR;

        case XA_CAPTURER_CONFIG_PARAM_STATE:
            /* ...runtime state control parameter valid only in execution state */
            XF_CHK_ERR(d->state & XA_CAPTURER_FLAG_POSTINIT_DONE, XA_CAPTURER_CONFIG_FATAL_STATE);

            /* ...get requested state */
            i_value = (UWORD32) * (WORD32 *)pv_value;

            /* ...pass to state control hook */
            return xa_hw_capturer_control(d, i_value);

        default:
            /* ...unrecognized parameter */
            return XF_CHK_ERR(0, XA_API_FATAL_INVALID_CMD_TYPE);
    }
}

/* ...state retrieval function */
static inline UWORD32 xa_hw_capturer_get_state(XACapturer *d)
{
    if (d->state & XA_CAPTURER_FLAG_RUNNING)
        return XA_CAPTURER_STATE_RUN;
    else if (d->state & XA_CAPTURER_FLAG_PAUSED)
        return XA_CAPTURER_STATE_PAUSE;
    else
        return XA_CAPTURER_STATE_IDLE;
}

/* ...retrieve configuration parameter */
static XA_ERRORCODE xa_capturer_get_config_param(XACapturer *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...sanity check - capturer must be initialized */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...make sure pre-initialization is completed */
    XF_CHK_ERR(d->state & XA_CAPTURER_FLAG_PREINIT_DONE, XA_CAPTURER_CONFIG_FATAL_STATE);

    /* ...process individual configuration parameter */
    switch (i_idx)
    {
        case XA_CAPTURER_CONFIG_PARAM_PCM_WIDTH:
            /* ...return current PCM width */
            *(WORD32 *)pv_value = d->pcm_width;
            return XA_NO_ERROR;

        case XA_CAPTURER_CONFIG_PARAM_CHANNELS:
            /* ...return current channel number */
            *(WORD32 *)pv_value = d->channels;
            return XA_NO_ERROR;

        case XA_CAPTURER_CONFIG_PARAM_INTERLEAVE:
            /* ...return current channel interleave status */
            *(WORD32 *)pv_value = d->interleave;
            return XA_NO_ERROR;

        case XA_CAPTURER_CONFIG_PARAM_SAMPLE_RATE:
            /* ...return current sampling rate */
            *(WORD32 *)pv_value = d->rate;
            return XA_NO_ERROR;

        case XA_CAPTURER_CONFIG_PARAM_FRAME_SIZE:
            /* ...return current audio frame length (in bytes) */
            *(WORD32 *)pv_value = FSL_FEATURE_PDM_FIFO_WIDTH * BUFFER_LENGTH;
            return XA_NO_ERROR;

        case XA_CAPTURER_CONFIG_PARAM_STATE:
            /* ...return current execution state */
            *(WORD32 *)pv_value = xa_hw_capturer_get_state(d);
            return XA_NO_ERROR;

        case XA_CAPTURER_CONFIG_PARAM_BYTES_PRODUCED:
            /* ...return current execution state */
            *(WORD32 *)pv_value = d->tot_bytes_produced;
            return XA_NO_ERROR;

        default:
            /* ...unrecognized parameter */
            return XF_CHK_ERR(0, XA_API_FATAL_INVALID_CMD_TYPE);
    }
}

/* ...execution command */
static XA_ERRORCODE xa_capturer_execute(XACapturer *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...sanity check - pointer must be valid */
    XF_CHK_ERR(d, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...capturer must be in running state */
    XF_CHK_ERR(d->state & (XA_CAPTURER_FLAG_RUNNING | XA_CAPTURER_FLAG_IDLE), XA_CAPTURER_EXEC_FATAL_STATE);

    /* ...process individual command type */
    switch (i_idx)
    {
        case XA_CMD_TYPE_DO_EXECUTE:
            /* ...silently ignore; everything is done in "set_input" */
            return XA_NO_ERROR;

        case XA_CMD_TYPE_DONE_QUERY:
            /* ...always report "no" - tbd - is that needed at all? */
            XF_CHK_ERR(pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

            if (d->produced == 0)
            {
                *(WORD32 *)pv_value = 1;
            }
            else
            {
                *(WORD32 *)pv_value = 0;
            }

            return XA_NO_ERROR;

        case XA_CMD_TYPE_DO_RUNTIME_INIT:
            /* ...silently ignore */
            return XA_NO_ERROR;

        default:
            /* ...unrecognized command */
            return XF_CHK_ERR(0, XA_API_FATAL_INVALID_CMD_TYPE);
    }
}

/* ...get number of produced bytes */
static XA_ERRORCODE xa_capturer_get_output_bytes(XACapturer *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...sanity check - check parameters */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...input buffer index must be valid */
    XF_CHK_ERR(i_idx == 0, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...capturer must be in post-init state */
    XF_CHK_ERR(d->state & XA_CAPTURER_FLAG_POSTINIT_DONE, XA_CAPTURER_EXEC_FATAL_STATE);

    /* ...input buffer must exist */
    XF_CHK_ERR(d->output, XA_CAPTURER_EXEC_FATAL_INPUT);

    /* ...return number of bytes produced */
    *(WORD32 *)pv_value = d->produced;

    d->produced = 0;

    return XA_NO_ERROR;
}

/*******************************************************************************
 * Memory information API
 ******************************************************************************/

/* ..get total amount of data for memory tables */
static XA_ERRORCODE xa_capturer_get_memtabs_size(XACapturer *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...basic sanity checks */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...check capturer is pre-initialized */
    XF_CHK_ERR(d->state & XA_CAPTURER_FLAG_PREINIT_DONE, XA_CAPTURER_CONFIG_FATAL_STATE);

    /* ...we have all our tables inside API structure */
    *(WORD32 *)pv_value = 0;

    return XA_NO_ERROR;
}

/* ..set memory tables pointer */
static XA_ERRORCODE xa_capturer_set_memtabs_ptr(XACapturer *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...basic sanity checks */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...check capturer is pre-initialized */
    XF_CHK_ERR(d->state & XA_CAPTURER_FLAG_PREINIT_DONE, XA_CAPTURER_CONFIG_FATAL_STATE);

    /* ...do not do anything; just return success - tbd */
    return XA_NO_ERROR;
}

/* ...return total amount of memory buffers */
static XA_ERRORCODE xa_capturer_get_n_memtabs(XACapturer *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...basic sanity checks */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...we have 1 input buffer only */
    *(WORD32 *)pv_value = 1;

    return XA_NO_ERROR;
}

/* ...return memory buffer data */
static XA_ERRORCODE xa_capturer_get_mem_info_size(XACapturer *d, WORD32 i_idx, pVOID pv_value)
{
    UWORD32 i_value;

    /* ...basic sanity check */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...command valid only after post-initialization step */
    XF_CHK_ERR(d->state & XA_CAPTURER_FLAG_POSTINIT_DONE, XA_CAPTURER_CONFIG_FATAL_STATE);

    switch (i_idx)
    {
        case 0:
            /* ...input buffer specification; accept exact audio frame */
            i_value = FSL_FEATURE_PDM_FIFO_WIDTH * BUFFER_LENGTH * d->channels;
            break;

        default:
            /* ...invalid index */
            return XF_CHK_ERR(0, XA_API_FATAL_INVALID_CMD_TYPE);
    }

    /* ...return buffer size to caller */
    *(WORD32 *)pv_value = (WORD32)i_value;

    return XA_NO_ERROR;
}

/* ...return memory alignment data */
static XA_ERRORCODE xa_capturer_get_mem_info_alignment(XACapturer *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...basic sanity check */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...command valid only after post-initialization step */
    XF_CHK_ERR(d->state & XA_CAPTURER_FLAG_POSTINIT_DONE, XA_CAPTURER_CONFIG_FATAL_STATE);

    /* ...all buffers are at least 4-bytes aligned */
    *(WORD32 *)pv_value = 4;

    return XA_NO_ERROR;
}

/* ...return memory type data */
static XA_ERRORCODE xa_capturer_get_mem_info_type(XACapturer *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...basic sanity check */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...command valid only after post-initialization step */
    XF_CHK_ERR(d->state & XA_CAPTURER_FLAG_POSTINIT_DONE, XA_CAPTURER_CONFIG_FATAL_STATE);

    switch (i_idx)
    {
        case 0:
            /* ...input buffers */
            *(WORD32 *)pv_value = XA_MEMTYPE_OUTPUT;
            return XA_NO_ERROR;

        default:
            /* ...invalid index */
            return XF_CHK_ERR(0, XA_API_FATAL_INVALID_CMD_TYPE);
    }
}

static inline UWORD32 xa_hw_capturer_read_FIFO(XACapturer *d)
{
    WORD32 *out_buffer_ptr = NULL;
    UWORD32 i;

    d->over_flow_flag = 0;
    out_buffer_ptr    = d->output;

	if (d->interleave == 1)
	{
		for (i = 0; i < BUFFER_LENGTH; i++)
		{
			for (int mic = 0; mic < d->channels; mic++)
			{
				*out_buffer_ptr =
					d->circular_buf_h.buffer[mic * 2 + d->circular_buf_h.pingpong][i];
				out_buffer_ptr++;
			}
		}
	}
	else
	{
		for (int mic = 0; mic < d->channels; mic++)
		{
			for (i = 0; i < BUFFER_LENGTH; i++)
			{
				*out_buffer_ptr =
					d->circular_buf_h.buffer[mic * 2 + d->circular_buf_h.pingpong][i];
				out_buffer_ptr++;
			}
		}
	}

	d->circular_buf_h.full = false;
	d->circular_buf_h.pingpong ^= 1;

	return (FSL_FEATURE_PDM_FIFO_WIDTH * BUFFER_LENGTH * d->channels);


    /* Should not get here - error in number of PDM channels. */
    return 0;
}

/* ...set memory pointer */
static XA_ERRORCODE xa_capturer_set_mem_ptr(XACapturer *d, WORD32 i_idx, pVOID pv_value)
{
    WORD32 bytes_read = 0;
    /* ...basic sanity check */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...codec must be initialized */
    XF_CHK_ERR(d->state & XA_CAPTURER_FLAG_POSTINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...select memory buffer */
    switch (i_idx)
    {
        case 0:
            /* ...input buffer */
            d->output = pv_value;

            if ((d->newDataAvailable & (0xFF << 0)) == (0xFF << 0))
            {
                bytes_read = xa_hw_capturer_read_FIFO(d);

                d->produced           = bytes_read;
                d->tot_bytes_produced = d->tot_bytes_produced + d->produced;

                d->newDataAvailable &= ~(0xFF << 0);
            }

            return XA_NO_ERROR;

        default:
            /* ...invalid index */
            return XF_CHK_ERR(0, XA_API_FATAL_INVALID_CMD_TYPE);
    }
}

/*******************************************************************************
 * API command hooks
 ******************************************************************************/
static XA_ERRORCODE (*const xa_capturer_api[])(XACapturer *, WORD32, pVOID) = {
    [XA_API_CMD_GET_API_SIZE]           = xa_capturer_get_api_size,
    [XA_API_CMD_INIT]                   = xa_capturer_init,
    [XA_API_CMD_SET_CONFIG_PARAM]       = xa_capturer_set_config_param,
    [XA_API_CMD_GET_CONFIG_PARAM]       = xa_capturer_get_config_param,
    [XA_API_CMD_EXECUTE]                = xa_capturer_execute,
    [XA_API_CMD_GET_OUTPUT_BYTES]       = xa_capturer_get_output_bytes,
    [XA_API_CMD_GET_MEMTABS_SIZE]       = xa_capturer_get_memtabs_size,
    [XA_API_CMD_SET_MEMTABS_PTR]        = xa_capturer_set_memtabs_ptr,
    [XA_API_CMD_GET_N_MEMTABS]          = xa_capturer_get_n_memtabs,
    [XA_API_CMD_GET_MEM_INFO_SIZE]      = xa_capturer_get_mem_info_size,
    [XA_API_CMD_GET_MEM_INFO_ALIGNMENT] = xa_capturer_get_mem_info_alignment,
    [XA_API_CMD_GET_MEM_INFO_TYPE]      = xa_capturer_get_mem_info_type,
    [XA_API_CMD_SET_MEM_PTR]            = xa_capturer_set_mem_ptr,

};

/* ...total numer of commands supported */
#define XA_CAPTURER_API_COMMANDS_NUM (sizeof(xa_capturer_api) / sizeof(xa_capturer_api[0]))

/*******************************************************************************
 * API entry point
 ******************************************************************************/

XA_ERRORCODE xa_capturer(xa_codec_handle_t p_xa_module_obj, WORD32 i_cmd, WORD32 i_idx, pVOID pv_value)
{
    XACapturer *capturer = (XACapturer *)p_xa_module_obj;

    /* ...check if command index is sane */
    XF_CHK_ERR(i_cmd < XA_CAPTURER_API_COMMANDS_NUM, XA_API_FATAL_INVALID_CMD);

    /* ...see if command is defined */
    XF_CHK_ERR(xa_capturer_api[i_cmd], XA_API_FATAL_INVALID_CMD);

    /* ...execute requested command */
    return xa_capturer_api[i_cmd](capturer, i_idx, pv_value);
}
#endif //CAPTURER_PDM
