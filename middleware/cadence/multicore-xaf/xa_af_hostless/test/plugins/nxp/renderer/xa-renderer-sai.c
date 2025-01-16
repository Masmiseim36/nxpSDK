/*
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
/*
 * Copyright 2024 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * xa-renderer.c
 *
 * Hardware renderer implementation using SAI
 ******************************************************************************/
#ifdef RENDERER_SAI

#define MODULE_TAG                      RENDERER

/*******************************************************************************
 * Includes
 ******************************************************************************/
#ifdef HAVE_FREERTOS
#include "FreeRTOS.h"
#include "task.h"
#else
#include <xtensa/xos.h>
#endif

#include "audio/xa-renderer-api.h"
#include "xf-debug.h"

#include "fsl_sai_edma.h"

/*******************************************************************************
 * Tracing configuration
 ******************************************************************************/
//TRACE_TAG(INIT, 1);
//TRACE_TAG(PROCESS, 1);
//TRACE_TAG(UNDERRUN, 1);

/*******************************************************************************
 * Hardware parameters
 ******************************************************************************/
/* Fixed DMA interface */
#ifdef DMA0
#define DMA_RENDERER DMA0
#else
#define DMA_RENDERER DMA2
#endif
/* Maximum number of renderer components that can exist simultaneously */
#define MAX_RENDERERS 2
/* Maximum number of bytes that can be transferred in one DMA request */
#define MAX_DMA_TRANSFER_SIZE 4096
/* Maximum number of DMA transfers pre PCM frame */
#define MAX_DMA_TRANSFER_PER_FRAME 3
/* Maxmimum configurable frame size, based on DMA limits */
#define MAX_FRAME_SIZE (MAX_DMA_TRANSFER_SIZE * MAX_DMA_TRANSFER_PER_FRAME)


typedef struct XARenderer
{
    /***************************************************************************
     * Internal stuff
     **************************************************************************/

    /* ...component state */
    UWORD32                     state;

    /* ...notification callback pointer */
    xa_renderer_cb_t           *cdata;

    /* ...input buffer pointer */
    void                       *input;

    /* ...output buffer pointer */
    void                       *output;

    /* ...pointer to DMA FIFO buffer */
    UWORD8                     *fifo_head[2];

    /* ...number of bytes consumed */
    UWORD32                     consumed;

    /* ...number of bytes copied in fifo*/
    UWORD32                     submited_inbytes;

    /* ...total number of frames submitted */
    UWORD32                     submitted;

    /* ...total number of frames rendered */
    UWORD32                     rendered;

    /***************************************************************************
     * Run-time data
     **************************************************************************/

    /* ...size of one frame, in bytes */
    UWORD32                     frame_size;

    /* ...size of one frame, in bytes for HW codec*/
    UWORD32                     codec_frame_size;

    /* ...number of channels in application */
    UWORD32                     channels;

    /* ...number of channels for HW codec */
    UWORD32                     codec_channels;

    /* ...sample width in application */
    UWORD32                     pcm_width;

    /* ...sample width for HW codec */
    UWORD32                     codec_pcm_width;

    /* ...current sampling rate */
    UWORD32                     rate;

    /* ...flag for detecting underrun..made to non zero over submit */
    UWORD32                     submit_flag;

    /* bytes produced*/
    UWORD64                     bytes_produced;

    /* ...cumulative output bytes produced*/
    UWORD64             cumulative_bytes_produced;

    /* DMA handle */
    edma_handle_t                saiTxDmaHandle;
    edma_tcd_t                  *edmaDescriptor;

    /* SAI configuration */
    UWORD32                     sai_device;
    UWORD32                     sai_master;
    UWORD32                     sai_mode;
    UWORD32                     sai_sck_polarity;
    UWORD32                     sai_ws_polarity;
    UWORD32                     position;

    UWORD32                     renderIndex;

    /* ...input over flag */
    UWORD32             input_over;

    /* ...execution complete flag */
    UWORD32     exec_done;

#ifdef HAVE_FREERTOS
    TaskHandle_t                irq_thread;
#else
    XosThread                   irq_thread;
    XosSem                      irq_sem;
    UWORD8                      irq_stack[1024];
#endif

} XARenderer;

/*******************************************************************************
 * Operating flags
 ******************************************************************************/
#define XA_RENDERER_FLAG_PREINIT_DONE   (1 << 0)
#define XA_RENDERER_FLAG_POSTINIT_DONE  (1 << 1)
#define XA_RENDERER_FLAG_IDLE           (1 << 2)
#define XA_RENDERER_FLAG_RUNNING        (1 << 3)
#define XA_RENDERER_FLAG_PAUSED         (1 << 4)

