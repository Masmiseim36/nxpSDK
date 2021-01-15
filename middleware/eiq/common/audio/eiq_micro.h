/*
 * Copyright 2018-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _EIQ_MICRO_H_
#define _EIQ_MICRO_H_

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

#include "eiq_iui.h"
#include "eiq_micro_conf.h"
#include "stdbool.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

typedef struct
{
  EIQ_IUi_t base;
  void (*setReadyCallback)(EIQ_IBufferAddrUpdater_t updater);
  uint8_t* (*getReadyBuff)(void);
} EIQ_Micro_t;

EIQ_Micro_t* EIQ_MicroInit();

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

#endif /* _EIQ_MICRO_H_ */
