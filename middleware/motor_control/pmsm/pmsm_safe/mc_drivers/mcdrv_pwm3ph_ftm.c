/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mcdrv_pwm3ph_ftm.h"
#include "mcdrv_periph_init.h"
#include "safety_flow_check.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Output enable mask. */
#define MCDRV_FTM_OUTEN_MASK  (0xFFFFFFFFUL & (~((3UL << M1_PWM_PAIR_PHA) |\
                                                 (3UL << M1_PWM_PAIR_PHB) |\
                                                 (3UL << M1_PWM_PAIR_PHC))))

/* The over-current FTM fault mask. */
#define MCDRV_FTM_FAULT_MASK  (1U << M1_PWM_FAULT_IDX)


/* The PWM driver control structure. */
typedef struct _mcdrv_pwm3ph_ftm
{
    frac16_t  *pf16MOD;      /* The FTM counter modulo. */
    frac16_t  *pf16CnVPhA0;  /* Phase A raising edge value register. */
    frac16_t  *pf16CnVPhA1;  /* Phase A falling edge value register. */
    frac16_t  *pf16CnVPhB0;  /* Phase B raising edge value register. */
    frac16_t  *pf16CnVPhB1;  /* Phase B falling edge value register. */
    frac16_t  *pf16CnVPhC0;  /* Phase C raising edge value register. */
    frac16_t  *pf16CnVPhC1;  /* Phase C falling edge value register. */
    uint32_t  *pui32PWMLOAD; /* The load control register. */
    uint32_t  *pui32OUTMASK; /* The output enable mask register. */
    uint16_t  *pui16FMS;     /* The fault control register. */
} mcdrv_pwm3ph_ftm_t;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* Initialize the PWM driver structure. */
static const mcdrv_pwm3ph_ftm_t c_sfsM1Pwm3ph =
{
    .pf16MOD      = (frac16_t*)(uint32_t)&M1_PWM_PERIPH->MOD,
    .pf16CnVPhA0  = (frac16_t*)(uint32_t)&M1_PWM_PERIPH->CONTROLS[M1_PWM_PAIR_PHA].CnV,
    .pf16CnVPhA1  = (frac16_t*)(uint32_t)&M1_PWM_PERIPH->CONTROLS[M1_PWM_PAIR_PHA + 1U].CnV,
    .pf16CnVPhB0  = (frac16_t*)(uint32_t)&M1_PWM_PERIPH->CONTROLS[M1_PWM_PAIR_PHB].CnV,
    .pf16CnVPhB1  = (frac16_t*)(uint32_t)&M1_PWM_PERIPH->CONTROLS[M1_PWM_PAIR_PHB + 1U].CnV,
    .pf16CnVPhC0  = (frac16_t*)(uint32_t)&M1_PWM_PERIPH->CONTROLS[M1_PWM_PAIR_PHC].CnV,
    .pf16CnVPhC1  = (frac16_t*)(uint32_t)&M1_PWM_PERIPH->CONTROLS[M1_PWM_PAIR_PHC + 1U].CnV,
    .pui32PWMLOAD = (uint32_t*)(uint32_t)&M1_PWM_PERIPH->PWMLOAD,
    .pui32OUTMASK = (uint32_t*)(uint32_t)&M1_PWM_PERIPH->OUTMASK,
    .pui16FMS     = (uint16_t*)(uint32_t)&M1_PWM_PERIPH->FMS
};


/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * Function updates FTM value registers based on required duty cycle values.
 */
void MCDRV_fsFtmPwm3PhSet(GMCLIB_3COOR_T_F16 sPhaseDuty)
{
    frac16_t f16DutyCycle;
    frac16_t f16Modulo;

    /* FTM counter modulo. */
    f16Modulo = *c_sfsM1Pwm3ph.pf16MOD;

    /* Calculate duty cycle for PWM phase A. */
    f16DutyCycle = MLIB_MulSat_F16(f16Modulo, sPhaseDuty.f16A);
    *c_sfsM1Pwm3ph.pf16CnVPhA1 = f16DutyCycle;
    *c_sfsM1Pwm3ph.pf16CnVPhA0 = MLIB_NegSat_F16(f16DutyCycle);

    /* Calculate duty cycle for PWM phase B. */
    f16DutyCycle = MLIB_MulSat_F16(f16Modulo, sPhaseDuty.f16B);
    *c_sfsM1Pwm3ph.pf16CnVPhB1 = f16DutyCycle;
    *c_sfsM1Pwm3ph.pf16CnVPhB0 = MLIB_NegSat_F16(f16DutyCycle);

    /* Calculate duty cycle for PWM phase C. */
    f16DutyCycle = MLIB_MulSat_F16(f16Modulo, sPhaseDuty.f16C);
    *c_sfsM1Pwm3ph.pf16CnVPhC1 = f16DutyCycle;
    *c_sfsM1Pwm3ph.pf16CnVPhC0 = MLIB_NegSat_F16(f16DutyCycle);

    /* Set LDOK bit in the FTM PWMLOAD register to load buffered register
       values. */
    *c_sfsM1Pwm3ph.pui32PWMLOAD = FTM_PWMLOAD_LDOK_MASK;

    /* The MC.SM PWM driver update node. */
    FS_FLOW_CHK_NODE(g_sfsFlowChkFL, FS_FLCK_FL_MCDRV_PWM_UPDATE);
}

/*!
 * Function enables selected PWM outputs.
 */
void MCDRV_fsFtmPwm3PhOutEn(void)
{
    /* Enable all selected FTM periphery outputs. */
    *c_sfsM1Pwm3ph.pui32OUTMASK = MCDRV_FTM_OUTEN_MASK;
}

/*!
 * Function disables all PWM outputs.
 */
void MCDRV_fsFtmPwm3PhOutDis(void)
{
    /* Disable all FTM periphery outputs. */
    *c_sfsM1Pwm3ph.pui32OUTMASK = FTM_OUTMASK_CH0OM_MASK |
                                  FTM_OUTMASK_CH1OM_MASK |
                                  FTM_OUTMASK_CH2OM_MASK |
                                  FTM_OUTMASK_CH3OM_MASK |
                                  FTM_OUTMASK_CH4OM_MASK |
                                  FTM_OUTMASK_CH5OM_MASK |
                                  FTM_OUTMASK_CH6OM_MASK |
                                  FTM_OUTMASK_CH7OM_MASK;
}

/*!
 * Function returns actual values of over-current flag.
 */
uint16_t MCDRV_fsFtmPwm3PhFltGet(void)
{
    uint16_t ui16FaultStatus;

    /* Read all FTM periphery faults. */
    ui16FaultStatus = (*c_sfsM1Pwm3ph.pui16FMS & MCDRV_FTM_FAULT_MASK);

    /* Clear fault flags in the FMS register by writing zero to it. */
    /*! @note This also clears the WPEN register. */
    *c_sfsM1Pwm3ph.pui16FMS = 0U;

    return ui16FaultStatus;
}

