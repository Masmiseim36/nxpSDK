/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
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

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Function updates FTM value register
 *
 * @param this   Pointer to the current object
 *
 * @return none
 */
RAM_FUNC_LIB
void MCDRV_eFlexPwm3PhSet(mcdrv_pwm3ph_pwma_t *this)
{
    frac16_t f16DutyCycle, f16DutyCycleTemp, f16ModuloTemp;
    GMCLIB_3COOR_T_F16 sUABCtemp;

    /* pointer to duty cycle structure */
    sUABCtemp = *this->psUABC;

    /* get modulo value from module 0 VAL1 register  */
    f16ModuloTemp = this->pui32PwmBaseAddress->SM[this->ui16PhASubNum].VAL1 + 1;

    /* phase A */
    f16DutyCycle                                            = MLIB_Mul_F16(f16ModuloTemp, sUABCtemp.f16A);
    f16DutyCycleTemp                                        = MLIB_Neg_F16(f16DutyCycle);
    this->pui32PwmBaseAddress->SM[this->ui16PhASubNum].VAL2 = f16DutyCycleTemp;
    this->pui32PwmBaseAddress->SM[this->ui16PhASubNum].VAL3 = f16DutyCycle;

    /* phase B */
    f16DutyCycle                                            = MLIB_Mul_F16(f16ModuloTemp, sUABCtemp.f16B);
    f16DutyCycleTemp                                        = MLIB_Neg_F16(f16DutyCycle);
    this->pui32PwmBaseAddress->SM[this->ui16PhBSubNum].VAL2 = f16DutyCycleTemp;
    this->pui32PwmBaseAddress->SM[this->ui16PhBSubNum].VAL3 = f16DutyCycle;

    /* phase C */
    f16DutyCycle                                            = MLIB_Mul_F16(f16ModuloTemp, sUABCtemp.f16C);
    f16DutyCycleTemp                                        = MLIB_Neg_F16(f16DutyCycle);
    this->pui32PwmBaseAddress->SM[this->ui16PhCSubNum].VAL2 = f16DutyCycleTemp;
    this->pui32PwmBaseAddress->SM[this->ui16PhCSubNum].VAL3 = f16DutyCycle;

    /* set LDOK bits */
    this->pui32PwmBaseAddress->MCTRL |= PWM_MCTRL_LDOK_MASK;

}

/*!
 * @brief Function enables PWM outputs
 *
 * @param this   Pointer to the current object
 *
 * @return none
 */
RAM_FUNC_LIB
void MCDRV_eFlexPwm3PhOutEn(mcdrv_pwm3ph_pwma_t *this)
{

    uint8_t ui8MaskTemp = 0U;

    ui8MaskTemp = (1U << (this->ui16PhASubNum)) | (1U << (this->ui16PhBSubNum)) | (1U << (this->ui16PhCSubNum));

    /* PWM outputs of sub-modules 0,1 and 2 enabled */
    /* PWM_A output */
    this->pui32PwmBaseAddress->OUTEN = (this->pui32PwmBaseAddress->OUTEN & ~(uint16_t)PWM_OUTEN_PWMA_EN_MASK) |
                                       PWM_OUTEN_PWMA_EN(ui8MaskTemp) | (this->pui32PwmBaseAddress->OUTEN);

    /* PWM_B output */
    this->pui32PwmBaseAddress->OUTEN = (this->pui32PwmBaseAddress->OUTEN & ~(uint16_t)PWM_OUTEN_PWMB_EN_MASK) |
                                       PWM_OUTEN_PWMB_EN(ui8MaskTemp) | (this->pui32PwmBaseAddress->OUTEN);

}

/*!
 * @brief Function disables PWM outputs
 *
 * @param this   Pointer to the current object
 *
 * @return none
 */
RAM_FUNC_LIB
void MCDRV_eFlexPwm3PhOutDis(mcdrv_pwm3ph_pwma_t *this)
{

    uint32_t ui32MaskTemp  = 0U;
    uint16_t ui16PhSubTemp = 0U;

    ui16PhSubTemp = ~((1U << (this->ui16PhASubNum)) | (1U << (this->ui16PhBSubNum)) | (1U << (this->ui16PhCSubNum)));

    /* PWM outputs of used PWM sub-modules disabled */
    /* PWM_A output */
    ui32MaskTemp =
        ((this->pui32PwmBaseAddress->OUTEN & PWM_OUTEN_PWMA_EN_MASK) >> PWM_OUTEN_PWMA_EN_SHIFT) & ui16PhSubTemp;

    this->pui32PwmBaseAddress->OUTEN =
        (this->pui32PwmBaseAddress->OUTEN & ~(uint16_t)PWM_OUTEN_PWMA_EN_MASK) | PWM_OUTEN_PWMA_EN(ui32MaskTemp);

    /* PWM_B output */
    ui32MaskTemp =
        ((this->pui32PwmBaseAddress->OUTEN & PWM_OUTEN_PWMB_EN_MASK) >> PWM_OUTEN_PWMB_EN_SHIFT) & ui16PhSubTemp;

    this->pui32PwmBaseAddress->OUTEN =
        (this->pui32PwmBaseAddress->OUTEN & ~(uint16_t)PWM_OUTEN_PWMB_EN_MASK) | PWM_OUTEN_PWMB_EN(ui32MaskTemp);

}

/*!
 * @brief Function return actual value of over current flag
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
RAM_FUNC_LIB
bool_t MCDRV_eFlexPwm3PhFltGet(mcdrv_pwm3ph_pwma_t *this)
{   
    bool_t bStatusPass = FALSE;	
    uint16_t ui16StatusFlags;
    uint16_t ui16StatusPins;
    
    /* read fault flags */    
    ui16StatusFlags = (((this->pui32PwmBaseAddress->FSTS & PWM_FSTS_FFLAG_MASK) >> PWM_FSTS_FFLAG_SHIFT) & ((uint16_t)(1) << this->ui16FaultFixNum | (uint16_t)(1) << this->ui16FaultAdjNum));
    
    /* read fault pins status */   
    /* Reading pin status because fault flag is only triggered by signal edge, there can be situations where fault signals are     
     * asserted the moment system is powered on, and eFlexPWM module hasn't been initialized yet. In thiscase, fault flags won't     
     * be set even though fault signals are valid.     
     *  */    
    ui16StatusPins = (((this->pui32PwmBaseAddress->FSTS & PWM_FSTS_FFPIN_MASK) >> PWM_FSTS_FFPIN_SHIFT) & ((uint16_t)(1) << this->ui16FaultFixNum | (uint16_t)(1) << this->ui16FaultAdjNum));
    
    /* clear faults flag */    
    this->pui32PwmBaseAddress->FSTS = ((this->pui32PwmBaseAddress->FSTS & ~(PWM_FSTS_FFLAG_MASK)) | ((uint16_t)(1) << this->ui16FaultFixNum | (uint16_t)(1) << this->ui16FaultAdjNum));
    
    if((ui16StatusFlags > 0)||(ui16StatusPins > 0))    
    {    	
       bStatusPass = TRUE;    
    } 
    else 
    {    	
       bStatusPass = FALSE;    
    }
    return(bStatusPass);
}

