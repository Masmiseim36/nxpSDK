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

#include "tensorflow/lite/micro/kernels/micro_ops.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/c/common.h"

#include "tensorflow/lite/micro/examples/micro_speech/audio_provider.h"
#include "tensorflow/lite/micro/examples/micro_speech/command_responder.h"
#include "tensorflow/lite/micro/examples/micro_speech/feature_provider.h"
#include "tensorflow/lite/micro/examples/micro_speech/micro_features/micro_model_settings.h"
#include "tensorflow/lite/micro/examples/micro_speech/recognize_commands.h"
#include "tensorflow/lite/micro/examples/micro_speech/micro_features/micro_features_generator.h"

/* Model data file */
#include "microspeech_model_data.h"

/* ...generic commands */
#include "xa_apicmd_standards.h"
/* ...generic error codes */
#include "xa_error_standards.h"
/* ...common types */
#include "xa_type_def.h"
/* ...xa-microspeech api header */
#include "xa-microspeech-inference-api.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef XAF_PROFILE
#include "xaf-clk-test.h"
extern clk_t inference_cycles;
#endif

int  microspeech_inference_init_ops(void *op_resolver, void *addl_persist);
int  microspeech_inference_exec_postprocess(void *output_buffer, void *output_tensor, int *out_bytes, void *addl_persist);
   
#ifdef __cplusplus
}
#endif


#define TENSOR_ARENA_SIZE                    (15*1024)
#define NUM_OPS                              (4)
#define INPUT_FRAMESIZE_kFeatureElementCount (49*40)
#define CONSUMED_kFeatureSliceSize           (40)
#define PRODUCED_INFERENCE                   (12)

/* ...microspeech specific recognizer defined and used locally */
static tflite::MicroErrorReporter microspeech_local_error_reporter;


#define INIT_SPEC(inference_spec) \
{ \
    memset(&inference_spec, 0, sizeof(inference_spec)); \
    inference_spec.input_size        = INPUT_FRAMESIZE_kFeatureElementCount; \
    inference_spec.output_size       = PRODUCED_INFERENCE; \
    inference_spec.tensor_arena_size = TENSOR_ARENA_SIZE; \
    inference_spec.op_resolver_size  = sizeof(tflite::MicroMutableOpResolver<NUM_OPS>); \
    inference_spec.input_stride      = CONSUMED_kFeatureSliceSize; \
    inference_spec.model             = (void *) g_model; \
    inference_spec.addl_persist_size = sizeof(RecognizeCommands); \
    inference_spec.addl_scratch_size = 0; \
    inference_spec.inference_init_ops = microspeech_inference_init_ops; \
    inference_spec.inference_exec_postprocess = microspeech_inference_exec_postprocess; \
}

int microspeech_inference_init_ops(void *op_resolver, void *addl_persist)
{
    tflite::MicroMutableOpResolver<NUM_OPS> *micro_op_resolver = new (op_resolver) tflite::MicroMutableOpResolver<NUM_OPS>;

    micro_op_resolver->AddDepthwiseConv2D();
    micro_op_resolver->AddFullyConnected();
    micro_op_resolver->AddReshape();
    micro_op_resolver->AddSoftmax();

    RecognizeCommands *recognizer = new (addl_persist) RecognizeCommands(&microspeech_local_error_reporter);
    (void)recognizer;

    return 0;
}

/* ...microspeech specific post process of output for recognizing keywords */
int  microspeech_inference_exec_postprocess(void *output_buffer, void *output_tensor, int *out_bytes, void *addl_persist)
{
    TfLiteTensor* output = nullptr;
    RecognizeCommands *recognizer = (RecognizeCommands *)addl_persist;

    output = (TfLiteTensor *) output_tensor;

    // Determine whether a command was recognized based on the output of inference
    const char* found_command = nullptr;
    uint8_t score = 0;
    bool is_new_command = false;
    const int32_t current_time = LatestAudioTimestamp();
    
    TfLiteStatus process_status = recognizer->ProcessLatestResults(output, current_time, &found_command, &score, &is_new_command);

    if (process_status != kTfLiteOk) 
    {
        return -1;
    }

    {
        int *pOut_32bit =  (int *) output_buffer;
        pOut_32bit[0] =  is_new_command;
        if ( found_command[0] == 's')
        {
            pOut_32bit[1] = 0;
        }
        if ( found_command[0] == 'u')
        {
            pOut_32bit[1] = 1;
        }
        if ( found_command[0] == 'y')
        {
            pOut_32bit[1] = 2;
        }
        if ( found_command[0] == 'n')
        {
            pOut_32bit[1] = 3;
        }
        pOut_32bit[2] = score;
    }

    *out_bytes = PRODUCED_INFERENCE;

    return 0;
}

