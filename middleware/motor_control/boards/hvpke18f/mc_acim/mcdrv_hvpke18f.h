/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _MCDRV_HVP_KE18F_H_
#define _MCDRV_HVP_KE18F_H_

#include "fsl_device_registers.h"
#include "mcdrv_pwm3ph_ftm.h"
#include "mcdrv_adc_adc16.h"

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
    uint32_t ui32SystemClock;
    uint32_t ui32BusClock;
    uint32_t ui32AsynClock;
    uint16_t ui16PwmFreq;
    uint16_t ui16PwmDeadTime;
    uint16_t ui16PwmModulo;
    uint16_t ui16FastLoopFreq;
    uint16_t ui16SlowLoopFreq;
    uint16_t ui16SpeedLoopModulo;
} clock_setup_t;

/* define motor 1 ADC periphery (ADC16) */
#define M1_MCDRV_ADC (MCDRV_ADC16)

/* define motor 1 3-ph PWM periphery */
#define M1_MCDRV_PWM3PH (MCDRV_FTM0)

/* define motor 1 slow control loop timer */
#define M1_MCDRV_TMR_SLOWLOOP (MCDRV_FTM2)

/* define motor 1 ADC trigger PDB */
#define M1_MCDRV_PDB (MCDRV_PDB0)

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

/* assignment of FTM channels to motor phases
 * 0 - FTM channels 0&1
 * 2 - FTM channels 2&3
 * 4 - FTM channels 4&5 
 * 6 - FTM channels 6&7 */
#define M1_PWM_PAIR_PHA (0)
#define M1_PWM_PAIR_PHB (2)
#define M1_PWM_PAIR_PHC (4)

/* Braking resistor macros */
#define M1_BRAKE_SET() GPIOE->PSOR |= GPIO_PSOR_PTSO(1 << 8)
#define M1_BRAKE_CLEAR() GPIOE->PCOR |= GPIO_PCOR_PTCO(1 << 8)

/* Over-current Fault detection */
#define M1_FAULT_NUM (0)

/******************************************************************************
 * ADC measurement definition
 ******************************************************************************/
/* Configuration table of ADC channels according to the input pin signals:
 * Valid for Kinetis KE18 HVP board (HVP-KE18F) together with HVP-MC3PH
 * 
 * Proper ADC channel assignment needs to follow these rules:
 *   - at least one phase current must be assigned to both ADC modules
 *   - two other phase current channels must be assigned to different ADC modules
 *   - Udcb and auxiliary channels must be assigned to differrent ADC modules

 *   Quantity         | ADC Module 0  | ADC Module 1
 *   --------------------------------------------------------------------------
 *   I_A (PTA0)       | ADCA_CH0      | --------
 *   I_B (PTB0)       | --------      | ADCB_CH14
 *   I_C (PTB1)       | ADCA_CH5      | ADCB_CH15
 *   U_DCB (PTC7)     | --------      | ADCB_CH5
 */
/* Phase current A assingned to ADC0 and ADC1 only */
#define M1_ADC0_PH_A (0)
#define M1_ADC1_PH_A (MCDRV_CHAN_OFF)
/* Phase current B assigned to ADC1 only */
#define M1_ADC0_PH_B (MCDRV_CHAN_OFF)
#define M1_ADC1_PH_B (14)
/* Phase current C assigned to ADC0 only */
#define M1_ADC0_PH_C (5)
#define M1_ADC1_PH_C (15)
/* Udc bus voltage is assigned to ADC0 only */
#define M1_ADC0_UDCB (MCDRV_CHAN_OFF)
#define M1_ADC1_UDCB (5)
/* Aux channel is assigned to ADC1 only */
#define M1_ADC0_AUX (6)
#define M1_ADC1_AUX (MCDRV_CHAN_OFF)

/* offset measurement filter window */     
#define ADC_OFFSET_WINDOW (3)
/******************************************************************************
 * MC driver macro definition and check - do not change this part
 ******************************************************************************/
/******************************************************************************
 * Define motor 1 ADC control functions
 ******************************************************************************/
#ifdef M1_MCDRV_ADC
#if (M1_MCDRV_ADC == MCDRV_ADC16)
#define M1_MCDRV_ADC_PERIPH_INIT() (InitADC16())
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
#if (M1_MCDRV_PWM3PH == MCDRV_FTM)
#define M1_MCDRV_PWM_PERIPH_INIT() (InitFTM0())
#define M1_MCDRV_PWM3PH_SET(par) (MCDRV_FtmPwm3PhSet(par))
#define M1_MCDRV_PWM3PH_EN(par) (MCDRV_FtmPwm3PhOutEn(par))
#define M1_MCDRV_PWM3PH_DIS(par) (MCDRV_FtmPwm3PhOutDis(par))
#define M1_MCDRV_PWM3PH_FLT_GET(par) (MCDRV_FtmPwm3PhFltGet(par))
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

/******************************************************************************
 * define motor 1 ADC trigger PDB
 ******************************************************************************/
#ifdef M1_MCDRV_PDB
#if (M1_MCDRV_PDB == MCDRV_PDB0)
#define M1_MCDRV_PDB_INIT() InitPDB()
#else
#error MCDRV: Unsupported PDB periphery!
#endif
#else
#error MCDRV: PDB periphery was not specified!
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
 * global variable definitions
 ******************************************************************************/
extern mcdrv_pwm3ph_ftm_t g_sM1Pwm3ph;
extern mcdrv_adc16_t g_sM1AdcSensor;
extern clock_setup_t g_sClockSetup;

/*******************************************************************************
 * API
 ******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

void InitClock(void);
void MCDRV_Init_M1(void);
void InitFTM0(void);
void InitFTM2(void);
void InitPDB(void);
void InitADC16(void);
void InitRelay(void);
float_t M1_GetIPMTemperature(frac16_t f16RawData);

#ifdef __cplusplus
}
#endif
#endif /* _MCDRV_HVP_KE18F_H_  */
