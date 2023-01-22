/*
 * Copyright (c) 2006-2020 Cadence Design Systems, Inc.
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


#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <vorbis_dec/xa_vorbis_dec_api.h>
#include <xa_type_def.h>
#include <xa_error_standards.h>
#include <xa_apicmd_standards.h>
#include <xa_error_handler.h>
#include <xa_memory_standards.h>

#ifdef __XCC__
#include <xtensa/hal.h>
#endif

//#define INPUT_PING_PONG
//#define OUTPUT_PING_PONG
//#define SCRATCH_PING_PONG

//#define STEP_PLAY 1
#ifndef STEP_PLAY 
#define STEP_PLAY 0
#endif /* STEP_PLAY */

#ifdef SCRATCH_PING_PONG
#define SCRATCH_TEST(p_scratch_test, pattern) \
    WORD32 nm; \
    pUWORD8 pnt = (pUWORD8)p_scratch_test; \
    for(nm = 0; nm < ui_scr_size; nm++) \
    { \
        if(pnt[nm] != (UWORD8)pattern) \
        { \
            scr_ping_pong_fail = 1; \
            fprintf(stderr, "Scratch Ping Pong Failed\n"); \
            break; \
        } \
    } \
    if(scr_ping_pong_fail == 1) \
        break;
#endif

#if STEP_PLAY
    /* Play PLAY_TIME_MS milliseconds of audio, mute the output for
    MUTE_TIME_MS milliseconds, skip SKIP_TIME_MS milliseconds of audio,
    repeat. */
    #define PLAY_TIME_MS 800  /* ms */
    #define MUTE_TIME_MS 100  /* ms */
    #define SKIP_TIME_MS  50  /* ms */

    #define STEP_AT_NUM_FRAME_OFFSET    -1
    #define NEW_OFFSET_POSITION_IN_BYTES 1234

    int play_time_ms = 0;
    int mute_bytes = 0;
    int flush_frames_counter = 0;
    int file_end = -1;
#endif /* STEP_PLAY */

//#define SMALL_INPUT_CHUNK_FEED  /* SMALL_INPUT_CHUNK_FEED Enabler */

#ifdef SMALL_INPUT_CHUNK_FEED
    #define CHUNK_SIZE 78
    int varChunkSize = CHUNK_SIZE;
    int errFlagSmallChunk = 0;      /* Flag for printing the insufficient data error only once. */
#endif // SMALL_INPUT_CHUNK_FEED

VOID xa_vorbis_dec_error_handler_init();
VOID xa_testbench_error_handler_init();

extern xa_error_info_struct xa_vorbis_dec_error_info;
extern xa_error_info_struct xa_testbench_error_info;

#if !defined(DISPLAY_MESSAGE)
    #define DISPLAY_MESSAGE 1
#endif  /* DISPLAY_MESSAGE */

#if !defined(DISPLAY_COMMENT)
    #define DISPLAY_COMMENT 1
#endif  /* DISPLAY_COMMENT */

#if defined(__PROFILE__) && !defined __XCC__
    #undef __PROFILE__
#endif

#ifdef __PROFILE__
    #include <xtensa/sim.h>
    #include <sys/times.h>
#endif

/*****************************************************************************/
/* Constant hash defines                                                     */
/*****************************************************************************/
#define MAX_STACK_PROC 10
#define MAX_MEM_ALLOCS 100
#define XA_MAX_CMD_LINE_LENGTH 300
#define XA_MAX_ARGS 20
#define XA_SCREEN_WIDTH 80
#define PARAMFILE "parameter.txt"

/*****************************************************************************/
/* Error codes for the testbench                                             */
/*****************************************************************************/
#define XA_TESTBENCH_MFMAN_FATAL_MEM_ALLOC_FAILED               0xffff8000
#define XA_TESTBENCH_MFMAN_FATAL_FILE_OPEN_FAILED               0xffff8001

#ifdef __PROFILE__
/* The CPU_LOAD_WINDOW specifies the time period before the CPU load
   is calculated. A 20 ms window is less than a full output buffer for
   a stereo stream encoded at the highest sample rate (48 kHz). */
#define CPU_LOAD_WINDOW 0.02 /* seconds */
#endif

/*****************************************************************************/
/* Global variables                                                          */
/*****************************************************************************/
pVOID   g_pv_arr_alloc_memory[MAX_MEM_ALLOCS];
WORD32  g_w_malloc_count;
FILE    *g_pf_inp=0, *g_pf_out=0;
WORD32  raw_vorbis_file_flag =0;
WORD32  raw_vorbis_file_last_pkt_gran_pos = -1;
WORD32  ogg_max_page_size = 12;  /* Library default */
WORD32  runtime_mem = 0;    /* Library default, corresponds to 4k blocksize */

#if DISPLAY_MESSAGE
/*******************************************************************************
 *                                                                           
 *  Function name : xa_display_id_message                                    
 *                                                                           
 *  Description   : Display the ID message of the process                   
 *                                                                           
 *  parameters    : WORD8 lib_name[] (library name)                          
 *                  WORD8 lib_version[] (library version)                    
 *                  WORD8 api_version[] (API version)                        
 *                                                                          
 *  Return value  : none                                                     
 ******************************************************************************/

VOID xa_display_id_message(WORD8 lib_name[], WORD8 lib_version[])
{
    WORD8 str[4][XA_SCREEN_WIDTH] = 
    {
        "TENSILICA, INC.\n",
        "http://www.tensilica.com\n",
        "",
        ""
    };
    WORD8 spaces[XA_SCREEN_WIDTH/2 + 1];
    WORD32 i, spclen;

    strcpy((char*)str[2], (char*)lib_name);
    strcat(strcat((char*)str[2], " version: "), (char*)lib_version);
    strcat((char*)str[2], "\n");
    strcat((char*)str[4-1], "\n");

    for(i = 0; i < XA_SCREEN_WIDTH/2 + 1; i++) 
    {
        spaces[i] = ' ';
    }

    for(i = 0; i < 4; i++) 
    {
        spclen = XA_SCREEN_WIDTH/2 - strlen((char*)str[i])/2;
        spaces[spclen] = '\0';
        printf("%s", (char*)spaces);
        spaces[spclen] = ' ';
        printf("%s", (char*)str[i]);
    }
}
#endif /* DISPLAY_MESSAGE */

/* Function to print help */
static void print_usage( char* argv[] )
{
    printf( "Usage: %s [options] -ifile:<input_file> -ofile:<output_file>\n", argv[0] );
    printf( "Options:\n" );
    printf( "-r                         : Set input stream type to raw vorbis, default type: ogg vorbis\n" );
    printf( "-g<granule_position>       : Last packet granule position; default: -1\n" );
    printf( "-ogg_maxpage:<n>           : max page size - n in kBytes, Min: 12, Max: 128; default: 12\n" );
    printf( "-rtmem:<n>                 : Runtime mem flag - n, Min: 0, Max: 1; default: 0\n" );
    printf( "                             0 - Runtime memory allocation for blocksize of 4k\n" );
    printf( "                             1 - Runtime memory allocation for blocksize of 8k (worst case)\n" );
#ifdef SMALL_INPUT_CHUNK_FEED
    printf( "-inSize<chunk_size>:       : Input chunk size for small input chunk test, Min:8, Max: Input buffer size; default: 78 \n" );
#endif //#ifdef SMALL_INPUT_CHUNK_FEED
}

#if DISPLAY_COMMENT

#define READ_LE_WORD32(ptr) \
       ((WORD32)(*((UWORD8 *)(ptr))) | \
    (WORD32)(*(((UWORD8 *)(ptr)) + 1) << 8) | \
    (WORD32)(*(((UWORD8 *)(ptr)) + 2) << 16) | \
    (WORD32)(*(((UWORD8 *)(ptr)) + 3) << 24))

