/*
 * Copyright 2020 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _MCDRV_PERIPH_INIT_H_
#define _MCDRV_PERIPH_INIT_H_

#include "hardware_cfg.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * PWM configuration.
 ******************************************************************************/
/* PWM periphery for motor M1. */
#define M1_PWM_PERIPH             (FTM0)

/* Number of PWM periods per one fast control loop execution. */
#define M1_FOC_FREQ_VS_PWM_FREQ   (1U)

/* Output PWM frequency in Hz. */
#define M1_PWM_FREQ               (IRQ_FAST_LOOP_FREQ * M1_FOC_FREQ_VS_PWM_FREQ)

/* Output PWM dead-time in nanoseconds. */
#define M1_PWM_DEADTIME           (1560U)

/* Assignment of the following FTM channels to the motor phases A, B, and C.
 * 0 - FTM channels 0&1
 * 2 - FTM channels 2&3
 * 4 - FTM channels 4&5
 * 6 - FTM channels 6&7 */
#define M1_PWM_PAIR_PHA   (0U)
#define M1_PWM_PAIR_PHB   (2U)
#define M1_PWM_PAIR_PHC   (4U)

/* Over-current fault detection FTM periphery input. */
#define M1_PWM_FAULT_IDX  (0U)      /*< FTM fault channel number. */


/*******************************************************************************
 * GPIO pin initialization and control.
 ******************************************************************************/
/* The in-rush relay activation delay. */
#define M1_INRUSH_RELAY_DLY     (uint16_t)(0.75F * (float_t)CLOCK_SYSTICK_FREQ)


/* In-rush relay control macros. */
#define M1_RELAY_ON()          (PIN_RELAY_GPIO->PSOR = (0x01UL << PIN_RELAY_PIN))
#define M1_RELAY_OFF()         (PIN_RELAY_GPIO->PCOR = (0x01UL << PIN_RELAY_PIN))


/*******************************************************************************
 * ADC measurement definition
 ******************************************************************************/
/* ADC channels assignment.
 *
 * Proper ADC channel assignment needs to follow these rules:
 *   - Only two phase currents are measured per SVM sector:
 *         -SVM sector 1 - I_PHB and I_PHC
 *         -SVM sector 2 - I_PHA and I_PHC
 *         -SVM sector 3 - I_PHA and I_PHC
 *         -SVM sector 4 - I_PHA and I_PHB
 *         -SVM sector 5 - I_PHA and I_PHB
 *         -SVM sector 6 - I_PHB and I_PHC
 *     The measurable phase current pair must, therefore, be assigned to two
 *     different ADC converters.
 *   - DC-bus voltage (U_DCB) and auxiliary (AUX) channels must be assigned to
 *     different ADC converters.
 *   - (ADC16 only) Raise the ADC_SC1_AIEN_MASK flag in I_PH measurement
 *     channels for ADC16 periphery, which should raise an interrupt.
 *
 * This application is configured for the Kinetis KV11 HVP board (HVP-K11Z75M),
 * together with HVP-MC3PH. Following table thus applies:
 *
 *                      Quantity  | ADC0  |  ADC1 |
 *                      ----------------------------
 *                       I_PHA    | ----- |  CH4  |
 *                       I_PHB    |  CH9  |  CH9  |
 *                       I_PHC    |  CH0  | ----- |
 *                       U_DCB    |  CH4  | ----- |
 *                       AUX      | ----- |  CH6  |
 */
/* SVM sector 1&6 - the phase B and phase C are measured.
   -Assign each phase current to different ADC. */
#define M1_ADC_SVM16_I_PHB_ADC     (ADC1)
#define M1_ADC_SVM16_I_PHB_CHN     (9U)
#define M1_ADC_SVM16_I_PHC_ADC     (ADC0)
#define M1_ADC_SVM16_I_PHC_CHN     (0U | ADC_SC1_AIEN_MASK)

/* SVM sector 2&3 - the phase A and phase C are measured.
   -Assign each phase current to different ADC. */
#define M1_ADC_SVM23_I_PHA_ADC     (ADC1)
#define M1_ADC_SVM23_I_PHA_CHN     (4U)
#define M1_ADC_SVM23_I_PHC_ADC     (ADC0)
#define M1_ADC_SVM23_I_PHC_CHN     (0U | ADC_SC1_AIEN_MASK)

/* SVM sector 4&5 - the phase A and phase B are measured.
   -Assign each phase current to different ADC. */
#define M1_ADC_SVM45_I_PHA_ADC     (ADC1)
#define M1_ADC_SVM45_I_PHA_CHN     (4U)
#define M1_ADC_SVM45_I_PHB_ADC     (ADC0)
#define M1_ADC_SVM45_I_PHB_CHN     (9U | ADC_SC1_AIEN_MASK)

/* DC-bus voltage U_DCB channel and converter assignment.
   -Assign U_DCB to different ADC converter than the AUX signal. */
#define M1_ADC_U_DCB_ADC           (ADC0)
#define M1_ADC_U_DCB_CHN           (4U)

/* Auxiliary channel AUX channel and converter assignment.
   -Assign AUX to different ADC converter than the U_DCB signal. */
#define M1_ADC_AUX_ADC             (ADC1)
#define M1_ADC_AUX_CHN             (6U)

/* Phase current offset measurement moving-average filter window in
   powers of two (for example 8 means that 256 samples will be taken for each
   SVM sector). */
#define ADC_I_PH_OFFSET_SMPLS       (10U)

/* Phase current offset measurement settle time after PWM enablement in
   milliseconds. */
#define ADC_I_PH_OFFSET_SETTLE      (100U)

/*******************************************************************************
 * API
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*!
 * @brief   Motor control driver main initialization function. Peripherals
 *          required for safe motor-control functionality are configured.
 *
 * @param   void
 *
 * @return  none
 */
void MCDRV_fsInit(void);

/*!
 * @brief   The function starts the fast and slow loop timers.
 *
 * @param   void
 *
 * @return  none
 */
void MCDRV_fsStartTimers(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _MCDRV_PERIPH_INIT_H_ */
