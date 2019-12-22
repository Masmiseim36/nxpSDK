/*
 * Copyright 2019 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "project_setup_evkbimxrt1050.h"

#include "freemaster.h"
#include "freemaster_serial.h"
#include "freemaster_serial_lpuart.h"

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
void WatchdogEnable(RTWDOG_Type *WDOGx, uint32_t timeout)   
{   
  uint32_t prescaler = 0;
  uint16_t window = 0;

  __asm("cpsid i");
    
    WDOGx->CNT = RTWDOG_UPDATE_KEY; /* Unlock sequence */ 
    WDOGx->TOVAL = (uint16_t)timeout; /* Set timeout */
    
    /* Enable rtwdog, LPO clock, interrupt disabled, update enabled, 32b refresh, window mode, prescaler 255 enabled/disabled */
    WDOGx->CS = RTWDOG_CS_EN_MASK | RTWDOG_CS_CLK(1) | RTWDOG_CS_INT(0) | 
                 RTWDOG_CS_UPDATE(1) | RTWDOG_CS_CMD32EN_MASK |
                 (window == 0 ? RTWDOG_CS_WIN(0) : RTWDOG_CS_WIN(1)) |
                 (prescaler == 0 ? RTWDOG_CS_PRES(0) : RTWDOG_CS_PRES(1));
                 
    if (window > 0)
    {
        WDOGx->WIN = (uint16_t)window;
    }

    __asm("cpsie i");
}  
     
/*!
 * @brief   Watchdog disabling function
 * 
 * @param   void
 *
 * @return  None
 */
