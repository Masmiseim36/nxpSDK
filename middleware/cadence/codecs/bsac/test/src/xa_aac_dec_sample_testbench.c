/*
 * Copyright (c) 2008-2022 Cadence Design Systems, Inc.
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


#include "aac_dec/xa_aac_dec_api.h"

#ifdef CSTUB_HIFI2
#define  PROFILE 1
#endif

#if !defined(PROFILE)
#define PROFILE 1
#endif /* PROFILE */

//#define SCRATCH_TRASH_TEST
//#define OUTPUT_BUF_PING_PONG_TEST    
//#define INPUT_BUF_PING_PONG_TEST    

#if !defined(SHOW_SIZES)
#define SHOW_SIZES 1
#endif /* SHOW_SIZES */

#ifndef STEP_PLAY
#define STEP_PLAY 0
#endif

#if PROFILE
#define PROF_ALLOCATE
#endif /* PROFILE */

#include "xa_bsac_dec_common.h"

/* Scratch test */
SCRATCH_TRASH_TEST_ALLOCATE

/* Output ping pong test */
OUTPUT_BUF_PING_PONG_TEST_ALLOCATE

/* Input ping pong test */
INPUT_BUF_PING_PONG_TEST_ALLOCATE

#if STEP_PLAY
/* Play PLAY_TIME_MS milliseconds of audio, mute the output for
   MUTE_TIME_MS milliseconds, skip SKIP_TIME_MS milliseconds of audio,
   repeat. */
#define PLAY_TIME_MS 500 /* ms */
#define MUTE_TIME_MS 100  /* ms */
#define SKIP_TIME_MS 500 /* ms */
#endif /* STEP_PLAY */


#if !defined(DISPLAY_MESSAGE)
#define DISPLAY_MESSAGE 1
#endif /* DISPLAY_MESSAGE */

#ifdef __cplusplus
extern "C" {
#endif

#  define CODEC_FUNC xa_bsac_dec


    extern DLL_SHARED XA_ERRORCODE
    CODEC_FUNC (xa_codec_handle_t p_xa_aacmch_dec_obj,
                WORD32            i_cmd,
                WORD32            i_idx,
                pVOID             pv_value);
#ifdef __cplusplus
}
#endif

pVOID g_pv_arr_alloc_memory[MAX_MEM_ALLOCS];
WORD  g_w_malloc_count;
FILE *g_pf_inp=(FILE *)0, *g_pf_out=(FILE *)0;

VOID xa_aac_dec_error_handler_init();
VOID xa_testbench_error_handler_init();

extern xa_error_info_struct xa_aac_dec_error_info;
extern xa_error_info_struct xa_testbench_error_info;

int outnchans = 0;
int chanmap = 0xfffffff;

void
xa_shift_input_buffer (signed char *buf, int buf_size, int bytes_consumed)
{
    int i;
    ae_p16s *dst;
    ae_p16s *src;

    if (bytes_consumed <= 0)
        return;

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

        dst = (ae_p16s *)buf;
        src = (ae_p16s *)&buf[bytes_consumed];
        for (i = 0; i < (buf_size - bytes_consumed) >> 1; i++)
        {
            dst[i] = src[i];
        }
        return;
    }

    /* Default, non-aligned data movement. */
    for (i = 0; i < buf_size - bytes_consumed; i++)
    {
        buf[i] = buf[i + bytes_consumed];
    }
}


/****************************************
 * PCM / Wave file write functions
 ****************************************/

static void write16_bits_lh(FILE *fp, int i)
{
    putc(i & 0xff, fp);
    putc((i >> 8) & 0xff, fp);
}

static void write32_bits_lh(FILE *fp, int i)
{
    write16_bits_lh(fp, (int)(i & 0xffffL));
    write16_bits_lh(fp, (int)((i >> 16) & 0xffffL));
}

static void write24_bits_lh(FILE *fp, int i)
{
    putc((i >> 8) & 0xff, fp);
    putc((i >> 16) & 0xff, fp);
    putc((i >> 24) & 0xff, fp);
}

static long
pcm_write (long *pTimeCh, int outchmap, int frameSize, FILE *fp , int pcm_sample_size)
{
    int i,ch, chmap, sample_ctr = 0;

    if(pcm_sample_size != 16)
    {
        long sample_24;
        for(i=0; i < frameSize; i++)
        {
            chmap = outchmap;
            for(ch=0 ; ch < 8 ; ch++)
            {
                /* Check if any channel data is present at this sample offset */
                if ((chmap & 0x0f) == 0x0f)
                {
                    /* Don't write unused channels. */
                }
                else
                {
                    sample_24 = pTimeCh[i*outnchans+ch];
                    write24_bits_lh(fp,sample_24);
                    sample_ctr++;
                }
                chmap >>= 4;
            }
        }
    }
    else
    {
        short *pTimeChShort = (short *)pTimeCh;
        short sample_16;
        for(i=0; i < frameSize; i++)
        {
            chmap = outchmap;
            for(ch=0 ; ch < 8; ch++)
            {
                /* Check if any channel data is present at this sample offset */
                if ((chmap & 0x0f) == 0x0f)
                {
                    /* Don't write unused channels. */
                }
                else
                {
                    sample_16 = pTimeChShort[i*outnchans+ch];
                    write16_bits_lh(fp, sample_16);
                    sample_ctr++;
                }
                chmap >>= 4;
            }
        }

    }
    return (sample_ctr);
}


#ifdef WAV_HEADER

