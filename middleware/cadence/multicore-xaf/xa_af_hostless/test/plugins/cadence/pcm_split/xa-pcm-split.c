/*
* Copyright (c) 2015-2024 Cadence Design Systems Inc.
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
 * xa-pcm-split.c
 *
 * Sample pcm_split plugin
 ******************************************************************************/

#define MODULE_TAG                      PCM_SPLIT

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include <stdint.h>
#include <string.h>

#include "audio/xa-pcm-split-api.h"

/* ...debugging facility */
#include "xf-debug.h"

#include "xaf-api.h"

#ifdef XAF_PROFILE
#include "xaf-clk-test.h"
extern clk_t pcm_split_cycles;
#endif

/*******************************************************************************
 * Internal plugin-specific definitions
 ******************************************************************************/

#define XA_MIMO_CFG_DEFAULT_PCM_WIDTH	16
#define XA_MIMO_CFG_DEFAULT_CHANNELS	2
#define XA_MIMO_CFG_DEFAULT_FRAMESIZE	512

#define XA_MIMO_CFG_PERSIST_SIZE	4
#define XA_MIMO_CFG_SCRATCH_SIZE	4
#define XA_MIMO_CFG_IN_BUFFER_SIZE	(1024*4)
#define XA_MIMO_CFG_OUT_BUFFER_SIZE	(1024*4)

#define XA_MIMO_IN_PORTS		1
#define XA_MIMO_OUT_PORTS		2

/*******************************************************************************
 * Internal functions definitions
 ******************************************************************************/

/* ...API structure */
typedef struct XAPcmAec
{
    /* ...aec state */
    UWORD32                 state;

    /* ...sampling rate */
    UWORD32                 sample_rate;

    /* ... input port parameters */
    /* ...number of bytes in input buffer */
    UWORD32                 in_buffer_size;

    /* ...input buffers */
    void               	    *input[XA_MIMO_IN_PORTS];

    UWORD32                 consumed[XA_MIMO_IN_PORTS];

    /* ...number of valid samples in individual buffers */
    UWORD32                 input_length[XA_MIMO_IN_PORTS];

    /* ...number of input ports */
    UWORD32                 num_in_ports;


    /* ... output port parameters */
    /* ...number of bytes in input/output buffer */
    UWORD32                 out_buffer_size;

    /* ...output buffer */
    void               	    *output[XA_MIMO_OUT_PORTS];

    UWORD32                 produced[XA_MIMO_OUT_PORTS];

    UWORD32                 num_out_ports; /* ... number of output ports active */

    /* ...scratch buffer pointer */
    void               	    *scratch;
    void               	    *persist;

    UWORD32		    persist_size;
    UWORD32		    scratch_size;

    UWORD32                 pcm_width;
    UWORD32                 channels;

    WORD16		    port_state[XA_MIMO_IN_PORTS + XA_MIMO_OUT_PORTS];

}   XAPcmAec;

/*******************************************************************************
 * Aec state flags
 ******************************************************************************/

#define XA_AEC_FLAG_PREINIT_DONE      (1 << 0)
#define XA_AEC_FLAG_POSTINIT_DONE     (1 << 1)
#define XA_AEC_FLAG_RUNNING           (1 << 2)
#define XA_AEC_FLAG_OUTPUT            (1 << 3)
#define XA_AEC_FLAG_INPUT_OVER        (1 << 4)
#define XA_AEC_FLAG_COMPLETE          (1 << 5)
#define XA_AEC_FLAG_EXEC_DONE         (1 << 6)
#define XA_AEC_FLAG_PORT_PAUSED       (1 << 7)
#define XA_AEC_FLAG_PORT_CONNECTED    (1 << 8)

/*******************************************************************************
 * DSP functions
 ******************************************************************************/

/* ...aec preinitialization (default parameters) */
static inline void xa_aec_preinit(XAPcmAec *d)
{
    /* ...pre-configuration initialization; reset internal data */
    memset(d, 0, sizeof(*d));

    /* ...set default parameters */
    d->num_in_ports = XA_MIMO_IN_PORTS;
    d->num_out_ports = XA_MIMO_OUT_PORTS;

    d->pcm_width = XA_MIMO_CFG_DEFAULT_PCM_WIDTH;
    d->channels = XA_MIMO_CFG_DEFAULT_CHANNELS;

    d->in_buffer_size = XA_MIMO_CFG_IN_BUFFER_SIZE;
    d->out_buffer_size = XA_MIMO_CFG_OUT_BUFFER_SIZE;
    d->persist_size = XA_MIMO_CFG_PERSIST_SIZE;
    d->scratch_size = XA_MIMO_CFG_SCRATCH_SIZE;
}