VOID xa_display_comment (pWORD8 pb_comment_mem, UWORD32 ui_comment_mem_size)
{
    UWORD32 ui_loop_counter;
    UWORD32 ui_comment_mem_offset = 0;
    WORD32 i_no_of_usr_comments;
    UWORD32  ui_str_length;

    /*************************************************************************/
    /*  The comment buffer contains the raw comment data extracted from      */
    /*  comment Header.  The comment buffer format is as per below           */
    /*  specification.                                                       */
    /*  1) [vendor_length] = read an unsigned integer of 32 bits               */
    /*  2) [vendor_string] = read a UTF-8 vector as [vendor_length] octets   */
    /*  3) [user_comment_list_length] = read an unsigned integer of 32 bits  */
    /*  4) iterate [user_comment_list_length] times {                        */
    /*     5) [length] = read an unsigned integer of 32 bits                 */
    /*     6) this iteration's user comment = read a UTF-8 vector as [length]*/
    /*           octets                                                      */
    /*     }                                                                 */
    /*************************************************************************/  

    printf("\n");

    ui_comment_mem_offset = 0;
    if (ui_comment_mem_size < 4)
        return;

    ui_str_length = READ_LE_WORD32(&pb_comment_mem[ui_comment_mem_offset]);
    if (ui_str_length == 0xffffffff) 
    {
        printf("Warning: Unable to get vendor string and user comment fields.\n");
        return;
    }

    ui_comment_mem_offset += 4;
    if (ui_comment_mem_size <= (ui_str_length + ui_comment_mem_offset)) 
    {
        return;
    }

    printf("Vendor string: ");
    for (ui_loop_counter = 0; ui_str_length--; ui_loop_counter++) 
    {
        printf("%c", pb_comment_mem[ui_comment_mem_offset++]);
    }
    printf("\n");

    if (ui_comment_mem_size <= (ui_comment_mem_offset + 4)) 
    {
        return;
    }

    i_no_of_usr_comments = READ_LE_WORD32(&pb_comment_mem[ui_comment_mem_offset]);
    ui_comment_mem_offset += 4;
    while (i_no_of_usr_comments--) 
    {
        if (ui_comment_mem_size <= (ui_comment_mem_offset + 4)) 
        {
            break;
        }

        ui_str_length = READ_LE_WORD32(&pb_comment_mem[ui_comment_mem_offset]);
        ui_comment_mem_offset += 4;

        if (ui_comment_mem_size <= (ui_comment_mem_offset + ui_str_length)) 
        {
            break;
        }

        printf("User comment: ");
        
        for (ui_loop_counter = 0; ui_str_length--; ui_loop_counter++) 
        {
            printf("%c", pb_comment_mem[ui_comment_mem_offset++]);
        }
        printf("\n");
    }
    printf("\n");
}
#endif

/*******************************************************************************
 *                                                                           
 *  Function name : xa_fwrite                                      
 *                                                                           
 *  Description   : write little endian data irrespective of the endiannness 
 *                  of the data                                                 
 *                                                                           
 *  parameters    : pVOID buffer[]                                           
 *                  int size                                                 
 *                  int nwords                                               
 *                  FILE *fp                            
 *                                                                          
 *  Return value  : none                                                     
 ******************************************************************************/
#ifdef __XTENSA_EL__
void xa_fwrite(void *buffer,
      int   size,
      int   nwords,
      FILE *fp) 
{
    int i, j;
    char *pb_buf = (char *) buffer;

    for (i = 0; i < nwords; i++) 
    {
        for (j = 0; j < size; j++) 
        {
            putc(pb_buf[i*size+j], fp);
        }
    }
}
#else
void xa_fwrite(void *buffer,
      int   size,
      int   nwords,
      FILE *fp) 
{
    int i, k;
    char *pb_buf = (char *) buffer;

    for (i = 0; i < nwords; i++) 
    {
        for (k = size - 1; k >= 0; k--) 
        {
            putc(pb_buf[i*size+k], fp);
        }
    }
}
#endif

/*******************************************************************************
 *                                                                           
 *  Function name : xa_ogg_dec_main_process                                    
 *                                                                           
 *  Description   : Processing of multiple components                
 *                  Loop1: Set params + Mem alloc                            
 *                  Loop2: Set params + Init process + Get params            
 *                  Loop3: Execute                                                  
 *                                                                           
 *  parameters    : WORD32 argc (Arguments count)                            
 *                  pWORD8 argv[] (Argument strings)                          
 *                                                                          
 *  Return value  : none                                                     
 ******************************************************************************/
XA_ERRORCODE xa_ogg_dec_main_process(int  argc, char *argv[]) 
{
#if DISPLAY_MESSAGE
    /* Library Info and Identification strings */
    WORD8 pb_lib_name[XA_SCREEN_WIDTH]    = "";
    WORD8 pb_lib_version[XA_SCREEN_WIDTH] = "";
    WORD8 pb_api_version[XA_SCREEN_WIDTH] = "";
#endif

#if DISPLAY_COMMENT
    UWORD32 ui_comment_mem_size;
    pWORD8  pb_comment_mem;
#endif

    /* Error code */
    XA_ERRORCODE err_code = XA_NO_ERROR;

    pVOID p_xa_process_api_obj;

    /* First part                                        */
    /* Error Handler Init                                */
    /* Get Library Name, Library Version and API Version */
    /* Initialize API structure + Default config set     */
    /* Set config params from user                       */
    /* Initialize memory tables                          */
    /* Get memory information and allocate memory        */
    UWORD32 ui_api_size;
    UWORD32 ui_proc_mem_tabs_size;
    UWORD32 ui_no_of_mem_table;
    UWORD32 ui_loop_counter;
    UWORD32 ui_inp_size = 0;
    WORD32  i_init_done;
    UWORD32 ui_samp_freq;
    UWORD32 ui_num_chan;
    UWORD32 ui_pcm_wd_sz;
    WORD32  i_decode_done; 
    pUWORD8 pb_inp_buf = NULL;
#ifdef INPUT_PING_PONG
    pUWORD8 pb_inp_buf_ping = NULL;
    pUWORD8 pb_inp_buf_pong = NULL;
#endif
    WORD32  i_buff_size;
    UWORD32 ui_bytes_consumed;
    WORD32  i_bytes_read;
    UWORD32 ui_op_bytes;
    pWORD8  pb_out_buf= NULL;
#ifdef OUTPUT_PING_PONG
    pWORD8  pb_out_buf_ping = NULL;
    pWORD8  pb_out_buf_pong = NULL;
#endif
#ifdef SCRATCH_PING_PONG
    pWORD8  pb_scr_buf = NULL;
    pWORD8  pb_scr_buf_ping = NULL;
    pWORD8  pb_scr_buf_pong = NULL;
    WORD32  ui_scr_size;
#endif
    XA_ERRORCODE exec_ret_val;
    int numFrames =0;

    /* Stream position variables */
    int decoded_frame_strm_pos=-1;

  
#ifdef __PROFILE__
    struct tms start, stop;
    clock_t cycles = 0;
    FLOAT64 mcps_peak = 0, mcps_curr = 0, mcps_ave = 0, mcps_sum = 0;
    WORD32 peak_frame = 0, frame = 1;
    WORD32 samples=0;
    WORD32 avg=0;
#endif
  

#if STEP_PLAY // RUNTIME INIT
    int input_over = 0;
#endif //#if STEP_PLAY // RUNTIME INIT

    xa_codec_func_t *p_xa_process_api = xa_vorbis_dec;

    /* The process error info structure */
    xa_error_info_struct *p_proc_err_info;

    p_proc_err_info = &xa_vorbis_dec_error_info;

    g_w_malloc_count=0;
    /* ******************************************************************/
    /* Initialize the error handler                                     */
    /* ******************************************************************/
    xa_vorbis_dec_error_handler_init();

    /* ******************************************************************/
    /* Get the library name, library version and API version            */
    /* ******************************************************************/

#if DISPLAY_MESSAGE

    /* Get information about the library and API */
    err_code = (*p_xa_process_api)( 0, XA_API_CMD_GET_LIB_ID_STRINGS, 
                                       XA_CMD_TYPE_LIB_NAME, pb_lib_name);
    _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

    err_code = (*p_xa_process_api)( 0, XA_API_CMD_GET_LIB_ID_STRINGS, 
                               XA_CMD_TYPE_LIB_VERSION, pb_lib_version);
    _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

    err_code = (*p_xa_process_api)( 0, XA_API_CMD_GET_LIB_ID_STRINGS, 
                               XA_CMD_TYPE_API_VERSION, pb_api_version);
    _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);    

    /* Display the Tensilica identification message */
    xa_display_id_message(pb_lib_name, pb_lib_version);
   
