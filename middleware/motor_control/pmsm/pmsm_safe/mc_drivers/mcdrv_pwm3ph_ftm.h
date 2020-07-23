/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _MCDRV_PWM3PH_FTM_H_
#define _MCDRV_PWM3PH_FTM_H_

MISRAC_DISABLE /* MISRA check disabled (IAR IDE only). */

#include "gmclib.h"

MISRAC_ENABLE /* MISRA check enabled (IAR IDE only). */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * API
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*!
 * @brief   Function updates FTM value registers based on the required phase
 *          duty cycle values.
 *
 * @param   sPhaseDuty - The required three-phase duty cycle structure.
 *
 * @return  none
 */
void MCDRV_fsFtmPwm3PhSet(GMCLIB_3COOR_T_F16 sPhaseDuty);

/*!
 * @brief   Function enables selected PWM outputs.
 *
 * @param   void
 *
 * @return  none
 */
void MCDRV_fsFtmPwm3PhOutEn(void);

/*!
 * @brief   Function disables all PWM outputs.
 *
 * @note    This will disables the FTM periphery PWM outputs.
 *
 * @param   void
 *
 * @return  none
 */
void MCDRV_fsFtmPwm3PhOutDis(void);

/*!
 * @brief   Function returns actual values of over-current flag.
 *
 * @param   void
 *
 * @return  Masked fault status register content. A non-zero value means that
 *          over-current fault was detected.
 */
uint16_t MCDRV_fsFtmPwm3PhFltGet(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _MCDRV_PWM3PH_FTM_H_ */

