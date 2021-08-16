/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mc_periph_init.h"
#include "fsl_common.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* configuration structure for 3-phase PWM mc driver */
mcdrv_pwm3ph_ftm_t g_sM1Pwm3ph;

/* structure for current and voltage measurement*/
mcdrv_adc16_t g_sM1AdcSensor;

/* structure containing clock frequencies and modulos */
clock_setup_t g_sClockSetup;

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief   void MCDRV_Init_M1(void)
 *           - Motor control driver main initialization
 *           - Calls initialization functions of peripherals required for motor
 *             control functionality
 *
 * @param   void
 *
 * @return  none
 */
void MCDRV_Init_M1(void)
{
    /* Init application clock dependent variables */
    InitClock();

    /* Init ADC */
    InitADC();

    /* 6-channel PWM peripheral init  */
    InitPWM();
    
}

/*!
 * @brief      Core, bus, flash clock setup
 *
 * @param      void
 *
 * @return     none
 */
void InitClock(void)
{
    g_sClockSetup.ui32SystemClock = CLOCK_GetFreq(kCLOCK_CoreSysClk);
    g_sClockSetup.ui32BusClock    = CLOCK_GetFreq(kCLOCK_BusClk);

    g_sClockSetup.ui16M1PwmFreq = M1_PWM_FREQ; /* 10kHz */
                                               /* PWM module calculated as follows:
                                                * PWM_MOD = PWM_CLOCK / PWM_FREQUNCY = 60 MHz / 10 kHz = 6000 */

    g_sClockSetup.ui16M1PwmModulo     = g_sClockSetup.ui32BusClock / g_sClockSetup.ui16M1PwmFreq;
    g_sClockSetup.ui16M1PwmDeadTime   = g_sClockSetup.ui32BusClock / (1000000000U / M1_PWM_DEADTIME);
    g_sClockSetup.ui16M1SpeedLoopFreq = M1_SPEED_LOOP_FREQ; /* 1kHz */
}

/*!
 * @brief   void InitFTM0(void)
 *           - Initialization of the FTM0 peripheral for motor M1
 *           - 3-phase center-aligned PWM
 *
 * @param   void
 *
 * @return  none
 */
void InitPWM(void)
{
    /* ---------------------------------------- */
    /* Initialization FTM 3-phase PWM mc driver */
    g_sM1Pwm3ph.pui32PwmBase = (FTM_Type *)(FTM0); /* FTM0 base address */
    g_sM1Pwm3ph.ui16ChanPhA  = M1_PWM_PAIR_PHA;    /* PWM phase A top&bottom channel pair number */
    g_sM1Pwm3ph.ui16ChanPhB  = M1_PWM_PAIR_PHB;    /* PWM phase B top&bottom channel pair number */
    g_sM1Pwm3ph.ui16ChanPhC  = M1_PWM_PAIR_PHC;    /* PWM phase C top&bottom channel pair number */

    /* FTM Fault number for over current fault detection */
    g_sM1Pwm3ph.ui16FaultFixNum = M1_FAULT_NUM;
}


/*!
 * @brief   void InitADC16(void)
 *           - Initialization of the ADC16 peripheral
 *           - Initialization of the A/D converter for current and voltage sensing
 *
 * @param   void
 *
 * @return  none
 */
void InitADC(void)
{
    /**************************************/
    /* motor M1 ADC driver initialization */
    /**************************************/
    /* offset filter window */
    g_sM1AdcSensor.ui16OffsetFiltWindow = ADC_OFFSET_WINDOW;

    /* Phase current measurement */
    /* Sector 1,6 - measured currents Ic & Ib */
    /* ADC0, channel Ic = M1_ADC0_PH_C, , SAMPLE & RESULT = 0 */
    g_sM1AdcSensor.sCurrSec16.pui32AdcBasePhaC = (ADC_Type *)ADC0;
    g_sM1AdcSensor.sCurrSec16.ui16ChanNumPhaC  = M1_ADC0_PH_C;
    /* ADC1, channel Ib = M1_ADC1_PH_B, , SAMPLE & RESULT = 0 */
    g_sM1AdcSensor.sCurrSec16.pui32AdcBasePhaB = (ADC_Type *)ADC1;
    g_sM1AdcSensor.sCurrSec16.ui16ChanNumPhaB  = M1_ADC1_PH_B;

    /* Sector 2,3 - measured currents Ic & Ia*/
    /* ADC0, channel Ic = M1_ADC0_PH_C, SAMPLE & RESULT = 0 */
    g_sM1AdcSensor.sCurrSec23.pui32AdcBasePhaC = (ADC_Type *)ADC0;
    g_sM1AdcSensor.sCurrSec23.ui16ChanNumPhaC  = M1_ADC0_PH_C;
    /* ADC1, channel Ia = M1_ADC1_PH_A, SAMPLE & RESULT = 0 */
    g_sM1AdcSensor.sCurrSec23.pui32AdcBasePhaA = (ADC_Type *)ADC1;
    g_sM1AdcSensor.sCurrSec23.ui16ChanNumPhaA  = M1_ADC1_PH_A;

    /* Sector 4,5 - measured currents Ia & Ib */
    /* ADC0, channel Ia = M1_ADC0_PH_A, SAMPLE & RESULT = 0 */
    g_sM1AdcSensor.sCurrSec45.pui32AdcBasePhaA = (ADC_Type *)ADC0;
    g_sM1AdcSensor.sCurrSec45.ui16ChanNumPhaA  = M1_ADC0_PH_A;
    /* ADC1, channel Ib = M1_ADC1_PH_B, SAMPLE & RESULT = 0  */
    g_sM1AdcSensor.sCurrSec45.pui32AdcBasePhaB = (ADC_Type *)ADC1;
    g_sM1AdcSensor.sCurrSec45.ui16ChanNumPhaB  = M1_ADC1_PH_B;

    /* UDCbus channel measurement */
    /*  channel Udcb = M1_ADC0_UDCB, SAMPLE & RESULT = 1 */
    g_sM1AdcSensor.pui32UdcbAdcBase = (ADC_Type *)ADC0;
    g_sM1AdcSensor.ui16ChanNumVDcb  = M1_ADC0_UDCB;

    /* Aux channel measurement */
    /*  channel Aux = M1_ADC1_AUX, SAMPLE & RESULT = 1 */
    g_sM1AdcSensor.pui32AuxAdcBase = (ADC_Type *)ADC1;
    g_sM1AdcSensor.ui16ChanNumAux  = M1_ADC1_AUX;

    /* Assign channels and init all pointers */
    MCDRV_Curr3Ph2ShChanAssignInit(&g_sM1AdcSensor);

}
