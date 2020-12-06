/*
 * Copyright 2019-2020 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef _SLN_FLASH_MGMT_
#define _SLN_FLASH_MGMT_

/*!
 * SLN Flash Management
 *
 * Sector Size: 256 KBytes
 * Page Size:  512 Bytes
 *
 * One Sector per File
 *
 * Sector Layout:
 *
 * Page #   | Usage
 * ---------|------------------
 *    0     | Sector Map
 *    1     | File {Header, Data}
 *    2     | File {Header, Data}
 *    ...   | ...
 *    255   | File {Header, Data}
 *
 *  New File entries are page aligned; some wasted space may occur
 *
 */

#include <stdint.h>
#include "sln_flash_config.h"

#define SLN_FLASH_MGMT_BASE_ADDR (0x1700000)
#define SLN_FLASH_MGMT_LAST_ADDR (0x1F40000)
#define SLN_FLASH_MGMT_SIZE_BYTES (SLN_FLASH_MGMT_LAST_ADDR - SLN_FLASH_MGMT_BASE_ADDR)
#define SLN_FLASH_MGMT_SECTOR_SIZE (SECTOR_SIZE)
#define SLN_FLASH_MGMT_FILE_ADDR(x) (SLN_FLASH_MGMT_BASE_ADDR + (x * SLN_FLASH_MGMT_SECTOR_SIZE))

#define SLN_FLASH_MGMT_FILE_NAME_LEN (30U)
#define SLN_FLASH_MAP_SIZE (FLASH_PAGE_SIZE)
#define SLN_FLASH_MAX_MAP_ENTRIES (FLASH_PAGE_SIZE - 1)

#define SLN_FLASH_MGMT_MAP_OLD (0x00)
#define SLN_FLASH_MGMT_MAP_CURRENT (0xAA)
#define SLN_FLASH_MGMT_MAP_FREE (0xFF)

/**
 * @brief Returns codes of the flash management functions
 */
typedef enum _sln_flash_mgmt_status
{
    SLN_FLASH_MGMT_OK         = 0x00,  /**< No errors */
    SLN_FLASH_MGMT_ENOLOCK    = -0x01, /**< Error file is locked */
    SLN_FLASH_MGMT_ERETRY     = -0x10, /**< Fail to lock the file */
    SLN_FLASH_MGMT_ENOMEM     = -0x20, /**< Fail to allocate memory */
    SLN_FLASH_MGMT_ENOMEM2    = -0x21, /**< Fail to allocate memory */
    SLN_FLASH_MGMT_ENOMEM3    = -0x22, /**< Fail to allocate memory */
    SLN_FLASH_MGMT_ENOMEM4    = -0x23, /**< Fail to allocate memory */
    SLN_FLASH_MGMT_EIO        = -0x30, /**< Fail to erase the sector */
    SLN_FLASH_MGMT_EINVAL     = -0x40, /**< Invalid input value */
    SLN_FLASH_MGMT_EINVAL2    = -0x41, /**< Invalid input value */
    SLN_FLASH_MGMT_EINVAL3    = -0x42, /**< Invalid input value */
    SLN_FLASH_MGMT_ENOENTRY   = -0x50, /**< No entry found in the flash */
    SLN_FLASH_MGMT_ENOENTRY2  = -0x51, /**< No entry found in the flash */
    SLN_FLASH_MGMT_ENOENTRY3  = -0x52, /**< No entry found in the flash */
    SLN_FLASH_MGMT_EOVERFLOW  = -0x60, /**< The flash map is full */
    SLN_FLASH_MGMT_EOVERFLOW2 = -0x61, /**< The save file doesn't fit in the current sector */
    SLN_FLASH_MGMT_EENCRYPT   = -0x70, /**< Fail to encrypt or decrypt */
    SLN_FLASH_MGMT_EENCRYPT2  = -0x71, /**< Crc mismatch */
} sln_flash_mgmt_status_t;

/* Use the flash API with or without encryption */
#define SLN_FLASH_PLAIN (false)
#define SLN_FLASH_ENCRYPTED (true)

typedef struct _sln_flash_entry
{
    char name[SLN_FLASH_MGMT_FILE_NAME_LEN + 1];
    uint32_t address;
    bool isEncrypted;
} sln_flash_entry_t;

typedef struct _sln_flash_map
{
    uint8_t map[SLN_FLASH_MAX_MAP_ENTRIES];
    uint8_t reserved;
} sln_flash_map_t;

#define SLN_FLASH_FILE_HEADER_SIZE_BITS \
    (14UL) /*! Initial number of bits used for file size in the SLN flash entry header */
#define SLN_FLASH_FILE_HEADER_MAX_SIZE (0x4000UL) /*! Initial maximum file size per flash entry */
#define SLN_FLASH_FILE_HEADER_EXT_SIZE_BITS \
    (24UL) /*! Extended number of bits used for file size in the SLN flash entry header */
#define SLN_FLASH_FILE_HEADER_EXT_MAX_SIZE (0x1000000UL) /*! Extended maximum file size per flash entry */

