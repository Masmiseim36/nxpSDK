/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

// Bundle API auto-generated header file. Do not edit!
// Glow Tools version: 2022-05-11 (2ee55ec50) (Glow_Release_MCUX_SDK_2.12.0)

#ifndef _GLOW_BUNDLE_NANODET_M_H
#define _GLOW_BUNDLE_NANODET_M_H

#include <stdint.h>

// ---------------------------------------------------------------
//                       Common definitions
// ---------------------------------------------------------------
#ifndef _GLOW_BUNDLE_COMMON_DEFS
#define _GLOW_BUNDLE_COMMON_DEFS

// Glow bundle error code for correct execution.
#define GLOW_SUCCESS 0

// Memory alignment definition with given alignment size
// for static allocation of memory.
#define GLOW_MEM_ALIGN(size)  __attribute__((aligned(size)))

// Macro function to get the absolute address of a
// placeholder using the base address of the mutable
// weight buffer and placeholder offset definition.
#define GLOW_GET_ADDR(mutableBaseAddr, placeholderOff)  (((uint8_t*)(mutableBaseAddr)) + placeholderOff)

#endif

// ---------------------------------------------------------------
//                          Bundle API
// ---------------------------------------------------------------
// Model name: "nanodet_m"
// Total data size: 1643904 (bytes)
// Activations allocation efficiency: 1.0000
// Placeholders:
//
//   Name: "cls_pred"
//   Type: float<1 x 100 x 80>
//   Size: 8000 (elements)
//   Size: 32000 (bytes)
//   Offset: 307200 (bytes)
//
//   Name: "reg_pred"
//   Type: float<1 x 100 x 32>
//   Size: 3200 (elements)
//   Size: 12800 (bytes)
//   Offset: 339200 (bytes)
//
//   Name: "data"
//   Type: i8[S:0.031250000 O:0][-4.000,3.969]<1 x 320 x 320 x 3>
//   Size: 307200 (elements)
//   Size: 307200 (bytes)
//   Offset: 0 (bytes)
//
// NOTE: Placeholders are allocated within the "mutableWeight"
// buffer and are identified using an offset relative to base.
// ---------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif

// Placeholder address offsets within mutable buffer (bytes).
#define NANODET_M_cls_pred  307200
#define NANODET_M_reg_pred  339200
#define NANODET_M_data      0

// Memory sizes (bytes).
#define NANODET_M_CONSTANT_MEM_SIZE     216704
#define NANODET_M_MUTABLE_MEM_SIZE      352000
#define NANODET_M_ACTIVATIONS_MEM_SIZE  1075200

// Memory alignment (bytes).
#define NANODET_M_MEM_ALIGN  64

// Bundle entry point (inference function). Returns 0
// for correct execution or some error code otherwise.
int nanodet_m(uint8_t *constantWeight, uint8_t *mutableWeight, uint8_t *activations);

#ifdef __cplusplus
}
#endif
#endif
