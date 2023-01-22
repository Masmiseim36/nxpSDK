/*
 * Copyright (c) 2009-2020 Cadence Design Systems, Inc.
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
#include <assert.h>

#include "xa_type_def.h"
#include "xa_error_standards.h"
#include "xa_error_handler.h"
#include "xa_apicmd_standards.h"
#include "xa_memory_standards.h"
#include "sbc_enc/xa_sbc_enc_api.h"
#if __XTENSA_EB__
#include <xtensa/tie/xt_hifi2.h>
#endif

#ifdef __XCC__
#include <xtensa/hal.h>
#endif

#if !defined(SHOW_SIZES)
#define SHOW_SIZES 1
#endif  /* SHOW_SIZES */


VOID xa_sbc_enc_error_handler_init();
VOID xa_testbench_error_handler_init();

extern xa_error_info_struct xa_testbench_error_info;
extern xa_error_info_struct xa_sbc_enc_error_info;


#if !defined(WAV_HEADER)
#define WAV_HEADER 1
#endif  /* WAV_HEADER */

#if !defined(DISPLAY_MESSAGE)
#define DISPLAY_MESSAGE 1
#endif  /* DISPLAY_MESSAGE */

#if !defined(PROFILE)
#define PROFILE 1
#endif

#if PROFILE
#include <sys/times.h>
#include <xtensa/sim.h>
#endif

#define MAX_MEM_ALLOCS 100
#define XA_MAX_CMD_LINE_LENGTH 300
#define XA_MAX_ARGS 20
#define PARAMFILE_SBC "paramfilesimple_sbc_enc.txt"

/* This is used to verify the scratch memory behavior */
#define TEST_SCRATCH

#define XA_TESTBENCH_MFMAN_FATAL_MEM_ALLOC_FAILED       0xffff8000
#define XA_TESTBENCH_MFMAN_FATAL_FILE_OPEN_FAILED       0xffff8001


pVOID g_pv_arr_alloc_memory[MAX_MEM_ALLOCS];
WORD  g_w_malloc_count;
FILE *g_pf_inp, *g_pf_out;


#if WAV_HEADER

#if __XTENSA_EB__
#define word16_order(p) (AE_SHA32(p))
#define word32_order(p) (word16_swap(p))

static WORD32 word16_swap(WORD32 data)
{
  data = AE_SHA32(data);
  return (data << 16)+ ((data >> 16) & 0xFFFF);
}

#else
#define word32_order(p) (p)
#define word16_order(p) ((p))
#endif

/* Read data in little-endian (Intel) format regardless of the
   target processor byte order. */
WORD32 xa_fread (void *buffer, int size, int nwords, FILE *fp)
{
  int i, j, n=0, c;
  char *pb_buf = (char *) buffer;
  
  for (i = 0; i < nwords; i++) {
#if __XTENSA_EL__
    for (j = 0; j < size; j++) {
      c = getc(fp);
      if (c == EOF) return n;
      pb_buf[i * size + j] = c;
    }
#else
    for (j = size - 1; j >= 0; j--) {
      c = getc(fp);
      if (c == EOF) return n;
      pb_buf[i * size + j] = c;
    }
#endif
    n++;
  }
  return n;
}

int
read_wav_header (FILE *FilePtr, /* file to write */
                 pWORD32 DataSize, /* total bytes in the wav file */
                 pWORD32 SamplingRate, /* sample rate */
                 pWORD32 NumOfChannels, /* output channels */
                 pWORD32 BitsPerSample /* bits per sample */)
{

  char  WaveTag[4];
  WORD16 BytesPerSample;
  WORD32 longtmp;
  WORD16 shorttmp;
  WORD32 sRate;
  WORD16 nChannels, bitpSample;
  
  fseek(FilePtr, 0, SEEK_SET);

  /* RIFF chunk - 12 bytes */
  fread(&WaveTag, 4, 1, FilePtr);
  if(strncmp("RIFF", WaveTag, 4) != 0)
    {
      return 1;
    }
  fread(&longtmp, 4, 1, FilePtr);
  fread(&WaveTag, 4, 1, FilePtr);
  if(strncmp("WAVE", WaveTag, 4) != 0)
    {
      return 1;
    }

  /* FORMAT chunk - 24 bytes */
  fread(&WaveTag, 4, 1, FilePtr);
  if(strncmp("fmt ", WaveTag, 4) != 0)
    {
      return 1;
    }

  fread(&longtmp, 4, 1, FilePtr);
  fread(&shorttmp, 2, 1, FilePtr);
  fread(&nChannels,  2, 1, FilePtr);
  *NumOfChannels = (WORD32) word16_order(nChannels);
  fread(&sRate,  4, 1, FilePtr);
  *SamplingRate = word32_order(sRate);
  fread(&longtmp, 4, 1, FilePtr);
  fread(&BytesPerSample,  2, 1, FilePtr);
  BytesPerSample = word16_order(BytesPerSample);
  fread(&bitpSample,  2, 1, FilePtr);
  *BitsPerSample =(WORD32) word16_order(bitpSample);
    

  /* DATA chunk - Data length + 8 bytes */
  fread(&WaveTag, 4, 1, FilePtr);
  if(strncmp("data", WaveTag, 4) != 0)
    {
      return 1;
    }
  fread(DataSize,  4, 1, FilePtr);
  *DataSize = word32_order(*DataSize);

  *DataSize = *DataSize / BytesPerSample;
  return 0;
}

