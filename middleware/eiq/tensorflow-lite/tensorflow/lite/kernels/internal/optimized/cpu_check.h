/* Copyright 2017 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

/* File modified by NXP. Changes are described in file
   /middleware/eiq/tensorflow-lite/readme.txt in section "Release notes" */

#ifndef TENSORFLOW_LITE_KERNELS_INTERNAL_OPTIMIZED_CPU_CHECK_H_
#define TENSORFLOW_LITE_KERNELS_INTERNAL_OPTIMIZED_CPU_CHECK_H_

#include "tensorflow/lite/kernels/cpu_backend_context.h"
//#include "tensorflow/lite/kernels/internal/optimized/neon_check.h"

namespace tflite {

struct CpuFlags {
  bool neon_dotprod = false;
};

inline void GetCpuFlags(CpuBackendContext* cpu_backend_context,
                        CpuFlags* cpu_flags) {
#if defined(TFLITE_MCU)
#define RUY_PLATFORM(X) (0)
#endif
#if RUY_PLATFORM(ARM)
  ruy::Context* ruy_context = cpu_backend_context->ruy_context();
  cpu_flags->neon_dotprod =
      ruy_context != nullptr && (ruy_context->GetRuntimeEnabledPaths() &
                                 ruy::Path::kNeonDotprod) != ruy::Path::kNone;
#else
  cpu_flags->neon_dotprod = false;
#endif
}

}  // namespace tflite

#endif  // TENSORFLOW_LITE_KERNELS_INTERNAL_OPTIMIZED_CPU_CHECK_H_
