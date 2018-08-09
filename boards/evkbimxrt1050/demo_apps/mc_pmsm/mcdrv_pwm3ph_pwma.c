/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
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
    
    uint8_t ui8MaskTemp = 0U;  
    
    ui8MaskTemp = (1U << (this->ui16PhASubNum))|(1U << (this->ui16PhBSubNum))|(1U << (this->ui16PhCSubNum));  
      
    /* PWM outputs of sub-modules 0,1 and 2 enabled */
    /* PWM_A output */
    this->pui32PwmBaseAddress->OUTEN =
        (this->pui32PwmBaseAddress->OUTEN & ~PWM_OUTEN_PWMA_EN_MASK) | PWM_OUTEN_PWMA_EN(ui8MaskTemp) 
          | (this->pui32PwmBaseAddress->OUTEN);

    /* PWM_B output */
    this->pui32PwmBaseAddress->OUTEN =
        (this->pui32PwmBaseAddress->OUTEN & ~PWM_OUTEN_PWMB_EN_MASK) | PWM_OUTEN_PWMB_EN(ui8MaskTemp) 
          | (this->pui32PwmBaseAddress->OUTEN);

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
    
    uint32_t    ui32MaskTemp = 0U;
    uint16_t     ui16PhSubTemp = 0U; 

    ui16PhSubTemp = ~((1U << (this->ui16PhASubNum))|(1U << (this->ui16PhBSubNum))|(1U << (this->ui16PhCSubNum))); 
    
    /* PWM outputs of used PWM sub-modules disabled */
    /* PWM_A output */
    ui32MaskTemp = ((this->pui32PwmBaseAddress->OUTEN & PWM_OUTEN_PWMA_EN_MASK) 
                    >> PWM_OUTEN_PWMA_EN_SHIFT) & ui16PhSubTemp;
    
    this->pui32PwmBaseAddress->OUTEN =
      (this->pui32PwmBaseAddress->OUTEN & ~PWM_OUTEN_PWMA_EN_MASK) | PWM_OUTEN_PWMA_EN(ui32MaskTemp);
     
    /* PWM_B output */  
    ui32MaskTemp = ((this->pui32PwmBaseAddress->OUTEN & PWM_OUTEN_PWMB_EN_MASK) 
                    >> PWM_OUTEN_PWMB_EN_SHIFT) & ui16PhSubTemp;
    
    this->pui32PwmBaseAddress->OUTEN =
        (this->pui32PwmBaseAddress->OUTEN & ~PWM_OUTEN_PWMB_EN_MASK) | PWM_OUTEN_PWMB_EN(ui32MaskTemp);

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
    this->pui32PwmBaseAddress->FSTS = ((this->pui32PwmBaseAddress->FSTS & ~(PWM_FSTS_FFLAG_MASK)) |
                                       (1 << this->ui16FaultFixNum | 1 << this->ui16FaultAdjNum));

    return ((s_statusPass > 0));
}

