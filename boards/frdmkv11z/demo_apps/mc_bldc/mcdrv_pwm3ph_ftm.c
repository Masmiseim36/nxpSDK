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
#include "mcdrv_pwm3ph_ftm.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

static bool_t s_statusPass;

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Initialization function of 3-phase PWM FTM structure initialization
 *
 * @param this   Pointer to the current object
 * @param init   Pointer to initialization structure
 *
 * @return boot_t true on success
 */
bool_t MCDRV_FtmPwm3PhInit(mcdrv_pwm3ph_ftm_t *this, mcdrv_pwm3ph_ftm_init_t *init)
{
    s_statusPass = TRUE;

    this->pui32PwmBase = init->pui32PwmBase; /* FTM Base address */

    this->ui16ChanTopPhA = 0 + init->ui16ChanPairNumPhA;    /* FTM channel number for Phase A top */
    this->ui16ChanBottomPhA = 1 + init->ui16ChanPairNumPhA; /* FTM channel number for Phase A bottom */
    this->ui16ChanTopPhB = 0 + init->ui16ChanPairNumPhB;    /* FTM channel number for Phase B top */
    this->ui16ChanBottomPhB = 1 + init->ui16ChanPairNumPhB; /* FTM channel number for Phase B bottom */
    this->ui16ChanTopPhC = 0 + init->ui16ChanPairNumPhC;    /* FTM channel number for Phase C top */
    this->ui16ChanBottomPhC = 1 + init->ui16ChanPairNumPhC; /* FTM channel number for Phase C bottom */
    this->ui16PwmModulo = init->ui16PwmModulo;              /* FTM MOD value */
    this->pcBldcTable = init->pcBldcTable;

    return (s_statusPass);
}

/*!
 * @brief Function set duty cycle from input parameter
 *
 * @param this   		Pointer to the current object
 * @param i16InpDuty    New input duty cycle to the pwma module
 *
 * @return boot_t true on success
 */
bool_t MCDRV_FtmSetDutyCycle(mcdrv_pwm3ph_ftm_t *this, int16_t i16InpDuty)
{
    int16_t i16FirstEdge, i16SecondEdge, i16Duty;

    s_statusPass = TRUE;

    i16Duty = MLIB_Mul_F16((i16InpDuty), (this->ui16PwmModulo) / 4);

    i16FirstEdge = -(this->ui16PwmModulo) / 4 - i16Duty;
    if (i16FirstEdge < (-(this->ui16PwmModulo) / 2))
        i16FirstEdge = -(this->ui16PwmModulo) / 2;

    i16SecondEdge = (this->ui16PwmModulo) / 4 + i16Duty;
    if (i16SecondEdge > ((this->ui16PwmModulo) / 2))
        i16SecondEdge = (this->ui16PwmModulo) / 2;

    this->pui32PwmBase->CONTROLS[0].CnV = i16FirstEdge;
    this->pui32PwmBase->CONTROLS[1].CnV = i16SecondEdge;
    this->pui32PwmBase->CONTROLS[2].CnV = i16FirstEdge;
    this->pui32PwmBase->CONTROLS[3].CnV = i16SecondEdge;
    this->pui32PwmBase->CONTROLS[4].CnV = i16FirstEdge;
    this->pui32PwmBase->CONTROLS[5].CnV = i16SecondEdge;

    this->pui32PwmBase->PWMLOAD |= (FTM_PWMLOAD_LDOK_MASK);

    return (s_statusPass);
}

/*!
* @brief Function set pwm sector from input
*
* @param this   		Pointer to the current object
* @param sector   		Actual commutation sector
*
* @return boot_t true on success
*/
bool_t MCDRV_FtmSetPwmOutput(mcdrv_pwm3ph_ftm_t *this, int16_t i16Sector)
{
    s_statusPass = TRUE;

    this->pui32PwmBase->INVCTRL = *((this->pcBldcTable) + (2 * i16Sector + 1));
    this->pui32PwmBase->OUTMASK = *((this->pcBldcTable) + (2 * i16Sector));

    this->pui32PwmBase->SYNC |= (FTM_SYNC_SWSYNC_MASK);
    return (s_statusPass);
}