/* ...do pcm-gain scaling of stereo PCM-16 streams */
static XA_ERRORCODE xa_aec_do_execute_stereo_16bit(XAPcmAec *d)
{
    WORD32     i, nSize;

    XF_CHK_ERR(d->state & XA_AEC_FLAG_RUNNING, XA_API_FATAL_INVALID_CMD_TYPE);

    if((d->num_in_ports==1) && (d->num_out_ports==2))
    {
	/* Playback-> speaker/src splitter */
        /* 1 in, 2 out */

        /* ...check I/O buffer */
#if 1
        XF_CHK_ERR(d->input[0], XA_PCM_SPLIT_EXEC_FATAL_STATE);
        XF_CHK_ERR(d->output[0], XA_PCM_SPLIT_EXEC_NONFATAL_NO_DATA);
        XF_CHK_ERR(d->output[1], XA_PCM_SPLIT_EXEC_NONFATAL_NO_DATA);
#endif

        WORD32    filled = d->input_length[0];
        WORD16    *pIn0  = (WORD16 *) d->input[0];
        WORD16    *pOut0 = (WORD16 *) d->output[0];
        WORD16    *pOut1 = (WORD16 *) d->output[1];

        /* reset consumed/produced counters */
        for (i = 0;i < (d->num_in_ports); i++)
          d->consumed[i] = 0;

        for (i = 0;i < (d->num_out_ports); i++)
          d->produced[i] = 0;

        for (i = 0;i < (d->num_in_ports + d->num_out_ports); i++)
        {
          if((d->port_state[i] & XA_AEC_FLAG_PORT_PAUSED) || (!(d->port_state[i] & XA_AEC_FLAG_PORT_CONNECTED)))
          {
              /* non-fatal error if one of the port is paused or not connected*/
              TRACE(PROCESS, _b("Port:%d is paused or not connected"), i);
              return XA_PCM_SPLIT_EXEC_NONFATAL_NO_DATA;
          }
        }

        filled = (filled > d->out_buffer_size) ? d->out_buffer_size : filled;
        nSize = filled >> 1;    //size of each sample is 2 bytes

        /* ...Processing loop */
        for (i = 0; i < nSize; i++)
        {
            WORD16     input0;
            input0 = *pIn0++;

            *pOut0++ = (WORD16)input0;
            *pOut1++ = (WORD16)input0;
        }

        /* ...save total number of produced bytes */
        d->produced[0] = (UWORD32)((void *)pOut0 - d->output[0]);
        d->produced[1] = (UWORD32)((void *)pOut1 - d->output[1]);

        /* ...put flag saying we have output buffer */
        if(d->produced[0])
       	  d->state |= XA_AEC_FLAG_OUTPUT;

        /* ...save total number of consumed bytes */
        //d->consumed[0] = (UWORD32)((void *)pIn0 - d->input[0]);
        d->consumed[0] = filled;
        d->input_length[0] -= d->consumed[0];

      	if(d->port_state[0] & XA_AEC_FLAG_COMPLETE)
	{
	  d->state |= XA_AEC_FLAG_EXEC_DONE; //set state: EXEC_DONE/COMPLETE
	}

        TRACE(PROCESS, _b("consumed: %u bytes, produced: (%u, %u) bytes (%u samples) done=%d"), d->consumed[0], d->produced[0], d->produced[1], nSize, (d->state & XA_AEC_FLAG_EXEC_DONE)?1:0);
    }
    else
    {
        TRACE(ERROR, _x("unsupported input(%d)/output(%d) port numbers"), d->num_in_ports, d->num_out_ports);
	return XA_PCM_SPLIT_EXEC_FATAL_STATE;
    }


    /* ...return success result code */
    return XA_NO_ERROR;
}

