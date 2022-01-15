/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __MAIN_CM33_H__
#define __MAIN_CM33_H__

#include "FreeRTOS.h"
#include "task.h"

#include "rpmsg_lite.h"
#include "rpmsg_ns.h"
#include "rpmsg_queue.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
typedef struct _app_handle
{
    TaskHandle_t shell_task_handle;
    TaskHandle_t ipc_task_handle;
} app_handle_t;

#endif /* __MAIN_CM33_H__ */
