/*
 * Copyright 2018-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _EIQ_PXP_H_
#define _EIQ_PXP_H_

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

#include "fsl_pxp.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
void EIQ_PXP_Init();
void EIQ_PXP_Rotate(uint32_t input_buffer, uint32_t output_buffer);

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

#endif /* _IMAGE_H_ */
