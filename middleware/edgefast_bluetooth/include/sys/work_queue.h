/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _WORK_QUEUE_H_
#define _WORK_QUEUE_H_

#include "fsl_os_abstraction.h"

/*! @brief Work queue task priority.
 */
#ifndef CONFIG_WORK_QUEUE_TASK_PRIORITY
    #define CONFIG_WORK_QUEUE_TASK_PRIORITY 4
#endif

/*! @brief Work queue task stack size.
 */
#ifndef CONFIG_WORK_QUEUE_TASK_STACK_SIZE
#if (defined(CONFIG_BT_SMP) && (CONFIG_BT_SMP > 0))
    #define CONFIG_WORK_QUEUE_TASK_STACK_SIZE 4096
#else
    #define CONFIG_WORK_QUEUE_TASK_STACK_SIZE 2048
#endif /* CONFIG_BT_SMP */
#endif

#ifdef configTICK_RATE_HZ
#define CONFIG_OS_TICK_INTERVAL configTICK_RATE_HZ
#endif /* configTICK_RATE_HZ */

#ifndef CONFIG_OS_TICK_INTERVAL
#define CONFIG_OS_TICK_INTERVAL 1
#endif /* CONFIG_OS_TICK_INTERVAL */

#ifndef CONFIG_WORK_QUEUE_MSG_QUEUE_COUNT
#define CONFIG_WORK_QUEUE_MSG_QUEUE_COUNT 16
#endif

struct bt_work_queue {
    osa_msgq_handle_t queue;
    OSA_MSGQ_HANDLE_DEFINE(queueHandle, CONFIG_WORK_QUEUE_MSG_QUEUE_COUNT, sizeof(void *));
	osa_task_handle_t thread;
    OSA_TASK_HANDLE_DEFINE(threadHandle);
};

typedef enum bt_work_state {
    BT_WORK_STATE_NONE = 0,                    /* NO Work state */
    BT_WORK_STATE_PENDING = (1 << 0),          /* Work item pending state */
    BT_WORK_STATE_DELAY_PENDING = (1 << 1),    /* Work item delay pending state */
} bt_work_state_t ;

struct bt_work;

typedef void (*bt_work_handler_t)(struct bt_work *work);

struct bt_work
{
    void *next;
    bt_work_handler_t handler;
    uint32_t state;
};

struct bt_delayed_work
{
    struct bt_work work;
    uint32_t timeOut;
    union
    {
        struct bt_work_queue *workQueue;
        struct bt_delayed_work *next;
    };
};

struct bt_delayed_work_queue
{
    struct bt_delayed_work* pending;
    osa_mutex_handle_t mutex;
    OSA_MUTEX_HANDLE_DEFINE(mutexHandle);
    uint32_t                tick;
};

#define BT_WORK_INITIALIZER(work_handler) \
{ \
    .handler = work_handler, \
    .state = BT_WORK_STATE_NONE \
}

#define BT_WORK_DEFINE(work, work_handler) \
    struct bt_work work = \
        BT_WORK_INITIALIZER(work_handler)

int bt_work_queue_init(void);

int bt_work_init(struct bt_work *work, bt_work_handler_t handler);
void bt_work_submit(struct bt_work *work);

void bt_delayed_work_init(struct bt_delayed_work *work, bt_work_handler_t handler);
int bt_delayed_work_remaining_get(struct bt_delayed_work *work);
int bt_delayed_work_submit(struct bt_delayed_work *work, int32_t delay);
int bt_delayed_work_submit_anyways(struct bt_delayed_work *work, int32_t delay);
int bt_delayed_work_cancel(struct bt_delayed_work *work);

#define k_work_queue_init bt_work_queue_init

#define k_work bt_work
#define k_work_init bt_work_init
#define k_work_submit bt_work_submit

#define k_work_delayable bt_delayed_work
#define k_work_init_delayable bt_delayed_work_init
#define k_work_delayable_remaining_get bt_delayed_work_remaining_get
#define k_work_schedule bt_delayed_work_submit
#define k_work_reschedule bt_delayed_work_submit_anyways
#define k_work_cancel_delayable bt_delayed_work_cancel

struct k_work_delayable * k_work_delayable_from_work(struct k_work *work);

#endif /* _WORK_QUEUE_H_ */