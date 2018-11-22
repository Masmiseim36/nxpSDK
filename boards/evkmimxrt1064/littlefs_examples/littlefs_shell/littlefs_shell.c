/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_shell.h"
#include "fsl_debug_console.h"
#include "lfs_support.h"

#include "pin_mux.h"
#include "board.h"
#include "clock_config.h"
#include "fsl_common.h"
#include "fsl_flexspi.h"
/*******************************************************************************
* Definitions
******************************************************************************/


#define SHELL_Printf PRINTF

/*******************************************************************************
* Prototypes
******************************************************************************/

/* SHELL user send data callback */
void SHELL_SendDataCallback(uint8_t *buf, uint32_t len);

/* SHELL user receive data callback */
void SHELL_ReceiveDataCallback(uint8_t *buf, uint32_t len);

/* SHELL LFS command handlers */
static int32_t lfs_format_handler(p_shell_context_t context, int32_t argc, char **argv);
static int32_t lfs_mount_handler(p_shell_context_t context, int32_t argc, char **argv);
static int32_t lfs_unmount_handler(p_shell_context_t context, int32_t argc, char **argv);
static int32_t lfs_cd_handler(p_shell_context_t context, int32_t argc, char **argv);
static int32_t lfs_ls_handler(p_shell_context_t context, int32_t argc, char **argv);
static int32_t lfs_rm_handler(p_shell_context_t context, int32_t argc, char **argv);
static int32_t lfs_mkdir_handler(p_shell_context_t context, int32_t argc, char **argv);
static int32_t lfs_write_handler(p_shell_context_t context, int32_t argc, char **argv);
static int32_t lfs_cat_handler(p_shell_context_t context, int32_t argc, char **argv);

/*******************************************************************************
 * Variables
 ******************************************************************************/

lfs_t lfs;
struct lfs_config cfg;
int lfs_mounted;

static const shell_command_context_t lfs_format_command = {"format", "\r\n\"format yes\": Formats the filesystem\r\n",
                                                           lfs_format_handler, SHELL_OPTIONAL_PARAMS};
static const shell_command_context_t lfs_mount_command = {"mount", "\r\n\"mount\": Mounts the filesystem\r\n",
                                                          lfs_mount_handler, 0};
static const shell_command_context_t lfs_unmount_command = {"unmount", "\r\n\"unmount\": Unmounts the filesystem\r\n",
                                                            lfs_unmount_handler, 0};
static const shell_command_context_t lfs_umount_command = {"umount", "", lfs_unmount_handler, 0}; // unmount alias
static const shell_command_context_t lfs_cd_command = {"cd", "", lfs_cd_handler, SHELL_OPTIONAL_PARAMS};
static const shell_command_context_t lfs_ls_command = {"ls", "\r\n\"ls <path>\": Lists directory content\r\n",
                                                       lfs_ls_handler, 1};
static const shell_command_context_t lfs_rm_command = {"rm", "\r\n\"rm <path>\": Removes file or directory\r\n",
                                                       lfs_rm_handler, 1};
static const shell_command_context_t lfs_mkdir_command = {"mkdir", "\r\n\"mkdir <path>\": Creates a new directory\r\n",
                                                          lfs_mkdir_handler, 1};
static const shell_command_context_t lfs_write_command = {
    "write", "\r\n\"write <path> <text>\": Writes/appends text to a file\r\n", lfs_write_handler, 2};
static const shell_command_context_t lfs_cat_command = {"cat", "\r\n\"cat <path>\": Prints file content\r\n",
                                                        lfs_cat_handler, 1};

/*******************************************************************************
 * Code
 ******************************************************************************/
flexspi_device_config_t deviceconfig = {
    .flexspiRootClk = 120000000,
    .flashSize = FLASH_SIZE,
    .CSIntervalUnit = kFLEXSPI_CsIntervalUnit1SckCycle,
    .CSInterval = 2,
    .CSHoldTime = 3,
    .CSSetupTime = 3,
    .dataValidTime = 0,
    .columnspace = 0,
    .enableWordAddress = 0,
    .AWRSeqIndex = 0,
    .AWRSeqNumber = 0,
    .ARDSeqIndex = NOR_CMD_LUT_SEQ_IDX_READ_FAST_QUAD,
    .ARDSeqNumber = 1,
    .AHBWriteWaitUnit = kFLEXSPI_AhbWriteWaitUnit2AhbCycle,
    .AHBWriteWaitInterval = 0,
};

