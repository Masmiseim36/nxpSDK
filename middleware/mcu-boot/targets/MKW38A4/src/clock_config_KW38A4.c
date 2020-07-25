/*
 * Copyright (c) 2014-2015, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "bootloader_common.h"
#include "bootloader/bl_context.h"
#include "property/property.h"
#include "fsl_device_registers.h"
#include <assert.h>

////////////////////////////////////////////////////////////////////////////////
// Declarations
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////
// See bootloader_common.h for documentation on this function.
void configure_clocks(bootloader_clock_option_t option)
{
  
//#if BL_TARGET_FLASH
#if 0
    static uint32_t s_defaultClockDivider;
    if (option == kClockOption_EnterBootloader)
    {
        s_defaultClockDivider = SIM->CLKDIV1;

        // General procedure to be implemented:
        // 1. Read clock flags and divider from bootloader config in property store
        bootloader_configuration_data_t *config = &g_bootloaderContext.propertyInterface->store->configurationData;
        uint8_t options = config->clockFlags;

        // 2. If NOT High Speed, do nothing (use reset clock config)
        if (options & kClockFlag_HighSpeed)
        {
            // High speed flag is set (meaning disabled), so just use default clocks.
            uint32_t coreClockDivider = (((SIM->CLKDIV1 & SIM_CLKDIV1_OUTDIV1_MASK) >> SIM_CLKDIV1_OUTDIV1_SHIFT) + 1);
            SystemCoreClock = kDefaultClock / coreClockDivider;
            return;
        }

        // 3. Set OUTDIV1 based on divider in config. 
        // The divider values are masked by the maximum bits per divider.
        uint32_t div1 = ((~config->clockDivider) & (SIM_CLKDIV1_OUTDIV1_MASK >> SIM_CLKDIV1_OUTDIV1_SHIFT)) + 1;

        // 4. Get MCGOUTCLK
        uint32_t McgOutClk = kDefaultClock;

        // 5. Keep core clock up kMinCoreClockWithUsbSupport if usb is supported.
        uint32_t freq = McgOutClk;

        // 6. Keep core clock below kMaxCoreClock
        while ((freq / div1) > kMaxCoreClock)
        {
            ++div1;
        }

        assert((div1 >= kDivider_Min) && (div1 <= kDivider_Max));

        uint32_t div4 = div1;

        // 8. Keep flash freq below max.
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

//        if ((div1 == 1) && ( div4 > 8))
//        {
//            return;
//        }

        // 9. Now set the dividers
        SIM->CLKDIV1 = SIM_CLKDIV1_OUTDIV1(div1 - 1) | 
                       SIM_CLKDIV1_OUTDIV4(div4 - 1); // Update system prescalers

        // 10. Update SystemCoreClock global.
        SystemCoreClock = McgOutClk / div1;
    }
    else if (option == kClockOption_ExitBootloader)
    {
        SIM->CLKDIV1 = s_defaultClockDivider;
    }

#endif // BL_TARGET_FLASH
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
