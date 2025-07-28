/*
 * Copyright (c) 2011-2025 Cadence Design Systems, Inc.
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
#include <assert.h>
#include <math.h>

#include "xa_type_def.h"
#include "xa_error_standards.h"
#include "xa_error_handler.h"
#include "xa_apicmd_standards.h"
#include "xa_memory_standards.h"
#include "src_pp/xa_src_pp_api.h"
#include "xa_src_pp_waveio.h"
#include "xa_jitt_buf.h"


// #define OUTPUT_PING_PONG
// #define INPUT_PING_PONG
// #define SCRATCH_TRASH_TEST

#ifdef ASRC_ENABLE
//#define DISPLAY_MESSAGE_ASRC
//#define ASRC_ENABLE_RUNTIME_DRIFT_TEST
//#define ASRC_DRIFT_DBG                                // Uncomment this to enable debug prints related to drift application.
#endif

#ifdef ASRC_ENABLE_RUNTIME_DRIFT_TEST                   
#define CS_DRIFT_FROM_FILE                            //  This flag  enables the drift & inp_smap value from side-info file. !! This code is not yet tested !!.
#endif // ASRC_ENABLE_RUNTIME_DRIFT_TEST

#ifdef __XCC__
#include <xtensa/hal.h>
#endif

#if !defined(PROFILE) && __XCC__
#define PROFILE 1
#endif

#if !defined(SHOW_SIZES)
#define SHOW_SIZES 1
#endif /* SHOW_SIZES */

#if PROFILE
#include <sys/times.h>
#include <xtensa/sim.h>
struct tms start, stop;
double Peak = 0, Ave, Sum = 0;
int Peak_frame;
unsigned long long total_samples = 0;
#endif
#ifdef USE_SRCPLUS_LIBRARY 
#define PARAMFILE "paramfile_srcplus_pp.txt"
#elif USE_SRC384KHZ_TRIMMED_LIBRARY
#define PARAMFILE "paramfile_src384k_pp.txt"
#elif NUM_IOSAMPLES_INSYNC
#define PARAMFILE "paramfile_frio_hifi3.txt"
#else
#define PARAMFILE "paramfile_src_pp.txt"
#endif
#define XA_MAX_CMD_LINE_LENGTH 300
#define XA_MAX_ARGS 20
#define MAX_MEM_ALLOCS 100

#define MAX_INPUT_CHUNK_LEN 512
#define MAX_CHANNELS 24

#ifdef ASRC_ENABLE
#define XA_SRC_PUSH_MODE 0
#define XA_SRC_PULL_MODE 1
#endif

#define ALIGNMENT 8

#define XA_TESTBENCH_MFMAN_FATAL_MEM_ALLOC_FAILED       0xffff8000
#define XA_TESTBENCH_MFMAN_FATAL_FILE_OPEN_FAILED       0xffff8001

#define DISPLAY_MESSAGE

extern xa_error_info_struct xa_testbench_error_info;
extern xa_error_info_struct xa_src_pp_error_info;

VOID xa_testbench_error_handler_init(void);
VOID xa_src_pp_error_handler_init(void);

char *binName = NULL;
WORD  g_w_malloc_count;
pVOID g_pv_arr_alloc_memory[MAX_MEM_ALLOCS];

/*  Pointers to the raw input/output buffers used by the file I/O in the main loop. */
pVOID ppin_buffer;
pVOID ppout_buffer;

/* API obj */
xa_codec_handle_t xa_process_handle;
xa_error_info_struct *p_proc_err_info;
/* The process API function */
xa_codec_func_t *p_xa_process_api;

/********************************************************************************
 * UserConfig defines a structure for command-line configuration parameters
 ********************************************************************************/
/*int user_filter[512];*/
typedef struct _user_config
{
    const char *in_file_name;
    const char *out_file_name;
    int         channels;
    int         fs_in;
    int         fs_out;
    int         input_chunk_size;
    int         bytes_per_sample;
#ifdef POLYPHASE_CUBIC_INTERPOLATION
    int         enable_cubic;
#endif /* #ifdef POLYPHASE_CUBIC_INTERPOLATION */
    int         reset;
#ifdef ASRC_ENABLE
    int         enable_asrc;
    long long   drift_asrc;
    int         enable_pull;
#ifdef CS_DRIFT_FROM_FILE	
    const char *cs_cf_file_name;
#endif /* CS_DRIFT_FROM_FILE */
#endif /* #ifdef ASRC_ENABLE */
} user_config ;

WORD8 pb_input_file_path[XA_MAX_CMD_LINE_LENGTH] = "";
WORD8 pb_output_file_path[XA_MAX_CMD_LINE_LENGTH] = "";

/* User Defaults */
const user_config default_config =
{
#ifdef ASRC_ENABLE
#ifdef POLYPHASE_CUBIC_INTERPOLATION
#ifdef CS_DRIFT_FROM_FILE
    NULL, NULL, 2,  48000, 48000, MAX_INPUT_CHUNK_LEN, 3, 0, -1, 0, 0, 0, NULL 
#else
    NULL, NULL, 2,  48000, 48000, MAX_INPUT_CHUNK_LEN, 3, 0, -1, 0, 0, 0 
#endif
#else /* #ifdef POLYPHASE_CUBIC_INTERPOLATION */
#ifdef CS_DRIFT_FROM_FILE
    NULL, NULL, 2,  48000, 48000, MAX_INPUT_CHUNK_LEN, 3, -1, 0, 0 , 0, NULL
#else
    NULL, NULL, 2,  48000, 48000, MAX_INPUT_CHUNK_LEN, 3, -1, 0, 0 , 0
#endif
#endif /* #ifdef POLYPHASE_CUBIC_INTERPOLATION*/
#else /* #ifdef ASRC_ENABLE */
#ifndef POLYPHASE_CUBIC_INTERPOLATION
    NULL, NULL, 2,  48000, 48000, MAX_INPUT_CHUNK_LEN, 3, -1 
#else  /* #ifndef POLYPHASE_CUBIC_INTERPOLATION */
    NULL, NULL, 2,  48000, 48000, MAX_INPUT_CHUNK_LEN, 3, 0, -1
#endif /* #ifndef POLYPHASE_CUBIC_INTERPOLATION */
#endif /* #ifdef ASRC_ENABLE */

};

int ROUND(double inp)
{
    int out = (int)(inp + 0.5);
    return out;
}