const uint32_t customLUT[CUSTOM_LUT_LENGTH] = {
        /* Normal read mode -SDR */
        [4 * NOR_CMD_LUT_SEQ_IDX_READ_NORMAL] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x03, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_1PAD, 0x18),
        [4 * NOR_CMD_LUT_SEQ_IDX_READ_NORMAL + 1] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_READ_SDR, kFLEXSPI_1PAD, 0x04, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),

        /* Fast read mode - SDR */
        [4 * NOR_CMD_LUT_SEQ_IDX_READ_FAST] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x0B, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_1PAD, 0x18),
        [4 * NOR_CMD_LUT_SEQ_IDX_READ_FAST + 1] = FLEXSPI_LUT_SEQ(
            kFLEXSPI_Command_DUMMY_SDR, kFLEXSPI_1PAD, 0x08, kFLEXSPI_Command_READ_SDR, kFLEXSPI_1PAD, 0x04),

        /* Fast read quad mode - SDR */
        [4 * NOR_CMD_LUT_SEQ_IDX_READ_FAST_QUAD] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x6B, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_1PAD, 0x18),
        [4 * NOR_CMD_LUT_SEQ_IDX_READ_FAST_QUAD + 1] = FLEXSPI_LUT_SEQ(
            kFLEXSPI_Command_DUMMY_SDR, kFLEXSPI_4PAD, 0x08, kFLEXSPI_Command_READ_SDR, kFLEXSPI_4PAD, 0x04),

        /* Read extend parameters */
        [4 * NOR_CMD_LUT_SEQ_IDX_READSTATUS] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x81, kFLEXSPI_Command_READ_SDR, kFLEXSPI_1PAD, 0x04),

        /* Write Enable */
        [4 * NOR_CMD_LUT_SEQ_IDX_WRITEENABLE] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x06, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),

        /* Erase Sector  */
        [4 * NOR_CMD_LUT_SEQ_IDX_ERASESECTOR] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0xD7, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_1PAD, 0x18),

        /* Page Program - single mode */
        [4 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_SINGLE] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x02, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_1PAD, 0x18),
        [4 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_SINGLE + 1] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_WRITE_SDR, kFLEXSPI_1PAD, 0x04, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),

        /* Page Program - quad mode */
        [4 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_QUAD] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x32, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_1PAD, 0x18),
        [4 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_QUAD + 1] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_WRITE_SDR, kFLEXSPI_4PAD, 0x04, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),

        /* Read ID */
        [4 * NOR_CMD_LUT_SEQ_IDX_READID] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0xAB, kFLEXSPI_Command_DUMMY_SDR, kFLEXSPI_1PAD, 0x18),
        [4 * NOR_CMD_LUT_SEQ_IDX_READID + 1] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_READ_SDR, kFLEXSPI_1PAD, 0x04, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),

        /* Enable Quad mode */
        [4 * NOR_CMD_LUT_SEQ_IDX_WRITESTATUSREG] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x01, kFLEXSPI_Command_WRITE_SDR, kFLEXSPI_1PAD, 0x04),

        /* Enter QPI mode */
        [4 * NOR_CMD_LUT_SEQ_IDX_ENTERQPI] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x35, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),

        /* Exit QPI mode */
        [4 * NOR_CMD_LUT_SEQ_IDX_EXITQPI] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_4PAD, 0xF5, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),

        /* Read status register */
        [4 * NOR_CMD_LUT_SEQ_IDX_READSTATUSREG] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x05, kFLEXSPI_Command_READ_SDR, kFLEXSPI_1PAD, 0x04),
};



void SHELL_SendDataCallback(uint8_t *buf, uint32_t len)
{
    while (len--)
    {
        PUTCHAR(*(buf++));
    }
}

void SHELL_ReceiveDataCallback(uint8_t *buf, uint32_t len)
{
    while (len--)
    {
        *(buf++) = GETCHAR();
    }
}

static int32_t lfs_format_handler(p_shell_context_t context, int32_t argc, char **argv)
{
    int res;

    if (lfs_mounted)
    {
        SHELL_Printf("LFS is mounted, please unmount it first.\r\n");
        return 0;
    }

    if (argc != 2 || strcmp(argv[1], "yes"))
    {
        SHELL_Printf("Are you sure? Please issue command \"format yes\" to proceed.\r\n");
        return 0;
    }

    res = lfs_format(&lfs, &cfg);
    if (res)
    {
        PRINTF("Error formatting LFS: %d\r\n", res);
    }

    return 0;
}

