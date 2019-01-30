/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _MCDRV_PWM3PH_PWMA_H_
#define _MCDRV_PWM3PH_PWMA_H_

#include "mlib.h"
#include "mlib_types.h"
#include "fsl_device_registers.h"
#include "gmclib.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define MCDRV_PWMA (1)

/* init sensors/actuators pointers */
#define M1_SET_PTR_DUTY(par1) (g_sM1Pwm3ph.psUABC = &(par1))

typedef struct _mcdrv_pwm3ph_pwma
{
    GMCLIB_3COOR_T_F16 *psUABC;    /* pointer to the 3-phase pwm duty cycles */
    PWM_Type *pui32PwmBaseAddress; /* PWMA base address */
    uint16_t ui16PhASubNum;        /* PWMA phase A sub-module number */
    uint16_t ui16PhBSubNum;        /* PWMA phase B sub-module number */
    uint16_t ui16PhCSubNum;        /* PWMA phase C sub-module number */
    uint16_t ui16FaultFixNum;      /* PWMA fault number for fixed over-current fault detection */
    uint16_t ui16FaultAdjNum;      /* PWMA fault number for adjustable over-current fault detection */
} mcdrv_pwm3ph_pwma_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief Function updates FTM value register
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_eFlexPwm3PhSet(mcdrv_pwm3ph_pwma_t *this);

/*!
 * @brief Function enables PWM outputs
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_eFlexPwm3PhOutEn(mcdrv_pwm3ph_pwma_t *this);

/*!
 * @brief Function disables PWM outputs
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_eFlexPwm3PhOutDis(mcdrv_pwm3ph_pwma_t *this);

/*!
 * @brief Function return actual value of over current flag
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_eFlexPwm3PhFltGet(mcdrv_pwm3ph_pwma_t *this);

#ifdef __cplusplus
}
#endif

#endif /* _MCDRV_PWM3PH_PWMA_H_ */

