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
 * xa-renderer.c
 *
 * Hardware renderer implementation using I2S
 ******************************************************************************/

#define MODULE_TAG                      RENDERER

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <xtensa/xos.h>

#include "audio/xa-renderer-api.h"
#include "xf-debug.h"

#include "fsl_dma.h"
#include "fsl_i2s.h"

/*******************************************************************************
 * Tracing configuration
 ******************************************************************************/
TRACE_TAG(INIT, 1);
TRACE_TAG(PROCESS, 1);
TRACE_TAG(UNDERRUN, 1);

/*******************************************************************************
 * Hardware parameters
 ******************************************************************************/
/* Fixed MCLK to derive I2S dividers from in master mode */
#if I2S_SUPPORT_44100_HZ
#define I2S_MCLK_FREQ 22579200
#else
#define I2S_MCLK_FREQ 24576000
#endif
/* Fixed DMA interface */
#define DMA_RENDERER DMA1
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
    void                   *output;

    /* ...pointer to DMA FIFO buffer */
    UWORD8                     *fifo_head;

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

    /* ...number of channels */
    UWORD32                     channels;

    /* ...sample width */
    UWORD32                     pcm_width;

    /* ...current sampling rate */
    UWORD32                     rate;

    /* ...flag for detecting underrun..made to non zero over submit */
    UWORD32                     submit_flag;

    /* bytes produced*/
    UWORD64                     bytes_produced;

    /* ...cumulative output bytes produced*/
    UWORD64             cumulative_bytes_produced;

    /* DMA handle */
    dma_handle_t                i2sTxDmaHandle;
    dma_descriptor_t           *dmaDescriptor;

    /* I2S configuration */
    UWORD32                     i2s_device;
    UWORD32                     i2s_master;
    i2s_mode_t                  i2s_mode;
    UWORD32                     i2s_sck_polarity;
    UWORD32                     i2s_ws_polarity;

    UWORD32                     renderIndex;

    /* ...input over flag */
    UWORD32             input_over;

    /* ...execution complete flag */
    UWORD32     exec_done;

    XosThread                   irq_thread;
    XosSem                      irq_sem;
    UWORD8                      irq_stack[1024];

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

