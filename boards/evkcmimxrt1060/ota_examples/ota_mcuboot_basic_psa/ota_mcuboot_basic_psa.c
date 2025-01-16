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
#include "mcuboot_app_support.h"
#include "update.h"

#include <ctype.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BUF_SZ  8192

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

static shell_status_t shellCmd_query(shell_handle_t shellHandle, int32_t argc,
        char **argv);
static shell_status_t shellCmd_update(shell_handle_t shellHandle, int32_t argc,
        char **argv);
static shell_status_t shellCmd_install(shell_handle_t shellHandle, int32_t argc,
        char **argv);
static shell_status_t shellCmd_accept(shell_handle_t shellHandle, int32_t argc,
        char **argv);
static shell_status_t shellCmd_cancel(shell_handle_t shellHandle, int32_t argc,
        char **argv);
static shell_status_t shellCmd_clean(shell_handle_t shellHandle, int32_t argc,
        char **argv);
static shell_status_t shellCmd_reject(shell_handle_t shellHandle, int32_t argc,
        char **argv);
static shell_status_t shellCmd_reboot(shell_handle_t shellHandle, int32_t argc,
        char **argv);

static int flash_sha256(uint32_t offset, size_t size, uint8_t sha256[32]);

/*******************************************************************************
 * Variables
 ******************************************************************************/

static SHELL_COMMAND_DEFINE(query, "\n\"query [compNum]\": "
        "Query and print information of component\r\n",
        shellCmd_query, SHELL_IGNORE_PARAMETER_COUNT);
static SHELL_COMMAND_DEFINE(update, "\n\"update [compNum]\": "
        "Start an update process of component using XMODEM\r\n",
        shellCmd_update, SHELL_IGNORE_PARAMETER_COUNT);
static SHELL_COMMAND_DEFINE(install, "\n\"install\": "
        "Mark all downloaded images as STAGED (ready to install)\r\n",
        shellCmd_install, SHELL_IGNORE_PARAMETER_COUNT);
static SHELL_COMMAND_DEFINE(accept, "\n\"accept [compNum]\": "
        "Mark an active image in TRIAL state as UPDATED\r\n",
        shellCmd_accept, SHELL_IGNORE_PARAMETER_COUNT);
static SHELL_COMMAND_DEFINE(cancel, "\n\"cancel [compNum]\": "
        "Abandon an update of component in WRITING or CANDIDATE state\r\n",
        shellCmd_cancel, SHELL_IGNORE_PARAMETER_COUNT);
static SHELL_COMMAND_DEFINE(reject, "\n\"reject\": "
        "Abandon an installation that is in STAGED or TRIAL state\r\n",
        shellCmd_reject, SHELL_IGNORE_PARAMETER_COUNT);
static SHELL_COMMAND_DEFINE(clean, "\n\"clean [compNum]\": "
        "Prepare the component for another update\r\n",
        shellCmd_clean, SHELL_IGNORE_PARAMETER_COUNT);
static SHELL_COMMAND_DEFINE(reboot, "\n\"reboot\": "
        "Reboot the platform\r\n",
        shellCmd_reboot, SHELL_IGNORE_PARAMETER_COUNT);

SDK_ALIGN(static uint8_t s_shellHandleBuffer[SHELL_HANDLE_SIZE], 4);
static shell_handle_t s_shellHandle;

/*
 * Buffer used to handover data from XMODEM to flash programming routine.
 * Uses 4B alignment to be compatible with mflash.
 **/
static uint32_t progbuf[1024 / sizeof(uint32_t)];

static hashctx_t sha256_xmodem_ctx;
static psa_fwu_component_t component_num;

/*******************************************************************************
 * Code
 ******************************************************************************/
static const char* state_to_str(uint8_t state) {
    switch (state) {
    case PSA_FWU_READY:
        return "READY";
    case PSA_FWU_WRITING:
        return "WRITING";
    case PSA_FWU_CANDIDATE:
        return "CANDIDATE";
    case PSA_FWU_STAGED:
        return "STAGED";
    case PSA_FWU_FAILED:
        return "FAILED";
    case PSA_FWU_TRIAL:
        return "TRIAL";
    case PSA_FWU_REJECTED:
        return "REJECTED";
    case PSA_FWU_UPDATED:
        return "UPDATED";
    default:
        return "UNEXPECTED";
    }
}

static void print_hash(const void *src, size_t size) {
    const unsigned char *src8 = src;
    for (size_t i = 0; i < size; i++) {
        PRINTF("%02X", src8[i]);
    }
}

