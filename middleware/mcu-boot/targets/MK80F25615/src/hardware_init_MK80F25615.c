/*
 * Copyright (c) 2014-2015 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "bootloader_common.h"
#include "bootloader/bl_context.h"
#include "fsl_device_registers.h"
#include "qspi.h"
#include "fsl_lpuart.h"
#include "utilities/fsl_assert.h"
#if BL_ENABLE_CRC_CHECK
#include "bootloader/bl_app_crc_check.h"
#endif
////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

#define kIRC_48M 48000000UL

enum
{
    //! @brief Mask for the bit of RCM_MR[BOOTROM] indicating that the BOOTCFG0 pin was asserted.
    kBootedViaPinMask = 1
};

#define PORT_IRQC_INTERRUPT_FALLING_EDGE 0xA
#define PORT_IRQC_INTERRUPT_DISABLE 0

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

/*
 *  This function is called if parameters for QSPI io configuration is invalid
 */
static status_t quadspi_iomux_init_error_handle(void)
{
    // Restore all quadspi related pin mux to default value out of reset.
    for (uint32_t pin = 0; pin < 14; pin++)
    {
        PORTE->PCR[pin] &= (uint32_t)~PORT_PCR_MUX_MASK;
    }
    return kStatus_InvalidArgument;
}

/* This function is called for configurating pinmux for qspi module
 * This function only supports switching default mode on specified pins
 * If the isParallel parameter is used to determine which group of QSPI interface should be configured.
 */
status_t quadspi_iomux_configuration(qspi_config_t *config_base)
{
    bool needInitQuadSPI0A = true;
    bool needInitQuadSPI0B = false;
    bool isBasicConfiguration = false;

    // If the config_base pointer is invalid, only QuadSPI0A will be configured.
    if (config_base == NULL)
    {
        needInitQuadSPI0A = true;
        needInitQuadSPI0B = false;
        isBasicConfiguration = true;
    }
    else // Determine which Ports need to be configured based on parameters in qspi config block
    {
        if (config_base->sflash_type > kQspiFlashPad_Octal)
        {
            return quadspi_iomux_init_error_handle();
        }

        if (config_base->sflash_A1_size > 0)
        {
            needInitQuadSPI0A = true;
        }
        else
        {
            needInitQuadSPI0A = false;
        }

        if (config_base->sflash_B1_size > 0)
        {
            needInitQuadSPI0B = true;
        }
        else
        {
            needInitQuadSPI0B = false;
        }
    }

    if (needInitQuadSPI0A)
    {
        // Configure default GPIOs for basic read
        PORTE->PCR[1] = PORT_PCR_MUX(5); // QSPI0A_SCLK is ALT5 for pin PTE1
        PORTE->PCR[2] = PORT_PCR_MUX(5); // QSPI0A_DATA0 is ALT5 for pin PTE2
        PORTE->PCR[4] = PORT_PCR_MUX(5); // QSPI0A_DATA1 is ALT5 for pin PTE4
        PORTE->PCR[5] = PORT_PCR_MUX(5); // QSPI0A_SS0_B is ALT5 for pin PTE5

        if (isBasicConfiguration)
        {
            return kStatus_Success;
        }

        if (config_base->sflash_A2_size > 0)
        {
            PORTE->PCR[7] = PORT_PCR_MUX(7); // QSPI0A_SS1_B is ALT7 for pin PTE7
        }

        if ((config_base->sflash_type == kQspiFlashPad_Quad) || (config_base->sflash_type == kQspiFlashPad_Octal))
        {
            PORTE->PCR[0] = PORT_PCR_MUX(5); // QSPI0A_DATA3 is ALT5 for pin PTE0
            PORTE->PCR[3] = PORT_PCR_MUX(5); // QSPI0A_DATA2 is ALT5 for pin PTE3
        }
        if (config_base->sflash_type == kQspiFlashPad_Octal)
        {
            PORTE->PCR[8] = PORT_PCR_MUX(5);  // QSPI0B_DQ4 is ALT5 for pin PTE8
            PORTE->PCR[10] = PORT_PCR_MUX(5); // QSPI0B_DQ5 is ALT5 for pin PTE10
            PORTE->PCR[9] = PORT_PCR_MUX(5);  // QSPI0B_DQ6 is ALT5 for pin PTE9
            PORTE->PCR[6] = PORT_PCR_MUX(5);  // QSPI0B_DQ7 is ALT5 for pin PTE6

            if (config_base->differential_clock_pin_enable)
            {
                PORTE->PCR[7] = PORT_PCR_MUX(5); // CK# is ALT5 for pin PTE7
            }
            if (config_base->flash_CK2_clock_pin_enable)
            {
                PORTE->PCR[12] = PORT_PCR_MUX(5); // CK2 is ALT5 for pin PTE12
                if (config_base->differential_clock_pin_enable)
                {
                    PORTE->PCR[13] = PORT_PCR_MUX(5); // CK2# is ALT5 for pin PTE13
                }
            }
        }

        if (config_base->dqs_enable)
        {
            PORTE->PCR[11] = PORT_PCR_MUX(7); // QSPI0A_DQS is ALT7 for pin PTE11
        }
    }
    if (needInitQuadSPI0B)
    {
        PORTE->PCR[7] = PORT_PCR_MUX(5);  // QSPI0B_SCLK is ALT5 for pin PTE7
        PORTE->PCR[8] = PORT_PCR_MUX(5);  // QSPI0B_DATA0 is ALT5 for pin PTE8
        PORTE->PCR[10] = PORT_PCR_MUX(5); // QSPI0B_DATA1 is ALT5 for pin PTE10
        PORTE->PCR[11] = PORT_PCR_MUX(5); // QSPI0B_SS0_B is ALT5 for pin PTE11

        if (config_base->sflash_type == kQspiFlashPad_Quad)
        {
            PORTE->PCR[9] = PORT_PCR_MUX(5); // QSPI0B_DATA2 is ALT5 for pin PTE9
            PORTE->PCR[6] = PORT_PCR_MUX(5); // QSPI0B_DATA3 is ALT5 for pin PTE6
        }

        if (config_base->sflash_B2_size > 0)
        {
            PORTE->PCR[13] = PORT_PCR_MUX(5); // QSPI0B_SS1_B is ALT5 for pin PTE13
        }

        if (config_base->dqs_enable)
        {
            PORTE->PCR[12] = PORT_PCR_MUX(5); // QSPI0B_DQS is ALT5 for pin PTE12
        }
    }

    return kStatus_Success;
}

