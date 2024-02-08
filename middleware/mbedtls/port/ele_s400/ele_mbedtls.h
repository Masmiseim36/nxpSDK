/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef ELE_MBEDTLS_H
#define ELE_MBEDTLS_H

#include "fsl_common.h"

#if defined(MIMXRT1189_cm33_SERIES) || defined(MIMXRT1187_cm33_SERIES)
#define S3MU MU_RT_S3MUA
#elif defined(MIMXRT1189_cm7_SERIES) || defined(MIMXRT1187_cm7_SERIES)
#define S3MU MU_APPS_S3MUA
#else
#error "No valid SoC defined"
#endif /* MIMXRT1189_cm33_SERIES | MIMXRT1189_cm7_SERIES */

typedef struct {
    uint32_t session_handle;
    bool is_fw_loaded;
} ele_ctx_t;

#if defined(MBEDTLS_THREADING_C)
#include "threading_alt.h"
/* MUTEX FOR HW Modules*/
extern mbedtls_threading_mutex_t mbedtls_threading_hwcrypto_ele_mutex;
#endif /* defined(MBEDTLS_THREADING_C) */

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief Application init for Crypto blocks.
 *
 * This function is provided to be called by MCUXpresso SDK applications.
 * It calls basic init for Crypto Hw acceleration and Hw entropy modules.
 */
status_t CRYPTO_InitHardware(void);

/*!
 * @brief Application deinit for Crypto blocks.
 *
 * This function is provided to be called by MCUXpresso SDK applications.
 * It calls basic deinit for Crypto Hw acceleration and Hw entropy modules.
 */
status_t CRYPTO_DeinitHardware(void);

#ifdef __cplusplus
}
#endif

#endif /* ELE_MBEDTLS_H */
