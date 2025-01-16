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


/*****************************************************************************/
/* File includes                                                             */
/*****************************************************************************/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <xtensa/sim.h>
#include "xa_type_def.h"
#include "xa_error_standards.h"
#include "xa_error_handler.h"
#include "xa_apicmd_standards.h"
#include "xa_memory_standards.h"
#include "mp3_enc/xa_mp3_enc_api.h"

#ifdef __XCC__
#include <xtensa/hal.h>
#endif

#if !defined(SHOW_SIZES)
#define SHOW_SIZES 1
#endif /* SHOW_SIZES */

VOID xa_mp3_enc_error_handler_init();
VOID xa_testbench_error_handler_init();
VOID MP3EncDisplayCmdLine(VOID);

extern xa_error_info_struct xa_testbench_error_info;
extern xa_error_info_struct xa_mp3_enc_error_info;


/*****************************************************************************/
/* Process select hash defines                                               */
/*****************************************************************************/
#define WAV_READER
#define DISPLAY_MESSAGE

/*****************************************************************************/
/* Constant hash defines                                                     */
/*****************************************************************************/
#define MAX_MEM_ALLOCS 100
#define XA_MAX_CMD_LINE_LENGTH 300
#define XA_MAX_ARGS 20
#define XA_SCREEN_WIDTH 80
#define PARAMFILE "paramfilesimple.txt"

/*****************************************************************************/
/* Error codes for the testbench                                             */
/*****************************************************************************/
#define XA_TESTBENCH_MFMAN_FATAL_MEM_ALLOC_FAILED         0xFFFF8000
#define XA_TESTBENCH_MFMAN_FATAL_FILE_OPEN_FAILED         0xFFFF8001
#define XA_TESTBENCH_FATAL_INVALID_WAV_HEADER             0xFFFF8002

/*****************************************************************************/
/* Global variables                                                          */
/*****************************************************************************/
pVOID g_pv_arr_alloc_memory[MAX_MEM_ALLOCS];
WORD  g_w_malloc_count;
FILE *g_pf_inp, *g_pf_out;

#define _PROFILE
#ifdef _PROFILE
#include <sys/times.h>
struct tms start, stop;
clock_t cycles;
double Peak, Curr, Ave, Sum;
WORD32 Peak_frame, frmCnt;
#endif

/*****************************************************************************/
/*                                                                           */
/*  Function name : xa_fread                                                 */
/*                                                                           */
/*  Description   : read little endian data irrespective of the endiannness  */
/*                  of the data                                              */
/*                                                                           */
/*  Inputs        : none                                                     */
/*                                                                           */
/*  Globals       : pVOID buffer[]                                           */
/*                  int size                                                 */
/*                  int nwords                                               */
/*                  FILE *fp                                                 */
/*                                                                           */
/*  Processing    : read little endian data irrespective of the endiannness  */
/*                  of the data                                              */
/*                                                                           */
/*  Outputs       : none                                                     */
/*                                                                           */
/*  Returns       : none                                                     */
/*                                                                           */
/*  Issues        : none                                                     */
/*                                                                           */
/*  Revision history :                                                       */
/*                                                                           */
/*        DD MM YYYY       Author                Changes                     */
/*                                                                           */
/*****************************************************************************/

#if __XTENSA_EL__

int xa_fread(
        pVOID buffer[],
        int size,
        int nwords,
        FILE *fp)
{
    nwords = fread(buffer, size, nwords, fp);
    return (size * nwords);
}

#else

int xa_fread(
        pVOID buffer[],
        int size,
        int nwords,
        FILE *fp)
{
    int i, k;
    pWORD8 pb_buf = (pWORD8)buffer;

    for (i = 0; i < nwords; i++)
    {
        for (k = size - 1; k >= 0; k--)
        {
            pb_buf[i*size+k] = getc(fp);
            if (feof(fp)) break;
        }
        if (feof(fp)) break;
    }

    return (i * size);
}

#endif

/*****************************************************************************/
/*                                                                           */
/*  Function name : wav_header_decode                                        */
/*                                                                           */
/*  Description   : Read wav header and write to config params               */
/*                                                                           */
/*  Inputs        : xa_wav_reader_state_struct *p_state (wav reader state)   */
/*                  WORD8 *inptr (header data)                               */
/*                                                                           */
/*  Globals       : none                                                     */
/*                                                                           */
/*  Processing    : Read  wav header                                         */
/*                                                                           */
/*  Outputs       : none                                                     */
/*                                                                           */
/*  Returns       : none                                                     */
/*                                                                           */
/*  Issues        : none                                                     */
/*                                                                           */
/*  Revision history :                                                       */
/*                                                                           */
/*        DD MM YYYY       Author                Changes                     */
/*                                                                           */
/*****************************************************************************/

