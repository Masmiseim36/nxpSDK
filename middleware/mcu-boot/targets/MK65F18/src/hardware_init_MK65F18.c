/*
 * Copyright (c) 2013-2015 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "bootloader_common.h"
#include "bootloader/bl_context.h"
#include "fsl_device_registers.h"
#include "fsl_uart.h"
#include "utilities/kinetis_family.h"
#include "smc.h"
#include "usb_device_config.h"
#include "usb.h"
#include "microseconds.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

#if defined(TOWER) || defined(FREEDOM)
#define BOOT_PIN_NUMBER 10
#define BOOT_PIN_PORT PORTA
#define BOOT_PIN_GPIO GPIOA
#define BOOT_PIN_ALT_MODE 1
#endif

#define BOOT_PIN_DEBOUNCE_READ_COUNT 500

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

void init_hardware(void)
{
    exit_vlpr();

    // Disable the SYSMPU otherwise USB cannot access the bus
    SYSMPU->CESR = 0;

    // Enable all the ports
    SIM->SCGC5 |= (SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTD_MASK |
                   SIM_SCGC5_PORTE_MASK);

    // Enable flash access for crossbar swicth master 4 (USB FS/LS)
    FMC->PFAPR |= FMC_PFAPR_M4AP_MASK;
    
    // Load the user configuration data so that we can configure the clocks
    g_bootloaderContext.propertyInterface->load_user_config();
}

void deinit_hardware(void)
{
    SIM->SCGC5 &= (uint32_t) ~(SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK |
                               SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTE_MASK);

    // Restore SIM_SOPTx related bits being changed
    SIM->SOPT1 &= (uint32_t)~SIM_SOPT1_USBREGEN_MASK;
    SIM->SOPT2 &= (uint32_t) ~(SIM_SOPT2_USBSRC_MASK | SIM_SOPT2_PLLFLLSEL_MASK);

    // Restore flash access for crossbar swicth master 4 (USB FS/LS)
    FMC->PFAPR &= (uint32_t)~FMC_PFAPR_M4AP_MASK;
}

// Note: Keep this function here to ensure compatibility, all usb related clock
// configuration is maintained by USB stack itself.
bool usb_clock_init(void)
{
#if ((BL_CONFIG_USB_HID) || (BL_CONFIG_USB_MSC))

    SIM->CLKDIV2 = (uint32_t)0x0UL; /* Update USB clock prescalers */
                                    // Select IRC48M clock
    SIM->SOPT2 |= (SIM_SOPT2_USBSRC_MASK | SIM_SOPT2_PLLFLLSEL_MASK);

    // Enable USB-OTG IP clocking
    SIM->SCGC4 |= (SIM_SCGC4_USBOTG_MASK);

    // Configure enable USB regulator for device
    SIM->SOPT1 |= SIM_SOPT1_USBREGEN_MASK;
    /* SIM_SOPT1: OSC32KSEL=0 */
    SIM->SOPT1 &=
        (uint32_t)~SIM_SOPT1_OSC32KSEL_MASK; /* System oscillator drives 32 kHz clock for various peripherals */

    USB0->CLK_RECOVER_IRC_EN = 0x03;
    USB0->CLK_RECOVER_CTRL |= USB_CLK_RECOVER_CTRL_CLOCK_RECOVER_EN_MASK;

    USB0->CLK_RECOVER_CTRL |= 0x20;
