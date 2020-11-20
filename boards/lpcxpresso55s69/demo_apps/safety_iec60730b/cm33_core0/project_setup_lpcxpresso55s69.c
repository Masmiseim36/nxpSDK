/*
 * Copyright 2019 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "project_setup_lpcxpresso55s69.h"

#include "fsl_lpadc.h"
#include "fsl_power.h"
#include "fsl_power.h"
#include "fsl_clock.h"
#include "fsl_usart.h"
#include "fsl_iocon.h"
#include "freemaster.h"
#include "freemaster_serial.h"
#include "freemaster_serial_usart.h"

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
    SYSCON->CLOCK_CTRL |= SYSCON_CLOCK_CTRL_FRO1MHZ_CLK_ENA_MASK; /* Enable FRO_1MHz */
    SYSCON->WDTCLKDIV = 0; /* WD clock 1MHz */
    SYSCON->AHBCLKCTRL.AHBCLKCTRL0 |= SYSCON_AHBCLKCTRL0_WWDT_MASK; /* Enable clock to WDT */

    uint32_t bitMask = 0x400000;
    
    /* reset register is in SYSCON */
    /* set bit */
    SYSCON->PRESETCTRLSET[0] = bitMask;
    /* wait until it reads 0b1 */
    while (0u == (SYSCON->PRESETCTRLX[0] & bitMask))
    {
    }
    
    /* clear bit */
    SYSCON->PRESETCTRLCLR[0] = bitMask;
    /* wait until it reads 0b0 */
    while (bitMask == (SYSCON->PRESETCTRLX[0] & bitMask))
    {
    }
    
    USED_WDOG->TC = WWDT_TC_COUNT(WATCHDOG_TIMEOUT_VALUE); /* refresh value */ 
    USED_WDOG->MOD = WWDT_MOD_WDRESET(1) | WWDT_MOD_WDEN(1);
    USED_WDOG->WINDOW = 0xFFFFFF; /* Disable Window mode */

    __asm("CPSID i");
    USED_WDOG->FEED = 0xAA; /* Start WDOG */
    USED_WDOG->FEED = 0x55; 
    __asm("CPSIE i");
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
  /* Wdog is disabled on LPCdisabled after reset by default */
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
void SystickInitialisation(uint32_t reload_value)
{
//  SYSCON->SYSTICKCLKSEL.SYSTICKCLKSEL0 = 0; /*Main clock select */
//  SYSCON->SYSTICKCLKDIV0 &= ~(SYSCON_SYSTICKCLKDIV0_DIV_MASK); /*0 = div 1 */
  
  SysTick->VAL = 0;
  SysTick->LOAD = reload_value;
  SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk;
}

/* Second timer for CLOCK TEST */
void second_timer_inicialization(void)
{
  SYSCON->AHBCLKCTRL.AHBCLKCTRL0 |= SYSCON_AHBCLKCTRL0_RTC_MASK;
  
  SYSCON->CLOCK_CTRL |= SYSCON_CLOCK_CTRL_FRO1MHZ_CLK_ENA_MASK;
  
  
  SYSCON->CTIMERCLKSEL.CTIMERCLKSEL0 = 0x4; /* OSCILATOR 1MHZ */
  SYSCON->AHBCLKCTRL.AHBCLKCTRL1 |= SYSCON_AHBCLKCTRL1_TIMER0_MASK; /*Enable clock to Ctimer0*/

  SYSCON->PRESETCTRL.PRESETCTRL0 &= ~(SYSCON_PRESETCTRL1_TIMER0_RST_MASK);    // Reset the CTIMER0
  SYSCON->PRESETCTRL.PRESETCTRL0 |= (SYSCON_PRESETCTRL1_TIMER0_RST_MASK);
  
  
  CTIMER0->CTCR &= ~(CTIMER_CTCR_CTMODE_MASK);
  CTIMER0->TCR  |=  CTIMER_TCR_CEN_MASK; /*Enable counter*/
  CTIMER0->TCR  |=  CTIMER_TCR_CRST_MASK; /*Counter reset*/
  CTIMER0->TCR  &= ~(CTIMER_TCR_CRST_MASK); /*Counter stop reset*/
  
}

