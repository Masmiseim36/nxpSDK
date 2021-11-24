/*
 * Copyright 2020-2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef FWK_FLASH_H_
#define FWK_FLASH_H_

#include "hal_flash_dev.h"

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief Only one flash device is supported. Registered a flash filesystem device
 * @param dev Pointer to a flash device structure
 * @return int Return 0 if registration was successful
 */
int FWK_Flash_DeviceRegister(const flash_dev_t *dev);

/**
 * @brief Init internal structures for flash.
 * @return int Return 0 if the init process was successful
 */
sln_flash_status_t FWK_Flash_Init();

/**
 * @brief Deinit internal structures for flash.
 * @return int Return 0 if the init process was successful
 */
sln_flash_status_t FWK_Flash_Deinit();

/**
 * @brief Format the filesystem
 * @return the status of formating operation
 */
sln_flash_status_t FWK_Flash_Format();

/**
 * @brief Save the data into a file from the file system
 * @param path Path of the file in the file system
 * @param buf  Buffer which contains the data that is going to be saved
 * @param size Size of the buffer
 * @return the status of save operation
 */
sln_flash_status_t FWK_Flash_Save(const char *path, void *buf, unsigned int size);

/**
 * @brief Append the data to an existing file.
 * @param path Path of the file in the file system
 * @param buf  Buffer which contains the data that is going to be append
 * @param size Size of the buffer
 * @param overwrite Boolean parameter. If true the existing file will be truncated. Similar to SLN_flash_save
 * @return the status of append operation
 */
sln_flash_status_t FWK_Flash_Append(const char *path, void *buf, unsigned int size,  bool overwrite);

/**
 * @brief Read from a file
 * @param path Path of the file in the file system
 * @param buf  Buffer in which to store the read value
 * @param size Size that was read.
 * @return the status of read operation
 */
sln_flash_status_t FWK_Flash_Read(const char *path, void *buf, unsigned int size);

/**
 * @brief Make directory operation
 * @param path Path of the directory in the file system
 * @return the status of mkdir operation
 */
sln_flash_status_t FWK_Flash_Mkdir(const char *path);

/**
 * @brief Remove file
 * @param path Path of the file that shall be removed
 * @return the status of rm operation
 */
sln_flash_status_t FWK_Flash_Rm(const char *path);

/**
 * @brief Rename existing file
 * @param OldPath Path of the file that is renamed
 * @param NewPath New Path of the file
 * @return status of rename operation
 */
sln_flash_status_t FWK_Flash_Rename(const char *oldPath, const char *newPath);

/**
 * @brief Cleanup function. Might imply defragmentation, erased unused sectors etc.
 *
 * @param timeout Time consuming operation. Set a time constrain to be sure that is not disturbing the system.
 *               Timeout = 0 means no timeout
 * @return status of cleanup operation
 */
sln_flash_status_t FWK_Flash_Cleanup(uint32_t timeout);

#if defined(__cplusplus)
}
#endif

#endif /*FWK_FLASH_H_ */
