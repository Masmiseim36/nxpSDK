/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "tf_benchmark.h"
#include "get_top_n.h"

#ifndef __XCC__
#include <cmsis_compiler.h>
#else
#define __ALIGNED(x) __attribute__((aligned(x)))
#endif

tflite::ErrorReporter* error_reporter = nullptr;
const tflite::Model* model = nullptr;
tflite::MicroInterpreter* interpreter = nullptr;
TfLiteTensor* input = nullptr;
TfLiteTensor* output = nullptr;
TfLiteTensor* output_t = nullptr;
tflite::MicroErrorReporter micro_error_reporter;
constexpr int kTensorArenaSize = 9 * 1024 * 1024;
uint8_t tensor_arena[kTensorArenaSize] __ALIGNED(16);

class BProfiler : public tflite::MicroProfiler {
public:
    explicit BProfiler(NNServer* server): server_(server){}

    uint32_t BeginEvent(const char* tag) {
        start_time_ = os_clock_now();
        TFLITE_DCHECK(tag != nullptr);
        event_tag_ = tag;
        return 0;
    }

    void EndEvent(uint32_t event_handle) {
        int64_t end_time = os_clock_now() - start_time_;
        server_->run_ns += end_time;
        printf("%s - %s took %lld us\r\n", server_->output.name[server_->output.num_outputs], event_tag_, (int64_t)(end_time));
        server_->output.timing[server_->output.num_outputs] += (end_time/1e6);
        server_->output.type[server_->output.num_outputs] = (char*)event_tag_;
        server_->output.num_outputs++;}

private:
    NNServer* server_;
    int64_t start_time_;
    const char* event_tag_;
    TF_LITE_REMOVE_VIRTUAL_DELETE;
};

void MODEL_ConvertInput(uint8_t* data, int size, TfLiteType type)
{
    if (data){
        switch (type)
        {
        case kTfLiteUInt8:
            memcpy(input->data.data, data, size);
            break;
        case kTfLiteInt8:
            for (int i = size - 1; i >= 0; i--)
            {
                input->data.int8[i]  =
                    static_cast<int>(data[i]) - 127;
            }
            break;
        case kTfLiteFloat32:
            for (int i = size - 1; i >= 0; i--)
            {
                input->data.f[i] =
                    (static_cast<int>(data[i]) - 127.5f) / 127.5f;
            }
            break;
        default:
            assert("Unknown input tensor data type");
        }

    } else {
        switch (type)
        {
        case kTfLiteUInt8:
            break;
        case kTfLiteInt8:
            break;
        case kTfLiteFloat32:
            for(int i = 0; i < size; i++)
                input->data.f[i] = 3.1415926;
            break;
        default:
            assert("Unknown input tensor data type");
        }
    }
}

int Model_Setup(NNServer* server) {
    model = tflite::GetModel(server->model_upload);
    if (model->version() != TFLITE_SCHEMA_VERSION)
    {
        TF_LITE_REPORT_ERROR(&micro_error_reporter,
                             "Model provided is schema version %d not equal "
                             "to supported version %d.",
                             model->version(), TFLITE_SCHEMA_VERSION);
        return kStatus_Fail;
    }
    auto* subgraphs = model->subgraphs();
    const tflite::SubGraph* subgraph = (*subgraphs)[0];
    auto tensors = subgraph->tensors();
    const flatbuffers::Vector<flatbuffers::Offset<tflite::Operator>>* operators = subgraph->operators();
    for (int i = 0; i < operators->size(); i++) {
        const tflite::Operator* cur_operator = (*operators)[i];
        auto* outputs = cur_operator->outputs();
        for (int j=0; j<outputs->size(); j++)
        {
            int idx = (int)(*outputs)[j];
            const tflite::Tensor* tensor = (*tensors)[idx];
            char* name = (char*)tensor->name()->c_str();
            server->output.name[i] =  name;
        }
    }

    server->output.outputs_size = subgraph->outputs()->size();
    for (int i = 0; i < server->output.outputs_size; i++)
    {
        int idx = (int)(*subgraph->outputs())[i];
        auto tensor = (*tensors)[idx];
        char* name = (char*) tensor->name ()->c_str();
        for (int j = operators->size(); j >= 0; j--){
            if ( strcmp (server->output.name[j], name) == 0){
                server->output.index[i] = j;
            }
        }
    }
    return 0;
}

int Model_RunInference(NNServer* server) {
    BProfiler profiler(server);
    tflite::AllOpsResolver resolver;

    // Build an interpreter to run the model with.
    tflite::MicroInterpreter static_interpreter(model, resolver, tensor_arena, kTensorArenaSize, &micro_error_reporter, &profiler);
    interpreter = &static_interpreter;

    // Allocate memory from the tensor_arena for the model's tensors.
    TfLiteStatus allocate_status = interpreter->AllocateTensors();
    if (allocate_status != kTfLiteOk) {
        TF_LITE_REPORT_ERROR(error_reporter, "AllocateTensors() failed");
        printf("tflite allocate tensors failed.\r\n");
        return 1;
    }

    // Obtain pointers to the model's input and output tensors.
    input = interpreter->input(0);
    output = interpreter->output(0);

    server->input_dims_data = input->dims->data;

    int64_t run_ns = 0;
    for(int i=0; i< server->output.num_outputs; i++)
    {
        server->output.timing[i] = 0;
    }
    for(int i = 1; i <= server->inference_count; i++)
    {
        server->output.num_outputs = 0;
        printf("loop %d: \r\n", i);
        MODEL_ConvertInput(server->image_upload_data,
                           input->dims->data[1] * input->dims->data[3] * input->dims->data[2], input->type);
        server->run_ns = 0;
        // Run inference, and report any error
        TfLiteStatus invoke_status = interpreter->Invoke();
        if (invoke_status != kTfLiteOk) {
            TF_LITE_REPORT_ERROR(error_reporter, "Invoke failed\n");
            return -1;
        }

        printf("run ms: %f ", (float)(server->run_ns/1e6));

        run_ns += server->run_ns;

        result_t topResults[1];

        tensor_type_t outputType;

        switch (output->type)
        {
        case kTfLiteFloat32:
            outputType = kTensorType_FLOAT32;
            break;
        case kTfLiteUInt8:
            outputType = kTensorType_UINT8;
            break;
        case kTfLiteInt8:
            outputType = kTensorType_INT8;
            break;
        default:
            return -1;
            assert("Unknown input tensor data type");
        };

        for(int i=0; i<interpreter->outputs().size(); i++){
            server->output.data [i] = interpreter->output(i)->data.raw;
            server->output.bytes [i] = interpreter->output(i)->bytes;
            server->output.shape_data [i] = interpreter->output (i)->dims->data;
            server->output.shape_size [i] = interpreter->output (i)->dims->size;
            switch (interpreter->output (i)->type){
            case kTfLiteFloat32:
                strcpy(server->output.data_type [i], "FLOAT32");
                break;
            case kTfLiteUInt8:
                strcpy(server->output.data_type [i], "UINT8");
                break;
            case kTfLiteInt8:
                strcpy(server->output.data_type [i], "INT8");
                break;
            default:
                break;
            }
        }

        MODEL_GetTopN(output->data.uint8, output->dims->data[output->dims->size - 1], outputType, 1, 0, topResults);

        printf("index %d, score: %f \r\n", topResults[0].index, topResults[0].score);
        server->timing.run = topResults[0].index;
    }

    server->run_ns = (int64_t)(run_ns/(int64_t)server->inference_count);
    return 0;
}
