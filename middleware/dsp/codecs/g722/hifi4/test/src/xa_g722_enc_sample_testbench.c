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

#define PARAMFILE "paramfilesimple_encode.txt"
#define XA_MAX_CMD_LINE_LENGTH 300
#define XA_MAX_ARGS 5

#define XA_TESTBENCH_FATAL_MEM_ALLOC_FAILED  XA_ERROR_CODE(xa_severity_fatal, 0, 0, 0)
#define XA_TESTBENCH_FATAL_FILE_OPEN_FAILED  XA_ERROR_CODE(xa_severity_fatal, 0, 0, 1)
#define XA_TESTBENCH_FATAL_CONFIGURATION     XA_ERROR_CODE(xa_severity_fatal, 1, 0, 0)

/* Input and output buffer sizes */
/* Size of the output of the encoder (max num samples per frame) x (max num bits per sample ) 
 * Note that the encoder testbench supports any block size currently 10ms frames  */
#define XA_G722_MAX_SAMPLES_PER_FRAME_ENCODER 160
#define XA_G722_MAX_NUM_BITS_PER_FRAME_ENCODER   (XA_G722_MAX_SAMPLES_PER_FRAME_ENCODER * XA_G722_MAX_NUM_BITS_PER_ENCODED_SAMPLE)
#define XA_G722_MAX_NUM_BYTES_PER_FRAME_ENCODER  (XA_G722_MAX_NUM_BITS_PER_FRAME_ENCODER >> 3)

/*Local Prototypes */
VOID xa_g722_enc_show_usage(VOID);

/* These are codec library error*/
VOID xa_g722_codec_error_handler_init();
extern  xa_error_info_struct xa_g722_codec_error_info;

/* These are testbench error*/
VOID xa_testbench_error_handler_init();
extern  xa_error_info_struct xa_testbench_error_info;

XA_ERRORCODE xa_error_handler(
    xa_error_info_struct *p_mod_err_info,
    const char *pb_context,
    XA_ERRORCODE code);

XA_ERRORCODE xa_g722_enc_main_process(char *in_out_tests[], int argc);

