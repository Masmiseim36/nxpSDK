/*
 * Copyright (c) 2022 Cadence Design Systems, Inc.
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


#ifdef  SMALL_INPUT_CHUNK_FEED
/**
 * Defining SMALL_INPUT_CHUNK_FEED enbables the test application to 
 * feed the data in small chunks of size CHUNK_SIZE to test
 * the partial input feature of the MP3 Decoder library
 */
#if !defined(CHUNK_SIZE)
#define CHUNK_SIZE 100
#endif
int varChunkSize = CHUNK_SIZE;
#endif

/* Activate this for STUB Testing of REINIT cases */
//#define STREAM_CHANGE_REINIT_TEST  
#ifdef STREAM_CHANGE_REINIT_TEST
/*   Note: Profile and stream info under this STUB will not be updated correctly */
int reInit_err_typ = 0;
int reInit_active = 0;
#endif /* STREAM_CHANGE_REINIT_TEST */

// This macro enables VLC rewind feature in library. This feature 
// can improve the decoder's behaviour on some error streams on which
// VLC overrun happens due to corrupted/wrong huffmann values.
//#define ACTIVATE_VLC_REWIND

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#if __XCC__
#include <xtensa/hal.h>
#include <xtensa/tie/xt_hifi2.h>
#include <xtensa/sim.h>
#endif
#include "xa_type_def.h"
#include "xa_error_standards.h"
#include "xa_error_handler.h"
#include "xa_apicmd_standards.h"
#include "xa_memory_standards.h"
#include "mp3_dec/xa_mp3_dec_api.h"
#include "id3_tag_decode.h"

#if !defined(SHOW_SIZES)
#define SHOW_SIZES 1
#endif  /* SHOW_SIZES */

#ifndef STEP_PLAY
#define STEP_PLAY 0
#endif

#if STEP_PLAY
/* Play PLAY_TIME_MS milliseconds of audio, mute the output for
   MUTE_TIME_MS milliseconds, skip SKIP_TIME_MS milliseconds of audio,
   repeat. */
#define PLAY_TIME_MS 100 /* ms */
#define MUTE_TIME_MS 25  /* ms */
#define SKIP_TIME_MS 20 /* ms */
#endif /* STEP_PLAY */


VOID xa_mp3_dec_error_handler_init();
VOID xa_testbench_error_handler_init();

extern xa_error_info_struct xa_testbench_error_info;
extern xa_error_info_struct xa_mp3_dec_error_info;

VOID MP3DecDisplayCmdLine(void);
#if !defined(WAV_HEADER)
#define WAV_HEADER 1
#endif  /* WAV_HEADER */

#if !defined(ENABLE_ID3)
#define ENABLE_ID3 1
#endif  /* ENABLE_ID3 */

#if !defined(DISPLAY_MESSAGE)
#define DISPLAY_MESSAGE  1
#endif  /* DISPLAY_MESSAGE */

#if !defined(PROFILE) && __XCC__
#define PROFILE 1
#endif

#if !__XCC__ && !__XTENSA_EL__ && !__XTENSA_EB__
#define __XTENSA_EL__ 1
#endif

#if PROFILE
#include <sys/times.h>
#endif

#define MAX_MEM_ALLOCS 100
#define XA_MAX_CMD_LINE_LENGTH 300
#define XA_MAX_ARGS 20
#define PARAMFILE "paramfilesimple.txt"


#define XA_TESTBENCH_MFMAN_FATAL_MEM_ALLOC_FAILED  0xffff8000
#define XA_TESTBENCH_MFMAN_FATAL_FILE_OPEN_FAILED  0xffff8001

pVOID g_pv_arr_alloc_memory[MAX_MEM_ALLOCS];
WORD  g_w_malloc_count;
FILE *g_pf_inp = NULL;
FILE *g_pf_out = NULL;

#if MEM_INPUT
extern const char stream_in[];
extern const int stream_in_size;
const char *stream_in_ptr;
int stream_in_remaining;
#endif




/* Store data in little-endian (Intel) format regardless of the
   target processor byte order. */
void
    xa_fwrite (void *buffer, int size, int nwords, FILE *fp)
{
#if !SUPPRESS_OUTPUT

#if __XTENSA_EL__

    fwrite(buffer, size, nwords, fp);

#else /* !__XTENSA_EL__ */

    int i, j;
    char *pb_buf = (char *) buffer;

    for (i = 0; i < nwords; i++) {
        for (j = size - 1; j >= 0; j--) {
            putc(pb_buf[i * size + j], fp);
        }
    }

#endif /* __XTENSA_EL__ */

#endif /* SUPPRESS_OUTPUT */
}

size_t
    xa_fread (void *ptr, size_t size, size_t nmemb, FILE *stream)
{
#if MEM_INPUT

    /* Assume size is 1. */
    int bytes_to_read = nmemb;
    if (bytes_to_read > stream_in_remaining)
        bytes_to_read = stream_in_remaining;
    memcpy(ptr, stream_in_ptr, bytes_to_read);
    stream_in_ptr += bytes_to_read;
    stream_in_remaining -= bytes_to_read;
    return bytes_to_read;

#else /* MEM_INPUT */

    return fread(ptr, size, nmemb, stream);

#endif /* MEM_INPUT */
}


#if WAV_HEADER

static void
    write16_bits_lh(FILE *fp, WORD32 i)
{
    putc(i & 0xff, fp);
    putc((i >> 8) & 0xff, fp);
}


static void
    write32_bits_lh(FILE *fp, WORD32 i)
{
    write16_bits_lh(fp, (WORD32)(i & 0xffffL));
    write16_bits_lh(fp, (WORD32)((i >> 16) & 0xffffL));
}

WORD32
    write_wav_header (FILE *fp, /* file to write */
    WORD32 pcmbytes, /* total bytes in the wav file */
    WORD32 freq, /* sample rate */
    WORD32 channels, /* output channels */
    WORD32 bits /* bits per sample */)
{
    WORD32 bytes = (bits + 7) / 8;
    fwrite("RIFF", 1, 4, fp); /* label */
    write32_bits_lh(fp, pcmbytes + 44 - 8); /* length in bytes without header */
    fwrite("WAVEfmt ", 1, 8, fp); /* 2 labels */
    write32_bits_lh(fp, 2 + 2 + 4 + 4 + 2 + 2); /* length of PCM format decl area */
    write16_bits_lh(fp, 1); /* is pcm? */
    write16_bits_lh(fp, channels);
    write32_bits_lh(fp, freq);
    write32_bits_lh(fp, freq * channels * bytes); /* bps */
    write16_bits_lh(fp, channels * bytes);
    write16_bits_lh(fp, bits);
    fwrite("data", 1, 4, fp);
    write32_bits_lh(fp, pcmbytes);

    return (ferror(fp) ? -1 : 0);
}

#endif /* WAV_HEADER */

void
    xa_shift_input_buffer (char *buf, int buf_size, int bytes_consumed)
{
    int i;

    if (bytes_consumed <= 0)
        return;

#if __XCC__
    /* Optimize 2-byte aligned data movement. */
    if ((((int)buf | buf_size | bytes_consumed) & 1) == 0)
    {
        /* Optimize 4-byte aligned data movement. */
        if ((((int)buf | buf_size | bytes_consumed) & 2) == 0)
        {
            ae_p16x2s *dst = (ae_p16x2s *)buf;
            ae_p16x2s *src = (ae_p16x2s *)&buf[bytes_consumed];
            for (i = 0; i < (buf_size - bytes_consumed) >> 2; i++)
            {
                dst[i] = src[i];
            }
            return;
        }

        ae_p16s *dst = (ae_p16s *)buf;
        ae_p16s *src = (ae_p16s *)&buf[bytes_consumed];
        for (i = 0; i < (buf_size - bytes_consumed) >> 1; i++)
        {
            dst[i] = src[i];
        }
        return;
    }
#endif

    /* Default, non-aligned data movement. */
    for (i = 0; i < buf_size - bytes_consumed; i++)
    {
        buf[i] = buf[i + bytes_consumed];
    }
}


