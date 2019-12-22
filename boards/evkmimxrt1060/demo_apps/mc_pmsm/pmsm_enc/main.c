/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
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
bool_t bDemoModePosition = FALSE;

/* Counters used for demo mode */
static uint32_t ui32SpeedStimulatorCnt = 0;
static uint32_t ui32PositionStimulatorCnt = 0;

/* Counter for button pressing */
static uint32_t ui32ButtonFilter = 0;

/* Application and board ID  */
app_ver_t   g_sAppId = {
    "evk-imxrt1060",    /* board id */
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
 * @brief Main function
 */
int main(void)
{
    uint32_t ui32PrimaskReg;

    /* Disable all interrupts before peripherals are initialized */
    ui32PrimaskReg = DisableGlobalIRQ();
    
    /* Disable demo mode after reset */
    bDemoModeSpeed = FALSE;
    bDemoModePosition = FALSE;
    
    /* Pass actual demo id and board info to FM */
    g_sAppIdFM = g_sAppId;

    /* Init board hardware. */
    BOARD_Init();
    
    /* Init UART for FreeMaster communication */ 
    BOARD_InitUART(BOARD_FMSTR_UART_BAUDRATE);
    
    /* SysTick initialization for CPU load measurement */
    BOARD_InitSysTick();
    
    /* Init peripheral motor control driver for motor M1 */
    MCDRV_Init_M1();
      
    /* FreeMaster init */
    FMSTR_Init();
     
    /* Turn off application */
    M1_SetAppSwitch(0);
     
    /* Enable interrupts */
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
*
* @param   void
*
* @return  none
*/
void ADC1_IRQHandler(void)
{
    /* Start CPU tick number couting */
    SYSTICK_START_COUNT();
       
    /* M1 State machine */
    SM_StateMachineFast(&g_sM1Ctrl); 
    
    /* Stop CPU tick number couting and store actual and maximum ticks */
    SYSTICK_STOP_COUNT(g_ui32NumberOfCycles);
    g_ui32MaxNumberOfCycles = g_ui32NumberOfCycles>g_ui32MaxNumberOfCycles ? g_ui32NumberOfCycles : g_ui32MaxNumberOfCycles;
    
    /* Call FreeMASTER recorder */
    FMSTR_Recorder(0);
      
    /* Add empty instructions for correct interrupt flag clearing */
    M1_END_OF_ISR;
}

/*!
* @brief   TMR1 reload ISR called with 1ms period and processes following functions:
*           - motor M1 slow application machine function
*
* @param   void
*
* @return  none
*/
void TMR1_IRQHandler(void)
{   
    static int16_t ui16i = 0;
  
    /* M1 Slow StateMachine call */
    SM_StateMachineSlow(&g_sM1Ctrl);
    
    /* If in STOP state turn on LED */
    if (M1_GetAppState() == 2)
    {
        USER_LED_OFF();
    }

    /* If in FAULT state LED blinking*/
    else if (M1_GetAppState() == 0)
    {
        bDemoModeSpeed = FALSE;
        bDemoModePosition = FALSE;
        
        if (ui16i-- < 0)
        {
            USER_LED_TOGGLE();
            ui16i = 125;
        }
    }

    /* If in RUN or INIT state turn on LED */
    else
    {
        USER_LED_ON();
    }
    
    /* Demo speed stimulator */
    DemoSpeedStimulator();
    
    /* Demo position stimulator */
    DemoPositionStimulator();
                   
    /* Clear the CSCTRL0[TCF1] flag */  
    TMR1->CHANNEL[0].CSCTRL |= TMR_CSCTRL_TCF1(0x00);
    TMR1->CHANNEL[0].CSCTRL  &= ~(TMR_CSCTRL_TCF1_MASK);
    
    /* Clear the CSCTRL0[TCF] flag */
    TMR1->CHANNEL[0].SCTRL  &= ~(TMR_SCTRL_TCF_MASK); 
    
    /* Add empty instructions for correct interrupt flag clearing */
    M1_END_OF_ISR;
}

/*!
* @brief   SW8 Button interrupt handler
*
* @param   void
*
* @return  none
*/
void GPIO5_Combined_0_15_IRQHandler(void)
{
    if (GPIO5->ISR & GPIO_ICR1_ICR0_MASK)
    {
        /* Clear the flag */
        GPIO5->ISR |= GPIO_ISR_ISR(1);

        /* Proceed only if pressing longer than timeout */
        if (ui32ButtonFilter > 300)
        {
            ui32ButtonFilter = 0;
            
            /* Speed demo */
            if (bDemoModeSpeed)
            {
                /* Stop application */
                M1_SetSpeed(0);
                M1_SetAppSwitch(0);
                bDemoModeSpeed = FALSE;
            }
            else
            {
                /* Start application */
                M1_SetAppSwitch(1);
                bDemoModeSpeed = TRUE;
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
void DemoSpeedStimulator(void)
{
    /* Increase push button pressing counter  */
    if (ui32ButtonFilter < 1000)
        ui32ButtonFilter++;
    
    if (bDemoModeSpeed)
    {
        ui32SpeedStimulatorCnt++;
        switch (ui32SpeedStimulatorCnt)
        {
            case 10:
                M1_SetAppSwitch(0);
                break;
            case 20:
                g_sM1Drive.eControl = kControlMode_SpeedFOC;
                g_sM1Drive.sMCATctrl.ui16PospeSensor = MCAT_SENSORLESS_CTRL;
                M1_SetAppSwitch(1);
                break;    
            case 1000:
                M1_SetSpeed(1000.0F);
                break;
            case 5000:
                M1_SetSpeed(2000.0F);
                break;
            case 10000:
                M1_SetSpeed(-1000.0F);
                break;
            case 15000:
                M1_SetSpeed(-2000.0F);
                break;
            case 19800:
                M1_SetSpeed(0.0F);
                break;
            case 20000:
                ui32SpeedStimulatorCnt = 0;
                M1_SetAppSwitch(0);
                break;
            default:
                break; 
        }
    }
}

/*!
* @brief   DemoPositionStimulator
*           - When demo mode is enabled it changes the required position according
*             to predefined profile
*
* @param   void
*
* @return  none
*/
void DemoPositionStimulator(void)
{
    if (bDemoModePosition)
    {
        ui32PositionStimulatorCnt++;
        switch (ui32PositionStimulatorCnt)
        {
            case 1:
                M1_SetAppSwitch(0);
                break;
            case 2:
                g_sM1Drive.eControl = kControlMode_PositionFOC;
                g_sM1Drive.sMCATctrl.ui16PospeSensor = MCAT_ENC_CTRL;
                M1_SetAppSwitch(1);
                break;
            case 10:
                M1_SetPosition(ACC32(20));
                break;                   
            case 3000:              
                M1_SetPosition(ACC32(10));
                break;              
            case 6000:
                M1_SetPosition(ACC32(20));
                break;            
            case 9000:
                M1_SetPosition(ACC32(0));
                break;            
            case 12000:
                M1_SetPosition(ACC32(20));
                break;             
            case 15000:
                M1_SetPosition(ACC32(0));
                break;              
            case 18000:
                ui32PositionStimulatorCnt = 0; 
                M1_SetAppSwitch(0);
                break;
            default:
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
void BOARD_Init(void)
{
    /* Init board hardware. */
    BOARD_InitBootPins();
    /* Initialize clock configuration */
    BOARD_InitBootClocks();
    /* Init GPIO pins */
    BOARD_InitGPIO();
}

/*!
* @brief   void BOARD_InitGPIO(void)
*           - Initialization of the GPIO peripherals
*
* @param   void
*
* @return  none
*/
void BOARD_InitGPIO(void)
{
   /* LED pin configuration */
    const gpio_pin_config_t output_pin_config = {
        kGPIO_DigitalOutput, /* Set current pin as digital output */
        (uint8_t)1U          /* Set default logic high */
    }; 
    
    /* SW8 Button configuration */
    const gpio_pin_config_t user_button_config = {
        kGPIO_DigitalInput,     /* Set pin as digital input */
        0,                      /* Set default output logic, which has no use in input  */
        kGPIO_IntFallingEdge    /* Set pin interrupt mode: falling-edge sensitive */
    };
    
    /* Enable port for USER LED */
    GPIO_PinInit(BOARD_USER_LED_GPIO, BOARD_USER_LED_GPIO_PIN, &output_pin_config); 
    GPIO_PortClear(BOARD_USER_LED_GPIO, 1U << BOARD_USER_LED_GPIO_PIN);
    
    /* Enable GPIO pin interrupt for SW8 button */
    EnableIRQ(BOARD_USER_BUTTON_IRQ);
    GPIO_PinInit(BOARD_USER_BUTTON_GPIO, BOARD_USER_BUTTON_GPIO_PIN, &user_button_config);
    GPIO_PortEnableInterrupts(BOARD_USER_BUTTON_GPIO, 1U << BOARD_USER_BUTTON_GPIO_PIN);
    NVIC_SetPriority(BOARD_USER_BUTTON_IRQ, BOARD_USER_BUTTON_PRIORITY);
}

 
/*!
*@brief      Initialization of the UART module 
*
*@param      u32BaudRate         Baud rate
*            
*@return     none
*/
void BOARD_InitUART(uint32_t u32BaudRate)
{
    lpuart_config_t config;
    
    LPUART_GetDefaultConfig(&config);
    config.baudRate_Bps = BOARD_FMSTR_UART_BAUDRATE;
    config.enableTx = true;
    config.enableRx = true;

    LPUART_Init(BOARD_FMSTR_UART_PORT, &config, BOARD_DebugConsoleSrcFreq());
    
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
