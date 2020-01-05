/*
 * Copyright (c) 2014-2015 Freescale Semiconductor, Inc.
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

typedef enum _clock_soruce_enum
{
    kClockSource_SystemOSC = 1u,
    kClockSource_SlowIRC = 2u,
    kClockSource_FastIRC = 3u,
    kClockSource_RtcOSC = 4u,
    kClockSource_SystemCFM = 5u,
    kClockSource_SystemPLL = 6u,
    kClockSource_UsbPhyPLL = 7u,
} clock_source_type_t;

enum
{
    kFIRC48M = 48000000ul,
    kFIRC52M = 52000000ul,
    kFIRC56M = 56000000ul,
    kFIRC60M = 60000000ul,
    kIRC8M = 8000000ul,
    kIRC2M = 2000000ul,
    //! The minimum core clock with usb workable is
    kMinCoreClockWithUsbSupport = 20000000u,
};

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////
//! return the frequency of HIRC
uint32_t get_firc_clock(void);
//! return the frequency of SIRC
uint32_t get_sirc_clock(void);

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////
uint32_t get_firc_clock(void)
{
    uint32_t range = (SCG->FIRCCFG & SCG_FIRCCFG_RANGE_MASK) >> SCG_FIRCCFG_RANGE_SHIFT;
    uint32_t clockFreq = 0;

    switch (range)
    {
        case 0:
            clockFreq = kFIRC48M;
            break;
        case 1:
            clockFreq = kFIRC52M;
            break;
        case 2:
            clockFreq = kFIRC56M;
            break;
        case 4:
            clockFreq = kFIRC60M;
            break;
        default:
            assert(false);
            break;
    }

    return clockFreq;
}

uint32_t get_sirc_clock(void)
{
    uint32_t range = (SCG->SIRCCFG & SCG_SIRCCFG_RANGE_MASK) >> SCG_SIRCCFG_RANGE_SHIFT;
    return range ? kIRC8M : kIRC2M;
}

// See bootloader_common.h for documentation on this function.
void configure_clocks(bootloader_clock_option_t option)
{
#if defined(BL_TARGET_FLASH)
    // General procedure to be implemented:
    // 1. Read clock flags and divider from bootloader config in property store
    bootloader_configuration_data_t *config = &g_bootloaderContext.propertyInterface->store->configurationData;
    uint8_t options = config->clockFlags;

    // Check if the USB HID peripheral is enabled. If it is enabled, we always
    // use the 48MHz IRC.
    bool isUsbEnabled = config->enabledPeripherals & kPeripheralType_USB_HID;

    // 2. If NOT High Speed and USB is NOT enabled, do nothing (use reset clock config)
    if ((options & kClockFlag_HighSpeed) && !isUsbEnabled)
    {
        // High speed flag is set (meaning disabled), so just use default clocks.
        return;
    }

    // 3. Set DIVCORE based on divider in config. OUTDIV4 starts out at /1.
    // The divider values are masked by the maximum bits per divider.
    uint32_t divCore = ((~config->clockDivider) & (SCG_CSR_DIVCORE_MASK >> SCG_CSR_DIVCORE_SHIFT)) + 1;
    uint32_t divSlow = ((SCG->CSR & SCG_CSR_DIVSLOW_MASK) >> SCG_CSR_DIVSLOW_SHIFT) + 1;

    uint32_t firc_clock = get_firc_clock();
    // If USB is enabled, the CPU clock must not be allowed to go below 20 MHz. So the max
    // DIVCORE divider is 2.
    if (isUsbEnabled)
    {
        while ((divCore * kMinCoreClockWithUsbSupport) > firc_clock)
        {
            divCore--;
            assert(divCore);
        }
    }

    // Update SystemCoreClock global.
    SystemCoreClock = firc_clock / divCore;

    // 4. Keep bus freq below max.
    //
    // The bus/flash clock is divided by DIVSLOW in addition to DIVCORE:
    //      SCGCLOCK -> DIVCORE -> DIVSLOW -> bus_clk
    uint32_t freq = SystemCoreClock;
    while ((kMaxBusClock * divSlow) < freq)
    {
        // Increase bus/flash clock divider.
        ++divSlow;
    }

    // 5. Now set the dividers before we switch to the 48MHz clock.
    uint32_t tmp = SCG->RCCR;
    tmp &= (uint32_t) ~(SCG_RCCR_DIVCORE_MASK | SCG_RCCR_DIVSLOW_MASK);
    tmp |= SCG_RCCR_DIVCORE(divCore - 1) | SCG_RCCR_DIVSLOW(divSlow - 1);
    SCG->RCCR = tmp;

    // 6. Turn on 48MHz IRC
    uint32_t clockSource = (SCG->CSR & SCG_CSR_SCS_MASK) >> SCG_CSR_SCS_SHIFT;
    ;
    if (clockSource != kClockSource_FastIRC)
    {
        SCG->FIRCCSR |= SCG_FIRCCSR_FIRCEN_MASK;

        tmp = SCG->RCCR;
        tmp &= (uint32_t)~SCG_RCCR_SCS_MASK;
        tmp |= SCG_RCCR_SCS(kClockSource_FastIRC);
        SCG->RCCR = tmp;
    }

    // Wait until the switch to HIRC is completed.
    while (!(SCG->FIRCCSR & SCG_FIRCCSR_FIRCVLD_MASK))
    {
    }
#endif // defined(BL_TARGET_FLASH)
}

// See bootloader_common.h for documentation on this function.
uint32_t get_system_core_clock(void)
{
    uint32_t systemCoreClock = SystemCoreClock;

    // Update SystemCoreClock out of reset.
    clock_source_type_t clocksource = (clock_source_type_t)((SCG->CSR & SCG_CSR_SCS_MASK) >> SCG_CSR_SCS_SHIFT);

    // Default clock source: FastIRC
    uint32_t systemClock = get_firc_clock();

    // Clock source is Slow IRC
    if (clocksource == kClockSource_SlowIRC)
    {
        systemClock = get_sirc_clock();
    }

    uint32_t coreClockDivider = ((SCG->CSR & SCG_CSR_DIVCORE_MASK) >> SCG_CSR_DIVCORE_SHIFT) + 1;
    systemCoreClock = systemClock / coreClockDivider;

    return systemCoreClock;
}

// See bootloader_common.h for documentation on this function.
uint32_t get_bus_clock(void)
{
    uint32_t busClockDivider = ((SCG->CSR & SCG_CSR_DIVSLOW_MASK) >> SCG_CSR_DIVSLOW_SHIFT) + 1;
    return SystemCoreClock / busClockDivider;
}
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
