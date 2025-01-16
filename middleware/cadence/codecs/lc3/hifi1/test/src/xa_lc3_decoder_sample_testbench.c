/*
 * Copyright (c) 2019-2023 Cadence Design Systems, Inc.
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


/* turn off stdlib function warnings in visual studio */
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "xa_lc3_codec_api.h"
#include "xa_error_handler.h"
#include "tinywaveout_c.h"
#include "assert.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))

#define ASSERT(test) assert(test)

#define SHOW_SIZES
#define INIT_SYMBOL	0

#define XA_MAX_CMD_LINE_LENGTH 300
#define XA_MAX_ARGS 20
#define PARAMFILE "paramfilesimple_decode.txt"

/* G192 bitstream writing/reading */
#define G192_GOOD_FRAME 0x6B21
#define G192_BAD_FRAME 0x6B20
#define G192_REDUNDANCY_FRAME 0x6B22
#define G192_ZERO 0x007F
#define G192_ONE 0x0081
#define READ_G192FER      /* allow C executable to also read G192 formatted FER files */


/*****************************************************************************/
/* Error codes for the testbench                                             */
/*****************************************************************************/
#define XA_TESTBENCH_FATAL_MEM_ALLOC_FAILED                0xFFFF8000
#define XA_TESTBENCH_FATAL_FILE_OPEN_FAILED                0xFFFF8001

#ifdef OUTPUT_BUF_PING_PONG_TEST
pWORD32  pb_out_buf_ping = NULL;
pWORD32  pb_out_buf_pong = NULL;
WORD32 pb_out_buf_pp_count = 0;
#endif

#ifdef __XTENSA__
#define __PROF_ALLOCATE__
#ifndef __MCPS_MEASUREMENT__
#define __MCPS_MEASUREMENT__
#endif
#endif

#ifdef __MCPS_MEASUREMENT__
#include <xtensa/tie/xt_hifi2.h>
#include <sys/times.h>
#include <xtensa/sim.h>
/* Following variable shows the frame number that will be profiled */
/* Set profile_frame = 0 to profile all frames */
#endif

#ifdef __PROF_ALLOCATE__
#include <xtensa/xt_profiling.h>
#define PROFILER_START {xt_profile_enable();}
#define PROFILER_STOP  {xt_profile_disable();}

#define PROFILER_FRAME   -1// -1 - all w/o stop for get mcps
#else
#define PROFILER_START
#define PROFILER_STOP
#endif

#ifdef __MCPS_MEASUREMENT__

double u_seconds;
struct tms start, stop;

struct mcpsAlgType {
    clock_t cycles;
    double mcps_peak;
    double mcps_curr;
    double mcps_ave;
    double mcps_sum;
    WORD32 peak_frame;
    const pWORD8 nameAlg;
};

static struct mcpsAlgType mcpsforAlgs[] = {
    { 0, 0, 0, 0, 0, 0, "lc3_dec" }
};

#endif


/* struct to hold command line arguments */
typedef struct
{
    pWORD8 inputFilename;
    pWORD8 outputFilename;
    WORD32 bitrate;
    WORD32 bipsOut;
    WORD32 dc;
    int   formatG192;
    char *configFilenameG192;
    float frame_ms;
    WORD32 hide_counter;
    WORD32 plcMeth;
    pWORD8 epf;
    int   epmode;
    int hrmode;
    pWORD8 edf;
    int  startFrame;
    int  stopFrame;
} Arguments_dec;

/* local helper functions */
static VOID parseCmdl_dec(WORD32 ac, pWORD8* av, Arguments_dec* arg);
static VOID exit_if(WORD32 condition, const pWORD8 message);
VOID interleave(pWORD32 in, pWORD32 out, WORD32 n, WORD32 channels, WORD32 bipsout);
static FILE *open_bitstream_reader(const char *file, UWORD32 *samplerate, WORD32 *bitrate, WORD16 *channels,UWORD32 *signal_len, float *frame_ms, int *epmode,int *hrmode, int g192, const char *file_cfg);
WORD32 read_bitstream_frame(FILE* bitstream_file, pUWORD8 bytes, WORD32 size, int g192,  int *bfi_ext);
WORD16 loopy_read16(FILE* f);
static VOID safe_fclose(FILE* f);

VOID xa_lc3_error_handler_init();
VOID xa_testbench_error_handler_init();

extern xa_error_info_struct xa_lc3_error_info;
extern xa_error_info_struct xa_testbench_error_info;

/*****************************************************************************/
/* Global variables                                                          */
/*****************************************************************************/

