/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _M1_SM_REF_SOL_H_
#define _M1_SM_REF_SOL_H_

#include "sm_common_enc.h"
#include "m1_pmsm_appconfig.h"
#include "state_machine.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define M1_SPEED_CONV_SCALE 628.06F /* Speed conversion scale */

#define MCAT_SENSORLESS_CTRL 0U /* Sensorless control flag */
#define MCAT_ENC_CTRL 1U        /* Position quadrature encoder control flag */

/* Application info */
typedef struct _app_ver
{
    char cBoardID[15];
    char cExampleID[30];
    char cAppVer[5];
    uint16_t ui16FeatureSet;
} app_ver_t;
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
void M1_SetAppSwitch(bool_t bValue);

/*!
 * @brief Get application switch value
 *
 * @param void  No input parameter
 *
 * @return bool_t Return bool value, true or false
 */
bool_t M1_GetAppSwitch(void);

/*!
 * @brief Get application state
 *
 * @param void  No input parameter
 *
 * @return uint16_t Return current application state
 */
uint16_t M1_GetAppState(void);

/*!
 * @brief Set spin speed of the motor in float value
 *
 * @param fltSpeedCmd  Speed command - set speed
 *
 * @return None
 */
void M1_SetSpeed(float_t fltSpeedCmd);

/*!
 * @brief Set position of the motor in acc value
 *
 * @param a32PositionCmd  Position command - set position
 *
 * @return None
 */
void M1_SetPosition(acc32_t a32PositionCmdDemo);

/*!
 * @brief Get spin speed of the motor in float value
 *
 * @param void  No input parameter
 *
 * @return float_t Float value of the current speed
 */
float_t M1_GetSpeed(void);

#ifdef __cplusplus
}
#endif

#endif /* _M1_SM_REF_SOL_H_ */
