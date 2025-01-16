/*
* Copyright 2016, Freescale Semiconductor, Inc.
* Copyright 2016-2021, 2024 NXP
*
* NXP Proprietary. This software is owned or controlled by NXP and may
* only be used strictly in accordance with the applicable license terms. 
* By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that
* you have read, and that you agree to comply with and are bound by,
* such license terms.  If you do not agree to be bound by the applicable
* license terms, then you may not retain, install, activate or otherwise
* use the software.
 */

#include "mcdrv_cmt_pwma.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Function read actual values of PWMA counter and value register
 *
 * @param this   Pointer to the current object
 *
 * @return none
 */
RAM_FUNC_LIB
void MCDRV_CmtGet(mcdrv_cmt_pwma_t *this)
{
  
    /* read actual values of counter and defined value register */
    *this->pui16PwmCntAct   = (uint16_t)this->pui32PwmBase->SM[this->ui8PwmSubmoduleNumber].CNT;
    *this->pui16PwmValueAct = (uint16_t)this->pui32PwmBase->SM[this->ui8PwmSubmoduleNumber].VAL2;
    
}

/*!
 * @brief Function update PWMA value register
 *
 * @param this   Pointer to the current object
 *
 * @return none
 */
RAM_FUNC_LIB
void MCDRV_CmtSet(mcdrv_cmt_pwma_t *this, uint16_t ui16TimeNew)
{
  
    /* update defined value register */
    this->pui32PwmBase->SM[this->ui8PwmSubmoduleNumber].VAL2 = PWM_VAL2_VAL2(ui16TimeNew);
    this->pui32PwmBase->MCTRL = (this->pui32PwmBase->MCTRL & ~PWM_MCTRL_LDOK_MASK) | PWM_MCTRL_LDOK(0x8);
    
}