static const pWORD8 USAGE_MESSAGE_DEC =
"Usage: xa_lc3_dec_test [OPTIONS] <INPUT> <OUTPUT> \n"
"\n"
"  INPUT is bitstream file, OUTPUT is wav file.\n"
"\nOptions:\n"
"  -h                  Show this message.\n"
"  -q                  Hide frame counter.\n"
"\nCore codec options:\n"
"  -bps NUM            Output bits per sample. NUM must be 16 (default) or 24.\n"
"  -formatG192         Activate G192 bitstream format. A filename.cfg will be used to\n"
"                      store/load decoder info.\n"
"  -cfgG192 FILE       Specify a configuration file for G192 bitstream format.\n"
"  -dc NUM                   0: Don't use delay compensation\n"
"                            1: Compensate delay in decoder (default)\n"
"                            2: Split delay equally between encoder and decoder\n"
"  -y                  StartFrame: frame number where encoding/decoding shall start\n"
"  -z                  StopFrame: frame number where encoding/decoding shall stop\n"
"\nPLC options:\n"
"  -epf FILE           Enable packet loss simulation using error pattern from FILE.\n"
"  -edf FILE           Write error pattern as detected to FILE.\n"
"  \n";

static const pWORD8 MISSING_ARGUMENT_MESSAGE = "Not enough parameters! Use -h to show help.";
static const pWORD8 INVALID_ARGUMENT_MESSAGE = "Incorrect Input parameters! Use -h to show help.";

static WAVEFILEOUT* output_wav = NULL;
static FILE* input_bitstream = NULL;
static FILE* error_pattern_file = NULL;
static FILE* error_detection_file = NULL;

