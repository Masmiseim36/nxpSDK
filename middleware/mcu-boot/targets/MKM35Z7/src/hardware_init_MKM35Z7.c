/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "bootloader_common.h"
#include "bl_context.h"
#include "fsl_device_registers.h"
#include "fsl_uart.h"
#include "kinetis_family.h"
#include "smc.h"
#include "peripherals_pinmux.h"
#include "pin_mux.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////
#define BOOT_PIN_NUMBER 1
#define BOOT_PIN_PORT PORTD
#define BOOT_PIN_GPIO GPIOD
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
#if defined(BL_TARGET_FLASH)
    exit_vlpr();
#endif

    // Enable all the ports
    SIM->SCGC5 |= (SIM_SCGC5_PORTI_MASK | SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTE_MASK |
                   SIM_SCGC5_PORTF_MASK | SIM_SCGC5_PORTA_MASK);
    SIM->SCGC6 |= (SIM_SCGC6_PORTK_MASK | SIM_SCGC6_PORTL_MASK);
    
    // Load the user configuration data so that we can configure the clocks
    g_bootloaderContext.propertyInterface->load_user_config();
}

void deinit_hardware(void)
{
    SIM->SCGC5 &= (uint32_t) ~(SIM_SCGC5_PORTI_MASK | SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTD_MASK |
                               SIM_SCGC5_PORTE_MASK | SIM_SCGC5_PORTF_MASK | SIM_SCGC5_PORTA_MASK);
    SIM->SCGC6 &= (uint32_t) ~(SIM_SCGC6_PORTK_MASK | SIM_SCGC6_PORTL_MASK);
}

uint32_t get_bus_clock(void)
{
    uint32_t busClockDivider = ((SIM->CLKDIV1 & SIM_CLKDIV1_CLKDIVBUS_MASK) >> SIM_CLKDIV1_CLKDIVBUS_SHIFT) + 1;
    return (SystemCoreClock / busClockDivider);
}

uint32_t get_uart_clock(uint32_t instance)
{
    switch (instance)
    {
        case 0:
            // UART0 uses bus clock
            return get_bus_clock();
        case 1:
            // UART1 uses the system clock
            return SystemCoreClock;
        case 2:
            // UART2 uses bus clock
            return get_bus_clock();
        case 3:
            // UART3 uses the system clock
            return SystemCoreClock;
        case 4:
        case 5:
        default:
            return 0;
    }
}

bool is_boot_pin_asserted(void)
{  
#ifdef BL_TARGET_FLASH
    BOARD_InitBootPin();

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