XA_ERRORCODE wav_header_decode(
        FILE *in_file,
        UWORD32 *n_channels,
        UWORD32 *sample_rate,
        UWORD32 *pcm_sz,
        WORD32 *length)
{
    WORD8 wav_hdr[40];
    WORD8 data_start[4];
    WORD16 num_ch;
    UWORD32   f_samp;
    WORD16 output_format;
    WORD32 check, count = 0;
    WORD32 len;

    if(fread(wav_hdr, 1, 40, in_file) != 40)
        return XA_TESTBENCH_FATAL_INVALID_WAV_HEADER;

    if(wav_hdr[0]  != 'R' && wav_hdr[1]  != 'I' && wav_hdr[2]  != 'F'
            && wav_hdr[3]  != 'F')
    {
        return XA_TESTBENCH_FATAL_INVALID_WAV_HEADER;
    }

    if(wav_hdr[20] != 01 && wav_hdr[21] != 00)
    {
        return XA_TESTBENCH_FATAL_INVALID_WAV_HEADER;
    }
    num_ch = (WORD16)((UWORD8)wav_hdr[23] * 256 + (UWORD8)wav_hdr[22]);
    f_samp = ((UWORD8)wav_hdr[27] * 256 * 256 * 256);
    f_samp += ((UWORD8)wav_hdr[26] * 256 * 256);
    f_samp += ((UWORD8)wav_hdr[25] * 256);
    f_samp += ((UWORD8)wav_hdr[24]);

    output_format  = ((UWORD8)wav_hdr[35] * 256);
    output_format += ((UWORD8)wav_hdr[34]);

    *n_channels    = num_ch;
    *sample_rate   = f_samp;
    *pcm_sz = output_format;

    data_start[0] = wav_hdr[36];
    data_start[1] = wav_hdr[37];
    data_start[2] = wav_hdr[38];
    data_start[3] = wav_hdr[39];

    check = 1;
    while(check)
    {
        if( data_start[0] == 'd' && data_start[1] == 'a' &&
                data_start[2] == 't' &&  data_start[3] == 'a')
        {
            fread(data_start, 1, 4, in_file);
            len = ((UWORD8)data_start[3] * 256 * 256 * 256);
            len += ((UWORD8)data_start[2] * 256 * 256);
            len += ((UWORD8)data_start[1] * 256);
            len += ((UWORD8)data_start[0]);
            *length = len;
            check = 0;
        }
        else
        {

            data_start[0] = data_start[1];
            data_start[1] = data_start[2];
            data_start[2] = data_start[3];
            if (!fread(&data_start[3],1,1,in_file))
                return XA_TESTBENCH_FATAL_INVALID_WAV_HEADER;
        }
        count++;
        if(count > 80)
        {
            *length = 0xffffffff;
            return(XA_TESTBENCH_FATAL_INVALID_WAV_HEADER);

        }
    }
    return XA_NO_ERROR;
}

/*****************************************************************************/
/*                                                                           */
/*  Function name : xa_mp3_enc_set_config_param                              */
/*                                                                           */
/*  Description   : Set config parameters                                    */
/*                                                                           */
/*  Inputs        : pVOID p_xa_process_api_obj (process API obj)             */
/*                  WORD32 argc (Arguments count)                            */
/*                  char *argv[] (Argument strings)                          */
/*                                                                           */
/*  Globals       : none                                                     */
/*                                                                           */
/*  Processing    : Set config params inside API                             */
/*                                                                           */
/*  Outputs       : none                                                     */
/*                                                                           */
/*  Returns       : XA_ERRORCODE error_value  (Error value)                  */
/*                                                                           */
/*  Issues        : none                                                     */
/*                                                                           */
/*  Revision history :                                                       */
/*                                                                           */
/*        DD MM YYYY       Author                Changes                     */
/*                                                                           */
/*****************************************************************************/

