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
 * xa-capturer.c
 *
 * dummy (dumping data to file)capturer implementation
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

#define MODULE_TAG                      CAPTURER

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "osal-timer.h"
#include <stdio.h>
#include "audio/xa-capturer-api.h"
#include "xf-debug.h"
#include "xaf-api.h"
#include <string.h>
#ifdef XAF_PROFILE
#include "xaf-clk-test.h"
extern clk_t capturer_cycles;
#endif
#include "xaf-fio-test.h"

/*******************************************************************************
 * Codec parameters
 ******************************************************************************/

/* ...total length of HW FIFO in bytes */
#define HW_FIFO_LENGTH                  (8192)

/* maximum allowed framesize in bytes per channel. Default framesize: 8ms frame at 48 kHz with 2 bytes per sample */
#define MAX_FRAME_SIZE_IN_BYTES_DEFAULT    (48 * 8 * 2)

/* minimum allowed framesize in bytes per channel */
#define MIN_FRAME_SIZE_IN_BYTES    ( 128 )


/*******************************************************************************
 * Local data definition
 ******************************************************************************/

typedef struct XACapturer
{
    /***************************************************************************
     * Internal stuff
     **************************************************************************/

    /* ...component state */
    UWORD32                     state;

    /* ...notification callback pointer */
    xa_capturer_cb_t       *cdata;

    /* ...input buffer pointer */
    void                   *output;
    
    /* ...estimation of amount of samples that can be written into FIFO */
    UWORD32                     fifo_avail;

    UWORD32                   capturer_eof;

    /* ...number of samples produced */
    UWORD32                     produced;
    /* ...number of bytes copied in fifo*/
    //UWORD32                     submited;
    /***************************************************************************
     * Run-time data
     **************************************************************************/

    /* ...size of PCM sample in bytes  */
    UWORD32                     sample_size;

    /* ...number of channels */
    UWORD32                     channels;

    /* ...sample width in bits */
    UWORD32                     pcm_width;
    
    /* ...framesize in bytes per channel */
    UWORD32                     frame_size_bytes;    

    /* ...current sampling rate */
    UWORD32                     rate;

    UWORD32             over_flow_cnt;

    UWORD32            over_flow_flag;

    FILE * fw;

    UWORD32        interrupt_cnt;

    /*total bytes produced by the component*/
    UWORD64              tot_bytes_produced;

    /*total bytes to be produced*/
    UWORD64            bytes_end;

    /* ...framesize in samples per channel */
    UWORD32     frame_size;

}   XACapturer;

#define MAX_UWORD32 ((UWORD64)0xFFFFFFFF)
/*******************************************************************************
 * Operating flags
 ******************************************************************************/

#define XA_CAPTURER_FLAG_PREINIT_DONE   (1 << 0)
#define XA_CAPTURER_FLAG_POSTINIT_DONE  (1 << 1)
#define XA_CAPTURER_FLAG_IDLE           (1 << 2)
#define XA_CAPTURER_FLAG_RUNNING        (1 << 3)
#define XA_CAPTURER_FLAG_PAUSED         (1 << 4)
/*******************************************************************************
 * global variables
 ******************************************************************************/

static xf_timer_t cap_timer;
const char capturer_in_file[] = "capturer_in.pcm";
void xa_capturer_callback(xa_capturer_cb_t *cdata, WORD32 i_idx);

static void xa_fw_handler(void *arg)
{
    XACapturer *d = arg;

    d->fifo_avail = d->fifo_avail + (d->frame_size_bytes*d->channels);

    if((d->fifo_avail) > (2 * d->frame_size_bytes * d->channels))
    {/*over run case*/
       d->fifo_avail = 0;
    }
    else if(((int)d-> fifo_avail) < 0)
    {/* under run */
       d->fifo_avail=0;
    }
    else
    {
       d->cdata->cb(d->cdata, 0);

    }
}


/*******************************************************************************
 * Codec access functions
 ******************************************************************************/