/*******************************************************************************
*
*  Function name : xa_lc3_dec_main_process
*
*  Description   : 
*                  
*
*  parameters    : WORD32 argc (Arguments count)
*                  pWORD8 argv[] (Argument strings)
*
*  Return value  : none
******************************************************************************/
XA_ERRORCODE xa_lc3_dec_main_process(WORD32  argc, pWORD8 argv[])
{
    Arguments_dec arg;
    UWORD32      nSamplesFile = 0xffffffff;
    WORD32       nSamples = 0, nBytes = 0, nSamples_out;
    WORD32       frame = 1;
    WORD32       delay = 0;
    WORD32   sample_buf[XA_LC3_MAX_CHANNELS * XA_LC3_MAX_SAMPLES];
    xa_error_info_struct *p_proc_codec_err_info = NULL;
    xa_error_info_struct *p_proc_testbench_err_info = NULL;
    WORD32      handle_size = 0, scratch_size = 0, scratch_size_init = 0, inp_buf_size = 0, out_buf_size = 0;
    XA_ERRORCODE error_code = XA_NO_ERROR;
    xa_lc3_dec_init_cfg_t dec_init_cfg;
    xa_codec_handle_t decoder_state = NULL;
    pWORD32     scratch_init = NULL;
    pWORD32     scratch = NULL;
    pUWORD8     inp_buf = NULL;
    pWORD32     out_buf = NULL;

    output_wav = NULL;
    input_bitstream = NULL;
    error_pattern_file = NULL;
    error_detection_file = NULL;
    int bfi_ext=0;


#ifdef __MCPS_MEASUREMENT__
    xt_iss_client_command("all", "disable");
#endif

    /* ******************************************************************/
    /* Initialize the error handler : testbench                         */
    /* ******************************************************************/
    p_proc_testbench_err_info = (xa_error_info_struct*)&xa_testbench_error_info;
    xa_testbench_error_handler_init();

    /* ******************************************************************/
    /* Initialize the error handler : lc3 codeclibrary                 */
    /* ******************************************************************/
    p_proc_codec_err_info = (xa_error_info_struct*)&xa_lc3_error_info;
    xa_lc3_error_handler_init();

    /* Library name version etc print */
    fprintf(stderr, "\n");
    fprintf(stderr, " %s library version %s\n",
        xa_lc3_get_lib_name_string(),
        xa_lc3_get_lib_version_string());
    fprintf(stderr, " API version: %s\n", xa_lc3_get_lib_api_version_string());
    fprintf(stderr, " Cadence, Inc. https://www.cadence.com\n");
    fprintf(stderr, "\n");

    /************************************************/
    /* Get arguments                                */
    /************************************************/
    exit_if(argc <= 1, USAGE_MESSAGE_DEC);

    /* Default settings */
    memset(&arg, 0, sizeof(arg));
    arg.bipsOut = XA_LC3_BIPS_OUT_DEF_VAL;

    /* Parse Command-line */
    parseCmdl_dec(argc, argv, &arg);

    exit_if(arg.bipsOut != XA_LC3_BIPS_VAL_16 && arg.bipsOut != XA_LC3_BIPS_VAL_24, "Only 16 or 24 bits per sample are supported!");


    dec_init_cfg.bipsOut = arg.bipsOut;
    dec_init_cfg.sampleRate = XA_LC3_SAMPLE_RATE_DEF_VAL;
    dec_init_cfg.nChannels = XA_LC3_NUM_OF_CH_DEF_VAL;

    /* Open Input Bitstream File */
    input_bitstream = open_bitstream_reader(arg.inputFilename, &dec_init_cfg.sampleRate, &arg.bitrate, &dec_init_cfg.nChannels,&nSamplesFile,&arg.frame_ms, &arg.epmode,&arg.hrmode, arg.formatG192, arg.configFilenameG192);
    exit_if(input_bitstream == NULL, "Error opening bitstream file!");
    exit_if(!nSamplesFile, "Nothing to decode, zero data!");
    exit_if(!dec_init_cfg.nChannels, "Nothing to decode, zero channels!");
    exit_if(arg.hrmode == 1, "HR bitstreams not supported!");

    /************************************************************
    *                     Get Memory sizes from library        *
    ************************************************************/
    handle_size = xa_lc3_dec_get_handle_byte_size(&dec_init_cfg);

    if(handle_size < 0)    
        _XA_HANDLE_ERROR(p_proc_codec_err_info, "", handle_size);

    scratch_size_init = XA_LC3_DEC_MAX_USER_SYSTEM_SCRATCH_SIZE;

    if (scratch_size_init < 0)
        _XA_HANDLE_ERROR(p_proc_codec_err_info, "", scratch_size_init);

    inp_buf_size = XA_LC3_MAX_BYTES;

    if(arg.bipsOut == XA_LC3_BIPS_VAL_16)
        out_buf_size = XA_LC3_MAX_CHANNELS * XA_LC3_MAX_SAMPLES * sizeof(WORD16);
    else //arg.bipsOut = XA_LC3_BIPS_VAL_24
        out_buf_size = XA_LC3_MAX_CHANNELS * XA_LC3_MAX_SAMPLES * sizeof(WORD32);

    /************************************************************
    *                     Allocate Memory                       *
    ************************************************************/
    decoder_state = (xa_codec_handle_t)malloc(handle_size);
    if (decoder_state == NULL)
    {
        _XA_HANDLE_ERROR(p_proc_testbench_err_info, "API state alloc", XA_TESTBENCH_FATAL_MEM_ALLOC_FAILED);
    }

    memset(decoder_state, INIT_SYMBOL, handle_size);



    inp_buf = (pUWORD8)malloc(inp_buf_size);
    if (inp_buf == NULL)
    {
        _XA_HANDLE_ERROR(p_proc_testbench_err_info, "API input alloc", XA_TESTBENCH_FATAL_MEM_ALLOC_FAILED);
    }

    memset(inp_buf, INIT_SYMBOL, inp_buf_size);

    out_buf = (pWORD32)malloc(out_buf_size);
    if (out_buf == NULL)
    {
        _XA_HANDLE_ERROR(p_proc_testbench_err_info, "API output alloc", XA_TESTBENCH_FATAL_MEM_ALLOC_FAILED);
    }
    memset(out_buf, INIT_SYMBOL, out_buf_size);

#ifdef SHOW_SIZES
    fprintf(stderr, "\nPersistent state size: %d bytes\n", handle_size);
    fprintf(stderr, "Input buffer size:     %d bytes\n", inp_buf_size);
    fprintf(stderr, "Output buffer size:    %d bytes\n", out_buf_size);
#endif

    /*******************************************************************
    *                  Initialization.                                *
    *******************************************************************/
    
		scratch_init = (pWORD32)malloc(scratch_size_init);
    if (scratch_init == NULL)
    {
        _XA_HANDLE_ERROR(p_proc_testbench_err_info, "API scratch alloc", XA_TESTBENCH_FATAL_MEM_ALLOC_FAILED);
    }

    memset(scratch_init, INIT_SYMBOL, scratch_size_init);

            nSamples_out = nSamples * dec_init_cfg.nChannels;
    error_code = xa_lc3_dec_init(decoder_state, (pWORD32)scratch_init, &dec_init_cfg, nSamples_out, (WORD16)(arg.frame_ms*10));
    _XA_HANDLE_ERROR(p_proc_codec_err_info, "", error_code);
    
    /* Freeing up scratch size used for intialization */
    free(scratch_init);
		
		scratch_size = xa_lc3_dec_get_scratch_byte_size(decoder_state);

    if (scratch_size < 0)
        _XA_HANDLE_ERROR(p_proc_codec_err_info, "", scratch_size);
    
#ifdef SHOW_SIZES
		fprintf(stderr, "Scratch buffer size:   %d bytes\n\n", scratch_size);
#endif
    
		scratch = (pWORD32)malloc(scratch_size);
    
    if (scratch == NULL)
    {
        _XA_HANDLE_ERROR(p_proc_testbench_err_info, "API scratch alloc", XA_TESTBENCH_FATAL_MEM_ALLOC_FAILED);
    }

    memset(scratch, INIT_SYMBOL, scratch_size);

    
    WORD16 frame_dms = (WORD16)(arg.frame_ms*10);
    error_code = xa_lc3_dec_set_param(decoder_state, XA_LC3_DEC_PARAM_FRAME_DMS, &frame_dms);
     _XA_HANDLE_ERROR(p_proc_codec_err_info, "", error_code); 
    
     error_code = xa_lc3_dec_get_param(decoder_state, XA_LC3_DEC_PARAM_FRAME_LENGTH, &nSamples);
    _XA_HANDLE_ERROR(p_proc_codec_err_info, "", error_code);
if(arg.dc)
{
    error_code = xa_lc3_dec_get_param(decoder_state, XA_LC3_DEC_PARAM_DELAY_COMPENSATION, &delay);
    delay = delay/arg.dc;
    _XA_HANDLE_ERROR(p_proc_codec_err_info, "", error_code);
}
    /* Open Output Wav File */
    output_wav = CreateWav(arg.outputFilename, dec_init_cfg.sampleRate, dec_init_cfg.nChannels, arg.bipsOut);
    exit_if(output_wav == NULL, "Error creating wav file!");

    /* open auxillary files */
    if (arg.epf)
    {
        error_pattern_file = fopen(arg.epf, "rb");
        exit_if(error_pattern_file == NULL, "Error opening error pattern file!");
    }

    if (arg.edf)
    {
        error_detection_file = fopen(arg.edf, "wb");
        exit_if(error_detection_file == NULL, "Error creating error detection file!");
    }    

    fprintf(stderr, "Sample rate:        %i\n", dec_init_cfg.sampleRate);
    fprintf(stderr, "Number of channels: %i\n", dec_init_cfg.nChannels);
    fprintf(stderr, "Frame length:       %i\n", nSamples);
    fprintf(stderr, "Output format:      %i bit\n", arg.bipsOut);
    fprintf(stderr, "Target Bitrate:     %i\n\n", arg.bitrate);

#ifdef OUTPUT_BUF_PING_PONG_TEST

    pb_out_buf_ping = (pWORD32)malloc(out_buf_size);
    if (out_buf == NULL)
    {
        _XA_HANDLE_ERROR(p_proc_testbench_err_info, "API output alloc", XA_TESTBENCH_FATAL_MEM_ALLOC_FAILED);
    }

    pb_out_buf_pong = (pWORD32)malloc(out_buf_size);
    if (out_buf == NULL)
    {
        _XA_HANDLE_ERROR(p_proc_testbench_err_info, "API output alloc", XA_TESTBENCH_FATAL_MEM_ALLOC_FAILED);
    }

#endif

/* Decoder loop */
    while (1)
    {
        if (frame < arg.startFrame)
        {
            goto while_end;
        }

            /* Read bitstream */
            nBytes = read_bitstream_frame(input_bitstream, inp_buf, XA_LC3_MAX_BYTES, arg.formatG192, &bfi_ext);
            if (nBytes < 0)
            {
                break;
            }

            /* read error pattern */
            if (error_pattern_file && loopy_read16(error_pattern_file))
            {
                nBytes = 0; /* tell decoder packet is lost and needs to be concealed */
            }

            nSamples_out = nSamples * dec_init_cfg.nChannels;

            /* Run Decoder */

#ifdef OUTPUT_BUF_PING_PONG_TEST
            if ((pb_out_buf_pp_count++) & 1)
            {
                out_buf = pb_out_buf_ping;
            }
            else
            {
                out_buf = pb_out_buf_pong;
            }
#endif


#ifdef __PROF_ALLOCATE__
            if (PROFILER_FRAME == -1)
            {
                PROFILER_START
            }
            else if (PROFILER_FRAME == frame)
            {
                PROFILER_START
            }
#endif
#ifdef __MCPS_MEASUREMENT__
#ifdef __PROF_ALLOCATE__
            if (PROFILER_FRAME == -1)
                xt_iss_client_command("all", "enable");
#else
            xt_iss_client_command("all", "enable");
#endif				
            times(&start);
#endif
            error_code = xa_lc3_dec_process(decoder_state, inp_buf, out_buf, scratch, (pUWORD32)&nBytes, (pUWORD32)&nSamples_out,scratch_size,bfi_ext);

#ifdef __MCPS_MEASUREMENT__
            times(&stop);
            xt_iss_client_command("all", "disable");
            mcpsforAlgs[0].cycles = stop.tms_utime - start.tms_utime;
#endif
#ifdef __PROF_ALLOCATE__
            if (PROFILER_FRAME == -1)
            {
                PROFILER_STOP
            }
            else if (PROFILER_FRAME == frame)
            {
                PROFILER_STOP
                    exit(0);
            }
#endif
        
            _XA_HANDLE_ERROR(p_proc_codec_err_info, "", error_code);

            /* write error detection to file */
            if (error_detection_file != NULL)
            {
                 WORD16 tmp = (error_code == XA_LC3_EXECUTE_NONFATAL_DECODE_ERROR);
                 fwrite(&tmp, 2, 1, error_detection_file);
            }

            /* interleave samples for writing */
            interleave(out_buf, sample_buf, nSamples_out / dec_init_cfg.nChannels, dec_init_cfg.nChannels, dec_init_cfg.bipsOut);

            /* Write frame to file */
            WriteWavLong(output_wav, sample_buf + delay * dec_init_cfg.nChannels ,MIN(nSamples_out/dec_init_cfg.nChannels-delay,nSamplesFile) * dec_init_cfg.nChannels);
            nSamplesFile -= nSamples - delay;
            delay = 0;

#ifdef __MCPS_MEASUREMENT__
        {
            WORD32 cnt = 0;
            {
                u_seconds = ((double)nSamples * 1000000) / dec_init_cfg.sampleRate;
                mcpsforAlgs[cnt].mcps_curr = (double)mcpsforAlgs[cnt].cycles / u_seconds;
                mcpsforAlgs[cnt].mcps_sum += mcpsforAlgs[cnt].mcps_curr;
                mcpsforAlgs[cnt].mcps_ave = mcpsforAlgs[cnt].mcps_sum / frame;
                if (mcpsforAlgs[cnt].mcps_peak < mcpsforAlgs[cnt].mcps_curr)
                {
                    mcpsforAlgs[cnt].mcps_peak = mcpsforAlgs[cnt].mcps_curr;
                    mcpsforAlgs[cnt].peak_frame = frame;
                }

                fprintf(stderr, "%s [%d] MCPS: %f Average: %f Peak: %f @ [%d]\n",
                    mcpsforAlgs[cnt].nameAlg, frame, mcpsforAlgs[cnt].mcps_curr,
                    mcpsforAlgs[cnt].mcps_ave, mcpsforAlgs[cnt].mcps_peak,
                    mcpsforAlgs[cnt].peak_frame);

            }
        }
#endif

        //frame++;
while_end:
        if (!arg.hide_counter)
        {
            if ((frame >= arg.startFrame && frame <= arg.stopFrame && arg.stopFrame != 0) || (arg.stopFrame == 0 && arg.startFrame == 0))
            {
                printf("Frame %i\n", frame);
                fflush(stdout);
            } 
        }
        frame++;
        
        if (frame > arg.stopFrame && arg.stopFrame != 0)
        {
            break;
        }
    }

    if (nSamplesFile > 0 && nSamplesFile < nSamples)
    {
        memset(sample_buf, INIT_SYMBOL, (nSamplesFile * dec_init_cfg.nChannels) * sizeof(sample_buf[0]));
        WriteWavLong(output_wav, sample_buf, nSamplesFile * dec_init_cfg.nChannels);
    }
    puts("\nProcessing done!");
    if (output_wav)
    {
        printf("%llu samples clipped!\n", output_wav->clipCount);
    }
    
    free(scratch);
    free(out_buf);
    free(inp_buf);
    free(decoder_state);

#ifdef __MCPS_MEASUREMENT__
    {
        WORD32 cnt = 0;
        {
            FILE *mcps_log_file = fopen("mcps_report.txt", "at");

            fprintf(stderr, "---------- [%s] ----------\n", mcpsforAlgs[cnt].nameAlg);
            fprintf(stderr, "Peak MCPS = %f\n", mcpsforAlgs[cnt].mcps_peak);
            fprintf(stderr, "Peak frame = %d\n", mcpsforAlgs[cnt].peak_frame);
            fprintf(stderr, "Average MCPS = %f\n", mcpsforAlgs[cnt].mcps_ave);
            fprintf(stderr, "Total frames  = %d\n", --frame);
            fprintf(mcps_log_file, "Decoder: ");
            fprintf(mcps_log_file, " %s  %s \n", arg.inputFilename, arg.outputFilename);
            fprintf(mcps_log_file, "Peak MCPS = %f\n", mcpsforAlgs[cnt].mcps_peak);
            fprintf(mcps_log_file, "Peak frame = %d\n", mcpsforAlgs[cnt].peak_frame);
            fprintf(mcps_log_file, "Average MCPS = %f\n", mcpsforAlgs[cnt].mcps_ave);
            fprintf(mcps_log_file, "Total frames  = %d\n",--frame);
            fclose(mcps_log_file);
        }
        mcpsforAlgs[cnt].mcps_peak = 0;
        mcpsforAlgs[cnt].peak_frame = 0;
        mcpsforAlgs[cnt].mcps_ave = 0;
        mcpsforAlgs[cnt].cycles = 0;
        mcpsforAlgs[cnt].mcps_sum = 0;
        mcpsforAlgs[cnt].mcps_curr = 0;
    }
#endif

    printf("Done.\n\n\n");
    return XA_NO_ERROR;

} /* xa_lc3_dec_main_process */


  // Set cache attribute to Write Back No Allocate when the last argument is -wbna
