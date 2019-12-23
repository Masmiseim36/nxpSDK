/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "main.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* CPU load measurement using Systick*/
uint32_t g_ui32NumberOfCycles = 0;
uint32_t g_ui32MaxNumberOfCycles = 0;

/* Counter of unsuccessful PDB triggers */
volatile uint16_t ui16TriggerErrorCnt;

/* Demo mode enabled/disabled */
bool_t bDemoMode = FALSE;

/* Used for demo mode */
static uint32_t ui32SpeedStimulatorCnt = 0;

/* Counter for button pressing */
static uint32_t ui32ButtonFilter = 0;

/* Application and board ID  */
app_ver_t   g_sAppId = {
    "twr-kv46f",        /* board id */
    "bldc",             /* motor type */
    MCRSP_VER,          /* sw version */
};

/* Structure used in FM to get required ID's */
app_ver_t   g_sAppIdFM;

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
    bDemoMode = FALSE;
    ui32SpeedStimulatorCnt = 0;

    /* Pass actual demo id and board info to FM */
    g_sAppIdFM = g_sAppId;
    
    /* Init board hardware. */
    BOARD_Init();

    /* Initialize peripheral motor control driver for motor M1*/
    MCDRV_Init_M1();

    /* Init UART for FreeMaster communication */
    BOARD_InitUART(g_sClockSetup.ui32FastPeripheralClock, BOARD_FMSTR_UART_BAUDRATE);

    /* SysTick initialization for CPU load measurement */
    BOARD_InitSysTick();
    
    /* FreeMaster init */
    FMSTR_Init();

    /* Turn off application */
    M1_SetAppSwitch(0);

    /* Enable interrupts */
    EnableGlobalIRQ(ui32PrimaskReg);

    /* Infinite loop */
    while (1)
    {
        FMSTR_Poll();
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
    /* Start CPU tick number couting */
    SYSTICK_START_COUNT();
    
    /* State machine */
    SM_StateMachineFast(&g_sM1Ctrl);

    /* stop CPU tick number couting and store actual and maximum ticks */
    SYSTICK_STOP_COUNT(g_ui32NumberOfCycles);
    g_ui32MaxNumberOfCycles = g_ui32NumberOfCycles>g_ui32MaxNumberOfCycles ? g_ui32NumberOfCycles : g_ui32MaxNumberOfCycles;

    /* Call FreeMASTER recorder */
    FMSTR_Recorder(0);

    /* Clear the EOS flag */
    ADC->STAT |= (ADC_STAT_EOSI1_MASK | ADC_STAT_EOSI0_MASK);
	
	/* Add empty instructions for correct interrupt flag clearing */
    M1_END_OF_ISR;
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
void DemoSpeedStimulator(void)
{
    /* Increase push button pressing counter  */
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

/*!
*@brief      Initialization of the Clocks and Pins
*
*@param      none
*            
*@return     none
*/
void BOARD_Init(void)
{
    /* Initialize pins configuration */
    BOARD_InitBootPins();
    /* Initialize clock configuration */
    BOARD_BootClockHSRUN();
    /* Enable & setup interrupts */
    EnableIRQ(PORTE_IRQn);
    NVIC_SetPriority(PORTE_IRQn, 4);
}

/*!
*@brief      Initialization of the UART module 
*
*@param      u32UClockSpeedinHz  UART module input clock in Hz
*            u32BaudRate         Baud rate
*            
*@return     none
*/
void BOARD_InitUART(uint32_t u32UClockSpeedinHz, uint32_t u32BaudRate)
{
    uart_config_t config;

    /*
     * config.baudRate_Bps = 115200U;
     * config.parityMode = kUART_ParityDisabled;
     * config.stopBitCount = kUART_OneStopBit;
     * config.txFifoWatermark = 0;
     * config.rxFifoWatermark = 1;
     * config.enableTx = false;
     * config.enableRx = false;
     */
    UART_GetDefaultConfig(&config);
    config.baudRate_Bps = BOARD_FMSTR_UART_BAUDRATE;
    config.enableTx = true;
    config.enableRx = true;

    UART_Init(BOARD_FMSTR_UART_PORT, &config, u32UClockSpeedinHz);
        
    /* Register communication module used by FreeMASTER driver. */
    FMSTR_SerialSetBaseAddress(BOARD_FMSTR_UART_PORT);

    #if FMSTR_SHORT_INTR || FMSTR_LONG_INTR
        /* Enable UART interrupts. */
        EnableIRQ(BOARD_UART_IRQ);
        EnableGlobalIRQ(0);
    #endif   
}

/*!
*@brief      SysTick initialization for CPU cycle measurement
*
*@param      none
*            
*@return     none
*/
void BOARD_InitSysTick(void)
{
    /* Initialize SysTick core timer to run free */
    /* Set period to maximum value 2^24*/
    SysTick->LOAD = 0xFFFFFF;
    
    /*Clock source - System Clock*/
    SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;
    
    /*Start Sys Timer*/
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}