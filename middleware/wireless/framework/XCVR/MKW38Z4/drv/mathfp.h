/*
 * Copyright (c) 2017, Freescale Semiconductor, Inc.
 * Copyright 2018-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef MATHFP_H
/* clang-format off */
#define MATHFP_H
/* clang-format on */

#include "fsl_common.h"
#include "fsl_device_registers.h"

/*!
 * @addtogroup fixed_pt_math Fixed Point Math Routines
 * @{
 */

/*! @file*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define FLTTOFP(f,q)    ((int16_t)((f)*(float64_t)(1<<q)))
#define FPTOFLT(x,q)    ((float64_t)(x) / (float64_t)(1<<(q)))

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*! *********************************************************************************
* \brief  This function performs the arctan function on fixed point data using an approximation of the arctan.
*
* \param[in] y The Y axis value S(3,12) fixed point format.
* \param[in] x The X axis value in S(3,12) fixed point format.
*
* \return The approximate value of the angle in the range -pi to +pi. Returns 0xFFFF in case of error.
*
* \details 
* This function is known to violate HIS metrics for cyclomatic complexity (CCM) and language scope (VOCF).
***********************************************************************************/
int16_t atan2fp(int16_t y, int16_t x);

/*! @}*/

#if defined(__cplusplus)
}
#endif

#endif /* ifndef MATHFP_H */
