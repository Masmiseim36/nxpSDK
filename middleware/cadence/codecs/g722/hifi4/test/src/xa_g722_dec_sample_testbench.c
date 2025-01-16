/*
 * Copyright (c) 2012-2020 Cadence Design Systems, Inc.
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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#if defined (__XTENSA__)
#include <xtensa/tie/xt_hifi2.h>
#endif
#include <g722_codec/xa_g722_codec_api.h>
#include <xa_type_def.h>
#include <xa_error_standards.h>
#include <xa_error_handler.h>

#ifdef __XCC__
#include <xtensa/hal.h>
#endif

#ifdef __PROFILE__
#include <sys/times.h>
#include <xtensa/sim.h>
#endif

#define PARAMFILE "paramfilesimple_decode.txt"
#define XA_MAX_CMD_LINE_LENGTH 300
#define XA_MAX_ARGS 9

#define XA_TESTBENCH_FATAL_MEM_ALLOC_FAILED  XA_ERROR_CODE(xa_severity_fatal, 0, 0, 0)
#define XA_TESTBENCH_FATAL_FILE_OPEN_FAILED  XA_ERROR_CODE(xa_severity_fatal, 0, 0, 1)
#define XA_TESTBENCH_FATAL_CONFIGURATION     XA_ERROR_CODE(xa_severity_fatal, 1, 0, 0)


/* Size of the output of the decoder 
 * Note that the decoder supports any block size 
   currently 20ms frames  */
#define XA_G722_MAX_SAMPLES_PER_FRAME_DECODER 320
#define XA_G722_MAX_NUM_BITS_PER_FRAME_DECODER  (XA_G722_MAX_SAMPLES_PER_FRAME_DECODER * XA_G722_MAX_NUM_BITS_PER_ENCODED_SAMPLE) 
#define XA_G722_MAX_NUM_BYTES_PER_FRAME_DECODER  (XA_G722_MAX_NUM_BITS_PER_FRAME_DECODER >> 3)

/* Maximum supported loss pattern length in this testbench */
#define MAX_LOSS_PATTERN_LENGTH 2000

/* The codeword for erasures in G.192 */
#define G192_ERASURE_CODEWORD 0x6B20

/* Normal frames G.192 */
#define G192_NORMAL_FRAME 0x6B21

/* Local prototypes */
VOID xa_g722_dec_show_usage(VOID);

/* These are codec library error*/
VOID xa_g722_codec_error_handler_init();
extern  xa_error_info_struct xa_g722_codec_error_info;

/* These are testbench error*/
VOID xa_testbench_error_handler_init();
extern  xa_error_info_struct xa_testbench_error_info;

XA_ERRORCODE xa_g722_dec_main_process(char *in_out_tests[], int argc);

XA_ERRORCODE xa_g722_dec_paramfile_parse(char* paramfile_name)
{
    FILE *param_file_id;
    char curr_cmd[XA_MAX_CMD_LINE_LENGTH];
    WORD32 fargc, curpos;
    WORD32 processcmd = 0;
    char pb_output_file_path[XA_MAX_CMD_LINE_LENGTH] = "";
    char fargv[XA_MAX_ARGS][XA_MAX_CMD_LINE_LENGTH];
    char *pargv[XA_MAX_ARGS];
    XA_ERRORCODE error_code = XA_NO_ERROR;

    char pb_input_file_path[XA_MAX_CMD_LINE_LENGTH] = "";
    char pb_second_input_file_path[XA_MAX_CMD_LINE_LENGTH] = "";

    param_file_id = fopen(paramfile_name, "r");

    if (param_file_id == NULL)
    {
        fprintf(stderr, "Error opening Parameter File !!!\n");
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
        {
            /* skip it */
            continue;
        }
        while(sscanf(curr_cmd + curpos, "%s", fargv[fargc]) != EOF)
        {

            if(fargv[0][0]=='/' && fargv[0][1]=='/')
            {
                break;
            }

            if(strcmp(fargv[0], "@echo") == 0)
            {
                break;
            }

            if(strcmp(fargv[fargc], "@New_line") == 0)
            {
                fgets(curr_cmd + curpos, XA_MAX_CMD_LINE_LENGTH,
                    param_file_id);
                continue;
            }

            curpos += strlen(fargv[fargc]);

            while(*(curr_cmd + curpos)==' ' || *(curr_cmd + curpos)=='\t')
            {
                curpos++;
            }
            fargc++;
        }
        if(fargc < 1)    /* for blank lines etc. */
        {
            continue;
        }
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
            strcpy(pb_second_input_file_path, pb_input_file_path);
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
            int ix;
            for( ix = 1; ix < fargc+1;)
            {
                pargv[ix] = fargv[ix-1];
                if(ix == 1)
                {
                    strcat(pb_input_file_path, fargv[ix-1]);
                    pargv[ix] = pb_input_file_path;
                }

                else if(ix == 2)
                {
                    strcat(pb_output_file_path, fargv[ix-1]);
                    pargv[ix] = pb_output_file_path;
                }

                if(ix == 4 )
                {
                    if (fargv[ix-1][0] != '-')
                    {
                        strcat(pb_second_input_file_path, fargv[ix-1]);
                        pargv[ix] = pb_second_input_file_path;
                    }
                }

                ix++;
            }
            error_code = xa_g722_dec_main_process(pargv, fargc+1);
        }

    }
    return error_code;
}

