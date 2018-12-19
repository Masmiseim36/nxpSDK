/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "fsl_debug_console.h"
#include "serial_manager.h"
#include "fsl_shell.h"
#include "lfs_support.h"

#include "pin_mux.h"
#include "board.h"
#include "clock_config.h"
#include "fsl_flexspi.h"
/*******************************************************************************
* Definitions
******************************************************************************/


#define SHELL_Printf PRINTF

/*******************************************************************************
* Prototypes
******************************************************************************/

/* SHELL LFS command handlers */
static shell_status_t lfs_format_handler(shell_handle_t shellHandle, int32_t argc, char **argv);
static shell_status_t lfs_mount_handler(shell_handle_t shellHandle, int32_t argc, char **argv);
static shell_status_t lfs_unmount_handler(shell_handle_t shellHandle, int32_t argc, char **argv);
static shell_status_t lfs_cd_handler(shell_handle_t shellHandle, int32_t argc, char **argv);
static shell_status_t lfs_ls_handler(shell_handle_t shellHandle, int32_t argc, char **argv);
static shell_status_t lfs_rm_handler(shell_handle_t shellHandle, int32_t argc, char **argv);
static shell_status_t lfs_mkdir_handler(shell_handle_t shellHandle, int32_t argc, char **argv);
static shell_status_t lfs_write_handler(shell_handle_t shellHandle, int32_t argc, char **argv);
static shell_status_t lfs_cat_handler(shell_handle_t shellHandle, int32_t argc, char **argv);

/*******************************************************************************
 * Variables
 ******************************************************************************/

lfs_t lfs;
struct lfs_config cfg;
int lfs_mounted;

SHELL_COMMAND_DEFINE(format,
                     "\r\n\"format yes\": Formats the filesystem\r\n",
                     lfs_format_handler,
                     SHELL_IGNORE_PARAMETER_COUNT);
SHELL_COMMAND_DEFINE(mount, "\r\n\"mount\": Mounts the filesystem\r\n", lfs_mount_handler, 0);
SHELL_COMMAND_DEFINE(unmount, "\r\n\"unmount\": Unmounts the filesystem\r\n", lfs_unmount_handler, 0);
SHELL_COMMAND_DEFINE(umount, "", lfs_unmount_handler, 0); // unmount alias
SHELL_COMMAND_DEFINE(cd, "", lfs_cd_handler, SHELL_IGNORE_PARAMETER_COUNT);
SHELL_COMMAND_DEFINE(ls, "\r\n\"ls <path>\": Lists directory content\r\n", lfs_ls_handler, 1);
SHELL_COMMAND_DEFINE(rm, "\r\n\"rm <path>\": Removes file or directory\r\n", lfs_rm_handler, 1);
SHELL_COMMAND_DEFINE(mkdir, "\r\n\"mkdir <path>\": Creates a new directory\r\n", lfs_mkdir_handler, 1);
SHELL_COMMAND_DEFINE(write, "\r\n\"write <path> <text>\": Writes/appends text to a file\r\n", lfs_write_handler, 2);
SHELL_COMMAND_DEFINE(cat, "\r\n\"cat <path>\": Prints file content\r\n", lfs_cat_handler, 1);

SDK_ALIGN(static uint8_t s_shellHandleBuffer[SHELL_HANDLE_SIZE], 4);
static shell_handle_t s_shellHandle;

extern serial_handle_t g_serialHandle;

/*******************************************************************************
 * Code
 ******************************************************************************/
flexspi_device_config_t deviceconfig = {
    .flexspiRootClk = 42000000, /* 42MHZ SPI serial clock */
    .isSck2Enabled = false,
    .flashSize = FLASH_SIZE,
    .CSIntervalUnit = kFLEXSPI_CsIntervalUnit1SckCycle,
    .CSInterval = 2,
    .CSHoldTime = 0,
    .CSSetupTime = 3,
    .dataValidTime = 1,
    .columnspace = 3,
    .enableWordAddress = true,
    .AWRSeqIndex = HYPERFLASH_CMD_LUT_SEQ_IDX_WRITEDATA,
    .AWRSeqNumber = 1,
    .ARDSeqIndex = HYPERFLASH_CMD_LUT_SEQ_IDX_READDATA,
    .ARDSeqNumber = 1,
    .AHBWriteWaitUnit = kFLEXSPI_AhbWriteWaitUnit2AhbCycle,
    .AHBWriteWaitInterval = 20,
};

