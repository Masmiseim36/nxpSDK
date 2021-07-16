/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* This is a generic configuration file of the motor control driver. You need to edit the file.
 * Remove this warning statement after this file is edited manually or
 * re-generate this file using MC_PMSM Config Tool component.
 */
#warning This mc_periph_init.h file needs to be edited to configure low-level motor control driver.
#define MC_PMSM_DISABLE 1 // TODO: set to 0 after this configuration is ready to be used

#ifndef _MC_PERIPH_INIT_H_
#define _MC_PERIPH_INIT_H_

#include "fsl_device_registers.h"
#include "mcdrv_pwm3ph_ftm.h"
#include "mcdrv_adc_adc16.h"

/******************************************************************************
 * Timing
 ******************************************************************************/
/* Fast loop interrupt generation timer*/
#define M1_PWM_TIMER            (FTM0)
/* PWM generation timer */
#define M1_PWM_TIMER_FTM0
/* PWM frequency in Hz*/
#define M1_PWM_FREQ             (10000U)
/* PWM modulo = FTM_input_clock / M1_PWM_FREQ */
#define M1_PWM_MODULO           (40000000UL / M1_PWM_FREQ)
/* PWM vs. Fast control loop ratio */
#define M1_FOC_FREQ_VS_PWM_FREQ (1U)
/* Fast loop frequency in Hz */
#define M1_FAST_LOOP_FREQ       (M1_PWM_FREQ / M1_FOC_FREQ_VS_PWM_FREQ)
/* Slow loop interrupt generation timer*/
#define M1_SLOW_LOOP_TIMER      (FTM2)
/* Slow loop interrupt generation timer */
#define M1_SLOW_LOOP_TIMER_FTM2
/* Slow control loop frequency */
#define M1_SLOW_LOOP_FREQ       (1000U)
/* Slow Loop modulo = FTM_input_clock / M1_SLOW_LOOP_FREQ */
#define M1_SLOW_LOOP_MODULO     (40000000UL / M1_SLOW_LOOP_FREQ)
/* Fast loop period */
#define M1_FAST_LOOP_TS         ((float_t)1.0 / (float_t)(M1_FAST_LOOP_FREQ))
/* Slow loop period */
#define M1_SLOW_LOOP_TS         ((float_t)1.0 / (float_t)(M1_SLOW_LOOP_FREQ))

 /******************************************************************************
  * Output control
  ******************************************************************************/
/* Assignment of FTM channels to motor phases */
#define M1_PWM_PAIR_PHA         (0U)
#define M1_PWM_PAIR_PHB         (2U)
#define M1_PWM_PAIR_PHC         (4U)

/* Output PWM deadtime enable */
#define M1_PWM_DEADTIME_ENABLE          (1U)
/* Output PWM deadtime prescaler */
#define M1_PWM_DEADTIME_LENGTH_DTPS     (1U)
/* Output PWM deadtime register value */
#define M1_PWM_DEADTIME_LENGTH_DTVAL    (20U)

/* Over-current Fault enable */
#define M1_FAULT_ENABLE         (1U)
/* Over-current Fault detection number */
#define M1_FAULT_NUM            (1U)
/* Over-current Fault detection polarity */
#define M1_FAULT_POL            (0U)
/* Over-current Fault detection use CMP */
#define M1_FAULT_CMP_ENABLE     (1U)
/* Over-current Fault detection CMP instance */
#define M1_FAULT_CMP_INSTANCE   (CMP1)
/* Over-current Fault detection threshold */
#define M1_FAULT_CMP_THRESHOLD  (61U)

/* DC bus braking resistor control */
#define M1_BRAKE_SET()          
#define M1_BRAKE_CLEAR()        

/* Top and Bottom transistors PWM polarity */
#define M1_PWM_POL_TOP         (1U)
#define M1_PWM_POL_BOTTOM      (1U)

/******************************************************************************
 * ADC measurement definition
 ******************************************************************************/
/* Configuration table of ADC channels according to the input pin signals:
 *
 * Proper ADC channel assignment needs to follow these rules:
 *   - one phase current must be assigned to both ADC modules
 *   - two other phase current channels must be assigned to different ADC modules
 *   - Udcb and auxiliary channels must be assigned to differrent ADC modules

 *   Quantity | ADC Module 0 | ADC Module 1
 *   --------------------------------------------------------------------------
 *   I_A      |   ADC_CH8   |   ADC_CH8
 *   I_B      |   -------   |   ADC_CH0
 *   I_C      |   ADC_CH0   |   -------
 *   U_DCB    |   ADC_CH13   |   -------
 *   AUX      |   -------   |   ADC_CH26
 */
