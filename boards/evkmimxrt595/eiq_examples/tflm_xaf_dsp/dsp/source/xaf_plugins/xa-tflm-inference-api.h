/*
* Copyright (c) 2015-2021 Cadence Design Systems Inc.
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
/*******************************************************************************
 * xa-tflm-inference-api.h
 *
 * TFLM Inference component API
 ******************************************************************************/

#ifndef __XA_TFLM_INFERENCE_API_H__
#define __XA_TFLM_INFERENCE_API_H__

/*******************************************************************************
 * Constants definitions
 ******************************************************************************/

/* ...Inference specific configuration parameters */
enum xa_config_param_inference {
    XA_INFERENCE_CONFIG_PARAM_CHANNELS          = 0x0,
    XA_INFERENCE_CONFIG_PARAM_SAMPLE_RATE       = 0x1,
    XA_INFERENCE_CONFIG_PARAM_PCM_WIDTH         = 0x2,
    XA_INFERENCE_CONFIG_PARAM_PRODUCED          = 0x3,
    XA_INFERENCE_CONFIG_PARAM_FRAME_SIZE        = 0x4,
    XA_INFERENCE_CONFIG_PARAM_COUNT,
};


enum xa_error_inference {
    XA_INFERENCE_ERROR_START = 0,
/*******************************************************************************
 * Class 0: API Errors
 ******************************************************************************/
    /* ...non-fatal erros */
    
    /* ...fatal erros */

/*******************************************************************************
 * Class 1: Configuration Errors
 ******************************************************************************/
    /* ...non-fatal erros */
    XA_INFERENCE_CONFIG_NONFATAL_RANGE,
    XA_INFERENCE_CONFIG_NONFATAL_STATE,
    XA_INFERENCE_CONFIG_NONFATAL_READONLY,

    /* ...fatal erros */
    XA_INFERENCE_CONFIG_FATAL_RANGE,
    XA_INFERENCE_CONFIG_FATAL_TRACK_STATE,

/*******************************************************************************
 * Class 2: Execution Class Errors
 ******************************************************************************/
    /* ...non-fatal erros */
    XA_INFERENCE_EXEC_NONFATAL_STATE,
    XA_INFERENCE_EXEC_NONFATAL_NO_DATA,
    XA_INFERENCE_EXEC_NONFATAL_INPUT,
    XA_INFERENCE_EXEC_NONFATAL_OUTPUT,
    
    /* ...fatal erros */
    XA_INFERENCE_EXEC_FATAL_STATE,
    XA_INFERENCE_EXEC_FATAL_INPUT,
    XA_INFERENCE_EXEC_FATAL_OUTPUT,

/******************************************************************************/
    XA_INFERENCE_ERROR_END
};

/*******************************************************************************
 * XAF TFLM inference specification definition
 ******************************************************************************/

typedef struct xaf_tflm_inference_spec
{
    /* ...size of input frame / buffer to inference */
    WORD32 input_size;
    
    /* ...size of output produced by inference */
    WORD32 output_size;

    /* ...input consumed (stride) after each inference */
    WORD32 input_stride;
 
    /* ...tensor arena size of inference */
    WORD32 tensor_arena_size;

    /* ...size of op resolver struct (function of num_operators in inference */
    WORD32 op_resolver_size;

    /* ...additional persist required (if any) by inference wrapper */
    WORD32 addl_persist_size;
    
    /* ...additional scratch required (if any) by inference wrapper */
    WORD32 addl_scratch_size;

    /* ...model flatbuffer pointer */
    void     *model; 
    
    /* ...callback function that <TFLM-n/w>-inference-wrapper MUST implement to
       initialize TFLM oprators required by the network */
    int (*inference_init_ops)(void *, void *);

    /* ...callback function that <TFLM-n/w>-inference-wrapper should (OPTIONAL)
       implement if it requires to post-process the TFLM inference output.
       This function gives access to TfLiteTensor output from Invoke() and 
       component output buffer to the wrapper */
    int (*inference_exec_postprocess)(void *, void *, int *, void *);

} xaf_tflm_inference_spec_t;

/*******************************************************************************
 * XAF TFLM inference generc API to be used by inference wrappers
 ******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
/*******************************************************************************
 * xa_tflm_inference_init_spec:
 *      API to initialize specification requirements of TFLM component.
 *
 * Input Parameters:
 *  inference_spec:  
 *      Structure containing specification details of TFLM component.
 *
 ******************************************************************************/

XA_ERRORCODE xa_tflm_inference_init_spec(xa_codec_handle_t p_xa_module_obj, 
                                           xaf_tflm_inference_spec_t *inference_spec);

/*******************************************************************************
 * xa_tflm_inference_api:
 *      Generic XA-API for XA-TFLM-Inference layer 
 ******************************************************************************/
 
XA_ERRORCODE xa_tflm_inference_api(
              xa_codec_handle_t p_xa_module_obj, 
              WORD32 i_cmd, 
              WORD32 i_idx, 
              pVOID pv_value);

#ifdef __cplusplus
}
#endif
#endif /* __XA_TFLM_INFERENCE_API_H__ */
