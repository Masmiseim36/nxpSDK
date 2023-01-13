/*
 * Copyright 2020 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _MID_MC_API_CONNECTOR_H_
#define _MID_MC_API_CONNECTOR_H_

#include "mid_def.h"

/*******************************************************************************
 * API
 ******************************************************************************/
/*!
 * @brief   Function sets External Position to FOC
 *
 * @param   f16PosExt - Required FOC electrical position.
 *
 * @return  none
 */
RAM_FUNC_LIB
void MID_MC_SetExternalPosEl(frac16_t f16PosExt);

/*!
 * @brief   Function gets External Position from FOC
 *
 * @param   void
 *
 * @return  Actual FOC external electrical position [frac].
 */
RAM_FUNC_LIB
frac16_t MID_MC_GetExternalPosEl(void);

/*!
 * @brief   Function gets Estimated Position from FOC
 *
 * @param   void
 *
 * @return  Actual FOC estimated electrical position [frac].
 */
RAM_FUNC_LIB
frac16_t MID_MC_GetEstimatedPosEl(void);

/*!
 * @brief   Function sets DQ voltages, used in Voltage control
 *
 * @param   sUDQReq     Required DQ voltages [V]
 *
 * @return  None
 */
void MID_MC_SetUDQReq(GMCLIB_2COOR_DQ_T_FLT sUDQReq);

/*!
 * @brief   Function gets DQ voltages
 *
 * @param   *sUDQReq    Structure returning DQ voltage [V]
 *
 * @return  None
 */
void MID_MC_GetUDQReq(GMCLIB_2COOR_DQ_T_FLT *sUDQReq);

/*!
 * @brief   Function gets observed voltages
 *
 * @param   *sEDQObsrv  Structure returning DQ observed voltage [V]
 *
 * @return  None
 */
RAM_FUNC_LIB
void MID_MC_GetEDQObsrv(GMCLIB_2COOR_DQ_T_FLT *sEDQObsrv);

/*!
 * @brief   Function sets DQ required currents, used in Current control
 *
 * @param   sIDQReq Required DQ currents [A]
 *
 * @return  None
 */
RAM_FUNC_LIB
void MID_MC_SetIDQReq(GMCLIB_2COOR_DQ_T_FLT sIDQReq);

/*!
 * @brief   Function gets actual DQ currents
 *
 * @param   *sIDQfbck   Structure returning DQ current [A]
 *
 * @return  None
 */
RAM_FUNC_LIB
void MID_MC_GetIDQfbck(GMCLIB_2COOR_DQ_T_FLT *sIDQfbck);

/*!
 * @brief   Function gets actual DCbus voltage
 *
 * @param   void
 *
 * @return  Actual filtered DCbus voltage [V]
 */
float_t MID_MC_GetUDCbusFilt(void);

/*!
 * @brief   Function gets actual estimated electrical speed
 *
 * @param   void
 *
 * @return  Actual Estimated Electrical Speed [electrical rad/s]
 */
RAM_FUNC_LIB
float_t MID_MC_GetSpeedElEst(void);

/*!
 * @brief   Function Enables/Disables FOC Current loop
 *
 * @param   bValue  TRUE = current loop On, FALSE = current loop Off
 *
 * @return  None
 */
RAM_FUNC_LIB
void MID_MC_EnableCurrentLoop(bool_t bValue);

/*!
 * @brief   Function Enables/Disables FOC External Position On/Off
 *
 * @param   bValue  TRUE = external position On, FALSE = external position Off
 *
 * @return  None
 */
RAM_FUNC_LIB
void MID_MC_UseEstimatedPos(bool_t bValue);

/*!
 * @brief   Function Updates Bemf Observer parameters
 *
 * @param   fltI_gain - Bemf observer I gain
 *          fltU_gain - Bemf observer U gain
 *          fltE_gain - Bemf observer E gain
 *          fltWI_gain - Bemf observer WI gain
 *          fltBemfDQ_Kps - Bemf observer fltBemfDQ_Kps gain
 *          fltBemfDQ_Kis - Bemf observer fltBemfDQ_Kis gain
 *
 * @return  None
 */
RAM_FUNC_LIB
void MID_MC_UpdateBemfObsrv(float_t fltI_gain,
                        float_t fltU_gain,
                        float_t fltE_gain,
                        float_t fltWI_gain,
                        float_t fltBemfDQ_Kps,
                        float_t fltBemfDQ_Kis);

/*!
 * @brief   Function Updates Tracking Observer parameters
 *
 * @param   fltTO_Kpz = Tracking observer fltTO_Kpz gain
 *          fltTO_Kiz = Tracking observer fltTO_Kiz gain
 *          fltTO_Theta = Tracking observer fltTO_Theta gain
 *
 * @return  None
 */
RAM_FUNC_LIB
void MID_MC_UpdateTrackingObsrv(float_t fltTO_Kpz,
                                float_t fltTO_Kiz,
                                float_t fltTO_Theta);

/*!
 * @brief   Function reads voltages, currents, etc. for MID. Called before MID state
 *          machine. User defined.
 *
 * @param   void
 *
 * @return  None
 */
RAM_FUNC_LIB
void MID_MC_ReadSignals(void);

/*!
 * @brief   Function applying results/output from MID. Called after MID state
 *          machine. User defined.
 *
 * @param   void
 *
 * @return  None
 */
RAM_FUNC_LIB
void MID_MC_ApplySignals(void);

#endif /* _MID_MC_API_CONNECTOR_H_ */
