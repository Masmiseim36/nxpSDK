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
 * xa-factory.c
 *
 * DSP processing framework core - component factory
 ******************************************************************************/

#define MODULE_TAG                      FACTORY

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include <stdlib.h>
#include <string.h>
#include "xa_type_def.h"

/* ...platform run-time */
#include "xaf-test.h"

/* ...debugging facility */
#include "xf-debug.h"
#include "xaf-api.h"
#include "xa_type_def.h"

/*******************************************************************************
 * Local types definitions
 ******************************************************************************/

/* ...component descriptor */
typedef struct xf_component_id
{
    /* ...class id (string identifier) */
    const char         *id;

    /* ...class constructor */
    void *  (*factory)(UWORD32 core, xa_codec_func_t process, xaf_comp_type comp_type);

    /* ...component API function */
    xa_codec_func_t    *process;

}   xf_component_id_t;

/*******************************************************************************
 * External functions
 ******************************************************************************/

/* ...components API functions */
extern XA_ERRORCODE xa_mp3_decoder(xa_codec_handle_t, WORD32, WORD32, pVOID);
extern XA_ERRORCODE xa_hotword_decoder(xa_codec_handle_t, WORD32, WORD32, pVOID);
extern XA_ERRORCODE xa_aac_decoder(xa_codec_handle_t, WORD32, WORD32, pVOID);
extern XA_ERRORCODE xa_mixer(xa_codec_handle_t, WORD32, WORD32, pVOID);
extern XA_ERRORCODE xa_pcm_gain(xa_codec_handle_t, WORD32, WORD32, pVOID);
extern XA_ERRORCODE xa_client_proxy(xa_codec_handle_t, WORD32, WORD32, pVOID);
extern XA_ERRORCODE xa_voice_seeker(xa_codec_handle_t, WORD32, WORD32, pVOID);
extern XA_ERRORCODE xa_vit_pre_proc(xa_codec_handle_t, WORD32, WORD32, pVOID);
extern XA_ERRORCODE xa_mp3_encoder(xa_codec_handle_t, WORD32, WORD32, pVOID);
extern XA_ERRORCODE xa_amr_wb_decoder(xa_codec_handle_t, WORD32, WORD32, pVOID);
extern XA_ERRORCODE xa_src_pp_fx(xa_codec_handle_t, WORD32, WORD32, pVOID);
extern XA_ERRORCODE xa_renderer(xa_codec_handle_t , WORD32 , WORD32 , pVOID);
extern XA_ERRORCODE xa_capturer_dmic(xa_codec_handle_t , WORD32 , WORD32 , pVOID);
extern XA_ERRORCODE xa_capturer_i2s(xa_codec_handle_t , WORD32 , WORD32 , pVOID);
extern XA_ERRORCODE xa_vorbis_decoder(xa_codec_handle_t, WORD32, WORD32, pVOID);
extern XA_ERRORCODE xa_dummy_aec22(xa_codec_handle_t, WORD32, WORD32, pVOID);
extern XA_ERRORCODE xa_dummy_aec23(xa_codec_handle_t, WORD32, WORD32, pVOID);
extern XA_ERRORCODE xa_pcm_split(xa_codec_handle_t, WORD32, WORD32, pVOID);
extern XA_ERRORCODE xa_mimo_mix(xa_codec_handle_t, WORD32, WORD32, pVOID);
extern XA_ERRORCODE xa_pcm_stereo(xa_codec_handle_t, WORD32, WORD32, pVOID);
extern XA_ERRORCODE xa_opus_encoder(xa_codec_handle_t, WORD32, WORD32, pVOID);
extern XA_ERRORCODE xa_microspeech_fe(xa_codec_handle_t, WORD32, WORD32, pVOID);
extern XA_ERRORCODE xa_microspeech_inference(xa_codec_handle_t, WORD32, WORD32, pVOID);
extern XA_ERRORCODE xa_person_detect_inference(xa_codec_handle_t, WORD32, WORD32, pVOID);
extern XA_ERRORCODE xa_opus_decoder(xa_codec_handle_t, WORD32, WORD32, pVOID);
extern XA_ERRORCODE xa_sbc_encoder(xa_codec_handle_t, WORD32, WORD32, pVOID);
extern XA_ERRORCODE xa_sbc_decoder(xa_codec_handle_t, WORD32, WORD32, pVOID);

/* ...component class factories */
extern void * xa_audio_codec_factory(UWORD32 core, xa_codec_func_t process, xaf_comp_type comp_type);
extern void * xa_mixer_factory(UWORD32 core, xa_codec_func_t process, xaf_comp_type comp_type);
extern void * xa_renderer_factory(UWORD32 core, xa_codec_func_t process,xaf_comp_type comp_type);
extern void * xa_capturer_factory(UWORD32 core, xa_codec_func_t process,xaf_comp_type comp_type);
extern void * xa_mimo_proc_factory(UWORD32 core, xa_codec_func_t process, xaf_comp_type comp_type);

/*******************************************************************************
 * Local constants definitions
 ******************************************************************************/

const char *comp_id[] = {"audio-decoder",
                         "audio-encoder",
                         "mixer",
                         "pre-proc",
                         "post-proc",
                         "renderer",
                         "capturer",
                         "mimo-proc12",
                         "mimo-proc21",
                         "mimo-proc22",
                         "mimo-proc23",
                         "mimo-proc10",
                         "mimo-proc11",
            };

