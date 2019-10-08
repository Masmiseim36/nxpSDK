/*
 * Copyright 2016-2017 NXP
 *
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "bootloader/bl_context.h"
#include "bootloader_common.h"
#include "fsl_device_registers.h"
#include "microseconds/microseconds.h"
#include "property/property.h"
#include "target_config.h"
#include "flexspi/fsl_flexspi.h"
#if BL_FEATURE_SEMC_NAND_MODULE || BL_FEATURE_SEMC_NOR_MODULE
#include "semc/fsl_semc.h"
#endif // #if BL_FEATURE_SEMC_NAND_MODULE || BL_FEATURE_SEMC_NOR_MODULE
#include "utilities/fsl_assert.h"
#include "ocotp/fsl_ocotp.h"
#include "fusemap.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////
#ifndef FREQ_396MHz
#define FREQ_396MHz (396UL * 1000 * 1000)
#endif
#ifndef FREQ_528MHz
#define FREQ_528MHz (528UL * 1000 * 1000)
#endif
#ifndef FREQ_24MHz
#define FREQ_24MHz (24UL * 1000 * 1000)
#endif
#ifndef FREQ_480MHz
#define FREQ_480MHz (480UL * 1000 * 1000)
#endif

enum
{
    kMaxIpgClock = 144000000UL,
};

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////
void clock_setup(void);

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

// See bootloader_common for documentation on this function.
void configure_clocks(bootloader_clock_option_t option)
{
    if (option == kClockOption_EnterBootloader)
    {
        clock_setup();
    }
}

bool usb_clock_init(void)
{
    // Enable clock gate
    CCM->CCGR6 |= CCM_CCGR6_CG0_MASK;

    // Enable USB Clocks
    CCM_ANALOG->PLL_USB1_SET = CCM_ANALOG_PLL_USB1_EN_USB_CLKS_MASK;

    // Clear SFTRST
    USBPHY->CTRL_CLR = USBPHY_CTRL_SFTRST_MASK;

    // Clear Clock gate
    USBPHY->CTRL_CLR = USBPHY_CTRL_CLKGATE_MASK;

    // Clear power down register
    USBPHY->PWD = 0;

    // Disable Charger Detect
    USB_ANALOG->INSTANCE[0].CHRG_DETECT |= (USB_ANALOG_CHRG_DETECT_EN_B_MASK | USB_ANALOG_CHRG_DETECT_CHK_CHRG_B_MASK);

    USB->USBCMD &= (uint32_t)~USBHS_USBCMD_RS_MASK;

    return true;
}

// Get OCOTP clock
uint32_t get_ocotp_clock(void)
{
    uint32_t ahbBusDivider = ((CCM->CBCDR & CCM_CBCDR_IPG_PODF_MASK) >> CCM_CBCDR_IPG_PODF_SHIFT) + 1;
    return SystemCoreClock / ahbBusDivider;
}

#if BL_FEATURE_FLEXSPI_NOR_MODULE || BL_FEATURE_SPINAND_MODULE
//!@brief Configure clock for FlexSPI peripheral
void flexspi_clock_config(uint32_t instance, uint32_t freq, uint32_t sampleClkMode)
{
    uint32_t pfd480 = 0;
    uint32_t cscmr1 = 0;
    uint32_t frac = 0;
    uint32_t podf = 0;

    typedef struct _flexspi_clock_param
    {
        uint8_t frac;
        uint8_t podf;
    } flexspi_clock_param_t;

    const flexspi_clock_param_t k_sdr_clock_config[kFlexSpiSerialClk_200MHz + 1] = {
        // Reserved, 30MHz     50MHz     60MHz        75MHz    80MHz       100MHz   133MHz       166MHz   200MHz
        { 0, 0 }, { 34, 8 }, { 22, 8 }, { 24, 6 }, { 30, 4 }, { 18, 6 }, { 14, 6 }, { 17, 4 }, { 26, 2 }, { 22, 2 }
    };
    const flexspi_clock_param_t k_ddr_clock_config[kFlexSpiSerialClk_200MHz + 1] = {
        // Reserved, 30MHz,  50MHz,       60MHz,      75MHz,   80Mhz,   100MHz,      133MHz,   166MHz,     200MHz
        { 0, 0 }, { 24, 6 }, { 22, 4 }, { 12, 6 }, { 30, 2 }, { 18, 3 }, { 22, 2 }, { 33, 1 }, { 26, 1 }, { 22, 1 }
    };

    do
    {
        if ((sampleClkMode != kFlexSpiClk_SDR) && (sampleClkMode != kFlexSpiClk_DDR))
        {
            break;
        }

        pfd480 = CCM_ANALOG->PFD_480 & (~CCM_ANALOG_PFD_480_PFD0_FRAC_MASK);
        cscmr1 = CCM->CSCMR1 & (~CCM_CSCMR1_FLEXSPI_PODF_MASK);

        // Note: Per ANALOG IP Owner's recommendation, FRAC should be even number,
        //       PODF should be even nubmer as well if the divider is greater than 1

        const flexspi_clock_param_t *flexspi_config_array = NULL;
        if (sampleClkMode == kFlexSpiClk_SDR)
        {
            flexspi_config_array = &k_sdr_clock_config[0];
        }
        else
        {
            flexspi_config_array = &k_ddr_clock_config[0];
        }

        if (freq >= kFlexSpiSerialClk_30MHz)
        {
            if (freq > kFlexSpiSerialClk_200MHz)
            {
                freq = kFlexSpiSerialClk_30MHz;
            }

            frac = flexspi_config_array[freq].frac;
            podf = flexspi_config_array[freq].podf;

            pfd480 |= CCM_ANALOG_PFD_480_PFD0_FRAC(frac);
            cscmr1 |= CCM_CSCMR1_FLEXSPI_PODF(podf - 1);

            FLEXSPI->MCR0 |= FLEXSPI_MCR0_MDIS_MASK;
            flexspi_clock_gate_disable(instance);

            if (pfd480 != CCM_ANALOG->PFD_480)
            {
                CCM_ANALOG->PFD_480 = pfd480;
            }
            if (cscmr1 != CCM->CSCMR1)
            {
                CCM->CSCMR1 = cscmr1;
            }
            flexspi_clock_gate_enable(instance);
            FLEXSPI->MCR0 &= ~FLEXSPI_MCR0_MDIS_MASK;
        }
        else
        {
            // Do nothing
        }
    } while (0);
}

//!@brief Gate on the clock for the FlexSPI peripheral
void flexspi_clock_gate_enable(uint32_t instance)
{
    CCM->CCGR6 |= CCM_CCGR6_CG5_MASK;
}

//!@brief Gate off the clock the FlexSPI peripheral
void flexspi_clock_gate_disable(uint32_t instance)
{
    CCM->CCGR6 &= (uint32_t)~CCM_CCGR6_CG5_MASK;
}

#endif // #if BL_FEATURE_FLEXSPI_NOR_MODULE || BL_FEATURE_SPINAND_MODULE

#if BL_FEATURE_SEMC_NAND_MODULE || BL_FEATURE_SEMC_NOR_MODULE
//!@brief Get Clock for SEMC peripheral
status_t semc_get_clock(semc_clock_type_t type, uint32_t *freq)
{
    uint32_t clockFrequency = 0;
    status_t status = kStatus_Success;

#ifndef BL_TARGET_FPGA
    uint32_t ipgBusDivider;
    uint32_t axiClkDivider;
    uint32_t pfd1Frac;
    uint32_t pfd1Clk;

    switch (type)
    {
        case kSemcClkType_IpgClock:
            ipgBusDivider = ((CCM->CBCDR & CCM_CBCDR_IPG_PODF_MASK) >> CCM_CBCDR_IPG_PODF_SHIFT) + 1;
            clockFrequency = SystemCoreClock / ipgBusDivider;
            break;
        case kSemcClkType_AxiClock:
            pfd1Frac = (CCM_ANALOG->PFD_528 & CCM_ANALOG_PFD_528_PFD1_FRAC_MASK) >> CCM_ANALOG_PFD_528_PFD1_FRAC_SHIFT;
            axiClkDivider = ((CCM->CBCDR & CCM_CBCDR_SEMC_PODF_MASK) >> CCM_CBCDR_SEMC_PODF_SHIFT) + 1;
            pfd1Clk = FREQ_528MHz / pfd1Frac * 18;
            clockFrequency = pfd1Clk / axiClkDivider;
            break;
        default:
            status = kStatus_InvalidArgument;
            break;
    }
#else
#warning "Need to check with FPGA team"
    switch (type)
    {
        case kSemcClkType_IpgClock:
            clockFrequency = 24000000;
            break;
        case kSemcClkType_AxiClock:
            clockFrequency = 12000000;
            break;
        default:
            status = kStatus_InvalidArgument;
            break;
    }
#endif
    *freq = clockFrequency;

    return status;
}

//!@brief Configure axi clock for SEMC peripheral
void semc_axi_clock_config(semc_clk_freq_t freq)
{
#if !defined(BL_TARGET_FPGA)
    uint32_t pfd528 = 0;
    uint32_t cbcdr = 0;
    uint32_t frac = 0;
    uint32_t podf = 0;

    // Select the clk source
    /* Note1: There are total three kinds of clock source for SEMC AXI clk
    //  1. CCM_CBCDR[SEMC_CLK_SEL] = 1'b1 : SEMC alternative clock
    //     CCM_CBCDR[SEMC_ALT_CLK_SEL]:
    //                                  1'b0: PLL2 PFD2
    //                                  1'b1: PLL3 PFD1
    //  2. CCM_CBCDR[SEMC_CLK_SEL] = 1'b0 : Periph_clk
    //     CCM_CBCDR[PERIPH_CLK_SEL] = 1'b0: PLL2 (pll2_main_clk) and ARM PLL
    //     CCM_CBCMR[PRE_PERIPH_CLK_SEL]:
    //                                  2'b00: PLL2 Main
    //                                  2'b01: PLL2 PFD2
    //                                  2'b10: PLL2 PFD0
    //  3. CCM_CBCDR[SEMC_CLK_SEL] = 1'b0 : Periph_clk
    //     CCM_CBCDR[PERIPH_CLK_SEL] = 1'b1: periph_clk2
    //     CCM_CBCMR[PERIPH_CLK2_SEL]:
    //                                  2'b00: PLL3 Main
    //                                  2'b01: OSC

    // Note2: top clk is defined in hapi_clock_init()
    //    PLL2 PFD2: 500MHz (selected)
    //    PLL3 PFD1:
    */
    CCM->CBCDR |= CCM_CBCDR_SEMC_CLK_SEL_MASK;
    CCM->CBCDR &= ~CCM_CBCDR_SEMC_ALT_CLK_SEL_MASK;

    // Now set the divider
    pfd528 = CCM_ANALOG->PFD_528 & (~CCM_ANALOG_PFD_528_PFD2_FRAC_MASK);
    cbcdr = CCM->CBCDR & (~CCM_CBCDR_SEMC_PODF_MASK);

    switch (freq)
    {
        case kSemcClkFreq_33MHz:
            // FRAC = 35, 528 * 18 / 35 = 271.54Hz
            // PODF = 8, 271.54 / 8 = 33.9MHz
            frac = 35;
            podf = 8;
            break;
        case kSemcClkFreq_40MHz:
            // FRAC = 34, 528 * 18 / 34 = 279.53Hz
            // PODF = 7, 279.53 / 7 = 39.9MHz
            frac = 34;
            podf = 7;
            break;
        case kSemcClkFreq_50MHz:
            // FRAC = 27, 528 * 18 / 27 = 352Hz
            // PODF = 7, 352 / 7 = 50.29MHz
            frac = 27;
            podf = 7;
            break;
        case kSemcClkFreq_108MHz:
            // FRAC = 18, 528 * 18 / 29 = 327.72z
            // PODF = 8, 528 / 3 = 109.2MHz
            frac = 29;
            podf = 3;
            break;
        case kSemcClkFreq_133MHz:
            // FRAC = 18, 528 * 18 / 18 = 528MHz
            // PODF = 4, 528 / 4 = 132MHz
            frac = 18;
            podf = 4;
            break;
        case kSemcClkFreq_166MHz:
            // FRAC = 19, 528 * 18 / 19 = 500.21MHz
            // PODF = 3, 500.21 / 3 = 166.7MHz
            frac = 19;
            podf = 3;
            break;
        case kSemcClkFreq_66MHz:
        default:
            // FRAC = 18, 528 * 18 / 18 = 528Hz
            // PODF = 8, 528 / 8 = 66MHz
            frac = 18;
            podf = 8;
            break;
    }

    pfd528 |= CCM_ANALOG_PFD_528_PFD2_FRAC(frac);
    cbcdr |= CCM_CBCDR_SEMC_PODF(podf - 1);
    if (pfd528 != CCM_ANALOG->PFD_528)
    {
        CCM_ANALOG->PFD_528 = pfd528;
    }
    if (cbcdr != CCM->CBCDR)
    {
        // Any change of this divider might involve handshake with EMI.
        CCM->CBCDR = cbcdr;
        // Check busy indicator for semc_clk_podf.
        while (CCM->CDHIPR & CCM_CDHIPR_SEMC_PODF_BUSY_MASK)
            ;
    }