/* ADC channels assingnment */
/* Phase current assignment in sectors 1,6 */
#define M1_SEC16_PH_C_BASE      (ADC0)
#define M1_SEC16_PH_C_CHANNEL   (0U)
#define M1_SEC16_PH_B_BASE      (ADC1)
#define M1_SEC16_PH_B_CHANNEL   (0U)
/* Phase current assignment in sectors 2,3 */
#define M1_SEC23_PH_C_BASE      (ADC0)
#define M1_SEC23_PH_C_CHANNEL   (0U)
#define M1_SEC23_PH_A_BASE      (ADC1)
#define M1_SEC23_PH_A_CHANNEL   (8U)
/* Phase current assignment in sectors 4,5 */
#define M1_SEC45_PH_A_BASE      (ADC0)
#define M1_SEC45_PH_A_CHANNEL   (8U)
#define M1_SEC45_PH_B_BASE      (ADC1)
#define M1_SEC45_PH_B_CHANNEL   (0U)
/* DC bus voltage channels assingnment */
#define M1_UDCB_BASE            (ADC0)
#define M1_UDCB_CHANNEL         (13U)
/* Auxiliary channel assingnment */
#define M1_AUX_BASE             (ADC1)
#define M1_AUX_CHANNEL          (26U)

/* ADC0 channel muxing (ADC_MUXSEL) */
#define ADC0_MUXSEL             (0U)
/* ADC1 channel muxing (ADC_MUXSEL) */
#define ADC1_MUXSEL             (0U)

/* Phase current offset measurement filter window */
#define ADC_OFFSET_WINDOW       (3U)

/* PDB pre-trigger delay (dead-time/2) */
#define PDB_PRETRIG_DELAY       (10U)

/******************************************************************************
 * Inrush relay
 ******************************************************************************/
/* Inrush relay enable */
#define M1_INRUSH_ENABLE        (0U)
/* Inrush relay delay */
#define M1_INRUSH_DELAY         (0U)
/* Inrush relay pin */
#define M1_INRUSH_SET()         
#define M1_INRUSH_CLEAR()       

/******************************************************************************
 * MC driver macro definition and check - do not change this part
 ******************************************************************************/
/******************************************************************************
 * Define motor ADC control functions
 ******************************************************************************/
#define M1_MCDRV_ADC_GET(par) \
    MCDRV_Curr3Ph2ShGet(par); \
    MCDRV_VoltDcBusGet(par);  \
    MCDRV_AuxValGet(par);
#define M1_MCDRV_CURR_3PH_CHAN_ASSIGN(par) (MCDRV_Curr3Ph2ShChanAssign(par))
#define M1_MCDRV_CURR_3PH_CALIB_INIT(par) (MCDRV_Curr3Ph2ShCalibInit(par))
#define M1_MCDRV_CURR_3PH_CALIB(par) (MCDRV_Curr3Ph2ShCalib(par))
#define M1_MCDRV_CURR_3PH_CALIB_SET(par) (MCDRV_Curr3Ph2ShCalibSet(par))

/******************************************************************************
 * Define motor 3-ph PWM control functions
 ******************************************************************************/
#define M1_MCDRV_PWM3PH_SET(par) (MCDRV_FtmPwm3PhSet(par))
#define M1_MCDRV_PWM3PH_EN(par) (MCDRV_FtmPwm3PhOutEn(par))
#define M1_MCDRV_PWM3PH_DIS(par) (MCDRV_FtmPwm3PhOutDis(par))
#define M1_MCDRV_PWM3PH_FLT_GET(par) (MCDRV_FtmPwm3PhFltGet(par))

/******************************************************************************
 * global variable definitions
 ******************************************************************************/
extern mcdrv_pwm3ph_ftm_t g_sM1Pwm3ph;
extern mcdrv_adc16_t g_sM1AdcSensor;

/*******************************************************************************
 * API
 ******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
void MCDRV_Init_M1(void);

#ifdef __cplusplus
}
#endif
#endif /* _MC_PERIPH_INIT_H_  */
