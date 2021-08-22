/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "bl_context.h"
#include "bootloader_common.h"
#include "fsl_device_registers.h"
#include "smc.h"
#include "target_config.h"
#if BL_FEATURE_CRC_CHECK
#include "bl_app_crc_check.h"
#endif
#include "pin_mux.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

void init_hardware(void)
{
    // If we are in VLPR mode, exit it
    //! @todo If exiting from VLPR mode, we need to raise/restore clocks afterwards.
    exit_vlpr();

    // Enable PORTA clock for LPUART0 and LPI2C0
    PCC->CLKCFG[PCC_PORTA_INDEX] = PCC_CLKCFG_CGC(1);
    // Enable PORTB clock for LPSPI0
    PCC->CLKCFG[PCC_PORTB_INDEX] = PCC_CLKCFG_CGC(1);
    // Enable PORTC clock for msCAN
    PCC->CLKCFG[PCC_PORTC_INDEX] = PCC_CLKCFG_CGC(1);
    // Enable PORTD clock for BOOT PIN
    PCC->CLKCFG[PCC_PORTD_INDEX] = PCC_CLKCFG_CGC(1);
    // Enable PORTE clock for BOOT PIN
    PCC->CLKCFG[PCC_PORTE_INDEX] = PCC_CLKCFG_CGC(1);

    // Load the user configuration data so that we can configure the clocks
    g_bootloaderContext.propertyInterface->load_user_config();
}

void deinit_hardware(void)
{
#if BL_FEATURE_CRC_CHECK && BL_FEATURE_CRC_ASSERT
    restore_crc_check_failure_pin();
#endif

    // Disable PORTA clock for LPUART0 and LPI2C0
    PCC->CLKCFG[PCC_PORTA_INDEX] = 0;
    // Disable PORTB clock for LPSPI0
    PCC->CLKCFG[PCC_PORTB_INDEX] = 0;
    // Disable PORTC clock for msCAN
    PCC->CLKCFG[PCC_PORTC_INDEX] = 0;
    // Disable PORTD clock for BOOT PIN
    PCC->CLKCFG[PCC_PORTD_INDEX] = 0;

    // Restore SIM/PCC related bits being changed
    PCC->CLKCFG[PCC_LPUART0_INDEX] = 0;
    PCC->CLKCFG[PCC_LPSPI0_INDEX] = 0;
    PCC->CLKCFG[PCC_LPI2C0_INDEX] = 0;
    PCC->CLKCFG[PCC_MSCAN0_INDEX] = 0;
}

uint32_t get_uart_clock(uint32_t instance)
{
    assert(instance < 3);
    switch (instance)
    {
        case 0:
            return CLOCK_GetIpFreq(kCLOCK_Lpuart0);
        case 1:
            return CLOCK_GetIpFreq(kCLOCK_Lpuart1);
        case 2:
            return CLOCK_GetIpFreq(kCLOCK_Lpuart2);
        default:
            return CLOCK_GetIpFreq(kCLOCK_Lpuart0);
    }
}

// See bootloader_common.h for documentation on this function.
uint32_t get_bus_clock(void)
{
    return SystemCoreClock / (((SCG->CSR & SCG_CSR_DIVSLOW_MASK) >> SCG_CSR_DIVSLOW_SHIFT) + 1);
}

bool is_boot_pin_asserted(void)
{
    // BUTTON_1 on FRDM-KE64Z board
    // GPIO, Pullup resistor enabled.

    uint32_t defaultPTD2 = PORTD->PCR[2];

    PORTD->PCR[2] = PORT_PCR_MUX(1) | PORT_PCR_PE(1) | PORT_PCR_PS(1);
    // Set GPIOD Pin 2 to input mode
    GPIOD->PDDR &= ~(1u << 2);

    // Note: The PTD2 is connected with a pull-up resistor, so ROM use the negative logic to determine whether BOOT Pin
    //       is asserted or not
    uint32_t readCount = 0;
    uint32_t assertCount = 0;
    while (readCount++ < 500)
    {
        assertCount += ((GPIOD->PDIR & (1u << 2)) ? 0 : 1);
        register uint32_t delayCnt = 10;
        while (delayCnt--)
        {
            __NOP();
        }
    }

    // Restore to default GPIO setting
    PORTD->PCR[2] = defaultPTD2;

    return (assertCount > 250 ? true : false);
}

#if DEBUG && __ICCARM__

// Included only to satisfy linker.
size_t __write(int handle, const unsigned char *buf, size_t size)
{
    return size;
}

#endif // DEBUG && __ICCARM__

void debug_init(void) {}

void update_available_peripherals()
{
    // Disable CAN peripheral if it is not present
    if (!(PCC->CLKCFG[PCC_MSCAN0_INDEX] & PCC_CLKCFG_PR_MASK))
    {
        g_bootloaderContext.propertyInterface->store->configurationData.enabledPeripherals &= ~kPeripheralType_CAN;
    }
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