#define RENDERER_SUBMIT_NULL            (1 << 0)
#define RENDERER_SUBMIT_INIT            (1 << 1)
#define RENDERER_SUBMIT_RUNNING         (1 << 2)

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* DMA descriptors for data tranfers */
#if (XCHAL_DCACHE_SIZE > 0)
AT_NONCACHEABLE_SECTION_ALIGN(
#else
SDK_ALIGN(
#endif
    static edma_tcd_t s_edmaDescriptorPingpongSAI[MAX_DMA_TRANSFER_PER_FRAME * 2 * MAX_RENDERERS], 16
);

sai_edma_handle_t saiTxHandle;

/* Track which static renderers are being used */
static uint8_t g_renderIndexMap[MAX_RENDERERS];

/* Index -> hardware device maps */
#ifdef SAI0
static I2S_Type *sai_device_map[] = { SAI0, SAI1, SAI2 };
static const uint8_t dma_channel_map[] = { 0, 2, 4 };
#define DMA_REQUEST kDmaRequestMuxSai0Tx
#else
static I2S_Type *sai_device_map[] = { SAI3 };
static const uint8_t dma_channel_map[] = { 0 };
#define DMA_REQUEST kDmaRequestMuxSai3Tx
#endif


extern edma_handle_t g_saiDmaHandle;


/*******************************************************************************
 * Local functions
 ******************************************************************************/

/* Taking requested frame size, calculate the number of DMA linked descriptors
 * needed to execute a frame transfer of this size.
 *
 * Return 0 if the frame size cannot be evenly divided.
 */
static uint32_t dma_calc_num_descriptors(uint32_t frame_size)
{
    uint32_t num_descriptors;

    if (frame_size <= MAX_DMA_TRANSFER_SIZE)
    {
        num_descriptors = 1;
    }
    else
    {
        /* Need to setup multiple DMA descriptors for each frame. */
        num_descriptors = (frame_size / MAX_DMA_TRANSFER_SIZE);
        if (frame_size % MAX_DMA_TRANSFER_SIZE)
        {
            num_descriptors += 1;
        }

        if (frame_size % num_descriptors)
        {
            /* Invalid frame size - not even divisible into descriptors. */
            num_descriptors = 0;
        }
    }

    return num_descriptors;
}

/* ...start renderer operation */
static void xa_hw_renderer_start(XARenderer *d)
{
	sai_transfer_t xfer[2U] = {
			{
					.data = d->fifo_head[0],
					.dataSize = d->codec_frame_size
			},
			{
					.data = d->fifo_head[1],
					.dataSize = d->codec_frame_size
			}
	};
    TRACE(INIT, _b("xa_hw_renderer_start"));

    SAI_TransferSendLoopEDMA(sai_device_map[d->sai_device], &saiTxHandle, &xfer[0], 2U);
}

/* ...stop hardware renderer */
static void xa_hw_renderer_stop(XARenderer *d)
{
	SAI_TransferTerminateSendEDMA(sai_device_map[d->sai_device], &saiTxHandle);
}

/* ...close hardware renderer */
static void xa_hw_renderer_close(XARenderer *d)
{
    xa_hw_renderer_stop(d);

    g_renderIndexMap[d->renderIndex] = 0;

#ifdef HAVE_FREERTOS
    vTaskDelete(d->irq_thread);
#else
    xos_sem_delete(&d->irq_sem);

    xos_thread_abort(&d->irq_thread, 0);
    xos_thread_join(&d->irq_thread, NULL);
    xos_thread_delete(&d->irq_thread);
#endif
}

/* ...pause renderer operation */
static inline void xa_hw_renderer_pause(XARenderer *d)
{
    TRACE(INIT, _b("xa_hw_renderer_pause"));

    EDMA_StopTransfer(&g_saiDmaHandle);
}

/* ...resume renderer operation */
static inline void xa_hw_renderer_resume(XARenderer *d)
{
    TRACE(INIT, _b("xa_hw_renderer_resume"));

    EDMA_StartTransfer(&g_saiDmaHandle);
}

#ifdef HAVE_FREERTOS
void TxRenderCallback(void *arg)
#else
int TxRenderCallback(void *arg, int wake_value)
#endif
{
    XARenderer *d = (XARenderer*) arg;
    int32_t err = XOS_OK;

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

        /* ...zero out buffer just completed */
        memset(d->fifo_head[(d->rendered - 1) % 2], 0, d->codec_frame_size);

        d->cdata->cb(d->cdata, 0);
    }
}

/* DMA transfer callback. */
static void TxRenderCallbackISR(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData)
{
    XARenderer *d = (XARenderer*) userData;
#ifdef HAVE_FREERTOS
    BaseType_t woken = pdFALSE;
#endif

	if (d->exec_done)
	{
		xa_hw_renderer_stop(d);
		d->state ^= XA_RENDERER_FLAG_RUNNING | XA_RENDERER_FLAG_IDLE;
		return;
	}
	/* ...submit null buffer until renderer has actual data */
	if (d->submit_flag & RENDERER_SUBMIT_NULL)
	{
		d->submitted++;
	}

	d->rendered++;

	if (d->rendered >= d->submitted)
	{
		/* ...underrun condition */

		/* ...reset submitted value to match rendered. */
		d->submitted = d->rendered + 1;

		d->state ^= XA_RENDERER_FLAG_RUNNING | XA_RENDERER_FLAG_IDLE;
	}

	/* ...notify user input buffer consumption only if in running state */
	if (d->submit_flag == RENDERER_SUBMIT_RUNNING)
	{
#ifdef HAVE_FREERTOS
		xTaskNotifyFromISR(d->irq_thread, 0, eNoAction, &woken);
		portYIELD_FROM_ISR(woken);
#else
		xos_sem_put(&d->irq_sem);
#endif
	}

	if (d->submit_flag == RENDERER_SUBMIT_INIT)
	{
		d->submit_flag = RENDERER_SUBMIT_RUNNING;
	}
}

/* Hardware-specific initialization
 * Setup SAI and DMA */
static void evk_hw_renderer_init(void* ptr)
{
	sai_transceiver_t s_TxConfig;
    edma_channel_config_t transferConfig;
    XARenderer *d = (XARenderer*) ptr;
    uint32_t num_descriptors, tx_size, buf_index, next_desc, fifo_index;
    bool mono, enable_int;
    uint8_t channelNum = d->codec_channels == 1 ? 2 : d->codec_channels;
    sai_mono_stereo_t sai_channel = d->codec_channels == 1 ? kSAI_MonoLeft : kSAI_Stereo;

    EDMA_EnableRequest(DMA_RENDERER, DMA_REQUEST);
    EDMA_CreateHandle(&g_saiDmaHandle, DMA_RENDERER, dma_channel_map[d->sai_device]);
    EDMA_SetChannelMux(DMA_RENDERER, dma_channel_map[d->sai_device], DMA_REQUEST);

    SAI_Init(sai_device_map[d->sai_device]);
    SAI_TransferTxCreateHandleEDMA(sai_device_map[d->sai_device], &saiTxHandle, TxRenderCallbackISR, ptr, &g_saiDmaHandle);
    SAI_GetClassicI2SConfig(&s_TxConfig, d->pcm_width, sai_channel, 1U << dma_channel_map[d->sai_device]);
    s_TxConfig.masterSlave = d->sai_master;
    SAI_TransferTxSetConfigEDMA(sai_device_map[d->sai_device], &saiTxHandle, &s_TxConfig);
    /* set bit clock divider */
    SAI_TxSetBitClockRate(sai_device_map[d->sai_device], CLOCK_GetSaiClkFreq(), d->rate, d->codec_pcm_width,
            channelNum);

	sai_master_clock_t mclkConfig;
    mclkConfig.mclkOutputEnable = true;
    mclkConfig.mclkHz           = 24576000U;
    mclkConfig.mclkSourceClkHz  = 24576000U;
    SAI_SetMasterClockConfig(sai_device_map[d->sai_device], &mclkConfig);

    /* Calculate how many DMA descriptors will be needed per frame transfer */
    num_descriptors = dma_calc_num_descriptors(d->codec_frame_size);
    /* Calculate fixed size per DMA transfer.  Must be evenly divisible by
     * number of descriptors. */
    tx_size = d->codec_frame_size / num_descriptors;
    /* Only enable DMA interrupt on initial transfer if using one descriptor */
    enable_int = (num_descriptors == 1);

}

