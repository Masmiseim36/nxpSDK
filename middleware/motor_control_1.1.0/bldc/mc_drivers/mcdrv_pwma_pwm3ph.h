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
#ifndef _MCDRV_PWMA_PWM3PH_H_
#define _MCDRV_PWMA_PWM3PH_H_

#include "mlib.h"
#include "mlib_types.h"
#include "fsl_device_registers.h"
#include "gmclib.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define MCDRV_PWMA (1)

typedef struct _mcdrv_pwma_pwm3ph
{
    GMCLIB_3COOR_T_F16 *psUABC;    /* pointer to the 3-phase pwm duty cycles */
    PWM_Type *pui32PwmBaseAddress; /* PWMA base address */
    uint16_t ui16PhASubNum;        /* PWMA phase A sub-module number */
    uint16_t ui16PhBSubNum;        /* PWMA phase B sub-module number */
    uint16_t ui16PhCSubNum;        /* PWMA phase C sub-module number */
    uint16_t ui16FaultFixNum;      /* PWMA fault number for fixed over-current fault detection */
    uint16_t ui16FaultAdjNum;      /* PWMA fault number for adjustable over-current fault detection */
    uint16_t ui16PwmModulo;        /* PWMA MODULO Value */
    const uint16_t *pcBldcTable;   /* pointer to BLDC commutation Table */
} mcdrv_pwma_pwm3ph_t;

typedef struct _mcdrv_pwma_pwm3ph_init
{
    PWM_Type *pui32PwmBaseAddress; /* PWMA base address */
    uint16_t ui16PhASubNum;        /* PWMA phase A sub-module number */
    uint16_t ui16PhBSubNum;        /* PWMA phase B sub-module number */
    uint16_t ui16PhCSubNum;        /* PWMA phase C sub-module number */
    uint16_t ui16FaultFixNum;      /* PWMA fault number for fixed over-current fault detection */
    uint16_t ui16FaultAdjNum;      /* PWMA fault number for adjustable over-current fault detection */
    uint16_t ui16PwmModulo;        /* PWMA MODULO Value */
    const uint16_t *pcBldcTable;   /* pointer to BLDC commutation Table */
} mcdrv_pwma_pwm3ph_init_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif
/*!
 * @brief Initialization function of 3-phase PWM, PWMA structure initialization
 *
 * @param this   Pointer to the current object
 * @param init   Pointer to initialization structure
 *
 * @return boot_t true on success
 */
bool_t MCDRV_eFlexPwm3PhInit(mcdrv_pwma_pwm3ph_t *this, mcdrv_pwma_pwm3ph_init_t *init);

/*!
 * @brief Function updates FTM value register
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_eFlexPwm3PhSet(mcdrv_pwma_pwm3ph_t *this);

/*!
 * @brief Function enables PWM outputs
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_eFlexPwm3PhOutEn(mcdrv_pwma_pwm3ph_t *this);

/*!
 * @brief Function disables PWM outputs
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_eFlexPwm3PhOutDis(mcdrv_pwma_pwm3ph_t *this);

/*!
 * @brief Function return actual value of over current flag
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_eFlexPwm3PhFltGet(mcdrv_pwma_pwm3ph_t *this);

/*!
 * @brief Function set duty cycle from input parameter
 *
 * @param this   		Pointer to the current object
 * @param i16InpDuty    New input duty cycle to the pwma module
 *
 * @return boot_t true on success
 */
bool_t MCDRV_eFlexSetPwmOutput(mcdrv_pwma_pwm3ph_t *this, int16_t i16Sector);

/*!
* @brief Function set pwm sector from input
*
* @param this   		Pointer to the current object
* @param sector   		Actual commutation sector
*
* @return boot_t true on success
*/
bool_t MCDRV_eFlexSetDutyCycle(mcdrv_pwma_pwm3ph_t *this, int16_t i16InpDuty);

#ifdef __cplusplus
}
#endif

#endif /* _MCDRV_PWMA_PWM3PH_H_ */