static int32_t lfs_mount_handler(p_shell_context_t context, int32_t argc, char **argv)
{
    int res;

    if (lfs_mounted)
    {
        SHELL_Printf("LFS already mounted\r\n");
        return 0;
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

    return 0;
}

static int32_t lfs_unmount_handler(p_shell_context_t context, int32_t argc, char **argv)
{
    int res;

    if (!lfs_mounted)
    {
        SHELL_Printf("LFS not mounted\r\n");
        return 0;
    }

    res = lfs_unmount(&lfs);
    if (res)
    {
        PRINTF("Error unmounting LFS: %i\r\n", res);
    }

    lfs_mounted = 0;
    return 0;
}

static int32_t lfs_cd_handler(p_shell_context_t context, int32_t argc, char **argv)
{
    SHELL_Printf(
        "There is no concept of current directory in this example.\r\nPlease always specify the full path.\r\n");
    return 0;
}

static int32_t lfs_ls_handler(p_shell_context_t context, int32_t argc, char **argv)
{
    int res;
    lfs_dir_t dir;
    struct lfs_info info;

    if (!lfs_mounted)
    {
        SHELL_Printf("LFS not mounted\r\n");
        return 0;
    }

    res = lfs_dir_open(&lfs, &dir, argv[1]);
    if (res)
    {
        PRINTF("Error opening directory: %i\r\n", res);
        return 0;
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
        return 0;
    }

    return 0;
}

static int32_t lfs_rm_handler(p_shell_context_t context, int32_t argc, char **argv)
{
    int res;

    if (!lfs_mounted)
    {
        SHELL_Printf("LFS not mounted\r\n");
        return 0;
    }

    res = lfs_remove(&lfs, argv[1]);

    if (res)
    {
        PRINTF("Error while removing: %i\r\n", res);
    }

    return 0;
}

static int32_t lfs_mkdir_handler(p_shell_context_t context, int32_t argc, char **argv)
{
    int res;

    if (!lfs_mounted)
    {
        SHELL_Printf("LFS not mounted\r\n");
        return 0;
    }

    res = lfs_mkdir(&lfs, argv[1]);

    if (res)
    {
        PRINTF("Error creating directory: %i\r\n", res);
    }

    return 0;
}

static int32_t lfs_write_handler(p_shell_context_t context, int32_t argc, char **argv)
{
    int res;
    lfs_file_t file;

    if (!lfs_mounted)
    {
        SHELL_Printf("LFS not mounted\r\n");
        return 0;
    }

    res = lfs_file_open(&lfs, &file, argv[1], LFS_O_APPEND | LFS_O_CREAT);
    if (res)
    {
        PRINTF("Error opening file: %i\r\n", res);
        return 0;
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

    return 0;
}

static int32_t lfs_cat_handler(p_shell_context_t context, int32_t argc, char **argv)
{
    int res;
    lfs_file_t file;
    uint8_t buf[16];

    if (!lfs_mounted)
    {
        SHELL_Printf("LFS not mounted\r\n");
        return 0;
    }

    res = lfs_file_open(&lfs, &file, argv[1], LFS_O_RDONLY);
    if (res)
    {
        PRINTF("Error opening file: %i\r\n", res);
        return 0;
    }

    do
    {
        res = lfs_file_read(&lfs, &file, buf, sizeof(buf));
        if (res < 0)
        {
            PRINTF("Error reading file: %i\r\n", res);
            break;
        }
        SHELL_SendDataCallback(buf, res);
    } while (res);

    res = lfs_file_close(&lfs, &file);
    if (res)
    {
        PRINTF("Error closing file: %i\r\n", res);
    }

    return 0;
}

int main(void)
{
    status_t status;
    shell_context_struct user_context;

    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    const clock_usb_pll_config_t g_ccmConfigUsbPll = {.loopDivider = 0U};

    CLOCK_InitUsb1Pll(&g_ccmConfigUsbPll);
    CLOCK_InitUsb1Pfd(kCLOCK_Pfd0, 24);   /* Set PLL3 PFD0 clock 360MHZ. */
    CLOCK_SetMux(kCLOCK_FlexspiMux, 0x3); /* Choose PLL3 PFD0 clock as flexspi source clock. */
    CLOCK_SetDiv(kCLOCK_FlexspiDiv, 2);   /* flexspi clock 120M. */

    lfs_get_default_config(&cfg);

    status = lfs_storage_init(&cfg);
    if (status != kStatus_Success)
    {
        PRINTF("LFS storage init failed: %i\r\n", status);
        return status;
    }

    /* Init SHELL */
    SHELL_Init(&user_context, SHELL_SendDataCallback, SHELL_ReceiveDataCallback, SHELL_Printf, "LFS>> ");

    SHELL_RegisterCommand(&lfs_format_command);
    SHELL_RegisterCommand(&lfs_mount_command);
    SHELL_RegisterCommand(&lfs_unmount_command);
    SHELL_RegisterCommand(&lfs_umount_command);
    SHELL_RegisterCommand(&lfs_cd_command);
    SHELL_RegisterCommand(&lfs_ls_command);
    SHELL_RegisterCommand(&lfs_rm_command);
    SHELL_RegisterCommand(&lfs_mkdir_command);
    SHELL_RegisterCommand(&lfs_write_command);
    SHELL_RegisterCommand(&lfs_cat_command);

    SHELL_Main(&user_context);

    while (1)
    {
    }
}
