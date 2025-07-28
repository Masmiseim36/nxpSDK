/*
 * Copyright (c) 2012-2021 Cadence Design Systems, Inc.
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
#include "g711_codec/xa_g711_codec_api.h"
#include "xa_type_def.h"
#include "xa_error_standards.h"
#include "xa_error_handler.h"

#ifdef __XCC__
#include <xtensa/hal.h>
#endif

#ifdef __PROFILE__
#include <sys/times.h>
#include <xtensa/sim.h>
#endif


#define PARAMFILE "paramfilesimple_encode.txt"
#define XA_MAX_CMD_LINE_LENGTH 300
#define XA_MAX_ARGS 6

#define XA_TESTBENCH_FATAL_MEM_ALLOC_FAILED  XA_ERROR_CODE(xa_severity_fatal, 0, 0, 0)
#define XA_TESTBENCH_FATAL_FILE_OPEN_FAILED  XA_ERROR_CODE(xa_severity_fatal, 0, 0, 1)
#define XA_TESTBENCH_FATAL_CONFIGURATION     XA_ERROR_CODE(xa_severity_fatal, 1, 0, 0)

/* These are codec library error*/
VOID xa_g711_codec_error_handler_init();
extern struct xa_error_info_struct xa_g711_codec_error_info;

/* These are testbench error*/
VOID xa_testbench_error_handler_init();
extern struct xa_error_info_struct xa_testbench_error_info;

XA_ERRORCODE xa_error_handler(
    xa_error_info_struct *p_mod_err_info,
    const char *pb_context,
    XA_ERRORCODE code);

XA_ERRORCODE xa_g711_enc_main_process(char *in_out_tests[], int argc);

/******************************************************************************
 * Function Name: xa_g711_paramfile_parse

 * Description: 1. Parse the parameter file
*******************************************************************************/
XA_ERRORCODE xa_g711_paramfile_parse(char* paramfile_name)
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
        {   /* skip it */
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

        if(fargc < 1)    /* for blank lines etc. */
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
            int ix = 0;
            char pb_input_file_name[XA_MAX_CMD_LINE_LENGTH] = "";
            char pb_output_file_name[XA_MAX_CMD_LINE_LENGTH] = "";

            /* input file */
            strcat(pb_input_file_name, pb_input_file_path);
            strcat(pb_input_file_name, fargv[0]);
            pargv[ix+1] = pb_input_file_name;

            /* output file */
            strcat(pb_output_file_name, pb_output_file_path);
            strcat(pb_output_file_name, fargv[1]);
            pargv[ix+2] = pb_output_file_name;

            /* law */
            pargv[ix+3] = fargv[2];


            error_code = xa_g711_enc_main_process(pargv,fargc+1);
        }
    }

    return error_code;
}

