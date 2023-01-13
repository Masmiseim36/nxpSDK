/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "porting.h"
#include <sys/work_queue.h>

#define LOG_ENABLE IS_ENABLED(CONFIG_BT_DEBUG_WORK_QUEUE)
#define LOG_MODULE_NAME bt_work_queue
#include "fsl_component_log.h"
LOG_MODULE_DEFINE(LOG_MODULE_NAME, kLOG_LevelTrace);

#define DELAYED_TIMER_INTERVAL   10

static struct bt_delayed_work_queue g_DelayedWorkQueueHead;
static struct bt_work_queue s_workQueueHead;

static void work_queue_task(void *param);
static void delayed_work_queue_tick_update(struct bt_delayed_work_queue *work_queue);
static void delayed_work_queue_init(struct bt_delayed_work_queue *work_queue);
void bt_delayed_work_queue_insert_head(struct bt_delayed_work_queue *work_queue, struct bt_delayed_work *work);
void bt_delayed_work_queue_insert_tail(struct bt_delayed_work_queue *work_queue, struct bt_delayed_work *work);
void bt_delayed_work_queue_insert_sort(struct bt_delayed_work_queue *work_queue, struct bt_delayed_work *work);
void bt_delayed_work_queue_remove(struct bt_delayed_work_queue *work_queue, struct bt_delayed_work *work);
struct bt_delayed_work *k_delayed_work_queue_remove_head(struct bt_delayed_work_queue *work_queue);
struct bt_delayed_work *k_delayed_work_queue_remove_tail(struct bt_delayed_work_queue *work_queue);

static OSA_TASK_DEFINE(work_queue_task, CONFIG_WORK_QUEUE_TASK_PRIORITY, 1, CONFIG_WORK_QUEUE_TASK_STACK_SIZE, 0);

static void work_queue_trigger_delayed_work(void)
{
    static struct bt_work work = {NULL, NULL, 0};
    struct bt_work *work_p = &work;
    osa_status_t ret;

    ret = OSA_MsgQPut(s_workQueueHead.queue, &work_p);
    assert(KOSA_StatusSuccess == ret);
    (void)ret;
}

static void work_queue_task(void *param)
{
    struct bt_work_queue *work_queue = (struct bt_work_queue *)param;
    struct bt_work *work;
    bt_work_handler_t handler;
    uint32_t timeout;
    osa_status_t ret;

    #define DELAY_WORK_TIMER_TASK_DELAY_FACTOR 0
    #define DELAY_WORK_TIMER_TASK_DELAY_TIME                                  \
        ((((uint32_t)DELAYED_TIMER_INTERVAL) >> DELAY_WORK_TIMER_TASK_DELAY_FACTOR) >    \
         (1000/CONFIG_OS_TICK_INTERVAL)) ?                                         \
         (((uint32_t)DELAYED_TIMER_INTERVAL) >> DELAY_WORK_TIMER_TASK_DELAY_FACTOR) :     \
         (1000/CONFIG_OS_TICK_INTERVAL)
    timeout = DELAY_WORK_TIMER_TASK_DELAY_TIME;
    do
    {
        ret = OSA_MsgQGet(work_queue->queue, &work, timeout);
        if ( KOSA_StatusSuccess == ret )
        {
            BT_INFO("%p, new task delegation\n", work);
            handler = work->handler;

            /* Reset pending state so it can be resubmitted by handler */
            if (work->state & BT_WORK_STATE_PENDING)
            {
                work->state &= ~BT_WORK_STATE_PENDING;
                if (NULL != handler)
                {
                    handler(work);
                    BT_INFO("%p, task delegation excuted\n", work);
                }
                else
                {
                    BT_INFO("%p, invalid handler\n", work);
                }
            }
        }

        /* delay queue time schedulier */
        delayed_work_queue_tick_update(&g_DelayedWorkQueueHead);
        if (NULL != g_DelayedWorkQueueHead.pending)
        {
            timeout = DELAY_WORK_TIMER_TASK_DELAY_TIME;
        }
        else
        {
            timeout = osaWaitForever_c;
        }
        /* Make sure we don't hog up the CPU if the FIFO never (or
         * very rarely) gets empty.
         */
        (void)OSA_TaskYield();
    } while (true);
}

static void delayed_work_queue_init(struct bt_delayed_work_queue *work_queue)
{
    osa_status_t ret;

    work_queue->pending = NULL;
    work_queue->tick = 0;
    ret = OSA_MutexCreate((osa_mutex_handle_t)work_queue->mutexHandle);
    if (KOSA_StatusSuccess == ret)
    {
        work_queue->mutex = (osa_mutex_handle_t)work_queue->mutexHandle;
    }
    assert(KOSA_StatusSuccess == ret);
}

