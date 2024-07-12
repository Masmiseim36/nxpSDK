/*
 * Copyright 2022-2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/************************************************************************************
 * Include
 ************************************************************************************/

#include <stdio.h>
#include "fsl_common.h"
#include "fwk_fs_abstraction.h"
#include "fwk_lfs_mflash.h"
#include "lfs.h"
#include "peripherals.h"

#if FWK_FSABSTRACTION_THREADSAFE
#include "fsl_os_abstraction.h"
#endif

#if FWK_FSABSTRACTION_PROFILING
#include "dbg_logging.h"
#include "fsl_debug_console.h"
#endif

#define DEBUG_FWK_FSABSTRACTION 0

#if defined(DEBUG_FWK_FSABSTRACTION) && (DEBUG_FWK_FSABSTRACTION == 2)
#include "fsl_debug_console.h"
#define DBG_PRINTF  PRINTF
#define INFO_PRINTF PRINTF
#elif defined(DEBUG_FWK_FSABSTRACTION) && (DEBUG_FWK_FSABSTRACTION == 1)
#include "fsl_debug_console.h"
#define DBG_PRINTF PRINTF
#define INFO_PRINTF(...)
#else
#define DBG_PRINTF(...)
#define INFO_PRINTF(...)
#endif

#define FSA_CRITICAL_SECTION_ENTER() \
    ;                                \
    uint32_t __primask = DisableGlobalIRQ();
#define FSA_CRITICAL_SECTION_EXIT() \
    ;                               \
    EnableGlobalIRQ(__primask)

/************************************************************************************
 * Private memory declarations
 ************************************************************************************/

static lfs_t                    lfs_ctx;
static const struct lfs_config *cfg_p;
static bool                     lfs_mounted   = false;
static uint8_t                  fsa_init_done = 0u;

static uint32_t                     file_buffer[LITTLEFS_CACHE_SIZE / 4];
static const struct lfs_file_config file_cfg = {
    // Must be cache_size.
    .buffer = file_buffer,

    // Optional list of custom attributes related to the file. If the file
    // is opened with read access, these attributes will be read from disk
    // during the open call. If the file is opened with write access, the
    // attributes will be written to disk every file sync or close. This
    // write occurs atomically with update to the file's contents.
    //
    // Custom attributes are uniquely identified by an 8-bit type and limited
    // to LFSA_ATTR_MAX bytes. When read, if the stored attribute is smaller
    // than the buffer, it will be padded with zeros. If the stored attribute
    // is larger, then it will be silently truncated. If the attribute is not
    // found, it will be created implicitly.
    .attrs = NULL,

    // Number of custom attributes in the list
    .attr_count = 0,
};

#if FWK_FSABSTRACTION_THREADSAFE
static OSA_MUTEX_HANDLE_DEFINE(mLfsMutexId);
#endif

#if FWK_FSABSTRACTION_PROFILING
volatile unsigned long fsa_current_time_write_flash = 0;
volatile unsigned long fsa_max_time_write_flash     = 0;
#endif

/************************************************************************************
 * Private functions
 ************************************************************************************/

/************************************************************************************
 * Public functions
 ************************************************************************************/

int FSA_Format(void)
{
    int res;

    /* FSA shall be initialized first for mutex creation */
    do
    {
        res = FSA_Init();
        if (res >= 0)
        {
            res = 0;
        }
        else
        {
            /* there was an error at mount time */
            break;
        }

        /* get pointer to const default config structure from peripherals.c,
           if config structure needs to be changed, update peripherals.c directly for your application */
        (void)lfs_get_default_config(&cfg_p);

#if FWK_FSABSTRACTION_THREADSAFE
        /* prevent simultaneous mount/format at same time */
        (void)OSA_MutexLock((osa_mutex_handle_t)mLfsMutexId, osaWaitForever_c);
#endif
        res = lfs_format(&lfs_ctx, cfg_p);

#if FWK_FSABSTRACTION_THREADSAFE
        (void)OSA_MutexUnlock((osa_mutex_handle_t)mLfsMutexId);
#endif
    } while (false);
    return res;
}

int FSA_FlashInit(void)
{
    return lfs_storage_init();
}

