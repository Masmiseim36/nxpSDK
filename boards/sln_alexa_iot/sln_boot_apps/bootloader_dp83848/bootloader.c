/*
 * Copyright 2019-2020 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/* Board includes */
#include "board.h"
#include "pin_mux.h"
#include "device_utils.h"

/* FreeRTOS kernel includes */
#include "FreeRTOS.h"
#include "task.h"

#if BOOTLOADER_MSD_ENABLED
#include "usb_device_config.h"
#include "usb.h"
#include "usb_device.h"
#include "usb_device_class.h"
#include "usb_device_msc.h"
#include "usb_device_ch9.h"
#include "usb_device_descriptor.h"
#include "disk.h"
#endif // BOOTLOADER_MSD_ENABLED

#include "sln_flash_mgmt.h"

#if BOOTLOADER_WIFI_ENABLED
#include "network_connection.h"
#include "wifi_credentials.h"
#endif

#include "bootloader.h"

#if USB_COMPOSITE
#include "composite.h"
#endif

/* AWS includes */
#include AWS_LOGGING_INCLUDE

#if BOOTLOADER_AWS_IOT_OTA_ENABLED
#include "sln_ota.h"
#else
#include "aws_application_version.h"
#endif

#include "nor_encrypt_bee.h"
#include "sln_auth.h"

#include "bl_drv/fsl_dcp_ext.h"

#include "sln_RT10xx_RGB_LED_driver.h"

#if BOOTLOADER_OTW_ENABLE
#include "sln_otw.h"
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define SET_THUMB_ADDRESS(x) (x | 0x1)

#define DEBUG_LOG_DELAY_MS(x) vTaskDelay(portTICK_PERIOD_MS *x)

// Set Temporary Stack Top to end of first block of OC RAM
#if (defined(MIMXRT106A_SERIES) || defined(MIMXRT106L_SERIES) || defined(MIMXRT106F_SERIES))
#define TEMP_STACK_TOP (0x20208000)
#else
#error "TEMP_STACK_TOP is not defined for this device!"
#endif

