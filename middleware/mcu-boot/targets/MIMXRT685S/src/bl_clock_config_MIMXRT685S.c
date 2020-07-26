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
void configure_clocks(bootloader_clock_option_t option) {}

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

void flexspi_clock_config(uint32_t instance, uint32_t freqOption, uint32_t sampleClkMode)
{
    do
    {
        if ((freqOption < 1) || (freqOption > kFlexSpiSerialClk_200MHz) || (sampleClkMode > kFlexSpiClk_DDR))
        {
            break;
        }

        uint32_t clockSrc =
            (CLKCTL0->FLEXSPIFCLKSEL & CLKCTL0_FLEXSPIFCLKSEL_SEL_MASK) >> CLKCTL0_FLEXSPIFCLKSEL_SEL_SHIFT;
        uint32_t clockDiv = 1;
        uint32_t currentDiv =
            (CLKCTL0->FLEXSPIFCLKDIV & CLKCTL0_FLEXSPIFCLKDIV_DIV_MASK) >> CLKCTL0_FLEXSPIFCLKDIV_DIV_SHIFT;

        uint32_t needHaltClk = 0;
        if (clockSrc == kOspiClockSrc_48M_60M_Clk)
        {
            if (sampleClkMode == kFlexSpiClk_SDR)
            {
                if (freqOption < kFlexSpiSerialClk_50MHz)
                {
                    clockDiv = 2; // 24MHz
                }
                else
                {
                    clockDiv = 1; // 48MHz
                }
            }
            else // DDR mode
            {
                clockDiv = 1; // 48MHz / 4 = 12MHz
            }
        }
        if ((clockSrc == kOspiClockSrc_MainClk) || (clockSrc == kOspiClockSrc_MainPllClk))
        {
            uint32_t qspiSrcClk = CLOCK_GetMainClkFreq();
            if (clockSrc == kOspiClockSrc_MainPllClk)
            {
                qspiSrcClk = CLOCK_GetSysPfdFreq(kCLOCK_Pfd0);
            }

            uint32_t freqList[] = { 30, 30, 50, 60, 80, 100, 120, 133, 166, 200 };

            clockDiv = 1;
            uint32_t freqMHz = qspiSrcClk / kFreq_1MHz;
            if (sampleClkMode == kFlexSpiClk_DDR)
            {
                freqMHz /= 4;
            }
            bool hasFoundDiv = false;

            while (!hasFoundDiv)
            {
                uint32_t divFreq = freqMHz / clockDiv;
                if (divFreq <= freqList[freqOption])
                {
                    hasFoundDiv = true;
                    break;
                }
                else
                {
                    ++clockDiv;
                }
            }
        }
        else if ((clockSrc == kOspiClockSrc_Aux0PllClk) || (clockSrc == kOspiClockSrc_Aux1PllClk))
        {
            uint32_t expectedPfd = 24; // Aux0_pll will be configured to 396MHz
            if (sampleClkMode == kFlexSpiClk_SDR)
            {
                switch (freqOption)
                {
                    default:
                    case kFlexSpiSerialClk_30MHz:
                        clockDiv = 13; // 528 * 18 / 24 / 13 = 30.46
                        break;
                    case kFlexSpiSerialClk_50MHz:
                        clockDiv = 8; // 528 * 18 / 24 / 8 = 49
                        break;
                    case kFlexSpiSerialClk_60MHz:
                        expectedPfd = 32;
                        clockDiv = 5; // 528 * 18 / 32 / 5 = 59.4
                        break;
                    case kFlexSpiSerialClk_80MHz:
                        clockDiv = 5; // 528 * 18 / 20 / 6 = 79.2
                        break;
                    case kFlexSpiSerialClk_100MHz:
                        clockDiv = 4; // 528 * 18 / 24 / 6 = 99
                        break;
                    case kFlexSpiSerialClk_120MHz:
                        expectedPfd = 27;
                        clockDiv = 3; // 528 * 18 / 27 / 6 = 117
                        break;
                    case kFlexSpiSerialClk_133MHz:
                        clockDiv = 3; // 528 * 18 / 24 / 3 = 132
                        break;
                    case kFlexSpiSerialClk_166MHz:
                        expectedPfd = 29;
                        clockDiv = 2; // 528 * 18 / 29 / 2 = 163
                        break;
                    case kFlexSpiSerialClk_200MHz:
                        clockDiv = 2; // 528 * 18 / 24 / 2 = 198
                        break;
                }
            }
            else
            {
                expectedPfd = 24; // Aux0_pll will be configured to 396MHz
                switch (freqOption)
                {
                    default:
                    case kFlexSpiSerialClk_30MHz:
                        expectedPfd = 32;
                        clockDiv = 5; // 528 * 18 / 32 / 5 / 2 = 29.7
                        break;
                    case kFlexSpiSerialClk_50MHz:
                        clockDiv = 4; // 528 * 18 / 24 / 4 /2 = 49.5
                        break;
                    case kFlexSpiSerialClk_60MHz:
                        expectedPfd = 26;
                        clockDiv = 3; // 528 * 18 / 26 / 3 /2 = 60.8
                        break;
                    case kFlexSpiSerialClk_80MHz:
                        expectedPfd = 29;
                        clockDiv = 2; // 528 * 18 / 29 / 2 / 2 = 81.75
                        break;
                    case kFlexSpiSerialClk_100MHz:
                        clockDiv = 2; // 528 * 18 / 24 / 2 / 2  = 99
                        break;
                    case kFlexSpiSerialClk_120MHz:
                        expectedPfd = 20; // 528 * 18 / 20 / 2 / 2  = 118.75
                        clockDiv = 2;
                        break;
                    case kFlexSpiSerialClk_133MHz:
                        expectedPfd = 12; // 528 * 18 / 12 / 3 / 2  = 132
                        clockDiv = 3;
                        break;
                    case kFlexSpiSerialClk_166MHz:
                        expectedPfd = 29; // 528 * 18 / 29 / 1 / 2 = 163.8
                        clockDiv = 1;
                        break;
                    case kFlexSpiSerialClk_200MHz:
                        clockDiv = 1; // 528 * 18 / 24 / 1 / 2 = 198
                        break;
                }
            }

            uint32_t currentPfd = 0;
            if (clockSrc == kOspiClockSrc_Aux0PllClk)
            {
                currentPfd = (CLKCTL0->SYSPLL0PFD & CLKCTL0_SYSPLL0PFD_PFD2_MASK) >> CLKCTL0_SYSPLL0PFD_PFD2_SHIFT;
            }
            else
            {
                currentPfd = (CLKCTL0->SYSPLL0PFD & CLKCTL0_SYSPLL0PFD_PFD3_MASK) >> CLKCTL0_SYSPLL0PFD_PFD3_SHIFT;
            }
            uint32_t currentClkSrc =
                (CLKCTL0->FLEXSPIFCLKSEL & CLKCTL0_FLEXSPIFCLKSEL_SEL_MASK) >> CLKCTL0_FLEXSPIFCLKSEL_SEL_SHIFT;

            needHaltClk = (currentDiv != clockDiv) || (currentPfd != expectedPfd) || (clockSrc != currentClkSrc);

            if (needHaltClk)
            {
                // Halt the counter if PDF reconfiguration is required to avoid possible glitch
                CLKCTL0->FLEXSPIFCLKDIV |= (CLKCTL0_FLEXSPIFCLKDIV_HALT_MASK);
            }

            // Need some delay before changing OSPIFFCLKSEL
            __DSB();
            __ISB();
            // Switch clock source
            CLKCTL0->FLEXSPIFCLKSEL = CLKCTL0_FLEXSPIFCLKSEL_SEL(clockSrc);

            if (currentPfd != expectedPfd)
            {
                // Reconfigure PFD
                if (clockSrc == kOspiClockSrc_Aux0PllClk)
                {
                    CLOCK_InitSysPfd(kCLOCK_Pfd2, expectedPfd);
                }
                else
                {
                    CLOCK_InitSysPfd(kCLOCK_Pfd3, expectedPfd);
                }
                // need some delay before next step
                __DSB();
                __ISB();
            }
        }

        // Update clock divider
        CLKCTL0->FLEXSPIFCLKDIV = CLKCTL0_FLEXSPIFCLKDIV_DIV(clockDiv - 1);

    } while (0);
}

//!@brief Gate on the clock for the FlexSPI peripheral
void flexspi_clock_gate_enable(uint32_t instance)
{
}

//!@brief Gate off the clock the FlexSPI peripheral
void flexspi_clock_gate_disable(uint32_t instance)
{
}

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
