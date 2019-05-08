/*
 * Copyright (c) 2015 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 *
 *
 *
 *          This demo shows a simple led blinking example, to re-enter Bootloader,
 *          hold SW2 about 2 seconds, then you can talk to Bootloader via host tool again.
 *
 *
 *
 */

#include "fsl_device_registers.h"
#include "milliseconds_delay.h"
#include "misc.h"
#include "led_bsp.h"
#include <stdbool.h>

//! @brief Intialize necessary IPs.
void hardware_init(void);
//! @brief Toggle LEDs according to specified parameter leds.
void led_toggle(uint32_t leds);

//! @brief switch to PEE mode from FEE mode.
//! In this function, the QuadSPI source clock is changed to MCGFLL,
//! The QuadSPI serial clock divider is set to 1.
//! The SystemCoreClock is updated to 120MHz, the MCG is swithced from FEE to PEE mode.
#if defined(__ICCARM__)
#pragma section = "ramfunc_section"
#pragma section = "ramfunc_section_init"
void clock_change(void) @"ramfunc_section";
#elif defined(__CC_ARM)
extern uint32_t Load$$EXEC_m_ramfunc$$Base[];   // Base address for loading ram function
extern uint32_t Load$$EXEC_m_ramfunc$$Length[]; // Size of ram function
extern uint32_t Image$$EXEC_m_ramfunc$$Base[];
void clock_change(void) __attribute__((section("ramfunc_section"))); // Execute address of ram function
#elif defined(__GNUC__)
extern uint32_t ramfunc_load_address[];
extern uint32_t ramfunc_length[];
extern uint32_t ramfunc_execution_address[];
void clock_change(void) __attribute__((section("ramfunc_section"))); // Execute address of ram function
#endif

// Copy ram functions from ROM to RAM.
void copy_to_ram(void);

int main(void)
{
    copy_to_ram();
    clock_change();

    hardware_init();
    milliseconds_delay_init();

    uint32_t led_idx = LED0;
    while (1)
    {
        led_toggle(led_idx);
        milliseconds_delay(500);
        led_toggle(led_idx);

        led_idx <<= 1;
        if (led_idx > MAX_LED_IDX)
        {
            led_idx = LED0;
        }

        // Re-enter bootloader if the bootloader pin is clicked.
        if (is_bootloader_pin_clicked())
        {
            app_enter_bootloader();
        }
    }
}

void hardware_init(void)
{
#if defined(SIM_SCGC5_PORTA_MASK)
    SIM->SCGC5 |= (SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTD_MASK |
                   SIM_SCGC5_PORTE_MASK);
#else
    SIM->SCGC5 |=
        (SIM_SCGC5_PTA_MASK | SIM_SCGC5_PTB_MASK | SIM_SCGC5_PTC_MASK | SIM_SCGC5_PTD_MASK | SIM_SCGC5_PTE_MASK);
#endif

    // Enable the LED pins GPIO
    LED0_PORT->PCR[LED0_OFFSET] = PORT_PCR_MUX(1);
    LED1_PORT->PCR[LED1_OFFSET] = PORT_PCR_MUX(1);
#if MAX_LED_IDX == LED2
    LED2_PORT->PCR[LED2_OFFSET] = PORT_PCR_MUX(1);
#endif // #if MAX_LED_IDX == LED2

    // Configure port to output mode
    LED0_GPIO->PDDR |= (1 << LED0_OFFSET);
    LED1_GPIO->PDDR |= (1 << LED1_OFFSET);
#if MAX_LED_IDX == LED2
    LED2_GPIO->PDDR |= (1 << LED2_OFFSET);
#endif // #if MAX_LED_IDX == LED2

// If led will be turned on with low level, need to turn all of them off at startup.
#if LED_POLARITY == LED_ON_LOW_LEVEL
    LED0_GPIO->PTOR |= (1 << LED0_OFFSET);
    LED1_GPIO->PTOR |= (1 << LED1_OFFSET);
#if MAX_LED_IDX == LED2
    LED2_GPIO->PTOR |= (1 << LED2_OFFSET);
#endif // #if MAX_LED_IDX == LED2
#endif // #if LED_POLARITY == LED_ON_LOW_LEVEL

    bootloader_pin_init();
}

void led_toggle(uint32_t leds)
{
    if (leds & LED0)
    {
        LED0_GPIO->PTOR |= (1 << LED0_OFFSET);
    }
    if (leds & LED1)
    {
        LED1_GPIO->PTOR |= (1 << LED1_OFFSET);
    }
#if MAX_LED_IDX == LED2
    if (leds & LED2)
    {
        LED2_GPIO->PTOR |= (1 << LED2_OFFSET);
    }
#endif // #if MAX_LED_IDX == LED2
}

