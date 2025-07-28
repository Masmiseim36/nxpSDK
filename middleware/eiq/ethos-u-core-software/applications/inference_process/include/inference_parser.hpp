/*
 * Copyright (c) 2022 Arm Limited. All rights reserved.
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

#include "tensorflow/lite/schema/schema_generated.h"
#include "flatbuffers/flexbuffers.h"

#include <stdlib.h>
#include <string>

namespace InferenceProcess {

template <typename T, typename U>
class Array {
public:
    Array() = delete;
    Array(T *const data, U &size, size_t capacity) : _data{data}, _size{size}, _capacity{capacity} {}

    auto size() const {
        return _size;
    }

    auto capacity() const {
        return _capacity;
    }

    void push_back(const T &data) {
        _data[_size++] = data;
    }

private:
    T *const _data;
    U &_size;
    const size_t _capacity{};
};

template <typename T, typename U>
Array<T, U> makeArray(T *const data, U &size, size_t capacity) {
    return Array<T, U>{data, size, capacity};
}

struct ModelInfo{
    std::vector<size_t> inputDims;
    std::vector<size_t> outputDims;

    std::vector<std::vector<size_t>> inputShapes;
    std::vector<std::vector<size_t>> outputShapes;

    std::vector<int> inputTypes;
    std::vector<int> outputTypes;

    std::vector<int32_t> inputDataOffset;
    std::vector<int32_t> outputDataOffset;

    bool isVelaModel;
};

class InferenceParser {
public:
    template <typename T, typename U, size_t S>
    bool parseModel(const void *buffer, char (&description)[S], T &&ifmDims, U &&ofmDims) {
        // Create model handle
        const tflite::Model *model = tflite::GetModel(buffer);
        if (model->subgraphs() == nullptr) {
            printf("Warning: nullptr subgraph\n");
            return true;
        }

        strncpy(description, model->description()->c_str(), S - 1);
        description[S - 1] = '\0';

        // Get input dimensions for first subgraph
        auto *subgraph = *model->subgraphs()->begin();
        bool failed    = getSubGraphDims(subgraph, subgraph->inputs(), ifmDims);
        if (failed) {
            return true;
        }

        // Get output dimensions for last subgraph
        subgraph = *model->subgraphs()->rbegin();
        failed   = getSubGraphDims(subgraph, subgraph->outputs(), ofmDims);
        if (failed) {
            return true;
        }

        return false;
    }

    #define OFFLINE_MEM_ALLOC_METADATA "OfflineMemoryAllocation"
    template <size_t S>
    bool getModelInfo(const void *buffer, char (&description)[S], ModelInfo &info) {
        info.isVelaModel = false;
        // Create model handle
        const tflite::Model *model = tflite::GetModel(buffer);
        if (model->subgraphs() == nullptr) {
            return true;
        }

        strncpy(description, model->description()->c_str(), S - 1);
        description[S - 1] = '\0';

        //Get adress offset
        auto *md = model->metadata();
        const int32_t* address_offsets = NULL;
        if (md) {
            for (uint32_t mid=0; mid < md->size(); ++mid) {
                const auto meta = md->Get(mid);
                if (meta->name()->str() != OFFLINE_MEM_ALLOC_METADATA)
                    continue;
                // grab raw buffer and dump it..
                auto meta_vec = model->buffers()->Get(meta->buffer())->data();
                address_offsets = reinterpret_cast<const int32_t*>(meta_vec->data() + 12);
                info.isVelaModel = true;
                }
        }

        //Get input info
        auto *subgraph = *model->subgraphs()->begin();
        auto tensorMap = subgraph->inputs();
        if (subgraph == nullptr || tensorMap == nullptr) {
            return true;
        }

        for (auto index = tensorMap->begin(); index != tensorMap->end(); ++index) {
            auto tensor = subgraph->tensors()->Get(*index);
            auto shape = tensor->shape();
            size_t size = 1;

            std::vector<size_t> tmp;
            for (auto it = shape->begin(); it != shape->end(); ++it) {
                tmp.push_back(*it);
                size *= *it;
            }
            size_t tensorSize;
            bool failed = getTensorTypeSize(tensor->type(), tensorSize);
            if (failed) {
                return true;
            }

            size *= tensorSize;

            info.inputTypes.push_back(tensor->type());
            info.inputDims.push_back(size);
            info.inputShapes.push_back(tmp);
            if (address_offsets != NULL) {
                info.inputDataOffset.push_back(address_offsets[*index]);
            }
        }

        //Get output info
        subgraph = *model->subgraphs()->rbegin();
        tensorMap = subgraph->outputs();
        if (subgraph == nullptr || tensorMap == nullptr) {
            return true;
        }

        auto op = subgraph->operators()->end() - 1;
        auto opcode = model->operator_codes()->Get(op->opcode_index());
        if (opcode->builtin_code() == tflite::BuiltinOperator_CUSTOM
               && opcode->custom_code()->str() == "TFLite_Detection_PostProcess") {
            constexpr int kBatchSize = 1;
            constexpr int kNumCoordBox = 4;
            auto *data = op->custom_options();
            const flexbuffers::Map& m = flexbuffers::GetRoot(data->data(), data->size()).AsMap();
            const size_t num_detected_boxes =
                    m["max_detections"].AsInt32() * m["max_classes_per_detection"].AsInt32();

            size_t size;
            bool failed = getTensorTypeSize(tflite::TensorType::TensorType_FLOAT32, size);
            if (failed) {
                return true;
            }

            info.outputShapes.push_back({kBatchSize, num_detected_boxes, kNumCoordBox});
            info.outputDims.push_back(size * kBatchSize * num_detected_boxes * kNumCoordBox);
            info.outputShapes.push_back({kBatchSize, num_detected_boxes});
            info.outputDims.push_back(size * kBatchSize * num_detected_boxes);
            info.outputShapes.push_back({kBatchSize, num_detected_boxes});
            info.outputDims.push_back(size * kBatchSize * num_detected_boxes);
            info.outputShapes.push_back({1});
            info.outputDims.push_back(size);

            for (auto index = tensorMap->begin(); index != tensorMap->end(); ++index) {
                if (address_offsets != NULL) {
                    info.outputDataOffset.push_back(address_offsets[*index]);
                }
                info.outputTypes.push_back(tflite::TensorType::TensorType_FLOAT32);
            }
        } else {
            for (auto index = tensorMap->begin(); index != tensorMap->end(); ++index) {
                auto tensor = subgraph->tensors()->Get(*index);
                auto shape = tensor->shape();
                size_t size = 1;

                std::vector<size_t> tmp;
                for (auto it = shape->begin(); it != shape->end(); ++it) {
                    tmp.push_back(*it);
                    size *= *it;
                }
                size_t tensorSize;
                bool failed = getTensorTypeSize(tensor->type(), tensorSize);
                if (failed) {
                    return true;
                }
                size *= tensorSize;

                info.outputTypes.push_back(tensor->type());
                info.outputDims.push_back(size);
                info.outputShapes.push_back(tmp);
                if (address_offsets != NULL) {
                    info.outputDataOffset.push_back(address_offsets[*index]);
                }
            }
        }

        return false;
    }


private:
    bool getShapeSize(const flatbuffers::Vector<int32_t> *shape, size_t &size) {
        size = 1;

        if (shape == nullptr) {
            printf("Warning: nullptr shape size.\n");
            return true;
        }

        if (shape->Length() == 0) {
            printf("Warning: shape zero length.\n");
            return true;
        }

        for (auto it = shape->begin(); it != shape->end(); ++it) {
            size *= *it;
        }

        return false;
    }

    bool getTensorTypeSize(const enum tflite::TensorType type, size_t &size) {
        switch (type) {
        case tflite::TensorType::TensorType_UINT8:
        case tflite::TensorType::TensorType_INT8:
            size = 1;
            break;
        case tflite::TensorType::TensorType_INT16:
            size = 2;
            break;
        case tflite::TensorType::TensorType_INT32:
        case tflite::TensorType::TensorType_FLOAT32:
            size = 4;
            break;
        default:
            printf("Warning: Unsupported tensor type\n");
            return true;
        }

        return false;
    }

    template <typename T>
    bool getSubGraphDims(const tflite::SubGraph *subgraph, const flatbuffers::Vector<int32_t> *tensorMap, T &dims) {
        if (subgraph == nullptr || tensorMap == nullptr) {
            printf("Warning: nullptr subgraph or tensormap.\n");
            return true;
        }

        if ((dims.capacity() - dims.size()) < tensorMap->size()) {
            printf("Warning: tensormap size is larger than dimension capacity.\n");
            return true;
        }

        for (auto index = tensorMap->begin(); index != tensorMap->end(); ++index) {
            auto tensor = subgraph->tensors()->Get(*index);
            size_t size;
            size_t tensorSize;

            bool failed = getShapeSize(tensor->shape(), size);
            if (failed) {
                return true;
            }

            failed = getTensorTypeSize(tensor->type(), tensorSize);
            if (failed) {
                return true;
            }

            size *= tensorSize;

            if (size > 0) {
                dims.push_back(size);
            }
        }

        return false;
    }
};

} // namespace InferenceProcess
