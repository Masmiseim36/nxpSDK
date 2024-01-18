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

static lfs_t                    lfs;
static const struct lfs_config *cfg_p;
static bool                     lfs_mounted   = false;
static uint8_t                  fsa_init_done = 0;

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
    FSA_Init();

    /* get pointer to const default config structure from peripherals.c,
        if config structure needs to be changed, update peripherals.c directly for your application */
    lfs_get_default_config(&cfg_p);

#if FWK_FSABSTRACTION_THREADSAFE
    /* prevent simultaneous mount/format at same time */
    (void)OSA_MutexLock((osa_mutex_handle_t)mLfsMutexId, osaWaitForever_c);
#endif
    res = lfs_format(&lfs, cfg_p);

#if FWK_FSABSTRACTION_THREADSAFE
    (void)OSA_MutexUnlock((osa_mutex_handle_t)mLfsMutexId);
#endif

    return res;
}

int FSA_FlashInit(void)
{
    return lfs_storage_init();
}

int FSA_Init(void)
{
    int res = 1; // File system already initialized

    if (fsa_init_done == 0)
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
            lfs_get_default_config(&cfg_p);

            FSA_CRITICAL_SECTION_ENTER();

            res = lfs_mount(&lfs, cfg_p);
            if (res)
            {
                /* Can not mount => format the File System */
                INFO_PRINTF("\rError mounting LFS: %d -> formatting\r\n", res);
                res = lfs_format(&lfs, cfg_p);
                if (res)
                {
                    DBG_PRINTF("\rError formatting LFS: %d\r\n", res);
                }
                else
                {
                    res = lfs_mount(&lfs, cfg_p);
                    if (res)
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
    res = fsa_init_done;

    if (fsa_init_done == 0)
    {
        if (!lfs_mounted)
        {
            DBG_PRINTF("LFS not mounted\r\n");
        }

        res = lfs_unmount(&lfs);
        if (res)
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

        res = lfs_file_opencfg(&lfs, &file, file_name, LFS_O_RDONLY, &file_cfg);

        if (res)
        {
            INFO_PRINTF("\rError opening file: %i   -> create new file\r\n", res);

            /* Create new file */
            res = lfs_file_opencfg(&lfs, &file, file_name, LFS_O_CREAT, &file_cfg);

            if (res)
            {
                DBG_PRINTF("\rError creating file: %i\r\n", res);
            }
            else
            {
                res = lfs_file_close(&lfs, &file);
                if (res)
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
                res = lfs_file_seek(&lfs, &file, offset, LFS_SEEK_SET);
            }

            if (res < 0)
            {
                DBG_PRINTF("\rError file seek: %i\r\n", res);
                size = (int)((uint32_t)res | 0xf000u);
            }
            else
            {
                size = lfs_file_read(&lfs, &file, buffer, buf_length);
                if (size < 0)
                {
                    /* display the error code */
                    DBG_PRINTF("\rError reading file: %i\r\n", size);
                }
            }

            res = lfs_file_close(&lfs, &file);
            if (res)
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

    return res;
}

int FSA_ReadBufferFromFile(const char *file_name, uint8_t *buffer, uint16_t buf_length)
{
    return FSA_ReadBufferFromFileLocation(file_name, buffer, buf_length, 0);
}

int FSA_WriteBufferToFile(const char *file_name, const uint8_t *buffer, uint16_t buf_length)
{
    int res = -255;

    if (buf_length == 0)
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

        res = lfs_file_opencfg(&lfs, &file, file_name, LFS_O_CREAT | LFS_O_WRONLY | LFS_O_TRUNC, &file_cfg);
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
            if (res)
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

    return res;
}

int FSA_DeleteFile(const char *file_name)
{
    int        res;
    lfs_file_t file;

#if FWK_FSABSTRACTION_THREADSAFE
    /* Emptying file content shall be atomic */
    (void)OSA_MutexLock((osa_mutex_handle_t)mLfsMutexId, osaWaitForever_c);
#endif

    res = lfs_remove(&lfs, file_name);
    DBG_PRINTF("\rlfs_remove res=%d\r\n", res);

    res = lfs_file_opencfg(&lfs, &file, file_name, LFS_O_CREAT, &file_cfg);
    DBG_PRINTF("\rlfs_remove res=%d\r\n", res);

    res = lfs_file_close(&lfs, &file);
    DBG_PRINTF("\rlfs_remove res=%d\r\n", res);

#if FWK_FSABSTRACTION_THREADSAFE
    (void)OSA_MutexUnlock((osa_mutex_handle_t)mLfsMutexId);
#endif

    return res;
}

int FSA_CheckFileSize(const char *file_name)
{
    int             res;
    struct lfs_info info = {0};

    res = lfs_stat(&lfs, file_name, &info);
    if (res == LFS_ERR_OK)
    {
        res = info.size;
    }

    return res;
}
