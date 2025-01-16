/*
 * Copyright (c) 2014-2022 Cadence Design Systems, Inc.
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


#ifndef __XA_DRM_DEC_SAMPLE_TESTBENCH_H__
#define __XA_DRM_DEC_SAMPLE_TESTBENCH_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <assert.h>

#include <xtensa/tie/xt_hifi2.h>
#ifdef __XCC__
#include <xtensa/hal.h>
#endif

#include "xa_type_def.h"
#include "xa_memory_standards.h"
#include "xa_error_handler.h"

#include "drm_dec/xa_drm_dec_api.h"
#include "xa_waveio.h"

#define MAX_MEM_ALLOCS 100
#define MAX_CMD_LINE_LENGTH 300
#define XA_MAX_ARGS 20
#define XA_SCREEN_WIDTH 80

#define PARAMFILE "paramfilesimple.txt"

#define xa_class_mfman 0
#define XA_TESTBENCH 2 //RJ

enum xa_error_fatal_testbench 
{
    XA_TESTBENCH_MFMAN_FATAL_MEM_ALLOC_FAILED   = XA_ERROR_CODE(xa_severity_fatal, xa_class_mfman, XA_TESTBENCH, 0),
    XA_TESTBENCH_MFMAN_FATAL_FILE_OPEN_FAILED   = XA_ERROR_CODE(xa_severity_fatal, xa_class_mfman, XA_TESTBENCH, 1),
    XA_TESTBENCH_MFMAN_FATAL_FILE_READ_FAILED   = XA_ERROR_CODE(xa_severity_fatal, xa_class_mfman, XA_TESTBENCH, 2),
    XA_TESTBENCH_MFMAN_FATAL_FILE_WRITE_FAILED  = XA_ERROR_CODE(xa_severity_fatal, xa_class_mfman, XA_TESTBENCH, 3),
    XA_TESTBENCH_MFMAN_FATAL_FILE_CLOSE_FAILED  = XA_ERROR_CODE(xa_severity_fatal, xa_class_mfman, XA_TESTBENCH, 4),
    XA_TESTBENCH_FATAL_HELP_REQUESTED           = XA_ERROR_CODE(xa_severity_fatal, xa_class_mfman, XA_TESTBENCH, 5),
    XA_TESTBENCH_FATAL_INVALID_ARGUMENT         = XA_ERROR_CODE(xa_severity_fatal, xa_class_mfman, XA_TESTBENCH, 6),
    XA_TESTBENCH_FATAL_INPUT_NOT_SPECIFIED      = XA_ERROR_CODE(xa_severity_fatal, xa_class_mfman, XA_TESTBENCH, 7),
    XA_TESTBENCH_FATAL_OUTPUT_NOT_SPECIFIED     = XA_ERROR_CODE(xa_severity_fatal, xa_class_mfman, XA_TESTBENCH, 8),
    XA_TESTBENCH_FATAL_INIT_FAILED			    = XA_ERROR_CODE(xa_severity_fatal, xa_class_mfman, XA_TESTBENCH, 9),
};
enum xa_error_nonfatal_testbench 
{
    XA_TESTBENCH_MFMAN_NONFATAL_PARTIAL_FRAME   = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_mfman, XA_TESTBENCH, 0),
    XA_TESTBENCH_MFMAN_NONFATAL_STEP_PLAY_RAW   = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_mfman, XA_TESTBENCH, 1),
};

#define API_CALL(cmd, idx, pvalue, context_str) \
    err_code = (*(pobj->p_xa_process_api))(pobj->p_xa_process_api_obj, (cmd), (idx), (pvalue));\
_XA_HANDLE_ERROR(pobj->p_proc_err_info, (context_str), err_code);

#define SET_CONFIG(idx, pvalue, context_str) API_CALL( XA_API_CMD_SET_CONFIG_PARAM,idx, pvalue, context_str)
#define GET_CONFIG(idx, pvalue, context_str) API_CALL( XA_API_CMD_GET_CONFIG_PARAM,idx, pvalue, context_str)

#define TESTBENCH_ERROR(err, context_str) _XA_HANDLE_ERROR(pobj->p_test_err_info, (context_str),  (err));

#define RETURN_ON_FATAL_ERROR(fn) {XA_ERRORCODE err = (fn); if(err & XA_FATAL_ERROR) return err;}

#define PRINT_INVALID_ARG(str) \
    fprintf(stderr, "Unknown argument : %s", str);  \
fprintf(stderr, "Try -h to get complete option list\n");

#define ARGTYPE_INT(PATTERN, CFG) \
    if(strcmp((argv[argidx]), (PATTERN)) == 0) \
{ \
    pcfg->CFG.val = atoi(argv[argidx+1]);\
    pcfg->CFG.modified = 1;\
    continue;\
}

#define ARGTYPE_STR(PATTERN, CFG) \
    if(strcmp((argv[argidx]), (PATTERN)) == 0) \
{\
    strcpy(pcfg->CFG, argv[argidx+1]);\
    continue;\
}
#define IGNORE_ARG(PATTERN) \
    if(strcmp((argv[argidx]), (PATTERN)) == 0) \
{\
    argidx-=1;\
    continue;\
}\

#define SET_UPDATED_CONFIG(i_idx, p_param, context) \
    if((p_param)->modified)\
{ \
    SET_CONFIG( i_idx, (void *)&((p_param)->val), context); \
    (p_param)->modified = 0;\
}

#define BUF_INIT(pbuf, ptr, sz) \
    (pbuf)->base_ptr = (pWORD8)(ptr);\
(pbuf)->size = sz;\
(pbuf)->data_length = 0;\
(pbuf)->inp_over    = 0;\
(pbuf)->bytes_consumed    = 0;

#define BUF_EMPTY_STARTPTR(pbuf) ((unsigned char *)(&(pbuf)->base_ptr[(pbuf)->data_length]))
#define BUF_EMPTY_SPACE(pbuf)    ((pbuf)->size-(pbuf)->data_length)

VOID xa_drm_dec_error_handler_init();
VOID xa_testbench_error_handler_init();

extern xa_error_info_struct xa_drm_dec_error_info;
extern xa_error_info_struct xa_testbench_error_info;

typedef struct _config_param_t
{
    int val;
    int modified;
} config_param_t;

typedef struct _user_config_t
{
    char ifile[MAX_CMD_LINE_LENGTH];
    char ofile[MAX_CMD_LINE_LENGTH];

    config_param_t pcm_width;       // SET for Decoder and Encoder; GET for Decoder and Encoder
    config_param_t out_num_chan;        // SET for Encoder, GET for Decoder
    config_param_t samp_freq;       // SET for Encoder, GET for Decoder
    config_param_t frameSize;       // Only SET for Encoder and GET for decoder

    config_param_t  stopFrame;
    config_param_t  startFrame;
    config_param_t  helpMode ;

#ifdef RAW_AAC_SUPPORTED
    config_param_t bsfmt;
#endif
    config_param_t  qmfLowPower;
    config_param_t  outputMode;
    config_param_t  frameCorrupt;

    config_param_t  isInHoldOffMode;
    /* concealment parameters */
    config_param_t  conceal_fi;
    config_param_t  conceal_fo;
    config_param_t  conceal_mr;
    config_param_t  conceal_cn_level;

} user_config_t;