void clock_change(void)
{
    __disable_irq(); // Disable global variable before changing the clock in case an interrupt is triggered during
                     // change clock source.
                     // Disable QuadSPI module, avoid glitch during clock switching.
    QuadSPI0->MCR |= QuadSPI_MCR_MDIS_MASK;

/*
 * Configure CLKDIV1 first, in case some clock exceeds the maximum clock during clock switch.
 */
#if CPU_IS_ARM_CORTEX_M4
    /* SIM_CLKDIV1: OUTDIV1=1,OUTDIV2=1,OUTDIV3=1,OUTDIV4=4 */
    SIM->CLKDIV1 = 0x01140000U;
#elif CPU_IS_ARM_CORTEX_M0P
    /* SIM_CLKDIV1: OUTDIV1=1,OUTDIV2=5,OUTDIV4=5,OUTDIV5=1 */
    SIM->CLKDIV1 = 0x15051000U;
#endif

    // Restore to FEI mode before configuring external OSC.
    MCG->C4 &= (uint8_t) ~(MCG_C4_DMX32_MASK | MCG_C4_DRST_DRS_MASK);
    MCG->C1 |= MCG_C1_IREFS_MASK;
    while ((MCG->S & MCG_S_IREFST_MASK) == 0x00U)
    {
    }

/*
 *  Switch to PEE mode, reuse almost the codes from SystemInit()
 */
/* SIM_SCGC5: PORTA=1 */
#if defined(SIM_SCGC5_PORTA_MASK)
    SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;
#else
    SIM->SCGC5 |= SIM_SCGC5_PTA_MASK;
#endif
    /* Configure external OSC pinmux */
    PORTA->PCR[18] &= (uint32_t) ~(uint32_t)((PORT_PCR_ISF_MASK | PORT_PCR_MUX(0x07)));
    PORTA->PCR[19] &= (uint32_t) ~(uint32_t)((PORT_PCR_ISF_MASK | PORT_PCR_MUX(0x07)));
    MCG->SC = 0;     /* Set SC (fast clock internal reference divider) */
    MCG->C2 = 0x24U; /* Set C2 (freq. range, ext. and int. reference selection etc) */
    // Delay about 50us to ensure the C2[RANGE] is updated correctly.
    for (volatile uint32_t delayCnt = 0; delayCnt < 4800; delayCnt++)
    {
        __ASM("nop");
    }
    OSC->CR = 0x80U;                    /* Set OSC_CR (OSCERCLK enable, oscillator capacitor load) */
    MCG->C7 = 0;                        /* Set C7 (OSC Clock Select) */
    MCG->C1 = 0x22 | MCG_C1_CLKS(0x02); /* Set C1 (clock source selection, FLL ext. reference divider, int. reference
                                           enable etc.) - PBE mode*/
    while ((MCG->S & MCG_S_OSCINIT0_MASK) == 0x00U)
    {
    }

    /* Set C4 (FLL output; trim values not changed) */
    MCG->C4 &= (uint8_t) ~(MCG_C4_DMX32_MASK | MCG_C4_DRST_DRS_MASK);
    /* PLL clock can be used to generate clock for some devices regardless of clock generator (MCGOUTCLK) mode. */
    MCG->C5 = 0x00U; /* Set C5 (PLL settings, PLL reference divider etc.) */
#if CPU_IS_ARM_CORTEX_M4
    /* OSC is 12MHz, PLL clock = 12 * 20 / 2 = 120MHz */
    MCG->C6 = 0x44U & (uint8_t) ~(MCG_C6_PLLS_MASK); /* Set C6 (PLL select, VCO divider etc.) */
#elif CPU_IS_ARM_CORTEX_M0P
    /* OSC is 12MHz, PLL clock = 12 * 24 / 2 = 144MHz */
    MCG->C6 = 0x48U & (uint8_t) ~(MCG_C6_PLLS_MASK); /* Set C6 (PLL select, VCO divider etc.) */
#endif
    MCG->C6 |= MCG_C6_PLLS_MASK;
    while ((MCG->S & MCG_S_LOCK0_MASK) == 0x00U)
    { /* Wait until PLL is locked*/
    }
    MCG->C1 &= (uint8_t) ~(MCG_C1_CLKS_MASK);

    // Need to wait about 1ms until the MCGPLL is stable.
    for (volatile uint32_t delayCnt = 0; delayCnt < 60000; delayCnt++)
    {
        __ASM("nop");
    }

    /*
     *
     * Now switch the QuadSPI Clock source and Configure the Clock divider
     *
     */
    // If the serial clock divider is 1, need to change it to 2 because the maximum
    // supported clock under SDK mode is 100MHz while QuadSPI source clock is 120MHz.
    // For DDR mode, it is OK, because the actual serial clock is QuadSPI source clock / 4.
    uint32_t sclkcfg = (QuadSPI0->MCR & QuadSPI_MCR_SCLKCFG_MASK) >> QuadSPI_MCR_SCLKCFG_SHIFT;
    if (sclkcfg == 0U)
    {
        uint32_t tmp = QuadSPI0->MCR;
        tmp &= (uint32_t)~QuadSPI_MCR_SCLKCFG_MASK;
        tmp |= QuadSPI_MCR_SCLKCFG(1);
        QuadSPI0->MCR = tmp;
    }
    uint32_t tmp = QuadSPI0->SOCCR;
    tmp &= (uint32_t)~QuadSPI_SOCCR_QSPISRC_MASK;
    tmp |= QuadSPI_SOCCR_QSPISRC(2);
    QuadSPI0->SOCCR = tmp; // Change QuadSPI source clock to MCGPLL

    // Cleare AHB buffer after switching clock source.
    QuadSPI0->MCR &= (uint32_t)~QuadSPI_MCR_MDIS_MASK; // Re-enable QuadSPI module
    // Clear AHB buffer first here to avoid potential timing issue
    QuadSPI0->MCR |= QuadSPI_MCR_SWRSTHD_MASK | QuadSPI_MCR_SWRSTSD_MASK;
    // Wait ~10ms to ensure the AHB buffer is cleared.
    for (volatile uint32_t delayCnt = 0; delayCnt < 960; delayCnt++)
    {
        __ASM("nop");
    }
    // Disable module during the reset procedure
    QuadSPI0->MCR |= QuadSPI_MCR_MDIS_MASK;
    // Clear the reset bits.
    QuadSPI0->MCR &= (uint32_t) ~(QuadSPI_MCR_SWRSTHD_MASK | QuadSPI_MCR_SWRSTSD_MASK);
    QuadSPI0->MCR &= (uint32_t)~QuadSPI_MCR_MDIS_MASK; // Re-enable QuadSPI module

    __DSB();
    __ISB();

    // Enable global IRQ because the clock switch is complete.
    __enable_irq();

#if CPU_IS_ARM_CORTEX_M4
    SystemCoreClock = 120000000ul; // SystemCoreClock is 120MHz now.
#elif CPU_IS_ARM_CORTEX_M0P
    SystemCoreClock = 72000000ul;                    // SystemCoreClock is 72MHz now.
#endif
}

