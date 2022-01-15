/*******************************************************************************
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

#define MODULE_TAG                      CLIENT_PROXY

/*******************************************************************************
 * Includes
 ******************************************************************************/

#if (XA_CLIENT_PROXY)

#include <stdint.h>
#include <string.h>

/* ...debugging facility */
#include "xf-debug.h"
#include "client_proxy_api.h"
#include "xa-mimo-proc-api.h"
#ifdef XAF_PROFILE
#include "xaf-clk-test.h"
extern clk_t client_proxy_cycles;
#endif

#include "srtm_utils.h"
#include "srtm_config.h"
#include "rpmsg_lite.h"
#include "dsp_config.h"
#include "fsl_gpio.h"

/* EAP Library, configuration */
#if (defined EAP_PROC)
#include "EAP16.h" // EAP16 library header
#elif (defined EAP32_PROC)
#include "EAP32.h" // EAP32 library header
#endif
#include "EAP_Parameter_AutoVolumeLeveler.h"
#include "EAP_Parameter_ConcertSound.h"
#include "EAP_Parameter_LoudnessMaximiser.h"
#include "EAP_Parameter_MusicEnhancerRMSLimiter.h"
#include "EAP_Parameter_VoiceEnhancer.h"
#include "EAP_Parameter_AllEffectOff.h"
#include "EAP_Parameter_Custom.h"
#include "EAP_Parameter_Crossover2waySpeakers.h"
#include "EAP_Parameter_CrossoverForSubwoofer.h"
#include "EAP_Parameter_ToneGenerator.h"

/*******************************************************************************
 * Tracing configuration
 ******************************************************************************/

//TRACE_TAG(INIT, 1);
//TRACE_TAG(PROCESS, 1);

/*******************************************************************************
 * Internal functions definitions
 ******************************************************************************/

/* ...API structure */
typedef struct client_proxy_t
{
    /* ...pcm gain module state */
    UWORD32                 state;

    /* ...number of channels (channel mask?) */
    UWORD32                 channels;

    /* ...PCM sample width */
    UWORD32                 pcm_width;

    /* ...sampling rate */
    UWORD32                 sample_rate;

    /* ...number of bytes in input/output buffer */
    UWORD32                 buffer_size;

    /* ...input buffer */
    void               *input;

    /* ...output buffer */
    void               *output;

    /* ...scratch buffer pointer */
    void               *scratch;

    /* ...number of available bytes in the input buffer */
    UWORD32                 input_avail;

    /* ...number of bytes consumed from input buffer */
    UWORD32                 consumed;

    /* ...number of produced bytes */
    UWORD32                 produced;

    UWORD32 frame_size_us;

}   client_proxy_t;


/*******************************************************************************
 * Client proxy state flags
 ******************************************************************************/

#define XA_CLIENT_PROXY_FLAG_PREINIT_DONE      (1 << 0)
#define XA_CLIENT_PROXY_FLAG_POSTINIT_DONE     (1 << 1)
#define XA_CLIENT_PROXY_FLAG_RUNNING           (1 << 2)
#define XA_CLIENT_PROXY_FLAG_OUTPUT            (1 << 3)
#define XA_CLIENT_PROXY_FLAG_COMPLETE          (1 << 4)

#define LVM_FRAME_SIZE_MS 10


extern dsp_handle_t dsp;

//EAP Lib
static LVM_Handle_t         EAP_hInstance;      /* Instance handle */
static LVM_MemTab_t         EAP_MemTab;         /* Memory allocation table */
static LVM_VersionInfo_st   EAP_VersionInfo;    /* Version info structure */
static LVM_ControlParams_t  EAP_ControlParams;  /* control Parameters */
static LVM_InstParams_t     EAP_InstParams;     /* EAP_InstParams */

#ifdef ALGORITHM_EQNB
static LVM_HeadroomParams_t EAP_HeadroomParams; /* Headroom parameters */
#endif

LVM_ReturnStatus_en EAP_Init(client_proxy_t *d);
LVM_ReturnStatus_en EAP_SetSampleRateAndNumOfChannels(client_proxy_t *d);
LVM_ReturnStatus_en EAP_SetConfig(LVM_ControlParams_t *pEAP_ControlParamsSet);
LVM_ReturnStatus_en EAP_UpdateConfig(LVM_ControlParams_t *pEAP_ControlParamsUpdate, LVM_INT8 paramNum);
LVM_ReturnStatus_en EAP_Execute(void * inputBuffer, void * outputBuffer[2], int numSamples, LVM_UINT32 audioTimeMs);
LVM_ReturnStatus_en EAP_Deinit();
LVM_UINT32 EAP_AudioTime = 0;

/*******************************************************************************
 * DSP functions
 ******************************************************************************/