int FSA_Init(void)
{
    int res = 1; // File system already initialized

    if (fsa_init_done == 0u)
    {
#if FWK_FSABSTRACTION_THREADSAFE
        /* Mutex create */
        (void)OSA_MutexCreate(mLfsMutexId);
        assert(NULL != mLfsMutexId);
#endif

        /* Initialize underlying layer mflash driver for littleFS */
        res = lfs_storage_init();
        if (res != kStatus_Success)
        {
            DBG_PRINTF("LFS storage init failed: %i\r\n", res);
        }
        else
        {
            /* get pointer to const default config structure from peripherals.c,
                if config structure needs to be changed, update peripherals.c directly for your application */
            (void)lfs_get_default_config(&cfg_p);

            FSA_CRITICAL_SECTION_ENTER();

            res = lfs_mount(&lfs_ctx, cfg_p);
            if (res != 0)
            {
                /* Can not mount => format the File System */
                INFO_PRINTF("\rError mounting LFS: %d -> formatting\r\n", res);
                res = lfs_format(&lfs_ctx, cfg_p);
                if (res != 0)
                {
                    DBG_PRINTF("\rError formatting LFS: %d\r\n", res);
                }
                else
                {
                    res = lfs_mount(&lfs_ctx, cfg_p);
                    if (res != 0)
                    {
                        DBG_PRINTF("\rCan not mount after formating: %d\r\n", res);
                    }
                    else
                    {
#if defined(DEBUG_FWK_FSABSTRACTION) && (DEBUG_FWK_FSABSTRACTION > 0)
                        int *start_addr_dbg = (int *)cfg_p->context;
                        INFO_PRINTF("LFS mounted (after format): start address:%x blk_size=%d blk_count=%d\r\n",
                                    *start_addr_dbg, cfg_p->block_size, cfg_p->block_count);
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

            FSA_CRITICAL_SECTION_EXIT();
        }
    }

    /* Count number of calls to FSA_Init() so it can be Deinit only
        when counter is back to zero */
    if (lfs_mounted == true)
    {
        FSA_CRITICAL_SECTION_ENTER();
        fsa_init_done++;
        FSA_CRITICAL_SECTION_EXIT();
        INFO_PRINTF("fsabstraction_init_done %d\r\n", fsa_init_done);
    }
    else
    {
        DBG_PRINTF("LFS not mounted\r\n");
    }

    return res;
}

int FSA_DeInit(void)
{
    int res;

    FSA_CRITICAL_SECTION_ENTER();

    fsa_init_done--;
    res = (int)fsa_init_done;

    if (fsa_init_done == 0u)
    {
        if (!lfs_mounted)
        {
            DBG_PRINTF("LFS not mounted\r\n");
        }

        res = lfs_unmount(&lfs_ctx);
        if (res != 0)
        {
            INFO_PRINTF("\rError unmounting LFS: %i\r\n", res);
        }

        lfs_mounted = false;

#if FWK_FSABSTRACTION_THREADSAFE
        /* Take mutex so ensure it is free when destroying it */
        (void)OSA_MutexLock((osa_mutex_handle_t)mLfsMutexId, osaWaitForever_c);

        (void)OSA_MutexUnlock((osa_mutex_handle_t)mLfsMutexId);

        (void)OSA_MutexDestroy(mLfsMutexId);
#endif
    }

    FSA_CRITICAL_SECTION_EXIT();

    return res;
}

int FSA_ReadBufferFromFileLocation(const char *file_name, uint8_t *buffer, uint16_t buf_length, unsigned int offset)
{
    int res = -255;

    if (lfs_mounted)
    {
        lfs_file_t file;

#if FWK_FSABSTRACTION_THREADSAFE
        /* prevent Multiple file opening - file cache buffer is shared between all files */
        (void)OSA_MutexLock((osa_mutex_handle_t)mLfsMutexId, osaWaitForever_c);
#endif

        res = lfs_file_opencfg(&lfs_ctx, &file, file_name, (int)LFS_O_RDONLY, &file_cfg);

        if (res != 0)
        {
            INFO_PRINTF("\rError opening file: %i   -> create new file\r\n", res);

            /* Create new file */
            res = lfs_file_opencfg(&lfs_ctx, &file, file_name, (int)LFS_O_CREAT, &file_cfg);

            if (res != 0)
            {
                DBG_PRINTF("\rError creating file: %i\r\n", res);
            }
            else
            {
                res = lfs_file_close(&lfs_ctx, &file);
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
            int size;

            if (offset != 0U)
            {
                res = lfs_file_seek(&lfs_ctx, &file, (int)offset, (int)LFS_SEEK_SET);
            }

            if (res < 0)
            {
                DBG_PRINTF("\rError file seek: %i\r\n", res);
                size = (int)((uint32_t)res | 0xf000u);
            }
            else
            {
                size = lfs_file_read(&lfs_ctx, &file, buffer, buf_length);
                if (size < 0)
                {
                    /* display the error code */
                    DBG_PRINTF("\rError reading file: %i\r\n", size);
                }
            }

            res = lfs_file_close(&lfs_ctx, &file);
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

#if FWK_FSABSTRACTION_THREADSAFE
        (void)OSA_MutexUnlock((osa_mutex_handle_t)mLfsMutexId);
#endif
    }
    else
    {
        /* Not mounted */
        res = -255;
    }
    return res;
}

int FSA_ReadBufferFromFile(const char *file_name, uint8_t *buffer, uint16_t buf_length)
{
    return FSA_ReadBufferFromFileLocation(file_name, buffer, buf_length, 0);
}

int FSA_WriteBufferToFile(const char *file_name, const uint8_t *buffer, uint16_t buf_length)
{
    int res = -255;

    if (buf_length == 0u)
    {
        res = FSA_DeleteFile(file_name);
    }
    else if (lfs_mounted)
    {
        lfs_file_t file;

#if FWK_FSABSTRACTION_PROFILING
        /* For profiling purpose, just to avoid to be preempted by other tasks */
        FSA_CRITICAL_SECTION_ENTER();
        DEBUG_DWT_CYCLE_CNT_START();
#elif FWK_FSABSTRACTION_THREADSAFE
        /* prevent Multiple file opening - file cache buffer is shared between all files */
        (void)OSA_MutexLock((osa_mutex_handle_t)mLfsMutexId, osaWaitForever_c);
#endif

        res =
            lfs_file_opencfg(&lfs_ctx, &file, file_name,
                             (int)((uint32_t)LFS_O_CREAT | (uint32_t)LFS_O_WRONLY | (uint32_t)LFS_O_TRUNC), &file_cfg);
        if (res != 0)
        {
            DBG_PRINTF("\rError opening file: %i\r\n", res);
        }
        else
        {
            int         size;
            const void *buffer_to_copy;

            buffer_to_copy = buffer;

            size = lfs_file_write(&lfs_ctx, &file, buffer_to_copy, buf_length);
            if (size < 0)
            {
                /* size contains the error code */
                DBG_PRINTF("\rError writing file: %i\r\n", size);
            }

            res = lfs_file_close(&lfs_ctx, &file);
            if (res != 0)
            {
                DBG_PRINTF("\rError closing file: %i\r\n", res);
            }
            else
            {
                /* All successful
                   res will return the number of bytes written or potentially the error code returned by lfs_file_write
                 */
                res = size;
            }
        }
#if FWK_FSABSTRACTION_PROFILING
        FSA_CRITICAL_SECTION_EXIT();
        DEBUG_DWT_CYCLE_CNT_GET(fsa_current_time_write_flash);
        if (fsa_current_time_write_flash > fsa_max_time_write_flash)
        {
            fsa_max_time_write_flash = fsa_current_time_write_flash;
        }
        PRINTF("Current time to write in flash: %lu. Max time: %lu.\r\n", fsa_current_time_write_flash,
               fsa_max_time_write_flash);
#elif FWK_FSABSTRACTION_THREADSAFE
        (void)OSA_MutexUnlock((osa_mutex_handle_t)mLfsMutexId);
#endif
    }
    else
    {
        /* do nothing */
    }

    return res;
}

int FSA_DeleteFile(const char *file_name)
{
    int res;
    (void)res; /* res status only used in debug code */
#if FWK_FSABSTRACTION_THREADSAFE
    /* Emptying file content shall be atomic */
    (void)OSA_MutexLock((osa_mutex_handle_t)mLfsMutexId, osaWaitForever_c);
#endif
    do
    {
        res = lfs_remove(&lfs_ctx, file_name);
        DBG_PRINTF("\rlfs_remove res=%d\r\n", res);
        if (res < 0)
        {
            break;
        }
    } while (false);
#if FWK_FSABSTRACTION_THREADSAFE
    (void)OSA_MutexUnlock((osa_mutex_handle_t)mLfsMutexId);
#endif

    return res;
}

int FSA_CheckFileSize(const char *file_name)
{
    int             res;
    struct lfs_info info = {0};

    res = lfs_stat(&lfs_ctx, file_name, &info);
    if (res == (int)LFS_ERR_OK)
    {
        res = (int)info.size;
    }
    else if (res == (int)LFS_ERR_NOENT)
    {
        /* When a file does not exist, the function returns a size of zero */
        res = 0;
    }

    return res;
}