XA_ERRORCODE
    xa_mp3_dec_set_config_param (int                argc,
    char              *argv[],
    xa_codec_handle_t  p_xa_process_api_obj)
{
    LOOPIDX i;
    XA_ERRORCODE err_code = XA_NO_ERROR;

    /* the process API function */
    xa_codec_func_t *p_xa_process_api = xa_mp3_dec;

    xa_error_info_struct *p_proc_err_info = &xa_mp3_dec_error_info;

    for(i = 0; i < argc; i++)
    {
        /* PCM Word Size (For single input file) */
        if(!strncmp(argv[i], "-pcmsz:", 7))
        {
            pWORD8 pb_arg_val = (pWORD8)(argv[i] + 7);
            UWORD32 ui_pcm_wd_sz = atoi((const char *)pb_arg_val);
            err_code = (*p_xa_process_api)(p_xa_process_api_obj,
                XA_API_CMD_SET_CONFIG_PARAM,
                XA_MP3DEC_CONFIG_PARAM_PCM_WDSZ, &ui_pcm_wd_sz);
            _XA_HANDLE_ERROR(p_proc_err_info, "" , err_code);

        }

        /* CRC check */
        if (!strncmp(argv[i], "-crc:", 5))
        {
            pWORD8 pb_arg_val = (pWORD8)(argv[i] + 5);
            UWORD32 ui_crc_check = atoi((const char *)pb_arg_val);
            err_code = (*p_xa_process_api)(p_xa_process_api_obj,
                XA_API_CMD_SET_CONFIG_PARAM,
                XA_MP3DEC_CONFIG_PARAM_CRC_CHECK, &ui_crc_check);
            _XA_HANDLE_ERROR(p_proc_err_info, "" , err_code);
        }

        /* MCH Enable Flag */
        if (!strncmp(argv[i], "-mch:", 5))
        {
            pWORD8 pb_arg_val = (pWORD8)(argv[i] + 5);
            UWORD32 ui_mch_enable = atoi((const char *)pb_arg_val);
            err_code = (*p_xa_process_api)(p_xa_process_api_obj,
                XA_API_CMD_SET_CONFIG_PARAM,
                XA_MP3DEC_CONFIG_PARAM_MCH_ENABLE, &ui_mch_enable);
            _XA_HANDLE_ERROR(p_proc_err_info, "mch enable error" , err_code);
        }

#ifdef SMALL_INPUT_CHUNK_FEED
        /* chunk-size variable to be set by the user */
        if (!strncmp(argv[i], "-inSize:", 8))
        {
            pWORD8 pb_arg_val = (pWORD8)(argv[i] + 8);
            varChunkSize = atoi((const char *)pb_arg_val);
        }
#endif

        /* Non-standard stream support Flag */
        if (!strncmp(argv[i], "-nonstd:", 8))
        {
            pWORD8 pb_arg_val = (pWORD8)(argv[i] + 8);
            UWORD32 nonstd_support = atoi((const char *)pb_arg_val);
            err_code = (*p_xa_process_api)(p_xa_process_api_obj,
                XA_API_CMD_SET_CONFIG_PARAM,
                XA_MP3DEC_CONFIG_PARAM_NONSTD_STRM_SUPPORT, &nonstd_support);
            _XA_HANDLE_ERROR(p_proc_err_info, "nonstd enable error" , err_code);
        }
    }
    return XA_NO_ERROR;
}

XA_ERRORCODE
    xa_mp3_dec_get_config_param (xa_codec_handle_t p_xa_process_api_obj,
    pWORD32           pi_bitrate,
    pWORD32           pi_samp_freq,
    pWORD32           pi_num_chan,
    pWORD32           pi_pcm_wd_sz
    )
{
    XA_ERRORCODE err_code = XA_NO_ERROR;
    /* the process API function */
    xa_codec_func_t *p_xa_process_api = xa_mp3_dec;
    xa_error_info_struct *p_proc_err_info = &xa_mp3_dec_error_info;

    /* Data rate */
    {
        err_code = (*p_xa_process_api)(p_xa_process_api_obj,
            XA_API_CMD_GET_CONFIG_PARAM,
            XA_MP3DEC_CONFIG_PARAM_BITRATE, pi_bitrate);
        _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

    }
    /* Sampling frequency */
    {
        err_code = (*p_xa_process_api)(p_xa_process_api_obj,
            XA_API_CMD_GET_CONFIG_PARAM,
            XA_MP3DEC_CONFIG_PARAM_SAMP_FREQ, pi_samp_freq);
        _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

    }
    /* Number of channels */
    {
        err_code = (*p_xa_process_api)(p_xa_process_api_obj,
            XA_API_CMD_GET_CONFIG_PARAM,
            XA_MP3DEC_CONFIG_PARAM_NUM_CHANNELS, pi_num_chan);
        _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);
    }
    /* PCM word size */
    {
        err_code = (*p_xa_process_api)(p_xa_process_api_obj,
            XA_API_CMD_GET_CONFIG_PARAM,
            XA_MP3DEC_CONFIG_PARAM_PCM_WDSZ, pi_pcm_wd_sz);
        _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);
    }
    return XA_NO_ERROR;
}


static XA_ERRORCODE
    display_mch_status (xa_codec_handle_t p_xa_process_api_obj)
{
    XA_ERRORCODE err_code = XA_NO_ERROR;
    /* the process API function */
    xa_codec_func_t *p_xa_process_api = xa_mp3_dec;
    xa_error_info_struct *p_proc_err_info = &xa_mp3_dec_error_info;

    /* MCH information */
    /* MCH_STATUS info. */
    {
        int mch_status;
        static const char *mch_status_str[] = {
            "Unsupported",
            "Disabled",
            "Not present",
            "Present"
        };

        err_code = (*p_xa_process_api)(p_xa_process_api_obj,
            XA_API_CMD_GET_CONFIG_PARAM,
            XA_MP3DEC_CONFIG_PARAM_MCH_STATUS, &mch_status);
        _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

        fprintf(stdout, "\nLayer II Multi-Channel Content: %s\n", mch_status_str[mch_status]);
        if (mch_status != XA_MP3DEC_MCH_STATUS_PRESENT)
            return XA_NO_ERROR;
    }

    /* Channel-Config */
    {
        int chan_config;
        err_code = (*p_xa_process_api)(p_xa_process_api_obj,
            XA_API_CMD_GET_CONFIG_PARAM,
            XA_MP3DEC_CONFIG_PARAM_CHAN_CONFIG, &chan_config);
        _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

        fprintf(stdout, "\tChannel Configuration: %d\n", chan_config);
    }

    /* Channel-Map information */
    {
        int chan_map;
        err_code = (*p_xa_process_api)(p_xa_process_api_obj,
            XA_API_CMD_GET_CONFIG_PARAM,
            XA_MP3DEC_CONFIG_PARAM_CHAN_MAP, &chan_map);
        _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

        fprintf(stdout, "\tChannel Map:           %08x    -   ",    chan_map);
        switch (chan_map){
        case 0xFFF43120:
            fprintf(stdout, "[X|X|X|Rs|Ls|C|R|L]");
            break;
        case 0xFFFF3120:
            fprintf(stdout, "[X|X|X|X|S|C|R|L]");
            break;
        case 0xFFFFF120:
            fprintf(stdout, "[X|X|X|X|X|C|R|L]");
            break;
        case 0xFFF65120:
            fprintf(stdout, "[X|X|X|R2|L2|C|R|L]");
            break;
        case 0xFFFF4320:
            fprintf(stdout, "[X|X|X|X|Rs|Ls|R|L]");
            break;
        case 0xFFFFF320:
            fprintf(stdout, "[X|X|X|X|X|S|R|L]");
            break;
        case 0xFFFFFF20:
            fprintf(stdout, "[X|X|X|X|X|X|R|L]");
            break;
        case 0xFFFF6520:
            fprintf(stdout, "[X|X|X|X|R2|L2|R|L]");
            break;
        case 0xFFFFFFF1:
            fprintf(stdout, "[X|X|X|X|X|X|X|M]");
            break;                           
        case 0xFFFFF651:                 
            fprintf(stdout, "[X|X|X|X|X|R2|L2|M]");
            break;
        case 0xFF743120:
            fprintf(stdout, "[X|X|Lfe|Rs|Ls|C|R|L]");
            break;
        case 0xFFF73120:
            fprintf(stdout, "[X|X|X|Lfe|S|C|R|L]");
            break;
        case 0xFFFF7120:
            fprintf(stdout, "[X|X|X|X|Lfe|C|R|L]");
            break;
        case 0xFF765120:
            fprintf(stdout, "[X|X|Lfe|R2|L2|C|R|L]");
            break;
        case 0xFFF74320:
            fprintf(stdout, "[X|X|X|Lfe|Rs|Ls|R|L]");
            break;
        case 0xFFFF7320:
            fprintf(stdout, "[X|X|X|X|Lfe|S|R|L]");
            break;
        case 0xFFFFF720:
            fprintf(stdout, "[X|X|X|X|X|Lfe|R|L]");
            break;
        case 0xFFF76520:
            fprintf(stdout, "[X|X|X|Lfe|R2|L2|R|L]");
            break;
        case 0xFFFFFF71:
            fprintf(stdout, "[X|X|X|X|X|X|Lfe|M]");
            break;                           
        case 0xFFFF7651:                 
            fprintf(stdout, "[X|X|X|X|Lfe|R2|L2|M]");
            break;
        }
        fprintf(stdout, "\n");

    }

    /* Num of extra channels in mch */
    {
        int num_xchans;
        err_code = (*p_xa_process_api)(p_xa_process_api_obj,
            XA_API_CMD_GET_CONFIG_PARAM,
            XA_MP3DEC_CONFIG_PARAM_NUM_XCHANS, &num_xchans);
        _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);
        fprintf(stdout, "\tExtra Channels:        %d\n",
            num_xchans);
    }

    /* LFE */
    {
        int lfe_present;
        err_code = (*p_xa_process_api)(p_xa_process_api_obj,
            XA_API_CMD_GET_CONFIG_PARAM,
            XA_MP3DEC_CONFIG_PARAM_LFE_PRESENT, &lfe_present);
        _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

        fprintf(stdout, "\tLFE Present:           %s\n",
            lfe_present ? "Yes" : "No");
    }

    /* Extension-Stream Presence */
    {
        int extn_present;
        err_code = (*p_xa_process_api)(p_xa_process_api_obj,
            XA_API_CMD_GET_CONFIG_PARAM,
            XA_MP3DEC_CONFIG_PARAM_EXTN_PRESENT, &extn_present);
        _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

        fprintf(stdout, "\tExtension Present:     %s\n",
            extn_present ? "Yes" : "No");
    }

    return XA_NO_ERROR;
}


