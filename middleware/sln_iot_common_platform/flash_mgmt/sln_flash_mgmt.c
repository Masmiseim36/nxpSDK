/*
 * Copyright 2018-2020 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"

#include "sln_encrypt.h"
#include "sln_flash.h"
#include "sln_flash_mgmt.h"
#if defined(FSL_SDK_ENABLE_DRIVER_CACHE_CONTROL) && FSL_SDK_ENABLE_DRIVER_CACHE_CONTROL
#include "fsl_cache.h"
#endif

#define GC_THRESHOLD (SLN_FLASH_MAX_MAP_ENTRIES / 2)

static SemaphoreHandle_t s_fileLock;

static sln_flash_entry_t *s_flashEntries = NULL;
static uint32_t s_fileCount              = 0;
static uint32_t *s_flashCrcList          = NULL;

static sln_encrypt_ctx_t s_flashMgmtEncCtx = {
    .key = {0x2c, 0x7d, 0x13, 0x18, 0x26, 0xb0, 0xd0, 0xaa, 0xab, 0xf7, 0x16, 0x88, 0x09, 0xcf, 0x4f, 0x3e},

    .iv = {0xef, 0xf0, 0xf9, 0xec, 0xfc, 0xf1, 0xf4, 0xf9, 0xf8, 0xf9, 0xfa, 0x02, 0xfc, 0xfd, 0xfe, 0xff},

    .keySize           = 16,
    .handle.channel    = kDCP_Channel1,
    .handle.keySlot    = kDCP_KeySlot1,
    .handle.swapConfig = kDCP_NoSwap,
};

static sln_flash_mgmt_cbs_t s_flashMgmtCbs = {NULL};

/*! @brief Structure to hold file meta data during operations */
typedef struct _file_meta
{
    uint32_t fileBaseAddr;  /*!< fileBaseAddr: Flash base address for file sector. */
    uint32_t fileHeadAddr;  /*!< fileHeadAddr: Flash address for current file header. */
    uint32_t fileDataAddr;  /*!< fileDataAddr: Flash address for current file data. */
    uint32_t mapIdx;        /*!< mapIdx: Index of first page in sector map. */
    uint32_t fileSize;      /*!< fileSize: Size in bytes of file including header */
    uint32_t pageCount;     /*!< pageCount: Number of pages that file occupies. */
    uint32_t crcValue;      /*!< crcValue: CRC32 MPEG2 value of data on NVM. */
    uint32_t dataPlainLen;  /*!< dataPlainLen: Plaintext length of data. */
    uint32_t dataCryptLen;  /*!< dataCryptLen: Encrypted length of data. */
    uint32_t flashTableIdx; /*!< flashTableIdx: Global index of file. */
    bool useEncryption;     /*!< useEncryption: Boolean field to indicate if file is encrypted. */
} file_meta_t;

/*! @brief Get file meta info from global file table, set initial file header address */
static int32_t get_file_info_from_name(file_meta_t *meta, const char *fileName)
{
    int32_t ret = SLN_FLASH_MGMT_OK;

    if ((NULL == meta) || (NULL == fileName))
    {
        ret = SLN_FLASH_MGMT_EINVAL;
    }
    else
    {
        uint32_t fileNameLen = safe_strlen(fileName, SLN_FLASH_MGMT_FILE_NAME_LEN);

        // Search global file table for the file name
        for (meta->flashTableIdx = 0; meta->flashTableIdx < s_fileCount; meta->flashTableIdx++)
        {
            if (0 == memcmp(s_flashEntries[meta->flashTableIdx].name, fileName, fileNameLen))
            {
                meta->fileBaseAddr  = s_flashEntries[meta->flashTableIdx].address;
                meta->useEncryption = s_flashEntries[meta->flashTableIdx].isEncrypted;
                break;
            }
        }

        if (s_fileCount == meta->flashTableIdx)
        {
            // Return error code if no entry found
            ret = SLN_FLASH_MGMT_ENOENTRY;
        }
        else
        {
            // Set file header address to first potential address (just after sector map)
            meta->fileHeadAddr = meta->fileBaseAddr + SLN_FLASH_MAP_SIZE;
        }
    }

    return ret;
}

/*! @brief Set file size meta data from given length */
static int32_t set_file_size_info(file_meta_t *meta, uint32_t len)
{
    int32_t ret = SLN_FLASH_MGMT_OK;

    if (NULL == meta)
    {
        ret = SLN_FLASH_MGMT_EINVAL;
    }
    else
    {
        uint32_t pageMod = 0;

        // Set plain text length regardless
        meta->dataPlainLen = len;

        if (meta->useEncryption)
        {
            // Encryption requires 16-byte divisible fileSize
            meta->dataCryptLen = SLN_Encrypt_Get_Crypt_Length(meta->dataPlainLen);
            meta->fileSize     = sizeof(sln_file_header_t) + meta->dataCryptLen;
        }
        else
        {
            meta->fileSize = sizeof(sln_file_header_t) + meta->dataPlainLen;
        }

        meta->pageCount = meta->fileSize / FLASH_PAGE_SIZE;

        pageMod = meta->fileSize % FLASH_PAGE_SIZE;

        // Adjust page count for integer division rounding
        if (pageMod)
        {
            meta->pageCount++;
        }
    }

    return ret;
}

