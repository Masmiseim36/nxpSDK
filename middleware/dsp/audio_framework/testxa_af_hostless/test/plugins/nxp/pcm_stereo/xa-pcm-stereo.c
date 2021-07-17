/*******************************************************************************
* Copyright 2020 NXP
* Copyright (c) 2015-2019 Cadence Design Systems, Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining
* a copy of this software and associated documentation files (the
* "Software"), to use this Software with Cadence processor cores only and 
* not with any other processors and platforms, subject to
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

******************************************************************************/
/*******************************************************************************
 * xa-pcm-stereo.c
 *
 * Sample pcm mono to stereo plugin
 ******************************************************************************/

#define MODULE_TAG                      PCM_STEREO

#if XA_PCM_STEREO

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include <stdint.h>
#include <string.h>

/* ...debugging facility */
#include "osal-timer.h"
#include "xf-debug.h"
#include "audio/xa-pcm-stereo-api.h"

#ifdef XAF_PROFILE
#include "xaf-clk-test.h"
extern clk_t pcm_stereo_cycles;
#endif

/*******************************************************************************
 * Tracing configuration
 ******************************************************************************/

TRACE_TAG(INIT, 1);
TRACE_TAG(PROCESS, 1);

/*******************************************************************************
 * Internal functions definitions
 ******************************************************************************/

/* ...API structure */
typedef struct XAPcmStereo
{
    /* ...pcm stereo module state */
    UWORD32                 state;
    
    /* ...number of channels */
    UWORD32                 channels;
    
    /* ...PCM sample width */
    UWORD32                 pcm_width;

    /* ...sampling rate */
    UWORD32                 sample_rate;

    /* ...number of bytes in input buffer */
    UWORD32                 buffer_size;
    
    /* ...input buffer */
    void               *input;
    
    /* ...output buffer */
    void               *output;

    /* ...number of available bytes in the input buffer */
    UWORD32                 input_avail;    
    
    /* ...number of bytes consumed from input buffer */
    UWORD32                 consumed;    

    /* ...number of produced bytes */
    UWORD32                 produced;

}   XAPcmStereo;

/*******************************************************************************
 * Pcm stereo state flags
 ******************************************************************************/

#define XA_PCM_STEREO_FLAG_PREINIT_DONE      (1 << 0)
#define XA_PCM_STEREO_FLAG_POSTINIT_DONE     (1 << 1)
#define XA_PCM_STEREO_FLAG_RUNNING           (1 << 2)
#define XA_PCM_STEREO_FLAG_OUTPUT            (1 << 3)
#define XA_PCM_STEREO_FLAG_COMPLETE          (1 << 4)

/*******************************************************************************
 * DSP functions
 ******************************************************************************/

/* ...pcm stereo component pre-initialization (default parameters) */
static inline void xa_pcm_stereo_preinit(XAPcmStereo *d)
{
  
    /* ...pre-configuration initialization; reset internal data */
    memset(d, 0, sizeof(*d));
        
    /* ...set default parameters */
    d->channels = 2;
    d->pcm_width = 16;
    d->sample_rate = 48000;
    d->buffer_size = 512;
}

/* ...apply stereo to 16-bit PCM stream */
static XA_ERRORCODE xa_pcm_stereo_do_execute(XAPcmStereo *d)
{
    WORD32     i, j, nSize;
    UWORD8    *pIn = (UWORD8 *) d->input;
    UWORD8    *pOut = (UWORD8 *) d->output;
    UWORD32    filled = d->input_avail;
    WORD16     input;
    UWORD8     bytesPerChannel;

    bytesPerChannel = (d->pcm_width + 7U) / 8U;
    nSize = filled / bytesPerChannel;

    /* ...check I/O buffer */
    XF_CHK_ERR(d->input, XA_PCM_STEREO_EXEC_FATAL_INPUT);    
    XF_CHK_ERR(d->output, XA_PCM_STEREO_EXEC_FATAL_INPUT);

    /* ...Processing loop */
    for (i = 0; i < nSize; i++)
    {    
        for (j = 0; j < bytesPerChannel; j++)
        {
        	input = *pIn++;
        	pOut[j] = input;
        	pOut[j + bytesPerChannel] = input;
        }
    	pOut += (bytesPerChannel * 2);
    }

    /* ...save total number of consumed bytes */
    d->consumed = (UWORD32)((void *)pIn - d->input);

    /* ...save total number of produced bytes */
    d->produced = (UWORD32)((void *)pOut - d->output);

    /* ...put flag saying we have output buffer */
    d->state |= XA_PCM_STEREO_FLAG_OUTPUT;
    
    TRACE(PROCESS, _b("produced: %u bytes (%u samples)"), d->produced, nSize);

    /* ...return success result code */
    return XA_NO_ERROR;
}    

