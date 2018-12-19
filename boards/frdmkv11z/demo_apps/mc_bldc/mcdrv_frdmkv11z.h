/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _MCDRV_TWR_KV11Z_H_
#define _MCDRV_TWR_KV11Z_H_

#include "mcdrv_pwm3ph_ftm.h"
#include "mcdrv_ftm_cmt.h"
#include "mcdrv_adc_adc16.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Structure used during clocks and modulo calculations */
typedef struct _clock_setup
{
    uint32_t ui32CoreSystemClock;
    uint32_t ui32BusFlashClock;
    uint16_t ui16PwmFreq;
    uint16_t ui16PwmModulo;
    uint16_t ui32CmtTimerFreq;
    uint16_t ui16CtrlLoopFreq;
} clock_setup_t;

/* Define motor 1 ADC periphery (ADC16) */
#define M1_MCDRV_ADC MCDRV_ADC16

/* Define motor 1 3-ph PWM periphery */
#define M1_MCDRV_PWM3PH MCDRV_FTM0

/* Define motor 1 asynchronous time event*/
#define M1_MCDRV_TMR_CMT MCDRV_FTM1

/* Define motor 1 slow control loop timer */
#define M1_MCDRV_TMR_SLOWLOOP MCDRV_FTM2

/* Define motor 1 ADC trigger PDB */
#define M1_MCDRV_PDB MCDRV_PDB0

/* ISR priority */
#define ISR_PRIORITY_PDB0 (1)       /* PDB interrupt priority */
#define ISR_PRIORITY_ADC0 (1)       /* sensorless control */
#define ISR_PRIORITY_SLOW_TIMER (3) /* speed control loop (low ISR priority) */
#define ISR_PRIORITY_FORCED_CMT (1) /* forced commutation (when missed sensorless cmt, open loop, timing) */

/******************************************************************************
 * Clock & PWM definition
 ******************************************************************************/
#define CPU_CLOCK (DEFAULT_SYSTEM_CLOCK) /* 75 MHz, CLOCK_SETUP = 1 */
#define BUS_CLOCK (DEFAULT_SYSTEM_CLOCK / 3)
#define CMT_TIMER_FREQ (DEFAULT_SYSTEM_CLOCK / 128) /* Frequency of commutation timer in Hz (FTM1) */
#define CTRL_LOOP_FREQ (1000)                       /* Frequency of control loop in Hz */
#define PWM_FREQ (20000)                            /* PWM frequency - 20kHz */

/* Assignment of FTM channels to motor phases
 * 0 - FTM channels 0&1
 * 2 - FTM channels 2&3
 * 4 - FTM channels 4&5
 */
#define M1_PWM_PAIR_PHA (0)
#define M1_PWM_PAIR_PHB (2)
#define M1_PWM_PAIR_PHC (4)

/* Array with swap and mask constants */
extern const char bldcCommutationTableComp[16];

/* FTM channel number for asynchronous time event */
#define M1_FTM_CMT_CHAN (0)

/******************************************************************************
 * ADC channels definitions
 ******************************************************************************/
/* Predefined values of ADC channels */
#define ADC_TEMPERATURE (26) /* Temperature sensor */
#define ADC_NO_CHAN (31)     /* Default unassigned channel */

/* Configuration table of ADC channels according to the input pin signals:
 * Valid for Kinetis KV11 Freedom board (FRDM-KV11Z) together with FRDM-MC-LVBLDC
 *
 * Proper ADC channel assignment needs to follow these rules:
 *   - only one ADC module can be assigned to sense required variable
 *   - auxiliary quantity must be assigned to last free ADC module
 *
 *   Quantity          |   ADC0           |   ADC1
 *   --------------------------------------------------------------------------
 *   BEMF_A (PTE18)    | ADC0_SE6  (0x06) | ADC1_SE1  (0x01)
 *   BEMF_B (PTB1)     | ADC0_SE9  (0x09) | ADC1_SE9  (0x09)
 *   BEMF_C (PTE19)    | ADC0_SE7  (0x07) | ADC1_SE7  (0x07)
 *   U_dcb  (PTE17)    | ADC0_SE5  (0x05) | ADC1_SE5  (0x05)
 *   I_dcb  (PTE30)    |                  | ADC1_SE4  (0x04)
 */