static unsigned int
    output_wordsize (unsigned int sample_bits)
{
    /* Round up to the next 2-byte size: 16 -> 2; 24 -> 4. */
    return 2 * ((sample_bits + 15) / 16);
}


static void
    pack_32_to_24_bits (pWORD8 pb_out_ptr,
    WORD32 input)
{
#if __XTENSA_EL__
    pb_out_ptr[0] = ((WORD32)input >>  8) & 0xff;
    pb_out_ptr[1] = ((WORD32)input >> 16) & 0xff;
    pb_out_ptr[2] = ((WORD32)input >> 24) & 0xff;
#else  /* __XTENSA_EL__ */
    pb_out_ptr[2] = ((WORD32)input >>  8) & 0xff;
    pb_out_ptr[1] = ((WORD32)input >> 16) & 0xff;
    pb_out_ptr[0] = ((WORD32)input >> 24) & 0xff;
#endif  /* __XTENSA_EL__ */
}


XA_ERRORCODE
    xa_mp3_dec_main_process (WORD32  argc,
    char   *argv[])
{
    LOOPIDX i;
    unsigned int ui_nsamples;
    unsigned int j;

    int frame = 0;

#if DISPLAY_MESSAGE
    /* Library Info and Identification strings */
    WORD8 pb_process_name[30] = "";
    WORD8 pb_lib_version[30] = "";
#endif

    /* Error code */
    XA_ERRORCODE err_code = XA_NO_ERROR;
    XA_ERRORCODE err_code_exec = XA_NO_ERROR;

    /* API obj */
    xa_codec_handle_t xa_process_handle;

    /* First part                                        */
    /* Error Handler Init                                */
    /* Get Library Name, Library Version and API Version */
    /* Initialize API structure + Default config set     */
    /* Set config params from user                       */
    /* Initialize memory tables                          */
    /* Get memory information and allocate memory        */

    /* Memory variables */
    UWORD32 n_mems;
    UWORD32 ui_proc_mem_tabs_size;
    /* API size */
    UWORD32 pui_api_size;
    /* Process initing done query variable */
    UWORD32 ui_init_done, ui_exec_done;
    pWORD8 pb_inp_buf, pb_out_buf;

    UWORD32 ui_inp_size;
    WORD32 i_bytes_consumed, i_bytes_read;
    WORD32 i_buff_size;

    WORD32 i_out_bytes, i_total_bytes = 0;
    WORD32 i_bitrate, i_samp_freq, i_num_chan, i_pcm_wd_sz;

#if ENABLE_ID3
    /* ID3 tag specific declarations */
    UWORD8 id3_buf[128];
    WORD32 id3_v1_found = 0, id3_v1_decoded = 0;
    id3v1_struct id3v1;
    WORD32 cur_pos;
    WORD32 id3_v2_found = 0, id3_v2_complete = 0;
    id3v2_struct id3v2;
#endif
#ifdef SCRATCH_TRASH_TEST
    WORD32 scratch_size;
    void *p_scratch;
#endif
#ifdef OUTPUT_PING_PONG
    WORD8 *p_output_buffer_ping, *p_output_buffer_pong;
    WORD32 out_idx;
#endif /* #ifdef OUTPUT_PING_PONG */

    /* The process API function */
    xa_codec_func_t *p_xa_process_api;
    /* The set config from argc argv */
    XA_ERRORCODE (*p_set_config_param) (int                argc,
        char              *argv[],
        xa_codec_handle_t  p_xa_process_api_obj);

    /* The get config from API */
    XA_ERRORCODE (*p_get_config_param) (xa_codec_handle_t p_xa_process_api_obj,
        pWORD32           pi_bitrate,
        pWORD32           pi_samp_freq,
        pWORD32           pi_num_chan,
        pWORD32           pi_pcm_wd_sz);

    /* The error init function */
    VOID (*p_error_init)();

    /* The process error info structure */
    xa_error_info_struct *p_proc_err_info;

    /* Stack process struct initing */
    p_xa_process_api = xa_mp3_dec;
    p_set_config_param = xa_mp3_dec_set_config_param;
    p_get_config_param = xa_mp3_dec_get_config_param;
    p_error_init = xa_mp3_dec_error_handler_init;
    p_proc_err_info = &xa_mp3_dec_error_info;
    /* Stack process struct initing end */

    /* ******************************************************************/
    /* Initialize the error handler                                     */
    /* ******************************************************************/
    (*p_error_init)();

    /* ******************************************************************/
    /* Get the library name, library version and API version            */
    /* ******************************************************************/

#if DISPLAY_MESSAGE
    /* Get the library name string */
    err_code = (*p_xa_process_api)(NULL, XA_API_CMD_GET_LIB_ID_STRINGS,
        XA_CMD_TYPE_LIB_NAME, pb_process_name);

    _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

    /* Get the library version string */
    err_code = (*p_xa_process_api)(NULL, XA_API_CMD_GET_LIB_ID_STRINGS,
        XA_CMD_TYPE_LIB_VERSION, pb_lib_version);

    _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

    /* Display the Tensilica identification message */
    fprintf(stdout, "\n%s version %s\n", pb_process_name, pb_lib_version);
    fprintf(stdout, "Cadence Design Systems, Inc. https://ip.cadence.com\n\n");
#endif

    /* ******************************************************************/
    /* Initialize API structure and set config params to default        */
    /* ******************************************************************/

    /* Get the API size */
    err_code = (*p_xa_process_api)(NULL, XA_API_CMD_GET_API_SIZE, 0,
        &pui_api_size);

    _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

    /* Allocate memory for API */
    g_pv_arr_alloc_memory[g_w_malloc_count] = malloc(pui_api_size);

    if (g_pv_arr_alloc_memory[g_w_malloc_count] == NULL) {
        _XA_HANDLE_ERROR(&xa_testbench_error_info, "API struct alloc",
            XA_TESTBENCH_MFMAN_FATAL_MEM_ALLOC_FAILED);
    }

    /* API object requires 4 bytes (WORD32) alignment;
    * malloc() provides at least 8-byte alignment.
    */
    assert((((unsigned int) g_pv_arr_alloc_memory[g_w_malloc_count]) & 3) == 0);

    /* Set API object with the memory allocated */
    xa_process_handle = (xa_codec_handle_t) g_pv_arr_alloc_memory[g_w_malloc_count];

    g_w_malloc_count++;

    /* Set the config params to default values */
    err_code = (*p_xa_process_api)(xa_process_handle,
        XA_API_CMD_INIT,
        XA_CMD_TYPE_INIT_API_PRE_CONFIG_PARAMS,
        NULL);

    _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

    /* ******************************************************************/
    /* Set config parameters got from the user present in argc argv     */
    /* ******************************************************************/

    err_code = (*p_set_config_param)(argc, argv, xa_process_handle);
    _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

#ifdef ACTIVATE_VLC_REWIND
    {
        UWORD32 activate_vlc_rewind;     
        activate_vlc_rewind = ACTIVATE_VLC_REWIND;          
        err_code = (*p_xa_process_api)(xa_process_handle,
            XA_API_CMD_SET_CONFIG_PARAM,
            XA_MP3DEC_CONFIG_PARAM_ACTIVATE_VLC_REWIND, 
            &activate_vlc_rewind);
        _XA_HANDLE_ERROR(p_proc_err_info, "" , err_code);
        
    }
#endif    

    /* ******************************************************************/
    /* Initialize Memory info tables                                    */
    /* ******************************************************************/

    /* Get memory info tables size */
    err_code = (*p_xa_process_api)(xa_process_handle,
        XA_API_CMD_GET_MEMTABS_SIZE, 0,
        &ui_proc_mem_tabs_size);

    _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);


    g_pv_arr_alloc_memory[g_w_malloc_count] = malloc(ui_proc_mem_tabs_size);

    if(g_pv_arr_alloc_memory[g_w_malloc_count] == NULL) {
        _XA_HANDLE_ERROR(&xa_testbench_error_info, "Mem tables alloc",
            XA_TESTBENCH_MFMAN_FATAL_MEM_ALLOC_FAILED);
    }

    /* Memory table requires 4 bytes (WORD32) alignment; malloc()
    * provides at least 8-byte alignment.
    */
    assert((((unsigned int) g_pv_arr_alloc_memory[g_w_malloc_count]) & 3) == 0);

    /* Set pointer for process memory tables  */
    err_code = (*p_xa_process_api)(xa_process_handle,
        XA_API_CMD_SET_MEMTABS_PTR, 0,
        (void *) g_pv_arr_alloc_memory[g_w_malloc_count]);

    _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

    g_w_malloc_count++;

    /* initialize the API, post config, fill memory tables  */
    err_code = (*p_xa_process_api)(xa_process_handle,
        XA_API_CMD_INIT,
        XA_CMD_TYPE_INIT_API_POST_CONFIG_PARAMS,
        NULL);

    _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

    /* ******************************************************************/
    /* Allocate Memory with info from library                           */
    /* ******************************************************************/

    /* Get number of memory tables required */
    err_code = (*p_xa_process_api)(xa_process_handle,
        XA_API_CMD_GET_N_MEMTABS,
        0,
        &n_mems);

    _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

    for (i = 0; i < (WORD32) n_mems; i++) {
        int ui_size, ui_alignment, ui_type;
        pVOID pv_alloc_ptr;

        /* Get memory size */
        err_code = (*p_xa_process_api)(xa_process_handle,
            XA_API_CMD_GET_MEM_INFO_SIZE,
            i,
            &ui_size);

        _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

        /* Get memory alignment */
        err_code = (*p_xa_process_api)(xa_process_handle,
            XA_API_CMD_GET_MEM_INFO_ALIGNMENT,
            i,
            &ui_alignment);

        _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

        /* Get memory type */
        err_code = (*p_xa_process_api)(xa_process_handle,
            XA_API_CMD_GET_MEM_INFO_TYPE,
            i,
            &ui_type);

        _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

        g_pv_arr_alloc_memory[g_w_malloc_count] = malloc(ui_size);

        if(g_pv_arr_alloc_memory[g_w_malloc_count] == NULL) {
            _XA_HANDLE_ERROR(&xa_testbench_error_info, "Mem tables alloc",
                XA_TESTBENCH_MFMAN_FATAL_MEM_ALLOC_FAILED);
        }

        /* The following assertion should never fail because malloc() on
        * Xtensa always returns memory aligned on at least an 8-byte
        * boundary.
        */
        assert((((unsigned int) g_pv_arr_alloc_memory[g_w_malloc_count]) % ui_alignment) == 0);

        pv_alloc_ptr = (void *) g_pv_arr_alloc_memory[g_w_malloc_count];

        g_w_malloc_count++;

        /* Set the buffer pointer */
        err_code = (*p_xa_process_api)(xa_process_handle,
            XA_API_CMD_SET_MEM_PTR,
            i,
            pv_alloc_ptr);

        _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

        if(ui_type == XA_MEMTYPE_INPUT) {
            pb_inp_buf = (pWORD8) pv_alloc_ptr;
            ui_inp_size = ui_size;
        }
        if(ui_type == XA_MEMTYPE_OUTPUT) {
            pb_out_buf = (pWORD8) pv_alloc_ptr;
#ifdef OUTPUT_PING_PONG
            p_output_buffer_ping = (pWORD8)pv_alloc_ptr;
            out_idx = i;
            g_pv_arr_alloc_memory[g_w_malloc_count] = malloc(ui_size);
            assert((g_pv_arr_alloc_memory[g_w_malloc_count]) != NULL);
            p_output_buffer_pong = (pWORD8) g_pv_arr_alloc_memory[g_w_malloc_count];
            g_w_malloc_count++;
#endif /* #ifdef OUTPUT_PING_PONG */

        }

#ifdef SCRATCH_TRASH_TEST
        if(ui_type == XA_MEMTYPE_SCRATCH) {
            scratch_size = ui_size;
            p_scratch = (void *)pv_alloc_ptr;
        }
#endif /* #ifdef SCRATCH_TRASH_TEST */

#if SHOW_SIZES
        switch (ui_type) {
        case XA_MEMTYPE_INPUT:
            fprintf(stdout, "Input buffer size: %u bytes\n", ui_size);
            break;
        case XA_MEMTYPE_OUTPUT:
            fprintf(stdout, "Output buffer size: %u bytes\n", ui_size);
            break;
        case XA_MEMTYPE_SCRATCH:
            fprintf(stdout, "Scratch buffer size: %u bytes\n", ui_size);
            break;
        case XA_MEMTYPE_PERSIST:
            fprintf(stdout, "Persistent buffer size: %u bytes\n", ui_size);
            break;
        default:
            assert(0);
        }
#endif  /* SHOW_SIZES */
    }

    /* End first part */

    i_bytes_consumed = ui_inp_size;