/* Transfer ping pong buffer for audio data */
#ifdef CPU_MIMXRT595SFFOA_dsp
SDK_ALIGN(
#else
AT_NONCACHEABLE_SECTION_ALIGN(
#endif
    static uint8_t g_i2sTxBuffer[MAX_FRAME_SIZE * 2 * MAX_RENDERERS], 4
);

/* DMA descriptors for data tranfers */
#ifdef CPU_MIMXRT595SFFOA_dsp
SDK_ALIGN(
#else
AT_NONCACHEABLE_SECTION_ALIGN(
#endif
    static dma_descriptor_t s_dmaDescriptorPingpongI2S[MAX_DMA_TRANSFER_PER_FRAME * 2 * MAX_RENDERERS], 16
);

/* Track which static renderers are being used */
static uint8_t g_renderIndexMap[MAX_RENDERERS];

/* Index -> hardware device maps */
static I2S_Type *i2s_device_map[] = { I2S0, I2S1, I2S2, I2S3, I2S4, I2S5 };
static const uint8_t dma_channel_map[] = { 1, 3, 5, 7, 9, 11 };


/*******************************************************************************
 * Local functions
 ******************************************************************************/

/* Taking requested frame size, calculate the number of DMA linked descriptors
 * needed to execute a frame transfer of this size.
 *
 * Return 0 if the frame size cannot be evenly divided.
 */
static uint32_t dma_calc_num_descriptors(XARenderer *d)
{
    uint32_t num_descriptors;

    if (d->frame_size <= MAX_DMA_TRANSFER_SIZE)
    {
        num_descriptors = 1;
    }
    else
    {
        /* Need to setup multiple DMA descriptors for each frame. */
        num_descriptors = (d->frame_size / MAX_DMA_TRANSFER_SIZE);
        if (d->frame_size % MAX_DMA_TRANSFER_SIZE)
        {
            num_descriptors += 1;
        }

        if (d->frame_size % num_descriptors)
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
    TRACE(INIT, _b("xa_hw_renderer_start"));

    /* ...enable I2S DMA FIFO */
    i2s_device_map[d->i2s_device]->FIFOCFG |= I2S_FIFOCFG_DMATX_MASK;

    DMA_EnableChannelInterrupts(DMA_RENDERER, dma_channel_map[d->i2s_device]);
    DMA_StartTransfer(&d->i2sTxDmaHandle);

    I2S_Enable(i2s_device_map[d->i2s_device]);
}

/* ...stop hardware renderer */
static void xa_hw_renderer_stop(XARenderer *d)
{
    DMA_DisableChannelInterrupts(DMA_RENDERER, dma_channel_map[d->i2s_device]);
    DMA_AbortTransfer(&d->i2sTxDmaHandle);

    /* Wait until all transmitted data get out of FIFO */
    while ((i2s_device_map[d->i2s_device]->FIFOSTAT & I2S_FIFOSTAT_TXEMPTY_MASK) == 0U)
    {
    }

    /* ...disable I2S DMA after FIFO data is flushed */
    i2s_device_map[d->i2s_device]->FIFOCFG &= (~I2S_FIFOCFG_DMATX_MASK);
    i2s_device_map[d->i2s_device]->FIFOCFG |= I2S_FIFOCFG_EMPTYTX_MASK;

    I2S_Disable(i2s_device_map[d->i2s_device]);
}

/* ...close hardware renderer */
static void xa_hw_renderer_close(XARenderer *d)
{
    int32_t exitcode;

    xa_hw_renderer_stop(d);

    g_renderIndexMap[d->renderIndex] = 0;

    xos_sem_delete(&d->irq_sem);

    xos_thread_abort(&d->irq_thread, 0);
    xos_thread_join(&d->irq_thread, &exitcode);
    xos_thread_delete(&d->irq_thread);
}

/* ...pause renderer operation */
static inline void xa_hw_renderer_pause(XARenderer *d)
{
    TRACE(INIT, _b("xa_hw_renderer_pause"));

    DMA_DisableChannel(DMA_RENDERER, dma_channel_map[d->i2s_device]);
}

/* ...resume renderer operation */
static inline void xa_hw_renderer_resume(XARenderer *d)
{
    TRACE(INIT, _b("xa_hw_renderer_resume"));

    DMA_EnableChannel(DMA_RENDERER, dma_channel_map[d->i2s_device]);
}

static int TxRenderCallback(void *arg, int wake_value)
{
    XARenderer *d = (XARenderer*) arg;

    while (1)
    {
        xos_sem_get(&d->irq_sem);
        d->cdata->cb(d->cdata, 0);
    }
}

/* DMA transfer callback. */
static void TxRenderCallbackISR(struct _dma_handle *handle, void *userData, bool transferDone, uint32_t intmode)
{
    XARenderer *d = (XARenderer*) userData;

    if (transferDone)
    {
        /* ...submit null buffer until renderer has actual data */
        if (d->submit_flag & RENDERER_SUBMIT_NULL)
        {
            d->submitted++;
        }

        d->rendered++;

        if (d->rendered >= d->submitted)
        {
            /* ...underrun condition */

            /* ...stop DMA on underrun to avoid transmitting old data. */
            xa_hw_renderer_stop(d);

            /* ...reset rendered value to match submitted. */
            d->rendered = d->submitted;

            d->state ^= XA_RENDERER_FLAG_RUNNING | XA_RENDERER_FLAG_IDLE;
        }

        /* ...notify user input buffer consumption only if in running state */
        if (d->submit_flag == RENDERER_SUBMIT_RUNNING)
        {
            xos_sem_put(&d->irq_sem);
        }

        if (d->submit_flag == RENDERER_SUBMIT_INIT)
        {
            d->submit_flag = RENDERER_SUBMIT_RUNNING;
        }
    }
}

/* Hardware-specific initialization
 * Setup I2S and DMA */
static void evk_hw_renderer_init(void* ptr)
{
    i2s_config_t s_TxConfig;
    dma_channel_config_t transferConfig;
    XARenderer *d = (XARenderer*) ptr;
    uint32_t num_descriptors, tx_size, buf_index, next_desc;
    bool mono, enable_int;

    I2S_TxGetDefaultConfig(&s_TxConfig);

    s_TxConfig.dataLength = d->pcm_width;
    /* Codec requires 32 bit frameLength even for mono channel */
    s_TxConfig.frameLength = d->channels == 1 ? d->pcm_width * 2 : d->pcm_width * d->channels;
    s_TxConfig.mode = d->i2s_mode;
    s_TxConfig.sckPol = d->i2s_sck_polarity;
    s_TxConfig.wsPol = d->i2s_ws_polarity;

    /* Configure I2S master/slave based on configured input */
    if (d->i2s_master)
    {
        s_TxConfig.masterSlave = kI2S_MasterSlaveNormalMaster;
        /* Configure I2S divider based on the configured data format. */
        s_TxConfig.divider = (I2S_MCLK_FREQ / d->rate / d->pcm_width / d->channels);
    }
    else
    {
        s_TxConfig.masterSlave = kI2S_MasterSlaveNormalSlave;
        /* I2S function clock will use bit clock. */
        s_TxConfig.divider = 0;
    }

    if (d->channels == 1)
    {
        s_TxConfig.oneChannel = 1;
    }

    I2S_TxInit(i2s_device_map[d->i2s_device], &s_TxConfig);

    if (d->channels > 2)
    {
        mono = d->channels == 3 ? true : false;
        I2S_EnableSecondaryChannel(i2s_device_map[d->i2s_device], kI2S_SecondaryChannel1, mono, d->pcm_width * 2);
    }

    if (d->channels >= 5)
    {
        mono = d->channels == 5 ? true : false;
        I2S_EnableSecondaryChannel(i2s_device_map[d->i2s_device], kI2S_SecondaryChannel2, mono, d->pcm_width * 4);
    }

    if (d->channels >= 7)
    {
        mono = d->channels == 7 ? true : false;
        I2S_EnableSecondaryChannel(i2s_device_map[d->i2s_device], kI2S_SecondaryChannel3, mono, d->pcm_width * 6);
    }

    DMA_SetChannelPriority(DMA_RENDERER, dma_channel_map[d->i2s_device], kDMA_ChannelPriority3);
    DMA_CreateHandle(&d->i2sTxDmaHandle, DMA_RENDERER, dma_channel_map[d->i2s_device]);

    DMA_SetCallback(&d->i2sTxDmaHandle, TxRenderCallbackISR, ptr);

    /* Calculate how many DMA descriptors will be needed per frame transfer */
    num_descriptors = dma_calc_num_descriptors(d);
    /* Calculate fixed size per DMA transfer.  Must be evenly divisible by
     * number of descriptors. */
    tx_size = d->frame_size / num_descriptors;
    /* Only enable DMA interrupt on initial transfer if using one descriptor */
    enable_int = (num_descriptors == 1);

    DMA_PrepareChannelTransfer(&transferConfig,
                               &d->fifo_head[0],
                               (void *)&i2s_device_map[d->i2s_device]->FIFOWR,
                               DMA_CHANNEL_XFER(true, false, enable_int, false, 4U, kDMA_AddressInterleave1xWidth, kDMA_AddressInterleave0xWidth, tx_size),
                               kDMA_MemoryToPeripheral,
                               NULL,
                               &d->dmaDescriptor[0]);

    DMA_SubmitChannelTransfer(&d->i2sTxDmaHandle, &transferConfig);

    /* Build list of descriptors for ping pong buffer transfers. */
    for (int i = 0; i < (num_descriptors * 2); i++)
    {
        enable_int = ((num_descriptors == 1) || ((num_descriptors - 2) == i) || (((num_descriptors * 2) - 2) == i));

        /* Final descriptor points back to starting buffer location */
        if (i == ((num_descriptors * 2) - 1))
        {
            buf_index = 0;
            next_desc = 0;
        }
        else
        {
            buf_index = tx_size * (i + 1);
            next_desc = i + 1;
        }

        DMA_SetupDescriptor(&d->dmaDescriptor[i],
                            DMA_CHANNEL_XFER(true, false, enable_int, false, 4U, kDMA_AddressInterleave1xWidth,
                            kDMA_AddressInterleave0xWidth, tx_size),
                            &d->fifo_head[buf_index],
                            (void *)&i2s_device_map[d->i2s_device]->FIFOWR,
                            &d->dmaDescriptor[next_desc]);
    }
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

    d->fifo_head = &g_i2sTxBuffer[MAX_FRAME_SIZE * i];
    d->dmaDescriptor = &s_dmaDescriptorPingpongI2S[MAX_RENDERERS * i];

    xos_sem_create(&d->irq_sem, 0, 0);
    xos_thread_create(&d->irq_thread, NULL, TxRenderCallback, d, "TxRendererCallback", d->irq_stack, sizeof(d->irq_stack), XOS_MAX_PRIORITY - 1, 0, 0);

    /* Initialize hardware */
    evk_hw_renderer_init(d);

    return XA_NO_ERROR;
}

/* ...submit data (in bytes) into internal renderer ring-buffer */
static UWORD32 xa_hw_renderer_submit(XARenderer *d, void *b, UWORD32 n)
{
    UWORD32 bytes_write;
    UWORD32 buffer_available;
    UWORD32 offset;

    /* ...reset optional output-bytes produced */
    d->bytes_produced = 0;

    buffer_available = (2 - (d->submitted - d->rendered)) * d->frame_size;
    bytes_write = (n > buffer_available ? buffer_available : n);
    if (bytes_write > 0)
    {
        offset = (d->submitted % 2) * d->frame_size;
        memcpy((char*) d->fifo_head + offset, b, bytes_write);

        buffer_available -= d->frame_size;

        /* ...increment number of submitted frames */
        d->submitted++;
    }

    /* ...declare exec done on input over and if no more valid data is available */
    d->exec_done = (d->input_over && (n == 0));
    if (d->exec_done)
    {
        /* Flush DMA and shutdown output */
        xa_hw_renderer_stop(d);
    }

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
             * if interfaces share I2S clocks */
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
#if I2S_SUPPORT_44100_HZ
        d->rate = 44100;
#else
        d->rate = 48000;
#endif

        /* ...default to 4k  */
        d->frame_size = MAX_DMA_TRANSFER_SIZE;

        /* ...hardware defaults */
        d->i2s_device = 1;
        d->i2s_master = 1;
        d->i2s_mode = kI2S_ModeI2sClassic;
        d->i2s_sck_polarity = 0;
        d->i2s_ws_polarity = 0;

        /* ...and mark renderer has been created */
        d->state = XA_RENDERER_FLAG_PREINIT_DONE;

        return XA_NO_ERROR;
    }

    case XA_CMD_TYPE_INIT_API_POST_CONFIG_PARAMS:
    {
        /* ...post-configuration initialization (all parameters are set) */
        XF_CHK_ERR(d->state & XA_RENDERER_FLAG_PREINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

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

    case XA_RENDERER_CONFIG_PARAM_SAMPLE_RATE:
        /* ...command is valid only in configuration state */
        XF_CHK_ERR((d->state & XA_RENDERER_FLAG_POSTINIT_DONE) == 0, XA_RENDERER_CONFIG_FATAL_STATE);
        /* ...get requested sampling rate */
        i_value = (UWORD32) *(WORD32 *)pv_value;
        /* ...allow 8, 16, 24, 32, 48, 11,025, 22,05, 44,1 kHz */
        XF_CHK_ERR(i_value == 48000 || i_value == 44100 || i_value == 32000 || i_value == 24000 || i_value == 22050 || i_value == 16000 || i_value == 11025 || i_value == 8000,
                   XA_RENDERER_CONFIG_NONFATAL_RANGE);
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
        /* ...check it is evenly divisible into DMA transfers */
        XF_CHK_ERR(dma_calc_num_descriptors(d) > 0, XA_RENDERER_CONFIG_NONFATAL_RANGE);
        /* ...apply setting */
        d->frame_size = i_value;

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
        d->i2s_device = i_value;

        return XA_NO_ERROR;

    case XA_RENDERER_CONFIG_PARAM_I2S_MASTER_SLAVE:
        /* ...command is valid only in configuration state */
        XF_CHK_ERR((d->state & XA_RENDERER_FLAG_POSTINIT_DONE) == 0, XA_RENDERER_CONFIG_FATAL_STATE);
        /* ...get requested boolean value */
        i_value = (UWORD32) *(WORD32 *)pv_value;
        /* ...apply setting */
        d->i2s_master = i_value;

        return XA_NO_ERROR;

    case XA_RENDERER_CONFIG_PARAM_I2S_MODE:
        /* ...command is valid only in configuration state */
        XF_CHK_ERR((d->state & XA_RENDERER_FLAG_POSTINIT_DONE) == 0, XA_RENDERER_CONFIG_FATAL_STATE);
        /* ...get requested mode */
        i_value = (UWORD32) *(WORD32 *)pv_value;
        XF_CHK_ERR(i_value == kI2S_ModeI2sClassic || i_value == kI2S_ModeDspWs50 || i_value == kI2S_ModeDspWsShort ||
                   i_value == kI2S_ModeDspWsLong, XA_RENDERER_CONFIG_NONFATAL_RANGE);
        /* ...apply setting */
        d->i2s_mode = (i2s_mode_t) i_value;

        return XA_NO_ERROR;

    case XA_RENDERER_CONFIG_PARAM_I2S_SCK_POLARITY:
        /* ...command is valid only in configuration state */
        XF_CHK_ERR((d->state & XA_RENDERER_FLAG_POSTINIT_DONE) == 0, XA_RENDERER_CONFIG_FATAL_STATE);
        /* ...get requested boolean value */
        i_value = (UWORD32) *(WORD32 *)pv_value;
        /* ...apply setting */
        d->i2s_sck_polarity = i_value;

        return XA_NO_ERROR;

    case XA_RENDERER_CONFIG_PARAM_I2S_WS_POLARITY:
        /* ...command is valid only in configuration state */
        XF_CHK_ERR((d->state & XA_RENDERER_FLAG_POSTINIT_DONE) == 0, XA_RENDERER_CONFIG_FATAL_STATE);
        /* ...get requested boolean value */
        i_value = (UWORD32) *(WORD32 *)pv_value;
        /* ...apply setting */
        d->i2s_ws_polarity = i_value;

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
    XF_CHK_ERR(d->output, XA_RENDERER_EXEC_NONFATAL_OUTPUT);

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
