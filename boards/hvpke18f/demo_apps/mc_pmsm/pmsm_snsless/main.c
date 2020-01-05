/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "fsl_port.h"
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

/* Demo mode enabled/disabled */
bool_t bDemoModeSpeed = FALSE;

/* Used for demo mode */
static uint32_t ui32SpeedStimulatorCnt = 0;

/* Counter for button pressing */
static uint32_t ui32ButtonFilter = 0;

/* Application and board ID  */
app_ver_t   g_sAppId = {
    "hvp-ke18",    /* board id */
    "pmsm",             /* motor type */
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
    bDemoModeSpeed = FALSE;
    ui32SpeedStimulatorCnt = 0;
            
    /* Pass actual demo id and board info to FM */
    g_sAppIdFM = g_sAppId;

    /* Init board hardware. */
    BOARD_Init();

    /* Turn on the HVP inrush relay for 1s */
    InitRelay();
    
    /* Initialize peripheral motor control driver for motor M1*/
    MCDRV_Init_M1();
        
    /* Init UART for FreeMaster communication */ 
    BOARD_InitUART(g_sClockSetup.ui32AsynClock, BOARD_FMSTR_UART_BAUDRATE);
    
    /* SysTick initialization for CPU load measurement */
    BOARD_InitSysTick();
    
    /* FreeMaster init */
    FMSTR_Init();

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
    /* Start CPU tick number couting */
    SYSTICK_START_COUNT();

    /* StateMachine call */
    SM_StateMachineFast(&g_sM1Ctrl);

    /* stop CPU tick number couting and store actual and maximum ticks */
    SYSTICK_STOP_COUNT(g_ui32NumberOfCycles);
    g_ui32MaxNumberOfCycles = g_ui32NumberOfCycles>g_ui32MaxNumberOfCycles ? g_ui32NumberOfCycles : g_ui32MaxNumberOfCycles;

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

    /* If in STOP state turn off green */
    if (M1_GetAppState() == 2)
    {
        LED_GREEN_OFF();
    }

    /* If in FAULT state green blinking*/
    else if (M1_GetAppState() == 0)
    {
        if (ui16i-- < 0)
        {
            LED_GREEN_TOGGLE();
            ui16i = 125;
        }
    }
    /* If in RUN or INIT state turn on green */
    else
    {
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
*           - handling the PDB error interrupt: reinitializes the PDB module
*
* @param   void
*
* @return  none
*/
void PDB0_IRQHandler(void)
{
    /* PDB Error interrupt */
    if ((PDB0->CH[0].S & PDB_S_ERR_MASK))
    {
        PDB0->SC &= (~PDB_SC_PDBEN_MASK);   /* Disable PDB */
        PDB0->CH[0].S &= (~PDB_S_CF_MASK);  /* Clear flag on channel 0 */
        PDB0->CH[0].S &= (~PDB_S_ERR_MASK); /* Clear error on channel 0 */
        PDB0->SC |= PDB_SC_PDBEN_MASK;      /* Enable PDB */
    }

    /* PDB delay interrupt */
    else
    {
        PDB0->SC = (PDB0->SC & (~PDB_SC_TRGSEL_MASK)) | PDB_SC_TRGSEL(0x0); /* PDB Trigger on (FTM0 init trigger) */
        PDB0->SC |= PDB_SC_LDOK_MASK;                                       /* LDOK */
        PDB0->SC &= (~PDB_SC_PDBIF_MASK);                                   /* Clear PDB interrupt flag */
        
        PDB1->SC = (PDB1->SC & (~PDB_SC_TRGSEL_MASK)) | PDB_SC_TRGSEL(0x0); /* PDB Trigger on (FTM0 init trigger) */
        PDB1->SC |= PDB_SC_LDOK_MASK;                                       /* LDOK */
        PDB1->SC &= (~PDB_SC_PDBIF_MASK);                                   /* Clear PDB interrupt flag */
    }
}

/*!
* @brief   PDB_Error_ISR_Handler
*           - PDB trigger error clear
*
* @param   void
*
* @return  none
*/
void PDB1_IRQHandler(void)
{
    /* PDB Error interrupt */
    if ((PDB1->CH[0].S & PDB_S_ERR_MASK))
    {
        PDB1->SC &= (~PDB_SC_PDBEN_MASK);   /* Disable PDB */
        PDB1->CH[0].S &= (~PDB_S_CF_MASK);  /* Clear flag on channel 0 */
        PDB1->CH[0].S &= (~PDB_S_ERR_MASK); /* Clear error on channel 0 */
        PDB1->SC |= PDB_SC_PDBEN_MASK;      /* Enable PDB */
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
void DemoSpeedStimulator(void)
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
                break;
        }
    }
}

void BOARD_Init(void)
{
    /* LED pin configuration */
    const gpio_pin_config_t led_config = {
        kGPIO_DigitalOutput, /* Set current pin as digital output */
        (uint8_t)0U          /* Set default logic high */
    };  
  
    /* Initialize clock configuration */
    BOARD_BootClockHSRUN();  
    /* Init pins set in pin_mux file */
    BOARD_InitBootPins();
    
    /* enable port for HVP GREEN LED */
    GPIO_PinInit(GPIOE, 6U, &led_config);

    /* enable port for Braking resistor */
    GPIO_PinInit(GPIOE, 8U, &led_config);

    /* enable port for Inrush relay */
    GPIO_PinInit(GPIOE, 2U, &led_config);
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
    lpuart_config_t config;

    /* Clock setting for LPUART2 */
    PCC->CLKCFG[PCC_LPUART2_INDEX] = PCC_CLKCFG_PCS(6);
    PCC->CLKCFG[PCC_LPUART2_INDEX] |= PCC_CLKCFG_CGC(1); 
    
    /*
     * config.baudRate_Bps = 115200U;
     * config.parityMode = kUART_ParityDisabled;
     * config.stopBitCount = kUART_OneStopBit;
     * config.txFifoWatermark = 0;
     * config.rxFifoWatermark = 1;
     * config.enableTx = false;
     * config.enableRx = false;
     */
    LPUART_GetDefaultConfig(&config);
    config.baudRate_Bps = BOARD_FMSTR_UART_BAUDRATE;
    config.enableTx = true;
    config.enableRx = true;

    LPUART_Init(BOARD_FMSTR_UART_PORT, &config, u32UClockSpeedinHz);
    
    /* Register communication module used by FreeMASTER driver. */
    FMSTR_SerialSetBaseAddress(BOARD_FMSTR_UART_PORT);

    #if FMSTR_SHORT_INTR || FMSTR_LONG_INTR
        /* Enable UART interrupts. */
        EnableIRQ(BOARD_UART_IRQ);
        EnableGlobalIRQ(0);
    #endif
}