/* ...runtime reset */
static XA_ERRORCODE xa_aec_do_runtime_init(XAPcmAec *d)
{
    int i, num_ports;

    num_ports = XA_MIMO_IN_PORTS + XA_MIMO_OUT_PORTS;
    /*... resetting input over state */
    for (i=0; i<num_ports; i++)
    {
        d->port_state[i] &= ~XA_AEC_FLAG_COMPLETE;
    }

    d->state = XA_AEC_FLAG_PREINIT_DONE | XA_AEC_FLAG_POSTINIT_DONE | XA_AEC_FLAG_RUNNING;
    /* ...no special processing is needed here */
    return XA_NO_ERROR;
}

/*******************************************************************************
 * Commands processing
 ******************************************************************************/
/* ...codec API size query */
static XA_ERRORCODE xa_aec_get_api_size(XAPcmAec *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...check parameters are sane */
    XF_CHK_ERR(pv_value, XA_API_FATAL_INVALID_CMD_TYPE);
    /* ...retrieve API structure size */
    *(WORD32 *)pv_value = sizeof(*d);

    return XA_NO_ERROR;
}

/* ...standard codec initialization routine */
static XA_ERRORCODE xa_aec_init(XAPcmAec *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...sanity check - aec must be valid */
    XF_CHK_ERR(d, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...process particular initialization type */
    switch (i_idx)
    {
    case XA_CMD_TYPE_INIT_API_PRE_CONFIG_PARAMS:
    {
        /* ...pre-configuration initialization; reset internal data */
        xa_aec_preinit(d);

        /* ...and mark aec has been created */
        d->state = XA_AEC_FLAG_PREINIT_DONE;

        return XA_NO_ERROR;
    }

    case XA_CMD_TYPE_INIT_API_POST_CONFIG_PARAMS:
    {
        /* ...post-configuration initialization (all parameters are set) */
        XF_CHK_ERR(d->state & XA_AEC_FLAG_PREINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

        /* ...calculate input/output buffer size in bytes */
        //d->in_buffer_size = d->channels * d->frame_size * (d->pcm_width == 16 ? sizeof(WORD16) : sizeof(WORD32));

        /* ...mark post-initialization is complete */
        d->state |= XA_AEC_FLAG_POSTINIT_DONE;

        return XA_NO_ERROR;
    }

    case XA_CMD_TYPE_INIT_PROCESS:
    {
        /* ...kick run-time initialization process; make sure aec is setup */
        XF_CHK_ERR(d->state & XA_AEC_FLAG_POSTINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

        /* ...enter into execution stage */
        d->state |= XA_AEC_FLAG_RUNNING;

        return XA_NO_ERROR;
    }

    case XA_CMD_TYPE_INIT_DONE_QUERY:
    {
        /* ...check if initialization is done; make sure pointer is sane */
        XF_CHK_ERR(pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

        /* ...put current status */
        *(WORD32 *)pv_value = (d->state & XA_AEC_FLAG_RUNNING ? 1 : 0);

        return XA_NO_ERROR;
    }

    default:
        /* ...unrecognized command type */
        TRACE(ERROR, _x("Unrecognized command type: %X"), i_idx);
        return XA_API_FATAL_INVALID_CMD_TYPE;
    }
}

/* ...set aec configuration parameter */
static XA_ERRORCODE xa_aec_set_config_param(XAPcmAec *d, WORD32 i_idx, pVOID pv_value)
{
    UWORD32     i_value;

    /* ...sanity check - aec pointer must be sane */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...pre-initialization must be completed, aec must be idle */
    XF_CHK_ERR(d->state & XA_AEC_FLAG_PREINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...get parameter value  */
    i_value = (UWORD32) *(WORD32 *)pv_value;

    /* ...process individual configuration parameter */
    switch (i_idx & 0xF)
    {
    case XA_PCM_SPLIT_CONFIG_PARAM_SAMPLE_RATE:
         {
            /* ...set aec sample rate */
            switch((UWORD32)i_value)
            {
                case 4000:
                case 8000:
                case 11025:
                case 12000:
                case 16000:
                case 22050:
                case 24000:
                case 32000:
                case 44100:
                case 48000:
                case 64000:
                case 88200:
                case 96000:
                case 128000:
                case 176400:
                case 192000:
                    d->sample_rate = (UWORD32)i_value;
                    break;
                default:
                    XF_CHK_ERR(0, XA_PCM_SPLIT_CONFIG_FATAL_RANGE);
            }
	break;
        }

    case XA_PCM_SPLIT_CONFIG_PARAM_PCM_WIDTH:
        /* ...check value is permitted (16 bits only) */
        XF_CHK_ERR(i_value == 16, XA_PCM_SPLIT_CONFIG_FATAL_RANGE);
        d->pcm_width = (UWORD32)i_value;
	break;

    case XA_PCM_SPLIT_CONFIG_PARAM_CHANNELS:
        /* ...allow stereo only */
        XF_CHK_ERR((i_value <= 2) && (i_value > 0), XA_PCM_SPLIT_CONFIG_FATAL_RANGE);
        d->channels = (UWORD32)i_value;
	break;

    case XA_PCM_SPLIT_CONFIG_PARAM_PORT_PAUSE:
        {
          XF_CHK_ERR((i_value < (d->num_in_ports + d->num_out_ports)), XA_PCM_SPLIT_CONFIG_FATAL_RANGE);
          d->port_state[i_value] |= XA_AEC_FLAG_PORT_PAUSED;
          TRACE(PROCESS, _b("Pause port:%d"), i_value);
        }
	break;

    case XA_PCM_SPLIT_CONFIG_PARAM_PORT_RESUME:
        {
          XF_CHK_ERR((i_value < (d->num_in_ports + d->num_out_ports)), XA_PCM_SPLIT_CONFIG_FATAL_RANGE);
          d->port_state[i_value] &= ~XA_AEC_FLAG_PORT_PAUSED;
          TRACE(PROCESS, _b("Resume port:%d"), i_value);
        }
	break;

    case XA_PCM_SPLIT_CONFIG_PARAM_PORT_CONNECT:
        {
          XF_CHK_ERR((i_value < (d->num_in_ports + d->num_out_ports)), XA_PCM_SPLIT_CONFIG_FATAL_RANGE);
          d->port_state[i_value] |= XA_AEC_FLAG_PORT_CONNECTED;
          TRACE(PROCESS, _b("Connect on port:%d"), i_value);
        }
	break;

    case XA_PCM_SPLIT_CONFIG_PARAM_PORT_DISCONNECT:
        {
          XF_CHK_ERR((i_value < (d->num_in_ports + d->num_out_ports)), XA_PCM_SPLIT_CONFIG_FATAL_RANGE);
          d->port_state[i_value] &= ~XA_AEC_FLAG_PORT_CONNECTED;
          TRACE(PROCESS, _b("Disconnect on port:%d"), i_value);
        }
	break;

    default:
        TRACE(ERROR, _x("Invalid parameter: %X"), i_idx);
        return XA_API_FATAL_INVALID_CMD_TYPE;
    }

    return XA_NO_ERROR;
}

/* ...retrieve configuration parameter */
static XA_ERRORCODE xa_aec_get_config_param(XAPcmAec *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...sanity check - aec must be initialized */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...make sure pre-initialization is completed */
    XF_CHK_ERR(d->state & XA_AEC_FLAG_PREINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...process individual configuration parameter */
    switch (i_idx & 0xF)
    {
    case XA_PCM_SPLIT_CONFIG_PARAM_SAMPLE_RATE:
        /* ...return aec sample rate */
        *(WORD32 *)pv_value = d->sample_rate;
        return XA_NO_ERROR;

    case XA_PCM_SPLIT_CONFIG_PARAM_PCM_WIDTH:
        /* ...return current PCM width */
        *(WORD32 *)pv_value = d->pcm_width;
        return XA_NO_ERROR;

    case XA_PCM_SPLIT_CONFIG_PARAM_CHANNELS:
        /* ...return current channel number */
        *(WORD32 *)pv_value = d->channels;
        return XA_NO_ERROR;

#ifdef XA_EXT_CONFIG_TEST
    case XA_PCM_SPLIT_CONFIG_PARAM_PRODUCED:
    {
        xaf_ext_buffer_t *ext_buf = (xaf_ext_buffer_t *) pv_value;
        memcpy(ext_buf->data, &d->produced, sizeof(d->produced));
        ext_buf->valid_data_size = sizeof(d->produced);
        return XA_NO_ERROR;
    }
#endif

    default:
        TRACE(ERROR, _x("Invalid parameter: %X"), i_idx);
        return XA_API_FATAL_INVALID_CMD_TYPE;
    }
}

/* ...execution command */
static XA_ERRORCODE xa_aec_execute(XAPcmAec *d, WORD32 i_idx, pVOID pv_value)
{
    XA_ERRORCODE ret;
#ifdef XAF_PROFILE
    clk_t pcm_split_start, pcm_split_stop;
#endif

    /* ...sanity check - aec must be valid */
    XF_CHK_ERR(d, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...aec must be in running state */
    XF_CHK_ERR(d->state & XA_AEC_FLAG_RUNNING, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...process individual command type */
    switch (i_idx)
    {
    case XA_CMD_TYPE_DO_EXECUTE:
        /* ...perform pcm-gain of the channels */
#ifdef XAF_PROFILE
        pcm_split_start = clk_read_start(CLK_SELN_THREAD);
#endif
        ret = xa_aec_do_execute_stereo_16bit(d);
#ifdef XAF_PROFILE
        pcm_split_stop = clk_read_stop(CLK_SELN_THREAD);
        pcm_split_cycles += clk_diff(pcm_split_stop, pcm_split_start);
#endif
        return ret;

    case XA_CMD_TYPE_DONE_QUERY:
        /* ...check if processing is complete */
        XF_CHK_ERR(pv_value, XA_API_FATAL_INVALID_CMD_TYPE);
        *(WORD32 *)pv_value = (d->state & XA_AEC_FLAG_EXEC_DONE? 1 : 0);
        return XA_NO_ERROR;

    case XA_CMD_TYPE_DO_RUNTIME_INIT:
        /* ...reset aec operation */
        return xa_aec_do_runtime_init(d);

    default:
        /* ...unrecognized command */
        TRACE(ERROR, _x("Invalid index: %X"), i_idx);
        return XA_API_FATAL_INVALID_CMD_TYPE;
    }
}

/* ...set number of input bytes */
static XA_ERRORCODE xa_aec_set_input_bytes(XAPcmAec *d, WORD32 i_idx, pVOID pv_value)
{
    UWORD32     size;

    /* ...sanity check - check parameters */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...track index must be valid */
    XF_CHK_ERR(i_idx >= 0 && i_idx < d->num_in_ports, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...aec must be initialized */
    XF_CHK_ERR(d->state & XA_AEC_FLAG_POSTINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...input buffer must exist */
    XF_CHK_ERR(d->input[i_idx], XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...input frame length should not be zero (in bytes) */
    XF_CHK_ERR((size = (UWORD32)*(WORD32 *)pv_value) >= 0, XA_PCM_SPLIT_CONFIG_FATAL_RANGE);

    /* ...all is correct; set input buffer length in bytes */
    d->input_length[i_idx] = size;

    return XA_NO_ERROR;
}

/* ...get number of output bytes */
static XA_ERRORCODE xa_aec_get_output_bytes(XAPcmAec *d, WORD32 i_idx, pVOID pv_value)
{
    int p_idx;
    /* ...sanity check - check parameters */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    p_idx = i_idx-d->num_in_ports;
    /* ...track index must be zero */
    XF_CHK_ERR((p_idx >= 0) && (p_idx < d->num_out_ports), XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...aec must be running */
    XF_CHK_ERR(d->state & XA_AEC_FLAG_RUNNING, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...output buffer must exist */
    XF_CHK_ERR(d->output, XA_PCM_SPLIT_EXEC_FATAL_STATE);

    /* ...return number of produced bytes */
    *(WORD32 *)pv_value = (d->state & XA_AEC_FLAG_OUTPUT ? d->produced[p_idx] : 0);

    return XA_NO_ERROR;
}

/* ...get number of consumed bytes */
static XA_ERRORCODE xa_aec_get_curidx_input_buf(XAPcmAec *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...sanity check - check parameters */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...track index must be valid */
    XF_CHK_ERR(i_idx >= 0 && i_idx < XA_MIMO_IN_PORTS, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...aec must be running */
    XF_CHK_ERR(d->state & XA_AEC_FLAG_RUNNING, XA_PCM_SPLIT_EXEC_FATAL_STATE);

    /* ...input buffer must exist */
    XF_CHK_ERR(d->input[i_idx], XA_PCM_SPLIT_EXEC_FATAL_STATE);

    /* ...return number of bytes consumed (always consume fixed-length chunk) */
    *(WORD32 *)pv_value = d->consumed[i_idx];

    return XA_NO_ERROR;
}

/* ...end-of-stream processing */
static XA_ERRORCODE xa_aec_input_over(XAPcmAec *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...sanity check */
    int i, input_over_count = 0;
    XF_CHK_ERR(d, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ... port specific end-of-stream flag */
    d->port_state[i_idx] |= XA_AEC_FLAG_COMPLETE;
    for(i=0; i<d->num_in_ports; i++)
    {
    	input_over_count += (XA_AEC_FLAG_COMPLETE & d->port_state[i])?1:0;
    }

    /* ...put overall end-of-stream flag */
    if(input_over_count == d->num_in_ports)
	d->state |= XA_AEC_FLAG_COMPLETE;

    TRACE(PROCESS, _b("Input-over-condition signalled for port %d"), i_idx);

    return XA_NO_ERROR;
}

/*******************************************************************************
 * Memory information API
 ******************************************************************************/

/* ..get total amount of data for memory tables */
static XA_ERRORCODE xa_aec_get_memtabs_size(XAPcmAec *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...basic sanity checks */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...check aec is pre-initialized */
    XF_CHK_ERR(d->state & XA_AEC_FLAG_PREINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...we have all our tables inside API structure - good? tbd */
    *(WORD32 *)pv_value = 0;

    return XA_NO_ERROR;
}

/* ..set memory tables pointer */
static XA_ERRORCODE xa_aec_set_memtabs_ptr(XAPcmAec *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...basic sanity checks */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...check aec is pre-initialized */
    XF_CHK_ERR(d->state & XA_AEC_FLAG_PREINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...do not do anything; just return success - tbd */
    return XA_NO_ERROR;
}

/* ...return total amount of memory buffers */
static XA_ERRORCODE xa_aec_get_n_memtabs(XAPcmAec *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...basic sanity checks */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...we have N input ports, M output ports and 1 persistent and 1 scratch buffer */
    *(WORD32 *)pv_value = d->num_in_ports + d->num_out_ports + 1 + 1;

    return XA_NO_ERROR;
}

/* ...return memory buffer data */
static XA_ERRORCODE xa_aec_get_mem_info_size(XAPcmAec *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...basic sanity check */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...return frame buffer minimal size only after post-initialization is done */
    XF_CHK_ERR(d->state & XA_AEC_FLAG_POSTINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...all buffers are of the same length */
    WORD32 n_mems = (d->num_in_ports + d->num_out_ports + 1 + 1);
    if(i_idx < d->num_in_ports)
    {
        /* ...input buffers */
        *(WORD32 *)pv_value = d->in_buffer_size;
    }
    else
    if(i_idx < (d->num_in_ports + d->num_out_ports))
    {
        /* ...output buffer */
        *(WORD32 *)pv_value = d->out_buffer_size;
    }
    else
    if(i_idx < (n_mems-1))
    {
        /* ...persist buffer */
        *(WORD32 *)pv_value = d->persist_size;
    }
    else
    if(i_idx < (n_mems))
    {
        /* ...scratch buffer */
        *(WORD32 *)pv_value = d->scratch_size;
    }
    else{
	return XA_API_FATAL_INVALID_CMD_TYPE;
    }

    return XA_NO_ERROR;
}

/* ...return memory alignment data */
static XA_ERRORCODE xa_aec_get_mem_info_alignment(XAPcmAec *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...basic sanity check */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...return frame buffer minimal size only after post-initialization is done */
    XF_CHK_ERR(d->state & XA_AEC_FLAG_POSTINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...all buffers are 4-bytes aligned */
    *(WORD32 *)pv_value = 4;

    return XA_NO_ERROR;
}

/* ...return memory type data */
static XA_ERRORCODE xa_aec_get_mem_info_type(XAPcmAec *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...basic sanity check */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...return frame buffer minimal size only after post-initialization is done */

    XF_CHK_ERR(d->state & XA_AEC_FLAG_POSTINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

    WORD32 n_mems = (d->num_in_ports + d->num_out_ports + 1 + 1);
    if(i_idx < d->num_in_ports)
    {
        /* ...input buffers */
        *(WORD32 *)pv_value = XA_MEMTYPE_INPUT;
    }
    else
    if(i_idx < (d->num_in_ports + d->num_out_ports))
    {
        /* ...output buffer */
        *(WORD32 *)pv_value = XA_MEMTYPE_OUTPUT;
    }
    else
    if(i_idx < (n_mems-1))
    {
        /* ...scratch buffer */
        *(WORD32 *)pv_value = XA_MEMTYPE_PERSIST;
    }
    else
    if(i_idx < (n_mems))
    {
        /* ...scratch buffer */
        *(WORD32 *)pv_value = XA_MEMTYPE_SCRATCH;
    }
    else{
	return XA_API_FATAL_INVALID_CMD_TYPE;
    }
    return XA_NO_ERROR;
}

/* ...set memory pointer */
static XA_ERRORCODE xa_aec_set_mem_ptr(XAPcmAec *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...basic sanity check */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...codec must be initialized */
    XF_CHK_ERR(d->state & XA_AEC_FLAG_POSTINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

    WORD32 n_mems = (d->num_in_ports + d->num_out_ports + 1 + 1);
    if(i_idx < d->num_in_ports)
    {
        /* ...input buffers */
        d->input[i_idx] = pv_value;
    }
    else
    if(i_idx < (d->num_in_ports + d->num_out_ports))
    {
        /* ...output buffer */
        d->output[i_idx-d->num_in_ports] = pv_value;
    }
    else
    if(i_idx < (n_mems-1))
    {
        /* ...persistent buffer */
        d->persist = pv_value;
    }
    else
    if(i_idx < (n_mems))
    {
        /* ...scratch buffer */
        d->scratch = pv_value;
    }
    else{
	return XA_API_FATAL_INVALID_CMD_TYPE;
    }
    return XA_NO_ERROR;
}

/*******************************************************************************
 * API command hooks
 ******************************************************************************/

static XA_ERRORCODE (* const xa_aec_api[])(XAPcmAec *, WORD32, pVOID) =
{
    [XA_API_CMD_GET_API_SIZE]           = xa_aec_get_api_size,

    [XA_API_CMD_INIT]                   = xa_aec_init,
    [XA_API_CMD_SET_CONFIG_PARAM]       = xa_aec_set_config_param,
    [XA_API_CMD_GET_CONFIG_PARAM]       = xa_aec_get_config_param,

    [XA_API_CMD_EXECUTE]                = xa_aec_execute,
    [XA_API_CMD_SET_INPUT_BYTES]        = xa_aec_set_input_bytes,
    [XA_API_CMD_GET_OUTPUT_BYTES]       = xa_aec_get_output_bytes,
    [XA_API_CMD_GET_CURIDX_INPUT_BUF]   = xa_aec_get_curidx_input_buf,
    [XA_API_CMD_INPUT_OVER]             = xa_aec_input_over,

    [XA_API_CMD_GET_MEMTABS_SIZE]       = xa_aec_get_memtabs_size,
    [XA_API_CMD_SET_MEMTABS_PTR]        = xa_aec_set_memtabs_ptr,
    [XA_API_CMD_GET_N_MEMTABS]          = xa_aec_get_n_memtabs,
    [XA_API_CMD_GET_MEM_INFO_SIZE]      = xa_aec_get_mem_info_size,
    [XA_API_CMD_GET_MEM_INFO_ALIGNMENT] = xa_aec_get_mem_info_alignment,
    [XA_API_CMD_GET_MEM_INFO_TYPE]      = xa_aec_get_mem_info_type,
    [XA_API_CMD_SET_MEM_PTR]            = xa_aec_set_mem_ptr,
};

/* ...total number of commands supported */
#define XA_AEC_API_COMMANDS_NUM   (sizeof(xa_aec_api) / sizeof(xa_aec_api[0]))

/*******************************************************************************
 * API entry point
 ******************************************************************************/

XA_ERRORCODE xa_pcm_split(xa_codec_handle_t p_xa_module_obj, WORD32 i_cmd, WORD32 i_idx, pVOID pv_value)
{
    XAPcmAec    *d = (XAPcmAec *) p_xa_module_obj;

    /* ...check if command index is sane */
    XF_CHK_ERR(i_cmd < XA_AEC_API_COMMANDS_NUM, XA_API_FATAL_INVALID_CMD);

    /* ...see if command is defined */
    XF_CHK_ERR(xa_aec_api[i_cmd], XA_API_FATAL_INVALID_CMD);

    /* ...execute requested command */
    return xa_aec_api[i_cmd](d, i_idx, pv_value);
}

