/*
 * Copyright 2021 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "project_setup_frdmk32l2a4s.h"
#include "clock_config.h"
#include "fsl_lpuart.h"
#include "pin_mux.h"
#include "board.h"

#include "freemaster.h"
#include "freemaster_serial_lpuart.h"

/*******************************************************************************
* Definitions
******************************************************************************/

/*******************************************************************************
* Prototypes
******************************************************************************/

/*******************************************************************************
* Variables
******************************************************************************/

/*******************************************************************************
* Code
******************************************************************************/

/*!
* @brief   Watchdog configuration function
*
*          Enables the watchdog. Also in Wait and Stop mode. Updates are allowed
*
 * @param   *WDOGx - pointer to the base address of the periphery
 *          timeout 
*
* @return  None
*/
void WatchdogEnable(void)
{
    WDOG0->CNT = WDOG_UPDATE_KEY;            /* Update of Watchdog registers */
    WDOG0->TOVAL = (uint16_t)WATCHDOG_TIMEOUT_VALUE;        /* Watchdog timeout value */

    /* Watchdog enabled in chip stop mode. */
    /* Watchdog enabled in chip wait mode. */
    /* Updates allowed. */
    /* Watchdog enabled. */
    /* Enables support for 32-bit refresh/unlock command write words. 16-bit or 8-bit is NOT supported. */
    /* LPO clock , 1 KHz */

    WDOG0->CS =  WDOG_CS_STOP_MASK | WDOG_CS_WAIT_MASK | WDOG_CS_UPDATE_MASK | WDOG_CS_EN_MASK | WDOG_CS_CMD32EN_MASK | WDOG_CS_CLK(1);
}

/*!
* @brief   Watchdog disabling function
*
* @param   *WDOG0x - pointer to the base address of the periphery
*
* @return  None
*/
void WatchdogDisable(void)
{
    __asm("cpsid i");
    WDOG0->CNT = WDOG_UPDATE_KEY;
    WDOG0->TOVAL = 0xFFFF;
    WDOG0->CS = (uint32_t) ((WDOG0->CS) & ~WDOG_CS_EN_MASK) | WDOG_CS_UPDATE_MASK;
    __asm("cpsie i");
}


/*!
* @brief   Initialization of Systick timer
*
*          This function configures the Systick as a source of interrupt
*
* @param   reload_value - defines the period of counter refresh
*
* @return  None
*/
void SystickInit(uint32_t reload_value)
{
    SysTick->VAL = 0;
    SysTick->LOAD = reload_value;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk;
}

/******************************************************************************
* clock setup
* TODO
*
******************************************************************************/
void ClockInit(void)
{
 
  BOARD_BootClockHSRUN();
 
  
}