#endif

    /* ******************************************************************/
    /* Initialize API structure and set config params to default        */
    /* ******************************************************************/
    err_code = (*p_xa_process_api)( 0, XA_API_CMD_GET_API_SIZE, 0, &ui_api_size);
    _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

    /* Allocate memory for API */
    g_pv_arr_alloc_memory[g_w_malloc_count] = malloc(ui_api_size);

    if (NULL == g_pv_arr_alloc_memory[g_w_malloc_count]) 
    {
        _XA_HANDLE_ERROR(&xa_testbench_error_info, "API struct alloc",
            XA_TESTBENCH_MFMAN_FATAL_MEM_ALLOC_FAILED);
    }

    /* Memset API object to zero */
    memset(g_pv_arr_alloc_memory[g_w_malloc_count], 0, ui_api_size);
    
    /* Set API object with the memory allocated, API object requires 4 bytes (WORD32) alignment*/
    p_xa_process_api_obj = (pVOID) g_pv_arr_alloc_memory[g_w_malloc_count];

    g_w_malloc_count++;

    /* Set the config params to default values */
    err_code = (*p_xa_process_api)((xa_codec_handle_t)p_xa_process_api_obj,
        XA_API_CMD_INIT,
        XA_CMD_TYPE_INIT_API_PRE_CONFIG_PARAMS,
        NULL
    );
    _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

#if DISPLAY_COMMENT
    ui_comment_mem_size = 2048;

    err_code = (*p_xa_process_api)((xa_codec_handle_t)p_xa_process_api_obj, 
        XA_API_CMD_SET_CONFIG_PARAM, 
        XA_VORBISDEC_CONFIG_PARAM_COMMENT_MEM_SIZE, 
        &ui_comment_mem_size
    );
    _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);                               

    g_pv_arr_alloc_memory[g_w_malloc_count] = malloc(ui_comment_mem_size);

    if(NULL == g_pv_arr_alloc_memory[g_w_malloc_count]) 
    {
        _XA_HANDLE_ERROR(&xa_testbench_error_info, "Comment Memory alloc",
            XA_TESTBENCH_MFMAN_FATAL_MEM_ALLOC_FAILED);
    }

    pb_comment_mem = (pWORD8) g_pv_arr_alloc_memory[g_w_malloc_count];
    g_w_malloc_count++;                               
                               
    err_code = (*p_xa_process_api)((xa_codec_handle_t)p_xa_process_api_obj, 
        XA_API_CMD_SET_CONFIG_PARAM, 
        XA_VORBISDEC_CONFIG_PARAM_COMMENT_MEM_PTR, 
        (pVOID)pb_comment_mem
    );
    _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);
    memset(pb_comment_mem, 0, ui_comment_mem_size);
#endif                           

    /* ******************************************************************/
    /* Initialize Memory info tables                                    */
    /* ******************************************************************/
    /* Get memory info tables size */
    err_code = (*p_xa_process_api)((xa_codec_handle_t)p_xa_process_api_obj, 
        XA_API_CMD_GET_MEMTABS_SIZE, 
        0, 
        &ui_proc_mem_tabs_size);
        _XA_HANDLE_ERROR(p_proc_err_info, "", err_code
    );

    g_pv_arr_alloc_memory[g_w_malloc_count] = malloc(ui_proc_mem_tabs_size);

    if(NULL == g_pv_arr_alloc_memory[g_w_malloc_count]) 
    {
        _XA_HANDLE_ERROR(&xa_testbench_error_info, "Mem tables alloc",
            XA_TESTBENCH_MFMAN_FATAL_MEM_ALLOC_FAILED);
    }

    /* Memory table requires 4 bytes (WORD32) alignment */
    err_code = (*p_xa_process_api)((xa_codec_handle_t)p_xa_process_api_obj, 
        XA_API_CMD_SET_MEMTABS_PTR, 
        0,
        (pVOID) g_pv_arr_alloc_memory[g_w_malloc_count]);
        _XA_HANDLE_ERROR(p_proc_err_info, "", err_code
    );

    g_w_malloc_count++;

    /* Set raw file mode */
    err_code = (*p_xa_process_api)((xa_codec_handle_t)p_xa_process_api_obj,
        XA_API_CMD_SET_CONFIG_PARAM,
        XA_VORBISDEC_CONFIG_PARAM_RAW_VORBIS_FILE_MODE,
        &raw_vorbis_file_flag);
        _XA_HANDLE_ERROR(p_proc_err_info, "", err_code
    );

    if( raw_vorbis_file_flag == 1)
        printf("Raw vorbis file decode \n");
    else
        printf("Ogg vorbis file decode \n");

    /* Set max page size for ogg vorbis streams */
    err_code = (*p_xa_process_api)((xa_codec_handle_t)p_xa_process_api_obj,
        XA_API_CMD_SET_CONFIG_PARAM,
        XA_VORBISDEC_CONFIG_PARAM_OGG_MAX_PAGE_SIZE,
        &ogg_max_page_size);
    _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

    /* Set runtime mem flag - 0 for max blocksize = 4k, 1 for max blocksize = 8k */
    err_code = (*p_xa_process_api)((xa_codec_handle_t)p_xa_process_api_obj,
        XA_API_CMD_SET_CONFIG_PARAM,
        XA_VORBISDEC_CONFIG_PARAM_RUNTIME_MEM,
        &runtime_mem);
    _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);
    
    /* initialize the API, post config, fill memory tables    */
    err_code = (*p_xa_process_api)((xa_codec_handle_t)p_xa_process_api_obj,
        XA_API_CMD_INIT,
        XA_CMD_TYPE_INIT_API_POST_CONFIG_PARAMS,
        NULL
        );

    _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

    if( raw_vorbis_file_flag == 1)
    {
        /* Set raw file mode */
        err_code = (*p_xa_process_api)((xa_codec_handle_t)p_xa_process_api_obj,
            XA_API_CMD_SET_CONFIG_PARAM,
            XA_VORBISDEC_CONFIG_PARAM_RAW_VORBIS_LAST_PKT_GRANULE_POS,
            &raw_vorbis_file_last_pkt_gran_pos);
            _XA_HANDLE_ERROR(p_proc_err_info, "", err_code
        );

        printf("Raw vorbis file decode : raw_vorbis_file_last_pkt_gran_pos = %d\n", raw_vorbis_file_last_pkt_gran_pos);
    }

    /********************************************************************/
    /* Allocate Memory with info from library                           */
    /********************************************************************/

    /* Get number of memory tables required */                                                               
    err_code = (*p_xa_process_api)((xa_codec_handle_t)p_xa_process_api_obj, 
        XA_API_CMD_GET_N_MEMTABS, 
        0, 
        &ui_no_of_mem_table);
        _XA_HANDLE_ERROR(p_proc_err_info, "", err_code
    );                                 

    for( ui_loop_counter = 0; (WORD32)ui_loop_counter < (WORD32) ui_no_of_mem_table; 
    ui_loop_counter++)
    {
        WORD32 ui_size, ui_alignment, ui_type, ui_priority;
        pVOID pv_alloc_ptr;

        /* Allocate memory to memory buffers */
        /* Get memory size */
        err_code = (*p_xa_process_api)((xa_codec_handle_t)p_xa_process_api_obj, 
            XA_API_CMD_GET_MEM_INFO_SIZE, 
            ui_loop_counter, 
            &ui_size);
            _XA_HANDLE_ERROR(p_proc_err_info, "", err_code
         );                                 

        /* Get memory type */
        err_code = (*p_xa_process_api)((xa_codec_handle_t)p_xa_process_api_obj, 
            XA_API_CMD_GET_MEM_INFO_TYPE, 
            ui_loop_counter, 
            &ui_type);
            _XA_HANDLE_ERROR(p_proc_err_info, "", err_code
        );                                    
    
        /* Get memory alignment  */
        err_code = (*p_xa_process_api)((xa_codec_handle_t)p_xa_process_api_obj, 
            XA_API_CMD_GET_MEM_INFO_ALIGNMENT, 
            ui_loop_counter, 
            &ui_alignment);
            _XA_HANDLE_ERROR(p_proc_err_info, "", err_code
        );                                  

        err_code = (*p_xa_process_api)((xa_codec_handle_t)p_xa_process_api_obj, 
            XA_API_CMD_GET_MEM_INFO_PRIORITY, 
            ui_loop_counter, 
            &ui_priority);
            _XA_HANDLE_ERROR(p_proc_err_info, "", err_code
        );

        g_pv_arr_alloc_memory[g_w_malloc_count] = malloc(ui_size);
    
        if(NULL == g_pv_arr_alloc_memory[g_w_malloc_count]) 
        {
            _XA_HANDLE_ERROR(&xa_testbench_error_info, "Mem tables alloc",
                XA_TESTBENCH_MFMAN_FATAL_MEM_ALLOC_FAILED);
        }

        pv_alloc_ptr = (pVOID) g_pv_arr_alloc_memory[g_w_malloc_count];
        g_w_malloc_count++;

        err_code = (*p_xa_process_api)((xa_codec_handle_t)p_xa_process_api_obj, XA_API_CMD_SET_MEM_PTR, 
            ui_loop_counter, pv_alloc_ptr
        );
        _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);
    
