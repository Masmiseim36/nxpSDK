/* Copyright 2017 The TensorFlow Authors. All Rights Reserved.
   Copyright 2020 NXP

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

#include <algorithm>
#include <functional>
#include <queue>

#include "get_top_n.h"

// Returns the top N confidence values over threshold in the provided vector,
// sorted by confidence in descending order.
void MODEL_GetTopN(const uint8_t* tensorData, int tensorSize, tensor_type_t tensorType,
                   size_t numResults, float threshold,
                   std::vector<std::pair<float, int>>* topResults)
{
  // Will contain top N results in ascending order.
  std::priority_queue<std::pair<float, int>, std::vector<std::pair<float, int>>,
                      std::greater<std::pair<float, int>>>
      top_result_pq;

  const long count = tensorSize;
  for (int i = 0; i < count; ++i) {
    float value;
    switch (tensorType) {
      case kTensorType_FLOAT32: {
        const float* predictions = reinterpret_cast<const float*>(tensorData);
        value = predictions[i];
        break;
      }
      case kTensorType_UINT8: {
        const uint8_t* predictions = tensorData;
        value = predictions[i] / 255.0;
        break;
      }
      case kTensorType_INT8: {
        const int8_t* predictions = reinterpret_cast<const int8_t*>(tensorData);
        value = ((int)predictions[i] + 128) / 255.0;
        break;
      }
    }
    // Only add it if it beats the threshold and has a chance at being in
    // the top N.
    if (value < threshold) {
      continue;
    }

    top_result_pq.push(std::pair<float, int>(value, i));

    // If at capacity, kick the smallest value out.
    if (top_result_pq.size() > numResults) {
      top_result_pq.pop();
    }
  }

  // Copy to output vector and reverse into descending order.
  while (!top_result_pq.empty()) {
    topResults->push_back(top_result_pq.top());
    top_result_pq.pop();
  }
  std::reverse(topResults->begin(), topResults->end());
}
