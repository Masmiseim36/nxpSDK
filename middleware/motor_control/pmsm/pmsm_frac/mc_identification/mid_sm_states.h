/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _MID_SM_STATES_H_
#define _MID_SM_STATES_H_

#include "mid_sm_ctrl.h"
#include "mid_auxiliary.h"
#include "rs_measure.h"
#include "pwrstg_characteristic.h"
#include "ls_measure.h"
#include "ke_measure.h"
#include "pp_measure.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*! @brief The MID state machine structure declaration */
extern mid_sm_app_ctrl_t g_sMIDCtrl;

/*! @brief global variables */
extern mid_get_rs_a1_t sMIDRs;
extern mid_get_char_a1_t sMIDPwrStgChar;
extern mid_get_ls_a1_t sMIDLs;
extern mid_get_ke_a1_t sMIDKe;
extern mid_get_pp_a1_t sMIDPp;
extern mid_align_a1_t sMIDAlignment;
extern uint16_t ui16FaultMID;
extern uint16_t ui16EnableMeasurement;
extern frac16_t *pf16Ud;
extern frac16_t *pf16Id;
extern frac16_t *pf16Uq;
extern frac16_t *pf16Iq;

/*! @brief FMSTR scales */
extern volatile float fltMIDresistanceScale;
extern volatile float fltMIDfrequencyScale;
extern volatile float fltMIDimpedanceScale;
extern volatile float fltMIDinductanceDScale;
extern volatile float fltMIDinductanceQScale;
extern volatile float fltMIDbemfConstScale;

#endif /* _MID_SM_STATES_ */
