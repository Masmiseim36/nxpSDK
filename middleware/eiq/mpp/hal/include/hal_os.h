/*
 * Copyright 2022 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * HAL OS public header
 */

#ifndef _HAL_OS_H
#define _HAL_OS_H

/* include OS used for the HAL */
#include "hal_freertos.h"

int hal_mutex_create (hal_mutex_t *mutex);
void hal_mutex_remove (hal_mutex_t mutex);
int hal_mutex_lock (hal_mutex_t mutex);
int hal_mutex_unlock (hal_mutex_t mutex);

#endif /* _HAL_OS_H */
