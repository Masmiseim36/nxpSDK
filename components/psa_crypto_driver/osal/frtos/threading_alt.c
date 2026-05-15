/*
 * Copyright 2024-2026 NXP
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*!
 * @file threading_alt.c
 * @brief Alternative threading definitions for MbedTLS with FreeRTOS.
 *
 * MbedTLS / PSA requires ALT mutex implementations for threading support.
 */

#include "threading_alt.h"
#if defined(MBEDTLS_THREADING_C) && defined(MBEDTLS_THREADING_ALT)
#include "mbedtls/threading.h"

void mbedtls_alt_mutex_init(mbedtls_threading_mutex_t *mutex)
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

void mbedtls_alt_mutex_free(mbedtls_threading_mutex_t *mutex)
{
    if (mutex->is_valid == 1)
    {
        vSemaphoreDelete(mutex->mutex);
        mutex->is_valid = 0;
    }
}

int mbedtls_alt_mutex_lock(mbedtls_threading_mutex_t *mutex)
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

int mbedtls_alt_mutex_unlock(mbedtls_threading_mutex_t *mutex)
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

void config_mbedtls_threading_alt(void)
{
    /* Configure mbedtls to use threading alt mutexes. */
    mbedtls_threading_set_alt(mbedtls_alt_mutex_init,
                              mbedtls_alt_mutex_free,
                              mbedtls_alt_mutex_lock,
                              mbedtls_alt_mutex_unlock);
}

#endif /* MBEDTLS_THREADING_C && MBEDTLS_THREADING_ALT */
