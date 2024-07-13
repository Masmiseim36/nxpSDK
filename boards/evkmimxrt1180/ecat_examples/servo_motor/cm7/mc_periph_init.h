/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _MC_PERIPH_INIT_H_
#define _MC_PERIPH_INIT_H_


#include "mcdrv_adc_imxrt118x.h"

#include "mcdrv_pwm3ph_pwma.h"
#include "mcdrv_enc_eqd2.h"
#include "m1_pmsm_appconfig.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Structure used during clocks and modulo calculations */
typedef struct _clock_setup
{
    uint32_t ui32FastPeripheralClock;
    uint32_t ui32CpuFrequency;
    uint32_t ui32BusClock;
    uint32_t ui32SysPllClock;
    uint16_t ui16M1SpeedLoopFreq;
    uint16_t ui16M1SpeedLoopModulo;
    uint16_t ui16M1PwmFreq;
    uint16_t ui16M1PwmModulo;
    uint16_t ui16M1PwmDeadTime;
} clock_setup_t;

#define TP0_OFF() RGPIO_PortClear(RGPIO4, 1U << 8U)
#define TP0_ON()  RGPIO_PortSet(RGPIO4, 1U << 8U)
#define TP0_TOOGLE() RGPIO_PortToggle(RGPIO4, 1u << 8U)

#define TP1_OFF() RGPIO_PortClear(RGPIO4, 1U << 13U)
#define TP1_ON()  RGPIO_PortSet(RGPIO4, 1U << 13U)
#define TP1_TOOGLE() RGPIO_PortToggle(RGPIO4, 1u << 13U)

#define TP2_OFF() RGPIO_PortClear(RGPIO4, 1U << 12U)
#define TP2_ON()  RGPIO_PortSet(RGPIO4, 1U << 12U)
#define TP2_TOOGLE() RGPIO_PortToggle(RGPIO4, 1u << 12U)

/******************************************************************************
 * Clock & PWM definition for motor 1
 ******************************************************************************/
#define M1_PWM_FREQ (16000)         /* PWM frequency - 16kHz */
#define M1_FOC_FREQ_VS_PWM_FREQ (1) /* FOC calculation is called every n-th PWM reload */
#define M1_SPEED_LOOP_FREQ (1000)   /* Speed loop frequency */
#define M1_PWM_DEADTIME (500)       /* Output PWM deadtime value in nanoseconds */

#define M1_FAST_LOOP_TS ((float_t)1.0 / (float_t)(M1_PWM_FREQ / M1_FOC_FREQ_VS_PWM_FREQ))
#define M1_SLOW_LOOP_TS ((float_t)1.0 / (float_t)(M1_SLOW_LOOP_FREQ))
#define M1_TIME_ONESEC_COUNT (M1_PWM_FREQ / M1_FOC_FREQ_VS_PWM_FREQ)

/* Fast loop frequency in Hz */
#define M1_FAST_LOOP_FREQ       (M1_PWM_FREQ / M1_FOC_FREQ_VS_PWM_FREQ)
/* Slow control loop frequency */
#define M1_SLOW_LOOP_FREQ       (1000U)

/* Over Current Fault detection */
#define M1_FAULT_NUM (0)

/* Braking resistor macros */
#define M1_BRAKE_SET()
#define M1_BRAKE_CLEAR()
/* DC bus braking threshold hysteresis */
#define M1_U_DCB_HYSTERESIS (0.05F)

/******************************************************************************
 * ADC measurement definition for motor 1
 ******************************************************************************/
/* Configuration table of ADC channels according to the input pin signals:
 * Valid for iMXRT1180 together with FRDM-MC-LVPMSM
 *
 * Motor 1
 * Quantity     | ADC1                 | ADC2    
 *              | A         | B        | A        | B        
 * -----------------------------------------------------------------------
 * M1_I_A       | ADC1_A5   |          |          | ADC2_B1
 * M1_I_B       |           | ADC1_B5  |          |
 * M1_I_C       |           |          | ADC2_A2  |
 * M1_U_DCB     |           | ADC1_B4  |          |
 *
 */