XA_ERRORCODE xa_g722_paramfile_parse(char* paramfile_name)
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
                ix++;
            }
            error_code = xa_g722_enc_main_process(pargv,fargc+1);
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
XA_ERRORCODE xa_g722_enc_main_process(char *in_out_tests[], int argc)
{
    WORD32 inp_speech_size, enc_speech_size, handle_size, scratch_size;

    pVOID scratch_encoder;
    xa_codec_handle_t speech_encoder_state = NULL;
    pWORD16 inp_speech;
    pWORD8 enc_speech;

    WORD32  frame;
    int i;

    int samples_to_process = 0;
    int encoded_bytes_per_frame;

    FILE *enc_instream_fid;
    FILE *enc_outstream_fid;

    /* Error code */
    XA_ERRORCODE error_code = XA_NO_ERROR;

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
     *  Opening Input Speech File                                             *
     **************************************************************************/

    enc_instream_fid = fopen(in_out_tests[1],"rb");
    if (enc_instream_fid == NULL)
    {
        _XA_HANDLE_ERROR(
                p_proc_testbench_err_info,
                "Input File",
                XA_TESTBENCH_FATAL_FILE_OPEN_FAILED);
    }

    fprintf(stderr, "Input speech file:  %s\n", in_out_tests[1]);

    /**************************************************************************
     *  Opening Output Bitstream  File                                        *
     **************************************************************************/

    enc_outstream_fid = fopen(in_out_tests[2],"wb");
    if (enc_outstream_fid == NULL)
    {
        _XA_HANDLE_ERROR(
                p_proc_testbench_err_info,
                "Output File",
                XA_TESTBENCH_FATAL_FILE_OPEN_FAILED);
    }

    fprintf(stderr, "Output bitstream file:  %s\n", in_out_tests[2]);


    /**************************************************************************
     *  Get  Memory sizes from library                                        *
     **************************************************************************/

    handle_size = xa_g722_enc_get_handle_byte_size();
    scratch_size = xa_g722_enc_get_scratch_byte_size();
    inp_speech_size = XA_G722_MAX_SAMPLES_PER_FRAME_ENCODER * (XA_G722_NUM_BITS_PER_PCM_SAMPLE/8);
    enc_speech_size = XA_G722_MAX_NUM_BYTES_PER_FRAME_ENCODER;

    fprintf(stderr, "\nPersistent state size: %d bytes\n", handle_size);
    fprintf(stderr, "Scratch buffer size:   %d bytes\n", scratch_size);
    fprintf(stderr, "Input buffer size:     %d bytes\n", inp_speech_size);
    fprintf(stderr, "Output buffer size:    %d bytes\n\n", enc_speech_size);

    /**************************************************************************
     *                     Allocate Memory                                    *
     **************************************************************************/

    speech_encoder_state = (xa_codec_handle_t)malloc(handle_size);

    if(speech_encoder_state == NULL)
    {
        _XA_HANDLE_ERROR(
                p_proc_testbench_err_info,
                "API state alloc",
                XA_TESTBENCH_FATAL_MEM_ALLOC_FAILED);
    }
    memset(speech_encoder_state,0,handle_size);

    scratch_encoder = (pVOID)malloc(scratch_size);
    if(scratch_encoder == NULL)
    {
        _XA_HANDLE_ERROR(
            p_proc_testbench_err_info,
            "API scratch alloc",
            XA_TESTBENCH_FATAL_MEM_ALLOC_FAILED);
    }

    inp_speech = (pWORD16)malloc(inp_speech_size);

    if(inp_speech == NULL)
    {
        _XA_HANDLE_ERROR(
                p_proc_testbench_err_info,
                "API input alloc",
                XA_TESTBENCH_FATAL_MEM_ALLOC_FAILED);
    }

    enc_speech = (pWORD8)malloc(enc_speech_size);

    if(enc_speech == NULL)
    {
        _XA_HANDLE_ERROR(
                p_proc_testbench_err_info,
                "API output alloc",
                XA_TESTBENCH_FATAL_MEM_ALLOC_FAILED);
    }

    /* In order to have compatibility with Big endian this conformance frame
     * has been added to interface the input buffer when it s necessary
     */
    WORD32 app_inp_speech_size = (XA_G722_MAX_SAMPLES_PER_FRAME_ENCODER) * sizeof(WORD16);
    pWORD16 app_inp_speech = NULL;
    app_inp_speech = (pWORD16)malloc(app_inp_speech_size);

    if(app_inp_speech == NULL)
    {
        _XA_HANDLE_ERROR(
                p_proc_testbench_err_info,
                "API conformance buffer alloc",
                XA_TESTBENCH_FATAL_MEM_ALLOC_FAILED);
    }

    /* This is an application buffer where the conformance frame are
     * stored.
     */
    WORD32 app_enc_speech_size = (XA_G722_MAX_NUM_BYTES_PER_FRAME_ENCODER) * sizeof(WORD16);
    pWORD16 app_enc_speech = NULL;
    app_enc_speech  = (pWORD16) malloc(app_enc_speech_size);
    if(app_enc_speech == NULL){
            _XA_HANDLE_ERROR(
                    p_proc_testbench_err_info,
                    "API conformance buffer alloc",
                    XA_TESTBENCH_FATAL_MEM_ALLOC_FAILED);
    }

   /***************************************************************************
    *                              Initialization.                            *
    **************************************************************************/

    error_code = xa_g722_enc_init(speech_encoder_state,
                                  scratch_encoder);
    _XA_HANDLE_ERROR(p_proc_codec_err_info, "", error_code);

    frame = 0;
    memset(enc_speech,0,enc_speech_size);
    memset(app_enc_speech,0,app_enc_speech_size);


    /**************************************************************************
     *                          Start encoding frame by frame                 *
     **************************************************************************/

    while( (samples_to_process = fread(inp_speech,sizeof(WORD16),
                                        XA_G722_MAX_SAMPLES_PER_FRAME_ENCODER,
                                        enc_instream_fid)) != 0)
    {

        frame++;

        /* samples * bits per sample (4)(64Kbps@16KHz) / 8) */
        encoded_bytes_per_frame = samples_to_process/2;

        for (i = 0; i < samples_to_process; i++)
        {
            app_inp_speech[i] = inp_speech[i];
#if __XTENSA_EB__
            app_inp_speech[i] = AE_SHA32(app_inp_speech[i]);
#endif
        }

#ifdef __PROFILE__
        xt_iss_client_command("all","enable");
        times(&start);
#endif

        /* Encode one frame of speech. */
        error_code = xa_g722_enc (
                speech_encoder_state,
                app_inp_speech,
                enc_speech,
                samples_to_process);

#if __PROFILE__
    times(&stop);
    xt_iss_client_command("all","disable");
#endif
        _XA_HANDLE_ERROR(p_proc_codec_err_info, "", error_code);

#ifdef __PROFILE__

        cycles = stop.tms_utime - start.tms_utime;
        mcps_curr = (cycles * 16000.0 /* Hz */ / (samples_to_process /* samples/frame */ * 1000000.0));
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

        /* Expand each 8-bit codeword into 16 bits for compliance testing
         * Also, mask-out any sign extension.  The compliance test vectors
         * simply have zeros in the upper 8 bits
         */
        for (i = 0; i < encoded_bytes_per_frame; i++)
        {
            app_enc_speech[i] = ((WORD16) enc_speech[i]) & 0x00FF;

#if __XTENSA_EB__
            app_enc_speech[i] = AE_SHA32(app_enc_speech[i]);
#endif /* __XTENSA_EB__ */
        }

        /* write conformance frame to file */
        fwrite(
            app_enc_speech,
            sizeof(WORD16),
            encoded_bytes_per_frame,
            enc_outstream_fid);

        fflush(enc_outstream_fid);

    } /* do exec */

    fprintf(stderr, "\n%d frame(s) processed\n\n", frame);

#ifdef __PROFILE__
    fprintf(stderr, "Peak MCPS = %f\n", mcps_peak);
    fprintf(stderr, "Peak frame = %d\n", peak_frame);
    fprintf(stderr, "Average MCPS = %f\n\n", mcps_ave);
#endif

    /* Closing the files */
    fclose(enc_instream_fid);
    fclose(enc_outstream_fid);

    /* MEM used has to be freed before finishing execution */
    free(speech_encoder_state);
    free(inp_speech);
    free(enc_speech);

    free(app_enc_speech);
    free(app_inp_speech);

    return XA_NO_ERROR;
}

VOID xa_g722_enc_show_usage()
{
    fprintf(stderr, "Usage: xa_g722_enc_test <speech_file> <bitstream_file>\n");
    fprintf(stderr, "Format for <speech_file>:\n");
    fprintf(stderr, "  Speech is read from a binary file of 16-bit Uniform PCM data.\n");
    fprintf(stderr, "Format for <bitstream_file>:\n");
    fprintf(stderr, "  80 words (2-byte) containing 80 codewords.\n");
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

    if(argc == 1)
    {
        xa_g722_paramfile_parse(PARAMFILE);
    }
    else
    {
        if (argc != 3)
        {
            xa_g722_enc_show_usage();
        }
        else
        {
            xa_g722_enc_main_process(argv,argc);
        }
    }
    return 0;
}