/*****************************************************************************
 * Function Name: xa_g711_enc_main_process

 * Description: 1. This function parses the commandline argumant passed to
                   the encoder.
                2. Initialize the G.711 error handler testbench and library.
                3. Allocate persistent, Scratch, Input, output memory buffer.               
                4. Initialize and run encoder.
                5. Print frame and MCPS numbers.
                6. Write encoded frames to the output.   
                7. Free memory used by encoder.
*****************************************************************************/
XA_ERRORCODE xa_g711_enc_main_process(char *in_out_tests[], int argc)
{
    WORD32 handle_size, scratch_size, inp_speech_size, enc_speech_size;

    xa_codec_handle_t speech_encoder_state = NULL;
    pVOID  scratch;
    pWORD16 inp_speech;
    pUWORD8 enc_speech;

    int frame;

    xa_g711_law_t law;
    xa_g711_frame_type_t enc_frame_type;

    FILE *enc_instream_fid, *enc_outstream_fid;

    /* Error code */
    XA_ERRORCODE error_code = XA_NO_ERROR;

    /* ******************************************************************/
    /* Initialize the error handler testbench                           */
    /* ******************************************************************/
    xa_error_info_struct *p_proc_testbench_err_info = NULL;
    p_proc_testbench_err_info = (xa_error_info_struct*)&xa_testbench_error_info;
    xa_testbench_error_handler_init();

    /* ******************************************************************/
    /* Initialize the error handler library                             */
    /* ******************************************************************/
    xa_error_info_struct *p_proc_codec_err_info = NULL;
    p_proc_codec_err_info = (xa_error_info_struct*)&xa_g711_codec_error_info;
    xa_g711_codec_error_handler_init();

    int i;

#ifdef __PROFILE__
    struct tms start, stop;
    clock_t cycles;
    double mcps_peak = 0, mcps_curr, mcps_ave, mcps_sum = 0;
    int peak_frame = 0;
#endif

    fprintf(stderr, "\n");
    fprintf(stderr, "%s library version %s\n",
            xa_g711_get_lib_name_string(),
            xa_g711_get_lib_version_string());
    fprintf(stderr, "API version: %s\n", xa_g711_get_lib_api_version_string());
    fprintf(stderr, "Tensilica, Inc. http://www.tensilica.com\n");
    fprintf(stderr, "\n");


    /************************************************************************/
    /*  Opening Input Speech File                                           */
    /************************************************************************/
    enc_instream_fid = fopen(in_out_tests[1],"rb");
    if (enc_instream_fid == NULL) {
        _XA_HANDLE_ERROR(p_proc_testbench_err_info, "Input File", XA_TESTBENCH_FATAL_FILE_OPEN_FAILED);
    }
    fprintf(stderr, "Input speech file:  %s\n", in_out_tests[1]);

    /************************************************************************/
    /*  Opening Output Bitstream  File                                      */
    /************************************************************************/
    enc_outstream_fid = fopen(in_out_tests[2],"wb");
    if (enc_outstream_fid == NULL) {
        _XA_HANDLE_ERROR(p_proc_testbench_err_info, "Output File", XA_TESTBENCH_FATAL_FILE_OPEN_FAILED);
    }
    fprintf(stderr, "Output bitstream file:  %s\n", in_out_tests[2]);

    /* Check the law type */
    if (in_out_tests[3][0] == 'A')
    {
        law = XA_G711_LAW_A;
    }
    else if (in_out_tests[3][0] == 'U')
    {
        law = XA_G711_LAW_U;
    }
    else
    {
        fprintf(stderr, "Invalid law type: %s\n", in_out_tests[3]);
        _XA_HANDLE_ERROR(p_proc_testbench_err_info, "Law type", XA_TESTBENCH_FATAL_CONFIGURATION);
    }
    fprintf(stderr, "Law type: %s\n", (law == XA_G711_LAW_A) ? "A" : "U");


    /************************************************************
    *        Get  Memory sizes from library                     *
    *************************************************************/
    handle_size = xa_g711_enc_get_handle_byte_size();
    scratch_size = xa_g711_enc_get_scratch_byte_size();
    inp_speech_size = XA_G711_MAX_SAMPLES_PER_FRAME * (XA_G711_NUM_BITS_PER_SAMPLE/8);
    enc_speech_size = XA_G711_MAX_NUM_BYTES_PER_FRAME;

    fprintf(stderr, "\nPersistent state size: %d bytes\n", handle_size);
    fprintf(stderr, "Scratch buffer size:   %d bytes\n", scratch_size);
    fprintf(stderr, "Input buffer size:     %d bytes\n", inp_speech_size);
    fprintf(stderr, "Output buffer size:    %d bytes\n\n", enc_speech_size);

    /************************************************************
    *                     Allocate Memory                       *
    ************************************************************/
    speech_encoder_state = (xa_codec_handle_t)malloc(handle_size);
    if(speech_encoder_state == NULL) {
        _XA_HANDLE_ERROR(p_proc_testbench_err_info, "API state alloc", XA_TESTBENCH_FATAL_MEM_ALLOC_FAILED);
    }

    scratch = (pVOID)malloc(scratch_size);
    if(scratch == NULL) {
        _XA_HANDLE_ERROR(p_proc_testbench_err_info, "API scratch alloc", XA_TESTBENCH_FATAL_MEM_ALLOC_FAILED);
    }

    inp_speech = (pWORD16)malloc(inp_speech_size);
    if(inp_speech == NULL) {
        _XA_HANDLE_ERROR(p_proc_testbench_err_info, "API input alloc",XA_TESTBENCH_FATAL_MEM_ALLOC_FAILED);
    }

    enc_speech = (pUWORD8)malloc(enc_speech_size);
    if(enc_speech == NULL) {
        _XA_HANDLE_ERROR(p_proc_testbench_err_info, "API output alloc", XA_TESTBENCH_FATAL_MEM_ALLOC_FAILED);
    }

    /* this is an application buffer where the conformance frame are stored and written to file */
    /* The conformance output is 16-bits per codeword */
    int app_enc_speech_size = XA_G711_MAX_NUM_BYTES_PER_FRAME * sizeof(WORD16);
    pWORD16 app_enc_speech = NULL;
    app_enc_speech  = (pWORD16) malloc(app_enc_speech_size);
    if(app_enc_speech == NULL) {
        _XA_HANDLE_ERROR(p_proc_testbench_err_info, "API conformance buffer alloc", XA_TESTBENCH_FATAL_MEM_ALLOC_FAILED);
    }

    /********************************************************************************
     *                              Initialization.                                 *
     ********************************************************************************/
    error_code = xa_g711_enc_init(speech_encoder_state,
                                  scratch );

    _XA_HANDLE_ERROR(p_proc_codec_err_info, "", error_code);

    frame = 0;

    /************************************************
     *        Start encoding frame by frame         *
     ************************************************/
    while(fread(inp_speech, sizeof(WORD16), XA_G711_MAX_SAMPLES_PER_FRAME, enc_instream_fid ) == XA_G711_MAX_SAMPLES_PER_FRAME) {
        frame++;
#if __XTENSA_EB__
        {
            int i;
            for (i = 0; i < XA_G711_MAX_SAMPLES_PER_FRAME; i++) {
                inp_speech[i] = AE_SHA32(inp_speech[i]);
            }
        }
#endif

        /* scratch test */
        memset(scratch,0x0a,scratch_size);

#ifdef __PROFILE__
        xt_iss_profile_enable();
        times(&start);
#endif


        error_code = xa_g711_enc(speech_encoder_state,
                                 inp_speech,
                                 enc_speech,
                                 law,
                                 &enc_frame_type);

        _XA_HANDLE_ERROR(p_proc_codec_err_info, "", error_code);

#ifdef __PROFILE__
        times(&stop);
        xt_iss_profile_disable();

        cycles = stop.tms_utime - start.tms_utime;
        mcps_curr = (cycles * 8000.0 /* Hz */ / (80 /* samples/frame */ * 1000000.0));
        mcps_sum += mcps_curr;
        mcps_ave = mcps_sum / frame;
        if (mcps_peak < mcps_curr) {
            mcps_peak = mcps_curr;
            peak_frame = frame;
        }
#endif

#ifdef __PROFILE__
        fprintf(stderr, "[%d] MCPS: %f Average: %f Peak: %f @ [%d]\n",frame,mcps_curr, mcps_ave, mcps_peak, peak_frame);
#endif

        /* Expand each 8-bit codeword into 16 bits for compliance testing
         * Also, mask-out any sign extension.  The compliance test vectors
         * simply have zeros in the upper 8 bits
         */
        for (i = 0; i < XA_G711_MAX_NUM_BYTES_PER_FRAME; i++)
        {
            app_enc_speech[i] = ((WORD16) enc_speech[i]) & 0x00FF;

#if __XTENSA_EB__
            app_enc_speech[i] = AE_SHA32(app_enc_speech[i]);
#endif /* __XTENSA_EB__ */
        }

        /* write conformance frame to file */
        fwrite(app_enc_speech, sizeof(WORD16), XA_G711_MAX_NUM_BYTES_PER_FRAME, enc_outstream_fid);

        fflush(enc_outstream_fid);

#ifdef TEST_SCRATCH
        /* Memset the scratch buffer.  This is to test that the scratch buffer values have no effect on behavior. */
        memset(scratch,0xAA,scratch_size);
#endif

    } /* do exec */

    fprintf(stderr, "\n%d frame(s) processed\n\n", frame);

#ifdef __PROFILE__
    fprintf(stderr, "Peak MCPS = %f\n", mcps_peak);
    fprintf(stderr, "Peak frame = %d\n", peak_frame);
    fprintf(stderr, "Average MCPS = %f\n\n", mcps_ave);
#endif

    fclose(enc_instream_fid);
    fclose(enc_outstream_fid);

    free(speech_encoder_state);
    free(scratch);
    free(inp_speech);
    free(enc_speech);

    free(app_enc_speech);

    return XA_NO_ERROR;
}

