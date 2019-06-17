// Copyright 2018 The Gemmlowp Authors. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// kernel_dsp: a collection of ARM DSP optimized kernels.
// Check in kernel_default.h which one(s) are actually used by default.
// Others are mere experiments; they are still covered by tests
// in case they might be useful some day.

// Copyright 2017 The TensorFlow Authors. All Rights Reserved.
//   Copyright 2018 NXP. All Rights Reserved.
//
//Licensed under the Apache License, Version 2.0 (the "License");
//you may not use this file except in compliance with the License.
//You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
//Unless required by applicable law or agreed to in writing, software
//distributed under the License is distributed on an "AS IS" BASIS,
//WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//See the License for the specific language governing permissions and
//limitations under the License.
//==============================================================================

#ifndef GEMMLOWP_INTERNAL_KERNEL_DSP_H_
#define GEMMLOWP_INTERNAL_KERNEL_DSP_H_

#include "kernel.h"

namespace gemmlowp {

// Our main GEMM kernel.
struct DSP_Kernel2x2Depth2 : KernelBase {
  typedef KernelFormat<
      KernelSideFormat<CellFormat<2, 2, CellOrder::DepthMajor>, 1>,
      KernelSideFormat<CellFormat<2, 2, CellOrder::DepthMajor>, 1> >
      Format;

  const char* Name() const override { return "DSP, 2x2, depth 2"; }

  // TODO(benoitjacob): reorder function arguments so dst comes last
  void Run(std::int32_t* dst_ptr, std::size_t dst_row_stride,
           std::size_t dst_col_stride, const std::uint8_t* lhs_ptr,
           const std::uint8_t* rhs_ptr, std::size_t start_depth,
           std::size_t run_depth) const override {
    ScopedProfilingLabel label("optimized kernel (DSP 2x2)");

#ifdef __GNUC__
#define GEMMLOWP_LABEL_LOOP "gemmlowp_label_loop_%="
#define GEMMLOWP_LABEL_ACCUMULATE "gemmlowp_label_accumulate_%="
#define GEMMLOWP_LABEL_FINISH "gemmlowp_label_finish_%="
#else
#define GEMMLOWP_LABEL_LOOP "gemmlowp_label_loop"
#define GEMMLOWP_LABEL_ACCUMULATE "gemmlowp_label_accumulate"
#define GEMMLOWP_LABEL_FINISH "gemmlowp_label_finish"
#endif

    // Equal for both LSH and RSH
    const int run_depth_shift = Format::Lhs::kCells * Format::Lhs::Cell::kWidth * Format::kDepth;

    int acc0 = 0;
    int acc1 = 0;
    int acc2 = 0;
    int acc3 = 0;

    asm volatile(
      GEMMLOWP_LABEL_LOOP ":\n"
  
      // Load LHS values
      "ldr r0, [%[lhs_ptr]], %[run_depth_shift]\n"
      "uxtb16 r1, r0, ror #8\n"
      "uxtb16 r0, r0\n"
      // Load RHS values
      "ldr r2, [%[rhs_ptr]], %[run_depth_shift]\n"
      "uxtb16 r3, r2, ror #8\n"
      "uxtb16 r2, r2\n"

      // Multiply and accumulate
      "smlad %[acc0], r0, r2, %[acc0]\n"
      "smlad %[acc1], r1, r2, %[acc1]\n"
      "smlad %[acc2], r0, r3, %[acc2]\n"
      "smlad %[acc3], r1, r3, %[acc3]\n"

      "subs %[run_depth], %[run_depth], #2\n"
      "bne " GEMMLOWP_LABEL_LOOP "\n"
      : // Outputs
        [acc0] "+r"(acc0),
        [acc1] "+r"(acc1),
        [acc2] "+r"(acc2),
        [acc3] "+r"(acc3)
      : // Inputs
        [lhs_ptr] "r"(lhs_ptr),
        [rhs_ptr] "r"(rhs_ptr),
        [run_depth] "r"(run_depth),
        [run_depth_shift] "i"(run_depth_shift)
      : // Clobbers
        "cc", "r0", "r1", "r2", "r3");

    asm volatile(
      "lsl %[dst_col_stride], %[dst_col_stride], #2\n"
      "cmp %[start_depth], #0\n"
      "bne " GEMMLOWP_LABEL_ACCUMULATE "\n"

      // Write accumulators back (overwrite)
      "strd %[acc0], %[acc1], [%[dst_ptr]]\n"
      "add %[dst_ptr], %[dst_col_stride]\n"
      "strd %[acc2], %[acc3], [%[dst_ptr]]\n"
      "b " GEMMLOWP_LABEL_FINISH "\n"

      GEMMLOWP_LABEL_ACCUMULATE ":\n"

      // Write accumulators back (accumulate)
      "ldrd r0, r1, [%[dst_ptr]]\n"
      "add %[acc0], r0\n"
      "add %[acc1], r1\n"
      "strd %[acc0], %[acc1], [%[dst_ptr]]\n"
      "add %[dst_ptr], %[dst_col_stride]\n"
      "ldrd r0, r1, [%[dst_ptr]]\n"
      "add %[acc2], r0\n"
      "add %[acc3], r1\n"
      "strd %[acc2], %[acc3], [%[dst_ptr]]\n"

      GEMMLOWP_LABEL_FINISH ":\n"
      : // Outputs
      : // Inputs
        [start_depth] "r"(start_depth),
        [dst_ptr] "r"(dst_ptr),
        [dst_col_stride] "r"(dst_col_stride),
        [acc0] "r"(acc0),
        [acc1] "r"(acc1),
        [acc2] "r"(acc2),
        [acc3] "r"(acc3)
      : // Clobbers
        "cc", "r0", "r1", "r2", "r3");

#undef GEMMLOWP_LABEL_LOOP
#undef GEMMLOWP_LABEL_ACCUMULATE
#undef GEMMLOWP_LABEL_FINISH

  }
};
  
}  // namespace gemmlowp

#endif  // GEMMLOWP_INTERNAL_KERNEL_DSP_H_