/*! @brief Get sector map from NVM; increment flash header address as needed */
static int32_t get_sector_file_map(file_meta_t *meta, sln_flash_map_t *flashMap)
{
    int32_t ret = SLN_FLASH_MGMT_OK;

    if ((NULL == meta) || (NULL == flashMap))
    {
        ret = SLN_FLASH_MGMT_EINVAL;
    }
    else
    {
        // Set initial value of mapIdx to check for bad entry
        meta->mapIdx = SLN_FLASH_MAX_MAP_ENTRIES;

        // Read out flash map into RAM
        SLN_Read_Flash_At_Address(meta->fileBaseAddr, (uint8_t *)flashMap, sizeof(sln_flash_map_t));

        // Iterate through the map until we find a FREE or CURRENT marker
        for (uint32_t idx = 0; idx < SLN_FLASH_MAX_MAP_ENTRIES; idx++)
        {
            if (flashMap->map[idx] == SLN_FLASH_MGMT_MAP_CURRENT || flashMap->map[idx] == SLN_FLASH_MGMT_MAP_FREE)
            {
                meta->mapIdx = idx;
                break;
            }
        }

        if (SLN_FLASH_MAX_MAP_ENTRIES == meta->mapIdx)
        {
            // No Current or Free marker found
            ret = SLN_FLASH_MGMT_ENOENTRY3;
        }
        else
        {
            if (flashMap->map[meta->mapIdx] != SLN_FLASH_MGMT_MAP_CURRENT)
            {
                // Indicate to caller that this hasn't been written to, yet
                ret = SLN_FLASH_MGMT_ENOENTRY2;
            }
            else
            {
                // Increment file header address
                meta->fileHeadAddr += FLASH_PAGE_SIZE * meta->mapIdx;
            }
        }
    }

    return ret;
}

/*! @brief Get file size from SLN flash file entry header */
static uint32_t get_entry_file_size(const sln_file_header_t *header)
{
    uint32_t size = header->sizeBytes;

    if (!header->isSize14bits)
    {
        size |= (header->extSizeBytes << SLN_FLASH_FILE_HEADER_SIZE_BITS);
    }

    return size;
}

/*! @brief Save file size to SLN flash file entry header */
static void save_entry_file_size(sln_file_header_t *header, uint32_t size)
{
    header->sizeBytes = size & (SLN_FLASH_FILE_HEADER_MAX_SIZE - 1);

    if (size >= SLN_FLASH_FILE_HEADER_MAX_SIZE)
    {
        header->extSizeBytes = size >> SLN_FLASH_FILE_HEADER_SIZE_BITS;
        header->isSize14bits = 0;
    }
    else
    {
        header->isSize14bits = 1;
    }
}
#pragma GCC push_options
#pragma GCC optimize("O0")
/*! @brief Calculate the CRC-32 MPEG2 for file data. */
static int32_t calc_crc_32(file_meta_t *meta, uint8_t *data, uint32_t len)
{
    int32_t ret = SLN_FLASH_MGMT_OK;

    if ((NULL == data) || (NULL == meta))
    {
        ret = SLN_FLASH_MGMT_EINVAL;
    }
    else
    {
        size_t crcCount = 4;

        /* NOTE: compute CRC twice because DCP gives back different hashes
         * for some buffer sizes which are multiple of 64. */
        SLN_Crc(&s_flashMgmtEncCtx, data, len, &(meta->crcValue), &crcCount);
        ret = SLN_Crc(&s_flashMgmtEncCtx, data, len, &(meta->crcValue), &crcCount);

        if (kStatus_Success != ret)
        {
            ret = SLN_FLASH_MGMT_EENCRYPT;
        }
    }

    return ret;
}
#pragma GCC pop_options

/*! @brief Get file data from NVM into RAM */
static int32_t get_file_data(file_meta_t *meta, uint8_t *data)
{
    int32_t ret = SLN_FLASH_MGMT_OK;

    meta->fileDataAddr = meta->fileHeadAddr + sizeof(sln_file_header_t);

    if (meta->useEncryption)
    {
        meta->fileDataAddr = SLN_Flash_Get_Read_Address(meta->fileDataAddr);
#if defined(SLN_ENABLE_DRIVER_CACHE_CONTROL) && SLN_ENABLE_DRIVER_CACHE_CONTROL
        DCACHE_CleanByRange((uint32_t)data, meta->dataPlainLen);
#endif
        // Decrypt the message and store it in data
        ret = SLN_Decrypt_AES_CBC_PKCS7(&s_flashMgmtEncCtx, (uint8_t *)meta->fileDataAddr, meta->dataCryptLen, data,
                                        (size_t *)&(meta->dataPlainLen));

#if defined(SLN_ENABLE_DRIVER_CACHE_CONTROL) && SLN_ENABLE_DRIVER_CACHE_CONTROL
        DCACHE_InvalidateByRange((uint32_t)data, meta->dataPlainLen);
#endif
        if (SLN_FLASH_MGMT_OK != ret)
        {
            ret = SLN_FLASH_MGMT_EENCRYPT;
        }
    }
    else
    {
        ret = SLN_Read_Flash_At_Address(meta->fileDataAddr, data, meta->dataPlainLen);
    }

    return ret;
}

