/*
 * Copyright 2022-2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * HAL FREE RTOS public header
 */

#ifndef _HAL_FREERTOS_H
#define _HAL_FREERTOS_H

#include "FreeRTOS.h"

/** precomputation of the OS tick period with no precision loss */
#define TICK_PERIOD_MS   (1000*128 / configTICK_RATE_HZ) / 128

/* period (us) of the high precision RunTime counter (= /10 OS tick) */
#define HAL_EXEC_TIMER_US (TICK_PERIOD_MS * 1000 / 10)

/* max number of tasks expected in the system */
#define HAL_MAX_TASKS 10

#endif /* _HAL_FREERTOS_H */
