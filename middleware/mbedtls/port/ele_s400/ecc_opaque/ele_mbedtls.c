/**
 * \file ele_s400/ecc_opaque/ele_mbedtls.c
 *
 * This is an alternate file to the regular ele_s400/ele_mbedtls.c,
 * containing definitions specific to the handling of opaque
 * keys for P-256 curves.
 *
 * \warning This implementation is not compatible with some of the other
 * MbedTLS features (e.g. PK module key material loading/cert parsing for RSA).
 * It is only intended to showcase P-256 opaque key handling via the EdgeLock
 * Enclave (ELE).
 *
 */
/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

/* Guarding this file, so it can only be used when it's really meant to be */
#if defined(NXP_ELE_ECC_OPAQUE_KEY)

#if defined(MBEDTLS_THREADING_C)
/* Threading mutex implementations for mbedTLS. */
#include "mbedtls/threading.h"
#include "threading_alt.h"
#endif

#if !defined(MBEDTLS_MCUX_FREERTOS_THREADING_ALT) && defined(MBEDTLS_THREADING_C) && defined(MBEDTLS_THREADING_ALT)
extern void CRYPTO_ConfigureThreading(void);
#endif

#include "ele_mbedtls.h" /* ELE Crypto port layer */
#include "ele_crypto.h"  /* ELE Crypto SW */
#include "fsl_s3mu.h"    /* Messaging unit driver */
#include "ele_fw.h"      /* ELE FW, can be placed in bootable container in real world app */


/******************************************************************************/
/*************************** Mutex ********************************************/
/******************************************************************************/
#if defined(MBEDTLS_THREADING_C)

/**
 * \def MBEDTLS_MCUX_FREERTOS_THREADING_ALT
 * You can comment this macro if you provide your own alternate implementation.
 *
 */
#if defined(SDK_OS_FREE_RTOS)
#define MBEDTLS_MCUX_FREERTOS_THREADING_ALT
#endif

/*
 * Define global mutexes for HW accelerator
 */
mbedtls_threading_mutex_t mbedtls_threading_hwcrypto_ele_mutex;

#if defined(MBEDTLS_MCUX_FREERTOS_THREADING_ALT)
/**
 * @brief Initializes the mbedTLS mutex functions.
 *
 * Provides mbedTLS access to mutex create, destroy, take and free.
 *
 * @see MBEDTLS_THREADING_ALT
 */
static void CRYPTO_ConfigureThreadingMcux(void);
#endif /* defined(MBEDTLS_MCUX_FREERTOS_THREADING_ALT) */

#endif /* defined(MBEDTLS_THREADING_C) */

/******************************************************************************/
/******************** CRYPTO_InitHardware *************************************/
/******************************************************************************/

uint32_t g_isCryptoHWInitialized = false;

ele_ctx_t g_ele_ctx = {0u}; /* Global context */

/*!
 * @brief Application init for Crypto blocks.
 *
 * This function is provided to be called by MCUXpresso SDK applications.
 * It calls basic init for Crypto Hw acceleration and Hw entropy modules.
 */
status_t CRYPTO_InitHardware(void)
{
    status_t result = kStatus_Fail;

    if(g_isCryptoHWInitialized == true)
    {
        return (0);
    }

#if defined(MBEDTLS_THREADING_C) && defined(MBEDTLS_THREADING_ALT)

    CRYPTO_ConfigureThreadingMcux();

#endif /* (MBEDTLS_THREADING_C) && defined(MBEDTLS_THREADING_ALT) */


#if defined(MBEDTLS_THREADING_C)
    mbedtls_mutex_init(&mbedtls_threading_hwcrypto_ele_mutex);
#endif /* (MBEDTLS_THREADING_C) */

#if defined(MBEDTLS_THREADING_C)
    if ((result = mbedtls_mutex_lock(&mbedtls_threading_hwcrypto_ele_mutex)) != 0)
        return result;
#endif

    do
    {
        /****************** Load EdgeLock FW ***********************/
        if(g_ele_ctx.is_fw_loaded != true)
        {
            result = ELE_LoadFw(S3MU, ele_fw);
            if (result != kStatus_Success)
            {
                break;
            }
            else
            {
                g_ele_ctx.is_fw_loaded = true;
            }
        }

        /****************** Initialize EdgeLock services ************/
        result = ELE_InitServices(S3MU);
        if (result != kStatus_Success)
        {
            break;
        }

        /****************** Start RNG ***********************/
        result = ELE_StartRng(S3MU);
        if (result != kStatus_Success) {
            break;
        }

        /****************** Open EdgeLock session ******************/
        result = ELE_OpenSession(S3MU, &g_ele_ctx.session_handle);
        if (result != kStatus_Success)
        {
            break;
        }

        /****************** Open NVM Storage service **************************/
        result = ELE_OpenNvmStorageService(S3MU, g_ele_ctx.session_handle, &g_ele_ctx.storage_handle);
        if (result != kStatus_Success)
        {
            break;
        }

        g_isCryptoHWInitialized = true;

    } while (0);

#if defined(MBEDTLS_THREADING_C)
    if (mbedtls_mutex_unlock(&mbedtls_threading_hwcrypto_ele_mutex) != 0)
        return MBEDTLS_ERR_THREADING_MUTEX_ERROR;
#endif

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

    if(g_isCryptoHWInitialized == false)
    {
        return (0);
    }

#if defined(MBEDTLS_THREADING_C)
    if ((result = mbedtls_mutex_lock(&mbedtls_threading_hwcrypto_ele_mutex)) != 0)
        return result;
#endif

    do
    {
        /****************** Close Sign Service ********************************/
        if( g_ele_ctx.signature_gen_handle != 0u )
        {
            result = ELE_CloseSignService(S3MU, g_ele_ctx.signature_gen_handle);
            if (result != kStatus_Success)
            {
                break;
            }
            g_ele_ctx.signature_gen_handle = 0u;
        }

        /****************** Close Verify Service ******************************/
        if( g_ele_ctx.signature_verif_handle != 0u )
        {
            result = ELE_CloseVerifyService(S3MU, g_ele_ctx.signature_verif_handle);
            if (result != kStatus_Success)
            {
                break;
            }
            g_ele_ctx.signature_verif_handle = 0u;
        }

        /****************** Close Key Management Service **********************/
        if( g_ele_ctx.key_management_handle != 0u )
        {
            result = ELE_CloseKeyService(S3MU, g_ele_ctx.key_management_handle);
            if (result != kStatus_Success)
            {
                break;
            }
            g_ele_ctx.key_management_handle = 0u;
        }

        /****************** Close Key Store  ***********************************/
        if( g_ele_ctx.key_store_handle != 0u )
        {
            result = ELE_CloseKeystore(S3MU, g_ele_ctx.key_store_handle);
            if (result != kStatus_Success)
            {
                break;
            }
            g_ele_ctx.key_store_handle   = 0u;
            g_ele_ctx.is_keystore_opened = 0u;
        }

        /****************** Close NVM storage session **************************/
        if( g_ele_ctx.storage_handle != 0u )
        {
            result = ELE_CloseNvmStorageService(S3MU, g_ele_ctx.storage_handle);
            if (result != kStatus_Success)
            {
                break;
            }
            g_ele_ctx.storage_handle = 0u;
        }

        /****************** Close EdgeLock session ******************/
        result = ELE_CloseSession(S3MU, g_ele_ctx.session_handle);
        if (result != kStatus_Success)
        {
            break;
        }

        g_ele_ctx.session_handle = 0u;

        g_isCryptoHWInitialized = false;

    } while (0);



#if defined(MBEDTLS_THREADING_C)
    if (mbedtls_mutex_unlock(&mbedtls_threading_hwcrypto_ele_mutex) != 0)
        return MBEDTLS_ERR_THREADING_MUTEX_ERROR;
#endif

    return result;
}