/*! @brief Set file data to save into NVM */
static int32_t set_file_data(file_meta_t *meta, uint8_t *flashFile, uint8_t *data)
{
    int32_t ret = SLN_FLASH_MGMT_OK;

    if (meta->useEncryption)
    {
#if defined(SLN_ENABLE_DRIVER_CACHE_CONTROL) && SLN_ENABLE_DRIVER_CACHE_CONTROL
        DCACHE_CleanByRange((uint32_t)(flashFile + sizeof(sln_file_header_t)), meta->dataCryptLen);
#endif
        // Encrypt the message and store it in flashFile
        ret = SLN_Encrypt_AES_CBC_PKCS7(&s_flashMgmtEncCtx, data, meta->dataPlainLen,
                                        flashFile + sizeof(sln_file_header_t), meta->dataCryptLen);

#if defined(SLN_ENABLE_DRIVER_CACHE_CONTROL) && SLN_ENABLE_DRIVER_CACHE_CONTROL
        DCACHE_InvalidateByRange((uint32_t)(flashFile + sizeof(sln_file_header_t)), meta->dataCryptLen);
#endif
        if (ret != kStatus_Success)
        {
            ret = SLN_FLASH_MGMT_EENCRYPT;
        }
    }
    else
    {
        memcpy(flashFile + sizeof(sln_file_header_t), data, meta->dataPlainLen);
    }

    return ret;
}

/*! @brief Write file data into NVM page by page */
static int32_t write_file(file_meta_t *meta, uint8_t *flashFile)
{
    int32_t ret              = SLN_FLASH_MGMT_OK;
    uint32_t flashFileOffset = 0;
    uint32_t toCopy          = 0;

    // Update fileDataAddr
    meta->fileDataAddr = meta->fileHeadAddr + sizeof(sln_file_header_t);

    do
    {
        toCopy = (meta->fileSize > FLASH_PAGE_SIZE) ? FLASH_PAGE_SIZE : meta->fileSize;

        ret = SLN_Write_Flash_Page(meta->fileHeadAddr, (uint8_t *)flashFile + flashFileOffset, toCopy);

        if (kStatus_Success != ret)
        {
            break;
        }

        meta->fileSize -= FLASH_PAGE_SIZE;
        meta->fileHeadAddr += FLASH_PAGE_SIZE;
        flashFileOffset += toCopy;
    } while (--(meta->pageCount));

    return ret;
}

/*! @brief Initialize RAM CRC mirror for all files in global table. */
static int32_t init_ram_crc_mirror(uint32_t flashEntryIdx)
{
    int32_t ret                 = SLN_FLASH_MGMT_ENOLOCK;
    file_meta_t meta            = {0};
    sln_flash_map_t *currMap    = NULL;
    uint8_t *flashFile          = NULL;
    sln_file_header_t *flashHdr = NULL;

    if (NULL != s_fileLock)
    {
        ret = SLN_FLASH_MGMT_ERETRY;

        if (pdTRUE == xSemaphoreTake(s_fileLock, portMAX_DELAY))
        {
            int32_t temp = 0;
            ret          = SLN_FLASH_MGMT_OK;

            temp = strncmp(s_flashEntries[flashEntryIdx].name, "RESERVED", strlen("RESERVED"));
            if (0 == temp)
            {
                // No need to touch this file from this application, exit with OK status
                goto exit;
            }

            meta.flashTableIdx = flashEntryIdx;

            meta.fileBaseAddr = s_flashEntries[meta.flashTableIdx].address;

            meta.fileHeadAddr = meta.fileBaseAddr + SLN_FLASH_MAP_SIZE;

            // Get current flash address from map
            currMap = (sln_flash_map_t *)pvPortMalloc(sizeof(sln_flash_map_t));

            if (NULL == currMap)
            {
                ret = SLN_FLASH_MGMT_ENOMEM;
                goto exit;
            }

            ret = get_sector_file_map(&meta, currMap);

            if (SLN_FLASH_MGMT_OK != ret)
            {
                goto exit;
            }

            // Get the file header
            flashHdr = (sln_file_header_t *)pvPortMalloc(sizeof(sln_file_header_t));

            if (NULL == flashHdr)
            {
                ret = SLN_FLASH_MGMT_ENOMEM2;
                goto exit;
            }

            SLN_Read_Flash_At_Address(meta.fileHeadAddr, (uint8_t *)flashHdr, sizeof(sln_file_header_t));

            meta.fileDataAddr = meta.fileHeadAddr + sizeof(sln_file_header_t);

            meta.fileDataAddr = SLN_Flash_Get_Read_Address(meta.fileDataAddr);

            // Calculate CRC
            ret = calc_crc_32(&meta, (uint8_t *)(meta.fileDataAddr), get_entry_file_size(flashHdr));

            if (kStatus_Success != ret)
            {
                goto exit;
            }

            s_flashCrcList[flashEntryIdx] = meta.crcValue;
        }
    }

exit:
    vPortFree(flashFile);
    flashFile = NULL;

    vPortFree(currMap);
    currMap = NULL;

    vPortFree(flashHdr);
    flashHdr = NULL;

    xSemaphoreGive(s_fileLock);

    return ret;
}

/*!
 * @brief The garbage collector function runs at each boot and checks how many
 * entries from the map are occupied.
 *
 * If the occupied entries exceed the threshold then the current file is
 * copied to RAM, the sector is erased and the file is copied back in
 * the first block.
 */
