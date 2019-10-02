/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
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
 * @brief Function set duty cycle from input parameter
 *
 * @param this Pointer to the current object
 * @param i16InpDuty New input duty cycle to the pwma module
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

    this->pui32PwmBase->CONTROLS[this->ui16ChanPhA].CnV = i16FirstEdge;
    this->pui32PwmBase->CONTROLS[this->ui16ChanPhA + 1].CnV = i16SecondEdge;
    this->pui32PwmBase->CONTROLS[this->ui16ChanPhB].CnV = i16FirstEdge;
    this->pui32PwmBase->CONTROLS[this->ui16ChanPhB + 1].CnV = i16SecondEdge;
    this->pui32PwmBase->CONTROLS[this->ui16ChanPhC].CnV = i16FirstEdge;
    this->pui32PwmBase->CONTROLS[this->ui16ChanPhC + 1].CnV = i16SecondEdge;

    this->pui32PwmBase->PWMLOAD |= (FTM_PWMLOAD_LDOK_MASK);

    return (s_statusPass);
}

/*!
* @brief Function set pwm sector from input
*
* @param this Pointer to the current object
* @param sector Actual commutation sector
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

