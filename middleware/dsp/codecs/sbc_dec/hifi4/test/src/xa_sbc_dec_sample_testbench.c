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
#include "sbc_dec/xa_sbc_dec_api.h"

#ifdef __XCC__
#include <xtensa/hal.h>
#endif


#if !defined(SHOW_SIZES)
#define SHOW_SIZES 1
#endif	/* SHOW_SIZES */


VOID xa_sbc_dec_error_handler_init();
VOID xa_testbench_error_handler_init();

extern xa_error_info_struct xa_testbench_error_info;
extern xa_error_info_struct xa_sbc_dec_error_info;


#if !defined(WAV_HEADER)
#define WAV_HEADER 1
#endif	/* WAV_HEADER */

#if !defined(DISPLAY_MESSAGE)
#define DISPLAY_MESSAGE	1
#endif	/* DISPLAY_MESSAGE */

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

#define PARAMFILE_SBC "paramfilesimple_sbc_dec.txt"

/* This is used to verify the scratch memory behavior */
#define TEST_SCRATCH

#define XA_TESTBENCH_MFMAN_FATAL_MEM_ALLOC_FAILED	0xffff8000
#define XA_TESTBENCH_MFMAN_FATAL_FILE_OPEN_FAILED	0xffff8001


pVOID g_pv_arr_alloc_memory[MAX_MEM_ALLOCS];
WORD  g_w_malloc_count;
FILE *g_pf_inp, *g_pf_out;


/* Store data in little-endian (Intel) format regardless of the
   target processor byte order. */