VOID set_wbna(pWORD32 argc, pWORD8 argv[])
{
    if (*argc > 1 && !strcmp(argv[*argc - 1], "-wbna"))
    {
#ifdef __XCC__
        extern WORD8 _memmap_cacheattr_wbna_trapnull;
        xthal_set_cacheattr((unsigned)& _memmap_cacheattr_wbna_trapnull);
#endif
        (*argc)--;
    }
}

/*******************************************************************************
*
*  Function Name : main
*
*  Description   : Parse the parameter file and run the main process
*
*  parameters    : None
*
*  return value  : 0 on success, -1 on error
*
******************************************************************************/

WORD32 main(WORD32 argc, pWORD8 argv[])
{
    FILE* param_file_id;
    WORD8 curr_cmd[XA_MAX_CMD_LINE_LENGTH];
    WORD32 fargc, curpos;
    WORD32 processcmd = 0;
    WORD8 fargv[XA_MAX_ARGS][XA_MAX_CMD_LINE_LENGTH];
    pWORD8 pargv[XA_MAX_ARGS];
    WORD8 pb_input_file_path[XA_MAX_CMD_LINE_LENGTH] = "";
    WORD8 pb_output_file_path[XA_MAX_CMD_LINE_LENGTH] = "";
    WORD32 err_code = XA_NO_ERROR;

    // NOTE: set_wbna() should be called before any other dynamic
    // adjustment of the region attributes for cache.
    set_wbna(&argc, argv);

    if (argc == 1)
    {
        param_file_id = fopen(PARAMFILE, "r");
        if (param_file_id == NULL)
        {
            fprintf(stdout, "Error opened Parameter File: %s \n", PARAMFILE);
            exit(1);
        }

        /* Process one line at a time */
        while (fgets(curr_cmd, XA_MAX_CMD_LINE_LENGTH, param_file_id))
        {
            curpos = 0;
            fargc = 0;
            /* if it is not a param_file command and if */
            /* CLP processing is not enabled */
            if (curr_cmd[0] != '@' && !processcmd)
            {   /* skip it */
                continue;
            }

            while (sscanf(curr_cmd + curpos, "%s", fargv[fargc]) != EOF)
            {
                if (fargv[0][0] == '/' && fargv[0][1] == '/')
                    break;
                if (strcmp(fargv[0], "@echo") == 0)
                    break;
                if (strcmp(fargv[fargc], "@New_line") == 0)
                {
                    fgets(curr_cmd + curpos, XA_MAX_CMD_LINE_LENGTH,
                        param_file_id);
                    continue;
                }
                curpos += strlen(fargv[fargc]);
                while (*(curr_cmd + curpos) == ' ' || *(curr_cmd + curpos) == '\t')
                    curpos++;
                fargc++;
            }

            if (fargc < 1)    /* for blank lines etc. */
                continue;

            if (strcmp(fargv[0], "@Output_path") == 0)
            {
                if (fargc > 1) strcpy(pb_output_file_path, fargv[1]);
                else strcpy(pb_output_file_path, "");
                continue;
            }

            if (strcmp(fargv[0], "@Input_path") == 0)
            {
                if (fargc > 1) strcpy(pb_input_file_path, fargv[1]);
                else strcpy(pb_input_file_path, "");
                continue;
            }

            if (strcmp(fargv[0], "@Start") == 0)
            {
                processcmd = 1;
                continue;
            }

            if (strcmp(fargv[0], "@Stop") == 0)
            {
                processcmd = 0;
                continue;
            }

            /* otherwise if this a normal command and its enabled for execution */
            if (processcmd)
            {
                WORD32 j;
                WORD32 pos;
                WORD8 inputFilename[XA_MAX_CMD_LINE_LENGTH] = "";
                WORD8 outputFilename[XA_MAX_CMD_LINE_LENGTH] = "";
                WORD8 edf[XA_MAX_CMD_LINE_LENGTH] = "";
                WORD8 epf[XA_MAX_CMD_LINE_LENGTH] = "";
                WORD8 cfgG192[XA_MAX_CMD_LINE_LENGTH] = "";

                for (pos = 0; pos < fargc && fargv[pos][0] == '-'; pos++)
                {
		   /* START FRAME */
        	    if (!strcmp(fargv[pos], "-y") && pos + 1 < fargc)
        	    {
                        pos++;
        	        printf("Start Frame: %d!\n",atoi(fargv[pos]));
        	    }
        	    
        	   /* STOP FRAME */
        	    if (!strcmp(fargv[pos], "-z") && pos + 1 < fargc)
        	    {
                        pos++;
        	        printf("Stop Frame: %d!\n", atoi(fargv[pos]));
        	    }

        	    /* delay compensation */
        	    if (!strcmp(fargv[pos], "-dc") && pos + 1 < fargc)
        	    {
                        pos++;
        	    }
                    if (!strcmp(fargv[pos], "-bps") && pos + 1 < fargc)
                    {
                        pos++;
                    }

       		    if (!strcmp(fargv[pos], "-formatG192"))
       		    {
       		        /*puts("Reading bitstream in G192 format!"); */
       		    }

       		    if (!strcmp(fargv[pos], "-cfgG192") && pos + 1 < fargc)
       		    {
                        pos++;
                        strcat(cfgG192, pb_input_file_path);
                        strcat(cfgG192, fargv[pos]);
                        strcpy(fargv[pos], cfgG192);
        	        /*puts("Using user defined configuration file for G192 bitstream format!"); */
       		    }

                    if (!strcmp(fargv[pos], "-epf") && pos + 1 < fargc)
                    {
                        pos++;
                        strcat(epf, pb_input_file_path);
                        strcat(epf, fargv[pos]);
                        strcpy(fargv[pos], epf);
                    }
                    if (!strcmp(fargv[pos], "-edf") && pos + 1 < fargc)
                    {
                        pos++;
                        strcat(edf, pb_output_file_path);
                        strcat(edf, fargv[pos]);
                        strcpy(fargv[pos], edf);
                    }
                }

                strcat(inputFilename, pb_input_file_path);
                strcat(inputFilename, fargv[pos]);
                strcpy(fargv[pos], inputFilename);
                pos++;

                strcat(outputFilename, pb_output_file_path);
                strcat(outputFilename, fargv[pos]);
                strcpy(fargv[pos], outputFilename);
                pos++;

                for (j = 0; j < fargc; j++)
                {
                    pargv[j] = fargv[j];
                }

                fprintf(stdout, "\n");
                xa_lc3_dec_main_process(fargc, pargv);
                
                CloseWav(output_wav);
                safe_fclose(input_bitstream);
                safe_fclose(error_pattern_file);
                safe_fclose(error_detection_file);
            }
        }
        err_code = XA_NO_ERROR;
    }
    else
    {
        err_code = xa_lc3_dec_main_process(argc - 1, &argv[1]);

        CloseWav(output_wav);
        safe_fclose(input_bitstream);
        safe_fclose(error_pattern_file);
        safe_fclose(error_detection_file);
    }

    return err_code;
}