static int32_t garbage_collector(uint32_t flashEntryIdx)
{
    int32_t ret              = SLN_FLASH_MGMT_OK;
    uint32_t mapIdx          = 0;
    uint32_t fileBaseAddr    = s_flashEntries[flashEntryIdx].address;
    sln_flash_map_t *currMap = NULL;
    uint8_t *flashFile       = NULL;
    uint32_t fileHeadAddr    = 0;
    uint32_t pageCount       = 0;
    uint32_t flashFileOffset = 0;

    if (NULL == s_fileLock)
    {
        return SLN_FLASH_MGMT_ENOLOCK;
    }

    if (pdTRUE != xSemaphoreTake(s_fileLock, portMAX_DELAY))
    {
        return SLN_FLASH_MGMT_ERETRY;
    }

    if (0 == strncmp(s_flashEntries[flashEntryIdx].name, "RESERVED", strlen("RESERVED")))
    {
        // No need to touch this file from this application, exit with OK status
        goto exit;
    }

    fileHeadAddr = fileBaseAddr + SLN_FLASH_MAP_SIZE;

    // Get current flash address from map
    currMap = (sln_flash_map_t *)pvPortMalloc(sizeof(sln_flash_map_t));
    if (NULL == currMap)
    {
        ret = SLN_FLASH_MGMT_ENOMEM;
        goto exit;
    }

    SLN_Read_Flash_At_Address(fileBaseAddr, (uint8_t *)currMap, sizeof(sln_flash_map_t));
    for (mapIdx = 0; mapIdx < SLN_FLASH_MAX_MAP_ENTRIES; mapIdx++)
    {
        if (currMap->map[mapIdx] == SLN_FLASH_MGMT_MAP_CURRENT || currMap->map[mapIdx] == SLN_FLASH_MGMT_MAP_FREE)
        {
            break;
        }
    }

    if ((SLN_FLASH_MAX_MAP_ENTRIES <= mapIdx) ||
        ((SLN_FLASH_MGMT_MAP_CURRENT != currMap->map[mapIdx]) && (GC_THRESHOLD < mapIdx)))
    {
        // Something wrong as there is no current file saved but the
        // GC threshold exceeded. Erase the sector and exit.

        ret = SLN_Erase_Sector(s_flashEntries[flashEntryIdx].address);
        if (SLN_FLASH_MGMT_OK != ret)
        {
            ret = SLN_FLASH_MGMT_EIO;
        }

        goto exit;
    }

    if (mapIdx > GC_THRESHOLD)
    {
        uint32_t fileSize = 0;
        uint32_t fileAddr = fileHeadAddr + FLASH_PAGE_SIZE * mapIdx;

        // First, find the size of the current file
        sln_file_header_t currHdr;
        SLN_Read_Flash_At_Address(fileAddr, (uint8_t *)&currHdr, sizeof(sln_file_header_t));

        fileSize = get_entry_file_size(&currHdr) + sizeof(sln_file_header_t);

        flashFile = (uint8_t *)pvPortMalloc(fileSize);
        if (NULL == flashFile)
        {
            ret = SLN_FLASH_MGMT_ENOMEM;
            goto exit;
        }

        // Copy the current file from flash to RAM
        SLN_Read_Flash_At_Address(fileAddr, flashFile, fileSize);

        ret = SLN_Erase_Sector(s_flashEntries[flashEntryIdx].address);
        if (SLN_FLASH_MGMT_OK != ret)
        {
            ret = SLN_FLASH_MGMT_EIO;
            goto exit;
        }

        pageCount = fileSize / FLASH_PAGE_SIZE;
        if (fileSize % FLASH_PAGE_SIZE)
        {
            pageCount++;
        }

        // Update the map and mark the first pageCount entries as occupied
        memset(currMap->map, SLN_FLASH_MGMT_MAP_FREE, sizeof(currMap->map));
        memset(currMap->map, SLN_FLASH_MGMT_MAP_CURRENT, pageCount);
        SLN_Write_Flash_Page(fileBaseAddr, (uint8_t *)currMap, sizeof(sln_flash_map_t));

        // Save back the file to the first block
        do
        {
            uint32_t toCopy = (fileSize > FLASH_PAGE_SIZE) ? FLASH_PAGE_SIZE : fileSize;

            SLN_Write_Flash_Page(fileHeadAddr, (uint8_t *)flashFile + flashFileOffset, toCopy);
            fileSize -= FLASH_PAGE_SIZE;
            fileHeadAddr += FLASH_PAGE_SIZE;
            flashFileOffset += toCopy;
        } while (--pageCount);
    }

exit:
    vPortFree(flashFile);
    flashFile = NULL;

    vPortFree(currMap);
    currMap = NULL;

    xSemaphoreGive(s_fileLock);

    return ret;
}

