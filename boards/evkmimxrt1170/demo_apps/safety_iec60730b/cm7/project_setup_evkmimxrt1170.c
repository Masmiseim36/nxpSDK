/*
 * Copyright 2021 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "project_setup_evkmimxrt1170.h"

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
void WatchdogEnable(void)
{
    uint32_t prescaler = 0;
    uint16_t window    = 0;

    __asm("cpsid i");
    /* RT1170 has RTWDOG 3,4 - they are enabled by default and second one (RTWDOG4) is unused thus must be disabled*/
    /* RTWDOG 4 */
    if ((RTWDOG4->CS & RTWDOG_CS_CMD32EN_MASK) != 0U)
    {
        RTWDOG4->CNT = 0xD928C520U; /* 0xD928C520U is the update key */
    }
    else
    {
        RTWDOG4->CNT = 0xC520U;
        RTWDOG4->CNT = 0xD928U;
    }
    RTWDOG4->TOVAL = 0xFFFF;
    RTWDOG4->CS    = (uint32_t)((RTWDOG4->CS) & ~RTWDOG_CS_EN_MASK) | RTWDOG_CS_UPDATE_MASK;

    /* USED_WDOG */
    USED_WDOG->CNT   = RTWDOG_UPDATE_KEY;                /* Unlock sequence */
    USED_WDOG->TOVAL = (uint16_t)WATCHDOG_TIMEOUT_VALUE; /* Set timeout */

    /* Enable rtwdog, LPO clock, interrupt disabled, update enabled, 32b refresh, window mode, prescaler 255
     * enabled/disabled */
    USED_WDOG->CS = RTWDOG_CS_EN_MASK | RTWDOG_CS_CLK(1) | RTWDOG_CS_INT(0) | RTWDOG_CS_UPDATE(1) |
                    RTWDOG_CS_CMD32EN_MASK | (window == 0 ? RTWDOG_CS_WIN(0) : RTWDOG_CS_WIN(1)) |
                    (prescaler == 0 ? RTWDOG_CS_PRES(0) : RTWDOG_CS_PRES(1));

    if (window > 0)
    {
        USED_WDOG->WIN = (uint16_t)window;
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
void WatchdogDisable(void)
{
    __asm("cpsid i");
    /* RT1170 has RTWDOG 3,4 - they are enabled by default and second one (RTWDOG4) is unused thus must be disabled*/
    /* RTWDOG 4 */
    if ((RTWDOG4->CS & RTWDOG_CS_CMD32EN_MASK) != 0U)
    {
        RTWDOG4->CNT = 0xD928C520U; /* 0xD928C520U is the update key */
    }
    else
    {
        RTWDOG4->CNT = 0xC520U;
        RTWDOG4->CNT = 0xD928U;
    }
    RTWDOG4->TOVAL = 0xFFFF;
    RTWDOG4->CS    = (uint32_t)((RTWDOG4->CS) & ~RTWDOG_CS_EN_MASK) | RTWDOG_CS_UPDATE_MASK;

    /* USED_WDOG */
    USED_WDOG->CNT   = RTWDOG_UPDATE_KEY; /* Unlock sequence */
    USED_WDOG->TOVAL = (uint16_t)0xFFFF;  /* Write any nonzero value */
    /* Clear the EN bit to disable watchdog */
    USED_WDOG->CS = (uint32_t)((USED_WDOG->CS) & ~RTWDOG_CS_EN_MASK);

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
    clock_ip_name_t gpt_clock = kCLOCK_Gpt1;

    CLOCK_EnableClock(gpt_clock);

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

    /* set prescaler */
    if (GPT1->CR & GPT_CR_EN_24M_MASK)
    {
        if (prescaler > 0 && prescaler <= GPT_PR_PRESCALER24M_MASK + 1)
        {
            GPT1->PR = GPT_PR_PRESCALER24M(prescaler - 1);
        }
    }
    else
    {
        if (prescaler > 0 && prescaler <= GPT_PR_PRESCALER_MASK + 1)
        {
            GPT1->PR = GPT_PR_PRESCALER(prescaler - 1);
        }
    }

    GPT1->OCR[0] = GPT_OCR_COMP(compare); /* Compare value */

    /* Choose whether generates interrupt or not */
    // GPT1->IR = GPT_IR_OF1IE_MASK; /* Output compare 1 interrupt enable */
    // NVIC_EnableIRQ(GPT1_IRQn);    /* Enable interrupt */

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
    clock_ip_name_t gpt_clock = kCLOCK_Gpt2;

    CLOCK_EnableClock(gpt_clock);

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
    // GPT2->IR = GPT_IR_OF1IE_MASK; /* Output compare 1 interrupt enable */
    // NVIC_EnableIRQ(GPT2_IRQn);    /* Enable interrupt */

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
    SysTick->VAL  = 0;
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
 * @return  None
 */
void SerialInit()
{
    const lpuart_config_t LPUART_1_config = {.baudRate_Bps    = UART_BAUD_RATE,
                                             .parityMode      = kLPUART_ParityDisabled,
                                             .dataBitsCount   = kLPUART_EightDataBits,
                                             .isMsb           = false,
                                             .stopBitCount    = kLPUART_OneStopBit,
                                             .txFifoWatermark = 0,
                                             .rxFifoWatermark = 0,
                                             .enableRxRTS     = false,
                                             .enableTxCTS     = false,
                                             .txCtsSource     = kLPUART_CtsSourcePin,
                                             .txCtsConfig     = kLPUART_CtsSampleAtStart,
                                             .rxIdleType      = kLPUART_IdleTypeStartBit,
                                             .rxIdleConfig    = kLPUART_IdleCharacter1,
                                             .enableTx        = true,
                                             .enableRx        = true};

    LPUART_Init(APPLICATION_SERIAL_BASE, &LPUART_1_config, BOARD_DebugConsoleSrcFreq());

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
    gpio_pin_config_t portConfig = {pinDir ? kGPIO_DigitalOutput : kGPIO_DigitalInput, 0U, kGPIO_NoIntmode};

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
    /* Enable clock to ADC1 module */
    CLOCK_EnableClock(kCLOCK_Lpadc1);

    /* Reset ADC module */
    TESTED_ADC->CTRL |= ADC_CTRL_RST_MASK;
    TESTED_ADC->CTRL &= ~ADC_CTRL_RST_MASK;

    /* Reset FIFO(s) */
#if (defined(FSL_FEATURE_LPADC_FIFO_COUNT) && (FSL_FEATURE_LPADC_FIFO_COUNT == 2))
    TESTED_ADC->CTRL |= ADC_CTRL_RSTFIFO0_MASK;
    TESTED_ADC->CTRL |= ADC_CTRL_RSTFIFO1_MASK;
#else
    TESTED_ADC->CTRL |= ADC_CTRL_RSTFIFO_MASK;
#endif /* FSL_FEATURE_LPADC_FIFO_COUNT */

    /* Disable ADC */
    TESTED_ADC->CTRL &= ~ADC_CTRL_ADCEN_MASK;

    /* Conf ADC CFG register */
    TESTED_ADC->CFG |= ADC_CFG_PUDLY(0x80)    /* Power up delay. */
                       | ADC_CFG_REFSEL(0U)   /* Reference voltage. */
                       | ADC_CFG_PWRSEL(3U)   /* Power configuration. */
                       | ADC_CFG_TPRICTRL(0U) /* Trigger priority policy. */
                       | ADC_CFG_PWREN(1U);   /* LPADC Analog Pre-Enable */

    /* Disable PAUSE between conversions */
    TESTED_ADC->PAUSE = 0U;
    
    TESTED_ADC->TCTRL[0] = ADC_TCTRL_TCMD(1);

    /* Enable ADC */
    TESTED_ADC->CTRL |= ADC_CTRL_ADCEN_MASK;
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
#if defined(__CC_ARM) || defined(__ARMCC_VERSION)
    extern uint32_t Image$$RW_m_ncache$$Base[];
    /* RW_m_ncache_unused is a auxiliary region which is used to get the whole size of noncache section */
    extern uint32_t Image$$RW_m_ncache_unused$$Base[];
    extern uint32_t Image$$RW_m_ncache_unused$$ZI$$Limit[];
    uint32_t nonCacheStart = (uint32_t)Image$$RW_m_ncache$$Base;
    uint32_t size          = ((uint32_t)Image$$RW_m_ncache_unused$$Base == nonCacheStart) ?
                        0 :
                        ((uint32_t)Image$$RW_m_ncache_unused$$ZI$$Limit - nonCacheStart);
#elif defined(__MCUXPRESSO)
#if defined(__USE_SHMEM)
    extern uint32_t __base_rpmsg_sh_mem;
    extern uint32_t __top_rpmsg_sh_mem;
    uint32_t nonCacheStart = (uint32_t)(&__base_rpmsg_sh_mem);
    uint32_t size          = (uint32_t)(&__top_rpmsg_sh_mem) - nonCacheStart;
#else
    extern uint32_t __NCACHE_REGION_START;
    extern uint32_t __NCACHE_REGION_SIZE;
    uint32_t nonCacheStart = (uint32_t)(&__NCACHE_REGION_START);
    uint32_t size          = (uint32_t)(&__NCACHE_REGION_SIZE);
#endif
#elif defined(__ICCARM__) || defined(__GNUC__)
    extern uint32_t __NCACHE_REGION_START[];
    extern uint32_t __NCACHE_REGION_SIZE[];
    uint32_t nonCacheStart = (uint32_t)__NCACHE_REGION_START;
    uint32_t size          = (uint32_t)__NCACHE_REGION_SIZE;
#endif
    volatile uint32_t i = 0;

#if defined(__ICACHE_PRESENT) && __ICACHE_PRESENT
    /* Disable I cache and D cache */
    if (SCB_CCR_IC_Msk == (SCB_CCR_IC_Msk & SCB->CCR))
    {
        SCB_DisableICache();
    }
#endif
#if defined(__DCACHE_PRESENT) && __DCACHE_PRESENT
    if (SCB_CCR_DC_Msk == (SCB_CCR_DC_Msk & SCB->CCR))
    {
        SCB_DisableDCache();
    }
#endif

    /* Disable MPU */
    ARM_MPU_Disable();

#if WATCHDOG_ENABLED
    Watchdog_refresh; /********************************************************/
#endif

    /* MPU configure:
     * Use ARM_MPU_RASR(DisableExec, AccessPermission, TypeExtField, IsShareable, IsCacheable, IsBufferable,
     * SubRegionDisable, Size)
     * API in mpu_armv7.h.
     * param DisableExec       Instruction access (XN) disable bit,0=instruction fetches enabled, 1=instruction fetches
     * disabled.
     * param AccessPermission  Data access permissions, allows you to configure read/write access for User and
     * Privileged mode.
     *      Use MACROS defined in mpu_armv7.h:
     * ARM_MPU_AP_NONE/ARM_MPU_AP_PRIV/ARM_MPU_AP_URO/ARM_MPU_AP_FULL/ARM_MPU_AP_PRO/ARM_MPU_AP_RO
     * Combine TypeExtField/IsShareable/IsCacheable/IsBufferable to configure MPU memory access attributes.
     *  TypeExtField  IsShareable  IsCacheable  IsBufferable   Memory Attribtue    Shareability        Cache
     *     0             x           0           0             Strongly Ordered    shareable
     *     0             x           0           1              Device             shareable
     *     0             0           1           0              Normal             not shareable   Outer and inner write
     * through no write allocate
     *     0             0           1           1              Normal             not shareable   Outer and inner write
     * back no write allocate
     *     0             1           1           0              Normal             shareable       Outer and inner write
     * through no write allocate
     *     0             1           1           1              Normal             shareable       Outer and inner write
     * back no write allocate
     *     1             0           0           0              Normal             not shareable   outer and inner
     * noncache
     *     1             1           0           0              Normal             shareable       outer and inner
     * noncache
     *     1             0           1           1              Normal             not shareable   outer and inner write
     * back write/read acllocate
     *     1             1           1           1              Normal             shareable       outer and inner write
     * back write/read acllocate
     *     2             x           0           0              Device              not shareable
     *  Above are normal use settings, if your want to see more details or want to config different inner/outter cache
     * policy.
     *  please refer to Table 4-55 /4-56 in arm cortex-M7 generic user guide <dui0646b_cortex_m7_dgug.pdf>
     * param SubRegionDisable  Sub-region disable field. 0=sub-region is enabled, 1=sub-region is disabled.
     * param Size              Region size of the region to be configured. use ARM_MPU_REGION_SIZE_xxx MACRO in
     * mpu_armv7.h.
     */

    /* Region 0 setting: No data access permission. */
    MPU->RBAR = ARM_MPU_RBAR(0, 0x00000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_NONE, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_4GB);

    /* Region 1 setting: Memory with Device type, not shareable, non-cacheable. */
    MPU->RBAR = ARM_MPU_RBAR(1, 0x80000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_512MB);

    /* Region 2 setting: Memory with Device type, not shareable,  non-cacheable. */
    MPU->RBAR = ARM_MPU_RBAR(2, 0x60000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_512MB);

    /* Region 3 setting: Memory with Device type, not shareable, non-cacheable. */
    MPU->RBAR = ARM_MPU_RBAR(3, 0x00000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_1GB);

    /* Region 4 setting: Memory with Normal type, not shareable, outer/inner write back */
    MPU->RBAR = ARM_MPU_RBAR(4, 0x00000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_256KB);

    /* Region 5 setting: Memory with Normal type, not shareable, outer/inner write back */
    MPU->RBAR = ARM_MPU_RBAR(5, 0x20000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_256KB);

    /* Region 6 setting: Memory with Normal type, not shareable, outer/inner write back */
    MPU->RBAR = ARM_MPU_RBAR(6, 0x20200000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_1MB);

    /* Region 7 setting: Memory with Normal type, not shareable, outer/inner write back */
    MPU->RBAR = ARM_MPU_RBAR(7, 0x20300000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_512KB);

#if defined(XIP_EXTERNAL_FLASH) && (XIP_EXTERNAL_FLASH == 1)
    /* Region 8 setting: Memory with Normal type, not shareable, outer/inner write back. */
    MPU->RBAR = ARM_MPU_RBAR(8, 0x30000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_RO, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_16MB);
#endif

#ifdef USE_SDRAM
    /* Region 9 setting: Memory with Normal type, not shareable, outer/inner write back */
    MPU->RBAR = ARM_MPU_RBAR(9, 0x80000000U);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_64MB);
#endif

    while ((size >> i) > 0x1U)
    {
        i++;
#if WATCHDOG_ENABLED
        Watchdog_refresh; /********************************************************/
#endif
    }

    if (i != 0)
    {
        /* The MPU region size should be 2^N, 5<=N<=32, region base should be multiples of size. */
        assert(!(nonCacheStart % size));
        assert(size == (uint32_t)(1 << i));
        assert(i >= 5);

        /* Region 10 setting: Memory with Normal type, not shareable, non-cacheable */
        MPU->RBAR = ARM_MPU_RBAR(10, nonCacheStart);
        MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 1, 0, 0, 0, 0, i - 1);
    }

    /* Region 11 setting: Memory with Device type, not shareable, non-cacheable */
    MPU->RBAR = ARM_MPU_RBAR(11, 0x40000000);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_16MB);

    /* Region 12 setting: Memory with Device type, not shareable, non-cacheable */
    MPU->RBAR = ARM_MPU_RBAR(12, 0x41000000);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_2MB);

    /* Region 13 setting: Memory with Device type, not shareable, non-cacheable */
    MPU->RBAR = ARM_MPU_RBAR(13, 0x41400000);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_1MB);

    /* Region 14 setting: Memory with Device type, not shareable, non-cacheable */
    MPU->RBAR = ARM_MPU_RBAR(14, 0x41800000);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_1MB);

    /* Region 15 setting: Memory with Device type, not shareable, non-cacheable */
    MPU->RBAR = ARM_MPU_RBAR(15, 0x42000000);
    MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_1MB);

    /* Enable MPU */
    ARM_MPU_Enable(MPU_CTRL_PRIVDEFENA_Msk);

#if WATCHDOG_ENABLED
    Watchdog_refresh; /********************************************************/
#endif

    /* Enable I cache and D cache */
#if defined(__DCACHE_PRESENT) && __DCACHE_PRESENT
    SCB_EnableDCache();
#endif
#if defined(__ICACHE_PRESENT) && __ICACHE_PRESENT
    SCB_EnableICache();
#endif
}
