/*
 * Copyright 2021 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "project_setup_frdmke15z.h"
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
    WDOG->CNT = WDOG_UPDATE_KEY;            /* Update of Watchdog registers */
    WDOG->TOVAL = (uint16_t)WATCHDOG_TIMEOUT_VALUE;        /* Watchdog timeout value */

    /* Watchdog enabled in chip stop mode. */
    /* Watchdog enabled in chip wait mode. */
    /* Updates allowed. */
    /* Watchdog enabled. */
    /* Enables support for 32-bit refresh/unlock command write words. 16-bit or 8-bit is NOT supported. */
    /* LPO clock , 128 KHz */

    WDOG->CS =  WDOG_CS_STOP_MASK | WDOG_CS_WAIT_MASK | WDOG_CS_UPDATE_MASK | WDOG_CS_EN_MASK | WDOG_CS_CMD32EN_MASK | WDOG_CS_CLK(1);
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
    WDOG->CNT = WDOG_UPDATE_KEY;
    WDOG->TOVAL = 0xFFFF;
    WDOG->CS = (uint32_t) ((WDOG->CS) & ~WDOG_CS_EN_MASK) | WDOG_CS_UPDATE_MASK;
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


/*!
* @brief   Init of clock
*
* @param   void
*
* @return  None
*
*                            TRIMDIV   LPFLL         DIVCORE
* 8 MHz High Range OSC |-------/4------x36-----------/1-------------->  core clock 72 MHz
*                      |                          |     |  DIVSLOW
*                      |                          |     |---/3------->  flash clock, bus clock 24 MHz
*                      |                          |
*                      |                          |
*                      |                          | LPFLLDIV2
*                      |                          |-----/2----------->  FLL DIV2 clock 36 MHz
*                      |
*                      |
*                      |                SOSCDIV2
*                      |------------------/1------------------------->  SOSC DIV2 clock  8 MHz  (ADC0, LPUART1)
*
*
*                     SIRCDIV2
* Slow IRC 8MHz --------/1------------------------------------------->  SIRC DIV2 clock 8 MHz  (LPTMR0)
*
*/
void ClockInit(void)
{
   
  BOARD_BootClockRUN();
     
}

/*!
* @brief   Initialization of LPTMR
*
*          This function initializes the LPTMR. LPTMR is used for clock test.
*
* @param   call_type - for demo compatibility - not used
*
* @return  None
*/
void LPTMRInit(uint8_t call_type)
{
    PCC->CLKCFG[PCC_LPTMR0_INDEX] &= ~PCC_CLKCFG_CGC_MASK;     /* Clock disabled */
    PCC->CLKCFG[PCC_LPTMR0_INDEX] |= PCC_CLKCFG_PCS(2);        /* slow IRC Clock. */
    PCC->CLKCFG[PCC_LPTMR0_INDEX] |= PCC_CLKCFG_CGC_MASK;      /* Clock enabled */
      /* SIRCDIV */ /* SIRCCFG */
    /* slow IRC must be disabled */
    SCG->SIRCCSR &= ~SCG_SIRCCSR_SIRCEN_MASK; /* disable slow IRC */
    SCG->SIRCCFG |= SCG_SIRCCFG_RANGE_MASK;   /* Slow IRC high range clock (8 MHz) */
    SCG->SIRCDIV &= ~SCG_SIRCDIV_SIRCDIV2_MASK;
    SCG->SIRCDIV |= SCG_SIRCDIV_SIRCDIV2(1);
    SCG->SIRCCSR |= SCG_SIRCCSR_SIRCEN_MASK; /* enable slow IRC */
  
    /* enable clock for LPTMR */
    PCC->CLKCFG[PCC_LPTMR0_INDEX] &= ~PCC_CLKCFG_CGC_MASK;     /* Clock disabled */
    PCC->CLKCFG[PCC_LPTMR0_INDEX] |= PCC_CLKCFG_PCS(2);        /* slow IRC Clock. */
    PCC->CLKCFG[PCC_LPTMR0_INDEX] |= PCC_CLKCFG_CGC_MASK;      /* Clock enabled */

    LPTMR0->CSR = 0;                                        /* time counter mode */
    LPTMR0->CSR = LPTMR_CSR_TCF_MASK | LPTMR_CSR_TFC_MASK;  /* CNR reset on overflow */
    LPTMR0->PSR &= (~LPTMR_PSR_PBYP_MASK);                  /* prescaler enabled */
    LPTMR0->PSR &= (~LPTMR_PSR_PRESCALE_MASK);              /* Divide 2 */
    LPTMR0->PSR &= (~LPTMR_PSR_PCS_MASK);                   /* clear prescaler clock 0 selected MCGIRCLK  */


    LPTMR0->PSR |= LPTMR_PSR_PCS(0); /*8MHz for CLocktest */

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
        PCC->CLKCFG[PCC_PORTA_INDEX] |= PCC_CLKCFG_CGC_MASK;   /* Clock enabled */
    } else if (gpio == GPIOB_BASE) {
        PCC->CLKCFG[PCC_PORTB_INDEX] |= PCC_CLKCFG_CGC_MASK;   /* Clock enabled */
    } else if (gpio == GPIOC_BASE) {
        PCC->CLKCFG[PCC_PORTC_INDEX] |= PCC_CLKCFG_CGC_MASK;   /* Clock enabled */
    } else if (gpio == GPIOD_BASE) {
        PCC->CLKCFG[PCC_PORTD_INDEX] |= PCC_CLKCFG_CGC_MASK;   /* Clock enabled */
    } else if (gpio == GPIOE_BASE) {
        PCC->CLKCFG[PCC_PORTE_INDEX] |= PCC_CLKCFG_CGC_MASK;   /* Clock enabled */
    }
    #ifdef PORTF
    else if (gpio == GPIOF_BASE) {
        PCC->CLKCFG[PCC_PORTF_INDEX] |= PCC_CLKCFG_CGC_MASK;   /* Clock enabled */
    }
    #endif 
    #ifdef PORTG    
    else if (gpio == GPIOG_BASE) {
        PCC->CLKCFG[PCC_PORTG_INDEX] |= PCC_CLKCFG_CGC_MASK;   /* Clock enabled */
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
    PCC->CLKCFG[PCC_ADC0_INDEX] = (PCC_CLKCFG_CGC(0));
    PCC->CLKCFG[PCC_ADC0_INDEX] = (PCC_CLKCFG_CGC(TRUE) | PCC_CLKCFG_PCS(1));  /* System Oscillator Bus Clock */

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
}