/*!
* @brief   Setup of clock
*
* @param   void
*
* @return  None
*
*   FRO set to 30MHz ------------------------- MAINCLKSEL -------- MAINCLKPLLSEL ----- MAINCLK = 30MHZ
*
*/
void ClockSetup(void)
{
    /* TODO Change to SDK! */ 
  
    POWER_DisablePD(kPDRUNCFG_PD_FRO192M);               /*!< Ensure FRO is on  */
    CLOCK_SetupFROClocking(12000000U);                   /*!< Set up FRO to the 12 MHz, just for sure */
    CLOCK_AttachClk(kFRO12M_to_MAIN_CLK);                /*!< Switch to FRO 12MHz first to ensure we can change the clock setting */

    /*!< Configure fro_1m */
    SYSCON->CLOCK_CTRL |=  SYSCON_CLOCK_CTRL_FRO1MHZ_CLK_ENA_MASK;                 /*!< Ensure fro_1m is on */

    CLOCK_SetupFROClocking(96000000U);                   /* Enable FRO HF(96MHz) output */

    /*!< Configure RTC OSC */
    POWER_DisablePD(kPDRUNCFG_PD_XTAL32K);               /*!< Powered the XTAL 32 kHz RTC oscillator */
    POWER_DisablePD(kPDRUNCFG_PD_FRO32K);                /*!< Powered the FRO 32 kHz RTC oscillator */
    CLOCK_AttachClk(kFRO32K_to_OSC32K);                  /*!< Switch OSC32K to FRO32K */
    CLOCK_EnableClock(kCLOCK_Rtc);                       /*!< Enable the RTC peripheral clock */
    RTC->CTRL &= ~RTC_CTRL_SWRESET_MASK;                 /*!< Make sure the reset bit is cleared */

    PMC->OSTIMERr |= PMC_OSTIMER_CLOCKENABLE_MASK;               /* The OSTIMER 32KHz clock is enabled. */

    SYSCON->CLOCK_CTRL |= SYSCON_CLOCK_CTRL_PLU_DEGLITCH_CLK_ENA_MASK;  /* The FRO_1 MHz and FRO_12 MHz for PLU glitch removal is enabled. */
                                                                        /* Please make sure FRO_1 MHz and FRO_12 MHz are enabled. */

    POWER_SetVoltageForFreq(96000000U);                  /*!< Set voltage for the one of the fastest clock outputs: System clock output */
    CLOCK_SetFLASHAccessCyclesForFreq(96000000U);          /*!< Set FLASH wait states for core */

    /*!< Set up dividers */
    CLOCK_SetClkDiv(kCLOCK_DivAhbClk, 1U, false);         /*!< Set AHBCLKDIV divider to value 1 */

    /*!< Set up clock selectors - Attach clocks to the peripheries */
    CLOCK_AttachClk(kFRO_HF_to_MAIN_CLK);                 /*!< Switch MAIN_CLK to FRO_HF */
}