int32_t SLN_FLASH_MGMT_Init(sln_flash_entry_t *flashEntries, uint8_t erase)
{
    int32_t ret          = SLN_FLASH_MGMT_OK;
    uint32_t fileSysSize = 0;
    uint32_t idx         = 0;

    SLN_Encrypt_Init_Slot(&s_flashMgmtEncCtx);

    while (safe_strlen(flashEntries[idx].name, SLN_FLASH_MGMT_FILE_NAME_LEN))
    {
        if (flashEntries[idx].address % SECTOR_SIZE)
        {
            ret = SLN_FLASH_MGMT_EINVAL;
            break;
        }

        if (SLN_FLASH_MGMT_BASE_ADDR > flashEntries[idx].address)
        {
            ret = SLN_FLASH_MGMT_EINVAL2;
            break;
        }

        fileSysSize = idx * SECTOR_SIZE;

        if (SLN_FLASH_MGMT_SIZE_BYTES <= fileSysSize)
        {
            ret = SLN_FLASH_MGMT_EINVAL3;
            break;
        }

        if (erase)
        {
            if (strncmp(flashEntries[idx].name, "RESERVED", strlen("RESERVED")))
            {
                ret = SLN_Erase_Sector(flashEntries[idx].address);

                if (SLN_FLASH_MGMT_OK != ret)
                {
                    ret = SLN_FLASH_MGMT_EIO;
                    break;
                }
            }
        }

        idx++;
    }

    // Create a lock with priority inheritance
    s_fileLock = xSemaphoreCreateMutex();

    if (NULL == s_fileLock)
    {
        ret = SLN_FLASH_MGMT_ENOLOCK;
    }

    if (SLN_FLASH_MGMT_OK == ret)
    {
        s_flashEntries = flashEntries;
        s_fileCount    = idx;
        s_flashCrcList = (uint32_t *)pvPortMalloc(s_fileCount * sizeof(uint32_t));

        if (NULL == s_flashCrcList)
        {
            ret = SLN_FLASH_MGMT_ENOMEM;
        }
        else
        {
            memset(s_flashCrcList, 0, s_fileCount * sizeof(uint32_t));
        }

        // Run the garbage collector for each file
        for (idx = 0; idx < s_fileCount; idx++)
        {
            ret = garbage_collector(idx);
        }

        // Populate RAM CRCs
        for (idx = 0; idx < s_fileCount; idx++)
        {
            ret = init_ram_crc_mirror(idx);
        }
    }

    return ret;
}

int32_t SLN_FLASH_MGMT_Save(const char *name, uint8_t *data, uint32_t len)
{
    int32_t ret = SLN_FLASH_MGMT_ENOLOCK;

    if (NULL != s_fileLock)
    {
        if (pdTRUE == xSemaphoreTake(s_fileLock, portMAX_DELAY))
        {
            file_meta_t *meta         = NULL;
            uint8_t *flashFile        = NULL;
            sln_flash_map_t *flashMap = NULL;
            sln_file_header_t *newHdr = NULL;

            ret = SLN_FLASH_MGMT_OK;

            if (NULL == data)
            {
                // Bail out, no valid pointer
                ret = SLN_FLASH_MGMT_EINVAL;
                goto exit;
            }

            meta = (file_meta_t *)pvPortMalloc(sizeof(file_meta_t));

            if (NULL == meta)
            {
                ret = SLN_FLASH_MGMT_ENOMEM;
                goto exit;
            }
            else
            {
                // Clear out data
                memset(meta, 0x00, sizeof(file_meta_t));
            }

            // Get file meta info
            ret = get_file_info_from_name(meta, name);

            if (SLN_FLASH_MGMT_ENOENTRY == ret)
            {
                goto exit;
            }

            // Set file meta info
            ret = set_file_size_info(meta, len);

            if (SLN_FLASH_MGMT_OK != ret)
            {
                goto exit;
            }

            // Get current flash address from map
            flashMap = (sln_flash_map_t *)pvPortMalloc(sizeof(sln_flash_map_t));

            if (NULL == flashMap)
            {
                ret = SLN_FLASH_MGMT_ENOMEM;
                goto exit;
            }

            // Copy sector file map to ram
            ret = get_sector_file_map(meta, flashMap);

            if ((SLN_FLASH_MGMT_OK != ret) && (SLN_FLASH_MGMT_ENOENTRY2 != ret))
            {
                goto exit;
            }

            if ((flashMap->map[meta->mapIdx] == SLN_FLASH_MGMT_MAP_CURRENT) && (SLN_FLASH_MGMT_ENOENTRY3 != ret))
            {
                // Invalidate current file from flash
                sln_file_header_t currHdr;
                SLN_Read_Flash_At_Address(meta->fileHeadAddr, (uint8_t *)&currHdr, sizeof(sln_file_header_t));

                currHdr.valid = 0;
                SLN_Write_Flash_Page(meta->fileHeadAddr, (uint8_t *)&currHdr, sizeof(sln_file_header_t));

                // Invalidate previous map entries if they exist
                while (flashMap->map[meta->mapIdx] == SLN_FLASH_MGMT_MAP_CURRENT)
                {
                    flashMap->map[meta->mapIdx] = SLN_FLASH_MGMT_MAP_OLD;

                    // Increment address to next entry
                    meta->fileHeadAddr += FLASH_PAGE_SIZE;

                    meta->mapIdx++;

                    if (meta->mapIdx >= SLN_FLASH_MAX_MAP_ENTRIES)
                    {
                        ret = SLN_FLASH_MGMT_EOVERFLOW;
                        goto exit;
                    }
                }
            }

            // Check if the saved file fits in the sector
            if (meta->mapIdx + meta->pageCount > SLN_FLASH_MAX_MAP_ENTRIES)
            {
                ret = SLN_FLASH_MGMT_EOVERFLOW2;
                goto exit;
            }

            // Mark the newly occupied pages
            memset(&(flashMap->map[meta->mapIdx]), SLN_FLASH_MGMT_MAP_CURRENT, meta->pageCount);

            SLN_Write_Flash_Page(meta->fileBaseAddr, (uint8_t *)flashMap, sizeof(sln_flash_map_t));

            // Build File
            flashFile = (uint8_t *)pvPortMalloc(meta->fileSize);

            if (NULL == flashFile)
            {
                ret = SLN_FLASH_MGMT_ENOMEM2;
                goto exit;
            }

            // Set header data for this new save
            newHdr           = (sln_file_header_t *)flashFile;
            newHdr->valid    = 1;
            newHdr->clean    = 1;
            newHdr->reserved = 0x1F;
            save_entry_file_size(newHdr, meta->useEncryption ? meta->dataCryptLen : meta->dataPlainLen);

            // Set data, will enrypt if necessary
            ret = set_file_data(meta, flashFile, data);

            if (SLN_FLASH_MGMT_OK != ret)
            {
                goto exit;
            }

            // Run crc on file data
            ret = calc_crc_32(meta, flashFile + sizeof(sln_file_header_t), get_entry_file_size(newHdr));

            if (kStatus_Success != ret)
            {
                goto exit;
            }

            // Update header with calculated CRC value
            newHdr->crc = meta->crcValue;

            // Update ram list
            if (NULL != s_flashCrcList)
            {
                s_flashCrcList[meta->flashTableIdx] = meta->crcValue;
            }

            // Write to next entry
            ret = write_file(meta, flashFile);

        exit:
            vPortFree(meta);
            meta = NULL;
            vPortFree(flashMap);
            flashMap = NULL;
            vPortFree(flashFile);
            flashFile = NULL;
            xSemaphoreGive(s_fileLock);
        }
    }

    return ret;
}

