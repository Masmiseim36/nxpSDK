/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "fsl_shell.h"
#include "board.h"
#include "clock_config.h"
#include "app.h"

#include "mcuboot_app_support.h"
#include "mflash_drv.h"
#include "xmodem.h"
#include "platform_bindings.h"

#include <ctype.h>

#ifdef MCUBOOT_OTA_SB3_SUPPORT
#include "sb3_api.h"
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

static shell_status_t shellCmd_image(shell_handle_t shellHandle, int32_t argc, char **argv);
static shell_status_t shellCmd_xmodem(shell_handle_t shellHandle, int32_t argc, char **argv);
#ifdef MCUBOOT_OTA_SB3_SUPPORT
static shell_status_t shellCmd_xmodem_sb3(shell_handle_t shellHandle, int32_t argc, char **argv);
#endif
static shell_status_t shellCmd_mem(shell_handle_t shellHandle, int32_t argc, char **argv);
static shell_status_t shellCmd_reboot(shell_handle_t shellHandle, int32_t argc, char **argv);

static int flash_sha256(uint32_t offset, size_t size, uint8_t sha256[32]);


/*******************************************************************************
 * Variables
 ******************************************************************************/


static SHELL_COMMAND_DEFINE(image,
                            "\n\"image [info]\"          : Print image information"
                            "\n\"image test [imgNum]\"   : Mark candidate slot of given image number as ready for test"
                            "\n\"image accept [imgNum]\" : Mark active slot of given image number as accepted"
                            "\n\"image erase [imgNum]\"  : Erase candidate slot of given image number"
                            "\n",
                            shellCmd_image,
                            SHELL_IGNORE_PARAMETER_COUNT);

static SHELL_COMMAND_DEFINE(mem,
                            "\n\"mem read addr [size]\" : Read memory at given address"
                            "\n\"mem erase addr \"      : Erase sector containing given address"
                            "\n",
                            shellCmd_mem,
                            SHELL_IGNORE_PARAMETER_COUNT);

static SHELL_COMMAND_DEFINE(xmodem, "\n\"xmodem [imgNum]\": Start receiving with XMODEM-CRC\n", shellCmd_xmodem, SHELL_IGNORE_PARAMETER_COUNT);
#ifdef MCUBOOT_OTA_SB3_SUPPORT
static SHELL_COMMAND_DEFINE(xmodem_sb3, "\n\"xmodem_sb3\": Start SB3 receiving with XMODEM-CRC\n", shellCmd_xmodem_sb3, SHELL_IGNORE_PARAMETER_COUNT);
#endif
static SHELL_COMMAND_DEFINE(reboot, "\n\"reboot\": Triggers software reset\n", shellCmd_reboot, 0);

SDK_ALIGN(static uint8_t s_shellHandleBuffer[SHELL_HANDLE_SIZE], 4);
static shell_handle_t s_shellHandle;

/*
 * Buffer used to handover data from XMODEM to flash programming routine.
 * Uses 4B alignment to be compatible with mflash.
 **/
static uint32_t progbuf[1024/sizeof(uint32_t)];

static hashctx_t sha256_xmodem_ctx;

/*******************************************************************************
 * Code
 ******************************************************************************/

static void hexdump(const void *src, size_t size)
{
    const unsigned char *src8 = src;
    const int CNT             = 16;

    for (size_t i = 0; i < size; i++)
    {
        int n = i % CNT;
        if (n == 0)
            PRINTF("%08x  ", (uint32_t)src+i);
        PRINTF("%02X ", src8[i]);
        if ((i && n == CNT - 1) || (i + 1 == size))
        {
            int rem = CNT - 1 - n;
            for (int j = 0; j < rem; j++)
                PRINTF("   ");
            PRINTF("|");
            for (int j = n; j >= 0; j--)
                PUTCHAR(isprint(src8[i - j]) ? src8[i - j] : '.');
            PRINTF("|\n");
        }
    }
    PUTCHAR('\n');
}

