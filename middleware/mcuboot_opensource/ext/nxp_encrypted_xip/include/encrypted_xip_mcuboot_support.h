/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __ENCRYPTED_XIP_MCUBOOT_SUPPORT_H__
#define __ENCRYPTED_XIP_MCUBOOT_SUPPORT_H__

#include "fsl_common.h"
#include "bootutil/bootutil.h"

/* Handles encrypted XIP support of execution slot in three-slot modification of MCUBoot */
extern status_t encrypted_xip_process(struct boot_rsp *rsp);

#endif
