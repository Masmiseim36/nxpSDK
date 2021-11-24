/*
 * Copyright 2018-2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/* Board includes */
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "sln_push_buttons_driver.h"
#include "sln_rgb_led_driver.h"

/* FreeRTOS kernel includes */
#include "FreeRTOS.h"
#include "task.h"

#if ENABLE_LOGGING
#ifdef AWS_LOGGING_INCLUDE
#include AWS_LOGGING_INCLUDE
#endif /* AWS_LOGGING_INCLUDE */
#endif /* ENABLE_LOGGING */

#include "ksdk_mbedtls.h"

#include "sln_flash.h"
#include "sln_app_specific.h"
#include "bootloader.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#if ENABLE_LOGGING
#define LOGGING_STACK_SIZE   256
#define LOGGING_QUEUE_LENGTH 64
#endif /* ENABLE_LOGGING */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

__attribute__((weak)) void APP_SpecificCode(void)
{
}

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
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    APP_SpecificCode();

    /* Initialize Flash to allow writing */
    SLN_Flash_Init();

    RGB_LED_Init();
    PUSH_BUTTONS_Init();

#if ENABLE_LOGGING
    xLoggingTaskInitialize(LOGGING_STACK_SIZE, configMAX_PRIORITIES - 2, LOGGING_QUEUE_LENGTH);
#endif /* ENABLE_LOGGING */

    xTaskCreate(BootloaderMain, "BOOTLOADER_Task", 2048, NULL, configMAX_PRIORITIES - 3, NULL);

    /* Run RTOS */
    vTaskStartScheduler();

    /* Should not reach this statement */
    while (1)
    {
    }
}
