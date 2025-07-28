/*!
 * Copyright 2025 NXP
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * \file fwk_workq.h
 * \brief Simple workqueue implementation. A workqueue is an object that uses a dedicated thread
 *        to process work items in a FIFO manner.
 *
 */

#ifndef _FWK_WORKQ_H_
#define _FWK_WORKQ_H_

/* -------------------------------------------------------------------------- */
/*                                  Includes                                  */
/* -------------------------------------------------------------------------- */

#include "fsl_os_abstraction.h"
#include "fsl_component_generic_list.h"

/* -------------------------------------------------------------------------- */
/*                                Public macros                               */
/* -------------------------------------------------------------------------- */

/*!
 * \brief Defines a workqueue thread
 *
 * OSA API forces the user to define a thread handler, here we define a dummy
 * thread handler which will be replaced by the internal thread handler from
 * the workqueue service.
 */
#define FWK_WORKQ_THREAD_DEFINE(name, prio, stackSize) \
    static void name(void *workq)                      \
    {                                                  \
        (void)workq;                                   \
    }                                                  \
    static OSA_TASK_DEFINE(name, prio, 1, stackSize, false)

/*!
 * \brief Access a workqueue thread definition
 */
#define FWK_WORKQ_THREAD(name) OSA_TASK(name)

/*!
 * \brief Defines and initializes a work
 */
#define FWK_WORK_DEFINE(work, work_handler) \
    fwk_work_t work = {                     \
        .handler = work_handler,            \
    }

/* -------------------------------------------------------------------------- */
/*                                Public types                                */
/* -------------------------------------------------------------------------- */

typedef struct fwk_workq_t fwk_workq_t;
typedef struct fwk_work_t  fwk_work_t;
typedef void (*fwk_work_handler_t)(fwk_work_t *work);

struct fwk_workq_t
{
    OSA_TASK_HANDLE_DEFINE(thread);
    OSA_EVENT_HANDLE_DEFINE(notify);
    list_label_t pending;
    uint32_t     flags;
};

struct fwk_work_t
{
    list_element_t     node;
    fwk_work_handler_t handler;
    fwk_workq_t       *queue;
    uint32_t           flags;
};

/* -------------------------------------------------------------------------- */
/*                              Public functions                              */
/* -------------------------------------------------------------------------- */

/*!
 * \brief Initializes a work queue and starts its thread.
 *
 * This function should not be called twice for the same queue.
 *
 * \param[in] queue pointer to the queue.
 * \param[in] thread_def pointer to the thread definition provided by FWK_WORKQ_THREAD.
 * \return int 0 if success
 * \return int 1 if the queue was already started (no change done)
 * \return int -EINVAL if incorrect parameters.
 * \return int -ENOMEM if memory allocation failed.
 */
int WORKQ_Start(fwk_workq_t *queue, const osa_task_def_t *thread_def);

/*!
 * \brief Submit a work item to a work queue.
 *
 * \param[in] queue pointer to the work queue on which the work should run.
 * \param[in] work pointer to the work item.
 * \return int 0 if the work has been queued.
 * \return int 1 if the work was already queued.
 * \return int -EINVAL if incorrect parameters.
 * \return int -ENODEV if the queue is not started.
 */
int WORKQ_SubmitToQueue(fwk_workq_t *queue, fwk_work_t *work);

/*!
 * \brief Initializes and starts the system work queue.
 *
 * \return int as WORKQ_Start().
 */
int WORKQ_InitSysWorkQ(void);

/*!
 * \brief Submit a work item to the system work queue.
 *
 * \param[in] work pointer to the work item.
 * \return int as WORKQ_SubmitToQueue().
 */
int WORKQ_Submit(fwk_work_t *work);

#endif /* _FWK_WORKQ_H_ */