static VOID parseCmdl_dec(WORD32 ac, pWORD8* av, Arguments_dec* arg)
{
    WORD32 pos = 0;
    WORD32 isTrue = 0;
    arg->bipsOut  = 16;
    arg->dc = 1; 
    arg->plcMeth = 0;

    /* parse options in any order */
    for (; pos < ac && av[pos][0] == '-'; pos++)
    {
        isTrue = 0;
        if (!strcmp(av[pos], "-h"))
        {
            puts(USAGE_MESSAGE_DEC);
            exit(0);
        }

        if (!strcmp(av[pos], "-q"))
        {
            arg->hide_counter = 1;
            isTrue = 1;
        }

        /* error pattern */
        if (!strcmp(av[pos], "-epf") && pos + 1 < ac)
        {
            arg->epf = av[++pos];
            isTrue = 1;
            puts("Using error pattern file for frame loss simulation!");
        }

        if (!strcmp(av[pos], "-y") && pos + 1 < ac)
        {
            arg->startFrame = atoi(av[++pos]);
            printf("Start Frame: %d!\n", arg->startFrame);
            isTrue = 1;
        }
        
        if (!strcmp(av[pos], "-z") && pos + 1 < ac)
        {
            arg->stopFrame = atoi(av[++pos]);
            printf("Stop Frame: %d!\n", arg->stopFrame);
            isTrue = 1;
        }

        /* delay compensation */
        if (!strcmp(av[pos], "-dc") && pos + 1 < ac)
        {
            arg->dc = atoi(av[++pos]);
            isTrue = 1;
            exit_if(arg->dc < 0 || arg->dc > 2, "dc musst be 0, 1 or 2!");
        }
        

        /* Bits per sample */
        if (!strcmp(av[pos], "-bps") && pos + 1 < ac)
        {
            isTrue=1;
            arg->bipsOut = atoi(av[++pos]);
            
        }

        if (!strcmp(av[pos], "-formatG192"))
        {
            isTrue=1;
            arg->formatG192 = 1;
            puts("Reading bitstream in G192 format!");
        }

        if (!strcmp(av[pos], "-cfgG192") && pos + 1 < ac)
        {
            isTrue=1;
            arg->configFilenameG192 = av[++pos];
            puts("Using user defined configuration file for G192 bitstream format!");
        }

        /* Error detection pattern */
        if (!strcmp(av[pos], "-edf") && pos + 1 < ac)
        {
            isTrue=0;
            arg->edf = av[++pos];
            puts("Writing error detection file!");
        }
        
        if((isTrue==0)&&(strlen(av[pos])>0))
        {
            puts(INVALID_ARGUMENT_MESSAGE);
            exit(0);
        }
    }

    exit_if(pos + 1 >= ac, MISSING_ARGUMENT_MESSAGE);

    arg->inputFilename = av[pos++];
    arg->outputFilename = av[pos++];

    putchar('\n');
}

