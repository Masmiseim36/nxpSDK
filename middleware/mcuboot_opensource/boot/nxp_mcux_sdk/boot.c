/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include <stdbool.h>

#include "boot.h"
#include "flash_partitioning.h"
#include "bootutil/bootutil_log.h"
#include "bootutil/image.h"
#include "bootutil/bootutil.h"
#include "sysflash/sysflash.h"
#include "flash_map.h"
#include "bootutil_priv.h"

#include "fsl_debug_console.h"
#include "mflash_drv.h"

#ifdef CONFIG_BOOT_USE_PSA_CRYPTO
#include "psa/crypto.h"
#endif

#ifdef CONFIG_BOOT_SERIAL_RECOVERY
#include "boot_serial/boot_serial.h"
#include "serial_recovery_support.h"
#endif

#ifdef CONFIG_BOOT_MODE_ENCRYPTED_XIP
#include "encrypted_xip.h"
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#if defined(__IAR_SYSTEMS_ICC__)
#define __TOOLCHAIN__ __VERSION__
#elif defined(__GNUC__)
#define __TOOLCHAIN__ "GCC " __VERSION__
#else
#define __TOOLCHAIN__ "UNKNOWN"
#endif

#ifdef NDEBUG
#undef assert
#define assert(x) ((void)(x))
#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
#ifdef CONFIG_BOOT_MODE_FLASH_REMAP
extern void SBL_EnableRemap(uint32_t start_addr, uint32_t end_addr, uint32_t off);
extern void SBL_DisableRemap(void);
#endif

#pragma weak cleanup
void cleanup(void);

extern void SBL_DisablePeripherals(void);
extern int SBL_SerialRecovery_gpio_check(void);

/*******************************************************************************
 * Types
 ******************************************************************************/

struct arm_vector_table
{
    uint32_t msp;
    uint32_t reset;
};

static struct arm_vector_table *vt;

#ifdef CONFIG_BOOT_SERIAL_RECOVERY
const struct boot_uart_funcs boot_funcs = {
    .read = serial_recovery_read,
    .write = serial_recovery_write
};
#endif

int SerialRecoveryActive = 0;

/*******************************************************************************
 * Code
 ******************************************************************************/

/* Starts selected application */

void do_boot(struct boot_rsp *rsp)
{
    uintptr_t flash_base;
    int rc;

    /* The beginning of the image is the ARM vector table, containing
     * the initial stack pointer address and the reset vector
     * consecutively. Manually set the stack pointer and jump into the
     * reset vector
     */
    rc = flash_device_base(rsp->br_flash_dev_id, &flash_base);
    assert(rc == 0);

#if defined(MCUBOOT_DIRECT_XIP) && defined(CONFIG_BOOT_MODE_FLASH_REMAP)

    /* In case direct-xip mode and enabled flash remapping function check if
     * the secondary slot is chosen to boot. If so we have to modify boot_rsp
     * structure here and enable flash remapping just before the jumping to app.
     * Flash remapping function has to be disabled when bootloader starts.
     */

    if (rsp->br_image_off == (BOOT_FLASH_CAND_APP - BOOT_FLASH_BASE))
    {
        uintptr_t start, end, off;
        start = BOOT_FLASH_ACT_APP;
        end   = BOOT_FLASH_ACT_APP + (BOOT_FLASH_CAND_APP - BOOT_FLASH_ACT_APP);
        off   = BOOT_FLASH_CAND_APP - BOOT_FLASH_ACT_APP;

        SBL_EnableRemap(start, end, off);
        rsp->br_image_off = BOOT_FLASH_ACT_APP - BOOT_FLASH_BASE;
        PRINTF("Booting the secondary slot - flash remapping is enabled\n");
    }
    else
    {
        PRINTF("Booting the primary slot - flash remapping is disabled\n");
    }
#endif

    vt = (struct arm_vector_table *)(flash_base + rsp->br_image_off + rsp->br_hdr->ih_hdr_size);

    cleanup();

    __set_CONTROL(0);
    __set_MSP(vt->msp);
    __ISB();
    ((void (*)(void))vt->reset)();
}


/* Calls MCUBoot and executes image selected by the bootloader */

