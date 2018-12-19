/*
* Copyright 2017-2018 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#include "bootloader/bl_context.h"
#include "bootloader_common.h"
#include "fsl_device_registers.h"
#include "microseconds.h"
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
#define FREQ_396MHz (396UL * 1000 * 1000)
#define FREQ_528MHz (528UL * 1000 * 1000)
#define FREQ_24MHz (24UL * 1000 * 1000)
#define FREQ_480MHz (480UL * 1000 * 1000)

enum
{
    kMaxAHBClock = 144000000UL,
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
    USBPHY1->CTRL_CLR = USBPHY_CTRL_SFTRST_MASK;

    // Clear Clock gate
    USBPHY1->CTRL_CLR = USBPHY_CTRL_CLKGATE_MASK;

    // Clear power down register
    USBPHY1->PWD = 0;

    // Disable Charger Detect
    USB_ANALOG->INSTANCE[0].CHRG_DETECT |= (USB_ANALOG_CHRG_DETECT_EN_B_MASK | USB_ANALOG_CHRG_DETECT_CHK_CHRG_B_MASK);

    USB1->USBCMD &= (uint32_t)~USBHS_USBCMD_RS_MASK;

    return true;
}

void clock_setup(void)
{
    uint32_t clock_divider = 1;
    uint32_t fuse_div = 0;
    uint32_t clock_freq = 0;

    CLOCK_SetXtal0Freq(CPU_XTAL_CLK_HZ);
    // Get the Boot Up CPU Clock Divider
    // 00b = divide by 1
    // 01b = divide by 2
    // 10b = divide by 4
    // 11b = divide by 8
    fuse_div = ROM_OCOTP_LPB_BOOT_VALUE();
    clock_divider = 1 << fuse_div;

    // Get the Boot up frequency
    // 0 = 396Mhz
    // 1 = 528Mhz
    clock_freq = ROM_OCOTP_BOOT_FREQ_VALUE();

    CCM_ANALOG->PLL_ARM |= CCM_ANALOG_PLL_ARM_BYPASS_MASK;
    CCM_ANALOG->PLL_SYS |= CCM_ANALOG_PLL_SYS_BYPASS_MASK;
    CCM_ANALOG->PLL_USB1 |= CCM_ANALOG_PLL_USB1_BYPASS_MASK;
    // If clock is not configured, configure clock first, otherwise, just update SystemCoreClock

    /* Configure PLL_ARM: Reference clock = 24MHz
    * PLL_ARM = 24MHz * div / 2
    *  Core = PLL_ARM / 2 / clock_divider
    * To get 396MHz clock, PLL_ARM = 24 * 66 / 2 = 792, 792 / 2 = 396
    * To get 528MHz clock, PLL_ARM = 24 * 88 / 2 = 1056, 1056 / 2 = 528
    */
    uint32_t div = (clock_freq == 1) ? 88 : 66;
    CCM_ANALOG->PLL_ARM =
        CCM_ANALOG_PLL_ARM_BYPASS(1) | CCM_ANALOG_PLL_ARM_ENABLE(1) | CCM_ANALOG_PLL_ARM_DIV_SELECT(div);
    // Wait Until clock is locked
    while ((CCM_ANALOG->PLL_ARM & CCM_ANALOG_PLL_ARM_LOCK_MASK) == 0)
    {
    }

    /* Configure PLL_SYS */
    CCM_ANALOG->PLL_SYS &= ~CCM_ANALOG_PLL_SYS_POWERDOWN_MASK;
    // Wait Until clock is locked
    while ((CCM_ANALOG->PLL_SYS & CCM_ANALOG_PLL_SYS_LOCK_MASK) == 0)
    {
    }

    // Configure SYS_PLL PFD
    // PFD0 = 396MHz  - uSDHC CLOCK Source
    // PFD1 = 396MHz
    // PFD2 = 500MHz  - SEMC CLOCK Source
    // PFD3 = 396MHz
    CCM_ANALOG->PFD_528 =
        (CCM_ANALOG->PFD_528 & (~(CCM_ANALOG_PFD_528_PFD0_FRAC_MASK | CCM_ANALOG_PFD_528_PFD1_FRAC_MASK |
                                  CCM_ANALOG_PFD_528_PFD2_FRAC_MASK | CCM_ANALOG_PFD_528_PFD3_FRAC_MASK))) |
        CCM_ANALOG_PFD_528_PFD0_FRAC(24) | CCM_ANALOG_PFD_528_PFD1_FRAC(24) | CCM_ANALOG_PFD_528_PFD2_FRAC(19) |
        CCM_ANALOG_PFD_528_PFD3_FRAC(24);

    // Always configure USB1_PLL
    CCM_ANALOG->PLL_USB1 =
        CCM_ANALOG_PLL_USB1_DIV_SELECT(0) | CCM_ANALOG_PLL_USB1_POWER(1) | CCM_ANALOG_PLL_USB1_ENABLE(1);
    while ((CCM_ANALOG->PLL_USB1 & CCM_ANALOG_PLL_USB1_LOCK_MASK) == 0)
    {
    }
    CCM_ANALOG->PLL_USB1 &= ~CCM_ANALOG_PLL_USB1_BYPASS_MASK;

    // Configure USB_PLL PFD
    // PFD0 = 247MHz  - FLEXSPI CLOCK Source
    // PFD1 = 247MHz  - LPSPI CLOCK Source
    // PFD2 = 332MHz
    // PFD3 = 576MHz
    CCM_ANALOG->PFD_480 =
        (CCM_ANALOG->PFD_480 & (~(CCM_ANALOG_PFD_480_PFD0_FRAC_MASK | CCM_ANALOG_PFD_480_PFD1_FRAC_MASK |
                                  CCM_ANALOG_PFD_480_PFD2_FRAC_MASK | CCM_ANALOG_PFD_480_PFD3_FRAC_MASK))) |
        CCM_ANALOG_PFD_480_PFD0_FRAC(35) | CCM_ANALOG_PFD_480_PFD1_FRAC(35) | CCM_ANALOG_PFD_480_PFD2_FRAC(26) |
        CCM_ANALOG_PFD_480_PFD3_FRAC(15);

    // Set up CPU_PODF
    CCM->CACRR = CCM_CACRR_ARM_PODF(1);

    // Calculate the Final System Core Clock, it will be used to calculate the AHB / ARM Core divider later.
    SystemCoreClock = ((clock_freq == 0) ? FREQ_396MHz : FREQ_528MHz) / clock_divider;

    // Calculate the AHB clock divider
    uint32_t ahb_divider = 1;
    while ((SystemCoreClock / ahb_divider) > kMaxAHBClock)
    {
        ++ahb_divider;
    }

    // Set up AXI_PODF - SEMC clock root
    // Set up AHB_PODF - CORE clock
    // Set up IPG_PODF - BUS clock
    CCM->CBCDR = (CCM->CBCDR & (~(CCM_CBCDR_SEMC_PODF_MASK | CCM_CBCDR_AHB_PODF_MASK | CCM_CBCDR_IPG_PODF_MASK))) |
                 CCM_CBCDR_SEMC_PODF(ahb_divider - 1) | CCM_CBCDR_AHB_PODF(clock_divider - 1) |
                 CCM_CBCDR_IPG_PODF(ahb_divider - 1);

    // LPUART clock configuration, peripheral clock 20MHz
    CCM->CSCDR1 =
        (CCM->CSCDR1 & (~(CCM_CSCDR1_UART_CLK_SEL_MASK | CCM_CSCDR1_UART_CLK_PODF_MASK))) | CCM_CSCDR1_UART_CLK_PODF(3);

    /// Pre-peripheral clock configuration, FLEXSPI2 CLOCK SOURCE: PLL3_PFD0, CLOCK FREQ=30MHz
    CCM->CBCMR =
        (CCM->CBCMR &
         (~(CCM_CBCMR_PRE_PERIPH_CLK_SEL_MASK | CCM_CBCMR_FLEXSPI2_CLK_SEL_MASK | CCM_CBCMR_FLEXSPI2_PODF_MASK))) |
        CCM_CBCMR_PRE_PERIPH_CLK_SEL(3) | CCM_CBCMR_FLEXSPI2_CLK_SEL(1) | CCM_CBCMR_FLEXSPI2_PODF(7);

    // LPSPI clock configuration, Peripheral clock: 41MHz
    CCM->CBCMR = (CCM->CBCMR & (~(CCM_CBCMR_LPSPI_CLK_SEL_MASK | CCM_CBCMR_LPSPI_PODF_MASK))) |
                 CCM_CBCMR_LPSPI_CLK_SEL(0) | CCM_CBCMR_LPSPI_PODF(5);

    // FLEXSPI clock configuration, safe frequency: 30MHz
    CCM->CSCMR1 = ((CCM->CSCMR1 &
                    ~(CCM_CSCMR1_FLEXSPI_CLK_SEL_MASK | CCM_CSCMR1_FLEXSPI_PODF_MASK | CCM_CSCMR1_PERCLK_PODF_MASK |
                      CCM_CSCMR1_PERCLK_CLK_SEL_MASK)) |
                   CCM_CSCMR1_FLEXSPI_CLK_SEL(3) | CCM_CSCMR1_FLEXSPI_PODF(7) | CCM_CSCMR1_PERCLK_PODF(1));

    // NOTE: SEMC clock configuration needs handshake, so it will be handled by SEMC driver itself
    // uSDHC1&2 clock configuration
    // SEL: PULLL2 PFD0; DIV: 1 (PFD/2, freq=200MHz)
    CCM->CSCMR1 = (CCM->CSCMR1 & (~(CCM_CSCMR1_USDHC1_CLK_SEL_MASK | CCM_CSCMR1_USDHC2_CLK_SEL_MASK))) |
                  CCM_CSCMR1_USDHC1_CLK_SEL(1) | CCM_CSCMR1_USDHC2_CLK_SEL(1);
    CCM->CSCDR1 = (CCM->CSCDR1 & (~(CCM_CSCDR1_USDHC1_PODF_MASK | CCM_CSCDR1_USDHC2_PODF_MASK))) |
                  CCM_CSCDR1_USDHC1_PODF(1) | CCM_CSCDR1_USDHC2_PODF(1);

    // Finally, Enable PLL_ARM, PLL_SYS and PLL_USB1
    CCM_ANALOG->PLL_ARM &= ~CCM_ANALOG_PLL_ARM_BYPASS_MASK;
    CCM_ANALOG->PLL_SYS &= ~CCM_ANALOG_PLL_SYS_BYPASS_MASK;
    CCM_ANALOG->PLL_USB1 &= ~CCM_ANALOG_PLL_USB1_BYPASS_MASK;
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
    uint32_t temp = 0;
    uint32_t frac = 0;
    uint32_t podf = 0;

    do
    {
        if ((sampleClkMode > kFlexSpiClk_DDR) || (freq < 1) || (instance > 1))
        {
            break;
        }

        pfd480 = CCM_ANALOG->PFD_480 & (~CCM_ANALOG_PFD_480_PFD0_FRAC_MASK);
        if (instance == 0)
        {
            temp = CCM->CSCMR1 & (~CCM_CSCMR1_FLEXSPI_PODF_MASK);
        }
        else
        {
            temp = CCM->CBCMR & (~CCM_CBCMR_FLEXSPI2_PODF_MASK);
        }

        typedef struct
        {
            uint8_t frac;
            uint8_t podf;
        } flexspi_clk_freq_list_t;

        // Formula: 480MHz * 18 / (freq * podf)
        const flexspi_clk_freq_list_t k_flexspi_clk_list_sdr[kFlexSpiSerialClk_200MHz + 1] = {

            { 0, 0 },  // Reserved
            { 34, 8 }, // 30MHz
            { 22, 8 }, // 50MHz
            { 24, 6 }, // 60MHz
            { 30, 4 }, // 72MHz
            { 18, 6 }, // 80MHz
            { 12, 7 }, // 102MHz
            { 18, 4 }, // 120MHz
            { 13, 5 }, // 133MHz
            { 13, 4 }, // 166MHz
            { 22, 2 }  // 200MHz
        };

        // Formula: 480MHz * 18 / (freq * podf) / 2
        const flexspi_clk_freq_list_t k_flexspi_clk_list_ddr[kFlexSpiSerialClk_200MHz + 1] = {
            { 0, 0 },  // Reserved
            { 24, 6 }, // 30MHz
            { 22, 4 }, // 50MHz
            { 12, 6 }, // 60MHz
            { 30, 2 }, // 72MHz
            { 18, 3 }, // 80MHz
            { 21, 2 }, // 103MHz
            { 18, 2 }, // 120MHz
            { 33, 1 }, // 133MHz
            { 13, 2 }, // 166MHz
            { 22, 1 }  // 200MHz
        };

        // Note: Per ANALOG IP Owner's recommendation, FRAC should be even number,
        //       PODF should be even nubmer as well if the divider is greater than 1
        const flexspi_clk_freq_list_t *freq_list = k_flexspi_clk_list_sdr;
        if (sampleClkMode == kFlexSpiClk_DDR)
        {
            freq_list = k_flexspi_clk_list_ddr;
        }

        if (freq > kFlexSpiSerialClk_200MHz)
        {
            freq = kFlexSpiSerialClk_30MHz;
        }
        frac = freq_list[freq].frac;
        podf = freq_list[freq].podf;

        pfd480 |= CCM_ANALOG_PFD_480_PFD0_FRAC(frac);
        if (instance == 0)
        {
            temp |= CCM_CSCMR1_FLEXSPI_PODF(podf - 1);
        }
        else
        {
            temp |= CCM_CBCMR_FLEXSPI2_PODF(podf - 1);
        }

        FLEXSPI_Type *g_flexspiInstances[] = FLEXSPI_BASE_PTRS;

        // Disable FlexSPI before clock switch
        g_flexspiInstances[instance]->MCR0 |= FLEXSPI_MCR0_MDIS_MASK;
        flexspi_clock_gate_disable(instance);

        if (pfd480 != CCM_ANALOG->PFD_480)
        {
            CCM_ANALOG->PFD_480 = pfd480;
        }

        if (instance == 0)
        {
            if (temp != CCM->CSCMR1)
            {
                CCM->CSCMR1 = temp;
            }
        }
        else
        {
            if (temp != CCM->CBCMR)
            {
                CCM->CBCMR = temp;
            }
        }

        // Restore FlexSPI after clock switch
        flexspi_clock_gate_enable(instance);
        g_flexspiInstances[instance]->MCR0 &= ~FLEXSPI_MCR0_MDIS_MASK;

    } while (0);
}

