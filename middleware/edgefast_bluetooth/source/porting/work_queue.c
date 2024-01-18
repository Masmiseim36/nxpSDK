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

struct bt_work_queue {
    bt_fifo_t fifo;
	osa_task_handle_t thread;
    OSA_TASK_HANDLE_DEFINE(threadHandle);
};

struct bt_delayed_work_queue
{
    bt_fifo_t fifo;
    osa_mutex_handle_t mutex;
    OSA_MUTEX_HANDLE_DEFINE(mutexHandle);
    uint32_t                tick;
};


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
    static BT_WORK_DEFINE(dummy, NULL);

    bt_fifo_put(&s_workQueueHead.fifo, &dummy.node);
}

static void work_queue_task(void *param)
{
    struct bt_work_queue *work_queue = (struct bt_work_queue *)param;
    struct bt_work *work = NULL;
    bt_work_handler_t handler;
    uint32_t timeout;

    #define DELAY_WORK_TIMER_TASK_DELAY_FACTOR 0
    #define DELAY_WORK_TIMER_TASK_DELAY_TIME                                  \
        ((((uint32_t)DELAYED_TIMER_INTERVAL) >> DELAY_WORK_TIMER_TASK_DELAY_FACTOR) >    \
         (1000/CONFIG_OS_TICK_INTERVAL)) ?                                         \
         (((uint32_t)DELAYED_TIMER_INTERVAL) >> DELAY_WORK_TIMER_TASK_DELAY_FACTOR) :     \
         (1000/CONFIG_OS_TICK_INTERVAL)
    timeout = DELAY_WORK_TIMER_TASK_DELAY_TIME;
    do
    {
        work = bt_fifo_get(&work_queue->fifo, timeout);

        if (NULL != work)
        {
            LOG_INF("%p, new task delegation\n", work);
            handler = work->handler;

            /* Reset pending state so it can be resubmitted by handler */
            if (work->state & BT_WORK_STATE_PENDING)
            {
                work->state &= ~BT_WORK_STATE_PENDING;
                if (NULL != handler)
                {
                    handler(work);
                    LOG_INF("%p, task delegation executed\n", work);
                }
                else
                {
                    LOG_INF("%p, invalid handler\n", work);
                }
            }
        }

        /* delay queue time scheduler */
        delayed_work_queue_tick_update(&g_DelayedWorkQueueHead);
        (void)OSA_MutexLock(g_DelayedWorkQueueHead.mutex, osaWaitForever_c);
        if (bt_fifo_is_empty(&g_DelayedWorkQueueHead.fifo) > 0)
        {
            /*
             * Wait forever if the delay queue is empty. The task will be waked up
             * when there is a new node inserted into delay queue.
             */
            timeout = osaWaitForever_c;
        }
        else
        {
            /*
             * Polling the delay queue if the queue is not empty.
             */
            timeout = DELAY_WORK_TIMER_TASK_DELAY_TIME;
        }
        OSA_MutexUnlock(g_DelayedWorkQueueHead.mutex);

        /* Make sure we don't hog up the CPU if the FIFO never (or
         * very rarely) gets empty.
         */
        (void)OSA_TaskYield();
    } while (true);
}

static void delayed_work_queue_init(struct bt_delayed_work_queue *work_queue)
{
    osa_status_t ret;

    bt_fifo_init(&work_queue->fifo);
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

    bt_fifo_init(&s_workQueueHead.fifo);

    ret = OSA_TaskCreate((osa_task_handle_t)s_workQueueHead.threadHandle, OSA_TASK(work_queue_task), (void *)&s_workQueueHead);
    assert (KOSA_StatusSuccess == ret);
    if (KOSA_StatusSuccess == ret)
    {
        s_workQueueHead.thread = (osa_task_handle_t)s_workQueueHead.threadHandle;
    }
    return (KOSA_StatusSuccess == ret) ? -1 : 0;
}

void *bt_work_queue_task_handle(void)
{
    return s_workQueueHead.thread;
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
        bt_fifo_put(&work_queue->fifo, &work->node);
    }
}

void bt_work_submit(struct bt_work *work)
{
    bt_work_submit_to_queue(&s_workQueueHead, work);
}