XA_ERRORCODE xa_mp3_enc_set_config_param(int                argc,
        char              *argv[],
        xa_codec_handle_t  p_xa_process_api_obj,
        UWORD32           *pui_samp_freq,
        UWORD32           *pui_num_chan,
        UWORD32           *pui_pcm_wd_sz
#ifdef ENABLE_CUT_OFF_FREQ_CONFIG
        ,UWORD32           *pui_cut_off_freq
#endif // ENABLE_CUT_OFF_FREQ_CONFIG
        )
{
    LOOPIDX i;
    XA_ERRORCODE err_code = XA_NO_ERROR;
    xa_codec_func_t *p_xa_process_api = xa_mp3_enc;
    xa_error_info_struct *p_proc_err_info = &xa_mp3_enc_error_info;

    for(i = 0; i < argc; i++) {
        /* PCM Word Size (For single input file) */
        if(!strncmp(argv[i], "-pcmsz:", 7))
        {
            char *pb_arg_val = argv[i] + 7;
            *pui_pcm_wd_sz = atoi(pb_arg_val);
            err_code = (*p_xa_process_api)(p_xa_process_api_obj,
                    XA_API_CMD_SET_CONFIG_PARAM,
                    XA_MP3ENC_CONFIG_PARAM_PCM_WDSZ, pui_pcm_wd_sz);
            _XA_HANDLE_ERROR(p_proc_err_info, "" , err_code);

        }
        /* Sampling Frequency */
        if(!strncmp(argv[i], "-fs:", 4))
        {
            char *pb_arg_val = argv[i] + 4;
            *pui_samp_freq = atoi(pb_arg_val);
            err_code = (*p_xa_process_api)(p_xa_process_api_obj,
                    XA_API_CMD_SET_CONFIG_PARAM,
                    XA_MP3ENC_CONFIG_PARAM_SAMP_FREQ, pui_samp_freq);
            _XA_HANDLE_ERROR(p_proc_err_info, "" , err_code);
        }
        /* Total Number of Channels */
        if(!strncmp(argv[i], "-chans:", 7))
        {
            char *pb_arg_val = argv[i] + 7;
            *pui_num_chan = atoi(pb_arg_val);
            err_code = (*p_xa_process_api)(p_xa_process_api_obj,
                    XA_API_CMD_SET_CONFIG_PARAM,
                    XA_MP3ENC_CONFIG_PARAM_NUM_CHANNELS, pui_num_chan);
            _XA_HANDLE_ERROR(p_proc_err_info, "" , err_code);

        }
        /* stream bit rate */
        if(!strncmp(argv[i], "-br:", 4))
        {
            char *pb_arg_val = argv[i] + 4;
            UWORD32 ui_bitrate = atoi(pb_arg_val);
            err_code = (*p_xa_process_api)(p_xa_process_api_obj,
                    XA_API_CMD_SET_CONFIG_PARAM,
                    XA_MP3ENC_CONFIG_PARAM_BITRATE, &ui_bitrate);
            _XA_HANDLE_ERROR(p_proc_err_info, "" , err_code);

        }
#ifdef ENABLE_CUT_OFF_FREQ_CONFIG
        /* high cut of freq */
        if(!strncmp(argv[i], "-cf:", 4))
        {
            char *pb_arg_val = argv[i] + 4;
            *pui_cut_off_freq = atoi(pb_arg_val);
        }
#endif // ENABLE_CUT_OFF_FREQ_CONFIG
    }

    return XA_NO_ERROR;
}

/*****************************************************************************/
/*                                                                           */
/*  Function name : xa_mp3_enc_get_config_param                              */
/*                                                                           */
/*  Description   : Get config parameters                                    */
/*                                                                           */
/*  Inputs        : pVOID p_xa_process_api_obj (process API obj)             */
/*                  WORD32 argc (Arguments count)                            */
/*                  char *argv[] (Argument strings)                          */
/*                                                                           */
/*  Globals       : none                                                     */
/*                                                                           */
/*  Processing    : Get config params from API                               */
/*                                                                           */
/*  Outputs       : none                                                     */
/*                                                                           */
/*  Returns       : XA_ERRORCODE error_value  (Error value)                  */
/*                                                                           */
/*  Issues        : none                                                     */
/*                                                                           */
/*  Revision history :                                                       */
/*                                                                           */
/*        DD MM YYYY       Author                Changes                     */
/*                                                                           */
/*****************************************************************************/

XA_ERRORCODE xa_mp3_enc_get_config_param(xa_codec_handle_t p_xa_process_api_obj,
        pUWORD32 pui_bit_rate)
{
    XA_ERRORCODE err_code = XA_NO_ERROR;
    /* the process API function */
    xa_codec_func_t *p_xa_process_api = xa_mp3_enc;
    xa_error_info_struct *p_proc_err_info = &xa_mp3_enc_error_info;

    err_code = (*p_xa_process_api)(p_xa_process_api_obj,
            XA_API_CMD_GET_CONFIG_PARAM,
            XA_MP3ENC_CONFIG_PARAM_BITRATE, pui_bit_rate);

    _XA_HANDLE_ERROR(p_proc_err_info, "" , err_code);

    return XA_NO_ERROR;
}

#ifdef DISPLAY_MESSAGE

/*****************************************************************************/
/*                                                                           */
/*  Function name : xa_display_id_message                                    */
/*                                                                           */
/*  Description   : Display the ID message of the process                    */
/*                                                                           */
/*  Inputs        : char lib_name[] (library name)                           */
/*                  char lib_version[] (library version)                     */
/*                                                                           */
/*  Globals       : none                                                     */
/*                                                                           */
/*  Processing    : Display all the information about the process            */
/*                                                                           */
/*  Outputs       : none                                                     */
/*                                                                           */
/*  Returns       : none                                                     */
/*                                                                           */
/*  Issues        : none                                                     */
/*                                                                           */
/*  Revision history :                                                       */
/*                                                                           */
/*        DD MM YYYY       Author                Changes                     */
/*        29 07 2005       Tejaswi/Vishal        Created                     */
/*                                                                           */
/*****************************************************************************/

VOID xa_display_id_message(char lib_name[], char lib_version[])
{
    char str[4][XA_SCREEN_WIDTH] = {
        "CADENCE DESIGN SYSTEMS, INC.\n",
        "https://ip.cadence.com\n",
        "",
        ""
    };
    char spaces[XA_SCREEN_WIDTH/2 + 1];
    WORD32 i, spclen;

    strcpy(str[2], lib_name);
    strcat(strcat(str[2], " version: "), lib_version);
    strcat(str[2], "\n");
    strcat(str[4-1], "\n");

    for(i = 0; i < XA_SCREEN_WIDTH/2 + 1; i++)
    {
        spaces[i] = ' ';
    }

    for(i = 0; i < 4; i++)
    {
        spclen = XA_SCREEN_WIDTH/2 - strlen(str[i])/2;
        spaces[spclen] = '\0';
        printf("%s",spaces);
        spaces[spclen] = ' ';
        printf("%s",str[i]);
    }
}
#endif /* DISPLAY_MESSAGE */

