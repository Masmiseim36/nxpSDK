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
 * xa-capturer_i2s.c
 *
 * Hardware capturer implementation using I2S
 ******************************************************************************/

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

#include "fsl_dma.h"
#include "fsl_i2s.h"

/*******************************************************************************
 * Tracing configuration
 ******************************************************************************/
// TRACE_TAG(INIT, 1);
// TRACE_TAG(PROCESS, 1);
// TRACE_TAG(UNDERRUN, 1);

/*******************************************************************************
 * Hardware parameters
 ******************************************************************************/
/* Fixed MCLK to derive I2S dividers from in master mode */
#define I2S_MCLK_FREQ 24576000
/* Fixed DMA interface */
#define DMA_CAPTURER DMA1
/* Maximum number of capturer components that can exist simultaneously */
#define MAX_CAPTURERS 2
/* Maximum number of bytes that can be transferred in one DMA request */
#define MAX_DMA_TRANSFER_SIZE 4096
/* Maximum number of DMA transfers pre PCM frame */
#define MAX_DMA_TRANSFER_PER_FRAME 3
/* Maxmimum configurable frame size, based on DMA limits */
#define MAX_FRAME_SIZE (MAX_DMA_TRANSFER_SIZE * MAX_DMA_TRANSFER_PER_FRAME)

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

    /* ...pointer to DMA FIFO buffer */
    UWORD8 *fifo_head[2];

    /* ...number of samples produced */
    UWORD32 produced;

    /***************************************************************************
     * Run-time data
     **************************************************************************/

    /* ...number of channels */
    UWORD32 channels;

    /* ...sample width */
    UWORD32 pcm_width;

    /* ...current sampling rate */
    UWORD32 rate;

    /* ...size of one period buffer frame, in bytes */
    UWORD32 frame_size;

    UWORD32 over_flow_flag;

    /* total bytes produced by the component*/
    UWORD64 tot_bytes_produced;

    WORD32 newDataAvailable;

    dma_handle_t i2sRxDmaHandle;
    dma_descriptor_t *dmaDescriptor;

    /* I2S configuration */
    UWORD32 i2s_device;
    UWORD32 i2s_master;
    i2s_mode_t i2s_mode;
    UWORD32 i2s_sck_polarity;
    UWORD32 i2s_ws_polarity;

    UWORD32 captureIndex;
    WORD32 rxBufIndex;

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
/* DMA descriptors for data tranfers */
#if (XCHAL_DCACHE_SIZE > 0)
AT_NONCACHEABLE_SECTION_ALIGN(
#else
SDK_ALIGN(
#endif
    static dma_descriptor_t s_dmaDescriptorPingpongI2S[MAX_DMA_TRANSFER_PER_FRAME * 2 * MAX_CAPTURERS], 16);

/* Track which static capturers are being used */
static uint8_t g_captureIndexMap[MAX_CAPTURERS];

/* Index -> hardware device maps */
static I2S_Type *i2s_device_map[]      = {I2S0, I2S1, I2S2, I2S3, I2S4, I2S5};
static const uint8_t dma_channel_map[] = {0, 2, 4, 6, 8, 10};

/*******************************************************************************
 * Local functions
 ******************************************************************************/

/* Taking requested frame size, calculate the number of DMA linked descriptors
 * needed to execute a frame transfer of this size.
 *
 * Return 0 if the frame size cannot be evenly divided.
 */
static uint32_t dma_calc_num_descriptors(XACapturer *d)
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

#ifdef HAVE_FREERTOS
void RxCaptureCallback(void *arg)
#else
int RxCaptureCallback(void *arg, int wake_value)
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

static void RxCaptureCallbackISR(struct _dma_handle *handle, void *userData, bool transferDone, uint32_t intmode)
{
    XACapturer *d = (XACapturer *)userData;
#ifdef HAVE_FREERTOS
    BaseType_t woken = pdFALSE;
#endif

    if (transferDone)
    {
        /* Indicate data available and signal XAF.
         * Drop data if already overflow. */
        d->newDataAvailable = 1;

        if (d->over_flow_flag == 0)
        {
            d->over_flow_flag = 1;

#ifdef HAVE_FREERTOS
            xTaskNotifyFromISR(d->irq_thread, 0, eNoAction, &woken);
            portYIELD_FROM_ISR(woken);
#else
            xos_sem_put(&d->irq_sem);
#endif
        }
        else
        {
            /* Should not get here - overrun condition. */
        }
    }
}

/* Hardware-specific initialization
 * Start I2S DMA transfers */
static void evk_i2s_start(XACapturer *d)
{
    TRACE(INIT, _b("xa_hw_capturer_i2s_start"));

    /* ...enable I2S DMA FIFO */
    i2s_device_map[d->i2s_device]->FIFOCFG |= I2S_FIFOCFG_DMARX_MASK;

    DMA_EnableChannelInterrupts(DMA_CAPTURER, dma_channel_map[d->i2s_device]);
    DMA_StartTransfer(&d->i2sRxDmaHandle);

    I2S_Enable(i2s_device_map[d->i2s_device]);
}

/* Hardware-specific initialization
 * Setup I2S and DMA */
static void evk_i2s_config(void *ptr)
{
    i2s_config_t rxConfig;
    dma_channel_config_t transferConfig;
    uint32_t num_descriptors, tx_size, buf_index, next_desc, fifo_index;
    bool mono, enable_int;

    XACapturer *d = (XACapturer *)ptr;

    I2S_RxGetDefaultConfig(&rxConfig);

    rxConfig.dataLength  = d->pcm_width;
    rxConfig.frameLength = d->pcm_width * d->channels;
    rxConfig.mode        = d->i2s_mode;
    rxConfig.sckPol      = d->i2s_sck_polarity;
    rxConfig.wsPol       = d->i2s_ws_polarity;

    if (d->i2s_master)
    {
        rxConfig.masterSlave = kI2S_MasterSlaveNormalMaster;
        /* Configure I2S divider based on the configured data format. */
        rxConfig.divider = (I2S_MCLK_FREQ / d->rate / d->pcm_width / d->channels);
    }
    else
    {
        rxConfig.masterSlave = kI2S_MasterSlaveNormalSlave;
        /* I2S function clock will use external bit clock. */
        rxConfig.divider = 0;
    }

    if (d->channels == 1)
    {
        rxConfig.oneChannel = 1;
    }

    I2S_RxInit(i2s_device_map[d->i2s_device], &rxConfig);

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

    DMA_SetChannelPriority(DMA_CAPTURER, dma_channel_map[d->i2s_device], kDMA_ChannelPriority3);
    DMA_CreateHandle(&d->i2sRxDmaHandle, DMA_CAPTURER, dma_channel_map[d->i2s_device]);
    DMA_SetCallback(&d->i2sRxDmaHandle, RxCaptureCallbackISR, ptr);

    /* Calculate how many DMA descriptors will be needed per frame transfer */
    num_descriptors = dma_calc_num_descriptors(d);
    /* Calculate fixed size per DMA transfer.  Must be evenly divisible by
     * number of descriptors. */
    tx_size = d->frame_size / num_descriptors;
    /* Only enable DMA interrupt on initial transfer if using one descriptor */
    enable_int = (num_descriptors == 1);

    DMA_PrepareChannelTransfer(&transferConfig, (void *)&i2s_device_map[d->i2s_device]->FIFORD, d->fifo_head[0],
                               DMA_CHANNEL_XFER(true, false, enable_int, false, 4U, kDMA_AddressInterleave0xWidth,
                                                kDMA_AddressInterleave1xWidth, tx_size),
                               kDMA_PeripheralToMemory, NULL, &d->dmaDescriptor[0]);

    DMA_SubmitChannelTransfer(&d->i2sRxDmaHandle, &transferConfig);

    /* Build list of descriptors for ping pong buffer transfers. */
    for (int i = 0; i < (num_descriptors * 2); i++)
    {
        enable_int = ((num_descriptors == 1) || ((num_descriptors - 2) == i) || (((num_descriptors * 2) - 2) == i));

        /* Final descriptor points back to starting buffer location */
        if (i == ((num_descriptors * 2) - 1))
        {
            buf_index  = 0;
            next_desc  = 0;
            fifo_index = 0;
        }
        else
        {
            buf_index  = tx_size * ((i + 1) % num_descriptors);
            next_desc  = i + 1;
            fifo_index = (i < (num_descriptors - 1)) ? 0 : 1;
        }

        DMA_SetupDescriptor(&d->dmaDescriptor[i],
                            DMA_CHANNEL_XFER(true, false, enable_int, false, 4U, kDMA_AddressInterleave0xWidth,
                                             kDMA_AddressInterleave1xWidth, tx_size),
                            (void *)&i2s_device_map[d->i2s_device]->FIFORD, d->fifo_head[fifo_index] + buf_index,
                            &d->dmaDescriptor[next_desc]);
    }
}

static XA_ERRORCODE evk_hw_capturer_init(void *ptr)
{
    XACapturer *d = (XACapturer *)ptr;
    int i;

    d->rxBufIndex = 0;

    /* Assign DMA / audio buffer slot. */
    for (i = 0; i < MAX_CAPTURERS; i++)
    {
        if (!g_captureIndexMap[i])
        {
            g_captureIndexMap[i] = 1;
            d->captureIndex      = i;
            break;
        }
    }

    /* Check to make sure a free slot was available. */
    if (i >= MAX_CAPTURERS)
    {
        return XA_CAPTURER_CONFIG_FATAL_HW;
    }

    d->dmaDescriptor = &s_dmaDescriptorPingpongI2S[MAX_CAPTURERS * i];

#ifdef HAVE_FREERTOS
    xTaskCreate(RxCaptureCallback, "RxCaptureCallback", 1024, d, configMAX_PRIORITIES - 1, &d->irq_thread);
#else
    xos_sem_create(&d->irq_sem, 0, 0);
    xos_thread_create(&d->irq_thread, NULL, RxCaptureCallback, d, "RxCaptureCallback", d->irq_stack,
                      sizeof(d->irq_stack), XOS_MAX_PRIORITY - 1, 0, 0);
#endif

    evk_i2s_config(ptr);

    return XA_NO_ERROR;
}

/*******************************************************************************
 * Codec access functions
 ******************************************************************************/
static inline void xa_hw_capturer_close(XACapturer *d)
{
    DMA_DisableChannelInterrupts(DMA_CAPTURER, dma_channel_map[d->i2s_device]);
    DMA_AbortTransfer(&d->i2sRxDmaHandle);

    /* ...disable I2S DMA after FIFO data is flushed */
    i2s_device_map[d->i2s_device]->FIFOCFG &= (~I2S_FIFOCFG_DMARX_MASK);
    i2s_device_map[d->i2s_device]->FIFOCFG |= I2S_FIFOCFG_EMPTYRX_MASK;

    I2S_Disable(i2s_device_map[d->i2s_device]);

    g_captureIndexMap[d->captureIndex] = 0;

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

            /* ...set default capturer parameters - 16-bit stereo @ 48KHz */
            d->channels  = 2;
            d->pcm_width = 16;
            d->rate      = 48000;

            /* ...default to 4k  */
            d->frame_size = MAX_DMA_TRANSFER_SIZE;

            /* ...hardware defaults */
            d->i2s_device       = 1;
            d->i2s_master       = 0;
            d->i2s_mode         = kI2S_ModeI2sClassic;
            d->i2s_sck_polarity = 0;
            d->i2s_ws_polarity  = 0;

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

            evk_i2s_start(d);

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
static XA_ERRORCODE xa_hw_capturer_control(XACapturer *d, UWORD32 state)
{
    switch (state)
    {
        case XA_CAPTURER_STATE_START:
            /* ...capturer must be in idle state */
            XF_CHK_ERR(d->state & XA_CAPTURER_FLAG_IDLE, XA_CAPTURER_EXEC_NONFATAL_STATE);

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
            /* ...check value is permitted (16 or 32-bits only) */
            XF_CHK_ERR(i_value == 16 || i_value == 32, XA_CAPTURER_CONFIG_NONFATAL_RANGE);
            /* ...apply setting */
            d->pcm_width = i_value;

            return XA_NO_ERROR;

        case XA_CAPTURER_CONFIG_PARAM_CHANNELS:
            /* ...command is valid only in configuration state */
            XF_CHK_ERR((d->state & XA_CAPTURER_FLAG_POSTINIT_DONE) == 0, XA_CAPTURER_CONFIG_FATAL_STATE);
            /* ...get requested channel number */
            i_value = (UWORD32) * (WORD32 *)pv_value;
            /* ...allow 1-8 channels */
            XF_CHK_ERR((i_value >= 1 && i_value <= 8), XA_CAPTURER_CONFIG_NONFATAL_RANGE);
            /* ...apply setting */
            d->channels = (UWORD32)i_value;

            return XA_NO_ERROR;

        case XA_CAPTURER_CONFIG_PARAM_SAMPLE_RATE:
            /* ...command is valid only in configuration state */
            XF_CHK_ERR((d->state & XA_CAPTURER_FLAG_POSTINIT_DONE) == 0, XA_CAPTURER_CONFIG_FATAL_STATE);
            /* ...get requested sampling rate */
            i_value = (UWORD32) * (WORD32 *)pv_value;
            /* ...allow 8, 16, 24, 32, 48 kHz */
            XF_CHK_ERR(i_value == 48000 || i_value == 32000 || i_value == 24000 || i_value == 16000 || i_value == 8000,
                       XA_CAPTURER_CONFIG_NONFATAL_RANGE);
            /* ...apply setting */
            d->rate = (UWORD32)i_value;

            return XA_NO_ERROR;

        case XA_CAPTURER_CONFIG_PARAM_FRAME_SIZE:
            /* ...command is valid only in configuration state */
            XF_CHK_ERR((d->state & XA_CAPTURER_FLAG_POSTINIT_DONE) == 0, XA_CAPTURER_CONFIG_FATAL_STATE);
            /* ...get requested frame size */
            i_value = (UWORD32) * (WORD32 *)pv_value;
            /* ...check it is below max supported value */
            XF_CHK_ERR(i_value <= MAX_FRAME_SIZE, XA_CAPTURER_CONFIG_NONFATAL_RANGE);
            /* ...check it is evenly divisible into DMA transfers */
            XF_CHK_ERR(dma_calc_num_descriptors(d) > 0, XA_CAPTURER_CONFIG_NONFATAL_RANGE);
            /* ...apply setting */
            d->frame_size = i_value;

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

        case XA_CAPTURER_CONFIG_PARAM_I2S_INTERFACE:
            /* ...command is valid only in configuration state */
            XF_CHK_ERR((d->state & XA_CAPTURER_FLAG_POSTINIT_DONE) == 0, XA_CAPTURER_CONFIG_FATAL_STATE);
            /* ...get requested interface */
            i_value = (UWORD32) * (WORD32 *)pv_value;
            /* ...check it is in allowable range */
            XF_CHK_ERR(i_value <= 5, XA_CAPTURER_CONFIG_NONFATAL_RANGE);
            /* ...apply setting */
            d->i2s_device = i_value;

            return XA_NO_ERROR;

        case XA_CAPTURER_CONFIG_PARAM_I2S_MASTER_SLAVE:
            /* ...command is valid only in configuration state */
            XF_CHK_ERR((d->state & XA_CAPTURER_FLAG_POSTINIT_DONE) == 0, XA_CAPTURER_CONFIG_FATAL_STATE);
            /* ...get requested boolean value */
            i_value = (UWORD32) * (WORD32 *)pv_value;
            /* ...apply setting */
            d->i2s_master = i_value;

            return XA_NO_ERROR;

        case XA_CAPTURER_CONFIG_PARAM_I2S_MODE:
            /* ...command is valid only in configuration state */
            XF_CHK_ERR((d->state & XA_CAPTURER_FLAG_POSTINIT_DONE) == 0, XA_CAPTURER_CONFIG_FATAL_STATE);
            /* ...get requested mode */
            i_value = (UWORD32) * (WORD32 *)pv_value;
            XF_CHK_ERR(i_value == kI2S_ModeI2sClassic || i_value == kI2S_ModeDspWs50 ||
                           i_value == kI2S_ModeDspWsShort || i_value == kI2S_ModeDspWsLong,
                       XA_CAPTURER_CONFIG_NONFATAL_RANGE);
            /* ...apply setting */
            d->i2s_mode = (i2s_mode_t)i_value;

            return XA_NO_ERROR;

        case XA_CAPTURER_CONFIG_PARAM_I2S_SCK_POLARITY:
            /* ...command is valid only in configuration state */
            XF_CHK_ERR((d->state & XA_CAPTURER_FLAG_POSTINIT_DONE) == 0, XA_CAPTURER_CONFIG_FATAL_STATE);
            /* ...get requested boolean value */
            i_value = (UWORD32) * (WORD32 *)pv_value;
            /* ...apply setting */
            d->i2s_sck_polarity = i_value;

            return XA_NO_ERROR;

        case XA_CAPTURER_CONFIG_PARAM_I2S_WS_POLARITY:
            /* ...command is valid only in configuration state */
            XF_CHK_ERR((d->state & XA_CAPTURER_FLAG_POSTINIT_DONE) == 0, XA_CAPTURER_CONFIG_FATAL_STATE);
            /* ...get requested boolean value */
            i_value = (UWORD32) * (WORD32 *)pv_value;
            /* ...apply setting */
            d->i2s_ws_polarity = i_value;
            return XA_NO_ERROR;

        case XA_CAPTURER_CONFIG_PARAM_AUDIO_BUFFER_1:
            XF_CHK_ERR((d->state & XA_CAPTURER_FLAG_POSTINIT_DONE) == 0, XA_CAPTURER_CONFIG_FATAL_STATE);
            d->fifo_head[0] = (UWORD8 *)*(UWORD32 *)pv_value;
            return XA_NO_ERROR;

        case XA_CAPTURER_CONFIG_PARAM_AUDIO_BUFFER_2:
            XF_CHK_ERR((d->state & XA_CAPTURER_FLAG_POSTINIT_DONE) == 0, XA_CAPTURER_CONFIG_FATAL_STATE);
            d->fifo_head[1] = (UWORD8 *)*(UWORD32 *)pv_value;
            return XA_NO_ERROR;

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

        case XA_CAPTURER_CONFIG_PARAM_SAMPLE_RATE:
            /* ...return current sampling rate */
            *(WORD32 *)pv_value = d->rate;
            return XA_NO_ERROR;

        case XA_CAPTURER_CONFIG_PARAM_FRAME_SIZE:
            /* ...return current audio frame length (in bytes) */
            *(WORD32 *)pv_value = d->frame_size;
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

static UWORD32 xa_hw_capturer_read_FIFO(XACapturer *d)
{
    d->over_flow_flag = 0;

    /* Copy data from DMA buffer to XAF output buffer */
    memcpy(d->output, d->fifo_head[d->rxBufIndex], d->frame_size);

    /* Flip ping pong buffer for the next DMA transfer */
    d->rxBufIndex = (d->rxBufIndex + 1) % 2;

    return d->frame_size;
}

static XA_ERRORCODE xa_capturer_do_exec(XACapturer *d)
{
    WORD32 bytes_read = 0;

    if (d->newDataAvailable)
    {
        bytes_read = xa_hw_capturer_read_FIFO(d);

        d->produced           = bytes_read;
        d->tot_bytes_produced = d->tot_bytes_produced + d->produced;

        d->newDataAvailable = 0;
    }

    return XA_NO_ERROR;
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
            return xa_capturer_do_exec(d);

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
            i_value = d->frame_size;
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

/* ...set memory pointer */
static XA_ERRORCODE xa_capturer_set_mem_ptr(XACapturer *d, WORD32 i_idx, pVOID pv_value)
{
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

XA_ERRORCODE xa_capturer_i2s(xa_codec_handle_t p_xa_module_obj, WORD32 i_cmd, WORD32 i_idx, pVOID pv_value)
{
    XACapturer *capturer = (XACapturer *)p_xa_module_obj;

    /* ...check if command index is sane */
    XF_CHK_ERR(i_cmd < XA_CAPTURER_API_COMMANDS_NUM, XA_API_FATAL_INVALID_CMD);

    /* ...see if command is defined */
    XF_CHK_ERR(xa_capturer_api[i_cmd], XA_API_FATAL_INVALID_CMD);

    /* ...execute requested command */
    return xa_capturer_api[i_cmd](capturer, i_idx, pv_value);
}