static XA_ERRORCODE xa_hw_renderer_init(XARenderer *d)
{
    uint8_t i;

    /* ...reset frame counters */
    d->rendered = 0;
    d->submitted = 0;
    d->consumed = 0;
    d->bytes_produced = 0;
    d->submit_flag = 0;

    /* Assign DMA / audio buffer slot. */
    for (i = 0; i < MAX_RENDERERS; i++)
    {
        if (!g_renderIndexMap[i])
        {
            g_renderIndexMap[i] = 1;
            d->renderIndex = i;
            break;
        }
    }

    /* Check to make sure a free slot was available. */
    if (i >= MAX_RENDERERS)
    {
        return XA_RENDERER_CONFIG_FATAL_HW;
    }

    memset(d->fifo_head[0], 0, d->codec_frame_size);
    memset(d->fifo_head[1], 0, d->codec_frame_size);

    d->edmaDescriptor = &s_edmaDescriptorPingpongSAI[MAX_RENDERERS * i];

#ifdef HAVE_FREERTOS
    xTaskCreate(TxRenderCallback, "TxRenderCallback", 1024, d, configMAX_PRIORITIES - 1, &d->irq_thread);
#else
    xos_sem_create(&d->irq_sem, 0, 0);
    xos_thread_create(&d->irq_thread, NULL, TxRenderCallback, d, "TxRendererCallback", d->irq_stack, sizeof(d->irq_stack), XOS_MAX_PRIORITY - 1, 0, 0);
#endif

    /* Initialize hardware */
    evk_hw_renderer_init(d);

    return XA_NO_ERROR;
}

/* ...submit data (in bytes) into internal renderer ring-buffer */
static UWORD32 xa_hw_renderer_submit(XARenderer *d, void *b, UWORD32 n)
{
    UWORD32 bytes_write, i;
    UWORD32 buffer_available;
    UWORD32 zfill = 0;
    UWORD8 *input_buffer = b;
    UWORD8 *fifo_head = d->fifo_head[d->submitted % 2];
    UWORD8 input_buffer_step, fifo_head_step, k;
    int j, o;

    input_buffer_step = d->pcm_width >> 3;
    fifo_head_step    = d->codec_pcm_width >> 3;

    /* ...reset optional output-bytes produced */
    d->bytes_produced = 0;

    buffer_available = (2 - (d->submitted - d->rendered)) * d->frame_size;
    bytes_write = (n > buffer_available ? buffer_available : n);
    zfill = d->frame_size - n;

    if (bytes_write > 0)
    {
        /* ...Processing loop - filling fifo head buffer */
        if ((d->channels == d->codec_channels) && (d->pcm_width == d->codec_pcm_width))
        {
            /*... For the same num of channels and pcm_width values copy data with faster method */
            memcpy(fifo_head, input_buffer, bytes_write);
#ifdef FSL_FEATURE_SOC_XCACHE_COUNT
            xthal_dcache_region_writeback(fifo_head, bytes_write);
#endif
            if(zfill > 0)
            {
            	memset(fifo_head + bytes_write, 0, zfill);
            }
        }
        else
        {
            /*... For the different num of channels or pcm_width values interleave the actual data with zeros  */
            for (i = 0; i < (bytes_write / input_buffer_step); )
            {
                /* Copy bytes from input buffer to fifo head buffer */
                for (k = 0; k < d->channels; k++)
                {
                    for (j = (fifo_head_step - 1), o = (input_buffer_step - 1); j >= 0; j--, o--)
                    {
                        fifo_head[j] = o >= 0 ? input_buffer[o] : 0xFF;
                    }

                    /* Update pointers */
                    input_buffer = input_buffer + input_buffer_step;
                    fifo_head    = fifo_head + fifo_head_step;
                    i++;
                }

                /* If the codec has more channels than the application, add zeros to the fifo head buffer ... */
                for (k = 0; k < (d->codec_channels - d->channels); k++)
                {
                    for (j = 0; j < fifo_head_step; j++)
                    {
                        fifo_head[j] = 0x00;
                    }
                    /* Update fifo head pointer */
                    fifo_head = fifo_head + fifo_head_step;
                }
            }
        }

        /* ...write to optional output port buffer */
        if (d->output)
        {
            memcpy(d->output, b, bytes_write);
        }
        d->bytes_produced = bytes_write;

        buffer_available -= d->frame_size;

        /* ...increment number of submitted frames */
        d->submitted++;
    }

    /* ...declare exec done on input over and if no more valid data is available */
    d->exec_done = (d->input_over && (n == 0));

    /* ...move to INIT state if renderer was transferring empty data */
    if (d->submit_flag == RENDERER_SUBMIT_NULL)
    {
        d->submit_flag = RENDERER_SUBMIT_INIT;
    }

    /* ...start up transmission if input buffer fills with 2 frames */
    if ((d->state & XA_RENDERER_FLAG_IDLE) && (buffer_available == 0))
    {
        xa_hw_renderer_start(d);

        d->submit_flag = RENDERER_SUBMIT_RUNNING;
        d->state ^= XA_RENDERER_FLAG_IDLE | XA_RENDERER_FLAG_RUNNING;
    }

    return bytes_write;
}

/* ...state retrieval function */
static inline UWORD32 xa_hw_renderer_get_state(XARenderer *d)
{
    if (d->state & XA_RENDERER_FLAG_RUNNING)
        return XA_RENDERER_STATE_RUN;
    else if (d->state & XA_RENDERER_FLAG_PAUSED)
        return XA_RENDERER_STATE_PAUSE;
    else
        return XA_RENDERER_STATE_IDLE;
}