void init_hardware(void)
{
    // Disable the SYSMPU otherwise USB cannot access the bus
    SYSMPU->CESR &= (uint32_t)~SYSMPU_CESR_VLD_MASK;

    // Enable all the ports
    SIM->SCGC5 |= (SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTD_MASK |
                   SIM_SCGC5_PORTE_MASK);

    uint32_t tmp = SIM->SOPT2;
    tmp &= (uint32_t)~SIM_SOPT2_LPUARTSRC_MASK;
    // set PLLFLLSEL to select the IRC48Mhz
    // set LPUART Clock source to MCGFLLCLK or MCGPLLCLK
    tmp |= SIM_SOPT2_PLLFLLSEL_MASK | SIM_SOPT2_LPUARTSRC(1);
    SIM->SOPT2 = tmp;

    // Enable flash access for crossbar swicth master 3 (USB FS/LS)
    FMC->PFAPR |= FMC_PFAPR_M3AP_MASK;
    
    // Load the user configuration data so that we can configure the clocks
    g_bootloaderContext.propertyInterface->load_user_config();
}

void deinit_hardware(void)
{
    // Restore SYSMPU to default state (enabled)
    SYSMPU->CESR |= SYSMPU_CESR_VLD_MASK;

#if BL_ENABLE_CRC_CHECK && BL_FEATURE_CRC_ASSERT
    restore_crc_check_failure_pin();
#endif

    // Keep enabling PORTE clock since it is needed for QSPI module
    if (is_quadspi_configured())
    {
        SIM->SCGC5 &=
            (uint32_t) ~(SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTD_MASK);
    }
    else
    {
        SIM->SCGC5 &= (uint32_t) ~(SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK |
                                   SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTE_MASK);
    }

    // Restore SIM_SOPTx related bits being changed
    SIM->SOPT1 &= (uint32_t) ~(SIM_SOPT1_USBREGEN_MASK);
    SIM->SOPT2 &= (uint32_t) ~(SIM_SOPT2_LPUARTSRC_MASK | SIM_SOPT2_USBSRC_MASK | SIM_SOPT2_PLLFLLSEL_MASK);

    // Restore flash access for crossbar swicth master 3 (USB FS/LS)
    FMC->PFAPR &= (uint32_t)~FMC_PFAPR_M3AP_MASK;
}

bool usb_clock_init(void)
{
    // Select IRC48M clock
    SIM->SOPT2 |= (SIM_SOPT2_USBSRC_MASK | SIM_SOPT2_PLLFLLSEL_MASK);

    // Enable USB-OTG IP clocking
    SIM->SCGC4 |= (SIM_SCGC4_USBOTG_MASK);

    // Configure enable USB regulator for device
    SIM->SOPT1 |= SIM_SOPT1_USBREGEN_MASK;

    USB0->CLK_RECOVER_CTRL |= USB_CLK_RECOVER_CTRL_CLOCK_RECOVER_EN_MASK;
    USB0->CLK_RECOVER_IRC_EN |= USB_CLK_RECOVER_IRC_EN_IRC_EN_MASK;
    return true;
}

uint32_t get_bus_clock(void)
{
    uint32_t coreClockDivider = ((SIM->CLKDIV1 & SIM_CLKDIV1_OUTDIV1_MASK) >> SIM_CLKDIV1_OUTDIV1_SHIFT) + 1;
    uint32_t busClockDivider = ((SIM->CLKDIV1 & SIM_CLKDIV1_OUTDIV2_MASK) >> SIM_CLKDIV1_OUTDIV2_SHIFT) + 1;
    uint32_t mcgOutClock = SystemCoreClock * coreClockDivider;

    return (mcgOutClock / busClockDivider);
}

//! @brief Return uart clock frequency according to instance
//! @note: This function is updated according the "K80_256 RM Rev0 draft a", section 6.7.9
uint32_t get_uart_clock(uint32_t instance)
{
    uint32_t lpuartClock;
    uint32_t pllFllFrac = ((SIM->CLKDIV3 & SIM_CLKDIV3_PLLFLLFRAC_MASK) >> SIM_CLKDIV3_PLLFLLFRAC_SHIFT) + 1;
    uint32_t pllFllDiv = ((SIM->CLKDIV3 & SIM_CLKDIV3_PLLFLLDIV_MASK) >> SIM_CLKDIV3_PLLFLLDIV_SHIFT) + 1;
    lpuartClock = kIRC_48M * pllFllFrac / pllFllDiv;

    return lpuartClock;
}

bool is_boot_pin_asserted(void)
{
    // Boot pin uses ROM boot pin
    return false;
}

void dummy_byte_callback(uint8_t byte)
{
    (void)byte;
}

void debug_init(void)
{
}

#if __ICCARM__

size_t __write(int handle, const unsigned char *buf, size_t size)
{
    return size;
}

#endif // __ICCARM__

void update_available_peripherals()
{
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
