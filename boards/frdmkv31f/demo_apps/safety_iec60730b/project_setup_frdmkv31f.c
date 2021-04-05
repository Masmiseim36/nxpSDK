/*
 * Copyright 2021 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "project_setup_frdmkv31f.h"
#include "clock_config.h"
#include "pin_mux.h"
#include "fsl_uart.h"
#include "freemaster.h"
#include "freemaster_serial_uart.h"
#include "board.h"

/*******************************************************************************
* Code
******************************************************************************/

/*!
* @brief   Watchdog configuration function
*
*          Enables the watchdog. Also in Wait and Stop mode. Updates are allowed
*
* @param   None
*
* @return  None
*/
void WatchdogEnable(void)
{
    uint16_t timeout_low;
    uint16_t timeout_high;

    /* split timeout value into two 16-bit variables */
    timeout_low = (uint16_t)WATCHDOG_TIMEOUT_VALUE;
    timeout_high = (uint16_t)(WATCHDOG_TIMEOUT_VALUE >> 16);

    WDOG->UNLOCK = 0xC520;       /* unlock sequence */
    WDOG->UNLOCK = 0xD928;       /* unlock sequence */
    __asm("nop");                 /* must wait at least one cycle after unlocking */
    WDOG->TOVALL = timeout_low;  /* timeout low value */
    WDOG->TOVALH = timeout_high; /* timeout high value */
    
    WDOG->PRESC &= ~WDOG_PRESC_PRESCVAL_MASK;

    /* WD enabled. WD enabled in CPU Wait mode, CPU Stop mode. */
    WDOG->STCTRLH = ( WDOG_STCTRLH_WAITEN_MASK | WDOG_STCTRLH_STOPEN_MASK | WDOG_STCTRLH_WDOGEN_MASK |WDOG_STCTRLH_ALLOWUPDATE_MASK);
    __asm("nop");
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
    __asm("cpsid i");
    WDOG->UNLOCK = 0xC520;  /* unlock sequence */
    WDOG->UNLOCK = 0xD928;
    __asm("cpsie i");
    /* Clear the WDOGEN bit to disable the watchdog */
    WDOG->STCTRLH &= ~WDOG_STCTRLH_WDOGEN_MASK;
}

/*!
* @brief   Initialization of Systick timer
*
*          This function configures the Systick as a source of interrupt
*
* @param   reloadValue - defines the period of counter refresh
*
* @return  None
*/
void SystickInit(uint32_t reloadValue)
{
    SysTick->VAL = 0;
    SysTick->LOAD = reloadValue;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk;
}

/*!
* @brief   Initialization of LPTMR
*
*          This function initializes the LPTMR. LPTMR is used for clock test.
*          MCGIRCLK is source (4MHz)
*
* @param   void
*
* @return  None
*/
void LPTMRInit(void)
{

    SIM->SCGC5 |= SIM_SCGC5_LPTMR_MASK; /* enable clock gate to LPTMR */
    LPTMR0->CSR = 0;                                     /* time counter mode */
    LPTMR0->CSR = LPTMR_CSR_TCF_MASK|LPTMR_CSR_TFC_MASK; /* CNR reset on overflow */
    LPTMR0->PSR |= LPTMR_PSR_PBYP_MASK;                  /* prescaler bypassed, selected clock directly clocks the CNR */
    LPTMR0->PSR &= (~LPTMR_PSR_PCS_MASK);                /* clear prescaler clock 0 selected MCGIRCLK  */
    LPTMR0->CMR = 0;                                     /* clear the compare register */
    LPTMR0->CSR |= LPTMR_CSR_TEN_MASK;                   /* enable timer */
}

/*!
* @brief   Setup of clock
*
*          Core clock: FLL, 80 MHz
*          Fast IRC: 4 MHz
*          Oscilator  8 MHz
*
*
* @param   void
*
* @return  None
*
*                             FRDIV      FLL       FLL Factor                 OUTDIV1
* 8 MHz external oscilator|---/256 ---(31.25 KHz)----x2560-----(MCGOUTCLK)-------/1---->  Core clock     80 MHz
*                         |                                                   OUTDIV2
*                         |                                                  ---/2----->  Bus clock      40 MHz
*                         |                                                   OUTDIV3
*                         |                                                  ---/4----->  FlexBus clock  20 MHz
*                         |                                                   OUTDIV4
*                         |                                                  ---/4----->  Flash clock    20 MHz
*
* 4 MHz IRC /1 --> MCGIRCLK: 4 MHz
*
*/
void ClockInit(void)
{
  BOARD_BootClockHSRUN();
}

/*!
 * @brief  Sets port direction and mux.
 *
 * @param  gpio   - definition from iec60730b_dio.h, for example FS_DIO_GPIOA for GPIO A.
 *         pcr    - base address of PCR register.
 *         pinNum - pin number.
 *         pinDir - pin direction.
 *         pinMux - pin mux (usually GPIO_MUX == 1).
 *
 * @return  None
 */
