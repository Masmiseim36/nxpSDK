/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <stdint.h>

#include "fsl_common.h"

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

status_t IMAGE_GetImage(uint8_t* dstData, int32_t dstWidth, int32_t dstHeight, int32_t dstChannels);
const char* IMAGE_GetImageName();

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _IMAGE_H_ */