/* Phase current A assigned to ADC1 and ADC2 */
#define M1_ADC1_PH_A (5)
#define M1_ADC2_PH_A (N/A)
/* Phase current A assigned to ADC1 and ADC2 */
#define M1_ADC1_PH_B (5)
#define M1_ADC2_PH_B (1)
/* Phase current A assigned to ADC1 and ADC2 */
#define M1_ADC1_PH_C (N/A)
#define M1_ADC2_PH_C (2)
/* Phase current A assigned to ADC1 and ADC2 */
#define M1_ADC1_UDCB (4)
#define M1_ADC2_UDCB (N/A)

/* offset measurement filter window */
#define ADC_OFFSET_WINDOW (3)
/******************************************************************************
 * MC driver macro definition and check - do not change this part
 ******************************************************************************/
/******************************************************************************
 * Define common ADC control functions for motor 1
 ******************************************************************************/
/******************************************************************************
 * Define common ADC control functions for motor 1
 ******************************************************************************/
#define M1_MCDRV_ADC_PERIPH_INIT() (InitADC())
#define M1_MCDRV_ADC_GET(par) MCDRV_CurrAndVoltDcBusGet(par);

#define M1_MCDRV_CURR_3PH_CHAN_ASSIGN(par)
#define M1_MCDRV_CURR_3PH_CALIB_INIT(par) (MCDRV_Curr3Ph2ShCalibInit(par))
#define M1_MCDRV_CURR_3PH_CALIB(par) (MCDRV_Curr3Ph2ShCalib(par))
#define M1_MCDRV_CURR_3PH_CALIB_SET(par) (MCDRV_Curr3Ph2ShCalibSet(par))

/******************************************************************************
 * Define motor 1 slow control loop timer
 ******************************************************************************/
#define M1_MCDRV_TMR_SLOWLOOP_INIT() InitTMR1()

/******************************************************************************
 * Define 3-ph PWM control functions for motor 1
 ******************************************************************************/
#define M1_MCDRV_PWM_PERIPH_INIT() (M1_InitPWM())
#define M1_MCDRV_PWM3PH_SET(par) (MCDRV_eFlexPwm3PhSet(par))
#define M1_MCDRV_PWM3PH_EN(par) (MCDRV_eFlexPwm3PhOutEn(par))
#define M1_MCDRV_PWM3PH_DIS(par) (MCDRV_eFlexPwm3PhOutDis(par))
#define M1_MCDRV_PWM3PH_FLT_GET(par) (MCDRV_eFlexPwm3PhFltGet(par))

/******************************************************************************
 * Define position and speed sensor - quadrature encoder for motor 1
 ******************************************************************************/
#define M1_MCDRV_QD_PERIPH_INIT() M1_InitQD()
#define M1_MCDRV_QD_GET(par) \
        MCDRV_QdEncGetPosition(par); \
        MCDRV_QdEncGetSpeed(par); 
#define M1_MCDRV_QD_SET_DIRECTION(par) (MCDRV_QdEncSetDirection(par))
#define M1_MCDRV_QD_SET_PULSES(par) \
        MCDRV_QdEncSetPulses(par);  \
        MCDRV_QdEncUpdateParameters(par);
#define M1_MCDRV_QD_CLEAR(par) (MCDRV_QdEncClear(par))

/******************************************************************************
 * Define motor 1 CMP2 for overcurrent detection
 ******************************************************************************/
#define M1_MCDRV_CMP_INIT() InitCMP()

/******************************************************************************
 * Global variable definitions
 ******************************************************************************/
extern mcdrv_adc_t g_sM1AdcSensor;
extern mcdrv_pwm3ph_pwma_t g_sM1Pwm3ph;
extern mcdrv_eqd_enc_t g_sM1Enc;

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
void M1_InitQD(void);
void InitCMP(void);
void M1_OpenPWM(void);
void M1_ClosePWM(void);

#ifdef __cplusplus
}
#endif

#endif /* _MC_PERIPH_INIT_H_  */

