/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "bootloader_common.h"
#include "bootloader/bl_context.h"
#include "fsl_device_registers.h"
#include "smc.h"
#include "target_config.h"
#if BL_ENABLE_CRC_CHECK
#include "bootloader/bl_app_crc_check.h"
#endif
////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

uint32_t busClock = 24000000u; //! 24MHz default bus clock

enum
{
    //! @brief Mask for the bit of RCM_MR[BOOTROM] indicating that the BOOTCFG0 pin was asserted.
    kBootedViaPinMask = 1
};

typedef enum _rom_clock_manage_option
{
    kClock_Init = 1,
    kClock_Restore = 2,
} rom_clock_manage_option_t;

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////
extern uint32_t get_firc_clock();

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

    // Select Clock source for LPUART0
    PCC->CLKCFG[PCC_LPUART0_INDEX] = PCC_CLKCFG_PCS((uint32_t)kClockSource_FastIRC);
    // Select Clock source for LPI2C0
    PCC->CLKCFG[PCC_LPSPI0_INDEX] = PCC_CLKCFG_PCS((uint32_t)kClockSource_FastIRC);
    // Select Clock source for LPSPI0
    PCC->CLKCFG[PCC_LPI2C0_INDEX] = PCC_CLKCFG_PCS((uint32_t)kClockSource_FastIRC);

    SystemCoreClock = get_system_core_clock();
}

void deinit_hardware(void)
{
#if BL_ENABLE_CRC_CHECK && BL_FEATURE_CRC_ASSERT
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

    return get_firc_clock() / 2;
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
    while(readCount++ < 500)
    {
        assertCount += ((GPIOD->PDIR & (1u << 2)) ? 0 : 1);
        register uint32_t delayCnt = 10;
        while(delayCnt--)
        {
            __NOP();
        }
    }

    // Restore to default GPIO setting
    PORTD->PCR[2] = defaultPTD2;

    return (assertCount > 250 ? true: false);
}

#if DEBUG && __ICCARM__

// Included only to satisfy linker.
size_t __write(int handle, const unsigned char *buf, size_t size)
{
    return size;
}

#endif // DEBUG && __ICCARM__

void debug_init(void)
{
}

void update_available_peripherals()
{
    // Disable CAN peripheral if it is not present
    if (!(PCC->CLKCFG[PCC_MSCAN0_INDEX] & PCC_CLKCFG_PR_MASK))
    {
        g_bootloaderContext.propertyInterface->store->configurationData.enabledPeripherals &= ~kPeripheralType_CAN;
    }
}

// Note: This function is added to satisfy the MDK when the microlib is selected
void __aeabi_assert(const char *file, const char *line, int val)
{
    if (val == 0u)
    {
        while(1)
        {
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