/*****************************************************************************/
/*                                                                           */
/*  Function name : xa_mp3_enc_main_process                                  */
/*                                                                           */
/*  Description   : Stacked processing with function pointer selection       */
/*                                                                           */
/*  Inputs        : WORD32 argc (Arguments count)                            */
/*                  char *argv[] (Argument strings)                          */
/*                                                                           */
/*  Globals       : None                                                     */
/*                                                                           */
/*  Processing    : Stacked processing of multiple components                */
/*                  Loop1: Set params + Mem alloc                            */
/*                  Loop2: Set params + Init process + Get params            */
/*                  Loop3: Execute                                           */
/*                                                                           */
/*  Outputs       : None                                                     */
/*                                                                           */
/*  Returns       : XA_ERRORCODE error_value  (Error value)                  */
/*                                                                           */
/*  Issues        : none                                                     */
/*                                                                           */
/*  Revision history :                                                       */
/*                                                                           */
/*        DD MM YYYY       Author                Changes                     */
/*        29 07 2005       Tejaswi/Vishal        Created                     */
/*                                                                           */
/*****************************************************************************/

XA_ERRORCODE xa_mp3_enc_main_process(int argc, char *argv[])
{
    LOOPIDX i;

#ifdef DISPLAY_MESSAGE
    /* Library Info and Identification strings */
    char pb_process_name[XA_SCREEN_WIDTH]    = "";
    char pb_lib_version[XA_SCREEN_WIDTH]     = "";
#endif

    /* Error code */
    XA_ERRORCODE err_code = XA_NO_ERROR;

    /* API obj */
    xa_codec_handle_t p_xa_process_api_obj;


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

#ifdef PING_PONG_TEST
    pWORD8 pb_out_buf2;
    int outindex;
#endif

    UWORD32 ui_inp_size;
    WORD32 i_bytes_consumed, i_bytes_read;
    WORD32 i_buff_size;
    WORD32 out_buf_size;

    WORD32 i_out_bytes;
    WORD32 i_total_length;
    UWORD32 ui_samp_freq, ui_num_chan, ui_pcm_wd_sz, ui_bit_rate;
#ifdef ENABLE_CUT_OFF_FREQ_CONFIG
    UWORD32 ui_cut_off_freq = -1;
#endif // ENABLE_CUT_OFF_FREQ_CONFIG

#ifdef SCRATCH_TRASH
    pVOID scratch_trash_ptr;
    int scratch_size ;
#endif

    /* The process API function */
    xa_codec_func_t *p_xa_process_api;

    /* The set config from argc argv */
    XA_ERRORCODE (*p_set_config_param)(int                argc,
            char              *argv[],
            xa_codec_handle_t  p_xa_process_api_obj,
            UWORD32           *pui_samp_freq,
            UWORD32           *pui_num_chan,
            UWORD32           *pui_pcm_wd_sz
#ifdef ENABLE_CUT_OFF_FREQ_CONFIG
            ,UWORD32           *pui_cut_off_freq
#endif // ENABLE_CUT_OFF_FREQ_CONFIG
            );

    /* The get config from API */
    XA_ERRORCODE (*p_get_config_param)(xa_codec_handle_t p_xa_process_api_obj,
            pUWORD32          pui_bitrate);

    /* The error init function */
    VOID (*p_error_init)();

    /* The process error info structure */
    xa_error_info_struct *p_proc_err_info;

    /* Stack process struct initing */
    p_xa_process_api = xa_mp3_enc;
    p_set_config_param = xa_mp3_enc_set_config_param;
    p_get_config_param = xa_mp3_enc_get_config_param;
    p_error_init = xa_mp3_enc_error_handler_init;
    p_proc_err_info = &xa_mp3_enc_error_info;
    /* Stack process struct initing end */

    /* ******************************************************************/
    /* Initialize the error handler                                     */
    /* ******************************************************************/
    (*p_error_init)();

    /* ******************************************************************/
    /* Get the library name, library version and API version            */
    /* ******************************************************************/

#ifdef DISPLAY_MESSAGE
    /* Get the library name string */
    err_code = (*p_xa_process_api)(NULL, XA_API_CMD_GET_LIB_ID_STRINGS,
            XA_CMD_TYPE_LIB_NAME, pb_process_name);

    _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

    /* Get the library version string */
    err_code = (*p_xa_process_api)(NULL, XA_API_CMD_GET_LIB_ID_STRINGS,
            XA_CMD_TYPE_LIB_VERSION, pb_lib_version);

    _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

    /* Display the Tensilica identification message */
    xa_display_id_message(pb_process_name, pb_lib_version);
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
#if SHOW_SIZES
    printf("API structure size: %u bytes\n", pui_api_size);
#endif
    /* API object requires 4 bytes (WORD32) alignment */
    assert((((unsigned int) g_pv_arr_alloc_memory[g_w_malloc_count]) & 3) == 0);

    /* Set API object with the memory allocated */
    p_xa_process_api_obj = (void *) g_pv_arr_alloc_memory[g_w_malloc_count];

    g_w_malloc_count++;

    /* Set the config params to default values */
    err_code = (*p_xa_process_api)(p_xa_process_api_obj,
            XA_API_CMD_INIT,
            XA_CMD_TYPE_INIT_API_PRE_CONFIG_PARAMS,
            NULL);

    _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

    /* ******************************************************************/
    /* Set config parameters got from the user present in argc argv     */
    /* ******************************************************************/

    err_code = (*p_set_config_param)(argc,
            argv,
            p_xa_process_api_obj,
            &ui_samp_freq,
            &ui_num_chan,
            &ui_pcm_wd_sz
#ifdef ENABLE_CUT_OFF_FREQ_CONFIG
            , &ui_cut_off_freq
#endif // ENABLE_CUT_OFF_FREQ_CONFIG
            );

    _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

#ifdef WAV_READER
    {
        err_code =  wav_header_decode(g_pf_inp, &ui_num_chan, &ui_samp_freq,
                &ui_pcm_wd_sz, &i_total_length);
        _XA_HANDLE_ERROR(&xa_testbench_error_info, "", err_code);

        /* PCM Word Size (For single input file) */
        {
            err_code = (*p_xa_process_api)(p_xa_process_api_obj,
                    XA_API_CMD_SET_CONFIG_PARAM,
                    XA_MP3ENC_CONFIG_PARAM_PCM_WDSZ, &ui_pcm_wd_sz);
            _XA_HANDLE_ERROR(p_proc_err_info, "" , err_code);

        }
        /* Sampling Frequency */
        {
            err_code = (*p_xa_process_api)(p_xa_process_api_obj,
                    XA_API_CMD_SET_CONFIG_PARAM,
                    XA_MP3ENC_CONFIG_PARAM_SAMP_FREQ, &ui_samp_freq);
            _XA_HANDLE_ERROR(p_proc_err_info, "" , err_code);

        }
        /* Total Number of Channels */
        {
            err_code = (*p_xa_process_api)(p_xa_process_api_obj,
                    XA_API_CMD_SET_CONFIG_PARAM,
                    XA_MP3ENC_CONFIG_PARAM_NUM_CHANNELS, &ui_num_chan);
            _XA_HANDLE_ERROR(p_proc_err_info, "" , err_code);

        }

    }
#endif

#ifdef ENABLE_CUT_OFF_FREQ_CONFIG
    if(ui_cut_off_freq != -1)
    {
        // fs/2 = 1 
        double frac_bandwidth = ((double) ui_cut_off_freq *2) / (double) (ui_samp_freq);

        if(frac_bandwidth <= 1.0)
        {
            //convert to 1.23 format
            int ifrac_bandwidth = (int)(frac_bandwidth * (1<<24));
            err_code = (*p_xa_process_api)(
                    p_xa_process_api_obj, XA_API_CMD_SET_CONFIG_PARAM,
                    XA_MP3ENC_CONFIG_PARAM_FRAC_BANDWIDTH,
                    &ifrac_bandwidth);
            _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);
        }
    }