int write_wav_header(
    FILE *fp,
    int freq,
    int channels,
    int bits,
    int frameSize,
    int frameCounter,
    int numWritten
)
{
    int bytes = (bits + 7) / 8;
    fwrite("RIFF", 1, 4, fp); /* label */

    /* write32_bits_lh(fp, (4096 * frameCounter) + 58 - 8); */ /* length in bytes without header */
    write32_bits_lh(fp, (numWritten * bytes) + 58 - 8); /* length in bytes without header */

    fwrite("WAVEfmt ", 1, 8, fp); /* 2 labels */
    write32_bits_lh(fp, 2 + 2 + 4 + 4 + 2 + 2  + 2); /* length of PCM format decl area */
    write16_bits_lh(fp, 1); /* is pcm? */
    write16_bits_lh(fp, channels);
    write32_bits_lh(fp, freq);
    write32_bits_lh(fp, freq * channels * bytes); /* bps */
    write16_bits_lh(fp, channels * bytes);
    write16_bits_lh(fp, bits);
    write16_bits_lh(fp, 0);
    fwrite("fact", 1, 4, fp);
    write32_bits_lh(fp, 4);
    write32_bits_lh(fp, frameSize * frameCounter);
    fwrite("data", 1, 4, fp);

    /*  write32_bits_lh(fp, (4096 * frameCounter)); */
    write32_bits_lh(fp, (numWritten * bytes));

    return (ferror(fp) ? -1 : 0);
}

#endif

/*****************************************************************
 * showusage: show the usage message
 *****************************************************************/

void showusage()
{
    puts(
        "\n"
        "Usage:  <aac_dec> [OPTIONS] -ifile:<inputfile> -ofile:<outputfile>\n"
        "\n"
        "        OPTIONS:\n"
        "         -b<bsfomat> Input file format (default: rawbsac)\n"
        "         -c<int>     Input number of channels (1 -  mono , 2 - stereo(default)  )\n"
        "         -f<int>     Force mono to stereo output (0 - Off, 1 - On(default))\n"
        "         -h          help\n"
        "         -l<int>     Number of layers to be decoded (0 to 48(default))\n"
        "         -p<int>     Sampling rate of raw bitstream\n"
        "                        (default - 44100 Hz)\n"
        "         -s<int>     Start frame offset to decode (Valid values: 0 to 32767. default: 0. Debug only)\n"
        "         -t<int>     Stop frame offset to decode (Valid values: 0 to 32767. default: 0. Debug only)\n"
        "         -w<int>     PCM sample size (16 or 24(default))\n"
        "\n"
        "         -ifile:     Input bit stream file\n"
        "         -ofile:     Output PCM or Wave file\n"
        "\n"
    );
}



/****************************************
 * Function to parse cmdline parameter and
 *    set config variables in the codec
 *    library
 ****************************************/

int startFrame =  0;                                   /*!< number of first frame which output is written to output file */
int stopFrame  =  0;                                   /*!< number of last frame which output is written to output file */

XA_AACDEC_EBITSTREAM_TYPE bsType = XA_AACDEC_EBITSTREAM_TYPE_BSAC_RAW;
int parse_cmdline(void *p_hdl, char *argv[], int argc)
{

    int err_code = 0;
    char argtyp = (char)0;
    int argidx = 0;
    char *argval = 0;
    char one = '1';
    int i_value;
    unsigned int ui_value;
    xa_codec_handle_t p_xa_process_api_obj = (xa_codec_handle_t)p_hdl;
    xa_codec_func_t *p_xa_process_api = CODEC_FUNC;
    xa_error_info_struct *p_proc_err_info = &xa_aac_dec_error_info;
    char externalBsFormat[20]="";                           /*!< bitstream format */

    /* initialize commandline parser */

    while (1)
    {
        if ( argidx >= argc ) break;

        if(strncmp((argv[argidx]), "-", 1) != 0)
        {
            fprintf(stderr,"\nCommand line option has to be prefixed with - (%s)\n",argv[argidx]);
            showusage();
            return 1;
        }


        /* Check & parse the argument type  & its value */
        if (argv[argidx][0] == '-')
        {
            argtyp = argv[argidx][1];

            if (strlen(argv[argidx]) == 2)
            {
                if(argtyp != 'i' && argtyp != 'o' && argtyp != 'b')
                {
                    printf("\n Incorrect command line argument/value: (%s)\n",argv[argidx]);
                    showusage();
                    return 1;
                }
                /* Argument type has either no argument value or has argument value after blank-spaces */
                /* default values */
                argval = &one;
                argval[1] = '\0';
                if ( (argidx+1) < argc )
                {
                    if (argv[argidx+1][0] != '-')
                    {
                        /* To take care of case where argument-flag and option-value have blank-spaces between them */
                        argidx++;
                        argval = &argv[argidx][0];
                    }
                }
            }
            else
            {
                argval = &argv[argidx][2];
            }
        }
        else
        {
            argtyp = ' ';
            argval = NULL;
        }
        argidx++;

        /* Set config parameters based on argtype */
        switch(argtyp)
        {
        case 'i':

            {
                /* Open input file if it is not opened by -ifile: option (in the main wrapper) */
                if ( g_pf_inp == (FILE *)0 )
                {
                    char pb_input_file_name[MAX_CMD_LINE_LENGTH] = "";
                    strcpy(pb_input_file_name,argval);
                    g_pf_inp = fopen(pb_input_file_name,"rb");
                    if (g_pf_inp == (FILE *)0)
                    {
                        fprintf(stderr,"\n bitstream file not found\n\n");
                        return -1;
                    }
                }
            }
            break;

        case 'o':
            /* Open output file if it is not opened by -ofile: option (in the main wrapper) */
            if ( g_pf_out == (FILE *)0 )
            {
                char pb_output_file_name[MAX_CMD_LINE_LENGTH] = "";
                strcpy(pb_output_file_name,argval);
                g_pf_out = fopen(pb_output_file_name,"wb");
                if (g_pf_out == (FILE *)0)
                {
                    fprintf(stderr,"\nOutput file could not be opened\n\n");
                    return -1;
                }
            }
            break;

        case 'b':

            strcpy(externalBsFormat,argval);

            /* Set bitstream type at the end */
            bsType = XA_AACDEC_EBITSTREAM_TYPE_UNKNOWN;

            /* Check For rawbsac stream */
            if (!strcmp(externalBsFormat,"rawbsac"))
            {
                bsType = XA_AACDEC_EBITSTREAM_TYPE_BSAC_RAW;
            }

            ui_value = (WORD32)bsType;

            SET_CONFIG( XA_AACDEC_CONFIG_PARAM_EXTERNALBSFORMAT, &ui_value, "");

            break;

        case 's':
            startFrame = atol(argval);
            break;

        case 't':
            stopFrame = atol(argval);
            break;

        case 'p':
        {
            i_value = (WORD32)atol(argval);
            SET_CONFIG( XA_AACDEC_CONFIG_PARAM_EXTERNALSAMPLINGRATE, &i_value, "");
            break;
        }
        case 'f':
        {
            /* Force stereo */
            i_value = (WORD32)atol(argval);
            SET_CONFIG( XA_AACDEC_CONFIG_PARAM_TO_STEREO, &i_value, "");
            break;
        }

        case 'w':
        {
            /* pcm sample size */
            i_value = (WORD32)atol(argval);
            SET_CONFIG( XA_AACDEC_CONFIG_PARAM_PCM_WDSZ, &i_value, "");
            break;
        }

        /* Input number of channels in BSAC Case */
        case 'c':
        {
            i_value = atol(argval);
            SET_CONFIG( XA_AACDEC_CONFIG_PARAM_EXTERNALCHCONFIG, &i_value, "");
            break;
        }

        case 'l':
        {
            i_value = atol(argval);
            SET_CONFIG( XA_AACDEC_CONFIG_PARAM_DECODELAYERS, &i_value, "");
            break;
        }

        case 'h':
            showusage();
            err_code = 0;
            return err_code;
            break;

        default:
            fprintf(stderr,"\nUnsupported command line option (%s)\n", argv[argidx-1]);
            showusage();
	    return 1;
        } /* switch */
    } /* while */

    if (argc <= 1)
    {
        showusage();
    }

    return err_code;
}