/* check condition and if it fails, exit with error message */
static VOID exit_if(WORD32 condition, const pWORD8 message)
{
    if (condition)
    {
        puts(message);
        exit(1);
    }
}

/* close file ignoring NULL pointer */
static VOID safe_fclose(FILE* f)
{
    if (f != NULL)
        fclose(f);
}

/* read value from file and rewind if end is reached */
WORD16 loopy_read16(FILE* f)
{
    WORD16 tmp = 0;
    if (fread(&tmp, sizeof(tmp), 1, f) != 1)
    {
        fseek(f, 0, SEEK_SET);
        fread(&tmp, sizeof(tmp), 1, f);
    }
    return tmp;
}
#if 0
FILE* open_bitstream_reader(const pWORD8 file, pWORD32 samplerate, pWORD32 bitrate, pWORD32 channels)
{
    FILE* f_use = fopen(file, "rb");

    if (f_use)
    {
        WORD16 tmp = 0;
        fread(&tmp, sizeof(tmp), 1, f_use);
        *samplerate = tmp * 100;
        fread(&tmp, sizeof(tmp), 1, f_use);
        *bitrate = tmp * 100;
        fread(&tmp, sizeof(tmp), 1, f_use);
        *channels = tmp;
    }

    return f_use;
}
#else
/* open file with extra extension */
static FILE *fopen_with_ext(const char *file, const char *ext, const char *mode)
{
    FILE *f = NULL;
    char *tmp = (char *)malloc(strlen(file) + strlen(ext) + 1);
    sprintf(tmp, "%s%s", file, ext);
    f = fopen(tmp, mode);
    free(tmp);
    return f;
}


