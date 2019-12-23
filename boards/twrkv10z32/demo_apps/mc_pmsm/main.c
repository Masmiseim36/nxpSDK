/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "board.h"
#include "mcdrv.h"
#include "m1_sm_demo.h"

#include "fsl_common.h"
#include "fsl_port.h"
#include "pin_mux.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/


/* three instruction added after interrupt flag clearing as required */
#define M1_END_OF_ISR \
    {                 \
        __asm("nop"); \
        __asm("nop"); \
        __asm("nop"); \
    }

#define LED_LV3PH_RED_TOGGLE() // for example (GPIOA->PTOR = (1U << 5U))
#define LED_LV3PH_RED_ON() // for example (GPIOA->PSOR = (1U << 5U))
#define LED_LV3PH_RED_OFF() // for example (GPIOA->PCOR = (1U << 5U))

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

    /* Init board hardware. */
    /* Initialize pins configuration */
    BOARD_InitPins();

    /* Initialize clock configuration */ 
    BOARD_BootClockRUN();

    /* SW2 pin configuration */
    PORT_SetPinInterruptConfig(PORTB, 0U, kPORT_InterruptRisingEdge); /* Enable interrupt */

    /* Enable & setup interrupts */
    EnableIRQ(PORTB_PORTC_PORTD_PORTE_IRQn);
    NVIC_SetPriority(PORTB_PORTC_PORTD_PORTE_IRQn, 4);

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
*           - demo mode if enabled
*
* @param   void
*
* @return  none
*/
void ADC1_IRQHandler(void)
{
    /* Disable trigger for PDB */
    PDB0->SC = (PDB0->SC & (~PDB_SC_TRGSEL_MASK)) | PDB_SC_TRGSEL(0xF); /* PDB Trigger off (software trigger) */
    PDB0->SC |= PDB_SC_LDOK_MASK;                                       /* LDOK */

    /* StateMachine call */
    SM_StateMachineFast(&g_sM1Ctrl);
}

/*!
* @brief   FTM2 reload ISR called with 1ms period and processes following functions:
*           - motor M1 slow application machine function
*
* @param   void
*
* @return  none
*/
void FTM2_IRQHandler(void)
{
    static int16_t i = 0;

    /* Slow StateMachine call */
    SM_StateMachineSlow(&g_sM1Ctrl);

    /* If in STOP state turn on red */
    if (M1_GetAppState() == 2)
    {
        LED_LV3PH_RED_OFF(); /*!< Turn off user LED */
    }

    /* If in FAULT state red blinking*/
    else if (M1_GetAppState() == 0)
    {
        if (i-- < 0)
        {
            LED_LV3PH_RED_TOGGLE(); /*!< Toggle user LED */
            i = 125;
        }
    }

    /* If in RUN or INIT state turn on green */
    else
    {
        LED_LV3PH_RED_ON(); /*!< Turn on user LED */
    }

    /* Demo Speed Simulator */
    DemoSpeedStimulator();

    /* Clear the TOF flag */
    FTM2->SC &= ~(FTM_SC_TOF_MASK);

    /* add empty instructions for correct interrupt flag clearing */
    M1_END_OF_ISR;
}

/*!
* @brief   PDB_Error_ISR_Handler
*           - handling the PDB error interrupt: re-initiates the PDB module
*
* @param   void
*
* @return  none
*/
void PDB0_IRQHandler(void)
{
    /* PDB Error interrupt */
    if ((PDB0->CH[0].S & PDB_S_ERR_MASK) || (PDB0->CH[1].S & PDB_S_ERR_MASK))
    {
        PDB0->SC &= (~PDB_SC_PDBEN_MASK);   /* Disable PDB */
        PDB0->CH[0].S &= (~PDB_S_CF_MASK);  /* Clear flag on channel 0 */
        PDB0->CH[0].S &= (~PDB_S_ERR_MASK); /* Clear error on channel 0 */
        PDB0->CH[1].S &= (~PDB_S_CF_MASK);  /* Clear flag on channel 1 */
        PDB0->CH[1].S &= (~PDB_S_ERR_MASK); /* Clear error on channel 1 */
        PDB0->SC |= PDB_SC_PDBEN_MASK;      /* Enable PDB */
    }

    /* PDB delay interrupt */
    else
    {
        PDB0->SC = (PDB0->SC & (~PDB_SC_TRGSEL_MASK)) | PDB_SC_TRGSEL(0x8); /* PDB Trigger on (FTM0 init trigger) */
        PDB0->SC |= PDB_SC_LDOK_MASK;                                       /* LDOK */
        PDB0->SC &= (~PDB_SC_PDBIF_MASK);                                   /* Clear PDB interrupt flag */
    }
}

/*!
* @brief   Port interrupt handler
*
* @param   void
*
* @return  none
*/
void PORTB_PORTC_PORTD_PORTE_IRQHandler(void)
{
    if (PORTB->PCR[0] & PORT_PCR_ISF_MASK)
    {
        /* clear the flag */
        PORTB->PCR[0] |= PORT_PCR_ISF_MASK;

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
            case 3000:
                M1_SetSpeed(FRAC16(2000.0 / M1_N_MAX));
                break;
            case 6000:
                M1_SetSpeed(FRAC16(4000.0 / M1_N_MAX));
                break;
            case 9000:
                M1_SetSpeed(-FRAC16(1000.0 / M1_N_MAX));
                break;
            case 12000:
                M1_SetSpeed(-FRAC16(2000.0 / M1_N_MAX));
                break;
            case 15000:
                M1_SetSpeed(-FRAC16(4000.0 / M1_N_MAX));
                break;
            case 18000:
                ui32SpeedStimulatorCnt = 0;
                break;
            default:
                break;
        }
    }
}
