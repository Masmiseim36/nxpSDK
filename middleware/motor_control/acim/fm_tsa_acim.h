/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _FM_TSA_ACIM_H_
#define _FM_TSA_ACIM_H_

#include "freemaster.h"
#include "freemaster_tsa.h"

#include "m1_sm_ref_sol.h"
#include "mid_sm_states.h"

/* global control variables */
extern bool_t bDemoMode;

/* global used misc variables */
extern uint32_t g_ui32NumberOfCycles;
extern uint32_t g_ui32MaxNumberOfCycles;

/* Application and board ID */
extern app_ver_t g_sAppIdFM;

#endif
