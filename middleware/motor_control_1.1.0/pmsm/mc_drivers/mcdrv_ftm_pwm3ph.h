/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef _MCDRV_FTM_PWM3PH_H_
#define _MCDRV_FTM_PWM3PH_H_

#include "fsl_device_registers.h"
#include "mlib.h"
#include "gmclib.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define MCDRV_FTM0 (0)

/* init sensors/actuators pointers */
#define M1_SET_PTR_DUTY(par1) (g_sM1Pwm3ph.psUABC = &(par1))

typedef struct _mcdrv_ftm_pwm3ph
{
    GMCLIB_3COOR_T_F16 *psUABC; /* pointer to the 3-phase pwm duty cycles */
    FTM_Type *pui32PwmBase;     /* pointer to phase A top value */
    uint16_t ui16ChanBottomPhA; /* number of channel for phase A bottom */
    uint16_t ui16ChanTopPhA;    /* number of channel for phase A top */
    uint16_t ui16ChanBottomPhB; /* number of channel for phase B bottom */
    uint16_t ui16ChanTopPhB;    /* number of channel for phase B top */
    uint16_t ui16ChanBottomPhC; /* number of channel for phase C bottom */
    uint16_t ui16ChanTopPhC;    /* number of channel for phase C top */
    uint16_t ui16FaultNum;      /* FTM Fault number for over current fault detection */
    uint16_t ui16FaultFixNum;   /* FTM fault number for fixed over-current fault detection */
    uint16_t ui16FaultAdjNum;   /* FTM fault number for adjustable over-current fault detection */
} mcdrv_ftm_pwm3ph_t;

typedef struct _mcdrv_ftm_pwm3ph_init
{
    FTM_Type *pui32PwmBase;      /* pointer to phase A top value */
    uint16_t ui16ChanPairNumPhA; /* number of pair channel for phase A */
    uint16_t ui16ChanPairNumPhB; /* number of pair channel for phase B */
    uint16_t ui16ChanPairNumPhC; /* number of pair channel for phase C */
    uint16_t ui16FaultFixNum;    /* FTM fault number for fixed over-current fault detection */
    uint16_t ui16FaultAdjNum;    /* FTM fault number for adjustable over-current fault detection */
} mcdrv_ftm_pwm3ph_init_t;

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
bool_t MCDRV_FtmPwm3PhInit(mcdrv_ftm_pwm3ph_t *this, mcdrv_ftm_pwm3ph_init_t *init);

/*!
 * @brief FTM value register updates
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_FtmPwm3PhSet(mcdrv_ftm_pwm3ph_t *this);

/*!
 * @brief Function enables PWM outputs
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_FtmPwm3PhOutEn(mcdrv_ftm_pwm3ph_t *this);
/*!
 * @brief Function disables PWM outputs
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_FtmPwm3PhOutDis(mcdrv_ftm_pwm3ph_t *this);

/*!
 * @brief Function return actual value of over current flag
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_FtmPwm3PhFltGet(mcdrv_ftm_pwm3ph_t *this);

#ifdef __cplusplus
}
#endif

#endif /* _MCDRV_FTM_PWM3PH_H_ */