static int process_received_data(uint32_t dst_addr, uint32_t offset,
        uint32_t size) {
    psa_status_t ret;
    uint32_t *data = progbuf;
    uint32_t addr = offset;

    /* 1kB programming buffer should be ok with all page size alignments */

    while (size) {
        size_t chunk = (size < MFLASH_PAGE_SIZE) ? size : MFLASH_PAGE_SIZE;

        /* mlfash takes entire page, in case of last data of smaller size it will
         program more data, which shouln't be a problem as the space allocated
         for the image slot is page aligned */

        ret = psa_fwu_write(component_num, addr, data, chunk);
        if (ret != PSA_SUCCESS) {
            /* With some terminals it takes a while before they recover receiving to the console */
            SDK_DelayAtLeastUs(100000, SystemCoreClock);
            PRINTF("Failed to program flash at %x (PSA error %d)\r\n", addr, ret);
            return -1;
        }

        sha256_update(&sha256_xmodem_ctx, data, chunk);
        addr += chunk;
        data += chunk / sizeof(uint32_t);
        size -= chunk;
    }

    return 0;
}

static shell_status_t shellCmd_query(shell_handle_t shellHandle, int32_t argc,
        char **argv) {
    int image = 0;
    psa_fwu_component_info_t info;
    psa_status_t psa_status;

    if (argc > 2) {
        PRINTF("Too many arguments. See help...\r\n");
        return kStatus_SHELL_Error;
    }

    if (argc == 2) {
        char *parse_end;
        image = strtol(argv[1], &parse_end, 10);

        if (image < 0 || image >= MCUBOOT_IMAGE_NUMBER || *parse_end != '\0') {
            PRINTF("Wrong component number.\r\n");
            return kStatus_SHELL_Error;
        }
    }

    psa_fwu_component_t component = image;
    psa_status = psa_fwu_query(component, &info);
    if (psa_status == PSA_SUCCESS) {
        PRINTF("Component id:%d state: %s\r\n", component,
                state_to_str(info.state));
        PRINTF("Version %d.%d.%d.%d\r\n", info.version.major, info.version.minor,
                info.version.patch, info.version.build);
    } else {
        PRINTF("Failed to query the component, status %d\r\n", psa_status);
    }

    bl_print_image_info(NULL);

    return kStatus_SHELL_Success;
}

static shell_status_t shellCmd_update(shell_handle_t shellHandle, int32_t argc,
        char **argv) {
    int image = 0;
    psa_fwu_component_info_t info;
    psa_status_t psa_status;
    long recvsize;
    uint8_t sha256_recv[32], sha256_flash[32];

    if (argc > 2) {
        PRINTF("Too many arguments. See help...\r\n");
        return kStatus_SHELL_Error;
    }

    if (argc == 2) {
        char *parse_end;
        image = strtol(argv[1], &parse_end, 10);

        if (image < 0 || image >= MCUBOOT_IMAGE_NUMBER || *parse_end != '\0') {
            PRINTF("Wrong component number.\r\n");
            return kStatus_SHELL_Error;
        }
    }

    psa_fwu_component_t component = image;
    component_num = image;

    psa_status = psa_fwu_start(component, NULL, 0);
    if (psa_status != PSA_SUCCESS) {
        SDK_DelayAtLeastUs(100000, SystemCoreClock);
        PRINTF("Start of update process failed, PSA error %d )\r\n", psa_status);
        return kStatus_SHELL_Success;
    }

    psa_fwu_query(component, &info);

    uint32_t start_addr = info.impl.update_phy_start;
    size_t max_size = info.impl.update_phy_size;
    struct xmodem_cfg cfg = { .putc = xmodem_putc, .getc = xmodem_getc,
            .canread = xmodem_canread,
            .canread_retries = xmodem_canread_retries, .dst_addr = start_addr,
            .maxsize = max_size, .buffer = (uint8_t*) progbuf, .buffer_size =
                    sizeof(progbuf), .buffer_full_callback =
                    process_received_data };

    PRINTF("Started xmodem download into flash at 0x%X\r\n", start_addr);

    sha256_init(&sha256_xmodem_ctx);

    PRINTF(
            "Initiated XMODEM-CRC transfer. Receiving... (Press 'x' to cancel)\r\n");

    recvsize = xmodem_receive(&cfg);

    /* With some terminals it takes a while before they recover receiving to the console */
    SDK_DelayAtLeastUs(100000, SystemCoreClock);

    if (recvsize < 0) {
        PRINTF("\nTransfer failed (%d)\r\n", recvsize);
        psa_fwu_cancel(component);
        return kStatus_SHELL_Success;
    }

    PRINTF("\nReceived %u bytes\r\n", recvsize);

    sha256_finish(&sha256_xmodem_ctx, sha256_recv);
    flash_sha256(start_addr, recvsize, sha256_flash);

    PRINTF("SHA256 of received data: ");
    print_hash(sha256_recv, 10);
    PRINTF("...\r\n");

    PRINTF("SHA256 of flashed data:  ");
    print_hash(sha256_flash, 10);
    PRINTF("...\r\n");

    psa_status = psa_fwu_finish(component);
    if (psa_status != PSA_SUCCESS) {
        PRINTF("Unable to finish update process (PSA error %d)\r\n", psa_status);
        psa_fwu_cancel(component);
    }

    return kStatus_SHELL_Success;
}

