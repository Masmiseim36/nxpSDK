/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __MCUBOOT_ENC_SUPPORT_H__
#define __MCUBOOT_ENC_SUPPORT_H__

#include "fsl_common.h"
#include "bootutil/bootutil.h"

/* Handles on-the-fly decryption support between staging slots and active slot */
extern status_t mcuboot_process_encryption(struct boot_rsp *rsp);

#endif
