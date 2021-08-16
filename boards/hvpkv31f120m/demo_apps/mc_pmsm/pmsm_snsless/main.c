/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "freemaster.h"
#include "pin_mux.h"
#include "peripherals.h"
#include "fsl_port.h"
#include "board.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Version info */
#define MCRSP_VER "2.0.0" /* motor control package version */

#define HVP_BOARD 1

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
static void BOARD_InitGPIO(void);
static void BOARD_InitSysTick(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* CPU load measurement using Systick */
uint32_t g_ui32NumberOfCycles    = 0U;
uint32_t g_ui32MaxNumberOfCycles = 0U;

/* Demo mode enabled/disabled */
bool_t bDemoModeSpeed = FALSE;

/* Used for demo mode */
static uint32_t ui32SpeedStimulatorCnt    = 0U;

/* Counter for button pressing */
static uint32_t ui32ButtonFilter = 0U;

/* Application and board ID  */
app_ver_t g_sAppId = {
    "hvp-kv31", /* board id */
    "pmsm",     /* motor type */
    MCRSP_VER,  /* sw version */
};

/* Structure used in FM to get required ID's */
app_ver_t g_sAppIdFM = {
	"",
	"",
	MCRSP_VER,
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
    /* Disable demo mode after reset */
    bDemoModeSpeed         = FALSE;
    ui32SpeedStimulatorCnt = 0U;

    /* Pass actual demo id and board info to FM */
    g_sAppIdFM = g_sAppId;

    /* Init board hardware. */
    BOARD_Init();

    /* SysTick initialization for CPU load measurement */
    BOARD_InitSysTick();

    /* Turn off application */
    M1_SetAppSwitch(FALSE);

    /* Infinite loop */
    while (1)
    {
        /* FreeMASTER Polling function */
        FMSTR_Poll();
    }
}

/* ADC1_IRQn interrupt handler */
void FAST_LOOP_IRQHANDLER(void) {
    /* ADC conversion complete (fast loop period) interrupt.
    * Processes motor M1 fast application machine function.
    * Demo mode if enabled. */

    /* Disable trigger for PDB */
    PDB0->SC = (PDB0->SC & (~PDB_SC_TRGSEL_MASK)) | PDB_SC_TRGSEL(0xF); /* PDB Trigger off (software trigger) */
    PDB0->SC |= PDB_SC_LDOK_MASK;  /* LDOK */

    /* Start CPU tick number couting */
    SYSTICK_START_COUNT();

    /* State machine */
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

/* FTM2_IRQn interrupt handler */
void SLOW_LOOP_IRQHANDLER(void) {
    /* Slow Loop periodic interrupt period.
    * Processes motor M1 slow application machine function. */

    /* Demo speed stimulator */
    DemoSpeedStimulator();

    /* Slow StateMachine call */
    SM_StateMachineSlow(&g_sM1Ctrl);

    /* Clear the TOF flag */
    FTM2->SC &= ~(FTM_SC_TOF_MASK);

    /* Add instructions for correct interrupt flag clearing */
    M1_END_OF_ISR;

}

/* PDB0_IRQn interrupt handler */
void PDB_ERROR_IRQHANDLER(void) {
    /* Handling the PDB error interrupt.
    * Reinitializes the PDB module */

    /* PDB Error interrupt */
    if ((PDB0->CH[0].S & PDB_S_ERR_MASK) || (PDB0->CH[1].S & PDB_S_ERR_MASK)) {
        PDB0->SC &= (~PDB_SC_PDBEN_MASK);   /* Disable PDB */
        PDB0->CH[0].S &= (~PDB_S_CF_MASK);  /* Clear flag on channel 0 */
        PDB0->CH[0].S &= (~PDB_S_ERR_MASK); /* Clear error on channel 0 */
        PDB0->CH[1].S &= (~PDB_S_CF_MASK);  /* Clear flag on channel 1 */
        PDB0->CH[1].S &= (~PDB_S_ERR_MASK); /* Clear error on channel 1 */
        PDB0->SC |= PDB_SC_PDBEN_MASK;      /* Enable PDB */
    }
    /* PDB delay interrupt */
    else{
        PDB0->SC = (PDB0->SC & (~PDB_SC_TRGSEL_MASK)) | PDB_SC_TRGSEL(0x8); /* PDB Trigger on (FTM2 init trigger) */
        PDB0->SC |= PDB_SC_LDOK_MASK;  /* LDOK */
        PDB0->SC &= (~PDB_SC_PDBIF_MASK);  /* Clear PDB interrupt flag */
    }

    /* Add instructions for correct interrupt flag clearing */
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

    if (bDemoModeSpeed)
    {
        ui32SpeedStimulatorCnt++;
        switch (ui32SpeedStimulatorCnt)
        {
            case 100:
                M1_SetSpeed(1000.0F);
                break;
            case 3000:
                M1_SetSpeed(2000.0F);
                break;
            case 6000:
                M1_SetSpeed(4000.0F);
                break;
            case 9000:
                M1_SetSpeed(-1000.0F);
                break;
            case 12000:
                M1_SetSpeed(-2000.0F);
                break;
            case 15000:
                M1_SetSpeed(-4000.0F);
                break;
            case 18000:
                ui32SpeedStimulatorCnt = 0;
                break;
            default:
            	;
                break;
        }
    }
}

/*!
 *@brief      Initialization of the Clocks and Pins
 *
 *@param      none
 *
 *@return     none
 */
static void BOARD_Init(void)
{
    /* Initialize clock configuration */
    BOARD_BootClockHSRUN();
    /* Init pins set in pin_mux file */
    BOARD_InitBootPins();

    /* Init GPIO pins */
    BOARD_InitGPIO();

    /* Init peripherals set in peripherals file */
    BOARD_InitBootPeripherals();
}

/*!
 *@brief      Initialization of the GPIO pins
 *
 *@param      none
 *
 *@return     none
 */
static void BOARD_InitGPIO(void)
{
    /* LED pin configuration */
    const gpio_pin_config_t led_config = {
        kGPIO_DigitalOutput, /* Set current pin as digital output */
        (uint8_t)0U          /* Set default logic high */
    };

    /* enable port for HVP GREEN LED */
    GPIO_PinInit(GPIOB, 19U, &led_config);

    /* enable port for Braking resistor */
    GPIO_PinInit(GPIOC, 0U, &led_config);

    /* enable port for Inrush relay */
    GPIO_PinInit(GPIOC, 13U, &led_config);
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

