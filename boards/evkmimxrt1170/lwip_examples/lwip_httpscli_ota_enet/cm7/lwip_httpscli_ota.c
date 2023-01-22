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
#include "pin_mux.h"
#include "board.h"
#include "lwip/netifapi.h"
#include "lwip/opt.h"
#include "lwip/tcpip.h"
#include "lwip/dhcp.h"
#include "lwip/prot/dhcp.h"
#include "ksdk_mbedtls.h"
#include "mflash_drv.h"
#include "fsl_debug_console.h"
#include "ota_config.h"
#include "network_cfg.h"
#include "fsl_shell.h"
#include "sysflash/sysflash.h"
#include "flash_map.h"
#include "mcuboot_app_support.h"

#ifdef WIFI_MODE
#include "wpl.h"
#endif

#include "fsl_enet.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/

int initNetwork(void);

static shell_status_t shellCmd_ota(shell_handle_t shellHandle, int32_t argc, char **argv);
static shell_status_t shellCmd_netinit(shell_handle_t shellHandle, int32_t argc, char **argv);
static shell_status_t shellCmd_image(shell_handle_t shellHandle, int32_t argc, char **argv);
static shell_status_t shellCmd_reboot(shell_handle_t shellHandle, int32_t argc, char **argv);

#ifdef WIFI_MODE
static shell_status_t shellCmd_wifi(shell_handle_t shellHandle, int32_t argc, char **argv);
#endif

/*******************************************************************************
 * Variables
 ******************************************************************************/
#if BOARD_NETWORK_USE_100M_ENET_PORT
phy_ksz8081_resource_t g_phy_resource;
#else
phy_rtl8211f_resource_t g_phy_resource;
#endif

SHELL_COMMAND_DEFINE(ota,
                     "\n\"ota <imageNumber> <filePath> <host> <port>\": Starts download of OTA image\n",
                     shellCmd_ota,
                     SHELL_IGNORE_PARAMETER_COUNT);

SHELL_COMMAND_DEFINE(image,
                     "\n\"image [info]\"              : Print image information"
                     "\n\"image test <imageNumber>\"  : Mark secondary image of given number as ready for test"
                     "\n\"image accept <imageNumber>\": Mark primary image of given number as accepted"
                     "\n",
                     shellCmd_image,
                     SHELL_IGNORE_PARAMETER_COUNT);

SHELL_COMMAND_DEFINE(reboot, "\n\"reboot\": Triggers software reset\n", shellCmd_reboot, 0);

#ifdef WIFI_MODE
SHELL_COMMAND_DEFINE(wifi,
                     "\n\"wifi conf [ssid pass]\" : Get/Set WiFi SSID and passphrase"
                     "\n\"wifi join\"             : Connect to network"
                     "\n\"wifi leave\"            : Disconnect from network"
                     "\n",
                     shellCmd_wifi,
                     SHELL_IGNORE_PARAMETER_COUNT);

char wifi_ssid[32 + 1] = WIFI_SSID;
char wifi_pass[64 + 1] = WIFI_PASSWORD;
#endif

SDK_ALIGN(static uint8_t s_shellHandleBuffer[SHELL_HANDLE_SIZE], 4);
static shell_handle_t s_shellHandle;
extern serial_handle_t g_serialHandle;

/*******************************************************************************
 * Code
 ******************************************************************************/
void BOARD_InitModuleClock(void)
{
    const clock_sys_pll1_config_t sysPll1Config = {
        .pllDiv2En = true,
    };
    CLOCK_InitSysPll1(&sysPll1Config);

#if BOARD_NETWORK_USE_100M_ENET_PORT
    clock_root_config_t rootCfg = {.mux = 4, .div = 10}; /* Generate 50M root clock. */
    CLOCK_SetRootClock(kCLOCK_Root_Enet1, &rootCfg);
#else
    clock_root_config_t rootCfg = {.mux = 4, .div = 4}; /* Generate 125M root clock. */
    CLOCK_SetRootClock(kCLOCK_Root_Enet2, &rootCfg);
#endif
}

void IOMUXC_SelectENETClock(void)
{
#if BOARD_NETWORK_USE_100M_ENET_PORT
    IOMUXC_GPR->GPR4 |= IOMUXC_GPR_GPR4_ENET_REF_CLK_DIR_MASK; /* 50M ENET_REF_CLOCK output to PHY and ENET module. */
#else
    IOMUXC_GPR->GPR5 |= IOMUXC_GPR_GPR5_ENET1G_RGMII_EN_MASK; /* bit1:iomuxc_gpr_enet_clk_dir
                                                                 bit0:GPR_ENET_TX_CLK_SEL(internal or OSC) */
#endif
}

void BOARD_ENETFlexibleConfigure(enet_config_t *config)
{
#if BOARD_NETWORK_USE_100M_ENET_PORT
    config->miiMode = kENET_RmiiMode;
#else
    config->miiMode = kENET_RgmiiMode;
#endif
}

static void MDIO_Init(void)
{
    (void)CLOCK_EnableClock(s_enetClock[ENET_GetInstance(EXAMPLE_ENET)]);
    ENET_SetSMI(EXAMPLE_ENET, EXAMPLE_CLOCK_FREQ, false);
}