int bt_work_queue_init(void)
{
    osa_status_t ret;

    delayed_work_queue_init(&g_DelayedWorkQueueHead);

    ret = OSA_MsgQCreate((osa_msgq_handle_t)s_workQueueHead.queueHandle, CONFIG_WORK_QUEUE_MSG_QUEUE_COUNT, sizeof(void*));
    assert (KOSA_StatusSuccess == ret);
    if (KOSA_StatusSuccess == ret)
    {
        s_workQueueHead.queue = (osa_msgq_handle_t)s_workQueueHead.queueHandle;
    }

    ret = OSA_TaskCreate((osa_task_handle_t)s_workQueueHead.threadHandle, OSA_TASK(work_queue_task), (void *)&s_workQueueHead);
    assert (KOSA_StatusSuccess == ret);
    if (KOSA_StatusSuccess == ret)
    {
        s_workQueueHead.thread = (osa_task_handle_t)s_workQueueHead.threadHandle;
    }
    return (KOSA_StatusSuccess == ret) ? -1 : 0;
}

int bt_work_init(struct bt_work *work, bt_work_handler_t handler)
{
    assert(NULL != work);

    work->state = BT_WORK_STATE_NONE;
    work->handler = handler;
    return 0;
}

static void bt_work_submit_to_queue(struct bt_work_queue *work_queue,
                      struct bt_work *work)
{
    if (!(work->state & BT_WORK_STATE_PENDING))
    {
        work->state |= BT_WORK_STATE_PENDING;
        (void)OSA_MsgQPut(work_queue->queue, &work);
    }
}

void bt_work_submit(struct bt_work *work)
{
    bt_work_submit_to_queue(&s_workQueueHead, work);
}

/* delayed work queue start */
void bt_delayed_work_queue_insert_head(struct bt_delayed_work_queue *work_queue, struct bt_delayed_work *work)
{
    if (0 != (work->work.state & BT_WORK_STATE_DELAY_PENDING))
    {
        return;
    }
    work->work.state |= BT_WORK_STATE_DELAY_PENDING;

    (void)OSA_MutexLock(work_queue->mutex, osaWaitForever_c);
    if (NULL == work_queue->pending)
    {
        work_queue->tick = OSA_TimeGetMsec();
    }
    work->work.next = work_queue->pending;
    work_queue->pending = work;
    work_queue_trigger_delayed_work();
    OSA_MutexUnlock(work_queue->mutex);
}

void bt_delayed_work_queue_insert_tail(struct bt_delayed_work_queue *work_queue, struct bt_delayed_work *work)
{
    struct bt_delayed_work *p;
    struct bt_delayed_work *q = NULL;

    if (0 != (work->work.state & BT_WORK_STATE_DELAY_PENDING))
    {
        return;
    }
    work->work.state |= BT_WORK_STATE_DELAY_PENDING;

    work->work.next = NULL;

    (void)OSA_MutexLock(work_queue->mutex, osaWaitForever_c);
    p = work_queue->pending;
    while (NULL != p)
    {
        q = p;
        p = (struct bt_delayed_work *)p->work.next;
    }
    if (NULL == q)
    {
        work_queue->tick = OSA_TimeGetMsec();
        work_queue->pending = work;
    }
    else
    {
        q->work.next = work;
    }
    work_queue_trigger_delayed_work();
    OSA_MutexUnlock(work_queue->mutex);
}

void bt_delayed_work_queue_insert_sort(struct bt_delayed_work_queue *work_queue, struct bt_delayed_work *work)
{
    struct bt_delayed_work *p;
    struct bt_delayed_work *q = NULL;

    if (0 != (work->work.state & BT_WORK_STATE_DELAY_PENDING))
    {
        return;
    }
    work->work.state |= BT_WORK_STATE_DELAY_PENDING;

    BT_INFO("%p, delay task submit with timeOut %d\n",work, work->timeOut);
    work->work.next = NULL;

    (void)OSA_MutexLock(work_queue->mutex, osaWaitForever_c);
    p = work_queue->pending;
    if (NULL == p)
    {
        work_queue->tick = OSA_TimeGetMsec();
        work_queue->pending = work;
    }
    else
    {
        while (NULL != p)
        {
            if (p->timeOut <= work->timeOut)
            {
                q = p;
                p = (struct bt_delayed_work *)p->work.next;
            }
            else
            {
                if (NULL == q)
                {
                    work->work.next = work_queue->pending;
                    work_queue->pending = work;
                }
                else
                {
                    work->work.next = q->work.next;
                    q->work.next = work;
                }
                break;
            }
        }
        if (NULL == p)
        {
            work->work.next = q->work.next;
            q->work.next = work;
        }
    }
    work_queue_trigger_delayed_work();
    OSA_MutexUnlock(work_queue->mutex);
}

void bt_delayed_work_queue_remove(struct bt_delayed_work_queue *work_queue, struct bt_delayed_work *work)
{
    struct bt_delayed_work *p;
    struct bt_delayed_work *q = NULL;

    (void)OSA_MutexLock(work_queue->mutex, osaWaitForever_c);
    p = work_queue->pending;
    while ((NULL != p) && (p != work))
    {
        q = p;
        p = (struct bt_delayed_work *)p->work.next;
    }
    if (p == work)
    {
        if (NULL == q)
        {
            work_queue->pending = (struct bt_delayed_work *)p->work.next;
        }
        else
        {
            q->work.next = p->work.next;
        }
        work->work.next = NULL;
        p->work.state &= ~BT_WORK_STATE_DELAY_PENDING;
    }

    OSA_MutexUnlock(work_queue->mutex);
}