void print_usage(void)
{
    printf("\nUsage         : xa_sample_rate_converter -ifile:<input filename>  -ofile:<output filename> -inrate:<input samplerate>");
    printf(" -outrate:<output sample rate> -insize:<input chunk size>  -ch:<number of input channels> -pcmwidth:<width of pcm sample in bytes>"); 
#ifdef POLYPHASE_CUBIC_INTERPOLATION
    printf(" -enable_cubic:<enable cubic interpolation>");
#endif /* #ifdef POLYPHASE_CUBIC_INTERPOLATION */
    printf(" -reset:<reset value>");
#ifdef ASRC_ENABLE
	printf(" -enable_asrc:<enable asrc>  -enable_pull:<enable pull> -drift_asrc:<drift apply to 1 output sample>");
#endif /* #ifdef ASRC_ENABLE */
    printf(" \n\n\n");

    printf("\t input file name                      : Complete path for the input file\n");
    printf("\t output file name                     : Complete path for the output file\n");
    printf("\t input sample rate                    : samples per second in the input stream. In case of raw pcm input files, the user must provide this information (default = 48000)\n");
    printf("\t                                        Expected value is a standard audio sample rate between 4 kHz to 384kHz* \n");
    printf("\t output sample rate                   : required samples per second in the output stream (default = 48000)\n");
    printf("\t                                        Expected value is a standard audio sample rate between 4 kHz to 384kHz*\n");
    printf("\t input chunk size                     : Number of pcm samples in the input buffer (applicable for SRC and ASRC Push Mode)\n");
    printf("\t                                        Number of pcm samples in the output buffer (applicable for ASRC Pull Mode) \n");
    printf("\t                                        For SRC and ASRC Push Mode Input chunk size can be from 4 to 512 (default : 512) \n");
    printf("\t                                        For ASRC Pull Mode, Input chunk size should follow this criterion : trunc(chunk size*fs_in/fs_out) >= 4 and chunk size <= 512 (default : 512)\n");
    printf("\t number of channels in the input file : in case of raw pcm input files, the user must provide this information Range: 1-24 (default = 2)\n");
    printf("\t pcm width                            : in case of raw pcm input files, the user must provide this information Pcmwidth should be 2 or 3 bytes(default = 3 bytes)\n");
#ifdef POLYPHASE_CUBIC_INTERPOLATION
    printf("\t enable_cubic interpolation           : Enable or disable cubic interpolation(default = 0, cubic interpolation is disabled by default )\n");
    printf("\t                                        For the current library, any value other than 0 or 1 is invalid\n");
#endif /* #ifdef POLYPHASE_CUBIC_INTERPOLATION */
    printf("\t reset value                          : To enable run time init, this value must be equal to frame_count when reset needs to be done. Multiple frame_count values are not allowed (default = 0)\n");
#ifdef ASRC_ENABLE
    printf("\t enable asrc                          : Enable or disable asrc (default = 0, asrc is disabled by default )\n");
    printf("\t                                        For the current library, any value other than 0 or 1 is invalid\n");
    printf("\t enable pull                          : Enable pull mode for asrc (default = 0, push mode is enabled by default) \n");
    printf("\t asrc drift value                     : drift apply to 1 output sample. This value must be between the ranges -0.04 to 0.04 (default = 0) with step of 0.000001 \n");
    printf("\t In case of wave input files, input sample rate, number of channels and pcm width are read from wave header\n");
#endif /* #ifdef ASRC_ENABLE */   

    printf("\n* when input sample rate = 384 kHz or 352.8 kHz, the output sample rate must be set to 48 kHz	\n");
    printf("* For output sample rate = 384 kHz, only the following sample rate are allowed:32 kHz, 44.1 kHz, 48 kHz, 96 kHz and 192 kHz\n");	
    printf("* when input or output sample rate = 4 kHz or 6 kHz, only the following sample rate are allowed : 16 kHz and 48 kHz\n");	
    printf("* ASRC and streams with more than two channels are not supported when input or output sample rate is 384 kHz.\n");	
    printf("NOTE : Above features are applicable for the default package. In case of custom library, some features/conversion ratios may not be supported\n");	

}


XA_ERRORCODE read_user_config (user_config *puser_config, int argc, char **argv)
{
    int input=0, output=0;
    /* Get command line parameters */
    while (argc)
    {
        if ('-' == argv[0][0])
        {
            argv[0]++;
            if (0 == strncmp(argv[0], "ifile:", 6))
            {
                puser_config->in_file_name = argv[0]+6;
                input = 1;
            }
            else if (0 == strncmp(argv[0], "ofile:", 6))
            {
                puser_config->out_file_name = argv[0]+6;
                output = 1;
            }
#ifdef CS_DRIFT_FROM_FILE			
            else if (0 == strncmp(argv[0], "sfile:", 6))
            {
                puser_config->cs_cf_file_name = argv[0]+6;               
            }	
#endif		
            else if (0 == strncmp(argv[0], "outrate:", 8))
            {
                puser_config->fs_out = atoi(argv[0]+8);
            }
            else if (0 == strncmp(argv[0], "inrate:", 7))
            {
                puser_config->fs_in = atoi(argv[0]+7);
            }
            else if (0 == strncmp(argv[0], "insize:", 7))
            {
                puser_config->input_chunk_size = atoi(argv[0]+7);
            }
            else if (0 == strncmp(argv[0], "ch:", 3))
            {
                puser_config->channels = atoi(argv[0]+3);
            }
            else if (0 == strncmp(argv[0], "pcmwidth:", 9))
            {
                puser_config->bytes_per_sample = atoi(argv[0]+9);
            }
#ifdef POLYPHASE_CUBIC_INTERPOLATION
            else if (0 == strncmp(argv[0], "enable_cubic:", 13))
            {
                puser_config->enable_cubic = atoi(argv[0]+13);

                if (0 > puser_config->enable_cubic || puser_config->enable_cubic >= 2)
                    printf("The enable cubic flag must be 0-%d, not \"%s\"\n", 1, argv[0]);

            }
#endif /* #ifdef POLYPHASE_CUBIC_INTERPOLATION */
            else if (0 == strncmp(argv[0], "reset:", 6))
            {
                puser_config->reset = atoi(argv[0]+6);

                if ( puser_config->reset  < 0)
                    printf("The reset flag must be greater than or equal to 0 \n");
            }
#ifdef ASRC_ENABLE
#ifdef CS_DRIFT_FROM_FILE			
            else if (0 == strncmp(argv[0], "sfile:", 6))
            {
                puser_config->cs_cf_file_name = argv[0]+6;               
            }	
#endif
            else if (0 == strncmp(argv[0], "enable_asrc:", 12))
            {
                puser_config->enable_asrc = atoi(argv[0]+12);

                if (0 > puser_config->enable_asrc || puser_config->enable_asrc >= 2)
                    printf("The enable asrc flag must be 0-%d, not \"%s\"\n", 1, argv[0]);
            }
            else if (0 == strncmp(argv[0], "enable_pull:", 12))
            {
                puser_config->enable_pull = atoi(argv[0]+12);

                if (0 > puser_config->enable_pull || puser_config->enable_pull >= 2)
                    printf("The enable pull flag must be 0-%d, not \"%s\"\n", 1, argv[0]);
            }
            else if (0 == strncmp(argv[0], "drift_asrc:", 11))
            {
                double drift_float = atof(argv[0]+11);   
                if ( ( drift_float > 0.04 ) | ( drift_float < -0.04 ) )
                {
                    printf("The asrc drift must be between the ranges ( -0.04 to 0.04 ), not \"%f\"\n", drift_float);

                    if ( drift_float > 0.04 )
                        drift_float = 0.04;           /* drift_float = min( drift_float, 0.04 ); */

                    if ( drift_float < -0.04 )
                        drift_float = -0.04;          /* drift_float = max( drift_float, -0.04 ); */                    

                    printf(" So drift value modified to \"%f\"\n", drift_float);

                }

                puser_config->drift_asrc = ( long long)( (drift_float)*  ( (long long)1 << 31) ); /* converting drift_float to Q31 fixed value */
            }
#endif /* #ifdef ASRC_ENABLE */
            else
            {
                printf("Unknown parameter: \"-%s\"\n", argv[0]);
                print_usage();
                return -1;
            }
        }
        else
        {
            printf("Unknown parameter: \"%s\"\n", argv[0]);
            print_usage();
            return -1;
        }

        argv++;
        argc--;
    }/* while (argc) */

    if(input == 0)
    {
        printf("\nPlease give input file to process via \"-ifile:\" option\n");
        print_usage();
        return -1;
    }

    if(output == 0)
    {
        printf("\nPlease give name of output file to write via \"-ofile:\" option\n");
        print_usage();
        return -1;
    }

    return XA_NO_ERROR;
}

int read_from_wave_file(
    FILE *fp_in,
    pVOID pin,
    int bufsize,
    int channels,
    int bytes_per_sample,
    int curr_index,
    int total_length)
{
    int insize= 0, ret_val, val, index = 0;

    /* Assumes interleaved input */
    if(bytes_per_sample == TWO_BYTES_PER_SAMPLE)
    {
        /* 16 bit pcm */
        pWORD16 pin_16bitpcm=(WORD16 *)pin;  
        index = 0;

        while(channels*bufsize > index)
        {
            ret_val = fread(&val,TWO_BYTES_PER_SAMPLE,1,fp_in);

            curr_index++;

            if (curr_index > total_length)
                break;

            pin_16bitpcm[index] = val;
            ++index;

            insize += ret_val;
            
        }

    }/* if */
    else
    {
        /* 24 bit pcm */
        pWORD32 pin_24bitpcm=(WORD32 *)pin;

        index = 0;
        while(channels*bufsize > index)
        {
            ret_val = fread(&val,THREE_BYTES_PER_SAMPLE,1,fp_in);

            curr_index++;

            if (curr_index > total_length)
                break;

            /* Make the input sample MSB aligned */
            val <<= 8;

            pin_24bitpcm[index] = val;
            ++index;

            insize += ret_val;
            
        }

    }
    return insize/channels;
}