/*!
* @brief   Initialization of CTIMER
*
*          This function initializes the CTIMER. CTIMER is used for After reset WDog test.
*
* @param   void
*
* @return  None
*/
void CTIMER_initialisation(void)
{ 
  SYSCON->CTIMERCLKSEL.CTIMERCLKSEL0 = 0x0; /* MAIN CLOCK */
  SYSCON->AHBCLKCTRL.AHBCLKCTRL1 |= SYSCON_AHBCLKCTRL1_TIMER0_MASK; /* Enable clock to Ctimer0 */

  SYSCON->PRESETCTRL.PRESETCTRL0 &= ~(SYSCON_PRESETCTRL1_TIMER0_RST_MASK); 
  SYSCON->PRESETCTRL.PRESETCTRL0 |= (SYSCON_PRESETCTRL1_TIMER0_RST_MASK);
  
  CTIMER0->CTCR &= ~(CTIMER_CTCR_CTMODE_MASK);
 
  CTIMER0->TCR  |=  CTIMER_TCR_CEN_MASK;    /* Enable counter */
  CTIMER0->TCR  |=  CTIMER_TCR_CRST_MASK;   /* Counter reset */
  CTIMER0->TCR  &= ~(CTIMER_TCR_CRST_MASK); /* Counter stop reset */
}
/*!
* @brief  Sets port direction and mux
*
* @param 
*
* @return  None
*/
void PortSetup(uint8_t *pByte, uint32_t *pDir, uint32_t *pIocon, uint32_t pinDir, uint32_t pinNum, uint32_t pull, uint32_t clock_enable_shift)
{
  /* Enable clock to GPIO module */
  SYSCON->AHBCLKCTRL.AHBCLKCTRL0 |= (1<< clock_enable_shift);
  
  *pIocon |= IOCON_PIO_DIGIMODE(1); /*Enable Digi mode*/
  *pIocon &= ~(IOCON_PIO_MODE_MASK); /*Clear PULL setting*/
  *pIocon |= IOCON_PIO_MODE(pull); /*Set pullup*/
  
  
  if (pinDir == PIN_DIRECTION_OUT)
  {
    *pDir |= (1 << pinNum);    /* PINx = 1 = output */
  } else if (pinDir == PIN_DIRECTION_IN)
  {
    *pDir &= ~(1 << pinNum); /* PINx = 0 = input */
  }
}


/*!
* @brief   Initialization of ADC0
*
*          8 MHz System Oscillator Bus Clock is the source clock.
*          single-ended 16-bit conversion
*
* @param   void
*
* @return  None
*/
void Adc0Setup(void)
{
    /* Analog pin setup 
     * TODO: pin tool SDK
     */
    /* GPIO_0_23, P19_4 on board */
    IOCON->PIO[0][23] = (0x400 | 0x10);
  
    lpadc_config_t configStruct;
    lpadc_conv_command_config_t commandConfigStruct;
    lpadc_conv_trigger_config_t triggerConfigStruct;

    /* Enable clock to ADC */
    CLOCK_SetClkDiv(kCLOCK_DivAdcAsyncClk, 16U, true);
    CLOCK_AttachClk(kMAIN_CLK_to_ADC_CLK);

    /* Disable LDOGPADC power down */
    POWER_DisablePD(kPDRUNCFG_PD_LDOGPADC);

    /* Configure ADC module */
    LPADC_GetDefaultConfig(&configStruct);
    configStruct.enableAnalogPreliminary = true;
    configStruct.referenceVoltageSource = kLPADC_ReferenceVoltageAlt2;
    configStruct.conversionAverageMode = kLPADC_ConversionAverage128;
    configStruct.powerLevelMode = kLPADC_PowerLevelAlt4;
    LPADC_Init(ADC0, &configStruct);
    
    /* Request offset calibration */
    //LPADC_DoOffsetCalibration(ADC0); // uncomment for auto calibration (must feed watchdog during calibration)
    ADC0->OFSTRIM = 0x10003; // manual calibration
    
    /* Request gain calibration. */
    //LPADC_DoAutoCalibration(ADC0); // uncomment for auto calibration (must feed watchdog during calibration)
    ADC0->GCR[0] = 0x00011AE; // manual calibration
    ADC0->GCR[1] = 0x0001138; // manual calibration
    
    /************************************/
    /* Set conversion CMD configuration */
    /************************************/
    LPADC_GetDefaultConvCommandConfig(&commandConfigStruct);
    commandConfigStruct.conversionResolutionMode = kLPADC_ConversionResolutionHigh;
    
    /* 3V3 */
    commandConfigStruct.channelNumber = 12U; /* 3V3 channel */
    LPADC_SetConvCommandConfig(ADC0, 1U, &commandConfigStruct); /* Command ID = 1 (idx 0) */
    
    /* 1V */
    commandConfigStruct.channelNumber = 13U; /* 1V channel */
    LPADC_SetConvCommandConfig(ADC0, 2U, &commandConfigStruct); /* Command ID = 2 (idx 1) */
    
    /* EXTERNAL PIN GPIO_0_23 (P19,4 on board) */
    commandConfigStruct.channelNumber = 0; /* external pin channel */
    LPADC_SetConvCommandConfig(ADC0, 3U, &commandConfigStruct); /* Command ID = 3 (idx 2) */

    /*****************************/
    /* Set trigger configuration */
    /*****************************/
    LPADC_GetDefaultConvTriggerConfig(&triggerConfigStruct);
    triggerConfigStruct.enableHardwareTrigger = false;
    
    /* 3V3 */
    triggerConfigStruct.targetCommandId = 1U;
    LPADC_SetConvTriggerConfig(ADC0, 0U, &triggerConfigStruct); /* Trigger ID = 0 */

    /* 1V */
    triggerConfigStruct.targetCommandId = 2U;
    LPADC_SetConvTriggerConfig(ADC0, 1U, &triggerConfigStruct); /* Trigger ID = 1 */
    
    /* EXTERNAL PIN */
    triggerConfigStruct.targetCommandId = 3U;
    LPADC_SetConvTriggerConfig(ADC0, 2U, &triggerConfigStruct); /* Trigger ID = 2 */
}

