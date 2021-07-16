/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _FM_TSA_PMSM_H_
#define _FM_TSA_PMSM_H_

#include "freemaster.h"
#include "freemaster_tsa.h"
#include "mc_periph_init.h"

#ifdef PMSM_SNSLESS_ENC
#include "m1_sm_snsless_enc.h"
#else
#include "m1_sm_snsless.h"
#endif

#include "mid_sm_states.h"

/* global control variables */
#ifdef PMSM_SNSLESS_ENC
extern bool_t bDemoModePosition;
#endif

extern bool_t bDemoModeSpeed;

/* global used misc variables */
extern uint32_t g_ui32NumberOfCycles;
extern uint32_t g_ui32MaxNumberOfCycles;

/* Application and board ID  */
extern app_ver_t g_sAppIdFM;

#endif
