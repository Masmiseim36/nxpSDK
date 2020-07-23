/*
 * Copyright 2020 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_PERIPH_INIT_H_
#define _APP_PERIPH_INIT_H_

#include "hardware_cfg.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @brief The board name. */
#define APP_BOARD_NAME     "HVP-KV11Z75M"
/*! @brief The motor type. */
#define APP_MOTOR_TYPE     "PMSM"
/*! @brief The software version. */
#define APP_SW_VERSION     "0.2.0"

/* CPU tick measurement structure. */
typedef struct _cpu_tick_meas
{
    uint32_t ui32MeasAct;  /* Actual measured number of CPU ticks since the
                              measurement start. */
    uint32_t ui32MeasMax;  /* Maximal measured number of CPU ticks. */
} cpu_tick_meas_t;


/*******************************************************************************
 * Global variable definitions
 ******************************************************************************/
/* Fast-control loop CPU usage measurement. */
extern cpu_tick_meas_t    g_sCpuTickMeas;

/*******************************************************************************
 * API
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*!
 * @brief   Application peripheral initialization function.
 *
 * @details The function initializes non-safety timers and serial interfaces.
 *
 * @param   void
 *
 * @return  none
 */
void APPPER_Init(void);

/*!
 * @brief   Function starts the CPU tick measurement by reloading the systick
 *          timer.
 *
 * @param   void
 *
 * @return  none
 */
static inline void APPPER_CpuTickMeasStart(void);

/*!
 * @brief   Function retrieves the CPU tick measurement results.
 *
 * @param   sMeas - Structure with CPU tick measurement results.
 *
 * @return  none
 */
static inline void APPPER_CpuTickMeasGetTicks(cpu_tick_meas_t *sMeas);
/*!
 * @brief   Function recovers and clears the CPU tick timer overflow status.
 *
 * @note    Reading of the SysTick CTRL register resets the COUNTFLAG flag. The
 *          CTRL register therefore should not be read anywhere else.
 *
 * @param   void
 *
 * @return  The function returns 1U when the overflow occurred, otherwise the
 *          zero is returned.
 */
static inline uint16_t APPPER_CpuTickMeasTmrOverflow(void);


/*!
 * Function starts the CPU tick measurement by reloading the SysTick timer.
 */
/*! @note MISAR rule 8.5 violation. */
static inline void APPPER_CpuTickMeasStart(void)
{
    /* Reload the SysTick timer. */
    SysTick->VAL = SysTick->LOAD;
}

/*!
 * Function retrieves the CPU tick measurement results.
 */
/*! @note MISAR rule 8.5 violation. */
static inline void APPPER_CpuTickMeasGetTicks(cpu_tick_meas_t *sMeas)
{
    /* Get the number system clock cycles since the SystIck reload.  */
    sMeas->ui32MeasAct = SysTick->LOAD;
    sMeas->ui32MeasAct -= SysTick->VAL;

    /* Check whether the actual measurement is a new maximum. */
    if(sMeas->ui32MeasAct > sMeas->ui32MeasMax)
    {
        /* A new maximum was found. */
        sMeas->ui32MeasMax = sMeas->ui32MeasAct;
    }
}

/*!
 * Function recovers and clears the CPU tick timer overflow status.
 */
/*! @note MISAR rule 8.5 violation. */
static inline uint16_t APPPER_CpuTickMeasTmrOverflow(void)
{
    return (uint16_t)((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) >> SysTick_CTRL_COUNTFLAG_Pos);
}


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _APP_PERIPH_INIT_H_ */
