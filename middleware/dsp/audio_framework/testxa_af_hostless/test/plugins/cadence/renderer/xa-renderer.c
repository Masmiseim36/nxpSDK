/*
* Copyright (c) 2015-2022 Cadence Design Systems Inc.
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
 * dummy (dumping data to file)renderer implementation
 *
 * Copyright (c) 2012 Tensilica Inc. ALL RIGHTS RESERVED.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 ******************************************************************************/

#define MODULE_TAG                      RENDERER

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "osal-timer.h"
#include <stdio.h>
#include "audio/xa-renderer-api.h"
#include "xf-debug.h"
#include "xaf-api.h"
#include <string.h>

#ifdef XAF_PROFILE
#include "xaf-clk-test.h"
extern clk_t renderer_cycles;
#endif

/*******************************************************************************
 * Codec parameters
 ******************************************************************************/

/* ...total length of HW FIFO in bytes */
#define HW_FIFO_LENGTH                  8192

/* maximum allowed framesize in bytes per channel. This is the default framesize */
#define MAX_FRAME_SIZE_IN_BYTES_DEFAULT    ( HW_FIFO_LENGTH / 4 )      

/* minimum allowed framesize in bytes per channel */
#define MIN_FRAME_SIZE_IN_BYTES    ( 128 )

#define HW_I2S_SF (44100)

#define READ_FIFO(payload) {\
        if(d->output)\
        {\
            /* ...write to optional output buffer */\
            memcpy(d->output, d->pfifo_r, payload);\
            d->bytes_produced = payload;\
        }\
        /* ...write to output file and increment read pointer */\
        fwrite((char *)d->pfifo_r, 1, payload, d->fw); d->pfifo_r += payload;\
        if((UWORD32)d->pfifo_r >= (UWORD32)&g_fifo_renderer[2*payload])\
        {\
            d->pfifo_r = (void*)g_fifo_renderer;\
        }\
    }

#define UPDATE_WPTR(offset, payload) {\
        d->pfifo_w += offset;\
        if((UWORD32)d->pfifo_w >= (UWORD32)&g_fifo_renderer[2*payload])\
        {\
            d->pfifo_w = (void*)g_fifo_renderer;\
        }\
    }

/*******************************************************************************
 * Local data definition
 ******************************************************************************/

typedef struct XARenderer
{
    /***************************************************************************
     * Internal stuff
     **************************************************************************/

    /* ...component state */
    UWORD32                     state;

    /* ...notification callback pointer */
    xa_renderer_cb_t       *cdata;

    /* ...input buffer pointer */
    void                   *input;

    /* ...output buffer pointer */
    void                   *output;

    /* ...estimation of amount of samples that can be written into FIFO */
    UWORD32                     fifo_avail;

    /* ...number of samples consumed */
    UWORD32                     consumed;
    /* ...number of bytes copied in fifo*/
    UWORD32                     submited_inbytes;
    /***************************************************************************
     * Run-time data
     **************************************************************************/
    
    /* ...size of PCM sample in bytes  */
    UWORD32                     sample_size;

    /* ...number of channels */
    UWORD32                     channels;

    /* ...sample width */
    UWORD32                     pcm_width;
    
    /* ...framesize in bytes per channel */
    UWORD32                     frame_size_bytes;     

    /* ...current sampling rate */
    UWORD32                     rate;
    
    /* ...flag for detecting underrun..made to non zero over submit */
    UWORD32              submit_flag;

    FILE * fw;

    /* ...cumulative output bytes produced*/
    UWORD64             cumulative_bytes_produced;

    /* ...output bytes produced*/
    UWORD32             bytes_produced;

    /* ... FIFO read pointer */
    void    *pfifo_r;

    /* ... FIFO write pointer */
    void    *pfifo_w;

    /* ...input over flag */
    UWORD32             input_over;

    /* ...execution complete flag */
    UWORD32     exec_done;

    /* ...framesize in samples per channel */
    UWORD32     frame_size;

    /* ...input port bypass flag: 0 disabled (default), 1 enabled */
    UWORD32     inport_bypass;

}   XARenderer;


