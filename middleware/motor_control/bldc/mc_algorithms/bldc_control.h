/*
 * The Clear BSD License
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 * that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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

