/*
 * Copyright 2014-2016 Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __FSL_DEVICE_REGISTERS_H__
#define __FSL_DEVICE_REGISTERS_H__

/*
 * Include the cpu specific register header files.
 *
 * The CPU macro should be declared in the project or makefile.
 */
#if (defined(CPU_MKV46F128VLH16) || defined(CPU_MKV46F128VLL16) || defined(CPU_MKV46F256VLH16) || \
    defined(CPU_MKV46F256VLL16))

#define KV46F16_SERIES

/* CMSIS-style register definitions */
#include "MKV46F16.h"
/* CPU specific feature definitions */
#include "MKV46F16_features.h"

#else
    #error "No valid CPU defined!"
#endif

#endif /* __FSL_DEVICE_REGISTERS_H__ */

/*******************************************************************************
 * EOF
 ******************************************************************************/