static shell_status_t shellCmd_install(shell_handle_t shellHandle, int32_t argc,
        char **argv) {
    psa_status_t psa_status;

    if (argc > 1) {
        PRINTF("Too many arguments. See help...\r\n");
        return kStatus_SHELL_Error;
    }

    psa_status = psa_fwu_install();
    if (psa_status == PSA_SUCCESS_REBOOT) {
        PRINTF("Image succesfully marked as STAGED, please reboot...\r\n");
    } else {
        PRINTF(
                "Unable to mark image as STAGED (is in CANDIDATE state), PSA error %d\r\n",
                psa_status);
    }

    return kStatus_SHELL_Success;
}

static shell_status_t shellCmd_cancel(shell_handle_t shellHandle, int32_t argc,
        char **argv) {
    int image = 0;
    psa_fwu_component_t component;
    psa_status_t psa_status;

    if (argc > 2) {
        PRINTF("Too many arguments. See help...\r\n");
        return kStatus_SHELL_Error;
    }

    if (argc == 2) {
        char *parse_end;
        image = strtol(argv[1], &parse_end, 10);

        if (image < 0 || image >= MCUBOOT_IMAGE_NUMBER || *parse_end != '\0') {
            PRINTF("Wrong component number.\r\n");
            return kStatus_SHELL_Error;
        }
    }
    component = image;

    psa_status = psa_fwu_cancel(component);
    if (psa_status != PSA_SUCCESS) {
        PRINTF("Unable to cancel update for component, PSA error %d\r\n",
                psa_status);
    }

    return kStatus_SHELL_Success;
}

static shell_status_t shellCmd_clean(shell_handle_t shellHandle, int32_t argc,
        char **argv) {
    int image = 0;
    psa_fwu_component_t component;
    psa_status_t psa_status;

    if (argc > 2) {
        PRINTF("Too many arguments. See help...\r\n");
        return kStatus_SHELL_Error;
    }

    if (argc == 2) {
        char *parse_end;
        image = strtol(argv[1], &parse_end, 10);

        if (image < 0 || image >= MCUBOOT_IMAGE_NUMBER || *parse_end != '\0') {
            PRINTF("Wrong component number.\r\n");
            return kStatus_SHELL_Error;
        }
    }
    component = image;

    psa_status = psa_fwu_clean(component);
    if (psa_status != PSA_SUCCESS) {
        PRINTF("Unable to clean the component, PSA error %d\r\n", psa_status);
    }

    return kStatus_SHELL_Success;
}

static shell_status_t shellCmd_reject(shell_handle_t shellHandle, int32_t argc,
        char **argv) {
    psa_status_t psa_status;

    if (argc > 1) {
        PRINTF("Too many arguments. See help...\r\n");
        return kStatus_SHELL_Error;
    }

    //error code depends on usage...
    psa_status = psa_fwu_reject(0);
    if (psa_status == PSA_SUCCESS_REBOOT)
        PRINTF("Tested image rejected. Please reboot...\r\n");
    else if (psa_status == PSA_SUCCESS)
        PRINTF("Staged image rejected, please clean staging area...\r\n");
    else
        PRINTF("No image to reject or other failure, PSA error %d\r\n",
                psa_status);

    return kStatus_SHELL_Success;
}

static shell_status_t shellCmd_accept(shell_handle_t shellHandle, int32_t argc,
        char **argv) {
    psa_status_t psa_status;

    if (argc > 1) {
        PRINTF("Too many arguments. See help...\r\n");
        return kStatus_SHELL_Error;
    }

    psa_status = psa_fwu_accept();
    if (psa_status != PSA_SUCCESS) {
        PRINTF("No image to accept or other failure, PSA error %d\r\n",
                psa_status);
    }

    return kStatus_SHELL_Success;
}

static shell_status_t shellCmd_reboot(shell_handle_t shellHandle, int32_t argc,
        char **argv) {
    psa_fwu_request_reboot();
    return kStatus_SHELL_Success;
}

