/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <iostream>
#include <memory>
#include <stdio.h>
#include <vector>

#include <executorch/backends/nxp/runtime/NeutronDriver.h>
#include <executorch/extension/data_loader/buffer_data_loader.h>
#include <executorch/extension/evalue_util/print_evalue.h>
#include <executorch/extension/runner_util/inputs.h>
#include <executorch/runtime/executor/method.h>
#include <executorch/runtime/executor/program.h>
#include <executorch/runtime/platform/log.h>
#include <executorch/runtime/platform/platform.h>
#include <executorch/runtime/platform/runtime.h>

#include "board_init.h"
#include "demo_config.h"
#include "fsl_debug_console.h"
#include "image_data.h"
#include "labels.h"
#include "model_pte.h"
#include "timer.h"

using executorch::aten::ScalarType;
using executorch::aten::Tensor;
using executorch::aten::TensorImpl;
using executorch::extension::BufferCleanup;
using executorch::extension::BufferDataLoader;
using executorch::runtime::Error;
using executorch::runtime::EValue;
using executorch::runtime::HierarchicalAllocator;
using executorch::runtime::MemoryAllocator;
using executorch::runtime::MemoryManager;
using executorch::runtime::Method;
using executorch::runtime::MethodMeta;
using executorch::runtime::Program;
using executorch::runtime::Result;
using executorch::runtime::Span;
using executorch::runtime::Tag;
using executorch::runtime::TensorInfo;

static uint8_t method_allocator_pool[512 * 1024U] __ALIGNED(16) __attribute__((section("NonCacheable")));
static uint8_t temp_allocator_pool[512 * 1024U] __ALIGNED(16) __attribute__((section("NonCacheable")));

class NMemoryAllocator : public executorch::runtime::MemoryAllocator {
    public:
        NMemoryAllocator(uint32_t size, uint8_t* base_address)
            : MemoryAllocator(size, base_address), used_(0) {}

        void* allocate(size_t size, size_t alignment = kDefaultAlignment) override {
            void* ret = executorch::runtime::MemoryAllocator::allocate(size, alignment);
            if (ret != nullptr) {
                if ((size & (alignment - 1)) == 0) {
                    used_ += size;
                } else {
                    used_ = (used_ | (alignment - 1)) + 1 + size;
                }
            }
            return ret;
        }

        // Returns the used size of the allocator's memory buffer.
        size_t used_size() const {
            return used_;
        }

        // Returns the free size of the allocator's memory buffer.
        size_t free_size() const {
            return executorch::runtime::MemoryAllocator::size() - used_;
        }

    private:
        size_t used_;
};

typedef struct {
    float score;
    int index;
} result_t;

void et_pal_init(void) {}