/* Read interleaved input pcm samples */
int read_from_file(
    FILE *fp_in,
    pVOID pin,
    int bufsize,
    int channels,
    int bytes_per_sample)
{
    int insize= 0, ret_val, val, index = 0;

    /* Assumes interleaved input */
    if(bytes_per_sample == TWO_BYTES_PER_SAMPLE)
    {
        /* 16 bit pcm */
        pWORD16 pin_16bitpcm=(WORD16 *)pin;  
        index = 0;

        while(channels*bufsize > index)
        {
            ret_val = fread(&val,TWO_BYTES_PER_SAMPLE,1,fp_in);

            if (feof(fp_in))
                break;

            pin_16bitpcm[index] = val;
            ++index;

            insize += ret_val;
        }

    }/* if */
    else
    {
        /* 24 bit pcm */
        pWORD32 pin_24bitpcm=(WORD32 *)pin;

        index = 0;
        while(channels*bufsize > index)
        {
            ret_val = fread(&val,THREE_BYTES_PER_SAMPLE,1,fp_in);

            if (feof(fp_in))
                break;

            /* Make the input sample MSB aligned */
            val <<= 8;

            pin_24bitpcm[index] = val;
            ++index;

            insize += ret_val;
        }

    }

    return insize/channels;
}

/* Write interleaved output pcm samples */
void write_to_file(FILE *fp_out, pVOID pin, int size, int channels, int bytes_per_sample)
{    
    if(bytes_per_sample == THREE_BYTES_PER_SAMPLE)
    {
        int val, index = 0;       
        int *temp=(int *)pin;	
        /* The output is stored in interleaved fashion in 24-bit/16-bit in pin buffer */
        {
            while(size*channels > index)
            {
                val = temp[index];
                /* Write 24-bit PCM sample which is MSB-aligned */
                val >>= 8; 
                fwrite(&val,bytes_per_sample,1,fp_out);
                ++index;
            }
        }
    }/* if */
    else
    {
        WORD16 *temp = (WORD16 *)pin;
        WORD16 val; 
        int index=0;

        while(size*channels > index)
        {
            val = temp[index];			
            fwrite(&val,bytes_per_sample,1,fp_out);	
            ++index; 
        }

    }/* else */
}



#ifdef CS_DRIFT_FROM_FILE
FILE *fp_side_info = NULL;
#endif