void copy_to_ram(void)
{
    uint8_t *codeRelocateRomStart;
    uint32_t codeRelocateSize;
    uint8_t *codeReloocateRamStart;
#if defined(__ICCARM__)
    codeRelocateRomStart = (uint8_t *)__section_begin("ramfunc_section_init");
    codeRelocateSize = (uint32_t)__section_size("ramfunc_section_init");
    codeReloocateRamStart = (uint8_t *)__section_begin("ramfunc_section");
#elif defined(__CC_ARM)
    codeRelocateRomStart = (uint8_t *)Load$$EXEC_m_ramfunc$$Base;
    codeRelocateSize = (uint32_t)Load$$EXEC_m_ramfunc$$Length;
    codeReloocateRamStart = (uint8_t *)Image$$EXEC_m_ramfunc$$Base;
#elif defined(__GNUC__)
    codeRelocateRomStart = (uint8_t *)ramfunc_load_address;
    codeRelocateSize = (uint32_t)ramfunc_length;
    codeReloocateRamStart = (uint8_t *)ramfunc_execution_address;
#endif

    while (codeRelocateSize)
    {
        *codeReloocateRamStart++ = *codeRelocateRomStart++;
        codeRelocateSize--;
    }
}

//! @brief Init bootloader pin
void bootloader_pin_init(void)
{
    // Configure Enter Bootloader pin (SW2-PTA4)
    PORTA->PCR[4] = PORT_PCR_MUX(1);     // Configure PTA21 to GPIO mode
    GPIOA->PDDR &= (uint32_t) ~(1 << 4); // Configure SW1 to input mode
}

//! @brief determine whether boot pin is asserted
bool is_bootloader_pin_clicked(void)
{
    static uint32_t count = 0;
    static bool isPinAsserted = false;
    static bool isPinReleased = false;
    if (!isPinAsserted)
    {
        if (!(GPIOA->PDIR & (1 << 4)))
        {
            count++;
        }
        else
        {
            count = 0;
        }

        if (count == 2)
        {
            isPinAsserted = true;
            count = 0;
        }
    }
    else
    {
        if (GPIOA->PDIR & (1 << 4))
        {
            count++;
        }
        else
        {
            count = 0;
        }

        if (count == 2)
        {
            isPinReleased = true;
            count = 0;
        }
    }

    return (isPinAsserted && isPinReleased);
}
