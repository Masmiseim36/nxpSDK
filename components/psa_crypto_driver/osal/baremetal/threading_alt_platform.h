/*
 * Copyright 2024-2026 NXP
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*!
 * @file threading_alt.h
 * @brief Template mbedtls alt threading declarations for baremetal implementations.
 *
 * @warning This is not a full implementation and a baremetal semaphore must
 *          be provided by the user for actual synchronization.
 *
 * MbedTLS / PSA requires ALT mutex implementations for threading support.
 */

#ifndef __THREADING_ALT_PLATFORM_H__
#define __THREADING_ALT_PLATFORM_H__

/**
 * @brief Mutex struct used to synchronize mbed TLS operations.
 *
 */
typedef struct
{
    void *mutex; /**< Add user-provided baremetal mutex attribute here. */

    char is_valid; /**< Flag used by mbedTLS to track wether a mutex is valid. */
} mbedtls_threading_mutex_t;

#endif /* ifndef __THREADING_ALT_PLATFORM_H__ */
