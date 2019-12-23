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

/* Demo mode enabled/disabled */
bool_t bDemoMode = FALSE;

/* Used for demo mode */
static uint32_t ui32SpeedStimulatorCnt = 0;

/* Counter for button pressing */
static uint32_t ui32ButtonFilter = 0;

/* Application and board ID  */
app_ver_t   g_sAppId = {
    "hvp-kv46f",        /* board id */
    "bldc",             /* motor type */
    MCRSP_VER,          /* sw version */
};

/* Structure used in FM to get required ID's */
app_ver_t   g_sAppIdFM;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void BOARD_Init(void);
void BOARD_InitGPIO(void);
void InitSysTick(void);
void BOARD_InitUART(uint32_t u32UClockSpeedinHz, uint32_t u32BaudRate);
void DemoSpeedStimulator(void);

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
    BOARD_Init();

    /* Turn on the HVP inrush relay for 1s */
    InitRelay();

    /* Initialize peripheral motor control driver for motor M1*/
    MCDRV_Init_M1();

    /* Init UART for FreeMaster communication */ 
    BOARD_InitUART(g_sClockSetup.ui32FastPeripheralClock, BOARD_FMSTR_UART_BAUDRATE);

    /* FreeMaster init */
    FMSTR_Init();
    
    /* SysTick initialization for CPU load measurement */
    InitSysTick();

    /* Turn off application */
    M1_SetAppSwitch(0);

    /* Enable demo mode after reset */
    bDemoMode = FALSE;
    ui32SpeedStimulatorCnt = 0;

    /* Pass actual demo id and board info to FM */
    g_sAppIdFM = g_sAppId;

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
    /* State machine */
    SM_StateMachineFast(&g_sM1Ctrl);

    /* Call FreeMASTER recorder */
    FMSTR_Recorder(0);

    /* Clear the EOS flag */
    ADC->STAT |= ADC_STAT_EOSI0_MASK;

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
    static int16_t i = 0;

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
        if (i-- < 0)
        {
            LED_GREEN_TOGGLE();
            i = 125;
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
    FTM3->SC &= ~(FTM_SC_TOF_MASK);

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
void DemoSpeedStimulator(void)
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

void BOARD_Init(void)
{
    /* Initialize clock configuration */
    BOARD_BootClockHSRUN();  
    
    /* Init pins set in pin_mux file */
    BOARD_InitBootPins();
    
    /* Init GPIO pins */
    BOARD_InitGPIO();     
}

/*!
*@brief      Initialization of the GPIO pins
*
*@param      none
*            
*@return     none
*/
void BOARD_InitGPIO(void)
{
    /* LED pin configuration */
    const gpio_pin_config_t led_config = {
        kGPIO_DigitalOutput, /* Set current pin as digital output */
        (uint8_t)0U          /* Set default logic high */
    };

    /* Init XBARA */
    XBARA_Init(XBARA);

    /* enable port for HVP GREEN LED */
    GPIO_PinInit(GPIOB, 19U, &led_config);

    /* enable port for Braking resistor */
    GPIO_PinInit(GPIOC, 16U, &led_config);

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
void InitSysTick(void)
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