int xa_src_pp_main_process (int argc, char **argv)
{
    /* User Config */
    user_config def_user_config;
    FILE *fp_in = NULL, *fp_out = NULL ;
    int i;
    /* Error code */
    XA_ERRORCODE err_code = XA_NO_ERROR;
    VOID (*p_error_init)(void);
    /* API size */
    UWORD32 pui_api_size, bufsize, insize, outsize;
    UWORD32 ui_proc_mem_tabs_size;
    wave_header wave_info;
    WORD32 frame_count = 0;
    WORD32 is_wave = 0;
    WORD32 max_outsize = 0;

#ifdef SCRATCH_TRASH_TEST
    WORD32 scratch_size;
    void *p_scratch;
#endif
#ifdef OUTPUT_PING_PONG
    void *p_output_buffer_ping, *p_output_buffer_pong;
    WORD32 out_idx;
#endif /* #ifdef OUTPUT_PING_PONG */

#ifdef INPUT_PING_PONG
    void *p_input_buffer_ping, *p_input_buffer_pong;
    WORD32 in_idx;
#endif /* #ifdef INPUT_PING_PONG */

#ifdef ASRC_ENABLE_RUNTIME_DRIFT_TEST
    double drift_float_runtime = 0.04;
    double tot_drift_to_all_inp = 0.0;
#endif /* #ifdef ASRC_ENABLE_RUNTIME_DRIFT_TEST */

    WORD8 pb_input_file_name[XA_MAX_CMD_LINE_LENGTH];
    WORD8 pb_output_file_name[XA_MAX_CMD_LINE_LENGTH];
#ifdef NUM_IOSAMPLES_INSYNC
    WORD32 total_inp_samples, total_out_samples;
#endif /* NUM_IOSAMPLES_INSYNC */

#ifdef ASRC_ENABLE_RUNTIME_DRIFT_TEST
    WORD64 total_consumed_samples = 0;
    WORD64 total_generated_samples = 0;
#endif /* #ifdef ASRC_ENABLE_RUNTIME_DRIFT_TEST */

#ifdef ASRC_ENABLE
    xa_src_jitt_struct jitter_states, jitter_states_output;    
    int error_jitetr_ret, frame_size_for_record, inp_jit_buffer_length, out_jit_buffer_length, out_frame_size_pull_mode, jit_buffer_lvl;
    float drift;
#endif

    int total_length, curr_sample;

#ifdef DISPLAY_MESSAGE_ASRC
    WORD64 pre_Fm;
#endif

#ifdef DISPLAY_MESSAGE
    /* Library Info and Identification strings */
    char pb_process_name[30] = "";
    char pb_lib_version[30] = "";
    char pb_api_version[30] = "";
#endif
    /* Memory variables */
    UWORD32 n_mems;    

    /* The process API function */
    p_xa_process_api = xa_src_pp;

    p_error_init = xa_src_pp_error_handler_init;

    p_proc_err_info = &xa_src_pp_error_info;

    /********************************************************************
     *  Initialize the error handler                                    *
     ********************************************************************/
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

    /* Get the api version string */
    err_code = (*p_xa_process_api)(NULL, XA_API_CMD_GET_LIB_ID_STRINGS,
                                   XA_CMD_TYPE_API_VERSION, pb_api_version);
    _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

    /* Display the Tensilica identification message */
    fprintf(stdout, "\n%s LIB version : %s  API version : %s\n", pb_process_name, pb_lib_version, pb_api_version);
    fprintf(stdout, "Cadence, Inc. https://www.cadence.com\n\n");
#endif  //DISPLAY_MESSAGE

    /* ******************************************************************/
    /* Initialize API structure and set config params to default        */
    /* ******************************************************************/
    /* Get the API size */
    err_code = (*p_xa_process_api)(NULL, XA_API_CMD_GET_API_SIZE, 0, &pui_api_size);
    _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

#if SHOW_SIZES
    printf("\nAPI structure size: %u bytes\n", pui_api_size);
#endif

    /* Allocate memory for API */
    g_pv_arr_alloc_memory[g_w_malloc_count] = malloc(pui_api_size);

    if (g_pv_arr_alloc_memory[g_w_malloc_count] == NULL)
    {
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
    err_code = (*p_xa_process_api)(
                   xa_process_handle,
                   XA_API_CMD_INIT,
                   XA_CMD_TYPE_INIT_API_PRE_CONFIG_PARAMS,
                   NULL);
    _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

    /* ******************************************************************/
    /* Initialize Memory info tables                                    */
    /* ******************************************************************/
    /* Get memory info tables size */
    err_code = (*p_xa_process_api)(xa_process_handle,
                                   XA_API_CMD_GET_MEMTABS_SIZE, 0,
                                   &ui_proc_mem_tabs_size);
    _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

#if SHOW_SIZES
    printf("MEMTABS size: %u bytes\n", ui_proc_mem_tabs_size);
#endif

    g_pv_arr_alloc_memory[g_w_malloc_count] = malloc(ui_proc_mem_tabs_size);
    if(g_pv_arr_alloc_memory[g_w_malloc_count] == NULL)
    {
        _XA_HANDLE_ERROR(&xa_testbench_error_info, "Mem tables alloc",
                         XA_TESTBENCH_MFMAN_FATAL_MEM_ALLOC_FAILED);
    }

    /* Memory table requires 4 bytes (WORD32) alignment; malloc()
     * provides at least 8-byte alignment */
    assert((((unsigned int) g_pv_arr_alloc_memory[g_w_malloc_count]) & 3) == 0);

    /* Set pointer for process memory tables */
    err_code = (*p_xa_process_api)(
                   xa_process_handle,
                   XA_API_CMD_SET_MEMTABS_PTR, 0,
                   (void *) g_pv_arr_alloc_memory[g_w_malloc_count]);
    _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

    g_w_malloc_count++;

    /* ******************************************************************/
    /* Read user config, set configs for Sample Rate Converter          */
    /* ******************************************************************/
    def_user_config = default_config;

    if(read_user_config(&def_user_config,argc, argv) != XA_NO_ERROR)
        return -1;

    strcpy((char *)pb_input_file_name, (char *)pb_input_file_path);
    strcat((char *)pb_input_file_name, def_user_config.in_file_name);

    fp_in = fopen((char *)pb_input_file_name,"rb");
    if (fp_in == NULL)
    {
        _XA_HANDLE_ERROR(&xa_testbench_error_info, "Input File", XA_TESTBENCH_MFMAN_FATAL_FILE_OPEN_FAILED);
    }
#ifdef CS_DRIFT_FROM_FILE
    if (def_user_config.cs_cf_file_name == NULL)
    {
        printf("\nPlease give input file to process via \"-sfile:\" option\n");
        print_usage();
        return -1;
    }

    fp_side_info = fopen((char *)def_user_config.cs_cf_file_name,"rb");
    if (fp_side_info == NULL)
        _XA_HANDLE_ERROR(&xa_testbench_error_info, "Side Info File", XA_TESTBENCH_MFMAN_FATAL_FILE_OPEN_FAILED);
#endif

    strcpy((char *)pb_output_file_name, (char *)pb_output_file_path);
    strcat((char *)pb_output_file_name, def_user_config.out_file_name);

    fp_out = fopen((char *)pb_output_file_name,"wb");
    if (fp_out == NULL)
        _XA_HANDLE_ERROR(&xa_testbench_error_info, "Output File", XA_TESTBENCH_MFMAN_FATAL_FILE_OPEN_FAILED);

    /* Wave header read/write */
    if(read_wave_hdr(fp_in, &wave_info) == XA_NO_ERROR)
    {
        /* Input is wave file */
        is_wave = 1;
        def_user_config.bytes_per_sample = wave_info.bytes_per_sample;

        if ((wave_info.bytes_per_sample != 3) && (wave_info.bytes_per_sample != 2))
        {
            fprintf(stderr, "Only 24 bit and 16 bit wave format is supported!...\n");
            return -1;
        }

        def_user_config.fs_in = wave_info.fs_in;
        def_user_config.channels = wave_info.channels;
        wave_info.fs_out = def_user_config.fs_out;
		wave_info.out_bytes_per_sample = wave_info.bytes_per_sample;

        /* Write the wave header for the output file */
        write_wav_hdr(fp_out, &wave_info);
    }
    else
    {
        /* Move back to the begining */
        fseek(fp_in, 0, SEEK_SET);
    }


    /*************** Set Configs **************/
    err_code = (*p_xa_process_api)(xa_process_handle,
                                   XA_API_CMD_SET_CONFIG_PARAM,
                                   XA_SRC_PP_CONFIG_PARAM_INPUT_SAMPLE_RATE,
                                   &def_user_config.fs_in);
    _XA_HANDLE_ERROR(p_proc_err_info, "Set Input Fs Error", err_code);

    err_code = (*p_xa_process_api)(xa_process_handle,
                                   XA_API_CMD_SET_CONFIG_PARAM,
                                   XA_SRC_PP_CONFIG_PARAM_OUTPUT_SAMPLE_RATE,
                                   &def_user_config.fs_out);
    _XA_HANDLE_ERROR(p_proc_err_info, "Set Output Fs Error", err_code);

    err_code = (*p_xa_process_api)(xa_process_handle,
                                   XA_API_CMD_SET_CONFIG_PARAM,
                                   XA_SRC_PP_CONFIG_PARAM_INPUT_CHANNELS,
                                   &def_user_config.channels);
    _XA_HANDLE_ERROR(p_proc_err_info, "Set Input Channels Error", err_code);

	err_code = (*p_xa_process_api)(xa_process_handle,
                                   XA_API_CMD_SET_CONFIG_PARAM,
                                   XA_SRC_PP_CONFIG_PARAM_BYTES_PER_SAMPLE,
                                   &def_user_config.bytes_per_sample);
    _XA_HANDLE_ERROR(p_proc_err_info, "Set Input PCM width Error", err_code);

#ifdef POLYPHASE_CUBIC_INTERPOLATION
	
    err_code = (*p_xa_process_api)(xa_process_handle,
                                   XA_API_CMD_SET_CONFIG_PARAM,
                                   XA_SRC_PP_CONFIG_PARAM_ENABLE_CUBIC,
                                   &def_user_config.enable_cubic);
    _XA_HANDLE_ERROR(p_proc_err_info, "Set Enable Cubic Interpolation Error", err_code);

#endif /* #ifdef POLYPHASE_CUBIC_INTERPOLATION */

 
#ifdef ASRC_ENABLE

        err_code = (*p_xa_process_api)(xa_process_handle,
                                   XA_API_CMD_SET_CONFIG_PARAM,
                                   XA_SRC_PP_CONFIG_PARAM_ENABLE_ASRC,
                                   &def_user_config.enable_asrc);
        _XA_HANDLE_ERROR(p_proc_err_info, " Set enable asrc value Error", err_code);

        if ( def_user_config.enable_asrc == 1 )
        {            
            err_code = (*p_xa_process_api)(xa_process_handle,
                                       XA_API_CMD_SET_CONFIG_PARAM,
                                       XA_SRC_PP_CONFIG_PARAM_ENABLE_PULL,
                                       &def_user_config.enable_pull);
            _XA_HANDLE_ERROR(p_proc_err_info, " Set enable pull mode Error", err_code);

            err_code = (*p_xa_process_api)(xa_process_handle,
                                          XA_API_CMD_SET_CONFIG_PARAM,
                                          XA_SRC_PP_CONFIG_PARAM_DRIFT_ASRC,
                                          &def_user_config.drift_asrc);
            _XA_HANDLE_ERROR(p_proc_err_info, "ASRC drift value Error", err_code);
        }

    /* Set number of samples in the input buffer */
    err_code = (*p_xa_process_api)(xa_process_handle,
                                   XA_API_CMD_SET_CONFIG_PARAM,
                                   XA_SRC_PP_CONFIG_PARAM_INPUT_CHUNK_SIZE,
                                   &def_user_config.input_chunk_size);
    _XA_HANDLE_ERROR(p_proc_err_info, "Set Input Chunk Size Error", err_code);

    out_frame_size_pull_mode = def_user_config.input_chunk_size;

    if(def_user_config.enable_pull == XA_SRC_PULL_MODE && def_user_config.enable_asrc == 1)
    {
        float frame_size = ((float)(out_frame_size_pull_mode * def_user_config.fs_in / def_user_config.fs_out) + 1); //AI:1
        frame_size = frame_size + ceil((float)def_user_config.fs_in / def_user_config.fs_out);
        drift = (float)(def_user_config.drift_asrc+1)/2147483648;
        frame_size_for_record  = ceil(frame_size * (1-drift));
    }

#endif /* #ifdef ASRC_ENABLE */


    /* initialize the API, post config, fill memory tables */
    err_code = (*p_xa_process_api)(
                   xa_process_handle,
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

    for (i = 0; i < (WORD32) n_mems; i++)
    {
        int ui_size, ui_alignment, ui_type;
        pVOID pv_alloc_ptr;
        

        //WORD32 n_input;
      
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
        err_code = (*p_xa_process_api)(
                       xa_process_handle,
                       XA_API_CMD_GET_MEM_INFO_TYPE,
                       i,
                       &ui_type);
        _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

        g_pv_arr_alloc_memory[g_w_malloc_count] = malloc(ui_size);
        if(g_pv_arr_alloc_memory[g_w_malloc_count] == NULL)
        {
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
        err_code = (*p_xa_process_api)(
                       xa_process_handle,
                       XA_API_CMD_SET_MEM_PTR,
                       i,
                       pv_alloc_ptr);
        _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

        switch (ui_type)
        {
        case XA_MEMTYPE_INPUT:

#ifdef ASRC_ENABLE

            inp_jit_buffer_length = 0;

            if( (def_user_config.enable_pull == XA_SRC_PULL_MODE) && (def_user_config.enable_asrc == 1))
            {
                    void *pJitterBuffer;
                    inp_jit_buffer_length = ui_size * 2;
                    g_pv_arr_alloc_memory[g_w_malloc_count]  = malloc( inp_jit_buffer_length );
                    pJitterBuffer = g_pv_arr_alloc_memory[g_w_malloc_count];
                    g_w_malloc_count++;  
                    error_jitetr_ret = JittBufInit( &jitter_states, pJitterBuffer, inp_jit_buffer_length,  def_user_config.bytes_per_sample, def_user_config.channels, frame_size_for_record );

                    if( error_jitetr_ret != 0 )
                    {
                        printf("\n Jitter Buffer Initialization Failed \n");
                        return 0;        
                    }                    
            }
#endif

#if SHOW_SIZES
            fprintf(stdout, "Input buffer size: %u bytes\n", ui_size);
#ifdef ASRC_ENABLE
            fprintf(stdout, "Input Jitter buffer size: %u bytes\n", inp_jit_buffer_length);
#endif
#endif /* SHOW_SIZES */     

            ppin_buffer = pv_alloc_ptr;

#ifdef INPUT_PING_PONG
            p_input_buffer_ping = (WORD32 *)pv_alloc_ptr;

            g_pv_arr_alloc_memory[g_w_malloc_count] = malloc(ui_size);
            if(g_pv_arr_alloc_memory[g_w_malloc_count] == NULL) {
            _XA_HANDLE_ERROR(&xa_testbench_error_info, "Mem tables alloc",
                             XA_TESTBENCH_MFMAN_FATAL_MEM_ALLOC_FAILED);
            }

            /* 8-byte alignment check, should never fail on xtensa */
            assert((((unsigned int) g_pv_arr_alloc_memory[g_w_malloc_count]) % ui_alignment) == 0);
            p_input_buffer_pong = (void *) g_pv_arr_alloc_memory[g_w_malloc_count];
            g_w_malloc_count++;

            in_idx = i;
#endif /* INPUT_PING_PONG */

            break;

        case XA_MEMTYPE_OUTPUT:

#ifdef ASRC_ENABLE
            out_jit_buffer_length = 0;
            
            if ( ((def_user_config.enable_pull == XA_SRC_PUSH_MODE) || (def_user_config.enable_pull == XA_SRC_PULL_MODE))  && (def_user_config.enable_asrc == 1) ) /* playback mode */
            {                    
                void *pJitterBuffer;
                int conversion_ratio, extra_samples_for_jitt_buf;
                
                if(def_user_config.enable_pull == XA_SRC_PULL_MODE)
                {
                    out_jit_buffer_length = out_frame_size_pull_mode; 
                    conversion_ratio = ceil(((float)def_user_config.fs_out/def_user_config.fs_in));
                    extra_samples_for_jitt_buf = MAX_REMAINING_SAMPLES * conversion_ratio;
                }
                else
                {
                    out_jit_buffer_length = ceil((float)(def_user_config.input_chunk_size *  def_user_config.fs_out )/ def_user_config.fs_in);   
                    conversion_ratio = ceil(((float)def_user_config.fs_out/def_user_config.fs_in));
                    extra_samples_for_jitt_buf = ceil(out_jit_buffer_length * (float)(def_user_config.drift_asrc+1)/2147483648) + (MAX_REMAINING_SAMPLES * conversion_ratio);                
                }

                jit_buffer_lvl = out_jit_buffer_length ;
                out_jit_buffer_length = ((out_jit_buffer_length * 2) + ( extra_samples_for_jitt_buf)) * def_user_config.channels ;

                if ( def_user_config.bytes_per_sample == TWO_BYTES_PER_SAMPLE )
                {
                    out_jit_buffer_length *= 2;
                }
                else
                {
                    out_jit_buffer_length *= 4;                    
                }

                g_pv_arr_alloc_memory[g_w_malloc_count]  = malloc( out_jit_buffer_length );
                pJitterBuffer = g_pv_arr_alloc_memory[g_w_malloc_count];
                g_w_malloc_count++;  
                error_jitetr_ret = JittBufInit( &jitter_states_output, pJitterBuffer, out_jit_buffer_length,  def_user_config.bytes_per_sample, def_user_config.channels, jit_buffer_lvl);
                if( error_jitetr_ret != 0 )
                {
                    printf("\n Jitter Buffer Initialization Failed \n");
                    return 0;        
                }    
            }

#endif

            ppout_buffer = (WORD32 *)pv_alloc_ptr;
#if SHOW_SIZES
            fprintf(stdout, "Output buffer size: %u bytes\n", ui_size);
#ifdef ASRC_ENABLE
            fprintf(stdout, "Output Jitter buffer size: %u bytes\n", out_jit_buffer_length);
#endif
#endif /* SHOW_SIZES */

#ifdef OUTPUT_PING_PONG
            p_output_buffer_ping = (WORD32 *)pv_alloc_ptr;

            g_pv_arr_alloc_memory[g_w_malloc_count] = malloc(ui_size);
            if(g_pv_arr_alloc_memory[g_w_malloc_count] == NULL) {
            _XA_HANDLE_ERROR(&xa_testbench_error_info, "Mem tables alloc",
                             XA_TESTBENCH_MFMAN_FATAL_MEM_ALLOC_FAILED);
            }

            /* 8-byte alignment check, should never fail on xtensa */
            assert((((unsigned int) g_pv_arr_alloc_memory[g_w_malloc_count]) % ui_alignment) == 0);
            p_output_buffer_pong = (void *) g_pv_arr_alloc_memory[g_w_malloc_count];
            g_w_malloc_count++;

            out_idx = i;
#endif /* #ifdef OUTPUT_PING_PONG */
           
            max_outsize = ui_size/(4* def_user_config.channels);

            break;

        case XA_MEMTYPE_SCRATCH:
#if SHOW_SIZES
            fprintf(stdout, "Scratch buffer size: %u bytes\n", ui_size);
#endif /* SHOW_SIZES */

#ifdef SCRATCH_TRASH_TEST
            scratch_size = ui_size;
            p_scratch = (void *)pv_alloc_ptr;
#endif /* #ifdef SCRATCH_TRASH_TEST */
           
            break;

        case XA_MEMTYPE_PERSIST:
#if SHOW_SIZES
            fprintf(stdout, "Persistent buffer size: %u bytes\n", ui_size);
#endif /* SHOW_SIZES */ 
            break;

        default:
            assert(0);
        }
    } /* for (i = 0; i < (WORD32) n_mems; i++)  */



    /* ******************************************************************/
    /* Initialization of the Sample Rate Converter                      */
    /* ******************************************************************/
    err_code = (*p_xa_process_api)(xa_process_handle,
                                   XA_API_CMD_INIT,
                                   XA_CMD_TYPE_INIT_PROCESS,
                                   NULL);
    _XA_HANDLE_ERROR(p_proc_err_info, "Initialization Error", err_code);

    fprintf(stdout, "\n");

    /* ******************************************************************/
    /* Convey the SRC process details to the user                       */
    /* ******************************************************************/
    {
        int num_of_stages;

        /* Get the number of stages */
        err_code = (*p_xa_process_api)(xa_process_handle,
                                       XA_API_CMD_GET_CONFIG_PARAM,
                                       XA_SRC_PP_CONFIG_PARAM_GET_NUM_STAGES,
                                       &num_of_stages);
        _XA_HANDLE_ERROR(p_proc_err_info, "Get number of stages Error", err_code);

        printf("\nConverting from %d to %d", def_user_config.fs_in, def_user_config.fs_out);
        printf(" in %d stages\n", num_of_stages);

        printf("\nChannels: %d\n", def_user_config.channels);
        printf("\nInput Sample Rate: %d\n", def_user_config.fs_in);
        printf("\nOutput Sample Rate: %d\n", def_user_config.fs_out);

        printf("\nMaximum output size: %d samples\n",max_outsize);
    }

    printf("\n");

#ifdef ASRC_ENABLE
    if((def_user_config.enable_asrc == 1) && (def_user_config.enable_pull==1))
    {
        bufsize = frame_size_for_record; 
    }
    else
#endif
    {
        bufsize = def_user_config.input_chunk_size;
    }

#if PROFILE
    Peak = 0, Ave=0, Sum = 0;
    Peak_frame = 0;
    total_samples = 0;
#endif

    /* ******************************************************************/
    /* Execution Loop                                                   */
    /* ******************************************************************/
#ifdef NUM_IOSAMPLES_INSYNC
    total_inp_samples = 0;
    total_out_samples = 0;
#endif /* NUM_IOSAMPLES_INSYNC */

    curr_sample = 0;
    if(is_wave)
    {
        total_length = wave_info.length / def_user_config.bytes_per_sample;
    }

       
    while(1)
    {
#ifdef CS_DRIFT_FROM_FILE /* chunksize and drift from file */
        int inp_frame_size;
        fscanf(fp_side_info, " %d %lf", &inp_frame_size,  &drift_float_runtime);
        if ( drift_float_runtime > 0.04 )
        {
            drift_float_runtime = 0.04;
        }
        if ( drift_float_runtime < -0.04 )
        {
            drift_float_runtime = -0.04;
        }  
        def_user_config.drift_asrc = ( int)( (drift_float_runtime)*  ( (long long)1 << 31) ); /* converting drift_float to Q31 fixed value */
        err_code = (*p_xa_process_api)(xa_process_handle,
                                      XA_API_CMD_SET_CONFIG_PARAM,
                                      XA_SRC_PP_CONFIG_PARAM_DRIFT_ASRC,
                                      &def_user_config.drift_asrc);
        _XA_HANDLE_ERROR(p_proc_err_info, "ASRC drift value Error", err_code);

        if((def_user_config.enable_pull == XA_SRC_PUSH_MODE) && (def_user_config.enable_asrc == 1))
        {
            bufsize = inp_frame_size;
        }
#endif /* CS_DRIFT_FROM_FILE */

#ifdef PROFILE
        WORD32 samples_per_channel;
#endif

#ifdef ASRC_ENABLE

       if ( (def_user_config.enable_pull == XA_SRC_PULL_MODE) && (def_user_config.enable_asrc == 1) ) /* record mode */
       {        
           void *ppin_buffer_tmp/*[2]*/;

           /* The input file & jitter buffer data are assumed in interleaved format. */
           if ( def_user_config.bytes_per_sample == TWO_BYTES_PER_SAMPLE)
           {
               short *pIn_tmp = ( short *)jitter_states.pJittBuf;
               ppin_buffer_tmp = (void *)&pIn_tmp[jitter_states.JittBufLvl];
           }
           else
           {
               int *pIn_tmp = ( int *)jitter_states.pJittBuf;
               ppin_buffer_tmp = (void *)&pIn_tmp[jitter_states.JittBufLvl];            
           } 

           
            if(is_wave)
            {
                insize = read_from_wave_file(fp_in, ppin_buffer_tmp, bufsize, def_user_config.channels,def_user_config.bytes_per_sample, curr_sample, total_length);
                curr_sample += (insize*def_user_config.channels);
            }
            else
            {
                insize = read_from_file(fp_in, ppin_buffer_tmp, bufsize, def_user_config.channels,def_user_config.bytes_per_sample);
            }

           error_jitetr_ret = JittBufLevelUpdate(  &jitter_states, ( insize * def_user_config.channels) );

           if( error_jitetr_ret != 0 )
           {
               printf("\n Jitter Buffer state update Failed \n");
               return 0;        
           } 
           if ( insize != 0 )
           {
               if ( jitter_states.JittBufLvl < frame_size_for_record )
               {
                   printf("\n No Sufficient Input Samples \n");
                   break;                
               }
               error_jitetr_ret =  JittBufRead( &jitter_states, ppin_buffer, frame_size_for_record, def_user_config.bytes_per_sample, def_user_config.channels );
               if( error_jitetr_ret != 0 )
               {
                   printf("\n Jitter Buffer read Failed \n");
                   return 0;        
               }
           }
       }
       else
#endif
       {        
            if(is_wave)
            {
                insize = read_from_wave_file(fp_in, ppin_buffer, bufsize, def_user_config.channels,def_user_config.bytes_per_sample, curr_sample, total_length);
                curr_sample += (insize*def_user_config.channels);
            }
            else
            {
                insize = read_from_file(fp_in, ppin_buffer, bufsize, def_user_config.channels,def_user_config.bytes_per_sample);
            }

            if((insize != 0) && ((int)insize < def_user_config.input_chunk_size))
            {
                int idx, j;
                idx = insize;
                if(def_user_config.bytes_per_sample == TWO_BYTES_PER_SAMPLE)
                {
                    short *pin = (short *)ppin_buffer;
                    for(j=(idx*def_user_config.channels);j<(def_user_config.input_chunk_size*def_user_config.channels);j++)
                    {
                        pin[j] = 0;
                    }
                }
                else
                {
                    int *pin = (int *)ppin_buffer;
                    for(j=(idx*def_user_config.channels);j<(def_user_config.input_chunk_size*def_user_config.channels);j++)
                    {
                        pin[j] = 0;
                    }
                }
                insize = def_user_config.input_chunk_size;
            }

        }       
        
#ifdef NUM_IOSAMPLES_INSYNC
        total_inp_samples += insize;
#endif /* NUM_IOSAMPLES_INSYNC */

#ifdef PROFILE
#ifdef ASRC_ENABLE
        if((def_user_config.enable_pull == XA_SRC_PULL_MODE) && (def_user_config.enable_asrc == 1))
        {
            samples_per_channel = frame_size_for_record;
        }
        else
#endif
        {
            samples_per_channel = insize;
        }
#endif

        if (insize != 0) 
        {
            /* Set number of samples in the input buffer */
#ifdef ASRC_ENABLE

            if((def_user_config.enable_pull == XA_SRC_PULL_MODE) && (def_user_config.enable_asrc == 1))
            {
                err_code = (*p_xa_process_api)(xa_process_handle,
                                           XA_API_CMD_SET_CONFIG_PARAM,
                                           XA_SRC_PP_CONFIG_PARAM_INPUT_CHUNK_SIZE,
                                           &frame_size_for_record);

                _XA_HANDLE_ERROR(p_proc_err_info, "Set Input Chunk Size Error", err_code);
            }
            else
            {
                err_code = (*p_xa_process_api)(xa_process_handle,
                                           XA_API_CMD_SET_CONFIG_PARAM,
                                           XA_SRC_PP_CONFIG_PARAM_INPUT_CHUNK_SIZE,
                                           &insize);

                _XA_HANDLE_ERROR(p_proc_err_info, "Set Input Chunk Size Error", err_code);
            }
#else
            err_code = (*p_xa_process_api)(xa_process_handle,
                                           XA_API_CMD_SET_CONFIG_PARAM,
                                           XA_SRC_PP_CONFIG_PARAM_INPUT_CHUNK_SIZE,
                                           &insize);

            _XA_HANDLE_ERROR(p_proc_err_info, "Set Input Chunk Size Error", err_code);
#endif
        
            /* Set Input Buffer Pointer */
            err_code = (*p_xa_process_api)(xa_process_handle,
                                           XA_API_CMD_SET_CONFIG_PARAM,
                                           XA_SRC_PP_CONFIG_PARAM_SET_INPUT_BUF_PTR,
                                           ppin_buffer);
            _XA_HANDLE_ERROR(p_proc_err_info, "Set Input Buffer Pointer Error", err_code);
        }
        else 
        {
            /* Notify input is over, so previous output samples in intermediate SRC buffers
             * will be flushed to output */
            err_code = (*p_xa_process_api)(xa_process_handle,
                                           XA_API_CMD_INPUT_OVER,
                                           0,
                                           NULL);
            _XA_HANDLE_ERROR(p_proc_err_info, "Input-over flag set Error", err_code);
        }

        /* Set Output Buffer Pointer */
        err_code = (*p_xa_process_api)(xa_process_handle,
                                       XA_API_CMD_SET_CONFIG_PARAM,
                                       XA_SRC_PP_CONFIG_PARAM_SET_OUTPUT_BUF_PTR,
                                       ppout_buffer);
        _XA_HANDLE_ERROR(p_proc_err_info, "Set Output Buffer Pointer Error", err_code);

#ifdef SCRATCH_TRASH_TEST
        memset(p_scratch, 0xca, scratch_size);
#endif /* #ifdef SCRATCH_TRASH_TEST */

#if PROFILE
        xt_iss_client_command("all", "enable");
        times(&start);
#endif
        /* Execute the Sample Rate Converter */
        err_code = (*p_xa_process_api)(xa_process_handle,
                                       XA_API_CMD_EXECUTE,
                                       XA_CMD_TYPE_DO_EXECUTE,
                                       NULL);
        _XA_HANDLE_ERROR(p_proc_err_info, "Exec Error", err_code);
#if PROFILE
        times(&stop);
        xt_iss_client_command("all", "disable");
#endif
        ++frame_count;

#ifdef DISPLAY_MESSAGE_ASRC
        /* Get the number of remaining fraction of number of input samples  */
        err_code = (*p_xa_process_api)(xa_process_handle,
                                       XA_API_CMD_GET_CONFIG_PARAM,
                                       XA_SRC_PP_CONFIG_PARAM_GET_DRIFT_FRACT_ASRC,
                                       &pre_Fm);
        _XA_HANDLE_ERROR(p_proc_err_info, "Get Remaining ASRC drift fraction Error", err_code);

        {
            double  pre_Fm_double = ( double)pre_Fm / ( (long long)1<<32); 

            printf("\n Remaining/unconsumed ASRC drift fraction(in no of Input samples):--> %lf  \n", pre_Fm_double);
        }  
#endif /* #ifdef DISPLAY_MESSAGE_ASRC */
        /* Get the number of samples in the output buffer */
        err_code = (*p_xa_process_api)(xa_process_handle,
                                       XA_API_CMD_GET_CONFIG_PARAM,
                                       XA_SRC_PP_CONFIG_PARAM_OUTPUT_CHUNK_SIZE,
                                       &outsize);
        _XA_HANDLE_ERROR(p_proc_err_info, "Get Output Chunk Size Error", err_code);

#ifdef ASRC_ENABLE
        if ( (def_user_config.enable_pull == XA_SRC_PULL_MODE) && (def_user_config.enable_asrc == 1) )  // Pull Model : record mode : variable input , fixed output
        {   
            int consumed_inp_samples, consumed_inputs;
            err_code = (*p_xa_process_api)(xa_process_handle,
                                            XA_API_CMD_GET_CONFIG_PARAM,
                                            XA_SRC_PP_CONFIG_PARAM_GET_ASRC_RETURN,
                                            &consumed_inp_samples);
            _XA_HANDLE_ERROR(p_proc_err_info, "Get ASRC Return Error", err_code);

            consumed_inputs = consumed_inp_samples; 
            bufsize = consumed_inputs;   

#ifdef ASRC_DRIFT_DBG
            printf("\n consumed inp sample = %d \n", consumed_inputs );                    
#endif //ASRC_DRIFT_DBG

#ifdef ASRC_ENABLE_RUNTIME_DRIFT_TEST
            total_consumed_samples += consumed_inputs;   
            total_generated_samples += outsize;

            tot_drift_to_all_inp += ( drift_float_runtime * consumed_inputs );
#endif /* #ifdef ASRC_ENABLE_RUNTIME_DRIFT_TEST */

            if((consumed_inp_samples != 0) && (insize != 0))
            {
                
                error_jitetr_ret =  JittBufMove(  &jitter_states, ( consumed_inputs * def_user_config.channels), def_user_config.bytes_per_sample );
    
                if( error_jitetr_ret != 0 )
                {
                    printf("\n Jitter Buffer mem move Failed \n");
                    return 0;        
                } 
            }
        }
        
        if ( ((def_user_config.enable_pull == XA_SRC_PUSH_MODE) || (def_user_config.enable_pull == XA_SRC_PULL_MODE)) && ( outsize != 0) && (def_user_config.enable_asrc == 1)) // Push model  Playback mode : fixed input, variable output
        {
            error_jitetr_ret =  JittBufWrite( &jitter_states_output, ppout_buffer, outsize, def_user_config.bytes_per_sample, def_user_config.channels );
            if( error_jitetr_ret != 0 )
            {
                printf("\n Jitter Buffer mem write Failed \n");
                return 0;        
            } 

#ifdef ASRC_DRIFT_DBG
            printf("\n generated outputs = %d \n", outsize ); 
#endif // #ifdef ASRC_DRIFT_DBG

#ifdef ASRC_ENABLE_RUNTIME_DRIFT_TEST
            total_consumed_samples += def_user_config.input_chunk_size;//xa_src_config_struct.cur_chunk_size;   
            total_generated_samples += outsize;

            tot_drift_to_all_inp += ( drift_float_runtime * def_user_config.input_chunk_size /*xa_src_config_struct.cur_chunk_size*/ );
#endif /* #ifdef ASRC_ENABLE_RUNTIME_DRIFT_TEST */
        }
#endif

#ifdef ASRC_ENABLE
        /* Write to file */
        if ( (((def_user_config.enable_pull == XA_SRC_PUSH_MODE)  && ( outsize != 0)) || ((def_user_config.enable_pull == XA_SRC_PULL_MODE) && (insize != 0))) && (def_user_config.enable_asrc == 1)) // Push model  Playback mode : fixed input, variable output
        {
			pVOID ppout_buffer_tmp;//[1];
            if ( def_user_config.bytes_per_sample == TWO_BYTES_PER_SAMPLE)
            {
                short *pOut_tmp = ( short *)jitter_states_output.pJittBuf;
                ppout_buffer_tmp = (void *)pOut_tmp;                            
            }
            else
            {
                int *pOut_tmp = ( int *)jitter_states_output.pJittBuf;
                ppout_buffer_tmp = (void *)pOut_tmp;                            

           }
            
            if(def_user_config.enable_pull == XA_SRC_PULL_MODE)
            {
                outsize = out_frame_size_pull_mode;
            }

            write_to_file(fp_out, ppout_buffer_tmp, outsize, def_user_config.channels, def_user_config.bytes_per_sample); 

            error_jitetr_ret =  JittBufMove(  &jitter_states_output, ( outsize * def_user_config.channels), def_user_config.bytes_per_sample );            

            if( error_jitetr_ret != 0 )
            {
                printf("\n Jitter Buffer mem move Failed \n");
                return 0;        
            } 
        }
        else
#endif
        {
            write_to_file(fp_out, ppout_buffer, outsize, def_user_config.channels, def_user_config.bytes_per_sample);
        }
#ifdef NUM_IOSAMPLES_INSYNC
        total_out_samples += outsize;
#endif /* NUM_IOSAMPLES_INSYNC */

        /* Break from here if input is finished */ 
        if (insize == 0)    
        break;

/* Check the ratio of number of in/out samples only for SRC_FRIO lib build */
#ifdef NUM_IOSAMPLES_INSYNC
        /* Calculate expected total number of output samples 
              and compare them with the actual total         */
        {
            int exp_out;
            exp_out = ROUND((double)total_inp_samples*def_user_config.fs_out/def_user_config.fs_in);
            if ((abs(exp_out-total_out_samples) > 1) )
            {
                fprintf(stderr,"IO sync lost!!! \n");
                exit(1);
            }
        }
#endif /* NUM_IOSAMPLES_INSYNC */

#if PROFILE
        {
            clock_t cycles = stop.tms_utime - start.tms_utime;
            double Curr = ((double) cycles / samples_per_channel * def_user_config.fs_in / (1000000));
            unsigned long long total_msec;
            int msec;
            unsigned long long total_seconds;
            int seconds;
            int minutes;
            Sum += Curr;
            Ave = Sum / (frame_count);

            if (Peak < Curr)
            {
                Peak = Curr;
                Peak_frame = (frame_count);
            }
            total_samples += samples_per_channel;


            total_msec =
                (unsigned long long)((double)total_samples / def_user_config.fs_in * 1000.0);
            msec = (int)(total_msec % 1000);
            total_seconds = total_msec / 1000;
            seconds = (int)(total_seconds % 60);
            minutes = (int)(total_seconds / 60);

            fprintf(stdout, "[%d|%u] %d:%02d.%03d MCPS: %.2f Average: %.2f Peak: %.2f @ [%d]\n",
                    frame_count, bufsize*(frame_count-1), minutes, seconds, msec,
                    Curr, Ave, Peak, Peak_frame);
        }
#else
        printf("Processed %d frames\r",frame_count);
#endif

        /* If reset is desired do reset */
        if(def_user_config.reset == frame_count)
        {
            err_code = (*p_xa_process_api)(xa_process_handle,
                                           XA_API_CMD_EXECUTE,
                                           XA_CMD_TYPE_DO_RUNTIME_INIT,
                                           NULL);
            _XA_HANDLE_ERROR(p_proc_err_info, "Runtime init/ Reset ", err_code);

#ifdef ASRC_ENABLE
            if(def_user_config.enable_asrc == 1)
            {
                if(def_user_config.enable_pull == 1)
                {
                    error_jitetr_ret = JittBufInit( &jitter_states, jitter_states.pJittBuf, inp_jit_buffer_length,  def_user_config.bytes_per_sample, def_user_config.channels, frame_size_for_record );
                    if( error_jitetr_ret != 0 )
                    {
                        printf("\n Jitter Buffer Initialization Failed \n");
                        return 0;
                    }
                }

                error_jitetr_ret = JittBufInit( &jitter_states_output, jitter_states_output.pJittBuf, out_jit_buffer_length,  def_user_config.bytes_per_sample, def_user_config.channels, jit_buffer_lvl);
                if( error_jitetr_ret != 0 )
                {
                    printf("\n Jitter Buffer Initialization Failed \n");
                    return 0;        
                }
            }
#endif

            fprintf(stderr, "Reset is processed\n");

            def_user_config.reset = -1;

#ifdef NUM_IOSAMPLES_INSYNC
            /* Reset IO-sync equation after runtime init */
            total_inp_samples = 0;
            total_out_samples = 0;
#endif /* NUM_IOSAMPLES_INSYNC */

        }
#ifdef OUTPUT_PING_PONG
        if((frame_count&1) == 0)
        {
            /* Set the buffer pointer */
            {
                err_code = (*p_xa_process_api)(xa_process_handle,
                                            XA_API_CMD_SET_MEM_PTR,
                                            out_idx,
                                            p_output_buffer_ping);
                _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);
                ppout_buffer = p_output_buffer_ping;
            }
        }
        else
        {
            /* Set the buffer pointer */
            {
                err_code = (*p_xa_process_api)(xa_process_handle,
                                            XA_API_CMD_SET_MEM_PTR,
                                            out_idx,
                                            p_output_buffer_pong);
                _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);
                ppout_buffer = p_output_buffer_pong;
            }
        }
#endif /* #ifdef OUTPUT_PING_PONG */

#ifdef INPUT_PING_PONG
        if((frame_count&1) == 0)
        {
            /* Set the buffer pointer */
            {
                err_code = (*p_xa_process_api)(xa_process_handle,
                                            XA_API_CMD_SET_MEM_PTR,
                                            in_idx,
                                            p_input_buffer_ping);
                _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);
                ppin_buffer = p_input_buffer_ping;
            }
        }
        else
        {
            /* Set the buffer pointer */
            {
                err_code = (*p_xa_process_api)(xa_process_handle,
                                            XA_API_CMD_SET_MEM_PTR,
                                            in_idx,
                                            p_input_buffer_pong);
                _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);
                ppin_buffer = p_input_buffer_pong;
            }
        }
#endif /* #ifdef INPUT_PING_PONG */
       
    }