#ifdef SCRATCH_PING_PONG
        if(XA_MEMTYPE_SCRATCH == ui_type) 
        {
            pb_scr_buf_ping = (pWORD8)pv_alloc_ptr;
            ui_scr_size = ui_size;
            memset(pb_scr_buf_ping, 0xbd, ui_scr_size);

            g_pv_arr_alloc_memory[g_w_malloc_count] = malloc(ui_size);
    
            if(NULL == g_pv_arr_alloc_memory[g_w_malloc_count]) 
            {
                _XA_HANDLE_ERROR(&xa_testbench_error_info, "Mem tables alloc",
                    XA_TESTBENCH_MFMAN_FATAL_MEM_ALLOC_FAILED);
            }

            pv_alloc_ptr = (pVOID) g_pv_arr_alloc_memory[g_w_malloc_count];
            g_w_malloc_count++;

            pb_scr_buf_pong = (pWORD8)pv_alloc_ptr;
            ui_scr_size = ui_size;
            memset(pb_scr_buf_pong, 0xbd, ui_scr_size);

            pb_scr_buf = pb_scr_buf_ping;
        }
#endif /* #ifdef SCRATCH_PING_PONG */
        if(XA_MEMTYPE_INPUT == ui_type) 
        {
#ifndef INPUT_PING_PONG
            pb_inp_buf = (pUWORD8)pv_alloc_ptr;
            ui_inp_size = ui_size;
            memset(pb_inp_buf, 0, ui_inp_size);
#else /* #ifndef INPUT_PING_PONG */
            pb_inp_buf_ping = (pUWORD8)pv_alloc_ptr;
            ui_inp_size = ui_size;
            memset(pb_inp_buf_ping, 0, ui_inp_size);

            g_pv_arr_alloc_memory[g_w_malloc_count] = malloc(ui_size);
    
            if(NULL == g_pv_arr_alloc_memory[g_w_malloc_count]) 
            {
                _XA_HANDLE_ERROR(&xa_testbench_error_info, "Mem tables alloc",
                    XA_TESTBENCH_MFMAN_FATAL_MEM_ALLOC_FAILED);
            }

            pv_alloc_ptr = (pVOID) g_pv_arr_alloc_memory[g_w_malloc_count];
            g_w_malloc_count++;

            pb_inp_buf_pong = (pUWORD8)pv_alloc_ptr;
            ui_inp_size = ui_size;
            memset(pb_inp_buf_pong, 0, ui_inp_size);

            pb_inp_buf = pb_inp_buf_ping;
#endif /* #ifndef INPUT_PING_PONG */
        }
        if(XA_MEMTYPE_OUTPUT == ui_type) 
        {
#ifndef OUTPUT_PING_PONG
            pb_out_buf =  (pWORD8)pv_alloc_ptr;
#else /* #ifndef OUTPUT_PING_PONG */
            pb_out_buf_ping =  (pWORD8)pv_alloc_ptr;

            g_pv_arr_alloc_memory[g_w_malloc_count] = malloc(ui_size);

            if(NULL == g_pv_arr_alloc_memory[g_w_malloc_count]) 
            {
                _XA_HANDLE_ERROR(&xa_testbench_error_info, "Mem tables alloc",
                    XA_TESTBENCH_MFMAN_FATAL_MEM_ALLOC_FAILED);
            }

            pv_alloc_ptr = (pVOID) g_pv_arr_alloc_memory[g_w_malloc_count];
            g_w_malloc_count++;

            pb_out_buf_pong = (pWORD8)pv_alloc_ptr;

            pb_out_buf = pb_out_buf_ping;
#endif /* #ifndef OUTPUT_PING_PONG */
        }

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
        }
    }
    /* End first part */


    /* Second part        */
    /* Initialize process */
  
#ifdef SMALL_INPUT_CHUNK_FEED
    errFlagSmallChunk = 0;
#endif // #ifdef SMALL_INPUT_CHUNK_FEED

    i_buff_size = 0;
    ui_bytes_consumed = 0;
    exec_ret_val = XA_NO_ERROR;

    do 
    {
        int bytes2read =0;

        /* Copy unconsumed data at the base portion of the input buffer. */
        i_buff_size  -= ui_bytes_consumed;
        if(ui_bytes_consumed!= 0)
        {
            for(ui_loop_counter = 0; ui_loop_counter < ((UWORD32)i_buff_size); ui_loop_counter++) 
            {
                pb_inp_buf[ui_loop_counter] = pb_inp_buf[ui_loop_counter + ui_bytes_consumed];
            }
        }

#ifdef SMALL_INPUT_CHUNK_FEED

        /* Try and Read at least varChunkSize at time or smaller from the file */
        {
            bytes2read = ((int)ui_bytes_consumed < varChunkSize)? ui_bytes_consumed:varChunkSize;
            bytes2read = (bytes2read==0)? varChunkSize:bytes2read;
            bytes2read = (bytes2read > ((int)ui_inp_size-i_buff_size))?
            (ui_inp_size-i_buff_size ):bytes2read;

            if( raw_vorbis_file_flag == 1)
                bytes2read = ((int)ui_inp_size-i_buff_size);
        }
#else //#ifdef SMALL_INPUT_CHUNK_FEED
        bytes2read = ui_inp_size-i_buff_size ;
#endif // #ifdef SMALL_INPUT_CHUNK_FEED

        /* Read data as per the size of input buffer */
        i_bytes_read = fread(pb_inp_buf + (i_buff_size), 1, bytes2read, g_pf_inp);

        /* New buffer size */
        i_buff_size   += i_bytes_read ;

        /* Set end of input file if i_buff_size is zero. Or if last call to decoder returns with "need more"
        and fileend is reached and i_bytes_read are zero */
        if((i_buff_size <= 0)||
        ((exec_ret_val ==XA_VORBISDEC_EXECUTE_NONFATAL_OV_INSUFFICIENT_DATA) && (feof(g_pf_inp)&&(ui_bytes_consumed ==0) &&  (i_bytes_read ==0)))) 
        {
            /* Tell that the input is over in this buffer */
            err_code = (*p_xa_process_api)((xa_codec_handle_t)p_xa_process_api_obj, 
                XA_API_CMD_INPUT_OVER, 0, 0
            );
            _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);
        }
     
        /* Set number of bytes to be processed */
        err_code = (*p_xa_process_api)((xa_codec_handle_t)p_xa_process_api_obj, 
            XA_API_CMD_SET_INPUT_BYTES, 
            0,
            &i_buff_size
        );
        _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);
      
