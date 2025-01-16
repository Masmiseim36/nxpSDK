/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"

#define ELS_PKC_CRYPTOHW_INITIALIZED    (0xF0F0F0F0U)
#define ELS_PKC_CRYPTOHW_UNINITIALIZED (0x0F0F0F0FU)

/*!
 * @brief Application init for Crypto blocks.
 *
 * This function is provided to be called by MCUXpresso SDK applications.
 * It calls basic init for Crypto Hw acceleration and Hw entropy modules.
 * Function is expected to be called in single thread context only.
 */
status_t CRYPTO_InitHardware(void);
