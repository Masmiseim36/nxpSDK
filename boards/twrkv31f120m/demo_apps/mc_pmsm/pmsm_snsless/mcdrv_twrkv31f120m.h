/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _MCDRV_TWR_KV31F_H_
#define _MCDRV_TWR_KV31F_H_

#include "fsl_device_registers.h"
#include "mcdrv_pwm3ph_ftm.h"
#include "mcdrv_mosfet_predrv_spi.h"
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
#define M1_PWM_DEADTIME (500)

#define M1_FAST_LOOP_TS ((float_t)1.0/(float_t)(M1_PWM_FREQ / M1_FOC_FREQ_VS_PWM_FREQ))
#define M1_SLOW_LOOP_TS ((float_t)1.0/(float_t)(M1_SLOW_LOOP_FREQ))
#define M1_TIME_ONESEC_COUNT (M1_PWM_FREQ / M1_FOC_FREQ_VS_PWM_FREQ)

/* Assignment of FTM channels to motor phases
 * 0 - FTM channels 0&1
 * 2 - FTM channels 2&3
 * 4 - FTM channels 4&5 
 * 6 - FTM channels 6&7 */
#define M1_PWM_PAIR_PHA (0)
#define M1_PWM_PAIR_PHB (2)
#define M1_PWM_PAIR_PHC (4)

/* Over-current Fault detection */
#define M1_FAULT_NUM (1)

/* Braking resistor macros */
#define M1_BRAKE_SET() 
#define M1_BRAKE_CLEAR()  

/******************************************************************************
 * ADC measurement definition
 ******************************************************************************/
/* Configuration table of ADC channels according to the input pin signals:
 * Valid for Kinetis KV31 Tower board (TWR-KV31F120M) together with TWR-MC-LV3PH
 *
 * Proper ADC channel assignment needs to follow these rules:
 *   - at least one phase current must be assigned to both ADC modules
 *   - two other phase current channels must be assigned to different ADC modules
 *   - Udcb and auxiliary channels must be assigned to different ADC modules

 *   Quantity         | ADC Module 0  | ADC Module 1
 *   --------------------------------------------------------------------------
 *   I_A              | --------      | ADCB_CH8
 *   I_B              | ADCA_CH3      | ADCB_CH0
 *   I_C              | ADCA_CH0      | --------
 *   U_DCB            | ADCA_CH13     | --------
 */
/* Phase current A assignment to ADC1 only */
#define M1_ADC0_PH_A (MCDRV_CHAN_OFF)
#define M1_ADC1_PH_A (8)
/* Phase current B assignment to ADC0 and ADC1 */
#define M1_ADC0_PH_B (3)
#define M1_ADC1_PH_B (0)
/* Phase current C assignment to ADC0 only */
#define M1_ADC0_PH_C (0)
#define M1_ADC1_PH_C (MCDRV_CHAN_OFF)
/* Udc bus voltage is assigned to ADC0 only */
#define M1_ADC0_UDCB (13)
#define M1_ADC1_UDCB (MCDRV_CHAN_OFF)
/* Aux channel is assigned to ADC1 only */
#define M1_ADC0_AUX (MCDRV_CHAN_OFF)
#define M1_ADC1_AUX (30)

/* offset measurement filter window */     
#define ADC_OFFSET_WINDOW (3)
/******************************************************************************
 * MC driver macro definition and check - do not change this part
 ******************************************************************************/
/******************************************************************************
 * define motor 1 ADC control functions
 ******************************************************************************/
#define M1_MCDRV_ADC_PERIPH_INIT() (InitADC16())
#define M1_MCDRV_ADC_GET(par) \
    MCDRV_Curr3Ph2ShGet(par); \
    MCDRV_VoltDcBusGet(par);  \
    MCDRV_AuxValGet(par);
#define M1_MCDRV_CURR_3PH_CHAN_ASSIGN(par) (MCDRV_Curr3Ph2ShChanAssign(par))
#define M1_MCDRV_CURR_3PH_CALIB_INIT(par) (MCDRV_Curr3Ph2ShCalibInit(par))
#define M1_MCDRV_CURR_3PH_CALIB(par) (MCDRV_Curr3Ph2ShCalib(par))
#define M1_MCDRV_CURR_3PH_CALIB_SET(par) (MCDRV_Curr3Ph2ShCalibSet(par))

/******************************************************************************
 * define motor 1 3-ph PWM control functions                                  *
 ******************************************************************************/
#define M1_MCDRV_PWM_PERIPH_INIT() (InitFTM0())
#define M1_MCDRV_PWM3PH_SET(par) (MCDRV_FtmPwm3PhSet(par))
#define M1_MCDRV_PWM3PH_EN(par) (MCDRV_FtmPwm3PhOutEn(par))
#define M1_MCDRV_PWM3PH_DIS(par) (MCDRV_FtmPwm3PhOutDis(par))
#define M1_MCDRV_PWM3PH_FLT_GET(par) (MCDRV_FtmPwm3PhFltGet(par))

/******************************************************************************
 * define motor 1 slow control loop timer
 ******************************************************************************/
#define M1_MCDRV_TMR_SLOWLOOP_INIT() InitFTM2()

/******************************************************************************
 * define motor 1 ADC trigger PDB
 ******************************************************************************/
#define M1_MCDRV_PDB_INIT() InitPDB()

/******************************************************************************
 * define motor 1 3-ph driver control functions
 ******************************************************************************/
#define M1_MCDRV_DRV3PH_INIT() InitSPI()
#define M1_MCDRV_DRV3PH_RD_OC(par) (MCDRV_Driver3PhReadOc(par))
#define M1_MCDRV_DRV3PH_RD_INT(par) (MCDRV_Driver3PhReadInt(par))
#define M1_MCDRV_DRV3PH_CLR_FLG(par) (MCDRV_Driver3PhClearFlags(par))
#define M1_MCDRV_DRV3PH_RD_S0(par) (MCDRV_Driver3PhGetSr0(par))
#define M1_MCDRV_DRV3PH_RD_S1(par) (MCDRV_Driver3PhGetSr1(par))
#define M1_MCDRV_DRV3PH_RD_S2(par) (MCDRV_Driver3PhGetSr2(par))
#define M1_MCDRV_DRV3PH_RD_S3(par) (MCDRV_Driver3PhGetSr3(par))
         
/******************************************************************************
 * global variable definitions
 ******************************************************************************/
extern mcdrv_pwm3ph_ftm_t g_sM1Pwm3ph;
extern mcdrv_adc16_t g_sM1AdcSensor;
extern mcdrv_spi_drv3ph_t g_sM1Driver3ph;
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
void InitSPI(void);
void InitFTM2(void);
void InitPDB(void);
void InitADC16(void);

#ifdef __cplusplus
}
#endif
#endif /* _MCDRV_TWR_KV31F_H_  */