int sbl_boot_main(void)
{
    int rc = -1;
    struct boot_rsp rsp;

#ifdef CONFIG_BOOT_USE_PSA_CRYPTO
    psa_status_t psa_status;
    int i;
    /* MCUX-84288,MCUX-84297 - this is workaround to fix random issues with 
     * entropy source on devices with DCP module */
    for(i = 0; i < 10; i++)
    {
        psa_status = psa_crypto_init();
        if(psa_status == PSA_SUCCESS)
        {
            break;
        }
        BOOT_LOG_WRN("Warning: failed to init PSA crypto backend...trying again the initialization");
    }
    if (psa_status != PSA_SUCCESS)
    {
        BOOT_LOG_ERR("FAILED to init PSA crypto backend! PSA error %d", psa_status);
        while(1)
          ;
    }
#endif

    rc = mflash_drv_init();
    if (rc != 0)
    {
        BOOT_LOG_ERR("FAILED to init mflash!");
        while(1)
          ;
    }
   
    BOOT_LOG_INF("Bootloader Version %s", BOOTLOADER_VERSION);
    BOOT_LOG_INF("Built " __DATE__ " " __TIME__);
    BOOT_LOG_INF("Toolchain " __TOOLCHAIN__);
    BOOT_LOG_INF("Upgrade mode: " UPGRADE_MODE);

#if defined(CONFIG_BOOT_SERIAL_RECOVERY)
    if(SBL_SerialRecovery_gpio_check())
    {
        BOOT_LOG_INF("Serial recovery button pressed");
        BOOT_LOG_INF("Entering the serial recovery mode...");
        SerialRecoveryActive = 1;
        boot_serial_start(&boot_funcs);
    }
#endif
    
#if defined(CONFIG_BOOT_MODE_ENCRYPTED_XIP)
    /* Initialize encryption XIP extension for overwrite-only mode */
    rc = encrypted_xip_init();
    if (rc != 0)
    {
        BOOT_LOG_ERR("FAILED to init encrypted XIP extension!");
    }
    bool cfg_found = false;
    rc = encrypted_xip_cfg_check(boot_flash_meta_map, &cfg_found, NULL);
    if(rc != kStatus_Success){
        BOOT_LOG_ERR("Fatal error of encrypted XIP extension!");
    }
    if(cfg_found == true){
      rc = encrypted_xip_cfg_initEncryption(boot_flash_meta_map);
      if(rc != kStatus_Success){
        BOOT_LOG_ERR("FAILED to initialize encryption unit!");
      }
    }
#endif
    
    rc = boot_go(&rsp);
    if (rc != 0)
    {
        BOOT_LOG_ERR("Unable to find bootable image");
#if defined(CONFIG_BOOT_SERIAL_RECOVERY)
        BOOT_LOG_INF("Entering the serial recovery mode...");
        SerialRecoveryActive = 1;
        boot_serial_start(&boot_funcs);
#endif
        for (;;)
            ;
    }

#if defined(CONFIG_BOOT_MODE_ENCRYPTED_XIP)
    /* Finish operations related to encryption XIP */
    rc = encrypted_xip_finish();
    if (rc != 0)
    {
        BOOT_LOG_ERR("encrypted_xip_finish failed!");
    }
#endif
    BOOT_LOG_INF("Bootloader chainload address offset: 0x%x", rsp.br_image_off);
    BOOT_LOG_INF("Reset_Handler address offset: 0x%x", rsp.br_image_off + rsp.br_hdr->ih_hdr_size);
    BOOT_LOG_INF("Jumping to the image\r\n\r\n");
    do_boot(&rsp);

    BOOT_LOG_ERR("Never should get here");
    for (;;)
        ;
}

void cleanup(void)
{
    SBL_DisablePeripherals();
}

#if !defined(MCUBOOT_DIRECT_XIP) && !defined(MCUBOOT_SWAP_USING_MOVE) && !defined(MCUBOOT_OVERWRITE_ONLY) && !defined(MCUBOOT_SINGLE_APPLICATION_SLOT)
#warning "Make sure scratch area is defined in 'boot_flash_map' array if required by defined swap mechanism"
#endif

#if defined(MCUBOOT_DIRECT_XIP) && CONFIG_UPDATEABLE_IMAGE_NUMBER > 1
#error "DIRECT_XIP (using remapping) and multiple images is not currently supported"
#endif
