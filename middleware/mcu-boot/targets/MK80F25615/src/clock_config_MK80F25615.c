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
#include "qspi.h"
#include "utilities/fsl_assert.h"
#include "target_config.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

// Clock mode types
typedef enum _target_clock_mode
{
    kClockMode_FEI = 0,
    kClockMode_FEE = 1,
    kClockMode_Default = kClockMode_FEI,
} target_clock_mode_t;

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////

// This function implements clock mode switch between FEI and PEE mode used in this bootloader
void clock_mode_switch(const target_clock_mode_t currentMode,
                       const target_clock_mode_t expectedMode,
                       bool isQspiConfigured);

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

// See bootloader_common for documentation on this function.
void configure_clocks(bootloader_clock_option_t option)
{
#if (BL_TARGET_FLASH)

    static target_clock_mode_t s_currentClockMode = kClockMode_FEI;
    static uint32_t s_defaultClockDivider;

    // Check if qspi module is configured.
    bool isQuadSpiConfigured = is_quadspi_configured();

    if (option == kClockOption_EnterBootloader)
    {
        s_defaultClockDivider = SIM->CLKDIV1;
        // 1. Read clock flags and divider from bootloader config in property store
        bootloader_configuration_data_t *config = &g_bootloaderContext.propertyInterface->store->configurationData;
        uint8_t options = config->clockFlags;

        // Check if the USB HID peripheral is enabled.
        bool isUsbEnabled = config->enabledPeripherals & kPeripheralType_USB_HID;

        // 2. Update SystemCoreClock (DEFAULT_SYSTEM_CLOCK is default value of MCGOUTCLK when OUTDIV1 is equal to 0)
        // In this case, rom should consider the situation that FOPT[LPBOOT] is eqaul to 0
        uint32_t coreClockDivider = ((SIM->CLKDIV1 & SIM_CLKDIV1_OUTDIV1_MASK) >> SIM_CLKDIV1_OUTDIV1_SHIFT) + 1;
        uint32_t mcgOutClock = isQuadSpiConfigured ? kFLL_96MHz : kFLL_48MHz;

        // 3. If any of following conditions are met, do nothing (use reset clock config)
        //      (1) NOT High Speed is NOT enabled
        //      (2) usb peripheral is NOT enabled
        //      (3) qspi doesn't need to run at high frequency
        if ((options & kClockFlag_HighSpeed) && (!isUsbEnabled) && (!isQuadSpiConfigured))
        {
            // High speed flag is set (meaning disabled), so just use default clocks.
            SystemCoreClock = kDefaultClock / coreClockDivider;
            return;
        }

        SystemCoreClock = mcgOutClock / coreClockDivider;

        // 4. Set OUTDIV1 based on divider in config. OUTDIV4 starts out at /1.
        // The divider values are masked by the maximum bits per divider.
        uint32_t div1 = ((~config->clockDivider) & (SIM_CLKDIV1_OUTDIV1_MASK >> SIM_CLKDIV1_OUTDIV1_SHIFT)) + 1;
        uint32_t div2 = div1;
        uint32_t div3 = div1;
        uint32_t div4 = div1;

        if (isQuadSpiConfigured)
        {
            div1 = 1; // Core = 96MHz
            div2 = 2; // Bus = 48MHz
            div3 = 2; // FlexBus = 48MHz
            div4 = 4; // Flash  = 24MHz
        }
        else if (isUsbEnabled)
        {
            div1 = 1; // Core = 48MHz
            div2 = 1; // Bus = 48MHz
            div3 = 1; // FlexBus = 48MHz
            div4 = 2; // Flash = 24MHz
        }

        // 5. Now set the dividers
        SIM->CLKDIV1 = SIM_CLKDIV1_OUTDIV1(div1 - 1) | SIM_CLKDIV1_OUTDIV2(div2 - 1) | SIM_CLKDIV1_OUTDIV3(div3 - 1) |
                       SIM_CLKDIV1_OUTDIV4(div4 - 1); /* Update system prescalers */

        // 6. Update SystemCoreClock global.
        SystemCoreClock = mcgOutClock / div1;

        // Switch to FEE mode.
        clock_mode_switch(s_currentClockMode, kClockMode_FEE, isQuadSpiConfigured);
        s_currentClockMode = kClockMode_FEE;
    }
    else if (option == kClockOption_ExitBootloader)
    {
        if (!isQuadSpiConfigured)
        {
            clock_mode_switch(s_currentClockMode, kClockMode_FEI, isQuadSpiConfigured);
            SIM->CLKDIV1 = s_defaultClockDivider;
        }
    }

#endif // BL_TARGET_FLASH
}