#endif // ENABLE_CUT_OFF_FREQ_CONFIG

    /* ******************************************************************/
    /* Initialize Memory info tables                                    */
    /* ******************************************************************/

    /* Get memory info tables size */
    err_code = (*p_xa_process_api)(
            p_xa_process_api_obj,
            XA_API_CMD_GET_MEMTABS_SIZE, 0,
            &ui_proc_mem_tabs_size);

    _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);


    g_pv_arr_alloc_memory[g_w_malloc_count] = malloc(ui_proc_mem_tabs_size);

    if(g_pv_arr_alloc_memory[g_w_malloc_count] == NULL)
    {
        _XA_HANDLE_ERROR(&xa_testbench_error_info, "Mem tables alloc",
                XA_TESTBENCH_MFMAN_FATAL_MEM_ALLOC_FAILED);
    }

    /* API object requires 4 bytes (WORD32) alignment */
    assert((((unsigned int) g_pv_arr_alloc_memory[g_w_malloc_count]) & 3) == 0);

    /* Set pointer for process memory tables */
    err_code = (*p_xa_process_api)(p_xa_process_api_obj,
            XA_API_CMD_SET_MEMTABS_PTR,
            0,
            (void *) g_pv_arr_alloc_memory[g_w_malloc_count]);

    _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);


    g_w_malloc_count++;

    /* initialize the API, post config, fill memory tables */
    err_code = (*p_xa_process_api)(
            p_xa_process_api_obj,
            XA_API_CMD_INIT,
            XA_CMD_TYPE_INIT_API_POST_CONFIG_PARAMS,
            NULL);

    _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);


    /* ******************************************************************/
    /* Allocate Memory with info from library                           */
    /* ******************************************************************/

    /* Get number of memory tables required */
    err_code = (*p_xa_process_api)(
            p_xa_process_api_obj,
            XA_API_CMD_GET_N_MEMTABS, 0, &n_mems);

    _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

    for(i = 0; i < (WORD32)n_mems; i++)
    {
        int ui_size, ui_alignment, ui_type;
        pVOID pv_alloc_ptr;

        /* Get memory size */
        err_code = (*p_xa_process_api)(
                p_xa_process_api_obj,
                XA_API_CMD_GET_MEM_INFO_SIZE, i,
                &ui_size);

        _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

        /* Get memory alignment */
        err_code = (*p_xa_process_api)(
                p_xa_process_api_obj,
                XA_API_CMD_GET_MEM_INFO_ALIGNMENT, i,
                &ui_alignment);

        _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

        /* Get memory type */
        err_code = (*p_xa_process_api)(
                p_xa_process_api_obj,
                XA_API_CMD_GET_MEM_INFO_TYPE, i,
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
        err_code = (*p_xa_process_api)(p_xa_process_api_obj,
                XA_API_CMD_SET_MEM_PTR,
                i,
                pv_alloc_ptr);

        _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

        if (ui_type == XA_MEMTYPE_INPUT) {
            pb_inp_buf = pv_alloc_ptr;
            ui_inp_size = ui_size;
        }
        else if (ui_type == XA_MEMTYPE_OUTPUT) {
            pb_out_buf = pv_alloc_ptr;
            out_buf_size = ui_size;
#ifdef PING_PONG_TEST
            pb_out_buf2 = malloc(ui_size); //pong
            outindex = i;
#endif
        }
#if SHOW_SIZES
        switch (ui_type) {
            case XA_MEMTYPE_INPUT:
                printf("Input buffer size: %u bytes\n", ui_size);
                break;
            case XA_MEMTYPE_OUTPUT:
                printf("Output buffer size: %u bytes\n", ui_size);
                break;
            case XA_MEMTYPE_SCRATCH:
#ifdef SCRATCH_TRASH
                scratch_trash_ptr = pv_alloc_ptr;
                scratch_size = ui_size;
#endif
                printf("Scratch buffer size: %u bytes\n", ui_size);
                break;
            case XA_MEMTYPE_PERSIST:
                printf("Persistent buffer size: %u bytes\n", ui_size);
                break;
            default:
                assert(0);
        }
#endif /* SHOW_SIZES */
    }

    /* End first part */

    /* Second part        */
    /* Initialize process */
    /* Get config params  */

    /* ******************************************************************/
    /* Initialize process in a loop (to handle junk data at beginning)  */
    /* ******************************************************************/
    i_bytes_consumed = ui_inp_size;
    i_buff_size = ui_inp_size;

    do
    {
        for(i = 0; i < (WORD32)ui_inp_size - i_bytes_consumed; i++)
        {
            pb_inp_buf[i] = pb_inp_buf[i+i_bytes_consumed];
        }

        i_bytes_read = xa_fread((pVOID)(pb_inp_buf + (ui_inp_size - i_bytes_consumed)),
                sizeof(WORD16), i_bytes_consumed/sizeof(WORD16), g_pf_inp);
        i_total_length -= i_bytes_read;

        /* New buffer size */
        i_buff_size = i_buff_size - (i_bytes_consumed - i_bytes_read);

        if( i_buff_size <= 0 )
        {
            i_buff_size = 0;

            /* Tell that the input is over in this buffer */
            err_code = (*p_xa_process_api)(
                    p_xa_process_api_obj,
                    XA_API_CMD_INPUT_OVER, 0, NULL);

            _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);
        }

        /* Set number of bytes to be processed */
        err_code = (*p_xa_process_api)(
                p_xa_process_api_obj,
                XA_API_CMD_SET_INPUT_BYTES, 0, &i_buff_size);

        _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

        /* Initialize the process */
        err_code = (*p_xa_process_api)(
                p_xa_process_api_obj, XA_API_CMD_INIT,
                XA_CMD_TYPE_INIT_PROCESS, NULL);

        _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);


        /* Checking for end of initialization */
        err_code = (*p_xa_process_api)(
                p_xa_process_api_obj, XA_API_CMD_INIT,
                XA_CMD_TYPE_INIT_DONE_QUERY,
                &ui_init_done);

        _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

        /* How much buffer is used in input buffers */
        err_code = (*p_xa_process_api)(
                p_xa_process_api_obj,
                XA_API_CMD_GET_CURIDX_INPUT_BUF, 0,
                &i_bytes_consumed);

        _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

    } while(!ui_init_done);
    /* Init end of process condition end */
    /* End second part */

