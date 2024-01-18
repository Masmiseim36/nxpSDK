/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _WORK_QUEUE_H_
#define _WORK_QUEUE_H_

#include <sys/slist.h>

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

struct bt_work;

typedef enum bt_work_state {
    BT_WORK_STATE_NONE = 0,                    /* NO Work state */
    BT_WORK_STATE_PENDING = (1 << 0),          /* Work item pending state */
    BT_WORK_STATE_DELAY_PENDING = (1 << 1),    /* Work item delay pending state */
} bt_work_state_t ;

typedef void (*bt_work_handler_t)(struct bt_work *work);

struct bt_work
{
    bt_list_node_t node;
    bt_work_handler_t handler;
    uint32_t state;
};

struct bt_delayed_work
{
    struct bt_work work;
    uint32_t timeOut;
    void *workQueue;
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
void *bt_work_queue_task_handle(void);

int bt_work_init(struct bt_work *work, bt_work_handler_t handler);
void bt_work_submit(struct bt_work *work);
void bt_work_cancel(struct bt_work *work);

void bt_delayed_work_init(struct bt_delayed_work *work, bt_work_handler_t handler);
int bt_delayed_work_remaining_get(struct bt_delayed_work *work);
int bt_delayed_work_submit(struct bt_delayed_work *work, int32_t delay);
int bt_delayed_work_submit_anyways(struct bt_delayed_work *work, int32_t delay);
int bt_delayed_work_cancel(struct bt_delayed_work *work);

#define k_work_queue_init bt_work_queue_init

#define k_work bt_work
#define k_work_init bt_work_init
#define k_work_submit bt_work_submit
#define k_work_cancel bt_work_cancel

#define k_work_delayable bt_delayed_work
#define k_work_init_delayable bt_delayed_work_init
#define k_work_delayable_remaining_get bt_delayed_work_remaining_get
#define k_work_schedule bt_delayed_work_submit
#define k_work_reschedule bt_delayed_work_submit_anyways
#define k_work_cancel_delayable bt_delayed_work_cancel

struct k_work_delayable * k_work_delayable_from_work(struct k_work *work);

#define Z_WORK_DELAYABLE_INITIALIZER(work_handler) { \
	.work = { \
		.handler = work_handler, \
	}, \
}

#define K_WORK_DELAYABLE_DEFINE(work, work_handler) \
	struct k_work_delayable work \
	  = Z_WORK_DELAYABLE_INITIALIZER(work_handler)

#endif /* _WORK_QUEUE_H_ */