#ifdef _MKE15Z7_H_ 
/*This function only enable CLOCK to PORT which are use by TSI peripheral */
void tsi_port_clock_enable(void)
{
  PORT_Type* used_port[] = TSI_PORT_USE;
                    //0 = NULL = termination;
  for(int i= 0; used_port[i]!= 0; i++)
  {
   switch ((uint32_t)used_port[i])
   {
   case (uint32_t)PORTA:
    PCC->CLKCFG[PCC_PORTA_INDEX] |= PCC_CLKCFG_CGC_MASK;   /* Clock enabled */     
   break;

   case (uint32_t)PORTB:
    PCC->CLKCFG[PCC_PORTB_INDEX] |= PCC_CLKCFG_CGC_MASK;   /* Clock enabled */     
   break;

   case (uint32_t)PORTC:
    PCC->CLKCFG[PCC_PORTC_INDEX] |= PCC_CLKCFG_CGC_MASK;   /* Clock enabled */     
   break;

   case (uint32_t)PORTD:
    PCC->CLKCFG[PCC_PORTD_INDEX] |= PCC_CLKCFG_CGC_MASK;   /* Clock enabled */     
   break;

   case (uint32_t)PORTE:
    PCC->CLKCFG[PCC_PORTE_INDEX] |= PCC_CLKCFG_CGC_MASK;   /* Clock enabled */     
   break;
  
   }

 }

}
  


/*!
 * @brief   Initialization of TSI in Self capacitive mode
 *
 * Configyures TSI in self capacitive (single-wire) mode
 * conversion complete interrupt disabled
 *
 * @param   void
 *
 * @return  None
 */