/*****************************************************************************
 * Function Name: showusage()
 * Description: Display the usage of G.711 Encoder.              
******************************************************************************/
void showusage()
{
    fprintf(stderr, "Usage: xa_g711_enc_test -i <speech_file> -o <bitstream_file> <law>\n");
    fprintf(stderr, "Format for <speech_file>:\n");
    fprintf(stderr, "  Speech is read from a binary file of 16-bit PCM data.\n");
    fprintf(stderr, "Format for <bitstream_file>:\n");
    fprintf(stderr, "  80 words (2-byte) containing 80 PCM codewords.\n");
    fprintf(stderr, "<law>:\n");
    fprintf(stderr, "  'A' for A-law\n");
    fprintf(stderr, "  'U' for Mu-law\n");
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

/**********************************************************************************
 * Function Name: main()
 * Description: 1. Application program main gate. 
 *              2. One can pass a parameter file OR encoder command line argument 
                   to run the encoder.
***********************************************************************************/
int main(int argc, char *argv[])
{
    // NOTE: set_wbna() should be called before any other dynamic
    // adjustment of the region attributes for cache.
    set_wbna(&argc, argv);

    if(argc == 1)//default name
    {
        xa_g711_paramfile_parse(PARAMFILE);
    }
    else
    {
        if (argc != 4) {
            showusage();
        }
        else {
            xa_g711_enc_main_process(argv,argc);//in/out test name
        }
    }
    return 0;
}