//!@brief Gate on the clock for the FlexSPI peripheral
void flexspi_clock_gate_enable(uint32_t instance)
{
    if (instance == 0)
    {
        CCM->CCGR6 |= CCM_CCGR6_CG5_MASK;
    }
    else
    {
        CCM->CCGR7 |= CCM_CCGR7_CG1_MASK;
    }
}

//!@brief Gate off the clock the FlexSPI peripheral
void flexspi_clock_gate_disable(uint32_t instance)
{
    if (instance == 0)
    {
        CCM->CCGR6 &= (uint32_t)~CCM_CCGR6_CG5_MASK;
    }
    else
    {
        CCM->CCGR7 &= (uint32_t)~CCM_CCGR7_CG1_MASK;
    }
}

//!@brief Get Clock for FlexSPI peripheral
status_t flexspi_get_clock(uint32_t instance, flexspi_clock_type_t type, uint32_t *freq)
{
    uint32_t clockFrequency = 0;
    status_t status = kStatus_Success;

    uint32_t ahbBusDivider;
    uint32_t seralRootClkDivider;
    uint32_t arm_clock = SystemCoreClock;

    switch (type)
    {
        case kFlexSpiClock_CoreClock:
            clockFrequency = SystemCoreClock;
            break;
        case kFlexSpiClock_AhbClock:
        {
            // Note: In I.MXRT_512, actual AHB clock is IPG_CLOCK_ROOT
            ahbBusDivider = ((CCM->CBCDR & CCM_CBCDR_IPG_PODF_MASK) >> CCM_CBCDR_IPG_PODF_SHIFT) + 1;
            clockFrequency = arm_clock / ahbBusDivider;
        }
        break;
        case kFlexSpiClock_SerialRootClock:
        {
            uint32_t pfdFrac;
            uint32_t pfdClk;

            // PLL_480_PFD0
            pfdFrac = (CCM_ANALOG->PFD_480 & CCM_ANALOG_PFD_480_PFD0_FRAC_MASK) >> CCM_ANALOG_PFD_480_PFD0_FRAC_SHIFT;
            pfdClk = FREQ_480MHz / pfdFrac * 18;

            seralRootClkDivider = ((CCM->CSCMR1 & CCM_CSCMR1_FLEXSPI_PODF_MASK) >> CCM_CSCMR1_FLEXSPI_PODF_SHIFT) + 1;

            clockFrequency = pfdClk / seralRootClkDivider;
        }
        break;
        default:
            status = kStatus_InvalidArgument;
            break;
    }
    *freq = clockFrequency;

    return status;
}

// Get max supported Frequency in this SoC
status_t flexspi_get_max_supported_freq(uint32_t instance, uint32_t *freq, uint32_t clkMode)
{
    status_t status = kStatus_InvalidArgument;
    do
    {
        if (freq == NULL)
        {
            break;
        }

        if (kFlexSpiClk_DDR == clkMode)
        {
            *freq = (166UL * 1000 * 1000);
        }
        else
        {
            *freq = (166UL * 1000 * 1000);
        }

        status = kStatus_Success;

    } while (0);

    return status;
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

void flexspi_sw_delay_us(uint64_t us)
{
    while (us--)
    {
        microseconds_delay(1);
    }
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
