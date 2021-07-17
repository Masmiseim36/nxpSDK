/*
 * Copyright (c) 2013-2015 Freescale Semiconductor, Inc.
 * Copyright 2016-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#define exit exit_default
#include <stdbool.h>

#include "bl_context.h"
#include "bootloader_common.h"
#include "fsl_assert.h"
#if !BL_FEATURE_HAS_NO_INTERNAL_FLASH
#if !BL_DEVICE_IS_LPC_SERIES
#include "fsl_flash.h"
#else
#include "fsl_iap.h"
#endif
#endif // #if !BL_FEATURE_HAS_NO_INTERNAL_FLASH
#include "fsl_rtos_abstraction.h"
#include "microseconds.h"
#include "vector_table_info.h"
#if BL_FEATURE_CRC_CHECK
#include "bl_app_crc_check.h"
#endif

#include "memory.h"

#if BL_FEATURE_RELIABLE_UPDATE
#include "bl_reliable_update.h"
#endif

#include "bl_api.h"

//! @addtogroup bl_core
//! @{

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////
bool is_valid_application_location(uint32_t applicationAddress);
void jump_to_application(uint32_t imageBase);
int main(void);

////////////////////////////////////////////////////////////////////////////////
// Codes
////////////////////////////////////////////////////////////////////////////////
//! A given jump address is considered valid if:
//! - Not 0x00000000
//! - Not 0xffffffff
//! - Not the reset handler entry point for the bootloader
//! - Is in flash or is in RAM or QuadSPI (if available)
//! @note this interface is also used by the configure_quadspi command
bool is_valid_application_location(uint32_t applicationAddress)
{
    const memory_map_entry_t *map;
    // Verify that the jumpLocation is non zero and then either within flash or RAM, both calculations are:
    // (jumpLocation >= startAddress) && (jumpLocation < (startAddress + size))
    if ((!applicationAddress) ||              // address is not null AND
        (applicationAddress == 0xffffffff) || // address is not blank Flash (0xff) AND
        (applicationAddress == (uint32_t)&Reset_Handler))
    {
        return false;
    }

    bool isValid = false;
    const uint32_t minThumb2InstructionSize = 2; // smallest thumb2 instruction size is 16-bit.
    // Check if the application address is in valid executable memory range
    status_t status = find_map_entry(applicationAddress, minThumb2InstructionSize, &map);
    if ((status == kStatus_Success) && (map->memoryProperty & kMemoryIsExecutable))
    {
        isValid = true;
    }

    return isValid;
}

#if !BL_FEATURE_HAS_NO_INTERNAL_FLASH
static void bootloader_flash_init(void)
{
    g_bootloaderContext.flashDriverInterface->flash_init(g_bootloaderContext.allFlashState);
#if !BL_DEVICE_IS_LPC_SERIES
    (void)FTFx_CACHE_Init(g_bootloaderContext.allFlashCacheState);
#endif
#if BL_FEATURE_SUPPORT_DFLASH
    check_available_dFlash();
    if (g_bootloaderContext.dflashDriverInterface != NULL)
    {
        (void)g_bootloaderContext.dflashDriverInterface->flash_init(g_bootloaderContext.dFlashState);
    }
#endif // BL_FEATURE_SUPPORT_DFLASH
}
#endif // #if !BL_FEATURE_HAS_NO_INTERNAL_FLASH

//! @brief Initialize the bootloader and peripherals.
//!
//! This function initializes hardware and clocks, loads user configuration data, and initialzes
//! a number of drivers. It then enters the active peripheral detection phase by calling
//! get_active_peripheral(). Once the peripheral is detected, the packet and comand interfaces
//! are initialized.
//!
//! Note that this routine may not return if peripheral detection times out and the bootloader
//! jumps directly to the user application in flash.
static void bootloader_init(void)
{
    // Init the global irq lock
    lock_init();

    // Init pinmux and other hardware setup.
    init_hardware();

    // Message so python instantiated debugger can tell the
    // bootloader application is running on the target.
    debug_printf("\r\n\r\nRunning bootloader...\r\n");
//    debug_printf("Build time:55 %s %s\r\n", __DATE__, __TIME__);

#if !BL_FEATURE_HAS_NO_INTERNAL_FLASH
    // Init flash driver.
    bootloader_flash_init();
#endif // #if !BL_FEATURE_HAS_NO_INTERNAL_FLASH

    // Configure clocks.
    configure_clocks(kClockOption_EnterBootloader);

    // Start the lifetime counter
    microseconds_init();

#if BL_FEATURE_BYPASS_WATCHDOG
    bootloader_watchdog_init();
#endif // BL_FEATURE_BYPASS_WATCHDOG

    // Init address range of flash array, SRAM_L and SRAM U.
    g_bootloaderContext.memoryInterface->init();

    // Fully init the property store.
    g_bootloaderContext.propertyInterface->init();

#if BL_FEATURE_RELIABLE_UPDATE
    bootloader_reliable_update_as_requested(kReliableUpdateOption_Normal, 0);
#endif // BL_FEATURE_RELIABLE_UPDATE

    if (is_valid_application_location(g_bootloaderContext.imageStart))
    {
        microseconds_shutdown();
        jump_to_application(g_bootloaderContext.imageStart);
    }
#if defined(DEBUG) && !defined(DEBUG_PRINT_DISABLE)
    standard_version_t version = g_bootloaderContext.propertyInterface->store->bootloaderVersion;
    debug_printf("Bootloader version %c%d.%d.%d\r\n", version.name, version.major, version.minor, version.bugfix);
#endif
}

//! @brief Bootloader outer loop.
//!
//! Infinitely calls the command interface and active peripheral control interface pump routines.
static void bootloader_run(void)
{
    // Switch to ISP mode
    debug_printf("OTA Application boot failed!\r\n");
    debug_printf("The OTA application can be downloaded to this device via the following typical appoaches:\r\n");
    debug_printf("1. ISP interface, steps:\r\n");
    debug_printf("    a) Switch the board to ISP mode, reset the board\r\n");
    debug_printf("    b) blhost <peripheral> -- fill-memory 0x1c000  4 0xc1503051\r\n");
    debug_printf("    c) blhost <peripheral> -- fill-memory 0x1c004  4 0x20000014\r\n");
    debug_printf("    d) blhost <peripheral> --  configure-memory 0x9 0x1c000 \r\n");
    debug_printf("    e) blhost <peripheral> --  flash-erase-region <addr> <length> \r\n");
    debug_printf("    f) blhost <peripheral> --  write-memory <addr> <ota-application file> \r\n");
    debug_printf("2. IDE download\r\n");
    debug_printf("3. User customized interface\r\n");
    while (1)
    {
    }
}

//! @brief Entry point for the bootloader.
int main(void)
{
    bootloader_init();
    bootloader_run();

    // Should never end up here.
    debug_printf("Warning: reached end of main()\r\n");
    return 0;
}

void jump_to_application(uint32_t imageStart)
{
    typedef void (*application_callback_t)(void);
    static uint32_t s_stackPointer;
    uint32_t applicationPointer;
    static application_callback_t s_appCallback;

    uint32_t *vectorTable = (uint32_t *)imageStart;

    s_stackPointer = vectorTable[0];
    applicationPointer = vectorTable[1];

    if (is_valid_application_location(applicationPointer))
    {
        debug_printf("Application address=%x\r\n", applicationPointer);
        s_appCallback = (application_callback_t)applicationPointer;
        // Set the MSPLIM to 0 to avoid potential stack related issues after jumping to the user application, the user
        // application can re-configure the SPLIM in the startup code
        __set_MSPLIM(0u);
        __set_MSP(s_stackPointer);
        s_appCallback();
    }
    // Fixed the stack popping issue before calling s_appCallback()
    __DSB();
    __ISB();
}

//! Since we never exit this gets rid of the C standard functions that cause
//! extra ROM size usage.
#undef exit
void exit(int arg) {}

#if defined(__CC_ARM) || (__ARMCC_VERSION)
#define ITM_Port8(n) (*((volatile unsigned char *)(0xE0000000 + 4 * n)))
#define ITM_Port16(n) (*((volatile unsigned short *)(0xE0000000 + 4 * n)))
#define ITM_Port32(n) (*((volatile unsigned long *)(0xE0000000 + 4 * n)))

#define DEMCR (*((volatile unsigned long *)(0xE000EDFC)))
#define TRCENA 0x01000000

FILE __stdout;
FILE __stdin;

int fputc(int ch, FILE *f)
{
    if (DEMCR & TRCENA)
    {
        while (ITM_Port32(0) == 0)
            ;
        ITM_Port8(0) = ch;
    }
    return (ch);
}
#endif

//! @}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
