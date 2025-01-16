/*
* Copyright 2016, Freescale Semiconductor, Inc.
* Copyright 2016-2024 NXP
*
* NXP Proprietary. This software is owned or controlled by NXP and may
* only be used strictly in accordance with the applicable license terms. 
* By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that
* you have read, and that you agree to comply with and are bound by,
* such license terms.  If you do not agree to be bound by the applicable
* license terms, then you may not retain, install, activate or otherwise
* use the software.
 */

#ifndef _MC_PERIPH_INIT_H_
#define _MC_PERIPH_INIT_H_

#include "mlib_FP.h"
#include "gflib_FP.h"
#include "gmclib_FP.h"
#include "amclib_FP.h"

/******************************************************************************
 * Timing
 ******************************************************************************/
/* PWM frequency in Hz*/
#define M1_PWM_FREQ             (10000U)
/* PWM modulo = FTM_input_clock / M1_PWM_FREQ */
#define M1_PWM_MODULO           (40000000UL / M1_PWM_FREQ)
/* PWM vs. Fast control loop ratio */
#define M1_FOC_FREQ_VS_PWM_FREQ (1U)
#ifdef DUAL_MOTOR
#define M2_FOC_FREQ_VS_PWM_FREQ (1U)
#endif
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
/* DC bus braking resistor control */
#define M1_BRAKE_SET()
#define M1_BRAKE_CLEAR()
#ifdef DUAL_MOTOR
#define M2_BRAKE_SET()
#define M2_BRAKE_CLEAR()
#endif

/* DC bus braking threshold hysteresis */
#define M1_U_DCB_HYSTERESIS (0.05F)
#ifdef DUAL_MOTOR
#define M2_U_DCB_HYSTERESIS (0.05F)
#endif

/******************************************************************************
 * MC driver macro definition and check - do not change this part
 ******************************************************************************/
/******************************************************************************
 * Define motor ADC control functions
 ******************************************************************************/
#define M1_MCDRV_ADC_GET(par)
#define M1_MCDRV_CURR_3PH_CHAN_ASSIGN(par)
#define M1_MCDRV_CURR_3PH_CALIB_INIT(par)
#define M1_MCDRV_CURR_3PH_CALIB(par)
#define M1_MCDRV_CURR_3PH_CALIB_SET(par)
#ifdef DUAL_MOTOR
#define M2_MCDRV_ADC_GET(par)
#define M2_MCDRV_CURR_3PH_CHAN_ASSIGN(par)
#define M2_MCDRV_CURR_3PH_CALIB_INIT(par)
#define M2_MCDRV_CURR_3PH_CALIB(par)
#define M2_MCDRV_CURR_3PH_CALIB_SET(par)
#endif

/******************************************************************************
 * Define motor 3-ph PWM control functions
 ******************************************************************************/
#define M1_MCDRV_PWM3PH_SET(par)
#define M1_MCDRV_PWM3PH_EN(par)
#define M1_MCDRV_PWM3PH_DIS(par)
#define M1_MCDRV_PWM3PH_FLT_GET(par)    (1U)
#ifdef DUAL_MOTOR
#define M2_MCDRV_PWM3PH_SET(par)
#define M2_MCDRV_PWM3PH_EN(par)
#define M2_MCDRV_PWM3PH_DIS(par)
#define M2_MCDRV_PWM3PH_FLT_GET(par)    (1U)
#endif

/******************************************************************************
 * Define position and speed sensor - quadrature encoder for motor 1, 2
 ******************************************************************************/
#define M1_MCDRV_QD_GET(par)
#define M1_MCDRV_QD_SET_DIRECTION(par)
#define M1_MCDRV_QD_SET_PULSES(par)
#define M1_MCDRV_QD_CLEAR(par)
#define MCDRV_QdEncSetPulses(par)
#ifdef DUAL_MOTOR
#define M2_MCDRV_QD_GET(par)
#define M2_MCDRV_QD_SET_DIRECTION(par)
#define M2_MCDRV_QD_SET_PULSES(par)
#define M2_MCDRV_QD_CLEAR(par)
#endif

/******************************************************************************
 * global variable definitions - definitions are only template
 ******************************************************************************/
typedef struct _mcdrv_adc_temp
{
  GMCLIB_3COOR_T_F16 *psIABC; /* pointer to the 3-phase currents */

  uint16_t *pui16SVMSector; /* pointer to the SVM sector */
  frac16_t *pui16AuxChan;   /* pointer to auxiliary ADC channel number */
  frac16_t *pf16UDcBus;     /* pointer to DC Bus voltage variable */
} mcdrv_adc_temp_t;

typedef struct _mcdrv_pwm_temp
{
  GMCLIB_3COOR_T_F16 *psUABC; /* pointer to the 3-phase pwm duty cycles */
} mcdrv_pwm_temp_t;

typedef struct _clock_setup
{
  uint32_t ui32CpuFrequency;

	/* Motor 1 */
  uint16_t ui16M1SpeedLoopFreq;
  uint16_t ui16M1PwmFreq;

#ifdef DUAL_MOTOR
	/* Motor 2 */
  uint16_t ui16M2SpeedLoopFreq;
  uint16_t ui16M2PwmFreq;
#endif
} clock_setup_t;

typedef struct _mcdrv_qd_enc_temp
{
  AMCLIB_TRACK_OBSRV_T_FLT sTo; /* tracking observer structure */
  float_t *pfltSpdMeEst;        /* pointer to measured mechanical speed  */
  frac16_t *pf16PosElEst;       /* pointer to measured electrical position */
  uint16_t ui16Pp;              /* number of motor pole pairs */
  acc32_t a32PosMeReal;         /* real position (revolution counter + mechanical position) */
	float_t fltSpdMeEst;   				/* Measured Mechanical Speed */
	frac16_t f16PosMe;     				/* Meassured Mechanical Position */
	frac16_t f16PosMeEst;  				/* Position Encoder Mechanical */
	bool_t bDirection;   					/* Encoder direction */
	float_t fltSpdEncMin;  				/* Encoder minimal speed */
	uint16_t ui16PulseNumber; 		/* Encoder pulses */
	acc32_t a32PosMeGain; 				/* Position mechanical gain */
} mcdrv_qd_enc_temp_t;

extern mcdrv_adc_temp_t g_sM1AdcSensor;
extern mcdrv_pwm_temp_t g_sM1Pwm3ph;
extern clock_setup_t g_sClockSetup;
extern mcdrv_qd_enc_temp_t g_sM1Enc;

#ifdef DUAL_MOTOR
extern mcdrv_adc_temp_t g_sM2AdcSensor;
extern mcdrv_pwm_temp_t g_sM2Pwm3ph;
extern mcdrv_qd_enc_temp_t g_sM2Enc;
#endif

/*******************************************************************************
 * API
 ******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif
#endif /* _MC_PERIPH_INIT_H_  */