void WatchdogDisable(RTWDOG_Type *WDOGx)
{  
    __asm("cpsid i");
    
    WDOGx->CNT = RTWDOG_UPDATE_KEY; /* Unlock sequence */    
    WDOGx->TOVAL = (uint16_t)0xFFFF; /* Write any nonzero value */
    /* Clear the EN bit to disable watchdog */
    WDOGx->CS = (uint32_t) ((WDOGx->CS) & ~RTWDOG_CS_EN_MASK);
    
    __asm("cpsie i");
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
    /* Gpt serial clock, gpt bus clock enable */  
    CCM->CCGR1 |= (CCM_CCGR1_CG10_MASK | CCM_CCGR1_CG11_MASK); 
    
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
 * @brief   Initialization of GPT2 timer
 *
 *          Used for clock test
 *
 * @param   clkSource - 
 * @param   compare   - 32bit value
 * @param   prescaler - valid range 1-4096
 *
 * @return  None
 */
void GPT2_Init(uint32_t clkSource, uint32_t compare, uint32_t prescaler)
{
    /* Gpt2 serial clock, gpt2 bus clock enable */  
    CCM->CCGR0 |= (CCM_CCGR0_CG12_MASK | CCM_CCGR0_CG13_MASK); 
    
    /* Reset registers */
    GPT2->CR = GPT_CR_SWR(1U);
    
    /* Wait enable, stop enable, counter is reset when timer is disabled */
    GPT2->CR |= GPT_CR_WAITEN_MASK | GPT_CR_STOPEN_MASK | GPT_CR_ENMOD_MASK;
    
    /* Clock source and prescaler */
    if (clkSource == GPT_SRC_24M)
    {
        GPT2->CR |= GPT_CR_CLKSRC(clkSource) | GPT_CR_EN_24M_MASK; /* Set 24MHz clock source */
    }
    else
    {
        GPT2->CR |= GPT_CR_CLKSRC(clkSource); /* Set clock source */
    }
  
    if (prescaler > 0 && prescaler <= GPT_PR_PRESCALER_MASK + 1)
    {
        GPT2->PR = GPT_PR_PRESCALER(prescaler - 1);
    }
    
    GPT2->OCR[0] = GPT_OCR_COMP(compare); /* Compare value */
    
    /* Choose whether generates interrupt or not */
    //GPT2->IR = GPT_IR_OF1IE_MASK; /* Output compare 1 interrupt enable */
    //NVIC_EnableIRQ(GPT2_IRQn);    /* Enable interrupt */
    
    GPT2->CR |= GPT_CR_EN_MASK; /* Enable GPT */
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
void SerialInit(LPUART_Type *Uart_X, uint32_t baudRate, uint32_t clockFreq)
{
    const lpuart_config_t LPUART_1_config =
    {
      .baudRate_Bps = baudRate,
      .parityMode = kLPUART_ParityDisabled,
      .dataBitsCount = kLPUART_EightDataBits,
      .isMsb = false,
      .stopBitCount = kLPUART_OneStopBit,
      .txFifoWatermark = 0,
      .rxFifoWatermark = 0,
      .enableRxRTS = false,
      .enableTxCTS = false,
      .txCtsSource = kLPUART_CtsSourcePin,
      .txCtsConfig = kLPUART_CtsSampleAtStart,
      .rxIdleType = kLPUART_IdleTypeStartBit,
      .rxIdleConfig = kLPUART_IdleCharacter1,
      .enableTx = true,
      .enableRx = true
    };
    
    LPUART_Init(Uart_X, &LPUART_1_config, clockFreq);

    FMSTR_SerialSetBaseAddress(APPLICATION_SERIAL_BASE);
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

/*!
* @brief   Initialization of ADC0
*
*          
*
* @param   void
*
* @return  None
*/
void AdcInit(void)
{
    /* Enable clock to ADC module 1 */
    CLOCK_EnableClock(kCLOCK_Adc1); 

    /* Single-ended 12-bit conversion (MODE = 0x1) */
    /* Set divide ratio to 2 (ADIV = 0x1) */
    /* 4 samples averaged (AVGS = 0x0) */
    /* IPG clock select (ADICLK = 0x0) */
    /* Software trigger selected */
    ADC1->CFG = ( ADC_CFG_ADICLK(0U) | ADC_CFG_MODE(2U) | ADC_CFG_ADIV(1U) | ADC_CFG_AVGS(0U) | ADC_CFG_ADTRG(0U) );

    /* HW averaging disabled (AVGE = 0) */
    /* One conversion or one set of conversion (ADCO = 0) */ 
    ADC1->GC = (ADC_GC_AVGE(0U) | ADC_GC_ADCO(0U));
    
    /* Asynchronous clock output disabled */
    ADC1->GC |= ADC_GC_ADACKEN(0U); 
    
    /* ------- ADC self calibration procedure start ----- */ 
    /* Starting the calibration of ADC1 */
    /* Clear the CALF and launch the calibration. */
    ADC1->GS = ADC_GS_CALF_MASK; /* Clear the CALF. */
    ADC1->GC |= ADC_GC_CAL_MASK; /* Launch the calibration. */
      
    /* Check the status of CALF bit in ADC_GS and the CAL bit in ADC_GC. */
    while (0U != (ADC1->GC & ADC_GC_CAL_MASK))
    {
        /* Check the CALF when the calibration is active. */
        if (0U != (ADC1->GS & ADC_GS_CALF_MASK))
        {
            break;
        }
    }
}

/*!
* @brief  DCP module initialization (used in Flash test)       
*
* @param  pBuffer - pointer to context switching buffer
*
* @return None
*/
void DCPInit(uint32_t *buffer)
{    
    /* Turn off the entire data cache */
    SCB_DisableDCache();
  
    /* DCP clock enable */
    CCM->CCGR0 = (CCM->CCGR0 & ~CCM_CCGR0_CG5_MASK) | CCM_CCGR0_CG5(0x3);

    /* residual_writes=1, context_caching=0, interrupt disable */
    DCP->CTRL = DCP_CTRL_SFTRST(0) | DCP_CTRL_CLKGATE(0) | 
                DCP_CTRL_PRESENT_CRYPTO(1) | DCP_CTRL_PRESENT_SHA(1) | 
                DCP_CTRL_GATHER_RESIDUAL_WRITES(0) | DCP_CTRL_ENABLE_CONTEXT_CACHING(0) | 
                DCP_CTRL_ENABLE_CONTEXT_SWITCHING(0) | DCP_CTRL_CHANNEL_INTERRUPT_ENABLE(0);
    
    /* context_switching=1 */
    DCP->CTRL |= DCP_CTRL_ENABLE_CONTEXT_SWITCHING(1);

    /* Enable all channels */
    DCP->CHANNELCTRL = DCP_CHANNELCTRL_ENABLE_CHANNEL(0xF);
    
    /* Store the CSB address into DCP->CONTEXT */
    DCP->CONTEXT = (uint32_t)buffer;
 }

/*!
* @brief   This function performs the MPU (Memory Protection Unit) configuration
*          for the various memory regions inc1uding cache setup and the exact 
*          behaviour of this function is contro11ed by a number of defined symbo1s.
*
* @param   none
*
* @return  None
*/
void MPUSetup(void)
{
    /* Disable I cache and D cache */
    SCB_DisableICache();
    SCB_DisableDCache();
  
    /* Disable MPU */
    ARM_MPU_Disable();

#if WATCHDOG_ENABLED
  Watchdog_refresh; /********************************************************/
#endif
    
    /* MPU configure:
     * Use ARM_MPU_RASR(DisableExec, AccessPermission, TypeExtField, IsShareable, IsCacheable, IsBufferable, SubRegionDisable, Size) 
     * API in core_cm7.h.
     * param DisableExec       Instruction access (XN) disable bit,0=instruction fetches enabled, 1=instruction fetches disabled.
     * param AccessPermission  Data access permissions, allows you to configure read/write access for User and Privileged mode.
     *      Use MACROS defined in core_cm7.h: ARM_MPU_AP_NONE/ARM_MPU_AP_PRIV/ARM_MPU_AP_URO/ARM_MPU_AP_FULL/ARM_MPU_AP_PRO/ARM_MPU_AP_RO
     * Combine TypeExtField/IsShareable/IsCacheable/IsBufferable to configure MPU memory access attributes.
     *  TypeExtField  IsShareable  IsCacheable  IsBufferable   Memory Attribtue    Shareability        Cache
     *     0             x           0           0             Strongly Ordered    shareable
     *     0             x           0           1              Device             shareable           
     *     0             0           1           0              Normal             not shareable   Outer and inner write through no write allocate           
     *     0             0           1           1              Normal             not shareable   Outer and inner write back no write allocate           
     *     0             1           1           0              Normal             shareable       Outer and inner write through no write allocate    
     *     0             1           1           1              Normal             shareable       Outer and inner write back no write allocate    
     *     1             0           0           0              Normal             not shareable   outer and inner noncache
     *     1             1           0           0              Normal             shareable       outer and inner noncache
     *     1             0           1           1              Normal             not shareable   outer and inner write back write/read acllocate
     *     1             1           1           1              Normal             shareable       outer and inner write back write/read acllocate
     *     2             x           0           0              Device              not shareable   
     *  Above are normal use settings, if your want to see more details or want to config different inner/outter cache policy.      
     *  please refer to Table 4-55 /4-56 in arm cortex-M7 generic user guide <dui0646b_cortex_m7_dgug.pdf>
     * param SubRegionDisable  Sub-region disable field. 0=sub-region is enabled, 1=sub-region is disabled.
     * param Size              Region size of the region to be configured. use ARM_MPU_REGION_SIZE_xxx MACRO in core_cm7.h.
     */

    /* Region 0 setting: Memory with Device type, not shareable, non-cacheable. */
    MPU->RBAR = ARM_MPU_RBAR(0, 0xC0000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_512MB);

    /* Region 1 setting: Memory with Device type, not shareable,  non-cacheable. */
    MPU->RBAR = ARM_MPU_RBAR(1, 0x80000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_1GB);

    /* Region 2 setting */
#if defined(XIP_EXTERNAL_FLASH) && (XIP_EXTERNAL_FLASH == 1)
    /* Setting Memory with Normal type, not shareable, outer/inner write back. */
    MPU->RBAR = ARM_MPU_RBAR(2, 0x60000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_RO, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_64MB);
#else
    /* Setting Memory with Device type, not shareable, non-cacheable. */
    MPU->RBAR = ARM_MPU_RBAR(2, 0x60000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_RO, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_64MB);
#endif

    /* Region 3 setting: Memory with Device type, not shareable, non-cacheable. */
    MPU->RBAR = ARM_MPU_RBAR(3, 0x00000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_1GB);

    /* Region 4 setting: Memory with Normal type, not shareable, outer/inner write back */
    MPU->RBAR = ARM_MPU_RBAR(4, 0x00000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_128KB);

    /* Region 5 setting: Memory with Normal type, not shareable, outer/inner write back */
    MPU->RBAR = ARM_MPU_RBAR(5, 0x20000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_128KB);

    /* Region 6 setting: Memory with Normal type, not shareable, outer/inner write back */
    MPU->RBAR = ARM_MPU_RBAR(6, 0x20200000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_256KB);
 
    /* The define sets the cacheable memory to shareable, 
     * this suggestion is referred from chapter 2.2.1 Memory regions, 
     * types and attributes in Cortex-M7 Devices, Generic User Guide */
#if defined(SDRAM_IS_SHAREABLE)
    /* Region 7 setting: Memory with Normal type, not shareable, outer/inner write back */
    MPU->RBAR = ARM_MPU_RBAR(7, 0x80000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 1, 1, 1, 0, ARM_MPU_REGION_SIZE_32MB);
#else
    /* Region 7 setting: Memory with Normal type, not shareable, outer/inner write back */
    MPU->RBAR = ARM_MPU_RBAR(7, 0x80000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_32MB);
#endif
    
    /* Region 8 setting, set last 2MB of SDRAM can't be accessed by cache, glocal variables which are not expected to be
     * accessed by cache can be put here */
    /* Memory with Normal type, not shareable, non-cacheable */
    MPU->RBAR = ARM_MPU_RBAR(8, 0x81E00000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 1, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_2MB);

    /* Enable MPU */
    ARM_MPU_Enable(MPU_CTRL_PRIVDEFENA_Msk);

    /* Enable I cache and D cache */
    SCB_EnableDCache();
    SCB_EnableICache();
}
