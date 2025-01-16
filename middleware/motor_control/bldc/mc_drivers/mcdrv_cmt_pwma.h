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
#ifndef _MCDRV_FTM_CMT_H_
#define _MCDRV_FTM_CMT_H_

#include "gmclib_types.h"
#include "fsl_device_registers.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* init sensors/actuators pointers */
#define M1_SET_PTR_CNT_ACT(par1) (g_sM1CmtTmr.pui16PwmCntAct = &(par1))
#define M1_SET_PTR_VALUE_ACT(par1) (g_sM1CmtTmr.pui16PwmValueAct = &(par1))

typedef struct _mcdrv_cmt_pwma
{
    PWM_Type *pui32PwmBase;     /* pointer FlexTimer base address */
    uint16_t *pui16PwmCntAct;   /* pointer to actual value of PWM counter */
    uint16_t *pui16PwmValueAct; /* pointer to actual value of PWM value register */
    uint8_t ui8PwmSubmoduleNumber; /* Number of PWM submodule used for CMT timer 8 */
      
} mcdrv_cmt_pwma_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief Function read actual values of PWMA counter and value register
 *
 * @param this   Pointer to the current object
 *
 * @return none
 */
RAM_FUNC_LIB
void MCDRV_CmtGet(mcdrv_cmt_pwma_t *this);

/*!
 * @brief Function update PWMA value register
 *
 * @param this   Pointer to the current object
 *
 * @return none
 */
RAM_FUNC_LIB
void MCDRV_CmtSet(mcdrv_cmt_pwma_t *this, uint16_t ui16TimeNew);

#ifdef __cplusplus
}
#endif

#endif /* _MCDRV_FTM_CMT_H_ */