static inline void xa_client_proxy_preinit(client_proxy_t *d)
{
    /* ...pre-configuration initialization; reset internal data */
    memset(d, 0, sizeof(*d));

    /* ...set default parameters */
    d->channels = 2;
    d->pcm_width = 16;
    d->sample_rate = 48000;
    EAP_AudioTime = 0;
}

static XA_ERRORCODE xa_client_proxy_do_execute(client_proxy_t *d)
{
    WORD32  k, i, nSize;
#if(defined EAP_PROC)
    WORD16  *pIn = (WORD16 *) d->input;
    WORD16  *pOut[2];
#elif (defined EAP32_PROC)
    WORD32  *pIn = (WORD32 *) d->input;
    WORD32  *pOut[2];
#endif
    UWORD32 filled = d->input_avail;

    if (EAP_ControlParams.XO_OperatingMode == LVM_MODE_ON)
    {
        for(i = 0; i < 2; i++)
        {
            pOut[i] = malloc(filled * d->channels);
        }
    }
    else
    {
#if(defined EAP_PROC)
        pOut[0] = (WORD16 *) d->output;
#elif (defined EAP32_PROC)
        pOut[0] = (WORD32 *) d->output;
#endif
        pOut[1] = NULL;
    }
    nSize = filled / (d->pcm_width >> 3);
    k = (WORD32)(d->buffer_size - filled);

    /* ...check I/O buffer */
    XF_CHK_ERR(d->input, XA_CLIENT_PROXY_EXEC_FATAL_INPUT);
    XF_CHK_ERR(d->output, XA_CLIENT_PROXY_EXEC_FATAL_INPUT);

    (k > 0 ? memset((void *)pIn + filled, 0x00, k) : 0);

    EAP_AudioTime += LVM_FRAME_SIZE_MS;

    if(LVM_SUCCESS != EAP_Execute(pIn, (void *)pOut, nSize/d->channels, EAP_AudioTime))
    {
    	return XA_CLIENT_PROXY_EXEC_FATAL_STATE;
    }

    if (EAP_ControlParams.XO_OperatingMode == LVM_MODE_ON)
    {
        if (d->channels > 1)
        {
            for (i = 0; i< nSize/d->channels; i++)
            {
                /* Interleave data - mix low and high band output of crossover, take just right channel */
#if(defined EAP_PROC)
                *((WORD16*)d->output + 2 * i)     = *(pOut[0] + d->channels * i);
                *((WORD16*)d->output + 2 * i + 1) = *(pOut[1] + d->channels * i);
#elif (defined EAP32_PROC)
                *((WORD32*)d->output + 2 * i)     = *(pOut[0] + d->channels * i);
                *((WORD32*)d->output + 2 * i + 1) = *(pOut[1] + d->channels * i);
#endif
            }
        }
        else
        {
            for (i = 0; i< nSize/d->channels; i++)
            {
                /* In case of mono input just use the low band for the mono output */
#if(defined EAP_PROC)
                *((WORD16*)d->output + i) = *(pOut[0] + i);
#elif (defined EAP32_PROC)
                *((WORD32*)d->output + i) = *(pOut[0] + i);
#endif
            }
        }
    }

    /* ...save total number of consumed bytes */
    d->consumed = filled;

    /* ...save total number of produced bytes */
    d->produced = filled;

    /* ...put flag saying we have output buffer */
    d->state |= XA_CLIENT_PROXY_FLAG_OUTPUT;

    TRACE(PROCESS, _b("produced: %u bytes (%u samples)"), d->produced, nSize);

    if (EAP_ControlParams.XO_OperatingMode == LVM_MODE_ON)
    {
        for(i = 0; i < 2; i++)
        {
            free(pOut[i]);
        }
    }

    /* ...return success result code */
    return XA_NO_ERROR;
}

/* ...runtime reset */
static XA_ERRORCODE xa_client_proxy_do_runtime_init(client_proxy_t *d)
{
    /* ...no special processing is needed here */
    return XA_NO_ERROR;
}

/*******************************************************************************
 * Commands processing
 ******************************************************************************/

