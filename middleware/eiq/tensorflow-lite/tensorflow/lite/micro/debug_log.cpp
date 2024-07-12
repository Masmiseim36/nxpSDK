/* Copyright 2020 The TensorFlow Authors. All Rights Reserved.
   Copyright 2022-2024 NXP

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

#include "tensorflow/lite/micro/debug_log.h"

#ifndef TF_LITE_STRIP_ERROR_STRINGS
extern "C" {
#include "fsl_debug_console.h"
}
#endif

extern "C" void DebugLog(const char* format, va_list args) {
#ifndef TF_LITE_STRIP_ERROR_STRINGS
  // Reusing TF_LITE_STRIP_ERROR_STRINGS to disable DebugLog completely to get
  // maximum reduction in binary size. This is because we have DebugLog calls
  // via TF_LITE_CHECK that are not stubbed out by TF_LITE_REPORT_ERROR.
#if SDK_DEBUGCONSOLE == DEBUGCONSOLE_DISABLE /* Disable debug console */
#elif SDK_DEBUGCONSOLE == DEBUGCONSOLE_REDIRECT_TO_SDK /* Select printf, scanf, putchar, getchar of SDK version. */
  DbgConsole_Vprintf(format, args);
#elif SDK_DEBUGCONSOLE == \
    DEBUGCONSOLE_REDIRECT_TO_TOOLCHAIN /* Select printf, scanf, putchar, getchar of toolchain. \ */
  vprintf(format, args);
#endif /* SDK_DEBUGCONSOLE */
#endif
}