#ifdef __PROFILE__
        times(&start);
#endif

        /* Initialize the process */
        err_code = (*p_xa_process_api)((xa_codec_handle_t)p_xa_process_api_obj, XA_API_CMD_INIT, 
            XA_CMD_TYPE_INIT_PROCESS, 0
        );

        exec_ret_val = err_code;

#ifdef __PROFILE__
        times(&stop);
        cycles = stop.tms_utime - start.tms_utime;
        printf("Init Cycles: %ld\n", cycles);
        cycles = 0;
#endif

#ifdef SMALL_INPUT_CHUNK_FEED
        /* print error log only once for insuficient data error. */
        if((errFlagSmallChunk == 0) || err_code != XA_VORBISDEC_EXECUTE_NONFATAL_OV_INSUFFICIENT_DATA)
        {
            _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);
        }

        if (err_code == XA_VORBISDEC_EXECUTE_NONFATAL_OV_INSUFFICIENT_DATA)
            errFlagSmallChunk = 1;

#else // #ifdef SMALL_INPUT_CHUNK_FEED
        _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);
#endif // #ifdef SMALL_INPUT_CHUNK_FEED    

        /* Checking for end of initialization */
        err_code = (*p_xa_process_api)((xa_codec_handle_t)p_xa_process_api_obj, XA_API_CMD_INIT, 
            XA_CMD_TYPE_INIT_DONE_QUERY, &i_init_done
        );
        _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

        /* How much buffer is used in input buffers */
        err_code = (*p_xa_process_api)((xa_codec_handle_t)p_xa_process_api_obj, 
            XA_API_CMD_GET_CURIDX_INPUT_BUF,
            0, 
            &ui_bytes_consumed
        );
        _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

    }while(!i_init_done);
    /* Init end of process condition end */
   
#if DISPLAY_COMMENT
    xa_display_comment(pb_comment_mem, ui_comment_mem_size);
#endif

    /* ******************************************************************/
    /* Get config params from API                                       */
    /* ******************************************************************/
    err_code = (*p_xa_process_api)((xa_codec_handle_t)p_xa_process_api_obj, 
        XA_API_CMD_GET_CONFIG_PARAM, 
        XA_VORBISDEC_CONFIG_PARAM_SAMP_FREQ, 
        &ui_samp_freq
    );
    _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

    err_code = (*p_xa_process_api)((xa_codec_handle_t)p_xa_process_api_obj, 
        XA_API_CMD_GET_CONFIG_PARAM, 
        XA_VORBISDEC_CONFIG_PARAM_NUM_CHANNELS, 
        &ui_num_chan
    );
    _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);
  
    err_code = (*p_xa_process_api)((xa_codec_handle_t)p_xa_process_api_obj, 
        XA_API_CMD_GET_CONFIG_PARAM, 
        XA_VORBISDEC_CONFIG_PARAM_PCM_WDSZ, 
        &ui_pcm_wd_sz
    );
    _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

    printf("Channels: %d\n", ui_num_chan);
    printf("Sample Rate: %d Hz\n", ui_samp_freq);
    printf("PCM Sample Size: %d bits\n\n", ui_pcm_wd_sz);
                              
#ifdef SMALL_INPUT_CHUNK_FEED
    errFlagSmallChunk = 0;
#endif // #ifdef SMALL_INPUT_CHUNK_FEED

    /* Third part for executing each process */
    exec_ret_val = XA_NO_ERROR;
    do
    {        
        int bytes2read =0;
#ifdef INPUT_PING_PONG
        if(pb_inp_buf == pb_inp_buf_ping)
        {
            memcpy(pb_inp_buf_pong, pb_inp_buf_ping, ui_inp_size);
            pb_inp_buf = pb_inp_buf_pong;
            memset(pb_inp_buf_ping, 0xca, ui_inp_size);
            printf("Switched ip buf \n");
        }
        else
        {
            memcpy(pb_inp_buf_ping, pb_inp_buf_pong, ui_inp_size);
            pb_inp_buf = pb_inp_buf_ping;
            memset(pb_inp_buf_pong, 0xca, ui_inp_size);
            printf("Switched ip buf \n");
        }
        /* Set input buffer pointer */
        err_code = (*p_xa_process_api)((xa_codec_handle_t)p_xa_process_api_obj, XA_API_CMD_SET_MEM_PTR, 
            XA_MEMTYPE_INPUT, pb_inp_buf
        );
        _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);
#endif /* #ifdef INPUT_PING_PONG */
#if STEP_PLAY // RUNTIME INIT
        if (((play_time_ms >= PLAY_TIME_MS) || (numFrames == STEP_AT_NUM_FRAME_OFFSET)) && (!input_over)
            && ( raw_vorbis_file_flag == 0)
        )
        {
            int skip_bytes = 0;
            int cur_strm_pos = 0;
            int new_stream_pos = 0;
            int bitrate =0;

            play_time_ms = 0;

            err_code = (*p_xa_process_api)((xa_codec_handle_t)p_xa_process_api_obj,
                XA_API_CMD_GET_CONFIG_PARAM,
                XA_VORBISDEC_CONFIG_PARAM_GET_CUR_BITRATE,
                &bitrate
            );
            _XA_HANDLE_ERROR(p_proc_err_info, "Get Bitrate Error", err_code);


            /* Based on the current bit rate, calculate the number of
            bytes that need to be skipped after consuming the bytes
            from the last EXECUTE call. */
            skip_bytes = (int)((double)bitrate * SKIP_TIME_MS / 8 / 1000.0);

            /* Based on the number of bytes that need to be skipped,
            adjust the consumed input bytes. */

            /* Read the decoded buffer stream position */
            err_code = (*p_xa_process_api)((xa_codec_handle_t)p_xa_process_api_obj,
                XA_API_CMD_GET_CONFIG_PARAM,
                XA_CONFIG_PARAM_CUR_INPUT_STREAM_POS,
                &cur_strm_pos
            );
            _XA_HANDLE_ERROR(p_proc_err_info, "Get Input Stream Position Error", err_code);

            /* Calculate the new stream position */
            new_stream_pos = cur_strm_pos + skip_bytes;
            if (new_stream_pos > ((int)(file_end-ui_inp_size)) && cur_strm_pos <= ((int)(file_end-ui_inp_size))) 
                new_stream_pos = file_end-ui_inp_size;

            if (numFrames == STEP_AT_NUM_FRAME_OFFSET) 
                new_stream_pos = NEW_OFFSET_POSITION_IN_BYTES;
           
            /* Set and display the new current buffer position w.r.t file pointer */
            err_code = (*p_xa_process_api)((xa_codec_handle_t)p_xa_process_api_obj,
                XA_API_CMD_SET_CONFIG_PARAM,
                XA_CONFIG_PARAM_CUR_INPUT_STREAM_POS,
                &new_stream_pos
            );
            _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

            fseek(g_pf_inp, new_stream_pos, SEEK_SET); 
            ui_bytes_consumed= i_buff_size = 0;

            /* Calculate mute samples to be inserted & frames to be flushed before runtime init is complete */
            mute_bytes = (int)(MUTE_TIME_MS*ui_samp_freq/1000.0)*2*ui_num_chan;
            flush_frames_counter = 1;

            /* Isuue a runtime init command to flush out the states of decoder filters nad transforms */
            err_code = (*p_xa_process_api)((xa_codec_handle_t)p_xa_process_api_obj,
                XA_API_CMD_EXECUTE,
                XA_CMD_TYPE_DO_RUNTIME_INIT,
                NULL
            );
            _XA_HANDLE_ERROR(p_proc_err_info, "", err_code); 
            fprintf(stdout, "RUNTIME INIT ISSUED: New Input Buffer Position | Old buffer position: [%d | %d]\n", new_stream_pos, cur_strm_pos); 
        }