#define MAX_UWORD32 ((UWORD64)0xFFFFFFFF)
/*******************************************************************************
 * Operating flags
 ******************************************************************************/

#define XA_RENDERER_FLAG_PREINIT_DONE   (1 << 0)
#define XA_RENDERER_FLAG_POSTINIT_DONE  (1 << 1)
#define XA_RENDERER_FLAG_IDLE           (1 << 2)
#define XA_RENDERER_FLAG_RUNNING        (1 << 3)
#define XA_RENDERER_FLAG_PAUSED         (1 << 4)
/*******************************************************************************
 * global variables
 ******************************************************************************/

static xf_timer_t rend_timer;
const char grenderer_out_file[] = "renderer_out.pcm";
UWORD8 g_fifo_renderer[HW_FIFO_LENGTH];

/*******************************************************************************
 * xa_fw_fifo_handler
 *
 * Process FIFO exchange / underrun interrupt. Do a copy from ring buffer into
 * I2S high-buffer (normal operation). In case of low-buffer emptiness
 * (underrun) reset the FIFO and perform a recovery
 ******************************************************************************/

static void xa_fw_handler(void *arg)
{
    XARenderer *d = arg;

    //d->consumed = d->submited_inbytes;
    //d->fifo_avail = d->fifo_avail + d->submited_inbytes;
    d->fifo_avail = d->fifo_avail + (d->frame_size_bytes * d->channels);

    if((d->fifo_avail)>HW_FIFO_LENGTH)
       {/*under run case*/
           d->state ^= XA_RENDERER_FLAG_RUNNING | XA_RENDERER_FLAG_IDLE;
           d->fifo_avail = HW_FIFO_LENGTH;
           __xf_timer_stop(&rend_timer);
           d->cdata->cb(d->cdata, 0);
       }
      else if(((int)d-> fifo_avail) <= 0)
       {/* over run */
           d->state ^= XA_RENDERER_FLAG_RUNNING | XA_RENDERER_FLAG_IDLE;
           d->fifo_avail=HW_FIFO_LENGTH;
           __xf_timer_stop(&rend_timer);
           d->cdata->cb(d->cdata, 0);
       }
      else
      {
           d->cdata->cb(d->cdata, 0);
      }
}

/*******************************************************************************
 * Codec access functions
 ******************************************************************************/

static inline void xa_fw_renderer_close(XARenderer *d)
{
    fclose(d->fw);
    if ((d->state & XA_RENDERER_FLAG_RUNNING))
    {
        __xf_timer_stop(&rend_timer);
    }
    __xf_timer_destroy(&rend_timer);
}

