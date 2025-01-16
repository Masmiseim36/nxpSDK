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
#include <stdio.h>
#include <ctype.h>

#include "httpsclient.h"
#include "board.h"
#include "lwip/netifapi.h"
#include "lwip/opt.h"
#include "lwip/tcpip.h"
#include "lwip/dhcp.h"
#include "lwip/prot/dhcp.h"
#include "app.h"
#include "mflash_drv.h"
#include "fsl_debug_console.h"
#include "ota_config.h"
#include "network_cfg.h"
#include "fsl_shell.h"
#include "sysflash/sysflash.h"
#include "flash_map.h"
#include "flash_helper.h"
#include "mcuboot_app_support.h"

#ifdef WIFI_MODE
#include "wpl.h"
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

int initNetwork(void);

static shell_status_t shellCmd_ota(shell_handle_t shellHandle, int32_t argc, char **argv);
static shell_status_t shellCmd_image(shell_handle_t shellHandle, int32_t argc, char **argv);
static shell_status_t shellCmd_reboot(shell_handle_t shellHandle, int32_t argc, char **argv);

#ifdef WIFI_MODE
static shell_status_t shellCmd_wifi(shell_handle_t shellHandle, int32_t argc, char **argv);
#endif

/*******************************************************************************
 * Variables
 ******************************************************************************/

static SHELL_COMMAND_DEFINE(ota,
                            "\n\"ota <imageNumber> <filePath> <host> <port>\": Starts download of OTA image\n",
                            shellCmd_ota,
                            SHELL_IGNORE_PARAMETER_COUNT);

static SHELL_COMMAND_DEFINE(image,
                            "\n\"image [info]\"          : Print image information"
                            "\n\"image test [imgNum]\"   : Mark candidate slot of given image number as ready for test"
                            "\n\"image accept [imgNum]\" : Mark active slot of given image number as accepted"
                            "\n\"image erase [imgNum]\"  : Erase candidate slot of given image number"
                            "\n",
                            shellCmd_image,
                            SHELL_IGNORE_PARAMETER_COUNT);

static SHELL_COMMAND_DEFINE(reboot, "\n\"reboot\": Triggers software reset\n", shellCmd_reboot, 0);

#ifdef WIFI_MODE
static SHELL_COMMAND_DEFINE(wifi,
                            "\n\"wifi conf [ssid pass]\" : Get/Set WiFi SSID and passphrase"
                            "\n\"wifi join\"             : Connect to network"
                            "\n\"wifi leave\"            : Disconnect from network"
                            "\n",
                            shellCmd_wifi,
                            SHELL_IGNORE_PARAMETER_COUNT);

static char wifi_ssid[32 + 1] = WIFI_SSID;
static char wifi_pass[64 + 1] = WIFI_PASSWORD;
#endif

SDK_ALIGN(static uint8_t s_shellHandleBuffer[SHELL_HANDLE_SIZE], 4);
static shell_handle_t s_shellHandle;

/*******************************************************************************
 * Code
 ******************************************************************************/

#ifdef WIFI_MODE
static shell_status_t shellCmd_wifi(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    const char *action   = argv[1];
    static int connected = 0;

    if (argc > 4)
    {
        PRINTF("Too many arguments.\n");
        return kStatus_SHELL_Error;
    }

    /* wifi conf [ssid password] */

    if (!strcmp(action, "conf"))
    {
        /* SSID */
        if (argc > 2)
        {
            const char *ssid = argv[2];

            if (strlen(ssid) > sizeof(wifi_ssid) - 1)
            {
                PRINTF("SSID too long (max %d)\n", sizeof(wifi_ssid) - 1);
                return kStatus_SHELL_Error;
            }
            strcpy(wifi_ssid, ssid);
        }

        /* Password */
        if (argc > 3)
        {
            const char *pass = argv[3];

            if (strlen(pass) > sizeof(wifi_pass) - 1)
            {
                PRINTF("Passphrase too long (max %d)\n", sizeof(wifi_pass) - 1);
                return kStatus_SHELL_Error;
            }
            strcpy(wifi_pass, pass);
        }

        PRINTF("SSID \"%s\"; Passphrase \"%s\"\n", wifi_ssid, wifi_pass);
    }

    /* wifi join */

    else if (!strcmp(action, "join"))
    {
        int result;

        if (connected)
        {
            PRINTF("Already connected\n");
            return kStatus_SHELL_Success;
        }

        result = WPL_AddNetwork(wifi_ssid, wifi_pass, "ota");
        if (result != WPLRET_SUCCESS)
        {
            PRINTF("Failed to create wifi network descriptor (%d)\n", result);
            return kStatus_SHELL_Error;
        }

        PRINTF("Joining: \"%s\"\n", wifi_ssid);

        result = WPL_Join("ota");
        if (result != WPLRET_SUCCESS)
        {
            PRINTF("Failed to join WiFi network.\n");
            WPL_RemoveNetwork("ota");
            return kStatus_SHELL_Error;
        }

        PRINTF("Successfully joined: \"%s\"\n", wifi_ssid);

        char ip[16];
        WPL_GetIP(ip, 1);

        PRINTF("Got IP address %s\n", ip);

        connected = 1;
    }

    /* wifi leave */

    else if (!strcmp(action, "leave"))
    {
        if (!connected)
        {
            PRINTF("No connection\n");
            return kStatus_SHELL_Success;
        }

        WPL_Leave();
        WPL_RemoveNetwork("ota");

        connected = 0;
    }

    else
    {
        PRINTF("Wrong arguments. See 'help'\n");
        return kStatus_SHELL_Error;
    }

    return kStatus_SHELL_Success;
}
#endif