/*****************************************************************************************
 * Function to open bit-stream file
 *     After an initialization and synchronization phase, get_params function is called to
 *     get the parameters available by reading the input bit-stream.
 *****************************************************************************************/

int get_params(void *p_hdl,
               int * p_numChannels,
               int *p_channelMode,
               int *p_sampleRate,
               int *p_bsfmt,
               int *p_data_rate)
{
    int err_code = 0;
    char pFmt[20] = {""};

    int  aac_sampleRate=0;

    /* The process API function */
    xa_codec_func_t *p_xa_process_api = CODEC_FUNC;

    /* The process API object */
    xa_codec_handle_t p_xa_process_api_obj = (xa_codec_handle_t)p_hdl;

    /* The process error info structure */
    xa_error_info_struct *p_proc_err_info = &xa_aac_dec_error_info;

    /* Get Number Of Channels in the Bit Stream */
    GET_CONFIG( XA_AACDEC_CONFIG_PARAM_NUM_CHANNELS, p_numChannels, "Get number of channels");

    GET_CONFIG( XA_AACDEC_CONFIG_PARAM_ACMOD, p_channelMode, "Get channel mode value");

    GET_CONFIG( XA_AACDEC_CONFIG_PARAM_SAMP_FREQ, p_sampleRate, "Get output sampling frequency");

    GET_CONFIG( XA_AACDEC_CONFIG_PARAM_AAC_SAMPLERATE, &aac_sampleRate, "Get sample rate"); //SDEO: I think we can remove this
    
    GET_CONFIG( XA_AACDEC_CONFIG_PARAM_EXTERNALBSFORMAT, p_bsfmt, "Get bitstream foramt"); 

    switch(*p_bsfmt)
    {
    case XA_AACDEC_EBITSTREAM_TYPE_BSAC_RAW:
        strcpy(pFmt,"BSAC-RAW");
        break;
    default:
        strcpy(pFmt,"NO");
        err_code = -1;
        break;
    }

    fprintf(stderr, "Bitstream format:   %s\n",pFmt);
    fprintf(stderr, "Format:             ");

    /* Determine the format */
    fprintf(stderr, "plain AAC\n");
    fprintf (stderr, "Output Sample rate: %i Hz\n", *p_sampleRate);
    fprintf (stderr, "AAC Sample rate:    %i Hz\n", aac_sampleRate);
    fprintf (stderr, "Input config:       ");

    switch (*p_channelMode)
    {
    case XA_AACDEC_CHANNELMODE_MONO:
        fprintf(stderr, "Mono\n");
        break;
    case XA_AACDEC_CHANNELMODE_STEREO:
        fprintf(stderr, "Stereo\n");
        break;
    case XA_AACDEC_CHANNELMODE_DUAL_CHANNEL:
        fprintf(stderr, "Dual mono\n");
        break;

    default:
        fprintf(stderr, "unkown channel mode\n");
        err_code = -1;
        break;
    }
    fprintf (stderr, "Output Channels:    %d\n", *p_numChannels);

    GET_CONFIG( XA_AACDEC_CONFIG_PARAM_CHANMAP, &chanmap, "Get channel map value");

    fprintf (stderr, "Output Channel Map: %08x\n\n", chanmap);

    /* Get maximum number of output channels in output buffer */
    GET_CONFIG( XA_AACDEC_CONFIG_PARAM_OUTNCHANS, &outnchans, "Get ioutput number of channels");

    return err_code;
}

/*****************************************************************
 * decoder main                                                  *
 *****************************************************************/