#ifdef _PROFILE
    Sum = 0;
    frmCnt = 0;
#endif /* _PROFILE */

    /* Get config param: For getting the re-adjusted bit-rate */
    err_code = (*p_get_config_param)(p_xa_process_api_obj, &ui_bit_rate);

    _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

    /* Third part for executing each process */
    do
    {
        for(i = 0; i < (WORD32)ui_inp_size - i_bytes_consumed; i++)
        {
            pb_inp_buf[i] = pb_inp_buf[i+i_bytes_consumed];
        }

        i_bytes_read = xa_fread((pVOID)(pb_inp_buf + (ui_inp_size - i_bytes_consumed)),
                sizeof(WORD16), i_bytes_consumed/sizeof(WORD16), g_pf_inp);
        i_total_length -= i_bytes_read;

        /* New buffer size */
        i_buff_size = i_buff_size - (i_bytes_consumed - i_bytes_read);

        if((i_total_length <= 0)  || (i_bytes_read != i_bytes_consumed))
        {
            if (i_total_length <= 0)
            {
                i_buff_size = i_total_length + i_bytes_read;
            }
            else
            {
                i_buff_size = i_bytes_read;
            }

            /* Tell that the input is over in this buffer */
            err_code = (*p_xa_process_api)(p_xa_process_api_obj,
                    XA_API_CMD_INPUT_OVER,
                    0,
                    NULL);

            _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

        }

        /* Set number of bytes to be processed */
        err_code = (*p_xa_process_api)(p_xa_process_api_obj,
                XA_API_CMD_SET_INPUT_BYTES,
                0,
                &i_buff_size);

        _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

#ifdef _PROFILE
        times(&start);
        xt_iss_client_command("all", "enable");
#endif /* _PROFILE */
        // trash the scratch between the do execute calls
#ifdef SCRATCH_TRASH             
        memset(scratch_trash_ptr, 10, scratch_size);
#endif

        /* Execute process */
        err_code = (*p_xa_process_api)
            (p_xa_process_api_obj,
             XA_API_CMD_EXECUTE,
             XA_CMD_TYPE_DO_EXECUTE, NULL);

#ifdef _PROFILE
        xt_iss_client_command("all", "disable");
        times(&stop);
#endif /* _PROFILE */

        _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

        /* Checking for end of processing */
        err_code = (*p_xa_process_api)(
                p_xa_process_api_obj,
                XA_API_CMD_EXECUTE,
                XA_CMD_TYPE_DONE_QUERY,
                &ui_exec_done);

        _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

        /* Get the output bytes */
        err_code = (*p_xa_process_api)(
                p_xa_process_api_obj,
                XA_API_CMD_GET_OUTPUT_BYTES, 0,
                &i_out_bytes);

        _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

        assert(i_out_bytes <= out_buf_size);
#ifdef _PROFILE
        {
            frmCnt++;
            cycles = stop.tms_utime - start.tms_utime;
            Curr = ((double)cycles * ui_samp_freq) /
                ((ui_inp_size/(sizeof(WORD16)*ui_num_chan))* 1000000);
            Sum += Curr;
            Ave = Sum / frmCnt;
            if (Peak < Curr)
            {
                Peak = Curr;
                Peak_frame = frmCnt;
            }
            fprintf(stderr, "[%d] MCPS: %.2f Average: %.2f Peak: %.2f @ [%d]\n",
                    frmCnt, Curr, Ave, Peak, Peak_frame);
        }
#endif /* _PROFILE */

        fwrite(pb_out_buf, sizeof(char), i_out_bytes, g_pf_out);

#ifdef PING_PONG_TEST
        // swap ping pong
        pWORD8 temp_ping = pb_out_buf;
        pb_out_buf = pb_out_buf2;
        pb_out_buf2 = temp_ping;
        err_code = (*p_xa_process_api)(p_xa_process_api_obj,
                XA_API_CMD_SET_MEM_PTR,
                outindex,
                pb_out_buf);
        _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);
