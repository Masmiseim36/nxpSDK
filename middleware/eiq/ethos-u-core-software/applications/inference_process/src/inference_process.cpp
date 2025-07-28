/*
 * Copyright (c) 2019-2022 Arm Limited. All rights reserved.
 * Copyright 2020-2022 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/micro/micro_log.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_time.h"
#include "tensorflow/lite/schema/schema_generated.h"

#ifdef LAYER_BY_LAYER_PROFILER
#include "layer_by_layer_profiler.hpp"
#endif

#include "crc.hpp"

#include "ethosu_log.h"
#include <ethosu_driver.h>

#include "inference_process.hpp"

#include "cmsis_compiler.h"

#include <inttypes.h>

#if ETHOSU_LOG_SEVERITY >= ETHOSU_LOG_DEBUG
#include "tensorflow/lite/micro/micro_string.h"
namespace tflite {
    class NxpMicroErrorReporter : public ErrorReporter {
    public:
    ~NxpMicroErrorReporter() override {}
    int Report(const char* format, va_list args) {
        static constexpr int kMaxLogLen = 256;
        char log_buffer[kMaxLogLen];
        int len = MicroVsnprintf(log_buffer, kMaxLogLen, format, args);
        PRINTF(log_buffer);
        PRINTF("\r\n");
        return len;
    };
    private:
    TF_LITE_REMOVE_VIRTUAL_DELETE
    };
}
#endif

using namespace std;

namespace InferenceProcess {
DataPtr::DataPtr(void *_data, size_t _size) : data(_data), size(_size) {}

void DataPtr::invalidate() {
#if defined(__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U)
    SCB_InvalidateDCache_by_Addr(reinterpret_cast<uint32_t *>(data), size);
#endif
}

void DataPtr::clean() {
#if defined(__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U)
    SCB_CleanDCache_by_Addr(reinterpret_cast<uint32_t *>(data), size);
#endif
}

char *DataPtr::begin() const {
    return static_cast<char *>(data);
}

char *DataPtr::end() const {
    return static_cast<char *>(data) + size;
}

InferenceJob::InferenceJob() : numBytesToPrint(0), externalContext(nullptr) {}

InferenceJob::InferenceJob(const string &_name,
                           const DataPtr &_networkModel,
                           const vector<DataPtr> &_input,
                           const vector<DataPtr> &_output,
                           const vector<DataPtr> &_expectedOutput,
                           const std::vector<uint8_t> &_pmuEventConfig,
                           const uint32_t _pmuCycleCounterEnable,
                           struct ethosu_driver *_ethosuDriver,
                           const size_t _numBytesToPrint,
                           void *_externalContext,
                           uint32_t _flash_offset,
                           uint32_t _arena_offset,
                           bool _isEthosuOp) :
    name(_name),
    networkModel(_networkModel), input(_input), output(_output), expectedOutput(_expectedOutput),
    pmuEventConfig(_pmuEventConfig), ethosuMonitor(_pmuCycleCounterEnable), ethosuDriver(_ethosuDriver),
    numBytesToPrint(_numBytesToPrint), externalContext(_externalContext),
    flashOffset(_flash_offset), arenaOffset(_arena_offset), isEthosuOp(_isEthosuOp) {}

void InferenceJob::invalidate() {
    networkModel.invalidate();

    for (auto &it : input) {
        it.invalidate();
    }

    for (auto &it : output) {
        it.invalidate();
    }

    for (auto &it : expectedOutput) {
        it.invalidate();
    }
}

void InferenceJob::clean() {
    networkModel.clean();

    for (auto &it : input) {
        it.clean();
    }

    for (auto &it : output) {
        it.clean();
    }

    for (auto &it : expectedOutput) {
        it.clean();
    }
}

InferenceProcess::InferenceProcess(uint8_t *_tensorArena, size_t _tensorArenaSize) :
    tensorArena(_tensorArena), tensorArenaSize(_tensorArenaSize) {}

bool InferenceProcess::runJob(InferenceJob &job) {
    bool ret;
    if (job.isEthosuOp) {
        ret = runEthosuOp(job);
    } else {
	ret = runModel(job);
    }
    return ret;
}

std::vector<ethosu_pmu_event_type> PMU_DEFAULT_CONFIG{ETHOSU_PMU_CYCLE,
                                                      ETHOSU_PMU_NPU_IDLE,
                                                      ETHOSU_PMU_NPU_ACTIVE,
                                                      ETHOSU_PMU_MAC_ACTIVE};

bool InferenceProcess::runEthosuOp(InferenceJob &job) {
#define NUM_BASE_ADDR 3 // flash,arena and fast_memory
#define PTR_TO_UINT64(ptr) (static_cast<uint64_t>(reinterpret_cast<uintptr_t>(ptr)))
    LOG_INFO("Running inference job, name: %s, type: OP", job.name.c_str());
    void *custom_data_ptr = job.networkModel.data;
    int custom_data_size = job.networkModel.size;
    uint64_t arena_addr = PTR_TO_UINT64(job.input[0].data) + job.arenaOffset;
    uint64_t base_addr[NUM_BASE_ADDR] = {0};

    base_addr[1] = arena_addr;
    if (job.input.size() == 2) {
	// Flash memory at first
        base_addr[0] = PTR_TO_UINT64(job.input[1].data) + job.flashOffset;
    } else {
	// No flash tensor
        base_addr[0] = arena_addr;
    }

    struct ethosu_driver* drv = ethosu_reserve_driver();
    if (drv == NULL) {
        return true;
    }

    job.ethosuMonitor.configure(job.ethosuDriver, job.pmuEventConfig);
    if (job.output.size() == 1) {
        auto qread_buffer = reinterpret_cast<EthosuQreadEvent*>(job.output[0].data);
        size_t buffer_size = job.output[0].size / sizeof(EthosuQreadEvent);
        int ret = ethosu_invoke_async(drv, custom_data_ptr, custom_data_size, base_addr,
                         NULL, NUM_BASE_ADDR, 0);
        if (ret < 0) {
             return true;
        }

        size_t qread_number = 0;
        while (true) {
            ret = ethosu_wait(drv, false);
            if (qread_number != buffer_size) {
                auto pre = &(qread_buffer[qread_number]);
                auto cur = &(qread_buffer[qread_number + 1]);
                job.ethosuMonitor.monitorSample(job.ethosuDriver, cur);
                if (pre->qread != cur->qread || pre->status != cur->status)
                    qread_number ++;
            }
            if (ret == 0) {
                job.output[0].size = qread_number * sizeof(EthosuQreadEvent);
                break;
            } else if (ret < 0) {
                return true;
            }
        }
    } else {
        int ret = ethosu_invoke_v3(drv, custom_data_ptr, custom_data_size, base_addr,
                         NULL, NUM_BASE_ADDR, 0);
        if (ret != 0) {
            return true;
        }
    }
    job.ethosuMonitor.monitorSample(job.ethosuDriver);
    ethosu_release_driver(drv);

    LOG_INFO("Finished running job: %s", job.name.c_str());
    return false;
}

bool InferenceProcess::runModel(InferenceJob &job) {
    LOG_INFO("Running inference job, name: %s, type: Model", job.name.c_str());

    // Get model handle and verify that the version is correct
    const tflite::Model *model = ::tflite::GetModel(job.networkModel.data);
    if (model->version() != TFLITE_SCHEMA_VERSION) {
        LOG_ERR("Model schema version unsupported: version=%" PRIu32 ", supported=%d.",
                model->version(),
                TFLITE_SCHEMA_VERSION);
        return true;
    }

    // Create the TFL micro interpreter
    tflite::MicroMutableOpResolver<3> resolver;
    tflite::MicroInterpreter interpreter(
        model, resolver, tensorArena, tensorArenaSize);

    resolver.AddEthosU();
    resolver.AddDetectionPostprocess();
    resolver.AddDequantize();

    // Set external context
    if (job.externalContext != nullptr) {
        interpreter.SetMicroExternalContext(job.externalContext);
    }

    // Allocate tensors
    TfLiteStatus status = interpreter.AllocateTensors();
    if (status != kTfLiteOk) {
        LOG_ERR("Failed to allocate tensors for inference: job=%s", job.name.c_str());
        return true;
    }

    job.ethosuMonitor.configure(job.ethosuDriver, job.pmuEventConfig);
    if (job.input.size() > 0) { // Copy by A-core when ifm is empty
        // Copy IFM data from job descriptor to TFLu arena
        if (copyIfm(job, interpreter)) {
            return true;
        }
    }

    // Get the current cycle counter value
    uint32_t cpuCyclesBegin = tflite::GetCurrentTimeTicks();

    // Run the inference
    status = interpreter.Invoke();

    // Calculate nbr of CPU cycles for the Invoke call
    job.cpuCycles = tflite::GetCurrentTimeTicks() - cpuCyclesBegin;

    if (status != kTfLiteOk) {
        LOG_ERR("Invoke failed for inference: job=%s", job.name.c_str());
        return true;
    }

    if (job.output.size() > 0) {// Copy by A-core when ofm is empty
        // Copy output data from TFLu arena to job descriptor
        if (copyOfm(job, interpreter)) {
            return true;
        }
    }
    job.ethosuMonitor.monitorSample(job.ethosuDriver);

#if ETHOSU_LOG_SEVERITY >= ETHOSU_LOG_INFO
    printJob(job, interpreter);
#endif

    LOG_INFO("\n");
    LOG_INFO("Finished running job: %s", job.name.c_str());

    return false;
}

bool InferenceProcess::copyIfm(InferenceJob &job, tflite::MicroInterpreter &interpreter) {
    // Create a filtered list of non empty input tensors
    vector<TfLiteTensor *> inputTensors;
    for (size_t i = 0; i < interpreter.inputs_size(); ++i) {
        TfLiteTensor *tensor = interpreter.input(i);

        if (tensor->bytes > 0) {
            inputTensors.push_back(tensor);
        }
    }

    if (job.input.size() != inputTensors.size()) {
        LOG_ERR("Number of input buffers does not match number of non empty network tensors: input=%zu, network=%zu",
                job.input.size(),
                inputTensors.size());
        return true;
    }

    // Copy input data from job to TFLu arena
    for (size_t i = 0; i < inputTensors.size(); ++i) {
        DataPtr &input       = job.input[i];
        TfLiteTensor *tensor = inputTensors[i];

        if (input.size != tensor->bytes) {
            LOG_ERR("Job input size does not match network input size: job=%s, index=%zu, input=%zu, network=%u",
                    job.name.c_str(),
                    i,
                    input.size,
                    tensor->bytes);
            return true;
        }

        copy(input.begin(), input.end(), tensor->data.uint8);
    }

    return false;
}

bool InferenceProcess::copyOfm(InferenceJob &job, tflite::MicroInterpreter &interpreter) {
    // Skip copy if output is empty
    if (job.output.empty()) {
        return false;
    }

    if (interpreter.outputs_size() != job.output.size()) {
        LOG_ERR("Output size mismatch: job=%zu, network=%u", job.output.size(), interpreter.outputs_size());
        return true;
    }

    for (unsigned i = 0; i < interpreter.outputs_size(); ++i) {
        DataPtr &output      = job.output[i];
        TfLiteTensor *tensor = interpreter.output(i);

        if (tensor->bytes > output.size) {
            LOG_ERR("Tensor size mismatch: tensor=%d, expected=%d", tensor->bytes, output.size);
            return true;
        }

        copy(tensor->data.uint8, tensor->data.uint8 + output.size, output.begin());
    }

    return false;
}

bool InferenceProcess::compareOfm(InferenceJob &job, tflite::MicroInterpreter &interpreter) {
    // Skip verification if expected output is empty
    if (job.expectedOutput.empty()) {
        return false;
    }

    if (job.expectedOutput.size() != interpreter.outputs_size()) {
        LOG_ERR("Expected number of output tensors mismatch: job=%s, expected=%zu, network=%zu",
                job.name.c_str(),
                job.expectedOutput.size(),
                interpreter.outputs_size());
        return true;
    }

    for (unsigned int i = 0; i < interpreter.outputs_size(); i++) {
        const DataPtr &expected    = job.expectedOutput[i];
        const TfLiteTensor *output = interpreter.output(i);

        if (expected.size != output->bytes) {
            LOG_ERR("Expected output tensor size mismatch: job=%s, index=%u, expected=%zu, network=%zu",
                    job.name.c_str(),
                    i,
                    expected.size,
                    output->bytes);
            return true;
        }

        const char *exp = expected.begin();
        for (unsigned int j = 0; j < output->bytes; ++j) {
            if (output->data.uint8[j] != exp[j]) {
                LOG_ERR("Expected output tensor data mismatch: job=%s, index=%u, offset=%u, "
                        "expected=%02x, network=%02x\n",
                        job.name.c_str(),
                        i,
                        j,
                        exp[j],
                        output->data.uint8[j]);
                return true;
            }
        }
    }

    return false;
}

void InferenceProcess::printJob(InferenceJob &job, tflite::MicroInterpreter &interpreter) {
    LOG("arena_used_bytes : %zu\r\n", interpreter.arena_used_bytes());

    // Print all of the output data, or the first NUM_BYTES_TO_PRINT bytes,
    // whichever comes first as well as the output shape.
    LOG("num_of_outputs: %d\r\n", interpreter.outputs_size());
    LOG("output_begin\r\n");
    LOG("[\r\n");

    for (unsigned int i = 0; i < interpreter.outputs_size(); i++) {
        printOutputTensor(interpreter.output(i), job.numBytesToPrint);

        if (i != interpreter.outputs_size() - 1) {
            LOG(",\r\n");
        }
    }

    LOG("]\r\n");
    LOG("output_end\r\n");
}

void InferenceProcess::printOutputTensor(TfLiteTensor *output, size_t bytesToPrint) {
    constexpr auto crc        = Crc();
    const uint32_t crc32      = crc.crc32(output->data.data, output->bytes);
    const int numBytesToPrint = min(output->bytes, bytesToPrint);
    int dims_size             = output->dims->size;

    LOG("{\r\n");
    LOG("\"dims\": [%d,", dims_size);

    for (int i = 0; i < output->dims->size - 1; ++i) {
        LOG("%d,", output->dims->data[i]);
    }

    LOG("%d],\r\n", output->dims->data[dims_size - 1]);
    LOG("\"data_address\": \"%08" PRIx32 "\",\r\n", (uint32_t)output->data.data);
    LOG("\"data_bytes\": %d,\n", output->bytes);

    if (numBytesToPrint) {
        LOG("\"crc32\": \"%08" PRIx32 "\",\r\n", crc32);
        LOG("\"data\":\"");

        for (int i = 0; i < numBytesToPrint - 1; ++i) {
            /*
             * Workaround an issue when compiling with GCC where by
             * printing only a '\n' the produced global output is wrong.
             */
            if (i % 15 == 0 && i != 0) {
                LOG("0x%02x,\r\n", output->data.uint8[i]);
            } else {
                LOG("0x%02x,", output->data.uint8[i]);
            }
        }

        LOG("0x%02x\"\r\n", output->data.uint8[numBytesToPrint - 1]);
    } else {
        LOG("\"crc32\": \"%08" PRIx32 "\"\r\n", crc32);
    }

    LOG("}");
}

void InferenceProcess::tfluDebugLog(const char *s) {
    LOG("%s", s);
}

} // namespace InferenceProcess
