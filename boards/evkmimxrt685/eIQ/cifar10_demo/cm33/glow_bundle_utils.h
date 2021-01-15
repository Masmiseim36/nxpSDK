/*
 * Copyright 2019-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _GLOW_BUNDLE_UTILS_H
#define _GLOW_BUNDLE_UTILS_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Alignment size definition.
// Valid alignment sizes are 1 ... 255.
typedef uint8_t align_t;

// Allocate memory dynamically with given alignment and size.
void* alignedAlloc(align_t alignment, size_t size);

// Free memory previously allocated with "alignedAlloc".
void alignedFree(void *ptr);

#ifdef __cplusplus
}
#endif

#endif // _GLOW_BUNDLE_UTILS_H
