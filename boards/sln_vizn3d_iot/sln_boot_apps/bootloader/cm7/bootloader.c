/*
 * Copyright 2019-2021 NXP.
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

#include "flash_ica_driver.h"

#include "bootloader.h"
#include "sln_rgb_led_driver.h"
#include "sln_push_buttons_driver.h"

/* Mbedtls includes */
#ifdef MBEDTLS_CONFIG_FILE
#include MBEDTLS_CONFIG_FILE
#endif /* MBEDTLS_CONFIG_FILE */

#if SDK_2_8
#include "aws_application_version.h"
#else
#ifdef AWS_VERSION_INCLUDE
#include AWS_VERSION_INCLUDE
#endif /* AWS_VERSION_INCLUDE */
#endif /* SDK_2_8 */

#if ENABLE_LOGGING
#ifdef AWS_LOGGING_INCLUDE
#include AWS_LOGGING_INCLUDE
#endif /* AWS_LOGGING_INCLUDE */
#endif

#if defined(MBEDTLS_THREADING_ALT)
#ifdef AWS_SYS_INIT_INCLUDE
#include AWS_SYS_INIT_INCLUDE
#endif /* AWS_SYS_INIT_INCLUDE */
#endif /* MBEDTLS_THREADING_ALT */

#include "sln_update.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define SET_THUMB_ADDRESS(x) (x | 0x1)

#define DEBUG_LOG_DELAY_MS(x) vTaskDelay(portTICK_PERIOD_MS *x)

// Set Temporary Stack Top to end of first block of OC RAM
#if (defined(MIMXRT106A_SERIES) || defined(MIMXRT106L_SERIES) || defined(MIMXRT106F_SERIES) || \
     defined(MIMXRT106S_SERIES))
#define TEMP_STACK_TOP (0x20208000)
#elif (defined(MIMXRT1176_cm7_SERIES) || defined(MIMXRT117F_cm7_SERIES) || defined(MIMXRT117H_cm7_SERIES))
#define TEMP_STACK_TOP (0x202C0000)
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
__attribute__((weak)) bool SLN_CheckForUpdate(void)
{
    return false;
}

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
    appaddr += FLEXSPI_AMBA_BASE;

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
            if (status != SLN_FLASH_NO_ERROR)
            {
                configPRINTF(("[WARNING] FICA_SetCurAppStartType failed\r\n"));
            }
        }

        status = FICA_get_app_img_start_addr(imgtype, &appaddr);
    }

    if (SLN_FLASH_NO_ERROR != status)
    {
        // Boot back into itself
        appaddr = SCB->VTOR - FLEXSPI_AMBA_BASE;
    }

    configPRINTF(("Launching into application at 0x%X...\r\n", appaddr));

#if ENABLE_LOGGING
    vTaskDelay(portTICK_PERIOD_MS * 100);

    DbgConsole_Flush();

    /* main app does not start correctly without this. why? */
    DbgConsole_Deinit();

    NVIC_DisableIRQ(BOARD_UART_IRQ);
#endif /* ENABLE_LOGGING */

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

#if ENABLE_LOGGING
    vTaskDelay(portTICK_PERIOD_MS * 100);

    DbgConsole_Flush();

    DbgConsole_Deinit();

    NVIC_DisableIRQ(BOARD_UART_IRQ);
#endif /* ENABLE_LOGGING */

    if (SLN_FLASH_NO_ERROR != status)
    {
        // Safer to just jump back into our vector table
        appaddr = SCB->VTOR;
    }

    // Run the Bootloader
    JumpToAddr(appaddr);
}

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

    /* This function call will return true if an update operation is needed */
    if (SLN_CheckForUpdate())
    {
        /* Suspend here, otherwise the main function will
         * launch the target application while the update task starts */
        vTaskSuspend(NULL);
    }

    configPRINTF(("Jumping to main application...\r\n"));

    RGB_LED_SetBrightnessColor(LED_BRIGHT_MEDIUM, LED_COLOR_OFF);

    // Launch the target application
    jumpToMainAppTask();
}