/* ...component class id */
static const xf_component_id_t xf_component_id[] =
{
#if XA_MP3_DECODER
    { "audio-decoder/mp3",       xa_audio_codec_factory,     xa_mp3_decoder },
#endif
#if XA_HOTWORD_DECODER
    { "audio-decoder/hotword",   xa_audio_codec_factory,     xa_hotword_decoder },
#endif
#if XA_MIXER
    { "mixer",                   xa_mixer_factory,           xa_mixer },
#endif
#if XA_PCM_GAIN
    { "post-proc/pcm_gain",      xa_audio_codec_factory,     xa_pcm_gain },
#endif
#if XA_CLIENT_PROXY
    { "post-proc/client_proxy",      xa_audio_codec_factory,     xa_client_proxy },
#endif
#if XA_VOICE_SEEKER
	{ "pre-proc/voice_seeker",      xa_audio_codec_factory,     xa_voice_seeker },
#endif
#if XA_VIT_PRE_PROC
    { "pre-proc/vit_pre_proc",      xa_audio_codec_factory,     xa_vit_pre_proc },
#endif
#if XA_MP3_ENCODER
    { "audio-encoder/mp3",       xa_audio_codec_factory,     xa_mp3_encoder },
#endif
#if XA_AMR_WB_DEC
    { "audio-decoder/amr-wb",    xa_audio_codec_factory,     xa_amr_wb_decoder },
#endif
#if XA_AAC_DECODER
    { "audio-decoder/aac",      xa_audio_codec_factory,     xa_aac_decoder },
#endif
#if XA_RENDERER
    { "renderer",              xa_renderer_factory,        xa_renderer },
#endif
#if XA_CAPTURER
    { "capturer_dmic",              xa_capturer_factory,        xa_capturer_dmic },
    { "capturer_i2s",              xa_capturer_factory,        xa_capturer_i2s },
#endif
#if XA_SRC_PP_FX
    { "post-proc/src-pp",        xa_audio_codec_factory,     xa_src_pp_fx },
#endif
#if XA_VORBIS_DECODER
    { "audio-decoder/vorbis",       xa_audio_codec_factory,     xa_vorbis_decoder },
#endif
#if XA_AEC22
    { "mimo-proc22/aec22",      xa_mimo_proc_factory,       xa_dummy_aec22 },
#endif
#if XA_AEC23
    { "mimo-proc23/aec23",      xa_mimo_proc_factory,       xa_dummy_aec23 },
#endif
#if XA_PCM_SPLIT
    { "mimo-proc12/pcm_split",  xa_mimo_proc_factory,       xa_pcm_split },
#endif
#if XA_MIMO_MIX
    { "mimo-proc21/mimo_mix",    xa_mimo_proc_factory,       xa_mimo_mix },
#endif
#if XA_OPUS_ENCODER
    { "audio-encoder/opus",       xa_audio_codec_factory,     xa_opus_encoder},
#endif
#if XA_TFLM_MICROSPEECH
    { "post-proc/microspeech_fe",      xa_audio_codec_factory,    xa_microspeech_fe },
    { "post-proc/microspeech_inference",      xa_audio_codec_factory,    xa_microspeech_inference },
#endif
#if XA_TFLM_PERSON_DETECT
    { "post-proc/person_detect_inference",      xa_audio_codec_factory,    xa_person_detect_inference },
#endif
#if XA_OPUS_DECODER
    { "audio-decoder/opus",       xa_audio_codec_factory,     xa_opus_decoder},
#endif
#if XA_PCM_STEREO
    { "post-proc/pcm_stereo",      xa_audio_codec_factory,     xa_pcm_stereo },
#endif
#if XA_SBC_ENCODER
    { "audio-encoder/sbc",       xa_audio_codec_factory,     xa_sbc_encoder},
#endif
#if XA_SBC_DECODER
    { "audio-decoder/sbc",       xa_audio_codec_factory,     xa_sbc_decoder},
#endif
};

/* ...number of items in the map */
#define XF_COMPONENT_ID_MAX     (sizeof(xf_component_id) / sizeof(xf_component_id[0]))

/*******************************************************************************
 * Global definition
 ******************************************************************************/
const int xf_io_ports[XAF_MAX_COMPTYPE][2] =
{
    {1, 1},     /* XAF_DECODER      */
    {1, 1},     /* XAF_ENCODER      */
    {4, 1},     /* XAF_MIXER        */
    {1, 1},     /* XAF_PRE_PROC     */
    {1, 1},     /* XAF_POST_PROC    */
    {1, 0},     /* XAF_RENDERER     */
    {0, 1},     /* XAF_CAPTURER     */
    {1, 2},     /* XAF_MIMO_PROC_12 */
    {2, 1},     /* XAF_MIMO_PROC_21 */
    {2, 2},     /* XAF_MIMO_PROC_22 */
    {2, 3},     /* XAF_MIMO_PROC_23 */
    {1, 0},     /* XAF_MIMO_PROC_10 */
    {1, 1},     /* XAF_MIMO_PROC_11 */
};

/*******************************************************************************
 * Enry points
 ******************************************************************************/

void * xf_component_factory(UWORD32 core, xf_id_t id, UWORD32 length)
{
    UWORD32     i;
    xaf_comp_type comp_type;

    /* ...find component-id in static map */
    for (i = 0; i < XF_COMPONENT_ID_MAX; i++)
    {
        for(comp_type = XAF_DECODER; comp_type < XAF_MAX_COMPTYPE; comp_type++)
        {
            if(NULL != strstr(xf_component_id[i].id, comp_id[comp_type]))
                break;
        }

        /* ...symbolic search - not too good; would prefer GUIDs in some form */
        if (!strncmp(id, xf_component_id[i].id, length))
        {
            /* ...pass control to specific class factory */
            return xf_component_id[i].factory(core, xf_component_id[i].process, comp_type);
        }
    }

    /* ...component string id is not recognized */
    TRACE(ERROR, _b("Unknown component type: %s"), id);

    return NULL;
}
