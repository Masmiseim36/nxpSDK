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
#if (defined(CPU_MKL27Z128VFM4) || defined(CPU_MKL27Z128VFT4) || defined(CPU_MKL27Z128VLH4) || \
    defined(CPU_MKL27Z128VMP4) || defined(CPU_MKL27Z256VFM4) || defined(CPU_MKL27Z256VFT4) || \
    defined(CPU_MKL27Z256VLH4) || defined(CPU_MKL27Z256VMP4))

#define KL27Z4_SERIES

/* CMSIS-style register definitions */
#include "MKL27Z4.h"
/* CPU specific feature definitions */
#include "MKL27Z4_features.h"

#else
    #error "No valid CPU defined!"
#endif

#endif /* __FSL_DEVICE_REGISTERS_H__ */

/*******************************************************************************
 * EOF
 ******************************************************************************/