#endif

        /* How much buffer is used in input buffers */
        err_code = (*p_xa_process_api)(
                p_xa_process_api_obj,
                XA_API_CMD_GET_CURIDX_INPUT_BUF, 0,
                &i_bytes_consumed);

        _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

        /* Do till the process execution is done */
    } while (!ui_exec_done);

#ifdef _PROFILE
    fprintf(stderr,"\nPeak MCPS = %7.1f\n",Peak);
    fprintf(stderr,"Peak frame = %d\n",Peak_frame);
    fprintf(stderr,"Average MCPS = %7.1f\n",Ave);
#endif /* _PROFILE */

    return XA_NO_ERROR;
}

/* End xa_main_process() */

/*****************************************************************************/
/*                                                                           */
/*  Function Name : main                                                     */
/*                                                                           */
/*  Description   : Main function                                            */
/*                                                                           */
/*  Inputs        : None                                                     */
/*                                                                           */
/*  Globals       : None                                                     */
/*                                                                           */
/*  Processing    : Parse the parameter file and run the main process        */
/*                                                                           */
/*  Outputs       : None                                                     */
/*                                                                           */
/*  Returns       : 0 on success, -1 on error                                */
/*                                                                           */
/*  Issues        : None                                                     */
/*                                                                           */
/*  Revision history :                                                       */
/*                                                                           */
/*        DD MM YYYY       Author                Changes                     */
/*                                                                           */
/*****************************************************************************/


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