/* main function
 *
 * The program starts execution.
 * It has access to the command arguments given to the program when it was
 * executed.
 */
XA_ERRORCODE xa_g722_dec_main_process(char *in_out_tests[], int argc)
{
    WORD32 handle_size, scratch_size, inp_speech_size, dec_speech_size;

    xa_codec_handle_t speech_decoder_state = NULL;
    pVOID scratch_decoder;
    pWORD16 dec_speech;
    pWORD8 inp_speech;

    WORD32 i;
    WORD32  frame;

    FILE *dec_instream_fid;
    FILE *dec_out_stream_fid;
    FILE *loss_pattern_fid;

    /* Error code */
    XA_ERRORCODE error_code = XA_NO_ERROR;

    WORD32 loss_pattern_length = 0;
    WORD32 loss_pattern_idx = 0;
    BOOL loss_pattern_available = 0;
    WORD32 plc_option;

    WORD32 decoded_samples_per_frame;
    WORD32 bytes_to_process;


    WORD32 frame_size = XA_G722_MAX_NUM_BYTES_PER_FRAME_DECODER;
    WORD32 loss_pattern_size = MAX_LOSS_PATTERN_LENGTH * sizeof(WORD16);
    pWORD16 loss_pattern = NULL;

    /**************************************************************************
     * Initialize the error handler testbench                                 *
     **************************************************************************/
    xa_error_info_struct *p_proc_testbench_err_info = NULL;
    p_proc_testbench_err_info = (xa_error_info_struct*)&xa_testbench_error_info;
    xa_testbench_error_handler_init();

    /**************************************************************************
     * Initialize the error handler library                                   *
     **************************************************************************/
    xa_error_info_struct *p_proc_codec_err_info = NULL;
    p_proc_codec_err_info = (xa_error_info_struct*)&xa_g722_codec_error_info;
    xa_g722_codec_error_handler_init();

#ifdef __PROFILE__
    struct tms start, stop;
    clock_t cycles;
    double mcps_peak = 0, mcps_curr = 0, mcps_ave = 0, mcps_sum = 0;
    int peak_frame = 0;
#endif

    /* Codec Information provided by developer */
    fprintf(stderr, "\n");
    fprintf(stderr, "%s library version %s\n",
        xa_g722_get_lib_name_string(),
        xa_g722_get_lib_version_string());
    fprintf(stderr, "API version: %s\n", xa_g722_get_lib_api_version_string());
    fprintf(stderr, "Tensilica, Inc. http://www.tensilica.com\n");
    fprintf(stderr, "\n");

    /**************************************************************************
     * Verify the parameters                                                  *
     **************************************************************************/

    /**************************************************************************
     *  Opening Input bitstream File                                          *
     **************************************************************************/

    dec_instream_fid = fopen(in_out_tests[1],"rb");
    if (dec_instream_fid == NULL)
    {
        _XA_HANDLE_ERROR(
                p_proc_testbench_err_info,
                "Input File",
                XA_TESTBENCH_FATAL_FILE_OPEN_FAILED);
    }

    fprintf(stderr, "Input speech file:  %s\n", in_out_tests[1]);

    /**************************************************************************
     * Opening Output Speech File                                             *
     **************************************************************************/

    dec_out_stream_fid = fopen(in_out_tests[2],"wb");
    if (dec_out_stream_fid == NULL)
    {
        _XA_HANDLE_ERROR(
                p_proc_testbench_err_info,
                "Output File",
                XA_TESTBENCH_FATAL_FILE_OPEN_FAILED);
    }

    fprintf(stderr, "Output bitstream file:  %s\n", in_out_tests[2]);

    /*************************************************
     Check the PLC support
    **************************************************/
    plc_option = 0;
    if(argc > 4) {
        if (strncmp(in_out_tests[3],"-plc",4) == 0)
        {
            plc_option = 1;
            loss_pattern_available = 1;
        }
        else
        {
            xa_g722_dec_show_usage();   
            return 1;
        }
    }

    /**************************************************************************
     * Opening Loss Pattern File                                             *
     **************************************************************************/
    if (loss_pattern_available)
    {
        loss_pattern_fid = fopen(in_out_tests[4],"rb");
        if (loss_pattern_fid == NULL)
        {
            _XA_HANDLE_ERROR(
                    p_proc_testbench_err_info,
                    "Loss Pattern File",
                    XA_TESTBENCH_FATAL_FILE_OPEN_FAILED);
        }
        fprintf(stderr, "Loss pattern file:  %s\n", in_out_tests[4]);
    }

    /* Start out assuming that the frame is 10ms. Overwride if -20ms is passed-in */
    frame_size = XA_G722_MAX_NUM_BYTES_PER_FRAME_DECODER/2;

    /* argc: 6 input file, output file and -plc pattern file and 20ms frame */
    if (argc == 6)
    {
        if (strcmp(in_out_tests[5], "-20ms") == 0)
        {
            frame_size = XA_G722_MAX_NUM_BYTES_PER_FRAME_DECODER;
        }
    }

    /**************************************************************************
     *  Get Memory sizes from library                                        *
    ***************************************************************************/
    handle_size     = xa_g722_dec_get_handle_byte_size();
    scratch_size    = xa_g722_dec_get_scratch_byte_size();
    inp_speech_size = XA_G722_MAX_NUM_BYTES_PER_FRAME_DECODER;
    dec_speech_size = XA_G722_MAX_SAMPLES_PER_FRAME_DECODER * (XA_G722_NUM_BITS_PER_PCM_SAMPLE / 8);
    fprintf(stderr, "\nPersistent state size: %d bytes\n", handle_size);
    fprintf(stderr, "Scratch buffer size:   %d bytes\n", scratch_size);
    fprintf(stderr, "Input buffer size:     %d bytes\n", inp_speech_size);
    fprintf(stderr, "Output buffer size:    %d bytes\n\n", dec_speech_size);


    /**************************************************************************
     *                     Allocate Memory                                    *
     **************************************************************************/

    speech_decoder_state = (xa_codec_handle_t)malloc(handle_size);

    if(speech_decoder_state == NULL)
    {
        _XA_HANDLE_ERROR(
                p_proc_testbench_err_info,
                "API state alloc",
                XA_TESTBENCH_FATAL_MEM_ALLOC_FAILED);
    }
    memset(speech_decoder_state,0,handle_size);

    scratch_decoder = (pVOID)malloc(scratch_size);
    if(scratch_decoder == NULL)
    {
        _XA_HANDLE_ERROR(
            p_proc_testbench_err_info,
            "API scratch alloc",
            XA_TESTBENCH_FATAL_MEM_ALLOC_FAILED);
    }
    memset(scratch_decoder,0,scratch_size);

    inp_speech = (pWORD8)malloc(inp_speech_size);
    if(inp_speech == NULL)
    {
        _XA_HANDLE_ERROR(
            p_proc_testbench_err_info,
            "API input alloc",
            XA_TESTBENCH_FATAL_MEM_ALLOC_FAILED);
    }

    dec_speech = (pWORD16)malloc(dec_speech_size);

    if(dec_speech == NULL)
    {
        _XA_HANDLE_ERROR(
                p_proc_testbench_err_info,
                "API output alloc",
                XA_TESTBENCH_FATAL_MEM_ALLOC_FAILED);
    }

    /* Allocate an application buffer to hold the loss pattern.
     * This is necessary even if no loss pattern file is provided
     */

    loss_pattern  = (pWORD16) malloc(loss_pattern_size * sizeof(WORD16));

    if(loss_pattern == NULL){
        _XA_HANDLE_ERROR(
                p_proc_testbench_err_info,
                "API loss pattern buffer alloc",
                XA_TESTBENCH_FATAL_MEM_ALLOC_FAILED);
    }

    if (loss_pattern_available)
    {
        /* Read the entire loss pattern file */
        loss_pattern_length = 0;

        while (fread(
                &loss_pattern[loss_pattern_length],
                sizeof(WORD16),
                1,
                loss_pattern_fid) == 1 )
        {
#if __XTENSA_EB__
            loss_pattern[loss_pattern_length] =
                AE_SHA32(loss_pattern[loss_pattern_length]);
#endif
            loss_pattern_length++;

            if ( loss_pattern_length >= MAX_LOSS_PATTERN_LENGTH ) {
                _XA_HANDLE_ERROR(
                        p_proc_testbench_err_info,
                        "API loss pattern too long",
                        XA_TESTBENCH_FATAL_CONFIGURATION);
            }
        }
        fprintf(stderr, "Loss pattern length: %d\n", loss_pattern_length);
        fclose(loss_pattern_fid);
    }
    else
    {
        /* Assume no loss of packets */
        loss_pattern_length = 1;
        loss_pattern[0] = G192_NORMAL_FRAME;
    }

    /* This is an application buffer where the conformance frame are
     * stored.
     */
    WORD32 app_inp_speech_size = (XA_G722_MAX_NUM_BYTES_PER_FRAME_DECODER) * sizeof(WORD16);
    pWORD16 app_inp_speech = NULL;
    app_inp_speech = (pWORD16)malloc(app_inp_speech_size);

    if(app_inp_speech == NULL)
    {
        _XA_HANDLE_ERROR(
                p_proc_testbench_err_info,
                "API conformance buffer alloc",
                XA_TESTBENCH_FATAL_MEM_ALLOC_FAILED);
    }

    if(plc_option == 0)
    {
        fprintf(stderr, "PLC Disabled\n\n");
    }

    /**************************************************************************
     *                              Initialization.                           *
     **************************************************************************/

    error_code = xa_g722_dec_init(
            speech_decoder_state,
            scratch_decoder,
            plc_option );

    _XA_HANDLE_ERROR(p_proc_codec_err_info, "", error_code);
    memset(dec_speech,0,dec_speech_size);
    memset(app_inp_speech,0,app_inp_speech_size);

    frame = 0;
    loss_pattern_idx = 0;

    /**************************************************************************
     *                              Start decoding frame by frame             *
     **************************************************************************/

    while((bytes_to_process = fread(app_inp_speech,sizeof(WORD16),frame_size,
                                    dec_instream_fid)) != 0)

    {

        decoded_samples_per_frame = (bytes_to_process<<1);
            
        frame++;

        /* Memset the scratch buffer.  This is to test that the scratch buffer values have no effect on behavior. */
        memset(scratch_decoder,0x0b,scratch_size);

        /* The compliance input test vectors have the codewords padded to 16-bits.
         * Compress them to 8-bit packed bitstream before decoding
         */
        for (i = 0; i < bytes_to_process; i++)
        {
#if __XTENSA_EB__
            app_inp_speech[i] = AE_SHA32(app_inp_speech[i]);
#endif

            inp_speech[i] = ((WORD16) app_inp_speech[i]) & 0x00FF;
        }

#ifdef __PROFILE__
        xt_iss_client_command("all","enable");
        times(&start);
#endif

        /* Process one frame of speech, according to the loss pattern */
        if ( loss_pattern[loss_pattern_idx] == G192_ERASURE_CODEWORD )
        {
            /* This is an erasure */
            error_code = xa_g722_dec (
                    speech_decoder_state,
                    inp_speech,
                    dec_speech,
                    bytes_to_process,
                    XA_G722_BAD_FRAME);

            _XA_HANDLE_ERROR(p_proc_codec_err_info, "", error_code);
        }
        else if ( loss_pattern[loss_pattern_idx] == G192_NORMAL_FRAME )
        {
            /* Encode one frame of speech. */
            error_code = xa_g722_dec (
                    speech_decoder_state,
                    inp_speech,
                    dec_speech,
                    bytes_to_process,
                    XA_G722_GOOD_FRAME);

            _XA_HANDLE_ERROR(p_proc_codec_err_info, "", error_code);
        }
        else
        {
            _XA_HANDLE_ERROR(
                    p_proc_testbench_err_info,
                    "Error code type is not allowed",
                    XA_TESTBENCH_FATAL_CONFIGURATION);
        }

        loss_pattern_idx++;

        if ( loss_pattern_idx == loss_pattern_length )
        {
            loss_pattern_idx=0;
        }

#if __PROFILE__
        times(&stop);
        xt_iss_client_command("all","disable");
#endif

#ifdef __PROFILE__

        cycles = stop.tms_utime - start.tms_utime;
        mcps_curr = (cycles * 16000.0 /* Hz */ / (decoded_samples_per_frame /* samples/frame */ * 1000000.0));
        mcps_sum += mcps_curr;
        mcps_ave = mcps_sum / frame;
        if (mcps_peak < mcps_curr)
        {
            mcps_peak = mcps_curr;
            peak_frame = frame;
        }

        fprintf(
                stderr,
                "[%d] MCPS: %f Average: %f Peak: %f @ [%d]\n",
                frame,
                mcps_curr,
                mcps_ave,
                mcps_peak,
                peak_frame);
#endif

#if __XTENSA_EB__
        /* Assume the input samples are in little-endian (Intel) byte order. */
        for (i = 0; i < decoded_samples_per_frame; i++)
        {
            dec_speech[i] = AE_SHA32(dec_speech[i]);
        }
#endif /* __XTENSA_EB__ */

        /* write speech sample to output */
        fwrite(
                dec_speech,
                sizeof(WORD16),
                decoded_samples_per_frame,
                dec_out_stream_fid);

        fflush(dec_out_stream_fid);


    } /* do exec */

#ifdef __PROFILE__
    fprintf(stderr, "\nPeak MCPS = %f\n", mcps_peak);
    fprintf(stderr, "Peak frame = %d\n", peak_frame);
    fprintf(stderr, "Average MCPS = %f\n\n", mcps_ave);
#endif

    /* Closing the files */
    fclose(dec_instream_fid);
    fclose(dec_out_stream_fid);

    /* MEM used has to be freed before finishing execution */
    free(speech_decoder_state);
    free(scratch_decoder);
    free(inp_speech);
    free(dec_speech);

    free(app_inp_speech);
    free(loss_pattern);

    return error_code;
}