/*!
* @brief   Initialization of LPTMR
*
*          This function initializes the LPTMR. LPTMR is used for clock test.
*
* @param   call_type - Macro WDOG_TEST or CLOCK_TEST (different timer settings
*
* @return  None
*/
void LPTMRInit(uint8_t call_type)
{
    PCC_LPTMR0 |= PCC_CLKCFG_CGC(1);  /* Enable LPTMR */
    LPTMR0->CSR = 0;                                        /* time counter mode */
    LPTMR0->CSR = LPTMR_CSR_TCF_MASK | LPTMR_CSR_TFC_MASK;  /* CNR reset on overflow */
    LPTMR0->PSR |= LPTMR_PSR_PBYP_MASK;                     /* prescaler bypassed, selected clock directly clocks the CNR */
    LPTMR0->PSR &= (~LPTMR_PSR_PCS_MASK);                  /* clear prescaler clock 0 selected MCGIRCLK  */
    
    if (call_type == WDOG_TEST ){
      LPTMR0->PSR |= LPTMR_PSR_PCS(2); /*32kHz for WDOG*/
    }else {
      LPTMR0->PSR |= LPTMR_PSR_PCS(0); /*8MHz for CLocktest */
    }
    LPTMR0->CMR = 0;                                        /* clear the compare register */
    LPTMR0->CSR |= LPTMR_CSR_TEN_MASK;                      /* enable timer */
 
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
        PCC0->CLKCFG[PCC_PORTA_INDEX] |= PCC_CLKCFG_CGC_MASK;   /* Clock enabled */
    } else if (gpio == GPIOB_BASE) {
        PCC0->CLKCFG[PCC_PORTB_INDEX] |= PCC_CLKCFG_CGC_MASK;   /* Clock enabled */
    } else if (gpio == GPIOC_BASE) {
        PCC0->CLKCFG[PCC_PORTC_INDEX] |= PCC_CLKCFG_CGC_MASK;   /* Clock enabled */
    } else if (gpio == GPIOD_BASE) {
        PCC0->CLKCFG[PCC_PORTD_INDEX] |= PCC_CLKCFG_CGC_MASK;   /* Clock enabled */
    } else if (gpio == GPIOE_BASE) {
        PCC0->CLKCFG[PCC_PORTE_INDEX] |= PCC_CLKCFG_CGC_MASK;   /* Clock enabled */
    }
    #ifdef PORTF
    else if (gpio == GPIOF_BASE) {
        PCC0->CLKCFG[PCC_PORTF_INDEX] |= PCC_CLKCFG_CGC_MASK;   /* Clock enabled */
    }
    #endif 
    #ifdef PORTG    
    else if (gpio == GPIOG_BASE) {
        PCC0->CLKCFG[PCC_PORTG_INDEX] |= PCC_CLKCFG_CGC_MASK;   /* Clock enabled */
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
    PCC0->CLKCFG[PCC_ADC0_INDEX] |= PCC_CLKCFG_CGC_MASK;   /* Clock enabled *//* enable clock gate to ADC0 */

    ADC0->SC1[0] = 0; /* conversion complete interrupt disabled, single-ended conversions anp input channels are selected */
    ADC0->SC1[0] |= ADC_SC1_ADCH(0x1F);   /* module disabled */
    ADC0->SC1[1] = 0;  /* conversion complete interrupt disabled, single-ended conversions anp input channels are selected */
    ADC0->SC1[1] |= ADC_SC1_ADCH(0x1F);  /* module disabled */
    ADC0->CFG1 = 0;
    ADC0->CFG1 |= ADC_CFG1_MODE(1); /* single-ended 12-bit conversion */

    ADC0->SC2 = 0;
    ADC0->SC3 = 0;
    ADC0->SC3 |= ADC_SC3_AVGS(1);
    ADC0->SC3 |= ADC_SC3_AVGE_MASK;
    
    PMC->REGSC |= PMC_REGSC_BGBE_MASK;       /* Bandgap buffer enable */
    
}

/*!
* @brief   Setup of UART or LPUART
*
* @param   Uart_X - instence UART0 or UART 1
* @param   brate  - desired baud rate
* @param   bclk - information about selected input clock frequency
*
* @return  1 - baud rate configurated correctly
*          0 - baud rate is not correctly configurated
*/

void SerialInit(void)
{
       lpuart_config_t config;

    /* Enable Clock into the LPUART peripheral */
   // CLOCK_SetIpSrc(BOARD_DEBUG_UART_PCC_ADDRESS, kCLOCK_IpSrcSircAsync);
    CLOCK_SetIpSrc(kCLOCK_Lpuart0, kCLOCK_IpSrcSircAsync);
    BOARD_InitPins();    

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
    config.baudRate_Bps = UART_BAUD_RATE;
    config.enableTx     = false;
    config.enableRx     = false;

    LPUART_Init((LPUART_Type*)BOARD_DEBUG_UART_BASEADDR, &config, BOARD_DEBUG_UART_CLK_FREQ);

    /* Register communication module used by FreeMASTER driver. */
    FMSTR_SerialSetBaseAddress((LPUART_Type*)BOARD_DEBUG_UART_BASEADDR);

#if FMSTR_SHORT_INTR || FMSTR_LONG_INTR
    /* Enable UART interrupts. */
    EnableIRQ(BOARD_UART_IRQ);
    EnableGlobalIRQ(0);
#endif
}