/* Copyright 2018 The TensorFlow Authors. All Rights Reserved.

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

#include "tensorflow/lite/profiling/time.h"

#if defined(_MSC_VER)
#include <chrono>  // NOLINT(build/c++11)
#include <thread>  // NOLINT(build/c++11)
#elif defined(__ICCARM__) || defined(__ARMCC_VERSION)
#include <time.h>
#else
#include <sys/time.h>
#if defined(_POSIX_TIMERS)
#include <time.h>
#endif
#endif

namespace tflite {
namespace profiling {
namespace time {

#if defined(_MSC_VER)

uint64_t NowMicros() {
  return static_cast<uint64_t>(
      std::chrono::duration_cast<std::chrono::microseconds>(
          std::chrono::system_clock::now().time_since_epoch())
          .count());
}

void SleepForMicros(uint64_t micros) {
  std::this_thread::sleep_for(std::chrono::microseconds(micros));
}

#else
#if defined(__ARMCC_VERSION)
uint64_t NowMicros() {
  return (static_cast<uint64_t>(clock()) * 1000000) / CLOCKS_PER_SEC;
}
#elif defined(__ICCARM__)
uint64_t NowMicros() {
  struct timespec ts;
  timespec_get(&ts, TIME_UTC);
  return static_cast<uint64_t>(ts.tv_sec) * 1000000 
    + ts.tv_nsec / 1000;
}
#else
uint64_t NowMicros() {
  struct timeval tv;
  gettimeofday(&tv, nullptr);
  return static_cast<uint64_t>(tv.tv_sec) * 1e6 + tv.tv_usec;
}
#endif

void SleepForMicros(uint64_t micros) {
#if defined(_POSIX_TIMERS)
  timespec sleep_time;
  sleep_time.tv_sec = micros / 1e6;
  micros -= sleep_time.tv_sec * 1e6;
  sleep_time.tv_nsec = micros * 1e3;
  nanosleep(&sleep_time, nullptr);
#else
  uint64_t time = NowMicros();
  while(NowMicros() - time < micros){};
#endif
}

#endif  // defined(_MSC_VER)

}  // namespace time
}  // namespace profiling
}  // namespace tflite