/* ...HW-renderer control function */
static XA_ERRORCODE xa_hw_renderer_control(XARenderer *d, UWORD32 state)
{
    switch (state)
    {
    case XA_RENDERER_STATE_START:
        /* ...process buffer start-up, on trigger from application */
        if ((d->state & XA_RENDERER_FLAG_IDLE))
        {
            /* ...start renderer DMA with NULL buffers.
             * This allows for a predictable latency between capturer and renderer
             * if interfaces share SAI clocks */
            d->submitted = 2;

            xa_hw_renderer_start(d);

            /* ...set DMA to transmit NULL data until valid data is available */
            d->submit_flag = RENDERER_SUBMIT_NULL;

            /* ...change state to Running */
            d->state ^= (XA_RENDERER_FLAG_IDLE | XA_RENDERER_FLAG_RUNNING);
        }
        return XA_NO_ERROR;

    case XA_RENDERER_STATE_RUN:
        /* ...renderer must be in paused state */
        XF_CHK_ERR(d->state & XA_RENDERER_FLAG_PAUSED, XA_RENDERER_EXEC_NONFATAL_STATE);

        /* ...resume renderer operation */
        xa_hw_renderer_resume(d);

        /* ...mark renderer is running */
        d->state ^= XA_RENDERER_FLAG_RUNNING | XA_RENDERER_FLAG_PAUSED;
        return XA_NO_ERROR;

    case XA_RENDERER_STATE_PAUSE:
        /* ...renderer must be in running state */
        XF_CHK_ERR(d->state & XA_RENDERER_FLAG_RUNNING, XA_RENDERER_EXEC_NONFATAL_STATE);
        /* ...pause renderer operation */
        xa_hw_renderer_pause(d);

        /* ...mark renderer is paused */
        d->state ^= XA_RENDERER_FLAG_RUNNING | XA_RENDERER_FLAG_PAUSED;
        return XA_NO_ERROR;

    case XA_RENDERER_STATE_IDLE:
        /* ...command is valid in any active state; stop renderer operation */
        xa_hw_renderer_close(d);

        /* ...reset renderer flags */
        d->state &= ~(XA_RENDERER_FLAG_RUNNING | XA_RENDERER_FLAG_PAUSED);
        return XA_NO_ERROR;

    default:
        /* ...unrecognized command */
        return XF_CHK_ERR(0, XA_API_FATAL_INVALID_CMD_TYPE);
    }
}

/*******************************************************************************
 * API command hooks
 ******************************************************************************/

/* ...standard codec initialization routine */
static XA_ERRORCODE xa_renderer_get_api_size(XARenderer *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...check parameters are sane */
    XF_CHK_ERR(pv_value, XA_API_FATAL_INVALID_CMD_TYPE);
    /* ...retrieve API structure size */
    *(WORD32 *)pv_value = sizeof(*d);

    return XA_NO_ERROR;
}

/* ...standard codec initialization routine */
static XA_ERRORCODE xa_renderer_init(XARenderer *d, WORD32 i_idx, pVOID pv_value)
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

        /* ...default to 48 kHz / 16-bit / 2-channel */
        d->channels = 2;
        d->pcm_width = 16;
#if SAI_SUPPORT_44100_HZ
        d->rate = 44100;
#else
        d->rate = 48000;
#endif

        /* ...default to 4k  */
        d->frame_size = MAX_DMA_TRANSFER_SIZE;

        d->codec_channels = 0;
        d->codec_pcm_width = 0;
        d->codec_frame_size = 0;

        /* ...hardware defaults */
        d->sai_device = 0;
        d->sai_master = kSAI_Master;
        d->position = 0;

        /* ...and mark renderer has been created */
        d->state = XA_RENDERER_FLAG_PREINIT_DONE;

        return XA_NO_ERROR;
    }

    case XA_CMD_TYPE_INIT_API_POST_CONFIG_PARAMS:
    {
        /* ...post-configuration initialization (all parameters are set) */
        XF_CHK_ERR(d->state & XA_RENDERER_FLAG_PREINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

        /* Fill in the codec parameters - it may differ from the application parameters, because some HW codecs only support 2 or 8 channel transmission */
        d->codec_channels   = d->codec_channels == 0 ? d->channels : d->codec_channels;
        d->codec_pcm_width  = d->codec_pcm_width == 0 ? d->pcm_width : d->codec_pcm_width;
        d->codec_frame_size = d->codec_frame_size == 0 ? d->frame_size: d->codec_frame_size;

        /* ...check calculated codec parameters */
        XF_CHK_ERR(d->codec_channels >= d->channels, XA_RENDERER_CONFIG_NONFATAL_RANGE);
        XF_CHK_ERR(d->codec_pcm_width >= d->pcm_width, XA_RENDERER_CONFIG_NONFATAL_RANGE);
        XF_CHK_ERR(d->codec_frame_size >= d->frame_size, XA_RENDERER_CONFIG_NONFATAL_RANGE);

        /* Renderer initialization */
        XF_CHK_ERR(xa_hw_renderer_init(d) == 0, XA_RENDERER_CONFIG_FATAL_HW);

        /* ...mark post-initialization is complete */
        d->state |= XA_RENDERER_FLAG_POSTINIT_DONE;
        return XA_NO_ERROR;
    }

    case XA_CMD_TYPE_INIT_PROCESS:
    {
        /* ...kick run-time initialization process; make sure setup is complete */
        XF_CHK_ERR(d->state & XA_RENDERER_FLAG_POSTINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);
        /* ...mark renderer is in idle state */
        d->state |= XA_RENDERER_FLAG_IDLE;
        return XA_NO_ERROR;
    }

    case XA_CMD_TYPE_INIT_DONE_QUERY:
    {
        /* ...check if initialization is done; make sure pointer is sane */
        XF_CHK_ERR(pv_value, XA_API_FATAL_INVALID_CMD_TYPE);
        /* ...put current status */
        *(WORD32 *)pv_value = (d->state & XA_RENDERER_FLAG_IDLE ? 1 : 0);
        return XA_NO_ERROR;
    }

    default:
        /* ...unrecognized command type */
        return XF_CHK_ERR(0, XA_API_FATAL_INVALID_CMD_TYPE);
    }
}

