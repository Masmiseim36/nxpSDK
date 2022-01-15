/*
 * Copyright (c) 2020-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#include <inttypes.h>
#include "psa/lifecycle.h"
#include "svc_num.h"
#include "tfm_hal_device_header.h"

__attribute__((naked))
uint32_t psa_rot_lifecycle_state(void)
{
    __ASM volatile("SVC %0           \n"
                   "BX LR            \n"
                   : : "I" (TFM_SVC_PSA_LIFECYCLE));
}
