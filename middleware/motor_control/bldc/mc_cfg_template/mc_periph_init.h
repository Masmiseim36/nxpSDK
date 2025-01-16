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

#ifndef _MC_PERIPH_INIT_H_
#define _MC_PERIPH_INIT_H_

#include "gdflib.h"



/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* init sensors/actuators pointers */
#define M1_SET_PTR_CNT_ACT(par) 
#define M1_SET_PTR_VALUE_ACT(par) 

/* init sensors/actuators pointers */
#define M1_SET_PTR_U_DC_BUS(par) 
#define M1_SET_PTR_I_DC_BUS(par) 
#define M1_SET_PTR_BEMF_VOLT(par) 
#define M1_SET_PTR_AUX_CHAN(par) 

/* Structure used during clocks and modulo calculations */
typedef struct _clock_setup
{
    uint16_t ui16M1SpeedLoopFreq;
    uint16_t ui16M1PwmFreq;
    uint32_t ui32CmtTimerFreq;
} clock_setup_t;

typedef struct _mcdrv_cmt_pwma
{
    uint16_t *pui16PwmCntAct;   /* pointer to actual value of PWM counter */
    uint16_t *pui16PwmValueAct; /* pointer to actual value of PWM value register */
} mcdrv_cmt_pwma_t;

typedef struct _mcdrv_adcetc
{
    uint16_t ui16Sector;        /* commutation sector */
} mcdrv_adcetc_t;
/******************************************************************************
 * Clock & PWM definition for motor 1
 ******************************************************************************/
/* PWM frequency - 20kHz */
#define M1_PWM_FREQ (20000U)
/* FOC calculation is called every n-th PWM reload */
#define M1_FOC_FREQ_VS_PWM_FREQ (1U)
/* Speed loop frequency */
#define M1_SPEED_LOOP_FREQ (1000U)   

/******************************************************************************
 * MC driver macro definition and check - do not change this part
 ******************************************************************************/
/******************************************************************************
 * Define common ADC control functions for motor 1
 ******************************************************************************/
#define M1_MCDRV_ADC_PERIPH_INIT()
#define M1_MCDRV_ADC_GET(par)
        
#define M1_MCDRV_ADC_ASSIGN_BEMF(par)      
#define M1_MCDRV_CURR_CALIB_INIT(par)
#define M1_MCDRV_CURR_CALIB(par)
#define M1_MCDRV_CURR_CALIB_SET(par)

/******************************************************************************
 * Define motor 1 slow control loop timer
 ******************************************************************************/
#define M1_MCDRV_TMR_SLOWLOOP_INIT()

/******************************************************************************
 * Define 3-ph PWM control functions for motor 1
 ******************************************************************************/
#define M1_MCDRV_PWM_PERIPH_INIT()
#define M1_MCDRV_PWM3PH_SET_PWM_OUTPUT(par1, par2)
#define M1_MCDRV_PWM3PH_SET_DUTY(par1, par2)
#define M1_MCDRV_PWM3PH_FLT_GET(par) (0U)

/******************************************************************************
 * Define motor 1 asynchronous time event functions
 ******************************************************************************/
#define M1_MCDRV_TMR_CMT_PERIPH_INIT()
#define M1_MCDRV_TMR_CMT_SET(par1, par2)
#define M1_MCDRV_TMR_CMT_GET(par)

/******************************************************************************
 * Define motor 1 CMP2 for overcurrent detection
 ******************************************************************************/
#define M1_MCDRV_CMP_INIT()

/******************************************************************************
 * Global variable definitions
 ******************************************************************************/
extern mcdrv_adcetc_t g_sM1AdcSensor;
extern clock_setup_t g_sClockSetup;

/*******************************************************************************
 * API
 ******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#endif /* _MC_PERIPH_INIT_H_  */