#endif /* STEP_PLAY */


        i_buff_size -= ui_bytes_consumed;

        /* Copy unconsumed data at the base portion of the input buffer. */
        if (ui_bytes_consumed != 0) 
        {
            for (ui_loop_counter = 0; ui_loop_counter < ((UWORD32)i_buff_size); ui_loop_counter++)
            {
                pb_inp_buf[ui_loop_counter] = pb_inp_buf[ui_loop_counter + ui_bytes_consumed];
            }
        }
 
#ifdef SMALL_INPUT_CHUNK_FEED

        /* Try and Read at least varChunkSize at time or smaller from the file */
        {
            bytes2read = ((int)ui_bytes_consumed < varChunkSize)? ui_bytes_consumed:varChunkSize;
            bytes2read = (bytes2read==0)? varChunkSize:bytes2read;
            bytes2read = (bytes2read > ((int)ui_inp_size-i_buff_size))?
            (ui_inp_size-i_buff_size ):bytes2read;

            if( raw_vorbis_file_flag == 1)
            bytes2read = ((int)ui_inp_size-i_buff_size);
        }
#else //#ifdef SMALL_INPUT_CHUNK_FEED
        bytes2read = ui_inp_size-i_buff_size ;
#endif // #ifdef SMALL_INPUT_CHUNK_FEED
    
        /* Read data as per the size of input buffer */
        i_bytes_read = fread(pb_inp_buf + (i_buff_size), 1, bytes2read, g_pf_inp);

        /* New buffer size */
        i_buff_size = i_buff_size + i_bytes_read;

#if STEP_PLAY // RUNTIME INIT
        // Special case for last rutime init case , where read bytes may be less than 12k
        // if((i_bytes_read < (int)ui_inp_size) && (flush_frames_counter == 1))
        //    i_buff_size = i_bytes_read;
#endif //#if STEP_PLAY // RUNTIME INIT

        /* Set end of input file if i_buff_size is zero. Or if last call to decoder returns with "need more"
        and fileend is reached and i_bytes_read are zero */
        if((i_buff_size <= 0)||
        ((exec_ret_val ==XA_VORBISDEC_EXECUTE_NONFATAL_OV_INSUFFICIENT_DATA) && (feof(g_pf_inp)&&(ui_bytes_consumed ==0) &&  (i_bytes_read ==0)))) 
        {
            /* Tell that the input is over in this buffer */
            err_code = (*p_xa_process_api)((xa_codec_handle_t)p_xa_process_api_obj, 
                XA_API_CMD_INPUT_OVER,0,0
            );
            _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

#if STEP_PLAY // RUNTIME INIT
            input_over =1;
#endif //#if STEP_PLAY // RUNTIME INIT
        }

        /* Set number of bytes to be processed */
        err_code = (*p_xa_process_api)((xa_codec_handle_t)p_xa_process_api_obj, 
            XA_API_CMD_SET_INPUT_BYTES,0,&i_buff_size
        );
        _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);    

#if 0  /* DISPLAY enable for current stream position  */
        {
            int curr_buffer_pos = -1;

            /* Get and display current buffer position w.r.t file pointer */
            err_code = (*p_xa_process_api)((xa_codec_handle_t)p_xa_process_api_obj,
                XA_API_CMD_GET_CONFIG_PARAM,
                XA_CONFIG_PARAM_CUR_INPUT_STREAM_POS,
                &curr_buffer_pos
            );
            _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

            fprintf(stdout, "Top of Input Buffer Position | file Rd pos: [%d | %d] \n", curr_buffer_pos, (int)ftell(g_pf_inp)); 
        }
#endif

#ifdef __PROFILE__
        xt_iss_client_command("all","enable");
        times(&start);
#endif

        /* Execute process */
        err_code = (*p_xa_process_api)((xa_codec_handle_t)p_xa_process_api_obj, XA_API_CMD_EXECUTE, 
            XA_CMD_TYPE_DO_EXECUTE, 0
        );

#ifdef __PROFILE__
        times(&stop);
        xt_iss_client_command("all","disable");
#endif

        exec_ret_val = err_code;
#ifdef SMALL_INPUT_CHUNK_FEED
        /* print error log only once for insuficient data error. */
        if((errFlagSmallChunk == 0) || err_code != XA_VORBISDEC_EXECUTE_NONFATAL_OV_INSUFFICIENT_DATA)
        {
            _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);
        }

        if (err_code == XA_VORBISDEC_EXECUTE_NONFATAL_OV_INSUFFICIENT_DATA)
            errFlagSmallChunk = 1;

#else // #ifdef SMALL_INPUT_CHUNK_FEED
        _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);
#endif // #ifdef SMALL_INPUT_CHUNK_FEED    
    
        /* Checking for end of processing */ 
        err_code = (*p_xa_process_api)((xa_codec_handle_t)p_xa_process_api_obj, XA_API_CMD_EXECUTE, 
            XA_CMD_TYPE_DONE_QUERY, &i_decode_done
        );
        _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

        /* Get the output bytes */
        err_code = (*p_xa_process_api)((xa_codec_handle_t)p_xa_process_api_obj, 
            XA_API_CMD_GET_OUTPUT_BYTES, 0, &ui_op_bytes
        );
        _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

        if(ui_op_bytes > 0)
        {
            xa_fwrite((pVOID)pb_out_buf, (ui_pcm_wd_sz/8), 
                ui_op_bytes/(ui_pcm_wd_sz/8), g_pf_out);
        }
    
        /* How much buffer is used in input buffers */
        err_code = (*p_xa_process_api)((xa_codec_handle_t)p_xa_process_api_obj, 
            XA_API_CMD_GET_CURIDX_INPUT_BUF, 
            0, 
            &ui_bytes_consumed
        );
        _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

        /* Get gen stream position. */
        err_code = (*p_xa_process_api)((xa_codec_handle_t)p_xa_process_api_obj,
            XA_API_CMD_GET_CONFIG_PARAM,
            XA_CONFIG_PARAM_GEN_INPUT_STREAM_POS,
            &decoded_frame_strm_pos
        ); 
        _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

#ifdef __PROFILE__
        samples=ui_op_bytes + samples;
        cycles += stop.tms_utime - start.tms_utime;
        if (ui_op_bytes) frame++;
        if(samples > (ui_samp_freq*ui_num_chan*2*CPU_LOAD_WINDOW)) 
        {
            avg++;
            mcps_curr = ((FLOAT64) cycles /
            (samples /
            (ui_num_chan*2)) *
            ui_samp_freq / 1000000);
            mcps_sum += mcps_curr;
            mcps_ave = mcps_sum / (avg);

            if (mcps_peak < mcps_curr) 
            {
                mcps_peak = mcps_curr;
                peak_frame = frame;
            }

            fprintf(stderr,"[%d|%u] MCPS: %f Average: %f Peak: %f @ [%d]\n",
            frame,decoded_frame_strm_pos, mcps_curr, mcps_ave, mcps_peak, peak_frame);
            cycles=0;
            samples=0;
        }
#endif
        if (ui_op_bytes) numFrames++;
#if !defined __XCC__
        fprintf(stdout,"[%d |%u] \n",numFrames,decoded_frame_strm_pos);
#endif

