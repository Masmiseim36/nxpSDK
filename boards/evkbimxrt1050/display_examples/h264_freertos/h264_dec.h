/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __H264_DEC_H__
#define __H264_DEC_H__

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

#include <stdint.h>
#include <stdbool.h>

/*******************************************************************************
 * API
 ******************************************************************************/

/*
 * Return value:
 *  - 0: Initialize success.
 *  - Not 0: Failed.
 */
int OpenH264_Init(void);

/*
 * Return value:
 *  - 0: Decode success.
 *  - Not 0: Failed.
 */
int OpenH264_Decode(const uint8_t *data, int len, bool isStartOfFile, bool isEndOfFile);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif
