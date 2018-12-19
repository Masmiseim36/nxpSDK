/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _MCDRV_TWR_KV58F220_H_
#define _MCDRV_TWR_KV58F220_H_

#include "fsl_device_registers.h"
#include "m1_sm_ref_sol.h"
#include "mcdrv_ftm_cmt.h"
#include "mcdrv_pwm3ph_pwma.h"
#include "mcdrv_mosfet_predrv_spi.h"
#include "mcdrv_adc_hsadc.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Structure used during clocks and modulo calculations */
typedef struct _clock_setup
{
    uint32_t ui32CoreSystemClock;
    uint32_t ui32FastBusClock;
    uint32_t ui32BusFlashClock;
    uint16_t ui16PwmFreq;
    uint16_t ui16PwmModulo;
    uint32_t ui32CmtTimerFreq;
    uint16_t ui16CtrlLoopFreq;
} clock_setup_t;

/* Define motor 1 ADC periphery (HSADC) */
#define M1_MCDRV_ADC MCDRV_HSADC

/* Define motor 1 3-ph PWM periphery */
#define M1_MCDRV_PWM3PH MCDRV_PWMA

/* Define motor 1 asynchronous time event*/
#define M1_MCDRV_TMR_CMT MCDRV_FTM1

/* Define motor 1 slow control loop timer */
#define M1_MCDRV_TMR_SLOWLOOP MCDRV_FTM2

/* Define motor 1 ADC trigger PDB */
#define M1_MCDRV_PDB MCDRV_PDB0

/* Define motor 1 driver */
#define M1_MCDRV_SPI_DRV3PH MCDRV_SPI_DRV

/******************************************************************************
 * Clock & PWM definition
 ******************************************************************************/
/* 120 MHz, CLOCK_SETUP = 1 */
#define CPU_CLOCK (DEFAULT_SYSTEM_CLOCK)
/* Frequency of control loop in Hz */
#define CTRL_LOOP_FREQ (1000)
/* PWM frequency - 20kHz */
#define PWM_FREQ (20000)

/* Assignment of FTM channels to motor phases
 * 0 - FTM channels 0&1
 * 2 - FTM channels 2&3
 * 4 - FTM channels 4&5
 */
#define M1_PWM_PAIR_PHA (0)
#define M1_PWM_PAIR_PHB (2)
#define M1_PWM_PAIR_PHC (4)

/* Array with swap and mask constants */
extern const uint16_t bldcCommutationTableComp[16];

/* FTM channel number for asynchronous time event */
#define M1_FTM_CMT_CHAN (0)

/* Predefined values of ADC channels */
#define ADC_TEMPERATURE (26) /* Temperature sensor */
#define ADC_NO_CHAN (31)     /* Default value for unassigned channel */

/* Configuration table of ADC channels according to the input pin signals:
 * Valid for Kinetis KV58 Tower board (TWR-KV58F) together with TWR-MC-LV3PH
 *
 * Proper ADC channel assignment needs to follow these rules:
 *   - only one ADC module can be assigned to sense required variable
 *   - auxiliary quantity must be assigned to last free ADC module
 *
 *   Quantity          |     HSADC0       |   HSADC1
 *   --------------------------------------------------------------------------
 *   BEMF_A            | ADCA_CH2  (0x02) | ADCC_CH2  (0x02)
 *   BEMF_B            | ADCA_CH3  (0x03) | ADCC_CH3  (0x03)
 *   BEMF_C            | ADCA_CH6C (0x06) | ADCD_CH2  (0x02)
 *   U_dcb             | ADCA_CH6A (0x06) |
 *   I_dcb             | ADCA_CH7A (0x07) |
 */
