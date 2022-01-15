/*******************************************************************************
* Copyright 2019 NXP
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
 * xa-sbc-encoder.c
 *
 * SBC encoder plugin - thin wrapper around SBCENC library
 ******************************************************************************/

#define MODULE_TAG                      SBCENC

/*******************************************************************************
 * Includes
 ******************************************************************************/

/* Enable just aac and mp3 / opus and vorbis to save memory*/
#if XA_SBC_ENCODER

#include "xf-debug.h"
#include "xa_sbc_enc_api.h"
#include "audio/xa-audio-decoder-api.h"

#ifdef XAF_PROFILE
#include "xaf-clk-test.h"
extern clk_t enc_cycles;
#endif

/*******************************************************************************
 * Tracing configuration
 ******************************************************************************/
/* ...API structure */
typedef struct XASbcEnc
{   
    /* ...channel mode */
    UWORD32                 chmode;

    /* ...sampling rate */
    UWORD32                 sample_rate;
 
} XASbcEnc;


/*******************************************************************************
 * API entry point
 ******************************************************************************/

XA_ERRORCODE xa_sbc_encoder(xa_codec_handle_t p_xa_module_obj, WORD32 i_cmd, WORD32 i_idx, pVOID pv_value)
{
    XA_ERRORCODE ret;
    char * ptr;
    XASbcEnc *sbc_enc_config_struct;

#ifdef XAF_PROFILE
    clk_t comp_start, comp_stop;
#endif


#ifdef XAF_PROFILE
        comp_start = clk_read_start(CLK_SELN_THREAD);
#endif

    /* ...process common audio-decoder commands */
    if(i_cmd == XA_API_CMD_GET_API_SIZE)
    {
        ret = xa_sbc_enc(p_xa_module_obj, i_cmd, i_idx, pv_value);

        *(UWORD32 *)pv_value += sizeof(XASbcEnc);
        
        return ret;
    }
    else if(i_cmd == XA_API_CMD_INIT)
    {
    	if(i_idx == XA_CMD_TYPE_INIT_API_PRE_CONFIG_PARAMS)
    	{
    		/* ...set defaults */
    		sbc_enc_config_struct = (XASbcEnc *)p_xa_module_obj;
    		sbc_enc_config_struct->sample_rate = 48000;
    		sbc_enc_config_struct->chmode = XA_SBC_ENC_CHMODE_MONO;
    	}
    }
    else if(i_cmd == XA_API_CMD_SET_CONFIG_PARAM)
    {
        UWORD32     i_value;

        sbc_enc_config_struct = (XASbcEnc *)p_xa_module_obj;
        i_value = (UWORD32) *(WORD32 *)pv_value;

        if(i_idx == XA_SBC_ENC_CONFIG_PARAM_SAMP_FREQ)
            sbc_enc_config_struct->sample_rate = (UWORD32)i_value;
        else if(i_idx == XA_SBC_ENC_CONFIG_PARAM_CHMODE)
            sbc_enc_config_struct->chmode = (UWORD32)i_value;
    }
    else if(i_cmd == XA_API_CMD_GET_CONFIG_PARAM)
    {
        sbc_enc_config_struct = (XASbcEnc *)p_xa_module_obj;

        if(i_idx == XA_CODEC_CONFIG_PARAM_CHANNELS)
        {
            *(WORD32 *)pv_value = (sbc_enc_config_struct->chmode == XA_SBC_ENC_CHMODE_MONO) ? 1 : 2;
            return XA_NO_ERROR;
        }
        else if((i_idx == XA_CODEC_CONFIG_PARAM_SAMPLE_RATE)||(i_idx == XA_SBC_ENC_CONFIG_PARAM_SAMP_FREQ))
        {
            *(WORD32 *)pv_value = sbc_enc_config_struct->sample_rate;
            return XA_NO_ERROR;
        }
        else if(i_idx == XA_CODEC_CONFIG_PARAM_PCM_WIDTH)
        {
            *(WORD32 *)pv_value = 16;
            return XA_NO_ERROR;
        }
    }

    ptr = (char *)p_xa_module_obj + sizeof(XASbcEnc);   
        
    ret = xa_sbc_enc((xa_codec_handle_t)ptr, i_cmd, i_idx, pv_value);

#ifdef XAF_PROFILE
        comp_stop = clk_read_stop(CLK_SELN_THREAD);
        enc_cycles += clk_diff(comp_stop, comp_start);
#endif
        return ret;
}

#endif