#endif
}

//!@brief Gate on the clock for the SEMC peripheral
void semc_clock_gate_enable(void)
{
    CCM->CCGR3 |= CCM_CCGR3_CG2_MASK;
}

//!@brief Gate off the clock the SEMC peripheral
void semc_clock_gate_disable(void)
{
    CCM->CCGR3 &= (uint32_t)~CCM_CCGR3_CG2_MASK;
}
#endif // #if BL_FEATURE_SEMC_NAND_MODULE || BL_FEATURE_SEMC_NOR_MODULE

//! @brief Gets the clock value used for microseconds driver
uint32_t microseconds_get_clock(void)
{
    // Get PIT clock source
    uint32_t ahbBusDivider = ((CCM->CBCDR & CCM_CBCDR_IPG_PODF_MASK) >> CCM_CBCDR_IPG_PODF_SHIFT) + 1;
    uint32_t periphDivider = ((CCM->CSCMR1 & CCM_CSCMR1_PERCLK_PODF_MASK) >> CCM_CSCMR1_PERCLK_PODF_SHIFT) + 1;
    return SystemCoreClock / ahbBusDivider / periphDivider;
}

//! @brief Get BUS clock value
uint32_t get_bus_clock(void)
{
    uint32_t ahbBusDivider = ((CCM->CBCDR & CCM_CBCDR_IPG_PODF_MASK) >> CCM_CBCDR_IPG_PODF_SHIFT) + 1;
    return SystemCoreClock / ahbBusDivider;
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