/*
 * xa_g722_dec_show_usage
 *
 * This function shows the way the application can be used, and the options
 * allowed. the rates allowed are 40 kbits , 32 kbits 24 kbits and finally
 * 16 kbits.
 */
VOID xa_g722_dec_show_usage()
{
    fprintf(stderr, "Usage: xa_g722_dec_test <bitstream_file> <synth_file> -plc [<loss_pattern_file>] "
            "[-20ms] \n");
    fprintf(stderr, "\n");
    fprintf(stderr, "Format for <bitstream_file>:\n");
    fprintf(stderr, "Format for <synth_file>:\n");
    fprintf(stderr, "  Synthesis is written to a binary file of 16 bits PCM data.\n");
    fprintf(stderr, "-plc:\n");
    fprintf(stderr, "  Not defined normal G722 decode\n");
    fprintf(stderr, "  If defined PLC support is enabled. You must specify G.192 compatable loss_pattern_file \n");
    fprintf(stderr, "Format for <loss_pattern_file>:\n");
    fprintf(stderr, "  G.192-compatible pattern loss file\n");
    fprintf(stderr, "[-20ms]:");
    fprintf(stderr, "  use 20ms frames format. If this option is not specified, default 10ms frames\n");
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

int main(int argc, char *argv[])
{
    // NOTE: set_wbna() should be called before any other dynamic
    // adjustment of the region attributes for cache.
    set_wbna(&argc, argv);

    if(argc == 1){
        xa_g722_dec_paramfile_parse(PARAMFILE);
    }
    else
    {
        /* Valid number of arguments are 3, 5 or 6 (plus one for the executable name) */
        if ( (argc < 3) || (argc > 6))
        {
            xa_g722_dec_show_usage();
        }
        else
        {
            xa_g722_dec_main_process(argv, argc);
        }
    }

    return 0;
}