static void print_hash(const void *src, size_t size)
{
    const unsigned char *src8 = src;
    for (size_t i = 0; i < size; i++)
    {
        PRINTF("%02X", src8[i]);
    }
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

static shell_status_t shellCmd_mem(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    int ret;
    uint32_t addr;
    uint32_t size = 128;
    char *parse_end;

    if (argc < 3 || argc > 4)
    {
        PRINTF("Wrong argument count\n");
        return kStatus_SHELL_Error;
    }

    addr = strtol(argv[2], &parse_end, 0);
    if (*parse_end != '\0')
    {
        PRINTF("Bad address\n");
        return kStatus_SHELL_Error;
    }

    if (argc == 4)
    {
        size = strtol(argv[3], &parse_end, 0);
        if (*parse_end != '\0')
        {
            PRINTF("Bad size\n");
            return kStatus_SHELL_Error;
        }
    }

    /* mem read addr [size] */

    if (!strcmp(argv[1], "read"))
    {
#ifdef MFLASH_PAGE_INTEGRITY_CHECKS
        if (mflash_drv_is_readable(addr) != kStatus_Success)
        {
            PRINTF("Page not readable\n");
            return kStatus_SHELL_Error;
        }
#endif
        hexdump((void *)addr, size);
    }

    /* mem erase addr */

    else if (!strcmp(argv[1], "erase"))
    {
        ret = mflash_drv_sector_erase(addr & ~(MFLASH_SECTOR_SIZE-1));
        if (ret)
        {
            PRINTF("Failed to erase sector (ret=%d)\n", ret);
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


static int process_received_data(uint32_t dst_addr, uint32_t offset, uint32_t size)
{
    int ret;
    uint32_t *data = progbuf;
    uint32_t addr = dst_addr + offset;

    /* 1kB programming buffer should be ok with all page size alignments */
    while (size)
    {
        size_t chunk = (size < MFLASH_PAGE_SIZE) ? size : MFLASH_PAGE_SIZE;

        /* mlfash takes entire page, in case of last data of smaller size it will
           program more data, which shouln't be a problem as the space allocated
           for the image slot is page aligned */

        ret = mflash_drv_page_program(addr, data);
        if (ret)
        {
            PRINTF("Failed to program flash at %x (ret %d)\n", addr, ret);
            return -1;
        }

        sha256_update(&sha256_xmodem_ctx, data, chunk);
        addr += chunk;
        data += chunk/sizeof(uint32_t);
        size -= chunk;
    }

    return 0;
}

#ifdef MCUBOOT_OTA_SB3_SUPPORT
static int process_received_data_sb3(uint32_t dst_addr, uint32_t offset, uint32_t size)
{
    int ret;
    uint32_t *data = progbuf;
    uint32_t chunk_sz;

    static uint32_t sb_size;
    static uint32_t bytes_processed;

    if(offset == 0)
    {
        /* first chunk */
        if(!sb3_parse_header(data, &sb_size))
        {
            return -1;
        }
        bytes_processed = 0;
    }

    if(sb_size == bytes_processed)
    {
        /* just in case */
        return -1;
    }

    if (sb_size - bytes_processed > size) {
        chunk_sz = size;
    } else {
        /* last chunk */
        chunk_sz = sb_size - bytes_processed;
    }

    /* Processing SB3 image */
    ret = sb3_api_pump((uint8_t *)data, chunk_sz);
    if (ret != kStatus_Success)
    {
        PRINTF("sb3_api_pump failed/n");
        return -1;
    }

    bytes_processed += chunk_sz;
    return 0;
}
#endif

static shell_status_t shellCmd_xmodem(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    int image = 0;
    long recvsize;
    uint8_t sha256_recv[32], sha256_flash[32];
    partition_t prt_ota;

    if (argc > 3)
    {
        PRINTF("Too many arguments.\n");
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

    if (bl_get_update_partition_info(image, &prt_ota) != kStatus_Success)
    {
        PRINTF("FAILED to determine address for download\n");
        return kStatus_SHELL_Error;
    }

    PRINTF("Started xmodem download into flash at 0x%X\n", prt_ota.start);

    struct xmodem_cfg cfg = {
        .putc = xmodem_putc,
        .getc = xmodem_getc,
        .canread = xmodem_canread,
        .canread_retries = xmodem_canread_retries,
        .dst_addr = prt_ota.start,
        .maxsize = prt_ota.size,
        .buffer = (uint8_t*)progbuf,
        .buffer_size = sizeof(progbuf),
        .buffer_full_callback = process_received_data
    };

    sha256_init(&sha256_xmodem_ctx);

    PRINTF("Initiated XMODEM-CRC transfer. Receiving... (Press 'x' to cancel)\n");

    recvsize = xmodem_receive(&cfg);

    /* With some terminals it takes a while before they recover receiving to the console */
    SDK_DelayAtLeastUs(1000000, SystemCoreClock);

    if (recvsize < 0)
    {
        PRINTF("\nTransfer failed (%d)\n", recvsize);
        return kStatus_SHELL_Error;
    }
    PRINTF("\nReceived %u bytes\n", recvsize);

    sha256_finish(&sha256_xmodem_ctx, sha256_recv);
    flash_sha256(prt_ota.start, recvsize, sha256_flash);

    PRINTF("SHA256 of received data: ");
    print_hash(sha256_recv, 10);
    PRINTF("...\n");

    PRINTF("SHA256 of flashed data:  ");
    print_hash(sha256_flash, 10);
    PRINTF("...\n");

    return kStatus_SHELL_Success;
}

#ifdef MCUBOOT_OTA_SB3_SUPPORT
static shell_status_t shellCmd_xmodem_sb3(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    long recvsize;
    partition_t prt_ota;

    if (argc > 2)
    {
        PRINTF("Too many arguments.\n");
        return kStatus_SHELL_Error;
    }

    if (bl_get_update_partition_info(0, &prt_ota) != kStatus_Success)
    {
        PRINTF("FAILED to determine address for download\n");
        return kStatus_SHELL_Error;
    }

    //Todo add provisioning check
    if (sb3_api_init() != kStatus_Success)
    {
        PRINTF("sb3_iap_init failed/n");
        return kStatus_SHELL_Error;
    }

    if(sb3_check_provisioning(false) == 0)
    {
        PRINTF("WARNING! Device doesn't seem to be configured properly! Check instructions in readme file.\n");
    }

    PRINTF("Started xmodem processing SB3\n");
    PRINTF("Make sure this device is provisioned to accept secure binary and its load address is 0x%X\n", prt_ota.start);

    struct xmodem_cfg cfg = {
        .putc = xmodem_putc,
        .getc = xmodem_getc,
        .canread = xmodem_canread,
        .canread_retries = xmodem_canread_retries,
        .dst_addr = prt_ota.start,
        .maxsize = prt_ota.size,
        .buffer = (uint8_t*)progbuf,
        .buffer_size = sizeof(progbuf),
        .buffer_full_callback = process_received_data_sb3
    };

    sha256_init(&sha256_xmodem_ctx);

    PRINTF("Initiated XMODEM-CRC transfer. Receiving... (Press 'x' to cancel)\n");

    recvsize = xmodem_receive(&cfg);

    /* With some terminals it takes a while before they recover receiving to the console */
    SDK_DelayAtLeastUs(1000000, SystemCoreClock);

    if (recvsize < 0)
    {
        PRINTF("\nTransfer failed (%d)\n", recvsize);
        return kStatus_SHELL_Error;
    }
    PRINTF("\nReceived %u bytes\n", recvsize);

    PRINTF("SB3 has been processed\n");
    sb3_api_finalize();
    sb3_api_deinit();

    return kStatus_SHELL_Success;
}
#endif

static shell_status_t shellCmd_reboot(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    PRINTF("System reset!\n");
    NVIC_SystemReset();

    /* return kStatus_SHELL_Success; */
}

static int flash_sha256(uint32_t offset, size_t size, uint8_t sha256[32])
{
    uint32_t buf[128 / sizeof(uint32_t)];
    status_t status;
    hashctx_t sha256ctx;

    sha256_init(&sha256ctx);

    while (size > 0)
    {
        size_t chunk = (size > sizeof(buf)) ? sizeof(buf) : size;
        /* mflash demands size to be in multiples of 4 */
        size_t chunkAlign4 = (chunk + 3) & (~3);

        status = mflash_drv_read(offset, buf, chunkAlign4);
        if (status != kStatus_Success)
        {
            return status;
        }

        sha256_update(&sha256ctx, (unsigned char *)buf, chunk);

        size -= chunk;
        offset += chunk;
    }

    sha256_finish(&sha256ctx, sha256);

    return kStatus_Success;
}

/*!
 * @brief Main function
 */
int main(void)
{
    int ret;
    s_shellHandle = &s_shellHandleBuffer[0];

    /* Init board hardware. */
    BOARD_InitHardware();

    ret = mflash_drv_init();
    if (ret)
    {
        PRINTF("Failed to init flash driver\n");
    }

    PRINTF("\n"
           "*************************************\n"
           "* Basic MCUBoot application example *\n"
           "*************************************\n\n");

    PRINTF("Built " __DATE__ " " __TIME__ "\n");

    ret = SHELL_Init(s_shellHandle, g_serialHandle, "$ ");
    if (ret != kStatus_SHELL_Success)
    {
        PRINTF("Failed to init shell\n");
        goto failed_init;
    }

    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(image));
    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(xmodem));
#ifdef MCUBOOT_OTA_SB3_SUPPORT
    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(xmodem_sb3));
#endif
    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(mem));
    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(reboot));


    while (1)
    {
        SHELL_Task(s_shellHandle);
    }

failed_init:
    while (1)
    {
    }
}
