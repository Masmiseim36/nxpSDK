/*
 * Copyright (c) 2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __OS_WRAPPER_DELAY_H__
#define __OS_WRAPPER_DELAY_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"

/**
 * \brief Wait until an absolute time (specified in kernel ticks) is reached.
 *
 * \return \ref OS_WRAPPER_SUCCESS if the time delay is executed, or
 *         \ref OS_WRAPPER_ERROR in case of error
 */
int32_t os_wrapper_delay_until(uint32_t ticks);

#ifdef __cplusplus
}
#endif

#endif /* __OS_WRAPPER_DELAY_H__ */