void PortInit(uint32_t gpio, uint32_t pcr, uint8_t pinNum, uint8_t pinDir, uint8_t pinMux)
{
    volatile GPIO_Type* pGPIO     = (GPIO_Type *)gpio;  
    volatile PORT_Type* pPORT_PCR = (PORT_Type *)pcr; 

    /* set GPIO and PORT pointers for PIN */
    if (gpio == GPIOA_BASE) {
        SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;    /* Clock enabled */
    } else if (gpio == GPIOB_BASE) {
        SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;    /* Clock enabled */
    } else if (gpio == GPIOC_BASE) {
        SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;    /* Clock enabled */
    } else if (gpio == GPIOD_BASE) {
        SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;    /* Clock enabled */
    } else if (gpio == GPIOE_BASE) {
        SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;    /* Clock enabled */
    }
    #ifdef PORTF
    else if (gpio == GPIOF_BASE) {
        SIM->SCGC5 |= SIM_SCGC5_PORTF_MASK;    /* Clock enabled */
    }
    #endif 
    #ifdef PORTG    
    else if (gpio == GPIOG_BASE) {
        SIM->SCGC5 |= SIM_SCGC5_PORTG_MASK;    /* Clock enabled */
    }
    #endif

    pPORT_PCR->PCR[pinNum] &= (~PORT_PCR_MUX_MASK);
    pPORT_PCR->PCR[pinNum] |= PORT_PCR_MUX(pinMux);     /* Alternative */

    if (pinDir == PIN_DIRECTION_OUT)
    {
        pGPIO->PDDR |= (1 << pinNum);    /* PINx = 1 = output */
    } else if (pinDir == PIN_DIRECTION_IN) {
        pGPIO->PDDR &= ~(1 << pinNum);   /* PINx = 0 = input */
    }
}

/*!
* @brief   UART setup
*
*  baud rate = UART Module clock/(16 x SBR)
*  9600 = 80 MHz/(16 x 521)
*
*
* @param   brate - Baud rate
*          bclk = input clock for UART
*          Uart_X = Uart instance used in aplication UART0 or UART1 (macros from device header file)
*
* @return  None
*/
void SerialInit(void)
{
     uart_config_t config;
     BOARD_InitPins();
    /*
     * config.baudRate_Bps = UART_BAUD_RATE;
     * config.parityMode = kUART_ParityDisabled;
     * config.stopBitCount = kUART_OneStopBit;
     * config.txFifoWatermark = 0;
     * config.rxFifoWatermark = 1;
     * config.enableTx = false;
     * config.enableRx = false;
     */
    UART_GetDefaultConfig(&config);
    config.baudRate_Bps = UART_BAUD_RATE;
    config.enableTx = false;
    config.enableRx = false;

    UART_Init((UART_Type *)(BOARD_DEBUG_UART_BASEADDR), &config, CLOCK_GetCoreSysClkFreq());

    /* Register communication module used by FreeMASTER driver. */
    FMSTR_SerialSetBaseAddress((UART_Type *)(BOARD_DEBUG_UART_BASEADDR));

#if FMSTR_SHORT_INTR || FMSTR_LONG_INTR
    /* Enable UART interrupts. */
    EnableIRQ(BOARD_UART_IRQ);
    EnableGlobalIRQ(0);
#endif
}

/*!
* @brief   Initialization of ADC_x
*
*          single-ended 12-bit conversion
*
* @param   ADC_x - instance of ADC = ADC0 or ADC1
*
* @return  None
*/
void AdcInit( ADC_Type * ADC_x)
{
    if (ADC_x == ADC0)
    {
        SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK; /* enable clock gate to ADC0 */
    }else
    {
        SIM->SCGC6 |= SIM_SCGC6_ADC1_MASK; /* enable clock gate to ADC1 */
    }


    ADC_x->SC1[0] = 0;  /* conversion complete interrupt disabled, single-ended conversions anp input channels are selected */
    ADC_x->SC1[0] |= ADC_SC1_ADCH(0x1F);  /* module disabled */
    ADC_x->SC1[1] = 0;  /* conversion complete interrupt disabled, single-ended conversions anp input channels are selected */
    ADC_x->SC1[1] |= ADC_SC1_ADCH(0x1F);  /* module disabled */
    ADC_x->CFG1 = 0; /* bus clock as a source,  */
    ADC_x->CFG1 |= ADC_CFG1_MODE(1); /* single-ended 12-bit conversion */
    ADC_x->SC2 = 0;
    ADC_x->SC3 = 0;
    ADC_x->SC3 |= ADC_SC3_AVGS(1);
    ADC_x->SC3 |= ADC_SC3_AVGE_MASK;

    PMC->REGSC |= PMC_REGSC_BGBE_MASK; /* enable bandgap buffer */
}
