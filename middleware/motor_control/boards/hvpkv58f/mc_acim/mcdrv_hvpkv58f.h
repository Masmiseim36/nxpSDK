/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _MCDRV_HVP_KV58F220M_H_
#define _MCDRV_HVP_KV58F220M_H_

#include "fsl_device_registers.h"
#include "mcdrv_pwm3ph_pwma.h"
#include "mcdrv_adc_hsadc.h"

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
    uint16_t ui16PwmFreq;
    uint16_t ui16PwmDeadTime;
    uint16_t ui16PwmModulo;
    uint16_t ui16FastLoopFreq;
    uint16_t ui16SlowLoopFreq;
    uint16_t ui16SpeedLoopModulo;
} clock_setup_t;

/* Define motor 1 ADC periphery (HSADC) */
#define M1_MCDRV_ADC (MCDRV_HSADC)

/* Define motor 1 3-ph PWM periphery */
#define M1_MCDRV_PWM3PH (MCDRV_PWMA)

/* Define motor 1 slow control loop timer */
#define M1_MCDRV_TMR_SLOWLOOP (MCDRV_FTM2)

/******************************************************************************
 * Clock & PWM definition
 ******************************************************************************/
/* PWM frequency */
#define M1_PWM_FREQ (10000)
/* FOC calculation is called every n-th PWM reload */
#define M1_FOC_FREQ_VS_PWM_FREQ (1)
/* Speed loop frequency */
#define M1_SLOW_LOOP_FREQ (1000)
/* Output PWM deadtime value in nanoseconds */
#define M1_PWM_DEADTIME (1500)

#define M1_FAST_LOOP_TS ((float_t)1.0/(float_t)(M1_PWM_FREQ / M1_FOC_FREQ_VS_PWM_FREQ))
#define M1_SLOW_LOOP_TS ((float_t)1.0/(float_t)(M1_SLOW_LOOP_FREQ))

/* Assignment of PWM channels to motor phases 
 * 0 - PWM channels 0&1
 * 1 - PWM channels 2&3
 * 2 - PWM channels 4&5 
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
 * Valid for Kinetis KV58 HVP board (HVP-KV58F220M) together with HVP-MC3PH
 * 
 * Proper ADC channel assignment needs to follow these rules:
 *   - at least one phase current must be assigned to both ADC modules
 *   - two other phase current channels must be assigned to different ADC modules
 *   - Udcb and auxiliary channels must be assigned to differrent ADC modules
 *
 * Quantity     | HSADC Module A (ADC0) | HSADC Module C (ADC1)
 * --------------------------------------------------------------------------
 * I_A          | HSADCA_CH2            | HSADCC_CH2
 * I_B          | ----------            | HSADCC_CH3
 * I_C          | HSADCA_CH6C           | ----------
 * U_DCB        | HSADCA_CH1            | ----------
 */

/* phase current A assigned to HSADCA and HSADCC */
#define M1_ADC0_PH_A (2)
#define M1_ADC1_PH_A (2)
/* phase current B assigned to HSADCC only */
#define M1_ADC0_PH_B MCDRV_CHAN_OFF
#define M1_ADC1_PH_B (3)
/* phase current C assigned to HSADCA only */
#define M1_ADC0_PH_C (6)
#define M1_ADC1_PH_C MCDRV_CHAN_OFF
/* DC-bus voltage is assigned to HSADCA only */
#define M1_ADC0_UDCB (1)
#define M1_ADC1_UDCB (MCDRV_CHAN_OFF)
/* aux channel is assigned to HSADCB only */
#define M1_ADC0_AUX                     (MCDRV_CHAN_OFF)
#define M1_ADC1_AUX                     (MCDRV_CHAN_OFF)
#define M1_ADC2_AUX                     (1)

/* offset measurement filter window */     
#define ADC_OFFSET_WINDOW (3)
/******************************************************************************
 * MC driver macro definition and check - do not change this part
 ******************************************************************************/
/******************************************************************************
 * Define motor 1 ADC control functions
 ******************************************************************************/
#ifdef M1_MCDRV_ADC
#if (M1_MCDRV_ADC == MCDRV_HSADC)
#define M1_MCDRV_ADC_PERIPH_INIT() (InitHSADC())
#define M1_MCDRV_ADC_GET(par) \
    MCDRV_Curr3Ph2ShGet(par); \
    MCDRV_VoltDcBusGet(par);  \
    MCDRV_AuxValGet(par);