int xa_aac_dec_main_process(int argc, char *argv[])
{
    int i;
    int  acmod = -1;

    int frameCounter = 0;
    int frameSize = 0;
    int i_num_chan = 2;
    int i_samp_freq = 48000, i_data_rate=0;
    int err_code;
    unsigned int err_code_exec = 0;
    unsigned int err_code_init = 0;
    int bsfmt =XA_AACDEC_EBITSTREAM_TYPE_UNKNOWN;

    int pcm_sample_size;
    xa_codec_handle_t p_xa_process_api_obj;

    /* The process API function */
    xa_codec_func_t *p_xa_process_api;

    /* The process error info structure */
    xa_error_info_struct *p_proc_err_info;

    long i_total_bytes = 0;



    /* Memory variables */
    unsigned int n_mems;
    unsigned int ui_proc_mem_tabs_size;
    void *p_mem_tabs;

    /* API size */
    unsigned int ui_api_size;

    /* Process initing done query variable */
    unsigned int ui_init_done, ui_exec_done;
    pWORD8 pb_inp_buf = NULL, pb_out_buf = NULL;

    UWORD32 ui_inp_size = 0;
    WORD32 i_bytes_consumed, i_bytes_read;
    WORD32 i_buff_size;

    WORD32 i_out_bytes;

#if DISPLAY_MESSAGE
    /* Library Info and Identification strings */
    WORD8 pb_process_name[XA_SCREEN_WIDTH]    = "";
    WORD8 pb_lib_version[XA_SCREEN_WIDTH]     = "";
#endif


    p_xa_process_api = CODEC_FUNC;
    p_proc_err_info = &xa_aac_dec_error_info;

    p_mem_tabs = NULL;
    p_xa_process_api_obj = NULL;
    /* Call decoder error handler init */
    xa_aac_dec_error_handler_init();

    /* ******************************************************************/
    /* Get the library name, library version and API version            */
    /* ******************************************************************/

#if DISPLAY_MESSAGE
    /* Get the library name string */
    err_code = (*p_xa_process_api)(NULL, XA_API_CMD_GET_LIB_ID_STRINGS,
                                   XA_CMD_TYPE_LIB_NAME, pb_process_name);

    _XA_HANDLE_ERROR(p_proc_err_info, "Lib-name Error", err_code);

    /* Get the library version string */
    err_code = (*p_xa_process_api)(NULL, XA_API_CMD_GET_LIB_ID_STRINGS,
                                   XA_CMD_TYPE_LIB_VERSION, pb_lib_version);

    _XA_HANDLE_ERROR(p_proc_err_info, "Lib-version Error", err_code);

    /* Display the Tensilica identification message */
    fprintf(stderr, "\n%s version %s\n", pb_process_name, pb_lib_version);
    fprintf(stderr, "Tensilica, Inc. http://www.tensilica.com\n\n");
#endif

    /* ******************************************************************/
    /* Initialize API structure and set config params to default        */
    /* ******************************************************************/

    /* Get the API size */
    err_code = (*p_xa_process_api)(NULL, XA_API_CMD_GET_API_SIZE, 0, &ui_api_size);

    _XA_HANDLE_ERROR(p_proc_err_info, "Api Size Error", err_code);

#if SHOW_SIZES
        printf("\nAPI structure size: %u bytes\n", ui_api_size);
#endif

    /* Allocate memory for API */
    p_xa_process_api_obj = MEM_malloc (ui_api_size, 4);

    /* ******************************************************************/
    /* Set the config params to default values                          */
    /* ******************************************************************/

    err_code = (*p_xa_process_api)(p_xa_process_api_obj, XA_API_CMD_INIT,
                                   XA_CMD_TYPE_INIT_API_PRE_CONFIG_PARAMS, NULL);

    _XA_HANDLE_ERROR(p_proc_err_info, "Pre Configuration Init Error", err_code);


    /* ******************************************************************/
    /* Set config parameters got from the user from argc argv           */
    /* ******************************************************************/

    err_code = parse_cmdline(p_xa_process_api_obj, argv, argc);
    if ( err_code )
    {
        fprintf(stderr, "User Configuration Error\n");
        err_code = -1;
        return err_code;
    }

    /* Allocate memory and Initialize */

    /* ******************************************************************/
    /* Initialize Memory info tables                                    */
    /* ******************************************************************/

    /* Get memory info tables size */
    err_code = (*p_xa_process_api)(p_xa_process_api_obj,
                                   XA_API_CMD_GET_MEMTABS_SIZE, 0, &ui_proc_mem_tabs_size);

    _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

#if SHOW_SIZES
        printf("\nMEMTABS size: %u bytes\n\n", ui_proc_mem_tabs_size);
#endif

    p_mem_tabs = MEM_malloc (ui_proc_mem_tabs_size, 4);

    /* Set pointer for process memory tables */
    err_code = (*p_xa_process_api)(p_xa_process_api_obj, XA_API_CMD_SET_MEMTABS_PTR, 0,
                                   p_mem_tabs);

    _XA_HANDLE_ERROR(p_proc_err_info, "Set mem-tab-ptr error", err_code);

    /* initialize the API, post config, fill memory tables */
    err_code = (*p_xa_process_api)(p_xa_process_api_obj, XA_API_CMD_INIT,
                                   XA_CMD_TYPE_INIT_API_POST_CONFIG_PARAMS, NULL);

    _XA_HANDLE_ERROR(p_proc_err_info, "post-config error", err_code);


    /*******************************************************************/
    /* Allocate Memory with info from library                           */
    /*******************************************************************/

    /* Get number of memory tables required */
    err_code = (*p_xa_process_api)(p_xa_process_api_obj, XA_API_CMD_GET_N_MEMTABS, 0, &n_mems);

    _XA_HANDLE_ERROR(p_proc_err_info, "Get-n-memtabs error", err_code);

    for(i = 0; i < (WORD32)n_mems; i++)
    {
        int ui_size, ui_alignment, ui_type;
        pVOID pv_alloc_ptr;

        /* Get memory size */
        err_code = (*p_xa_process_api)(p_xa_process_api_obj,
                                       XA_API_CMD_GET_MEM_INFO_SIZE, i, &ui_size);

        _XA_HANDLE_ERROR(p_proc_err_info, "Error in get-mem-info-size", err_code);

        /* Get memory alignment */
        err_code = (*p_xa_process_api)(p_xa_process_api_obj,
                                       XA_API_CMD_GET_MEM_INFO_ALIGNMENT, i, &ui_alignment);

        _XA_HANDLE_ERROR(p_proc_err_info, "Error in get-mem-info-alignment", err_code);

        /* Get memory type */
        err_code = (*p_xa_process_api)(p_xa_process_api_obj,
                                       XA_API_CMD_GET_MEM_INFO_TYPE, i, &ui_type);

        _XA_HANDLE_ERROR(p_proc_err_info, "Error in get-mem-info-type", err_code);

        pv_alloc_ptr = MEM_malloc (ui_size, ui_alignment);

        /* Set the buffer pointer */
        err_code = (*p_xa_process_api)(p_xa_process_api_obj, XA_API_CMD_SET_MEM_PTR, i, pv_alloc_ptr);

        _XA_HANDLE_ERROR(p_proc_err_info, "Error in set mem-ptr", err_code);

        if(ui_type == XA_MEMTYPE_INPUT)
        {
            pb_inp_buf = (pWORD8)pv_alloc_ptr;
            ui_inp_size = ui_size;
        }

        if(ui_type == XA_MEMTYPE_OUTPUT)
        {
            pb_out_buf = (pWORD8)pv_alloc_ptr;
        }

        /* Scratch test */
        SCRATCH_TRASH_TEST_ASSIGN

        /* Output ping pong test */
        OUTPUT_BUF_PING_PONG_TEST_ASSIGN

        /* Input ping pong test */
        INPUT_BUF_PING_PONG_TEST_ASSIGN

#if SHOW_SIZES
        switch (ui_type)
        {
        case XA_MEMTYPE_INPUT:
            printf("Input buffer size: %u bytes\n", ui_size);
            break;
        case XA_MEMTYPE_OUTPUT:
            printf("Output buffer size: %u bytes\n", ui_size);
            break;
        case XA_MEMTYPE_SCRATCH:
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
    ui_init_done = 0;

    unsigned short non_fatal_error_raw=0;
    int prev_bytes_consumed = 0;
    // [Bug 1918] Handling input over from sample test application
    // Required to avoid infinite loop
    int inp_over = 0;
    /* Seek for zeroth frame (frameCounter == 0) - Perform Initializations & parse the first frame */
    do
    {

        /* Move used data out of the Input buffer & fill new data in the input from file */
        xa_shift_input_buffer(pb_inp_buf, i_buff_size, i_bytes_consumed);

        i_buff_size -= i_bytes_consumed;

        prev_bytes_consumed = i_bytes_consumed;
        i_bytes_consumed = 0;

        i_bytes_read = ui_inp_size - i_buff_size;

        i_bytes_read = fread (pb_inp_buf+i_buff_size, 1, ui_inp_size - i_buff_size, g_pf_inp) ;

        i_buff_size += i_bytes_read;

        if(prev_bytes_consumed != i_bytes_read)
        //if (i_buff_size <= 0)
        {
            if (i_buff_size < 0)
            {
                i_buff_size = 0;
            }
            // [Bug 1918]  
            inp_over = 1;
            /* Tell that the input is over in this buffer */
            err_code = (*p_xa_process_api)(p_xa_process_api_obj, XA_API_CMD_INPUT_OVER, 0, NULL);

            _XA_HANDLE_ERROR(p_proc_err_info, "Input Over - Error", err_code);

        }

        /* Set number of bytes to be processed */
        err_code = (*p_xa_process_api)( p_xa_process_api_obj,
                                        XA_API_CMD_SET_INPUT_BYTES, 0, &i_buff_size);

        _XA_HANDLE_ERROR(p_proc_err_info, "Setting input-Bytes Error", err_code);

        /* Initialize the process */
        err_code_init = (*p_xa_process_api)(p_xa_process_api_obj, XA_API_CMD_INIT,
                                            XA_CMD_TYPE_INIT_PROCESS, NULL);

        if( ((err_code_init == XA_AACDEC_EXECUTE_FATAL_PARSING_ERROR) ||
            (err_code_init == XA_AACDEC_EXECUTE_FATAL_ZERO_FRAME_LENGTH)) && (inp_over == 0)
          )
        {
           if((non_fatal_error_raw))
           {
               /* for the first non-fatal frame error, skip the length equivalent to previous frame
                * skip byte-by-byte for subsequent frame errors  */
               i_bytes_consumed = 1;
           }
           else{
               i_bytes_consumed = prev_bytes_consumed;
           }
           non_fatal_error_raw = 1;
           // [Bug 1918] print the bits skipped to indicate the error
           // _XA_HANDLE_ERROR can't be introduced here as this is a special case of 
           // handling FATAL error 
           printf("Fatal error; skipping %d bytes and try again\n", i_bytes_consumed);
        }
        else
        {
            _XA_HANDLE_ERROR(p_proc_err_info, "Init-processing-error", err_code_init);
        
            /* How much buffer is used in input buffers */
            API_CALL( XA_API_CMD_GET_CURIDX_INPUT_BUF, 0, &i_bytes_consumed, "Input Buffer Consumed Check ");

            non_fatal_error_raw = 0;
        }

        /* Checking for end of initialization */
        err_code = (*p_xa_process_api)(p_xa_process_api_obj, XA_API_CMD_INIT,
                                       XA_CMD_TYPE_INIT_DONE_QUERY, &ui_init_done);

        _XA_HANDLE_ERROR(p_proc_err_info, "End of Init detect Error", err_code);

        /* Break if init is done */
        if (ui_init_done)  break;

    }
    while(1);   /* Init Loop */
    /* Init end of process condition end */

    /* ******************************************************************/
    /* Get config params from API                                       */
    /* ******************************************************************/

    /* Read parameters dervied by analyzing the input bit-streams */
    err_code = get_params(p_xa_process_api_obj,
                          &i_num_chan,
                          &acmod,
                          &i_samp_freq,
                          &bsfmt,
                          &i_data_rate);

    RETURN_ON_FATAL_ERROR(err_code);
    if ( err_code )
    {
        /* Non-fatal error */
        fprintf(stderr, "Invalid Parameters detected after initialization\n");
    }


    /* Get PCM Sample Size */
    GET_CONFIG( XA_AACDEC_CONFIG_PARAM_PCM_WDSZ, &pcm_sample_size, "Get PCM sample size");

#ifdef WAV_HEADER
    /* Write the outputfile wave header */
    write_wav_header(g_pf_out,i_samp_freq,i_num_chan,pcm_sample_size,
                     frameSize, frameCounter, i_total_bytes/(pcm_sample_size/8));
#endif

    ui_exec_done = 0;

#if STEP_PLAY
    int play_time_ms = 0;
    int mute_bytes = 0;
    FILE *pf_strm_pos = fopen("stream_position.txt", "rt");
    if (pf_strm_pos == NULL )
    {
        fprintf(stderr,"\nSTEP_PLAY: stream_position.txt file could not be opened\n\n");
        return 0;
    }
#endif
	if(startFrame < 0 || startFrame >= 32768)
	{
		printf("\nWARNING! Out of range value is set for startFrame and hence startFrame value is ignored.\n");
		startFrame = 0;
	}
	if(stopFrame < 0 || stopFrame >= 32768)
	{
		printf("\nWARNING! Out of range value is set for stopFrame and hence stopFrame value is ignored.\n");
		stopFrame = 0;
	}
	if((startFrame || stopFrame) && (startFrame > stopFrame))
	{
		printf("\nWARNING! startFrame # %d is greater than stopFrame # %d and hence startFrame value is ignored.\n", startFrame, stopFrame);
		startFrame = 0;
	}

    PROFILER_OPEN((&gProfiler[0]),"bsac_dec");

    /* Third part for executing each process */
    do /* until we run out of data from input file */
    {

#if STEP_PLAY
        if (play_time_ms >= PLAY_TIME_MS)
        {
            play_time_ms = 0;

            GET_CONFIG( XA_AACDEC_CONFIG_PARAM_DATA_RATE, &i_data_rate, "Get data rate of the stream");

            /* Mute the output for MUTE_TIME_MS milliseconds to separate
               the steps. */
            mute_bytes = ((MUTE_TIME_MS * i_samp_freq *
                           outnchans) / 1000) * (pcm_sample_size >> 3);

            int mute_bytes1 = mute_bytes;

            /* Based on the current bit rate, calculate the number of
               bytes that need to be skipped after consuming the bytes
               from the last EXECUTE call. */
            int skip_bytes = i_data_rate * SKIP_TIME_MS / (8 * 1000);
            int skip_bytes1 = skip_bytes;
            /*
             * [J478] - As BSAC decoder supports only raw bsac input, to test STEP_PLAY 
             *          stream position is provided through a text file. With the help 
             *          of stream positions available, after skipping specific number of 
             *          bytes syncing to next valid frame will be possible. This ensures 
             *          correct decoding of the stream.
             */
            unsigned int strm_pos ,curr_pos, next_pos;
            unsigned int cur_strm_pos;
            unsigned int fread_failed = 0;
            int fread_error;

            GET_CONFIG( XA_CONFIG_PARAM_CUR_INPUT_STREAM_POS, &cur_strm_pos, "Get current stream PCM position");

            if(skip_bytes != 0)
            {
                while(1)
                {
                    fread_error = fscanf(pf_strm_pos, "%u", &strm_pos);
                    if(fread_error != 1)
                    {
                        fread_failed = 1;
                        break;
                    }
                    else
                    {
                        if(strm_pos > cur_strm_pos)
                            break;
                    }
                }

                if(!fread_failed)
                {
                    curr_pos = strm_pos;
                    next_pos = curr_pos + skip_bytes;

                    while(1)
                    {
                        fread_error = fscanf(pf_strm_pos, "%u", &strm_pos);
                        if(fread_error != 1)
                        {
                            fread_failed = 1;
                            break;
                        }
                        else
                        {
                            if(next_pos <= strm_pos)
                                break;
                        }
                    }
                    if(fread_failed)
                    {
                        skip_bytes = 0;
                        mute_bytes = 0;
                    }
                    else
                    {
                        i_total_bytes += mute_bytes;
                        skip_bytes = strm_pos - curr_pos;
                    }

                }
                else
                { 
                    skip_bytes = 0;
                    mute_bytes = 0;
                }
            }

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

            fprintf(stderr, "Skip ahead by %d bytes\n", buf_skip+((skip_bytes>0)?skip_bytes:0));

            /* Read and Adjust current stream position */
            {
                unsigned int cur_strm_pos;
                GET_CONFIG( XA_CONFIG_PARAM_CUR_INPUT_STREAM_POS, &cur_strm_pos, "Get current stream PCM position");

                cur_strm_pos += (buf_skip + ((skip_bytes>0)?skip_bytes:0));

                SET_CONFIG( XA_CONFIG_PARAM_CUR_INPUT_STREAM_POS, &cur_strm_pos, "");
            }
            
            /* Send runtime init if either skiptime or mute time is non-zero */
            if(!fread_failed && (skip_bytes1 || mute_bytes1))
            {
                /* Runtime init to avoid clicks and distortions. */
                err_code = (*p_xa_process_api)(p_xa_process_api_obj, XA_API_CMD_EXECUTE,
                        XA_CMD_TYPE_DO_RUNTIME_INIT, NULL);
                _XA_HANDLE_ERROR(p_proc_err_info, "Runtime Init Error", err_code);
            }

        }
#endif /* STEP_PLAY */

        /* Copy to start. */
        xa_shift_input_buffer(pb_inp_buf, i_buff_size, i_bytes_consumed);

        i_buff_size -= i_bytes_consumed;

        prev_bytes_consumed = i_bytes_consumed;
        i_bytes_consumed = 0;

        i_bytes_read = ui_inp_size - i_buff_size;

        /* Fill data from input file */
        i_bytes_read = fread (pb_inp_buf+i_buff_size, 1, ui_inp_size - i_buff_size, g_pf_inp) ;

        i_buff_size += i_bytes_read;

        if(prev_bytes_consumed != i_bytes_read)
        {
            if (i_buff_size < 0)
            {
                i_buff_size = 0;
            }
            inp_over = 1;
            /* Tell that the input is over in this buffer */
            err_code = (*p_xa_process_api)(p_xa_process_api_obj, XA_API_CMD_INPUT_OVER, 0, NULL);

            _XA_HANDLE_ERROR(p_proc_err_info, "Input Over - Error", err_code);

        }

        /* Set number of bytes to be processed */
        err_code = (*p_xa_process_api)( p_xa_process_api_obj,
                                        XA_API_CMD_SET_INPUT_BYTES, 0, &i_buff_size);

        _XA_HANDLE_ERROR(p_proc_err_info, "Setting input-Bytes Error", err_code);

        PROFILER_START((&gProfiler[0]))

        /* Execute process */
        err_code_exec = (*p_xa_process_api)(p_xa_process_api_obj, XA_API_CMD_EXECUTE,
                                            XA_CMD_TYPE_DO_EXECUTE, NULL);

        PROFILER_STOP((&gProfiler[0]))

        if( ((err_code_exec == XA_AACDEC_EXECUTE_FATAL_PARSING_ERROR) ||
            (err_code_exec == XA_AACDEC_EXECUTE_FATAL_ZERO_FRAME_LENGTH)) && (inp_over == 0)
          )
        {
           if((non_fatal_error_raw))
           {
               /* for the first non-fatal frame error, skip the length equivalent to previous frame
                * skip byte-by-byte for subsequent frame errors  */
               i_bytes_consumed = 1;
           }else{
               i_bytes_consumed = prev_bytes_consumed;
           }
           non_fatal_error_raw = 1;
           // [Bug 1918] print the bits skipped to indicate the error
           // _XA_HANDLE_ERROR can't be introduced here as this is a special case of 
           // handling FATAL error 
           printf("Fatal error; skipping %d bytes and try again\n", i_bytes_consumed);
        }
        else
        {
          _XA_HANDLE_ERROR(p_proc_err_info, "Decoding Error", err_code_exec);
        
          /* How much buffer is used in input buffers */
          API_CALL( XA_API_CMD_GET_CURIDX_INPUT_BUF, 0, &i_bytes_consumed, "Input Buffer Consumed Check ");

           non_fatal_error_raw = 0;
        }

        /* Checking for end of processing */
        err_code = (*p_xa_process_api)(p_xa_process_api_obj, XA_API_CMD_EXECUTE,
                                       XA_CMD_TYPE_DONE_QUERY, &ui_exec_done);

        _XA_HANDLE_ERROR(p_proc_err_info, "End-exec Check Error", err_code);

        /* Get the output bytes */
        err_code = (*p_xa_process_api)(p_xa_process_api_obj, XA_API_CMD_GET_OUTPUT_BYTES,
                                       0, &i_out_bytes);

        _XA_HANDLE_ERROR(p_proc_err_info, "Get Output Bytes Error", err_code);

        if ( i_out_bytes ) /* Frame decoded */
        {
            if(pcm_sample_size == 16)
                frameSize = (i_out_bytes/2)/outnchans;
            else
                frameSize = (i_out_bytes/4)/outnchans;

            GET_CONFIG( XA_CONFIG_PARAM_GEN_INPUT_STREAM_POS, &gProfiler[0].streampos, "Get Generated PCM Position");

            PROFILER_UPDATE( (&gProfiler[0]), frameSize, i_samp_freq )

            /* write audio channels to pcm file */
            if (frameCounter >= startFrame)
            {
                i_total_bytes += (pcm_write((long *)pb_out_buf,chanmap,frameSize,
                                            g_pf_out,pcm_sample_size))*pcm_sample_size/8;

            }
            frameCounter++;

            if (stopFrame && (frameCounter >= stopFrame))
            {
                break;
            }


            /* every 16 frames, one can get the bitrate estimate */
            if ((frameCounter & 0xf) == 0)
            {
                GET_CONFIG( XA_AACDEC_CONFIG_PARAM_DATA_RATE, &i_data_rate, "Get data rate of the stream");
            }
        }

#if STEP_PLAY
        if(pcm_sample_size == 16)
        {
            frameSize = (i_out_bytes/2)/outnchans;
        }
        else
        {
            frameSize = (i_out_bytes/4)/outnchans;

        }
        /* Keep track of the play back time. */
        play_time_ms += (frameSize * 1000) / (i_samp_freq);

        /* Mute the output after ramp-down. */
        if (err_code_exec == XA_NO_ERROR)
        {
            for (; mute_bytes > 0; mute_bytes--)
            {
                putc(0, g_pf_out);
            }
        }
#endif /* STEP_PLAY */

        /* Check if done */
        if (ui_exec_done) break ;

    }
    while (1);   /* Loop to feed new data chunks from the input file */

#if STEP_PLAY
    if(pf_strm_pos)
        fclose(pf_strm_pos);
#endif

    PROFILER_CLOSE((&gProfiler[0]));

    if (err_code != 0)
    {
        fprintf(stderr,"error while decoding stream\n");
    }

#ifdef WAV_HEADER
    if(!fseek(g_pf_out,0,SEEK_SET))
    {
        write_wav_header(g_pf_out, i_samp_freq, i_num_chan, pcm_sample_size, frameSize, frameCounter, i_total_bytes/(pcm_sample_size/8));
    }
#endif
    return 0;
}

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

/****************************************************************************
 *   Main function for test-wrapper                                         *
 ****************************************************************************/

int
main (int   argc, char *argv[])
{
    FILE *param_file_id;

    char curr_cmd[MAX_CMD_LINE_LENGTH];
    int fargc, curpos;
    int processcmd = 0;

    char fargv[XA_MAX_ARGS][MAX_CMD_LINE_LENGTH];

    char *pargv[XA_MAX_ARGS];

    char pb_input_file_path[MAX_CMD_LINE_LENGTH] = "";
    char pb_output_file_path[MAX_CMD_LINE_LENGTH] = "";

    // NOTE: set_wbna() should be called before any other dynamic
    // adjustment of the region attributes for cache.
    set_wbna(&argc, argv);

    xa_testbench_error_handler_init();

    if(argc == 1)
    {
        if ((param_file_id = fopen(PARAMFILE, "r")) == NULL )
        {
            printf("Parameter file \"paramfilesimple.txt\" not found.\n");
            printf("Command line usage:\n");
            printf("<decoder> -ifile:<infile> -ofile:<outfile>\n");
            printf("Detailed description of the options: <decoder> -h\n");
            exit(0);
        }

        /* Process one line at a time */
        while(fgets(curr_cmd, MAX_CMD_LINE_LENGTH, param_file_id))
        {
            curpos = 0;
            fargc = 0;
            /* if it is not a param_file command and if */
            /* CLP processing is not enabled */
            if(curr_cmd[0] != '@' && !processcmd)     /* skip it */
            {
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
                    fgets(curr_cmd + curpos, MAX_CMD_LINE_LENGTH,
                          param_file_id);
                    continue;
                }
                curpos += strlen(fargv[fargc]);
                while(*(curr_cmd + curpos)==' ' || *(curr_cmd + curpos)=='\t')
                    curpos++;
                fargc++;
            }

            if(fargc < 1) /* for blank lines etc. */
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
                int err_code = XA_NO_ERROR;
                int file_flag = 0;

                printf("\n");
                for(i = 0; i < fargc; i++)
                {
                    printf("%s ", fargv[i]);
                    pargv[i] = fargv[i];

                    if(!strncmp(fargv[i], "-ifile:", 7))
                    {
                        char * pb_arg_val = fargv[i] + 7;
                        char pb_input_file_name[MAX_CMD_LINE_LENGTH] = "";

                        file_flag++;
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
                        char * pb_arg_val = fargv[i] + 7;
                        char pb_output_file_name[MAX_CMD_LINE_LENGTH] = "";

                        file_flag++;
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

                printf("\n");

                /* Allow io files are specified by either -ifile/ofile or -i & -o */
                if((file_flag != 2) && (file_flag !=0))
                {
                    err_code = XA_TESTBENCH_MFMAN_FATAL_FILE_OPEN_FAILED;
                    printf("Error: Unable to open input/output file.\n");
                    printf("Command line usage:\n");
                    printf("<decoder> -ifile:<infile> -ofile:<outfile>\n");
                    printf("Detailed description of the options: <decoder> -h\n");
                    xa_error_handler(&xa_testbench_error_info,
                                     "Input or Output File", err_code);
                }
                if(err_code == XA_NO_ERROR)
                {
                    MEM_init();
                    xa_aac_dec_main_process(fargc, pargv);
                }

                MEM_freeall();

                if(g_pf_inp)
                {
                    fclose(g_pf_inp);
                }
                if(g_pf_out)
                {
                    fclose(g_pf_out);
                }
            }
        }
    }
    else
    {
        int i;
        int err_code = XA_NO_ERROR;
        int file_flag = 0;

        printf("\n");
        for(i = 1; i < argc; i++)
        {
            printf("%s ", argv[i]);
            if(!strcmp(argv[i], "-h"))
            {
                showusage();
                return(0);
            }

            if(!strncmp(argv[i], "-ifile:", 7))
            {
                char * pb_arg_val = argv[i] + 7;
                char pb_input_file_name[MAX_CMD_LINE_LENGTH] = "";

                file_flag++;
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
                char * pb_arg_val = argv[i] + 7;
                char pb_output_file_name[MAX_CMD_LINE_LENGTH] = "";

                file_flag++;
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
        printf("\n");

        /* Allow io files are specified by either -ifile/ofile or -i & -o */
        if((file_flag != 2) && (file_flag !=0))
        {
            err_code = XA_TESTBENCH_MFMAN_FATAL_FILE_OPEN_FAILED;
            fprintf(stderr, "Error: Unable to open input/output file.\n");
            fprintf(stderr, "Command line usage:\n");
            fprintf(stderr, "<decoder> -ifile:<infile> -ofile:<outfile>\n");
            fprintf(stderr, "Detailed description of the options: <decoder> -h\n");
            xa_error_handler(&xa_testbench_error_info,
                             "Input or Output File", err_code);
        }

        if(err_code == XA_NO_ERROR)
        {
            MEM_init();
            xa_aac_dec_main_process(argc - 1, &argv[1]);

        }

        MEM_freeall();

        if(g_pf_inp)
        {
            fclose(g_pf_inp);
        }
        if(g_pf_out)
        {
            fclose(g_pf_out);
        }
    }


    return XA_NO_ERROR;
} /* end main decode function */
 
void MEM_init(void)
{
    g_w_malloc_count = 0;
}

void *MEM_malloc (int size, int align)
{
    void *return_ptr;
    if ( size == 0)
    {
        return NULL;
    }
    /* Allocate memory for API */
    g_pv_arr_alloc_memory[g_w_malloc_count] = malloc(size);

    if (g_pv_arr_alloc_memory[g_w_malloc_count] == NULL)
    {

        return NULL;
    }
    memset(g_pv_arr_alloc_memory[g_w_malloc_count], 0, size);

    /* API object requires 4 bytes (WORD32) alignment;
     *      * malloc() provides at least 8-byte alignment.
     *           */
    if(((unsigned int) g_pv_arr_alloc_memory[g_w_malloc_count]) & (align-1))
    {
        // Alignment not met; free and return NULL
        free(g_pv_arr_alloc_memory[g_w_malloc_count]);
        return NULL;
    }
    return_ptr = g_pv_arr_alloc_memory[g_w_malloc_count];
    g_w_malloc_count++;

    return return_ptr;
}

void MEM_freeall(void)
{
    int i;
    for(i = 0; i < g_w_malloc_count; i++)
    {
        if(g_pv_arr_alloc_memory[i])
        {
            free(g_pv_arr_alloc_memory[i]);
        }
    }
}