#endif
#if ((BL_CONFIG_HS_USB_HID) || (BL_CONFIG_HS_USB_MSC))
    MCG->C1 |= MCG_C1_IRCLKEN_MASK; // 32kHz IRC enable
    OSC->CR |= OSC_CR_ERCLKEN_MASK; // external reference clock enable
    // Configure EXT_PLL from USB HS PHY
    SIM->SOPT2 |= SIM_SOPT2_USBREGEN_MASK; // | SIM_SOPT2_PLLFLLSEL(2); //enable USB PHY PLL regulator, needs to be
                                           // enabled before enable PLL
    SIM->SCGC3 |= SIM_SCGC3_USBHS_MASK | SIM_SCGC3_USBHSPHY_MASK; // open HS USB PHY clock gate

    microseconds_delay(1000);

    SIM->USBPHYCTL =
        SIM_USBPHYCTL_USB3VOUTTRG(6) | SIM_USBPHYCTL_USBVREGSEL_MASK; // trim the USB regulator output to be 3.13V

    USBPHY->TRIM_OVERRIDE_EN = 0x001f; // override IFR value

    USBPHY->PLL_SIC |= USBPHY_PLL_SIC_PLL_POWER_MASK; // power up PLL
    if (BOARD_XTAL0_CLK_HZ == 24000000)
        USBPHY->PLL_SIC |= USBPHY_PLL_SIC_PLL_DIV_SEL(0);
    else if (BOARD_XTAL0_CLK_HZ == 16000000)
        USBPHY->PLL_SIC |= USBPHY_PLL_SIC_PLL_DIV_SEL(1);
    else if (BOARD_XTAL0_CLK_HZ == 12000000)
        USBPHY->PLL_SIC |= USBPHY_PLL_SIC_PLL_DIV_SEL(2);

    USBPHY->PLL_SIC &= ~USBPHY_PLL_SIC_PLL_BYPASS_MASK; // clear bypass bit

    USBPHY->PLL_SIC |= USBPHY_PLL_SIC_PLL_EN_USB_CLKS_MASK; // enable USB clock output from USB PHY PLL

    while (!(USBPHY->PLL_SIC & USBPHY_PLL_SIC_PLL_LOCK_MASK))
        ;
    USBPHY->CTRL &= ~USBPHY_CTRL_SFTRST_MASK;  // release PHY from reset
    USBPHY->CTRL &= ~USBPHY_CTRL_CLKGATE_MASK; // Clear to 0 to run clocks
    USBPHY->CTRL |= USBPHY_CTRL_SET_ENUTMILEVEL2_MASK | USBPHY_CTRL_SET_ENUTMILEVEL3_MASK;

    USBPHY->PWD = 0; // for normal operation

    USBPHY->ANACTRL |= USBPHY_ANACTRL_PFD_FRAC(24); // N=24

    USBPHY->ANACTRL |= USBPHY_ANACTRL_PFD_CLK_SEL(4); // div by 4
    USBPHY->ANACTRL &= ~USBPHY_ANACTRL_DEV_PULLDOWN_MASK;
    USBPHY->ANACTRL &= ~USBPHY_ANACTRL_PFD_CLKGATE_MASK;
    while (!(USBPHY->ANACTRL & USBPHY_ANACTRL_PFD_STABLE_MASK))
    {
    }
    USBPHY->TX |= 1 << 24;

    USBHS->USBCMD |= USBHS_USBCMD_RST_MASK;
    while (USBHS->USBCMD & USBHS_USBCMD_RST_MASK)
    { /* delay while resetting USB controller */
    }

    USBHS->USBMODE = USBHS_USBMODE_CM(2);
/* Set interrupt threshold control = 0 */
//    USBHS_USBCMD &= ~( USBHS_USBCMD_ITC(0xFF));
/* Setup Lockouts Off */
//    USBHS_USBMODE |= USBHS_USBMODE_SLOM_MASK;
#endif
    return true;
}

uint32_t get_bus_clock(void)
{
    uint32_t busClockDivider = ((SIM->CLKDIV1 & SIM_CLKDIV1_OUTDIV2_MASK) >> SIM_CLKDIV1_OUTDIV2_SHIFT) + 1;
    return (SystemCoreClock / busClockDivider);
}

// Return specified UART clock
// Note: LPUART0 is not supported here.
uint32_t get_uart_clock(uint32_t instance)
{
    switch (instance)
    {
        case 0:
        case 1:
            // UART0 and UART1 always use the system clock
            return SystemCoreClock;
        case 2:
            return get_bus_clock();
        case 3:
        case 4:
            // UART2-4 always use the bus clock.
            return get_bus_clock();
        default:
            return 0;
    }
}

bool is_boot_pin_asserted(void)
{
#ifdef BL_TARGET_FLASH
    // Initialize boot pin for GPIO
    BOOT_PIN_PORT->PCR[BOOT_PIN_NUMBER] |= PORT_PCR_MUX(BOOT_PIN_ALT_MODE);

    // Set boot pin as an input
    BOOT_PIN_GPIO->PDDR &= (uint32_t) ~(1 << BOOT_PIN_NUMBER);
    // Set boot pin pullup enabled, pullup select, filter enable
    BOOT_PIN_PORT->PCR[BOOT_PIN_NUMBER] |= (PORT_PCR_PE_MASK | PORT_PCR_PS_MASK | PORT_PCR_PFE_MASK);

    uint32_t readCount = 0;

    // Sample the pin a number of times
    for (uint32_t i = 0; i < BOOT_PIN_DEBOUNCE_READ_COUNT; i++)
    {
        readCount += ((BOOT_PIN_GPIO->PDIR) >> BOOT_PIN_NUMBER) & 1;
    }

    // boot pin is pulled high so we are measuring lows, make sure most of our measurements
    // registered as low
    return (readCount < (BOOT_PIN_DEBOUNCE_READ_COUNT / 2));
#else
    // Boot pin for Flash only target
    return false;
#endif
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
