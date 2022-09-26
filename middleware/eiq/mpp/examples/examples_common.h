/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _EXAMPLES_COMMON_H_
#define _EXAMPLES_COMMON_H_

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

/* Use TensorFlowLite-Micro as an inference engine by default */
#if !defined(INFERENCE_ENGINE_TFLM) && !defined(INFERENCE_ENGINE_GLOW)
#define INFERENCE_ENGINE_TFLM
#endif

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

#endif /* _EXAMPLES_COMMON_H_ */