#endif /* WAV_HEADER */


void
usage()
{
  fprintf(stdout,
	  "\n"
	  "Usage:  xt-run <binfile> [options] -ifile:<inputfile> -ofile:<outputfile>\n"
	  "        <binfile>     HiFi2 SBC Encoder executable\n"
	  "        <inputfile>   Input WAV file\n"
	  "        <outputfile>  Output SBC file\n"
	  "        -n<subbands>  Number of subbands: 4, 8 (default)\n"
	  "        -l<blocks>    Number of blocks: 4, 8, 12, 16 (default)\n"
	  "        -b<bitpool>   Bitpool value (default: 31)\n"
	  "        -s            Use stereo mode for 2-channel input\n"
	  "        -j            Use joint stereo mode for 2-channel input\n"
	  "        -p            Simple psycho-acoustic model (loudness; SNR by default)\n"
	  "        -h            Print this help message\n");
  exit(0);
}


XA_ERRORCODE
xa_sbc_enc_set_config_param (int                argc,
                             char              *argv[],
                             pWORD32           pi_samp_freq,
                             pWORD32           pi_num_chan,
                             xa_codec_handle_t  p_xa_process_api_obj)
{
  LOOPIDX i;
  XA_ERRORCODE err_code = XA_NO_ERROR;

  /* the process API function */
  xa_codec_func_t *p_xa_process_api = xa_sbc_enc;

  xa_error_info_struct *p_proc_err_info = &xa_sbc_enc_error_info;

  UWORD32 ui_chmode = XA_SBC_ENC_CHMODE_MONO;
  if (*pi_num_chan == 2)
    ui_chmode = XA_SBC_ENC_CHMODE_DUAL;

  err_code = (*p_xa_process_api)(p_xa_process_api_obj,
                                 XA_API_CMD_SET_CONFIG_PARAM,
                                 XA_SBC_ENC_CONFIG_PARAM_SAMP_FREQ, pi_samp_freq);
  _XA_HANDLE_ERROR(p_proc_err_info, "" , err_code);


  for(i = 0; i < argc; i++)
    {
      /* Subbands */
      if(!strncmp(argv[i], "-n", 2))
        {
          char *pb_arg_val = argv[i] + 2;
          UWORD32 ui_subbands = atoi(pb_arg_val);
          err_code = (*p_xa_process_api)(p_xa_process_api_obj,
                                         XA_API_CMD_SET_CONFIG_PARAM,
                                         XA_SBC_ENC_CONFIG_PARAM_SUBBANDS, &ui_subbands);
          _XA_HANDLE_ERROR(p_proc_err_info, "" , err_code);

        }

      /* Data blocks */
      if (!strncmp(argv[i], "-l", 2))
        {
          char *pb_arg_val = argv[i] + 2;
          UWORD32 ui_blocks = atoi(pb_arg_val);
          err_code = (*p_xa_process_api)(p_xa_process_api_obj,
                                         XA_API_CMD_SET_CONFIG_PARAM,
                                         XA_SBC_ENC_CONFIG_PARAM_BLOCKS, &ui_blocks);
          _XA_HANDLE_ERROR(p_proc_err_info, "" , err_code);
        }
                
      /* Stereo mode */
      if (!strncmp(argv[i], "-s", 2))
        {
	  if (*pi_num_chan == 2) {
	    ui_chmode = XA_SBC_ENC_CHMODE_STEREO;
	  } else {
	    fprintf(stderr, "Warning: Unable to use stereo mode for a mono stream.\n");
	  }
        }
            
      /* Joint stereo mode */
      if (!strncmp(argv[i], "-j", 2))
        {
	  if (*pi_num_chan == 2) {
	    ui_chmode = XA_SBC_ENC_CHMODE_JOINT;
	  } else {
	    fprintf(stderr, "Warning: Unable to use joint stereo mode for a mono stream.\n");
	  }
	}

      /* Allocation method */
      if (!strncmp(argv[i], "-p", 2))
        {
          UWORD32 ui_snr = 0;
          err_code = (*p_xa_process_api)(p_xa_process_api_obj,
                                         XA_API_CMD_SET_CONFIG_PARAM,
                                         XA_SBC_ENC_CONFIG_PARAM_SNR,
                                         &ui_snr);
          _XA_HANDLE_ERROR(p_proc_err_info, "" , err_code);
        }
       

      if(!strncmp(argv[i], "-b", 2))
        {
          char *pb_arg_val = argv[i] + 2;
          UWORD32 ui_bitpool = atoi(pb_arg_val);
          err_code = (*p_xa_process_api)(p_xa_process_api_obj,
                                         XA_API_CMD_SET_CONFIG_PARAM,
                                         XA_SBC_ENC_CONFIG_PARAM_BITPOOL, &ui_bitpool);
          _XA_HANDLE_ERROR(p_proc_err_info, "" , err_code);

        }

    }

  err_code = (*p_xa_process_api)(p_xa_process_api_obj,
				 XA_API_CMD_SET_CONFIG_PARAM,
				 XA_SBC_ENC_CONFIG_PARAM_CHMODE, &ui_chmode);
  _XA_HANDLE_ERROR(p_proc_err_info, "" , err_code);

  return XA_NO_ERROR;
}


