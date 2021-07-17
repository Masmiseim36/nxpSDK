/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _EAP_PROC_H_
#define _EAP_PROC_H_

#include "LVM.h" // EAP library

#define LVM_FRAME_SIZE_MS 10
#ifndef MAX_SAMPLE_SIZE
#define MAX_SAMPLE_SIZE 960
#endif

typedef int (*EAP_Initialize_T)(void *arg);
typedef int (*EAP_Execute_T)(void *arg, short *inputBuffer, int size);
typedef int (*EAP_Deinit_T)(void);

extern EAP_Initialize_T EAP_Initialize_func;
extern EAP_Execute_T EAP_Execute_func;
extern EAP_Deinit_T EAP_Deinit_func;

#endif
