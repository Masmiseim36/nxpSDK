/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "bootloader_common.h"
#include "bootloader/bl_context.h"
#include "property/property.h"
#include "fsl_device_registers.h"
#include "utilities/fsl_assert.h"
#include "target_config.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////
//! return the frequency of HIRC
uint32_t get_firc_clock(void);
//! return the frequency of SIRC
uint32_t get_sirc_clock(void);

const scg_sosc_config_t g_scgSysOscConfig_BOARD_BootClockRUN = {
    .freq = 8000000U, /* System Oscillator frequency: 8000000Hz */
    .enableMode = kSCG_SysOscEnable |
                  kSCG_SysOscEnableInLowPower, /* Enable System OSC clock, Enable System OSC in low power mode */
    .monitorMode = kSCG_SysOscMonitorDisable,  /* Monitor disabled */
    .div2 = kSCG_AsyncClkDivBy1,               /* System OSC Clock Divider 2: divided by 1 */
    .workMode = kSCG_SysOscModeOscLowPower,    /* Oscillator low power */
};

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////
uint32_t get_firc_clock(void)
{
    return kFIRC48M;
}

uint32_t get_sirc_clock(void)
{
    uint32_t range = (SCG->SIRCCFG & SCG_SIRCCFG_RANGE_MASK) >> SCG_SIRCCFG_RANGE_SHIFT;
    return range ? kIRC8M : kIRC2M;
}

// See bootloader_common.h for documentation on this function.
void configure_clocks(bootloader_clock_option_t option)
{
    static uint32_t defaultRCCR;
    if (option == kClockOption_EnterBootloader)
    {
        defaultRCCR = SCG->RCCR;
        // 0. Init SOSC according to board configuration.
        CLOCK_InitSysOsc(&g_scgSysOscConfig_BOARD_BootClockRUN);
        // Set the XTAL0 frequency based on board settings.
        CLOCK_SetXtal0Freq(g_scgSysOscConfig_BOARD_BootClockRUN.freq);

        // General procedure to be implemented:
        // 1. Read clock flags and divider from bootloader config in property store
        bootloader_configuration_data_t *config = &g_bootloaderContext.propertyInterface->store->configurationData;

        bool isMsCanEnabled = config->enabledPeripherals & kPeripheralType_CAN;

        // 2. If NOT High Speed is NOT enabled, do nothing (use reset clock config)
        uint8_t options = config->clockFlags;

        if ((options & kClockFlag_HighSpeed) && !isMsCanEnabled)
        {
            // High speed flag is set (meaning disabled), so just use default clocks.
            return;
        }

        // 3. Set DIVCORE based on divider in config.
        // The divider values are masked by the maximum bits per divider.
        uint32_t divCore = ((~config->clockDivider) & (SCG_CSR_DIVCORE_MASK >> SCG_CSR_DIVCORE_SHIFT)) + 1;
        uint32_t divSlow = ((SCG->CSR & SCG_CSR_DIVSLOW_MASK) >> SCG_CSR_DIVSLOW_SHIFT) + 1;;

        if (isMsCanEnabled)
        {
            divCore = 1;
        }

        uint32_t firc_clock = get_firc_clock();

        // Update SystemCoreClock global.
        SystemCoreClock = firc_clock / divCore;

        // 4. Keep bus freq below max.
        //
        // The bus clock is divided by DIVBUS in addition to DIVCORE:
        //      SCGCLOCK -> DIVSLOW -> bus_clk
        uint32_t freq = SystemCoreClock;
        while ((kMaxBusClock * divSlow) < freq)
        {
            // Increase bus clock divider.
            ++divSlow;
        }

        // 5. Keep flash freq below maximum supported frequency.
        //
        // The Flash clock is divided by DIVSLOW in addition to DIVCORE:
        //      SCGCLOK -> DIVSLOW -> flash_clk
        while ((kMaxFlashClock * divSlow) < freq)
        {
            // Increase slow clock divider.
            ++divSlow;
        }

        // 6. Now set the dividers before we switch to the 48MHz clock.
        uint32_t scg_rccr = SCG->RCCR;
        scg_rccr &= (uint32_t) ~(SCG_RCCR_DIVCORE_MASK | SCG_RCCR_DIVSLOW_MASK);
        scg_rccr |= SCG_RCCR_DIVCORE(divCore - 1) | SCG_RCCR_DIVSLOW(divSlow - 1);
        SCG->RCCR = scg_rccr;

        // 7. Switch to 48MHz FIRC
        uint32_t clockSource = (SCG->CSR & SCG_CSR_SCS_MASK) >> SCG_CSR_SCS_SHIFT;
        if (clockSource != kClockSource_FastIRC)
        {
            // Enable FIRC
            SCG->FIRCCSR |= SCG_FIRCCSR_FIRCEN_MASK;
            // Wait until  FIRC is enabled.
            while (!(SCG->FIRCCSR & SCG_FIRCCSR_FIRCVLD_MASK))
            {
            }
            SCG->RCCR = (SCG->RCCR & ~SCG_RCCR_SCS_MASK) | SCG_RCCR_SCS(kClockSource_FastIRC);
        }
        __DSB();
        __ISB();

        SCG->FIRCDIV = SCG_FIRCDIV_FIRCDIV1(2) | SCG_FIRCDIV_FIRCDIV2(2);
    }
    else
    {
        SCG->FIRCDIV = 0;
        SCG->RCCR = defaultRCCR;
    }

}

// See bootloader_common.h for documentation on this function.
// Note: this function doesn't apply to FPGA build
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

    systemCoreClock = systemClock / (((SCG->CSR & SCG_CSR_DIVCORE_MASK) >> SCG_CSR_DIVCORE_SHIFT) + 1);

    return systemCoreClock;
}

// See bootloader_common.h for documentation on this function.
uint32_t get_bus_clock(void)
{
    return SystemCoreClock / (((SCG->CSR & SCG_CSR_DIVSLOW_MASK) >> SCG_CSR_DIVSLOW_SHIFT) + 1);
}
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