/* ...set renderer configuration parameter */
static XA_ERRORCODE xa_renderer_set_config_param(XARenderer *d, WORD32 i_idx, pVOID pv_value)
{
    UWORD32     i_value;

    /* ...sanity check - pointers must be sane */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);
    /* ...pre-initialization must be completed */
    XF_CHK_ERR(d->state & XA_RENDERER_FLAG_PREINIT_DONE, XA_RENDERER_CONFIG_FATAL_STATE);

    /* ...process individual configuration parameter */
    switch (i_idx)
    {
    case XA_RENDERER_CONFIG_PARAM_PCM_WIDTH:
        /* ...command is valid only in configuration state */
        XF_CHK_ERR((d->state & XA_RENDERER_FLAG_POSTINIT_DONE) == 0, XA_RENDERER_CONFIG_FATAL_STATE);
        /* ...get requested PCM width */
        i_value = (UWORD32) *(WORD32 *)pv_value;
        /* ...check value is permitted  */
        XF_CHK_ERR(i_value == 16 || i_value == 32, XA_RENDERER_CONFIG_NONFATAL_RANGE);
        /* ...apply setting */
        d->pcm_width = i_value;

        return XA_NO_ERROR;

    case XA_RENDERER_CONFIG_PARAM_CODEC_PCM_WIDTH:
            /* ...command is valid only in configuration state */
            XF_CHK_ERR((d->state & XA_RENDERER_FLAG_POSTINIT_DONE) == 0, XA_RENDERER_CONFIG_FATAL_STATE);
            /* ...get requested PCM width */
            i_value = (UWORD32) *(WORD32 *)pv_value;
            /* ...check value is permitted  */
            XF_CHK_ERR(i_value == 16 || i_value == 32, XA_RENDERER_CONFIG_NONFATAL_RANGE);
            /* ...apply setting */
            d->codec_pcm_width = i_value;

            return XA_NO_ERROR;

    case XA_RENDERER_CONFIG_PARAM_CHANNELS:
        /* ...command is valid only in configuration state */
        XF_CHK_ERR((d->state & XA_RENDERER_FLAG_POSTINIT_DONE) == 0, XA_RENDERER_CONFIG_FATAL_STATE);
        /* ...get requested channel number */
        i_value = (UWORD32) *(WORD32 *)pv_value;
        /* ...allow 1-8 channels */
        XF_CHK_ERR(i_value >= 1 && i_value <= 8, XA_RENDERER_CONFIG_NONFATAL_RANGE);
        /* ...apply setting */
        d->channels = (UWORD32)i_value;

        return XA_NO_ERROR;

    case XA_RENDERER_CONFIG_PARAM_CODEC_CHANNELS:
            /* ...command is valid only in configuration state */
            XF_CHK_ERR((d->state & XA_RENDERER_FLAG_POSTINIT_DONE) == 0, XA_RENDERER_CONFIG_FATAL_STATE);
            /* ...get requested channel number */
            i_value = (UWORD32) *(WORD32 *)pv_value;
            /* ...allow 1-8 channels */
            XF_CHK_ERR(i_value >= 1 && i_value <= 8, XA_RENDERER_CONFIG_NONFATAL_RANGE);
            /* ...apply setting */
            d->codec_channels = (UWORD32)i_value;

            return XA_NO_ERROR;

    case XA_RENDERER_CONFIG_PARAM_SAMPLE_RATE:
        /* ...command is valid only in configuration state */
        XF_CHK_ERR((d->state & XA_RENDERER_FLAG_POSTINIT_DONE) == 0, XA_RENDERER_CONFIG_FATAL_STATE);
        /* ...get requested sampling rate */
        i_value = (UWORD32) *(WORD32 *)pv_value;
#ifdef SAI_SUPPORT_44100_HZ
        /* ...allow 11,025, 22,05, 44,1 kHz */
        XF_CHK_ERR(i_value == 44100 || i_value == 22050 || i_value == 11025, XA_RENDERER_CONFIG_NONFATAL_RANGE);
#else
        /* ...allow 8, 16, 24, 32, 48, 96 kHz */
        XF_CHK_ERR(i_value == 96000 || i_value == 48000 || i_value == 32000 || i_value == 24000 || i_value == 16000 || i_value == 8000,
                   XA_RENDERER_CONFIG_NONFATAL_RANGE);
#endif
        /* ...apply setting */
        d->rate = (UWORD32)i_value;

        return XA_NO_ERROR;

    case XA_RENDERER_CONFIG_PARAM_FRAME_SIZE:
        /* ...command is valid only in configuration state */
        XF_CHK_ERR((d->state & XA_RENDERER_FLAG_POSTINIT_DONE) == 0, XA_RENDERER_CONFIG_FATAL_STATE);
        /* ...get requested frame size */
        i_value = (UWORD32) *(WORD32 *)pv_value;
        /* ...check it is below max supported value */
        XF_CHK_ERR(i_value <= MAX_FRAME_SIZE, XA_RENDERER_CONFIG_NONFATAL_RANGE);
        /* ...apply setting */
        d->frame_size = i_value;
        /* ...check it is evenly divisible into DMA transfers */
        XF_CHK_ERR(dma_calc_num_descriptors(d->frame_size) > 0, XA_RENDERER_CONFIG_NONFATAL_RANGE);

        return XA_NO_ERROR;

    case XA_RENDERER_CONFIG_PARAM_CODEC_FRAME_SIZE:
            /* ...command is valid only in configuration state */
            XF_CHK_ERR((d->state & XA_RENDERER_FLAG_POSTINIT_DONE) == 0, XA_RENDERER_CONFIG_FATAL_STATE);
            /* ...get requested frame size */
            i_value = (UWORD32) *(WORD32 *)pv_value;
            /* ...check it is below max supported value */
            XF_CHK_ERR(i_value <= MAX_FRAME_SIZE, XA_RENDERER_CONFIG_NONFATAL_RANGE);
            /* ...apply setting */
            d->codec_frame_size = i_value;
            /* ...check it is evenly divisible into DMA transfers */
            XF_CHK_ERR(dma_calc_num_descriptors(d->codec_frame_size) > 0, XA_RENDERER_CONFIG_NONFATAL_RANGE);

            return XA_NO_ERROR;

    case XA_RENDERER_CONFIG_PARAM_CB:
        /* ...set opaque callback data function */
        d->cdata = (xa_renderer_cb_t *)pv_value;

        return XA_NO_ERROR;

    case XA_RENDERER_CONFIG_PARAM_STATE:
        /* ...runtime state control parameter valid only in execution state */
        XF_CHK_ERR(d->state & XA_RENDERER_FLAG_POSTINIT_DONE, XA_RENDERER_CONFIG_FATAL_STATE);
        /* ...get requested state */
        i_value = (UWORD32) *(WORD32 *)pv_value;
        /* ...pass to state control hook */
        return xa_hw_renderer_control(d, i_value);

    case XA_RENDERER_CONFIG_PARAM_I2S_INTERFACE:
        /* ...command is valid only in configuration state */
        XF_CHK_ERR((d->state & XA_RENDERER_FLAG_POSTINIT_DONE) == 0, XA_RENDERER_CONFIG_FATAL_STATE);
        /* ...get requested interface */
        i_value = (UWORD32) *(WORD32 *)pv_value;
        /* ...check it is in allowable range */
        XF_CHK_ERR(i_value <= 5, XA_RENDERER_CONFIG_NONFATAL_RANGE);
        /* ...apply setting */
        d->sai_device = i_value;

        return XA_NO_ERROR;

    case XA_RENDERER_CONFIG_PARAM_I2S_MASTER_SLAVE:
        /* ...command is valid only in configuration state */
        XF_CHK_ERR((d->state & XA_RENDERER_FLAG_POSTINIT_DONE) == 0, XA_RENDERER_CONFIG_FATAL_STATE);
        /* ...get requested boolean value */
        i_value = (UWORD32) *(WORD32 *)pv_value;
        /* ...apply setting */
        d->sai_master = i_value;

        return XA_NO_ERROR;

    case XA_RENDERER_CONFIG_PARAM_I2S_MODE:
        /* ...command is valid only in configuration state */
        XF_CHK_ERR((d->state & XA_RENDERER_FLAG_POSTINIT_DONE) == 0, XA_RENDERER_CONFIG_FATAL_STATE);
        /* ...get requested mode */
        i_value = (UWORD32) *(WORD32 *)pv_value;
        XF_CHK_ERR(i_value == kSAI_BusLeftJustified || i_value == kSAI_BusRightJustified || i_value == kSAI_BusI2S ||
                   i_value == kSAI_BusPCMA || i_value == kSAI_BusPCMB, XA_RENDERER_CONFIG_NONFATAL_RANGE);
        /* ...apply setting */
        d->sai_mode = i_value;

        return XA_NO_ERROR;

    case XA_RENDERER_CONFIG_PARAM_I2S_SCK_POLARITY:
        /* ...command is valid only in configuration state */
        XF_CHK_ERR((d->state & XA_RENDERER_FLAG_POSTINIT_DONE) == 0, XA_RENDERER_CONFIG_FATAL_STATE);
        /* ...get requested boolean value */
        i_value = (UWORD32) *(WORD32 *)pv_value;
        /* ...apply setting */
        d->sai_sck_polarity = i_value;

        return XA_NO_ERROR;

    case XA_RENDERER_CONFIG_PARAM_I2S_WS_POLARITY:
        /* ...command is valid only in configuration state */
        XF_CHK_ERR((d->state & XA_RENDERER_FLAG_POSTINIT_DONE) == 0, XA_RENDERER_CONFIG_FATAL_STATE);
        /* ...get requested boolean value */
        i_value = (UWORD32) *(WORD32 *)pv_value;
        /* ...apply setting */
        d->sai_ws_polarity = i_value;

        return XA_NO_ERROR;

    case XA_RENDERER_CONFIG_PARAM_I2S_POSITION:
            /* ...command is valid only in configuration state */
            XF_CHK_ERR((d->state & XA_RENDERER_FLAG_POSTINIT_DONE) == 0, XA_RENDERER_CONFIG_FATAL_STATE);
            /* ...get requested boolean value */
            i_value = (UWORD32) *(WORD32 *)pv_value;
            /* ...apply setting */
            d->position = i_value;

            return XA_NO_ERROR;

    case XA_RENDERER_CONFIG_PARAM_AUDIO_BUFFER_1:
        /* ...command is valid only in configuration state */
        XF_CHK_ERR((d->state & XA_RENDERER_FLAG_POSTINIT_DONE) == 0, XA_RENDERER_CONFIG_FATAL_STATE);
        d->fifo_head[0] = (UWORD8*) *(UWORD32*)pv_value;
        return XA_NO_ERROR;

    case XA_RENDERER_CONFIG_PARAM_AUDIO_BUFFER_2:
        /* ...command is valid only in configuration state */
        XF_CHK_ERR((d->state & XA_RENDERER_FLAG_POSTINIT_DONE) == 0, XA_RENDERER_CONFIG_FATAL_STATE);
        d->fifo_head[1] = (UWORD8*) *(UWORD32*)pv_value;
        return XA_NO_ERROR;

    default:
        /* ...unrecognized parameter */
        return XF_CHK_ERR(0, XA_API_FATAL_INVALID_CMD_TYPE);
    }
}

