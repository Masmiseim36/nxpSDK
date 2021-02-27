/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "bl_flexspi.h"
#include "bootloader_common.h"
#include "fsl_device_registers.h"
#include "microseconds.h"
#include "target_config.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

enum
{
    kFreq_1MHz = 1000000u,
    kFreq_4MHz = 4 * kFreq_1MHz,
    kFreq_24MHz = 24u * kFreq_1MHz,
    kFreq_48MHz = 48u * kFreq_1MHz,
    kFreq_198MHz = 198u * kFreq_1MHz,
    kFreq_264MHz = 264u * kFreq_1MHz,
    kFreq_16MHz = 16u * kFreq_1MHz,
    kFreq_96MHz = 96u * kFreq_1MHz,
    kFreq_240MHz = 240u * kFreq_1MHz,
    kDefaultUsbPllLockTimeUs = 200000u,
};

enum
{
    MAINCLKSELA_12MHz_IRC = 0,
    MAINCLKSELA_OSC_CLK,
    MAINCLKSELA_1M_LPOSC,
    MAINCLKSELA_48M_60M_IRC,
};

enum
{
    PLL528_CLKSRC_16M_IRC = 0,
    PLL528_CLKSRC_OSC,
    PLL528_CLKSRC_48MHz_60MHz_Div2
};

enum
{
    MAINCLKSELB_SYSCLK,
    MAINCLKSELB_16M_IRC,
    MAINCLKSELB_MAIN_PLL_CLK,
    MAINCLKSELB_32K_CLK
};

enum
{
    SYSOSCSEL_XTAL_CLK = 0,
    SYSOSCSEL_CLKIN_CLK,
};

enum
{
    USBHSFCLKSEL_OSC_CLK = 0,
    USBHSFCLKSEL_MAIN_CLK,
};

enum
{
    kOspiClockSrc_MainClk = 0,
    kOspiClockSrc_MainPllClk,
    kOspiClockSrc_Aux0PllClk,
    kOspiClockSrc_48M_60M_Clk,
    kOspiClockSrc_Aux1PllClk,
};
////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

// See bootloader_common for documentation on this function.
void configure_clocks(bootloader_clock_option_t option)
{
    SystemCoreClockUpdate();

    debug_printf("SystemCoreClock=%dMHz\r\n", SystemCoreClock / 1000000UL);
    debug_printf("FlexSPI1 Clock = %dMHz\r\n", CLOCK_GetFreq(kCLOCK_Flexspi0Clk) / 1000000UL);
    uint32_t currentClkSrc =
        (CLKCTL0->FLEXSPI0FCLKSEL & CLKCTL0_FLEXSPI0FCLKSEL_SEL_MASK) >> CLKCTL0_FLEXSPI0FCLKSEL_SEL_SHIFT;
    uint32_t currentClkDiv = CLKCTL0->FLEXSPI0FCLKDIV & 0xFFU;
    debug_printf("FlexSPI1 Clock source = %d, clock divider = %d\r\n", currentClkSrc, currentClkDiv);
}

// See bootloader_common.h for documentation on this function.
// Note: this function doesn't apply to FPGA build
uint32_t get_system_core_clock(void)
{
    return CLOCK_GetFreq(kCLOCK_CoreSysClk);
}

// See bootloader_common.h for documentation on this function.
uint32_t get_bus_clock(void)
{
    return get_system_core_clock();
}

void flexspi_clock_config(uint32_t instance, uint32_t freqOption, uint32_t sampleClkMode) {}

//!@brief Gate on the clock for the FlexSPI peripheral
void flexspi_clock_gate_enable(uint32_t instance) {}

//!@brief Gate off the clock the FlexSPI peripheral
void flexspi_clock_gate_disable(uint32_t instance) {}

//!@brief Get Clock for FlexSPI peripheral
status_t flexspi_get_clock(uint32_t instance, flexspi_clock_type_t type, uint32_t *freq)
{
    return kStatus_Success;
}

void flexspi_sw_delay_us(uint64_t us)
{
    while (us--)
    {
        microseconds_delay(1);
    }
}

// Get max supported Frequency in this SoC
status_t flexspi_get_max_supported_freq(uint32_t instance, uint32_t *freq, uint32_t clkMode)
{
    return kStatus_Success;
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