XA_ERRORCODE
xa_sbc_enc_get_config_param (xa_codec_handle_t p_xa_process_api_obj,
                             pWORD32           pi_bitrate)
{
  XA_ERRORCODE err_code = XA_NO_ERROR;
  /* the process API function */
  xa_codec_func_t *p_xa_process_api = xa_sbc_enc;
  xa_error_info_struct *p_proc_err_info = &xa_sbc_enc_error_info;

  /* Data rate */
  {
    err_code = (*p_xa_process_api)(p_xa_process_api_obj,
                                   XA_API_CMD_GET_CONFIG_PARAM,
                                   XA_SBC_ENC_CONFIG_PARAM_BITRATE, pi_bitrate);
    _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

  }


  return XA_NO_ERROR;
}


static unsigned int
output_wordsize (unsigned int sample_bits)
{
  /* Round up to the next 2-byte size: 16 -> 2; 24 -> 4. */
  return 2 * ((sample_bits + 15) / 16);
}


XA_ERRORCODE
xa_sbc_enc_main_process (WORD32  argc,
                         char   *argv[])
{
  LOOPIDX i;

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

#ifdef TEST_SCRATCH
  int scratch_size = 0;
#endif

  /* First part                                        */
  /* Error Handler Init                                */
  /* Get Library Name, Library Version and API Version */
  /* Initialize API structure + Default config set     */
  /* Set config params from user                       */
  /* Initialize memory tables                          */
  /* Get memory information and allocate memory        */

    
    
  /* Memory variables */
  UWORD32 n_mems = 0;
  UWORD32 ui_memtab_size;
  /* API size */
  UWORD32 ui_api_size;
  /* Process initing done query variable */
  UWORD32 ui_init_done, ui_exec_done;
  pWORD8 pb_inp_buf = NULL;
  pWORD8 pb_out_buf = NULL;

  UWORD32 ui_inp_size = 0;
  WORD32 i_bytes_consumed, i_bytes_read;
  WORD32 i_buff_size;

  WORD32 i_out_bytes, i_total_bytes = 0;
  WORD32 i_data_size, i_bitrate, i_samp_freq, i_num_chan, i_pcm_wd_sz;

  /* The process API function */
  xa_codec_func_t *p_xa_process_api;


  /* The set config from argc argv */
  XA_ERRORCODE (*p_set_config_param) (int                argc,
                                      char              *argv[],
                                      pWORD32           pi_samp_freq,
                                      pWORD32           pi_num_chan,
                                      xa_codec_handle_t  p_xa_process_api_obj);

  /* The get config from API */
  XA_ERRORCODE (*p_get_config_param) (xa_codec_handle_t p_xa_process_api_obj,
                                      pWORD32           pi_bitrate);

  /* The error init function */
  VOID (*p_error_init)();

  /* The process error info structure */
  xa_error_info_struct *p_proc_err_info;

  /* Stack process struct initing */
  p_xa_process_api = xa_sbc_enc;
  p_set_config_param = xa_sbc_enc_set_config_param;
  p_get_config_param = xa_sbc_enc_get_config_param;
  p_error_init = xa_sbc_enc_error_handler_init;
  p_proc_err_info = &xa_sbc_enc_error_info;
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
  fprintf(stdout, "Tensilica, Inc. http://www.tensilica.com\n\n");
#endif

  /* ******************************************************************/
  /* Initialize API structure and set config params to default        */
  /* ******************************************************************/

  /* Get the API size */
  err_code = (*p_xa_process_api)(NULL, XA_API_CMD_GET_API_SIZE, 0,
                                 &ui_api_size);

  _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

  /* Allocate memory for API */
  g_pv_arr_alloc_memory[g_w_malloc_count] = malloc(ui_api_size);

  if (g_pv_arr_alloc_memory[g_w_malloc_count] == NULL) {
    _XA_HANDLE_ERROR(&xa_testbench_error_info, "API struct alloc",
                     XA_TESTBENCH_MFMAN_FATAL_MEM_ALLOC_FAILED);
  }

  /* API object requires 4 bytes (WORD32) alignment;
   * malloc() provides at least 8-byte alignment.
   */
  assert((((unsigned int) g_pv_arr_alloc_memory[g_w_malloc_count]) & 3) == 0);

  /* Set API object with the memory allocated */
  xa_process_handle = (void *) g_pv_arr_alloc_memory[g_w_malloc_count];

  g_w_malloc_count++;

#if WAV_HEADER
  if (read_wav_header(g_pf_inp, &i_data_size, &i_samp_freq, &i_num_chan, &i_pcm_wd_sz))
    {
      fprintf(stderr, "Error: Unable to parse the WAV header.\n");
      exit(1);
    }
#endif

  /* Set the config params to default values */
  err_code = (*p_xa_process_api)(xa_process_handle,
                                 XA_API_CMD_INIT,
                                 XA_CMD_TYPE_INIT_API_PRE_CONFIG_PARAMS,
                                 NULL);

  _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

  /* ******************************************************************/
  /* Set config parameters got from the user present in argc argv     */
  /* ******************************************************************/

  err_code = (*p_set_config_param)(argc, argv, &i_samp_freq, &i_num_chan, xa_process_handle);
  _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

  /* ******************************************************************/
  /* Initialize Memory info tables                                    */
  /* ******************************************************************/

  /* Get memory info tables size */
  err_code = (*p_xa_process_api)(xa_process_handle,
                                 XA_API_CMD_GET_MEMTABS_SIZE, 0,
                                 &ui_memtab_size);

  _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);


  g_pv_arr_alloc_memory[g_w_malloc_count] = malloc(ui_memtab_size);

  if(g_pv_arr_alloc_memory[g_w_malloc_count] == NULL) {
    _XA_HANDLE_ERROR(&xa_testbench_error_info, "Mem tables alloc",
                     XA_TESTBENCH_MFMAN_FATAL_MEM_ALLOC_FAILED);
  }

  /* Memory table requires 4 bytes (WORD32) alignment; malloc()
   * provides at least 8-byte alignment.
   */
  assert((((unsigned int) g_pv_arr_alloc_memory[g_w_malloc_count]) & 3) == 0);

