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

#ifndef _MCDRV_PWM3PH_PWMA_H_
#define _MCDRV_PWM3PH_PWMA_H_

#include "mlib.h"
#include "fsl_device_registers.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

typedef struct _mcdrv_pwm3ph_pwma
{
    PWM_Type *pui32PwmBaseAddress;       /* pointer to phase A top value */
    uint16_t ui16PhASubNum;              /* number of channel for phase A */
    uint16_t ui16PhBSubNum;              /* number of channel for phase A top */
    uint16_t ui16PhCSubNum;              /* number of channel for phase B bottom */
    uint16_t ui16PwmModulo;              /* PWMA MODULO Value */
    uint16_t ui16FaultFixNum;            /* PWMA fault number for fixed over-current fault detection */
    uint16_t ui16FaultAdjNum;            /* PWMA fault number for adjustable over-current fault detection */
    const uint16_t *pcBldcTable;         /* pointer to BLDC commutation Table */
} mcdrv_pwm3ph_pwma_t;


/*******************************************************************************
 * API
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief Function set duty cycle from input parameter
 *
 * @param this Pointer to the current object
 * @param f16InpDuty New input duty cycle to the pwma module
 *
 * @return none
 */
RAM_FUNC_LIB
void MCDRV_eFlexSetDutyCycle(mcdrv_pwm3ph_pwma_t *this, frac16_t f16InpDuty);

/*!
 * @brief Function set pwm sector from input
 *
 * @param this Pointer to the current object
 * @param sector Actual commutation sector
 *
 * @return none
 */
RAM_FUNC_LIB
void MCDRV_eFlexSetPwmOutput(mcdrv_pwm3ph_pwma_t *this, int16_t i16Sector);

/*!
 * @brief Function return actual value of over current flag
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
RAM_FUNC_LIB
bool_t MCDRV_eFlexPwm3PhFltGet(mcdrv_pwm3ph_pwma_t *this);

#ifdef __cplusplus
}
#endif

#endif /* _MCDRV_PWM3PH_PWMA_H_ */
