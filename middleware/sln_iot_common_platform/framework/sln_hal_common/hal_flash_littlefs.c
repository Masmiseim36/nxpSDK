/*
 * Copyright 2020-2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#include "hal_flash_dev.h"
#include "lfs.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "sln_flash.h"
#include "sln_flash_config.h"
#include "fica_definition.h"
#include "fsl_flexspi.h"
#include "fsl_cache.h"
#include "pin_mux.h"

#include "fwk_log.h"
#include "fwk_flash.h"
#include "fwk_lpm_manager.h"
/*******************************************************************************
 * Defines
 ******************************************************************************/

typedef struct _sln_littlefs
{
    lfs_t lfs;
    SemaphoreHandle_t lock;
    struct lfs_config cfg;
    uint8_t lfsMounted;
} sln_littlefs_t;

#define LFS_START_ADDR FICA_IMG_FILE_SYS_ADDR
#define LFS_SECTORS    (FICA_FILE_SYS_SIZE / FLASH_SECTOR_SIZE)
#define CACHE_SIZE     2 * FLASH_PAGE_SIZE

#define LOOKAHEAD_BUF_SIZE 64

static int _lfs_qspiflash_read(
    const struct lfs_config *lfsc, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size);
static int _lfs_qspiflash_prog(
    const struct lfs_config *lfsc, lfs_block_t block, lfs_off_t off, const void *buffer, lfs_size_t size);
static int _lfs_qspiflash_erase(const struct lfs_config *lfsc, lfs_block_t block);
static int _lfs_qspiflash_sync(const struct lfs_config *lfsc);

/*******************************************************************************
 * Global Variables
 ******************************************************************************/

AT_NONCACHEABLE_SECTION_ALIGN_DTC(static uint8_t s_CacheBuffer[CACHE_SIZE], 8);
AT_NONCACHEABLE_SECTION_ALIGN_DTC(static uint8_t s_ReadBuffer[CACHE_SIZE], 8);
AT_NONCACHEABLE_SECTION_ALIGN_DTC(static uint8_t s_WriteBuffer[CACHE_SIZE], 8);
// the lookahead vector has to be 64bit-aligned (8B) (see below)
AT_NONCACHEABLE_SECTION_ALIGN_DTC(static uint8_t s_LookaheadBuffer[LOOKAHEAD_BUF_SIZE], 8);

static flash_dev_t s_FlashDev_Littlefs;
static sln_littlefs_t s_LittlefsHandler;

static const struct lfs_file_config s_FileDefault = {
        .buffer     = s_CacheBuffer,
        .attrs      = NULL,
        .attr_count = 0,
};

static const struct lfs_config s_LittlefsConfigDefault = {
    // block device driver context data
    .context = NULL,

    // block device operations
    .read  = _lfs_qspiflash_read,
    .prog  = _lfs_qspiflash_prog,
    .erase = _lfs_qspiflash_erase,
    .sync  = _lfs_qspiflash_sync,

    // block device configuration
    .read_size        = FLASH_PAGE_SIZE,
    .read_buffer      = s_ReadBuffer,
    .prog_buffer      = s_WriteBuffer,
    .prog_size        = FLASH_PAGE_SIZE,
    .lookahead_size   = LOOKAHEAD_BUF_SIZE,
    .lookahead_buffer = s_LookaheadBuffer,

    .cache_size   = CACHE_SIZE,
    .block_size   = FLASH_SECTOR_SIZE,
    .block_count  = LFS_SECTORS,
    .block_cycles = 100,

};

static hal_lpm_request_t s_LpmReq = {.dev = &s_FlashDev_Littlefs, .name = "Flash Littlefs"};

static uint32_t s_ErasedBlocks[LFS_SECTORS/32] = {0x0};

/*******************************************************************************
 * Code
 ******************************************************************************/

static bool _is_blockBitSet(uint32_t *blockBitfield, lfs_block_t block)
{
    return blockBitfield[block/32] & (1U << (block % 32));
}

