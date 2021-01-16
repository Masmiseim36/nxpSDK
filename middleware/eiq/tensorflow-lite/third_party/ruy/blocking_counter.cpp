/* Copyright 2019 Google LLC. All Rights Reserved.

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

#include "ruy/blocking_counter.h"

#include "ruy/check_macros.h"
#ifndef TFLITE_MCU
#include "ruy/wait.h"
#endif

namespace ruy {

void BlockingCounter::Reset(int initial_count) {
  int old_count_value = count_.load(std::memory_order_relaxed);
  RUY_DCHECK_EQ(old_count_value, 0);
  (void)old_count_value;
  count_.store(initial_count, std::memory_order_release);
}

bool BlockingCounter::DecrementCount() {
  int old_count_value = count_.fetch_sub(1, std::memory_order_acq_rel);
  RUY_DCHECK_GT(old_count_value, 0);
  int count_value = old_count_value - 1;
  bool hit_zero = (count_value == 0);
#ifndef TFLITE_MCU
  if (hit_zero) {
    std::lock_guard<std::mutex> lock(count_mutex_);
    count_cond_.notify_all();
  }
#endif
  return hit_zero;
}

void BlockingCounter::Wait(const Duration spin_duration) {
#ifndef TFLITE_MCU
  const auto& condition = [this]() {
    return count_.load(std::memory_order_acquire) == 0;
  };
  ruy::Wait(condition, spin_duration, &count_cond_, &count_mutex_);
#endif
}

}  // namespace ruy
