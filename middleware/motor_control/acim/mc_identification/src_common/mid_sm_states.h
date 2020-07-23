/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _MID_SM_STATES_H_
#define _MID_SM_STATES_H_

#include "mid_sm_ctrl.h"
#include "char_pwrstg.h"
#include "pwrstg_characteristic.h"
#include "rs_measure.h"
#include "noload_test.h"
#include "blocked_test.h"
#include "mech_measure.h"
#include "calc_acim.h"

extern mid_sm_app_ctrl_t g_g_sMIDCtrl;

/*******************************************************************************
 * API
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

void M1_MIDApplyPar(mid_acim_alg_prms_a1_t *AlgPar);

#ifdef __cplusplus
}
#endif

#endif /* _MID_SM_STATES_ */
