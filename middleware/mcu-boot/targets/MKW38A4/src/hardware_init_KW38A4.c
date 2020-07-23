/*
 * Copyright (c) 2013-2015, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "bootloader_common.h"
#include "bootloader/bl_context.h"
#include "fsl_device_registers.h"
#include "utilities/kinetis_family.h"
#include "drivers/smc/smc.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

#define BOOT_PIN_NUMBER 2u
#define BOOT_PIN_PORT PORTC
#define BOOT_PIN_GPIO GPIOC
#define BOOT_PIN_ALT_MODE 1u

#define BOOT_PIN_DEBOUNCE_READ_COUNT 500u

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////
void init_hardware(void)
{
#if defined(BL_TARGET_FLASH) && BL_TARGET_FLASH
    exit_vlpr();
#endif

    // Enable all the ports
    SIM->SCGC5 |= ((uint32_t)SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK);

    uint32_t tmp = SIM->SOPT2;
    tmp &= (uint32_t)~SIM_SOPT2_LPUART0SRC_MASK;
    tmp |= SIM_SOPT2_LPUART0SRC(1);
    SIM->SOPT2 = tmp; // Enable the LPUART clock source to the MCGFLLCLK

    //bring up peripheral clock to 20.9MHz, set it = 0
    SIM->CLKDIV1 &= (uint32_t)~SIM_CLKDIV1_OUTDIV4_MASK;
    ftfx_prefetch_speculation_status_t speculationStatus = {
        true,
        true,     
    };      
    (void)FTFx_CACHE_PflashSetPrefetchSpeculation(&speculationStatus);
}

void deinit_hardware(void)
{
    uint32_t tmp =((uint32_t)SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK);

    SIM->SCGC5 &= ~tmp; 
    
    SIM->SOPT2 &= (uint32_t)~SIM_SOPT2_LPUART0SRC_MASK;
    
    //restore the SIM_CLKDIV1 = 01
    tmp = SIM->CLKDIV1;
    tmp &= (uint32_t)~SIM_CLKDIV1_OUTDIV4_MASK;
    tmp |= SIM_CLKDIV1_OUTDIV4(1);
    SIM->CLKDIV1 = tmp;
     
    ftfx_prefetch_speculation_status_t speculationStatus = {
        false,
        false,     
    };      
    (void)FTFx_CACHE_PflashSetPrefetchSpeculation(&speculationStatus);    
}

uint32_t get_bus_clock(void)
{
    uint32_t busClockDivider = ((SIM->CLKDIV1 & (uint32_t)SIM_CLKDIV1_OUTDIV4_MASK) >> (uint32_t)SIM_CLKDIV1_OUTDIV4_SHIFT) + 1u;
    return (SystemCoreClock / busClockDivider);
}

uint32_t get_uart_clock(uint32_t instance)
{
    // LPUART clock
    return SystemCoreClock;

}

bool is_boot_pin_asserted(void)
{
#if defined(BL_TARGET_FLASH) && BL_TARGET_FLASH
    // Initialize boot pin for GPIO
    BOOT_PIN_PORT->PCR[(uint32_t)BOOT_PIN_NUMBER] = PORT_PCR_MUX((uint32_t)BOOT_PIN_ALT_MODE);
    // Set boot pin as an input
    BOOT_PIN_GPIO->PDDR &= ~(uint32_t)((uint32_t)1u << (uint32_t)BOOT_PIN_NUMBER);
    // Set boot pin pullup enabled, pullup select, filter enable
    BOOT_PIN_PORT->PCR[(uint32_t)BOOT_PIN_NUMBER] |= ((uint32_t)PORT_PCR_PE_MASK | (uint32_t)PORT_PCR_PS_MASK | (uint32_t)PORT_PCR_PFE_MASK);

    uint32_t readCount = 0u;

    // Sample the pin a number of times
    for (uint32_t i = 0u; i < (uint32_t)BOOT_PIN_DEBOUNCE_READ_COUNT; i++)
    {
        readCount += ((BOOT_PIN_GPIO->PDIR) >> (uint32_t)BOOT_PIN_NUMBER) & 1u;
    }

    // boot pin is pulled high so we are measuring lows, make sure most of our measurements
    // registered as low
    return (readCount < ((uint32_t)BOOT_PIN_DEBOUNCE_READ_COUNT / 2u));
#else
    // Boot pin for Flash only target
    return false;
#endif
}
extern void debug_init(void);
void debug_init(void)
{
}

#if defined(__ICCARM__)
extern size_t __write(int handle, const unsigned char *buf, size_t size);
size_t __write(int handle, const unsigned char *buf, size_t size)
{
    return size;
}

#endif // __ICCARM__

void update_available_peripherals(void)
{
    uint32_t k_sub_family_id = (SIM->SDID & (uint32_t)SIM_SDID_SUBFAMID_MASK) >> (uint32_t)SIM_SDID_SUBFAMID_SHIFT;

    // CAN module only supported in KW38x
    if (k_sub_family_id != 0x5u) 
    {            
        g_bootloaderContext.propertyInterface->store->configurationData.enabledPeripherals &= ~(uint8_t)kPeripheralType_CAN;        
    }       
}

//! @brief Update available peripherals based on specific chips
void update_flash_properties(void)
{
    uint32_t k_sub_family_id = (SIM->SDID & (uint32_t)SIM_SDID_SUBFAMID_MASK) >> (uint32_t)SIM_SDID_SUBFAMID_SHIFT;
    property_store_t *propertyStore = g_bootloaderContext.propertyInterface->store;
    
    // Flash size, KW37 PFlash = 512KB; KW38/39, PFlash = 256KB, DFlash = 256KB
    if (k_sub_family_id == 0x6u)
    {        
        propertyStore->flashBlockCount[0] = 2u;  
        g_bootloaderContext.allFlashState->ftfxConfig->flashDesc.blockCount = 2u;
    }
    else  
    {
        propertyStore->flashBlockCount[0] = 1u;
        g_bootloaderContext.allFlashState->ftfxConfig->flashDesc.blockCount = 1u;
    }
  
}

//! @brief Update DFlash based on specific chips
void check_available_dFlash(void)
{
#if  defined(BL_FEATURE_SUPPORT_DFLASH) && BL_FEATURE_SUPPORT_DFLASH
    if (((SIM->FCFG1 & (uint32_t)SIM_FCFG1_NVMSIZE_MASK) >> (uint32_t)SIM_FCFG1_NVMSIZE_SHIFT) == 0u)
    {
        g_bootloaderContext.dflashDriverInterface = (void *)0u;
        g_bootloaderContext.allFlashState->ftfxConfig->ifrDesc.resRange.dflashIfrStart = 0x0000U;
    }
#endif    
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