int32_t SLN_FLASH_MGMT_Update(const char *name, uint8_t *data, uint32_t *len)
{
    int32_t ret = SLN_FLASH_MGMT_ENOLOCK;

    if (NULL != s_fileLock)
    {
        if (pdTRUE == xSemaphoreTake(s_fileLock, portMAX_DELAY))
        {
            file_meta_t *meta          = NULL;
            uint8_t *flashFile         = NULL;
            sln_flash_map_t *flashMap  = NULL;
            sln_file_header_t *currHdr = NULL;

            ret = SLN_FLASH_MGMT_OK;

            if (NULL == data)
            {
                // Bail out, no valid pointer
                ret = SLN_FLASH_MGMT_EINVAL;
                goto exit;
            }

            meta = (file_meta_t *)pvPortMalloc(sizeof(file_meta_t));

            if (NULL == meta)
            {
                ret = SLN_FLASH_MGMT_ENOMEM;
                goto exit;
            }
            else
            {
                // Clear out data
                memset(meta, 0x00, sizeof(file_meta_t));
            }

            // Get file meta info
            ret = get_file_info_from_name(meta, name);

            if (SLN_FLASH_MGMT_ENOENTRY == ret)
            {
                goto exit;
            }

            // Get current flash address from map
            flashMap = (sln_flash_map_t *)pvPortMalloc(sizeof(sln_flash_map_t));

            if (NULL == flashMap)
            {
                ret = SLN_FLASH_MGMT_ENOMEM;
                goto exit;
            }

            // Copy sector file map to ram
            ret = get_sector_file_map(meta, flashMap);

            if (SLN_FLASH_MGMT_OK != ret)
            {
                goto exit;
            }

            currHdr = (sln_file_header_t *)pvPortMalloc(sizeof(sln_file_header_t));

            if (NULL == currHdr)
            {
                ret = SLN_FLASH_MGMT_ENOMEM2;
                goto exit;
            }

            // Get current file from flash
            SLN_Read_Flash_At_Address(meta->fileHeadAddr, (uint8_t *)currHdr, sizeof(sln_file_header_t));

            if (NULL != len)
            {
                // Update the reference with file size
                *len = get_entry_file_size(currHdr);
            }
            else
            {
                // Return an invalid input
                ret = SLN_FLASH_MGMT_EINVAL2;
                goto exit;
            }

            ret = set_file_size_info(meta, *len);

            if (SLN_FLASH_MGMT_OK != ret)
            {
                goto exit;
            }

            // Build File
            flashFile = (uint8_t *)pvPortMalloc(meta->fileSize);

            if (NULL == flashFile)
            {
                ret = SLN_FLASH_MGMT_ENOMEM3;
                goto exit;
            }

            // Indicate that this is updated so we won't use CRC in NVM
            currHdr->clean = 0;

            // Copy in current header data
            memcpy(flashFile, currHdr, sizeof(sln_file_header_t));

            // Set data, will enrypt if necessary
            ret = set_file_data(meta, flashFile, data);

            if (SLN_FLASH_MGMT_OK != ret)
            {
                goto exit;
            }

            // Run crc on file data
            ret = calc_crc_32(meta, flashFile + sizeof(sln_file_header_t), get_entry_file_size(currHdr));

            if (kStatus_Success != ret)
            {
                goto exit;
            }

            // Update ram list
            if (NULL != s_flashCrcList)
            {
                s_flashCrcList[meta->flashTableIdx] = meta->crcValue;
            }

            // Overwrite current entry
            ret = write_file(meta, flashFile);

        exit:
            vPortFree(flashMap);
            flashMap = NULL;
            vPortFree(currHdr);
            currHdr = NULL;
            vPortFree(flashFile);
            flashFile = NULL;
            vPortFree(meta);
            meta = NULL;
            xSemaphoreGive(s_fileLock);
        }
    }

    return ret;
}