/* ...retrieve configuration parameter */
static XA_ERRORCODE xa_renderer_get_config_param(XARenderer *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...sanity check - renderer must be initialized */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...make sure pre-initialization is completed */
    XF_CHK_ERR(d->state & XA_RENDERER_FLAG_PREINIT_DONE, XA_RENDERER_CONFIG_FATAL_STATE);

    /* ...process individual configuration parameter */
    switch (i_idx)
    {
    case XA_RENDERER_CONFIG_PARAM_PCM_WIDTH:
        /* ...return current PCM width */
        *(WORD32 *)pv_value = d->pcm_width;
        return XA_NO_ERROR;

    case XA_RENDERER_CONFIG_PARAM_CHANNELS:
        /* ...return current channel number */
        *(WORD32 *)pv_value = d->channels;
        return XA_NO_ERROR;

    case XA_RENDERER_CONFIG_PARAM_SAMPLE_RATE:
        /* ...return current sampling rate */
        *(WORD32 *)pv_value = d->rate;
        return XA_NO_ERROR;

    case XA_RENDERER_CONFIG_PARAM_FRAME_SIZE:
        /* ...return current audio frame length (in bytes) */
        *(WORD32 *)pv_value = d->frame_size;
        return XA_NO_ERROR;

    case XA_RENDERER_CONFIG_PARAM_STATE:
        /* ...return current execution state */
        *(WORD32 *)pv_value = xa_hw_renderer_get_state(d);
        return XA_NO_ERROR;

    case XA_RENDERER_CONFIG_PARAM_BYTES_PRODUCED:
        /* ...return current execution state */
        *(UWORD64 *)pv_value = d->cumulative_bytes_produced;
        return XA_NO_ERROR;

    default:
        /* ...unrecognized parameter */
        return XF_CHK_ERR(0, XA_API_FATAL_INVALID_CMD_TYPE);
    }
}