void Tsi0InitSelfCap(void)
{
    PCC->CLKCFG[PCC_TSI0_INDEX] = (PCC_CLKCFG_CGC(0));
    PCC->CLKCFG[PCC_TSI0_INDEX] = (PCC_CLKCFG_CGC(TRUE) | PCC_CLKCFG_PCS(1));  /* System Oscillator Bus Clock */

    /* Self capacitive mode: No Sensitivity Boost (basic) */
    TSI->MODE = 0;   
    TSI->MUL0 = 0;
    TSI->MUL1 = 0;
    TSI->SINC = 0;
    TSI->SSC0 = 0;
    TSI->SSC1 = 0;    
    TSI->SSC2 = 0;    
    TSI->MODE = TSI_MODE_SETCLK(1) | TSI_MODE_S_XCH(1) | TSI_MODE_S_CTRIM(7) | TSI_MODE_S_XDN(1);                                             //0x10381020;
    TSI->MUL0 = TSI_MUL0_M_SEN_RES(3) | TSI_MUL0_M_PRE_RES(3) | TSI_MUL0_M_PRE_CURRENT(3);                                                    //0x60006300;
    TSI->MUL1 = TSI_MUL1_M_NMIRROR(3) | TSI_MUL1_M_PMIRRORR(3) | TSI_MUL1_M_PMIRRORL(3) | TSI_MUL1_M_VPRE_CHOOSE_MASK | TSI_MUL1_M_MODE_MASK; //0x0005007E
    TSI->SINC = TSI_SINC_DECIMATION(7) | TSI_SINC_ORDER_MASK;                                                                                 //0x00270000;
    TSI->SSC0 = TSI_SSC0_BASE_NOCHARGE_NUM(4) | TSI_SSC0_CHARGE_NUM(6) | TSI_SSC0_PRBS_OUTSEL(6);                                             //0x60640000;
    TSI->SSC1 = TSI_SSC1_PRBS_SEED_LO(64) | TSI_SSC1_PRBS_WEIGHT_LO(96);                                                                      //0x00600040;
    TSI->SSC2 = TSI_SSC2_MOVE_REPEAT_NUM(1) | TSI_SSC2_MOVE_STEPS_NUM(1) | TSI_SSC2_MOVE_NOCHARGE_MAX(8) | TSI_SSC2_MOVE_NOCHARGE_MIN(1);     //0x10080101;

    /* initiate tsi wakeup threshold */
    TSI->TSHD = 0xFFFF;
    TSI->TSHD = 0;
    TSI->GENCS = 0;
    TSI->GENCS |= TSI_GENCS_TSIEN_MASK; /* enable tsi module */
}

/*!
 * @brief   Initialization of TSI in Mutual capacitive mode
 *
 * Configyures TSI in Mutual capacitive (dual-wire) mode
 * conversion complete interrupt disabled
 *
 * @param   void
 *
 * @return  None
 */
void Tsi0InitMutualCap(void)
{
    PCC->CLKCFG[PCC_TSI0_INDEX] = (PCC_CLKCFG_CGC(0));
    PCC->CLKCFG[PCC_TSI0_INDEX] = (PCC_CLKCFG_CGC(TRUE) | PCC_CLKCFG_PCS(1));  /* System Oscillator Bus Clock */

    /* Mutual capacitive mode */
//    TSI->MODE = 0x00000022;
//    TSI->MUL0 = 0x60006300;
//    TSI->MUL1 = 0x000041E0;
//    TSI->SINC = 0x00070000;
//    TSI->SSC0 = 0x60310000;
//    TSI->SSC1 = 0x00600040;
//    TSI->SSC2 = 0x10080101;
    
    TSI->MODE = 0x30400002;
    TSI->MUL0 = 0x60006320;
    TSI->MUL1 = 0x000100E0;
    TSI->SINC = 0x00230001;
    TSI->SSC0 = 0x60310000;
    TSI->SSC1 = 0x00600040;
    TSI->SSC2 = 0x10080101;

    /* initiate tsi wakeup threshold */
    TSI->TSHD = 0xFFFF;
    TSI->GENCS = 0;
    TSI->GENCS |= TSI_GENCS_TSIEN_MASK;         //enable tsi module
}

void Tsi0ClockEnable(void)
{
    PCC->CLKCFG[PCC_TSI0_INDEX] = (PCC_CLKCFG_CGC(0));
    PCC->CLKCFG[PCC_TSI0_INDEX] = (PCC_CLKCFG_CGC(TRUE) | PCC_CLKCFG_PCS(1));  /* System Oscillator Bus Clock */
    //TSI->GENCS |= TSI_GENCS_TSIEN_MASK; /* enable tsi module */
}

#endif


/*!
 * @brief   Init of LPUART
 *
 *
 *
 * @param   baudRate  - desired baud rate
 * @param   clockFreq - information about selected input clock frequency
 *
 * @return  1 - baud rate configurated correctly
 *          0 - baud rate is not correctly configurated
 */
void SerialInit(void)
{
    lpuart_config_t config;
   
    BOARD_InitPins();
    
    CLOCK_SetIpSrc(kCLOCK_Lpuart1, BOARD_DEBUG_UART_CLKSRC);

    /*
     * config.baudRate_Bps = UART_BAUD_RATE;
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

    LPUART_Init((LPUART_Type *)BOARD_DEBUG_UART_BASEADDR, &config,  CLOCK_GetIpFreq(kCLOCK_Lpuart1));

    /* Register communication module used by FreeMASTER driver. */
    FMSTR_SerialSetBaseAddress((LPUART_Type *)BOARD_DEBUG_UART_BASEADDR);

#if FMSTR_SHORT_INTR || FMSTR_LONG_INTR
    /* Enable UART interrupts. */
    EnableIRQ(BOARD_UART_IRQ);
    EnableGlobalIRQ(0);
#endif

}
