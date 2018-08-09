/*
 * The Clear BSD License
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 * that the following conditions are met:
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
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
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

#include "freemaster.h"
#include "board.h"
#include "mcdrv.h"
#include "m1_sm_demo.h"
#include "fsl_lpuart.h"
#include "fm_tsa_pmsm.h"
#include "pin_mux.h"

#include "fsl_common.h"
#include "fsl_gpio.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/


/* Three instruction added after interrupt flag clearing as required */
#define M1_END_OF_ISR \
    {                 \
        __DSB(); \
        __ISB(); \
    }

/* CPU load measurement SysTick START / STOP macros */
#define SYSTICK_START_COUNT() (SysTick->VAL = SysTick->LOAD)
#define SYSTICK_STOP_COUNT(par1) uint32_t val = SysTick->VAL; uint32_t load = SysTick->LOAD; par1 = load - val
/*******************************************************************************
 * Variables
 ******************************************************************************/

/* CPU load measurement using Systick*/
uint32_t g_ui32NumberOfCycles = 0;
uint32_t g_ui32MaxNumberOfCycles = 0;

/* Demo mode enabled/disabled */
bool_t bDemoMode = FALSE;

/* Counters used for demo mode */
static uint32_t ui32SpeedStimulatorCnt = 0;

/* Counter for button pressing */
static uint32_t ui32ButtonFilter = 0;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/* Demo Speed Stimulator */
static void DemoSpeedStimulator(void);
static void InitUART(uint32_t);
static void InitSysTick(void);

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
    
    /* Init pins set in pin_mux file */
    BOARD_InitBootPins();
    
    /* Initialize clock configuration */
    BOARD_BootClockRUN();
    
    /* Enable port for USER LED */
    GPIO_PinInit(BOARD_USER_LED_GPIO, BOARD_USER_LED_GPIO_PIN, &output_pin_config); 
    GPIO_PortClear(BOARD_USER_LED_GPIO, 1U << BOARD_USER_LED_GPIO_PIN);
    
    /* Enable GPIO pin interrupt for SW8 button */
    EnableIRQ(BOARD_USER_BUTTON_IRQ);
    GPIO_PinInit(BOARD_USER_BUTTON_GPIO, BOARD_USER_BUTTON_GPIO_PIN, &user_button_config);
    GPIO_PortEnableInterrupts(BOARD_USER_BUTTON_GPIO, 1U << BOARD_USER_BUTTON_GPIO_PIN);
    NVIC_SetPriority(BOARD_USER_BUTTON_IRQ, BOARD_USER_BUTTON_PRIORITY);

    /* Initialize peripheral motor control driver for motor M1*/
    MCDRV_Init_M1();
    
    /* Init UART for FreeMaster communication */ 
    InitUART(BOARD_FMSTR_UART_BAUDRATE);

    /* FreeMaster init */
    FMSTR_Init();

    /* SysTick initialization for CPU load measurement */
    InitSysTick();
    
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
        /* FreeMASTER Polling function */
        FMSTR_Poll();
    }
}

/*!
*@brief      Initialization of the UART module 
*
*@param      u32BaudRate         Baud rate
*            
*@return     none
*/
void InitUART(uint32_t u32BaudRate)
{
    lpuart_config_t config;
    
    LPUART_GetDefaultConfig(&config);
    config.baudRate_Bps = BOARD_FMSTR_UART_BAUDRATE;
    config.enableTx = true;
    config.enableRx = true;

    LPUART_Init(BOARD_FMSTR_UART_PORT, &config, BOARD_UARTSrcFreq());
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
    /* Start CPU tick number couting */
    SYSTICK_START_COUNT();
       
    /* M1 State machine */
    SM_StateMachineFast(&g_sM1Ctrl); 
    
    /* Stop CPU tick number couting and store actual and maximum ticks */
    SYSTICK_STOP_COUNT(g_ui32NumberOfCycles);
    g_ui32MaxNumberOfCycles = g_ui32NumberOfCycles>g_ui32MaxNumberOfCycles ? g_ui32NumberOfCycles : g_ui32MaxNumberOfCycles;
    
    /* Call FreeMASTER recorder */
    FMSTR_Recorder();
      
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
            
            if (bDemoMode)
            {
              /* Stop application */
              M1_SetSpeed(0);
              M1_SetAppSwitch(0);
              bDemoMode = FALSE;
            }
            else
            {
              /* Start application */
              bDemoMode = TRUE;
              ui32SpeedStimulatorCnt = 0;
              M1_SetAppSwitch(1);
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
    /* Increase push button pressing counter  */
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
    else
    {
        ui32SpeedStimulatorCnt = 0;
    }
}
