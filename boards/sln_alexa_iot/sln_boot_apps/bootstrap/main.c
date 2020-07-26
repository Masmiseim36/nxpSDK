/*
 * Copyright 2019 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.d
 */

/* Board includes */
#include "app.h"
#include "board.h"
#include "pin_mux.h"
#include "fsl_common.h"
#include "fsl_debug_console.h"

/* FreeRTOS kernel includes */
#include "FreeRTOS.h"
#include "task.h"

/* Security includes */
#include "ksdk_mbedtls.h"
#include "sln_auth.h"
#include "fusemap.h"

/* Flash includes */
#include "fica_definition.h"
#include "flexspi_hyper_flash_ops.h"
#include "fsl_flexspi.h"
#include "sln_flash.h"
#include "sln_flash_mgmt.h"
#include "sln_file_table.h" /* NOTE: Must be last to capture all file names. */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define SET_THUMB_ADDRESS(x) (x | 0x1)

#ifndef UNSECURE_JUMP_ADDRESS
#define UNSECURE_JUMP_ADDRESS FICA_IMG_BOOTLOADER_ADDR
#endif

#define NO_CONTEXT(x) (1U << x)
#define NO_CONTEXT_0 (1U << 0)
#define NO_CONTEXT_1 (1U << 1)
#define NO_CONTEXT_ALL (NO_CONTEXT_0 | NO_CONTEXT_1)

#define SECURITY_VIOLATION_MASK 0x0000003F
#define JTAG_VIOLATION 0x00000002
#define WDOG_VIOLATION 0x00000004
#define GPIO_VIOLATION 0x00000020
#define UNKNOWN_VIOLATION 0x00000019

typedef enum _security_violation
{
    SEC_NO_VIOLATION = 0,
    SEC_JTAG_VIOLATION,
    SEC_WDOG_VIOLATION,
    SEC_GPIO_VIOLATION,
    SEC_UNKOWN_VIOLATION,
} security_violation_t;

const char *kViolationStr[] = {"No security violation.", "ERROR: JTAG active violation!",
                               "ERROR: WDOG2 reset violation!", "ERROR: GPIO_EMC_19 violation!",
                               "ERROR: UNKNOWN violation!"};

#define ENABLE_CONTEXT_DUMP 0

// Set Temporary Stack Top to end of first block of OC RAM
extern void __base_SRAM_OC_NON_CACHEABLE(void);
#define TEMP_STACK_TOP (__base_SRAM_OC_NON_CACHEABLE + 0x8000)

typedef enum _bootstrap_status
{
    BOOT_SUCCESS        = kStatus_Success,
    BOOT_FAIL           = -1,
    BOOT_INVALID        = -2,
    BOOT_NULL_PTR       = -3,
    BOOT_NO_CONTEXT_0   = -4,
    BOOT_NO_CONTEXT_1   = -5,
    BOOT_NO_CONTEXT_ALL = -6,
} bootstrap_status_t;