static XA_ERRORCODE map_error_code(XA_ERRORCODE error_code)
{
    switch (error_code)
    {
    case XA_INFERENCE_CONFIG_NONFATAL_RANGE    : return XA_MICROSPEECH_INFERENCE_CONFIG_NONFATAL_RANGE;
    case XA_INFERENCE_CONFIG_NONFATAL_STATE    : return XA_MICROSPEECH_INFERENCE_CONFIG_NONFATAL_STATE;
    case XA_INFERENCE_CONFIG_NONFATAL_READONLY : return XA_MICROSPEECH_INFERENCE_CONFIG_NONFATAL_READONLY;
    case XA_INFERENCE_CONFIG_FATAL_RANGE       : return XA_MICROSPEECH_INFERENCE_CONFIG_FATAL_RANGE;
    case XA_INFERENCE_CONFIG_FATAL_TRACK_STATE : return XA_MICROSPEECH_INFERENCE_CONFIG_FATAL_TRACK_STATE;
    case XA_INFERENCE_EXEC_NONFATAL_STATE      : return XA_MICROSPEECH_INFERENCE_EXEC_NONFATAL_STATE;
    case XA_INFERENCE_EXEC_NONFATAL_NO_DATA    : return XA_MICROSPEECH_INFERENCE_EXEC_NONFATAL_NO_DATA;
    case XA_INFERENCE_EXEC_NONFATAL_INPUT      : return XA_MICROSPEECH_INFERENCE_EXEC_NONFATAL_INPUT;
    case XA_INFERENCE_EXEC_NONFATAL_OUTPUT     : return XA_MICROSPEECH_INFERENCE_EXEC_NONFATAL_OUTPUT;
    case XA_INFERENCE_EXEC_FATAL_STATE         : return XA_MICROSPEECH_INFERENCE_EXEC_FATAL_STATE;
    case XA_INFERENCE_EXEC_FATAL_INPUT         : return XA_MICROSPEECH_INFERENCE_EXEC_FATAL_INPUT;
    case XA_INFERENCE_EXEC_FATAL_OUTPUT        : return XA_MICROSPEECH_INFERENCE_EXEC_FATAL_OUTPUT;
    default                                    : return error_code;
    }
}


XA_ERRORCODE xa_microspeech_inference(xa_codec_handle_t p_xa_module_obj, WORD32 i_cmd, WORD32 i_idx, pVOID pv_value)
{
#ifdef XAF_PROFILE
    clk_t comp_start, comp_stop;
#endif
    XA_ERRORCODE error_code = XA_NO_ERROR;

#ifdef XAF_PROFILE
    if (i_cmd == XA_API_CMD_EXECUTE && i_idx == XA_CMD_TYPE_DO_EXECUTE)
    {
        comp_start = clk_read_start(CLK_SELN_THREAD);
    }
#endif

    error_code = xa_tflm_inference_api(p_xa_module_obj, i_cmd, i_idx, pv_value);

    if (i_cmd == XA_API_CMD_INIT && i_idx == XA_CMD_TYPE_INIT_API_PRE_CONFIG_PARAMS)
    {
        xaf_tflm_inference_spec_t inference_spec;
        INIT_SPEC(inference_spec);
        xa_tflm_inference_init_spec(p_xa_module_obj, &inference_spec); 
    }

#ifdef XAF_PROFILE
    if (i_cmd == XA_API_CMD_EXECUTE && i_idx == XA_CMD_TYPE_DO_EXECUTE)
    {
        comp_stop = clk_read_stop(CLK_SELN_THREAD);
        inference_cycles += clk_diff(comp_stop, comp_start);
    }
#endif

    return map_error_code(error_code);
}