static XA_ERRORCODE xa_renderer_do_exec(XARenderer *d)
{
    d->consumed = xa_hw_renderer_submit(d, d->input, d->submited_inbytes);

    d->cumulative_bytes_produced += d->consumed;

    return XA_NO_ERROR;
}

/* ...execution command */
static XA_ERRORCODE xa_renderer_execute(XARenderer *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...sanity check - pointer must be valid */
    XF_CHK_ERR(d, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...renderer must be in running state */
    XF_CHK_ERR(d->state & (XA_RENDERER_FLAG_RUNNING | XA_RENDERER_FLAG_IDLE), XA_RENDERER_EXEC_FATAL_STATE);

    /* ...process individual command type */
    switch (i_idx)
    {
    case XA_CMD_TYPE_DO_EXECUTE:
        return xa_renderer_do_exec(d);

    case XA_CMD_TYPE_DONE_QUERY:
        XF_CHK_ERR(pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

        *(WORD32 *)pv_value = d->exec_done;

        return XA_NO_ERROR;

    case XA_CMD_TYPE_DO_RUNTIME_INIT:
        /* ...silently ignore */
        return XA_NO_ERROR;

    default:
        /* ...unrecognized command */
        return XF_CHK_ERR(0, XA_API_FATAL_INVALID_CMD_TYPE);
    }
}

/* ...set number of input bytes */
static XA_ERRORCODE xa_renderer_set_input_bytes(XARenderer *d, WORD32 i_idx, pVOID pv_value)
{
    UWORD32 samples;

    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...make sure it is an input port  */
    XF_CHK_ERR(i_idx == 0, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...renderer must be initialized */
    XF_CHK_ERR(d->state & XA_RENDERER_FLAG_POSTINIT_DONE, XA_RENDERER_EXEC_FATAL_STATE);

    /* ...input buffer pointer must be valid */
    XF_CHK_ERR(d->input, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...check buffer size is sane */
    XF_CHK_ERR((samples = *(UWORD32 *)pv_value / (d->channels * d->pcm_width / 8)) >= 0, XA_RENDERER_EXEC_FATAL_INPUT);

    d->submited_inbytes = *(UWORD32 *)pv_value;

    /* ...all is correct */
    return XA_NO_ERROR;
}

/* ...get number of output bytes */
static XA_ERRORCODE xa_renderer_get_output_bytes(XARenderer *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...sanity check - check parameters */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...track index must be valid */
    XF_CHK_ERR(i_idx == 1, XA_API_FATAL_INVALID_CMD_TYPE);

    XF_CHK_ERR(d->state & XA_RENDERER_FLAG_POSTINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...output buffer must exist */
//    XF_CHK_ERR(d->output, XA_RENDERER_EXEC_NONFATAL_OUTPUT);

    /* ...return number of produced bytes */
    *(WORD32 *)pv_value = d->bytes_produced;

    return XA_NO_ERROR;
}

/* ...get number of consumed bytes */
static XA_ERRORCODE xa_renderer_get_curidx_input_buf(XARenderer *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...sanity check - check parameters */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...input buffer index must be valid */
    XF_CHK_ERR(i_idx == 0, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...renderer must be in post-init state */
    XF_CHK_ERR(d->state & XA_RENDERER_FLAG_POSTINIT_DONE, XA_RENDERER_EXEC_FATAL_STATE);

    /* ...input buffer must exist */
    XF_CHK_ERR(d->input, XA_RENDERER_EXEC_FATAL_INPUT);

    /* ...return number of bytes consumed */
    *(WORD32 *)pv_value = d->consumed;

    /* ...reset consumed value to zero */
    d->consumed = 0;

    return XA_NO_ERROR;
}

/*******************************************************************************
 * Memory information API
 ******************************************************************************/

/* ..get total amount of data for memory tables */
static XA_ERRORCODE xa_renderer_get_memtabs_size(XARenderer *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...basic sanity checks */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...check renderer is pre-initialized */
    XF_CHK_ERR(d->state & XA_RENDERER_FLAG_PREINIT_DONE, XA_RENDERER_CONFIG_FATAL_STATE);

    /* ...we have all our tables inside API structure */
    *(WORD32 *)pv_value = 0;

    return XA_NO_ERROR;
}

/* ..set memory tables pointer */
static XA_ERRORCODE xa_renderer_set_memtabs_ptr(XARenderer *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...basic sanity checks */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...check renderer is pre-initialized */
    XF_CHK_ERR(d->state & XA_RENDERER_FLAG_PREINIT_DONE, XA_RENDERER_CONFIG_FATAL_STATE);

    /* ...do not do anything; just return success - tbd */
    return XA_NO_ERROR;
}

/* ...return total amount of memory buffers */
static XA_ERRORCODE xa_renderer_get_n_memtabs(XARenderer *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...basic sanity checks */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...we have 1 input buffer and 1 optional output buffer */
    *(WORD32 *)pv_value = 2;

    return XA_NO_ERROR;
}

/* ...return memory buffer data */
static XA_ERRORCODE xa_renderer_get_mem_info_size(XARenderer *d, WORD32 i_idx, pVOID pv_value)
{
    UWORD32     i_value;

    /* ...basic sanity check */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...command valid only after post-initialization step */
    XF_CHK_ERR(d->state & XA_RENDERER_FLAG_POSTINIT_DONE, XA_RENDERER_CONFIG_FATAL_STATE);

    switch (i_idx)
    {
    case 0:
        /* ...input buffer specification; accept exact audio frame */
        i_value = d->frame_size;
        break;

    case 1:
        /* ...output buffer specification; accept exact audio frame */
        i_value = d->frame_size;
        break;

    default:
        /* ...invalid index */
        return XF_CHK_ERR(0, XA_API_FATAL_INVALID_CMD_TYPE);
    }

    /* ...return buffer size to caller */
    *(WORD32 *)pv_value = (WORD32) i_value;

    return XA_NO_ERROR;
}

/* ...return memory alignment data */
static XA_ERRORCODE xa_renderer_get_mem_info_alignment(XARenderer *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...basic sanity check */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...command valid only after post-initialization step */
    XF_CHK_ERR(d->state & XA_RENDERER_FLAG_POSTINIT_DONE, XA_RENDERER_CONFIG_FATAL_STATE);

    /* ...all buffers are at least 4-bytes aligned */
    *(WORD32 *)pv_value = 4;

    return XA_NO_ERROR;
}

/* ...return memory type data */
static XA_ERRORCODE xa_renderer_get_mem_info_type(XARenderer *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...basic sanity check */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...command valid only after post-initialization step */
    XF_CHK_ERR(d->state & XA_RENDERER_FLAG_POSTINIT_DONE, XA_RENDERER_CONFIG_FATAL_STATE);

    switch (i_idx)
    {
    case 0:
        /* ...input buffers */
        *(WORD32 *)pv_value = XA_MEMTYPE_INPUT;
        return XA_NO_ERROR;

    case 1:
        /* ...output buffers */
        *(WORD32 *)pv_value = XA_MEMTYPE_OUTPUT;
        return XA_NO_ERROR;

    default:
        /* ...invalid index */
        return XF_CHK_ERR(0, XA_API_FATAL_INVALID_CMD_TYPE);
    }
}

/* ...set memory pointer */
static XA_ERRORCODE xa_renderer_set_mem_ptr(XARenderer *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...basic sanity check */
    XF_CHK_ERR(d, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...codec must be initialized */
    XF_CHK_ERR(d->state & XA_RENDERER_FLAG_POSTINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

    TRACE(INIT, _b("xa_renderer_set_mem_ptr[%u]: %p"), i_idx, pv_value);

    /* ...select memory buffer */
    switch (i_idx)
    {
    case 0:
        /* ...basic sanity check */
        XF_CHK_ERR(pv_value, XA_API_FATAL_INVALID_CMD_TYPE);
        /* ...input buffer */
        d->input = pv_value;
        return XA_NO_ERROR;

    case 1:
        /* ...output buffer(optional). Can be NULL as this is optional output. */
        d->output = pv_value;
        return XA_NO_ERROR;

    default:
        /* ...invalid index */
        return XF_CHK_ERR(0, XA_API_FATAL_INVALID_CMD_TYPE);
    }
}

/* ...set input over */
static XA_ERRORCODE xa_renderer_input_over(XARenderer *d, WORD32 i_idx, pVOID pv_value)
{
    XF_CHK_ERR(d, XA_API_FATAL_INVALID_CMD_TYPE);

    d->input_over = 1;

    return XA_NO_ERROR;
}

/*******************************************************************************
 * API command hooks
 ******************************************************************************/

static XA_ERRORCODE (* const xa_renderer_api[])(XARenderer *, WORD32, pVOID) =
{
    [XA_API_CMD_GET_API_SIZE]           = xa_renderer_get_api_size,
    [XA_API_CMD_INIT]                   = xa_renderer_init,
    [XA_API_CMD_SET_CONFIG_PARAM]       = xa_renderer_set_config_param,
    [XA_API_CMD_GET_CONFIG_PARAM]       = xa_renderer_get_config_param,
    [XA_API_CMD_EXECUTE]                = xa_renderer_execute,
    [XA_API_CMD_SET_INPUT_BYTES]        = xa_renderer_set_input_bytes,
    [XA_API_CMD_GET_CURIDX_INPUT_BUF]   = xa_renderer_get_curidx_input_buf,
    [XA_API_CMD_GET_MEMTABS_SIZE]       = xa_renderer_get_memtabs_size,
    [XA_API_CMD_SET_MEMTABS_PTR]        = xa_renderer_set_memtabs_ptr,
    [XA_API_CMD_GET_N_MEMTABS]          = xa_renderer_get_n_memtabs,
    [XA_API_CMD_GET_MEM_INFO_SIZE]      = xa_renderer_get_mem_info_size,
    [XA_API_CMD_GET_MEM_INFO_ALIGNMENT] = xa_renderer_get_mem_info_alignment,
    [XA_API_CMD_GET_MEM_INFO_TYPE]      = xa_renderer_get_mem_info_type,
    [XA_API_CMD_SET_MEM_PTR]            = xa_renderer_set_mem_ptr,
    [XA_API_CMD_INPUT_OVER]             = xa_renderer_input_over,
    [XA_API_CMD_GET_OUTPUT_BYTES]       = xa_renderer_get_output_bytes,
};

/* ...total numer of commands supported */
#define XA_RENDERER_API_COMMANDS_NUM   (sizeof(xa_renderer_api) / sizeof(xa_renderer_api[0]))

/*******************************************************************************
 * API entry point
 ******************************************************************************/

XA_ERRORCODE xa_renderer(xa_codec_handle_t p_xa_module_obj, WORD32 i_cmd, WORD32 i_idx, pVOID pv_value)
{
    XARenderer *renderer = (XARenderer *) p_xa_module_obj;

    /* ...check if command index is sane */
    XF_CHK_ERR(i_cmd < XA_RENDERER_API_COMMANDS_NUM, XA_API_FATAL_INVALID_CMD);

    /* ...see if command is defined */
    XF_CHK_ERR(xa_renderer_api[i_cmd], XA_API_FATAL_INVALID_CMD);

    /* ...execute requested command */
    return xa_renderer_api[i_cmd](renderer, i_idx, pv_value);
}
#endif //RENDERER_SAI
