/*
 * Copyright 2024-2026 NXP
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*!
 * @file threading_alt.c
 * @brief Template MbedTLS alternative threading definitions for baremetal implementations.
 *
 * @warning This is not a full implementation and a baremetal semaphore must
 *          be provided by the user for actual synchronization.
 *
 * MbedTLS / PSA requires ALT mutex implementations for threading support.
 */

#include "threading_alt.h"
#if defined(MBEDTLS_THREADING_C) && defined(MBEDTLS_THREADING_ALT)
#include "mbedtls/threading.h"

void mbedtls_alt_mutex_init(mbedtls_threading_mutex_t *mutex)
{
    /* Add user-provided baremetal mutex init here */
    (void)mutex;
}

void mbedtls_alt_mutex_free(mbedtls_threading_mutex_t *mutex)
{
    if (mutex->is_valid == 1)
    {
        /* Add user-provided baremetal mutex cleanup here */
        (void)mutex;

        mutex->is_valid = 0;
    }
}

int mbedtls_alt_mutex_lock(mbedtls_threading_mutex_t *mutex)
{
    int ret = MBEDTLS_ERR_THREADING_MUTEX_ERROR;

    if (mutex->is_valid == 1)
    {
        /* Add user-provided baremetal mutex lock here */
        (void)mutex;
    }
    return ret;
}

int mbedtls_alt_mutex_unlock(mbedtls_threading_mutex_t *mutex)
{
    int ret = MBEDTLS_ERR_THREADING_MUTEX_ERROR;

    if (mutex->is_valid == 1)
    {
        /* Add user-provided baremetal mutex unlock here */
        (void)mutex;
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