int32_t SLN_FLASH_MGMT_ReadDataPtr(const char *name, const uint8_t **data, uint32_t *len)
{
    int32_t ret = SLN_FLASH_MGMT_ENOLOCK;

    if (NULL != s_fileLock)
    {
        if (pdTRUE == xSemaphoreTake(s_fileLock, portMAX_DELAY))
        {
            sln_file_header_t *flashHdr = NULL;
            file_meta_t *meta           = NULL;

            ret = SLN_FLASH_MGMT_OK;

            meta = (file_meta_t *)pvPortMalloc(sizeof(file_meta_t));

            if (NULL == meta)
            {
                ret = SLN_FLASH_MGMT_ENOMEM;
                goto exit;
            }
            else
            {
                // Clear out data
                memset(meta, 0x00, sizeof(file_meta_t));
            }

            // Get file meta info
            ret = get_file_info_from_name(meta, name);

            if (SLN_FLASH_MGMT_ENOENTRY == ret)
            {
                goto exit;
            }

            // Get the file header
            flashHdr = (sln_file_header_t *)pvPortMalloc(sizeof(sln_file_header_t));

            if (NULL == flashHdr)
            {
                ret = SLN_FLASH_MGMT_ENOMEM2;
                goto exit;
            }

            SLN_Read_Flash_At_Address(meta->fileHeadAddr, (uint8_t *)flashHdr, sizeof(sln_file_header_t));

            meta->fileDataAddr = SLN_Flash_Get_Read_Address(meta->fileHeadAddr + sizeof(sln_file_header_t));

            // Run crc on file data
            ret = calc_crc_32(meta, (uint8_t *)meta->fileDataAddr, get_entry_file_size(flashHdr));

            if (kStatus_Success != ret)
            {
                goto exit;
            }

            // Compare CRC
            if (flashHdr->clean)
            {
                if (flashHdr->crc != meta->crcValue)
                {
                    /* Return the data to the calling function to decide what to do in CRC failure */
                    ret = SLN_FLASH_MGMT_EENCRYPT2;
                }
            }
            else
            {
                if (NULL != s_flashCrcList)
                {
                    if (s_flashCrcList[meta->flashTableIdx] != meta->crcValue)
                    {
                        /* Return the data to the calling function to decide what to do in CRC failure */
                        ret = SLN_FLASH_MGMT_EENCRYPT2;
                    }
                }
                else
                {
                    ret = SLN_FLASH_MGMT_ENOMEM;
                    goto exit;
                }
            }

            if (len != NULL)
            {
                *len = get_entry_file_size(flashHdr);
            }
            if (data != NULL)
            {
                *data = (const uint8_t *)meta->fileDataAddr;
            }

        exit:
            vPortFree(flashHdr);
            flashHdr = NULL;
            vPortFree(meta);
            meta = NULL;
            xSemaphoreGive(s_fileLock);
        }
    }

    return ret;
}

int32_t SLN_FLASH_MGMT_Read(const char *name, uint8_t *data, uint32_t *len)
{
    int32_t ret = SLN_FLASH_MGMT_ENOLOCK;

    if (NULL != s_fileLock)
    {
        if (pdTRUE == xSemaphoreTake(s_fileLock, portMAX_DELAY))
        {
            sln_flash_map_t *flashMap   = NULL;
            sln_file_header_t *flashHdr = NULL;
            file_meta_t *meta           = NULL;
            uint8_t *msgDec             = NULL;

            ret = SLN_FLASH_MGMT_OK;

            meta = (file_meta_t *)pvPortMalloc(sizeof(file_meta_t));

            if (NULL == meta)
            {
                ret = SLN_FLASH_MGMT_ENOMEM;
                goto exit;
            }
            else
            {
                // Clear out data
                memset(meta, 0x00, sizeof(file_meta_t));
            }

            // Get file meta info
            ret = get_file_info_from_name(meta, name);

            if (SLN_FLASH_MGMT_ENOENTRY == ret)
            {
                goto exit;
            }

            // Get current flash address from map
            flashMap = (sln_flash_map_t *)pvPortMalloc(sizeof(sln_flash_map_t));

            if (NULL == flashMap)
            {
                ret = SLN_FLASH_MGMT_ENOMEM;
                goto exit;
            }

            // Copy sector file map to ram
            ret = get_sector_file_map(meta, flashMap);

            if (SLN_FLASH_MGMT_OK != ret)
            {
                goto exit;
            }

            // Get the file header
            flashHdr = (sln_file_header_t *)pvPortMalloc(sizeof(sln_file_header_t));

            if (NULL == flashHdr)
            {
                ret = SLN_FLASH_MGMT_ENOMEM2;
                goto exit;
            }

            SLN_Read_Flash_At_Address(meta->fileHeadAddr, (uint8_t *)flashHdr, sizeof(sln_file_header_t));

            // Set size info based on header information;
            // at this point dataPlainLen and dataCryptLen will be equal if using encryption
            ret = set_file_size_info(meta, get_entry_file_size(flashHdr));

            if (SLN_FLASH_MGMT_OK != ret)
            {
                goto exit;
            }

            // Allocate enough space for what is in NVM
            msgDec = (uint8_t *)pvPortMalloc(get_entry_file_size(flashHdr));

            if (NULL == msgDec)
            {
                ret = SLN_FLASH_MGMT_ENOMEM3;
                goto exit;
            }

            // Copy data into temporary buffer; decryption will recover true plain text length
            ret = get_file_data(meta, msgDec);

            if (SLN_FLASH_MGMT_OK != ret)
            {
                goto exit;
            }

            // Make sure we have a good address for CRC
            if (FlexSPI_AMBA_BASE > meta->fileDataAddr)
            {
                meta->fileDataAddr = SLN_Flash_Get_Read_Address(meta->fileDataAddr);
            }

            // Run crc on file data
            ret = calc_crc_32(meta, (uint8_t *)meta->fileDataAddr, get_entry_file_size(flashHdr));

            if (kStatus_Success != ret)
            {
                goto exit;
            }

            // Compare CRC
            if (flashHdr->clean)
            {
                if (flashHdr->crc != meta->crcValue)
                {
                    /* Return the data to the calling function to decide what to do in CRC failure */
                    ret = SLN_FLASH_MGMT_EENCRYPT2;
                }
            }
            else
            {
                if (NULL != s_flashCrcList)
                {
                    if (s_flashCrcList[meta->flashTableIdx] != meta->crcValue)
                    {
                        /* Return the data to the calling function to decide what to do in CRC failure */
                        ret = SLN_FLASH_MGMT_EENCRYPT2;
                    }
                }
                else
                {
                    ret = SLN_FLASH_MGMT_ENOMEM;
                    goto exit;
                }
            }

            // Update len reference
            if (NULL != len)
            {
                // Valid reference passed from calling function

                if (NULL == data)
                {
                    // Make sure we send back file size
                    *len = meta->dataPlainLen;

                    // Bail out assuming user just wanted size of file on NVM
                    ret = SLN_FLASH_MGMT_OK;
                    goto exit;
                }

                if (0 < *len)
                {
                    // Calling function has specified a length to copy
                    if (*len > meta->dataPlainLen)
                    {
                        // Calling function is copying too much data
                        *len = meta->dataPlainLen;
                    }
                }
                else
                {
                    // Update the reference with file size
                    *len = meta->dataPlainLen;
                }
            }
            else
            {
                // Return an invalid input
                ret = SLN_FLASH_MGMT_EINVAL;
                goto exit;
            }

            // Copy to caller's data buffer
            if (NULL != data)
            {
                memcpy(data, msgDec, *len);
            }

        exit:
            vPortFree(msgDec);
            msgDec = NULL;
            vPortFree(flashHdr);
            flashHdr = NULL;
            vPortFree(flashMap);
            flashMap = NULL;
            vPortFree(meta);
            meta = NULL;
            xSemaphoreGive(s_fileLock);
        }
    }

    return ret;
}

