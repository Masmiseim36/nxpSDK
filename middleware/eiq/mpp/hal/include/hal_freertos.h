/*
 * Copyright 2022 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * HAL FREE RTOS public header
 */

#ifndef _HAL_FREERTOS_H
#define _HAL_FREERTOS_H

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "atomic.h"

typedef SemaphoreHandle_t hal_mutex_t;

/* precomputation of the OS tick period with no precision loss */
#define TICK_PERIOD_MS   (1000*128 / configTICK_RATE_HZ) / 128

/* For RT1170,  TickType_t is uint32_t */
#define GET_TICK            (uint32_t)xTaskGetTickCount
/* Conversion to real time with the resolution of one tick period. */
#define TICK_TO_MS(os_tick) (os_tick * TICK_PERIOD_MS)

#define MPP_MALLOC                  pvPortMalloc
#define MPP_FREE                    vPortFree
#define MPP_DEVICE_NAME_MAX_LENGTH  (64)
#define MPP_ATOMIC_ENTER()          ATOMIC_ENTER_CRITICAL()
#define MPP_ATOMIC_EXIT()           ATOMIC_EXIT_CRITICAL()

#endif /* _HAL_FREERTOS_H */
