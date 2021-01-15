/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _MCDRV_MIMXRT1010_EVK_H_
#define _MCDRV_MIMXRT1010_EVK_H_

#include "mcdrv_adc_imxrt.h"
#include "mcdrv_pwm3ph_pwma.h"
#include "fsl_adc_etc.h"
#include "fsl_pit.h"
#include "fsl_common.h"
#include "fsl_xbara.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Version info */
#define MCRSP_VER "2.0.0" /* motor control package version */

/* Application info */
typedef struct _app_ver
{
    char cBoardID[15];
    char cMotorType[4];
    char cAppVer[5];
} app_ver_t;

/* Structure used during clocks and modulo calculations */
typedef struct _clock_setup
{
    uint32_t ui32FastPeripheralClock;
    uint32_t ui32BusClock;
    uint32_t ui32SysPllClock;
    uint16_t ui16M1SpeedLoopFreq;
    uint16_t ui16M1SpeedLoopModulo;
    uint16_t ui16M1PwmFreq;
    uint16_t ui16M1PwmModulo;
    uint16_t ui16M1PwmDeadTime;
} clock_setup_t;

/******************************************************************************
 * Clock & PWM definition for motor 1
 ******************************************************************************/
#define M1_PWM_FREQ (10000)         /* PWM frequency - 10kHz */
#define M1_FOC_FREQ_VS_PWM_FREQ (1) /* FOC calculation is called every n-th PWM reload */
#define M1_SPEED_LOOP_FREQ (1000)   /* Speed loop frequency */
#define M1_PWM_DEADTIME (500)       /* Output PWM deadtime value in nanoseconds */

#define M1_FAST_LOOP_TS ((float_t)1.0 / (float_t)(M1_PWM_FREQ / M1_FOC_FREQ_VS_PWM_FREQ))
#define M1_SLOW_LOOP_TS ((float_t)1.0 / (float_t)(M1_SLOW_LOOP_FREQ))
#define M1_TIME_ONESEC_COUNT (M1_PWM_FREQ / M1_FOC_FREQ_VS_PWM_FREQ)

/* Assignment of eFlexPWM channels to motor 1 phases
 * 0 - PWM channels A0&B0 - sub-module 0
 * 1 - PWM channels A1&B1 - sub-module 1
 * 2 - WPM channels A2&B2 - sub-module 2
 */
#define M1_PWM_PAIR_PHA (0)
#define M1_PWM_PAIR_PHB (1)
#define M1_PWM_PAIR_PHC (2)

/* Over Current Fault detection */
#define M1_FAULT_NUM (0)

/* Braking resistor macros */
#define M1_BRAKE_SET()
#define M1_BRAKE_CLEAR()

/******************************************************************************
 * ADC measurement definition for motor 1
 ******************************************************************************/
/* Configuration table of ADC channels according to the input pin signals:
 * Valid for iMXRT1010 together with FRDM-MC-LVPMSM
 *
 * Motor 1
 * Quantity     | Module A (ADC1)
 * --------------------------------------------------------------------------
 * M1_I_A       | ADC1_CH1
 * M1_I_B       | ADC1_CH2
 * M1_I_C       | ADC1_CH13
 * M1_U_DCB     | ADC1_CH10
 *
 */

/* Phase current A assigned to ADC1 */
#define M1_ADC1_PH_A (1)
/* Phase current A assigned to ADC1 */
#define M1_ADC1_PH_B (2)
/* Phase current A assigned to ADC1 */
#define M1_ADC1_PH_C (7)
/* Phase current A assigned to ADC1 */
#define M1_ADC1_UDCB (9)

/* offset measurement filter window */
#define ADC_OFFSET_WINDOW (3)
/******************************************************************************
 * MC driver macro definition and check - do not change this part
 ******************************************************************************/
/******************************************************************************
 * Define common ADC control functions for motor 1
 ******************************************************************************/

#define M1_MCDRV_ADC_PERIPH_INIT() (InitADC())
#define M1_MCDRV_ADC_GET(par) \
    MCDRV_Curr3Ph2ShGet(par); \
    MCDRV_VoltDcBusGet(par);  \
    MCDRV_AuxValGet(par);
#define M1_MCDRV_CURR_3PH_CHAN_ASSIGN(par) (MCDRV_Curr3Ph2ShChanAssign(par))
#define M1_MCDRV_CURR_3PH_CALIB_INIT(par) (MCDRV_Curr3Ph2ShCalibInit(par))
#define M1_MCDRV_CURR_3PH_CALIB(par) (MCDRV_Curr3Ph2ShCalib(par))
#define M1_MCDRV_CURR_3PH_CALIB_SET(par) (MCDRV_Curr3Ph2ShCalibSet(par))

/******************************************************************************
 * Define motor 1 slow control loop timer
 ******************************************************************************/
#define M1_MCDRV_TMR_SLOWLOOP_INIT() InitPIT()

/******************************************************************************
 * Define 3-ph PWM control functions for motor 1
 ******************************************************************************/
#define M1_MCDRV_PWM_PERIPH_INIT() (M1_InitPWM())
#define M1_MCDRV_PWM3PH_SET(par) (MCDRV_eFlexPwm3PhSet(par))
#define M1_MCDRV_PWM3PH_EN(par) (MCDRV_eFlexPwm3PhOutEn(par))
#define M1_MCDRV_PWM3PH_DIS(par) (MCDRV_eFlexPwm3PhOutDis(par))
#define M1_MCDRV_PWM3PH_FLT_GET(par) (MCDRV_eFlexPwm3PhFltGet(par))

/******************************************************************************
 * Global variable definitions
 ******************************************************************************/
extern mcdrv_adc_t g_sM1AdcSensor;
extern mcdrv_pwm3ph_pwma_t g_sM1Pwm3ph;
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
void InitPIT(void);
void M1_InitPWM(void);
void InitXBARA(void);
void InitADC_ETC(void);
void M1_InitQD(void);

#ifdef __cplusplus
}
#endif

#endif /* _MCDRV_MIMXRT1010_EVK_H_  */
