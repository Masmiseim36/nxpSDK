/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _MCDRV_FTM_PWM3PH_H_
#define _MCDRV_FTM_PWM3PH_H_

#include "mlib.h"
#include "gflib.h"
#include "gdflib.h"
#include "gmclib.h"

#include "gmclib_types.h"
#include "fsl_device_registers.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

typedef struct _mcdrv_pwm3ph_ftm
{
    GMCLIB_3COOR_T_F16 *psUABC; /* pointer to the 3-phase PWM duty cycles */
    FTM_Type *pui32PwmBase; /* pointer to phase A top value */
    uint16_t ui16ChanPhA; /* number of channel for phase A */
    uint16_t ui16ChanPhB; /* number of channel for phase A top */
    uint16_t ui16ChanPhC; /* number of channel for phase B bottom */
    uint16_t ui16PwmModulo; /* FTM MODULO Value */
    const char *pcBldcTable; /* pointer to BLDC commutation Table */
} mcdrv_pwm3ph_ftm_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief Function set duty cycle from input parameter
 *
 * @param this Pointer to the current object
 * @param i16InpDuty New input duty cycle to the pwma module
 *
 * @return boot_t true on success
 */
bool_t MCDRV_FtmSetDutyCycle(mcdrv_pwm3ph_ftm_t *this, int16_t i16InpDuty);

/*!
* @brief Function set pwm sector from input
*
* @param this Pointer to the current object
* @param sector Actual commutation sector
*
* @return boot_t true on success
*/
bool_t MCDRV_FtmSetPwmOutput(mcdrv_pwm3ph_ftm_t *this, int16_t i16Sector);

#ifdef __cplusplus
}
#endif

#endif /* _MCDRV_FTM_PWM3PH_H_ */