const uint32_t customLUT[CUSTOM_LUT_LENGTH] = {
        /* Read Data */
        [4 * HYPERFLASH_CMD_LUT_SEQ_IDX_READDATA] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0xA0, kFLEXSPI_Command_RADDR_DDR, kFLEXSPI_8PAD, 0x18),
        [4 * HYPERFLASH_CMD_LUT_SEQ_IDX_READDATA + 1] = FLEXSPI_LUT_SEQ(
            kFLEXSPI_Command_CADDR_DDR, kFLEXSPI_8PAD, 0x10, kFLEXSPI_Command_READ_DDR, kFLEXSPI_8PAD, 0x04),

        /* Write Data */
        [4 * HYPERFLASH_CMD_LUT_SEQ_IDX_WRITEDATA] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x20, kFLEXSPI_Command_RADDR_DDR, kFLEXSPI_8PAD, 0x18),
        [4 * HYPERFLASH_CMD_LUT_SEQ_IDX_WRITEDATA + 1] = FLEXSPI_LUT_SEQ(
            kFLEXSPI_Command_CADDR_DDR, kFLEXSPI_8PAD, 0x10, kFLEXSPI_Command_WRITE_DDR, kFLEXSPI_8PAD, 0x02),
        /* Read Status */
        [4 * HYPERFLASH_CMD_LUT_SEQ_IDX_READSTATUS] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x00),
        [4 * HYPERFLASH_CMD_LUT_SEQ_IDX_READSTATUS + 1] = FLEXSPI_LUT_SEQ(
            kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0xAA), // ADDR 0x555
        [4 * HYPERFLASH_CMD_LUT_SEQ_IDX_READSTATUS + 2] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x05),
        [4 * HYPERFLASH_CMD_LUT_SEQ_IDX_READSTATUS + 3] = FLEXSPI_LUT_SEQ(
            kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x70), // DATA 0x70
        [4 * HYPERFLASH_CMD_LUT_SEQ_IDX_READSTATUS + 4] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0xA0, kFLEXSPI_Command_RADDR_DDR, kFLEXSPI_8PAD, 0x18),
        [4 * HYPERFLASH_CMD_LUT_SEQ_IDX_READSTATUS + 5] = FLEXSPI_LUT_SEQ(
            kFLEXSPI_Command_CADDR_DDR, kFLEXSPI_8PAD, 0x10, kFLEXSPI_Command_DUMMY_RWDS_DDR, kFLEXSPI_8PAD, 0x0B),
        [4 * HYPERFLASH_CMD_LUT_SEQ_IDX_READSTATUS + 6] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_READ_DDR, kFLEXSPI_8PAD, 0x04, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0x0),

        /* Write Enable */
        [4 * HYPERFLASH_CMD_LUT_SEQ_IDX_WRITEENABLE] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x00),
        [4 * HYPERFLASH_CMD_LUT_SEQ_IDX_WRITEENABLE + 1] = FLEXSPI_LUT_SEQ(
            kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0xAA), // ADDR 0x555
        [4 * HYPERFLASH_CMD_LUT_SEQ_IDX_WRITEENABLE + 2] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x05),
        [4 * HYPERFLASH_CMD_LUT_SEQ_IDX_WRITEENABLE + 3] = FLEXSPI_LUT_SEQ(
            kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0xAA), // DATA 0xAA
        [4 * HYPERFLASH_CMD_LUT_SEQ_IDX_WRITEENABLE + 4] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x00),
        [4 * HYPERFLASH_CMD_LUT_SEQ_IDX_WRITEENABLE + 5] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x55),
        [4 * HYPERFLASH_CMD_LUT_SEQ_IDX_WRITEENABLE + 6] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x02),
        [4 * HYPERFLASH_CMD_LUT_SEQ_IDX_WRITEENABLE + 7] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x55),

        /* Erase Sector  */
        [4 * HYPERFLASH_CMD_LUT_SEQ_IDX_ERASESECTOR] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x00),
        [4 * HYPERFLASH_CMD_LUT_SEQ_IDX_ERASESECTOR + 1] = FLEXSPI_LUT_SEQ(
            kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0xAA), // ADDR 0x555
        [4 * HYPERFLASH_CMD_LUT_SEQ_IDX_ERASESECTOR + 2] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x05),
        [4 * HYPERFLASH_CMD_LUT_SEQ_IDX_ERASESECTOR + 3] = FLEXSPI_LUT_SEQ(
            kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x80), // DATA 0x80
        [4 * HYPERFLASH_CMD_LUT_SEQ_IDX_ERASESECTOR + 4] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x00),
        [4 * HYPERFLASH_CMD_LUT_SEQ_IDX_ERASESECTOR + 5] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0xAA),
        [4 * HYPERFLASH_CMD_LUT_SEQ_IDX_ERASESECTOR + 6] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x05),
        [4 * HYPERFLASH_CMD_LUT_SEQ_IDX_ERASESECTOR + 7] = FLEXSPI_LUT_SEQ(
            kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0xAA), // ADDR 0x555
        [4 * HYPERFLASH_CMD_LUT_SEQ_IDX_ERASESECTOR + 8] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x00),
        [4 * HYPERFLASH_CMD_LUT_SEQ_IDX_ERASESECTOR + 9] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x55),
        [4 * HYPERFLASH_CMD_LUT_SEQ_IDX_ERASESECTOR + 10] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x02),
        [4 * HYPERFLASH_CMD_LUT_SEQ_IDX_ERASESECTOR + 11] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x55),
        [4 * HYPERFLASH_CMD_LUT_SEQ_IDX_ERASESECTOR + 12] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x00, kFLEXSPI_Command_RADDR_DDR, kFLEXSPI_8PAD, 0x18),
        [4 * HYPERFLASH_CMD_LUT_SEQ_IDX_ERASESECTOR + 13] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_CADDR_DDR, kFLEXSPI_8PAD, 0x10, kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x00),
        [4 * HYPERFLASH_CMD_LUT_SEQ_IDX_ERASESECTOR + 14] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x30, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0x00),

        /* program page */
        [4 * HYPERFLASH_CMD_LUT_SEQ_IDX_PAGEPROGRAM] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x00),
        [4 * HYPERFLASH_CMD_LUT_SEQ_IDX_PAGEPROGRAM + 1] = FLEXSPI_LUT_SEQ(
            kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0xAA), // ADDR 0x555
        [4 * HYPERFLASH_CMD_LUT_SEQ_IDX_PAGEPROGRAM + 2] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x05),
        [4 * HYPERFLASH_CMD_LUT_SEQ_IDX_PAGEPROGRAM + 3] = FLEXSPI_LUT_SEQ(
            kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x00, kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0xA0), // DATA 0xA0
        [4 * HYPERFLASH_CMD_LUT_SEQ_IDX_PAGEPROGRAM + 4] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DDR, kFLEXSPI_8PAD, 0x00, kFLEXSPI_Command_RADDR_DDR, kFLEXSPI_8PAD, 0x18),
        [4 * HYPERFLASH_CMD_LUT_SEQ_IDX_PAGEPROGRAM + 5] = FLEXSPI_LUT_SEQ(
            kFLEXSPI_Command_CADDR_DDR, kFLEXSPI_8PAD, 0x10, kFLEXSPI_Command_WRITE_DDR, kFLEXSPI_8PAD, 0x80),
};