static void _set_blockBit(uint32_t *blockBitfield, lfs_block_t block)
{
    blockBitfield[block/32] |= (1U << (block % 32));
}

static void _clear_blockBit(uint32_t *blockBitfield, lfs_block_t block)
{
    blockBitfield[block/32] &= ~(1U << (block % 32));
}

static int _lfs_traverse_create_used_blocks(void *p, lfs_block_t block)
{
    uint32_t *usedBlocks = p;
    _set_blockBit(usedBlocks, block);
    //LOGD("Block %i is in use", block);
    return 0;
}

static bool _lfs_checkBlockEmpty(lfs_block_t block)
{
    const struct lfs_config lfsc = s_LittlefsConfigDefault;
    uint32_t src = (uint32_t)(LFS_START_ADDR + block * lfsc.block_size);

    for (int i = 0; i < FLASH_SECTOR_SIZE; i += sizeof(uint32_t))
    {
        uint32_t dst = 0;
        SLN_Read_Flash_At_Address((src + i), (uint8_t *)&dst, sizeof(uint32_t));
        if (dst != 0xFFFFFFFF)
        {
            return false;
        }
    }
    return true;
}

static int _lfs_qspiflash_read(
    const struct lfs_config *lfsc, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size)
{
    uint32_t src;
    uint32_t *dst;

    src = (uint32_t)(LFS_START_ADDR + block * lfsc->block_size + off);
    dst = (uint32_t *)buffer;

    SLN_Read_Flash_At_Address(src, (uint8_t *)dst, size);

    if (((uint32_t)src & 0x03) || ((uint32_t)dst & 0x03) || (size & 0x03))
    {
        return LFS_ERR_IO; /* unaligned access */
    }

    return LFS_ERR_OK;
}

static int _lfs_qspiflash_prog(
    const struct lfs_config *lfsc, lfs_block_t block, lfs_off_t off, const void *buffer, lfs_size_t size)
{
    status_t status;
    uint32_t prog_addr = LFS_START_ADDR + block * lfsc->block_size + off;

    for (uint32_t pos = 0; pos < size; pos += lfsc->prog_size)
    {
        status = SLN_Write_Flash_Page(prog_addr + pos, (void *)((uintptr_t)buffer + pos), lfsc->prog_size);
        if (status != kStatus_Success)
        {
            LOGE("lfs_qspiflash_prog failed");
            break;
        }
    }

    if (status == kStatus_Fail)
    {
        return LFS_ERR_CORRUPT;
    }
    else if (status != kStatus_Success)
    {
        return LFS_ERR_IO;
    }
    _clear_blockBit(s_ErasedBlocks, block);

    return LFS_ERR_OK;
}

static int _lfs_qspiflash_erase(const struct lfs_config *lfsc, lfs_block_t block)
{
    status_t status     = kStatus_Success;
    uint32_t erase_addr = LFS_START_ADDR + block * lfsc->block_size;

    if (_is_blockBitSet(s_ErasedBlocks, block))
    {
        LOGD("Block %i appears to have been erased already.", block);
        return LFS_ERR_OK;
    }

    if (!_lfs_checkBlockEmpty(block))
    {
        LOGD("Erase block %i.", block);
        status = SLN_Erase_Sector(erase_addr);
    }

    if (status == kStatus_Fail)
    {
        return LFS_ERR_CORRUPT;
    }
    else if (status != kStatus_Success)
    {
        return LFS_ERR_IO;
    }

    _set_blockBit(s_ErasedBlocks, block);

    return LFS_ERR_OK;
}

static int _lfs_qspiflash_sync(const struct lfs_config *lfsc)
{
    return LFS_ERR_OK;
}

static int _lfs_get_default_config(struct lfs_config *lfsc)
{
    *lfsc = s_LittlefsConfigDefault; /* copy pre-initialized lfs config structure */
    return 0;
}

