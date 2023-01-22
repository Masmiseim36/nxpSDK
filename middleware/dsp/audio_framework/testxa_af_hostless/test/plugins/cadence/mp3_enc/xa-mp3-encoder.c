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
 * xa-mp3-encoder.c
 *
 * MP3 encoder plugin - thin wrapper around MP3ENC library
 ******************************************************************************/
#if XA_MP3_ENCODER
#define MODULE_TAG                      MP3ENC

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "xf-debug.h"
#include "audio/xa-audio-decoder-api.h"
#ifndef PACK_WS_DUMMY
#include "xa_mp3_enc_api.h"
#else //PACK_WS_DUMMY
static XA_ERRORCODE xa_mp3_enc(xa_codec_handle_t p_xa_module_obj, WORD32 i_cmd, WORD32 i_idx, pVOID pv_value){return 0;};
#endif //PACK_WS_DUMMY

#ifdef XAF_PROFILE
#include "xaf-clk-test.h"
extern clk_t enc_cycles;
#endif

/* ...API structure */
typedef struct XAMp3Enc
{   
    /* ...number of channels */
    UWORD32                 channels;
    
    /* ...PCM sample width */
    UWORD32                 pcm_width;

    /* ...sampling rate */
    UWORD32                 sample_rate;
 
} XAMp3Enc;


/*******************************************************************************
 * API entry point
 ******************************************************************************/

XA_ERRORCODE xa_mp3_encoder(xa_codec_handle_t p_xa_module_obj, WORD32 i_cmd, WORD32 i_idx, pVOID pv_value)
{
    XA_ERRORCODE ret;
    char * ptr;
    XAMp3Enc *mp3_enc_config_struct;

#ifdef XAF_PROFILE
    clk_t comp_start, comp_stop;
#endif


#ifdef XAF_PROFILE
        comp_start = clk_read_start(CLK_SELN_THREAD);
#endif

    /* ...process common audio-decoder commands */
    if(i_cmd == XA_API_CMD_GET_API_SIZE)
    {
        ret = xa_mp3_enc(p_xa_module_obj, i_cmd, i_idx, pv_value);

        *(UWORD32 *)pv_value += sizeof(XAMp3Enc);
        
        return ret;
    }
#ifndef PACK_WS_DUMMY
    else if(i_cmd == XA_API_CMD_SET_CONFIG_PARAM)
    {
        UWORD32     i_value;

        mp3_enc_config_struct = (XAMp3Enc *)p_xa_module_obj;
        i_value = (UWORD32) *(WORD32 *)pv_value;

        if(i_idx == XA_MP3ENC_CONFIG_PARAM_PCM_WDSZ)
            mp3_enc_config_struct->pcm_width = (UWORD32)i_value;
        else if(i_idx == XA_MP3ENC_CONFIG_PARAM_SAMP_FREQ)
            mp3_enc_config_struct->sample_rate = (UWORD32)i_value;
        else if(i_idx == XA_MP3ENC_CONFIG_PARAM_NUM_CHANNELS)
            mp3_enc_config_struct->channels = (UWORD32)i_value;
    }
    else if(i_cmd == XA_API_CMD_GET_CONFIG_PARAM)
    {
        mp3_enc_config_struct = (XAMp3Enc *)p_xa_module_obj;

        if((i_idx == XA_CODEC_CONFIG_PARAM_CHANNELS)||(i_idx == XA_MP3ENC_CONFIG_PARAM_NUM_CHANNELS))
        {
            *(WORD32 *)pv_value = mp3_enc_config_struct->channels;
            return XA_NO_ERROR;
        }
        else if((i_idx == XA_CODEC_CONFIG_PARAM_SAMPLE_RATE)||(i_idx == XA_MP3ENC_CONFIG_PARAM_SAMP_FREQ))
        {
            *(WORD32 *)pv_value = mp3_enc_config_struct->sample_rate;
            return XA_NO_ERROR;
        }
        else if((i_idx == XA_CODEC_CONFIG_PARAM_PCM_WIDTH)||(i_idx == XA_MP3ENC_CONFIG_PARAM_PCM_WDSZ))
        {
            *(WORD32 *)pv_value = mp3_enc_config_struct->pcm_width;
            return XA_NO_ERROR;
        }
    }
#endif //PACK_WS_DUMMY

    ptr = (char *)p_xa_module_obj + sizeof(XAMp3Enc);   
        
    ret = xa_mp3_enc((xa_codec_handle_t)ptr, i_cmd, i_idx, pv_value);

#if 1 //TENA_2200
    if( (i_cmd == XA_API_CMD_INIT) && (i_idx == XA_CMD_TYPE_INIT_API_PRE_CONFIG_PARAMS))
    {
        mp3_enc_config_struct = (XAMp3Enc *)p_xa_module_obj;

        /* set default values (preinit) */
        mp3_enc_config_struct->sample_rate = 44100;
        mp3_enc_config_struct->channels = 2;
        mp3_enc_config_struct->pcm_width = 16;
    }
#endif

#ifdef XAF_PROFILE
        comp_stop = clk_read_stop(CLK_SELN_THREAD);
        enc_cycles += clk_diff(comp_stop, comp_start);
#endif
        return ret;
}
#endif