static int flash_sha256(uint32_t offset, size_t size, uint8_t sha256[32]) {
    uint32_t buf[128 / sizeof(uint32_t)];
    status_t status;
    hashctx_t sha256ctx;

    sha256_init(&sha256ctx);

    while (size > 0) {
        size_t chunk = (size > sizeof(buf)) ? sizeof(buf) : size;
        /* mflash demands size to be in multiples of 4 */
        size_t chunkAlign4 = (chunk + 3) & (~3);

        status = mflash_drv_read(offset, buf, chunkAlign4);
        if (status != kStatus_Success) {
            return status;
        }

        sha256_update(&sha256ctx, (unsigned char*) buf, chunk);

        size -= chunk;
        offset += chunk;
    }

    sha256_finish(&sha256ctx, sha256);

    return kStatus_Success;
}

/*
 * PSA API specifies READY state to be ready for an update. Currently, there are
 * two approaches how to ensures erased flash:
 * 1. Erase whole image slot before flash write operations
 * 2. Erase individual flash sectors before corresponding flash page writes
 * 
 * There are possible use-cases where only first approach can be used, for example
 * when encrypted XIP feature is used, therefore we need to ensure that staging 
 * area is truly erased.
 */
static void clean_staging_partition(int image) {
    uint32_t state;

    bl_get_image_state(0, &state);
    if (state == kSwapType_None || state == kSwapType_Permanent) {
        /* We have to use buffers as flash remap feature limits logical read */
        static uint32_t buf_erased[BUF_SZ / sizeof(uint32_t)];
        static uint32_t buf_flash[BUF_SZ / sizeof(uint32_t)];
        int32_t slot_size = BOOT_FLASH_CAND_APP - BOOT_FLASH_ACT_APP;
        partition_t prt;
        uint32_t addr, len;
        bool is_erased = true;

        if (bl_get_update_partition_info(image, &prt) != kStatus_Success)
            return;
        addr = prt.start;
        memset(buf_erased, 0xFF, BUF_SZ);

        len = slot_size;
        while (len > 0) {
            if (bl_flash_read(addr, buf_flash, BUF_SZ) != kStatus_Success) {
                PRINTF("Flash read failed\r\n");
                return;
            }
            if (memcmp(buf_flash, buf_erased, BUF_SZ) != 0) {
                is_erased = false;
                break;
            }
            len -= BUF_SZ;
            addr += BUF_SZ;
        }

        if (is_erased == false) {
            PRINTF("Staging area is not erased. Erasing...\r\n");
            uint32_t slotaddr = prt.start;
            uint32_t slotsize = prt.size;
            uint32_t slotcnt = (slotsize - 1 + MFLASH_SECTOR_SIZE)
                    / MFLASH_SECTOR_SIZE;

            for (int i = 0; i < slotcnt; i++) {
                if (mflash_drv_sector_erase(slotaddr) != kStatus_Success) {
                    PRINTF("Failed to erase sector at 0x%X\r\n", slotaddr);
                    return;
                }
                if (i % 10 == 0)
                    PUTCHAR('.');
                slotaddr += MFLASH_SECTOR_SIZE;
            }
            PRINTF("\nDone\r\n");
        }
    }
}

/*!
 * @brief Main function
 */
int main(void) {
    int ret;
    s_shellHandle = &s_shellHandleBuffer[0];

    /* Init board hardware. */
    BOARD_InitHardware();

    ret = mflash_drv_init();
    if (ret) {
        PRINTF("Failed to init flash driver\r\n");
    }

    PRINTF("\r\n"
            "*******************************************************\r\n"
            "* Basic MCUBoot application example using PSA FWU API *\r\n"
            "*******************************************************\n\r\n");

    PRINTF("Built " __DATE__ " " __TIME__ "\r\n");

    /* Initialiaze PSA FWU API and eventually clean staging area */
    int image;
    for (image = 0; image < MCUBOOT_IMAGE_NUMBER; image++) {
        psa_fwu_component_info_t dummy;

        clean_staging_partition(image);
        psa_fwu_query(image, &dummy);
    }

    ret = SHELL_Init(s_shellHandle, g_serialHandle, "$ ");
    if (ret != kStatus_SHELL_Success) {
        PRINTF("Failed to init shell\r\n");
        goto failed_init;
    }

    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(query));
    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(update));
    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(install));
    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(cancel));
    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(clean));
    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(reject));
    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(accept));
    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(reboot));

    while (1) {
        SHELL_Task(s_shellHandle);
    }

    failed_init: while (1) {
    }
}