int main(int argc, char *argv[])
{
    FILE *param_file_id;

    char curr_cmd[XA_MAX_CMD_LINE_LENGTH];
    int fargc, curpos;
    WORD32 processcmd = 0;
    int err_code = XA_NO_ERROR;

    char fargv[XA_MAX_ARGS][XA_MAX_CMD_LINE_LENGTH];

    char *pargv[XA_MAX_ARGS];

    char pb_input_file_path[XA_MAX_CMD_LINE_LENGTH] = "";
    char pb_output_file_path[XA_MAX_CMD_LINE_LENGTH] = "";

    // NOTE: set_wbna() should be called before any other dynamic
    // adjustment of the region attributes for cache.
    set_wbna(&argc, argv);

    xa_testbench_error_handler_init();

    if(argc == 1)
    {
        param_file_id = fopen(PARAMFILE, "r");
        if (param_file_id == NULL)
        {
               MP3EncDisplayCmdLine();
               err_code = XA_TESTBENCH_MFMAN_FATAL_FILE_OPEN_FAILED;
               xa_error_handler(&xa_testbench_error_info, "Parameter File", err_code);
               exit(1);
        }

        /* Process one line at a time */
        while(fgets(curr_cmd, XA_MAX_CMD_LINE_LENGTH, param_file_id))
        {
            curpos = 0;
            fargc = 0;
            /* if it is not a param_file command and if */
            /* CLP processing is not enabled */
            if(curr_cmd[0] != '@' && !processcmd)
            { /* skip it */
                continue;
            }

            while(sscanf(curr_cmd + curpos, "%s", fargv[fargc]) != EOF)
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

            if (fargc < 1) /* for blank lines etc. */
                continue;

            if(strcmp(fargv[0], "@Output_path") == 0)
            {
                if (fargc > 1) strcpy(pb_output_file_path, fargv[1]);
                else strcpy(pb_output_file_path, "");
                continue;
            }

            if(strcmp(fargv[0], "@Input_path") == 0)
            {
                if (fargc > 1) strcpy(pb_input_file_path, fargv[1]);
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
                    printf("%s ", fargv[i]);
                    pargv[i] = fargv[i];

                    if(!strncmp(fargv[i], "-ifile:", 7))
                    {
                        char *pb_arg_val = fargv[i] + 7;
                        char pb_input_file_name[XA_MAX_CMD_LINE_LENGTH] = "";

                        strcat(pb_input_file_name, pb_input_file_path);
                        strcat(pb_input_file_name, pb_arg_val);

                        g_pf_inp = NULL;
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

                        g_pf_out = NULL;
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

                printf("\n");

                if(err_code == XA_NO_ERROR)
                    xa_mp3_enc_main_process(fargc, pargv);

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

        err_code = XA_TESTBENCH_MFMAN_FATAL_FILE_OPEN_FAILED;
        for(i = 1; i < argc; i++)
        {
            printf("%s ", argv[i]);

            if(!strncmp(argv[i], "-help", 5))
            {
            	MP3EncDisplayCmdLine();
                return XA_NO_ERROR;
            }

            if(!strncmp(argv[i], "-ifile:", 7))
            {
                char *pb_arg_val = argv[i] + 7;
                char pb_input_file_name[XA_MAX_CMD_LINE_LENGTH] = "";

                err_code = XA_NO_ERROR;
                strcat(pb_input_file_name, pb_input_file_path);
                strcat(pb_input_file_name, pb_arg_val);

                g_pf_inp = NULL;
                g_pf_inp = fopen(pb_input_file_name, "rb");
                if(g_pf_inp == NULL)
                {
                    err_code = XA_TESTBENCH_MFMAN_FATAL_FILE_OPEN_FAILED;
                    xa_error_handler(&xa_testbench_error_info,
                            "Input File", err_code);
                }
            }

            if(!strncmp(argv[i], "-ofile:", 7))
            {
                char *pb_arg_val = argv[i] + 7;
                char pb_output_file_name[XA_MAX_CMD_LINE_LENGTH] = "";

                strcat(pb_output_file_name, pb_output_file_path);
                strcat(pb_output_file_name, pb_arg_val);

                g_pf_out = NULL;
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

        printf("\n");
        xa_error_handler(&xa_testbench_error_info,
                "Input or Output File", err_code);

        if(err_code == XA_NO_ERROR)
            xa_mp3_enc_main_process(argc - 1, &argv[1]);

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
} /* end xa_param_file_process */

VOID MP3EncDisplayCmdLine(VOID)
{
    printf("\n\nUSAGE:\n");
    printf("-ifile:<infile.wav> -ofile:<outfile.mp3> [-br:<bitrate> -help]\n");
    printf("-br:<bitrate>\t: MP3 bitrate in kbps (default : 128)\n");
#ifdef ENABLE_CUT_OFF_FREQ_CONFIG
    printf("-cf:<cut of freq>\t: Cut of frequency in Hz (default : internally calculated)\n");
#endif
    printf("-help\t\t: Prints this help message\n");
    printf("where,\n");
    printf("\t<infile> is the name of the mp3 input file.\n");
    printf("\t<outfile> is the name of the output file.\n");
    printf("\t<bitrate> is the bit-rate for encoding. \n");
}
