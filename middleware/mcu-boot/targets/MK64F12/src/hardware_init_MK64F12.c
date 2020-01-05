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
#include "smc.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

#define BOOT_PIN_NUMBER 6
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
    exit_vlpr();

    // Disable the SYSMPU otherwise USB cannot access the bus
    SYSMPU->CESR = 0;

    // Enable all the ports
    SIM->SCGC5 |= (SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTD_MASK |
                   SIM_SCGC5_PORTE_MASK);

    // Enable flash access for crossbar swicth master 4 (USB FS/LS)
    FMC->PFAPR |= FMC_PFAPR_M4AP_MASK;
    
    // Load the user configuration data so that we can configure the clocks
    g_bootloaderContext.propertyInterface->load_user_config();
}

void deinit_hardware(void)
{
    SIM->SCGC5 &= (uint32_t) ~(SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK |
                               SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTE_MASK);

    // Restore SIM_SOPTx related bits being changed
    SIM->SOPT1 &= (uint32_t)~SIM_SOPT1_USBREGEN_MASK;
    SIM->SOPT2 &= (uint32_t) ~(SIM_SOPT2_USBSRC_MASK | SIM_SOPT2_PLLFLLSEL_MASK);

    // Restore flash access for crossbar swicth master 4 (USB FS/LS)
    FMC->PFAPR &= (uint32_t)~FMC_PFAPR_M4AP_MASK;
}

bool usb_clock_init(void)
{
    SIM->SCGC4 &= ~SIM_SCGC4_USBOTG_MASK;

    SIM->CLKDIV2 = (uint32_t)0x00L;
    SIM->SOPT2 |= SIM_SOPT2_USBSRC_MASK | SIM_SOPT2_PLLFLLSEL(0x03);

    SIM->SCGC4 |= SIM_SCGC4_USBOTG_MASK;

    USB0->CLK_RECOVER_IRC_EN = 0x03;
    USB0->CLK_RECOVER_CTRL |= USB_CLK_RECOVER_CTRL_CLOCK_RECOVER_EN_MASK;

    USB0->CLK_RECOVER_CTRL |= 0x20;

    return true;
}

uint32_t get_bus_clock(void)
{
    uint32_t busClockDivider = ((SIM->CLKDIV1 & SIM_CLKDIV1_OUTDIV2_MASK) >> SIM_CLKDIV1_OUTDIV2_SHIFT) + 1;
    return (SystemCoreClock / busClockDivider);
}

uint32_t get_uart_clock(uint32_t instance)
{
    switch (instance)
    {
        case 0:
        case 1:
            // UART0 and UART1 always use the system clock
            return SystemCoreClock;
        case 2:
        case 3:
        case 4:
        case 5:
            // UART2, UART3, UART4, and UART5 always use the bus clock
            return get_bus_clock();
        default:
            return 0;
    }
}

bool is_boot_pin_asserted(void)
{
#ifdef BL_TARGET_FLASH
    // Initialize boot pin for GPIO
    BOOT_PIN_PORT->PCR[BOOT_PIN_NUMBER] |= PORT_PCR_MUX(BOOT_PIN_ALT_MODE);

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
