/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fwk_filesystem.h"
#include "fwk_platform_flash.h"
#include "lfs.h"
#include "peripherals.h"

#if FWK_FILESYSTEM_LOCAL_BUF_SIZE
#include "FunctionLib.h"
#endif

#if FWK_FILESYSTEM_PROFILING
#include "dbg_logging.h"
#include "fsl_debug_console.h"
#endif

#define DEBUG_FWK_FILESYSTEM 0

#if defined(DEBUG_FWK_FILESYSTEM) && (DEBUG_FWK_FILESYSTEM == 2)
#include "fsl_debug_console.h"
#define DBG_PRINTF  PRINTF
#define INFO_PRINTF PRINTF
#elif defined(DEBUG_FWK_FILESYSTEM) && (DEBUG_FWK_FILESYSTEM == 1)
#include "fsl_debug_console.h"
#define DBG_PRINTF PRINTF
#define INFO_PRINTF(...)
#else
#define DBG_PRINTF(...)
#define INFO_PRINTF(...)
#endif

#define FS_CRITICAL_SECTION_ENTER() \
    ;                               \
    uint32_t __primask = DisableGlobalIRQ();
#define FS_CRITICAL_SECTION_EXIT() \
    ;                              \
    EnableGlobalIRQ(__primask)

/*******************************************************************************
 * Variables
 ******************************************************************************/

static lfs_t             lfs;
static struct lfs_config cfg;
static bool              lfs_mounted          = false;
static uint8_t           filesystem_init_done = 0;

#if FWK_FILESYSTEM_PROFILING
volatile unsigned long fs_current_time_write_flash = 0;
volatile unsigned long fs_max_time_write_flash     = 0;
#endif

/*******************************************************************************
 * Code
 ******************************************************************************/

int FS_Init(void)
{
    int res = 1; // File system already initialized

    if (filesystem_init_done == 0)
    {
        lfs_get_default_config(&cfg);

        res = lfs_storage_init(&cfg);
        if (res != kStatus_Success)
        {
            DBG_PRINTF("LFS storage init failed: %i\r\n", res);
        }
        else
        {
            res = lfs_mount(&lfs, &cfg);
            if (res != 0)
            {
                /* Can not mount, => format the File System */
                INFO_PRINTF("\rError mounting LFS: %d -> formatting\r\n", res);
                res = lfs_format(&lfs, &cfg);
                if (res != 0)
                {
                    DBG_PRINTF("\rError formatting LFS: %d\r\n", res);
                }
                else
                {
                    res = lfs_mount(&lfs, &cfg);
                    if (res != 0)
                    {
                        DBG_PRINTF("\rCan not mount after formating: %d\r\n", res);
                    }
                    else
                    {
#if defined(DEBUG_FWK_FILESYSTEM) && (DEBUG_FWK_FILESYSTEM > 0)
                        int *start_addr_dbg = (int *)cfg.context;
                        INFO_PRINTF("LFS mounted (after format): start address:%x blk_size=%d blk_count=%d\r\n",
                                    *start_addr_dbg, cfg.block_size, cfg.block_count);
#endif
                        lfs_mounted = true;
                    }
                }
            }
            else
            {
                DBG_PRINTF("LFS mounted\r\n");
                lfs_mounted = true;
            }
        }
    }

    /* Count number of calls to FS_Init() so it can be Deinit only
        when counter is back to zero */
    if (lfs_mounted == true)
    {
        filesystem_init_done++;
        INFO_PRINTF("filesystem_init_done %d\r\n", filesystem_init_done);
    }
    else
    {
        DBG_PRINTF("LFS not mounted\r\n");
    }

    return res;
}

int FS_DeInit(void)
{
    int res = 1; // File system is still initialized by other requester
    filesystem_init_done--;

    if (filesystem_init_done == 0)
    {
        if (!lfs_mounted)
        {
            DBG_PRINTF("LFS not mounted\r\n");
        }

        res = lfs_unmount(&lfs);
        if (res != 0)
        {
            INFO_PRINTF("\rError unmounting LFS: %i\r\n", res);
        }

        lfs_mounted = false;
    }

    return res;
}

