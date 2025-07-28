/*
 * Copyright (c) 2019-2022 Arm Limited. All rights reserved.
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

#pragma once

#include <array>
#include <queue>
#include <stdlib.h>
#include <string>
#include <vector>

#include "ethosu_driver.h"
#include "ethosu_monitor.hpp"

struct TfLiteTensor;

namespace tflite {
// Forward declarations
class MicroInterpreter;
class MicroResourceVariables;
} // namespace tflite

namespace InferenceProcess {
struct DataPtr {
    void *data;
    size_t size;

    DataPtr(void *data = nullptr, size_t size = 0);

    void invalidate();
    void clean();

    char *begin() const;
    char *end() const;
};

struct InferenceJob {
    std::string name;
    DataPtr networkModel;
    std::vector<DataPtr> input;
    std::vector<DataPtr> output;
    std::vector<DataPtr> expectedOutput;
    std::vector<uint8_t> pmuEventConfig;
    EthosUMonitor ethosuMonitor;
    struct ethosu_driver *ethosuDriver;
    uint64_t cpuCycles{0};
    size_t numBytesToPrint;
    void *externalContext;
    uint32_t flashOffset;
    uint32_t arenaOffset;
    bool isEthosuOp;

    InferenceJob();
    InferenceJob(const std::string &name,
                 const DataPtr &networkModel,
                 const std::vector<DataPtr> &input,
                 const std::vector<DataPtr> &output,
                 const std::vector<DataPtr> &expectedOutput,
                 const std::vector<uint8_t> &pmuEventConfig,
                 const uint32_t pmuCycleCounterEnable,
                 struct ethosu_driver *ethosuDriver,
                 const size_t numBytesToPrint = 0,
                 void *externalContext        = nullptr,
                 const uint32_t flashOffset = 0,
                 const uint32_t arenaOffset = 0,
                 bool isEthosuOp = false);

    void invalidate();
    void clean();
};

class InferenceProcess {
public:
    InferenceProcess(uint8_t *_tensorArena, size_t _tensorArenaSize);

    bool runJob(InferenceJob &job);

private:
    static bool copyIfm(InferenceJob &job, tflite::MicroInterpreter &interpreter);
    static bool copyOfm(InferenceJob &job, tflite::MicroInterpreter &interpreter);
    static bool compareOfm(InferenceJob &job, tflite::MicroInterpreter &interpreter);
    static void printJob(InferenceJob &job, tflite::MicroInterpreter &interpreter);
    static void printOutputTensor(TfLiteTensor *output, size_t bytesToPrint);
    static void tfluDebugLog(const char *s);
    bool runModel(InferenceJob &job);
    bool runEthosuOp(InferenceJob &job);

    uint8_t *tensorArena;
    const size_t tensorArenaSize;
};
} // namespace InferenceProcess
