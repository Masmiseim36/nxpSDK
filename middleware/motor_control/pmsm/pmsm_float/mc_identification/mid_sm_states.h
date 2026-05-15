/*
* Copyright 2020, 2024 NXP
*
* NXP Proprietary. This software is owned or controlled by NXP and may
* only be used strictly in accordance with the applicable license terms. 
* By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that
* you have read, and that you agree to comply with and are bound by,
* such license terms.  If you do not agree to be bound by the applicable
* license terms, then you may not retain, install, activate or otherwise
* use the software.
 */

#ifndef _MID_SM_STATES_H_
#define _MID_SM_STATES_H_

#include "mid_def.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Pp Assist config params structure. */
typedef struct _pp_assist_cfg_params_t
{
    float_t   fltIdReqOpenLoop;  /* Openloop current [A]. */
    float_t   fltFreqElReq;      /* Required Electrical Speed [Hz]. */
    /* Advanced parameters */
    float_t   fltRampTime;       /* Frequency ramp time [s]. */
    float_t   fltZeroPosTime;    /* Steady position time [s]. */
    float_t   fltUMax;           /* Maximal motor voltage [V]. */
    float_t   fltDutyCycleLimit; /* Maximum allowable duty cycle in frac [-]. */
    float_t   fltDPiPropGain;    /* Proportional gain of the D-axis current loop controller [-]. */
    float_t   fltDPiIntegGain;   /* Integral gain of the D-axis current loop controller [-]. */
    float_t   fltQPiPropGain;    /* Proportional gain of the Q-axis current loop controller [-]. */
    float_t   fltQPiIntegGain;   /* Integral gain of the Q-axis current loop controller [-]. */
}pp_assist_cfg_params_t;

/* RL Estim config params structure. */
typedef struct _rl_estim_cfg_params_t
{
    float_t fltIDcMeas;          /* Measurement DC current [A]. */
    float_t fltIDcPosMax;        /* Maximum DC current [A]. */
    float_t fltIDcNegMax;        /* Maximum allowed negative d-axis DC current [A]. The value of fltIDcNegMax must be negative or zero. */
    float_t fltIDcLd;            /* DC current used for Ld measurement [A]. */
    float_t fltIDcLq;            /* DC current used for Lq measurement [A]. */
}rl_estim_cfg_params_t;

/* BJ Estim config params structure. */
typedef struct _bj_estim_cfg_params_t
{
    float_t fltIMeas;            /* Measurement current [A]. */
    float_t fltNN;               /* Nominal speed [rpm]. */
    /* Advanced parameters */
    bool_t  bEstimFriction;      /* Enable "Advanced" mode (friction estimation) [-]. */
    float_t fltAlignTime;        /* Time needed for rotor alignment [s]. */
    float_t fltIReqOl;           /* Required d-axis current for open loop startup [A]. */
    float_t fltNStepDc;          /* Required DC speed step for ramp [delta rpm/s]. */
    float_t fltNStepAc;          /* Required AC speed step for ramp [delta rpm/s]. */
    float_t fltNDcReq1;          /* Required speed for measurement point 1 [rpm]. */
    float_t fltNDcReq2;          /* Required speed for measurement point 2 ("Advanced mode") [rpm]. */
    float_t fltNAcReq;           /* Required amplitude of the injected AC speed [rpm]. */
    float_t fltSSTimeoutTime;    /* Time to reach steady state before timeout [s]. */
    float_t fltSSTimeMin;        /* Minimum time needed for stable behavior [s]. */
    float_t fltSSTimeTrans;      /* Transition time during selection of injected frequency [s]. */
    float_t fltFInjMax;          /* Maximum possible injection frequency [Hz]. */                          
    float_t fltFInjMin;          /* Minimum possible injection frequency [Hz]. */
    float_t fltFInjStep;         /* Injection frequency calculation step [Hz]. */
    float_t fltDcPiPropGain;     /* Proportional gain of the speed loops DC controller [-]. */
    float_t fltDcPiIntegGain;    /* Integral gain of the speed loops DC controller [-]. */
    float_t fltAcPiPropGain;     /* Proportional gain of the speed loops AC controller [-]. */
    float_t fltAcPiIntegGain;    /* Integral gain of the speed loops AC controller [-]. */
}bj_estim_cfg_params_t;

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*! @brief User control variables: */

/* MID measurement type selection user variable. */
extern mid_meas_type_t         eUserMIDMeasType;

/* MID known motor parameters set by user structure. */
extern mid_motor_params_user_t sUserMIDMotorParamsKnown;
/* MID measured motor parameters structure. */
extern mid_motor_params_user_t sUserMIDMotorParamsMeas;

/* MID measurement status user variable. */
extern mid_status_t            sUserMIDStatus;

/* Control motor during MID */
extern mid_pmsm_t              g_sMidDrive;

extern volatile float g_fltMIDvoltageScale;
extern volatile float g_fltMIDDCBvoltageScale;
extern volatile float g_fltMIDcurrentScale;
extern volatile float g_fltMIDspeedScale;
extern volatile float g_fltMIDspeedAngularScale;
extern volatile float g_fltMIDspeedMechanicalScale;

/* Pp Assist variables */
extern MCAA_PPASSIST_T_FLT    g_sPpAssistStruct;
extern pp_assist_cfg_params_t g_sPpAssistInitFMSTR;

/* EstimRL variables */
extern MCAA_ESTIMRL_T_FLT     g_sEstimRLStruct;
extern MCAA_ESTIMRL_RUN_T_FLT g_sEstimRLCtrlRun;
extern rl_estim_cfg_params_t  g_sEstimRLInitFMSTR;
extern float_t	              fltIDcPlot;
extern float_t	              fltLdPlot;
extern float_t	              fltLqPlot;
extern uint8_t                u8ModeEstimRL;

/* EstimBJ variables */
extern MCAA_ESTIMBJ_T_FLT     g_sEstimBJStruct;
extern bj_estim_cfg_params_t  g_sEstimBJInitFMSTR;

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
 *
 * @param   eMeasurementType - measurement type that will be done.
 *
 * @return  none
 */
RAM_FUNC_LIB
void MID_Start_BL(mid_meas_type_t eMeasurementType);

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
 * @brief   Function sets known machine parameters.
 *
 * @details The function can be called to provide known machine parameters prior
 *          measurement start.
 *
 * @note    This function MUST be called at least to set the non-zero number of
 *          pole-pairs.
 *          Providing correct parameter does not affect the electrical and
 *          mechanical parameter scheduling.
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