void
xa_fwrite (void *buffer, int size, int nwords, FILE *fp)
{
  int i, j;
  char *pb_buf = (char *) buffer;
  
  for (i = 0; i < nwords; i++) {
#if __XTENSA_EL__
    for (j = 0; j < size; j++) {
      putc(pb_buf[i * size + j], fp);
    }
#else
    for (j = size - 1; j >= 0; j--) {
      putc(pb_buf[i * size + j], fp);
    }
#endif
  }
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
usage()
{
  fprintf(stdout,
	  "\n"
	  "Usage:  xt-run <binfile> [options] -ifile:<inputfile> -ofile:<outputfile>\n"
	  "        <binfile>     HiFi2 SBC Decoder executable\n"
	  "        <inputfile>   Input SBC file\n"
#if WAV_HEADER
	  "        <outputfile>  Output WAV file\n"
#else
	  "        <outputfile>  Output PCM file\n"
#endif
	  "        -h            Print this help message\n");
  exit(0);
}


XA_ERRORCODE
xa_sbc_dec_get_config_param (xa_codec_handle_t p_xa_process_api_obj,
			     pWORD32           pi_bitrate,
			     pWORD32           pi_samp_freq,
			     pWORD32           pi_num_chan,
			     pWORD32           pi_pcm_wd_sz)
{
  XA_ERRORCODE err_code = XA_NO_ERROR;
  /* the process API function */
  xa_codec_func_t *p_xa_process_api = xa_sbc_dec;
  xa_error_info_struct *p_proc_err_info = &xa_sbc_dec_error_info;

  /* Data rate */
  {
    err_code = (*p_xa_process_api)(p_xa_process_api_obj,
				   XA_API_CMD_GET_CONFIG_PARAM,
				   XA_SBC_DEC_CONFIG_PARAM_BITRATE, pi_bitrate);
    _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

  }
  /* Sampling frequency */
  {
    err_code = (*p_xa_process_api)(p_xa_process_api_obj,
				   XA_API_CMD_GET_CONFIG_PARAM,
				   XA_SBC_DEC_CONFIG_PARAM_SAMP_FREQ, pi_samp_freq);
    _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

  }
  /* Number of channels */
  {
    err_code = (*p_xa_process_api)(p_xa_process_api_obj,
				   XA_API_CMD_GET_CONFIG_PARAM,
				   XA_SBC_DEC_CONFIG_PARAM_NUM_CHANNELS, pi_num_chan);
    _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);
  }
  /* PCM word size */
  {
    err_code = (*p_xa_process_api)(p_xa_process_api_obj,
				   XA_API_CMD_GET_CONFIG_PARAM,
				   XA_SBC_DEC_CONFIG_PARAM_PCM_WDSZ, pi_pcm_wd_sz);
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
xa_sbc_dec_main_process (WORD32  argc,
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
  UWORD32 ui_proc_mem_tabs_size = 0;
  /* API size */
  UWORD32 pui_api_size = 0;
  /* Process initing done query variable */
  UWORD32 ui_init_done = 0; 
  UWORD32 ui_exec_done = 0;
  pWORD8 pb_inp_buf = NULL;
  pWORD8 pb_out_buf = NULL;

  UWORD32 ui_inp_size = 0;
  WORD32 i_bytes_consumed = 0;
  WORD32 i_bytes_read = 0;
  WORD32 i_buff_size = 0;

  WORD32 i_out_bytes = 0, i_total_bytes = 0;
  WORD32 i_bitrate = 0, i_samp_freq = 0, i_num_chan = 0, i_pcm_wd_sz = 0;

  /* The process API function */
  xa_codec_func_t *p_xa_process_api;

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
  p_xa_process_api = xa_sbc_dec;
  p_get_config_param = xa_sbc_dec_get_config_param;
  p_error_init = xa_sbc_dec_error_handler_init;
  p_proc_err_info = &xa_sbc_dec_error_info;
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
				 &pui_api_size);
#if SHOW_SIZES
        printf("\nAPI structure size: %u bytes\n", pui_api_size);
#endif

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
  xa_process_handle = (void *) g_pv_arr_alloc_memory[g_w_malloc_count];

  g_w_malloc_count++;

  /* Set the config params to default values */
  err_code = (*p_xa_process_api)(xa_process_handle,
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
#if SHOW_SIZES
        printf("\nMEMTABS size: %u bytes\n\n", ui_proc_mem_tabs_size);
#endif

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

  /* Set pointer for process memory tables	*/
  err_code = (*p_xa_process_api)(xa_process_handle,
				 XA_API_CMD_SET_MEMTABS_PTR, 0,
				 (void *) g_pv_arr_alloc_memory[g_w_malloc_count]);

  _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

  g_w_malloc_count++;

  /* initialize the API, post config, fill memory tables	*/
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
#endif	/* SHOW_SIZES */
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

    i_bytes_read = fread(pb_inp_buf + (ui_inp_size - i_bytes_consumed),
			 sizeof(WORD8),
			 i_bytes_consumed,
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

  err_code = (*p_get_config_param)(xa_process_handle,
				   &i_bitrate,
				   &i_samp_freq,
				   &i_num_chan,
				   &i_pcm_wd_sz);
  _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

  fprintf(stdout, "\nData Rate: %d bps\n", i_bitrate);
  fprintf(stdout, "Sample Rate: %d Hz\n", i_samp_freq);
  fprintf(stdout, "Number of Channels: %d\n", i_num_chan);
  fprintf(stdout, "PCM Sample Size: %d bits\n", i_pcm_wd_sz);
  fprintf(stdout, "Standard: SBC\n");
  
  /* End second part */

#if WAV_HEADER
  write_wav_header(g_pf_out, 0x7fffffff, i_samp_freq,
		   i_num_chan, i_pcm_wd_sz);
#endif

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

    i_bytes_read = fread(pb_inp_buf + (ui_inp_size - i_bytes_consumed),
			 sizeof(char),
			 i_bytes_consumed,
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


#if PROFILE
    /* Assume that if no output was produced, no
     * significant cycles were consumed.
     */
    if (i_out_bytes)
      {
	frame++;
	
	clock_t cycles = stop.tms_utime - start.tms_utime;
	int samples = i_out_bytes / (output_wordsize(i_pcm_wd_sz) * i_num_chan);
	double Curr = ((double) cycles / samples * i_samp_freq / 1000000);
	Sum += Curr;
	Ave = Sum / frame;
	
	if (Peak < Curr) {
	  Peak = Curr;
	  Peak_frame = frame;
	}
	
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
	
	fprintf(stdout, "[%d] %d:%02d.%03d MCPS: %.2f Average: %.2f Peak: %.2f @ [%d]\n",
		frame, minutes, seconds, msec,
		Curr, Ave, Peak, Peak_frame);
      }
#endif

    err_code = (*p_get_config_param)(xa_process_handle,
				     &i_bitrate,
				     &i_samp_freq,
				     &i_num_chan,
				     &i_pcm_wd_sz);

    _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);
    
    unsigned int ui_nsamples = i_out_bytes / output_wordsize(i_pcm_wd_sz);
    
    i_total_bytes += (ui_nsamples * i_pcm_wd_sz) / 8;
    
    xa_fwrite((pVOID)pb_out_buf, (i_pcm_wd_sz/8), ui_nsamples, g_pf_out);
    
    /* How much buffer is used in input buffers */
    err_code = (*p_xa_process_api)(xa_process_handle,
				   XA_API_CMD_GET_CURIDX_INPUT_BUF,
				   0,
				   &i_bytes_consumed);

    _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

    assert(i_bytes_consumed <= i_buff_size);
   
    /* Do till the process execution is done */
  } while(!ui_exec_done);

#ifdef TEST_SCRATCH
  /* Memset the scratch buffer.  This is done to test that the scratch buffer
   * values have no effect on behavior.
   */
  memset(g_pv_arr_alloc_memory[3], 0xAA, scratch_size);
#endif

#if PROFILE
  fprintf(stdout,"\nPeak MCPS = %f\n", Peak);
  fprintf(stdout,"Peak frame = %d\n", Peak_frame);
  fprintf(stdout,"Average MCPS = %f\n", Ave);
#endif

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
  FILE *param_file_id;
  int err_code = XA_NO_ERROR;

  char curr_cmd[XA_MAX_CMD_LINE_LENGTH];
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
	    {	/* skip it */
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

	  if(fargc < 1)	/* for blank lines etc. */
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
		xa_sbc_dec_main_process(fargc, pargv);

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
	xa_sbc_dec_main_process(argc - 1, &argv[1]);

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

