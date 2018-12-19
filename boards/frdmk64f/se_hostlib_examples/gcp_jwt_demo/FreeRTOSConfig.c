/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "assert.h"

#define WEAK __attribute__ ((weak))

void vApplicationStackOverflowHook(void) {
    assert(0);
}

void vApplicationMallocFailedHook(void) {
    assert(0);
}

WEAK void vApplicationTickHook(void);

WEAK void vApplicationTickHook(void) {
}
