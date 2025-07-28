/*!
 * Copyright 2025 NXP
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * \file fwk_workq.c
 * \brief Simple workqueue implementation. A workqueue is an object that uses a dedicated thread
 *        to process work items in a FIFO manner.
 *
 */

/* -------------------------------------------------------------------------- */
/*                                  Includes                                  */
/* -------------------------------------------------------------------------- */

#include <string.h>
#include <stdbool.h>

#include "fwk_errno.h"
#include "fwk_hal_macros.h"
#include "fwk_workq.h"
#include "fsl_component_generic_list.h"
#include "fsl_os_abstraction.h"

/* -------------------------------------------------------------------------- */
/*                               Private macros                               */
/* -------------------------------------------------------------------------- */

#ifndef FWK_WORKQ_WAIT_TIME_MS
#define FWK_WORKQ_WAIT_TIME_MS osaWaitForever_c
#endif

/* -------------------------------------------------------------------------- */
/*                                Private types                               */
/* -------------------------------------------------------------------------- */

enum
{
    FWK_WORK_QUEUED  = 1U << 0,
    FWK_WORK_RUNNING = 1U << 1,
};

enum
{
    FWK_WORKQ_STARTED = 1U << 0,
};

/* -------------------------------------------------------------------------- */
/*                              Private functions                             */
/* -------------------------------------------------------------------------- */

static inline bool flag_test(const uint32_t *flags, uint32_t mask)
{
    return ((*flags & mask) != 0U);
}

static inline void flag_set(uint32_t *flags, uint32_t mask)
{
    *flags |= mask;
}

static inline void flag_clear(uint32_t *flags, uint32_t mask)
{
    *flags &= ~mask;
}

static inline uint32_t fwk_workq_lock(void)
{
    uint32_t intMask = 0U;

    OSA_EnterCritical(&intMask);
    return intMask;
}

static inline void fwk_workq_unlock(uint32_t intMask)
{
    OSA_ExitCritical(intMask);
}

static void workq_thread(void *workq)
{
    fwk_workq_t      *queue = (struct fwk_workq_t *)workq;
    fwk_work_t       *work;
    list_element_t   *node;
    osa_status_t      status;
    osa_event_flags_t flags;

    while (true)
    {
        status = OSA_EventWait(queue->notify, 1U, 1U, FWK_WORKQ_WAIT_TIME_MS, &flags);

        if (status == KOSA_StatusSuccess)
        {
            node = LIST_RemoveHead(&queue->pending);

            while (node != NULL)
            {
                uint32_t intMask = fwk_workq_lock();
                work             = CONTAINER_OF(node, fwk_work_t, node);
                flag_set(&work->flags, FWK_WORK_RUNNING);
                flag_clear(&work->flags, FWK_WORK_QUEUED);
                fwk_workq_unlock(intMask);

                work->handler(work);

                intMask = fwk_workq_lock();
                flag_clear(&work->flags, FWK_WORK_RUNNING);
                fwk_workq_unlock(intMask);

                node = LIST_RemoveHead(&queue->pending);
            }
        }

        if (gUseRtos_c == 0U)
        {
            /* baremetal support */
            break;
        }
    }
}

/* -------------------------------------------------------------------------- */
/*                              Public functions                              */
/* -------------------------------------------------------------------------- */

int WORKQ_Start(fwk_workq_t *queue, const osa_task_def_t *thread_def)
{
    int ret = 0;

    do
    {
        if ((queue == NULL) || (thread_def->stacksize == 0U))
        {
            ret = -EINVAL;
            break;
        }

        if (flag_test(&queue->flags, FWK_WORKQ_STARTED) == true)
        {
            ret = 1;
            break;
        }

        /* the OSA thread definition are const so make a local copy in RAM to change the thread handler */
        osa_task_def_t thread_local;
        (void)memcpy(&thread_local, thread_def, sizeof(osa_task_def_t));
        thread_local.pthread = workq_thread;

        LIST_Init(&queue->pending, 0U);

        if (OSA_EventCreate(queue->notify, 1U) != KOSA_StatusSuccess)
        {
            ret = -ENOMEM;
            break;
        }

        if (OSA_TaskCreate(queue->thread, &thread_local, queue) != KOSA_StatusSuccess)
        {
            (void)OSA_EventDestroy(queue->notify);
            ret = -ENOMEM;
            break;
        }

        uint32_t intMask = fwk_workq_lock();
        flag_set(&queue->flags, FWK_WORKQ_STARTED);
        fwk_workq_unlock(intMask);

    } while (false);

    return ret;
}

int WORKQ_SubmitToQueue(fwk_workq_t *queue, fwk_work_t *work)
{
    int ret = 0;

    do
    {
        if ((queue == NULL) || (work == NULL))
        {
            ret = -EINVAL;
            break;
        }

        if (work->handler == NULL)
        {
            ret = -EINVAL;
            break;
        }

        uint32_t intMask = fwk_workq_lock();

        if (flag_test(&work->flags, FWK_WORK_QUEUED) == true)
        {
            /* already queued */
            ret = 1;
            fwk_workq_unlock(intMask);
            break;
        }

        if (flag_test(&work->flags, FWK_WORK_RUNNING) == true)
        {
            /* if the work is currently running we have to use the
             * same queue it's running on to prevent handler re-entrancy */
            queue = work->queue;
            ret   = 2;
        }

        if (flag_test(&queue->flags, FWK_WORKQ_STARTED) == false)
        {
            /* the queue is not started */
            ret = -ENODEV;
            fwk_workq_unlock(intMask);
            break;
        }

        /* We are using a list not limited in size and in this case, LIST always returns success */
        (void)LIST_AddTail(&queue->pending, &work->node);

        flag_set(&work->flags, FWK_WORK_QUEUED);
        work->queue = queue;

        fwk_workq_unlock(intMask);

        /* Event set can't fail as we control the event handler */
        (void)OSA_EventSet(queue->notify, 1U);
    } while (false);

    return ret;
}
