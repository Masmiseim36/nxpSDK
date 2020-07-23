/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */
 
#ifndef MATHFP_H
/* clang-format off */
#define MATHFP_H
/* clang-format on */

#include "fsl_common.h"
#include "fsl_device_registers.h"

/*!
 * @addtogroup xcvr
 * @{
 */

/*! @file*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*! *********************************************************************************
* \brief  This function performs the arctan function on fixed point data using an approximation of the arctan.
*
* \param[in] y - The Y axis value S(3,12) fixed point format.
* \param[in] x - X axis value in S(3,12) fixed point format.
*
* \return The approximate value of the angle in the range -pi to +pi. Returns 0xFFFF in case of error.
*
* \details 
***********************************************************************************/
int16_t atan2fp(int16_t y, int16_t x);

#if defined(__cplusplus)
}
#endif

#endif /* ifndef MATHFP_H */
