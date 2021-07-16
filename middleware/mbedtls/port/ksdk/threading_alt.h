/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __THREADING_ALT_H__
#define __THREADING_ALT_H__

#include "FreeRTOS.h"
#include "semphr.h"

/**
 * @brief Mutex struct used to synchronize mbed TLS operations.
 *
 */
typedef struct
{
    SemaphoreHandle_t mutex; /**< @brief FreeRTOS semaphore. */
    char is_valid;           /**< @brief Flag used by mbedTLS to track wether a mutex is valid. */
} mbedtls_threading_mutex_t;

extern void mbedtls_threading_set_alt(void (*mutex_init)(mbedtls_threading_mutex_t *),
                                      void (*mutex_free)(mbedtls_threading_mutex_t *),
                                      int (*mutex_lock)(mbedtls_threading_mutex_t *),
                                      int (*mutex_unlock)(mbedtls_threading_mutex_t *));

#endif /* ifndef __THREADING_ALT_H__ */
