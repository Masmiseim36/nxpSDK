/*
 * Copyright (c) 2014 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "utilities/fsl_rtos_abstraction.h"
#include "utilities/fsl_assert.h"

void fsl_assert(void)
{
    lock_acquire();
    while (true)
    {
        // doing nothing
    }
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