/* open file with .cfg suffix if file_cfg is null */
static FILE *fopen_cfg(const char *file, const char *file_cfg, const char *mode)
{
    return file_cfg ? fopen(file_cfg, mode) : fopen_with_ext(file, ".cfg", mode);
}

static FILE *open_bitstream_reader(const char *file, UWORD32 *samplerate, WORD32 *bitrate, WORD16 *channels,
                                   UWORD32 *signal_len, float *frame_ms, WORD32 *epmode,WORD32 *hrmode,  int g192,
                                   const char *file_cfg)
{
    FILE *f     = fopen(file, "rb");
    FILE *f_use = f;
    FILE *f_cfg = NULL;

    if (g192)
    {
        f_cfg = fopen_cfg(file, file_cfg, "rb");
        exit_if(f_cfg == NULL, "Error opening G192 configuration-file!");
        f_use = f_cfg;
    }

    if (f_use)
    {
        UWORD16 header[10] = {0};
        fread(header, sizeof(header), 1, f_use);
        if (header[0] != 0xcc1c)
        {
            /* Old style bitstream header */
            *samplerate = header[0] * 100;
            *bitrate    = header[1] * 100;
            *channels   = header[2];
            *frame_ms   = (float)(1000/ 100.0);
            fseek(f_use, 6, SEEK_SET);
        }
        else
        {
            assert(header[1] >= 18);
            *samplerate = header[2] * 100;
            *bitrate    = header[3] * 100;
            *channels   = header[4];
            *frame_ms   = (float)(header[5] / 100.0);
            *epmode     = header[6];
            *signal_len = (UWORD32)header[7] | ((UWORD32)header[8] << 16);
            *hrmode     = header[1] > 18 ? header[9] : 0;
            fseek(f_use, header[1], SEEK_SET);
        }
    }

    return f;
}
#endif