#define M1_MCDRV_CURR_3PH_CHAN_ASSIGN(par) (MCDRV_Curr3Ph2ShChanAssign(par))
#define M1_MCDRV_CURR_3PH_CALIB_INIT(par) (MCDRV_Curr3Ph2ShCalibInit(par))
#define M1_MCDRV_CURR_3PH_CALIB(par) (MCDRV_Curr3Ph2ShCalib(par))
#define M1_MCDRV_CURR_3PH_CALIB_SET(par) (MCDRV_Curr3Ph2ShCalibSet(par))
#else
#error MCDRV: Unsupported ADC periphery!
#endif
#else
#error MCDRV: ADC periphery was not specified!
#endif

/******************************************************************************
 * Define motor 1 3-ph PWM control functions
 ******************************************************************************/
#ifdef M1_MCDRV_PWM3PH
#if (M1_MCDRV_PWM3PH == MCDRV_PWMA)
#define M1_MCDRV_PWM_PERIPH_INIT() (InitPWMA0())
#define M1_MCDRV_PWM3PH_SET(par) (MCDRV_eFlexPwm3PhSet(par))
#define M1_MCDRV_PWM3PH_EN(par) (MCDRV_eFlexPwm3PhOutEn(par))
#define M1_MCDRV_PWM3PH_DIS(par) (MCDRV_eFlexPwm3PhOutDis(par))
#define M1_MCDRV_PWM3PH_FLT_GET(par) (MCDRV_eFlexPwm3PhFltGet(par))
#else
#error MCDRV: Unsupported PWM periphery!
#endif
#else
#error MCDRV: PWM periphery was not specified!
#endif

/******************************************************************************
 * Define motor 1 slow control loop timer
 ******************************************************************************/
#ifdef M1_MCDRV_TMR_SLOWLOOP
#if (M1_MCDRV_TMR_SLOWLOOP == MCDRV_FTM2)
#define M1_MCDRV_TMR_SLOWLOOP_INIT() InitFTM2()
#else
#error MCDRV: Unsupported Timer periphery!
#endif
#else
#error MCDRV: Timer periphery was not specified!
#endif

/* Check whether ADC channel assignment is correct */
#if !(((M1_ADC0_PH_A != MCDRV_CHAN_OFF) && (M1_ADC1_PH_A != MCDRV_CHAN_OFF)) || \
      ((M1_ADC0_PH_B != MCDRV_CHAN_OFF) && (M1_ADC1_PH_B != MCDRV_CHAN_OFF)) || \
      ((M1_ADC0_PH_C != MCDRV_CHAN_OFF) && (M1_ADC1_PH_C != MCDRV_CHAN_OFF)))
#error MCDRV: None of phase current is assigned to both ADCs!
#endif

#if (M1_ADC0_UDCB != MCDRV_CHAN_OFF) && (M1_ADC0_AUX != MCDRV_CHAN_OFF) || \
    (M1_ADC1_UDCB != MCDRV_CHAN_OFF) && (M1_ADC1_AUX != MCDRV_CHAN_OFF)
#error MCDRV: The DC-bus voltage and Aux cannot be measured at the same ADC!
#endif

/* FNB41560 internal thermistor fourth-order approximation coeffs */
#define IPM_TEMP_APROX_P1            (-2.6028e-016F)
#define IPM_TEMP_APROX_P2            (2.4469e-011F)
#define IPM_TEMP_APROX_P3            (-7.4962e-007F)
#define IPM_TEMP_APROX_P4            (0.012514F)
#define IPM_TEMP_APROX_P5            (-19.265F)
/******************************************************************************
 * Global variable definitions
 ******************************************************************************/
extern mcdrv_pwm3ph_pwma_t g_sM1Pwm3ph;
extern mcdrv_hsadc_t g_sM1AdcSensor;
extern clock_setup_t g_sClockSetup;

/*******************************************************************************
 * API
 ******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

void InitClock(void);
void MCDRV_Init_M1(void);
void InitPWMA0(void);
void InitFTM2(void);
void InitHSADC(void);
void InitRelay(void);
float_t M1_GetIPMTemperature(frac16_t f16RawData);


#ifdef __cplusplus
}
#endif
#endif /* _MCDRV_HVP_KV58F_H_ */