// typedef for function used to do the jump to the application
typedef void (*app_entry_t)(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
app_entry_t appEntry = 0;

#if (defined(APP_MAJ_VER) && defined(APP_MIN_VER) && defined(APP_BLD_VER))
static AppVersion32_t localAppFirmwareVersion = {
    .u.x.ucMajor = APP_MAJ_VER,
    .u.x.ucMinor = APP_MIN_VER,
    .u.x.usBuild = APP_BLD_VER,
};
#else
static AppVersion32_t localAppFirmwareVersion = {
    .u.x.ucMajor = 0,
    .u.x.ucMinor = 0,
    .u.x.usBuild = 0,
};
#warning "No build version defined for this application!"
#endif
/*******************************************************************************
 * Code
 ******************************************************************************/

__attribute__((naked)) void HardFault_Handler(void)
{
    __asm("BKPT #0");
}

/**
 * @brief Trap device execution if fatal issue is detected.
 *
 * Traps execution when there is no valid Reset Vector detected in flash.
 *
 */
static void the_should_never_get_here_catch(void)
{
    volatile uint32_t delay = 1000000;

    while (1)
    {
        while (delay--)
            ;
    }
}

/**
 * @brief Jump Execution to Address function.
 *
 * Causes bootloader execution to jump to the passed address (Interrupt Vector Table Address).  The
 * function calculates where the reset vector is from the passed IVT address and jumps there.
 *
 * @param[in] appaddr IVT address that holds the Reset Vector to jump to
 */
static void JumpToAddr(uint32_t appaddr)
{
    appaddr += FlexSPI_AMBA_BASE;

    vTaskDelay(portTICK_PERIOD_MS * 10);

    // Point entry point address to entry point call function
    appEntry = (app_entry_t)(SET_THUMB_ADDRESS((*(uint32_t *)(appaddr + 4))));

    // Set the VTOR to the application vector table address.
    SCB->VTOR = (uint32_t)appaddr;

    // Set stack pointers to the application stack pointer.
    __set_MSP((uint32_t)TEMP_STACK_TOP);
    __set_PSP((uint32_t)TEMP_STACK_TOP);

    // Jump to main app entry point
    appEntry();
}

/**
 * @brief Jump to Main Application task
 *
 * RTOS task used by the Bootloader to jump to the current main application. The current application IVT
 * address is read from the flash.
 *
 */
static void jumpToMainAppTask(void)
{
#ifdef DEBUG_BOOTLOADER
    the_should_never_get_here_catch();
#endif
    int32_t status    = SLN_FLASH_NO_ERROR;
    int32_t imgtype   = FICA_IMG_TYPE_NONE;
    uint32_t appaddr  = 0;
    bool doCtxRestore = false;

    // Get Current Application Vector
    status = FICA_GetCurAppStartType(&imgtype);

    if (SLN_FLASH_NO_ERROR == status)
    {
        status = FICA_get_app_img_start_addr(imgtype, &appaddr);
    }

#if DISABLE_IMAGE_VERIFICATION
#warning "IMAGE VERIFICATION DISABLED! THIS IS NOT RECOMMENDED."
    // NOTE: This is not recommended for a production application
#else
#if ENABLE_UNSIGNED_USB_MSD
    // NOTE: To facilitate ease of use in SLN-ALEXA-IOT kits
    bool isUSBMode = false;

    if (SLN_FLASH_NO_ERROR == status)
    {
        status = FICA_get_usb_mode(&isUSBMode);
    }

    /* Verify the current image signature */
    if ((SLN_FLASH_NO_ERROR == status) && (!isUSBMode))
    {
        status = FICA_Verify_Signature(imgtype);

        if (SLN_FLASH_NO_ERROR != status)
        {
            /* Verify the backup image signature, as the current image failed verification */
            imgtype = ((FICA_IMG_TYPE_APP_A == imgtype) ? FICA_IMG_TYPE_APP_B : FICA_IMG_TYPE_APP_A);
            status  = FICA_Verify_Signature(imgtype);
        }

        if (SLN_FLASH_NO_ERROR != status)
        {
            doCtxRestore = true;
        }
    }
#else
    // NOTE: This is best security practice
    if (SLN_FLASH_NO_ERROR == status)
    {
        status = FICA_Verify_Signature(imgtype);

        if (SLN_FLASH_NO_ERROR != status)
        {
            /* Verify the backup image signature, as the current image failed verification */
            imgtype = ((FICA_IMG_TYPE_APP_A == imgtype) ? FICA_IMG_TYPE_APP_B : FICA_IMG_TYPE_APP_A);
            status  = FICA_Verify_Signature(imgtype);
        }

        if (SLN_FLASH_NO_ERROR != status)
        {
            doCtxRestore = true;
        }
    }
#endif // ENABLE_UNSIGNED_USB_MSD
#endif // DISABLE_IMAGE_VERIFICATION

    /* We failed to verify any image */
    if (bl_nor_encrypt_is_enabled())
    {
        /* If the device is running eXIP, then switch the PRDB context */
        if (doCtxRestore)
        {
            int32_t ctxStatus = SLN_FLASH_NO_ERROR;
            /* No point switching context if we could load the previous FICA */
            configPRINTF(("[ERROR] Swapping Crypto Contexts\r\n"));

            vTaskDelay(2000);

            ctxStatus = SLN_AUTH_swap_context(SLN_CRYPTO_CTX_1);

            if (SLN_FLASH_NO_ERROR != ctxStatus)
            {
                configPRINTF(("[ERROR] Failed to flip to previous image\r\n"));
            }

            /* Need to do a physical reset to load the new PRDB Context */
            configPRINTF(("Rebooting to use new restored context...\r\n"));
            vTaskDelay(2000);
            NVIC_SystemReset();
        }
    }

    /* Get the appaddress one last time in case we are booting into a different image */
    if (SLN_FLASH_NO_ERROR == status)
    {
        int32_t tmpImageType = FICA_IMG_TYPE_NONE;
        int32_t imgStatus    = SLN_FLASH_NO_ERROR;

        imgStatus = FICA_GetCurAppStartType(&tmpImageType);

        /* Update the FICA with the new imgtype */
        if ((tmpImageType != imgtype) && (SLN_FLASH_NO_ERROR == imgStatus))
        {
            status = FICA_SetCurAppStartType(imgtype);
        }

        status = FICA_get_app_img_start_addr(imgtype, &appaddr);
    }

    if (SLN_FLASH_NO_ERROR != status)
    {
        // Boot back into itself
        appaddr = SCB->VTOR - FlexSPI_AMBA_BASE;
    }

    configPRINTF(("Launching into application at 0x%X...\r\n", appaddr));

    vTaskDelay(portTICK_PERIOD_MS * 100);

    DbgConsole_Flush();

    /* main app does not start correctly without this. why? */
    DbgConsole_Deinit();

    NVIC_DisableIRQ(LPUART6_IRQn);

    RGB_LED_SetColor(LED_COLOR_OFF);

    JumpToAddr(appaddr);

    // Uh oh...
    the_should_never_get_here_catch();

    vTaskDelete(NULL);
}

void ReRunBootloader()
{
    int32_t status = SLN_FLASH_NO_ERROR;
#ifdef DEBUG_BOOTLOADER
    the_should_never_get_here_catch();
#endif

    uint32_t appaddr = 0;

    // Get Current Application Vector
    status = FICA_GetCurBootStartAddr(&appaddr);

    vTaskDelay(portTICK_PERIOD_MS * 100);

    DbgConsole_Flush();

    DbgConsole_Deinit();

    NVIC_DisableIRQ(LPUART6_IRQn);

    if (SLN_FLASH_NO_ERROR != status)
    {
        // Safer to just jump back into our vector table
        appaddr = SCB->VTOR;
    }

    // Run the Bootloader
    JumpToAddr(appaddr);
}

#if BOOTLOADER_MSD_ENABLED
void CheckForMSDMode()
{
#ifdef BOOT_TEST_MSD
    USB_MSC_Init();
#else
    // Check if USB MSD Mode button is pushed
    if (BUTTON_MSDPressed())
    {

#if ENABLE_UNSIGNED_USB_MSD
        // NOTE: To facilitate ease of use in SLN-ALEXA-IOT kits
        FICA_set_usb_mode();
#endif // ENABLE_UNSIGNED_USB_MSD

        USB_MSC_Init();
        vTaskSuspend(NULL);
    }
#endif // BOOT_TEST_MSD
}
#endif // BOOTLOADER_MSD_ENABLED

#if BOOTLOADER_AWS_IOT_OTA_ENABLED

void CheckForAppOTA(void)
{
#ifdef BOOT_TEST_APP_OTA
    xTaskCreate(AWS_OTA_Main, "AWS_OTA_Task", 1024, NULL, configMAX_PRIORITIES - 1, NULL);
#else
    // Check if App Detected WiFi OTA Image is Available
    if (FICA_is_OTA_FlashBitCleared())
    {
        // Set OTA bit here as a precaution, to not get stuck in bootloader
        // if something goes terribly wrong during OTA. The bit will be reset
        // when OTA finishes, so we can start OTA task again for self test phase
        if (FICA_Set_OTA_FlashBit() != SLN_FLASH_NO_ERROR)
        {
            configPRINTF(("Failed to set OTA bit\r\n"));
            return;
        }

        // Start OTA task
        Run_OTA_Task();

        // Suspend here, otherwise the main function will
        // launch the target application while OTA task starts
        vTaskSuspend(NULL);
    }
#endif // BOOT_TEST_APP_OTA
}
#endif // BOOTLOADER_AWS_IOT_OTA_ENABLED

#if BOOTLOADER_OTW_ENABLE
static void CheckForFwUpdateOTW()
{
    status_t status = kStatus_Success;
    bool otwFlag    = false;

    // Read the FWUPDATE bit from the flash
    status = FICA_initialize();
    if (SLN_FLASH_NO_ERROR == status)
    {
        status = FICA_get_comm_flag(FICA_COMM_FWUPDATE_BIT, &otwFlag);
    }

    if (SLN_FLASH_NO_ERROR == status)
    {
        if ((otwFlag == 0) || BUTTON_OTWPressed())
        {
            // Set the bit and disable the FWUPDATE
            status = FICA_set_comm_flag(FICA_COMM_FWUPDATE_BIT);

            if ((SLN_FLASH_NO_ERROR == status))
            {
                otwAppStart();
            }

            // Suspend here, otherwise the main function will
            // launch the target application while FWUPDATE task starts
            vTaskSuspend(NULL);
        }
    }
}
#endif

void BootloaderMain(void *args)
{
    volatile bool isWait = false; // Boolean to force entry into wait states
    status_t status      = kStatus_Success;

    configPRINTF(("\r\n\r\n*** BOOTLOADER v%d.%d.%d ***\r\n\r\n", localAppFirmwareVersion.u.x.ucMajor,
           localAppFirmwareVersion.u.x.ucMinor, localAppFirmwareVersion.u.x.usBuild));

    RGB_LED_SetBrightnessColor(LED_BRIGHT_MEDIUM, LED_COLOR_WHITE);

    // Hold execution here. This is useful for attaching debugger during encrypted XIP.
    while (isWait)
    {
        __asm("NOP");
    }

#if defined(MBEDTLS_THREADING_ALT)
    /* Initialize Amazon libraries. Need to do this here because
     * MBEDTLS_THREADING_ALT is now needed for AWS code to work.
     * When MBEDTLS_THREADING_ALT is defined, CRYPTO_Init must be called to
     * configure the alternate threading functions. Otherwise mbedtls
     * cannot be used, so image verification, amongst others, will fail. */
    if (SYSTEM_Init() != pdPASS)
    {
        configPRINTF(("SYSTEM_Init failed\r\n"));
    }
#endif

    status = SLN_AUTH_check_context(SLN_CRYPTO_CTX_1);

    configPRINTF(("Context check status %d\r\n", status));
    // DEBUG_LOG_DELAY_MS(1000);  // Optional delay, enable for debugging to ensure log is printed before a crash

    if (SLN_AUTH_NO_CONTEXT == status)
    {
        configPRINTF(("Ensuring context...\r\n"));
        // DEBUG_LOG_DELAY_MS(1000); // Optional delay, enable for debugging to ensure log is printed before a crash

        // Load crypto contexts and make sure they are valid (our own context should be good to get to this point!)
        status = bl_nor_encrypt_ensure_context();

        if (kStatus_Fail == status)
        {
            configPRINTF(("Failed to load crypto context...\r\n"));
            // DEBUG_LOG_DELAY_MS(1000); // Optional delay, enable for debugging to ensure log is printed before a crash

            // Double check if encrypted XIP is enabled
            if (!bl_nor_encrypt_is_enabled())
            {
                configPRINTF(("Not running in encrypted XIP mode, ignore error.\r\n"));
                // DEBUG_LOG_DELAY_MS(1000); // Optional delay, enable for debugging to ensure log is printed before a
                // crash

                // No encrypted XIP enabled, we can ignore the bad status
                status = kStatus_Success;
            }
        }
        else if (kStatus_ReadOnly ==
                 status) // Using this status from standard status to indicate that we need to split PRDB
        {
            volatile uint32_t delay = 1000000;
            // Set up context as needed for this application
            status = bl_nor_encrypt_split_prdb();

            configPRINTF(("Restarting BOOTLOADER...\r\n"));

            while (delay--)
                ;

            // Restart
            DbgConsole_Deinit();
            NVIC_DisableIRQ(LPUART6_IRQn);
            NVIC_SystemReset();
        }
    }
    else if (SLN_AUTH_OK == status)
    {
        configPRINTF(("Ensuring context...\r\n"));
        // DEBUG_LOG_DELAY_MS(1000); // Optional delay, enable for debugging to ensure log is printed before a crash

        // We will check to see if we need to update the backup to the reduced scope PRDB0 for bootloader space
        status = bl_nor_encrypt_ensure_context();

        if (kStatus_Fail == status)
        {
            configPRINTF(("Failed to load crypto context...\r\n"));
            // DEBUG_LOG_DELAY_MS(1000); // Optional delay, enable for debugging to ensure log is printed before a crash

            // Double check if encrypted XIP is enabled
            if (!bl_nor_encrypt_is_enabled())
            {
                configPRINTF(("Not running in encrypted XIP mode, ignore error.\r\n"));

                // No encrypted XIP enabled, we can ignore the bad status
                status = kStatus_Success;
            }
        }
        else if (kStatus_Success == status) // We have good PRDBs so we can update the backup
        {
            bool isMatch    = false;
            bool isOriginal = false;

            configPRINTF(("Checking backup context...\r\n"));
            // DEBUG_LOG_DELAY_MS(1000); // Optional delay, enable for debugging to ensure log is printed before a crash

            // Check if we have identical KIBs and initial CTR
            status = bl_nor_crypto_ctx_compare_backup(&isMatch, &isOriginal, SLN_CRYPTO_CTX_0);

            if (kStatus_Success == status)
            {
                if (isMatch && isOriginal)
                {
                    configPRINTF(("Updating backup context with valid address space...\r\n"));
                    // DEBUG_LOG_DELAY_MS(1000); // Optional delay, enable for debugging to ensure log is printed before
                    // a crash

                    // Update backup PRDB0
                    status = SLN_AUTH_backup_context(SLN_CRYPTO_CTX_0);
                }
            }
        }
    }

    // Hold execution here. This is useful for attaching debugger during encrypted XIP.
    while (isWait)
    {
        __asm("NOP");
    }

    if (kStatus_Success == status)
    {
#if BOOTLOADER_MSD_ENABLED
        // Before we boot, check for user input for MSD update
        CheckForMSDMode();
#endif

#if BOOTLOADER_AWS_IOT_OTA_ENABLED
        // Check for a valid pending OTA update
        CheckForAppOTA();
#endif

#if BOOTLOADER_OTW_ENABLE
        // Check for a valid pending OTW update
        CheckForFwUpdateOTW();
#endif
    }

    configPRINTF(("Jumping to main application...\r\n"));

    RGB_LED_SetBrightnessColor(LED_BRIGHT_MEDIUM, LED_COLOR_OFF);

    // Launch the target application
    jumpToMainAppTask();
}
