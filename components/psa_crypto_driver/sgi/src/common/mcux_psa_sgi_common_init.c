/*
 * Copyright 2025 NXP
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mcux_psa_sgi_common_init.h"


/******************************************************************************/
/*************************** Mutex ********************************************/
/******************************************************************************/
/*
 * Define global mutexes for HW accelerator
 */
mcux_mutex_t sgi_hwcrypto_mutex;

/******************************************************************************/
/******************** CRYPTO_InitHardware *************************************/
/******************************************************************************/

uint32_t g_isCryptoHWInitialized = false;

/*!
 * @brief Application init for Crypto blocks.
 *
 * This function is provided to be called by MCUXpresso SDK applications.
 * It calls basic init for Crypto Hw acceleration and Hw entropy modules.
 */
status_t CRYPTO_InitHardware(void)
{
    status_t result = kStatus_Fail;

    if (g_isCryptoHWInitialized == true) {
        return kStatus_Success;
    }

    /* Mutex for access to sgi crypto HW */
    if (mcux_mutex_init(&sgi_hwcrypto_mutex) != 0) {
        return kStatus_Fail;
    }

    if (mcux_mutex_lock(&sgi_hwcrypto_mutex) != 0) {
        if (mcux_mutex_free(&sgi_hwcrypto_mutex) != 0) {
            /* Mutex free failed, but we're already in error path */
        }
        return kStatus_Fail;
    }

    do {
        result = kStatus_Success;
        g_isCryptoHWInitialized = true;

    } while (0);

    if (mcux_mutex_unlock(&sgi_hwcrypto_mutex) != 0) {
        g_isCryptoHWInitialized = false;
        if (mcux_mutex_free(&sgi_hwcrypto_mutex) != 0) {
            /* Mutex free failed, but we're already in error path */
        }
        return kStatus_Fail;
    }

    return result;
}

/*!
 * @brief Application Deinit for Crypto blocks.
 *
 * This function is provided to be called by MCUXpresso SDK applications.
 * It calls basic deinit for Crypto Hw acceleration and Hw entropy modules.
 */
status_t CRYPTO_DeinitHardware(void)
{
    status_t result = kStatus_Fail;

    if (g_isCryptoHWInitialized == false) {
        return kStatus_Success;
    }

    if (mcux_mutex_lock(&sgi_hwcrypto_mutex) != 0) {
        return kStatus_Fail;
    }

    result = kStatus_Success;
    g_isCryptoHWInitialized = false;

    if (mcux_mutex_unlock(&sgi_hwcrypto_mutex) != 0) {
        return kStatus_Fail;
    }

    if (result == kStatus_Success) {
        if (mcux_mutex_free(&sgi_hwcrypto_mutex) != 0) {
            return kStatus_Fail;
        }
    }

    return result;
}