#ifdef ASRC_ENABLE_RUNTIME_DRIFT_TEST
    {        
        double tot_drift_to_asrc_inp;
        long long expected_out;
        double total_outputs_without_drift = ((double)def_user_config.fs_out * total_consumed_samples )/def_user_config.fs_in;        
        if ( def_user_config.enable_pull == XA_SRC_PULL_MODE ) /* record mode */
        {
            tot_drift_to_asrc_inp = ( tot_drift_to_all_inp * (double)def_user_config.fs_out ) / def_user_config.fs_in;
        }
        else
        {
            tot_drift_to_asrc_inp = ( tot_drift_to_all_inp * (double)def_user_config.fs_in ) / def_user_config.fs_out;
        }
        expected_out = (long long)( total_outputs_without_drift - tot_drift_to_asrc_inp );
        printf("\n generated output samples = %lld \n", total_generated_samples );
        printf("\n expected  output samples = %lld \n", expected_out );
    }
#endif /* #ifdef ASRC_ENABLE_RUNTIME_DRIFT_TEST */


    if(is_wave)
        write_pcm_length(fp_out);

    fclose(fp_in);
    fclose(fp_out);
#ifdef CS_DRIFT_FROM_FILE
    fclose(fp_side_info);
#endif

#ifdef OUTPUT_PING_PONG  
    free(p_output_buffer_ping);
    free(p_output_buffer_pong);