static inline void xa_fw_capturer_close(XACapturer *d)
{
    fclose(d->fw);
    __xf_timer_stop(&cap_timer);
    __xf_timer_destroy(&cap_timer);
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

static XA_ERRORCODE xa_fw_capturer_init (XACapturer *d)
{
   d->produced = 0;
   d->fw = NULL;
   d->tot_bytes_produced = 0;
   /*opening the output file*/
   d->fw = fopen(capturer_in_file,"rb");
   if ( d->fw == NULL )
   {
     /*file open failed*/
     return XA_FATAL_ERROR;
   }

   /*initialy FIFO will be empty so fifo_avail = HW_FIFO_LENGTH_SAMPLES*/
   d->fifo_avail = 0;

   /*initialises the timer */
   __xf_timer_init(&cap_timer, xa_fw_handler, d, 1);

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

        /* ...set default capturer parameters - 16-bit little-endian stereo @ 48KHz */        
        d->channels = 2;
        d->pcm_width = 16;
        d->rate = 48000;
        d->sample_size = ( d->pcm_width >> 3 ); /* convert bits to bytes */ 
        d->frame_size_bytes = MAX_FRAME_SIZE_IN_BYTES_DEFAULT; 
        d->frame_size = MAX_FRAME_SIZE_IN_BYTES_DEFAULT/d->sample_size; 

        /* ...and mark capturer has been created */
        d->state = XA_CAPTURER_FLAG_PREINIT_DONE;

        return XA_NO_ERROR;
    }

    case XA_CMD_TYPE_INIT_API_POST_CONFIG_PARAMS:
    {
        /* ...post-configuration initialization (all parameters are set) */

        XF_CHK_ERR(d->state & XA_CAPTURER_FLAG_PREINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

        XF_CHK_ERR(xa_fw_capturer_init(d) == 0, XA_CAPTURER_CONFIG_FATAL_HW);

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

        return XA_NO_ERROR;
    }

    case XA_CMD_TYPE_INIT_DONE_QUERY:
    {
        /* ...check if initialization is done; make sure pointer is sane */
        XF_CHK_ERR(pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

        /* ...put current status */
        *(WORD32 *)pv_value = (d->state & XA_CAPTURER_FLAG_IDLE ? 1 : 0);

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
        XF_CHK_ERR(d->state & XA_CAPTURER_FLAG_IDLE, XA_CAPTURER_EXEC_NONFATAL_STATE);

        __xf_timer_start(&cap_timer,
                         __xf_timer_ratio_to_period((d->frame_size_bytes / d->sample_size ), d->rate));  

        /* ...mark capturer is runnning */
        d->state ^= XA_CAPTURER_FLAG_IDLE | XA_CAPTURER_FLAG_RUNNING;

        return XA_NO_ERROR;

    case XA_CAPTURER_STATE_RUN:
        /* ...capturer must be in paused state */
        XF_CHK_ERR(d->state & XA_CAPTURER_FLAG_PAUSED, XA_CAPTURER_EXEC_NONFATAL_STATE);

        /* ...resume capturer operation */
        //xa_hw_capturer_resume(d);

        /* ...mark capturer is running */
        d->state ^= XA_CAPTURER_FLAG_RUNNING | XA_CAPTURER_FLAG_PAUSED;

        return XA_NO_ERROR;

    case XA_CAPTURER_STATE_PAUSE:
        /* ...capturer must be in running state */
        XF_CHK_ERR(d->state & XA_CAPTURER_FLAG_RUNNING, XA_CAPTURER_EXEC_NONFATAL_STATE);

        /* ...pause capturer operation */


        /* ...mark capturer is paused */
        d->state ^= XA_CAPTURER_FLAG_RUNNING | XA_CAPTURER_FLAG_PAUSED;

        return XA_NO_ERROR;

    case XA_CAPTURER_STATE_IDLE:
        /* ...command is valid in any active state; stop capturer operation */
        
        xa_fw_capturer_close(d);

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
    UWORD32     i_value;

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
        i_value = (UWORD32) *(WORD32 *)pv_value;

        /* ...check value is permitted (16 bits only) */
        XF_CHK_ERR(i_value == 16, XA_CAPTURER_CONFIG_NONFATAL_RANGE);

        /* ...apply setting */
        d->pcm_width = i_value;
        d->sample_size = ( d->pcm_width >> 3 ); /* convert bits to bytes */ 

        /* ...update internal variable frame_size_bytes */
        d->frame_size_bytes = d->frame_size * d->sample_size;
            
        return XA_NO_ERROR;

    case XA_CAPTURER_CONFIG_PARAM_CHANNELS:
        /* ...command is valid only in configuration state */
        XF_CHK_ERR((d->state & XA_CAPTURER_FLAG_POSTINIT_DONE) == 0, XA_CAPTURER_CONFIG_FATAL_STATE);

        /* ...get requested channel number */
        i_value = (UWORD32) *(WORD32 *)pv_value;

        /* ...allow mono or stereo only */
        XF_CHK_ERR( ( i_value == 2 ) || ( i_value == 1 ) , XA_CAPTURER_CONFIG_NONFATAL_RANGE);

        /* ...apply setting */
        d->channels = (UWORD32)i_value;

        return XA_NO_ERROR;

    case XA_CAPTURER_CONFIG_PARAM_SAMPLE_RATE:
        /* ...command is valid only in configuration state */
        XF_CHK_ERR((d->state & XA_CAPTURER_FLAG_POSTINIT_DONE) == 0, XA_CAPTURER_CONFIG_FATAL_STATE);

        /* ...get requested sampling rate */
        i_value = (UWORD32) *(WORD32 *)pv_value;

        /* ...allow 16 , 44.1 or 48KHz only  */
        XF_CHK_ERR(i_value == 16000 || i_value == 44100 || i_value == 48000, XA_CAPTURER_CONFIG_NONFATAL_RANGE);

        /* ...apply setting */
        d->rate = (UWORD32)i_value;

        return XA_NO_ERROR;

    case XA_CAPTURER_CONFIG_PARAM_FRAME_SIZE:
        /* ...command is valid only in configuration state */
        XF_CHK_ERR((d->state & XA_CAPTURER_FLAG_POSTINIT_DONE) == 0, XA_CAPTURER_CONFIG_FATAL_STATE);

        /* ...check it is valid framesize or not */
        XF_CHK_ERR( ( (*(WORD32 *)pv_value >= MIN_FRAME_SIZE_IN_BYTES) && ( *(WORD32 *)pv_value <= (HW_FIFO_LENGTH/4)) ), XA_CAPTURER_CONFIG_NONFATAL_RANGE);
        
        /* ...check frame_size_bytes fits within the FIFO buffer */
        XF_CHK_ERR( ( (*(WORD32 *)pv_value * d->channels * 2) <= HW_FIFO_LENGTH), XA_CAPTURER_CONFIG_NONFATAL_RANGE);        

        /* ...check frame_size_bytes is multiple of 4 or not */
        XF_CHK_ERR( ( (*(WORD32 *)pv_value & 0x3) == 0 ), XA_CAPTURER_CONFIG_NONFATAL_RANGE);        

        /* ...get requested frame size */
        d->frame_size_bytes = (UWORD32) *(WORD32 *)pv_value;

        TRACE(INIT, _b("frame_size_bytes:%d"), d->frame_size_bytes);
        
        return XA_NO_ERROR;
    case XA_CAPTURER_CONFIG_PARAM_SAMPLE_END:
        /* ...command is valid only in configuration state */
        XF_CHK_ERR((d->state & XA_CAPTURER_FLAG_POSTINIT_DONE) == 0, XA_CAPTURER_CONFIG_FATAL_STATE);

        /* ...check it is equal to the only frame size we support */
        XF_CHK_ERR((*(UWORD32 *)pv_value >= 0), XA_CAPTURER_CONFIG_NONFATAL_RANGE);
        
        /* ...get requested frame size */
        d->bytes_end = ((UWORD64) *(UWORD32 *)pv_value)* (d->sample_size) * (d->channels) ;

        TRACE(INIT, _b("bytes requested:%lld"), d->bytes_end);

        return XA_NO_ERROR;

    case XA_CAPTURER_CONFIG_PARAM_CB:
        /* ...set opaque callback data function */
        d->cdata = (xa_capturer_cb_t *)pv_value;

        return XA_NO_ERROR;

    case XA_CAPTURER_CONFIG_PARAM_STATE:
        /* ...runtime state control parameter valid only in execution state */
        XF_CHK_ERR(d->state & XA_CAPTURER_FLAG_POSTINIT_DONE, XA_CAPTURER_CONFIG_FATAL_STATE);

        /* ...get requested state */
        i_value = (UWORD32) *(WORD32 *)pv_value;

        /* ...pass to state control hook */
        return xa_hw_capturer_control(d, i_value);

    case XA_CAPTURER_CONFIG_PARAM_FRAME_SIZE_IN_SAMPLES:
        {
            WORD32 frame_size_bytes = *(WORD32 *)pv_value * d->sample_size;

            /* ...command is valid only in configuration state */
            XF_CHK_ERR((d->state & XA_CAPTURER_FLAG_POSTINIT_DONE) == 0, XA_CAPTURER_CONFIG_FATAL_STATE);
            
            /* ...check it is valid framesize or not */
            XF_CHK_ERR( ( (frame_size_bytes >= MIN_FRAME_SIZE_IN_BYTES) && ( frame_size_bytes <= (HW_FIFO_LENGTH/4)) ), XA_CAPTURER_CONFIG_NONFATAL_RANGE);
            
            /* ...check frame_size_bytes fits within the FIFO buffer */
            XF_CHK_ERR( ( (frame_size_bytes * d->channels * 2) <= HW_FIFO_LENGTH), XA_CAPTURER_CONFIG_NONFATAL_RANGE);        
            
            /* ...check frame_size_bytes is multiple of 4 or not */
            XF_CHK_ERR( ( (frame_size_bytes & 0x3) == 0 ), XA_CAPTURER_CONFIG_NONFATAL_RANGE);        
            
            /* ...get requested frame size */
            d->frame_size = (UWORD32) *(WORD32 *)pv_value;

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

    case XA_CAPTURER_CONFIG_PARAM_FRAME_SIZE: /* ...deprecated */
        /* ...return current audio frame length (in bytes) */
        *(WORD32 *)pv_value = d->frame_size_bytes;
        return XA_NO_ERROR;

#ifdef XA_EXT_CONFIG_TEST
    case XA_CAPTURER_CONFIG_PARAM_STATE:
    {
        /* ...return current execution state */
        UWORD32 state = xa_hw_capturer_get_state(d);
        xaf_ext_buffer_t *ext_buf = (xaf_ext_buffer_t *) pv_value;
        memcpy(ext_buf->data, &state, sizeof(state));
        ext_buf->valid_data_size = sizeof(state);
        return XA_NO_ERROR;
    }
#else
    case XA_CAPTURER_CONFIG_PARAM_STATE:
        /* ...return current execution state */
        *(WORD32 *)pv_value = xa_hw_capturer_get_state(d);
        return XA_NO_ERROR;
#endif

    case XA_CAPTURER_CONFIG_PARAM_BYTES_PRODUCED:
        /* ...return current execution state */
        *(UWORD32 *)pv_value = (UWORD32)(d->tot_bytes_produced > MAX_UWORD32 ? MAX_UWORD32 : d->tot_bytes_produced) ;
        return XA_NO_ERROR;

    case XA_CAPTURER_CONFIG_PARAM_FRAME_SIZE_IN_SAMPLES:
        /* ...return current audio frame length (in samples) */
        *(WORD32 *)pv_value = d->frame_size;
        return XA_NO_ERROR;

    default:
        /* ...unrecognized parameter */
        return XF_CHK_ERR(0, XA_API_FATAL_INVALID_CMD_TYPE);
    }
}

static XA_ERRORCODE xa_capturer_do_exec(XACapturer *d)
{
    WORD32 bytes_read = 0;
    static UWORD32 frame_cnt = 0;

    FIO_PRINTF(stdout,"%d\n",++frame_cnt);
    if(d->fifo_avail >= (d->frame_size_bytes * d->channels))
    {
        d->fifo_avail = d->fifo_avail - (d->frame_size_bytes * d->channels);

        if(d->output)
        {
            /* ... read bytes when output buffer is available. */
            bytes_read = fread(d->output,1,(d->frame_size_bytes * d->channels ), d->fw );
        }
        else
        {
            /* ... skip bytes when output buffer is unavailable. TENA-2528 */
            fseek(d->fw, (d->frame_size_bytes * d->channels), SEEK_CUR);

            TRACE(OUTPUT, _b("output buffer is NULL, dropped %u bytes"), (d->frame_size_bytes * d->channels));

            /* ... return no-data produced non-fatal error */
            return XA_CAPTURER_EXEC_NONFATAL_NO_DATA;
        }

        d->produced = bytes_read;
        d->tot_bytes_produced = d->tot_bytes_produced + d->produced;
        if(d->bytes_end!=0)
        {
            if( d->tot_bytes_produced > d->bytes_end )
            {
                if(d->produced > ( d->tot_bytes_produced - d->bytes_end ))
                {
                    d->produced = d->produced - ( d->tot_bytes_produced - d->bytes_end );
                }
                else
                {
                    d->produced = 0;
                }
            }
        }

    }
    else
    {
        /*overrun happened*/
        d->produced = 0;

    }

    return XA_NO_ERROR;
}

/* ...execution command */
static XA_ERRORCODE xa_capturer_execute(XACapturer *d, WORD32 i_idx, pVOID pv_value)
{
    XA_ERRORCODE ret;

    /* ...sanity check - pointer must be valid */
    XF_CHK_ERR(d, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...capturer must be in running state */
    XF_CHK_ERR(d->state & (XA_CAPTURER_FLAG_RUNNING | XA_CAPTURER_FLAG_IDLE), XA_CAPTURER_EXEC_FATAL_STATE);

    /* ...process individual command type */
    switch (i_idx)
    {
    case XA_CMD_TYPE_DO_EXECUTE:
        ret = xa_capturer_do_exec(d);
        return XA_NO_ERROR;

    case XA_CMD_TYPE_DONE_QUERY:
        /* ...always report "no" - tbd - is that needed at all? */
        XF_CHK_ERR(pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

        if((d->produced == 0) 
            && (d->output) /* TENA-2528 */
        )
        {
            __xf_timer_stop(&cap_timer);

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

#if 1 /* ... skip output buffer check if not available. TENA-2528 */
#else
    /* ...output buffer must exist */
    XF_CHK_ERR(d->output, XA_CAPTURER_EXEC_FATAL_INPUT);
#endif

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
    UWORD32     i_value;

    /* ...basic sanity check */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...command valid only after post-initialization step */
    XF_CHK_ERR(d->state & XA_CAPTURER_FLAG_POSTINIT_DONE, XA_CAPTURER_CONFIG_FATAL_STATE);

    switch (i_idx)
    {
    case 0:
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
    XF_CHK_ERR(d, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...codec must be initialized */
    XF_CHK_ERR(d->state & XA_CAPTURER_FLAG_POSTINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

    TRACE(INIT, _b("xa_capturer_set_mem_ptr[%u]: %p"), i_idx, pv_value);

    /* ...select memory buffer */
    switch (i_idx)
    {
    case 0:
        /* ...output buffer */
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
static XA_ERRORCODE (* const xa_capturer_api[])(XACapturer *, WORD32, pVOID) =
{
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
#define XA_CAPTURER_API_COMMANDS_NUM   (sizeof(xa_capturer_api) / sizeof(xa_capturer_api[0]))

/*******************************************************************************
 * API entry point
 ******************************************************************************/

XA_ERRORCODE xa_capturer(xa_codec_handle_t p_xa_module_obj, WORD32 i_cmd, WORD32 i_idx, pVOID pv_value)
{
    XA_ERRORCODE sh_test = 0;
    XACapturer *capturer = (XACapturer *) p_xa_module_obj;
#ifdef XAF_PROFILE
    clk_t comp_start, comp_stop;
#endif
    /* ...check if command index is sane */
    XF_CHK_ERR(i_cmd < XA_CAPTURER_API_COMMANDS_NUM, XA_API_FATAL_INVALID_CMD);

    /* ...see if command is defined */
    XF_CHK_ERR(xa_capturer_api[i_cmd], XA_API_FATAL_INVALID_CMD);

    /* ...execute requested command */
#ifdef XAF_PROFILE
    comp_start = clk_read_start(CLK_SELN_THREAD);
#endif
    sh_test = xa_capturer_api[i_cmd](capturer, i_idx, pv_value);
#ifdef XAF_PROFILE
    comp_stop = clk_read_stop(CLK_SELN_THREAD);
    capturer_cycles += clk_diff(comp_stop, comp_start);
#endif
    return sh_test;
}