static shell_status_t lfs_format_handler(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    int res;

    if (lfs_mounted)
    {
        SHELL_Printf("LFS is mounted, please unmount it first.\r\n");
        return kStatus_SHELL_Success;
    }

    if (argc != 2 || strcmp(argv[1], "yes"))
    {
        SHELL_Printf("Are you sure? Please issue command \"format yes\" to proceed.\r\n");
        return kStatus_SHELL_Success;
    }

    res = lfs_format(&lfs, &cfg);
    if (res)
    {
        PRINTF("Error formatting LFS: %d\r\n", res);
    }

    return kStatus_SHELL_Success;
}

static shell_status_t lfs_mount_handler(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    int res;

    if (lfs_mounted)
    {
        SHELL_Printf("LFS already mounted\r\n");
        return kStatus_SHELL_Success;
    }

    res = lfs_mount(&lfs, &cfg);
    if (res)
    {
        PRINTF("Error mounting LFS\r\n");
        lfs_unmount(&lfs); /* deinit lfs to release resources */
    }
    else
    {
        lfs_mounted = 1;
    }

    return kStatus_SHELL_Success;
}

static shell_status_t lfs_unmount_handler(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    int res;

    if (!lfs_mounted)
    {
        SHELL_Printf("LFS not mounted\r\n");
        return kStatus_SHELL_Success;
    }

    res = lfs_unmount(&lfs);
    if (res)
    {
        PRINTF("Error unmounting LFS: %i\r\n", res);
    }

    lfs_mounted = 0;
    return kStatus_SHELL_Success;
}

static shell_status_t lfs_cd_handler(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    SHELL_Printf(
        "There is no concept of current directory in this example.\r\nPlease always specify the full path.\r\n");
    return kStatus_SHELL_Success;
}

static shell_status_t lfs_ls_handler(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    int res;
    lfs_dir_t dir;
    struct lfs_info info;

    if (!lfs_mounted)
    {
        SHELL_Printf("LFS not mounted\r\n");
        return kStatus_SHELL_Success;
    }

    res = lfs_dir_open(&lfs, &dir, argv[1]);
    if (res)
    {
        PRINTF("Error opening directory: %i\r\n", res);
        return kStatus_SHELL_Success;
    }

    /* iterate until end of directory */
    while ((res = lfs_dir_read(&lfs, &dir, &info)) != 0)
    {
        if (res < 0)
        {
            /* break the loop in case of an error */
            PRINTF("Error reading directory: %i\r\n", res);
            break;
        }

        if (info.type == LFS_TYPE_REG)
        {
            SHELL_Printf("%8d %s\r\n", info.size, info.name);
        }
        else if (info.type == LFS_TYPE_DIR)
        {
            SHELL_Printf("%     DIR %s\r\n", info.name);
        }
        else
        {
            SHELL_Printf("%???\r\n");
        }
    }

    res = lfs_dir_close(&lfs, &dir);
    if (res)
    {
        PRINTF("Error closing directory: %i\r\n", res);
        return kStatus_SHELL_Success;
    }

    return kStatus_SHELL_Success;
}