void bt_work_cancel(struct bt_work *work)
{
    /* Here we could not remove from queue, so just mark it as "BT_WORK_STATE_NONE". */
    work->state = BT_WORK_STATE_NONE;
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
    if (bt_fifo_is_empty(&work_queue->fifo) > 0)
    {
        work_queue->tick = OSA_TimeGetMsec();
    }
    bt_fifo_prepend(&work_queue->fifo, &work->work.node);
    work_queue_trigger_delayed_work();
    OSA_MutexUnlock(work_queue->mutex);
}

void bt_delayed_work_queue_insert_tail(struct bt_delayed_work_queue *work_queue, struct bt_delayed_work *work)
{
    if (0 != (work->work.state & BT_WORK_STATE_DELAY_PENDING))
    {
        return;
    }
    work->work.state |= BT_WORK_STATE_DELAY_PENDING;

    (void)OSA_MutexLock(work_queue->mutex, osaWaitForever_c);
    if (bt_fifo_is_empty(&work_queue->fifo) > 0)
    {
        work_queue->tick = OSA_TimeGetMsec();
    }
    bt_fifo_append(&work_queue->fifo, &work->work.node);
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

    LOG_INF("%p, delay task submit with timeOut %d\n",work, work->timeOut);

    (void)OSA_MutexLock(work_queue->mutex, osaWaitForever_c);
    if (bt_fifo_is_empty(&work_queue->fifo) > 0)
    {
        work_queue->tick = OSA_TimeGetMsec();
    }

    p = (struct bt_delayed_work *)work_queue->fifo.list.head;
    while (NULL != p)
    {
        if (p->timeOut <= work->timeOut)
        {
            q = p;
            p = (struct bt_delayed_work *)p->work.node.next;
        }
        else
        {
            break;
        }
    }
    bt_fifo_insert(&work_queue->fifo, q, &work->work.node);
    work_queue_trigger_delayed_work();
    OSA_MutexUnlock(work_queue->mutex);
}

void bt_delayed_work_queue_remove(struct bt_delayed_work_queue *work_queue, struct bt_delayed_work *work)
{
    (void)OSA_MutexLock(work_queue->mutex, osaWaitForever_c);
    if (bt_fifo_remove(&work_queue->fifo, &work->work.node) == true)
    {
        work->work.state &= ~BT_WORK_STATE_DELAY_PENDING;
    }
    OSA_MutexUnlock(work_queue->mutex);
}

struct bt_delayed_work *k_delayed_work_queue_remove_head(struct bt_delayed_work_queue *work_queue)
{
    struct bt_delayed_work *p;

    (void)OSA_MutexLock(work_queue->mutex, osaWaitForever_c);
    p = bt_fifo_get(&work_queue->fifo, 0);
    if (NULL != p)
    {
        p->work.state &= ~BT_WORK_STATE_DELAY_PENDING;
    }
    OSA_MutexUnlock(work_queue->mutex);
    return p;
}

struct bt_delayed_work *k_delayed_work_queue_remove_tail(struct bt_delayed_work_queue *work_queue)
{
    struct bt_delayed_work *p;

    (void)OSA_MutexLock(work_queue->mutex, osaWaitForever_c);
    p = bt_fifo_peek_tail(&work_queue->fifo);
    if (NULL != p)
    {
        bt_fifo_remove(&work_queue->fifo, p);
        p->work.state &= ~BT_WORK_STATE_DELAY_PENDING;
    }
    OSA_MutexUnlock(work_queue->mutex);
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

    p = bt_fifo_peek_head(&work_queue->fifo);

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
                q = (struct bt_delayed_work *)p->work.node.next;
                if (p->timeOut > delta)
                {
                    p->timeOut -= delta;
                }
                else
                {
                    p->timeOut = 0;
                    LOG_INF("%p, delay task timeout\n",(uint32_t)p);
                    bt_delayed_work_queue_remove(work_queue, p);
                    temp = (void *)p;
                    bt_work_submit((struct bt_work *)temp);
                }
                p = q;
            }
        } while(false);
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
        LOG_WRN("%p, delay task has been submit %d\n", work, delay);
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