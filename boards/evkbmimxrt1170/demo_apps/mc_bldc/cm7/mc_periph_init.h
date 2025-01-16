/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _MC_PERIPH_INIT_H_
#define _MC_PERIPH_INIT_H_

#include "mcdrv_adcetc_imxrt11xx.h"
#include "mcdrv_pwm3ph_pwma.h"
#include "mcdrv_cmt_pwma.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Structure used during clocks and modulo calculations */
typedef struct _clock_setup
{
    uint32_t ui32FastPeripheralClock;
    uint16_t ui16M1SpeedLoopFreq;
    uint16_t ui16M1PwmFreq;
    uint16_t ui16M1PwmModulo;
    uint16_t ui16M1PwmDeadTime;
    uint32_t ui32CmtTimerFreq;
} clock_setup_t;

/* macro used for TSA table */
#define BLDC_SNSLESS_ENC

/******************************************************************************
 * Clock & PWM definition for motor 1
 ******************************************************************************/
#define M1_PWM_FREQ (20000U)         /* PWM frequency - 20kHz */
#define M1_FOC_FREQ_VS_PWM_FREQ (1U) /* FOC calculation is called every n-th PWM reload */
#define M1_SPEED_LOOP_FREQ (1000U)   /* Speed loop frequency */
#define M1_PWM_DEADTIME (500U)       /* Output PWM deadtime value in nanoseconds */

/* Over Current Fault detection */
#define M1_FAULT_NUM (0)

/* Array with swap and mask constants */
extern const uint16_t bldcCommutationTableComp[16];

/******************************************************************************
 * ADC measurement definition for motor 1
 ******************************************************************************/
/* Configuration table of ADC channels according to the input pin signals:
 * Valid for iMXRT1170 together with FRDM-MC-LVPMSM
 *
 * Motor 1
 * Quantity     | Module A (ADC1)       | Module B (ADC2)
 * --------------------------------------------------------------------------
 * M1_U_DCB -   GPIO_AD_08 - ADC1 CH1-A
 * M1_BEMF_A -  GPIO_AD_11 - ADC1 CH2-B
 * M1_BEMF_B -  GPIO_AD_12 - ADC1 CH3-A; ADC2 CH3-A
 * M1_BEMF_C -  GPIO_AD_13 - ADC1 CH3-B; ADC2 CH3-B
 * M1_I_DCB -   GPIO_AD_14 - ADC1 CH4-A; ADC2 CH4-A
 */ 
   
/* Phase BEMF A assigned to ADC1 and ADC2 */   
#define M1_ADC1_PH_A_CHNL (2U)
#define M1_ADC1_PH_A_SIDE (kLPADC_SampleChannelSingleEndSideB)

/* Phase BEMF B assigned to ADC1 and ADC2 */
#define M1_ADC1_PH_B_CHNL (3U)
#define M1_ADC1_PH_B_SIDE (kLPADC_SampleChannelSingleEndSideA)
#define M1_ADC2_PH_B_CHNL (3U)
#define M1_ADC2_PH_B_SIDE (kLPADC_SampleChannelSingleEndSideA)

/* Phase BEMF C assigned to ADC1 and ADC2 */
#define M1_ADC1_PH_C_CHNL (3U)
#define M1_ADC1_PH_C_SIDE (kLPADC_SampleChannelSingleEndSideB)
#define M1_ADC2_PH_C_CHNL (3U)
#define M1_ADC2_PH_C_SIDE (kLPADC_SampleChannelSingleEndSideB)

/* Phase voltage UDCB assigned to ADC1 and ADC2 */
#define M1_ADC1_UDCB_CHNL (1U)
#define M1_ADC1_UDCB_SIDE (kLPADC_SampleChannelSingleEndSideA)

/* Phase current IDCB assigned to ADC1 and ADC2 */
#define M1_ADC1_IDCB_CHNL (4U)
#define M1_ADC1_IDCB_SIDE (kLPADC_SampleChannelSingleEndSideA)
#define M1_ADC2_IDCB_CHNL (4U)
#define M1_ADC2_IDCB_SIDE (kLPADC_SampleChannelSingleEndSideA)   
   
/******************************************************************************
 * MC driver macro definition and check - do not change this part
 ******************************************************************************/
/******************************************************************************
 * Define common ADC control functions for motor 1
 ******************************************************************************/
#define M1_MCDRV_ADC_PERIPH_INIT() (InitADC())
#define M1_MCDRV_ADC_GET(par) \
        MCDRV_BemfVoltageGet(par); \
	MCDRV_VoltDcBusGet(par); \
	MCDRV_CurrDcBusGet(par);  \
	MCDRV_AuxValGet(par);
        
#define M1_MCDRV_ADC_ASSIGN_BEMF(par) MCDRV_AssignBemfChannel(par)        
#define M1_MCDRV_CURR_CALIB_INIT(par) MCDRV_CurrOffsetCalibInit(par)
#define M1_MCDRV_CURR_CALIB(par) MCDRV_CurrOffsetCalib(par)
#define M1_MCDRV_CURR_CALIB_SET(par) MCDRV_CurrOffsetCalibSet(par)

/******************************************************************************
 * Define motor 1 slow control loop timer
 ******************************************************************************/
#define M1_MCDRV_TMR_SLOWLOOP_INIT() InitTMR1()

/******************************************************************************
 * Define 3-ph PWM control functions for motor 1
 ******************************************************************************/
#define M1_MCDRV_PWM_PERIPH_INIT() (M1_InitPWM())
#define M1_MCDRV_PWM3PH_SET_PWM_OUTPUT(par1, par2) MCDRV_eFlexSetPwmOutput(par1, par2)
#define M1_MCDRV_PWM3PH_SET_DUTY(par1, par2) MCDRV_eFlexSetDutyCycle(par1, par2)
#define M1_MCDRV_PWM3PH_FLT_GET(par) (MCDRV_eFlexPwm3PhFltGet(par))

/******************************************************************************
 * Define motor 1 asynchronous time event functions
 ******************************************************************************/
#define M1_MCDRV_TMR_CMT_PERIPH_INIT() InitCMT()
#define M1_MCDRV_TMR_CMT_SET(par1, par2) MCDRV_CmtSet(par1, par2)
#define M1_MCDRV_TMR_CMT_GET(par) MCDRV_CmtGet(par)

/******************************************************************************
 * Define motor 1 CMP2 for overcurrent detection
 ******************************************************************************/
#define M1_MCDRV_CMP_INIT() InitCMP()

/******************************************************************************
 * Global variable definitions
 ******************************************************************************/
extern mcdrv_adcetc_t g_sM1AdcSensor;
extern mcdrv_pwm3ph_pwma_t g_sM1Pwm3ph;
extern mcdrv_cmt_pwma_t g_sM1CmtTmr;
extern clock_setup_t g_sClockSetup;

/*******************************************************************************
 * API
 ******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

void MCDRV_Init_M1(void);
void InitClock(void);
void InitADC(void);
void InitTMR1(void);
void M1_InitPWM(void);
void InitXBARA(void);
void InitADC_ETC(void);
void InitCMT(void);
void InitCMP(void);


#ifdef __cplusplus
}
#endif

#endif /* _MC_PERIPH_INIT_H_  */
