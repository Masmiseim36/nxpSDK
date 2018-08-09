/*
 * The Clear BSD License
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 * that the following conditions are met:
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
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
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

