/*
 * Copyright (c) 2013 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __MILLISECONDS_DELAY_H__
#define __MILLISECONDS_DELAY_H__

#include <stdint.h>

extern void milliseconds_delay_init(void);
extern void milliseconds_delay(uint32_t ms);

#endif