/* ...submit data (in bytes) into internal renderer ring-buffer */
static inline UWORD32 xa_fw_renderer_submit(XARenderer *d, void *b, UWORD32 bytes_write)
{
    FILE *fp = NULL;
    UWORD32 avail;
    UWORD32 k;
    UWORD32 zfill;
    UWORD32 payload;
    UWORD32 size;

    /* ...check buffer size is sane */
    XF_CHK_ERR((size = d->submited_inbytes / (d->sample_size * d->channels)) >= 0, XA_RENDERER_EXEC_FATAL_INPUT);

    /* ...make sure we have integral amount of samples in input buffer */
    /* ...TENA-3225: If input is over, consume partial input and zero fill the rest. */
    XF_CHK_ERR(d->input_over || ((size * d->sample_size * d->channels) == d->submited_inbytes), XA_RENDERER_EXEC_FATAL_INPUT);

    fp      = d ->fw;
    payload = d->frame_size_bytes*d->channels;
    avail   = d->fifo_avail;
    k       = 0;
    zfill   = 0;    

    /* ...reset optional output-bytes produced */
    d->bytes_produced = 0;

    if(avail >= payload)
    {
        k             = (payload > bytes_write) ? bytes_write : payload;
        zfill         = payload - k;
        d->fifo_avail = (avail -= payload);

        /* ...write one frame worth data to FIFO */
        memcpy((char *)d->pfifo_w, (char *)b, k);

        if (zfill)
        {
            /* ...write zeros to complete one frame worth data to FIFO */
            memset((char *)d->pfifo_w + k, 0, zfill);

            TRACE(OUTPUT, _b("submitted zero-fill bytes:%d"), zfill);
        }

        /* ...update the write pointer */
        UPDATE_WPTR(payload, payload);

        /* ...process buffer start-up */
        if (d->state & XA_RENDERER_FLAG_IDLE)
        {
            /* ...start-up transmission if FIFO gets at least 2 frames */
            //if (avail <= (HW_FIFO_LENGTH - (2 * payload)))
            if (avail == 0)
            {
                __xf_timer_start(&rend_timer,__xf_timer_ratio_to_period((d->frame_size_bytes / d->sample_size ),
                            d->rate));
                d->state ^= XA_RENDERER_FLAG_IDLE | XA_RENDERER_FLAG_RUNNING;

                /* ...write one frame worth data written to FIFO to output file */
                READ_FIFO(payload);

                TRACE(OUTPUT, _b("FIFO/timer started after buffer full:IDLE->RUNNING"));
            }
        }
        else
        {
            /* ...write one frame worth data written to FIFO to output file */
            READ_FIFO(payload);

        }

        /* ...declare exec done on input over and if no more valid data is available */
        d->exec_done = (d->input_over && (bytes_write == 0));

        if(d->exec_done) 
        {
            /* ... stop interrupts as soon as exec is done */
            __xf_timer_stop(&rend_timer);
            d->state ^= XA_RENDERER_FLAG_RUNNING | XA_RENDERER_FLAG_IDLE;
        
            TRACE(OUTPUT, _b("exec done, timer stopped"));
        }
    }

    return k;
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

static XA_ERRORCODE xa_fw_renderer_init (XARenderer *d)
{
   d->consumed = 0;
   d->fw = NULL;

   /*opening the output file*/
   d->fw = fopen(grenderer_out_file,"wb");
   if ( d->fw == NULL )
   {
     /*file open failed*/
     return XA_FATAL_ERROR;
   }
   /*initially FIFO will be empty so fifo_avail is 2x framesize bytes for ping and pong */
   d->fifo_avail = d->frame_size_bytes * d->channels * 2;

   /* ...make sure that the frame_size_bytes is within the FIFO length */
   XF_CHK_ERR(d->fifo_avail <= HW_FIFO_LENGTH, XA_RENDERER_CONFIG_NONFATAL_RANGE);

   /* ...initialize FIFO params, zero fill FIFO and init pointers to start of FIFO */
   d->pfifo_w = d->pfifo_r = (void *)g_fifo_renderer;
   memset(d->pfifo_w, 0, d->fifo_avail);
   
   /*initialises the timer ;timer0 is used as system timer*/
   __xf_timer_init(&rend_timer, xa_fw_handler, d, 1);

#ifdef XA_INPORT_BYPASS_TEST
    /* ...enabled at init for testing. To be enabled by set-config to the plugin. */
    d->inport_bypass = 1;
#endif

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
        /* ...set default renderer parameters - 16-bit little-endian stereo @ 48KHz */        
        d->channels = 2;
        d->pcm_width = 16;
        d->rate = 48000;
        d->sample_size = ( d->pcm_width >> 3 ); /* convert bits to bytes */ 
        d->frame_size_bytes = MAX_FRAME_SIZE_IN_BYTES_DEFAULT; 
        d->frame_size = MAX_FRAME_SIZE_IN_BYTES_DEFAULT/d->sample_size; 
        
        /* ...and mark renderer has been created */
        d->state = XA_RENDERER_FLAG_PREINIT_DONE;
        return XA_NO_ERROR;
    }
    case XA_CMD_TYPE_INIT_API_POST_CONFIG_PARAMS:
    {
        /* ...post-configuration initialization (all parameters are set) */
        XF_CHK_ERR(d->state & XA_RENDERER_FLAG_PREINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

        XF_CHK_ERR(xa_fw_renderer_init(d) == 0, XA_RENDERER_CONFIG_FATAL_HW);

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

/* ...HW-renderer control function */
static inline XA_ERRORCODE xa_hw_renderer_control(XARenderer *d, UWORD32 state)
{
    switch (state)
    {

    case XA_RENDERER_STATE_START:
        /* ...process buffer start-up, on trigger from application */
        if ((d->state & XA_RENDERER_FLAG_IDLE))
        {
            UWORD32 payload = d->frame_size_bytes * d->channels;

            /* ...start the FIFO from the pong buffer, hence adjust the read pointer and make it follow write pointer */
            d->pfifo_r = d->pfifo_w;

            /* ...write one frame worth data written to FIFO to output file */
            READ_FIFO(payload);

            /* ...to always start with full FIFO worth data */
            d->fifo_avail = 0;

            /* ...start-up transmission with zero filled FIFO */
            __xf_timer_start(&rend_timer, __xf_timer_ratio_to_period((d->frame_size_bytes / d->sample_size ), d->rate));

            /* ...change state to Running */
            d->state ^= (XA_RENDERER_FLAG_IDLE | XA_RENDERER_FLAG_RUNNING);
            
            TRACE(INIT, _b("FIFO/timer started, state:IDLE to RUNNING, fifo_avail:%d"), d->fifo_avail);
        }
        else
        {
            TRACE(INIT, _b("no change in state:RUNNING"));
        }
        return XA_NO_ERROR;

    case XA_RENDERER_STATE_RUN:
        /* ...renderer must be in paused state */
        XF_CHK_ERR(d->state & XA_RENDERER_FLAG_PAUSED, XA_RENDERER_EXEC_NONFATAL_STATE);
        /* ...mark renderer is running */
        d->state ^= XA_RENDERER_FLAG_RUNNING | XA_RENDERER_FLAG_PAUSED;
        return XA_NO_ERROR;

    case XA_RENDERER_STATE_PAUSE:
        /* ...renderer must be in running state */
        XF_CHK_ERR(d->state & XA_RENDERER_FLAG_RUNNING, XA_RENDERER_EXEC_NONFATAL_STATE);
        /* ...pause renderer operation */
        //xa_hw_renderer_pause(d);
        /* ...mark renderer is paused */
        d->state ^= XA_RENDERER_FLAG_RUNNING | XA_RENDERER_FLAG_PAUSED;
        return XA_NO_ERROR;

    case XA_RENDERER_STATE_IDLE:
        /* ...command is valid in any active state; stop renderer operation */
        xa_fw_renderer_close(d);

        /* ...reset renderer flags */
        d->state &= ~(XA_RENDERER_FLAG_RUNNING | XA_RENDERER_FLAG_PAUSED);
        return XA_NO_ERROR;

    default:
        /* ...unrecognized command */
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
        /* ...check value is permitted (16 bits only) */
        XF_CHK_ERR(i_value == 16, XA_RENDERER_CONFIG_NONFATAL_RANGE);
        /* ...apply setting */
        d->pcm_width = i_value;
        d->sample_size = ( d->pcm_width >> 3 ); /* convert bits to bytes */ 

        /* ...update internal variable frame_size_bytes */
        d->frame_size_bytes = d->frame_size * d->sample_size;
        return XA_NO_ERROR;

    case XA_RENDERER_CONFIG_PARAM_CHANNELS:
        /* ...command is valid only in configuration state */
        XF_CHK_ERR((d->state & XA_RENDERER_FLAG_POSTINIT_DONE) == 0, XA_RENDERER_CONFIG_FATAL_STATE);
        /* ...get requested channel number */
        i_value = (UWORD32) *(WORD32 *)pv_value;
        /* ...allow stereo only */
        XF_CHK_ERR((i_value == 2) || (i_value == 1), XA_RENDERER_CONFIG_NONFATAL_RANGE);
        /* ...apply setting */
        d->channels = (UWORD32)i_value;
        return XA_NO_ERROR;

    case XA_RENDERER_CONFIG_PARAM_SAMPLE_RATE:
        /* ...command is valid only in configuration state */
        XF_CHK_ERR((d->state & XA_RENDERER_FLAG_POSTINIT_DONE) == 0, XA_RENDERER_CONFIG_FATAL_STATE);
        /* ...get requested sampling rate */
        i_value = (UWORD32) *(WORD32 *)pv_value;
        
        /* ...allow 16 , 44.1 or 48KHz only  */
        XF_CHK_ERR(i_value == 16000 || i_value == 44100 || i_value == 48000, XA_RENDERER_CONFIG_NONFATAL_RANGE);
        /* ...apply setting */
        d->rate = (UWORD32)i_value;
        return XA_NO_ERROR;

    case XA_RENDERER_CONFIG_PARAM_FRAME_SIZE:
        /* ...command is valid only in configuration state */
        XF_CHK_ERR((d->state & XA_RENDERER_FLAG_POSTINIT_DONE) == 0, XA_RENDERER_CONFIG_FATAL_STATE);

        /* ...check it is valid framesize or not */
        XF_CHK_ERR( ( ( *(WORD32 *)pv_value >= MIN_FRAME_SIZE_IN_BYTES) && ( *(WORD32 *)pv_value <= MAX_FRAME_SIZE_IN_BYTES_DEFAULT ) ), XA_RENDERER_CONFIG_NONFATAL_RANGE);
        
        /* ...check frame_size_bytes is multiple of 4 or not */
        XF_CHK_ERR( ( (*(WORD32 *)pv_value & 0x3) == 0 ), XA_RENDERER_CONFIG_NONFATAL_RANGE);    
        
        /* ...get requested frame size */
        d->frame_size_bytes = (UWORD32) *(WORD32 *)pv_value;        

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

    case XA_RENDERER_CONFIG_PARAM_FRAME_SIZE_IN_SAMPLES:
        {
            WORD32 frame_size_bytes = *(WORD32 *)pv_value * d->sample_size;

            /* ...command is valid only in configuration state */
            XF_CHK_ERR((d->state & XA_RENDERER_FLAG_POSTINIT_DONE) == 0, XA_RENDERER_CONFIG_FATAL_STATE);
            
            /* ...check it is valid framesize or not */
            XF_CHK_ERR( ( ( frame_size_bytes >= MIN_FRAME_SIZE_IN_BYTES) && ( frame_size_bytes <= MAX_FRAME_SIZE_IN_BYTES_DEFAULT ) ), XA_RENDERER_CONFIG_NONFATAL_RANGE);
            
            /* ...check frame_size_bytes is multiple of 4 or not */
            XF_CHK_ERR( ( (frame_size_bytes & 0x3) == 0 ), XA_RENDERER_CONFIG_NONFATAL_RANGE);    
            
            /* ...get requested frame size */
            d->frame_size  = (UWORD32) *(WORD32 *)pv_value;

            /* ...update internal variable frame_size_bytes */
            d->frame_size_bytes = d->frame_size * d->sample_size;
            
            TRACE(INIT, _b("frame_size:%d"), d->frame_size);
            
            return XA_NO_ERROR;
        }
    default:
        /* ...unrecognized parameter */
        return XF_CHK_ERR(0, XA_API_FATAL_INVALID_CMD_TYPE);
    }
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

    case XA_RENDERER_CONFIG_PARAM_FRAME_SIZE: /* ...deprecated */
        /* ...return current audio frame length (in bytes) */
        *(WORD32 *)pv_value = d->frame_size_bytes;
        return XA_NO_ERROR;

    case XA_RENDERER_CONFIG_PARAM_STATE:
        /* ...return current execution state */
        *(WORD32 *)pv_value = xa_hw_renderer_get_state(d);
        return XA_NO_ERROR;

    case XA_RENDERER_CONFIG_PARAM_BYTES_PRODUCED:
        /* ...return current execution state */
        *(UWORD32 *)pv_value = (UWORD32)(d->cumulative_bytes_produced > MAX_UWORD32 ? MAX_UWORD32 : d->cumulative_bytes_produced) ;
        return XA_NO_ERROR;

#ifdef XA_EXT_CONFIG_TEST
    case XA_RENDERER_CONFIG_PARAM_FRAME_SIZE_IN_SAMPLES:
    {
        /* ...return current audio frame length (in samples) */
        xaf_ext_buffer_t *ext_buf = (xaf_ext_buffer_t *) pv_value;
        memcpy(ext_buf->data, &d->frame_size, sizeof(d->frame_size));
        ext_buf->valid_data_size = sizeof(d->frame_size);
        return XA_NO_ERROR;
    }
#else
    case XA_RENDERER_CONFIG_PARAM_FRAME_SIZE_IN_SAMPLES:
        /* ...return current audio frame length (in samples) */
        *(WORD32 *)pv_value = d->frame_size;
        return XA_NO_ERROR;
#endif

    default:
        /* ...unrecognized parameter */
        return XF_CHK_ERR(0, XA_API_FATAL_INVALID_CMD_TYPE);
    }
}

static XA_ERRORCODE xa_renderer_do_exec(XARenderer *d)
{
    d->consumed = xa_fw_renderer_submit(d, d->input, d->submited_inbytes);

    d->cumulative_bytes_produced += d->consumed;

    return XA_NO_ERROR;
}

/* ...execution command */
static XA_ERRORCODE xa_renderer_execute(XARenderer *d, WORD32 i_idx, pVOID pv_value)
{
    XA_ERRORCODE ret;

    /* ...sanity check - pointer must be valid */
    XF_CHK_ERR(d, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...renderer must be in running state */
    XF_CHK_ERR(d->state & (XA_RENDERER_FLAG_RUNNING | XA_RENDERER_FLAG_IDLE), XA_RENDERER_EXEC_FATAL_STATE);

    /* ...process individual command type */
    switch (i_idx)
    {
    case XA_CMD_TYPE_DO_EXECUTE:
        ret = xa_renderer_do_exec(d);
        return ret;

    case XA_CMD_TYPE_DONE_QUERY:
        XF_CHK_ERR(pv_value, XA_API_FATAL_INVALID_CMD_TYPE);
        
        *(UWORD32 *)pv_value = d->exec_done;

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
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...make sure it is an input port  */
    XF_CHK_ERR(i_idx == 0, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...renderer must be initialized */
    XF_CHK_ERR(d->state & XA_RENDERER_FLAG_POSTINIT_DONE, XA_RENDERER_EXEC_FATAL_STATE);

    /* ...input buffer pointer must be valid */
    XF_CHK_ERR(d->input, XA_API_FATAL_INVALID_CMD_TYPE);

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
    
    //XF_CHK_ERR(d->state & XA_RENDERER_FLAG_RUNNING, XA_API_FATAL_INVALID_CMD_TYPE);
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
        if(d->inport_bypass)
        {
            /* ...input buffer length 0 enabling input bypass mode */
            i_value = 0;
        }
        else
        {
            /* ...input buffer specification; accept exact audio frame */
            i_value = d->frame_size_bytes * d->channels;
        }
        break;

    case 1:
        /* ...output buffer specification; accept exact audio frame */
        i_value = d->frame_size_bytes * d->channels;
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
    /* ...basic sanity check */
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
    XA_ERRORCODE Rend_ret = 0;
    XARenderer *renderer = (XARenderer *) p_xa_module_obj;
#ifdef XAF_PROFILE
    clk_t comp_start, comp_stop;
#endif
    /* ...check if command index is sane */
    XF_CHK_ERR(i_cmd < XA_RENDERER_API_COMMANDS_NUM, XA_API_FATAL_INVALID_CMD);

    /* ...see if command is defined */
    XF_CHK_ERR(xa_renderer_api[i_cmd], XA_API_FATAL_INVALID_CMD);

    /* ...execute requested command */
#ifdef XAF_PROFILE
     if(XA_API_CMD_INIT != i_cmd)
     {
         comp_start = clk_read_start(CLK_SELN_THREAD);
     }
#endif
  
    Rend_ret = xa_renderer_api[i_cmd](renderer, i_idx, pv_value);
#ifdef XAF_PROFILE
    if(XA_API_CMD_INIT != i_cmd)
     {
        comp_stop = clk_read_stop(CLK_SELN_THREAD);
        renderer_cycles += clk_diff(comp_stop, comp_start);
     }
#endif
    return Rend_ret;
}