#if SHOW_SIZES
     fprintf(stderr,"\nAPI structure size: %u bytes\n", ui_api_size);
     fprintf(stderr,"MEMTABS size: %u bytes\n\n", ui_memtab_size);
#endif


  /* Set pointer for process memory tables      */
  err_code = (*p_xa_process_api)(xa_process_handle,
                                 XA_API_CMD_SET_MEMTABS_PTR, 0,
                                 (void *) g_pv_arr_alloc_memory[g_w_malloc_count]);

  _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

  g_w_malloc_count++;

  /* initialize the API, post config, fill memory tables        */
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

#ifdef TEST_SCRATCH
    if (i == 1)
    {
        scratch_size = ui_size;
    }
#endif

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
      pb_inp_buf = pv_alloc_ptr;
      ui_inp_size = ui_size;
    }
    if(ui_type == XA_MEMTYPE_OUTPUT) {
      pb_out_buf = pv_alloc_ptr;
    }

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

  /* Second part        */
  /* Initialize process */
  /* Get config params  */

  /* ******************************************************************/
  /* Initialize process in a loop (to handle junk data at beginning)  */
  /* ******************************************************************/
  i_buff_size = ui_inp_size;

  do {
    for(i = 0; i < (WORD32)ui_inp_size - i_bytes_consumed; i++) {
      pb_inp_buf[i] = pb_inp_buf[i + i_bytes_consumed];
    }

    i_bytes_read = (i_pcm_wd_sz/8)*xa_fread(pb_inp_buf + (ui_inp_size - i_bytes_consumed),
                                            i_pcm_wd_sz/8,
                                            i_bytes_consumed/(i_pcm_wd_sz/8),
                                            g_pf_inp);

    /* New buffer size */
    i_buff_size = i_buff_size - (i_bytes_consumed - i_bytes_read);

    if( i_buff_size <= 0 ) {
      i_buff_size = 0;

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

  } while (!ui_init_done);
  /* Init end of process condition end */

  /* ******************************************************************/
  /* Get config params from API                                       */
  /* ******************************************************************/

  err_code = (*p_get_config_param)(xa_process_handle, &i_bitrate);
  _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

  fprintf(stdout, "\nData Rate: %d bps\n", i_bitrate);
  fprintf(stdout, "Sample Rate: %d Hz\n", i_samp_freq);
  fprintf(stdout, "Number of Channels: %d\n", i_num_chan);
  fprintf(stdout, "PCM Sample Size: %d bits\n", i_pcm_wd_sz);
  
  /* End second part */

#if PROFILE
  struct tms start, stop;
  double Peak = 0, Ave = 0, Sum = 0;
  int frame = 0;
  int Peak_frame = 0;
  unsigned long long total_samples = 0;
  fprintf(stdout, "\n");
#endif

  /* Third part for executing each process */
  do {

    /* Execute process */

    for(i = 0; i < (WORD32)ui_inp_size - i_bytes_consumed; i++) {
      pb_inp_buf[i] = pb_inp_buf[i + i_bytes_consumed];
    }

    i_bytes_read = (i_pcm_wd_sz/8) * xa_fread(pb_inp_buf + (ui_inp_size - i_bytes_consumed),
                                              i_pcm_wd_sz/8,
                                              i_bytes_consumed/(i_pcm_wd_sz/8),
                                              g_pf_inp);

    /* New buffer size */
    i_buff_size = i_buff_size - (i_bytes_consumed - i_bytes_read);

    if (i_buff_size < ui_inp_size) {
      if (i_buff_size < 0)
	i_buff_size = 0;
      
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

#if PROFILE
    xt_iss_client_command("all","enable");
    times(&start);
#endif

    /* Execute process */
    err_code_exec = (*p_xa_process_api)(xa_process_handle,
                                        XA_API_CMD_EXECUTE,
                                        XA_CMD_TYPE_DO_EXECUTE,
                                        NULL);

#if PROFILE
    times(&stop);
    xt_iss_client_command("all","disable");
#endif

    _XA_HANDLE_ERROR(p_proc_err_info, "", err_code_exec);

#ifdef TEST_SCRATCH
  /* Memset the scratch buffer.  This is done to test that the scratch buffer
   * values have no effect on behavior.
   */
  memset(g_pv_arr_alloc_memory[3], 0xAA, scratch_size);
#endif

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
    
    /* How much buffer is used in input buffers */
    err_code = (*p_xa_process_api)(xa_process_handle,
                                   XA_API_CMD_GET_CURIDX_INPUT_BUF,
                                   0,
                                   &i_bytes_consumed);

    _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

#if PROFILE
    frame++;
    
    /* Assume that if no output was produced, no
     * significant cycles were consumed.
     */
    if (i_out_bytes) {
      clock_t cycles = stop.tms_utime - start.tms_utime;
      int samples = (i_bytes_consumed /
		     (output_wordsize(i_pcm_wd_sz) * i_num_chan));
      double Curr = ((double) cycles /
		     samples * i_samp_freq / 1000000.0);
      Sum += Curr;
      Ave = Sum / frame;
      
      if (Peak < Curr) {
	Peak = Curr;
	Peak_frame = frame;
      }
      
      /*
	Calculate the current time based on the total number of
	samples consumed by the encoder and the sample rate:
	
	time [ms] = samples / sample_rate [kHz]
	
	Alternatively, the current time can be calculated based on
	the total number of bytes produced by the encoder and the
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
      
      fprintf(stdout,
	      "[%d] %d:%02d.%03d MCPS: %.2f Average: %.2f Peak: %.2f @ [%d]\n",
	      frame, minutes, seconds, msec,
	      Curr, Ave, Peak, Peak_frame);
    }
#endif

    i_total_bytes += i_out_bytes;
    
    fwrite((pVOID)pb_out_buf, sizeof(char), i_out_bytes, g_pf_out);
    
    fflush(g_pf_out);
    
    assert(i_bytes_consumed <= i_buff_size);
   
    /* Do till the process execution is done */
  } while(!ui_exec_done);


#if PROFILE
  fprintf(stdout,"\nPeak MCPS = %f\n", Peak);
  fprintf(stdout,"Peak frame = %d\n", Peak_frame);
  fprintf(stdout,"Average MCPS = %f\n", Ave);
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
  FILE *param_file_id = NULL;
  int err_code = XA_NO_ERROR;

  char curr_cmd[XA_MAX_CMD_LINE_LENGTH] = "";
  WORD32 fargc, curpos;
  WORD32 processcmd = 0;

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
      param_file_id = fopen(PARAMFILE_SBC, "r");
      if (param_file_id == NULL)
        {
          err_code = XA_TESTBENCH_MFMAN_FATAL_FILE_OPEN_FAILED;
          xa_error_handler(&xa_testbench_error_info,
                           "Parameter File", err_code);
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

              fprintf(stdout, "\n");
              if(err_code == XA_NO_ERROR)
                xa_sbc_enc_main_process(fargc, pargv);

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

      for(i = 1; i < argc; i++)
        {
          fprintf(stdout, "%s ", argv[i]);

	  /* Show help message */
	  if(!strcmp(argv[i], "-h"))
	    {
	      usage();
	    }
        
          if(!strncmp(argv[i], "-ifile:", 7))
            {
              char *pb_arg_val = argv[i] + 7;
              char pb_input_file_name[XA_MAX_CMD_LINE_LENGTH] = "";

              file_flag++;
              err_code = XA_NO_ERROR;
              strcat(pb_input_file_name, pb_input_file_path);
              strcat(pb_input_file_name, pb_arg_val);

              g_pf_inp = NULL;
              g_pf_inp = fopen(pb_input_file_name, "rb");
              if(g_pf_inp == NULL)
                {
                  fprintf(stdout, "Input file: %s\n", pb_input_file_name);
                  err_code = XA_TESTBENCH_MFMAN_FATAL_FILE_OPEN_FAILED;
                  xa_error_handler(&xa_testbench_error_info,
                                   "Input File", err_code);
                }
            }

          if(!strncmp(argv[i], "-ofile:", 7))
            {
              char *pb_arg_val = argv[i] + 7;
              char pb_output_file_name[XA_MAX_CMD_LINE_LENGTH] = "";

              file_flag++;
              strcat(pb_output_file_name, pb_output_file_path);
              strcat(pb_output_file_name, pb_arg_val);

              g_pf_out = NULL;
              g_pf_out = fopen(pb_output_file_name, "wb");
              if(g_pf_out == NULL)
                {
                  fprintf(stdout, "Output file: %s\n", pb_output_file_name);
                  err_code = XA_TESTBENCH_MFMAN_FATAL_FILE_OPEN_FAILED;
                  xa_error_handler(&xa_testbench_error_info,
                                   "Output File", err_code);
                }
            }

        }
      g_w_malloc_count = 0;

      fprintf(stdout, "\n");
      if(file_flag != 2)
        {
          err_code = XA_TESTBENCH_MFMAN_FATAL_FILE_OPEN_FAILED;
          xa_error_handler(&xa_testbench_error_info,
                           "Input or Output File", err_code);
        }

      if(err_code == XA_NO_ERROR)
        xa_sbc_enc_main_process(argc - 1, &argv[1]);

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