typedef void (*app_entry_t)(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

app_entry_t appEntry     = 0;   // Application Entry Function for the jump
static fica_t s_thisFica = {0}; // FICA data structure

#if ENABLE_CONTEXT_DUMP
uint8_t tempCtxBuf[0x150] = {0};
#warning "ARE YOU SURE YOU WANT TO PRINT THIS INFORMATION OUT?"
#endif

/*******************************************************************************
 * Code
 ******************************************************************************/

__attribute__((naked)) void HardFault_Handler(void)
{
    __asm("BKPT #0");
}

static void print_security_violation(uint32_t status)
{
    security_violation_t type = SEC_NO_VIOLATION;

    if (JTAG_VIOLATION & status)
    {
        type = SEC_JTAG_VIOLATION;
        configPRINTF(("%s\r\n", (const char *)kViolationStr[type]));
    }

    if (WDOG_VIOLATION & status)
    {
        type = SEC_WDOG_VIOLATION;
        configPRINTF(("%s\r\n", (const char *)kViolationStr[type]));
    }

    if (GPIO_VIOLATION & status)
    {
        type = SEC_JTAG_VIOLATION;
        configPRINTF(("%s\r\n", (const char *)kViolationStr[type]));
    }

    if (GPIO_VIOLATION & status)
    {
        type = SEC_JTAG_VIOLATION;
        configPRINTF(("%s\r\n", (const char *)kViolationStr[type]));
    }
}

__attribute__((section(".data.$SRAM_DTC"))) static int32_t read_entry_point(uint32_t address, uint32_t *data)
{
    *data = *((uint32_t *)(address));

    return kStatus_Success;
}

static void the_should_never_get_here_catch(void)
{
    configPRINTF(("ERROR: Unable to boot, restarting...\r\n"));
    vTaskDelay(2000);
    NVIC_SystemReset();
}

static void jump_to_application(uint32_t appStartAddr)
{
    uint32_t appEntryPointAddr = 0;
    uint32_t entryPointCheck   = 0;
    uint32_t appVectorTable    = 0;

    if (FlexSPI_AMBA_BASE > appStartAddr)
    {
        appVectorTable = SLN_Flash_Get_Read_Address(appStartAddr);
    }
    else if ((FlexSPI_AMBA_BASE + FLASH_SIZE) <= appStartAddr)
    {
        // Leave this place!
        return;
    }
    else
    {
        appVectorTable = appStartAddr;
    }

    appEntryPointAddr = (*(uint32_t *)(appVectorTable + 4));

    // Point entry point address to entry point call function
    appEntry = (app_entry_t)(SET_THUMB_ADDRESS(appEntryPointAddr));

    // Leap of faith... after checking what's there first!
    read_entry_point(appVectorTable + 4, &entryPointCheck);
    if ((0xFFFFFFFF == entryPointCheck) || (0x00000000 == entryPointCheck))
    {
        the_should_never_get_here_catch();
    }
    else
    {
        configPRINTF(("Attempting to jump to 0x%X...\r\n", (uint32_t)appEntry));

        vTaskDelay(portTICK_PERIOD_MS * 50);

        DbgConsole_Flush();

        DbgConsole_Deinit();

        NVIC_DisableIRQ(LPUART6_IRQn);

        // Set the VTOR to the application vector table address.
        SCB->VTOR = (uint32_t)appVectorTable;

        // Set stack pointers to the application stack pointer.
        __set_MSP((uint32_t)TEMP_STACK_TOP);
        __set_PSP((uint32_t)TEMP_STACK_TOP);

        // Jump to main app entry point
        appEntry();
    }
}

static bootstrap_status_t check_prdb(void)
{
    sln_auth_status_t status = SLN_AUTH_OK;
    bootstrap_status_t ret   = BOOT_SUCCESS;
    uint8_t failMask         = 0;

    for (uint32_t ctx = 0; ctx < SLN_CRYPTO_CTX_COUNT; ctx++)
    {
        status = SLN_AUTH_check_context(ctx);

        if (SLN_AUTH_NO_CONTEXT == status)
        {
            configPRINTF(("Context %d is empty.\r\n", ctx));
            failMask |= NO_CONTEXT(ctx);
        }

        if ((SLN_AUTH_OK != status) && (SLN_AUTH_NO_CONTEXT != status))
        {
            configPRINTF(("ERROR: Error checking context, %d.\r\n", status));
            failMask = 0;
            ret      = BOOT_FAIL;
            break;
        }
    }

    switch (failMask)
    {
        case NO_CONTEXT_0:
            ret = BOOT_NO_CONTEXT_0;
            break;
        case NO_CONTEXT_1:
            ret = BOOT_NO_CONTEXT_1;
            break;
        case NO_CONTEXT_ALL:
            ret = BOOT_NO_CONTEXT_ALL;
            break;
        default:
            break;
    }

    return ret;
}

static bootstrap_status_t restore_prdb(bootstrap_status_t ctxStatus)
{
    bootstrap_status_t bootStatus = BOOT_SUCCESS;
    bool isEncryptXIP             = ROM_OCOTP_ENCRYPT_XIP_VALAUE();
    bool isKey0SelSet             = (ROM_OCOTP_BEE_KEY0_SEL_VALUE() > 0);
    bool isKey1SelSet             = (ROM_OCOTP_BEE_KEY1_SEL_VALUE() > 0);

    if (isEncryptXIP && (isKey0SelSet || isKey1SelSet))
    {
        sln_auth_status_t authStatus = SLN_AUTH_OK;

        configPRINTF(("Running secure context...\r\n"));

        if (BOOT_SUCCESS == ctxStatus)
        {
            // Nothing to restore!
            configPRINTF(("...nothing to restore!\r\n"));
        }
        else if (BOOT_NO_CONTEXT_0 == ctxStatus)
        {
            configPRINTF(("Restoring context 0...\r\n"));

            // Restore context configuraiton 0
            authStatus = SLN_AUTH_swap_context(SLN_CRYPTO_CTX_0);
        }
        else if (BOOT_NO_CONTEXT_1 == ctxStatus)
        {
            configPRINTF(("Restoring context 0...\r\n"));

            // Restore context configuration 1
            authStatus = SLN_AUTH_swap_context(SLN_CRYPTO_CTX_1);
        }
        else if (BOOT_NO_CONTEXT_ALL == ctxStatus)
        {
            configPRINTF(("Restoring all context..\r\n"));

            // Restore all context configurations
            for (uint32_t ctx = 0; ctx < SLN_CRYPTO_CTX_COUNT; ctx++)
            {
                authStatus = SLN_AUTH_swap_context(ctx);

                if (SLN_AUTH_OK != authStatus)
                {
                    break;
                }
            }
        }
        else
        {
            authStatus = SLN_AUTH_INVALID_ARG;
        }

        if ((BOOT_SUCCESS == ctxStatus) && (SLN_AUTH_OK == authStatus))
        {
            configPRINTF(("Carrying on as normal.\r\n"));
        }
        else if ((BOOT_SUCCESS != ctxStatus) && (SLN_AUTH_OK == authStatus))
        {
            configPRINTF(("Rebooting to use new restored context...\r\n"));
            vTaskDelay(2000);
            NVIC_SystemReset();
        }
        else if ((BOOT_SUCCESS != ctxStatus) && (SLN_AUTH_NO_CONTEXT == authStatus))
        {
            configPRINTF(("WARNING: restore context missing, could be first time...\r\n"));
            authStatus = SLN_AUTH_OK; // No context, we can assume first boot and set status OK
        }
        else if ((BOOT_SUCCESS != ctxStatus) && (SLN_AUTH_INVALID_ARG == authStatus))
        {
            configPRINTF(("ERROR: invalid context argument provided, %d\r\n", ctxStatus));
        }
        else
        {
            configPRINTF(("ERROR: context restore failure with status %d\r\n", authStatus));
        }

        if (SLN_AUTH_OK != authStatus)
        {
            bootStatus = BOOT_FAIL;
        }
    }
    else
    {
        configPRINTF(("Not running encrypted XIP, no restore.\r\n"));
    }

    return bootStatus;
}

#if !DISABLE_IMAGE_VERIFICATION
static bootstrap_status_t verify_image(fica_img_type_t imgType)
{
    bootstrap_status_t bootStatus = BOOT_SUCCESS;
    uint8_t *img                  = NULL;
    uint8_t *caRootPem            = NULL;
    uint8_t *caLeafPem            = NULL;
    uint32_t imgLen               = 0;
    uint8_t *imgSig               = NULL;

    if ((FICA_IMG_TYPE_NONE == imgType) || (FICA_NUM_IMG_TYPES <= imgType))
    {
        configPRINTF(("ERROR: Invalid imgType provided! Provided imgType = %d\r\n", imgType));
        bootStatus = BOOT_INVALID;
    }

    if (BOOT_SUCCESS == bootStatus)
    {
        status_t flashStatus = kStatus_Success;

        flashStatus = SLN_Read_Flash_At_Address(FICA_START_ADDR, (uint8_t *)&s_thisFica, sizeof(fica_t));

        if (kStatus_Success != flashStatus)
        {
            configPRINTF(("ERROR: Unable to read flash from address 0x%X!\r\n", (uint32_t)(&s_thisFica)));
            bootStatus = BOOT_FAIL;
        }
    }

    if (BOOT_SUCCESS == bootStatus)
    {
        uint8_t *temp = NULL;

        /* Grab the start of the image address from the FICA */
        img = (uint8_t *)(FlexSPI_AMBA_BASE + s_thisFica.records[imgType].imgAddr);

        // Calculate the image length without certificate
        imgLen = s_thisFica.records[imgType].imgLen - MAX_CERT_LEN;

        temp = (uint8_t *)img;
        temp += imgLen;

        if (strncmp((const char *)temp, "-----BEGIN CERTIFICATE-----", strlen("-----BEGIN CERTIFICATE-----")))
        {
            // No certificate at end, we can use entire image length (factory application load)
            imgLen = s_thisFica.records[imgType].imgLen;
        }

        imgSig = (uint8_t *)(s_thisFica.records[imgType].imgPkiSig);

        // Check if img address makes sense
        if ((FlexSPI_AMBA_BASE > (uint32_t)img) || ((FlexSPI_AMBA_BASE + FLASH_SIZE) < (uint32_t)img))
        {
            configPRINTF(("ERROR: Invalid img address 0x%X\r\n", (uint32_t)img));
            bootStatus = BOOT_INVALID;
        }
    }

    if (BOOT_SUCCESS == bootStatus)
    {
        sln_flash_mgmt_status_t fileStatus = SLN_FLASH_MGMT_OK;
        uint32_t caRootLen                 = 0;
        uint32_t caLeafLen                 = 0;

        fileStatus = SLN_FLASH_MGMT_Read(ROOT_CA_CERT, NULL, &caRootLen);

        if (SLN_FLASH_MGMT_OK == fileStatus)
        {
            // Found a file
            caRootPem = (uint8_t *)pvPortMalloc(caRootLen);
            memset(caRootPem, 0, caRootLen);

            if (NULL == caRootPem)
            {
                fileStatus = SLN_FLASH_MGMT_ENOMEM;
            }

            fileStatus = SLN_FLASH_MGMT_Read(ROOT_CA_CERT, caRootPem, &caRootLen);
        }

        if (SLN_FLASH_MGMT_OK == fileStatus)
        {
            // Allocate MAX cert len just to be safe
            caLeafPem = (uint8_t *)pvPortMalloc(MAX_CERT_LEN);
            memset(caLeafPem, 0, MAX_CERT_LEN);

            if (NULL == caLeafPem)
            {
                fileStatus = SLN_FLASH_MGMT_ENOMEM;
            }
            else
            {
                switch (imgType)
                {
                    case FICA_IMG_TYPE_BOOTLOADER:
                        fileStatus = SLN_FLASH_MGMT_Read(CRED_SIGNING_CERT, caLeafPem, &caLeafLen);
                        break;
                    case FICA_IMG_TYPE_APP_A:
                        fileStatus = SLN_FLASH_MGMT_Read(APP_A_SIGNING_CERT, caLeafPem, &caLeafLen);
                        break;
                    case FICA_IMG_TYPE_APP_B:
                        fileStatus = SLN_FLASH_MGMT_Read(APP_B_SIGNING_CERT, caLeafPem, &caLeafLen);
                        break;
                    default:
                        fileStatus = SLN_FLASH_MGMT_EINVAL;
                        break;
                }
            }
        }

        if (SLN_FLASH_MGMT_OK != fileStatus)
        {
            configPRINTF(("ERROR: Unable to read certificates! File Status: %d\r\n", fileStatus));
            bootStatus = BOOT_FAIL;
        }
    }

    if (BOOT_SUCCESS == bootStatus)
    {
        sln_auth_status_t authStatus = SLN_AUTH_OK;

        authStatus = SLN_AUTH_Verify_Cert(caRootPem, caLeafPem);

        if (SLN_AUTH_OK == authStatus)
        {
            authStatus = SLN_AUTH_Verify_Signature(caLeafPem, img, imgLen, imgSig);
        }

        if (SLN_AUTH_OK != authStatus)
        {
            configPRINTF(("EROOR: Unable to verify image authenticity, %d\r\n", authStatus));
            bootStatus = BOOT_FAIL;
        }
    }

    vPortFree(caRootPem);
    caRootPem = NULL;
    vPortFree(caLeafPem);
    caLeafPem = NULL;

    return bootStatus;
}
#endif

void bootstrap_task(void *arg)
{
    bootstrap_status_t status = BOOT_SUCCESS;
    status_t ficaStatus       = kStatus_Fail;
    fica_img_type_t imgType   = FICA_IMG_TYPE_NONE;

    // Check for boot configuration
    status = check_prdb();

#if ENABLE_CONTEXT_DUMP
    SLN_Read_Flash_At_Address(0x400, tempCtxBuf, 0x150);

    for (uint32_t idx = 0; idx < 0x150; idx++)
    {
        if (0 == (idx % 16))
        {
            PRINTF("\r\n");
        }
        PRINTF("%02X", tempCtxBuf[idx]);
        vTaskDelay(10);
    }

    PRINTF("\r\n");

    SLN_Read_Flash_At_Address(0x800, tempCtxBuf, 0x150);

    for (uint32_t idx = 0; idx < 0x150; idx++)
    {
        if (0 == (idx % 16))
        {
            PRINTF("\r\n");
        }
        PRINTF("%02X", tempCtxBuf[idx]);
        vTaskDelay(10);
    }

    PRINTF("\r\n");
#endif

    vTaskDelay(100);

    // Will take check_prdb status and perform restore if needed
    status = restore_prdb(status);

    if (BOOT_SUCCESS == status)
    {
        configPRINTF(("Reading FICA table...\r\n"));
        ficaStatus = SLN_Read_Flash_At_Address(FICA_START_ADDR, (uint8_t *)&s_thisFica, sizeof(fica_t));
    }

    if (kStatus_Success == ficaStatus)
    {
        configPRINTF(("Attempting to load bootloader...\r\n"));
        // Get current boot type
        imgType = s_thisFica.header.currBootType;

#if DISABLE_IMAGE_VERIFICATION
#warning "IMAGE VERIFICATION DISABLED! THIS IS NOT RECOMMENDED."
        // NOTE: This is not recommended for a production application
        status = BOOT_SUCCESS;
#else
        // Verify current bootable image
        status = verify_image(imgType);
#endif
    }

    /* Make sure we loaded the fica correctly and we failed to boot into the bootloader */
    if ((BOOT_SUCCESS != status) && (kStatus_Success == ficaStatus))
    {
        imgType = s_thisFica.header.currType;
        configPRINTF(("Unable to load bootloader, searching for application type %d...\r\n", imgType));

#if DISABLE_IMAGE_VERIFICATION
#warning "IMAGE VERIFICATION DISABLED! THIS IS NOT RECOMMENDED."
        // NOTE: This is not recommended for a production application
        status = BOOT_SUCCESS;
#else
        // Verify application in current FICA table
        status = verify_image(imgType);
#endif
    }

    /* Make sure we loaded the fica correctly and we failed to boot into the current fica images */
    if ((BOOT_SUCCESS != status) && (kStatus_Success == ficaStatus))
    {
        /* If the current type is APP A, try and boot APP B. Don't save it as it shouldn't persist in a fallback image
         */
        /* This could be an issue with OTA in the future but the fact that we are here means OTA won't work as we don't
         * don't have a valid bootloader to do it for us!
         */
        imgType = (s_thisFica.header.currType == FICA_IMG_TYPE_APP_A ? FICA_IMG_TYPE_APP_B : FICA_IMG_TYPE_APP_A);

        configPRINTF(("Unable to load application type %d, searching for application type %d...\r\n",
                      s_thisFica.header.currType, imgType));

#if DISABLE_IMAGE_VERIFICATION
#warning "IMAGE VERIFICATION DISABLED! THIS IS NOT RECOMMENDED."
        // NOTE: This is not recommended for a production application
        status = BOOT_SUCCESS;
#else
        // Verify fallback application
        status = verify_image(imgType);
#endif
    }

    if (BOOT_SUCCESS == status)
    {
        // Launch next application
#if DISABLE_IMAGE_VERIFICATION
        jump_to_application(UNSECURE_JUMP_ADDRESS); // HACK: Jump to where a bootloader should be
#else
        jump_to_application(s_thisFica.records[imgType].imgAddr);
#endif
    }

    the_should_never_get_here_catch();
}

int main(void)
{
    uint32_t isSecurityViolation = 0;

#if RELOCATE_VECTOR_TABLE
    BOARD_RelocateVectorTableToRam();
#endif

    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitWifi();
    BOARD_InitDebugConsole();

    CRYPTO_InitHardware();

    /* Initialize Flash to allow writing */
    SLN_Flash_Init();

    /* Initialize flash management */
    SLN_FLASH_MGMT_Init((sln_flash_entry_t *)g_fileTable, false);

    PRINTF("\r\n");
    configPRINTF(("Booting...\r\n"));

    isSecurityViolation = (SECURITY_VIOLATION_MASK & SNVS->HPSVSR);

    print_security_violation(isSecurityViolation);

    /* Setup task for playing offline audio */
    if (xTaskCreate(bootstrap_task, "Bootstrap_Task", 512, NULL, configMAX_PRIORITIES - 1, NULL) != pdPASS)
    {
        PRINTF("ERROR: Unable to create bootstrap_task.\r\n");
    }

    /* Run RTOS */
    vTaskStartScheduler();

    // Uh oh...
    the_should_never_get_here_catch();

    // ...again we should never get here.
    return 0;
}
