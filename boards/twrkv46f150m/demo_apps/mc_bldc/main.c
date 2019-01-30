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

#define LED_LV3PH_RED_TOGGLE() (GPIOE->PTOR = (1U << 24))
#define LED_LV3PH_RED_ON() (GPIOE->PSOR = (1U << 24))
#define LED_LV3PH_RED_OFF() (GPIOE->PCOR = (1U << 24))

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* Counter of unsuccessful PDB triggers */
volatile uint16_t ui16TriggerErrorCnt;

/* Demo mode enabled/disabled */
static bool_t bDemoMode = FALSE;

/* Used for demo mode */
static uint32_t ui32SpeedStimulatorCnt = 0;

/* Counter for button pressing */
static uint32_t ui32ButtonFilter = 0;

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

    /* Disable all interrupts before peripherals are initialized */
    ui32PrimaskReg = DisableGlobalIRQ();

    /* Init board hardware. */
    const gpio_pin_config_t gpio_config = {
        kGPIO_DigitalOutput, 0,
    };

    /* Initialize pins configuration */
    BOARD_InitPins();

    /* Initialize clock configuration */
    BOARD_BootClockRUN();

    /* Enable port for push button SW2 */
    PORT_SetPinInterruptConfig(PORTE, 6U, kPORT_InterruptRisingEdge);

    /* Enable & setup interrupts */
    EnableIRQ(PORTE_IRQn);
    NVIC_SetPriority(PORTE_IRQn, 4);

    /* Enable port for LV3PH RED USER LED and turn it off */
    GPIO_PinInit(GPIOE, 24U, &gpio_config);
    GPIO_PortClear(GPIOE, 1U << 24);

    /* Init application clock dependent variables */
    InitClock();

    /* Initialize peripheral motor control driver for motor M1*/
    MCDRV_Init_M1();

    /* Turn off application */
    M1_SetAppSwitch(0);

    /* Disable demo mode after reset */
    bDemoMode = FALSE;
    ui32SpeedStimulatorCnt = 0;

    /* Enable interrupts  */
    EnableGlobalIRQ(ui32PrimaskReg);

    /* Infinite loop */
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
void ADCA_IRQHandler(void)
{
    /* State machine */
    SM_StateMachineFast(&g_sM1Ctrl);

    /* Clear the EOS flag */
    ADC->STAT |= (ADC_STAT_EOSI1_MASK | ADC_STAT_EOSI0_MASK);
    
 
    __DSB();
}

/*!
* @brief   FTM1 ISR called asynchronously
*           - forced motor commutation
*
* @param   void
*
* @return  none
*/
void FTM1_IRQHandler(void)
{
    /* Asynchronous time event processing */
    M1_TimeEvent();

    /* Clear flag */
    FTM1->CONTROLS[0].CnSC &= ~(FTM_CnSC_CHF_MASK);

    /* Add empty instructions for correct interrupt flag clearing */
    M1_END_OF_ISR;
    
 
    __DSB();
}

/*!
* @brief   FTM3 reload ISR called with 1ms period and processes following functions:
*           - motor M1 slow application machine function
*
* @param   void
*
* @return  none
*/
void FTM3_IRQHandler(void)
{
    static int16_t ui16i = 0;

    /* Slow StateMachine call */
    SM_StateMachineSlow(&g_sM1Ctrl);

    /* If in STOP state turn on red */
    if (M1_GetAppState() == 2)
    {
        LED_LV3PH_RED_ON();
    }
    /* If in FAULT state red blinking*/
    else if (M1_GetAppState() == 0)
    {
        if (ui16i-- < 0)
        {
            LED_LV3PH_RED_TOGGLE();
            ui16i = 125;
        }
    }
    else
        LED_LV3PH_RED_OFF();

    /* Demo speed stimulator */
    DemoSpeedStimulator();

    /* Clear the TOF flag */
    FTM3->SC &= ~(FTM_SC_TOF_MASK);

    /* Add empty instructions for correct interrupt flag clearing */
    M1_END_OF_ISR;
    
 
    __DSB();
}

/*!
*@brief   PDB0 interrupt service routine
*          - Trigger error clear routine
*
*@param   void
*
*@return  none
*/
void PDB0_IRQHandler(void)
{
    if (PDB0->CH[0].S & PDB_S_ERR_MASK)
    {
        /* Increment trigger error counter */
        if (ui16TriggerErrorCnt < 0xffff)
            ui16TriggerErrorCnt++;

        /* Disable PDB */
        PDB0->SC &= ~(PDB_SC_PDBEN_MASK);
        PDB0->CH[0].S &= ~PDB_S_ERR_MASK;

        /* Enable PDB */
        PDB0->SC |= PDB_SC_PDBEN_MASK;
    }
    
 
    __DSB();
}

/*!
* @brief   Port E interrupt handler
*
* @param   void
*
* @return  none
*/
void PORTE_IRQHandler(void)
{
    if (PORTE->PCR[6] & PORT_PCR_ISF_MASK)
    {
        /* Clear the flag */
        PORTE->PCR[6] |= PORT_PCR_ISF_MASK;

        /* Proceed only if more than 200 ms passed */
        if (ui32ButtonFilter > 200)
        {
            ui32ButtonFilter = 0;
            if (bDemoMode)
            {
                /* Stop application */
                M1_SetAppSwitch(FALSE);
                bDemoMode = FALSE;
            }
            else
            {
                /* Start application */
                M1_SetAppSwitch(TRUE);
                bDemoMode = TRUE;
                ui32SpeedStimulatorCnt = 0;
            }
        }
    }
    
 
    __DSB();
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
void DemoSpeedStimulator(void)
{
    /* Increase if less then one second */
    if (ui32ButtonFilter < 1000)
        ui32ButtonFilter++;

    if (bDemoMode)
    {
        /* Increment speed stimulator counter */
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
    else
    {
        ui32SpeedStimulatorCnt = 0;
    }
}
