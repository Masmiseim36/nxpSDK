/*
 * Copyright 2020 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _MID_SM_STATES_H_
#define _MID_SM_STATES_H_

#include "mid_def.h"
#include "mid_mc_api_connector.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* RL Estim measurement parameters. */ 
#if (M1_PWM_FREQ <= 10000U)
  #define F_SAMPLING 		M1_PWM_FREQ             /* AP MID - Sampling frequency [Hz]. */
#else
  #define F_SAMPLING 		(M1_PWM_FREQ / 2U)      /* AP MID - Sampling frequency [Hz]. */
  #define ESTIMRL_HALF_TS       (TRUE)                  /* Execute RL Estim every second ADC IQR */ 
#endif

#define NUM_MEAS 		20U                     /* AP MID - Number of measurement. */
#define I_NOMINAL 		5.0F                    /* AP MID - Nominal current [A]. */
#define I_POSMAX 		6.0F                    /* AP MID - Maximum positive current [A]. */
#define I_NEGMAX 		-6.0F                   /* AP MID - Minimum positive current [A]. */
#define I_LD			0.0F                    /* AP MID - Current to determine inductance in d-axis [A]. */
#define I_LQ			I_NOMINAL               /* AP MID - Current to determine inductance in q-axis [A]. */

/* The default measurement configuration used to initialize the
   sUserMIDMeasConfig variable. */
#define MID_DEFAULT_MEAS_CONFIG {\
    .fltAlignId            = 0.5F,   \
    .fltKeIdReqOpenLoop    = 0.8F,   \
    .fltKeFreqElReq        = 20.0F,  \
    .fltPpIdReqOpenLoop    = 0.5F,   \
    .fltPpFreqElReq        = 10.0F,  \
    .fltMechKt             = 0.5F,   \
    .fltMechIqStartup      = 0.3F,   \
    .fltMechMergingCoeff   = 100.0F, \
    .fltMechIqAccelerate   = 0.3F,   \
    .fltMechIqDecelerate   = 0.05F,  \
    .fltMechSpeedThrsAccel = (800.0F / 60.F * 3.0F * 2.0F * FLOAT_PI), \
    .fltMechSpeedThrsDecel = (1100.0F / 60.F * 3.0F * 2.0F * FLOAT_PI),\
    .fltMechSpeedThrsInteg = (800.0F / 60.F * 3.0F * 2.0F * FLOAT_PI)}

/* RL Estim config params structure. */
typedef struct _rl_estim_cfg_params_t
{
  float_t fltIDcNom;            /* Nominal DC current [A]. */
  float_t fltIDcPosMax;         /* Maximum DC current [A]. */
  float_t fltIDcNegMax;         /* Maximum allowed negative d-axis DC current [A]. The value of fltIDcNegMax must be negative or zero. */
  float_t fltIDcLd;             /* DC current used for Ld measurement [A]. */
  float_t fltIDcLq;             /* DC current used for Lq measurement [A]. */
}rl_estim_cfg_params_t;

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*! @brief User control variables: */

/* MID start result user variable. */
extern uint32_t                ui32UserMIDStartResult;

/* MID measurement type selection user variable. */
extern mid_meas_type_t         eUserMIDMeasType;

/* Global structure for all measurements */
extern mid_config_t            sUserMIDMeasConfig;
/* MID Config result user variable */
extern uint16_t ui16MeasConfigResult;

/* MID known and measured motor parameter set user structure. */
extern mid_motor_params_user_t sUserMIDMotorParamsKnown;
extern mid_motor_params_user_t sUserMIDMotorParamsMeas;

/* MID measurement status user variable. */
extern mid_status_t            sUserMIDStatus;

/* Control motor during MID */
extern mid_pmsm_t g_sMidDrive;

extern volatile float g_fltMIDvoltageScale;
extern volatile float g_fltMIDDCBvoltageScale;
extern volatile float g_fltMIDcurrentScale;
extern volatile float g_fltMIDspeedScale;
extern volatile float g_fltMIDspeedAngularScale;
extern volatile float g_fltMIDspeedMechanicalScale;

/* EstimRL variables */
extern MCAA_ESTIMRL_T_FLT g_sEstimRLStruct;
extern MCAA_ESTIMRL_RUN_T_FLT g_sEstimRLCtrlRun;
extern rl_estim_cfg_params_t g_sEstimRLInitFMSTR;
extern float_t	fltIDcPlot;
extern float_t	fltLdPlot;
extern float_t	fltLqPlot;
extern uint8_t u8ModeEstimRL;

/* Control FOC and MID */
extern mid_pmsm_t g_sMidDrive;          // -> mid_sm_states.h

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
RAM_FUNC_LIB
void MID_Init_AR(void);

/*!
 * @brief  MID fast-loop process function.
 *
 * @param  void
 *
 * @return none
 */
RAM_FUNC_LIB
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
RAM_FUNC_LIB
uint32_t MID_Start_BL(mid_meas_type_t eMeasurementType);

/*!
 * @brief  MID measurement stop function.
 *
 * @param  void
 *
 * @return none
 */
RAM_FUNC_LIB
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
RAM_FUNC_LIB
bool_t MID_GetStatus_BL(mid_status_t *psMIDStatus);

/*!
 * @brief  Measurement configuration setup function.
 *
 * @param  psMeasConfig - Pointer to the measurement configuration structure.
 *
 * @return MID configuration result. See the MID Configuration result defines
 *         for more details.
 */
RAM_FUNC_LIB
uint16_t MID_SetMeasConfig_BL(mid_config_t *psMeasConfig);

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
RAM_FUNC_LIB
void MID_SetKnownMotorParams_BL(mid_motor_params_user_t *psMotorParams);

/*!
 * @brief   Function gets measured and/or known machine parameters.
 *
 * @param   sMotorParams - Pointer to the motor parameters structure destination.
 *
 * @return  None
 */
RAM_FUNC_LIB
void MID_GetMotorParams_BL(mid_motor_params_user_t *psMotorParams);

/*!
 * @brief   Function starts or stops MID.
 *
 * @param   pMidCmd - Pointer to commnad for start/stop MID.
 *
 * @return  None
 */
RAM_FUNC_LIB
void MID_Process_BL(mid_app_cmd_t *pMidCmd);

/*!
 * @brief   Function returns actual MID state.
 *
 * @param   None
 *
 * @return  Actual MID state
 */
RAM_FUNC_LIB
uint16_t MID_GetActualState(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* _MID_SM_STATES_ */