static shell_status_t shellCmd_ota(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    int ret, image;
    size_t image_size;
    partition_t storage;

    /* Initialized with default values */

    char *path = OTA_IMAGE_PATH_DEFAULT;
    char *host = OTA_SERVER_NAME_DEFAULT;
    char *port = OTA_SERVER_PORT_DEFAULT;

    if (argc < 2)
    {
        PRINTF("Image number must be specified; Use 'image' for image details.\n");
        return kStatus_SHELL_Error;
    }

    if (!isdigit((int)argv[1][0]))
    {
        PRINTF("Bad image number\n");
        return kStatus_SHELL_Error;
    }
    image = atoi(argv[1]);

    if (argc > 2)
        path = argv[2];
    if (argc > 3)
        host = argv[3];
    if (argc > 4)
        port = argv[4];

    if (argc > 5)
    {
        PRINTF("Too many arguments.\n");
        return kStatus_SHELL_Error;
    }

    if (image < 0 || image >= MCUBOOT_IMAGE_NUMBER)
    {
        PRINTF("Image number out of range.\n");
        return kStatus_SHELL_Error;
    }

    if (bl_get_update_partition_info(image, &storage) != kStatus_Success)
    {
        PRINTF("FAILED to determine address for download\n");
        return kStatus_SHELL_Error;
    }

    PRINTF(
        "Started OTA with:\n"
        "    image = %d\n"
        "   offset = 0x%X\n"
        "     file = %s\n"
        "     host = %s\n"
        "     port = %s\n",
        image, storage.start, path, host, port);

    /* File Download Over TLS */

    ret = https_client_tls_init(host, port);
    if (ret != SUCCESS)
    {
        PRINTF("FAILED to init TLS (ret=%d)\n", ret);
        goto cleanup;
    }

    ret = https_client_ota_download(host, path, storage.start, storage.size, &image_size);
    if (ret != SUCCESS)
    {
        PRINTF("FAILED to download OTA image (ret=%d)\n", ret);
        goto cleanup;
    }

    if (!bl_verify_image(storage.start, image_size))
    {
        PRINTF("FAILED to verify mcuboot image format\n");
        goto cleanup;
    }

    PRINTF("OTA image was downloaded successfully.\n");

cleanup:
    https_client_tls_release();

    return kStatus_SHELL_Success;
}

