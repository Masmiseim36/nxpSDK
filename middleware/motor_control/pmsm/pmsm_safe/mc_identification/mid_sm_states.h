/*
 * Copyright 2020 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _MID_SM_STATES_H_
#define _MID_SM_STATES_H_

#include "mid_mc_api_connector.h"


/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* The default measurement configuration used to initialize the
   sUserMIDMeasConfig variable. */
#define MID_DEFAULT_MEAS_CONFIG {\
    .ui32ScheduledMeas     = 0UL,    \
    .fltAlignId            = 0.5F,   \
    .fltRsIdMeas           = 0.5F,   \
    .fltLsIdAmplitudeReq   = 0.5F,   \
    .fltLsUdIncrement      = 10.0F,  \
    .fltLsFreqStart        = 1000.0F,\
    .fltLsFreqDecrement    = 100.0F, \
    .fltLsFreqMin          = 400.0F, \
    .fltKeIdReqOpenLoop    = 0.8F,   \
    .fltKeFreqElReq        = 20.0F,  \
    .fltPpIdReqOpenLoop    = 0.5F,   \
    .fltPpFreqElReq        = 10.0F,  \
    .fltPwrStgRs           = 13.0F,  \
    .fltPwrStgIdCalib      = 1.0F,   \
    .fltMechKt             = 0.5F,   \
    .fltMechIqStartup      = 0.3F,   \
    .fltMechMergingCoeff   = 100.0F, \
    .fltMechIqAccelerate   = 0.3F,   \
    .fltMechIqDecelerate   = 0.05F,  \
    .fltMechSpeedThrsAccel = (800.0F / 60.F * 3.0F * 2.0F * FLOAT_PI), \
    .fltMechSpeedThrsDecel = (1100.0F / 60.F * 3.0F * 2.0F * FLOAT_PI),\
    .fltMechSpeedThrsInteg = (800.0F / 60.F * 3.0F * 2.0F * FLOAT_PI)}


/* The default dead-time compensation table. */
#define MID_DT_COMP_TABLE {\
    -2.4386586622E-2F, -2.43722219E-2F,   -2.42415443E-2F,  -2.41509825E-2F,   \
    -2.41572875E-2F,   -2.4017475544E-2F, -2.39643398E-2F,  -2.38279532E-2F,   \
    -2.37755961E-2F,   -2.3690067233E-2F, -2.36318539E-2F,  -2.3462478E-2F,    \
    -2.3400714622E-2F, -2.32382025E-2F,   -2.31145806E-2F,  -2.30035297E-2F,   \
    -2.2938029844E-2F, -2.27916836E-2F,   -2.26897634E-2F,  -2.2488364944E-2F, \
    -2.2210929544E-2F, -2.19244975E-2F,   -2.15885005E-2F,  -2.12034508E-2F,   \
    -2.08759605E-2F,   -2.05207318E-2F,   -1.9858278333E-2F, -1.8292408433E-2F,\
    -1.59956514E-2F,   -1.27685573E-2F,   -8.9696981011E-3F, -5.73827512E-3F,  \
    -3.39879677E-3F,                                                           \
     3.73969064E-3F,    6.80589676E-3F,    1.08431708E-2F,   1.42017239E-2F,   \
     1.70319769E-2F,    1.8937572833E-2F,  2.0156933E-2F,    2.07561738E-2F,   \
     2.11317166E-2F,    2.14119646E-2F,    2.17606444E-2F,   2.2021239622E-2F, \
     2.23847068E-2F,    2.25250274E-2F,    2.2733708844E-2F, 2.2814368822E-2F, \
     2.2939559E-2F,     2.29012686E-2F,    2.31188535E-2F,   2.3104703E-2F,    \
     2.33448334E-2F,    2.3377716544E-2F,  2.3580446833E-2F, 2.35803164E-2F,   \
     2.37541981E-2F,    2.3864520711E-2F,  2.39939522E-2F,   2.3982562122E-2F, \
     2.40831393E-2F,    2.41243206E-2F,    2.42110677E-2F,   2.4303324522E-2F}







