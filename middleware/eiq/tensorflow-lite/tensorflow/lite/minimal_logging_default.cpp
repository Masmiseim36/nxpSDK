/* Copyright 2019 The TensorFlow Authors. All Rights Reserved.

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

#include "tensorflow/lite/minimal_logging.h"

#include <cstdio>

namespace tflite {
namespace logging_internal {

void MinimalLogger::LogFormatted(LogSeverity severity, const char* format,
                                 va_list args) {
  fprintf(stderr, "%s: ", GetSeverityName(severity));
#if defined(__GNUC__)
  /* anonymous unions are enabled by default */
#elif !defined(__IAR_SYSTEMS_ICC__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wformat-nonliteral"
#else
  #pragma language=extended
#endif   
  vfprintf(stderr, format, args);
#if defined(__GNUC__)
  /* anonymous unions are enabled by default */
#elif !defined(__IAR_SYSTEMS_ICC__)
#pragma clang diagnostic pop
#endif  
  fputc('\n', stderr);
}

}  // namespace logging_internal
}  // namespace tflite
