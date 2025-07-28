/*!
 * Copyright 2025 NXP
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * \file fwk_sysworkq.c
 * \brief Implements the system workqueue which is a workqueue available to any application.
 *
 */

/* -------------------------------------------------------------------------- */
/*                                  Includes                                  */
/* -------------------------------------------------------------------------- */

#include "fwk_workq.h"
#include "fsl_os_abstraction.h"

/* -------------------------------------------------------------------------- */
/*                               Private macros                               */
/* -------------------------------------------------------------------------- */

#ifndef CONFIG_FWK_SYSWORKQ_STACK_SIZE
#define CONFIG_FWK_SYSWORKQ_STACK_SIZE 608U
#endif

#ifndef CONFIG_FWK_SYSWORKQ_PRIO
#define CONFIG_FWK_SYSWORKQ_PRIO 4U
#endif

/* -------------------------------------------------------------------------- */
/*                               Private memory                               */
/* -------------------------------------------------------------------------- */

static fwk_workq_t sysworkq;
FWK_WORKQ_THREAD_DEFINE(sysworkq_thread, CONFIG_FWK_SYSWORKQ_PRIO, CONFIG_FWK_SYSWORKQ_STACK_SIZE);

/* -------------------------------------------------------------------------- */
/*                              Public functions                              */
/* -------------------------------------------------------------------------- */

int WORKQ_InitSysWorkQ(void)
{
    return WORKQ_Start(&sysworkq, FWK_WORKQ_THREAD(sysworkq_thread));
}

int WORKQ_Submit(fwk_work_t *work)
{
    return WORKQ_SubmitToQueue(&sysworkq, work);
}
