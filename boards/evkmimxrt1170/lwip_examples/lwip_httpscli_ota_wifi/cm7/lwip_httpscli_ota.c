/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2020 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "httpsclient.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "lwip/netifapi.h"
#include "lwip/opt.h"
#include "lwip/tcpip.h"
#include "lwip/dhcp.h"
#include "lwip/prot/dhcp.h"
#include "ksdk_mbedtls.h"
#include "mflash_drv.h"
#include "mcuboot_app_support.h"
#include "fsl_debug_console.h"
#include "ota_config.h"

#include "fsl_common.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/

int initNetwork(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

void *pvPortCalloc(size_t num, size_t size)
{
    void *ptr;
    int allocSize = num * size;

    ptr = pvPortMalloc(allocSize);
    if (ptr != NULL)
    {
        memset(ptr, 0, allocSize);
    }

    return ptr;
}

/*!
 * @brief The main function containing client thread.
 */
static void httpsclient_task(void *arg)
{
    int ret;

    /* Network Initialization */

    ret = initNetwork();
    if (ret)
    {
        PRINTF("FAILED to init network (ret=%d)\n", ret);
        goto task_cleanup;
    }

    /* File Download Over TLS */

    ret = https_client_tls_init();
    if (ret != SUCCESS)
    {
        PRINTF("FAILED to init TLS (ret=%d)\n", ret);
        goto tls_cleanup;
    }

    ret = https_client_ota_download(OTA_IMAGE_PATH);
    if (ret != SUCCESS)
    {
        PRINTF("FAILED to download OTA image (ret=%d)\n", ret);
        goto tls_cleanup;
    }

    /* Mark downloaded image ready for test */

    ret = bl_update_image_state(kSwapType_ReadyForTest);
    if (ret != kStatus_Success)
    {
        PRINTF("FAILED to mark OTA image as ReadyForTest (ret=%d)\n", ret);
        goto tls_cleanup;
    }

    PRINTF("OTA image was downloaded successfully. Now press the reset button to reboot the board.\n");

tls_cleanup:
    https_client_tls_release();
task_cleanup:
    vTaskDelete(NULL);
}

/*!
 * @brief Main function.
 */
int main(void)
{
    uint32_t state;
    int ret;

    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    SCB_DisableDCache();
    CRYPTO_InitHardware();

    mflash_drv_init();

    /* Get image state */

    if (bl_get_image_state(&state) != kStatus_Success)
    {
        PRINTF("Failed to read image status, cannot continue!\n");
        goto ota_abort;
    }

    if (state == kSwapType_Testing)
    {
        PRINTF("Current image is marked as TEST image.\n");
        PRINTF("Assuming all is OK and marking the image as PERMANENT.\n");

        ret = bl_update_image_state(kSwapType_Permanent);
        if (ret != kStatus_Success)
        {
            PRINTF("FAILED to mark OTA image as PERMANENT (ret=%d)\n", ret);
            goto ota_abort;
        }

        PRINTF("Press the reset button to reboot the board\n");
        goto ota_abort;
    }

    /* Start the OTA */

    if (xTaskCreate(httpsclient_task, "httpsclient_task", 2048, NULL, configMAX_PRIORITIES - 4 /*3*/, NULL) != pdPASS)
    {
        PRINTF("Task creation failed!\r\n");
        while (1)
            ;
    }

    /* Run RTOS */
    vTaskStartScheduler();

ota_abort:
    /* Should not reach this statement */
    for (;;)
        ;
}
