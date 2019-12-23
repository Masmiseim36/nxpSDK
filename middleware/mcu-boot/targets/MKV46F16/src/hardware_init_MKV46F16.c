/*
 * Copyright (c) 2013-2015 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "bootloader_common.h"
#include "bootloader/bl_context.h"
#include "fsl_device_registers.h"
#include "fsl_uart.h"
#include "utilities/kinetis_family.h"
#include "smc.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

#define BOOT_PIN_NUMBER 7
#define BOOT_PIN_PORT PORTC
#define BOOT_PIN_GPIO GPIOC
#define BOOT_PIN_ALT_MODE 1

#define BOOT_PIN_DEBOUNCE_READ_COUNT 500

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////
void init_hardware(void)
{
#if BL_TARGET_FLASH
    exit_vlpr();
#endif

    // Enable all the ports
    SIM->SCGC5 |= (SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTD_MASK |
                   SIM_SCGC5_PORTE_MASK);
#if !defined(KV46F16_SERIES)
    uint32_t tmp = SIM->SOPT2;
    tmp &= (uint32_t)~SIM_SOPT2_LPUARTSRC_MASK;
    tmp |= SIM_SOPT2_LPUARTSRC(1);
    SIM->SOPT2 = tmp; // Set the LPUART clock source to the IRC48Mhz clock
#endif
    
    // Load the user configuration data so that we can configure the clocks
    g_bootloaderContext.propertyInterface->load_user_config();
}

void deinit_hardware(void)
{
    SIM->SCGC5 &= (uint32_t) ~(SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK |
                               SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTE_MASK);
}

uint32_t get_bus_clock(void)
{
    uint32_t busClockDivider = ((SIM->CLKDIV1 & SIM_CLKDIV1_OUTDIV2_MASK) >> SIM_CLKDIV1_OUTDIV2_SHIFT) + 1;
    return (SystemCoreClock / busClockDivider);
}

uint32_t get_fast_peripheral_clock(void)
{
    uint32_t busClockDivider = ((SIM->CLKDIV1 & SIM_CLKDIV1_OUTDIV2_MASK) >> SIM_CLKDIV1_OUTDIV2_SHIFT) + 1;
    return (SystemCoreClock / busClockDivider);
}

uint32_t get_uart_clock(uint32_t instance)
{
    switch (instance)
    {
        case 0:
            // UART0 and UART1 always use the system clock
            return SystemCoreClock;
        default:
            return 0;
    }
}

bool is_boot_pin_asserted(void)
{
#ifdef BL_TARGET_FLASH
    // Initialize boot pin for GPIO
    BOOT_PIN_PORT->PCR[BOOT_PIN_NUMBER] = PORT_PCR_MUX(BOOT_PIN_ALT_MODE);
    // Set boot pin as an input
    BOOT_PIN_GPIO->PDDR &= (uint32_t) ~(1 << BOOT_PIN_NUMBER);
    // Set boot pin pullup enabled, pullup select, filter enable
    BOOT_PIN_PORT->PCR[BOOT_PIN_NUMBER] |= (PORT_PCR_PE_MASK | PORT_PCR_PS_MASK | PORT_PCR_PFE_MASK);

    uint32_t readCount = 0;

    // Sample the pin a number of times
    for (uint32_t i = 0; i < BOOT_PIN_DEBOUNCE_READ_COUNT; i++)
    {
        readCount += ((BOOT_PIN_GPIO->PDIR) >> BOOT_PIN_NUMBER) & 1;
    }

    // boot pin is pulled high so we are measuring lows, make sure most of our measurements
    // registered as low
    return (readCount < (BOOT_PIN_DEBOUNCE_READ_COUNT / 2));
#else
    // Boot pin for Flash only target
    return false;
#endif
}

void debug_init(void)
{
}

#if __ICCARM__

size_t __write(int handle, const unsigned char *buf, size_t size)
{
    return size;
}

#endif // __ICCARM__

void update_available_peripherals()
{
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