/* ...codec API size query */
static XA_ERRORCODE xa_client_proxy_get_api_size(client_proxy_t *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...check parameters are sane */
    XF_CHK_ERR(pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...retrieve API structure size */
    *(WORD32 *)pv_value = sizeof(*d);

    return XA_NO_ERROR;
}

/* ...standard codec initialization routine */
static XA_ERRORCODE xa_client_proxy_init(client_proxy_t *d, WORD32 i_idx, pVOID pv_value)
{
    XF_CHK_ERR(d, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...process particular initialization type */
    switch (i_idx)
    {
    case XA_CMD_TYPE_INIT_API_PRE_CONFIG_PARAMS:
    {
        /* ...pre-configuration initialization; reset internal data */
        xa_client_proxy_preinit(d);

        d->state = XA_CLIENT_PROXY_FLAG_PREINIT_DONE;

        return XA_NO_ERROR;
    }

    case XA_CMD_TYPE_INIT_API_POST_CONFIG_PARAMS:
    {
        if (LVM_SUCCESS != EAP_Init(d))
        {
            return XA_CLIENT_PROXY_EXEC_FATAL_STATE;
        }
        if (LVM_SUCCESS != EAP_SetConfig(&EAP_ControlParams))
        {
            return XA_CLIENT_PROXY_EXEC_FATAL_STATE;
        }

        /* ...post-configuration initialization (all parameters are set) */
        XF_CHK_ERR(d->state & XA_CLIENT_PROXY_FLAG_PREINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

        //compute buffer size
        UWORD64 product = (UWORD64)d->sample_rate * d->channels * (d->pcm_width>>3) * d->frame_size_us;
        d->buffer_size = (UWORD32)(product/1000000);

        /* Align the computed buffer size to 8-byte */
        d->buffer_size = (d->buffer_size + 7) & ~0x7;

        /* ...mark post-initialization is complete */
        d->state |= XA_CLIENT_PROXY_FLAG_POSTINIT_DONE;

        return XA_NO_ERROR;
    }

    case XA_CMD_TYPE_INIT_PROCESS:
    {
        XF_CHK_ERR(d->state & XA_CLIENT_PROXY_FLAG_POSTINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

        /* ...enter into execution stage */
        d->state |= XA_CLIENT_PROXY_FLAG_RUNNING;

        return XA_NO_ERROR;
    }

    case XA_CMD_TYPE_INIT_DONE_QUERY:
    {
        /* ...check if initialization is done; make sure pointer is sane */
        XF_CHK_ERR(pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

        /* ...put current status */
        *(WORD32 *)pv_value = (d->state & XA_CLIENT_PROXY_FLAG_RUNNING ? 1 : 0);

        return XA_NO_ERROR;
    }

    default:
        /* ...unrecognised command type */
        TRACE(ERROR, _x("Unrecognised command type: %X"), i_idx);
        return XA_API_FATAL_INVALID_CMD_TYPE;
    }
}

/* ...standard codec deinitialization routine */
static XA_ERRORCODE xa_client_proxy_deinit()
{
    if (LVM_SUCCESS != EAP_Deinit())
    {
        return XA_CLIENT_PROXY_EXEC_FATAL_STATE;
    }
    return XA_NO_ERROR;
}

static XA_ERRORCODE xa_client_proxy_set_config_param(client_proxy_t *d, WORD32 i_idx, pVOID pv_value)
{
    UWORD32     i_value;

    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    XF_CHK_ERR(d->state & XA_CLIENT_PROXY_FLAG_PREINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...get parameter value  */
    i_value = (UWORD32) *(WORD32 *)pv_value;

    /* ...process individual configuration parameter */
    switch (i_idx)
    {
    case XA_CLIENT_PROXY_CONFIG_PARAM_PCM_WIDTH:
        /* ...check value is permitted */
        XF_CHK_ERR((i_value == 16) || (i_value == 32), XA_CLIENT_PROXY_CONFIG_NONFATAL_RANGE);
        d->pcm_width = (UWORD32)i_value;
        return XA_NO_ERROR;

    case XA_CLIENT_PROXY_CONFIG_PARAM_CHANNELS:
        /* ...allow mono/stereo only */
        XF_CHK_ERR((i_value <= CLIENT_PROXY_MAX_CHANNELS), XA_CLIENT_PROXY_CONFIG_NONFATAL_RANGE);
        d->channels = (UWORD32)i_value;
        return XA_NO_ERROR;

    case XA_CLIENT_PROXY_CONFIG_PARAM_SAMPLE_RATE:
        {
            switch ((UWORD32)i_value)
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
                    XF_CHK_ERR(0, XA_CLIENT_PROXY_CONFIG_NONFATAL_RANGE);
            }
            return XA_NO_ERROR;
        }

    case XA_CLIENT_PROXY_CONFIG_PARAM_INPUT_FRAME_SIZE_US:
    case XA_CLIENT_PROXY_CONFIG_PARAM_OUTPUT_FRAME_SIZE_US:
        d->frame_size_us = (UWORD32)i_value;
        return XA_NO_ERROR;
    case XA_MIMO_PROC_CONFIG_PARAM_PORT_PAUSE:
    case XA_MIMO_PROC_CONFIG_PARAM_PORT_RESUME:
    case XA_MIMO_PROC_CONFIG_PARAM_PORT_CONNECT:
    case XA_MIMO_PROC_CONFIG_PARAM_PORT_DISCONNECT:
        return XA_NO_ERROR;

    case XA_CLIENT_PROXY_CONFIG_PARAM_EAP:
        {
            LVM_ReturnStatus_en LVM_Status;
            switch ((UWORD32)i_value)
            {
                case 1:
                    memcpy(&EAP_ControlParams, &ControlParamSet_allEffectOff, sizeof(LVM_ControlParams_t));
                    break;
                case 2:
                    memcpy(&EAP_ControlParams, &ControlParamSet_voiceEnhancer, sizeof(LVM_ControlParams_t));
                    break;
                case 3:
                    memcpy(&EAP_ControlParams, &ControlParamSet_musicEnhancerRmsLimiter, sizeof(LVM_ControlParams_t));
                    break;
                case 4:
                    memcpy(&EAP_ControlParams, &ControlParamSet_autoVolumeLeveler, sizeof(LVM_ControlParams_t));
                    break;
                case 5:
                    memcpy(&EAP_ControlParams, &ControlParamSet_loudnessMaximiser, sizeof(LVM_ControlParams_t));
                    break;
                case 6:
                    memcpy(&EAP_ControlParams, &ControlParamSet_concertSound, sizeof(LVM_ControlParams_t));
                    break;
                case 7:
                    memcpy(&EAP_ControlParams, &ControlParamSet_custom, sizeof(LVM_ControlParams_t));
                    break;
                case 8:
                    memcpy(&EAP_ControlParams, &ControlParamSet_toneGenerator, sizeof(LVM_ControlParams_t));
                    break;
                case 9:
                    memcpy(&EAP_ControlParams, &ControlParamSet_Crossover2WaySpeaker, sizeof(LVM_ControlParams_t));
                    break;
                case 10:
                    memcpy(&EAP_ControlParams, &ControlParamSet_CrossoverForSubwoofer, sizeof(LVM_ControlParams_t));
                    break;
                case 11:
                case 12:
                case 13:
                case 14:
                    LVM_Status = EAP_UpdateConfig(&EAP_ControlParams, i_value);
                    break;
                default:
                    XF_CHK_ERR(0, XA_CLIENT_PROXY_CONFIG_NONFATAL_RANGE);
            }
            if (i_value < 11)
            {
                LVM_Status = EAP_SetSampleRateAndNumOfChannels(d);
                if (LVM_Status == LVM_OUTOFRANGE)
                {
                    return LVM_Status;
                }
                LVM_Status = EAP_SetConfig(&EAP_ControlParams);
                if (LVM_SUCCESS != LVM_Status)
                {
                    return LVM_Status;
                }
                LVM_Status = LVM_ClearAudioBuffers(EAP_hInstance);
                if (LVM_SUCCESS != LVM_Status)
                {
                    return LVM_Status;
                }
            }
            else if (LVM_SUCCESS != LVM_Status)
            {
                return XA_CLIENT_PROXY_EXEC_NONFATAL_STATE;
            }

        return XA_NO_ERROR;
        }
    default:
        TRACE(ERROR, _x("Invalid parameter: %X"), i_idx);
        return XA_API_FATAL_INVALID_CMD_TYPE;
    }
}

/* ...retrieve configuration parameter */
static XA_ERRORCODE xa_client_proxy_get_config_param(client_proxy_t *d, WORD32 i_idx, pVOID pv_value)
{
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...make sure pre-initialization is completed */
    XF_CHK_ERR(d->state & XA_CLIENT_PROXY_FLAG_PREINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...process individual configuration parameter */
    switch (i_idx & 0xF)
    {
    case XA_CLIENT_PROXY_CONFIG_PARAM_SAMPLE_RATE:
        /* ...return sample rate */
        *(WORD32 *)pv_value = d->sample_rate;
        return XA_NO_ERROR;

    case XA_CLIENT_PROXY_CONFIG_PARAM_PCM_WIDTH:
        /* ...return current PCM width */
        *(WORD32 *)pv_value = d->pcm_width;
        return XA_NO_ERROR;

    case XA_CLIENT_PROXY_CONFIG_PARAM_CHANNELS:
        /* ...return current channel number */
        *(WORD32 *)pv_value = d->channels;
        return XA_NO_ERROR;

    case XA_CLIENT_PROXY_CONFIG_PARAM_PRODUCED:
        /* ...return no.of produced bytes */
        *(WORD32 *)pv_value = d->produced;
        return XA_NO_ERROR;

    case XA_CLIENT_PROXY_CONFIG_PARAM_INPUT_FRAME_SIZE_US:
    case XA_CLIENT_PROXY_CONFIG_PARAM_OUTPUT_FRAME_SIZE_US:
        *(WORD32 *)pv_value = d->frame_size_us;
        return XA_NO_ERROR;

    default:
        TRACE(ERROR, _x("Invalid parameter: %X"), i_idx);
        return XA_API_FATAL_INVALID_CMD_TYPE;
    }
}

/* ...execution command */
static XA_ERRORCODE xa_client_proxy_execute(client_proxy_t *d, WORD32 i_idx, pVOID pv_value)
{
    XA_ERRORCODE ret;
#ifdef XAF_PROFILE
    clk_t comp_start, comp_stop;
#endif

    XF_CHK_ERR(d, XA_API_FATAL_INVALID_CMD_TYPE);

    XF_CHK_ERR(d->state & XA_CLIENT_PROXY_FLAG_RUNNING, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...process individual command type */
    switch (i_idx)
    {
    case XA_CMD_TYPE_DO_EXECUTE:
        /* ...perform mixing of the channels */
#ifdef XAF_PROFILE
        comp_start = clk_read_start(CLK_SELN_THREAD);
#endif
        ret = xa_client_proxy_do_execute(d);
#ifdef XAF_PROFILE
        comp_stop = clk_read_stop(CLK_SELN_THREAD);
        client_proxy_cycles += clk_diff(comp_stop, comp_start);
#endif
        return ret;

    case XA_CMD_TYPE_DONE_QUERY:
        /* ...check if processing is complete */
        XF_CHK_ERR(pv_value, XA_API_FATAL_INVALID_CMD_TYPE);
        *(WORD32 *)pv_value = (d->state & XA_CLIENT_PROXY_FLAG_COMPLETE ? 1 : 0);
        return XA_NO_ERROR;

    case XA_CMD_TYPE_DO_RUNTIME_INIT:
        return xa_client_proxy_do_runtime_init(d);

    default:
        /* ...unrecognised command */
        TRACE(ERROR, _x("Invalid index: %X"), i_idx);
        return XA_API_FATAL_INVALID_CMD_TYPE;
    }
}

/* ...set number of input bytes */
static XA_ERRORCODE xa_client_proxy_set_input_bytes(client_proxy_t *d, WORD32 i_idx, pVOID pv_value)
{
    UWORD32     size;

    /* ...sanity check - check parameters */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...track index must be valid */
    XF_CHK_ERR(i_idx == 0, XA_API_FATAL_INVALID_CMD_TYPE);

    XF_CHK_ERR(d->state & XA_CLIENT_PROXY_FLAG_POSTINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...input buffer must exist */
    XF_CHK_ERR(d->input, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...input frame length should not be zero (in bytes) */
    XF_CHK_ERR((size = (UWORD32)*(WORD32 *)pv_value) >= 0, XA_CLIENT_PROXY_EXEC_NONFATAL_INPUT);

    /* ...all is correct; set input buffer length in bytes */
    d->input_avail = size;

    return XA_NO_ERROR;
}

/* ...get number of output bytes */
static XA_ERRORCODE xa_client_proxy_get_output_bytes(client_proxy_t *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...sanity check - check parameters */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...track index must be zero */
    XF_CHK_ERR(i_idx == 1, XA_API_FATAL_INVALID_CMD_TYPE);

    XF_CHK_ERR(d->state & XA_CLIENT_PROXY_FLAG_RUNNING, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...output buffer must exist */
    XF_CHK_ERR(d->output, XA_CLIENT_PROXY_EXEC_NONFATAL_OUTPUT);

    /* ...return number of produced bytes */
    *(WORD32 *)pv_value = ((d->state & XA_CLIENT_PROXY_FLAG_OUTPUT) ? d->produced : 0);

    return XA_NO_ERROR;
}

/* ...get number of consumed bytes */
static XA_ERRORCODE xa_client_proxy_get_curidx_input_buf(client_proxy_t *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...sanity check - check parameters */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...track index must be valid */
    XF_CHK_ERR(i_idx == 0, XA_API_FATAL_INVALID_CMD_TYPE);

    XF_CHK_ERR(d->state & XA_CLIENT_PROXY_FLAG_RUNNING, XA_CLIENT_PROXY_EXEC_FATAL_STATE);

    /* ...input buffer must exist */
    XF_CHK_ERR(d->input, XA_CLIENT_PROXY_EXEC_FATAL_INPUT);

    /* ...return number of bytes consumed (always consume fixed-length chunk) */
    *(WORD32 *)pv_value = d->consumed;

    /* ...and reset internally managed number of consumed bytes */
    d->consumed = 0;

    return XA_NO_ERROR;
}

/* ...end-of-stream processing */
static XA_ERRORCODE xa_client_proxy_input_over(client_proxy_t *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...sanity check */
    XF_CHK_ERR(d, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...put end-of-stream flag */
    d->state |= XA_CLIENT_PROXY_FLAG_COMPLETE;

    TRACE(PROCESS, _b("Input-over-condition signalled"));

    return XA_NO_ERROR;
}

/*******************************************************************************
 * Memory information API
 ******************************************************************************/
/* ..get total amount of data for memory tables */
static XA_ERRORCODE xa_client_proxy_get_memtabs_size(client_proxy_t *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...basic sanity checks */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    XF_CHK_ERR(d->state & XA_CLIENT_PROXY_FLAG_PREINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...we have all our tables inside API structure - good? tbd */
    *(WORD32 *)pv_value = 0;

    return XA_NO_ERROR;
}

/* ..set memory tables pointer */
static XA_ERRORCODE xa_client_proxy_set_memtabs_ptr(client_proxy_t *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...basic sanity checks */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    XF_CHK_ERR(d->state & XA_CLIENT_PROXY_FLAG_PREINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...do not do anything; just return success - tbd */
    return XA_NO_ERROR;
}

/* ...return total amount of memory buffers */
static XA_ERRORCODE xa_client_proxy_get_n_memtabs(client_proxy_t *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...basic sanity checks */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...we have 1 input buffer, 1 output buffer and 1 scratch buffer*/
    *(WORD32 *)pv_value = 3;

    return XA_NO_ERROR;
}

/* ...return memory buffer data */
static XA_ERRORCODE xa_client_proxy_get_mem_info_size(client_proxy_t *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...basic sanity check */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...return frame buffer minimal size only after post-initialization is done */
    XF_CHK_ERR(d->state & XA_CLIENT_PROXY_FLAG_POSTINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...all buffers are of the same length */
    *(WORD32 *)pv_value = (WORD32) d->buffer_size;

    return XA_NO_ERROR;
}

/* ...return memory alignment data */
static XA_ERRORCODE xa_client_proxy_get_mem_info_alignment(client_proxy_t *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...basic sanity check */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...return frame buffer minimal size only after post-initialization is done */
    XF_CHK_ERR(d->state & XA_CLIENT_PROXY_FLAG_POSTINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...all buffers are 4-bytes aligned */
    *(WORD32 *)pv_value = 4;

    return XA_NO_ERROR;
}

/* ...return memory type data */
static XA_ERRORCODE xa_client_proxy_get_mem_info_type(client_proxy_t *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...basic sanity check */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...return frame buffer minimal size only after post-initialization is done */
    XF_CHK_ERR(d->state & XA_CLIENT_PROXY_FLAG_POSTINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

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

    case 2:
        /* ...scratch buffer */
        *(WORD32 *)pv_value = XA_MEMTYPE_SCRATCH;
        return XA_NO_ERROR;

    default:
        /* ...invalid index */
        return XF_CHK_ERR(0, XA_API_FATAL_INVALID_CMD_TYPE);
    }
}

/* ...set memory pointer */
static XA_ERRORCODE xa_client_proxy_set_mem_ptr(client_proxy_t *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...basic sanity check */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...codec must be initialized */
    XF_CHK_ERR(d->state & XA_CLIENT_PROXY_FLAG_POSTINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

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

    case 2:
        /* ...scratch buffer */
        d->scratch = pv_value;
        return XA_NO_ERROR;

    default:
        /* ...invalid index */
        return XF_CHK_ERR(0, XA_API_FATAL_INVALID_CMD_TYPE);
    }
}


/*******************************************************************************
 * API command hooks
 ******************************************************************************/

static XA_ERRORCODE (* const xa_client_proxy_api[])(client_proxy_t *, WORD32, pVOID) =
{
    [XA_API_CMD_GET_API_SIZE]           = xa_client_proxy_get_api_size,

    [XA_API_CMD_INIT]                   = xa_client_proxy_init,
    [XA_API_CMD_DEINIT]                 = xa_client_proxy_deinit,
    [XA_API_CMD_SET_CONFIG_PARAM]       = xa_client_proxy_set_config_param,
    [XA_API_CMD_GET_CONFIG_PARAM]       = xa_client_proxy_get_config_param,

    [XA_API_CMD_EXECUTE]                = xa_client_proxy_execute,
    [XA_API_CMD_SET_INPUT_BYTES]        = xa_client_proxy_set_input_bytes,
    [XA_API_CMD_GET_OUTPUT_BYTES]       = xa_client_proxy_get_output_bytes,
    [XA_API_CMD_GET_CURIDX_INPUT_BUF]   = xa_client_proxy_get_curidx_input_buf,
    [XA_API_CMD_INPUT_OVER]             = xa_client_proxy_input_over,

    [XA_API_CMD_GET_MEMTABS_SIZE]       = xa_client_proxy_get_memtabs_size,
    [XA_API_CMD_SET_MEMTABS_PTR]        = xa_client_proxy_set_memtabs_ptr,
    [XA_API_CMD_GET_N_MEMTABS]          = xa_client_proxy_get_n_memtabs,
    [XA_API_CMD_GET_MEM_INFO_SIZE]      = xa_client_proxy_get_mem_info_size,
    [XA_API_CMD_GET_MEM_INFO_ALIGNMENT] = xa_client_proxy_get_mem_info_alignment,
    [XA_API_CMD_GET_MEM_INFO_TYPE]      = xa_client_proxy_get_mem_info_type,
    [XA_API_CMD_SET_MEM_PTR]            = xa_client_proxy_set_mem_ptr,
};

/* ...total number of commands supported */
#define XA_CLIENT_PROXY_API_COMMANDS_NUM   (sizeof(xa_client_proxy_api) / sizeof(xa_client_proxy_api[0]))

/*******************************************************************************
 * API entry point
 ******************************************************************************/

XA_ERRORCODE xa_client_proxy(xa_codec_handle_t p_xa_module_obj, WORD32 i_cmd, WORD32 i_idx, pVOID pv_value)
{
    client_proxy_t *d = (client_proxy_t *) p_xa_module_obj;

    /* ...check if command index is sane */
    XF_CHK_ERR(i_cmd < XA_CLIENT_PROXY_API_COMMANDS_NUM, XA_API_FATAL_INVALID_CMD);

    /* ...see if command is defined */
    XF_CHK_ERR(xa_client_proxy_api[i_cmd], XA_API_FATAL_INVALID_CMD);

    /* ...execute requested command */
    return xa_client_proxy_api[i_cmd](d, i_idx, pv_value);
}

// malloc memory
LVM_INT16               MallocAlign = 4;                        /* 4 byte Malloc alignment */

LVM_ReturnStatus_en EAP_Init(client_proxy_t *d)
{
    volatile LVM_ReturnStatus_en     LVM_Status;                             /* Function call status */
    LVM_UINT16              i;                                      /* loop index */
    LVM_INT32               temp32;                                 /* temporary address */

    // scratch
    LVM_INT16               *pScratchBase = LVM_NULL;
    LVM_UINT32              ScratchSize = 0;

    /******************************************************************************
    GET VERSION INFORMATION
    *******************************************************************************/
    LVM_Status=LVM_GetVersionInfo(&EAP_VersionInfo);
    if(LVM_Status != LVM_SUCCESS)
    {
        return LVM_Status;
    }


    /******************************************************************************
    SET THE INPUT PARAMETERS
    *******************************************************************************/
    /*
     * Select parameter configuration
     */

    memcpy(&EAP_ControlParams, &ControlParamSet_allEffectOff, sizeof(LVM_ControlParams_t)); /* Control Parameters */
    memcpy(&EAP_InstParams, &InstParams_allEffectOff, sizeof(LVM_InstParams_t));            /* Instance parameters */
#ifdef ALGORITHM_EQNB
    memcpy(&EAP_HeadroomParams, &HeadroomParams_allEffectOff, sizeof(LVM_HeadroomParams_t));/* Headroom parameters */
#endif

    LVM_Status = EAP_SetSampleRateAndNumOfChannels(d);
    if (LVM_Status != LVM_SUCCESS)
    {
        return LVM_Status;
    }
    /******************************************************************************
    Allocate memory
    Force alignment by allocating extra memory
    *******************************************************************************/
    LVM_Status = LVM_GetMemoryTable(    LVM_NULL,
                                        &EAP_MemTab,
                                        &EAP_InstParams);
    if (LVM_Status != LVM_SUCCESS)
    {
        return LVM_Status;
    }

    for (i=0; i<LVM_NR_MEMORY_REGIONS; i++)
    {
        /* Log the memory size */
        if (EAP_MemTab.Region[i].Size != 0)
        {
            temp32 = (LVM_INT32)malloc(EAP_MemTab.Region[i].Size + (LVM_UINT32)MallocAlign);
            EAP_MemTab.Region[i].pBaseAddress = (LVM_INT8 *)(temp32 + MallocAlign);
        }
        if (i == LVM_MEMREGION_TEMPORARY_FAST)
        {
            pScratchBase = EAP_MemTab.Region[i].pBaseAddress;
            ScratchSize = EAP_MemTab.Region[i].Size;
        }
    }


    /******************************************************************************
    Initialize
    *******************************************************************************/
    /*
    * Get an EAP Instance
    */
    EAP_InstParams.Platform = PLATFORM_ID;
    EAP_hInstance = LVM_NULL;                                           /* Initialise to NULL */
    LVM_Status = LVM_GetInstanceHandle( &EAP_hInstance,                 /* Init sets the instance handle */
                                        &EAP_MemTab,
                                        &EAP_InstParams);
    if (LVM_Status != LVM_SUCCESS)
    {
        return LVM_Status;
    }


#ifdef ALGORITHM_EQNB
    {
        // set headroom param config
        LVM_Status = LVM_SetHeadroomParams(EAP_hInstance, &EAP_HeadroomParams);
    }
#endif
    return LVM_Status;
}

LVM_ReturnStatus_en EAP_SetConfig(LVM_ControlParams_t *pEAP_ControlParamsSet)
{
    /* Function call status */
    volatile LVM_ReturnStatus_en LVM_Status;

    /******************************************************************************
    Call set control parameters
     - propagate the configuration to EAP
    *******************************************************************************/
    LVM_Status = LVM_SetControlParameters(EAP_hInstance, pEAP_ControlParamsSet);

    return LVM_Status;
}

LVM_ReturnStatus_en EAP_UpdateConfig(LVM_ControlParams_t *pEAP_ControlParamsUpdate, LVM_INT8 paramNum)
{
    volatile LVM_ReturnStatus_en LVM_Status;
    LVM_Status = LVM_GetControlParameters(EAP_hInstance, pEAP_ControlParamsUpdate);
    if (LVM_Status != LVM_SUCCESS)
    {
        return LVM_Status;
    }
    /* Store actual data in case of update failure */
    LVM_INT8 volume = pEAP_ControlParamsUpdate->VC_EffectLevel;
    LVM_INT8 balance = pEAP_ControlParamsUpdate->VC_Balance;
    switch (paramNum)
    {
        case 11:
            pEAP_ControlParamsUpdate->VC_EffectLevel += 2;
            break;
        case 12:
            pEAP_ControlParamsUpdate->VC_EffectLevel -= 2;
            break;
        case 13:
            pEAP_ControlParamsUpdate->VC_Balance -= 2;
            break;
        case 14:
            pEAP_ControlParamsUpdate->VC_Balance += 2;
            break;
    }

    LVM_Status = EAP_SetConfig(pEAP_ControlParamsUpdate);
    if (LVM_Status != LVM_SUCCESS)
    {
        /* Restore saved data */
        EAP_ControlParams.VC_EffectLevel = volume;
        EAP_ControlParams.VC_Balance = balance;
        return LVM_SUCCESS;
    }
    else
    {
        return LVM_Status;
    }
}

LVM_ReturnStatus_en EAP_SetSampleRateAndNumOfChannels(client_proxy_t *d){
    volatile LVM_ReturnStatus_en LVM_Status = LVM_SUCCESS;
    switch (d->sample_rate)
    {
        case 4000:
            LVM_Status = LVM_OUTOFRANGE;
            break;
        case 8000:
            EAP_ControlParams.SampleRate = LVM_FS_8000;
            break;
        case 11025:
            EAP_ControlParams.SampleRate = LVM_FS_11025;
            break;
        case 12000:
            EAP_ControlParams.SampleRate = LVM_FS_12000;
            break;
        case 16000:
            EAP_ControlParams.SampleRate = LVM_FS_16000;
            break;
        case 22050:
            EAP_ControlParams.SampleRate = LVM_FS_22050;
            break;
        case 24000:
            EAP_ControlParams.SampleRate = LVM_FS_24000;
            break;
        case 32000:
            EAP_ControlParams.SampleRate = LVM_FS_32000;
            break;
        case 44100:
            EAP_ControlParams.SampleRate = LVM_FS_44100;
            break;
        case 48000:
            EAP_ControlParams.SampleRate = LVM_FS_48000;
            break;
        case 96000:
            EAP_ControlParams.SampleRate = LVM_FS_96000;
            break;
        case 64000:
        case 88200:
        case 128000:
        case 176400:
        case 192000:
            LVM_Status = LVM_OUTOFRANGE;
            break;
    }
    switch (d->channels)
    {
        case 1:
            EAP_ControlParams.SourceFormat = LVM_MONO;
            break;
        case 2:
            EAP_ControlParams.SourceFormat = LVM_STEREO;
            break;
    }
    if (LVM_Status == LVM_OUTOFRANGE)
    {
        return LVM_Status;
    }
    else
    {
        return LVM_SUCCESS;
    }
}

LVM_ReturnStatus_en EAP_Execute(void * inputBuffer, void * outputBuffer[2], int numSamples, LVM_UINT32 audioTimeMs)
{
    /* Function call status */
    volatile LVM_ReturnStatus_en     LVM_Status;

    LVM_Status = LVM_Process(EAP_hInstance, /* Instance handle */
                             inputBuffer,   /* Input buffer */
#if (defined EAP_PROC)
                             (LVM_INT16 **)outputBuffer, /* Output buffer */
#elif (defined EAP32_PROC)
                             (LVM_INT32 **)outputBuffer, /* Output buffer */
#endif
                             numSamples,   /* Number of samples to process */
                             audioTimeMs); /* Audio Time*/

    return LVM_Status;
}

LVM_ReturnStatus_en EAP_Deinit()
{
    volatile LVM_ReturnStatus_en     LVM_Status;                             /* Function call status */
    LVM_UINT16              i;                                      /* loop index */
    LVM_INT32               temp32;                                 /* temporary address */

    /*
    * Free memory
    */
    LVM_Status = LVM_GetMemoryTable(EAP_hInstance,
                                    &EAP_MemTab,
                                    LVM_NULL);

    for (i=0; i<LVM_NR_MEMORY_REGIONS; i++)
    {
        if (EAP_MemTab.Region[i].Size != 0)
        {
            temp32 = (LVM_INT32)EAP_MemTab.Region[i].pBaseAddress - MallocAlign;
            free((LVM_INT8 *)temp32);
        }
    }
    return LVM_Status;
}
#endif
