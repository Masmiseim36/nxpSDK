/*
*
* Copyright 2025 NXP
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

#ifndef _M1_SM_SERVO_REF_SOL_H_
#define _M1_SM_SERVO_REF_SOL_H_

#include "sm_servo_common.h"
#include "state_machine.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define MCAT_SENSORLESS_CTRL 0U /* Sensorless control flag */
#define MCAT_ENC_CTRL 1U        /* Position quadrature encoder control flag */

/*******************************************************************************
 * Variables
 ******************************************************************************/
extern bool_t g_bM1SwitchAppOnOff;
extern mcdef_pmsm_t g_sM1Drive;
extern sm_app_ctrl_t g_sM1Ctrl;
extern run_substate_t g_eM1StateRun;

extern volatile float g_fltM1voltageScale;
extern volatile float g_fltM1DCBvoltageScale;
extern volatile float g_fltM1currentScale;
extern volatile float g_fltM1speedScale;
extern volatile float g_fltM1speedAngularScale;
extern volatile float g_fltM1speedMechanicalScale;

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * API
 ******************************************************************************/

/*!
 * @brief Set application switch value to On or Off mode
 *
 * @param bValue  bool value, true - On of false - Off
 *
 * @return None
 */
RAM_FUNC_LIB
void M1_SetAppSwitch(bool_t bValue);

/*!
 * @brief Get application switch value
 *
 * @param void  No input parameter
 *
 * @return bool_t Return bool value, true or false
 */
RAM_FUNC_LIB
bool_t M1_GetAppSwitch(void);

/*!
 * @brief Get application state
 *
 * @param void  No input parameter
 *
 * @return uint16_t Return current application state
 */
RAM_FUNC_LIB
uint16_t M1_GetAppState(void);

/*!
 * @brief Set spin speed of the motor in float value
 *
 * @param fltSpeedCmd  Speed command - set speed
 *
 * @return None
 */
RAM_FUNC_LIB
void M1_SetSpeed(float_t fltSpeedCmd);

/*!
 * @brief Set position of the motor in acc value
 *
 * @param a32PositionCmd  Position command - set position
 *
 * @return None
 */
RAM_FUNC_LIB
void M1_SetPosition(acc32_t a32PositionCmdDemo);

/*!
 * @brief Get spin speed of the motor in float value
 *
 * @param void  No input parameter
 *
 * @return float_t Float value of the current speed
 */
RAM_FUNC_LIB
float_t M1_GetSpeed(void);

/*!
 * @brief Enable PWM outputs
 *
 * @param void  No input parameter
 *
 * @return None
 */
RAM_FUNC_LIB
void M1_OpenPWM(void);

/*!
 * @brief Disable PWM outputs
 *
 * @param void  No input parameter
 *
 * @return None
 */
RAM_FUNC_LIB
void M1_ClosePWM(void);

#ifdef __cplusplus
}
#endif

#endif /* _M1_SM_SERVO_REF_SOL_H_ */
