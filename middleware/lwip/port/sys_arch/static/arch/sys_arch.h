/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 */

/*
 * Copyright (c) 2013-2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017,2020,2022,2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __ARCH_SYS_ARCH_H__
#define __ARCH_SYS_ARCH_H__

#include "lwip/opt.h"
#include "arch/cc.h"
#include "stdint.h"

#if !NO_SYS

#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "task.h"

#if !defined(configSUPPORT_STATIC_ALLOCATION) || (configSUPPORT_STATIC_ALLOCATION == 0U)
#error "configSUPPORT_STATIC_ALLOCATION is not enabled"
#endif

/* MEMP_NUM_SYS_THREAD: the maximum number of threads created by sys_thread_new. */
#ifndef MEMP_NUM_SYS_THREAD
#define MEMP_NUM_SYS_THREAD 4
#endif

/*
 * TOTAL_SYS_THREAD_STACKSIZE: The sum of thread stack sizes of all threads
 * created by sys_thread_new, in bytes.
 */
#ifndef TOTAL_SYS_THREAD_STACKSIZE
#define TOTAL_SYS_THREAD_STACKSIZE \
    ((TCPIP_THREAD_STACKSIZE + ((MEMP_NUM_SYS_THREAD - 1) * DEFAULT_THREAD_STACKSIZE)) * sizeof(StackType_t))
#endif

/*
 * SYS_MBOX_MAX_ITEMS: Maximum number of items the sys_mbox_t can hold in its queue.
 * Since the storage buffer for the items is statically allocated within each sys_mbox_t structure,
 * every message box will use this amount of memory even if using smaller number of items.
 */
#ifndef SYS_MBOX_MAX_ITEMS
#define SYS_MBOX_MAX_ITEMS 96
#endif

typedef struct _sys_sem_t
{
    SemaphoreHandle_t xHandle;
    StaticSemaphore_t xSemaphoreBuffer;
} sys_sem_t;

#define sys_sem_valid_val(sema)   ((sema).xHandle != NULL)
#define sys_sem_valid(sema)       (((sema) != NULL) && sys_sem_valid_val(*(sema)))
#define sys_sem_set_invalid(sema) ((sema)->xHandle = NULL)

typedef struct _sys_mutex_t
{
    SemaphoreHandle_t xHandle;
    StaticSemaphore_t xMutexBuffer;
} sys_mutex_t;

#define sys_mutex_valid_val(mutex)   ((mutex).xHandle != 0)
#define sys_mutex_valid(mutex)       (((mutex) != NULL) && sys_mutex_valid_val(*(mutex)))
#define sys_mutex_set_invalid(mutex) ((mutex)->xHandle = NULL)

typedef struct _sys_mbox_t
{
    QueueHandle_t xHandle;
    StaticQueue_t xQueueBuffer;
    uint8_t pucQueueStorageBuffer[sizeof(void *) * SYS_MBOX_MAX_ITEMS];
} sys_mbox_t;

#define sys_mbox_valid_val(mbox)   ((mbox).xHandle != NULL)
#define sys_mbox_valid(mbox)       (((mbox) != NULL) && sys_mbox_valid_val(*(mbox)))
#define sys_mbox_set_invalid(mbox) ((mbox)->xHandle = NULL)

typedef struct _sys_thread_t
{
    TaskHandle_t xHandle;
    StaticTask_t xTaskBuffer;
} *sys_thread_t;

#endif

typedef unsigned long sys_prot_t;

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

void sys_assert(const char *pcMessage);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* __ARCH_SYS_ARCH_H__ */