#if STEP_PLAY

        /* Update play time ms */
        play_time_ms += (int)((double)ui_op_bytes/ui_samp_freq * 1000.0);

        /* Insert mute samples if flushing of frames due to runtime init is over */
        if (flush_frames_counter)
        {
            flush_frames_counter--; 
            if ((mute_bytes) && (flush_frames_counter==0)) 
            {
                fprintf(stdout, " %d Mute samples inserted ...... \n", mute_bytes); 
                while(mute_bytes--)
                {
                    putc(0, g_pf_out);
                }
            }
        }

#endif /* STEP_PLAY */

#ifdef SCRATCH_PING_PONG
        int scr_ping_pong_fail = 0;
        if(pb_scr_buf == pb_scr_buf_ping)
        {
            /* Test other scratch area for corruption */
            SCRATCH_TEST(pb_scr_buf_pong, 0xbd);
            pb_scr_buf = pb_scr_buf_pong;
            memset(pb_scr_buf_ping, 0xbd, ui_scr_size);
        }
        else
        {
            /* Test other scratch area for corruption */
            SCRATCH_TEST(pb_scr_buf_ping, 0xbd);
            pb_scr_buf = pb_scr_buf_ping;
            memset(pb_scr_buf_pong, 0xbd, ui_scr_size);
        }
        /* Set scratch buffer pointer */
        err_code = (*p_xa_process_api)((xa_codec_handle_t)p_xa_process_api_obj, XA_API_CMD_SET_MEM_PTR, 
            XA_MEMTYPE_SCRATCH, pb_scr_buf
        );
        _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);
#endif /* #ifdef SCRATCH_PING_PONG */

#ifdef OUTPUT_PING_PONG
        if(pb_out_buf == pb_out_buf_ping)
        {
            pb_out_buf = pb_out_buf_pong;
        }
        else
        {
            pb_out_buf = pb_out_buf_ping;
        }
        /* Set output buffer pointer */
        err_code = (*p_xa_process_api)((xa_codec_handle_t)p_xa_process_api_obj, XA_API_CMD_SET_MEM_PTR, 
            XA_MEMTYPE_OUTPUT, pb_out_buf
        );
        _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);
#endif /* #ifdef OUTPUT_PING_PONG */

#ifdef END_FRAME 
        if (numFrames == END_FRAME) 
            break;
#endif
    }while(!i_decode_done);

#ifdef __PROFILE__
    fprintf(stderr,"\nPeak MCPS = %f\n", mcps_peak);
    fprintf(stderr,"Average MCPS = %f\n", mcps_ave);
#endif

    printf("Done.\n");
    return XA_NO_ERROR;
} /* xa_ogg_dec_main_process */