static int _lock()
{
    if (!s_LittlefsHandler.lfsMounted)
    {
        LOGE("Littlefs not mounted");
        return -1;
    }

    if (s_LittlefsHandler.lock == NULL)
    {
        LOGE("Littlefs _lock NULL");
        return -1;
    }

    if (pdTRUE != xSemaphoreTake(s_LittlefsHandler.lock, portMAX_DELAY))
    {
        LOGE("Littlefs lock take failed");
        return -1;
    }

    FWK_LpmManager_RuntimeGet(&s_LpmReq);

    return 0;
}

static int _unlock()
{
    if (s_LittlefsHandler.lock == NULL)
    {
        LOGE("Littlefs _unlock NULL");
        return -1;
    }
    if (pdTRUE != xSemaphoreGive(s_LittlefsHandler.lock))
    {
        LOGE("Littlefs lock give failed");
        return -1;
    }

    FWK_LpmManager_RuntimePut(&s_LpmReq);
    return 0;
}

static sln_flash_status_t _lfs_init()
{
    int res = kStatus_HAL_FlashSuccess;
    if (s_LittlefsHandler.lfsMounted)
    {
        return kStatus_HAL_FlashSuccess;
    }
    s_LittlefsHandler.lock = xSemaphoreCreateMutex();
    if (s_LittlefsHandler.lock == NULL)
    {
        LOGE("Littlefs create lock failed");
        return kStatus_HAL_FlashFail;
    }

    _lfs_get_default_config(&s_LittlefsHandler.cfg);
#if DEBUG
    BOARD_InitFlashResources();
#endif
    SLN_Flash_Init();
    if (res)
    {
        LOGE("Littlefs storage init failed: %i", res);
        return kStatus_HAL_FlashFail;
    }

    res = lfs_mount(&s_LittlefsHandler.lfs, &s_LittlefsHandler.cfg);
    if (res == 0)
    {
        s_LittlefsHandler.lfsMounted = 1;
        LOGD("Littlefs mount success");
    }
    else if (res == LFS_ERR_CORRUPT)
    {
        LOGE("Littlefs corrupt");
        lfs_format(&s_LittlefsHandler.lfs, &s_LittlefsHandler.cfg);
        LOGD("Littlefs attempting to mount after reformatting...");
        res = lfs_mount(&s_LittlefsHandler.lfs, &s_LittlefsHandler.cfg);
        if (res == 0)
        {
            s_LittlefsHandler.lfsMounted = 1;
            LOGD("Littlefs mount success");
        }
        else
        {
            LOGE("Littlefs mount failed again");
            return kStatus_HAL_FlashFail;
        }
    }
    else
    {
        LOGE("Littlefs error while mounting");
    }

    return res;
}

static sln_flash_status_t _lfs_cleanupHandler(const flash_dev_t *dev,
                                                             unsigned int timeout_ms)
{
    sln_flash_status_t status              = kStatus_HAL_FlashSuccess;
    uint32_t usedBlocks[LFS_SECTORS/32]    = {0};
    uint32_t emptyBlocks                   = 0;
    uint32_t startTime                     = 0;
    uint32_t currentTime                   = 0;

    if (_lock())
    {
        LOGE("Littlefs _lock failed");
        return kStatus_HAL_FlashFail;
    }

    /* create used block list */
    lfs_fs_traverse(&s_LittlefsHandler.lfs, _lfs_traverse_create_used_blocks,
                                 &usedBlocks);

    startTime = sln_current_time_us();

    /* find next block starting from free.i */
    for (int i = 0; i < LFS_SECTORS; i++)
    {
        currentTime = sln_current_time_us();
        /* Check timeout */
        if ((timeout_ms) && (currentTime >= (startTime + timeout_ms * 1000)))
        {
            break;
        }

        lfs_block_t block = (s_LittlefsHandler.lfs.free.i + i) % LFS_SECTORS;

        /* take next unused marked block */
        if (!_is_blockBitSet(usedBlocks, block))
        {
            /* If the block is marked as free but not yet erased, try to erase it */
            LOGD("Block %i is unused, try to erase it", block);
            _lfs_qspiflash_erase(&s_LittlefsConfigDefault, block);
            emptyBlocks += 1;
        }
    }

    LOGI("%i empty_blocks starting from %i available in %ims",
                 emptyBlocks, s_LittlefsHandler.lfs.free.i, (sln_current_time_us() - startTime)/1000);

    _unlock();
    return status;
}

