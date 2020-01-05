/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "mcdrv.h"
#include "m1_sm_ref_sol.h"

#include "fsl_common.h"
#include "fsl_port.h"
#include "pin_mux.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Three instruction added after interrupt flag clearing as required */
#define M1_END_OF_ISR \
    {                 \
        __asm("nop"); \
        __asm("nop"); \
        __asm("nop"); \
    }

/*******************************************************************************
 * Variables
 ******************************************************************************/

static bool_t bDemoMode;                /* demo mode enabled/disabled by SW2 button  */
static uint32_t ui32SpeedStimulatorCnt; /* used for demo mode */
static uint32_t ui32ButtonFilter;       /* counter for button pressing */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

static void DemoSpeedStimulator(void);

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
* @brief   Application main function processing peripheral function calling and
*          infinite loop
*
* @param   void
*
* @return  none
*/
int main(void)
{
    uint32_t ui32PrimaskReg;

    /* disable all interrupts before peripherals are initialized */
    ui32PrimaskReg = DisableGlobalIRQ();

    /* init board hardware. */
    /* LED pin configuration */
#if 0 /* output pin config disabled, there are LEDs on frdm-ke16z board, 
         but have conflict with PWM signals */
    const gpio_pin_config_t output_pin_config = {
        kGPIO_DigitalOutput, /* Set current pin as digital output */
        (uint8_t)1U          /* Set default logic high */
    };
#endif
    /* Initialize clock configuration */
    BOARD_InitBootClocks();
    
    /* Init pins set in pin_mux file */
    BOARD_InitBootPins();

    /* SW2 pin config - PTD3 */
    PORT_SetPinInterruptConfig(PORTD, 3U, kPORT_InterruptRisingEdge); /* Enable interrupt */
    
    /* Enable & setup interrupts */
    EnableIRQ(PORTBCD_IRQn);
    NVIC_SetPriority(PORTBCD_IRQn, 4);

    /* init application clock dependent variables */
    InitClock();

    /* initialize peripheral motor control driver for motor M1*/
    MCDRV_Init_M1();
    
    /* disable demo mode after reset */
    bDemoMode = FALSE;
    ui32SpeedStimulatorCnt = 0;

    /* enable interrupts  */
    EnableGlobalIRQ(ui32PrimaskReg);

    /* infinite loop */
    while (1)
    {
        
    }
}

/*!
* @brief   ADC conversion complete ISR called with 100us period processes
*           - motor M1 fast application machine function
*
* @param   void
*
* @return  none
*/
void ADC0_IRQHandler(void)
{
    /* StateMachine call */
    SM_StateMachineFast(&g_sM1Ctrl);
    
    /* add empty instructions for correct interrupt flag clearing */
    M1_END_OF_ISR;
}

/*!
* @brief   FTM1 interrupt service routine
*          - Forced commutation control
*
* @param   void
*
* @return  none
*/
void FTM1_IRQHandler(void)
{
    /* asynchronous time event processing */
    M1_TimeEvent();

    /* clear flag */
    FTM1->CONTROLS[0].CnSC &= ~(FTM_CnSC_CHF_MASK);

    /* add empty instructions for correct interrupt flag clearing */
    M1_END_OF_ISR;
}

/*!
* @brief   LPIT reload ISR called with 1ms period and processes following functions:
*           - motor M1 slow application machine function
*
* @param   void
*
* @return  none
*/
void LPIT0_IRQHandler(void)
{

    /* Slow StateMachine call */
    SM_StateMachineSlow(&g_sM1Ctrl);
    
    /* Demo Speed Simulator */
    DemoSpeedStimulator();

    /* Clear the TOF flag */
    LPIT0->MSR |= (LPIT_MSR_TIF0_MASK);

    /* add empty instructions for correct interrupt flag clearing */
    M1_END_OF_ISR;
}

/*!
* @brief   PDB_Error_ISR_Handler
*           - handling the PDB error interrupt: reinitializes the PDB module
*
* @param   void
*
* @return  none
*/
void PDB0_IRQHandler(void)
{
    PDB0->SC &= (~PDB_SC_PDBEN_MASK);   /* Disable PDB */
    PDB0->CH[0].S &= (~PDB_S_CF_MASK);  /* Clear flag on channel 0 */
    PDB0->CH[0].S &= (~PDB_S_ERR_MASK); /* Clear error on channel 0 */
    PDB0->SC |= PDB_SC_PDBEN_MASK;      /* Enable PDB */
}

/*!
* @brief   Port interrupt handler
*
* @param   void
*
* @return  none
*/
void PORTBCD_IRQHandler(void)
{
    if (PORTD->PCR[3] & PORT_PCR_ISF_MASK)
    {
        /* clear the flag */
        PORTD->PCR[3] |= PORT_PCR_ISF_MASK;

        /* proceed only if pressing longer than timeout */
        if (ui32ButtonFilter > 200)
        {
            ui32ButtonFilter = 0;
            if (bDemoMode)
            {
                M1_SetSpeed(0);
                M1_SetAppSwitch(FALSE);
                bDemoMode = FALSE;
            }
            else
            {
                M1_SetAppSwitch(TRUE);
                bDemoMode = TRUE;
                ui32SpeedStimulatorCnt = 0;
            }
        }
    }
}

/*!
* @brief   DemoSpeedStimulator
*           - When demo mode is enabled it changes the required speed according
*             to predefined profile
*
* @param   void
*
* @return  none
*/
static void DemoSpeedStimulator(void)
{
    /* increase push button pressing counter  */
    if (ui32ButtonFilter < 1000)
        ui32ButtonFilter++;

    if (bDemoMode)
    {
        ui32SpeedStimulatorCnt++;
        switch (ui32SpeedStimulatorCnt)
        {
            case 1:
                M1_SetSpeed(FRAC16(1000.0 / M1_N_MAX));
                break;
            case 2500:
                M1_SetSpeed(FRAC16(2000.0 / M1_N_MAX));
                break;
            case 5000:
                M1_SetSpeed(FRAC16(1500.0 / M1_N_MAX));
                break;
            case 7500:
                M1_SetSpeed(FRAC16(1000.0 / M1_N_MAX));
                break;
            case 10000:
                M1_SetSpeed(FRAC16(-2000.0 / M1_N_MAX));
                break;
            case 20000:
                ui32SpeedStimulatorCnt = 0;
                break;
            default:
                break;
        }
    }
}