/*******************************************************************************
 * Variables
 ******************************************************************************/
/*! @brief User control variables: */

/* MID start result user variable. */
extern uint32_t                ui32UserMIDStartResult;

/* MID measurement type selection user variable. */
extern mid_meas_type_t         eUserMIDMeasType;

/* MID measurement configuration user structure. */
extern mid_config_t            sUserMIDMeasConfig;

/* MID known and measured motor parameter set user structure. */
extern mid_motor_params_user_t sUserMIDMotorParamsKnown;
extern mid_motor_params_user_t sUserMIDMotorParamsMeas;

/* MID measurement status user variable. */
extern mid_status_t            sUserMIDStatus;

/* MID dead-time compensation table. */
extern float_t g_pfltMIDUdErrorLookUp[MID_CHAR_CURRENT_POINT_NUMBERS];

/*******************************************************************************
 * API
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*!
 * @brief  MID after-reset initialization function.
 *
 * @param  void
 *
 * @return none
 */
void MID_Init_AR(void);

/*!
 * @brief  MID fast-loop process function.
 *
 * @param  void
 *
 * @return none
 */
void MID_ProcessFast_FL(void);

/*!
 * @brief   MID measurement start function.

 * @details Motor parameters and measurement configuration must be correctly set
 *          in advance by  MID_UpdateMotorParamsManually() and MID_Config()
 *          functions to start the measurement.
 *          Check the return value for the start result. If non-zero value was
 *          returned, please check the known motor parameters and measurement
 *          configuration inputs.
 *
 * @param   eMeasurementType - measurement type that will be done.
 *
 * @return  Status word containing following measurement pre-check result:
 *           -b0 - Parameter Rs is missing when TRUE.
 *           -b1 - Parameter Ld is missing when TRUE.
 *           -b2 - Parameter Lq is missing when TRUE.
 *           -b3 - Parameter Ke is missing when TRUE.
 *           -b4 - Parameter Pp is missing when TRUE.
 */
uint32_t MID_Start_BL(mid_meas_type_t eMeasurementType);

/*!
 * @brief  MID measurement stop function.
 *
 * @param  void
 *
 * @return none
 */
void MID_Stop_BL(void);

/*!
 * @brief  Functions returns MID status.
 *
 * @param  psMIDStatus - Pointer to the MID status structure, which is updated
 *                       during the function call.
 *
 * @retval TRUE  - Measurement is ongoing.
 * @retval FALSE - MID is idle.
 */
bool_t MID_GetStatus_BL(mid_status_t *psMIDStatus);

/*!
 * @brief  Measurement configuration setup function.
 *
 * @param  psMeasConfig - Pointer to the measurement configuration structure.
 *
 * @return MID configuration result. See the mid_config_result_t data type
 *         definition for more details.
 */
mid_config_result_t MID_SetMeasConfig_BL(mid_config_t *psMeasConfig);

/*!
 * @brief   Function sets known machine parameters.
 *
 * @details The function can be called to provide known machine parameters prior
 *          measurement start.
 *
 * @note    This function MUST be called at least to set the non-zero number of
 *          pole-pairs.
 *          Providing correct parameter does not affect the electrical and
 *          mechanical parameter scheduling. Use MID_SetMeasConfig_BL() for
 *          this purpose.
 *
 * @param   sMotorParams - Pointer to motor parameters provided by the user.
 *
 * @return  None
 */
void MID_SetKnownMotorParams_BL(mid_motor_params_user_t *psMotorParams);

/*!
 * @brief   Function gets measured and/or known machine parameters.
 *
 * @param   sMotorParams - Pointer to the motor parameters structure destination.
 *
 * @return  None
 */
void MID_GetMotorParams_BL(mid_motor_params_user_t *psMotorParams);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* _MID_SM_STATES_ */