#endif

#ifdef INPUT_PING_PONG
    free(p_input_buffer_ping);
    free(p_input_buffer_pong);
#endif

#ifdef SCRATCH_TRASH_TEST
    free(p_scratch);
#endif

#if PROFILE
    fprintf(stdout,"\nPeak MCPS = %f\n", Peak);
    fprintf(stdout,"Peak frame = %d\n", Peak_frame);
    fprintf(stdout,"Average MCPS = %f\n", Ave);
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

int main(int argc, char **argv)
{
    FILE *param_file_id;
    WORD32 err_code = XA_NO_ERROR;

    WORD8 curr_cmd[XA_MAX_CMD_LINE_LENGTH];

    WORD32 fargc, curpos;
    WORD32 processcmd = 0;

    char fargv[XA_MAX_ARGS][XA_MAX_CMD_LINE_LENGTH];

    char *pargv[XA_MAX_ARGS];

    // NOTE: set_wbna() should be called before any other dynamic
    // adjustment of the region attributes for cache.
    set_wbna(&argc, argv);

    xa_testbench_error_handler_init();
    binName = argv[0];

    if (argc > 1)
    {
        if (0 == strcmp(argv[1], "-h") || 0 == strcmp(argv[1], "-help"))
        {
            print_usage();
            exit(1);
        }
    }

    if(argc == 1)
    {
        /* Use param file */
        param_file_id = fopen(PARAMFILE, "r");
        if(param_file_id == NULL)
        {
            err_code = XA_TESTBENCH_MFMAN_FATAL_FILE_OPEN_FAILED;
            xa_error_handler(&xa_testbench_error_info, "Parameter File (" PARAMFILE ")" , err_code);
            exit(1);
        }

        /* Process one line at a time */
        while(fgets((char *)curr_cmd, XA_MAX_CMD_LINE_LENGTH, param_file_id))
        {
            curpos = 0;
            fargc = 0;

            if(curr_cmd[0] != '@' && !processcmd)
            {
                /* skip it */
                continue;
            }

            while(sscanf((const char *)(curr_cmd + curpos), "%s", fargv[fargc]) != EOF) {
                if(fargv[0][0]=='/' && fargv[0][1]=='/')
                    break;
                if(strcmp(fargv[0], "@echo") == 0)
                    break;
                if(strcmp(fargv[fargc], "@New_line") == 0) {
                    fgets((char *)(curr_cmd + curpos), XA_MAX_CMD_LINE_LENGTH, param_file_id);
                    continue;
                }
                curpos += strlen(fargv[fargc]);
                while(*(curr_cmd + curpos)==' ' || *(curr_cmd + curpos)=='\t')
                    curpos++;
                fargc++;
            }

            if(fargc < 1)  /* for blank lines etc. */
                continue;

            if(strcmp(fargv[0], "@Output_path") == 0) {
                if(fargc > 1) strcpy((char *)pb_output_file_path, fargv[1]);
                else strcpy((char *)pb_output_file_path, "");
                continue;
            }

            if(strcmp(fargv[0], "@Input_path") == 0) {
                if(fargc > 1) strcpy((char *)pb_input_file_path, fargv[1]);
                else strcpy((char *)pb_input_file_path, "");
                continue;
            }

            if(strcmp(fargv[0], "@Start") == 0) {
                processcmd = 1;
                continue;
            }

            if(strcmp(fargv[0], "@Stop") == 0) {
                processcmd = 0;
                continue;
            }

            /* otherwise if this a normal command and its enabled for execution */
            if(processcmd) {
                int i;

                for(i = 0; i < fargc; i++) {
                    fprintf(stdout, "%s ", fargv[i]);
                    pargv[i] = fargv[i];
                }
                g_w_malloc_count = 0;

                fprintf(stdout, "\n");

                if(err_code == XA_NO_ERROR)
                    xa_src_pp_main_process(fargc, pargv);

                for(i = 0; i < g_w_malloc_count; i++) {
                    if(g_pv_arr_alloc_memory[i]) {
                        free(g_pv_arr_alloc_memory[i]);
                        g_pv_arr_alloc_memory[i] = NULL;
                    }
                }

            }
        }
        fclose( param_file_id );
    }
    else
    {
        int i;

        for(i = 1; i < argc; i++)
        {
            fprintf(stdout, "%s ", argv[i]);
        }

        g_w_malloc_count = 0;

        fprintf(stdout, "\n");

        if(err_code == XA_NO_ERROR)
        {
            /* Exclude binary name. */
            argv++;
            argc--;

            xa_src_pp_main_process(argc, argv);
        }

        for(i = 0; i < g_w_malloc_count; i++)
        {
            if(g_pv_arr_alloc_memory[i])
            {
                free(g_pv_arr_alloc_memory[i]);
                g_pv_arr_alloc_memory[i] = NULL;
            }
        }
    }

    return 0;
}
