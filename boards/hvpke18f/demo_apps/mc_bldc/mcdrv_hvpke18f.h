/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _MCDRV_HVP_KE18F_H_
#define _MCDRV_HVP_KE18F_H_

#include "mcdrv_pwm3ph_ftm.h"
#include "mcdrv_ftm_cmt.h"
#include "mcdrv_adc_adc16_hvp_ke18.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Version info */
#define MCRSP_VER       "2.0.0"        /* motor control package version */

/* Application info */
typedef struct 
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
    uint32_t ui32CmtTimerFreq;
    uint16_t ui16CtrlLoopFreq;
    uint16_t ui16CtrlLoopModulo;
} clock_setup_t;

/* Define motor 1 ADC periphery (ADC16) */
#define M1_MCDRV_ADC (MCDRV_ADC16)

/* Define motor 1 3-ph PWM periphery */
#define M1_MCDRV_PWM3PH (MCDRV_FTM0)

/* Define motor 1 asynchronous time event*/
#define M1_MCDRV_TMR_CMT (MCDRV_FTM1)

/* Define motor 1 slow control loop timer */
#define M1_MCDRV_TMR_SLOWLOOP (MCDRV_FTM2)

/* Define motor 1 ADC trigger PDB */
#define M1_MCDRV_PDB (MCDRV_PDB0)

/* Braking resistor macros */
#define M1_BRAKE_SET() GPIOE->PSOR |= GPIO_PSOR_PTSO(1 << 8)
#define M1_BRAKE_CLEAR() GPIOE->PCOR |= GPIO_PCOR_PTCO(1 << 8)

/* Over-current Fault detection */
#define M1_FAULT_NUM (0)

/******************************************************************************
 * Clock & PWM definition
 ******************************************************************************/
#define PWM_FREQ (20000)        /* PWM frequency - 20kHz */
#define CTRL_LOOP_FREQ (1000)   /* Control loop frequency */
#define M1_PWM_DEADTIME (1500)  /* Output PWM deadtime value in nanoseconds */

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

/* Configuration table of ADC channels according to the input pin signals:
 * Valid for Kinetis KE18 HVP board (HVP-KE18F) together with HVP-MC3PH
 *
 * Proper ADC channel assignment needs to follow these rules:
 *   - only one ADC module can be assigned to sense required variable
 *   - auxiliary quantity must be assigned to last free ADC module
 *
 *   Quantity          |   ADC0           |   ADC2
 *   --------------------------------------------------------------------------
 *   BEMF_A (PTB11)    |                  | ADC2_SE8 (0x08)
 *   BEMF_B (PTB8)     |                  | ADC2_SE11 (0x0B)
 *   BEMF_C (PTA6)     | ADC0_SE2  (0x02) | 
 *   U_dcb (PTE7)      |                  | ADC2_SE2 (0x02) 
 *   I_dcb (PTB10)     |                  | ADC2_SE9 (0x09)
 */
/* BEMF phase A assigned to ADC0 only */
#define ADC0_PH_A MCDRV_CHAN_OFF
#define ADC1_PH_A (1)
/* BEMF phase B assigned to ADC0 only */
#define ADC0_PH_B MCDRV_CHAN_OFF
#define ADC1_PH_B (0)
/* BEMF phase C assigned to ADC0 only */
#define ADC0_PH_C MCDRV_CHAN_OFF
#define ADC1_PH_C (13)
/* DC-bus voltage is assigned to ADC0 only */
#define ADC0_UDCB (5)
#define ADC1_UDCB MCDRV_CHAN_OFF
/* DC-bus current is assigned to ADC1 only */
#define ADC0_IDCB MCDRV_CHAN_OFF
#define ADC1_IDCB (2)
/* Aux channel is assigned to ADC1 only */
#define ADC0_AUX MCDRV_TEMP
#define ADC1_AUX MCDRV_CHAN_OFF

/******************************************************************************
 * MC driver macro definition and check - do not change this part
 ******************************************************************************/

/******************************************************************************
 * Define motor 1 ADC control functions
 ******************************************************************************/
#ifdef M1_MCDRV_ADC
#if (M1_MCDRV_ADC == MCDRV_ADC16)
#define M1_MCDRV_ADC_PERIPH_INIT() (InitADC16())
#define M1_MCDRV_ADC_GET(par)  \
    MCDRV_BemfVoltageGet(par); \
    MCDRV_VoltDcBusGet(par);   \
    MCDRV_CurrDcBusGet(par);   \
    MCDRV_AuxValGet(par);
#define M1_MCDRV_ADC_ASSIGN_BEMF(par) (MCDRV_AssignBemfChannel_hvp_ke18(par))
#define M1_MCDRV_CURR_CALIB_INIT(par) (MCDRV_CurrOffsetCalibInit(par))
#define M1_MCDRV_CURR_CALIB(par) (MCDRV_CurrOffsetCalib(par))
#define M1_MCDRV_CURR_CALIB_SET(par) (MCDRV_CurrOffsetCalibSet(par))
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
#define M1_MCDRV_PWM_PERIPH_INIT() (InitFTM0())
#define M1_MCDRV_PWM3PH_SET_PWM_OUTPUT(par1, par2) (MCDRV_FtmSetPwmOutput(par1, par2))
#define M1_MCDRV_PWM3PH_SET_DUTY(par1, par2) (MCDRV_FtmSetDutyCycle(par1, par2))
#define MC_MCDRV_PWM3PH_FLT_GET(par) (MCDRV_FtmPwm3PhFltGet(par))    
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
#define M1_MCDRV_TMR_CMT_PERIPH_INIT() (InitFTM1())
#define M1_MCDRV_TMR_CMT_SET(par1, par2) (MCDRV_FtmCmtSet(par1, par2))
#define M1_MCDRV_TMR_CMT_GET(par) (MCDRV_FtmCmtGet(par))
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
void InitRelay(void);
void InitClock(void);


#ifdef __cplusplus
}
#endif
#endif /* _MCDRV_HVP_KE18F_H_  */
