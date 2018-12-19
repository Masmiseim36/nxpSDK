/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _BLDC_CONTROL_H_
#define _BLDC_CONTROL_H_

#include "gflib.h"
#include "gmclib.h"
#include "gdflib.h"
#include "amclib.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief main BLDC control structure */
typedef struct _mcs_bldc_ctrl_t
{
    GFLIB_CTRL_PI_P_AW_T_A32 sSpeedPiParams;  /* speed PI controller structure */
    GFLIB_CTRL_PI_P_AW_T_A32 sIDcBusPiParams; /* DC-bus current PI controller structure */
    GDFLIB_FILTER_MA_T_A32 sIDcBusMaFilter;   /* DC-bus current moving average filter structure */
    GFLIB_RAMP_T_F32 sSpeedRampParams;        /* speed ramp structure */
    GDFLIB_FILTER_IIR1_T_F32 sUDcBusFilter;   /* Dc bus voltage filter */
    GDFLIB_FILTER_IIR1_T_F32 sIDcBusFilter;   /* Dc bus current filter */
    uint32_t ui32PeriodSixCmtSum;             /* sum of commutation periods */
    frac16_t f16SpeedMeasured;                /* average speed */
    frac16_t f16SpeedCmd;                     /* required speed command*/
    frac16_t f16SpeedMinimal;                 /* minimal speed threshold */
    frac16_t f16SpeedNominal;                 /* nominal speed */
    frac16_t f16SpeedRamp;                    /* speed ramp output */
    frac16_t f16SpeedPiErr;                   /* speed control error */
    frac16_t f16SpeedPiOutput;                /* speed PI controller output */
    int16_t i16SpeedScaleConst;               /* constant used for actual speed calculation */
    frac16_t f16IDcBusNoFilt;                 /* non-filtered DC-bus current */
    frac16_t f16IDcBus;                       /* DC-bus current value */
    frac16_t f16IDcBusRaw;                    /* raw DC-bus current measurement value */
    frac16_t f16IDcBusAlign;                  /* DC-bus alignment current */
    frac16_t f16IDcBusLim;                    /* DC-bus current limit value */
    frac16_t f16IDcBusZC;                     /* DC-bus current during zero-cross event */
    frac16_t f16IDcBusPiOutput;               /* DC-bus current PI controller */
    frac16_t f16IDcBusPiErr;                  /* DC-bus current control error */
    frac32_t f32IDcBusAcc;                    /* DC-bus current integrator variable */
    frac16_t f16UPhase;                       /* phase voltage measured value */
    frac16_t f16UPhaseBemf;                   /* BEMF voltage */
    frac16_t f16UDcBus;                       /* DC-bus voltage value */
    frac16_t f16UDcBusNoFilt;                 /* non-filtered DC-bus current */
    frac16_t f16DutyCycle;                    /* duty cycle */
    int16_t i16SectorCmt;                     /* commutation sector */
    uint16_t ui16MotorDir;                    /* spin direction */
    bool_t bIDcBusPiLimiting;                 /* current limitation flag */
    bool_t bSpeedPiStopIntFlag;               /* anti wind-up flag that stops PI controller integration */
    bool_t bIDcBusPiStopIntFlag;              /* anti wind-up flag that stops PI controller integration */

} mcs_bldc_ctrl_t;

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @name Motor control PMSM  functions
 * @{
 */

/*******************************************************************************
 * API
 ******************************************************************************/

 /*!
 * @brief BLDC motor aligment
 *
 * This function is used to align motor rotor to known initial position.
 *
 * @param psCtrlBLDC     The pointer of the BLDC control structure
 *
 * @return None
 */
void MCS_BLDCAlignment(mcs_bldc_ctrl_t *psCtrlBLDC);

/*!
 * @brief BLDC motor commutation process
 *
 * This function is used to commutate the the motor based on actual rotor position.
 *
 * @param psCtrlBLDC     The pointer of the BLDC control structure
 *
 * @return None
 */
void MCS_BLDCCommutation(mcs_bldc_ctrl_t *psCtrlBLDC);

/*!
 * @brief BLDC motor control routine
 *
 * This function is used to process control loop
 *
 * @param psCtrlBLDC     The pointer of the BLDC control structure
 *
 * @return None
 */
void MCS_BLDCControl(mcs_bldc_ctrl_t *psCtrlBLDC);

#ifdef __cplusplus
}
#endif

#endif /* _BLDC_CONTROL_H_ */