/* ...runtime reset */
static XA_ERRORCODE xa_pcm_stereo_do_runtime_init(XAPcmStereo *d)
{
    /* ...no special processing is needed here */
    return XA_NO_ERROR;
}

/*******************************************************************************
 * Commands processing
 ******************************************************************************/

/* ...codec API size query */
static XA_ERRORCODE xa_pcm_stereo_get_api_size(XAPcmStereo *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...check parameters are sane */
    XF_CHK_ERR(pv_value, XA_API_FATAL_INVALID_CMD_TYPE);
    
    /* ...retrieve API structure size */
    *(WORD32 *)pv_value = sizeof(*d);
    
    return XA_NO_ERROR;
}

/* ...standard codec initialization routine */
static XA_ERRORCODE xa_pcm_stereo_init(XAPcmStereo *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...sanity check - pcm stereo component must be valid */
    XF_CHK_ERR(d, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...process particular initialization type */
    switch (i_idx)
    {
    case XA_CMD_TYPE_INIT_API_PRE_CONFIG_PARAMS:
    {
        /* ...pre-configuration initialization; reset internal data */
        xa_pcm_stereo_preinit(d);

        /* ...and mark pcm stereo component has been created */
        d->state = XA_PCM_STEREO_FLAG_PREINIT_DONE;
        
        return XA_NO_ERROR;
    }
    
    case XA_CMD_TYPE_INIT_API_POST_CONFIG_PARAMS:
    {
        /* ...post-configuration initialization (all parameters are set) */
        XF_CHK_ERR(d->state & XA_PCM_STEREO_FLAG_PREINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);
      
        /* ...mark post-initialization is complete */
        d->state |= XA_PCM_STEREO_FLAG_POSTINIT_DONE;
        
        return XA_NO_ERROR;
    }
    
    case XA_CMD_TYPE_INIT_PROCESS:
    {
        /* ...kick run-time initialization process; make sure pcm stereo component is setup */
        XF_CHK_ERR(d->state & XA_PCM_STEREO_FLAG_POSTINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

        /* ...enter into execution stage */
        d->state |= XA_PCM_STEREO_FLAG_RUNNING;
        
        return XA_NO_ERROR;
    }
    
    case XA_CMD_TYPE_INIT_DONE_QUERY:
    {
        /* ...check if initialization is done; make sure pointer is sane */
        XF_CHK_ERR(pv_value, XA_API_FATAL_INVALID_CMD_TYPE);
        
        /* ...put current status */
        *(WORD32 *)pv_value = (d->state & XA_PCM_STEREO_FLAG_RUNNING ? 1 : 0);

        return XA_NO_ERROR;
    }
    
    default:
        /* ...unrecognised command type */
        TRACE(ERROR, _x("Unrecognised command type: %X"), i_idx);
        return XA_API_FATAL_INVALID_CMD_TYPE;
    }
}

/* ...set pcm stereo component configuration parameter */
static XA_ERRORCODE xa_pcm_stereo_set_config_param(XAPcmStereo *d, WORD32 i_idx, pVOID pv_value)
{
    UWORD32     i_value;
    
    /* ...sanity check - pcm stereo component pointer must be sane */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...pre-initialization must be completed, pcm stereo component must be idle */
    XF_CHK_ERR(d->state & XA_PCM_STEREO_FLAG_PREINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...get parameter value  */
    i_value = (UWORD32) *(WORD32 *)pv_value;
    
    /* ...process individual configuration parameter */
    switch (i_idx)
    {
    case XA_CODEC_CONFIG_PARAM_PCM_WIDTH:
    case XA_PCM_STEREO_CONFIG_PARAM_PCM_WIDTH:
        /* ...check value is permitted (<= 32 bits) */
        XF_CHK_ERR(i_value <= 32, XA_PCM_STEREO_CONFIG_NONFATAL_RANGE);
        d->pcm_width = (UWORD32)i_value;
        return XA_NO_ERROR;

    case XA_CODEC_CONFIG_PARAM_CHANNELS:
    case XA_PCM_STEREO_CONFIG_PARAM_CHANNELS:
        /* ...allow stereo output only */
        XF_CHK_ERR((i_value == 2), XA_PCM_STEREO_CONFIG_NONFATAL_RANGE);
        d->channels = (UWORD32)i_value;
        return XA_NO_ERROR;

    case XA_CODEC_CONFIG_PARAM_SAMPLE_RATE:
    case XA_PCM_STEREO_CONFIG_PARAM_SAMPLE_RATE:
        {
            /* ...set pcm stereo component sample rate */
            d->sample_rate = (UWORD32)i_value;
            return XA_NO_ERROR;
        }

    case XA_PCM_STEREO_CONFIG_PARAM_FRAME_SIZE:
        /* ...set pcm stereo component buffer size */
        d->buffer_size = (UWORD32)i_value;
        return XA_NO_ERROR;        
    
    default:
        TRACE(ERROR, _x("Invalid parameter: %X"), i_idx);
        return XA_API_FATAL_INVALID_CMD_TYPE;
    }
}

/* ...retrieve configuration parameter */
static XA_ERRORCODE xa_pcm_stereo_get_config_param(XAPcmStereo *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...sanity check - pcm stereo component must be initialized */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...make sure pre-initialization is completed */
    XF_CHK_ERR(d->state & XA_PCM_STEREO_FLAG_PREINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);
    
    /* ...process individual configuration parameter */
    switch (i_idx)
    {
    case XA_CODEC_CONFIG_PARAM_SAMPLE_RATE:
    case XA_PCM_STEREO_CONFIG_PARAM_SAMPLE_RATE:
        /* ...return sample rate */
        *(WORD32 *)pv_value = d->sample_rate;
        return XA_NO_ERROR;

    case XA_CODEC_CONFIG_PARAM_PCM_WIDTH:
    case XA_PCM_STEREO_CONFIG_PARAM_PCM_WIDTH:
        /* ...return current PCM width */
        *(WORD32 *)pv_value = d->pcm_width;
        return XA_NO_ERROR;

    case XA_CODEC_CONFIG_PARAM_CHANNELS:
    case XA_PCM_STEREO_CONFIG_PARAM_CHANNELS:
        /* ...return current channel number */
        *(WORD32 *)pv_value = d->channels;
        return XA_NO_ERROR;

    case XA_CODEC_CONFIG_PARAM_PRODUCED:
    case XA_PCM_STEREO_CONFIG_PARAM_PRODUCED:
        /* ...return no.of produced bytes */
        *(WORD32 *)pv_value = d->produced;
        return XA_NO_ERROR;        

    case XA_PCM_STEREO_CONFIG_PARAM_FRAME_SIZE:
        /* ...return pcm stereo component input buffer size */
        *(WORD32 *)pv_value = d->buffer_size * (i_idx + 1);
        return XA_NO_ERROR;        

    default:
        TRACE(ERROR, _x("Invalid parameter: %X"), i_idx);
        return XA_API_FATAL_INVALID_CMD_TYPE;
    }
}

/* ...execution command */
static XA_ERRORCODE xa_pcm_stereo_execute(XAPcmStereo *d, WORD32 i_idx, pVOID pv_value)
{
    XA_ERRORCODE ret;
#ifdef XAF_PROFILE
    clk_t comp_start, comp_stop;
#endif

    /* ...sanity check - pcm stereo component must be valid */
    XF_CHK_ERR(d, XA_API_FATAL_INVALID_CMD_TYPE);
    
    /* ...pcm stereo component must be in running state */
    XF_CHK_ERR(d->state & XA_PCM_STEREO_FLAG_RUNNING, XA_API_FATAL_INVALID_CMD_TYPE);
    
    /* ...process individual command type */
    switch (i_idx)
    {
    case XA_CMD_TYPE_DO_EXECUTE:
	/* ...perform conversion of mono to stereo */
#ifdef XAF_PROFILE
        comp_start = clk_read_start(CLK_SELN_THREAD);
#endif
        ret = xa_pcm_stereo_do_execute(d);
#ifdef XAF_PROFILE
        comp_stop = clk_read_stop(CLK_SELN_THREAD);
        pcm_stereo_cycles += clk_diff(comp_stop, comp_start);
#endif
        return ret;
        
    case XA_CMD_TYPE_DONE_QUERY:
        /* ...check if processing is complete */
        XF_CHK_ERR(pv_value, XA_API_FATAL_INVALID_CMD_TYPE);
        *(WORD32 *)pv_value = (d->state & XA_PCM_STEREO_FLAG_COMPLETE ? 1 : 0);
        return XA_NO_ERROR;
        
    case XA_CMD_TYPE_DO_RUNTIME_INIT:
        /* ...reset pcm stereo component operation */
        return xa_pcm_stereo_do_runtime_init(d);
        
    default:
        /* ...unrecognised command */
        TRACE(ERROR, _x("Invalid index: %X"), i_idx);
        return XA_API_FATAL_INVALID_CMD_TYPE;
    }
}

/* ...set number of input bytes */
static XA_ERRORCODE xa_pcm_stereo_set_input_bytes(XAPcmStereo *d, WORD32 i_idx, pVOID pv_value)
{
    UWORD32     size;
    
    /* ...sanity check - check parameters */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...track index must be valid */
    XF_CHK_ERR(i_idx == 0, XA_API_FATAL_INVALID_CMD_TYPE);
    
    /* ...pcm stereo component must be initialized */
    XF_CHK_ERR(d->state & XA_PCM_STEREO_FLAG_POSTINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);
    
    /* ...input buffer must exist */
    XF_CHK_ERR(d->input, XA_API_FATAL_INVALID_CMD_TYPE);
    
    /* ...input frame length should not be zero (in bytes) */
    XF_CHK_ERR((size = (UWORD32)*(WORD32 *)pv_value) >= 0, XA_PCM_STEREO_EXEC_NONFATAL_INPUT);

    /* ...all is correct; set input buffer length in bytes */
    d->input_avail = size;
    
    return XA_NO_ERROR;
}

/* ...get number of output bytes */
static XA_ERRORCODE xa_pcm_stereo_get_output_bytes(XAPcmStereo *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...sanity check - check parameters */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...track index must be zero */
    XF_CHK_ERR(i_idx == 1, XA_API_FATAL_INVALID_CMD_TYPE);
    
    /* ...pcm stereo component must be running */
    XF_CHK_ERR(d->state & XA_PCM_STEREO_FLAG_RUNNING, XA_API_FATAL_INVALID_CMD_TYPE);
    
    /* ...output buffer must exist */
    XF_CHK_ERR(d->output, XA_PCM_STEREO_EXEC_NONFATAL_OUTPUT);

    /* ...return number of produced bytes */
    *(WORD32 *)pv_value = ((d->state & XA_PCM_STEREO_FLAG_OUTPUT) ? d->produced : 0);

    return XA_NO_ERROR;
}

/* ...get number of consumed bytes */
static XA_ERRORCODE xa_pcm_stereo_get_curidx_input_buf(XAPcmStereo *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...sanity check - check parameters */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...track index must be valid */
    XF_CHK_ERR(i_idx == 0, XA_API_FATAL_INVALID_CMD_TYPE);
    
    /* ...pcm stereo component must be running */
    XF_CHK_ERR(d->state & XA_PCM_STEREO_FLAG_RUNNING, XA_PCM_STEREO_EXEC_FATAL_STATE);
    
    /* ...input buffer must exist */
    XF_CHK_ERR(d->input, XA_PCM_STEREO_EXEC_FATAL_INPUT);

    /* ...return number of bytes consumed (always consume fixed-length chunk) */
    *(WORD32 *)pv_value = d->consumed;
    
    /* ...and reset internally managed number of consumed bytes */
    d->consumed = 0;
    
    return XA_NO_ERROR;
}

/* ...end-of-stream processing */
static XA_ERRORCODE xa_pcm_stereo_input_over(XAPcmStereo *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...sanity check */
    XF_CHK_ERR(d, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...put end-of-stream flag */
    d->state |= XA_PCM_STEREO_FLAG_COMPLETE;
    
    TRACE(PROCESS, _b("Input-over-condition signalled"));
    
    return XA_NO_ERROR;
}

/*******************************************************************************
 * Memory information API
 ******************************************************************************/
/* ..get total amount of data for memory tables */
static XA_ERRORCODE xa_pcm_stereo_get_memtabs_size(XAPcmStereo *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...basic sanity checks */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...check pcm stereo component is pre-initialized */
    XF_CHK_ERR(d->state & XA_PCM_STEREO_FLAG_PREINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);
    
    /* ...we have all our tables inside API structure - good? tbd */
    *(WORD32 *)pv_value = 0;
    
    return XA_NO_ERROR;
}

/* ..set memory tables pointer */
static XA_ERRORCODE xa_pcm_stereo_set_memtabs_ptr(XAPcmStereo *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...basic sanity checks */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...check pcm stereo component is pre-initialized */
    XF_CHK_ERR(d->state & XA_PCM_STEREO_FLAG_PREINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);
    
    /* ...do not do anything; just return success - tbd */
    return XA_NO_ERROR;
}

/* ...return total amount of memory buffers */
static XA_ERRORCODE xa_pcm_stereo_get_n_memtabs(XAPcmStereo *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...basic sanity checks */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);
    
    /* ...we have 1 input buffer and 1 output buffer*/
    *(WORD32 *)pv_value = 2;
    
    return XA_NO_ERROR;
}

/* ...return memory buffer data */
static XA_ERRORCODE xa_pcm_stereo_get_mem_info_size(XAPcmStereo *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...basic sanity check */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...return frame buffer minimal size only after post-initialization is done */
    XF_CHK_ERR(d->state & XA_PCM_STEREO_FLAG_POSTINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...all buffers are of the same length */
    *(WORD32 *)pv_value = (WORD32) d->buffer_size * (i_idx + 1);
        
    return XA_NO_ERROR;
}

/* ...return memory alignment data */
static XA_ERRORCODE xa_pcm_stereo_get_mem_info_alignment(XAPcmStereo *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...basic sanity check */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...return frame buffer minimal size only after post-initialization is done */
    XF_CHK_ERR(d->state & XA_PCM_STEREO_FLAG_POSTINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...all buffers are 4-bytes aligned */
    *(WORD32 *)pv_value = 4;
        
    return XA_NO_ERROR;
}

/* ...return memory type data */
static XA_ERRORCODE xa_pcm_stereo_get_mem_info_type(XAPcmStereo *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...basic sanity check */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...return frame buffer minimal size only after post-initialization is done */
    XF_CHK_ERR(d->state & XA_PCM_STEREO_FLAG_POSTINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

    switch (i_idx)
    {
    case 0:
        /* ...input buffer */
        *(WORD32 *)pv_value = XA_MEMTYPE_INPUT;
        return XA_NO_ERROR;
        
    case 1:
        /* ...output buffer */
        *(WORD32 *)pv_value = XA_MEMTYPE_OUTPUT;
        return XA_NO_ERROR;

    default:
        /* ...invalid index */
        return XF_CHK_ERR(0, XA_API_FATAL_INVALID_CMD_TYPE);
    }
}

/* ...set memory pointer */
static XA_ERRORCODE xa_pcm_stereo_set_mem_ptr(XAPcmStereo *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...basic sanity check */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...codec must be initialized */
    XF_CHK_ERR(d->state & XA_PCM_STEREO_FLAG_POSTINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);
    
    /* ...select memory buffer */
    switch (i_idx)
    {
    case 0:
        /* ...input buffers */
        d->input = pv_value;
        return XA_NO_ERROR;

    case 1:
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

static XA_ERRORCODE (* const xa_pcm_stereo_api[])(XAPcmStereo *, WORD32, pVOID) = 
{
    [XA_API_CMD_GET_API_SIZE]           = xa_pcm_stereo_get_api_size,

    [XA_API_CMD_INIT]                   = xa_pcm_stereo_init, 
    [XA_API_CMD_SET_CONFIG_PARAM]       = xa_pcm_stereo_set_config_param,
    [XA_API_CMD_GET_CONFIG_PARAM]       = xa_pcm_stereo_get_config_param,

    [XA_API_CMD_EXECUTE]                = xa_pcm_stereo_execute,
    [XA_API_CMD_SET_INPUT_BYTES]        = xa_pcm_stereo_set_input_bytes,
    [XA_API_CMD_GET_OUTPUT_BYTES]       = xa_pcm_stereo_get_output_bytes,
    [XA_API_CMD_GET_CURIDX_INPUT_BUF]   = xa_pcm_stereo_get_curidx_input_buf,
    [XA_API_CMD_INPUT_OVER]             = xa_pcm_stereo_input_over,

    [XA_API_CMD_GET_MEMTABS_SIZE]       = xa_pcm_stereo_get_memtabs_size,
    [XA_API_CMD_SET_MEMTABS_PTR]        = xa_pcm_stereo_set_memtabs_ptr,    
    [XA_API_CMD_GET_N_MEMTABS]          = xa_pcm_stereo_get_n_memtabs,
    [XA_API_CMD_GET_MEM_INFO_SIZE]      = xa_pcm_stereo_get_mem_info_size,
    [XA_API_CMD_GET_MEM_INFO_ALIGNMENT] = xa_pcm_stereo_get_mem_info_alignment,
    [XA_API_CMD_GET_MEM_INFO_TYPE]      = xa_pcm_stereo_get_mem_info_type,
    [XA_API_CMD_SET_MEM_PTR]            = xa_pcm_stereo_set_mem_ptr,
};

/* ...total number of commands supported */
#define XA_PCM_STEREO_API_COMMANDS_NUM   (sizeof(xa_pcm_stereo_api) / sizeof(xa_pcm_stereo_api[0]))

/*******************************************************************************
 * API entry point
 ******************************************************************************/

XA_ERRORCODE xa_pcm_stereo(xa_codec_handle_t p_xa_module_obj, WORD32 i_cmd, WORD32 i_idx, pVOID pv_value)
{
    XAPcmStereo *d = (XAPcmStereo *) p_xa_module_obj;

    /* ...check if command index is sane */
    XF_CHK_ERR(i_cmd < XA_PCM_STEREO_API_COMMANDS_NUM, XA_API_FATAL_INVALID_CMD);
    
    /* ...see if command is defined */
    XF_CHK_ERR(xa_pcm_stereo_api[i_cmd], XA_API_FATAL_INVALID_CMD);
    
    /* ...execute requested command */
    return xa_pcm_stereo_api[i_cmd](d, i_idx, pv_value);
}

#endif /* XA_PCM_STEREO */
