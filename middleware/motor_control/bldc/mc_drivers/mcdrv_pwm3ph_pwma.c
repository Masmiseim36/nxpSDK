/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mcdrv_pwm3ph_pwma.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

static bool_t s_statusPass;

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Initialization function of 3-phase PWM, PWMA structure initialization
 *
 * @param this   Pointer to the current object
 * @param init   Pointer to initialization structure
 *
 * @return boot_t true on success
 */
bool_t MCDRV_eFlexPwm3PhInit(mcdrv_pwm3ph_pwma_t *this, mcdrv_pwm3ph_pwma_init_t *init)
{
    s_statusPass = TRUE;

    this->pui32PwmBaseAddress = init->pui32PwmBaseAddress; /* FTM Base address */

    this->ui16PhASubNum = init->ui16PhASubNum; /* PWMA phase A sub-module number */
    this->ui16PhBSubNum = init->ui16PhBSubNum; /* PWMA phase B sub-module number */
    this->ui16PhCSubNum = init->ui16PhCSubNum; /* PWMA phase C sub-module number */

    this->ui16PwmModulo = init->ui16PwmModulo;
    this->pcBldcTable = init->pcBldcTable;

    return (s_statusPass);
}

/*!
 * @brief Function updates FTM value register
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_eFlexPwm3PhSet(mcdrv_pwm3ph_pwma_t *this)
{
    frac16_t f16DutyCycle, f16DutyCycleTemp, f16ModuloTemp;
    GMCLIB_3COOR_T_F16 sUABCtemp;

    s_statusPass = TRUE;

    /* pointer to duty cycle structure */
    sUABCtemp = *this->psUABC;

    /* get modulo value from module 0 VAL1 register  */
    f16ModuloTemp = this->pui32PwmBaseAddress->SM[this->ui16PhASubNum].VAL1 + 1;

    /* phase A */
    f16DutyCycle = MLIB_Mul_F16(f16ModuloTemp, sUABCtemp.f16A);
    f16DutyCycleTemp = MLIB_Neg_F16(f16DutyCycle);
    this->pui32PwmBaseAddress->SM[this->ui16PhASubNum].VAL2 = f16DutyCycleTemp;
    this->pui32PwmBaseAddress->SM[this->ui16PhASubNum].VAL3 = f16DutyCycle;

    /* phase B */
    f16DutyCycle = MLIB_Mul_F16(f16ModuloTemp, sUABCtemp.f16B);
    f16DutyCycleTemp = MLIB_Neg_F16(f16DutyCycle);
    this->pui32PwmBaseAddress->SM[this->ui16PhBSubNum].VAL2 = f16DutyCycleTemp;
    this->pui32PwmBaseAddress->SM[this->ui16PhBSubNum].VAL3 = f16DutyCycle;

    /* phase C */
    f16DutyCycle = MLIB_Mul_F16(f16ModuloTemp, sUABCtemp.f16C);
    f16DutyCycleTemp = MLIB_Neg_F16(f16DutyCycle);
    this->pui32PwmBaseAddress->SM[this->ui16PhCSubNum].VAL2 = f16DutyCycleTemp;
    this->pui32PwmBaseAddress->SM[this->ui16PhCSubNum].VAL3 = f16DutyCycle;

    /* set LDOK bits */
    this->pui32PwmBaseAddress->MCTRL |= PWM_MCTRL_LDOK_MASK;

    return (s_statusPass);
}

/*!
 * @brief Function enables PWM outputs
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_eFlexPwm3PhOutEn(mcdrv_pwm3ph_pwma_t *this)
{
    s_statusPass = TRUE;

    /* PWM outputs of sub-modules 0,1 and 2 enabled */
    /* PWM_A output */
    this->pui32PwmBaseAddress->OUTEN =
        (this->pui32PwmBaseAddress->OUTEN & ~(uint16_t)PWM_OUTEN_PWMA_EN_MASK) | PWM_OUTEN_PWMA_EN(0x7);

    /* PWM_B output */
    this->pui32PwmBaseAddress->OUTEN =
        (this->pui32PwmBaseAddress->OUTEN & ~(uint16_t)PWM_OUTEN_PWMB_EN_MASK) | PWM_OUTEN_PWMB_EN(0x7);

    return (s_statusPass);
}

/*!
 * @brief Function disables PWM outputs
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_eFlexPwm3PhOutDis(mcdrv_pwm3ph_pwma_t *this)
{
    s_statusPass = TRUE;

    /* PWM outputs of sub-modules 0,1 and 2 disabled */
    /* PWM_A output */
    this->pui32PwmBaseAddress->OUTEN =
        (this->pui32PwmBaseAddress->OUTEN & ~(uint16_t)PWM_OUTEN_PWMA_EN_MASK) | PWM_OUTEN_PWMA_EN(0x0);

    /* PWM_B output */
    this->pui32PwmBaseAddress->OUTEN =
        (this->pui32PwmBaseAddress->OUTEN & ~(uint16_t)PWM_OUTEN_PWMB_EN_MASK) | PWM_OUTEN_PWMB_EN(0x0);

    return (s_statusPass);
}

/*!
 * @brief Function return actual value of over current flag
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_eFlexPwm3PhFltGet(mcdrv_pwm3ph_pwma_t *this)
{
    /* read over-current flags */
    s_statusPass = (((this->pui32PwmBaseAddress->FSTS & PWM_FSTS_FFLAG_MASK) >> 0) &
                    (1 << this->ui16FaultFixNum | 1 << this->ui16FaultAdjNum));

    /* clear faults flag */
    this->pui32PwmBaseAddress->FSTS = ((this->pui32PwmBaseAddress->FSTS & ~(uint16_t)(PWM_FSTS_FFLAG_MASK)) |
                                       (1 << this->ui16FaultFixNum | 1 << this->ui16FaultAdjNum));

    return ((s_statusPass > 0));
}

/*!
 * @brief Function set duty cycle from input parameter
 *
 * @param this   		Pointer to the current object
 * @param i16InpDuty    New input duty cycle to the pwma module
 *
 * @return boot_t true on success
 */
bool_t MCDRV_eFlexSetDutyCycle(mcdrv_pwm3ph_pwma_t *this, int16_t i16InpDuty)
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

    this->pui32PwmBaseAddress->SM[0].VAL2 = (uint16_t)i16FirstEdge;
    this->pui32PwmBaseAddress->SM[1].VAL2 = (uint16_t)i16FirstEdge;
    this->pui32PwmBaseAddress->SM[2].VAL2 = (uint16_t)i16FirstEdge;

    this->pui32PwmBaseAddress->SM[0].VAL3 = (uint16_t)i16SecondEdge;
    this->pui32PwmBaseAddress->SM[1].VAL3 = (uint16_t)i16SecondEdge;
    this->pui32PwmBaseAddress->SM[2].VAL3 = (uint16_t)i16SecondEdge;

    /* set LDOK bits */
    this->pui32PwmBaseAddress->MCTRL |= PWM_MCTRL_LDOK(0x7);
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
bool_t MCDRV_eFlexSetPwmOutput(mcdrv_pwm3ph_pwma_t *this, int16_t i16Sector)
{
    s_statusPass = TRUE;

    this->pui32PwmBaseAddress->MASK = *((this->pcBldcTable) + (2 * i16Sector));
    this->pui32PwmBaseAddress->DTSRCSEL = *((this->pcBldcTable) + (2 * i16Sector + 1));

    this->pui32PwmBaseAddress->SM[0].CTRL2 |= PWM_CTRL2_FORCE(1);

    return (s_statusPass);
}

