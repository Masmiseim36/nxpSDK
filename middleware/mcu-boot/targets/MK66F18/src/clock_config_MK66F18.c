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
#include <assert.h>

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

// Clock mode types
typedef enum _target_clock_mode
{
    kClockMode_FEI = 0,
    kClockMode_FEE = 1,
    kClockMode_FBI = 2,
    kClockMode_FBE = 3,
    kClockMode_PEE = 4,
    kClockMode_PBE = 5,
    kClockMode_Default = kClockMode_FEI,
} target_clock_mode_t;

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////

// This function implements clock mode switch between FEI and PEE mode used in this bootloader
void clock_mode_switch(const target_clock_mode_t currentMode, const target_clock_mode_t expectedMode);

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

// See bootloader_common.h for documentation on this function.
void configure_clocks(bootloader_clock_option_t option)
{
    static uint32_t s_defaultClockDivider;
    static target_clock_mode_t s_currentClockMode = kClockMode_FEI; 
    
    if (option == kClockOption_EnterBootloader)
    {
        s_defaultClockDivider = SIM->CLKDIV1;
        
        // General procedure to be implemented:
        // 1. Read clock flags and divider from bootloader config in property store
        bootloader_configuration_data_t * config = &g_bootloaderContext.propertyInterface->store->configurationData;
        uint8_t options = config->clockFlags;

        // Check if the USB HID peripheral is enabled. If it is enabled, we always
        // use the 48MHz IRC.
        bool isUsbEnabled = config->enabledPeripherals & kPeripheralType_USB_HID;

        // 2. If NOT High Speed, do nothing (use reset clock config)
        if ((!isUsbEnabled) && (options & kClockFlag_HighSpeed) )
        {
            // High speed flag is set (meaning disabled), so just use default clocks.
            SystemCoreClock = kDefaultClock / (((SIM->CLKDIV1 & SIM_CLKDIV1_OUTDIV1_MASK) >> SIM_CLKDIV1_OUTDIV1_SHIFT) + 1);
            return;
        }

        // 3. Set OUTDIV1 based on divider in config. OUTDIV4 starts out at /1.
        // The divider values are masked by the maximum bits per divider.
        uint32_t div1 = ((~config->clockDivider) & (SIM_CLKDIV1_OUTDIV1_MASK >> SIM_CLKDIV1_OUTDIV1_SHIFT)) + 1;

        // 4. Get MCGOUTCLK
        uint32_t McgOutClk = kMcgOutWithHighSpeedUsb;
        // 5. Bypass core clock divider when HighSpeed USB is enabled.
        div1 = 1;

        // 6. Keep core clock below kMaxCoreClock
        uint32_t freq = McgOutClk;
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

        // 8. Keep flexbus clock below kMaxFlexBusClock
        //
        //      MCGOUTCLK -> OUTDIV3 -> flexbus_clk
        freq = McgOutClk;
        while ((freq / div3) > kMaxFlexBusClock)
        {
            // Increase flexbus clock divider.
            ++div3;
        }
        assert((div3 >= kDivider_Min) && (div2 <= kDivider_Max));

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

        if((div1 == 1) && ((div2 > 8) || (div3 > 8) || (div4 > 8)))
        {
            return;
        }

        //10. Now set the dividers
        SIM->CLKDIV1 = SIM_CLKDIV1_OUTDIV1(div1 - 1) |
                       SIM_CLKDIV1_OUTDIV2(div2 - 1) |
                       SIM_CLKDIV1_OUTDIV3(div3 - 1) |
                       SIM_CLKDIV1_OUTDIV4(div4 - 1); // Update system prescalers

        //11. Update SystemCoreClock global.
        SystemCoreClock =  McgOutClk / div1;
        
        // 12. Change clock mode.
        clock_mode_switch(s_currentClockMode, kClockMode_PEE);
        s_currentClockMode = kClockMode_PEE;
        
        // Change the Clock source to PLL because MCG switches from FEI to PEE mode 
        uint32_t tmp = SIM->SOPT2;
        tmp &= (uint32_t)~SIM_SOPT2_PLLFLLSEL_MASK;
        tmp |= SIM_SOPT2_PLLFLLSEL(1);
        SIM->SOPT2 = tmp;
    }
    else if (option == kClockOption_ExitBootloader)
    {
        clock_mode_switch(s_currentClockMode, kClockMode_FEI);
        SIM->CLKDIV1 = s_defaultClockDivider;
    }
}

