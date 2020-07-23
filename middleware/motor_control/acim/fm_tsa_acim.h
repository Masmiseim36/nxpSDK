/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _FM_TSA_ACIM_H_
#define _FM_TSA_ACIM_H_

#include "freemaster.h"
#include "freemaster_tsa.h"

#include "mcdrv.h"

/* DEMO is defined in preprocessor */
#if (DEMO == 1)
#include "m1_sm_demo.h"
#else
#include "m1_sm_ref_sol.h"
#include "mid_sm_states.h"
#endif

#endif
