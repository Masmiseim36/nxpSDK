/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
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

