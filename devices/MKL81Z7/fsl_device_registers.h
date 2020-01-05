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
#if (defined(CPU_MKL81Z128VLH7) || defined(CPU_MKL81Z128VLK7) || defined(CPU_MKL81Z128VLL7) || \
    defined(CPU_MKL81Z128VMC7) || defined(CPU_MKL81Z128VMP7))

#define KL81Z7_SERIES

/* CMSIS-style register definitions */
#include "MKL81Z7.h"
/* CPU specific feature definitions */
#include "MKL81Z7_features.h"

#else
    #error "No valid CPU defined!"
#endif

#endif /* __FSL_DEVICE_REGISTERS_H__ */

/*******************************************************************************
 * EOF
 ******************************************************************************/