int32_t SLN_FLASH_MGMT_Erase(const char *name)
{
    int32_t ret           = SLN_FLASH_MGMT_OK;
    uint32_t fileNameLen  = safe_strlen(name, SLN_FLASH_MGMT_FILE_NAME_LEN);
    uint32_t fileBaseAddr = 0;

    // Get file address
    for (uint32_t idx = 0; idx < s_fileCount; idx++)
    {
        if (0 == memcmp(s_flashEntries[idx].name, name, fileNameLen))
        {
            fileBaseAddr = s_flashEntries[idx].address;
            break;
        }
    }

    // Exit if the entry is not found
    if (0 == fileBaseAddr)
    {
        ret = SLN_FLASH_MGMT_ENOENTRY;
        goto exit;
    }

    if (NULL != s_flashMgmtCbs.pre_sector_erase_cb)
    {
        configPRINTF(("Erasing file %s ... \r\n", name));
        s_flashMgmtCbs.pre_sector_erase_cb();
    }

    ret = SLN_Erase_Sector(fileBaseAddr);

    if (NULL != s_flashMgmtCbs.post_sector_erase_cb)
    {
        configPRINTF(("Erase operation for file %s finished, return code %d\r\n", name, ret));
        s_flashMgmtCbs.post_sector_erase_cb();
    }

exit:
    return ret;
}

int32_t SLN_FLASH_MGMT_Deinit(sln_flash_entry_t *flashEntries, uint8_t erase)
{
    int32_t ret          = SLN_FLASH_MGMT_OK;
    uint32_t fileSysSize = 0;
    uint32_t idx         = 0;

    while (NULL != flashEntries[idx].name)
    {
        if (flashEntries[idx].address % SECTOR_SIZE)
        {
            ret = SLN_FLASH_MGMT_EINVAL;
            break;
        }

        if (SLN_FLASH_MGMT_BASE_ADDR > flashEntries[idx].address)
        {
            ret = SLN_FLASH_MGMT_EINVAL2;
            break;
        }

        fileSysSize = idx * SECTOR_SIZE;

        if (SLN_FLASH_MGMT_SIZE_BYTES <= fileSysSize)
        {
            ret = SLN_FLASH_MGMT_EINVAL3;
            break;
        }

        if (erase)
        {
            ret = SLN_Erase_Sector(flashEntries[idx].address);

            if (SLN_FLASH_MGMT_OK != ret)
            {
                ret = SLN_FLASH_MGMT_EIO;
                break;
            }
        }
    }

    vPortFree(s_flashCrcList);
    s_flashCrcList = NULL;

    /* Create a lock with priority inheritance */
    vSemaphoreDelete(s_fileLock);
    s_fileLock = NULL;

    SLN_Encrypt_Deinit_Slot(&s_flashMgmtEncCtx);

    return ret;
}

int32_t SLN_FLASH_MGMT_SetCbs(sln_flash_mgmt_cbs_t *cbs)
{
    int32_t ret = SLN_FLASH_MGMT_OK;

    if (NULL == cbs)
    {
        ret = SLN_FLASH_MGMT_EINVAL;
    }
    else
    {
        s_flashMgmtCbs.pre_sector_erase_cb  = cbs->pre_sector_erase_cb;
        s_flashMgmtCbs.post_sector_erase_cb = cbs->post_sector_erase_cb;
    }

    return ret;
}
