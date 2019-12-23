/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _MCDRV_HVP_KV46F150M_H_
#define _MCDRV_HVP_KV46F150M_H_

#include "fsl_device_registers.h"
#include "mcdrv_pwm3ph_pwma.h"
#include "mcdrv_adc_adc12.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Version info */
#define MCRSP_VER       "2.0.0"        /* motor control package version */

/* Application info */
typedef struct _app_ver
{
    char    cBoardID[15];
    char    cMotorType[4];
    char    cAppVer[5];
}app_ver_t;


/* Structure used during clocks and modulo calculations */
typedef struct _clock_setup
{   
    uint32_t ui32FastPeripheralClock;
    uint32_t ui32BusClock;
    uint16_t ui16M1SpeedLoopFreq;
    uint16_t ui16M1SpeedLoopModulo;
    uint16_t ui16M1PwmFreq;
    uint16_t ui16M1PwmModulo;
    uint16_t ui16M1PwmDeadTime;    
    
} clock_setup_t;

/******************************************************************************
 * Clock & PWM definition
 ******************************************************************************/
/* PWM frequency */
#define M1_PWM_FREQ (10000)
/* FOC calculation is called every n-th PWM reload */
#define M1_FOC_FREQ_VS_PWM_FREQ (1)
/* Speed loop frequency */
#define M1_SPEED_LOOP_FREQ (1000)
/* Output PWM deadtime value in nanoseconds */
#define M1_PWM_DEADTIME (1500)

#define M1_FAST_LOOP_TS ((float_t)1.0/(float_t)(M1_PWM_FREQ / M1_FOC_FREQ_VS_PWM_FREQ))
#define M1_SLOW_LOOP_TS ((float_t)1.0/(float_t)(M1_SLOW_LOOP_FREQ))
#define M1_TIME_ONESEC_COUNT (M1_PWM_FREQ / M1_FOC_FREQ_VS_PWM_FREQ)

/* Assignment of PWM Submodules to motor phases
 * 0 - PWM Submodule 0
 * 1 - PWM Submodule 1
 * 2 - PWM Submodule 2
 * 3 - PWM Submodule 3
 */
#define M1_PWM_PAIR_PHA (0)
#define M1_PWM_PAIR_PHB (1)
#define M1_PWM_PAIR_PHC (2)

/* Braking resistor macros */
#define M1_BRAKE_SET() GPIOC->PSOR |= GPIO_PSOR_PTSO(1 << 16)
#define M1_BRAKE_CLEAR() GPIOC->PCOR |= GPIO_PCOR_PTCO(1 << 16)

/* Over-current Fault detection */
#define M1_FAULT_NUM (0)

/******************************************************************************
 * ADC measurement definition
 ******************************************************************************/
/* Configuration table of ADC channels according to the input pin signals:
 * Valid for Kinetis KV46 HVP board (HVP-KV46F150M) together with HVP-MC3PH
 * 
 * Proper ADC channel assignment needs to follow these rules:
 *   - at least one phase current must be assigned to both ADC modules
 *   - two other phase current channels must be assigned to different ADC modules
 *   - Udcb and auxiliary channels must be assigned to differrent ADC modules

 Quantity     | ADC module 0                  | ADC module 1
--------------------------------------------------------------------------
 I_phA        | ADCA_CH2 (pin18)              | -
 I_phB        | -                             | ADCB_CH2 (PTB0) 
 I_phC        | ADCA_CH3 (pin19)              | ADCB_CH3 (PTB1)
 U_dcbus      | ADCA_CH1 (PTE17)              | -
 Aux(IPM tmp) | -                             | ADCB_CH1 (PTE19) 
*/

/* Phase current A assingned to ADC0 only */
#define M1_ADC0_PH_A (2)
#define M1_ADC1_PH_A (MCDRV_CHAN_OFF)
/* Phase current B assingned to ADC1 only */
#define M1_ADC0_PH_B (MCDRV_CHAN_OFF)
#define M1_ADC1_PH_B (2)
/* Phase current C assingned to ADC0 and ADC1 */
#define M1_ADC0_PH_C (3)
#define M1_ADC1_PH_C (3)
/* Udc bus voltage is assigned to ADC0 only */
#define M1_ADC0_UDCB (1)
#define M1_ADC1_UDCB (MCDRV_CHAN_OFF)
/* Aux channel is assigned to ADC1 only */
#define M1_ADC0_AUX (MCDRV_CHAN_OFF)
#define M1_ADC1_AUX (4)

/* offset measurement filter window */     
#define ADC_OFFSET_WINDOW (3)
/******************************************************************************
 * MC driver macro definition and check - do not change this part
 ******************************************************************************/
/******************************************************************************
 * Define motor 1 ADC control functions
 ******************************************************************************/
#define M1_MCDRV_ADC_PERIPH_INIT() (InitADC12())
#define M1_MCDRV_ADC_GET(par) \
    MCDRV_Curr3Ph2ShGet(par); \
    MCDRV_VoltDcBusGet(par);  \
    MCDRV_AuxValGet(par);
#define M1_MCDRV_CURR_3PH_CHAN_ASSIGN(par) (MCDRV_Curr3Ph2ShChanAssign(par))
#define M1_MCDRV_CURR_3PH_CALIB_INIT(par) (MCDRV_Curr3Ph2ShCalibInit(par))
#define M1_MCDRV_CURR_3PH_CALIB(par) (MCDRV_Curr3Ph2ShCalib(par))
#define M1_MCDRV_CURR_3PH_CALIB_SET(par) (MCDRV_Curr3Ph2ShCalibSet(par))

/******************************************************************************
 * Define motor 1 3-ph PWM control functions
 ******************************************************************************/
#define M1_MCDRV_PWM_PERIPH_INIT() (InitPWMA())
#define M1_MCDRV_PWM3PH_SET(par) (MCDRV_eFlexPwm3PhSet(par))
#define M1_MCDRV_PWM3PH_EN(par) (MCDRV_eFlexPwm3PhOutEn(par))
#define M1_MCDRV_PWM3PH_DIS(par) (MCDRV_eFlexPwm3PhOutDis(par))
#define M1_MCDRV_PWM3PH_FLT_GET(par) (MCDRV_eFlexPwm3PhFltGet(par))

/******************************************************************************
 * Define motor 1 slow control loop timer
 ******************************************************************************/
#define M1_MCDRV_TMR_SLOWLOOP_INIT() InitFTM1()

/******************************************************************************
 * global variable definitions
 ******************************************************************************/
extern mcdrv_pwm3ph_pwma_t g_sM1Pwm3ph;
extern mcdrv_adc12_t g_sM1AdcSensor;
extern clock_setup_t g_sClockSetup;

/*******************************************************************************
 * API
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

void InitClock(void);
void MCDRV_Init_M1(void);
void InitPWMA(void);
void InitFTM1(void);
void InitADC12(void);
void InitRelay(void);

#ifdef __cplusplus
}
#endif
#endif /* _MCDRV_HVP_KV46F150M_H_ */

