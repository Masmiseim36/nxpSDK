/*
 * Copyright 2022-2024 NXP
 *
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *  */

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