static shell_status_t lfs_rm_handler(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    int res;

    if (!lfs_mounted)
    {
        SHELL_Printf("LFS not mounted\r\n");
        return kStatus_SHELL_Success;
    }

    res = lfs_remove(&lfs, argv[1]);

    if (res)
    {
        PRINTF("Error while removing: %i\r\n", res);
    }

    return kStatus_SHELL_Success;
}

static shell_status_t lfs_mkdir_handler(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    int res;

    if (!lfs_mounted)
    {
        SHELL_Printf("LFS not mounted\r\n");
        return kStatus_SHELL_Success;
    }

    res = lfs_mkdir(&lfs, argv[1]);

    if (res)
    {
        PRINTF("Error creating directory: %i\r\n", res);
    }

    return kStatus_SHELL_Success;
}

static shell_status_t lfs_write_handler(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    int res;
    lfs_file_t file;

    if (!lfs_mounted)
    {
        SHELL_Printf("LFS not mounted\r\n");
        return kStatus_SHELL_Success;
    }

    res = lfs_file_open(&lfs, &file, argv[1], LFS_O_APPEND | LFS_O_CREAT);
    if (res)
    {
        PRINTF("Error opening file: %i\r\n", res);
        return kStatus_SHELL_Success;
    }

    res = lfs_file_write(&lfs, &file, argv[2], strlen(argv[2]));
    if (res > 0)
        res = lfs_file_write(&lfs, &file, "\r\n", 2);

    if (res < 0)
    {
        PRINTF("Error writing file: %i\r\n", res);
    }

    res = lfs_file_close(&lfs, &file);
    if (res)
    {
        PRINTF("Error closing file: %i\r\n", res);
    }

    return kStatus_SHELL_Success;
}

static shell_status_t lfs_cat_handler(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    int res;
    lfs_file_t file;
    uint8_t buf[16];

    if (!lfs_mounted)
    {
        SHELL_Printf("LFS not mounted\r\n");
        return kStatus_SHELL_Success;
    }

    res = lfs_file_open(&lfs, &file, argv[1], LFS_O_RDONLY);
    if (res)
    {
        PRINTF("Error opening file: %i\r\n", res);
        return kStatus_SHELL_Success;
    }

    do
    {
        res = lfs_file_read(&lfs, &file, buf, sizeof(buf));
        if (res < 0)
        {
            PRINTF("Error reading file: %i\r\n", res);
            break;
        }
        SHELL_Write(s_shellHandle, (char *)buf, res);
    } while (res);

    res = lfs_file_close(&lfs, &file);
    if (res)
    {
        PRINTF("Error closing file: %i\r\n", res);
    }

    return kStatus_SHELL_Success;
}

int main(void)
{
    status_t status;

    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    // Set flexspi root clock to 166MHZ.
    const clock_usb_pll_config_t g_ccmConfigUsbPll = {.loopDivider = 0U};

    CLOCK_InitUsb1Pll(&g_ccmConfigUsbPll);
    CLOCK_InitUsb1Pfd(kCLOCK_Pfd0, 26);   /* Set PLL3 PFD0 clock 332MHZ. */
    CLOCK_SetMux(kCLOCK_FlexspiMux, 0x3); /* Choose PLL3 PFD0 clock as flexspi source clock. */
    CLOCK_SetDiv(kCLOCK_FlexspiDiv, 3);   /* flexspi clock 83M, DDR mode, internal clock 42M. */

    lfs_get_default_config(&cfg);

    status = lfs_storage_init(&cfg);
    if (status != kStatus_Success)
    {
        PRINTF("LFS storage init failed: %i\r\n", status);
        return status;
    }

    /* Init SHELL */
    s_shellHandle = &s_shellHandleBuffer[0];
    SHELL_Init(s_shellHandle, g_serialHandle, "LFS>> ");

    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(format));
    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(mount));
    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(unmount));
    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(umount));
    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(cd));
    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(ls));
    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(rm));
    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(mkdir));
    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(write));
    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(cat));

#if !(defined(SHELL_NON_BLOCKING_MODE) && (SHELL_NON_BLOCKING_MODE > 0U))
    SHELL_Task(s_shellHandle);
#endif

    while (1)
    {
    }
}