int main(void)
{
    BOARD_Init();
    TIMER_Init();

    neutronInit();

    executorch::runtime::runtime_init();

    PRINTF("CIFARNET example using a ExecuTorch model\r\n");
    auto loader = BufferDataLoader(model_pte, sizeof(model_pte));

    Result<Program> program = Program::load(&loader);
    if (!program.ok()) {
        PRINTF("Program loading failed\r\n");
    }
    PRINTF("Model buffer loaded, has %d methods\r\n", program->num_methods());

    const char* method_name = nullptr;
    {
        const auto method_name_result = program->get_method_name(0);
        method_name = *method_name_result;
    }
    PRINTF("Running method %s\r\n", method_name);

    Result<MethodMeta> method_meta = program->method_meta(method_name);
    if (!method_meta.ok()) {
        PRINTF("Failed to get method_meta for %s: 0x%x\r\n",
	    method_name, (unsigned int)method_meta.error());
    }

    NMemoryAllocator method_allocator{
        NMemoryAllocator(sizeof(method_allocator_pool), method_allocator_pool)};
    NMemoryAllocator temp_allocator{
        NMemoryAllocator(sizeof(temp_allocator_pool), temp_allocator_pool)};

    std::vector<uint8_t*> planned_buffers; // Owns the memory
    std::vector<Span<uint8_t>> planned_spans; // Passed to the allocator
    size_t num_memory_planned_buffers = method_meta->num_memory_planned_buffers();

    for (size_t id = 0; id < num_memory_planned_buffers; ++id) {
        size_t buffer_size = static_cast<size_t>(method_meta->memory_planned_buffer_size(id).get());
        PRINTF("Setting up planned buffer %zu, size %zu.\r\n", id, buffer_size);

        uint8_t* buffer =
            reinterpret_cast<uint8_t*>(method_allocator.allocate(buffer_size));
        planned_buffers.push_back(buffer);
        planned_spans.push_back({planned_buffers.back(), buffer_size});
    }

    HierarchicalAllocator planned_memory({planned_spans.data(), planned_spans.size()});

    MemoryManager memory_manager(&method_allocator, &planned_memory, &temp_allocator);

    Result<Method> method = program->load_method(method_name, &memory_manager);
    if (!method.ok()) {
        PRINTF("Loading of method %s failed with status 0x%\r\n" PRIx32,
	    method_name, method.error());
    }
    PRINTF("Method loaded.\r\n");

    PRINTF("Preparing inputs...\r\n");
    Tensor::SizesType sizes[] = {1, 3, 32, 32};
    Tensor::DimOrderType dim_order[] = {0, 1, 2, 3};

    TensorImpl impl(ScalarType::Float, 4, sizes, image_data, dim_order);
    Tensor tensor(&impl);
    Error status = method->set_input(tensor, 0);
    if (status != Error::Ok) {
        PRINTF("Preparing inputs tensors for method %s failed with status 0x%...\r\n",
	       method_name, status);
    }
    PRINTF("Input prepared. \r\n");

    PRINTF("Starting the model execution...\r\n");

    auto startTime = TIMER_GetTimeInUS();
    status = method->execute();
    if (status != Error::Ok) {
	PRINTF("Execution of method %s failed with status 0x%\r\n" PRIx32,
               method_name, status);
    } else {
        PRINTF("Model executed successfully.\r\n");
    }
    auto endTime = TIMER_GetTimeInUS();

    PRINTF("Core/NPU Frequency: %d MHz\r\n", CLOCK_GetFreq(kCLOCK_CoreSysClk)/1000000);
    PRINTF("method_allocator Addr: 0x%x - 0x%x\r\n", method_allocator_pool, method_allocator_pool + method_allocator.size());
    PRINTF("method_allocator_used: Total 0x%x (%d B); Used 0x%x (%d B); Used/Total %d %%\r\n",
           method_allocator.size(), method_allocator.size(), method_allocator.used_size(), method_allocator.used_size(),
	   100 * method_allocator.used_size() / method_allocator.size());
    PRINTF("temp_allocator Addr: 0x%x - 0x%x\r\n", temp_allocator_pool, temp_allocator_pool + temp_allocator.size());
    PRINTF("temp_allocator_used: Total 0x%x (%d B); Used 0x%x (%d B); Used/Total %d %%\r\n",
           temp_allocator.size(), temp_allocator.size(), temp_allocator.used_size(), temp_allocator.used_size(),
	   100 * temp_allocator.used_size() / temp_allocator.size());
    PRINTF("Model Addr: 0x%x - 0x%x\r\n", model_pte, model_pte + sizeof(model_pte));
    PRINTF("Model Size: 0x%x (%d B)\r\n", sizeof(model_pte), sizeof(model_pte));
    PRINTF("Total Size Used: %d B (Model (%d B) + method_allocator (%d B) + temp_allocator (%d B))\r\n",
           (sizeof(model_pte) + method_allocator.used_size() + temp_allocator.used_size()), sizeof(model_pte),
	   method_allocator.used_size(), temp_allocator.used_size());

    std::vector<EValue> outputs(method->outputs_size());
    PRINTF("%zu outputs: \r\n", outputs.size());
    status = method->get_outputs(outputs.data(), outputs.size());
    ET_CHECK(status == Error::Ok);

    result_t topResults[NUM_RESULTS];
    for (int i = 0; i < NUM_RESULTS; i++) {
        topResults[i] = {.score = 0.0f, .index = -1};
    }

    for (int i = 0; i < outputs[0].toTensor().numel(); i++) {
        float value = 0.0f;
        if (outputs[0].toTensor().scalar_type() == ScalarType::Float) {
	    value = outputs[0].toTensor().const_data_ptr<float>()[i];
        }

        if (value < (float)DETECTION_TRESHOLD/100) {
	    continue;
        }
        result_t pass = {.score = 0.0f, .index = -1};
        for (int n = 0; n < NUM_RESULTS; n++) {
            if (pass.index >= 0) {
                result_t swap = topResults[n];
                topResults[n] = pass;
                pass = swap;
            } else if (topResults[n].score < value) {
                pass = topResults[n];
                topResults[n] = {.score = value, .index = i};
            }
        }
    }

    const char* label = "No label detected";
    float confidence = 0;

    if (topResults[0].index >= 0) {
        auto result = topResults[0];
        confidence = result.score;
        int index = result.index;
        if (confidence * 100 > DETECTION_TRESHOLD)
            label = labels[index];
    }

    int score = (int)(confidence * 100);
    PRINTF("----------------------------------------\r\n");
    PRINTF("     Inference time: %d us\r\n", endTime - startTime);
    PRINTF("     Detected: %s (%d%%)\r\n", label, score);
    PRINTF("----------------------------------------\r\n");

    for (int i = 0; i < (int)outputs.size(); ++i) {
        Tensor t = outputs[i].toTensor();
        for (int j = 0; j < outputs[i].toTensor().numel(); ++j) {
            if (t.scalar_type() == ScalarType::Int) {
                PRINTF("Output[%d][%d]: %d\r\n", i, j,
                    outputs[i].toTensor().const_data_ptr<int>()[j]);
            } else {
                PRINTF("Output[%d][%d]: %f\r\n", i, j,
                    outputs[i].toTensor().const_data_ptr<float>()[j]);
            }
        }
    }

    neutronDeinit();
    PRINTF("Program complete, exiting.\r\n");
    return 0;
}