typedef struct _sln_file_header
{
    uint32_t valid : 1;         /*! Indicates this file is valid to read */
    uint32_t clean : 1;         /*! Indicates this file has been updated in place, if 0 CRC is invalid in NVM */
    uint32_t sizeBytes : 14;    /*! Bits 13:0 for file size */
    uint32_t isSize14bits : 1;  /*! Limits to 16 KB file size or extends to 16 MB */
    uint32_t reserved : 5;      /*! Padding */
    uint32_t extSizeBytes : 10; /*! Bits 23:14 for extended file size */
    uint32_t crc : 32;          /*! 32 bits of CRC */
} sln_file_header_t;

typedef struct
{
    void (*pre_sector_erase_cb)(void);  /*! Callback to be called before erasing a sector */
    void (*post_sector_erase_cb)(void); /*! Callback to be called after erasing a sector */
} sln_flash_mgmt_cbs_t;

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Initialize flash management; initializes private memory and file lock
 *
 * @param *flashEntries Pointer to array of flash entries, terminated with empty entry
 * @param erase Flag to indicate whether to erase all entries during initilialization
 *
 * @returns Status of initialization
 *
 */
int32_t SLN_FLASH_MGMT_Init(sln_flash_entry_t *flashEntries, uint8_t erase);

/*!
 * @brief Save to a named entry
 *
 * @param name String name of entry/file to save data into
 * @param data Pointer to data to save to file
 * @param len Length in bytes to save
 *
 * @returns Status of save
 */
int32_t SLN_FLASH_MGMT_Save(const char *name, uint8_t *data, uint32_t len);

/*!
 * @brief Update a named entry [can only clear bits as per nature of Flash Memory] [can't increase length, will fail]
 *
 * @param name String name of entry/file to update data
 * @param data Pointer to data to update to file
 * @param len Pointer for length in bytes to update, output back to user to know files size on NVM
 *
 * @returns Status of save [will fail with SLN_FLASH_MGMT_ENOENTRY2 if no previous save]
 */
int32_t SLN_FLASH_MGMT_Update(const char *name, uint8_t *data, uint32_t *len);

/*!
 * @brief Read from a named entry
 *
 * Usage:
 * @code
 *
 *      int32_t ret = SLN_FLASH_MGMT_OK;
 *      uint8_t *data = NULL;
 *      uint32_t len = 0;
 *
 *      ret = SLN_FLASH_MGMT_Read("file.dat", NULL, &len);
 *
 *      if (SLN_FLASH_MGMT_OK == ret)
 *      {
 *          // len is set to size in bytes of file data in Flash
 *          data = (uint8_t *)pvPortMalloc(len);
 *          if (NULL == data)
 *          {
 *              // Handle per application spec
 *          }
 *      }
 *      else
 *      {
 *          // Handle per application spec
 *      }
 *
 *      // Before calling len can be reduced to read less data,
 *      // but cannot be increased beyond size in file header (function will reduce len param value)
 *      ret = SLN_FLASH_MGMT_Read("file.dat", data, &len);
 *
 *      if (SLN_FLASH_MGMT_OK == ret)
 *      {
 *          // Data is read correctly
 *      }
 *      else
 *      {
 *          // Handle per application spec
 *      }
 *
 * @endcode
 *
 * @param name String name of entry/file to read from
 * @param data Pointer to data to copy data into
 * @param len Pointer for length in bytes to read
 *
 * @returns Status of read
 */
int32_t SLN_FLASH_MGMT_Read(const char *name, uint8_t *data, uint32_t *len);

/*!
 * @brief Read from a named entry and return pointer to constant data buffer in flash
 *
 * Note. This function does not support encrypted files
 *
 * @param name String name of entry/file to read from
 * @param data Pointer to constant data pointer residing in flash
 * @param len Pointer for length in bytes to read
 *
 * @returns Status of read
 */
int32_t SLN_FLASH_MGMT_ReadDataPtr(const char *name, const uint8_t **data, uint32_t *len);

/*!
 * @brief Completely erase an entry [sector erase]
 *
 * @param name String name of entry/file to erase
 *
 * @returns Status of the erase
 */
int32_t SLN_FLASH_MGMT_Erase(const char *name);

/*!
 * @brief De-initialize the file management
 *
 * @param *flashEntries Pointer to the list of entries to de-initialize
 * @param erase Flag to indicate whether on not to erase each entry/sector during de-init
 *
 * @returns Status of de-initialization
 */
int32_t SLN_FLASH_MGMT_Deinit(sln_flash_entry_t *flashEntries, uint8_t erase);

/*!
 * @brief Set the flash mgmt callbacks
 *
 * @param *cbs Pointer to a sln_flash_mgmt_cbs_t structure
 *
 * @returns SLN_FLASH_MGMT_OK on success, an error code otherwise
 */
int32_t SLN_FLASH_MGMT_SetCbs(sln_flash_mgmt_cbs_t *cbs);

#if defined(__cplusplus)
}
#endif

#endif /* _SLN_FLASH_MGMT_ */
