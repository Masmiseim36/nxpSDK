/*
 * Copyright 2018-2020 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/* Board includes */
#include "pin_mux.h"
#include "board.h"
#include "clock_config.h"
#include "fsl_debug_console.h"

/* FreeRTOS kernel includes */
#include "FreeRTOS.h"
#include "task.h"

/* AWS includes */
#if BOOTLOADER_AWS_IOT_OTA_ENABLED
#include "aws_clientcredential_keys.h"
#endif
#include AWS_LOGGING_INCLUDE

/* Crypto includes */
#include "ksdk_mbedtls.h"
#include "mbedtls/base64.h"

#if BOOTLOADER_WIFI_ENABLED
#include "wifi_credentials.h"
#endif

#include "sln_flash.h"
#include "sln_flash_mgmt.h"
#include "sln_RT10xx_RGB_LED_driver.h"

#include "bootloader.h"
#include "sln_auth.h"
#include "sln_file_table.h"
#include "nor_encrypt_bee.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define LOGGING_STACK_SIZE   256
#define LOGGING_QUEUE_LENGTH 64

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

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
    /* Enable additional fault handlers */
    SCB->SHCSR |= (SCB_SHCSR_BUSFAULTENA_Msk | /*SCB_SHCSR_USGFAULTENA_Msk |*/ SCB_SHCSR_MEMFAULTENA_Msk);

    /* Init board hardware */
    /* Relocate Vector Table */
#if RELOCATE_VECTOR_TABLE
    BOARD_RelocateVectorTableToRam();
#endif

    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /* Setup Crypto HW */
    bl_nor_encrypt_set_key();
    CRYPTO_InitHardware();

    /* Initialize Flash to allow writing */
    SLN_Flash_Init();

    /* Initialize flash management */
    SLN_FLASH_MGMT_Init((sln_flash_entry_t *)g_fileTable, false);

    /* Initialize RGB LED */
    RGB_LED_Init();

    xLoggingTaskInitialize(LOGGING_STACK_SIZE, configMAX_PRIORITIES - 2, LOGGING_QUEUE_LENGTH);

    xTaskCreate(BootloaderMain, "BOOTLOADER_Task", 2048, NULL, configMAX_PRIORITIES - 3, NULL);

    /* Run RTOS */
    vTaskStartScheduler();

    /* Should not reach this statement */
    while (1)
    {
    }
}