/*-----------------------------------------------------------*/
/*--------- mbedTLS threading functions for FreeRTOS --------*/
/*--------------- See MBEDTLS_THREADING_ALT -----------------*/
/*-----------------------------------------------------------*/
#if defined(MBEDTLS_MCUX_FREERTOS_THREADING_ALT)
/* Threading mutex implementations for mbedTLS. */
#include "mbedtls/threading.h"
#include "threading_alt.h"

/**
 * @brief Implementation of mbedtls_mutex_init for thread-safety.
 *
 */
void mcux_mbedtls_mutex_init(mbedtls_threading_mutex_t *mutex)
{
    mutex->mutex = xSemaphoreCreateMutex();

    if (mutex->mutex != NULL)
    {
        mutex->is_valid = 1;
    }
    else
    {
        mutex->is_valid = 0;
    }
}

/**
 * @brief Implementation of mbedtls_mutex_free for thread-safety.
 *
 */
void mcux_mbedtls_mutex_free(mbedtls_threading_mutex_t *mutex)
{
    if (mutex->is_valid == 1)
    {
        vSemaphoreDelete(mutex->mutex);
        mutex->is_valid = 0;
    }
}

/**
 * @brief Implementation of mbedtls_mutex_lock for thread-safety.
 *
 * @return 0 if successful, MBEDTLS_ERR_THREADING_MUTEX_ERROR if timeout,
 * MBEDTLS_ERR_THREADING_BAD_INPUT_DATA if the mutex is not valid.
 */
int mcux_mbedtls_mutex_lock(mbedtls_threading_mutex_t *mutex)
{
    int ret = MBEDTLS_ERR_THREADING_BAD_INPUT_DATA;

    if (mutex->is_valid == 1)
    {
        if (xSemaphoreTake(mutex->mutex, portMAX_DELAY))
        {
            ret = 0;
        }
        else
        {
            ret = MBEDTLS_ERR_THREADING_MUTEX_ERROR;
        }
    }

    return ret;
}

/**
 * @brief Implementation of mbedtls_mutex_unlock for thread-safety.
 *
 * @return 0 if successful, MBEDTLS_ERR_THREADING_MUTEX_ERROR if timeout,
 * MBEDTLS_ERR_THREADING_BAD_INPUT_DATA if the mutex is not valid.
 */
int mcux_mbedtls_mutex_unlock(mbedtls_threading_mutex_t *mutex)
{
    int ret = MBEDTLS_ERR_THREADING_BAD_INPUT_DATA;

    if (mutex->is_valid == 1)
    {
        if (xSemaphoreGive(mutex->mutex))
        {
            ret = 0;
        }
        else
        {
            ret = MBEDTLS_ERR_THREADING_MUTEX_ERROR;
        }
    }

    return ret;
}

static void CRYPTO_ConfigureThreadingMcux(void)
{
    /* Configure mbedtls to use FreeRTOS mutexes. */
    mbedtls_threading_set_alt(mcux_mbedtls_mutex_init,
                              mcux_mbedtls_mutex_free,
                              mcux_mbedtls_mutex_lock,
                              mcux_mbedtls_mutex_unlock);
}
#endif /* defined(MBEDTLS_MCUX_FREERTOS_THREADING_ALT) */

#endif /* NXP_ELE_ECC_OPAQUE_KEY */
