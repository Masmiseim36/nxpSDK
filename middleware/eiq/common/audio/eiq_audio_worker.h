/*
 * Copyright 2018-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _EIQ_VIDEO_WORKER_H_
#define _EIQ_VIDEO_WORKER_H_

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

#include "eiq_iworker.h"
#include "eiq_micro.h"
#include "eiq_speaker.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief Image structure */
typedef struct
{
  EIQ_IWorker_t base;
  EIQ_Micro_t *receiver;
  EIQ_Speaker_t *sender;
} EIQ_AudioWorker_t;

EIQ_AudioWorker_t* EIQ_AudioWorkerInit();

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

#endif /* _IMAGE_H_ */
