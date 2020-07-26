/*
 * Copyright 2018 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.d
 */

/* Board includes */
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
#include "fica_definition.h"

#include "sln_updater.h"

/*******************************************************************************
 * Code
 ******************************************************************************/

__attribute__((naked)) void HardFault_Handler(void)
{
    configPRINTF(("HardFault_Handler reached ... \r\n"));
    vTaskDelay(1000);

    __asm("BKPT #0");
}

static void the_should_never_get_here_catch(void)
{
    configPRINTF(("ERROR: Unable to boot, restarting...\r\n"));
    vTaskDelay(2000);
    NVIC_SystemReset();
}

void updater_task(void *arg)
{
    status_t updaterStatus    = kStatus_Success;
    bundle_meta_t bundle_meta = {0};
    int32_t fica_status       = 0;

    vTaskDelay(1000);

    /* Make sure FICA internal variables are initialized */
    fica_status = FICA_initialize();

    if (SLN_FLASH_NO_ERROR != fica_status)
    {
        configPRINTF(("FICA initialization failed, err: %d\r\n", fica_status));
        updaterStatus = kStatus_Fail;
    }

    if (kStatus_Success == updaterStatus)
    {
        /* Get updater bundle metadata */
        updaterStatus = upd_bundle_meta_get(&bundle_meta);
    }

    if (kStatus_Success == updaterStatus)
    {
        /* Validate updater bundle metadata */
        updaterStatus = upd_bundle_meta_validate(&bundle_meta);
    }

    if (kStatus_Success == updaterStatus)
    {
        /* Validate modules metadata */
        updaterStatus = upd_mod_meta_validate_all(&bundle_meta);
    }

    if (kStatus_Success == updaterStatus)
    {
        /* Check if modules dependencies are met */
        updaterStatus = upd_mod_dep_check(&bundle_meta);
    }

    if (kStatus_Success == updaterStatus)
    {
        /* Iterate through each module for validation
         * (certificate, signature, IVT address, etc.) */
        updaterStatus = upd_mod_validate_all(&bundle_meta);
    }

    if (kStatus_Success == updaterStatus)
    {
        /* Iterate through each module, perform update */
        updaterStatus = upd_mod_update_all(&bundle_meta);
    }

    if (kStatus_Success == updaterStatus)
    {
        configPRINTF(("All modules were successfully updated!.\r\n"));
    }
    else
    {
        configPRINTF(("ERROR during update!.\r\n"));
    }

    /* Apply needed settings from running from the other bank after reset */
    upd_mod_restore_main_app_settings(&bundle_meta, updaterStatus);

    configPRINTF(("Resetting ...\r\n"));

    /* give some time for last logs printing */
    vTaskDelay(1000);

    /* good bye, cruel world! */
    NVIC_SystemReset();

    vTaskDelete(NULL);
}

int main(void)
{
    volatile uint32_t wait = 0;

#if RELOCATE_VECTOR_TABLE
    BOARD_RelocateVectorTableToRam();
#endif

    BOARD_ConfigMPU();

    /* Enable additional fault handlers */
    SCB->SHCSR |= (SCB_SHCSR_BUSFAULTENA_Msk | SCB_SHCSR_USGFAULTENA_Msk | SCB_SHCSR_MEMFAULTENA_Msk);

    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    while (wait)
    {
        __asm("nop");
    }

    PRINTF("\r\n");
    configPRINTF(("Starting Updater ...\r\n"));

    /* Setup Crypto HW */
    bl_nor_encrypt_set_key();
    CRYPTO_InitHardware();

    /* Initialize Flash to allow writing */
    SLN_Flash_Init();

    /* Initialize flash management */
    SLN_FLASH_MGMT_Init((sln_flash_entry_t *)g_fileTable, false);

    /* Start updater task */
    xTaskCreate(updater_task, "Updater_Task", 4096, NULL, configMAX_PRIORITIES - 1, NULL);

    /* Run RTOS */
    vTaskStartScheduler();

    /* Uh oh... */
    the_should_never_get_here_catch();

    /* ...again we should never get here. */
    return 0;
}
