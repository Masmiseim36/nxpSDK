/* Copyright 2019 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "assert.h"

#define WEAK __attribute__((weak))

WEAK void vApplicationStackOverflowHook(void)
{
    assert(0);
}

WEAK void vApplicationMallocFailedHook(void)
{
    assert(0);
}

WEAK void vApplicationTickHook(void);

WEAK void vApplicationTickHook(void)
{
}
