/*
 * Copyright 2019-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "glow_bundle_utils.h"
#include <stdlib.h>
#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

// Dynamic memory allocation of aligned memory.
void* alignedAlloc(align_t alignment, size_t size) {
  size_t alloc_size;
  void *base_ptr;
  void *aligned_ptr;

  // Alignment should NOT be 0
  if (alignment == 0)
    return NULL;

  // Allocate memory using legacy "malloc" with the extra of:
  // - margin of (alignment - 1) bytes required to align the pointer
  // - 1 byte required to explicitly store the alignment offset
  alloc_size = size + alignment - 1 + sizeof(alignment);
  base_ptr = malloc(alloc_size);
  if(base_ptr == NULL)
    return NULL;
  aligned_ptr = (void*) ((((uintptr_t) base_ptr + sizeof(alignment)
      + alignment - 1) / alignment) * alignment);
  *((align_t*) aligned_ptr - 1) = (align_t) ((uintptr_t) aligned_ptr
      - (uintptr_t) base_ptr);
  return aligned_ptr;
}

// Dynamic memory de-allocation of aligned memory.
void alignedFree(void *ptr) {
  align_t offset;
  if (ptr == NULL)
    return;
  offset = *((align_t*) ptr - 1);
  free((void*) ((uintptr_t) ptr - offset));
}

#ifdef __cplusplus
}
#endif
