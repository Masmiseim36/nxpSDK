/*
 * Copyright 2018 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/* Board includes */
#include "board.h"
#include "pin_mux.h"
#include "fsl_debug_console.h"

/* FreeRTOS kernel includes */
#include "FreeRTOS.h"
#include "task.h"

/* Crypto includes */
#include "ksdk_mbedtls.h"
#include "mbedtls/base64.h"

/* AWS includes */
#include AWS_LOGGING_INCLUDE

/* USB includes */
#include "serial_manager.h"
#include "usb_device_config.h"
#include "usb_phy.h"
#include "usb.h"

#include "sln_flash_mgmt.h"

#if BOOTLOADER_WIFI_ENABLED
#include "wifi_credentials.h"
#endif

#include "sln_flash.h"

#if BOOTLOADER_AWS_IOT_OTA_ENABLED
#include "sln_ota.h"
#else
#include "aws_application_version.h"
#endif

#include "bootloader.h"
#include "sln_auth.h"
#include "sln_file_table.h"
#include "nor_encrypt_bee.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define LOGGING_STACK_SIZE 256
#define LOGGING_QUEUE_LENGTH 64

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

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

void BusFault_Handler(void)
{
#if 0
   volatile bool isWait = true;

   while(isWait)
   {
      __asm("NOP");
   }
#else
    __asm("BKPT #0");
#endif
}

int main(void)
{
#if RELOCATE_VECTOR_TABLE
    BOARD_RelocateVectorTableToRam();
#endif

    BOARD_ConfigMPU();

    /* Enable additional fault handlers */
    SCB->SHCSR |= (SCB_SHCSR_BUSFAULTENA_Msk | /*SCB_SHCSR_USGFAULTENA_Msk |*/ SCB_SHCSR_MEMFAULTENA_Msk);

    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    USB_DeviceClockInit();
    BOARD_InitDebugConsole();

    /* Setup Crypto HW */
    bl_nor_encrypt_set_key();
    CRYPTO_InitHardware();

    /* Initialize Flash to allow writing */
    SLN_Flash_Init();

    /* Initialize flash management */
    SLN_FLASH_MGMT_Init((sln_flash_entry_t *)g_fileTable, false);

    PRINTF("\r\n\r\n*** BOOTLOADER v%d.%d.%d ***\r\n\r\n", localAppFirmwareVersion.u.x.ucMajor,
           localAppFirmwareVersion.u.x.ucMinor, localAppFirmwareVersion.u.x.usBuild);

    xLoggingTaskInitialize(LOGGING_STACK_SIZE, configMAX_PRIORITIES - 2, LOGGING_QUEUE_LENGTH);

    xTaskCreate(BootloaderMain, "BOOTLOADER_Task", 2048, NULL, configMAX_PRIORITIES - 3, NULL);

    /* Run RTOS */
    vTaskStartScheduler();

    /* Should not reach this statement */
    while (1)
    {
    }
}