#if MEM_INPUT
    stream_in_ptr = stream_in;
    stream_in_remaining = stream_in_size;
#endif

#if ENABLE_ID3
    /* ID3V1 handling */
    {
        cur_pos = ftell(g_pf_inp);

        fseek(g_pf_inp, -128, SEEK_END);
        fread(id3_buf, 1, 128, g_pf_inp);

        /* search for ID3V1 */
        id3_v1_found = search_id3_v1(id3_buf + 0);

        if (id3_v1_found) {
            fprintf(stdout,  "ID3V1 data : \n");

            /* if ID3V1 is found, decode ID3V1 */
            decode_id3_v1(id3_buf + 3, &id3v1);
            id3_v1_decoded = 1;

            fprintf(stdout,  "\n");
        }

        fseek(g_pf_inp, cur_pos, SEEK_SET);
    }

    /* ID3V2 handling */
    {
        WORD32 flag = 0;
        WORD32 continue_flag = 0;
        WORD32 i_fread_bytes;
        pUWORD8 pub_input_ptr;

        i_fread_bytes = fread(pb_inp_buf,
            sizeof(char),
            ui_inp_size,
            g_pf_inp);

        i_bytes_consumed = 0;
        /* search for ID3V2 */
        id3_v2_found = search_id3_v2((pUWORD8)pb_inp_buf);

        if (id3_v2_found) {
            fprintf(stdout,  "ID3V2 data : \n");
            /* initialise the max fields */
            init_id3v2_field(&id3v2);

            while (!id3_v2_complete && id3_v2_found) {
                /* if ID3V2 is found, decode ID3V2 */
                id3_v2_complete = decode_id3_v2((const char *const)pb_inp_buf,
                    &id3v2,
                    continue_flag,
                    i_fread_bytes);

                if (!id3_v2_complete) {
                    continue_flag = 1;
                    i_bytes_consumed = id3v2.bytes_consumed;

                    if (i_bytes_consumed < i_fread_bytes)
                        xa_shift_input_buffer((char *)pb_inp_buf, i_fread_bytes, i_bytes_consumed);

                    fseek(g_pf_inp, i_bytes_consumed, SEEK_SET);

                    pub_input_ptr = (pUWORD8)pb_inp_buf;

                    if ((i_fread_bytes = fread(pub_input_ptr,
                        sizeof(UWORD8),
                        ui_inp_size,
                        g_pf_inp)) <= 0) {
                            fprintf(stdout, "ID3 Tag Decoding: End of file reached.\n");
                            flag = 1;      /* failed */
                            break;
                    }
                    i_buff_size = i_fread_bytes;
                }
            }

            if (id3_v2_complete) {
                fprintf(stdout,  "\n");

                i_bytes_consumed = id3v2.bytes_consumed;

                fseek(g_pf_inp, i_bytes_consumed, SEEK_SET);

                pub_input_ptr = (pUWORD8)pb_inp_buf;

                if ((i_fread_bytes = fread(pub_input_ptr,
                    sizeof(UWORD8),
                    ui_inp_size,
                    g_pf_inp)) <= 0) {
                        fprintf(stdout, "ID3V2 tag decoding: end of file reached.\n");
                        flag = 1;      /* failed */
                }

                i_buff_size = i_fread_bytes;
                i_bytes_consumed = 0;
            }

            if (flag) {
                return XA_NO_ERROR;
            }
        }
    }
