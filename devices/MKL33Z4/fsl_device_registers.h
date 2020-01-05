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
#if (defined(CPU_MKL33Z128VLH4) || defined(CPU_MKL33Z128VMP4) || defined(CPU_MKL33Z256VLH4) || \
    defined(CPU_MKL33Z256VMP4))

#define KL33Z4_SERIES

/* CMSIS-style register definitions */
#include "MKL33Z4.h"
/* CPU specific feature definitions */
#include "MKL33Z4_features.h"

#else
    #error "No valid CPU defined!"
#endif

#endif /* __FSL_DEVICE_REGISTERS_H__ */

/*******************************************************************************
 * EOF
 ******************************************************************************/
