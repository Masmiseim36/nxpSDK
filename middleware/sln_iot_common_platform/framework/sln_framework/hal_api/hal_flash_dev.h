/*
 * Copyright 2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
 * @brief hal flash device declaration. Flash devices can be used for things like maintaining a file system to store
 * persistent data. For example, device configurations like speaker volume and LED brightness can be saved to flash so
 * that upon reset they can be restored.
 */

#ifndef HAL_FLASH_DEV_H_
#define HAL_FLASH_DEV_H_

#include "fwk_sln_platform.h"
#include "fwk_common.h"

typedef struct _flash_dev flash_dev_t;

/*! @brief Error codes for flash hal device */
typedef enum _sln_flash_status
{
    kStatus_HAL_FlashSuccess      = 0,
    kStatus_HAL_FlashFail         = MAKE_FRAMEWORK_STATUS(kStatusFrameworkGroups_Flash, 1),
    kStatus_HAL_FlashDirExist     = MAKE_FRAMEWORK_STATUS(kStatusFrameworkGroups_Flash, 2),
    kStatus_HAL_FlashFileNotExist = MAKE_FRAMEWORK_STATUS(kStatusFrameworkGroups_Flash, 3),
} sln_flash_status_t;

/*! @brief Operation that needs to be implemented by a flash device */
typedef struct _flash_dev_operator
{
    sln_flash_status_t (*init)(const flash_dev_t *dev);
    sln_flash_status_t (*deinit)(const flash_dev_t *dev);
    sln_flash_status_t (*format)(const flash_dev_t *dev);
    sln_flash_status_t (*save)(const flash_dev_t *dev, const char *path, void *buf, unsigned int size);
    sln_flash_status_t (*append)(const flash_dev_t *dev, const char *path, void *buf, unsigned int size, bool overwrite);
    sln_flash_status_t (*read)(const flash_dev_t *dev, const char *path, void *buf, unsigned int size);
    sln_flash_status_t (*mkdir)(const flash_dev_t *dev, const char *path);
    sln_flash_status_t (*rm)(const flash_dev_t *dev, const char *path);
    sln_flash_status_t (*rename)(const flash_dev_t *dev, const char *OldPath, const char *NewPath);
    sln_flash_status_t (*cleanup)(const flash_dev_t *dev, unsigned int timeout_ms);
} flash_dev_operator_t;

/*! @brief Attributes of a flash device */
struct _flash_dev
{
    /* unique id */
    int id;
    /* operations */
    const flash_dev_operator_t *ops;
};

#endif /*HAL_FLASH_DEV_H_ */
