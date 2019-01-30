/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
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
    FTM_Type *pui32PwmBase;     /* pointer to phase A top value */
    uint16_t ui16ChanBottomPhA; /* number of channel for phase A bottom */
    uint16_t ui16ChanTopPhA;    /* number of channel for phase A top */
    uint16_t ui16ChanBottomPhB; /* number of channel for phase B bottom */
    uint16_t ui16ChanTopPhB;    /* number of channel for phase B top */
    uint16_t ui16ChanBottomPhC; /* number of channel for phase C bottom */
    uint16_t ui16ChanTopPhC;    /* number of channel for phase C top */
    uint16_t ui16PwmModulo;     /* FTM MODULO Value */
    const char *pcBldcTable;    /* pointer to BLDC commutation Table */
} mcdrv_pwm3ph_ftm_t;

typedef struct _mcdrv_pwm3ph_ftm_init
{
    FTM_Type *pui32PwmBase;      /* pointer to phase A top value */
    uint16_t ui16ChanPairNumPhA; /* number of pair channel for phase A */
    uint16_t ui16ChanPairNumPhB; /* number of pair channel for phase B */
    uint16_t ui16ChanPairNumPhC; /* number of pair channel for phase C */
    uint16_t ui16PwmModulo;      /* FTM MODULO Value */
    const char *pcBldcTable;     /* pointer to BLDC commutation Table */
} mcdrv_pwm3ph_ftm_init_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief Initialization function of 3-phase PWM FTM structure initialization
 *
 * @param this   Pointer to the current object
 * @param init   Pointer to initialization structure
 *
 * @return boot_t true on success
 */
bool_t MCDRV_FtmPwm3PhInit(mcdrv_pwm3ph_ftm_t *this, mcdrv_pwm3ph_ftm_init_t *init);

/*!
 * @brief Function set duty cycle from input parameter
 *
 * @param this   		Pointer to the current object
 * @param i16InpDuty    New input duty cycle to the pwma module
 *
 * @return boot_t true on success
 */
bool_t MCDRV_FtmSetDutyCycle(mcdrv_pwm3ph_ftm_t *this, int16_t i16InpDuty);

/*!
* @brief Function set pwm sector from input
*
* @param this   		Pointer to the current object
* @param sector   		Actual commutation sector
*
* @return boot_t true on success
*/
bool_t MCDRV_FtmSetPwmOutput(mcdrv_pwm3ph_ftm_t *this, int16_t i16Sector);

#ifdef __cplusplus
}
#endif

#endif /* _MCDRV_FTM_PWM3PH_H_ */

