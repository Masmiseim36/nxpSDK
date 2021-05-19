/*
 * Copyright 2018 - 2019 NXP
 *
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_clock.h"
#include "microseconds.h"
#include "property.h"
#include "bl_semc.h"
#include "fusemap.h"
////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////
#define FREQ_1MHz (1000000UL)
#define FREQ_528MHz (528UL * FREQ_1MHz)
#define FREQ_24MHz (24UL * FREQ_1MHz)
#define FREQ_480MHz (480UL * FREQ_1MHz)
#define FREQ_400MHz (400UL * FREQ_1MHz)
#define FREQ_500MHz (500UL * FREQ_1MHz)
#define FREQ_696MHz (696UL * FREQ_1MHz)
#define FREQ_19P2MHz (19200000u) // 19.2MHz
#define FREQ_594MHz (594UL * FREQ_1MHz)
#define FREQ_664MHz (664UL * FREQ_1MHz)

enum
{
    kCm7ClockSrc_RC400M = 0xaa,
    kCm7ClockSrc_PLL_ARM = 0x3c,
};

enum
{
    kCm4ClockSrc_RC400M = 0xaa,
    kCm4ClockSrc_PLL_480 = 0x3c,
};

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////
extern bool is_cm4_boot(void);
uint32_t get_cm7_core_src(void);
uint32_t get_cm4_core_src(void);
uint32_t get_osc_freq(void);

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////
static inline uint32_t get_clock_div(uint32_t root, uint32_t maxFreq)
{
    return ((root + maxFreq - 1) / maxFreq);
}

uint32_t get_osc_freq(void)
{
    uint32_t oscFreq = FREQ_24MHz;
    if (FUSE_BOOT_OSC_REF_VALUE)
    {
        oscFreq = FREQ_19P2MHz;
    }

    return oscFreq;
}

uint32_t get_cm7_core_src(void)
{
    if (FUSE_BOOT_FREQ_VALUE)
    {
        return kCm7ClockSrc_PLL_ARM;
    }
    else
    {
        return kCm7ClockSrc_RC400M;
    }
}

uint32_t get_cm4_core_src(void)
{
    if (FUSE_BOOT_FREQ_VALUE)
    {
        return kCm4ClockSrc_PLL_480;
    }
    else
    {
        return kCm4ClockSrc_RC400M;
    }
}

// See bootloader_common for documentation on this function.
void configure_clocks(bootloader_clock_option_t option)
{
    enum
    {
        kMaxFreq_M7 = 700 * FREQ_1MHz,
        kMaxFreq_M4 = 240 * FREQ_1MHz,
    };

    if (option == kClockOption_EnterBootloader)
    {
        SystemCoreClock = FREQ_24MHz; // Default core clock before initializing the boot clock
        // assert(CCM->CLOCK_ROOT[kCLOCK_Root_M7].CONTROL && CCM->CLOCK_ROOT[kCLOCK_Root_M7].CONTROL);

        uint32_t postDivider = 1 << (FUSE_LPB_BOOT_VALUE);
        if (postDivider > 8)
        {
            postDivider = 1;
        }

        if (is_cm4_boot())
        {
            uint32_t clockToSwitch;
            uint32_t cm4ClockDiv;
            if (get_cm4_core_src() == kCm4ClockSrc_RC400M)
            {
                clockToSwitch = FREQ_400MHz;
                cm4ClockDiv = get_clock_div(clockToSwitch, kMaxFreq_M4) * postDivider;
                CCM->CLOCK_ROOT[kCLOCK_Root_Lpuart1].CONTROL = CCM_CLOCK_ROOT_CONTROL_DIV(4) | CCM_CLOCK_ROOT_CONTROL_MUX(2);
            }
            else
            {
                clockToSwitch = FREQ_480MHz;
                cm4ClockDiv = get_clock_div(clockToSwitch, kMaxFreq_M4) * postDivider;
                CCM->CLOCK_ROOT[kCLOCK_Root_Lpuart1].CONTROL = CCM_CLOCK_ROOT_CONTROL_DIV(2) | CCM_CLOCK_ROOT_CONTROL_MUX(4);
            }
            SystemCoreClock = clockToSwitch / cm4ClockDiv;
        }
        else
        {
            if (get_cm7_core_src() == kCm7ClockSrc_RC400M)
            {
                uint32_t clockToSwitch = FREQ_400MHz;
                uint32_t cm7ClockDiv = get_clock_div(clockToSwitch, kMaxFreq_M7) * postDivider;
                CCM->CLOCK_ROOT[kCLOCK_Root_Lpuart1].CONTROL = CCM_CLOCK_ROOT_CONTROL_DIV(4) | CCM_CLOCK_ROOT_CONTROL_MUX(2);
                SystemCoreClock = clockToSwitch / cm7ClockDiv;
            }
            else /* get_cm7_core_src() == kCm7ClockSrc_PLL_ARM */
            {
                CCM->CLOCK_ROOT[kCLOCK_Root_Lpuart1].CONTROL = CCM_CLOCK_ROOT_CONTROL_DIV(2) | CCM_CLOCK_ROOT_CONTROL_MUX(4);
                if (FUSE_SPEED_LIMIT_VALUE == 1)
                {
                    SystemCoreClock = FREQ_500MHz / postDivider;
                }
                else
                {
                    SystemCoreClock = FREQ_696MHz / postDivider;
                }
            }
        }
    }
}