/************************************************/
void InitSerial(USART_Type *Uart_X, uint32_t brate, uint64_t bclk)
{  
    /* Init board hardware. */
    /* attach main clock divide to FLEXCOMM0 (debug console) 12MHz */
    CLOCK_AttachClk(kFRO12M_to_FLEXCOMM0);
    
    /* FreeMASTER communication layer initialization */
    /* Enables the clock for the I/O controller.: Enable Clock. */
    CLOCK_EnableClock(kCLOCK_Iocon);

    const uint32_t port0_pin29_config = (/* Pin is configured as FC0_RXD_SDA_MOSI_DATA */
                                         0x01u |
                                         /* No addition pin function */
                                         0x00u |
                                         /* Standard mode, output slew rate control is enabled */
                                         0x00u |
                                         /* Input function is not inverted */
                                         0x00u |
                                         /* Enables digital function */
                                         0x0100u |
                                         /* Open drain is disabled */
                                         0x00u);
    /* PORT0 PIN29 (coords: 92) is configured as FC0_RXD_SDA_MOSI_DATA */
    IOCON_PinMuxSet(IOCON, 0U, 29U, port0_pin29_config);

    const uint32_t port0_pin30_config = (/* Pin is configured as FC0_TXD_SCL_MISO_WS */
                                         0x01u |
                                         /* No addition pin function */
                                         0x00u |
                                         /* Standard mode, output slew rate control is enabled */
                                         0x00u |
                                         /* Input function is not inverted */
                                         0x00u |
                                         /* Enables digital function */
                                         0x0100u |
                                         /* Open drain is disabled */
                                         0x00u);
    /* PORT0 PIN30 (coords: 94) is configured as FC0_TXD_SCL_MISO_WS */
    IOCON_PinMuxSet(IOCON, 0U, 30U, port0_pin30_config);

    usart_config_t config;
    /*
     *   usartConfig->baudRate_Bps = brate;
     *   usartConfig->parityMode = kUSART_ParityDisabled;
     *   usartConfig->stopBitCount = kUSART_OneStopBit;
     *   usartConfig->bitCountPerChar = kUSART_8BitsPerChar;
     *   usartConfig->loopback = false;
     *   usartConfig->enableTx = false;
     *   usartConfig->enableRx = false;
    */
    USART_GetDefaultConfig(&config);
    /* Override the Default configuration to satisfy FreeMASTER needs */
    config.baudRate_Bps = brate;
    config.enableTx = true;
    config.enableRx = true;

    USART_Init(Uart_X, &config, bclk);

    /* Register communication module used by FreeMASTER driver. */
    FMSTR_SerialSetBaseAddress(Uart_X);

#if FMSTR_SHORT_INTR || FMSTR_LONG_INTR
    /* Enable UART interrupts. */
    EnableIRQ(FLEXCOMM0_IRQn);
    EnableGlobalIRQ(0);
#endif
}