static sln_flash_status_t _lfs_formatHandler(const flash_dev_t *dev)
{
    if (_lock())
    {
        LOGE("Littlefs _lock failed");
        return kStatus_HAL_FlashFail;
    }
    lfs_format(&s_LittlefsHandler.lfs, &s_LittlefsHandler.cfg);
    _unlock();
    return kStatus_HAL_FlashSuccess;
}

static sln_flash_status_t _lfs_rmHandler(const flash_dev_t *dev, const char *path)
{
    int res;

    if (_lock())
    {
        LOGE("Littlefs _lock failed");
        return kStatus_HAL_FlashFail;
    }

    res = lfs_remove(&s_LittlefsHandler.lfs, path);
    if (res)
    {
        LOGE("Littlefs while removing: %i", res);
        _unlock();
        if (res == LFS_ERR_NOENT)
        {
            return kStatus_HAL_FlashFileNotExist;
        }

        return kStatus_HAL_FlashFail;
    }
    _unlock();
    return kStatus_HAL_FlashSuccess;
}

static sln_flash_status_t _lfs_mkdirHandler(const flash_dev_t *dev, const char *path)
{
    int res;

    if (_lock())
    {
        LOGE("Littlefs _lock failed");
        return kStatus_HAL_FlashFail;
    }

    res = lfs_mkdir(&s_LittlefsHandler.lfs, path);

    if (res == LFS_ERR_EXIST)
    {
        LOGD("Littlefs directory exists: %i", res);
        _unlock();
        return kStatus_HAL_FlashDirExist;
    }
    else if (res)
    {
        LOGE("Littlefs creating directory: %i", res);
        _unlock();
        return kStatus_HAL_FlashFail;
    }
    _unlock();
    return kStatus_HAL_FlashSuccess;
}

static sln_flash_status_t _lfs_writeHandler(const flash_dev_t *dev, const char *path, void *buf, unsigned int size)
{
    int res;
    lfs_file_t file;

    if (_lock())
    {
        LOGE("Littlefs _lock failed");
        return kStatus_HAL_FlashFail;
    }

    res = lfs_file_opencfg(&s_LittlefsHandler.lfs, &file, path, LFS_O_CREAT, &s_FileDefault);
    if (res)
    {
        LOGE("Littlefs opening file: %i", res);
        _unlock();
        return kStatus_HAL_FlashFail;
    }

    res = lfs_file_write(&s_LittlefsHandler.lfs, &file, buf, size);
    if (res < 0)
    {
        LOGE("Littlefs writing file: %i", res);
        _unlock();
        return kStatus_HAL_FlashFail;
    }

    res = lfs_file_close(&s_LittlefsHandler.lfs, &file);
    if (res)
    {
        LOGE("Littlefs closing file: %i", res);
        _unlock();
        return kStatus_HAL_FlashFail;
    }

    _unlock();
    return kStatus_HAL_FlashSuccess;
}