struct bt_delayed_work *k_delayed_work_queue_remove_head(struct bt_delayed_work_queue *work_queue)
{
    struct bt_delayed_work *p;

    (void)OSA_MutexLock(work_queue->mutex, osaWaitForever_c);
    p = work_queue->pending;
    if (NULL == p)
    {
        work_queue->pending = NULL;
        OSA_MutexUnlock(work_queue->mutex);
        return p;
    }
    else
    {
        work_queue->pending = (struct bt_delayed_work *)p->work.next;
    }
    OSA_MutexUnlock(work_queue->mutex);
    p->work.state &= ~BT_WORK_STATE_DELAY_PENDING;
    return p;
}

struct bt_delayed_work *k_delayed_work_queue_remove_tail(struct bt_delayed_work_queue *work_queue)
{
    struct bt_delayed_work *p;
    struct bt_delayed_work *q = NULL;

    (void)OSA_MutexLock(work_queue->mutex, osaWaitForever_c);
    p = work_queue->pending;
    if (NULL == p)
    {
        OSA_MutexUnlock(work_queue->mutex);
        return p;
    }
    else
    {
        while (NULL != p->work.next)
        {
            q = p;
            p = (struct bt_delayed_work *)p->work.next;
        }
        if (NULL == q)
        {
            work_queue->pending = NULL;
        }
        else
        {
            q->work.next = NULL;
        }
    }
    OSA_MutexUnlock(work_queue->mutex);
    p->work.state &= ~BT_WORK_STATE_DELAY_PENDING;
    return p;
}

static void delayed_work_queue_tick_update(struct bt_delayed_work_queue *work_queue)
{
    struct bt_delayed_work *p;
    struct bt_delayed_work *q;
    uint32_t current;
    uint32_t delta;
    void *temp;

    (void)OSA_MutexLock(work_queue->mutex, osaWaitForever_c);

    p = work_queue->pending;

    if (NULL != p)
    {
        do
        {
            current = OSA_TimeGetMsec();
            if (current < work_queue->tick)
            {
                delta = work_queue->tick - current + (uint32_t)1;
            }
            else
            {
                delta = current - work_queue->tick;
            }
            if (delta < (uint32_t)DELAYED_TIMER_INTERVAL)
            {
                break;
            }
            work_queue->tick = current;

            while (NULL != p)
            {
                q = (struct bt_delayed_work *)p->work.next;
                if (p->timeOut > delta)
                {
                    p->timeOut -= delta;
                }
                else
                {
                    p->timeOut = 0;
                    BT_INFO("%p, delay task timeout\n",(uint32_t)p);
                    bt_delayed_work_queue_remove(work_queue, p);
                    temp = (void *)p;
                    bt_work_submit((struct bt_work *)temp);
                }
                p = q;
            }
        } while(0 != 1);
    }
    OSA_MutexUnlock(work_queue->mutex);
}

void bt_delayed_work_init(struct bt_delayed_work *work, bt_work_handler_t handler)
{
    work->timeOut = 0;
    work->workQueue = NULL;
    (void)bt_work_init(&work->work, handler);
}

int bt_delayed_work_remaining_get(struct bt_delayed_work *work)
{
    return (int)work->timeOut;
}

int bt_delayed_work_submit(struct bt_delayed_work *work, int32_t delay)
{
    if ((NULL != work->workQueue) && (&s_workQueueHead != work->workQueue))
    {
        return -1;
    }
    if (0 == (work->work.state & (BT_WORK_STATE_PENDING | BT_WORK_STATE_DELAY_PENDING)))
    {
        work->timeOut = (uint32_t)delay;
        work->workQueue = &s_workQueueHead;
        bt_delayed_work_queue_insert_sort(&g_DelayedWorkQueueHead, work);
    }
    else
    {
        BT_WARN("%p, delay task has been submit %d\n", work, delay);
    }

    return 0;
}

int bt_delayed_work_submit_anyways(struct bt_delayed_work *work, int32_t delay)
{
    if ((NULL != work->workQueue) && (&s_workQueueHead != work->workQueue))
    {
        return -1;
    }

    bt_delayed_work_queue_remove(&g_DelayedWorkQueueHead, work);

    work->timeOut = (uint32_t)delay;
    work->workQueue = &s_workQueueHead;
    bt_delayed_work_queue_insert_sort(&g_DelayedWorkQueueHead, work);

    return 0;
}

int bt_delayed_work_cancel(struct bt_delayed_work *work)
{
    bt_delayed_work_queue_remove(&g_DelayedWorkQueueHead, work);
    return 0;
}

struct k_work_delayable * k_work_delayable_from_work(struct k_work *work)
{
	return CONTAINER_OF(work, struct k_work_delayable, work);
}