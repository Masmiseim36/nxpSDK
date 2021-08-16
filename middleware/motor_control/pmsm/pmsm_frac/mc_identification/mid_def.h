/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _MID_DEF_H_
#define _MID_DEF_H_

#include "m1_pmsm_appconfig.h"
#include "mlib.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* MID Fault numbers */
#define MID_FAULT_NO_MOTOR (1U)
#define MID_FAULT_TOO_HIGH_RS (2U)
#define MID_FAULT_DC_CUR_NOT_REACHED (4U)
#define MID_FAULT_RS_OUT_OF_RANGE (8U)
#define MID_FAULT_LS_OUT_OF_RANGE (16U)
#define MID_FAULT_Z_OUT_OF_RANGE (32U)
#define MID_FAULT_AC_CUR_NOT_REACHED (64U)
#define MID_FAULT_KE_OUT_OF_RANGE (128U)

/* Current controllers' coefficients ensuring slow response for variable parameters */
#define MID_KP_GAIN ACC32(0.01935102757040625)
#define MID_KI_GAIN ACC32(0.0036080115416992)

/* Speed minimal ramp */
#define MID_SPEED_RAMP_UP FRAC16(0.000060606061)
#define MID_SPEED_RAMP_DOWN FRAC16(0.000060606061)

extern uint16_t ui16FaultMID;

#endif /* _MID_DEF_H_ */