#define USBPHY_PLL_DIV_SEL_20 3
#define USBPHY_PLL_DIV_SEL_25 5
bool usb_clock_init(void)
{
    USB_OTG1->USBCMD |= USBHS_USBCMD_RST_MASK;

    // Clear PHY from reset
    USBPHY1->CTRL_CLR = USBPHY_CTRL_SFTRST_MASK;
    // clear UTMI CLKGATE
    USBPHY1->CTRL &= ~USBPHY_CTRL_CLKGATE_MASK;
    USBPHY1->CTRL |= USBPHY_CTRL_ENUTMILEVEL2_MASK | USBPHY_CTRL_ENUTMILEVEL3_MASK;

    USBPHY1->CTRL |= USBPHY_CTRL_ENAUTOCLR_CLKGATE_MASK | USBPHY_CTRL_ENAUTOCLR_PHY_PWD_MASK;

    // Enable regulator
    USBPHY1->PLL_SIC |= USBPHY_PLL_SIC_PLL_REG_ENABLE_MASK;

    // FUSE_BOOT_OSC_REF_VALUE = 1'b0, 24 MHz input clock with default divid by 20
    // FUSE_BOOT_OSC_REF_VALUE = 1'b1, 19.2 MHz input clock with default divid by 25
    uint32_t pllSicVal = (USBPHY1->PLL_SIC & ~USBPHY_PLL_SIC_PLL_DIV_SEL_MASK);
    if (get_osc_freq() == FREQ_24MHz)
    {
        pllSicVal |= USBPHY_PLL_SIC_PLL_DIV_SEL(USBPHY_PLL_DIV_SEL_20);
    }
    else
    {
        pllSicVal |= USBPHY_PLL_SIC_PLL_DIV_SEL(USBPHY_PLL_DIV_SEL_25);
    }
    USBPHY1->PLL_SIC = pllSicVal;
    debug_printf("USBPHY1->PLL_SIC = %x\n", USBPHY1->PLL_SIC);

    // Enable the power
    USBPHY1->PLL_SIC |= USBPHY_PLL_SIC_PLL_POWER_MASK | USBPHY_PLL_SIC_PLL_ENABLE_MASK;
    // Wait for PLL lock
    microseconds_delay(100);

    // Enable usb clocks
    USBPHY1->PLL_SIC |= USBPHY_PLL_SIC_PLL_EN_USB_CLKS_MASK;
    // Clear bypass bit
    USBPHY1->PLL_SIC_CLR = USBPHY_PLL_SIC_PLL_BYPASS_MASK;

    // clear UTMI CLKGATE, to run the clocks
    USBPHY1->CTRL &= ~USBPHY_CTRL_CLKGATE_MASK;
    // Set to normal mode
    USBPHY1->PWD = 0;

    USB_OTG1->USBCMD &= (uint32_t)~USBHS_USBCMD_RS_MASK;

    return true;
}

// Get OCOTP clock
uint32_t get_ocotp_clock(void)
{
    return get_bus_clock();
}

