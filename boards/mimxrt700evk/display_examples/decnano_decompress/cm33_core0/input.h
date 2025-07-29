/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _INPUT_H_
#define _INPUT_H_

#include "fsl_common.h"

#define DEMO_PIC_FORMAT_RGB8None  0
#define DEMO_PIC_FORMAT_RGB8H     1
#define DEMO_PIC_FORMAT_RGB8HV    2
#define DEMO_PIC_FORMAT_ARGB8None 3
#define DEMO_PIC_FORMAT_ARGB8H    4
#define DEMO_PIC_FORMAT_ARGB8HV   5

#define DEMO_PIC_FORMAT DEMO_PIC_FORMAT_RGB8None
#define DEMO_PIC_WIDTH  256
#define DEMO_PIC_HEIGHT 256

extern uint8_t inputBuf[];

extern uint32_t inputBufLen;

#endif