#endif


    /* Second part        */
    /* Initialize process */
    /* Get config params  */

    /* ******************************************************************/
    /* Initialize process in a loop (to handle junk data at beginning)  */
    /* ******************************************************************/
    i_buff_size = ui_inp_size;
#ifdef SMALL_INPUT_CHUNK_FEED
    fseek(g_pf_inp, 0, SEEK_SET);
    memset((char *)pb_inp_buf, 0,  ui_inp_size);
    i_bytes_consumed = varChunkSize;
    i_bytes_consumed = (i_bytes_consumed > ui_inp_size)? ui_inp_size:i_bytes_consumed;
    i_buff_size = i_bytes_consumed;
#endif

#ifdef STREAM_CHANGE_REINIT_TEST
    reInit_err_typ = 0;
    reInit_active = 0;
REINIT_POINT:
    frame = 0;
#endif /* STREAM_CHANGE_REINIT_TEST */

    do {
        xa_shift_input_buffer((char *)pb_inp_buf, ui_inp_size, i_bytes_consumed);

#ifdef SMALL_INPUT_CHUNK_FEED
        /* Try and Read at least varChunkSize at time or smaller from the file */
        int i_bytes_requested = (i_bytes_consumed < varChunkSize)?i_bytes_consumed:varChunkSize;
        i_bytes_requested = (i_bytes_requested==0)?varChunkSize:i_bytes_requested;
        i_bytes_requested = (i_bytes_requested > (ui_inp_size-(i_buff_size - i_bytes_consumed)))? \
            ui_inp_size-(i_buff_size - i_bytes_consumed):i_bytes_requested;

        i_bytes_read = xa_fread(pb_inp_buf + (i_buff_size - i_bytes_consumed),
            sizeof(WORD8),
            i_bytes_requested,
            g_pf_inp);
#else 
        i_bytes_read = xa_fread(pb_inp_buf + (ui_inp_size - i_bytes_consumed),
            sizeof(WORD8),
            i_bytes_consumed,
            g_pf_inp);
#endif 

        /* New buffer size */
        i_buff_size = i_buff_size - (i_bytes_consumed - i_bytes_read);

#if 0 //For Debug:  SMALL_INPUT_CHUNK_FEED
        fprintf(stderr, "init buff-lvl %d (%d:%d:%d)\n", i_buff_size, i_bytes_requested, i_bytes_read, i_bytes_consumed ); 
#endif

#ifdef SMALL_INPUT_CHUNK_FEED
        /* Declare input over if bytes read are 0  and there is still space for input filling in the codec input buffer */
        if(( i_buff_size <= 0 ) || ((i_bytes_read == 0) && (i_buff_size<ui_inp_size)) )
#else
       // if( i_buff_size <= 0 ) 
       if(i_bytes_consumed != i_bytes_read)
#endif
        {
#ifdef SMALL_INPUT_CHUNK_FEED
            if (i_buff_size < 0) i_buff_size = 0;
#else
            if (i_buff_size < 0) i_buff_size = 0;
#endif

            /* Tell that the input is over in this buffer */
            err_code = (*p_xa_process_api)(xa_process_handle,
                XA_API_CMD_INPUT_OVER,
                0,
                NULL);

            _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);
        }

        /* Set number of bytes to be processed */
        err_code = (*p_xa_process_api)(xa_process_handle,
            XA_API_CMD_SET_INPUT_BYTES,
            0,
            &i_buff_size);

        _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

        /* Initialize the process */
        err_code = (*p_xa_process_api)(xa_process_handle,
            XA_API_CMD_INIT,
            XA_CMD_TYPE_INIT_PROCESS,
            NULL);

        _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

        /* Checking for end of initialization */
        err_code = (*p_xa_process_api)(xa_process_handle,
            XA_API_CMD_INIT,
            XA_CMD_TYPE_INIT_DONE_QUERY,
            &ui_init_done);

        _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

        /* How much buffer is used in input buffers */
        err_code = (*p_xa_process_api)(xa_process_handle,
            XA_API_CMD_GET_CURIDX_INPUT_BUF,
            0,
            &i_bytes_consumed);

        _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

#if 0 //For Debug:  SMALL_INPUT_CHUNK_FEED
        fprintf(stderr, "end-init buff-lvl %d (%d:%d:%d)\n", i_buff_size, i_bytes_requested, i_bytes_read, i_bytes_consumed ); 
#endif

    } while (!ui_init_done);
    /* Init end of process condition end */

    /* ******************************************************************/
    /* Get config params from API                                       */
    /* ******************************************************************/

    err_code = (*p_get_config_param)(xa_process_handle,
        &i_bitrate,
        &i_samp_freq,
        &i_num_chan,
        &i_pcm_wd_sz);
    _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

    fprintf(stdout, "\nData Rate: %d kbps\n", i_bitrate);
    fprintf(stdout, "Sample Rate: %d Hz\n", i_samp_freq);
    fprintf(stdout, "Number of Channels: %d\n", i_num_chan);
    fprintf(stdout, "PCM Sample Size: %d bits\n", i_pcm_wd_sz);

    {
        int chmode_info;
        err_code = (*p_xa_process_api)(xa_process_handle,
            XA_API_CMD_GET_CONFIG_PARAM,
            XA_MP3DEC_CONFIG_PARAM_CHMODE_INFO,
            &chmode_info);

        _XA_HANDLE_ERROR(p_proc_err_info, "Get Channel Mode Error", err_code);

        static const char *chmode_str[4] = { "Stereo", "Joint Stereo", "Dual Mono", "Mono" };
        static const char *is_str[2] = { "", ", Intensity Stereo" };
        static const char *ms_str[2] = { "", ", Mid-Side Stereo" };
        fprintf(stdout, "Channel Mode Info: 0x%02x (%s%s%s)",
            chmode_info, chmode_str[chmode_info & 0x3],
            is_str[(chmode_info >> 4) & 0x1], ms_str[(chmode_info >> 5) & 0x1]);
    }

    display_mch_status(xa_process_handle);

    /* End second part */

#ifdef STREAM_CHANGE_REINIT_TEST
    /* Write wave header only once when REINT is not active */
    if (!reInit_active) 
#endif /* STREAM_CHANGE_REINIT_TEST */
#if WAV_HEADER
    write_wav_header(g_pf_out, 0x7fffffff, i_samp_freq,
        i_num_chan, i_pcm_wd_sz);
#endif

#if PROFILE
    struct tms start, stop;
    double Peak = 0, Ave = 0, Sum = 0;
    int Peak_frame;
    unsigned long long total_samples = 0;
    int decoded_frame_strm_pos;
    fprintf(stdout, "\n");
#endif

#if STEP_PLAY
    int play_time_ms = 0;
    int mute_bytes = 0;