static sln_flash_status_t _lfs_appendHandler(const flash_dev_t *dev,
                                                            const char *path,
                                                            void *buf,
                                                            unsigned int size,
                                                            bool overwrite)
{
    int res;
    lfs_file_t file;

    if (_lock())
    {
        LOGE("Littlefs _lock failed");
        return kStatus_HAL_FlashFail;
    }

    res = lfs_file_opencfg(&s_LittlefsHandler.lfs, &file, path, LFS_O_APPEND, &s_FileDefault);
    if (res)
    {
        LOGE("Littlefs opening file: %i", res);
        _unlock();
        if (res == LFS_ERR_NOENT)
        {
            return kStatus_HAL_FlashFileNotExist;
        }
        return kStatus_HAL_FlashFail;
    }

    if (overwrite == true)
    {
        res = lfs_file_truncate(&s_LittlefsHandler.lfs, &file, 0);

        if (res < 0)
        {
            LOGE("Littlefs truncate file: %i", res);
            _unlock();
            return kStatus_HAL_FlashFail;
        }
    }

    res = lfs_file_write(&s_LittlefsHandler.lfs, &file, buf, size);
    if (res < 0)
    {
        LOGE("Littlefs writing file: %i", res);
        _unlock();
        return kStatus_HAL_FlashFail;
    }

    res = lfs_file_close(&s_LittlefsHandler.lfs, &file);
    if (res)
    {
        LOGE("Littlefs closing file: %i", res);
        _unlock();
        return kStatus_HAL_FlashFail;
    }

    _unlock();
    return kStatus_HAL_FlashSuccess;
}

static sln_flash_status_t _lfs_readHandler(const flash_dev_t *dev, const char *path, void *buf, unsigned int size)
{
    int res;
    int offset = 0;
    lfs_file_t file;

    if (_lock())
    {
        LOGE("Littlefs _lock failed");
        return kStatus_HAL_FlashFail;
    }
    res = lfs_file_opencfg(&s_LittlefsHandler.lfs, &file, path, LFS_O_RDONLY, &s_FileDefault);
    if (res)
    {
        LOGE("Littlefs opening file: %i", res);
        _unlock();
        if (res == LFS_ERR_NOENT)
        {
            return kStatus_HAL_FlashFileNotExist;
        }
        return kStatus_HAL_FlashFail;
    }

    do
    {
        res = lfs_file_read(&s_LittlefsHandler.lfs, &file, (buf + offset), size);
        if (res < 0)
        {
            LOGE("Littlefs reading file: %i", res);
            _unlock();
            return kStatus_HAL_FlashFail;
        }
        else if (res == 0)
        {
            LOGD("Littlefs reading file \"%s\": Read only %d. %d bytes not found ", path, offset, size);
            break;
        }

        offset += res;
        size -= res;
    } while (size > 0);

    res = lfs_file_close(&s_LittlefsHandler.lfs, &file);
    if (res)
    {
        LOGE("Littlefs closing file: %i", res);
        _unlock();
        return kStatus_HAL_FlashFail;
    }

    _unlock();
    return kStatus_HAL_FlashSuccess;
}

static sln_flash_status_t _lfs_renameHandler(const flash_dev_t *dev, const char *OldPath, const char *NewPath)
{
    int res;

    if (_lock())
    {
        LOGE("Littlefs _lock failed");
        return kStatus_HAL_FlashFail;
    }

    res = lfs_rename(&s_LittlefsHandler.lfs, OldPath, NewPath);
    if (res)
    {
        LOGE("Littlefs renaming file: %i", res);
        _unlock();
        return kStatus_HAL_FlashFail;
    }
    _unlock();
    return kStatus_HAL_FlashSuccess;
}

const static flash_dev_operator_t s_FlashDev_LittlefsOps = {
    .init   = _lfs_init,
    .deinit = NULL,
    .format = _lfs_formatHandler,
    .append = _lfs_appendHandler,
    .save   = _lfs_writeHandler,
    .read   = _lfs_readHandler,
    .mkdir  = _lfs_mkdirHandler,
    .rm     = _lfs_rmHandler,
    .rename = _lfs_renameHandler,
    .cleanup= _lfs_cleanupHandler,
};

static flash_dev_t s_FlashDev_Littlefs = {
    .id  = 0,
    .ops = &s_FlashDev_LittlefsOps,
};

int HAL_FlashDev_Littlefs_Init()
{
    int error = 0;
    LOGD("++HAL_FlashDev_Littlefs_Init");
    _lfs_init();

    LOGD("--HAL_FlashDev_Littlefs_Init");
    error = FWK_Flash_DeviceRegister(&s_FlashDev_Littlefs);

    FWK_LpmManager_RegisterRequestHandler(&s_LpmReq);
    return error;
}