void clock_mode_switch(const target_clock_mode_t currentMode, const target_clock_mode_t expectedMode)
{
    // Note: here only implements clock switch between FEI and PEE,
    // The other modes are not supported.
    assert(currentMode == kClockMode_FEI || currentMode == kClockMode_FEE || currentMode == kClockMode_PEE);
    assert(expectedMode == kClockMode_FEI || expectedMode == kClockMode_FEE || expectedMode == kClockMode_PEE);

    if (currentMode == expectedMode)
    {
        return;
    }
    if (expectedMode == kClockMode_PEE)
    {
        /* SIM->SCGC5: PORTA=1 */
        SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK; /* Enable clock gate for ports to enable pin routing */
        /* PORTA->PCR18: ISF=0,MUX=0 */
        PORTA->PCR[18] &= (uint32_t) ~(uint32_t)((PORT_PCR_ISF_MASK | PORT_PCR_MUX(0x07)));
        /* PORTA->PCR19: ISF=0,MUX=0 */
        PORTA->PCR[19] &= (uint32_t) ~(uint32_t)((PORT_PCR_ISF_MASK | PORT_PCR_MUX(0x07)));

        MCG->SC = 0;
        /* Switch to FBE Mode */
        /* OSC0->CR: ERCLKEN=1,??=0,EREFSTEN=0,??=0,SC2P=0,SC4P=0,SC8P=0,SC16P=0 */
        OSC->CR = (uint8_t)0x80U;
        /* MCG->C2: LOCRE0=0,??=0,RANGE0=2,HGO0=0,EREFS0=1,LP=0,IRCS=0 */
        MCG->C2 = (uint8_t)0xA4U;
        /* MCG->C1: CLKS=2,FRDIV=4,IREFS=0,IRCLKEN=1,IREFSTEN=0 */
        MCG->C1 = (uint8_t)0xA2U;
        /* MCG->C4: DMX32=0,DRST_DRS=0 */
        MCG->C4 &= (uint8_t) ~(uint8_t)0xE0U;
        /* MCG->C5: ??=0,PLLCLKEN0=0,PLLSTEN0=0,PRDIV0=1 */
        MCG->C5 = (uint8_t)0x00U;
        /* MCG->C6: LOLIE0=0,PLLS=0,CME0=0,VDIV0=0 */
        MCG->C6 = (uint8_t)0x00U;

        while ((MCG->S & MCG_S_OSCINIT0_MASK) == 0x00U) /* Wait until the oscillator is ready */
        {
        }

        while ((MCG->S & MCG_S_IREFST_MASK) != 0x00U)
        { /* Check that the source of the FLL reference clock is the external reference clock. */
        }
        while ((MCG->S & 0x0CU) != 0x08U)
        { /* Wait until external reference clock is selected as MCG output */
        }
        /* Switch to PBE Mode */
        /* MCG->C6: LOLIE0=0,PLLS=1,CME0=0,VDIV0=14 */
        MCG->C6 = (uint8_t)0x44U;
        while ((MCG->S & 0x0CU) != 0x08U)
        { /* Wait until external reference clock is selected as MCG output */
        }
        while ((MCG->S & MCG_S_LOCK0_MASK) == 0x00U)
        { /* Wait until locked */
        }
        /* Switch to PEE Mode */
        /* MCG->C1: CLKS=0,FRDIV=4,IREFS=0,IRCLKEN=1,IREFSTEN=0 */
        MCG->C1 &= (uint8_t) ~(MCG_C1_CLKS_MASK);
        while ((MCG->S & 0x0CU) != 0x0CU)
        { /* Wait until output of the PLL is selected */
        }
        while (MCG->S2 != 0)
        { /* Wait until output of the correct PLL is selected */
        }
    }
    else if (expectedMode == kClockMode_FEI)
    {
        /* Switch to PBE mode */
        uint8_t tmp = MCG->C1;
        tmp &= (uint8_t) ~(MCG_C1_CLKS_MASK | MCG_C1_IREFS_MASK);
        tmp |= MCG_C1_CLKS(0x02);
        MCG->C1 = tmp;
        MCG->C6 |= MCG_C6_PLLS_MASK;
        MCG->C2 &= (uint8_t)~MCG_C2_LP_MASK;
        while ((MCG->S & 0x0CU) != 0x08U) /* Wait until external reference clock is selected */
        {
        }

        /* Switch to FBE mode */
        MCG->C1 = (~MCG_C1_FRDIV_MASK & MCG->C1) | MCG_C1_FRDIV(0x03);
        MCG->C6 &= ~MCG_C6_PLLS_MASK;

        while ((MCG->S & MCG_S_PLLST_MASK) != 0x00U) /* Wait until source of PLLS clock is FLL clock */
        {
        }

        /* Switch to FEI mode */
        MCG->C1 &= ~MCG_C1_CLKS_MASK;
        MCG->C1 = (~MCG_C1_IREFS_MASK & MCG->C1) | MCG_C1_IREFS(1);
        while (!(MCG->S & MCG_S_IREFST_MASK))
        {
            // Wait until internal reference clock is ready.
        }

        // Wait 2 cycles of the slow IRC + 2 cycles of OSCERCLK after MCG_S[IREFST] has
        // been set to 1, about 70us, maximum ticks per us is 48, the instructions needs
        // for each loop are not less than 3.
        for (volatile uint32_t dummyCnt = 0; dummyCnt < (48 * 70 / 3); dummyCnt++)
        {
            __NOP();
        }

        while ((MCG->S & 0x0CU) != 0x00U) /* Wait until output of FLL is selected */
        {
        }

        OSC->CR = (uint8_t)0x0U;
        while ((MCG->S & MCG_S_OSCINIT0_MASK) != 0x00U) /* Wait until OSC is off */
        {
        }

        /* Restore Registers */
        /* MCG->C2: LOCRE0=1,??=0,RANGE0=0,HGO0=0,EREFS0=0,LP=0,IRCS=0 */
        MCG->C2 = (uint8_t)0x80U;
        MCG->C1 = (uint8_t)0x04U;
        MCG->C5 = (uint8_t)0x00U;
    }
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