/* BEMF phase A assigned to ADC0 only */
#define M1_ADC0_PH_A (6)
#define M1_ADC1_PH_A ADC_NO_CHAN
/* BEMF phase B assigned to ADC0 only */
#define M1_ADC0_PH_B (9)
#define M1_ADC1_PH_B ADC_NO_CHAN
/* BEMF phase C assigned to ADC0 only */
#define M1_ADC0_PH_C (7)
#define M1_ADC1_PH_C ADC_NO_CHAN
/* DC-bus voltage is assigned to ADC0 only */
#define M1_ADC0_UDCB (5)
#define M1_ADC1_UDCB ADC_NO_CHAN
/* DC-bus current is assigned to ADC1 only */
#define M1_ADC0_IDCB ADC_NO_CHAN
#define M1_ADC1_IDCB (4)
/* Aux channel is assigned to ADC1 only */
#define M1_ADC0_AUX ADC_NO_CHAN
#define M1_ADC1_AUX ADC_TEMPERATURE

/******************************************************************************
 * MC driver macro definition and check - do not change this part
 ******************************************************************************/

/******************************************************************************
 * Define motor 1 ADC control functions
 ******************************************************************************/
#ifdef M1_MCDRV_ADC
#if (M1_MCDRV_ADC == MCDRV_ADC16)
#define M1_MCDRV_ADC_PERIPH_INIT() InitADC16()
#define M1_MCDRV_ADC_GET(par)  \
    MCDRV_BemfVoltageGet(par); \
    MCDRV_VoltDcBusGet(par);   \
    MCDRV_CurrDcBusGet(par);   \
    MCDRV_AuxValGet(par);
#define M1_MCDRV_ADC_ASSIGN_BEMF(par) MCDRV_AssignBemfChannel(par)
#define M1_MCDRV_CURR_CALIB_INIT(par) MCDRV_CurrOffsetCalibInit(par)
#define M1_MCDRV_CURR_CALIB(par) MCDRV_CurrOffsetCalib(par)
#define M1_MCDRV_CURR_CALIB_SET(par) MCDRV_CurrOffsetCalibSet(par)
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
#if (M1_MCDRV_PWM3PH == MCDRV_FTM0)
#define M1_MCDRV_PWM_PERIPH_INIT() InitFTM0()
#define M1_MCDRV_PWM3PH_SET_PWM_OUTPUT(par1, par2) MCDRV_FtmSetPwmOutput(par1, par2)
#define M1_MCDRV_PWM3PH_SET_DUTY(par1, par2) MCDRV_FtmSetDutyCycle(par1, par2)
#else
#error MCDRV: Unsupported PWM periphery!
#endif
#else
#error MCDRV: PWM periphery was not specified!
#endif

/******************************************************************************
 * Define motor 1 asynchronous time event functions
 ******************************************************************************/
#ifdef M1_MCDRV_TMR_CMT
#if (M1_MCDRV_TMR_CMT == MCDRV_FTM1)
#define M1_MCDRV_TMR_CMT_PERIPH_INIT() InitFTM1()
#define M1_MCDRV_TMR_CMT_SET(par1, par2) MCDRV_FtmCmtSet(par1, par2)
#define M1_MCDRV_TMR_CMT_GET(par) MCDRV_FtmCmtGet(par)
#else
#error MCDRV: Unsupported Timer periphery!
#endif
#else
#error MCDRV: Timer periphery was not specified!
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
 * Define motor 1 ADC trigger PDB
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

/******************************************************************************
 * Global variable definitions
 ******************************************************************************/
extern mcdrv_pwm3ph_ftm_t g_sM1Pwm3ph;
extern mcdrv_ftm_cmt_t g_sM1CmtTmr;
extern mcdrv_adc16_t g_sM1AdcSensor;
extern clock_setup_t g_sClockSetup;

/*******************************************************************************
 * API
 ******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

void MCDRV_Init_M1(void);
void InitFTM0(void);
void InitFTM2(void);
void InitFTM1(void);
void InitPDB(void);
void InitADC16(void);

void InitClock(void);

#ifdef __cplusplus
}
#endif
#endif /* _MCDRV_FRDM_KV11Z_H_  */