#endif

    /* Third part for executing each process */
    do {

#if STEP_PLAY
        if (play_time_ms >= PLAY_TIME_MS)
        {
            play_time_ms = 0;

            /* Mute the output for MUTE_TIME_MS milliseconds to separate
            the steps. */
            mute_bytes = ((MUTE_TIME_MS * i_samp_freq)
                / 1000) * i_num_chan * (i_pcm_wd_sz)/8;

            i_total_bytes += mute_bytes;

            /* Based on the current bit rate, calculate the number of
            bytes that need to be skipped after consuming the bytes
            from the last EXECUTE call. */
            int skip_bytes = i_bitrate * SKIP_TIME_MS / 8;

            /* Based on the number of bytes that need to be skipped,
            adjust the consumed input bytes. */
            int buf_remaining = ui_inp_size - i_bytes_consumed;
            int buf_skip = (skip_bytes < buf_remaining) ? skip_bytes : buf_remaining;
            i_bytes_consumed += buf_skip;
            skip_bytes -= buf_skip;

            /* If the input buffer does not contain all bytes that need to
            be skipped, seek forward in the input file. */
            if (skip_bytes > 0)
                fseek(g_pf_inp, skip_bytes, SEEK_CUR);

            int cur_strm_pos;

            /* Read the decoded buffer stream position */
            err_code = (*p_xa_process_api)(xa_process_handle,
                XA_API_CMD_GET_CONFIG_PARAM,
                XA_CONFIG_PARAM_CUR_INPUT_STREAM_POS,
                &cur_strm_pos);

            _XA_HANDLE_ERROR(p_proc_err_info, "Get Input Stream Position Error", err_code);

            cur_strm_pos += (buf_skip + ((skip_bytes>0)? skip_bytes: 0));

            /* Update the stream position */
            err_code = (*p_xa_process_api)(xa_process_handle,
                XA_API_CMD_SET_CONFIG_PARAM,
                XA_CONFIG_PARAM_CUR_INPUT_STREAM_POS,
                &cur_strm_pos);

            _XA_HANDLE_ERROR(p_proc_err_info, "Set Current Input Stream Position Error", err_code);

            /* Runtime init to avoid clicks and distortions. */
            err_code = (*p_xa_process_api)(xa_process_handle,
                XA_API_CMD_EXECUTE,
                XA_CMD_TYPE_DO_RUNTIME_INIT,
                NULL);
            _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

        }
#endif /* STEP_PLAY */

        /* Execute process */

        xa_shift_input_buffer((char *)pb_inp_buf, ui_inp_size, i_bytes_consumed);

#ifdef SMALL_INPUT_CHUNK_FEED
        /* Try and Read at least varChunkSize at time or smaller from the file */
        int i_bytes_requested = (i_bytes_consumed < varChunkSize)? i_bytes_consumed:varChunkSize;
        i_bytes_requested = (i_bytes_requested==0)? varChunkSize:i_bytes_requested;
        i_bytes_requested = (i_bytes_requested > (ui_inp_size-(i_buff_size - i_bytes_consumed)))? 
            ui_inp_size-(i_buff_size - i_bytes_consumed):i_bytes_requested;

        i_bytes_read = xa_fread(pb_inp_buf + (i_buff_size - i_bytes_consumed), 
            sizeof(WORD8),
            i_bytes_requested,
            g_pf_inp);
#else
        i_bytes_read = xa_fread(pb_inp_buf + (ui_inp_size - i_bytes_consumed),
            sizeof(char),
            i_bytes_consumed,
            g_pf_inp);
#endif

        /* New buffer size */
        i_buff_size = i_buff_size - (i_bytes_consumed - i_bytes_read);

#if 0 //For Debug:  SMALL_INPUT_CHUNK_FEED
        fprintf(stderr, "exec buff-lvl %d (%d:%d:%d)\n", i_buff_size, i_bytes_requested, i_bytes_read, i_bytes_consumed ); 
#endif

#ifdef SMALL_INPUT_CHUNK_FEED
        /* Declare input over if bytes read are 0  and there is still space for input filling in the codec input buffer */
        if(( i_buff_size <= 0 ) || ((i_bytes_read == 0) && (i_buff_size<ui_inp_size)) )
#else
        if(i_bytes_consumed != i_bytes_read)
#endif
        {
#ifdef SMALL_INPUT_CHUNK_FEED
            if (i_buff_size < 0) i_buff_size = 0;
#else
            if (i_buff_size < 0) i_buff_size = 0;
#endif

            /* Tell that the input is over in this buffer */
            err_code = (*p_xa_process_api)(xa_process_handle,
                XA_API_CMD_INPUT_OVER,
                0,
                NULL);

            _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);
        }

        /* Set number of bytes to be processed */
        err_code = (*p_xa_process_api)(xa_process_handle,
            XA_API_CMD_SET_INPUT_BYTES,
            0,
            &i_buff_size);

        _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

#ifdef SCRATCH_TRASH_TEST
        memset(p_scratch, 0xca, scratch_size);
#endif /* #ifdef SCRATCH_TRASH_TEST */
#if PROFILE
        xt_iss_client_command("all", "enable");
        times(&start);
#endif

        /* Execute process */
        err_code_exec = (*p_xa_process_api)(xa_process_handle,
            XA_API_CMD_EXECUTE,
            XA_CMD_TYPE_DO_EXECUTE,
            NULL);

#if PROFILE
        times(&stop);
        xt_iss_client_command("all", "disable");
#endif

        _XA_HANDLE_ERROR(p_proc_err_info, "", err_code_exec);

        /* Checking for end of processing */
        err_code = (*p_xa_process_api)(xa_process_handle,
            XA_API_CMD_EXECUTE,
            XA_CMD_TYPE_DONE_QUERY,
            &ui_exec_done);

        _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

        /* Get the output bytes */
        err_code = (*p_xa_process_api)(xa_process_handle,
            XA_API_CMD_GET_OUTPUT_BYTES,
            0,
            &i_out_bytes);

        _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

#ifdef STREAM_CHANGE_REINIT_TEST
        if ((err_code_exec == XA_MP3DEC_EXECUTE_NONFATAL_CHANGED_SAMP_FREQ)
               || (err_code_exec == XA_MP3DEC_EXECUTE_NONFATAL_CHANGED_LAYER))
        {
           /* Check if same err. of CHNAGED_XXX has occurred twice consecutively. */
           if (reInit_err_typ == err_code_exec)
           {
                 fprintf(stderr, "going to REINIT\n"); 
                 reInit_err_typ = 0;
                 reInit_active = 1;
                 goto REINIT_POINT;
           }
        }
        /* Update reInit_err_typ variable if the input buff has sufficient data */
        if (err_code_exec != XA_MP3DEC_EXECUTE_NONFATAL_NEED_MORE)
           reInit_err_typ = err_code_exec;
#endif /* STREAM_CHANGE_REINIT_TEST */

        if (i_out_bytes)
        {
            frame++;
        }