/* BEMF phase A assigned to ADC0 only */
#define HSADCA_PH_A (2)
#define HSADCC_PH_A ADC_NO_CHAN
/* BEMF phase B assigned to ADC0 only */
#define HSADCA_PH_B (3)
#define HSADCC_PH_B ADC_NO_CHAN
/* BEMF phase C assigned to ADC0 only */
#define HSADCA_PH_C ADC_NO_CHAN
#define HSADCC_PH_C (2)
/* DC-bus voltage is assigned to ADC0 only */
#define HSADCA_UDCB (6)
#define HSADCC_UDCB ADC_NO_CHAN
/* DC-bus current is assigned to ADC1 only */
#define HSADCA_IDCB (7)
#define HSADCC_IDCB ADC_NO_CHAN
/* Aux channel is assigned to ADC1 only */
#define HSADCA_AUX (0)
#define HSADCC_AUX ADC_NO_CHAN

/******************************************************************************
 * MC driver macro definition and check - do not change this part
 ******************************************************************************/
/******************************************************************************
 * Define motor 1 ADC control functions
 ******************************************************************************/
#ifdef M1_MCDRV_ADC
#if (M1_MCDRV_ADC == MCDRV_HSADC)
#define M1_MCDRV_ADC_PERIPH_INIT() InitHSADC()
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
#if (M1_MCDRV_PWM3PH == MCDRV_PWMA)
#define M1_MCDRV_PWM_PERIPH_INIT() InitPWMA0()
#define M1_MCDRV_PWM3PH_SET_PWM_OUTPUT(par1, par2) MCDRV_eFlexSetPwmOutput(par1, par2)
#define M1_MCDRV_PWM3PH_SET_DUTY(par1, par2) MCDRV_eFlexSetDutyCycle(par1, par2)
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
 * Define motor 1 3-ph driver control functions
 ******************************************************************************/
#ifdef M1_MCDRV_SPI_DRV3PH
#if (M1_MCDRV_SPI_DRV3PH == MCDRV_SPI_DRV)
#define M1_MCDRV_DRV3PH_INIT() InitSPI()
#define M1_MCDRV_DRV3PH_RD_OC(par) MCDRV_Driver3PhReadOc(par);
#define M1_MCDRV_DRV3PH_RD_INT(par) MCDRV_Driver3PhReadInt(par);
#define M1_MCDRV_DRV3PH_CLR_FLG(par) MCDRV_Driver3PhClearFlags(par);
#define M1_MCDRV_DRV3PH_RD_S0(par) MCDRV_Driver3PhGetSr0(par);
#define M1_MCDRV_DRV3PH_RD_S1(par) MCDRV_Driver3PhGetSr1(par);
#define M1_MCDRV_DRV3PH_RD_S2(par) MCDRV_Driver3PhGetSr2(par);
#define M1_MCDRV_DRV3PH_RD_S3(par) MCDRV_Driver3PhGetSr3(par);
#else
#error MCDRV: Unsupported SPI periphery!
#endif
#else
#define M1_MCDRV_DRV3PH_RD_OC(par)
#define M1_MCDRV_DRV3PH_RD_INT(par)
#define M1_MCDRV_DRV3PH_CLR_FLG(par)
#define M1_MCDRV_DRV3PH_RD_S0(par)
#define M1_MCDRV_DRV3PH_RD_S1(par)
#define M1_MCDRV_DRV3PH_RD_S2(par)
#define M1_MCDRV_DRV3PH_RD_S3(par)
#endif

/******************************************************************************
 * Global variable definitions
 ******************************************************************************/
extern mcdrv_pwm3ph_pwma_t g_sM1Pwm3ph;
extern mcdrv_ftm_cmt_t g_sM1CmtTmr;
extern mcdrv_spi_drv3ph_t g_sM1Driver3ph;
extern mcdrv_hsadc_t g_sM1AdcSensor;
extern clock_setup_t g_sClockSetup;

/*******************************************************************************
 * API
 ******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

void MCDRV_Init_M1(void);
void InitPWMA0(void);
void InitFTM2(void);
void InitFTM1(void);
void InitPDB(void);
void InitHSADC(void);
void InitSPI(void);
void InitClock(void);

#ifdef __cplusplus
}
#endif
#endif /* _MCDRV_TWR_KV58F220_H_ */