//!@brief Configure axi clock for SEMC peripheral
void semc_axi_clock_config(semc_clk_freq_t freq)
{
    uint8_t mux;
    uint8_t div;

    // Select the clk source
    switch (freq)
    {
        case kSemcClkFreq_33MHz:
            // MUX = 5 --> PLL_528   528MHz
            // DIV = 16    528 / 16 = 33MHz
            mux = 5;
            div = 16;
            break;
        case kSemcClkFreq_40MHz:
            // MUX = 6 --> PLL_528_PFD1   594MHz
            // DIV = 15    594 / 15 = 39.6MHz
            mux = 6;
            div = 15;
            break;
        case kSemcClkFreq_50MHz:
            // MUX = 6 --> PLL_528_PFD1   594MHz
            // DIV = 12   594 / 12 = 49.5MHz
            mux = 6;
            div = 12;
            break;
        case kSemcClkFreq_108MHz:
            // MUX = 5 --> PLL_528   528MHz
            // DIV = 5   528 / 5 = 105.6MHz
            mux = 5;
            div = 5;
            break;
        case kSemcClkFreq_133MHz:
            // MUX = 7 --> PLL_480_PFD0   664.62MHz
            // DIV = 5   664.62 / 5 = 132.9MHz
            mux = 7;
            div = 5;
            break;
        case kSemcClkFreq_166MHz:
            // MUX = 7 --> PLL_480_PFD0   664.62MHz
            // DIV = 4   664.62 / 4 = 166.1MHz
            mux = 7;
            div = 4;
            break;
        case kSemcClkFreq_200MHz:
            // MUX = 6 --> PLL_528_PFD1   594MHz
            // DIV = 3   594 / 3 = 198MHz
            mux = 6;
            div = 3;
            break;
        case kSemcClkFreq_66MHz:
        default:
            // MUX = 5 --> PLL_528   528MHz
            // DIV = 8   528 / 8 = 66MHz
            mux = 5;
            div = 8;
            break;
    }

    CCM->CLOCK_ROOT[kCLOCK_Root_Semc].CONTROL =
        CCM_CLOCK_ROOT_CONTROL_DIV(div - 1) | CCM_CLOCK_ROOT_CONTROL_MUX(mux);
}

//!@brief Gate on the clock for the SEMC peripheral
void semc_clock_gate_enable(void)
{
    CCM->LPCG[kCLOCK_Semc].DIRECT = 1;
}

//!@brief Gate off the clock the SEMC peripheral
void semc_clock_gate_disable(void)
{
    CCM->LPCG[kCLOCK_Semc].DIRECT = 0;
}

uint32_t get_bus_clock(void)
{
    // PIT is from BUS clock, the maximum bus clock is 200MHz
    uint32_t clkRoot = CCM->CLOCK_ROOT[kCLOCK_Root_Bus].CONTROL;
    uint32_t div = (clkRoot & CCM_CLOCK_ROOT_CONTROL_DIV_MASK) >> CCM_CLOCK_ROOT_CONTROL_DIV_SHIFT;
    uint32_t mux = (clkRoot & CCM_CLOCK_ROOT_CONTROL_MUX_MASK) >> CCM_CLOCK_ROOT_CONTROL_MUX_SHIFT;
    uint32_t busFreq = 0;

    switch (mux)
    {
        default:
        case 0:
            busFreq = FREQ_1MHz * 24u;
            break;
        case 1:
            busFreq = FREQ_1MHz * 24u;
            break;
        case 2:
            busFreq = FREQ_1MHz * 400u;
            break;
        case 3:
            busFreq = FREQ_1MHz * 16u;
            break;
        case 4:
            busFreq = FREQ_1MHz * 480u;
            break;
        case 5:
            busFreq = FREQ_1MHz * 200u;
            break;
        case 6:
            busFreq = FREQ_1MHz * 528u;
            break;
        case 7:
            busFreq = FREQ_1MHz * 297u;
            break;
    }
    return busFreq / (div + 1);
}

//! @brief Gets the clock value used for microseconds driver
uint32_t microseconds_get_clock(void)
{
    return get_bus_clock();
}

//! @brief Return uart clock frequency according to instance
uint32_t get_uart_clock(uint32_t instance)
{
    // LPUART1 clock has been configured to 80MHz in clock_configure
    uint32_t lpuart_clock = 0;
    lpuart_clock = 80 * FREQ_1MHz; // 80MHz;
    return lpuart_clock;
}

void spi_clock_gate_enable(uint32_t instance)
{
    // Note: This function is no longer needed, empty function is created to satisfy the memory interface
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