void clock_mode_switch(const target_clock_mode_t currentMode,
                       const target_clock_mode_t expectedMode,
                       bool isQspiConfigured)
{
    // Note: here only implements clock switch between FEI and FEE,
    // The other modes are not supported.
    assert(currentMode == kClockMode_FEE || currentMode == kClockMode_FEI);
    assert(expectedMode == kClockMode_FEE || expectedMode == kClockMode_FEI);

    if (currentMode == expectedMode)
    {
        return;
    }

    if (expectedMode == kClockMode_FEE)
    {
        uint8_t tmp;
        /* Switch to FEE mode */

        MCG->C7 = 2; // Select IRC48M as Oscillator.
        // Delay ~50us to ensure that MCG_C7[OSCSEL] is correctly switched to IRC48M.
        for (volatile uint32_t delayCnt = 0; delayCnt < 1000; delayCnt++)
        {
            __ASM("nop");
        }
        MCG->C1 = MCG_C1_FRDIV(6); // FRDIV=6, divide IRC48M with 1280, Switch to external reference clock.

        tmp = MCG->C4;
        tmp &= (uint8_t)~MCG_C4_DRST_DRS_MASK;
        if (!isQspiConfigured)
        {
            tmp |= MCG_C4_DRST_DRS(1); // Multiply with 1280, MCGOUTCLK is 48Mhz
        }
        else
        {
            tmp |= MCG_C4_DRST_DRS(3); // Multiply with 2560, MCGOUTCLK is 96Mhz
        }
        MCG->C4 = tmp;

        // Wait until external reference clock is ready.
        while (MCG->S & MCG_S_IREFST_MASK)
        {
        }
        // Wait until FLL clock is stable
        for (volatile uint32_t delayCnt = 0; delayCnt < 32000; delayCnt++)
        {
            __ASM("nop");
        }
    }
    else if (expectedMode == kClockMode_FEI)
    {
        if (isQspiConfigured)
        {
            return;
        }
        MCG->C1 = MCG_C1_FRDIV(6) | MCG_C1_IREFS_MASK; // Switch to internal reference clock.
        while (!(MCG->S & MCG_S_IREFST_MASK))
        {
           // Wait until internal reference clock is ready.
        }
        
        // Wait 2 cycles of the slow IRC + 2 cycles of OSCERCLK after MCG_S[IREFST] has 
        // been set to 1, about 70us, maximum ticks per us is 96, the instructions needs
        // for each loop are not less than 3.
        for(volatile uint32_t dummyCnt=0; dummyCnt < (96 * 70 / 3); dummyCnt++)
        {
            __NOP();
        }

        MCG->C7 = 0;
        for (volatile uint32_t delayCnt = 0; delayCnt < 1000; delayCnt++)
        {
            __NOP();
        }

        // Restore registers to default value out of reset.
        MCG->C1 = 0x04;
        MCG->C4 &= (uint8_t)~MCG_C4_DRST_DRS_MASK;

        // Wait until FLL clock is stable
        for (volatile uint32_t delayCnt = 0; delayCnt < 32000; delayCnt++)
        {
            __NOP();
        }
    }
}

void quadspi_serial_clock_configure(qspi_serial_clock_freq_t freq)
{
    uint32_t tmp = QuadSPI0->MCR;

    tmp &= (uint32_t) ~(QuadSPI_MCR_SCLKCFG_MASK);
    switch (freq)
    {
        default:
        case kQspiSerialClockFreq_Low:
            tmp |= QuadSPI_MCR_SCLKCFG(0x03); // Divide by 4
            break;
        case kQspiSerialClockFreq_Mid:
            tmp |= QuadSPI_MCR_SCLKCFG(0x01); // Divide by 2
            break;
        case kQspiSerialClockFreq_High:
            tmp |= QuadSPI_MCR_SCLKCFG(0x00); // Divide by 1
            break;
    }

    QuadSPI0->MCR = tmp;
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
