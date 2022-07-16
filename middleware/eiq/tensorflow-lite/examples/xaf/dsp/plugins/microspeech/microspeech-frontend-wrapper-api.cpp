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

#ifndef TENSORFLOW_LITE_MICRO_EXAMPLES_MICRO_SPEECH_MAIN_FUNCTIONS_H_

#define TENSORFLOW_LITE_MICRO_EXAMPLES_MICRO_SPEECH_MAIN_FUNCTIONS_H_



// Expose a C friendly interface for main functions.

#ifdef __cplusplus

extern "C" {

#endif



//void microspeech_setup();

int  microspeech_frontend_init();

int  microspeech_frontend_process(void *pIn, void *pOut);



#ifdef __cplusplus

}

#endif



#endif  // TENSORFLOW_LITE_MICRO_EXAMPLES_MICRO_SPEECH_MAIN_FUNCTIONS_H_



#include "tensorflow/lite/micro/examples/micro_speech/audio_provider.h"

#include "tensorflow/lite/micro/examples/micro_speech/command_responder.h"

#include "tensorflow/lite/micro/examples/micro_speech/feature_provider.h"

#include "tensorflow/lite/micro/examples/micro_speech/micro_features/micro_model_settings.h"

//#include "tensorflow/lite/micro/examples/micro_speech/micro_features/tiny_conv_micro_features_model_data.h"

#include "tensorflow/lite/micro/examples/micro_speech/recognize_commands.h"

#include "tensorflow/lite/micro/kernels/micro_ops.h"

#include "tensorflow/lite/micro/micro_error_reporter.h"

#include "tensorflow/lite/micro/micro_interpreter.h"

#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"

#include "tensorflow/lite/schema/schema_generated.h"

#include "tensorflow/lite/c/common.h"

#include "tensorflow/lite/micro/examples/micro_speech/micro_features/micro_features_generator.h"



// Globals, used for compatibility with Arduino-style sketches.

namespace {

}  // namespace

int  microspeech_frontend_init()
{
    TfLiteStatus init_status = InitializeMicroFeatures(nullptr);

    return init_status;
}



int  microspeech_frontend_process(void *pIn, void *pOut)
{
    size_t num_samples_read;

    TfLiteStatus generate_status = GenerateMicroFeatures(
            nullptr, (const int16_t* )pIn, kMaxAudioSampleSize, kFeatureSliceSize,
            (int8_t*)pOut, &num_samples_read);

    return generate_status;
}

