/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _MCDRV_SCTIMER_LPC_H_
#define _MCDRV_SCTIMER_LPC_H_

#include "mlib.h"
#include "mlib_types.h"
#include "fsl_device_registers.h"
#include "gmclib.h"

#include "fsl_sctimer.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define MCDRV_PWMA (1)

/* init sensors/actuators pointers */
#define M1_SET_PTR_DUTY(par1) (g_sM1Pwm3ph.psUABC = &(par1))

typedef struct _mcdrv_pwm3ph_pwma
{
    GMCLIB_3COOR_T_F16 *psUABC; /* pointer to the 3-phase pwm duty cycles */

    SCT_Type *pui32PwmBaseAddress; /* SCTimer base address */

    uint16_t ui16DeadTimePWM;

    uint8_t updatedDutycycle;
    uint32_t eventNumberOutput0;
    uint32_t eventNumberOutput1;
    uint32_t eventNumberOutput2;
    uint32_t eventNumberOutput3;
    uint32_t eventNumberOutput4;
    uint32_t eventNumberOutput5;
    uint32_t eventNumberOutput6;

    uint32_t ui32OutSet0;
    uint32_t ui32OutSet1;
    uint32_t ui32OutSet2;
    uint32_t ui32OutSet3;
    uint32_t ui32OutSet4;
    uint32_t ui32OutSet5;

    uint32_t ui32OutClr0;
    uint32_t ui32OutClr1;
    uint32_t ui32OutClr2;
    uint32_t ui32OutClr3;
    uint32_t ui32OutClr4;
    uint32_t ui32OutClr5;

    uint16_t ui16FaultFixNum; /* PWMA fault number for fixed over-current fault detection */
    uint16_t ui16FaultAdjNum; /* PWMA fault number for adjustable over-current fault detection */
} mcdrv_sctimer_pwm_t;

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
bool_t MCDRV_scTmrPwm3PhSet(mcdrv_sctimer_pwm_t *this);

/*!
 * @brief Function enables PWM outputs
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_scTmrPwm3PhOutEn(mcdrv_sctimer_pwm_t *this);

/*!
 * @brief Function disables PWM outputs
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_scTmrPwm3PhOutDis(mcdrv_sctimer_pwm_t *this);

/*!
 * @brief Function initialite PWM outputs structure
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_scTmrPwm3PhOutInit(mcdrv_sctimer_pwm_t *this);

/*!
 * @brief Function return actual value of over current flag
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_scTmrPwm3PhFltGet(mcdrv_sctimer_pwm_t *this);

#ifdef __cplusplus
}
#endif

#endif /* _MCDRV_SCTIMER_LPC_H_ */
