/*
 * Copyright 2021 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "project_setup_evkmimx8mn.h"
#include "board.h"
#include "fsl_gpio.h"
//#include "freemaster.h"
//#include "freemaster_serial.h"
//#include "freemaster_serial_lpuart.h"

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
* @brief   Watchdog configuration function
*
*          Enables the watchdog. Also in Wait and Stop mode. Updates are allowed
*
* @param   timeout
* @param   window
* @param   prescaler
*
* @return  None
*/
void WatchdogEnable(void)   
{
    /* Memory protection unit initialization. */
    BOARD_InitMemory();

    /* Allow WDOG3 reset signal to reset only core m7. */
    SRC->M7RCR &= (~SRC_M7RCR_WDOG3_RST_OPTION_MASK | ~SRC_M7RCR_WDOG3_RST_OPTION_M7_MASK);

    /* Wdog clock enable. */
    CLOCK_EnableClock(kCLOCK_Wdog3);

    /* Wdog initialization.*/
    USED_WDOG->WICR = WDOG_WICR_WIE(0); /* Interrupt disabled. */
    USED_WDOG->WMCR = WDOG_WMCR_PDE(0); /* Power-down counter disabled. */
    USED_WDOG->WCR  = WDOG_WCR_WDE(1U) | WDOG_WCR_WDW(0) | /* Wdog enabled, wdog in low-power wait mode enabled. */
                      WDOG_WCR_WDZST(0) | WDOG_WCR_WDBG(1U) | /* Wdog in low-power modes enabled, wdog pause in debug enabled. */
                      WDOG_WCR_SRE(1U) | WDOG_WCR_WT(0) | /* SW reset extension set, timeout 0.5s. */
                      WDOG_WCR_WDT(0) | WDOG_WCR_SRS_MASK | WDOG_WCR_WDA_MASK; /* Disable timeout assert. */
}  
     
/*!
 * @brief   Watchdog disabling function
 * 
 * @param   void
 *
 * @return  None
 */
void WatchdogDisable(void)
{  
    /* Watchdog is disabled by default. */
}   

/*!
 * @brief   Initialization of GPT1 timer
 *
 *          Firstly used for watchdog test, then for clock test
 *
 * @param   clkSource - 
 * @param   compare   - 32bit value
 * @param   prescaler - valid range 1-4096
 *
 * @return  None
 */
void GPT1_Init(uint32_t clkSource, uint32_t compare, uint32_t prescaler)
{
    /* Gpt clock enable */  
    CCM->CCGR[16].CCGR = 0x3333U; /* Clock always enabled */

    /* Reset registers */
    GPT1->CR = GPT_CR_SWR(1U);
    
    /* Wait enable, stop enable, counter is reset when timer is disabled */
    GPT1->CR |= GPT_CR_WAITEN_MASK | GPT_CR_STOPEN_MASK | GPT_CR_ENMOD_MASK;
    
    /* Clock source and prescaler */
    if (clkSource == GPT_SRC_24M)
    {
        GPT1->CR |= GPT_CR_CLKSRC(clkSource) | GPT_CR_EN_24M_MASK; /* Set 24MHz clock source */
    }
    else
    {
        GPT1->CR |= GPT_CR_CLKSRC(clkSource); /* Set clock source */
    }
  
    if (prescaler > 0 && prescaler <= GPT_PR_PRESCALER_MASK + 1)
    {
        GPT1->PR = GPT_PR_PRESCALER(prescaler - 1);
    }
    
    GPT1->OCR[0] = GPT_OCR_COMP(compare); /* Compare value */
    
    /* Choose whether generates interrupt or not */
    //GPT1->IR = GPT_IR_OF1IE_MASK; /* Output compare 1 interrupt enable */    
    //NVIC_EnableIRQ(GPT1_IRQn);    /* Enable interrupt */
    
    GPT1->CR |= GPT_CR_EN_MASK; /* Enable GPT */ 
}

/*!
 * @brief   Initialization of Systick timer
 *
 *          This function configures the Systick as a source of interrupt
 *
 * @param   compare - defines the period of counter refresh
 *
 * @return  None
 */
void SystickInitialisation(uint32_t compare)
{
    SysTick->VAL = 0;
    SysTick->LOAD = compare;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk;
}

/*!
 * @brief   Setup of Clock
 *
 * @return  None
 */
void ClockInit(void)
{
    /* SDK Initialization */
    BOARD_BootClockRUN();
    
    CLOCK_SetRootMux(kCLOCK_RootGpt1, kCLOCK_GptRootmuxSysPll1Div2); /* Set GPT1 source to SYSTEM PLL1 DIV2 400MHZ */
    CLOCK_SetRootDivider(kCLOCK_RootGpt1, 1U, 4U);                   /* Set root clock to 400MHZ / 4 = 100MHZ */
}

/*!
 * @brief   Setup of LPUART
 *       
 * @param   Uart_X  - peripheral base address
 * @param   baudRate  - desired baud rate
 * @param   clockFreq - information about selected input clock frequency
 *
 * @return  None
 */
void SerialInit(UART_Type *Uart_X, uint32_t baudRate, uint32_t clockFreq)
{
//    const lpuart_config_t LPUART_1_config =
//    {
//      .baudRate_Bps = baudRate,
//      .parityMode = kLPUART_ParityDisabled,
//      .dataBitsCount = kLPUART_EightDataBits,
//      .isMsb = false,
//      .stopBitCount = kLPUART_OneStopBit,
//      .txFifoWatermark = 0,
//      .rxFifoWatermark = 0,
//      .enableRxRTS = false,
//      .enableTxCTS = false,
//      .txCtsSource = kLPUART_CtsSourcePin,
//      .txCtsConfig = kLPUART_CtsSampleAtStart,
//      .rxIdleType = kLPUART_IdleTypeStartBit,
//      .rxIdleConfig = kLPUART_IdleCharacter1,
//      .enableTx = true,
//      .enableRx = true
//    };
//    
//    LPUART_Init(Uart_X, &LPUART_1_config, clockFreq);
//
//    FMSTR_SerialSetBaseAddress(APPLICATION_SERIAL_BASE);
}

/*!
* @brief  Sets port direction and mux
*
* @note   How to determine pin GPIOx + pin number from schematic? E.g. GPIO_AD_B1_07 from schematic could 
*         be find in ref. manual in corresponding SW_MUX_CTL register (IOMUXC chapter)- GPIO1, 23. 
*
* @param  gpio   - definition from device header file, for example GPIO1_BASE for GPIO1
*         pinNum - pin number
*         pinDir - pin direction (0 = input, 1 = output)
*
* @return  None
*/
void PortSetup(uint32_t gpio, uint8_t pinNum, uint8_t pinDir)
{  
    /* Port configuration structure */
    gpio_pin_config_t portConfig = { pinDir ? kGPIO_DigitalOutput : kGPIO_DigitalInput, 0U, kGPIO_NoIntmode };

    /* Initialization */
    GPIO_PinInit((GPIO_Type *)gpio, pinNum, &portConfig);
}
