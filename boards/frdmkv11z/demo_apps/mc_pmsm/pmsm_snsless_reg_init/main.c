/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "fsl_port.h"
#include "mc_periph_init.h"
#include "freemaster.h"
#include "pin_mux.h"
#include "peripherals.h"
#include "m1_sm_snsless.h"
#include "fsl_uart.h"
#include "fsl_port.h"

#include "freemaster_serial_uart.h"
#include "board.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Version info */
#define MCRSP_VER "2.0.0" /* motor control package version */

/* Macro for correct Cortex CM0 / CM4 end of interrupt */
#define M1_END_OF_ISR \
    {                 \
        __DSB();      \
        __ISB();      \
    }

/* CPU load measurement SysTick START / STOP macros */
#define SYSTICK_START_COUNT() (SysTick->VAL = SysTick->LOAD)
#define SYSTICK_STOP_COUNT(par1)   \
    uint32_t val  = SysTick->VAL;  \
    uint32_t load = SysTick->LOAD; \
    par1          = load - val

static void DemoSpeedStimulator(void);

/* Init HW */
static void BOARD_Init(void);
static void BOARD_InitSysTick(void);
/*******************************************************************************
 * Variables
 ******************************************************************************/
/* CPU load measurement using Systick*/
uint32_t g_ui32NumberOfCycles    = 0;
uint32_t g_ui32MaxNumberOfCycles = 0;

/* Demo mode enabled/disabled */
bool_t bDemoMode = FALSE;

/* Used for demo mode */
static uint32_t ui32SpeedStimulatorCnt = 0;

/* Counter for button pressing */
static uint32_t ui32ButtonFilter = 0;

/* Application and board ID  */
app_ver_t g_sAppId = {
    "frdm-kv11z", /* board id */
    "pmsm",       /* motor type */
    MCRSP_VER,    /* sw version */
};
/* Structure used in FM to get required ID's */
app_ver_t g_sAppIdFM = {
    "", 		  /* board id */
    "",       	  /* motor type */
    MCRSP_VER,    /* sw version */
};

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

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

    /* Disable demo mode after reset */
    bDemoMode              = FALSE;
    ui32SpeedStimulatorCnt = 0;

    /* Pass actual demo id and board info to FM */
    g_sAppIdFM = g_sAppId;

    /* Init board hardware. */
    BOARD_Init();

    /* Initialize peripheral motor control driver for motor M1*/
    MCDRV_Init_M1();

    /* SysTick initialization for CPU load measurement */
    BOARD_InitSysTick();

    /* Turn off application */
    M1_SetAppSwitch(0);

    /* Enable interrupts  */
    EnableGlobalIRQ(ui32PrimaskReg);

    /* Infinite loop */
    while (1)
    {
        /* FreeMASTER Polling function */
        FMSTR_Poll();
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
void ADC0_IRQHandler(void)
{
    /* Disable trigger for PDB */
    PDB0->SC = (PDB0->SC & (~PDB_SC_TRGSEL_MASK)) | PDB_SC_TRGSEL(0xF); /* PDB Trigger off (software trigger) */
    PDB0->SC |= PDB_SC_LDOK_MASK;                                       /* LDOK */

    /* Start CPU tick number couting */
    SYSTICK_START_COUNT();

    /* StateMachine call */
    SM_StateMachineFast(&g_sM1Ctrl);

    /* stop CPU tick number couting and store actual and maximum ticks */
    SYSTICK_STOP_COUNT(g_ui32NumberOfCycles);
    g_ui32MaxNumberOfCycles =
        g_ui32NumberOfCycles > g_ui32MaxNumberOfCycles ? g_ui32NumberOfCycles : g_ui32MaxNumberOfCycles;

    /* Call FreeMASTER recorder */
    FMSTR_Recorder(0);

    /* Add empty instructions for correct interrupt flag clearing */
    M1_END_OF_ISR;
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
    static int16_t ui16i = 0;

    /* Slow StateMachine call */
    SM_StateMachineSlow(&g_sM1Ctrl);

    /* If in STOP state turn on red */
    if (M1_GetAppState() == 2U)
    {
        LED_RED_ON();
        LED_GREEN_OFF();
    }

    /* If in FAULT state red blinking*/
    else if (M1_GetAppState() == 0U)
    {
        if (ui16i-- < 0)
        {
            LED_RED_TOGGLE();
            ui16i = 125;
        }
        LED_GREEN_OFF();
    }

    /* If in RUN or INIT state turn on green */
    else
    {
        LED_RED_OFF();
        LED_GREEN_ON();
    }

    /* Demo speed stimulator */
    DemoSpeedStimulator();

    /* Clear the TOF flag */
    FTM2->SC &= ~(FTM_SC_TOF_MASK);

    /* add instructions for correct interrupt flag clearing */
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
void PDB0_PDB1_IRQHandler(void)
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

    /* Add empty instructions for correct interrupt flag clearing */
    M1_END_OF_ISR;
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

        /* Proceed only if pressing longer than timeout */
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
                bDemoMode              = TRUE;
                ui32SpeedStimulatorCnt = 0;
            }
        }
    }

    /* Add empty instructions for correct interrupt flag clearing */
    M1_END_OF_ISR;
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
    /* Increment push button pressing counter  */
    if (ui32ButtonFilter < 1000)
        ui32ButtonFilter++;

    if (bDemoMode)
    {
        ui32SpeedStimulatorCnt++;
        switch (ui32SpeedStimulatorCnt)
        {
            case 100:
                M1_SetSpeed(FRAC16(1000.0 / M1_N_MAX));
                break;
            case 3000:
                M1_SetSpeed(FRAC16(2000.0 / M1_N_MAX));
                break;
            case 6000:
                M1_SetSpeed(FRAC16(4000.0 / M1_N_MAX));
                break;
            case 9000:
                M1_SetSpeed(FRAC16(900.0 / M1_N_MAX));
                break;
            case 12000:
                M1_SetSpeed(FRAC16(4000.0 / M1_N_MAX));
                break;
            case 15000:
                M1_SetSpeed(FRAC16(2000.0 / M1_N_MAX));
                break;
            case 18000:
                M1_SetSpeed(FRAC16(800.0 / M1_N_MAX));
                ui32SpeedStimulatorCnt = 0;
                break;
            default:
                break;
        }
    }
}

/*!
 * @brief   static void BOARD_Init(void)
 *           - Initialization of clocks, pins and GPIO
 *
 * @param   void
 *
 * @return  none
 */
static void BOARD_Init(void)
{
    /* Initialize clock configuration */
    BOARD_BootClockRUN();
    /* Init pins set in pin_mux file */
    BOARD_InitBootPins();
    /* Init peripherals set in peripherals file */
    BOARD_InitBootPeripherals();
}

/*!
 *@brief      SysTick initialization for CPU cycle measurement
 *
 *@param      none
 *
 *@return     none
 */
static void BOARD_InitSysTick(void)
{
    /* Initialize SysTick core timer to run free */
    /* Set period to maximum value 2^24*/
    SysTick->LOAD = 0xFFFFFF;

    /*Clock source - System Clock*/
    SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;

    /*Start Sys Timer*/
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}
