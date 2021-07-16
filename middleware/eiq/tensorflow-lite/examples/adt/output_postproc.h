/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _OUTPUT_POSTPROCESS_H_
#define _OUTPUT_POSTPROCESS_H_

#include "model.h"

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/

void MODEL_ProcessOutput(const uint8_t *dataOut,
                         const uint8_t *dataIn,
                         const tensor_dims_t *dims,
                         tensor_type_t type,
                         int inferenceTime,
                         bool *skip);

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

#endif /* _OUTPUT_POSTPROCESS_H_ */
