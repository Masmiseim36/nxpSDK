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

#include "tensorflow/contrib/lite/context.h"
#include <stdio.h>
#include <string.h>

int TfLiteIntArrayGetSizeInBytes(int size) {
  static TfLiteIntArray dummy;
  return sizeof(dummy) + sizeof(dummy.data[0]) * size;
}

TfLiteIntArray* TfLiteIntArrayCreate(int size) {
  TfLiteIntArray* ret =
      (TfLiteIntArray*)malloc(TfLiteIntArrayGetSizeInBytes(size));
  ret->size = size;
  return ret;
}

void TfLiteIntArrayPrint(const char* s, TfLiteIntArray* a) {
  printf("%s: length=%d [", s, a->size);
  if (a->size) printf("%d", a->data[0]);
  int i = 1;
  for (; i < a->size; i++) {
    printf(" %d", a->data[i]);
  }
  printf("]\n");
}

int TfLiteIntArrayEqual(TfLiteIntArray* a, TfLiteIntArray* b) {
  if (a == b) return 1;
  if (a == NULL || b == NULL) return 0;
  if (a->size != b->size) return 0;
  int i = 0;
  for (; i < a->size; i++)
    if (a->data[i] != b->data[i]) return 0;
  return 1;
}

TfLiteIntArray* TfLiteIntArrayCopy(TfLiteIntArray* src) {
  if (!src) return NULL;
  TfLiteIntArray* ret = TfLiteIntArrayCreate(src->size);
  if (ret) {
    memcpy(ret->data, src->data, src->size * sizeof(int));
  }
  return ret;
}

void TfLiteIntArrayFree(TfLiteIntArray* a) { free(a); }

void TfLiteTensorDataFree(TfLiteTensor* t) {
  if (t->allocation_type == kTfLiteDynamic && t->data.raw) {
    free(t->data.raw);
  }
  t->data.raw = NULL;
}

void TfLiteTensorFree(TfLiteTensor* t) {
  TfLiteTensorDataFree(t);
  if (t->dims) TfLiteIntArrayFree(t->dims);
  t->dims = NULL;
}

void TfLiteTensorReset(TfLiteType type, const char* name, TfLiteIntArray* dims,
                       TfLiteQuantizationParams quantization, char* buffer,
                       size_t size, TfLiteAllocationType allocation_type,
                       const void* allocation, bool is_variable, TfLiteTensor* tensor) {
  TfLiteTensorFree(tensor);
  tensor->type = type;
  tensor->name = name;
  tensor->dims = dims;
  tensor->params = quantization;
  tensor->data.raw = buffer;
  tensor->bytes = size;
  tensor->allocation_type = allocation_type;
  tensor->allocation = allocation;
  tensor->is_variable = is_variable;
}

void TfLiteTensorRealloc(size_t num_bytes, TfLiteTensor* tensor) {
  if (tensor->allocation_type != kTfLiteDynamic) {
    return;
  }
  if (!tensor->data.raw) {
    tensor->data.raw = (char*) malloc(num_bytes);
  } else if (num_bytes > tensor->bytes) {
    tensor->data.raw = (char*) realloc(tensor->data.raw, num_bytes);
  }
  tensor->bytes = num_bytes;
}