int FS_ReadBufferFromFileLocation(const char *file_name, uint8_t *buffer, uint16_t buf_length, unsigned int offset)
{
    int res = -255;

    if (lfs_mounted)
    {
        lfs_file_t file;

        res = lfs_file_open(&lfs, &file, file_name, LFS_O_RDONLY);

        if (res != 0)
        {
            INFO_PRINTF("\rError opening file: %i   -> create new file\r\n", res);

            /* Create new file */
            res = lfs_file_open(&lfs, &file, file_name, LFS_O_CREAT);

            if (res != 0)
            {
                DBG_PRINTF("\rError creating file: %i\r\n", res);
            }
            else
            {
                res = lfs_file_close(&lfs, &file);
                if (res != 0)
                {
                    DBG_PRINTF("\rError closing file: %i\r\n", res);
                }
                else
                {
                    /* New File created */
                    res = 0;
                }
            }
        }
        else
        {
            if (offset != 0U)
            {
                res = lfs_file_seek(&lfs, &file, offset, LFS_SEEK_SET);
            }

            if (res < 0)
            {
                DBG_PRINTF("\rError file seek: %i\r\n", res);

                /* just close the file and exit */
                (void)lfs_file_close(&lfs, &file);
            }
            else
            {
                int size;

                size = lfs_file_read(&lfs, &file, buffer, buf_length);
                if (size < 0)
                {
                    /* return the error code */
                    res = size;

                    DBG_PRINTF("\rError reading file: %i\r\n", res);

                    /* just close the file and exit */
                    (void)lfs_file_close(&lfs, &file);
                }
                else
                {
                    res = lfs_file_close(&lfs, &file);
                    if (res != 0)
                    {
                        DBG_PRINTF("\rError closing file: %i\r\n", res);
                    }
                    else
                    {
                        /* All successful
                           res will return the number of bytes read */
                        res = size;
                    }
                }
            }
        }
    }

    return res;
}

int FS_ReadBufferFromFile(const char *file_name, uint8_t *buffer, uint16_t buf_length)
{
    return FS_ReadBufferFromFileLocation(file_name, buffer, buf_length, 0);
}

int FS_WriteBufferToFile(const char *file_name, const uint8_t *buffer, uint32_t buf_length)
{
    int res = -255;

    if (buf_length == 0)
    {
        res = FS_DeleteFile(file_name);
    }
    else if (lfs_mounted)
    {
        lfs_file_t file;

#if FWK_FILESYSTEM_PROFILING
        /* For profiling purpose, just to avoid to be preempted by other tasks */
        FS_CRITICAL_SECTION_ENTER();
        DEBUG_DWT_CYCLE_CNT_START();
#endif
        res = lfs_file_open(&lfs, &file, file_name, LFS_O_CREAT | LFS_O_WRONLY | LFS_O_TRUNC);
        if (res)
        {
            DBG_PRINTF("\rError opening file: %i\r\n", res);
        }
        else
        {
            int         size;
            const void *buffer_to_copy;

            buffer_to_copy = buffer;

            size = lfs_file_write(&lfs, &file, buffer_to_copy, buf_length);
            if (size < 0)
            {
                /* return the error code */
                res = size;

                DBG_PRINTF("\rError writing file: %i\r\n", res);
            }

            res = lfs_file_close(&lfs, &file);
            if (res != 0)
            {
                DBG_PRINTF("\rError closing file: %i\r\n", res);
            }
            else
            {
                /* All successful
                   res will return the number of bytes written  */
                res = size;
            }
        }
#if FWK_FILESYSTEM_PROFILING
        FS_CRITICAL_SECTION_EXIT();
        DEBUG_DWT_CYCLE_CNT_GET(fs_current_time_write_flash);
        if (fs_current_time_write_flash > fs_max_time_write_flash)
        {
            fs_max_time_write_flash = fs_current_time_write_flash;
        }
        PRINTF("Current time to write in flash: %lu. Max time: %lu.\r\n", fs_current_time_write_flash,
               fs_max_time_write_flash);
#endif
    }

    return res;
}

int FS_DeleteFile(const char *file_name)
{
    int        res;
    lfs_file_t file = {0};

    res = (int)lfs_remove(&lfs, file_name);
    DBG_PRINTF("\rlfs_remove res=%d\r\n", res);

    res = (int)lfs_file_open(&lfs, &file, file_name, LFS_O_CREAT);
    DBG_PRINTF("\rlfs_remove res=%d\r\n", res);

    res = (int)lfs_file_close(&lfs, &file);
    DBG_PRINTF("\rlfs_remove res=%d\r\n", res);

    return res;
}

int FS_CheckFileSize(const char *file_name)
{
    int             res;
    struct lfs_info info = {0};

    res = (int)lfs_stat(&lfs, file_name, &info);
    if (res == (int)LFS_ERR_OK)
    {
        res = info.size;
    }

    return res;
}

void *FS_InitGetHandle(void)
{
    void *p_lfs = NULL;

    if (filesystem_init_done)
    {
        p_lfs = (void *)&lfs;
    }
    else
    {
        if (FS_Init() == 0)
        {
            p_lfs = (void *)&lfs;
        }
    }
    return p_lfs;
}