static status_t MDIO_Write(uint8_t phyAddr, uint8_t regAddr, uint16_t data)
{
    return ENET_MDIOWrite(EXAMPLE_ENET, phyAddr, regAddr, data);
}

static status_t MDIO_Read(uint8_t phyAddr, uint8_t regAddr, uint16_t *pData)
{
    return ENET_MDIORead(EXAMPLE_ENET, phyAddr, regAddr, pData);
}


/*
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
*/

static int is_mcuboot_header(const void *data)
{
    const struct image_header *ih = data;

    /* check magic number */
    if (ih->ih_magic == IMAGE_MAGIC)
    {
        return 1;
    }

    return 0;
}

static void print_image_info(void)
{
    for (int image = 0; image < MCUBOOT_IMAGE_NUMBER; image++)
    {
        status_t status;
        uint32_t imgstate;
        const char *name = boot_image_names[image];

        status = bl_get_image_state(image, &imgstate);
        if (status != kStatus_Success)
        {
            PRINTF("Failed to get state of image %u (ret %d)", image, status);
            return;
        }

        PRINTF("Image %d; name %s; state %s:\n", image, name, bl_imgstate_to_str(imgstate));

        for (int slot = 0; slot < 2; slot++)
        {
            int faid                = image * 2 + slot;
            struct flash_area *fa   = &boot_flash_map[faid];
            uint32_t slotaddr       = fa->fa_off + BOOT_FLASH_BASE;
            uint32_t slotsize       = fa->fa_size;
            struct image_header *ih = (void *)slotaddr;

            int img_present = ih->ih_magic == IMAGE_MAGIC;

            PRINTF("  Slot %d; slotAddr %x; slotSize %u\n", faid, slotaddr, slotsize);

            if (img_present)
            {
                struct image_version *iv = &ih->ih_ver;
                char versionstr[40];

                snprintf(versionstr, sizeof(versionstr), "%u.%u.%u.%lu", iv->iv_major, iv->iv_minor, iv->iv_revision,
                         iv->iv_build_num);

                PRINTF("    <IMAGE %s: size %u; version %s>\n", fa->fa_name, ih->ih_img_size, versionstr);
            }
            else
            {
                PRINTF("    <EMPTY>\n");
            }
        }
    }
}

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
        "    file = %s\n"
        "    host = %s\n"
        "    port = %s\n",
        image, path, host, port);

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
    int ret, image;
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
        print_image_info();
        return kStatus_SHELL_Success;
    }

    if (argc != 3)
    {
        PRINTF("Wrong arguments. See 'help'\n");
        return kStatus_SHELL_Error;
    }

    image = atoi(argv[2]);
    if (image < 0 || image >= MCUBOOT_IMAGE_NUMBER)
    {
        PRINTF("Image number out of range.\n");
        return kStatus_SHELL_Error;
    }

    status = bl_get_image_state(image, &imgstate);
    if (status != kStatus_Success)
    {
        PRINTF("Failed to get state of image %u (ret %d)", image, status);
        return kStatus_SHELL_Error;
    }

    /* image test <imageNumber> */

    if (!strcmp(argv[1], "test"))
    {
        status = bl_update_image_state(image, kSwapType_ReadyForTest);
        if (status != kStatus_Success)
        {
            PRINTF("FAILED to mark image state as ReadyForTest (ret=%d)\n", status);
            return kStatus_SHELL_Error;
        }
    }

    /* image accept <imageNumber> */

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
            PRINTF("FAILED to accept image (ret=%d)\n", status);
            return kStatus_SHELL_Error;
        }
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
    gpio_pin_config_t gpio_config = {kGPIO_DigitalOutput, 0, kGPIO_NoIntmode};

    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_InitModuleClock();

    SCB_DisableDCache();

    IOMUXC_SelectENETClock();

#if BOARD_NETWORK_USE_100M_ENET_PORT
    BOARD_InitEnetPins();
    GPIO_PinInit(GPIO12, 12, &gpio_config);
    GPIO_WritePinOutput(GPIO12, 12, 0);
    SDK_DelayAtLeastUs(10000, CLOCK_GetFreq(kCLOCK_CpuClk));
    GPIO_WritePinOutput(GPIO12, 12, 1);
    SDK_DelayAtLeastUs(6, CLOCK_GetFreq(kCLOCK_CpuClk));
#else
    BOARD_InitEnet1GPins();
    GPIO_PinInit(GPIO11, 14, &gpio_config);
    /* For a complete PHY reset of RTL8211FDI-CG, this pin must be asserted low for at least 10ms. And
     * wait for a further 30ms(for internal circuits settling time) before accessing the PHY register */
    GPIO_WritePinOutput(GPIO11, 14, 0);
    SDK_DelayAtLeastUs(10000, CLOCK_GetFreq(kCLOCK_CpuClk));
    GPIO_WritePinOutput(GPIO11, 14, 1);
    SDK_DelayAtLeastUs(30000, CLOCK_GetFreq(kCLOCK_CpuClk));

    EnableIRQ(ENET_1G_MAC0_Tx_Rx_1_IRQn);
    EnableIRQ(ENET_1G_MAC0_Tx_Rx_2_IRQn);
#endif

    MDIO_Init();
    g_phy_resource.read  = MDIO_Read;
    g_phy_resource.write = MDIO_Write;
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