typedef struct _buffer_t
{
    pWORD8 base_ptr;
    WORD32 size;
    WORD32 data_length;
    WORD32 inp_over;
    WORD32 bytes_consumed;
    WORD32 frm_valid;
#ifdef INPUT_READ_ON_REQUEST
    WORD32 needmoredata;
#endif    
} buffer_t;

#if 1 //DRM_SUPPORTED
typedef struct HANDLE_DRM_t
{
    unsigned int pos_streamConfig;//to record the position of next config on config change
    int config_change_detected;
    int b_input_buffer_overrun;//indicates if requested input is more than the input buffer size
    int b_exec_error_insufficient_input;

    FILE  *fp;
    XA_DRM_DEC_DRM_AUDIO_CONFIG  drmAudioConf;

} HANDLE_DRM;
#endif

#ifdef RAW_AAC_SUPPORTED
typedef struct HANDLE_RAW_t
{
    FILE  *fp;
    char asc[64];
    int asc_length;
} HANDLE_RAW;
#endif

typedef struct HANDLE_INP_t
{
    int inp_fmt;

#if 1 //DRM_SUPPORTED
    HANDLE_DRM drm;
#endif
#ifdef RAW_AAC_SUPPORTED
	HANDLE_RAW raw;
#endif
} HANDLE_INP;

typedef struct _xa_testapp_drm_dec_state
{
    xa_codec_handle_t p_xa_process_api_obj;
    xa_codec_func_t *p_xa_process_api;
    xa_error_info_struct *p_proc_err_info;
    xa_error_info_struct *p_test_err_info;

    buffer_t inpbuf;
    user_config_t cfg;
    buffer_t outbuf;
    int frameCounter;

    unsigned int ui_init_done;
    unsigned int ui_exec_done;

    HANDLE_WAV out_wav;
    HANDLE_INP inp;
    
    // required to handle stream change
    int out_file_count;
} xa_testapp_drm_dec_state;

extern char pb_input_file_path[MAX_CMD_LINE_LENGTH];
extern char pb_output_file_path[MAX_CMD_LINE_LENGTH];

XA_ERRORCODE get_libconfig(xa_testapp_drm_dec_state *pobj);
XA_ERRORCODE get_defaultconfig(xa_testapp_drm_dec_state *pobj);
XA_ERRORCODE cmdlineparse(user_config_t *pcfg, int argc, char *argv[]);
void showusage();
void
xa_shift_input_buffer (signed char *buf, int buf_size, int bytes_consumed);
XA_ERRORCODE INP_Open(HANDLE_INP *pinp, char *filename
#ifdef RAW_AAC_SUPPORTED
        , int fmt
#endif
        );
XA_ERRORCODE INP_Read(HANDLE_INP *pinp, buffer_t *pbuf);
void INP_Close(HANDLE_INP *pinp);
XA_ERRORCODE INP_Seek(HANDLE_INP *pinp, buffer_t *pbuf, int iframes2discard, int *uibytesdiscarded);

void MEM_init(void);
void *MEM_malloc (int size, int align);
void MEM_freeall(void);

// TO BE REMOVED
XA_ERRORCODE set_params(xa_testapp_drm_dec_state *pobj);
int get_params(xa_testapp_drm_dec_state *pobj,
        int * p_numChannels,
        int *p_sampleRate,
        int * p_frame_size,
        int *p_pcm_width);

#endif // __XA_DRM_DEC_SAMPLE_TESTBENCH_H__
