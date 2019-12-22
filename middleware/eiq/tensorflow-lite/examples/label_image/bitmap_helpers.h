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

#ifndef TENSORFLOW_LITE_EXAMPLES_BITMAP_HELPERS_H_
#define TENSORFLOW_LITE_EXAMPLES_BITMAP_HELPERS_H_

#include "bitmap_helpers_impl.h"
#include "label_image.h"

namespace tflite {
namespace label_image {

uint8_t* ReadBmp(const char* input_bmp_data, size_t input_bmp_len, int* width, int* height,
                 int* channels, Settings* s);

template <class T>
void Resize(T* out, uint8_t* in, int image_height, int image_width,
            int image_channels, int wanted_height, int wanted_width,
            int wanted_channels, Settings* s);

// explicit instantiation
template void Resize<uint8_t>(uint8_t*, unsigned char*, int, int, int, int, int,
                              int, Settings*);
template void Resize<float>(float*, unsigned char*, int, int, int, int, int,
                            int, Settings*);

}  // namespace label_image
}  // namespace tflite

#endif  // TENSORFLOW_LITE_EXAMPLES_BITMAP_HELPERS_H_