static int read_bitstream_frame_G192(FILE *bitstream_file, int size, uint8_t *bytes, int *bfi_ext)
{
    int i = 0, j = 0, read = 0;
    uint16_t nbits = 0;
    int16_t currentBit = 0, frameIndicator = 0, nbytes = 0;

    /* Read frame indicator info -> good/bad/redundancy frame */
    read = (int)fread(&frameIndicator, sizeof(frameIndicator), 1, bitstream_file);
    if (read != 1)
    {
        return -1;
    }

    /* Read length info */
    read = (int)fread(&nbits, sizeof(nbits), 1, bitstream_file);

    nbytes = nbits / 8;

    exit_if(frameIndicator != G192_GOOD_FRAME && frameIndicator != G192_BAD_FRAME &&
                frameIndicator != G192_REDUNDANCY_FRAME,
            "Wrong G192 format detected in bitstream file! The sync word could not be recognized!");

    for (i = 0; i < nbytes && i < size; i++)
    {
        int byte = 0;
        for (j = 0; j < 8; j++)
        {
            read = (int)fread(&currentBit, sizeof(currentBit), 1, bitstream_file);
            if (currentBit == G192_ONE)
            {
                byte |= 1UL << j;
            }
        }
        bytes[i] = (uint8_t)byte;
    }
    if (frameIndicator == G192_GOOD_FRAME)
    {
        *bfi_ext = 0;
    }
    else if (frameIndicator == G192_BAD_FRAME)
    {
        nbytes = 0;
        *bfi_ext = 1;
    }
    else if (frameIndicator == G192_REDUNDANCY_FRAME)
    {
        *bfi_ext = 3;
    }

    return nbytes;
}

WORD32 read_bitstream_frame(FILE* bitstream_file, pUWORD8 bytes, WORD32 size, int g192, int *bfi_ext)
{
  if (g192)
  {
      return read_bitstream_frame_G192(bitstream_file, size, bytes, bfi_ext);
  }
  else
  {
    WORD32      i = 0;
    UWORD16 nbytes = 0;
    if (fread(&nbytes, sizeof(nbytes), 1, bitstream_file) != 1)
    {
        return -1; /* End of file reached */
    }
    for (i = 0; i < nbytes && i < size; i++)
    {
        bytes[i] = getc(bitstream_file);
    }
    if (nbytes != i)
    {
        return -1;
    }
    return nbytes;
  }
}
VOID interleave(pWORD32 in, pWORD32 out, WORD32 n, WORD32 channels, WORD32 bipsout)
{
    WORD32 ch, i;

    if (bipsout == 16)
    {
        pWORD16 p_in16 = (pWORD16)in;

        for (ch = 0; ch < channels; ch++)
        {
            for (i = 0; i < n; i++)
            {
                out[i * channels + ch] = *p_in16++;
            }
        }
    }
    else //bipsout = 24
    {
        pWORD32 p_in24 = in;

        for (ch = 0; ch < channels; ch++)
        {
            for (i = 0; i < n; i++)
            {
                out[i * channels + ch] = *p_in24++;
            }
        }
    }
}

#if TEST_FSANITIZE
const char *__asan_default_options(){
return "halt_on_error=0";
}
#endif