// Set cache attribute to Write Back No Allocate when the last argument is -wbna
void set_wbna(int *argc, char *argv[])
{
    if ( *argc > 1 && !strcmp(argv[*argc-1], "-wbna") )
    {
#ifdef __XCC__
        extern char _memmap_cacheattr_wbna_trapnull;
        xthal_set_cacheattr((unsigned)&_memmap_cacheattr_wbna_trapnull);
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
int main(int argc, char *argv[]) 
{
    FILE *param_file_id;
    WORD8 curr_cmd[XA_MAX_CMD_LINE_LENGTH];
    int fargc, curpos;
    int processcmd = 0;

    char fargv[XA_MAX_ARGS][XA_MAX_CMD_LINE_LENGTH];
    char *pargv[XA_MAX_ARGS];

    WORD8 pb_input_file_path[XA_MAX_CMD_LINE_LENGTH] = "";
    WORD8 pb_output_file_path[XA_MAX_CMD_LINE_LENGTH] = "";
   
    // NOTE: set_wbna() should be called before any other dynamic
    // adjustment of the region attributes for cache.
    set_wbna(&argc, argv);

    xa_testbench_error_handler_init();
    memset(g_pv_arr_alloc_memory,0,MAX_MEM_ALLOCS);

    raw_vorbis_file_flag =0;
    raw_vorbis_file_last_pkt_gran_pos = -1;

    if(1 == argc)    
    {
        if ((param_file_id = fopen(PARAMFILE, "r")) == NULL ) 
        {
            printf("Unable to open parameter file " PARAMFILE ".\n");
            printf("Command line usage:\n");
            printf("<decoder> -ifile:<input_file.ogg> -ofile:<output_file.pcm>\n");
            return XA_NO_ERROR;
        }   
       
        /* Process one line at a time */
        while(fgets((char*)curr_cmd, XA_MAX_CMD_LINE_LENGTH, param_file_id)) 
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

            while(sscanf((char*)(curr_cmd + curpos), "%s", fargv[fargc]) != EOF) 
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
                    fgets((char*)(curr_cmd + curpos), XA_MAX_CMD_LINE_LENGTH, param_file_id);
                    continue;
                }

                curpos += strlen(fargv[fargc]);
                while(*(curr_cmd + curpos)==' ' || *(curr_cmd + curpos)=='\t') 
                {
                    curpos++;
                }
                fargc++;
            }

            if(fargc < 1)    
            {
                /* for blank lines etc. */
                continue;
            }    

            if(strcmp(fargv[0], "@Output_path") == 0)    
            {
                if(fargc > 1) 
                {
                    strcpy((char*)pb_output_file_path, fargv[1]);
                }
                else 
                {
                    strcpy((char*)pb_output_file_path, "");
                }
                continue;
            }

            if(strcmp(fargv[0], "@Input_path") == 0) 
            {
                if(fargc > 1) 
                {
                    strcpy((char*)pb_input_file_path, fargv[1]);
                }
                else 
                {
                    strcpy((char*)pb_input_file_path, "");
                }
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

            raw_vorbis_file_flag = 0; // set to zero : default is .oggs format.
            raw_vorbis_file_last_pkt_gran_pos = -1;

            /* otherwise if this a normal command and its enabled for execution */
            if(processcmd) 
            {
                WORD32 i;
                WORD32 err_code = XA_NO_ERROR;
                int file_flag = 0;

                for(i = 0; i < fargc; i++) 
                {
                    printf("%s ", fargv[i]);
                    pargv[i] = fargv[i];

                    if(!strncmp(fargv[i], "-ifile:", 7)) 
                    {
                        pWORD8 pb_arg_val = (pWORD8)(fargv[i] + 7);
                        WORD8 pb_input_file_name[XA_MAX_CMD_LINE_LENGTH] = "";

                        file_flag++;
                        strcat((char*)pb_input_file_name, (char*)pb_input_file_path);
                        strcat((char*)pb_input_file_name, (char*)pb_arg_val);
                        g_pf_inp = NULL;
                        g_pf_inp = fopen((char*)pb_input_file_name, "rb");

                        if(g_pf_inp == NULL) 
                        {
                            err_code = XA_TESTBENCH_MFMAN_FATAL_FILE_OPEN_FAILED;
                            xa_error_handler(&xa_testbench_error_info, "Input File", err_code);
                        }
#if STEP_PLAY
                        /* Note end of file or filesize */
                        fseek(g_pf_inp, 0L, SEEK_END); 
                        file_end = ftell(g_pf_inp);
                        fseek(g_pf_inp, 0L, SEEK_SET); 
#endif /* STEP_PLAY */
                    }

                    if(!strncmp(fargv[i], "-ofile:", 7)) 
                    {
                        pWORD8 pb_arg_val = (pWORD8)(fargv[i] + 7);

                        WORD8 pb_output_file_name[XA_MAX_CMD_LINE_LENGTH] = "";

                        file_flag++;
                        strcat((char*)pb_output_file_name, (char*)pb_output_file_path);
                        strcat((char*)pb_output_file_name, (char*)pb_arg_val);

                        g_pf_out = NULL;
                        g_pf_out = fopen((char*)pb_output_file_name, "wb");
                        if(g_pf_out == NULL) 
                        {
                            err_code = XA_TESTBENCH_MFMAN_FATAL_FILE_OPEN_FAILED;
                            xa_error_handler(&xa_testbench_error_info, 
                            "Output File", err_code);
                        }
                    } 
                    if(!strncmp(fargv[i], "-rtmem:", 7))
                    {
                        WORD8 pb_buf[XA_MAX_CMD_LINE_LENGTH] = "";
                        pWORD8 pb_arg_val = (pWORD8)(fargv[i] + 7);

                        strcpy((char*)pb_buf, (char*)pb_arg_val);
                        runtime_mem = atoi((char*)pb_buf);
                        continue;
                    }

                    if(!strncmp(fargv[i], "-r", 2))
                    {
                        raw_vorbis_file_flag =1;
                    }

                    if(!strncmp(fargv[i], "-g", 2))
                    {
                        WORD8 pb_buf[XA_MAX_CMD_LINE_LENGTH] = "";
                        pWORD8 pb_arg_val = (pWORD8)(fargv[i] + 2);

                        strcpy((char*)pb_buf, (char*)pb_arg_val);
                        raw_vorbis_file_last_pkt_gran_pos =atoi((char*)pb_buf);
                    }

#ifdef SMALL_INPUT_CHUNK_FEED
                    if (!strncmp(fargv[i], "-inSize:", 8)) 
                    {
                        varChunkSize = atoi(((const char*)(fargv[i] + 8)));
                        /* FORCE the varChunk size to be even - to allow smooth byte swap operation on the input buffer */
                        /* Also feed no less than 8 bytes at a time */
                        varChunkSize &= ~1;

                        if (varChunkSize < 8) 
                            varChunkSize=8;
                    }
#endif //#ifdef SMALL_INPUT_CHUNK_FEED
                    if(!strncmp(fargv[i], "-ogg_maxpage:", 13))
                    {
                        WORD8 pb_buf[XA_MAX_CMD_LINE_LENGTH] = "";
                        pWORD8 pb_arg_val = (pWORD8)(fargv[i] + 13);

                        strcpy((char*)pb_buf, (char*)pb_arg_val);
                        ogg_max_page_size = atoi((char*)pb_buf);
                    }
                }
                g_w_malloc_count = 0;

                printf("\n");
                if(file_flag != 2) 
                {
                    err_code = XA_TESTBENCH_MFMAN_FATAL_FILE_OPEN_FAILED;
                    printf("Unable to open input/output file\n");
                    printf("Command line usage:\n");
                    printf("<decoder> -ifile:<input_file.ogg> -ofile:<output_file.pcm>\n");
                    xa_error_handler(&xa_testbench_error_info,
                    "Input or Output File", err_code);
                }

                if(XA_NO_ERROR == err_code) 
                {
                    xa_ogg_dec_main_process(fargc, pargv);
                }
                for(i = 0; i < g_w_malloc_count; i++) 
                {
                    if(g_pv_arr_alloc_memory[i]) 
                    {
                        free(g_pv_arr_alloc_memory[i]);
                    }
                }

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
        WORD32 i;
        WORD32 err_code = XA_NO_ERROR;
        WORD32 file_flag = 0;

        raw_vorbis_file_flag =0;
        raw_vorbis_file_last_pkt_gran_pos = -1;

        for(i = 1; i < argc; i++) 
        {
            printf("%s ", argv[i]);

            if(!strncmp(argv[i], "-h", 2))
            {
                print_usage(argv);
                return XA_NO_ERROR;
            }
            if(!strncmp(argv[i], "-ifile:", 7)) 
            {
                pWORD8 pb_arg_val = (pWORD8)(argv[i] + 7);
                WORD8 pb_input_file_name[XA_MAX_CMD_LINE_LENGTH] = "";

                file_flag++;
                err_code = XA_NO_ERROR;
                strcat((char*)pb_input_file_name, (char*)pb_input_file_path);
                strcat((char*)pb_input_file_name, (char*)pb_arg_val);

                g_pf_inp = NULL;
                g_pf_inp = fopen((char*)pb_input_file_name, "rb");
                if(g_pf_inp == NULL) 
                {
                    err_code = XA_TESTBENCH_MFMAN_FATAL_FILE_OPEN_FAILED;
                    xa_error_handler(&xa_testbench_error_info, "Input File", err_code);
                }

#if STEP_PLAY
                /* Note end of file or filesize */
                fseek(g_pf_inp, 0L, SEEK_END); 
                file_end = ftell(g_pf_inp);
                fseek(g_pf_inp, 0L, SEEK_SET); 
#endif /* STEP_PLAY */
            }

            if(!strncmp(argv[i], "-ofile:", 7)) 
            {
                pWORD8 pb_arg_val = (pWORD8)(argv[i] + 7);
                WORD8 pb_output_file_name[XA_MAX_CMD_LINE_LENGTH] = "";

                file_flag++;
                strcat((char*)pb_output_file_name, (char*)pb_output_file_path);
                strcat((char*)pb_output_file_name, (char*)pb_arg_val);

                g_pf_out = NULL;
                g_pf_out = fopen((char*)pb_output_file_name, "wb");
                if(g_pf_out == NULL) 
                {
                    err_code = XA_TESTBENCH_MFMAN_FATAL_FILE_OPEN_FAILED;
                    xa_error_handler(&xa_testbench_error_info, "Output File", err_code);
                }
            }
            if(!strncmp(argv[i], "-rtmem:", 7))
            {
                WORD8 pb_buf[XA_MAX_CMD_LINE_LENGTH] = "";
                pWORD8 pb_arg_val = (pWORD8)(argv[i] + 7);

                strcpy((char*)pb_buf, (char*)pb_arg_val);
                runtime_mem = atoi((char*)pb_buf);
                continue;
            }

            if(!strncmp(argv[i], "-r", 2))
            {
                raw_vorbis_file_flag =1;
            }
        
            if(!strncmp(argv[i], "-g", 2))
            {
                WORD8 pb_buf[XA_MAX_CMD_LINE_LENGTH] = "";
                pWORD8 pb_arg_val = (pWORD8)(argv[i] + 2);

                strcpy((char*)pb_buf, (char*)pb_arg_val);
                raw_vorbis_file_last_pkt_gran_pos =atoi((char*)pb_buf);
            }

#ifdef SMALL_INPUT_CHUNK_FEED
            if (!strncmp(argv[i], "-inSize:", 8)) 
            {
                varChunkSize = atoi(((const char*)(argv[i] + 8)));
                /* FORCE the varChunk size to be even - to allow smooth byte swap operation on the input buffer */
                /* Also feed no less than 8 bytes at a time */
                varChunkSize &= ~1;

                if (varChunkSize < 8) 
                    varChunkSize=8;
            }
#endif //#ifdef SMALL_INPUT_CHUNK_FEED
            if(!strncmp(argv[i], "-ogg_maxpage:", 13))
            {
                WORD8 pb_buf[XA_MAX_CMD_LINE_LENGTH] = "";
                pWORD8 pb_arg_val = (pWORD8)(argv[i] + 13);

                strcpy((char*)pb_buf, (char*)pb_arg_val);
                ogg_max_page_size = atoi((char*)pb_buf);
            }

        }
        g_w_malloc_count = 0;

        printf("\n");
        if(file_flag != 2) 
        {
            err_code = XA_TESTBENCH_MFMAN_FATAL_FILE_OPEN_FAILED;
            printf("Unable to open input/output file\n");
            printf("Command line syntax:\n");
            printf("<decoder> -ifile:<input file name> -ofile:<output file name>\n");
            xa_error_handler(&xa_testbench_error_info,
            "Input or Output File", err_code);
        }

        if(XA_NO_ERROR == err_code) 
        {
            xa_ogg_dec_main_process(argc - 1, &argv[1]);
        }

        for(i = 0; i < g_w_malloc_count; i++) 
        {
            if(g_pv_arr_alloc_memory[i]) 
            {
                free(g_pv_arr_alloc_memory[i]);
            }
        }

        if(g_pf_out) 
        {
            fclose(g_pf_out);
        }

        if(g_pf_inp) 
        {
            fclose(g_pf_inp);    
        }  
    }
    return XA_NO_ERROR;
} /* end xa_param_file_process */