static shell_status_t shellCmd_image(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    int image = 0;
    int ret;
    status_t status;
    uint32_t imgstate;

    if (argc > 3)
    {
        PRINTF("Too many arguments.\n");
        return kStatus_SHELL_Error;
    }

    /* image [info] */

    if (argc == 1 || (argc == 2 && !strcmp(argv[1], "info")))
    {
        bl_print_image_info(flash_sha256);
        return kStatus_SHELL_Success;
    }

    if (argc < 2)
    {
        PRINTF("Wrong arguments. See 'help'\n");
        return kStatus_SHELL_Error;
    }

    if (argc == 3)
    {
        char *parse_end;
        image = strtol(argv[2], &parse_end, 10);

        if (image < 0 || image >= MCUBOOT_IMAGE_NUMBER || *parse_end != '\0')
        {
            PRINTF("Wrong image number.\n");
            return kStatus_SHELL_Error;
        }
    }

    status = bl_get_image_state(image, &imgstate);
    if (status != kStatus_Success)
    {
        PRINTF("Failed to get state of image %u (status %d)", image, status);
        return kStatus_SHELL_Error;
    }

    /* image test [imgNum] */

    if (!strcmp(argv[1], "test"))
    {
        status = bl_update_image_state(image, kSwapType_ReadyForTest);
        if (status != kStatus_Success)
        {
            PRINTF("FAILED to mark image state as ReadyForTest (status=%d)\n", status);
            return kStatus_SHELL_Error;
        }
    }

    /* image accept [imgNum] */

    else if (!strcmp(argv[1], "accept"))
    {
        if (imgstate != kSwapType_Testing)
        {
            PRINTF("Image state is not set as Testing. Nothing to accept.\n", status);
            return kStatus_SHELL_Error;
        }

        status = bl_update_image_state(image, kSwapType_Permanent);
        if (status != kStatus_Success)
        {
            PRINTF("FAILED to accept image (status=%d)\n", status);
            return kStatus_SHELL_Error;
        }
    }

    /* image erase [imgNum] */

    else if (!strcmp(argv[1], "erase"))
    {
        partition_t ptn;

        ret = bl_get_update_partition_info(image, &ptn);
        if (ret != kStatus_Success)
        {
            PRINTF("Failed to determine update partition\n");
            return kStatus_SHELL_Error;
        }

        uint32_t slotaddr     = ptn.start;
        uint32_t slotsize     = ptn.size;
        uint32_t slotcnt      = (slotsize-1 + MFLASH_SECTOR_SIZE) / MFLASH_SECTOR_SIZE;

        PRINTF("Erasing inactive slot...");
        for (int i=0; i < slotcnt; i++)
        {
            ret = mflash_drv_sector_erase(slotaddr);
            if (ret)
            {
                PRINTF("\nFailed to erase sector at 0x%x (ret=%d)\n", slotaddr, ret);
                return kStatus_SHELL_Error;
            }
            slotaddr += MFLASH_SECTOR_SIZE;
        }
        PRINTF("done\n");
    }

    else
    {
        PRINTF("Wrong arguments. See 'help'\n");
        return kStatus_SHELL_Error;
    }

    return kStatus_SHELL_Success;
}

static shell_status_t shellCmd_reboot(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    PRINTF("System reset!\n");
    NVIC_SystemReset();

    /* return kStatus_SHELL_Success; */
}

static void ota_task(void *arg)
{
    int ret;
    s_shellHandle = &s_shellHandleBuffer[0];

    const char *phy =
#ifdef WIFI_MODE
        "WiFi";
#else
        "Ethernet";
#endif

    PRINTF("\nOTA HTTPS client demo (%s)\n\n", phy);

    /* network init */

    ret = initNetwork();
    if (ret)
    {
        PRINTF("FAILED to init network (ret=%d). Reboot the board and try again.\n", ret);
        goto failed_init;
    }

    /* shell init */

    ret = SHELL_Init(s_shellHandle, g_serialHandle, "$ ");
    if (ret != kStatus_SHELL_Success)
    {
        PRINTF("Failed to init shell.\n");
        goto failed_init;
    }

    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(ota));
    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(image));
    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(reboot));
#ifdef WIFI_MODE
    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(wifi));
#endif

    while (1)
    {
#if !(defined(SHELL_NON_BLOCKING_MODE) && (SHELL_NON_BLOCKING_MODE > 0U))
        SHELL_Task(s_shellHandle);
#endif
    }

failed_init:
    vTaskDelete(NULL);
}

/*!
 * @brief Main function.
 */
int main(void)
{
    BOARD_InitHardware();
    CRYPTO_InitHardware();

    mflash_drv_init();

    /* start the shell */

    if (xTaskCreate(ota_task, "ota_task", 2048 /* x4 */, NULL, SHELL_TASK_PRIORITY, NULL) != pdPASS)
    {
        PRINTF("Task creation failed!\r\n");
        while (1)
            ;
    }

    /* Run RTOS */
    vTaskStartScheduler();

    /* Should not reach this statement */
    for (;;)
        ;
}
