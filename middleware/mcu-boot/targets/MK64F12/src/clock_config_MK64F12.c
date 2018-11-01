/*
 * Copyright (c) 2013-2015 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "bootloader_common.h"
#include "bootloader/bl_context.h"
#include "property/property.h"
#include "fsl_device_registers.h"
#include "utilities/fsl_assert.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

// Clock mode types
typedef enum _target_clock_mode
{
    kClockMode_FEI_48MHz = 0,
    kClockMode_FEI_21MHz = 1,
    kClockMode_Default = kClockMode_FEI_21MHz,
} target_clock_mode_t;

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////

// This function implements clock mode switch between FEI and PEE mode used in this bootloader
void clock_mode_switch(const target_clock_mode_t currentMode, const target_clock_mode_t expectedMode);

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

// See bootloader_common for documentation on this function.
void configure_clocks(bootloader_clock_option_t option)
{
    static target_clock_mode_t s_currentClockMode = kClockMode_FEI_21MHz;
    static uint32_t s_defaultClockDivider;

    if (option == kClockOption_EnterBootloader)
    {
        s_defaultClockDivider = SIM->CLKDIV1;

        // General procedure to be implemented:
        // 1. Read clock flags and divider from bootloader config in property store
        bootloader_configuration_data_t *config = &g_bootloaderContext.propertyInterface->store->configurationData;
        uint8_t options = config->clockFlags;

        // Check if the USB HID peripheral is enabled. If it is enabled, we always
        // configure the MCGOUT to 48MHz.
        bool isUsbEnabled = config->enabledPeripherals & kPeripheralType_USB_HID;

        // 2. If NOT High Speed and USB isn't enabled, do nothing (use reset clock config)
        if ((options & kClockFlag_HighSpeed) && (!isUsbEnabled))
        {
            // Get actual Core clock.
            SystemCoreClock =
                kDefaultClock / (((SIM->CLKDIV1 & SIM_CLKDIV1_OUTDIV1_MASK) >> SIM_CLKDIV1_OUTDIV1_SHIFT) + 1);

            // High speed flag is set (meaning disabled), so just use default clocks.
            return;
        }

        // 3. Set OUTDIV1 based on divider in config. OUTDIV4 starts out at /1.
        // The divider values are masked by the maximum bits per divider.
        uint32_t div1 = ((~config->clockDivider) & (SIM_CLKDIV1_OUTDIV1_MASK >> SIM_CLKDIV1_OUTDIV1_SHIFT)) + 1;

        // 4. Get MCGOUTCLK
        uint32_t McgOutClk = kHIRC;

        // 5. Keep core clock up kMinCoreClockWithUsbSupport if usb is supported.
        uint32_t freq = McgOutClk;
        // If USB is enabled, the CPU clock must not be allowed to go below 20 MHz
        if (isUsbEnabled)
        {
            while ((freq / div1) < kMinCoreClockWithUsbSupport)
            {
                --div1;
            }
        }

        // 6. Keep core clock below kMaxCoreClock
        while ((freq / div1) > kMaxCoreClock)
        {
            ++div1;
        }

        assert((div1 >= kDivider_Min) && (div1 <= kDivider_Max));

        uint32_t div2 = div1;
        uint32_t div3 = div1;
        uint32_t div4 = div1;

        // 7. Keep bus freq below max.
        //
        // The bus clock is divided by OUTDIV2:
        //      MCGOUTCLK -> OUTDIV2 -> bus_clk
        freq = McgOutClk;
        while ((freq / div2) > kMaxBusClock)
        {
            // Increase bus clock divider.
            ++div2;
        }
        assert((div2 >= kDivider_Min) && (div2 <= kDivider_Max));

        // 8. Keep FlexBus freq below max.
        //
        // The FlexBus clock is divided by OUTDIV3:
        // MCGOUTCLK -> OUTDIV3 -> flexbus_clk
        freq = McgOutClk;
        while ((freq / div3) > kMaxFlexBusClock)
        {
            // Increase flexbus clock divider.
            ++div3;
        }
        assert((div3 >= kDivider_Min) && (div3 <= kDivider_Max));

        // 9. Keep flash freq below max.
        //
        // The flash clock is diveded by OUTDIV4:
        //      MCGOUTCLK -> OUTDIV4 ->flash_clk
        freq = McgOutClk;
        while ((freq / div4) > kMaxFlashClock)
        {
            // Increase bus/flash clock divider.
            ++div4;
        }
        assert((div4 >= kDivider_Min) && (div4 <= kDivider_Max));

        if ((div1 == 1) && ((div2 > 8) || (div3 > 8) || (div4 > 8)))
        {
            return;
        }

        // 9. Now set the dividers
        SIM->CLKDIV1 = SIM_CLKDIV1_OUTDIV1(div1 - 1) | SIM_CLKDIV1_OUTDIV1(div1 - 1) | SIM_CLKDIV1_OUTDIV3(div3 - 1) |
                       SIM_CLKDIV1_OUTDIV4(div4 - 1); /* Update system prescalers */

        // 10. Update SystemCoreClock global.
        SystemCoreClock = McgOutClk / div1;

        clock_mode_switch(s_currentClockMode, kClockMode_FEI_48MHz);

        s_currentClockMode = kClockMode_FEI_48MHz;
    }
    else if (option == kClockOption_ExitBootloader)
    {
        // Restore from FEE mode to FEI mode
        clock_mode_switch(s_currentClockMode, kClockMode_FEI_21MHz);

        // Restore clock divider
        SIM->CLKDIV1 = s_defaultClockDivider;
    }
}

void clock_mode_switch(const target_clock_mode_t currentMode, const target_clock_mode_t expectedMode)
{
    // Note: here only implements clock switch between FEI and FEE,
    // The other modes are not supported.
    assert(currentMode == kClockMode_FEI_21MHz || currentMode == kClockMode_FEI_48MHz);
    assert(expectedMode == kClockMode_FEI_21MHz || expectedMode == kClockMode_FEI_48MHz);

    if (currentMode == expectedMode)
    {
        return;
    }

    if (expectedMode == kClockMode_FEI_48MHz)
    {
        /* Switch to FEI mode */
        uint8_t tmp = MCG->C4;
        tmp &= (uint8_t) ~(MCG_C4_DRST_DRS_MASK);
        tmp |= MCG_C4_DMX32_MASK;  // Multiply with 1464, MCGOUTCLK is 48Mhz
        tmp |= MCG_C4_DRST_DRS(1); // Reference range:32.768kHZ
        MCG->C4 = tmp;

        while (!(MCG->S & MCG_S_IREFST_MASK))
            ; // Wait until internal reference clock is ready.
    }
    else if (expectedMode == kClockMode_FEI_21MHz)
    {
        // Restore registers to default value out of reset.
        MCG->C4 &= (uint8_t) ~(MCG_C4_DRST_DRS_MASK | MCG_C4_DMX32_MASK);
    }
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
