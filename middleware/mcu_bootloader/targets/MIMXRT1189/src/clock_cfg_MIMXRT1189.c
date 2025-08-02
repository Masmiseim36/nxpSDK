/*
 * Copyright 2018 - 2019 NXP
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "bl_semc.h"
#include "fsl_clock.h"
#include "fusemap.h"
#include "microseconds.h"
#include "property.h"
////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////
#define FREQ_1MHz (1000000UL)
#define FREQ_24MHz (24UL * FREQ_1MHz)
#define FREQ_400MHz (400UL * FREQ_1MHz)
#define FREQ_480MHz (480UL * FREQ_1MHz)
#define FREQ_594MHz (594UL * FREQ_1MHz)
#define FREQ_696MHz (696UL * FREQ_1MHz)

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////
// See bootloader_common for documentation on this function.
void configure_clocks(bootloader_clock_option_t option)
{
    if (option == kClockOption_EnterBootloader)
    {
        SystemCoreClock = CLOCK_GetRootClockFreq(kCLOCK_Root_M33);
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
    pllSicVal |= USBPHY_PLL_SIC_PLL_DIV_SEL(USBPHY_PLL_DIV_SEL_20);
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

//!@brief Configure axi clock for SEMC peripheral
void semc_axi_clock_config(semc_clk_freq_t freq)
{
#ifndef BL_TARGET_FPGA
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

    CCM->CLOCK_ROOT[kCLOCK_Root_Semc].CONTROL = CCM_CLOCK_ROOT_CONTROL_DIV(div - 1) | CCM_CLOCK_ROOT_CONTROL_MUX(mux);
#endif // #ifndef BL_TARGET_FPGA
}

//! @brief Gets the clock value used for microseconds driver
uint32_t microseconds_get_clock(void)
{
#ifndef BL_TARGET_FPGA
    return CLOCK_GetRootClockFreq(kCLOCK_Root_Bus_Aon);
#else
    return FPGA_FREQ_BUS;
#endif
}

//! @brief Return uart clock frequency according to instance
uint32_t get_uart_clock(uint32_t instance)
{
#ifndef BL_TARGET_FPGA
    switch (instance)
    {
        case 1:
        case 2:
            return CLOCK_GetRootClockFreq(kCLOCK_Root_Lpuart0102);
        case 3:
        case 4:
            return CLOCK_GetRootClockFreq(kCLOCK_Root_Lpuart0304);
        case 5:
        case 6:
            return CLOCK_GetRootClockFreq(kCLOCK_Root_Lpuart0506);
        case 7:
        case 8:
            return CLOCK_GetRootClockFreq(kCLOCK_Root_Lpuart0708);
        case 9:
        case 10:
            return CLOCK_GetRootClockFreq(kCLOCK_Root_Lpuart0910);
        case 11:
        case 12:
            return CLOCK_GetRootClockFreq(kCLOCK_Root_Lpuart1112);
        default:
            return 0;
    }
#else
    return FPGA_FREQ_UART;
#endif
}

void spi_clock_gate_enable(uint32_t instance)
{
    // Note: This function is no longer needed, empty function is created to satisfy the memory interface
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
