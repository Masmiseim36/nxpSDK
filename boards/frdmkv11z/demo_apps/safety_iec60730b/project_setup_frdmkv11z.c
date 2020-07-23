/*
 * Copyright 2019 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "project_setup_frdmkv11z.h"
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
    WDOG->STCTRLH = ( WDOG_STCTRLH_WAITEN_MASK | WDOG_STCTRLH_STOPEN_MASK | WDOG_STCTRLH_WDOGEN_MASK);
}

/*!
* @brief   Watchdog disabling function
*
* @param   *WDOGx - pointer to the base address of the periphery
*
* @return  None
*/
void WatchdogDisable(void)
{
    __asm("cpsid i");
    WDOG->UNLOCK = 0xC520;
    WDOG->UNLOCK = 0xD928;
    /* Clear the WDOGEN bit to disable the watchdog */
    WDOG->STCTRLH &= ~WDOG_STCTRLH_WDOGEN_MASK;
    __asm("cpsie i");
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

/*******************************************************************************
* clock setup
* 10 MHz external reference/256 ---> FLL(39.0625Khz) -->FLL Factor 1920-->75Mhz MCGOUTCLK/1 --> core clock: 75Mhz
*                                                                                        /3 --> bus clock:  25Mhz
* 4MHz IRC /1 --> MCGIRCLK: 4MHz
*
*******************************************************************************/
void ClockInit(void)
{
  BOARD_BootClockRUN();
 
}

/*!
* @brief   Initialization of LPTMR
*
*          This function initializes the LPTMR. LPTMR is used for clock test.
*
* @param   void
*
* @return  None
*/
void LPTMRInit(void)
{
    /* enable clock for LPTMR */
    SIM->SCGC5 |= SIM_SCGC5_LPTMR_MASK; /* enable clock gate to LPTMR */

    LPTMR0->CSR = 0;                                        /* time counter mode */
    LPTMR0->CSR = LPTMR_CSR_TCF_MASK | LPTMR_CSR_TFC_MASK;  /* CNR reset on overflow */
    LPTMR0->PSR |= LPTMR_PSR_PBYP_MASK;                     /* prescaler bypassed, selected clock directly clocks the CNR */
    LPTMR0->PSR &= (~LPTMR_PSR_PCS_MASK);                  /* clear prescaler clock 0 selected MCGIRCLK  */

    LPTMR0->PSR |= LPTMR_PSR_PCS(0);

    LPTMR0->CMR = 0;    /* clear the compare register */
    LPTMR0->CSR &= ~LPTMR_CSR_TEN_MASK;  /* stop the timer, this clears the TFC flag */ 
    LPTMR0->CSR |= LPTMR_CSR_TEN_MASK;  /* start the timer */
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
    volatile GPIO_Type* pGPIO   = (GPIO_Type *)gpio;  
    volatile PORT_Type* pPORT_PCR   = (PORT_Type *)pcr; 

    /* set GPIO and PORT pointers for PIN */
    if (gpio == GPIOA_BASE) {
        SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;    /* Clock enabled */
    } else if (gpio == GPIOB_BASE) {
        SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;    /* Clock enabled */
    } else if (gpio == GPIOC_BASE) {
        SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;    /* Clock enabled */
    } else if (gpio == GPIOD_BASE) {
        SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;   /* Clock enabled */
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
* @brief   Initialization of ADC0
*
*          8 MHz System Oscillator Bus Clock is the source clock.
*          single-ended 12-bit conversion
*
* @param   void
*
* @return  None
*/
void AdcInit(void)
{
  SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK; /* enable clock gate to ADC0 */

  ADC0->SC1[0] = 0;  /* conversion complete interrupt disabled, single-ended conversions anp input channels are selected */
  ADC0->SC1[0] |= ADC_SC1_ADCH(0x1F);  /* module disabled */
  ADC0->SC1[1] = 0;  /* conversion complete interrupt disabled, single-ended conversions anp input channels are selected */
  ADC0->SC1[1] |= ADC_SC1_ADCH(0x1F);  /* module disabled */
  ADC0->CFG1 = 0; /* bus clock as a source,  */
  ADC0->CFG1 |= ADC_CFG1_MODE(1); /* single-ended 12-bit conversion */
  ADC0->SC2 = 0;
  ADC0->SC3 = 0;
  ADC0->SC3 |= ADC_SC3_AVGS(1);
  ADC0->SC3 |= ADC_SC3_AVGE_MASK;

  PMC->REGSC |= PMC_REGSC_BGBE_MASK; /* enable bandgap buffer */
}

#if FMSTR_SERIAL_ENABLE
/*!
* @brief   Setup of UART
*/
void SerialInit(void)
{
    
     BOARD_InitPins();
     uart_config_t config;

    /*
     * config.baudRate_Bps = SERIAL_BAUD_RATE;
     * config.parityMode = kUART_ParityDisabled;
     * config.stopBitCount = kUART_OneStopBit;
     * config.txFifoWatermark = 0;
     * config.rxFifoWatermark = 1;
     * config.enableTx = false;
     * config.enableRx = false;
     */
    UART_GetDefaultConfig(&config);
    config.baudRate_Bps = SERIAL_BAUD_RATE;
    config.enableTx = false;
    config.enableRx = false;

    UART_Init((UART_Type*)BOARD_DEBUG_UART_BASEADDR, &config, BOARD_DEBUG_UART_CLK_FREQ);

    /* Register communication module used by FreeMASTER driver. */
    FMSTR_SerialSetBaseAddress((UART_Type*)BOARD_DEBUG_UART_BASEADDR);

#if FMSTR_SHORT_INTR || FMSTR_LONG_INTR
    /* Enable UART interrupts. */
    EnableIRQ(BOARD_UART_IRQ);
    EnableGlobalIRQ(0);
#endif
}
#endif /* FMSTR_SERIAL_ENABLE */