#ifdef METADATA_INFO_API
        if (i_out_bytes && (err_code_exec == XA_NO_ERROR)) 
        {

            int orig_or_copy;
            int copyright;
        
            err_code = (*p_xa_process_api)(xa_process_handle,
                XA_API_CMD_GET_CONFIG_PARAM,
                XA_MP3DEC_CONFIG_PARAM_ORIGINAL_OR_COPY,
	    		&orig_or_copy);
            _XA_HANDLE_ERROR(p_proc_err_info, "Get Metadata: Original or Copy Error", err_code);
            fprintf(stderr, "Metadata: Original(1) or Copy(0) = %d \n", orig_or_copy);

            err_code = (*p_xa_process_api)(xa_process_handle,
                XA_API_CMD_GET_CONFIG_PARAM,
                XA_MP3DEC_CONFIG_PARAM_COPYRIGHT_FLAG,
	    		&copyright);
            _XA_HANDLE_ERROR(p_proc_err_info, "Get Metadata: Copyright_ID Error", err_code);
            fprintf(stderr, "Metadata: Copyright Protected = %d\n", copyright);

			int mch_status;
			err_code = (*p_xa_process_api)(xa_process_handle,
				XA_API_CMD_GET_CONFIG_PARAM,
				XA_MP3DEC_CONFIG_PARAM_MCH_STATUS, &mch_status);
			_XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

            if(mch_status  == 3)
            {
			    xa_mch_ext_hdr_info_t mch_info;

		        err_code = (*p_xa_process_api)(xa_process_handle,
		            XA_API_CMD_GET_CONFIG_PARAM,
		            XA_MP3DEC_CONFIG_PARAM_MCH_EXT_HDR_INFO,
					&mch_info);
		        _XA_HANDLE_ERROR(p_proc_err_info, "Get Metadata: Multichannel Extension Header Info Error", err_code);
		        fprintf(stderr, "Metadata: ext_bit_stream_present = %d\n",         mch_info.ext_bit_stream_present);
				if(mch_info.ext_bit_stream_present){
		        	fprintf(stderr, "Metadata: n_ad_bytes = %d\n",                     mch_info.n_ad_bytes);
				}
		        fprintf(stderr, "Metadata: center = %d\n",                         mch_info.center);
		        fprintf(stderr, "Metadata: surround = %d\n",                       mch_info.surround);
		        fprintf(stderr, "Metadata: lfe = %d\n",                            mch_info.lfe);
		        fprintf(stderr, "Metadata: audio_mix = %d\n",                      mch_info.audio_mix);
		        fprintf(stderr, "Metadata: dematrix_procedure = %d\n",             mch_info.dematrix_procedure);
		        fprintf(stderr, "Metadata: no_of_multi_lingual_ch = %d\n",         mch_info.no_of_multi_lingual_ch);
		        fprintf(stderr, "Metadata: multi_lingual_fs = %d\n",               mch_info.multi_lingual_fs);
		        fprintf(stderr, "Metadata: multi_lingual_layer = %d\n",            mch_info.multi_lingual_layer);

				unsigned char *cpid_ptr = NULL;
		        err_code = (*p_xa_process_api)(xa_process_handle,
		            XA_API_CMD_GET_CONFIG_PARAM,
                    XA_MP3DEC_CONFIG_PARAM_MCH_COPYRIGHT_ID_PTR,
					&cpid_ptr);
                _XA_HANDLE_ERROR(p_proc_err_info, "Get Metadata: Copyright_ID pointer Error", err_code);

				if(cpid_ptr != NULL)
				{
            		fprintf(stderr, "Metadata: Copyright Identifier = %02x \n", (UWORD8)cpid_ptr[0]);
	            	fprintf(stderr, "Metadata: Copyright Number = %02x %02x %02x %02x %02x %02x %02x %02x\n",
								(UWORD8)cpid_ptr[1],
								(UWORD8)cpid_ptr[2],
								(UWORD8)cpid_ptr[3],
								(UWORD8)cpid_ptr[4],
								(UWORD8)cpid_ptr[5],
								(UWORD8)cpid_ptr[6],
								(UWORD8)cpid_ptr[7],
								(UWORD8)cpid_ptr[8]
						);
				}
            }
            fprintf(stderr, "Metadata: ==========================================\n");
		}
#endif /* METADATA_INFO_API */


#if PROFILE

        /* Assume that if no output was produced, no
        * significant cycles were consumed.
        */
        if (i_out_bytes) {
            clock_t cycles = stop.tms_utime - start.tms_utime;
            int samples = i_out_bytes / (output_wordsize(i_pcm_wd_sz) * i_num_chan);
            double Curr = ((double) cycles / samples * i_samp_freq / 1000000);
            Sum += Curr;
            Ave = Sum / frame;

            if (Peak < Curr) {
                Peak = Curr;
                Peak_frame = frame;
            }

            /* Read the decoded buffer stream position */
            err_code = (*p_xa_process_api)(xa_process_handle,
                XA_API_CMD_GET_CONFIG_PARAM,
                XA_CONFIG_PARAM_GEN_INPUT_STREAM_POS,
                &decoded_frame_strm_pos);

            _XA_HANDLE_ERROR(p_proc_err_info, "Get Decoded Frame Stream Position Error", err_code);

            /*
            Calculate the current time based on the total number of
            samples produced by the decoder and the sample rate:

            time [ms] = samples / sample_rate [kHz]

            Alternatively, the current time can be calculated based on
            the total number of bytes consumed by the decoder and the
            data rate:

            time [ms] = 8 * bytes / data_rate [kbps]
            */
            total_samples += samples;

            unsigned long long total_msec =
                (unsigned long long)((double)total_samples / i_samp_freq * 1000.0);
            int msec = (int)(total_msec % 1000);
            unsigned long long total_seconds = total_msec / 1000;
            int seconds = (int)(total_seconds % 60);
            int minutes = (int)(total_seconds / 60);

            fprintf(stdout, "[%d|%u] %d:%02d.%03d MCPS: %.2f Average: %.2f Peak: %.2f @ [%d]\n",
                frame, decoded_frame_strm_pos, minutes, seconds, msec,
                Curr, Ave, Peak, Peak_frame);
        }
#endif

        err_code = (*p_get_config_param)(xa_process_handle,
            &i_bitrate,
            &i_samp_freq,
            &i_num_chan,
            &i_pcm_wd_sz);
        _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

#ifdef DISPLAY_CONFIG_PER_FRAME
        fprintf(stdout, "\nData Rate: %d kbps\n", i_bitrate);
        fprintf(stdout, "Sample Rate: %d Hz\n", i_samp_freq);
        fprintf(stdout, "Number of Channels: %d\n", i_num_chan);
#endif /*DISPLAY_CONFIG_PER_FRAME*/

        ui_nsamples = (i_out_bytes / output_wordsize(i_pcm_wd_sz));
        if (24 == i_pcm_wd_sz)
        {
            for (i = 0, j = 0; i < ui_nsamples; i += 1, j += 3)
            {
                pack_32_to_24_bits(&pb_out_buf[j],
                    ((WORD32 *) pb_out_buf)[i]);
            }
        }

        i_total_bytes += (ui_nsamples * i_pcm_wd_sz) / 8;
        xa_fwrite((pVOID)pb_out_buf, (i_pcm_wd_sz/8), ui_nsamples, g_pf_out);

        /* How much buffer is used in input buffers */
        err_code = (*p_xa_process_api)(xa_process_handle,
            XA_API_CMD_GET_CURIDX_INPUT_BUF,
            0,
            &i_bytes_consumed);

        _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

        assert(i_bytes_consumed <= i_buff_size);

#if STEP_PLAY
        /* Keep track of the play back time. */
        play_time_ms += (ui_nsamples * 1000) / (i_samp_freq * i_num_chan);

        /* Mute the output after ramp-down. */
        if (err_code_exec == XA_NO_ERROR)
        {
            for (; mute_bytes > 0; mute_bytes--)
            {
                putc(0, g_pf_out);
            }
        }
#endif /* STEP_PLAY */
#ifdef OUTPUT_PING_PONG
        if(pb_out_buf == p_output_buffer_pong)
            pb_out_buf = p_output_buffer_ping;
        else
            pb_out_buf = p_output_buffer_pong;
        /* Set the buffer pointers */
        err_code = (*p_xa_process_api)(xa_process_handle, XA_API_CMD_SET_MEM_PTR,
					out_idx, pb_out_buf);
#endif /* #ifdef OUTPUT_PING_PONG */
#if defined(FRAMES) && (FRAMES > 0)
        if (frame >= FRAMES)
            break;
#endif /* FRAMES */

        /* Do till the process execution is done */
    } while(!ui_exec_done);

#if PROFILE
    fprintf(stdout,"\nPeak MCPS = %7.2f\n", Peak);
    fprintf(stdout,"Peak frame = %d\n", Peak_frame);
    fprintf(stdout,"Average MCPS = %7.2f\n", Ave);
#endif

#ifdef STREAM_CHANGE_REINIT_TEST
    /* Force 2 channel wave header (as stub) if REINTs are active */
    if (reInit_active) i_num_chan = 2;
#endif /* STREAM_CHANGE_REINIT_TEST */

#if WAV_HEADER
    if(!fseek(g_pf_out, 0, SEEK_SET)) {
        write_wav_header(g_pf_out,
            i_total_bytes,
            i_samp_freq,
            i_num_chan,
            i_pcm_wd_sz);
    }
#endif

    return XA_NO_ERROR;
} /* End xa_main_process() */



// Set cache attribute to Write Back No Allocate when the last argument is -wbna
void set_wbna(int *argc, char *argv[])
{
    if ( *argc > 1 && !strcmp(argv[*argc-1], "-wbna") ) {
#ifdef __XCC__
        extern char _memmap_cacheattr_wbna_trapnull;
     
        xthal_set_cacheattr((unsigned)&_memmap_cacheattr_wbna_trapnull);
#endif
        (*argc)--;
    }
}

int
    main (int   argc,
    char *argv[])
{
    int err_code = XA_NO_ERROR;

    // NOTE: set_wbna() should be called before any other dynamic
    // adjustment of the region attributes for cache.
    set_wbna(&argc, argv);

    xa_testbench_error_handler_init();

    if(argc == 1)
    {
        char pb_input_file_path[XA_MAX_CMD_LINE_LENGTH] = "";
        char pb_output_file_path[XA_MAX_CMD_LINE_LENGTH] = "";

        char curr_cmd[XA_MAX_CMD_LINE_LENGTH];

        WORD32 fargc, curpos;
        WORD32 processcmd = 0;

        char fargv[XA_MAX_ARGS][XA_MAX_CMD_LINE_LENGTH];
        char *pargv[XA_MAX_ARGS];

        FILE *param_file_id = fopen(PARAMFILE, "r");
        if (param_file_id == NULL)
        {
            MP3DecDisplayCmdLine();

            err_code = XA_TESTBENCH_MFMAN_FATAL_FILE_OPEN_FAILED;
            xa_error_handler(&xa_testbench_error_info,
                "Parameter File", err_code);
            exit(1);
        }

        /* Process one line at a time */
        while(fgets((char *)curr_cmd, XA_MAX_CMD_LINE_LENGTH, param_file_id))
        {
            curpos = 0;
            fargc = 0;
            /* if it is not a param_file command and if */
            /* CLP processing is not enabled */
            if(curr_cmd[0] != '@' && !processcmd)
            {  /* skip it */
                continue;
            }

            while(sscanf((const char *)(curr_cmd + curpos), "%s", fargv[fargc]) != EOF)
            {
                if(fargv[0][0]=='/' && fargv[0][1]=='/')
                    break;
                if(strcmp(fargv[0], "@echo") == 0)
                    break;
                if(strcmp(fargv[fargc], "@New_line") == 0)
                {
                    fgets(curr_cmd + curpos, XA_MAX_CMD_LINE_LENGTH,
                        param_file_id);
                    continue;
                }
                curpos += strlen(fargv[fargc]);
                while(*(curr_cmd + curpos)==' ' || *(curr_cmd + curpos)=='\t')
                    curpos++;
                fargc++;
            }

            if(fargc < 1)  /* for blank lines etc. */
                continue;

            if(strcmp(fargv[0], "@Output_path") == 0)
            {
                if(fargc > 1) strcpy(pb_output_file_path, fargv[1]);
                else strcpy(pb_output_file_path, "");
                continue;
            }

            if(strcmp(fargv[0], "@Input_path") == 0)
            {
                if(fargc > 1) strcpy(pb_input_file_path, fargv[1]);
                else strcpy(pb_input_file_path, "");
                continue;
            }

            if(strcmp(fargv[0], "@Start") == 0)
            {
                processcmd = 1;
                continue;
            }

            if(strcmp(fargv[0], "@Stop") == 0)
            {
                processcmd = 0;
                continue;
            }

            /* otherwise if this a normal command and its enabled for execution */
            if(processcmd)
            {
                int i;

                for(i = 0; i < fargc; i++)
                {
                    fprintf(stdout, "%s ", fargv[i]);
                    pargv[i] = fargv[i];

                    if(!strncmp(fargv[i], "-ifile:", 7))
                    {
                        char *pb_arg_val = fargv[i] + 7;
                        char pb_input_file_name[XA_MAX_CMD_LINE_LENGTH] = "";

                        strcat(pb_input_file_name, pb_input_file_path);
                        strcat(pb_input_file_name, pb_arg_val);

                        g_pf_inp = fopen(pb_input_file_name, "rb");
                        if(g_pf_inp == NULL)
                        {
                            err_code = XA_TESTBENCH_MFMAN_FATAL_FILE_OPEN_FAILED;
                            xa_error_handler(&xa_testbench_error_info,
                                "Input File", err_code);
                        }
                    }

                    if(!strncmp(fargv[i], "-ofile:", 7))
                    {
                        char *pb_arg_val = fargv[i] + 7;
                        char pb_output_file_name[XA_MAX_CMD_LINE_LENGTH] = "";

                        strcat(pb_output_file_name, pb_output_file_path);
                        strcat(pb_output_file_name, pb_arg_val);

                        g_pf_out = fopen(pb_output_file_name, "wb");
                        if(g_pf_out == NULL)
                        {
                            err_code = XA_TESTBENCH_MFMAN_FATAL_FILE_OPEN_FAILED;
                            xa_error_handler(&xa_testbench_error_info,
                                "Output File", err_code);
                        }
                    }

                }
                g_w_malloc_count = 0;
                fprintf(stdout, "\n");

                if(err_code == XA_NO_ERROR)
                    xa_mp3_dec_main_process(fargc, pargv);

                for(i = 0; i < g_w_malloc_count; i++)
                {
                    if(g_pv_arr_alloc_memory[i])
                        free(g_pv_arr_alloc_memory[i]);
                }

                if(g_pf_inp)
                    fclose(g_pf_inp);
                if(g_pf_out)
                    fclose(g_pf_out);

            }
        }
    }
    else

    {
        int i;
        int file_flag = 0;

#if MEM_INPUT
        file_flag++;
#endif

#if SUPPRESS_OUTPUT
        file_flag++;
#endif

        for(i = 1; i < argc; i++)
        {
            fprintf(stdout, "%s ", argv[i]);
            if(!strncmp(argv[i], "-help", 5))
            {
                MP3DecDisplayCmdLine();
                exit(1);
            }
#if !MEM_INPUT
            if(!strncmp(argv[i], "-ifile:", 7))
            {
                char *pb_arg_val = argv[i] + 7;
                char pb_input_file_name[XA_MAX_CMD_LINE_LENGTH] = "";

                file_flag++;
                err_code = XA_NO_ERROR;
                strcat(pb_input_file_name, pb_arg_val);

                g_pf_inp = fopen(pb_input_file_name, "rb");
                if(g_pf_inp == NULL)
                {
                    fprintf(stdout, "Input file: %s\n", pb_input_file_name);
                    err_code = XA_TESTBENCH_MFMAN_FATAL_FILE_OPEN_FAILED;
                    xa_error_handler(&xa_testbench_error_info,
                        "Input File", err_code);
                }
            }
#endif

#if !SUPPRESS_OUTPUT
            if(!strncmp(argv[i], "-ofile:", 7))
            {
                char *pb_arg_val = argv[i] + 7;
                char pb_output_file_name[XA_MAX_CMD_LINE_LENGTH] = "";

                file_flag++;
                strcat(pb_output_file_name, pb_arg_val);

                g_pf_out = fopen(pb_output_file_name, "wb");
                if(g_pf_out == NULL)
                {
                    fprintf(stdout, "Output file: %s\n", pb_output_file_name);
                    err_code = XA_TESTBENCH_MFMAN_FATAL_FILE_OPEN_FAILED;
                    xa_error_handler(&xa_testbench_error_info,
                        "Output File", err_code);
                }
            }
#endif /* SUPPRESS_OUTPUT */
        }
        g_w_malloc_count = 0;
        fprintf(stdout, "\n");
        if(file_flag != 2)
        {
            err_code = XA_TESTBENCH_MFMAN_FATAL_FILE_OPEN_FAILED;
            MP3DecDisplayCmdLine();
            xa_error_handler(&xa_testbench_error_info,
                "Input or Output File", err_code);
        }

        if(err_code == XA_NO_ERROR)
            xa_mp3_dec_main_process(argc - 1, &argv[1]);

        for(i = 0; i < g_w_malloc_count; i++)
        {
            if(g_pv_arr_alloc_memory[i])
                free(g_pv_arr_alloc_memory[i]);
        }

        if(g_pf_inp)
            fclose(g_pf_inp);
        if(g_pf_out)
            fclose(g_pf_out);
    }

    return XA_NO_ERROR;
}

void MP3DecDisplayCmdLine(void)
{
    printf("Usage: MP3ExecFileName -ifile:inputfilename -ofile:outputfilename <optional arguments>\n");
    printf("\n");
    printf("The following optional arguments can be given:\n");
    printf("\t[-pcmsz:]  PCM word size 16 or 24. Default value is 16\n");
    printf("\t[-crc:]    Flag to enable header CRC check. Default value is 0 (No CRC check support for Layer III) \n");
    printf("\t[-mch:]    Flag to enable/disable Multi-Channel decoding. (available only on mp3mch_dec) Default value is 1\n");
    printf("\t[-nonstd:] Relax the standard validity checks for streams. Supported values are:\n");
    printf("\t               0: No relaxation in standard validity checks (Default) \n");
    printf("\t               1: Relax bitrate, mode combination check \n");
    printf("\t[-help]    Prints this text and exit\n");
    printf("\n");
}


